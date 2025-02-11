#include <ncurses.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashmap.h"
#include "arraylist.h"
#include "chatter.h"


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

void parseInput(struct Chatter* chatter, char* input) {
    struct GUI* gui = chatter->gui;


    reprintUsernameWindow(gui);
    reprintChatWindow(gui);
}


struct Chatter* initChatter() {
    struct Chatter* chatter = (struct Chatter*)malloc(sizeof(struct Chatter));
    chatter->gui = initGUI();

    return chatter;
}

void destroyChatter(struct Chatter* chatter) {
    destroyGUI(chatter->gui);
    free(chatter);
}


int main(int argc, char *argv[]) {
    struct Chatter* chatter = initChatter();
    typeLoop(chatter);
    destroyChatter(chatter);
}