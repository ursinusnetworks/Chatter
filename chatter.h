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
    int CH; // Chat height
    WINDOW* chatsWindow;
    WINDOW* inputWindow;
    WINDOW* convWindow;
};

struct Chat {
    int sockfd;
    char name[65536];
};

struct Chatter {
    struct GUI* gui;
    char myname[65536];
    char activename[65536]; // Name of the active chat
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