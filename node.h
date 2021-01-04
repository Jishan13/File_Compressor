#ifndef NODE
#define NODE
typedef struct node{
 char *str;
 char *huffcode;
 int freq;
 int isHuffRoot;
 struct node *next;
 struct node *left;
}node;
#endif
