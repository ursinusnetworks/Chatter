#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct LinkedNode {
    struct LinkedNode* next;
    void* data;
};

struct LinkedList {
    struct LinkedNode* head;
};

struct LinkedList* LinkedList_init();
void LinkedList_free(struct LinkedList* list);
void LinkedList_addFirst(struct LinkedList* list, void* data);
void LinkedList_print(struct LinkedList* list);

#endif