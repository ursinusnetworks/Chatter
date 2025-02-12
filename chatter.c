#include <ncurses.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "linkedlist.h"
#include "hashmap.h"
#include "arraylist.h"
#include "chatter.h"

#define BACKLOG 20

///////////////////////////////////////////////////////////
//       Data Structure Memory Management
///////////////////////////////////////////////////////////

struct Chat* initChat(int sockfd) {
    struct Chat* chat = (struct Chat*)malloc(sizeof(struct Chat));
    chat->messagesIn = LinkedList_init();
    chat->messagesOut = LinkedList_init();
    chat->outCounter = 0;
    chat->sockfd = sockfd;
    return chat;
}

void destroyChat(struct Chat* chat) {
    freeMessages(chat->messagesIn);
    LinkedList_free(chat->messagesIn);
    freeMessages(chat->messagesOut);
    LinkedList_free(chat->messagesOut);
    free(chat);
}

void freeMessages(struct LinkedList* messages) {
    struct LinkedNode* node = messages->head;
    while (node != NULL) {
        struct Message* message = (struct Message*)node->data;
        free(message->text);
        free(message);
        node = node->next;
    }
}

struct Chatter* initChatter() {
    // Dynamically allocate all objects that need allocating
    struct Chatter* chatter = (struct Chatter*)malloc(sizeof(struct Chatter));
    chatter->gui = initGUI();
    strcpy(chatter->myname, "Anonymous");
    chatter->chats = LinkedList_init();
    return chatter;
}

void destroyChatter(struct Chatter* chatter) {
    destroyGUI(chatter->gui);
    struct LinkedNode* chatNode = chatter->chats->head;
    while (chatNode != NULL) {
        struct Chat* chat = (struct Chat*)chatNode->data;
        destroyChat(chat);
        chatNode = chatNode->next;
    }
    LinkedList_free(chatter->chats);
    free(chatter);
}

///////////////////////////////////////////////////////////
//       GUI Update Methods
//       See: https://linux.die.net/man/3/mvprintw
///////////////////////////////////////////////////////////

void reprintUsernameWindow(struct GUI* gui) {
    wclear(gui->convWindow);
    // TODO:  Tralie
    wrefresh(gui->convWindow);
}

void reprintChatWindow(struct GUI* gui) {
    wclear(gui->chatsWindow);
    // TODO: Print stuff in the chat window
    wrefresh(gui->chatsWindow);
}



///////////////////////////////////////////////////////////
//             Chat Session Messages Out
///////////////////////////////////////////////////////////

/**
 * @brief Establish a chat with an IP address
 * 
 * @param chatter Data about the current chat session
 * @param IP IP address in human readable form
 * @param port Port on which to establish connection
 */
void connectChat(struct Chatter* chatter, char* IP, char* port) {
    // Step 1: Establish connection
    
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
 * @brief Close chat with someone
 * 
 * @param chatter Data about the current chat session
 * @param name Close connection with this person
 */
void closeChat(struct Chatter* chatter, char* name) {

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



///////////////////////////////////////////////////////////
//             Chat Session Messages In
///////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////
//               Connection Management
///////////////////////////////////////////////////////////

/**
 * @brief Print out a socket error, pause, and then exit
 * 
 * @param chatter 
 * @param fmt 
 */
void socketErrorAndExit(struct Chatter* chatter, char* fmt) {
    char* error = (char*)malloc(strlen(fmt) + 100);
    sprintf(error, fmt, errno);
    printErrorGUI(chatter->gui, error);
    free(error);
    sleep(5);
    destroyChatter(chatter);
    exit(errno);
}

/**
 * @brief Continually loop through and accept new connections
 * 
 * @param pargs Pointer to the chatter data
 */
void* serverLoop(void* pargs) {
    struct Chatter* chatter = (struct Chatter*)pargs;
    while (1) {
        struct sockaddr_storage their_addr;
        socklen_t len = sizeof(their_addr);
        int sockfd = accept(chatter->serversock, (struct sockaddr*)&their_addr, &len);
        if (sockfd != -1) {
            // Put an entry 
        }
    }
}


int main(int argc, char *argv[]) {
    char* port = "60000";
    if (argc > 1) {
        port = argv[1];
    }
    // Step 1: Initialize chatter object and setup server to listen for incoming connections
    struct Chatter* chatter = initChatter();
    struct GUI* gui = chatter->gui;
    // Step 1a: Parse Parameters and initialize variables
    struct addrinfo hints;
    struct addrinfo* info;
    // Step 1b: Find address information of domain and attempt to open socket
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // OK to use either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; //Using TCP
    hints.ai_flags = AI_PASSIVE; // Use my IP (extremely important!!)
    getaddrinfo(NULL, port, &hints, &info);
    chatter->serversock = -1;
    struct addrinfo* node = info;
    while (node != NULL && chatter->serversock == -1) {
        chatter->serversock = socket(node->ai_family, node->ai_socktype, node->ai_protocol); // NOTE: Not bound to port yet
        if (chatter->serversock == -1) {
            printErrorGUI(gui, "Error on socket...trying another one\n");
            node = node->ai_next;
        }
        int yes = 1;
        if (setsockopt(chatter->serversock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            freeaddrinfo(info);
            socketErrorAndExit(chatter, "Error number %i setting socket options\n");
        }
        else if (bind(chatter->serversock, node->ai_addr, node->ai_addrlen) == -1) {
            freeaddrinfo(info);
            socketErrorAndExit(chatter, "Error number %i binding socket\n");
        }
    }
    freeaddrinfo(info);
    // Step 1c: Service requests (single threaded for now)
    if (chatter->serversock == -1 || node == NULL) {
        socketErrorAndExit(chatter, "ERROR: Error number %i on opening socket\n");
    }
    if (listen(chatter->serversock, BACKLOG) == -1) {
        socketErrorAndExit(chatter, "Error number %i listening on socket\n");
    }
    pthread_t serverThread;
    int res = pthread_create(&serverThread, NULL, serverLoop, (void*)chatter);
    if (res != 0) {
        socketErrorAndExit(chatter, "Error number %i creating server thread\n");
        destroyChatter(chatter);
        exit(res);
    }

    // Step 2: Begin the input loop on the client side
    typeLoop(chatter);

    // Step 3: Clean everything up when it's over
    destroyChatter(chatter);
}