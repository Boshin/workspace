#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

struct AvlNode {
    int     elem;
    AvlTree left;
    AvlTree right;
    int     height;
};

static int height(position p)
{
    if (NULL = p) {
        return -1;
    }
    else {
        return p->height;
    }
}

AvlTree makeEmpty(AvlTree t)
{
    if (NULL != t) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        free(t);
    }

    return NULL;
}

position find(int value, AvlTree t)
{
    if (NULL == t) {
        return NULL;
    }

    if (value < t->elem) {
        return find(value, t->left);
    }
    else if (value > t->elem) {
        return find(value, t->right);
    }
    else {
        return t;
    }
}

position findMin(AvlTree t)
{
    if (NULL == t)
        return NULL;

    if (NULL == t->left) {
        return t;
    }
    else {
        return findMin(t->left);
    }
}

position findMin_NonRecursive(AvlTree t)
{
    if (NULL != t) {
        while(NULL != t->left) {
            t = t->left;
        }
    }

    return t;
}

position findMax(AvlTree t)
{
    if (NULL == t)
        return NULL;

    if (NULL == t->left) {
        return t;
    }
    else {
        return findMax(t);
    }
}

position findMax_NonRecursive(AvlTree t)
{
    if (NULL != t) {
        while(NULL != t->right) {
            t = t->right;
        }
    }

    return t;
}

// This function can be called only if k2 has a left child.
// Perform a rotate between a node(k2) and its left child.
// Update heights, then return new root.
static position singleRotateWithLeft(position k2)
{
    position k1 = k2->left;

    k2->left = k1->right;
    k1->right = k2;
    
    k2->height = Max(height(k2->left), height(k2->right)) + 1;
    k1->height = Max(height(k1->left), height(k1->right)) + 1;

    return k1; // new root
}

// This function can be called only if k2 has a right child.
// Perform a rotate between a node(k2) and its right child.
// Update heights, then return new root.
static position singleRotateWithRight(position k2)
{
    position k1 = k2->right;
    
    k2->right = k1->left;
    k1->left = k2;

    k2->height = Max(height(k2->left), height(k2->right)) + 1;
    k1->height = Max(height(k1->left), height(k1->right)) + 1;

    return k1; // new root
}

// This function can be called only if k3 has a left child
// and K3's left child has a right child
// Do the left-rihgt double rotate. 
// Update height, then return new root
static position doubleRotateWithLeft(position k3)
{
    // position k1 = k3->left;
    // position k2 = k1->right;

    // k1->right = k2->left;
    // k3->left  = k2->right;

    // k2->left  = k1;
    // k2->right = k3;

    // k1->height = Max(height(k1->left), height(k1->right)) + 1;
    // k3->height = Max(height(k3->left), height(k3->right)) + 1;
    // k2->height = Max(height(k2->left), height(k2->right)) + 1;
    
    // Rotate between k1 and k2
    singleRotateWithRight(k3->left);

    // Rotate between k2 and k3
    return singleRotateWithleft(k3);

}

// This function can be called only if k3 has a right child
// and K3's right child has a left child
// Do the left-rihgt double rotate. 
// Update height, then return new root
static position doubleRotateWithRight(position k3)
{
    return NULL;
}

AvlTree insert(int value, AvlTree t)
{
    if (NULL == t) {
        t = malloc(sizeof(struct AvlNode));

        if (NULL == t) {
            printf("fatal error, out of memory\n");
            return t;
        }

        t->elem = value;
        t->left = t->right = NULL;
        t->height = 0;
    }
    else if (value < t->elem) {
         t = insert(value, t->left);

         if (height(t->left) - height(t->right) == 2) {
             if (value < t->left->elem) { // left-left
                 t = singleRotateWitLeft(t);
             }
             else { // left-right
                 t = doubleRotateWithLeft(t);
             }
         }
    }
    else if (value > t->elem) {
        t = insert(value, t->right);

        if (height(t->right - height(t->left)) == 2) {
            if (value > t->right->elem) { // right-right
                t = singleRotateWithRight(t);
            }
            else {  // right-left
                t = doubleRotateWithRight(t);
            }
        }
    }
    else {
        printf("value in the tree already\n");
    }

    t->height = Max(height(t->left), height(t->right)) + 1;
    return t;
}

AvlTree deleted(int value, AvlTree t)
{
    if (NULL == t) {
        printf("fatal error, value not found\n");
    }
    else if (value < t->elem) {
       t->left = deleted(value, t->left); 
    }
    else if (value > t->elem) {
        t->right = deleted(value, t->right);
    }
    // Found value to be deleted
    // have to children
    else if (t->left && t->right) {
        position tmp = findMin(t->right);
        t->elem = tmp->elem;
        t->right = deleted(tmp->elem, t->right);
    }
    // Found value to be deleted
    // have one or zero child
    else {
        position tmp = t;
        if (NULL == t->left) {
            t = t->right;
        }
        else if (NULL == t->right) {
            t = t->left;
        }
        else {
            // no child, just need delete this node
        }

        free(tmp);
    }

    return t;
}

int retrieve(position p)
{
    return 0;
}

int main(int argc, char *argv[])
{
    printf("AvlNode size:   %lu\n", sizeof(struct AvlNode));
    printf("AvlTree size: %lu\n", sizeof(AvlTree));

    return 0;
}
