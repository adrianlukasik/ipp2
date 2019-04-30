//
// Created by root on 29.04.19.
//

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include "map.h"
#include "structure.h"
#include "list.h"
#include "city_tree.h"
#include "dijkstra.h"
#include "set_routes.h"
#include "set_edges.h"

struct Map {
    Citytree ct;
    List routes[1000];
};

bool isCorrectYear(int year) {
    return year != 0;
}

bool isCorrectLength(unsigned length) {
    return length > 0;
}

bool isCorrectRouteId(unsigned routeId) {
    return routeId > 0 && routeId < 1000;
}

/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych miast, odcinków dróg ani
 * dróg krajowych.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Map *newMap(void) {
    Map *map = malloc(sizeof(Map));
    if (map == NULL)
        return NULL;
    Citytree ct = newCitynode();
    if (ct == NULL) {
        free(map);
        return NULL;
    }
    map->ct = ct;
    for (int i = 1; i < 1000; i++) {
        map->routes[i] = NULL;
    }
    return map;
}

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p map.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] map        – wskaźnik na usuwaną strukturę.
 */
void deleteMap(Map *map) {
    if (map != NULL) {
        for (int i = 1; i < 1000; i++)
            clearList(map->routes[i]);
        removeCitytree(map->ct);
        free(map);
    }
}

/** @brief Dodaje do mapy odcinek drogi między dwoma różnymi miastami.
 * Jeśli któreś z podanych miast nie istnieje, to dodaje go do mapy, a następnie
 * dodaje do mapy odcinek drogi między tymi miastami.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość @p true, jeśli odcinek drogi został dodany.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, obie podane nazwy miast są identyczne, odcinek drogi między tymi
 * miastami już istnieje lub nie udało się zaalokować pamięci.
 */
bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
    if (!isCorrectLength(length) || !isCorrectYear(builtYear))
        return false;
    if (!isCorrectNameCity(city1) || !isCorrectNameCity(city2))
        return false;
    if (strcmp(city1, city2) == 0)
        return false;
    if (map == NULL)
        return false;
    return addRoadBetweenCities(map->ct, city1, city2, length, builtYear);
}

/** @brief Modyfikuje rok ostatniego remontu odcinka drogi.
 * Dla odcinka drogi między dwoma miastami zmienia rok jego ostatniego remontu
 * lub ustawia ten rok, jeśli odcinek nie był jeszcze remontowany.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wartość @p true, jeśli modyfikacja się powiodła.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, któreś z podanych miast nie istnieje, nie ma odcinka drogi między
 * podanymi miastami, podany rok jest wcześniejszy niż zapisany dla tego odcinka
 * drogi rok budowy lub ostatniego remontu.
 */
bool
repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
    if (!isCorrectYear(repairYear))
        return false;
    if (!isCorrectNameCity(city1) || !isCorrectNameCity(city2))
        return false;
    if (strcmp(city1, city2) == 0)
        return false;
    if (map == NULL)
        return false;
    return repairRoadBetweenCities(map->ct, city1, city2, repairYear);
}

/** @brief Łączy dwa różne miasta drogą krajową.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga krajowa o podanym numerze, któreś z podanych
 * miast nie istnieje, obie podane nazwy miast są identyczne, nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami lub nie udało
 * się zaalokować pamięci.
 */
bool newRoute(Map *map, unsigned routeId,
              const char *city1, const char *city2) {
    if (!isCorrectRouteId(routeId))
        return false;
    if (!isCorrectNameCity(city1) || !isCorrectNameCity(city2))
        return false;
    if (strcmp(city1, city2) == 0)
        return false;
    if (map == NULL)
        return false;
    if (map->routes[routeId] != NULL)
        return false;
    Citytree citytree1 = isCity(map->ct, city1);
    Citytree citytree2 = isCity(map->ct, city2);
    if (!isCorrectCity(citytree1) || !isCorrectCity(citytree2))
        return false;
    map->routes[routeId] = dijkstraAlgorithm(citytree1, citytree2, routeId,
                                             NULL);
    if (map->routes[routeId] == NULL)
        return false;
    map->routes[routeId] = reverseList(map->routes[routeId]);
    addRouteIdToSetroutes(map->routes[routeId], routeId);

    return true;
}

/** @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudowany lub remontowany i wybiera
 * wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city       – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * nazwę, nie istnieje droga krajowa o podanym numerze, nie ma miasta o podanej
 * nazwie, przez podane miasto już przechodzi droga krajowa o podanym numerze,
 * podana droga krajowa kończy się w podanym mieście, nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool extendRoute(Map *map, unsigned routeId, const char *city) {
    if (!isCorrectRouteId(routeId))
        return false;
    if (!isCorrectNameCity(city))
        return false;
    if (map == NULL)
        return false;
    if (map->routes[routeId] == NULL)
        return false;
    Citytree citytree = isCity(map->ct, city);
    if (!isCorrectCity(citytree))
        return false;
    if (checkRoute(citytree, routeId))
        return false;

    Citytree beginCity = getBeginOnList(map->routes[routeId]);
    Citytree endCity = getLastOnList(map->routes[routeId]);

    struct Priority *first, *second;
    first = malloc(sizeof(struct Priority));
    second = malloc(sizeof(struct Priority));

    beginCity->setroutes = deleteNodeSR(beginCity->setroutes, routeId);
    List firstRoad = dijkstraAlgorithm(beginCity, citytree, routeId, first);

    Setroutes setroutesBuffer = getNewNodeSR();
    completeNodesetroutes(setroutesBuffer, routeId);
    beginCity->setroutes = insertSR(beginCity->setroutes, routeId,
                                    setroutesBuffer);

    endCity->setroutes = deleteNodeSR(endCity->setroutes, routeId);
    List secondRoad = dijkstraAlgorithm(citytree, endCity, routeId, second);

    setroutesBuffer = getNewNodeSR();
    completeNodesetroutes(setroutesBuffer, routeId);
    endCity->setroutes = insertSR(endCity->setroutes, routeId, setroutesBuffer);

    if (firstRoad == NULL && secondRoad == NULL) {
        return false;
    } else if (secondRoad == NULL) {
        List endFirstRoad = getEnd(firstRoad);
        endFirstRoad->next = map->routes[routeId]->next;
        free(map->routes[routeId]);
        map->routes[routeId] = firstRoad;
    } else if (firstRoad == NULL) {
        List endMainRoad = getEnd(map->routes[routeId]);
        endMainRoad->next = secondRoad->next;
        free(secondRoad);
    } else {
        if (comparePriority(first, second)) {
            List endFirstRoad = getEnd(firstRoad);
            endFirstRoad->next = map->routes[routeId]->next;
            free(map->routes[routeId]);
            map->routes[routeId] = firstRoad;
        } else {
            List endMainRoad = getEnd(map->routes[routeId]);
            endMainRoad->next = secondRoad->next;
            free(secondRoad);
        }
    }

    addRouteIdToSetroutes(map->routes[routeId], routeId);

    return true;
}

/** @brief Usuwa odcinek drogi między dwoma różnymi miastami.
 * Usuwa odcinek drogi między dwoma miastami. Jeśli usunięcie tego odcinka drogi
 * powoduje przerwanie ciągu jakiejś drogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli odcinek drogi został usunięty.
 * Wartość @p false, jeśli z powodu błędu nie można usunąć tego odcinka drogi:
 * któryś z parametrów ma niepoprawną wartość, nie ma któregoś z podanych miast,
 * nie istnieje droga między podanymi miastami, nie da się jednoznacznie
 * uzupełnić przerwanego ciągu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool removeRoad(Map *map, const char *city1, const char *city2) {
    if (!isCorrectNameCity(city1) || !isCorrectNameCity(city1))
        return false;
    if (map == NULL)
        return false;
    Citytree citytree1 = isCity(map->ct, city1);
    Citytree citytree2 = isCity(map->ct, city2);
    if (!isCorrectCity(citytree1) || !isCorrectCity(citytree2))
        return false;
    if (!checkEdge(citytree1, citytree2))
        return false;
    Vector v1 = getNewVector();
    Vector v2 = getNewVector();
    Vector v3 = getNewVector();
    inOrder(citytree1->setroutes, v1);
    inOrder(citytree2->setroutes, v2);
    citytree1->setedges = deleteNodeSE(citytree1->setedges, citytree2);
    citytree2->setedges = deleteNodeSE(citytree2->setedges, citytree1);
    int i = 0, j = 0;
    Road r = binSearchSE(citytree1->setedges, citytree2)->road;
    while (i < v1->size && j < v2->size) {
        if (v1->arr[i] == v2->arr[j]) {
            if (checkConnection(citytree1, citytree2, map->routes[v1->arr[i]])) {
                pushVector(v3, v1->arr[i]);

                citytree1->setroutes = deleteNodeSR(citytree1->setroutes, v1->arr[i]);
                citytree2->setroutes = deleteNodeSR(citytree2->setroutes, v1->arr[i]);

                List l = dijkstraAlgorithm(citytree1, citytree2, v1->arr[i], NULL);

                Setroutes setroutesBuffer1 = getNewNodeSR();
                completeNodesetroutes(setroutesBuffer1, v1->arr[i]);
                citytree1->setroutes = insertSR(citytree1->setroutes, v1->arr[i], setroutesBuffer1);

                Setroutes setroutesBuffer2 = getNewNodeSR();
                completeNodesetroutes(setroutesBuffer2, v1->arr[i]);
                citytree2->setroutes = insertSR(citytree2->setroutes, v1->arr[i], setroutesBuffer2);

                if (l == NULL) {
                    Setedges setedgesBuffer1 = getNewNodeSE();
                    completeNodesetedges(setedgesBuffer1, citytree2, r);
                    citytree1->setedges = insertSE(citytree1->setedges, citytree2, setedgesBuffer1);

                    Setedges setedgesBuffer2 = getNewNodeSE();
                    completeNodesetedges(setedgesBuffer2, citytree1, r);
                    citytree2->setedges = insertSE(citytree2->setedges, citytree1, setedgesBuffer2);

                    clearList(l);
                    clearVector(v1);
                    clearVector(v2);
                    clearVector(v3);
                    return false;
                }
            }
            i++;
            j++;
        } else if (v1->arr[i] > v2->arr[j]) {
            j++;
        } else {
            i++;
        }
    }

    for (int i = 0; i < v3->size; i++) {
        citytree1->setroutes = deleteNodeSR(citytree1->setroutes, v3->arr[i]);
        citytree2->setroutes = deleteNodeSR(citytree2->setroutes, v3->arr[i]);

        List order = getListCitytree(citytree1, citytree2, map->routes[v3->arr[i]]);
        List list;
        if (order->citytree == citytree1)
            list = dijkstraAlgorithm(citytree2, citytree1, v3->arr[i], NULL);
        else
            list = dijkstraAlgorithm(citytree1, citytree2, v3->arr[i], NULL);
        List l = dijkstraAlgorithm(citytree2, citytree1, v3->arr[i], NULL);
        order->next = l->next;
        List tmp = l->next;
        free(l);
        l = getEnd(tmp);
        l->next = order->next->next;
        free(order->next);
        addRouteIdToSetroutes(map->routes[v3->arr[i]], v3->arr[i]);
    }

    clearVector(v1);
    clearVector(v2);
    clearVector(v3);
    return true;
}

/** @brief Udostępnia informacje o drodze krajowej.
 * Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamięć na ten napis. Zwraca pusty napis, jeśli nie istnieje droga krajowa
 * o podanym numerze. Zaalokowaną pamięć trzeba zwolnić za pomocą funkcji free.
 * Informacje wypisywane są w formacie:
 * numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;…;nazwa miasta.
 * Kolejność miast na liście jest taka, aby miasta @p city1 i @p city2, podane
 * w wywołaniu funkcji @ref newRoute, które utworzyło tę drogę krajową, zostały
 * wypisane w tej kolejności.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej.
 * @return Wskaźnik na napis lub NULL, gdy nie udało się zaalokować pamięci.
 */
char const *getRouteDescription(Map *map, unsigned routeId) {
    if (!isCorrectRouteId(routeId) || map == NULL ||
        map->routes[routeId] == NULL) {
        char *string = malloc(sizeof(char));
        if (string == NULL)
            return NULL;
        string[0] = '\0';
        return string;
    }
    return getDescription(map->routes[routeId], routeId);
}