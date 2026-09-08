#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    int size;
} LinkedList;

LinkedList* ll_create(void);
void ll_free(LinkedList *ll);

int ll_insert_back(LinkedList *ll, int value);
int ll_insert_front(LinkedList *ll, int value);
int ll_get(const LinkedList *ll, int index, int *out_val);
int ll_find(const LinkedList *ll, int value);
int ll_remove_front(LinkedList *ll, int *out_val);

#endif