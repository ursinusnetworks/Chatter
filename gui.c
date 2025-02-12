// Programmer: Chris Tralie
// Purpose: To provide a simple 3 window interface where
// users can type commands and manage chats

#include "chatter.h"
#include "arraylist.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_SIZE 4
#define ADDR_WIDTH 10
char NULLTERM = '\0';

struct GUI* initGUI() {
    // Setup 3 windows
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    struct GUI* gui = (struct GUI*)malloc(sizeof(struct GUI));
    getmaxyx(stdscr, gui->H, gui->W);
    gui->CH = gui->H - TYPE_SIZE;
    curs_set(TRUE);
    gui->chatsWindow = newwin(gui->CH, gui->W-ADDR_WIDTH, 0, 0);
    gui->inputWindow = newwin(TYPE_SIZE, gui->W, gui->CH, 0);
    gui->convWindow  = newwin(gui->CH, ADDR_WIDTH, 0, gui->W-ADDR_WIDTH);

    char* s = "Hello!  Chats will go here!";
    mvwprintw(gui->chatsWindow, 0, 0, "%s", s); 
    wrefresh(gui->chatsWindow);
    mvwprintw(gui->convWindow, 0, 0, "Usernames\nGo\nHere!"); 
    wrefresh(gui->convWindow);
    return gui;
}

void destroyGUI(struct GUI* gui) {
    delwin(gui->chatsWindow);
    delwin(gui->inputWindow);
    delwin(gui->convWindow);
    endwin();
    free(gui);
}

void printErrorGUI(struct GUI* gui, char* error) {
    wclear(gui->chatsWindow);
    mvwprintw(gui->chatsWindow, 0, 0, "%s", error);
    wrefresh(gui->chatsWindow);
}

/**
 * @brief Incorporate something that the user typed into
 * the chat session
 * 
 * @param chatter Chat session object
 * @param input String that the user just inputted
 */
int parseInput(struct Chatter* chatter, char* input) {
    int finished = 0;
    struct GUI* gui = chatter->gui;
    // "connect <IP>"
    if (strncmp(input, "connect", strlen("connect")) == 0) {
        // Connect and start a conversation with a particular IP address
        char IP[40];
        char port[6];
        sscanf(input, "connect %39s %5s", IP, port);
        connectChat(chatter, IP, port);
    }
    else if (strncmp(input, "myname", strlen("myname")) == 0) {
        // Change my name
        sscanf(input, "myname %65535s", chatter->myname);
        broadcastMyName(chatter);
    }
    else if (strncmp(input, "sendfile", strlen("sendfile")) == 0) {
        // Send the following file message in the active conversation
        char filename[65536];
        sscanf(input, "sendfile %s", filename);
        sendFile(chatter, filename);
    }
    else if (strncmp(input, "send", strlen("send")) == 0) {
        // Send the following text message in the active conversation
        char* message = input + strlen("send") + 1;
        sendMessage(chatter, message);
    }
    else if (strncmp(input, "talkto", strlen("talkto")) == 0) {
        // Switch the active chat window to someone else
        char name[65536];
        sscanf(input, "talkto %65535s", name);
        switchTo(chatter, name);
    }
    else if (strncmp(input, "delete", strlen("delete")) == 0) {
        // Delete the message with this id in the active conversation
        uint16_t id;
        sscanf(input, "delete %hu", &id);
        deleteMessage(chatter, id);
    }
    else if (strncmp(input, "close", strlen("close")) == 0) {
        // Close the connection with someone
        char name[65536];
        sscanf(input, "close %65535s", name);
        closeChat(chatter, name);
    }
    else if (strncmp(input, "exit", strlen("exit")) == 0) {
        finished = 1;
    }
    else {
        char* fmt = "Unrecognized command %s;  (use connect, myname, send, sendfile, delete, close, talkto, exit)";
        char command[65536];
        sscanf(input, "%65535s", command);
        char* error = (char*)malloc(strlen(fmt) + strlen(command) + 1);
        sprintf(error, fmt, command);
        printErrorGUI(gui, error);
        free(error);
    }
    return finished;
}

void typeLoop(struct Chatter* chatter) {
    struct GUI* gui = chatter->gui;
    ArrayListBuf buf;
    int finished = 0;
    while (finished == 0) {
        wclear(gui->inputWindow);
        // Input loop
        ArrayListBuf_init(&buf);
        int ch = 0;
        do {
            ch = wgetch(gui->inputWindow);
            if (ch == KEY_UP) {
                // TODO: Scroll up in current conversation
            }
            else if (ch == KEY_DOWN) {
                // TODO: Scroll down in current conversation
            }
            else if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127) {
                // Handle deletes in the buffer
                if (buf.N > 0) {
                    buf.buff[buf.N-1] = '\0';
                    buf.N--;
                }
            }
            else if(ch != '\n') {
                // Accumulate current character
                ArrayListBuf_push(&buf, (char*)&ch, 1);
            }
            wclear(gui->inputWindow);
            for (int i = 0; i < buf.N; i++) {
                mvwprintw(gui->inputWindow, i/gui->W, i%gui->W, "%c", buf.buff[i]);
            }
        }
        while (ch != '\n');
        ArrayListBuf_push(&buf, &NULLTERM, 1);
        char* input = buf.buff;
        finished = parseInput(chatter, input);
        ArrayListBuf_free(&buf);
    }
}