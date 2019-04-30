//
// Created by root on 29.04.19.
//

#ifndef DROGI_STRUCTURE_H
#define DROGI_STRUCTURE_H

struct Node;
typedef struct Node *Tree;

struct Citynode;
typedef struct Citynode *Citytree;

struct Nodesetroutes;
typedef struct Nodesetroutes *Setroutes;

struct Nodesetedges;
typedef struct Nodesetedges *Setedges;

struct String {
    char *name;
    int length;
};

typedef struct String *City;

struct Way {
    unsigned length;
    int year;
};

typedef struct Way *Road;

/* Uzupełnia dane zaalokowanej struktury wskazywanej przez r. */
void completeRoad(Road r, unsigned length, int year);

/* Alokuje nowy obiekt Way i zwraca wskaźnik do niego. */
Road getNewRoad();

struct Dynamicarray {
    unsigned *arr;
    int capacity;
    int size;
};

typedef struct Dynamicarray *Vector;

/* Zwraca nowy wektor. */
Vector getNewVector();

/* Oblicza nową wartość parametru capacity. */
int calculateNewCapacity(Vector v);

/* Wrzuca wartość n na koniec wektora. */
void pushVector(Vector v, unsigned n);

/* Zwalnia pamięć przeznaczoną na wektor. */
void clearVector(Vector v);

/* Wyświetla elementy znajdujące się w wektorze. */
void printVector(Vector v);

#endif //DROGI_STRUCTURE_H
