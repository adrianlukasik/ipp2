//
// Created by root on 29.04.19.
//

#include <stdlib.h>
#include <stdio.h>
#include "structure.h"

#define INITIAL_CAPACITY 2

/* Uzupełnia dane zaalokowanej struktury wskazywanej przez r. */
void completeRoad(Road r, unsigned length, int year) {
    r->length = length;
    r->year = year;
}

/* Alokuje nowy obiekt Way i zwraca wskaźnik do niego. */
Road getNewRoad() {
    Road road = malloc(sizeof(struct Way));
    return road;
}

/* Zwraca nowy wektor. */
Vector getNewVector() {
    Vector v = malloc(sizeof(struct Dynamicarray));
    if (v == NULL)
        return NULL;
    v->size = 0;
    v->capacity = 0;
    v->arr = NULL;
    return v;
}

/* Oblicza nową wartość parametru capacity. */
int calculateNewCapacity(Vector v) {
    if (v->capacity == 0) {
        return INITIAL_CAPACITY;
    } else {
        return v->capacity * 2;
    }
}

/* Wrzuca wartość n na koniec wektora. */
void pushVector(Vector v, unsigned n) {
    if (v->size >= v->capacity) {
        int newCapacity = calculateNewCapacity(v);
        v->arr = realloc(v->arr, newCapacity * sizeof(unsigned));
        v->capacity = newCapacity;
    }
    v->arr[v->size] = n;
    ++v->size;
}

/* Zwalnia pamięć przeznaczoną na wektor. */
void clearVector(Vector v) {
    if (v != NULL) {
        if (v->arr != NULL)
            free(v->arr);
        free(v);
    }
}

/* Wyświetla elementy znajdujące się w wektorze. */
void printVector(Vector v) {
    if (v != NULL) {
        for (int i = 0; i < v->size; i++)
            printf("%u ", v->arr[i]);
        printf("\n");
    }
}