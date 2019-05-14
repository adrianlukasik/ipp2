#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H

#include <stdbool.h>
#include "structure.h"
#include "list.h"

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

struct DijkstraBuffer {
    Setedges information;
    Setedges visited;
    Heap_t *heap;
    Heap_t *garbage;
    Node_t *currentNode;
};

/* Wrzuca element node na kopiec wskazywany przez h. */
void push(Heap_t *h, Node_t *node);

/* Zdejmuje z kopca wskazywanego przez h element o najwyższym priorytecie. */
Node_t *pop(Heap_t *h);

/* Dodaje node na ostatnią pozycję w kopcu h. */
void addNode_t(Heap_t *h, Node_t *node);

/* Zwalnia przydzieloną pamięć kopcowi wskazywanemu przez h. */
void clearHeap(Heap_t *h);

/* Czyści śmietnik przechowujący node. */
void clearGarbage(Heap_t *garbage);

/* Zwraca true jeśli kopiec wskazywany przez h jest pusty. */
bool isEmptyHeap(Heap_t *h);

/* Zwalnia przydzieloną pamięć wskaźnikowi node. */
void clearNode_t(Node_t *node);

/* Zwalnia przydzieloną pamięć sekwencji związanej z node. */
void clearSequenceNodes_t(Node_t *node);

/* Zwraca wskaźnik na nowo zaalokowaną strukturę Node_t. */
Node_t *getNewNode_t();

/* Uzupełnia strukturę wskazywaną przez node o brakujące elementy. */
void completeNode_t(Node_t *node, Node_t *predecessor, Citytree citytree,
                    unsigned length, int year);

/* Próbuje przydzielić pamięć na utworzenie Node_t i uzupełnia go podanymi
 * parametrami. Zwraca wynik tej próby. */
bool tryCreateNode_t(Node_t **node, Node_t *predecessor, Citytree citytree,
                     unsigned length, int year);

/* Próbuje przydzielić pamięć na utworzenie pustego kopca.
 * Zwraca wynik tej próby. */
bool tryCreateHeap_t(Heap_t **heap);

/* Zwraca węzeł w zbiorze setInformation gdzie znajduje się citytree.
 * Jeśli takiego węzła nie ma, to zwraca wartość NULL. */
Setedges isInInformation(Citytree citytree, Setedges setInformation);

/* Zwraca minimum z dwóch liczb. */
int minInt(int a, int b);

/* Wylicza najwcześcniej rok związany z drogami. */
int calculateYear(Node_t *node, Setedges setedges);

/* Wylicza długość drogi. */
unsigned calculateLength(Node_t *node, Setedges setedges);

/* Zwraca wynik zapytania czy rozważana trasa jest krótsza od obecnej. */
bool isSmallerLength(Node_t *node, Setedges setedges, Setedges element);

/* Sprawdza czy miasto citytree jest zawarte w drodze krajowej routeId. */
bool containInRoute(Citytree citytree, unsigned routeId);

/* Sprawdza czy węzeł jest końcowym miastem. */
bool isFinalCity(Node_t *node, Citytree citytree);

/* Sprawdza czy miasto wskazywane przez citytree jest odwiedzone. */
bool isCityVisited(Citytree citytree, Setedges setVisited);

/* Tworzy listę w oparciu o node. */
List createListAboutNode_t(Node_t *node);

/* Aktualizuje priorytet na podstawie node. */
void updatePriority(struct Priority *priority, Node_t *node);

/* Zwalnia pamięć związaną z db. */
void freeDijkstraBuffer(struct DijkstraBuffer *db);

/* Zwraca zbiór krawędzi związany z miastem wskazywanym przez currentNode. */
Setedges getSetedges(struct DijkstraBuffer *db);

/* Zwraca miasto związane wskazywane przez currentNode. */
Citytree getCitytree(struct DijkstraBuffer *db);

/* Próbuje zadeklarować DijkstraBuffer.
 * Zwraca wynik rezultatu. */
bool tryInitDijkstraBuffer(struct DijkstraBuffer **db, Citytree citytree);

/* Dodaje informacje do zbioru informacji. */
bool addInformation(int newyear, unsigned newlength, Setedges setedges,
                    struct DijkstraBuffer *db);

/* Próbuje dodać do kopca heap nowy element.
 * Zwraca wynik tej próby. */
bool tryAddToHeap(Heap_t *heap, Node_t *predecessor, Citytree citytree,
                  unsigned length, int year);

/* Funkcja odwiedzająca wszystkich sąsiadów db->currentNode. */
void walkNeighbours(struct DijkstraBuffer *db, Setedges setedges,
                    unsigned routeId);

/* Sprawdza status obecnego węzła. Zwraca wynik zapytania o to czy obecny węzeł
 * nie znajduje się w zbiorze odwiedzonych lub jeśli znajduje się w zbiorze
 * odwiedzonych zwraca wynik porównania priorytetów. */
bool checkStatusNode(struct DijkstraBuffer *db);

/* Zwraca rezultat wykonania algorytmu Dijkstry dla miast wskazywanych przez
 * citytree1 i citytree2. Przy czym momentem startowym tego algorytmu jest
 * miasto wskazywane przez citytree1. */
List dijkstraAlgorithm(Citytree citytree1, Citytree citytree2, unsigned routeId,
                       struct Priority *priority);

/* Zwraca drogę między miastami currentCity oraz marginalCity w postaci listy,
 * która jest rezultatem użycia algorytmu Dijkstry. */
List getRoad(Citytree currentCity, Citytree marginalCity, unsigned routeId,
             struct Priority **priority, bool firstMarginal);

/* Dodaje wszystkim elementom z listy l numer routeId do zbiorów
 * dróg krajowych. */
void addRouteIdToSetroutes(List l, unsigned routeId);

/* Próbuje wykonać algorytm Dijkstry dla drogi krajowej związanej z listą l,
 * która zawieraja odcinek citytree1 - citytree2.
 * Zwraca wynik rezultatu, czy po usunięciu powyższego odcinka d istnieje
 * jednoznaczne uzupełnienie powyższej drogi. */
bool tryDijkstraForRoutesId(Citytree citytree1, Citytree citytree2, List l,
                            unsigned routeId);

/* Zwraca pierwsze Citytree znajdujące się na liście l. */
Citytree getBeginOnList(List l);

/* Zwraca ostatnie Citytree znajdujące sie na liście l. */
Citytree getLastOnList(List l);

#endif //DROGI_DIJKSTRA_H
