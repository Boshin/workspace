#ifndef _AVLTREE_H

struct AvlNode;
typedef struct AvlNode* position;
typedef struct AvlNode* AvlTree;

AvlTree  makeEmpty(AvlTree t);
position find(int value, AvlTree t);
position findMin(AvlTree t);
position findMin_NonRecursive(AvlTree t);
position findMax(AvlTree t);
position findMax_NonRecursive(AvlTree t);
AvlTree  insert(int value, AvlTree t);
AvlTree  deleted(int value, AvlTree t);
int      retrieve();

#endif // _AVLTREE_H
