#include "chatter.h"
#include "arraylist.h"
#include <ncurses.h>
#include <stdlib.h>

#define TYPE_SIZE 4
#define ADDR_WIDTH 10
char NULLTERM = '\0';

struct GUI* initGUI() {
    // Step 1: Setup 3 windows
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    struct GUI* gui = (struct GUI*)malloc(sizeof(struct GUI));
    getmaxyx(stdscr, gui->H, gui->W);
    curs_set(TRUE);
    gui->chatsWindow = newwin(gui->H - TYPE_SIZE, gui->W-ADDR_WIDTH, 0, 0);
    gui->inputWindow = newwin(TYPE_SIZE, gui->W, gui->H-TYPE_SIZE, 0);
    gui->convWindow  = newwin(gui->H - TYPE_SIZE, ADDR_WIDTH, 0, gui->W-ADDR_WIDTH);

    mvwprintw(gui->chatsWindow, 0, 0, "Hello!!!!!!!!"); // https://linux.die.net/man/3/mvprintw
    wrefresh(gui->chatsWindow);
    mvwprintw(gui->convWindow, 0, 0, "Usernames\nGo\nHere!!"); 
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

        // TODO: Parse input and send message to currently selected client
        ArrayListBuf_push(&buf, &NULLTERM, 1);
        char* input = buf.buff;
        parseInput(chatter, input);
        ArrayListBuf_free(&buf);
    }
}