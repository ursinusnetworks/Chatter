#include <ncurses.h>
#include <unistd.h>
#include "linkedlist.h"
#include "hashmap.h"

int main(int argc, char *argv[]) {
    // Step 1: Setup 3 windows

    // Step 2: Setup state for which window has focus
    initscr();

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    //noecho();
    curs_set(TRUE);
    int x = 0;
    /*while(1) {
        clear();
        mvprintw(0, x, "max_x = %i, max_y = %i", max_x, max_y);
        refresh();
        x++;
        sleep(1);

    }*/
   int ch = 0;
   do {
    ch = getch();
   }
   while (ch != '\n');
   
    endwin();
}