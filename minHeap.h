#ifndef MIN_HEAP
#define MIN_HEAP
#include "node.h"
 int getParent(int k);
 int getRightChild(int k);
 int getLeftChild(int k);
 void swap(int c, int p, node **root);
 void shiftUp(int child, node **root);
 void shiftDown(int k, int *minCounter, node **root);
 node * removeMin(node **root,int *minCounter);
 void insert(node *tmp,int *minCounter,node **root);
 void printHeap(int *minCounter, node **root);

#endif
