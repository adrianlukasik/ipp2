//
// Created by root on 29.04.19.
//

#ifndef DROGI_TREE_H
#define DROGI_TREE_H

#include "structure.h"

struct Node {
    char key;
    Tree left;
    Tree right;
    int height;
    Citytree citytree;
};

/* Uzupełnia dane zaalokowanej struktury wskazywanej przez t. */
void completeNode(Tree t, char key, Citytree citytreeBuffer);

// A utility function to get maximum of two integers
int max(int a, int b);

// A utility function to get height of the tree
int height(Tree N);

/* Helper function that allocates a new node with the given key and
	NULL left and right pointers. */
Tree newNode(char key);

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
Tree rightRotate(Tree y);

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
Tree leftRotate(Tree x);

// Get Balance factor of node N
int getBalance(Tree N);

Tree insert(Tree node, char key, Tree treeBuffer);

/* Given a non-empty binary search tree, return the
node with minimum key value found in that tree.
Note that the entire tree does not need to be
searched. */
Tree minValueNode(Tree node);

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
Tree deleteNode(Tree root, char key);

// A utility function to print preorder traversal of
// the tree.
// The function also prints height of every node
void preOrder(Tree root);

/* Wyszukiwanie binarne w drzewie t. */
Tree binSearch(Tree t, char key);

#endif //DROGI_TREE_H
