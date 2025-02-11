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
 * @brief Incorporate something that the user typed into
 * the chat session
 * 
 * @param chatter Chat session object
 * @param input String that the user just inputted
 */
void parseInput(struct Chatter* chatter, char* input) {
    struct GUI* gui = chatter->gui;
    // "connect <IP>"
    if (strncmp(input, "connect", strlen("connect")) == 0) {
        // Connect and start a conversation with a particular IP address
        char IP[40];
        sscanf(input, "connect %s", IP);


    }
    else if (strncmp(input, "myname", strlen("myname")) == 0) {
        // Change my name
        sscanf(input, "myname %65535s", chatter->myname);
    }
    else if (strncmp(input, "sendfile", strlen("sendfile")) == 0) {
        // Send the following file message in the active conversation
        char filename[65536];
        sscanf(input, "sendfile %s", filename);

    }
    else if (strncmp(input, "send", strlen("send")) == 0) {
        // Send the following text message in the active conversation
        char* message = input + strlen("send") + 1;

    }
    else if (strncmp(input, "delete", strlen("delete")) == 0) {
        // Delete the message with this id in the active conversation
        uint16_t id;
        sscanf(input, "delete %hu", &id);
        

    }
    else if (strncmp(input, "close", strlen("close")) == 0) {
        // Close the connection with someone
        char name[65536];
        sscanf(input, "close %65535s", name);


    }
    else if (strncmp(input, "talkto", strlen("talkto")) == 0) {
        // Switch the active chat window to someone else
        sscanf(input, "talkto %65535s", chatter->activename);
        

    }
    else {
        wclear(gui->chatsWindow);
        char command[65536];
        sscanf(input, "%65535s", command);
        mvwprintw(gui->chatsWindow, 0, 0, "Unrecognized command %s;  (use connect, myname, send, sendfile, delete, close, talkto)", command);
        wrefresh(gui->chatsWindow);
    }
    reprintUsernameWindow(gui);
    reprintChatWindow(gui);
}



int main(int argc, char *argv[]) {
    struct Chatter* chatter = initChatter();
    typeLoop(chatter);
    destroyChatter(chatter);
}