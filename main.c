#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dynamic_array.h"
#include "linked_list.h"
#include "hash_table.h"

/* Mesure du temps écoulé en secondes */
static double get_elapsed_sec(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void run_benchmark(int n) {
    struct timespec start, end;
    int dummy = 0;
    volatile long long sum = 0;

    printf("\n======================================================\n");
    printf(" BENCHMARK POUR N = %d elements\n", n);
    printf("======================================================\n");

    /* ===================================================================== */
    /* 1. DYNAMIC ARRAY                                                      */
    /* ===================================================================== */
    DynamicArray *da = da_create(4);

    /* Test insert_front sur 1 000 opérations (O(n) décalerait trop à grand n) */
    if (n <= 100000) {
        DynamicArray *da_front = da_create(4);
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < 1000; i++) da_insert_front(da_front, i);
        clock_gettime(CLOCK_MONOTONIC, &end);
        printf("[DynamicArray] Insert front (1000 ops) : %f s\n", get_elapsed_sec(start, end));
        da_free(da_front);
    }

    /* Remplissage via insert_back */
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < n; i++) da_insert_back(da, i);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("[DynamicArray] Insert back (%d ops)  : %f s\n", n, get_elapsed_sec(start, end));

    /* Accès direct à l'index n / 2 */
    clock_gettime(CLOCK_MONOTONIC, &start);
    da_get(da, n / 2, &dummy);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("[DynamicArray] Get(n/2)                : %.9f s\n", get_elapsed_sec(start, end));

    /* Recherche séquentielle d'un élément absent */
    clock_gettime(CLOCK_MONOTONIC, &start);
    da_find(da, -1);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("[DynamicArray] Find (absent)           : %f s\n", get_elapsed_sec(start, end));

    /* Parcours complet avec somme (test cache CPU) */
    sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < da->size; i++) sum += da->data[i];
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("[DynamicArray] Somme / Parcours        : %f s\n", get_elapsed_sec(start, end));

    /* ===================================================================== */
    /* 2. LINKED LIST                                                        */
    /* ===================================================================== */
    LinkedList *ll = ll_create();

    /* Test insert_front sur 1 000 opérations (O(1)) */
    if (n <= 100000) {
        LinkedList *ll_front = ll_create();
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < 1000; i++) ll_insert_front(ll_front, i);
        clock_gettime(CLOCK_MONOTONIC, &end);
        printf("[LinkedList]   Insert front (1000 ops) : %f s\n", get_elapsed_sec(start, end));
        ll_free(ll_front);
    }

    /* Remplissage via insert_back (avec pointeur tail O(1)) */
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < n; i++) ll_insert_back(ll, i);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("[LinkedList]   Insert back (%d ops)  : %f s\n", n, get_elapsed_sec(start, end));

    /* Accès par parcours séquentiel jusqu'à n / 2 */
    clock_gettime(CLOCK_MONOTONIC, &start);
    ll_get(ll, n / 2, &dummy);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("[LinkedList]   Get(n/2)                : %f s\n", get_elapsed_sec(start, end));

    /* Recherche séquentielle d'un élément absent */
    clock_gettime(CLOCK_MONOTONIC, &start);
    ll_find(ll, -1);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("[LinkedList]   Find (absent)           : %f s\n", get_elapsed_sec(start, end));

    /* Parcours complet avec somme (test sauts de mémoire) */
    sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    Node *cur = ll->head;
    while (cur) {
        sum += cur->value;
        cur = cur->next;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("[LinkedList]   Somme / Parcours        : %f s\n", get_elapsed_sec(start, end));

    /* ===================================================================== */
    /* 3. HASH TABLES (hash_good vs hash_bad)                                */
    /* ===================================================================== */
    HashTable *ht_g = ht_create();
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < n; i++) ht_insert(ht_g, i, hash_good);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("[Hash good]    Insert %d elems         : %f s\n", n, get_elapsed_sec(start, end));

    clock_gettime(CLOCK_MONOTONIC, &start);
    ht_contains(ht_g, -1, hash_good);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("[Hash good]    Contains (absent)       : %.9f s\n", get_elapsed_sec(start, end));

    /* hash_bad dégénère en liste chaînée unique : insertion totale en O(n^2) */
    if (n <= 100000) {
        HashTable *ht_b = ht_create();
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < n; i++) ht_insert(ht_b, i, hash_bad);
        clock_gettime(CLOCK_MONOTONIC, &end);
        printf("[Hash bad]     Insert %d elems         : %f s\n", n, get_elapsed_sec(start, end));

        clock_gettime(CLOCK_MONOTONIC, &start);
        ht_contains(ht_b, -1, hash_bad);
        clock_gettime(CLOCK_MONOTONIC, &end);
        printf("[Hash bad]     Contains (absent)       : %f s\n", get_elapsed_sec(start, end));
        ht_free(ht_b);
    } else {
        printf("[Hash bad]     Ignore pour n=%d (trop lent / quadratique O(n^2))\n", n);
    }

    /* Libération de la mémoire */
    da_free(da);
    ll_free(ll);
    ht_free(ht_g);
}

int main(void) {
    int sizes[] = {1000, 10000, 100000, 1000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        run_benchmark(sizes[i]);
    }

    return 0;
}