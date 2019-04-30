//
// Created by root on 29.04.19.
//

#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H

#include <stdbool.h>
#include "structure.h"
#include "list.h"

struct Priority {
    unsigned minLength;
    int maxMinYear;
};

struct Data;

typedef struct {
    struct Priority *priority;
    struct Data *data;
} Node_t;

struct Data {
    Citytree citytree;
    Node_t *predecessor;
};

typedef struct {
    Node_t **nodes;
    int len;
    int size;
} Heap_t;

/* Porównuje ze sobą first i second i zwraca wynik zapytania
 * czy first ma niemniejszy priorytet niż second. */
bool comparePriority(struct Priority *first, struct Priority *second);

/* Wrzuca element node na kopiec wskazywany przez h. */
void push(Heap_t *h, Node_t *node);

/* Zdejmuje z kopca wskazywanego przez h element o najwyższym priorytecie. */
Node_t *pop(Heap_t *h);

/* Zwalnia przydzieloną pamięć kopcowi wskazywanemu przez h. */
void clearHeap(Heap_t *h);

/* Zwraca true jeśli kopiec wskazywany przez h jest pusty. */
bool isEmptyHeap(Heap_t *h);

/* Zwalnia przydzieloną pamięć wskaźnikowi node. */
void clearNode_t(Node_t *node);

/* Zwraca wskaźnik na nowo zaalokowaną strukturę Node_t. */
Node_t *getNewNode_t();

/* Uzupełnia strukturę wskazywaną przez node o brakujące elementy. */
void completeNode_t(Node_t *node, Node_t *predecessor, Citytree citytree,
                    unsigned length, int year);

/* Funkcja odwiedzająca wszystkich sąsiadów node. */
void walkNeighbours(Node_t *node, Setedges setedges, Setedges setInformation,
                    Setedges setVisited, Heap_t *heap, unsigned routeId);

Setedges isInInformation(Citytree citytree, Setedges setInformation);

int minInt(int a, int b);

bool isSmallerLength(Node_t *node, Setedges setedges, Setedges element);

bool containInRoute(Citytree citytree, unsigned routeId);

/* Sprawdza czy miasto wskazywane przez citytree jest odwiedzone. */
bool isCityVisited(Citytree citytree, Setedges setVisited);

/* Tworzy listę w oparciu o node. */
List createListAboutNode_t(Node_t *node);

/* Zwraca rezultat wykonania algorytmu Dijkstry dla miast wskazywanych przez
 * citytree1 i citytree2. Przy czym momentem startowym tego algorytmu jest
 * miasto wskazywane przez citytree1. */
List dijkstraAlgorithm(Citytree citytree1, Citytree citytree2, unsigned routeId,
                       struct Priority *priority);

/* Dodaje wszystkim elementom z listy l numer routeId do zbiorów
 * dróg krajowych. */
void addRouteIdToSetroutes(List l, unsigned routeId);

/* Zwraca pierwsze Citytree znajdujące się na liście l. */
Citytree getBeginOnList(List l);

/* Zwraca ostatnie Citytree znajdujące sie na liście l. */
Citytree getLastOnList(List l);

#endif //DROGI_DIJKSTRA_H
