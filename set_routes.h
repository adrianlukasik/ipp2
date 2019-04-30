//
// Created by root on 29.04.19.
//

#ifndef DROGI_SET_ROUTES_H
#define DROGI_SET_ROUTES_H

#include "structure.h"

struct Nodesetroutes {
    unsigned key;
    Setroutes left;
    Setroutes right;
    int height;
};

/* Zwraca wskaźnik do nowo zaalokowanego elementu typu Nodesetroutes. */
Setroutes getNewNodeSR();

/* Uzupełnia dane zaalokowanej struktury wskazywanej przez setroutes. */
void completeNodesetroutes(Setroutes setroutes, unsigned key);

// A utility function to get maximum of two integers
int maxSR(int a, int b);

// A utility function to get heightSR of the tree
int heightSR(Setroutes N);

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
Setroutes rightRotateSR(Setroutes y);

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
Setroutes leftRotateSR(Setroutes x);

// Get Balance factor of node N
int getBalanceSR(Setroutes N);

Setroutes insertSR(Setroutes node, unsigned key, Setroutes setroutesBuffer);

/* Given a non-empty binary search tree, return the
node with minimum key value found in that tree.
Note that the entire tree does not need to be
searched. */
Setroutes minValueNodeSR(Setroutes node);

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
Setroutes deleteNodeSR(Setroutes root, unsigned key);

// A utility function to print preorder traversal of
// the tree.
// The function also prints heightSR of every node
void preOrderSR(Setroutes root);

/* Wyszukiwanie binarne w zbiorze dróg krajowych t. */
Setroutes binSearchSR(Setroutes t, unsigned key);

/* Wrzuca do wektora zawartość zbioru dróg krajowych. */
void inOrder(Setroutes t, Vector v);

#endif //DROGI_SET_ROUTES_H
