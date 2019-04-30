//
// Created by root on 29.04.19.
//

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "dijkstra.h"
#include "set_edges.h"
#include "city_tree.h"
#include "set_routes.h"

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

/* Zwraca rezultat wykonania algorytmu Dijkstry dla miast wskazywanych przez
 * citytree1 i citytree2. Przy czym momentem startowym tego algorytmu jest
 * miasto wskazywane przez citytree1. */
List dijkstraAlgorithm(Citytree citytree1, Citytree citytree2, unsigned routeId,
                       struct Priority *priority) {
    List result = NULL;
    Heap_t *h = (Heap_t *) calloc(1, sizeof(Heap_t));
    bool flag = 0;
    if (h == NULL)
        return false;
    Setedges setInformation = NULL;
    Setedges setVisited = NULL;
    Node_t *node = getNewNode_t();
    if (node == NULL) {
        clearHeap(h);
        return false;
    }
    completeNode_t(node, NULL, citytree1, 0, INT_MAX);
    push(h, node);
    while (!isEmptyHeap(h)) {
        node = pop(h);
        if (node->data->citytree == citytree2) {
            flag = 1;
            if (priority != NULL) {
                priority->minLength = node->priority->minLength;
                priority->maxMinYear = node->priority->maxMinYear;
            }
            break;
        }
        if (!isCityVisited(node->data->citytree, setVisited)) {
            walkNeighbours(node, node->data->citytree->setedges, setInformation,
                           setVisited, h, routeId);
            Setedges setedgesBuffer = getNewNodeSE();
            completeNodesetedges(setedgesBuffer, node->data->citytree, NULL);
            setVisited = insertSE(setVisited, node->data->citytree,
                                  setedgesBuffer);
        }
    }

    if (flag)
        result = createListAboutNode_t(node);


    clearSequenceNodes_t(node);
    clearHeap(h);
    removeSetedges(setVisited);
    removeSetedges(setInformation);

    return result;
}

/* Sprawdza czy miasto wskazywane przez citytree jest odwiedzone. */
bool isCityVisited(Citytree citytree, Setedges setVisited) {
    return binSearchSE(setVisited, citytree) != NULL;
}

Setedges isInInformation(Citytree citytree, Setedges setInformation) {
    return binSearchSE(setInformation, citytree);
}

int minInt(int a, int b) {
    return a < b ? a : b;
}

bool isSmallerLength(Node_t *node, Setedges setedges, Setedges element) {
    bool b1 = node->priority->minLength + setedges->road->length <
              element->road->length;
    bool b2 = node->priority->minLength + setedges->road->length ==
              element->road->length;
    bool b3 = element->road->year >=
              minInt(node->priority->maxMinYear, setedges->road->year);
    return b1 || (b2 && b3);
}

bool containInRoute(Citytree citytree, unsigned routeId) {
    return binSearchSR(citytree->setroutes, routeId) != NULL;
}

/* Funkcja odwiedzająca wszystkich sąsiadów node. */
void walkNeighbours(Node_t *node, Setedges setedges, Setedges setInformation,
                    Setedges setVisited, Heap_t *heap, unsigned routeId) {
    if (setedges != NULL) {
        if (!isCityVisited(setedges->key, setVisited) &&
            !containInRoute(setedges->key, routeId)) {
            Setedges element = isInInformation(setedges->key, setInformation);
            if (element == NULL) {
                Setedges setedgesBuffer = getNewNodeSE();
                Road road = getNewRoad();
                int newyear = minInt(node->priority->maxMinYear,
                                     setedges->road->year);
                unsigned newlength =
                        node->priority->minLength + setedges->road->length;
                completeRoad(road, newlength, newyear);
                completeNodesetedges(setedgesBuffer, setedges->key, road);
                setInformation = insertSE(setInformation, setedges->key,
                                          setedgesBuffer);
                Node_t *tmp = getNewNode_t();
                completeNode_t(tmp, node, setedges->key, newlength, newyear);
                push(heap, tmp);
            } else {
                if (isSmallerLength(node, setedges, element)) {
                    element->road->year = minInt(node->priority->maxMinYear,
                                                 setedges->road->year);
                    element->road->length =
                            node->priority->minLength + setedges->road->length;
                    Node_t *tmp = getNewNode_t();
                    completeNode_t(tmp, node, setedges->key,
                                   element->road->length, element->road->year);
                    push(heap, tmp);
                }
            }
        }
        walkNeighbours(node, setedges->left, setInformation, setVisited, heap,
                       routeId);
        walkNeighbours(node, setedges->right, setInformation, setVisited, heap,
                       routeId);
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
