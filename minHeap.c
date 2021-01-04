#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include "node.h"
#include "minHeap.h"
  int getParent(int k){
    return ((k-1)/2);
  }
  int getRightChild(int k){
    return ((2*k)+2);
  }
  int getLeftChild(int k){
    return ((2*k)+1);
  }
  void swap(int c, int p, node **root){
   node *t = root[p];
   root[p] = root[c];
   root[c] = t; 
  }
  void shiftUp(int child,node **root){
     
     if(child<1)return;
     int parent = getParent(child);
     if((root[child]->freq) < (root[parent]->freq)){
       swap(parent,child,root);   
       shiftUp(parent,root);
     }
  } 

  void shiftDown(int k,int *minCounter,node **root){
   if((getLeftChild(k))> *minCounter)return;
   int smallestIdx =k;
   int lc = getLeftChild(k);
   int rc = getRightChild(k);
    if((lc <= *minCounter) && (rc <= *minCounter) && (root[lc]->freq < root[rc]->freq)){
      if(root[lc]->freq <= root[k]->freq){
	smallestIdx = lc;
      }
    }
    else if((rc <= *minCounter) && (lc <= *minCounter)&& (root[rc]->freq <= root[lc]->freq)){
      if(root[rc]->freq <= root[k]->freq){
	smallestIdx = rc;
      }
    }
    else if((rc > *minCounter)){
       if(root[k]->freq >= root[lc]->freq){
          smallestIdx = lc;
       }
    }
    if(smallestIdx==k)return; 
    swap(k,smallestIdx,root);
    shiftDown(smallestIdx,minCounter,root);
  }

node* removeMin(node **root, int *minCounter){
   node* tmp = root[0];
   root[0] = root[*minCounter];
   root[*minCounter]= NULL;
    *minCounter = (*minCounter) - 1;
   shiftDown(0,minCounter,root);
  return tmp;
}
void insert(node *tmp,int *minCounter,node **root){
   *minCounter = (*minCounter) + 1;
   root[*minCounter]= tmp;
   shiftUp(*minCounter,root);
}
void printHeap(int *minCounter,node **root){
 int i =0;
  if(root[i]==NULL)return;
 for(i=0;i<= (*minCounter);i++){
   if(strcmp(root[i]->str," ")==0){
    printf("space: %d\n",root[i]-> freq);
   }else if(strcmp(root[i]->str,"\t")==0){
    printf("\\t %d\n",root[i]-> freq);
   }else if(strcmp(root[i]->str,"\n")==0){
    printf("\\n %d\n",root[i]-> freq);
   }
   else
   printf("%s %d\n", root[i]->str,root[i]-> freq);
 }

}
