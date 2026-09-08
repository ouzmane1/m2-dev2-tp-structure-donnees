#include "linked_list.h"
#include <stdlib.h>

LinkedList* ll_create(void) {
    LinkedList *ll = malloc(sizeof(LinkedList));
    if (!ll) return NULL;
    ll->head = NULL;
    ll->tail = NULL;
    ll->size = 0;
    return ll;
}

void ll_free(LinkedList *ll) {
    if (!ll) return;
    Node *cur = ll->head;
    while (cur) {
        Node *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(ll);
}

int ll_insert_front(LinkedList *ll, int value) {
    Node *node = malloc(sizeof(Node));
    if (!node) return 0;
    node->value = value;
    node->next = ll->head;
    ll->head = node;
    if (ll->tail == NULL) ll->tail = node;
    ll->size++;
    return 1;
}

int ll_insert_back(LinkedList *ll, int value) {
    Node *node = malloc(sizeof(Node));
    if (!node) return 0;
    node->value = value;
    node->next = NULL;
    if (ll->tail) {
        ll->tail->next = node;
        ll->tail = node;
    } else {
        ll->head = node;
        ll->tail = node;
    }
    ll->size++;
    return 1;
}

int ll_get(const LinkedList *ll, int index, int *out_val) {
    if (index < 0 || index >= ll->size) return 0;
    Node *cur = ll->head;
    for (int i = 0; i < index; i++) cur = cur->next;
    *out_val = cur->value;
    return 1;
}

int ll_find(const LinkedList *ll, int value) {
    Node *cur = ll->head;
    int idx = 0;
    while (cur) {
        if (cur->value == value) return idx;
        cur = cur->next;
        idx++;
    }
    return -1;
}

int ll_remove_front(LinkedList *ll, int *out_val) {
    if (ll->size == 0) return 0;
    Node *old_head = ll->head;
    if (out_val) *out_val = old_head->value;
    ll->head = old_head->next;
    if (ll->head == NULL) ll->tail = NULL;
    free(old_head);
    ll->size--;
    return 1;
}