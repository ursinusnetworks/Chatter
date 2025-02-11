#include <ncurses.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashmap.h"
#include "arraylist.h"
#include "chatter.h"

struct Chatter* initChatter() {
    struct Chatter* chatter = (struct Chatter*)malloc(sizeof(struct Chatter));
    chatter->gui = initGUI();
    strcpy(chatter->myname, "Anonymous");

    return chatter;
}

void destroyChatter(struct Chatter* chatter) {
    destroyGUI(chatter->gui);
    free(chatter);
}

///////////////////////////////////////////////////////////
//       GUI Update Methods
//       See: https://linux.die.net/man/3/mvprintw
///////////////////////////////////////////////////////////

void reprintUsernameWindow(struct GUI* gui) {
    wclear(gui->convWindow);
    // TODO: Print stuff in the username window
    wrefresh(gui->convWindow);
}

void reprintChatWindow(struct GUI* gui) {
    wclear(gui->chatsWindow);
    // TODO: Print stuff in the chat window
    wrefresh(gui->chatsWindow);
}



///////////////////////////////////////////////////////////
//             Chat Session Actions
///////////////////////////////////////////////////////////

/**
 * @brief Establish a chat with an IP address
 * 
 * @param chatter Data about the current chat session
 * @param IP IP address in human readable form
 */
void connect(struct Chatter* chatter, char* IP) {
    // TODO: Tralie
    
}

/**
 * @brief Send a message in the active chat
 * 
 * @param chatter Data about the current chat session
 * @param message 
 */
void sendMessage(struct Chatter* chatter, char* message) {
    
}

/**
 * @brief Delete message in the active chat
 * 
 * @param chatter Data about the current chat session
 * @param id ID of message to delete
 */
void deleteMessage(struct Chatter* chatter, uint16_t id) {

}

/**
 * @brief Send a file in the active chat
 * 
 * @param chatter Data about the current chat session
 * @param filename Path to file
 */
void sendFile(struct Chatter* chatter, char* filename) {

}

/**
 * @brief Broadcast my name to all active connections
 * NOTE: Name is held in chatter->myname
 * 
 * @param chatter Data about the current chat session
 */
void broadcastMyName(struct Chatter* chatter) {

}

/**
 * @brief Switch the active chat
 * 
 * @param chatter Data about the current chat session
 * @param name Switch chat to be with this person
 */
void switchTo(struct Chatter* chatter, char* name) {
    // TODO: Tralie
}

/**
 * @brief Close chat with someone
 * 
 * @param chatter Data about the current chat session
 * @param name Close connection with this person
 */
void closeChat(struct Chatter* chatter, char* name) {

}







int main(int argc, char *argv[]) {
    struct Chatter* chatter = initChatter();
    typeLoop(chatter);
    destroyChatter(chatter);
}