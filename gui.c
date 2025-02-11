// Programmer: Chris Tralie
// Purpose: To provide a simple 3 window interface where
// users can type commands and manage chats

#include "chatter.h"
#include "arraylist.h"
#include <ncurses.h>
#include <stdlib.h>

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

void typeLoop(struct Chatter* chatter) {
    struct GUI* gui = chatter->gui;
    ArrayListBuf buf;
    while (1) {
        wclear(gui->inputWindow);
        // Input loop
        ArrayListBuf_init(&buf);
        int ch = 0;
        do {
            ch = wgetch(gui->inputWindow);
            if (ch == KEY_UP) {
                // TODO: Move up in current conversations
            }
            else if (ch == KEY_DOWN) {
                // TODO: Move down in current conversations
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
        parseInput(chatter, input);
        ArrayListBuf_free(&buf);
    }
}