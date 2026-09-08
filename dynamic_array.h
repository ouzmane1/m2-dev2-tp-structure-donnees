#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} DynamicArray;

// Initialisation
DynamicArray* da_create(int initial_capacity);
void da_free(DynamicArray *da);


int da_insert_back(DynamicArray *da, int value);
int da_insert_front(DynamicArray *da, int value);
int da_get(const DynamicArray *da, int index, int *out_val);
int da_find(const DynamicArray *da, int value);
int da_remove_front(DynamicArray *da, int *out_val);

#endif