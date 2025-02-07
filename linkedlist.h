#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>

struct Node {
    struct Node* next;
    void* data;
};

struct LinkedList {
    struct Node* head;
};

struct LinkedList* LinkedList_init() {
    struct LinkedList* list = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    list->head = NULL;
    return list;
}

void LinkedList_free(struct LinkedList* list) {
    // Step 1: Clean up nodes
    struct Node* node = list->head;
    while (node != NULL) {
        struct Node* nextNode = node->next;
        free(node);
        node = nextNode;
    }
    // Step 2: Free list
    free(list);
}

void LinkedList_addFirst(struct LinkedList* list, void* data) {
    struct Node* newHead = (struct Node*)malloc(sizeof(struct Node));
    struct Node* head = list->head;
    newHead->next = list->head;
    newHead->data = data;
    list->head = newHead;
}

void LinkedList_print(struct LinkedList* list) {
    struct Node* node = list->head;
    while (node != NULL) {
        printf("%s ==> ", (char*)node->data);
        node = node->next;
    }
    printf("\n");
}

#endif