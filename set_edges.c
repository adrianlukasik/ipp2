//
// Created by root on 29.04.19.
//

#include <stdlib.h>
#include <stdio.h>
#include "set_edges.h"

/* Zwraca wskaźnik do nowo zaalokowanej struktury Nodesetedges. */
Setedges getNewNodeSE() {
    Setedges setedges = (Setedges) malloc(sizeof(struct Nodesetedges));
    return setedges;
}

/* Uzupełnia dane zaalokowanej struktury wskazywanej przez setedges. */
void completeNodesetedges(Setedges setedges, Citytree key, Road road) {
    setedges->key = key;
    setedges->left = NULL;
    setedges->right = NULL;
    setedges->height = 1;
    setedges->road = road;
}

// A utility function to get heightSE of the tree
int heightSE(struct Nodesetedges *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum of two integers
int maxSE(int a, int b) {
    return (a > b) ? a : b;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Nodesetedges *rightRotateSE(struct Nodesetedges *y) {
    struct Nodesetedges *x = y->left;
    struct Nodesetedges *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = maxSE(heightSE(y->left), heightSE(y->right)) + 1;
    x->height = maxSE(heightSE(x->left), heightSE(x->right)) + 1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Nodesetedges *leftRotateSE(struct Nodesetedges *x) {
    struct Nodesetedges *y = x->right;
    struct Nodesetedges *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = maxSE(heightSE(x->left), heightSE(x->right)) + 1;
    y->height = maxSE(heightSE(y->left), heightSE(y->right)) + 1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int getBalanceSE(struct Nodesetedges *N) {
    if (N == NULL)
        return 0;
    return heightSE(N->left) - heightSE(N->right);
}

struct Nodesetedges *insertSE(struct Nodesetedges *node, Citytree key,
                              Setedges setedgesBuffer) {
    /* 1. Perform the normal BST rotation */
    if (node == NULL)
        return setedgesBuffer;

    if (key < node->key)
        node->left = insertSE(node->left, key, setedgesBuffer);
    else if (key > node->key)
        node->right = insertSE(node->right, key, setedgesBuffer);
    else // Equal keys not allowed
        return node;

    /* 2. Update heightSE of this ancestor node */
    node->height = 1 + maxSE(heightSE(node->left),
                             heightSE(node->right));

    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = getBalanceSE(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotateSE(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotateSE(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotateSE(node->left);
        return rightRotateSE(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotateSE(node->right);
        return leftRotateSE(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

/* Given a non-empty binary search tree, return the
node with minimum key value found in that tree.
Note that the entire tree does not need to be
searched. */
struct Nodesetedges *minValueNodeSE(struct Nodesetedges *node) {
    struct Nodesetedges *current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
struct Nodesetedges *deleteNodeSE(struct Nodesetedges *root, Citytree key) {
    // STEP 1: PERFORM STANDARD BST DELETE

    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if (key < root->key)
        root->left = deleteNodeSE(root->left, key);

        // If the key to be deleted is greater than the
        // root's key, then it lies in right subtree
    else if (key > root->key)
        root->right = deleteNodeSE(root->right, key);

        // if key is same as root's key, then This is
        // the node to be deleted
    else {
        // node with only one child or no child
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Nodesetedges *temp = root->left ? root->left :
                                        root->right;

            // No child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else // One child case
                *root = *temp; // Copy the contents of
            // the non-empty child
            free(temp);
        } else {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            struct Nodesetedges *temp = minValueNodeSE(root->right);

            // Copy the inorder successor's data to this node
            root->key = temp->key;

            // Delete the inorder successor
            root->right = deleteNodeSE(root->right, temp->key);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + maxSE(heightSE(root->left),
                             heightSE(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalanceSE(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalanceSE(root->left) >= 0)
        return rightRotateSE(root);

    // Left Right Case
    if (balance > 1 && getBalanceSE(root->left) < 0) {
        root->left = leftRotateSE(root->left);
        return rightRotateSE(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceSE(root->right) <= 0)
        return leftRotateSE(root);

    // Right Left Case
    if (balance < -1 && getBalanceSE(root->right) > 0) {
        root->right = rightRotateSE(root->right);
        return leftRotateSE(root);
    }

    return root;
}

/* Wyszukiwanie binarne w zbiorze krawędzi t. */
Setedges binSearchSE(Setedges t, Citytree key) {
    while (t != NULL) {
        if (key < t->key)
            t = t->left;
        else if (key > t->key)
            t = t->right;
        else
            return t;
    }
    return t;
}
