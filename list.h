#ifndef DROGI_LIST_H
#define DROGI_LIST_H

#include <stdbool.h>
#include "structure.h"
#include "city_tree.h"

struct Listnode {
    Citytree citytree;
    List next;
};

/* Zwraca wskaźnik na nowo zaalokowany obiekt typu Listnode. */
List getNewListnode();

/* Uzupełnia listę list o parametry citytree i next. */
void completeList(List list, Citytree citytree, List next);

/* Uzupełnia listę list o parametr next. */
void completeListNext(List list, List next);

/* Zlicza liczbę cyfr liczby n. */
int countDigits(unsigned n);

/* Próbuje zaalokować pamięć na napis, który zawiera informację
 * o drodze krajowej.
 * Zwraca wynik tej próby. */
bool tryAllocate(char **description, int i, int *size);

/* Dodaje średnik na i-tą pozycje napisu description. */
void addSemicolon(char *description, int *i);

/* Uzupełnia napis description o nazwę miasta city.
 * Zwraca wynik tej próby. */
bool completeCityName(City city, char **description, int *i, int *size);

/* Uzupełnia napis description o długość drogi road.
 * Zwraca wynik tej próby. */
bool completeRoadLength(Road road, char **description, int *i, int *size);

/* Uzupełnia napis description o rok drogi road.
 * Zwraca wynik tej próby. */
bool completeRoadYear(Road road, char **description, int *i, int *size);

/* Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej
 * o numerze routeId, gdzie l jest początkiem listy opisującej tę drogę. */
char *getDescription(List l, unsigned routeId);

/* Odwraca listę, na którą wskazuje l oraz zwraca wskaźnik na jej początek. */
List reverseList(List l);

/* Usuwa listę wskazywaną przez l. */
void clearList(List l);

/* Zwraca koniec listy wskazywanej przez l. */
List getEnd(List l);

/* Wyświetl listę. */
void showList(List l);

/* Sprawdza czy miasta są obok siebie na liście. */
bool checkConnection(Citytree citytree1, Citytree citytree2, List l);

/* Zwraca List w momencie gdy napotka citytree1 lub citytree2. */
List getListCitytree(Citytree citytree1, Citytree citytree2, List l);

/* Porównuje dwie drogi, które są związane z listami i priorytetami.
 * Drogę z większym priorytetem dołączy do drogi mapListRoad.
 * Zwraca wartość true jeśli udało się połączyć drogi.
 * W przeciwnym przypadku zwraca wartość false. */
bool compareRoads(List firstRoad, struct Priority *first, List secondRoad,
                  struct Priority *second, List *mapListRoad);

#endif //DROGI_LIST_H
