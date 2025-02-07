#include <ncurses.h>
#include <unistd.h>
#include "linkedlist.h"
#include "hashmap.h"
#include "arraylist.h"

#define TYPE_SIZE 4
#define ADDR_WIDTH 10
char NULLTERM = '\0';


void printUsernames(WINDOW* convWindow) {
    mvwprintw(convWindow, 0, 0, "Usernames\nGo\nHere!!"); // https://linux.die.net/man/3/mvprintw
    wrefresh(convWindow);
}


int main(int argc, char *argv[]) {
    // Step 1: Setup 3 windows
    initscr();
    noecho();
    keypad(stdscr, TRUE);

    int W, H;
    getmaxyx(stdscr, H, W);
    curs_set(TRUE);
    WINDOW* chatsWindow = newwin(H - TYPE_SIZE, W-ADDR_WIDTH, 0, 0);
    WINDOW* inputWindow = newwin(TYPE_SIZE, W, H-TYPE_SIZE, 0);
    WINDOW* convWindow  = newwin(H - TYPE_SIZE, ADDR_WIDTH, 0, W-ADDR_WIDTH);
    printUsernames(convWindow);

    mvwprintw(chatsWindow, 0, 0, "Hello!!!!!!!!");
    wrefresh(chatsWindow);

    // Step 2: Do input loop
    ArrayListBuf buf;
    while (1) {
        wclear(inputWindow);
        // Input loop
        ArrayListBuf_init(&buf);
        int ch = 0;
        do {
            ch = wgetch(inputWindow);
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
            wclear(inputWindow);
            for (int i = 0; i < buf.N; i++) {
                mvwprintw(inputWindow, i/W, i%W, "%c", buf.buff[i]);
            }
        }
        while (ch != '\n');

        // TODO: Parse input and send message to currently selected client
        ArrayListBuf_push(&buf, &NULLTERM, 1);
        char* input = buf.buff;
        wclear(chatsWindow);
        mvwprintw(chatsWindow, 0, 0, "%s", input);
        wrefresh(chatsWindow);

        ArrayListBuf_free(&buf);
    }
    

    delwin(chatsWindow);
    delwin(inputWindow);
    delwin(convWindow);
    endwin();
}