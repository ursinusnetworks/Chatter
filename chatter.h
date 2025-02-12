#ifndef CHATTER_H
#define CHATTER_H

#include <stdint.h>
#include <ncurses.h>
#include "linkedlist.h"
#include "hashmap.h"

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
struct GUI* initGUI();
void destroyGUI(struct GUI* gui);
void printErrorGUI(struct GUI* gui, char* error);


struct Message {
    int id;
    char* text;
};
void freeMessages(struct LinkedList* messages);

struct Chat {
    char name[65536]; // Name of the person we're talking to
    int sockfd; // Socket associated to this chat
    int outCounter; // How many messages sent out on this chat
    struct LinkedList* messagesIn;
    struct LinkedList* messagesOut;
};
struct Chat* initChat(int sockfd);
void destroyChat(struct Chat* chat);

struct Chatter {
    struct GUI* gui;
    struct LinkedList* chats;
    char myname[65536];
    int sockactive; // File descriptor for the active chat
    int serversock; // File descriptor for the socket listening for incoming connections
};
struct Chatter* initChatter();
void destroyChatter(struct Chatter* chatter);


void reprintUsernameWindow(struct GUI* gui);
void reprintChatWindow(struct GUI* gui);
void typeLoop(struct Chatter* chatter);




////////////////////////////////////////////////////////////////////
//                    OUTGOING ACTIONS                            //
////////////////////////////////////////////////////////////////////

/**
 * @brief Establish a chat with an IP address
 * 
 * @param chatter Data about the current chat session
 * @param IP IP address in human readable form
 * @param port Port on which to establish connection
 */
void connectChat(struct Chatter* chatter, char* IP, char* port);

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
 * @brief Close chat with someone
 * 
 * @param chatter Data about the current chat session
 * @param name Close connection with this person
 */
void closeChat(struct Chatter* chatter, char* name);


/**
 * @brief Switch the active chat
 * 
 * @param chatter Data about the current chat session
 * @param name Switch chat to be with this person
 */
void switchTo(struct Chatter* chatter, char* name);



////////////////////////////////////////////////////////////////////
//                    INCOMING ACTIONS                            //
////////////////////////////////////////////////////////////////////



#endif