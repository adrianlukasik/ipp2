#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "dijkstra.h"
#include "set_edges.h"
#include "city_tree.h"
#include "set_routes.h"

/* Wrzuca element node na kopiec wskazywany przez h. */
void push(Heap_t *h, Node_t *node) {
    if (h->len + 1 >= h->size) {
        h->size = h->size ? h->size * 2 : 4;
        h->nodes = realloc(h->nodes, h->size * sizeof(Node_t *));
    }
    int i = h->len + 1;
    int j = i / 2;
    while (i > 1 && comparePriority(node->priority, h->nodes[j]->priority)) {
        h->nodes[i] = h->nodes[j];
        i = j;
        j = j / 2;
    }
    h->nodes[i] = node;
    h->len++;
}

/* Zdejmuje z kopca wskazywanego przez h element o najwyższym priorytecie. */
Node_t *pop(Heap_t *h) {
    int i, j, k;
    if (!h->len) {
        return NULL;
    }
    Node_t *node = h->nodes[1];
    h->nodes[1] = h->nodes[h->len];
    h->len--;
    i = 1;
    while (i != h->len + 1) {
        k = h->len + 1;
        j = 2 * i;
        if (j <= h->len && comparePriority(h->nodes[j]->priority,
                                           h->nodes[k]->priority)) {
            k = j;
        }
        if (j + 1 <= h->len && comparePriority(h->nodes[j + 1]->priority,
                                               h->nodes[k]->priority)) {
            k = j + 1;
        }
        h->nodes[i] = h->nodes[k];
        i = k;
    }
    return node;
}

/* Dodaje node na ostatnią pozycję w kopcu h. */
void addNode_t(Heap_t *h, Node_t *node) {
    if (h->len + 1 >= h->size) {
        h->size = h->size ? h->size * 2 : 4;
        h->nodes = realloc(h->nodes, h->size * sizeof(Node_t *));
    }
    h->nodes[h->len] = node;
    h->len++;
}

/* Zwalnia przydzieloną pamięć kopcowi wskazywanemu przez h. */
void clearHeap(Heap_t *h) {
    if (h != NULL) {
        for (int i = 0; i < h->len; i++) {
            clearNode_t(h->nodes[i + 1]);
        }
        free(h->nodes);
        free(h);
    }
}

/* Czyści śmietnik przechowujący node. */
void clearGarbage(Heap_t *garbage) {
    if (garbage != NULL) {
        for (int i = 0; i < garbage->len; i++)
            clearNode_t(garbage->nodes[i]);
        free(garbage->nodes);
        free(garbage);
    }
}

/* Zwraca true jeśli kopiec wskazywany przez h jest pusty. */
bool isEmptyHeap(Heap_t *h) {
    if (h == NULL)
        return true;
    return h->len == 0;
}

/* Zwalnia przydzieloną pamięć wskaźnikowi node. */
void clearNode_t(Node_t *node) {
    if (node != NULL) {
        if (node->data != NULL)
            free(node->data);
        if (node->priority != NULL)
            free(node->priority);
        free(node);
    }
}

/* Zwalnia przydzieloną pamięć sekwencji związanej z node. */
void clearSequenceNodes_t(Node_t *node) {
    if (node != NULL) {
        if (node->data != NULL) {
            clearSequenceNodes_t(node->data->predecessor);
            free(node->data);
        }
        if (node->priority != NULL)
            free(node->priority);
        free(node);
    }
}

/* Zwraca wskaźnik na nowo zaalokowaną strukturę Node_t. */
Node_t *getNewNode_t() {
    struct Priority *priority = malloc(sizeof(struct Priority));
    if (priority == NULL)
        return NULL;
    struct Data *data = malloc(sizeof(struct Data));
    if (data == NULL) {
        free(priority);
        return NULL;
    }
    Node_t *node = malloc(sizeof(Node_t));
    if (node == NULL) {
        free(priority);
        free(data);
        return NULL;
    }
    node->priority = priority;
    node->data = data;
    return node;
}

/* Uzupełnia strukturę wskazywaną przez node o brakujące elementy. */
void completeNode_t(Node_t *node, Node_t *predecessor, Citytree citytree,
                    unsigned length, int year) {
    node->priority->minLength = length;
    node->priority->maxMinYear = year;
    node->data->citytree = citytree;
    node->data->predecessor = predecessor;
}

/* Próbuje przydzielić pamięć na utworzenie Node_t i uzupełnia go podanymi
 * parametrami. Zwraca wynik tej próby. */
bool tryCreateNode_t(Node_t **node, Node_t *predecessor, Citytree citytree,
                     unsigned length, int year) {
    *node = getNewNode_t();
    if (*node == NULL)
        return false;
    completeNode_t(*node, predecessor, citytree, length, year);
    return true;
}

/* Próbuje przydzielić pamięć na utworzenie pustego kopca.
 * Zwraca wynik tej próby. */
bool tryCreateHeap_t(Heap_t **heap) {
    *heap = (Heap_t *) calloc(1, sizeof(Heap_t));
    if (*heap == NULL)
        return false;
    else return true;
}

/* Tworzy listę w oparciu o node. */
List createListAboutNode_t(Node_t *node) {
    if (node == NULL) {
        return NULL;
    } else {
        List current, begin;
        begin = getNewListnode();
        completeList(begin, node->data->citytree, NULL);
        current = begin;
        node = node->data->predecessor;
        while (node != NULL) {
            current->next = getNewListnode();
            current = current->next;
            completeList(current, node->data->citytree, NULL);
            node = node->data->predecessor;
        }
        return begin;
    }
}

/* Aktualizuje priorytet na podstawie node. */
void updatePriority(struct Priority *priority, Node_t *node) {
    if (priority != NULL) {
        priority->minLength = node->priority->minLength;
        priority->maxMinYear = node->priority->maxMinYear;
    }
}

/* Sprawdza czy węzeł jest końcowym miastem. */
bool isFinalCity(Node_t *node, Citytree citytree) {
    return node->data->citytree == citytree;
}

/* Zwalnia pamięć związaną z db. */
void freeDijkstraBuffer(struct DijkstraBuffer *db) {
    if (db != NULL) {
        if (db->heap != NULL)
            clearHeap(db->heap);
        if (db->garbage != NULL)
            clearGarbage(db->garbage);
        if (db->visited != NULL)
            removeSetedges(db->visited);
        if (db->information != NULL)
            removeSetedges(db->information);
        if (db->currentNode != NULL)
            clearNode_t(db->currentNode);
        free(db);
    }
}

/* Próbuje zadeklarować DijkstraBuffer.
 * Zwraca wynik rezultatu. */
bool tryInitDijkstraBuffer(struct DijkstraBuffer **db, Citytree citytree) {
    *db = (struct DijkstraBuffer *) calloc(1, sizeof(struct DijkstraBuffer));
    if (*db == NULL)
        return false;
    if (!tryCreateHeap_t(&((*db)->heap)) ||
        !tryCreateHeap_t(&((*db)->garbage)) ||
        !tryCreateNode_t(&((*db)->currentNode), NULL, citytree, 0, INT_MAX)) {
        freeDijkstraBuffer(*db);
        return false;
    }

    return true;
}

/* Zwraca zbiór krawędzi związany z miastem wskazywanym przez currentNode. */
Setedges getSetedges(struct DijkstraBuffer *db) {
    return db->currentNode->data->citytree->setedges;
}

/* Zwraca miasto związane wskazywane przez currentNode. */
Citytree getCitytree(struct DijkstraBuffer *db) {
    return db->currentNode->data->citytree;
}

/* Sprawdza status obecnego węzła. Zwraca wynik zapytania o to czy obecny węzeł
 * nie znajduje się w zbiorze odwiedzonych lub jeśli znajduje się w zbiorze
 * odwiedzonych zwraca wynik porównania priorytetów. */
bool checkStatusNode(struct DijkstraBuffer *db) {
    return !isCityVisited(getCitytree(db), db->visited) ||
           comparePriorityAndRoad(db->currentNode->priority,
                                  binSearchSE(db->visited,
                                              getCitytree(db))->road);
}

/* Zwraca rezultat wykonania algorytmu Dijkstry dla miast wskazywanych przez
 * citytree1 i citytree2. Przy czym momentem startowym tego algorytmu jest
 * miasto wskazywane przez citytree1. */
List dijkstraAlgorithm(Citytree citytree1, Citytree citytree2, unsigned routeId,
                       struct Priority *priority) {
    struct DijkstraBuffer *db;
    List result = NULL;
    if (!tryInitDijkstraBuffer(&db, citytree1))
        return NULL;
    push(db->heap, db->currentNode);
    while (!isEmptyHeap(db->heap)) {
        db->currentNode = pop(db->heap);
        if (isFinalCity(db->currentNode, citytree2)) {
            updatePriority(priority, db->currentNode);
            if (!isEmptyHeap(db->heap)) {
                Node_t *tmp = pop(db->heap);
                addNode_t(db->garbage, tmp);
                while (comparePriority(tmp->priority,
                                       db->currentNode->priority)) {
                    if (isFinalCity(tmp, citytree2)) {
                        goto end;
                    }
                    if (!isEmptyHeap(db->heap)) {
                        tmp = pop(db->heap);
                        addNode_t(db->garbage, tmp);
                    } else {
                        break;
                    }
                }
            }
            break;
        }
        if (checkStatusNode(db)) {
            walkNeighbours(db, getSetedges(db), routeId);
            if (!isCityVisited(getCitytree(db), db->visited)) {
                Road r = getNewRoad();
                if (r == NULL)
                    goto end;
                completeRoad(r, db->currentNode->priority->minLength,
                             db->currentNode->priority->maxMinYear);
                if (!addToSetedges(&(db->visited), getCitytree(db), r))
                    goto end;
            }
        }

        addNode_t(db->garbage, db->currentNode);
        db->currentNode = NULL;
    }

    if (db->currentNode != NULL)
        result = createListAboutNode_t(db->currentNode);
    end:
    freeDijkstraBuffer(db);
    return result;
}


/* Zwraca drogę między miastami currentCity oraz marginalCity w postaci listy,
 * która jest rezultatem użycia algorytmu Dijkstry. */
List getRoad(Citytree currentCity, Citytree marginalCity, unsigned routeId,
             struct Priority **priority, bool firstMarginal) {
    *priority = malloc(sizeof(struct Priority));
    if (*priority == NULL)
        return NULL;
    marginalCity->setroutes = deleteNodeSR(marginalCity->setroutes,
                                           routeId);
    Setroutes setroutesBuffer = getNewNodeSR();
    completeNodesetroutes(setroutesBuffer, routeId);
    List result;
    if (firstMarginal)
        result = dijkstraAlgorithm(marginalCity, currentCity, routeId,
                                   *priority);
    else
        result = dijkstraAlgorithm(currentCity, marginalCity, routeId,
                                   *priority);
    marginalCity->setroutes = insertSR(marginalCity->setroutes, routeId,
                                       setroutesBuffer);
    return result;
}

/* Sprawdza czy miasto wskazywane przez citytree jest odwiedzone. */
bool isCityVisited(Citytree citytree, Setedges setVisited) {
    return binSearchSE(setVisited, citytree) != NULL;
}

/* Zwraca węzeł w zbiorze setInformation gdzie znajduje się citytree.
 * Jeśli takiego węzła nie ma, to zwraca wartość NULL. */
Setedges isInInformation(Citytree citytree, Setedges setInformation) {
    return binSearchSE(setInformation, citytree);
}

/* Zwraca minimum z dwóch liczb. */
int minInt(int a, int b) {
    return a < b ? a : b;
}

/* Zwraca wynik zapytania czy rozważana trasa jest krótsza od obecnej. */
bool isSmallerLength(Node_t *node, Setedges setedges, Setedges element) {
    bool b1 = node->priority->minLength + setedges->road->length <
              element->road->length;
    bool b2 = node->priority->minLength + setedges->road->length ==
              element->road->length;
    bool b3 = element->road->year >=
              minInt(node->priority->maxMinYear, setedges->road->year);
    return b1 || (b2 && b3);
}

/* Sprawdza czy miasto citytree jest zawarte w drodze krajowej routeId. */
bool containInRoute(Citytree citytree, unsigned routeId) {
    return binSearchSR(citytree->setroutes, routeId) != NULL;
}

/* Wylicza najwcześcniej rok związany z drogami. */
int calculateYear(Node_t *node, Setedges setedges) {
    return minInt(node->priority->maxMinYear, setedges->road->year);
}

/* Wylicza długość drogi. */
unsigned calculateLength(Node_t *node, Setedges setedges) {
    return node->priority->minLength + setedges->road->length;
}

/* Dodaje informacje do zbioru informacji. */
bool addInformation(int newyear, unsigned newlength, Setedges setedges,
                    struct DijkstraBuffer *db) {
    Setedges setedgesBuffer = getNewNodeSE();
    if (setedgesBuffer == NULL)
        return false;
    Road road = getNewRoad();
    if (road == NULL) {
        free(setedgesBuffer);
        return false;
    }
    completeRoad(road, newlength, newyear);
    completeNodesetedges(setedgesBuffer, setedges->key, road);
    db->information = insertSE(db->information, setedges->key, setedgesBuffer);
    return true;
}

/* Próbuje dodać do kopca heap nowy element.
 * Zwraca wynik tej próby. */
bool tryAddToHeap(Heap_t *heap, Node_t *predecessor, Citytree citytree,
                  unsigned length, int year) {
    Node_t *node;
    if (!tryCreateNode_t(&node, predecessor, citytree, length, year))
        return false;
    push(heap, node);
    return true;
}

/* Funkcja odwiedzająca wszystkich sąsiadów db->currentNode. */
void walkNeighbours(struct DijkstraBuffer *db, Setedges setedges,
                    unsigned routeId) {
    if (setedges != NULL) {
        if (!isCityVisited(setedges->key, db->visited) &&
            !containInRoute(setedges->key, routeId)) {
            Setedges element = isInInformation(setedges->key, db->information);
            int year = calculateYear(db->currentNode, setedges);
            unsigned length = calculateLength(db->currentNode, setedges);
            if (element == NULL) {
                addInformation(year, length, setedges, db);
                tryAddToHeap(db->heap, db->currentNode, setedges->key, length,
                             year);
            } else if (isSmallerLength(db->currentNode, setedges, element)) {
                element->road->year = year;
                element->road->length = length;
                tryAddToHeap(db->heap, db->currentNode, setedges->key, length,
                             year);
            }
        }
        walkNeighbours(db, setedges->left, routeId);
        walkNeighbours(db, setedges->right, routeId);
    }
}

/* Dodaje wszystkim elementom z listy l numer routeId do zbiorów
 * dróg krajowych. */
void addRouteIdToSetroutes(List l, unsigned routeId) {
    Setroutes setroutesBuffer;
    while (l != NULL) {
        if (binSearchSR(l->citytree->setroutes, routeId) == NULL) {
            setroutesBuffer = getNewNodeSR();
            completeNodesetroutes(setroutesBuffer, routeId);
            l->citytree->setroutes = insertSR(l->citytree->setroutes, routeId,
                                              setroutesBuffer);
        }
        l = l->next;
    }
}

/* Próbuje wykonać algorytm Dijkstry dla drogi krajowej związanej z listą l,
 * która zawieraja odcinek citytree1 - citytree2.
 * Zwraca wynik rezultatu, czy po usunięciu powyższego odcinka d istnieje
 * jednoznaczne uzupełnienie powyższej drogi. */
bool tryDijkstraForRoutesId(Citytree citytree1, Citytree citytree2, List l,
                            unsigned routeId) {
    if (!checkConnection(citytree1, citytree2, l))
        return true;
    citytree1->setroutes = deleteNodeSR(citytree1->setroutes, routeId);
    citytree2->setroutes = deleteNodeSR(citytree2->setroutes, routeId);
    Setroutes srBuffer1 = getNewNodeSR();
    completeNodesetroutes(srBuffer1, routeId);
    Setroutes srBuffer2 = getNewNodeSR();
    completeNodesetroutes(srBuffer2, routeId);
    List result = dijkstraAlgorithm(citytree1, citytree2, routeId, NULL);
    citytree1->setroutes = insertSR(citytree1->setroutes, routeId, srBuffer1);
    citytree2->setroutes = insertSR(citytree2->setroutes, routeId, srBuffer2);
    if (result == NULL) {
        return false;
    } else {
        clearList(result);
        return true;
    }
}

/* Zwraca pierwsze Citytree znajdujące się na niepustej liście l. */
Citytree getBeginOnList(List l) {
    return l->citytree;
}

/* Zwraca ostatnie Citytree znajdujące sie na niepustej liście l. */
Citytree getLastOnList(List l) {
    while (l->next != NULL) {
        l = l->next;
    }
    return l->citytree;
}
