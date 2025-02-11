#ifndef CHATTER_H
#define CHATTER_H

#include <stdint.h>
#include <ncurses.h>

struct __attribute__((__packed__))  header_generic {
    uint8_t magic;
    char pad[6];
};

struct GUI {
    int W, H; // Width, height of terminal
    int CH; // Chat height
    WINDOW* chatsWindow;
    WINDOW* inputWindow;
    WINDOW* convWindow;
};

struct Chat {
    int sockfd;
    char name[65536];
};

struct Chatter {
    struct GUI* gui;
    char myname[65536];
    char activename[65536]; // Name of the active chat
};

struct GUI* initGUI();
void destroyGUI(struct GUI* gui);
void reprintUsernameWindow(struct GUI* gui);
void reprintChatWindow(struct GUI* gui);
void typeLoop(struct Chatter* chatter);

struct Chatter* initChatter();
void destroyChatter(struct Chatter* chatter);


/**
 * @brief Establish a chat with an IP address
 * 
 * @param chatter Data about the current chat session
 * @param IP IP address in human readable form
 */
void connect(struct Chatter* chatter, char* IP);

/**
 * @brief Send a message in the active chat
 * 
 * @param chatter Data about the current chat session
 * @param message 
 */
void sendMessage(struct Chatter* chatter, char* message);

/**
 * @brief Delete message in the active chat
 * 
 * @param chatter Data about the current chat session
 * @param id ID of message to delete
 */
void deleteMessage(struct Chatter* chatter, uint16_t id);

/**
 * @brief Send a file in the active chat
 * 
 * @param chatter Data about the current chat session
 * @param filename Path to file
 */
void sendFile(struct Chatter* chatter, char* filename);

/**
 * @brief Broadcast my name to all active connections
 * NOTE: Name is held in chatter->myname
 * 
 * @param chatter Data about the current chat session
 */
void broadcastMyName(struct Chatter* chatter);

/**
 * @brief Switch the active chat
 * 
 * @param chatter Data about the current chat session
 * @param name Switch chat to be with this person
 */
void switchTo(struct Chatter* chatter, char* name);

/**
 * @brief Close chat with someone
 * 
 * @param chatter Data about the current chat session
 * @param name Close connection with this person
 */
void closeChat(struct Chatter* chatter, char* name);


#endif