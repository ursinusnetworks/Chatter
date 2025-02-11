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
    WINDOW* chatsWindow;
    WINDOW* inputWindow;
    WINDOW* convWindow;
};

struct Chatter {
    struct GUI* gui;
};

struct GUI* initGUI();
void destroyGUI(struct GUI* gui);
void reprintUsernameWindow(struct GUI* gui);
void reprintChatWindow(struct GUI* gui);
void parseInput(struct Chatter* chatter, char* input);
void typeLoop(struct Chatter* chatter);

struct Chatter* initChatter();
void destroyChatter(struct Chatter* chatter);


#endif