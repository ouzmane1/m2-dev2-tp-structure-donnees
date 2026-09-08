#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define TABLE_SIZE 10007

typedef struct Entry {
    int value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry *buckets[TABLE_SIZE];
} HashTable;

/* Initialisation et libération */
HashTable* ht_create(void);
void ht_free(HashTable *ht);

int hash_good(int value);
int hash_bad(int value);

int ht_insert(HashTable *ht, int value, int (*hash_func)(int));
int ht_contains(const HashTable *ht, int value, int (*hash_func)(int));

#endif