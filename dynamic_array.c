#include "dynamic_array.h"
#include <stdlib.h>
#include <string.h>

DynamicArray* da_create(int initial_capacity) {
    DynamicArray *da = malloc(sizeof(DynamicArray));
    if (!da) return NULL;
    da->size = 0;
    da->capacity = initial_capacity > 0 ? initial_capacity : 4;
    da->data = malloc(da->capacity * sizeof(int));
    if (!da->data) {
        free(da);
        return NULL;
    }
    return da;
}

void da_free(DynamicArray *da) {
    if (!da) return;
    free(da->data);
    free(da);
}

static int da_resize(DynamicArray *da, int new_capacity) {
    int *new_data = realloc(da->data, new_capacity * sizeof(int));
    if (!new_data) return 0;
    da->data = new_data;
    da->capacity = new_capacity;
    return 1;
}

int da_insert_back(DynamicArray *da, int value) {
    if (da->size == da->capacity) {
        if (!da_resize(da, da->capacity * 2)) return 0;
    }
    da->data[da->size++] = value;
    return 1;
}

int da_insert_front(DynamicArray *da, int value) {
    if (da->size == da->capacity) {
        if (!da_resize(da, da->capacity * 2)) return 0;
    }
    /* Décale tous les éléments existants d'un cran vers la droite */
    memmove(&da->data[1], &da->data[0], da->size * sizeof(int));
    da->data[0] = value;
    da->size++;
    return 1;
}

int da_get(const DynamicArray *da, int index, int *out_val) {
    if (index < 0 || index >= da->size) return 0;
    *out_val = da->data[index];
    return 1;
}

int da_find(const DynamicArray *da, int value) {
    for (int i = 0; i < da->size; i++) {
        if (da->data[i] == value) return i;
    }
    return -1;
}

int da_remove_front(DynamicArray *da, int *out_val) {
    if (da->size == 0) return 0;
    if (out_val) *out_val = da->data[0];
    da->size--;
    /* Décale les éléments restants d'un cran vers la gauche */
    memmove(&da->data[0], &da->data[1], da->size * sizeof(int));
    return 1;
}