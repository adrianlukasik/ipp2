#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "city_tree.h"
#include "set_edges.h"

#define MIN_LENGTH_DESCRIPTION 10

/* Zwraca wskaźnik na nowo zaalokowany obiekt typu Listnode. */
List getNewListnode() {
    return malloc(sizeof(struct Listnode));
}

/* Uzupełnia listę list o parametry citytree i next. */
void completeList(List list, Citytree citytree, List next) {
    list->next = next;
    list->citytree = citytree;
}

/* Uzupełnia listę list o parametr next. */
void completeListNext(List list, List next) {
    if (list != NULL)
        list->next = next;
}

/* Zlicza liczbę cyfr liczby n. */
int countDigits(unsigned n) {
    if (n == 0)
        return 1;
    int count = 0;
    while (n > 0) {
        n /= 10;
        count++;
    }
    return count;
}

/* Próbuje zaalokować pamięć na napis, który zawiera informację
 * o drodze krajowej.
 * Zwraca wynik tej próby. */
bool tryAllocate(char **description, int i, int *size) {
    while (i >= *size) {
        *size *= 2;
        *description = realloc(*description, *size * sizeof(char));
        if (*description == NULL)
            return false;
    }
    return true;
}

/* Dodaje średnik na i-tą pozycje napisu description. */
void addSemicolon(char *description, int *i) {
    description[(*i)++] = ';';
}

/* Uzupełnia napis description o nazwę miasta city.
 * Zwraca wynik tej próby. */
bool completeCityName(City city, char **description, int *i, int *size) {
    if (!tryAllocate(description, *i + city->length, size))
        return false;
    sprintf(*description + *i, "%s", city->name);
    *i += city->length;
    return true;
}

/* Uzupełnia napis description o długość drogi road.
 * Zwraca wynik tej próby. */
bool completeRoadLength(Road road, char **description, int *i, int *size) {
    int numberDigits = countDigits(road->length);
    if (!tryAllocate(description, *i + numberDigits, size))
        return false;
    sprintf(*description + *i, "%u", road->length);
    *i += numberDigits;
    return true;
}

/* Uzupełnia napis description o rok drogi road.
 * Zwraca wynik tej próby. */
bool completeRoadYear(Road road, char **description, int *i, int *size) {
    int numberDigits =
            road->year < 0 ? countDigits((unsigned) -road->year) + 1 :
            countDigits((unsigned) road->year);
    if (!tryAllocate(description, *i + numberDigits, size))
        return false;
    sprintf(*description + *i, "%d", road->year);
    *i += numberDigits;
    return true;
}

/* Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej
 * o numerze routeId, gdzie l jest początkiem listy opisującej tę drogę. */
char *getDescription(List l, unsigned routeId) {
    int size = MIN_LENGTH_DESCRIPTION;
    char *description = malloc(size * sizeof(char));
    if (description == NULL)
        goto error;
    int i = countDigits(routeId);
    sprintf(description, "%u", routeId);
    description[i++] = ';';
    while (l->next != NULL) {
        if (!completeCityName(l->citytree->city, &description, &i, &size))
            goto error;
        addSemicolon(description, &i);
        Road r = binSearchSE(l->citytree->setedges, l->next->citytree)->road;
        if (!completeRoadLength(r, &description, &i, &size))
            goto error;
        addSemicolon(description, &i);
        if (!completeRoadYear(r, &description, &i, &size))
            goto error;
        addSemicolon(description, &i);
        l = l->next;
    }
    if (!completeCityName(l->citytree->city, &description, &i, &size))
        goto error;
    description = realloc(description, (i + 1) * sizeof(char));
    return description;

    error:
    return NULL;
}

/* Odwraca listę, na którą wskazuje l oraz zwraca wskaźnik na jej początek. */
List reverseList(List l) {
    List prev = NULL, current = l, next;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}

/* Usuwa listę wskazywaną przez l. */
void clearList(List l) {
    List next;
    while (l != NULL) {
        next = l->next;
        free(l);
        l = next;
    }
}

/* Zwraca koniec listy wskazywanej przez l. */
List getEnd(List l) {
    if (l == NULL)
        return NULL;
    while (l->next != NULL) {
        l = l->next;
    }
    return l;
}

void showList(List l) {
    while (l != NULL) {
        printf("%s\n", l->citytree->city->name);
        l = l->next;
    }
}

/* Sprawdza czy miasta są obok siebie na liście. */
bool checkConnection(Citytree citytree1, Citytree citytree2, List l) {
    while (l != NULL) {
        if (l->citytree == citytree1 || l->citytree == citytree2)
            break;
        l = l->next;
    }
    if (l == NULL) {
        return false;
    } else if (l->citytree == citytree1) {
        if (l->next == NULL)
            return false;
        if (l->next->citytree == citytree2)
            return true;
        else
            return false;
    } else if (l->citytree == citytree2) {
        if (l->next == NULL)
            return false;
        if (l->next->citytree == citytree1)
            return true;
        else
            return false;
    } else {
        return false;
    }
}

/* Zwraca List w momencie gdy napotka citytree1 lub citytree2. */
List getListCitytree(Citytree citytree1, Citytree citytree2, List l) {
    while (l != NULL) {
        if (l->citytree == citytree1 || l->citytree == citytree2)
            return l;
        l = l->next;
    }
    return l;
}

/* Porównuje dwie drogi, które są związane z listami i priorytetami.
 * Drogę z większym priorytetem dołączy do drogi mapListRoad.
 * Zwraca wartość true jeśli udało się połączyć drogi.
 * W przeciwnym przypadku zwraca wartość false. */
bool compareRoads(List firstRoad, struct Priority *first, List secondRoad,
                  struct Priority *second, List *mapListRoad) {
    if (firstRoad == NULL && secondRoad == NULL) {
        return false;
    } else if (secondRoad == NULL) {
        List endFirstRoad = getEnd(firstRoad);
        endFirstRoad->next = (*mapListRoad)->next;
        free(*mapListRoad);
        *mapListRoad = firstRoad;
    } else if (firstRoad == NULL) {
        List endMainRoad = getEnd(*mapListRoad);
        endMainRoad->next = secondRoad->next;
        free(secondRoad);
    } else {
        if (comparePriority(first, second)) {
            List endFirstRoad = getEnd(firstRoad);
            endFirstRoad->next = (*mapListRoad)->next;
            free(*mapListRoad);
            *mapListRoad = firstRoad;
            clearList(secondRoad);
        } else {
            List endMainRoad = getEnd(*mapListRoad);
            endMainRoad->next = secondRoad->next;
            free(secondRoad);
            clearList(firstRoad);
        }
    }
    return true;
}
