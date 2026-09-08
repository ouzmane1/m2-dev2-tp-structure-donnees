#include "hash_table.h"
#include <stdlib.h>

HashTable* ht_create(void) {
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) return NULL;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
    return ht;
}

void ht_free(HashTable *ht) {
    if (!ht) return;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *cur = ht->buckets[i];
        while (cur) {
            Entry *tmp = cur->next;
            free(cur);
            cur = tmp;
        }
    }
    free(ht);
}

int hash_good(int value) {
    int h = value % TABLE_SIZE;
    return h < 0 ? h + TABLE_SIZE : h;
}

int hash_bad(int value) {
    (void)value;
    return 0;
}

int ht_insert(HashTable *ht, int value, int (*hash_func)(int)) {
    int bucket = hash_func(value);
    Entry *entry = malloc(sizeof(Entry));
    if (!entry) return 0;
    entry->value = value;
    /* Insertion en tête de liste dans le bucket */
    entry->next = ht->buckets[bucket];
    ht->buckets[bucket] = entry;
    return 1;
}

int ht_contains(const HashTable *ht, int value, int (*hash_func)(int)) {
    int bucket = hash_func(value);
    Entry *cur = ht->buckets[bucket];
    while (cur) {
        if (cur->value == value) return 1;
        cur = cur->next;
    }
    return 0;
}