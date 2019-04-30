//
// Created by root on 29.04.19.
//

#ifndef DROGI_CITY_TREE_H
#define DROGI_CITY_TREE_H

#include <stdbool.h>
#include "structure.h"
#include "list.h"

struct Citynode {
    Tree root;
    Setroutes setroutes;
    Setedges setedges;
    City city;
};

/* Uzupełnia dane zaalokowanej struktury wskazywanej przez ct. */
void completeCitynode(Citytree ct);

/* Alokuje nowego Citynode i zwraca wskaźnik do niego. */
Citytree newCitynode();

/* Zwraca wskaźnik na miasto typu Citytree,
 * jeśli znajduje się w drzewie wskazywanym przez ct.
 * W przeciwnym przypadku zwraca NULL. */
Citytree isCity(Citytree ct, const char *city);

/* Zwraca wskaźnik na strukturę związaną z najdłuższym prefixem nazwy
 * miasta city. */
Citytree getLastPrefix(Citytree ct, const char *city, int *i, int n);

struct Buffer {
    int index;
    Citytree ct;
    int size;
    Citytree *bufferCitytree;
    Tree *bufferTree;
    City city;
    Road road;
    Setedges setedges;
};

/* Sprawdza czy istnieje miasto w strukturze. */
bool isExistCity(struct Buffer *buffer);

/* Sprawdza czy istnieją miasta związane z buforami i następnie weryfikuje
 * czy istnieje między nimi droga. */
bool isConnection(struct Buffer *buffer1, struct Buffer *buffer2);

/* Sprawdza czy istnieje droga między miastami citytree1 i citytree2. */
bool isRoad(Citytree citytree1, Citytree citytree2);

/* Dodaje miasto city do drzewa ct.
 * W przypadku gdy nie udało się zaalokować potrzebnej pamięci do utworzenia
 * nowego miasta zwraca wartość false.
 * W przeciwnym przypadku zwraca wartość true. */
//bool addCity(Citytree ct, const char *city);

/* Zwraca wskaźnik do struktury przechowującej dane o mieście.
 * Długość napisu reprezentującego miasto wynosi n. */
City getNewCity(int n);

/* Uzupełnia w strukturze wskazywanej przez city nazwę miasta. */
void completeCity(City city, const char *name);

/* Kasuje przydzieloną pamięć do miasta city. */
void removeCity(City city);

/* Kasuje przydzieloną pamieć do bufora bufferCitytree. */
void removeBufferCitytree(Citytree **bufferCitytree, int size);

/* Kasuje przydzieloną pamięć do bufora bufferTree. */
void removeBufferTree(Tree **bufferTree, int size);

/* Zwalnia pamięć przydzieloną buforowi potrzebną do utworzenia
 * miasta i drogi. */
void removeBuffer(struct Buffer *buffer);

/* Uzupełnia strukturę o elementy związane z dodaniem miast. */
void assignBufferOnCity(struct Buffer *buffer, const char *city);

/* Przydziela z buforu pamięć na połączenie między miastami.
 * Dodaje do struktur odpowiednie informacje związane z drogą między
 * miastami. */
void assignBufferOnRoad(struct Buffer *buffer1, struct Buffer *buffer2,
                        const char *city, unsigned length, int builtYear);

/* Próbuje zaalokować pamięć na utworzenie nowego miasta.
 * Jeśli udało się przydzielić wystarczającą pamięć zwraca true.
 * W przeciwnym przypadku zwalnia pamięć, która była potrzebna, ale
 * nie wystarczyła do utworzenia nowego miasta i zwraca false. */
bool tryAllocNewCityBuffer(struct Buffer *buffer);

/* Uzupełnia bufor o zmienne przydatne do alokacji pamięci. */
void completeBuffer(struct Buffer *buffer, Citytree ct, const char *city);

/* Czyści bufor, który prawidłowo zaalokował pamięć na miasto i drogę. */
void clearBuffer(struct Buffer *buffer);

/* Wyświetla miasta znajdujące się w drzewie ct. */
void showCities(Citytree ct, char *pom, int index);

/* Funkcja pomocnicza funkcji showCities.
 * Wywołuje funkcje showCities dla elementów drzewa t. */
void showCharCity(Tree t, char *pom, int index);

/* Usuwa strukturę wskazywaną przez ct. */
void removeCitytree(Citytree ct);

/* Usuwa strukturę wskazywaną przez t. */
void removeTree(Tree t);

/* Usuwa strukturę wskazywaną przez setroutes. */
void removeSetroutes(Setroutes setroutes);

/* Usuwa strukturę wskazywaną przez setedges. */
void removeSetedges(Setedges setedges);

/* Dodaje numer drogi krajowej do zbioru dróg krajowych ct.
 * Zwraca wartość true jeśli udało się zaalokować pamięć i dodać numer.
 * W przeciwnym przypadku zwraca false. */
bool addRoute(Citytree ct, unsigned n);

/* Sprawdza czy ct zawiera w zbiorze dróg krajowych drogę o numerze n. */
bool checkRoute(Citytree ct, unsigned n);

/* Dodaje node do zbioru krawędzi ct oraz drogę o długości length
 * i rok budowy builtYear.
 * Zwraca wartość true jeśli udało się zaalokować pamięć i dodać node.
 * W przecinwym przypadku zwraca false. */
bool addEdge(Citytree ct, Citytree node, unsigned length, int builtYear);

/* Sprawdza czy ct zawiera w zbiorze krawędzi wierzchołek node. */
bool checkEdge(Citytree ct, Citytree node);

/* Jeśli ct jest połączone krawędzią z node zwraca wskaźnik na miejsce
 * w zbiorze krawędzi ct, w którym znajduje się node.
 * W przeciwnym przypadku zwraca NULL. */
Setedges getEdge(Citytree ct, Citytree node);

/* Sprawdza czy wskaźnik citytree jest różny od NULL i czy istnieje
 * pole z nazwą miasta. */
bool isCorrectCity(Citytree citytree);

/* Jeśli nie ma miast city1 lub city2 to alokuje odpowiednią wielkość
 * pamięci na nie i dodaje je do drzewa ct. Następnie alokuje pamięć na drogę
 * o parametrach length i builtYear.
 * Zwraca wartość true jeśli udało się zaalokować potrzebną pamięć na miasta
 * city1, city2 oraz drogę między tymi miastami następnie dodaje wszystko do ct.
 * W przeciwnym przypadku nie narusza struktury drzewa ct i zwraca wartość
 * false. */
bool addRoadBetweenCities(Citytree ct, const char *city1, const char *city2,
                          unsigned length, int builtYear);

/* Ustawia rok drogi łączącej city1 i city2 na repairYear o ile istnieją miasta
 * city1, city2, droga między tymi miastami i parametr year drogi jest mniejszy
 * lub równy repairYear.
 * Jeżeli wszystkie te warunki są spełnione zmienia parametr year na repairYear
 * i zwraca wartość true.
 * W przeciwnym przypadku zwraca wartość false. */
bool repairRoadBetweenCities(Citytree ct, const char *city1, const char *city2,
                             int repairYear);


bool isCorrectNameCity(const char *city);

#endif //DROGI_CITY_TREE_H
