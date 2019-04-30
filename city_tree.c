//
// Created by root on 29.04.19.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "city_tree.h"
#include "tree.h"
#include "set_routes.h"
#include "set_edges.h"

/* Uzupełnia dane zaalokowanej struktury wskazywanej przez ct. */
void completeCitynode(Citytree ct) {
    ct->city = NULL;
    ct->root = NULL;
    ct->setedges = NULL;
    ct->setroutes = NULL;
}

/* Alokuje nowego Citynode i zwraca wskaźnik do niego. */
Citytree newCitynode() {
    Citytree citynode = (Citytree) malloc(sizeof(struct Citynode));
    if (citynode == NULL)
        return NULL;
    completeCitynode(citynode);
    return citynode;
}

/* Zwraca wskaźnik na miasto typu Citytree,
 * jeśli znajduje się w drzewie wskazywanym przez ct.
 * W przeciwnym przypadku zwraca NULL. */
Citytree isCity(Citytree ct, const char *city) {
    Tree t;
    int n = (int) strlen(city);
    for (int i = 0; i < n; i++) {
        t = ct->root;
        t = binSearch(t, city[i]);
        if (t == NULL)
            return false;
        ct = t->citytree;
    }
    if (ct->city == NULL)
        return NULL;
    else
        return ct;
}

/* Zwraca wskaźnik na strukturę związaną z najdłuższym prefixem nazwy
 * miasta city. */
Citytree getLastPrefix(Citytree ct, const char *city, int *i, int n) {
    Tree t;
    while (*i < n) {
        t = binSearch(ct->root, city[*i]);
        if (t == NULL)
            return ct;
        ct = t->citytree;
        (*i)++;
    }
    return ct;
}

/* Próbuje zaalokować pamięć na utworzenie nowego miasta.
 * Jeśli udało się przydzielić wystarczającą pamięć zwraca true.
 * W przeciwnym przypadku zwalnia pamięć, która była potrzebna, ale
 * nie wystarczyła do utworzenia nowego miasta i zwraca false. */
bool tryAllocNewCityBuffer(struct Buffer *buffer) {
    int diff = buffer->size - buffer->index;
    if (diff > 0) {
        buffer->bufferCitytree = malloc(diff * sizeof(Citytree));
        if (buffer->bufferCitytree == NULL)
            return false;
        for (int i = 0; i < diff; i++) {
            (buffer->bufferCitytree)[i] = malloc(sizeof(struct Citynode));
            if ((buffer->bufferCitytree)[i] == NULL) {
                removeBufferCitytree(&(buffer->bufferCitytree), i);
                return false;
            }
        }
        buffer->bufferTree = malloc(diff * sizeof(Tree));
        if (buffer->bufferTree == NULL) {
            removeBufferCitytree(&(buffer->bufferCitytree), diff);
            return false;
        }
        for (int i = 0; i < diff; i++) {
            (buffer->bufferTree)[i] = malloc(sizeof(struct Node));
            if ((buffer->bufferTree)[i] == NULL) {
                removeBufferTree(&(buffer->bufferTree), i);
                removeBufferCitytree(&(buffer->bufferCitytree), diff);
                return false;
            }
        }
    }
    if (!isExistCity(buffer)) {
        buffer->city = getNewCity(buffer->size);
        if (buffer->city == NULL) {
            removeBufferCitytree(&(buffer->bufferCitytree), diff);
            removeBufferTree(&(buffer->bufferTree), diff);
            return false;
        }
    }
    buffer->road = getNewRoad();
    if (buffer->road == NULL) {
        removeBufferCitytree(&(buffer->bufferCitytree), diff);
        removeBufferTree(&(buffer->bufferTree), diff);
        if (!isExistCity(buffer))
            free(buffer->city);
        return false;
    }
    buffer->setedges = getNewNodeSE();
    if (buffer->setedges == NULL) {
        removeBufferCitytree(&(buffer->bufferCitytree), diff);
        removeBufferTree(&(buffer->bufferTree), diff);
        if (!isExistCity(buffer))
            free(buffer->city);
        free(buffer->road);
        return false;
    }
    return true;
}

/* Czyści bufor, który prawidłowo zaalokował pamięć na miasto i drogę. */
void clearBuffer(struct Buffer *buffer) {
    if (buffer->size - buffer->index > 0) {
        free(buffer->bufferCitytree);
        free(buffer->bufferTree);
    }
}

void removeBuffer(struct Buffer *buffer) {
    int diff = buffer->size - buffer->index;
    removeBufferCitytree(&(buffer->bufferCitytree), diff);
    removeBufferTree(&(buffer->bufferTree), diff);
    if (!isExistCity(buffer))
        removeCity(buffer->city);
    free(buffer->road);
    free(buffer->setedges);
}

/* Uzupełnia bufor o zmienne przydatne do alokacji pamięci. */
void completeBuffer(struct Buffer *buffer, Citytree ct, const char *city) {
    buffer->index = 0;
    buffer->size = (int) strlen(city);
    buffer->ct = getLastPrefix(ct, city, &(buffer->index), buffer->size);
}

/* Sprawdza czy istnieje miasto w strukturze. */
bool isExistCity(struct Buffer *buffer) {
    return buffer->index == buffer->size && buffer->ct->city != NULL;
}

/* Sprawdza czy istnieją miasta związane z buforami i następnie weryfikuje
 * czy istnieje między nimi droga. */
bool isConnection(struct Buffer *buffer1, struct Buffer *buffer2) {
    return isExistCity(buffer1) && isExistCity(buffer2) &&
           (binSearchSE(buffer1->ct->setedges, buffer2->ct) != NULL);
}

/* Jeśli nie ma miast city1 lub city2 to alokuje odpowiednią wielkość
 * pamięci na nie i dodaje je do drzewa ct. Następnie alokuje pamięć na drogę
 * o parametrach length i builtYear.
 * Zwraca wartość true jeśli udało się zaalokować potrzebną pamięć na miasta
 * city1, city2 oraz drogę między tymi miastami następnie dodaje wszystko do ct.
 * W przeciwnym przypadku nie narusza struktury drzewa ct i zwraca wartość
 * false. */
bool addRoadBetweenCities(Citytree ct, const char *city1, const char *city2,
                          unsigned length, int builtYear) {
    struct Buffer buffer1, buffer2;
    completeBuffer(&buffer1, ct, city1);
    completeBuffer(&buffer2, ct, city2);
    if (isConnection(&buffer1, &buffer2)) {
        return false;
    }
    if (!tryAllocNewCityBuffer(&buffer1))
        return false;
    if (!tryAllocNewCityBuffer(&buffer2)) {
        removeBuffer(&buffer1);
        return false;
    }
    assignBufferOnCity(&buffer1, city1);
    assignBufferOnCity(&buffer2, city2);
    assignBufferOnRoad(&buffer1, &buffer2, city1, length, builtYear);
    assignBufferOnRoad(&buffer2, &buffer1, city2, length, builtYear);
    clearBuffer(&buffer1);
    clearBuffer(&buffer2);
    return true;
}

/* Przydziela z buforu pamięć na połączenie między miastami.
 * Dodaje do struktur odpowiednie informacje związane z drogą między
 * miastami. */
void assignBufferOnRoad(struct Buffer *buffer1, struct Buffer *buffer2,
                        const char *city, unsigned length, int builtYear) {
    if (!isExistCity(buffer1)) {
        completeCity(buffer1->city, city);
        buffer1->ct->city = buffer1->city;
    }
    completeRoad(buffer1->road, length, builtYear);
    completeNodesetedges(buffer1->setedges, buffer2->ct, buffer1->road);
    buffer1->ct->setedges = insertSE(buffer1->ct->setedges, buffer2->ct,
                                     buffer1->setedges);
}

/* Uzupełnia strukturę o elementy związane z dodaniem miast. */
void assignBufferOnCity(struct Buffer *buffer, const char *city) {
    int diff = buffer->size - buffer->index;
    for (int i = 0; i < diff; i++) {
        completeCitynode(buffer->bufferCitytree[i]);
        completeNode(buffer->bufferTree[i], city[buffer->index + i],
                     buffer->bufferCitytree[i]);
        buffer->ct->root = insert(buffer->ct->root, city[buffer->index + i],
                                  buffer->bufferTree[i]);
        Tree t = binSearch(buffer->ct->root, city[buffer->index + i]);
        buffer->ct = t->citytree;
    }
}

/* Zwraca wskaźnik do struktury przechowującej dane o mieście.
 * Długość napisu reprezentującego miasto wynosi n. */
City getNewCity(int n) {
    char *newname = malloc((n + 1) * sizeof(char));
    if (newname == NULL)
        return NULL;
    City newcity = malloc(sizeof(struct String));
    if (newcity == NULL) {
        free(newname);
        return NULL;
    }
    newcity->name = newname;
    newcity->length = n;
    return newcity;
}

/* Uzupełnia w strukturze wskazywanej przez city nazwę miasta. */
void completeCity(City city, const char *name) {
    sprintf(city->name, "%s", name);
}

/* Kasuje przydzieloną pamięć do miasta city. */
void removeCity(City city) {
    free(city->name);
    free(city);
}

/* Kasuje przydzieloną pamieć do bufora bufferCitytree. */
void removeBufferCitytree(Citytree **bufferCitytree, int size) {
    if (size > 0) {
        for (int i = 0; i < size; i++)
            free((*bufferCitytree)[i]);
        free(*bufferCitytree);
    }
}

/* Kasuje przydzieloną pamięć do bufora bufferTree. */
void removeBufferTree(Tree **bufferTree, int size) {
    if (size > 0) {
        for (int i = 0; i < size; i++)
            free((*bufferTree)[i]);
        free(*bufferTree);
    }
}

/* Wyświetla miasta znajdujące się w drzewie ct. */
void showCities(Citytree ct, char *pom, int index) {
    if (ct->root == NULL) {
        for (int i = 0; i < index; ++i)
            printf("%c", pom[i]);
        printf("\n");
    } else {
        showCharCity(ct->root, pom, index);
    }
}

/* Funkcja pomocnicza funkcji showCities.
 * Wywołuje funkcje showCities dla elementów drzewa t. */
void showCharCity(Tree t, char *pom, int index) {
    if (t != NULL) {
        pom[index] = t->key;
        showCities(t->citytree, pom, index + 1);
        showCharCity(t->left, pom, index);
        showCharCity(t->right, pom, index);
    }
}

/* Usuwa strukturę wskazywaną przez ct. */
void removeCitytree(Citytree ct) {
    if (ct != NULL) {
        removeSetroutes(ct->setroutes);
        removeSetedges(ct->setedges);
        if (ct->city != NULL) {
            free(ct->city->name);
            free(ct->city);
        }
        removeTree(ct->root);
        free(ct);
    }
}

/* Usuwa strukturę wskazywaną przez t. */
void removeTree(Tree t) {
    if (t != NULL) {
        removeCitytree(t->citytree);
        removeTree(t->left);
        removeTree(t->right);
        free(t);
    }
}

/* Usuwa strukturę wskazywaną przez setroutes. */
void removeSetroutes(Setroutes setroutes) {
    if (setroutes != NULL) {
        removeSetroutes(setroutes->left);
        removeSetroutes(setroutes->right);
        free(setroutes);
    }
}

/* Usuwa strukturę wskazywaną przez setedges. */
void removeSetedges(Setedges setedges) {
    if (setedges != NULL) {
        removeSetedges(setedges->left);
        removeSetedges(setedges->right);
        free(setedges->road);
        free(setedges);
    }
}

/* Dodaje numer drogi krajowej do zbioru dróg krajowych ct.
 * Zwraca wartość true jeśli udało się zaalokować pamięć i dodać numer.
 * W przeciwnym przypadku zwraca false. */
bool addRoute(Citytree ct, unsigned n) {
    Setroutes setroutesBuffer = malloc(sizeof(struct Nodesetroutes));
    if (setroutesBuffer == NULL)
        return false;
    completeNodesetroutes(setroutesBuffer, n);
    ct->setroutes = insertSR(ct->setroutes, n, setroutesBuffer);
    return true;
}

/* Sprawdza czy ct zawiera w zbiorze dróg krajowych drogę o numerze n. */
bool checkRoute(Citytree ct, unsigned n) {
    return binSearchSR(ct->setroutes, n) != NULL;
}

/* Dodaje node do zbioru krawędzi ct oraz drogę o długości length
 * i rok budowy builtYear.
 * Zwraca wartość true jeśli udało się zaalokować pamięć i dodać node.
 * W przecinwym przypadku zwraca false. */
bool addEdge(Citytree ct, Citytree node, unsigned length, int builtYear) {
    Setedges setedgesBuffer = malloc(sizeof(struct Nodesetedges));
    if (setedgesBuffer == NULL)
        return false;
    Road road = malloc(sizeof(struct Way));
    if (road == NULL) {
        free(setedgesBuffer);
        return false;
    }
    completeRoad(road, length, builtYear);
    completeNodesetedges(setedgesBuffer, node, road);
    ct->setedges = insertSE(ct->setedges, node, setedgesBuffer);
    return true;
}

/* Sprawdza czy ct zawiera w zbiorze krawędzi wierzchołek node. */
bool checkEdge(Citytree ct, Citytree node) {
    return binSearchSE(ct->setedges, node) != NULL;
}

/* Jeśli ct jest połączone krawędzią z node zwraca wskaźnik na miejsce
 * w zbiorze krawędzi ct, w którym znajduje się node.
 * W przeciwnym przypadku zwraca NULL. */
Setedges getEdge(Citytree ct, Citytree node) {
    return binSearchSE(ct->setedges, node);
}

/* Sprawdza czy wskaźnik citytree jest różny od NULL i czy istnieje
 * pole z nazwą miasta. */
bool isCorrectCity(Citytree citytree) {
    return citytree != NULL && citytree->city != NULL;
}

/* Ustawia rok drogi łączącej city1 i city2 na repairYear o ile istnieją miasta
 * city1, city2, droga między tymi miastami i parametr year drogi jest mniejszy
 * lub równy repairYear.
 * Jeżeli wszystkie te warunki są spełnione zmienia parametr year na repairYear
 * i zwraca wartość true.
 * W przeciwnym przypadku zwraca wartość false. */
bool repairRoadBetweenCities(Citytree ct, const char *city1, const char *city2,
                             int repairYear) {
    Citytree citytree1 = isCity(ct, city1);
    Citytree citytree2 = isCity(ct, city2);
    if (!isCorrectCity(citytree1) || !isCorrectCity(citytree2))
        return false;
    Setedges setedges = getEdge(citytree1, citytree2);
    if (setedges == NULL)
        return false;
    if (setedges->road->year > repairYear)
        return false;
    if (setedges->road->year != repairYear) {
        setedges->road->year = repairYear;
        setedges = getEdge(citytree2, citytree1);
        setedges->road->year = repairYear;
    }
    return true;
}

bool isCorrectChar(char c) {
    if ((c >= 0 && c <= 31) || c == ';')
        return false;
    return true;
}

bool isCorrectNameCity(const char *city) {
    int n = (int) strlen(city);
    for (int i = 0; i < n; i++) {
        if (!isCorrectChar(city[i])) {
            return false;
        }
    }
    return true;
}
