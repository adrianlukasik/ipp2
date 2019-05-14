#include <stdlib.h>
#include <stdio.h>
#include "structure.h"
#include "city_tree.h"

#define INITIAL_CAPACITY 2
#define INCORRECT_YEAR 0
#define INCORRECT_LENGTH 0

/* Porównuje ze sobą first i second i zwraca wynik zapytania
 * czy first ma niemniejszy priorytet niż second. */
bool comparePriority(struct Priority *first, struct Priority *second) {
    if (first->minLength < second->minLength)
        return true;
    else if (first->minLength == second->minLength)
        return first->maxMinYear >= second->maxMinYear;
    else
        return false;
}

/* Porównuje ze sobą priority i road i zwraca wynik zapytania
 * czy priority ma niemniejszy priorytet niż road. */
bool comparePriorityAndRoad(struct Priority *priority, Road road) {
    if (priority->minLength < road->length)
        return true;
    else if (priority->minLength == road->length)
        return priority->maxMinYear >= road->year;
    else
        return false;
}

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

/* Wrzuca do wektora zawartość zbioru dróg krajowych. */
void inOrder(Setroutes t, Vector v) {
    if (t != NULL) {
        inOrder(t->left, v);
        pushVector(v, t->key);
        inOrder(t->right, v);
    }
}

/* Zwraca wektor, którego elementami są wspólne drogi krajowe dwóch miast. */
Vector getCommonRoutesId(Citytree citytree1, Citytree citytree2) {
    Vector v1 = getNewVector();
    Vector v2 = getNewVector();
    Vector v3 = getNewVector();
    inOrder(citytree1->setroutes, v1);
    inOrder(citytree2->setroutes, v2);
    int i = 0, j = 0;
    while (i < v1->size && j < v2->size) {
        if (v1->arr[i] == v2->arr[j]) {
            pushVector(v3, v1->arr[i]);
            i++, j++;
        } else if (v1->arr[i] > v2->arr[j]) {
            j++;
        } else {
            i++;
        }
    }
    clearVector(v1);
    clearVector(v2);
    return v3;
}

/* Sprawdza czy podany rok year jest poprawny. */
bool isCorrectYear(int year) {
    return year != INCORRECT_YEAR;
}

/* Sprawdza czy długość length jest poprawna. */
bool isCorrectLength(unsigned length) {
    return length != INCORRECT_LENGTH;
}

/* Sprawdza czy numer drogi krajowej routeId jest poprawny. */
bool isCorrectRouteId(unsigned routeId) {
    return routeId >= MIN_ROUTE_ID && routeId <= MAX_ROUTE_ID;
}