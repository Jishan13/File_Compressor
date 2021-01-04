#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include "node.h"
#include "huffman.h"
node *createNode(char *hCode,char *str){
 node *leaf = (node*) malloc(sizeof(node));
 leaf->str = (char*)malloc(strlen(str)*sizeof(char));
 memset(leaf->str,'\0',strlen(leaf->str));
 memcpy(leaf->str,str,strlen(str));
 leaf->next=NULL;
 leaf->left=NULL;

 return leaf;
}
node * buildTree(node *t1,node *t2,int *ptr){
   int size = 25;
   char snum[20];
   node *tmp = (node*)malloc(sizeof(node*));
   if(t1!= NULL && t2 != NULL){
    snprintf(snum,size,"%d",*ptr);
    tmp->freq = ((t1->freq)+(t2->freq));
    tmp->str = (char*)malloc(size * sizeof(char));
    strcat(tmp->str,"tree");
    strcat(tmp->str,snum);
    tmp->left=t1;
    tmp->next=t2;
   }
 return tmp;
}
node *huffcodeToTree(node* huffmanTree,char* huffCode,char* str,int a){
  if(huffCode[a]=='\0'){
     huffmanTree = createNode(huffCode,str);
  }else if(huffCode[a]=='0'){
      if(huffmanTree == NULL){
          huffmanTree = createNode(huffCode,"tree");
          //huffmanTree->left = huffcodeToTree(huffmanTree->left,huffCode,str,a+1);
      }
    
       huffmanTree->left=huffcodeToTree(huffmanTree->left,huffCode,str,a+1);
     
  }
  else if(huffCode[a]=='1'){
    if(huffmanTree == NULL){
      huffmanTree = createNode(huffCode,"tree");
    }
    huffmanTree->next= huffcodeToTree(huffmanTree->next,huffCode,str,a+1);
     //huhuffcodeToTree(huffmanTree->next,huffCode,str,a+1);
  }
 return huffmanTree;
}
 
/*void PreprintHuffmanTree(node *t){
 if(t==NULL)return;
if(isspace(t->str[0])){
  if(t->str[0] == '\t')
    printf(" \\t ");
  else if(t->str[0] == '\n')
     printf(" \\n ");
  else printf("space");
 }
 else printf("%s ",t->str);

 PreprintHuffmanTree(t->left);
 PreprintHuffmanTree(t->next);
 //printHuffmanTree(t->next);

}

void InprintHuffmanTree(node *t){
 if(t==NULL)return;
 InprintHuffmanTree(t->left);

if(isspace(t->str[0])){
  if(t->str[0] == '\t')
    printf(" \\t ");
  else if(t->str[0] == '\n')
     printf(" \\n ");
  else printf("space");
 }
 else printf("%s ",t->str);

 InprintHuffmanTree(t->next);
 //printHuffmanTree(t->next);

}*/
