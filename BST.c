#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include "node.h"
#include "BST.h"
node *buildNode(char *as, char *sa){
  node *Node = (node*)malloc(sizeof(node));
  Node->huffcode = (char*)malloc(strlen(as)*sizeof(char));
  memset(Node->huffcode,'\0',strlen(Node->huffcode));
  memcpy(Node->huffcode,as,strlen(as));
  Node->str =  (char*)malloc(strlen(sa)*sizeof(char));
  memset(Node->str,'\0',strlen(Node->str));
  memcpy(Node->str,sa,strlen(sa));
  return Node;
}

node *InsertBST(node * RootPtr,char *ap, char *sp){
   //printf("%s %s",ap,sp);
   if(RootPtr==NULL){
     RootPtr = buildNode(ap,sp);
   }
   else if((strcmp(sp,RootPtr->str)<=0)){
    RootPtr->left=InsertBST(RootPtr->left,ap,sp);
   }
   else{
     RootPtr->next=InsertBST(RootPtr->next,ap,sp);
   }
   return RootPtr;
}
/*void searchBST(node *RootPtr,char *s, int f){
  if(RootPtr== NULL)return;
  if(strcmp(RootPtr->str,s)==0){
   writeToFile(s,f);
   return;
  }
  if(strcmp(RootPtr->str,s)<=0){
     searchBST(RootPtr->left,s,f);
  }
  if(strcmp(RootPtr->str,s)>0){
    searchBST(RootPtr->left,s,f);
  }
}*/
  
