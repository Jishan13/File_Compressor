#ifndef HUFFMAN
#define HUFFMAN
#include "node.h"
node * buildTree(node *t1,node *t2,int *ptr);
//void printHuffmanTree(node *t);
node *huffcodeToTree(node* huffmanTree,char* huffCode,char* str,int a);
#endif
