#include "set_routes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Zwraca wskaźnik do nowo zaalokowanego elementu typu Nodesetroutes. */
Setroutes getNewNodeSR() {
    return malloc(sizeof(struct Nodesetroutes));
}

/* Uzupełnia dane zaalokowanej struktury wskazywanej przez setroutes. */
void completeNodesetroutes(Setroutes setroutes, unsigned key) {
    setroutes->height = 1;
    setroutes->right = NULL;
    setroutes->left = NULL;
    setroutes->key = key;
}

// A utility function to get heightSR of the tree
int heightSR(struct Nodesetroutes *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum of two integers
int maxSR(int a, int b) {
    return (a > b) ? a : b;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Nodesetroutes *rightRotateSR(struct Nodesetroutes *y) {
    struct Nodesetroutes *x = y->left;
    struct Nodesetroutes *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = maxSR(heightSR(y->left), heightSR(y->right)) + 1;
    x->height = maxSR(heightSR(x->left), heightSR(x->right)) + 1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Nodesetroutes *leftRotateSR(struct Nodesetroutes *x) {
    struct Nodesetroutes *y = x->right;
    struct Nodesetroutes *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = maxSR(heightSR(x->left), heightSR(x->right)) + 1;
    y->height = maxSR(heightSR(y->left), heightSR(y->right)) + 1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int getBalanceSR(struct Nodesetroutes *N) {
    if (N == NULL)
        return 0;
    return heightSR(N->left) - heightSR(N->right);
}

struct Nodesetroutes *insertSR(struct Nodesetroutes *node, unsigned key,
                               Setroutes setroutesBuffer) {
    /* 1. Perform the normal BST rotation */
    if (node == NULL)
        return setroutesBuffer;

    if (key < node->key)
        node->left = insertSR(node->left, key, setroutesBuffer);
    else if (key > node->key)
        node->right = insertSR(node->right, key, setroutesBuffer);
    else // Equal keys not allowed
        return node;

    /* 2. Update heightSR of this ancestor node */
    node->height = 1 + maxSR(heightSR(node->left),
                             heightSR(node->right));

    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = getBalanceSR(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotateSR(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotateSR(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotateSR(node->left);
        return rightRotateSR(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotateSR(node->right);
        return leftRotateSR(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

/* Given a non-empty binary search tree, return the
node with minimum key value found in that tree.
Note that the entire tree does not need to be
searched. */
struct Nodesetroutes *minValueNodeSR(struct Nodesetroutes *node) {
    struct Nodesetroutes *current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
struct Nodesetroutes *deleteNodeSR(struct Nodesetroutes *root, unsigned key) {
    // STEP 1: PERFORM STANDARD BST DELETE

    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if (key < root->key)
        root->left = deleteNodeSR(root->left, key);

        // If the key to be deleted is greater than the
        // root's key, then it lies in right subtree
    else if (key > root->key)
        root->right = deleteNodeSR(root->right, key);

        // if key is same as root's key, then This is
        // the node to be deleted
    else {
        // node with only one child or no child
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Nodesetroutes *temp = root->left ? root->left :
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
            struct Nodesetroutes *temp = minValueNodeSR(root->right);

            // Copy the inorder successor's data to this node
            root->key = temp->key;

            // Delete the inorder successor
            root->right = deleteNodeSR(root->right, temp->key);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + maxSR(heightSR(root->left),
                             heightSR(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalanceSR(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalanceSR(root->left) >= 0)
        return rightRotateSR(root);

    // Left Right Case
    if (balance > 1 && getBalanceSR(root->left) < 0) {
        root->left = leftRotateSR(root->left);
        return rightRotateSR(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceSR(root->right) <= 0)
        return leftRotateSR(root);

    // Right Left Case
    if (balance < -1 && getBalanceSR(root->right) > 0) {
        root->right = rightRotateSR(root->right);
        return leftRotateSR(root);
    }

    return root;
}

// A utility function to print preorder traversal of
// the tree.
// The function also prints heightSR of every node
void preOrderSR(struct Nodesetroutes *root) {
    if (root != NULL) {
        printf("%d ", root->key);
        preOrderSR(root->left);
        preOrderSR(root->right);
    }
}

/* Wyszukiwanie binarne w zbiorze dróg krajowych t. */
Setroutes binSearchSR(Setroutes t, unsigned key) {
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

/* Usuwa strukturę wskazywaną przez setroutes. */
void removeSetroutes(Setroutes setroutes) {
    if (setroutes != NULL) {
        removeSetroutes(setroutes->left);
        removeSetroutes(setroutes->right);
        free(setroutes);
    }
}
