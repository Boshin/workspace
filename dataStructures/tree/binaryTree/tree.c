#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

struct treeNode {
    int        elem;
    searchTree left;
    searchTree right;
};

searchTree makeEmpty(searchTree t)
{
    if (NULL != t) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        free(t);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    printf("treeNode size:   %lu\n", sizeof(struct treeNode));
    printf("searchTree size: %lu\n", sizeof(searchTree));

    return 0;
}

position find(int value, searchTree t)
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

position findMin(searchTree t)
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

position findMin_NonRecursive(searchTree t)
{
    if (NULL != t) {
        while(NULL != t->left) {
            t = t->left;
        }
    }

    return t;
}

position findMax(searchTree t)
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

position findMax_NonRecursive(searchTree t)
{
    if (NULL != t) {
        while(NULL != t->right) {
            t = t->right;
        }
    }

    return t;
}
searchTree insert(int value, searchTree t)
{
    if (NULL == t) {
        t = malloc(sizeof(struct treeNode));

        if (NULL == t) {
            printf("fatal error, out of memory\n");
            return t;
        }

        t->elem = value;
        t->left = t->right = NULL;
    }
    else if (value < t->elem) {
        insert(value, t->left);
    }
    else if (value > t->elem) {
        insert(value, t->right);
    }
    else {
        printf("value in the tree already\n");
    }

    return t;
}

searchTree deleted(int value, searchTree t)
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
