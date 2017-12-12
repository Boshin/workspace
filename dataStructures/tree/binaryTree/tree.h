#ifndef _TREE_H

struct treeNode;
typedef struct treeNode* position;
typedef struct treeNode* searchTree;

searchTree makeEmpty(searchTree t);
position   find(int value, searchTree t);
position   findMin(searchTree t);
position   findMin_NonRecursive(searchTree t);
position   findMax(searchTree t);
position   findMax_NonRecursive(searchTree t);
searchTree insert(int value, searchTree t);
searchTree deleted(int value, searchTree t);
int        retrieve();

#endif // _TREE_H
