#ifndef DROGI_SET_EDGES_H
#define DROGI_SET_EDGES_H

#include "structure.h"

struct Nodesetedges {
    Citytree key;
    Setedges left;
    Setedges right;
    int height;
    Road road;
};

/* Zwraca wskaźnik do nowo zaalokowanej struktury Nodesetedges. */
Setedges getNewNodeSE();

/* Uzupełnia dane zaalokowanej struktury wskazywanej przez setedges. */
void completeNodesetedges(Setedges setedges, Citytree key, Road road);

// A utility function to get maximum of two integers
int maxSE(int a, int b);

// A utility function to get heightSE of the tree
int heightSE(Setedges N);

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
Setedges rightRotateSE(Setedges y);

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
Setedges leftRotateSE(Setedges x);

// Get Balance factor of node N
int getBalanceSE(Setedges N);

Setedges insertSE(Setedges node, Citytree key, Setedges setedgesBuffer);

/* Given a non-empty binary search tree, return the
node with minimum key value found in that tree.
Note that the entire tree does not need to be
searched. */
Setedges minValueNodeSE(Setedges node);

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
Setedges deleteNodeSE(Setedges root, Citytree key);

/* Wyszukiwanie binarne w zbiorze krawędzi t. */
Setedges binSearchSE(Setedges t, Citytree key);

/* Próbuje dodać do zbioru setedges element z kluczem citytreeKey
 * Jeśli udało się przydzielić pamięć, dodaje element i zwraca true.
 * W przeciwnym przypadku zwraca false. */
bool addToSetedges(Setedges *setedges, Citytree citytreeKey, Road road);

/* Usuwa strukturę wskazywaną przez setedges. */
void removeSetedges(Setedges setedges);

void wypiszMiasta(Setedges setedges);

#endif //DROGI_SET_EDGES_H
