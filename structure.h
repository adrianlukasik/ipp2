#ifndef DROGI_STRUCTURE_H
#define DROGI_STRUCTURE_H

#include <stdbool.h>

#define MAX_ROUTE_ID 999
#define MIN_ROUTE_ID 1

struct Node;
typedef struct Node *Tree;

struct Citynode;
typedef struct Citynode *Citytree;

struct Nodesetroutes;
typedef struct Nodesetroutes *Setroutes;

struct Nodesetroutes {
    unsigned key;
    Setroutes left;
    Setroutes right;
    int height;
};

struct Nodesetedges;
typedef struct Nodesetedges *Setedges;

struct Listnode;
typedef struct Listnode *List;

struct Priority {
    unsigned minLength;
    int maxMinYear;
};

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

/* Porównuje ze sobą first i second i zwraca wynik zapytania
 * czy first ma niemniejszy priorytet niż second. */
bool comparePriority(struct Priority *first, struct Priority *second);

/* Porównuje ze sobą priority i road i zwraca wynik zapytania
 * czy priority ma niemniejszy priorytet niż road. */
bool comparePriorityAndRoad(struct Priority *priority, Road road);

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

/* Wrzuca do wektora zawartość zbioru dróg krajowych. */
void inOrder(Setroutes t, Vector v);

/* Zwraca wektor, którego elementami są wspólne drogi krajowe dwóch miast. */
Vector getCommonRoutesId(Citytree citytree1, Citytree citytree2);

struct Map {
    Citytree ct;
    List routes[MAX_ROUTE_ID + 1];
};

/* Sprawdza czy podany rok year jest poprawny. */
bool isCorrectYear(int year);

/* Sprawdza czy długość length jest poprawna. */
bool isCorrectLength(unsigned length);

/* Sprawdza czy numer drogi krajowej routeId jest poprawny. */
bool isCorrectRouteId(unsigned routeId);

#endif //DROGI_STRUCTURE_H
