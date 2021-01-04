#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include "minHeap.h"
#include "node.h"
#include "huffman.h"
#include "BST.h"
node *head = NULL;
node **root =NULL;
node *huffmanTree =NULL;
char *finalBuffer = NULL;
int minCounter = 0;
int totalNodes = 0;
  
int createFile(char flag, char *path){
 int fd;
 if(flag == 'b'){
  fd = open("HuffmanCodebook",O_RDWR | O_CREAT, 0600);
  if(fd==-1){
   perror("HuffmanCodebook");
   exit(1);
  }
 } else{
   fd = open(path,O_RDWR | O_CREAT,0600);
   if(fd==-1){
    perror(path);
    exit(1);
    }
  }
return fd;
}
void writeToFile(char *s, int fd){
  int sz;
  sz = write(fd,s,strlen(s));

}

void searchBST(node *RootPtr,char *s, int f){
  if(RootPtr== NULL)return;
  if(strcmp(s,RootPtr->str)==0){
   writeToFile(RootPtr->huffcode,f);
   return;
  }
  if(strcmp(s,RootPtr->str)<=0){
     searchBST(RootPtr->left,s,f);
  }
  if(strcmp(s,RootPtr->str)>0){
    searchBST(RootPtr->next,s,f);
  }
}

int replace(node * root, char *s, int f,int idx){
   int r;
   if(root==NULL){
    return -5;
    //return r;
   }
   if((root->left == NULL)&&(root->next==NULL)){
    writeToFile(root->str,f);
     return 0;
    //return 0;
   }
   if(s[idx]== '0'){
     return replace(root->left,s,f,idx+1);    
   }else if(s[idx]=='1'){

     return replace(root->next,s,f,idx+1);
   }else if(s[idx]=='\0'){

      return -1;
   }

}
void decompress(char *buff, int f){
 int i =0;
 int bitIndx =0;
 char *bitStr = (char*) malloc(strlen(buff)*sizeof(char));
 memset(bitStr,'\0',strlen(bitStr));
 while(buff[i]!='\0'){
   bitStr[bitIndx]=buff[i];
   int ret = replace(huffmanTree,bitStr,f,0);
    if(ret == 0){
       memset(bitStr,'\0',strlen(bitStr));
       bitIndx = 0;
    }
    else{
    bitIndx++;
    }
   i++;
 }

}
void llToMinHeap(){
 node *nxtNode = NULL;
 node *iterator = head;
 if(iterator!=NULL){
  int i =0;
  nxtNode= iterator->next;
  iterator->left=NULL;
  iterator->next=NULL;
  root[i]= iterator;
  iterator = nxtNode; 
 }
 while(iterator!=NULL){
    nxtNode = iterator->next;
    iterator->left=NULL;
    iterator->next=NULL;
  insert(iterator,&minCounter,root);
  iterator = nxtNode;
 }
 head=NULL;
}
void huffmanTreeCreation(){
  int treeNo =0;
  while(minCounter > 0){
    node *tmp1= removeMin(root,&minCounter);
    node *tmp2= removeMin(root,&minCounter);
    huffmanTree = buildTree(tmp1,tmp2,&treeNo);
    insert(huffmanTree, &minCounter,root);
    treeNo++;
  }
  huffmanTree= root[0];
  root[0]= NULL;
  if(huffmanTree==NULL)return;
  huffmanTree->isHuffRoot=1;
}
node * BSTRoot= NULL;
void makeBSTtokens(char *buff,int fd){
 int i;
 int size = 101;
 char *str = (char*)malloc(size*sizeof(char));
 int str_index=0;
 memset(str,'\0',size);
 for(i=0;i< strlen(buff);i++){
    if(isspace(buff[i])){
     str[str_index]=buff[i];
     i++;
    }
    else{
     while(isspace(buff[i])==0){
      if(str_index >= size){
        size*=2;
        str = (char*)realloc(str,size*(sizeof(char*)));
      }
       
        str[str_index]= buff[i];
        str_index++;
        i++;
     }
    }
   if(strcmp("",str)){
      searchBST(BSTRoot,str,fd);
     i--;
   }
   str_index=0;
   memset(str,'\0',size);
 }
}

void readFile(char *fname,char *buffer,int size,int *track,char flag){
  //printf("%s\n",fname);
  int fd = open(fname,O_RDONLY);
  int FileDes;
  if(fd<0) {
  printf("Error: file \"%s\" is not valid file\n",fname);
  exit(1);
  }
  if(flag=='c'){
   char filename[strlen(fname)+sizeof(".hcz")];
   memset(filename,'\0',sizeof(filename));
   memcpy(filename,fname,strlen(fname));
   strcat(filename,".hcz");
   FileDes = createFile(flag,filename);
 }else if(flag == 'd'){
   char filename[strlen(fname)];
   memset(filename,'\0',sizeof(filename));
   memcpy(filename,fname,strlen(fname)-4);
   FileDes = createFile(flag,filename);
 }
  int newBuff =0;
  int status =1;
  int readIn=*track;
  do{
    status = read(fd,buffer+readIn,size-readIn);
    if(status<0){
      printf("Error: file expected, you gave directory");
      //perror(fname);
     exit(1);
    }
     //buffer[status]='\0';
    readIn+=status;
    *track=readIn;
    if(*track >=size){
      newBuff =1;
      size = 2*size;
      buffer = (char*) realloc(buffer,size*(sizeof(char)));
      memset(buffer+readIn,'\0',size);  
    }
  }while(status >0 && readIn< size);
  if(flag=='c'){
   makeBSTtokens(buffer,FileDes);
   memset(buffer,'\0',strlen(buffer));
   *track=0;
  }
  if(flag == 'd'){
  decompress(buffer,FileDes); 
  memset(buffer,'\0',strlen(buffer));
  *track=0;
  }
  finalBuffer = buffer;
  close(fd);
}
void readDir(char *pathName,char * buffer, int size, int *t,int *access,char flag){
  DIR *dir;
  struct dirent *sd;
  struct stat buff;
  dir = opendir(pathName);
  if(dir == NULL){
    if(*access == 0){
     printf("Expected a file, you gave a directory");
     exit(1);
    }
    perror(pathName);
    exit(1);
  }else{
    while((sd=readdir(dir))!=NULL){
         (*access)=(*access)+1;
       if(strcmp(sd->d_name,".")==0 || strcmp(sd->d_name,"..")==0)
        continue;
       
           //int length = sizeof(pathName)+(sizeof(sd->d_name) +sizeof("/")+sizeof("\0"));
           char localname[20000];
           strcpy(localname,pathName);
           strcat(localname,"/");
           strcat(localname,sd->d_name);
           if(stat(localname,&buff)==0){
             if(S_ISDIR(buff.st_mode)>0){
               readDir(localname,buffer,size,t,access,flag);
             }
            if(S_ISREG(buff.st_mode)>0){
              if(flag=='c'){
               if(strstr(localname,".hcz")!=NULL){
                  continue;
               }
               else{
                 readFile(localname,buffer,size,t,flag);
               }
              }
              else if(flag == 'd'){
                if(strstr(localname,".hcz")==NULL){
                  continue;
                }else{
                 readFile(localname,buffer,size,t,flag);
               }
              }
              else if(flag == ' '){
                 readFile(localname,buffer,size,t,flag);
              } 
            }
          }else{
           perror(localname);
          }
       }
    }
  
  
  
 //close(dir);
}
void getHuffmanCode(node* root,char *str,int idx,int fd,char *esc){
  if(root==NULL)return;
  char *sp = (char*) malloc(strlen(str)+sizeof(char));
  memset(sp,'\0',strlen(sp));
  //sp[0]='\0';
  //sp[strlen(sp)-1]='\0';
  memcpy(sp,str,strlen(str));
   if((root->left) != NULL){
     if(idx < sizeof(sp)){
        sp[idx]='0';
     }else{
       sp = realloc(str,sizeof(str)*2);
       sp[idx]='0';
     }
     getHuffmanCode(root->left,sp,idx+1,fd,esc);    
   }
   if((root->next)!=NULL){
     if(idx < sizeof(sp)){
        sp[idx]='1';
     }else{
       sp = realloc(str,sizeof(str)*2);
       sp[idx]='1';
     }
     getHuffmanCode(root->next,sp,idx+1,fd,esc); 
  }
  if(((root->left)==NULL)&&((root->next)==NULL)){
     char toPrint[strlen(sp)+strlen(root->str)+strlen(esc)];
     memset(toPrint,'\0',sizeof(toPrint));
     memcpy(toPrint,sp,strlen(sp));
     strcat(toPrint,"\t");
     if(isspace(root->str[0])){
        if(root->str[0] == '\t'){
          strcat(esc,"t");
          strcat(toPrint,esc);
          strcat(toPrint,"\n");
          writeToFile(toPrint,fd);
          memset(toPrint,'\0',strlen(toPrint));
          memcpy(esc,"SriKrishnaGovindHareMurari",sizeof("SriKrishnaGovindHareMurari"));
        }
        else if(root->str[0] == '\n'){
          strcat(esc,"n");
          strcat(toPrint,esc);
          strcat(toPrint,"\n");
          writeToFile(toPrint,fd);
          memset(toPrint,'\0',strlen(toPrint));
         memcpy(esc,"SriKrishnaGovindHareMurari",sizeof("SriKrishnaGovindHareMurari"));
        }
        else {
          strcat(toPrint,root->str);
          strcat(toPrint,"\n");
          writeToFile(toPrint,fd);
          memset(toPrint,'\0',strlen(toPrint));
        }
     }
    else {
      //char toPrint[strlen(sp)+strlen(root->str)];
      //memcpy(toPrint,sp,sizeof(sp));
      //strcat(toPrint,"\t");
      strcat(toPrint,root->str);
      strcat(toPrint,"\n");
      writeToFile(toPrint,fd);
      memset(toPrint,'\0',strlen(toPrint));
     //printf("%s %s\n",sp,root->str);
    }
 }
}
void addToList(char *str){
   if(head==NULL){
      node *tmp = (node*) malloc(sizeof(node));
      tmp->str = (char*) malloc((strlen(str)) * sizeof(char));
      memset(tmp->str,'\0',strlen(tmp->str));
      memcpy(tmp->str,str,strlen(str));
      tmp->freq = 1;
      tmp->next =NULL;
      tmp->left=NULL;
      head = tmp;
      totalNodes++;
    return;
   }
   else{
     int found = 0;
     node *curr = head;
     node *prev = NULL;
     while(curr!=NULL){
       if(strcmp(curr->str,str)==0){
          curr->freq++;
          found = 1;
       }
      prev= curr;
      curr = curr->next;
     }
    if(found == 0){
     node *tmp = (node*) malloc(sizeof(node));
     tmp->str = (char*) malloc((strlen(str)) * sizeof(char));
     tmp->freq = 1;
     memset(tmp->str,'\0',strlen(tmp->str));
     memcpy(tmp->str,str,strlen(str));
     tmp->next = curr;
     prev->next = tmp;
     totalNodes++;
    }
  }
}
void makeTokens(char *buff){
 int i;
 int size = 101;
 char *str = (char*)malloc(size*sizeof(char));
 int str_index=0;
 memset(str,'\0',size);
 for(i=0;i< strlen(buff);i++){
    if(isspace(buff[i])){
     str[str_index]=buff[i];
     i++;
    }
    else{
     while(isspace(buff[i])==0){
      if(str_index >= size){
        size*=2;
        str = (char*)realloc(str,size*(sizeof(char*)));
      }
       
        str[str_index]= buff[i];
        str_index++;
        i++;
     }
    }
   if(strcmp("",str)){
      addToList(str);
     i--;
   }
   str_index=0;
   memset(str,'\0',size);
 }
}
void makeCtokens(char *c, char flag){
 int i=0;
 int size = 101;
 char *str = (char*)malloc(size*sizeof(char));
 char * huffCode = (char*)malloc(size*sizeof(char));
 int str_indx =0;
 int huffIdx=0;
 while(c[i]!='\n'){
   i++;
 }
 i++;
 while(i<strlen(c)){
  while(c[i]!='\t'){
   huffCode[huffIdx] = c[i];
   huffIdx++;i++;
  }
  if(c[i]=='\t')i++;
  while(c[i]!='\n'){
   str[str_indx] = c[i];
   str_indx++;i++;
  }
  if(strcmp(str,"SriKrishnaGovindHareMurarit")==0){
   memset(str,'\0',size);
   memcpy(str,"\t",strlen("\t"));
  }
  else if(strcmp(str,"SriKrishnaGovindHareMurarin")==0){
   memset(str,'\0',size);
   memcpy(str,"\n",strlen("\n")); 
  }
   if(flag=='c')
     BSTRoot = InsertBST(BSTRoot,huffCode,str);
  
   if(flag=='d'){
     huffmanTree = huffcodeToTree(huffmanTree,huffCode,str,0);
   }

   str_indx=0;
   huffIdx =0;
   memset(str,'\0',size);
   memset(huffCode,'\0',size);
  i++;
 }

}
void printLL(){
  node *tmp = head;
  while(tmp!=NULL){
    if(isspace(tmp->str[0])){
      if(strcmp(tmp->str," ")==0)
      printf("space %d\n",tmp->freq);
      else if(tmp ->str[0]=='\t')
       printf("tab %d\n",tmp->freq);
      else
       printf("newLine %d\n",tmp->freq);
    }
    else{
    printf("%s %d\n",tmp->str, tmp->freq);
    tmp= tmp->next;
    //counter++;
  }
 }
}

int main(int argc, char *argv[]){
 int size = 10000;
 int *tracker;
 *tracker = 0;
 int DirAccCounter = 0;
 char* buffer=(char*) malloc(size*(sizeof(char)));
 if(argc>5){
   printf("Error: Too many arguements,max 5 arguements allowed\n");
   return 0;
 }
 if(argc>2 && argc < 6){
  if(argc==5){
   if(strcmp(argv[1],"-R")!=0){
     printf("Expected first flag: -R\n Flag inputed: %s",argv[1]);
     return 0;
   }
   if(!((strcmp(argv[2],"-c")==0)||(strcmp(argv[2],"-d")==0))){
     printf("Expected second flag: -c or -d\n Flag inputed: %s",argv[2]);
     return 0;
   }
   char *filePath = (char*)malloc((2*strlen(argv[3]))*sizeof(char));
   memset(filePath,'\0',strlen(filePath));
   memcpy(filePath,argv[3],strlen(argv[3]));
   if(strcmp(argv[2],"-c")==0){
     char *codebook = (char*)malloc(size *sizeof(char));
     memset(codebook,'\0',size);
     readFile(argv[4],codebook,size,tracker,' ');
     codebook = finalBuffer;
     makeCtokens(codebook,'c');
     size=100000; *tracker = 0;
     readDir(filePath,buffer,size,tracker,&DirAccCounter,'c');
     //buffer = finalBuffer;

   }else if(strcmp(argv[2],"-d")==0){
        char *codebook = (char*)malloc(size *sizeof(char));
        memset(codebook,'\0',size);
        readFile(argv[4],codebook,size,tracker,' ');
        makeCtokens(codebook,'d');
        size=100000; *tracker = 0;
        readDir(filePath,buffer,size,tracker,&DirAccCounter,'d');
   }
  
  
 }
 if(argc==3){
   if(strcmp(argv[1],"-b")!=0){
     printf("Expected Flag: -b\n Flag inputed: %s",argv[1]);
     return 0;
   }
   readFile(argv[2],buffer,size,tracker,' ');
   makeTokens(buffer);
   root = (node**) malloc(totalNodes * sizeof(node*));
   llToMinHeap();
   huffmanTreeCreation();
   int f =createFile('b',"");
   char *escapeSeq = (char*)malloc(28*sizeof(char));
   memset(escapeSeq,'\0',strlen(escapeSeq));
   memcpy(escapeSeq,"SriKrishnaGovindHareMurari",sizeof("SriKrishnaGovindHareMurari"));
   writeToFile(escapeSeq,f);
   writeToFile("\n",f);
   getHuffmanCode(huffmanTree,"",0,f,escapeSeq);
 } if(argc == 4){
     if(strcmp(argv[1],"-R")==0){
        if(strcmp(argv[2],"-b")!=0){
         printf("Expected Flag: -b\n Flag inputed: %s",argv[1]);
         return 0;
        }
     //int size = strlen(argv[3]);
     char *filePath = (char*)malloc((2*size)*sizeof(char));
     memcpy(filePath,argv[3],strlen(argv[3]));
     readDir(filePath,buffer,size,tracker,&DirAccCounter,' ');
     buffer = finalBuffer;
     makeTokens(buffer);
     root = (node**) malloc(totalNodes * sizeof(node*));
     llToMinHeap();
     huffmanTreeCreation();
     int f =createFile('b',"");
     char *escapeSeq = (char*)malloc(28*sizeof(char));
     memset(escapeSeq,'\0',strlen(escapeSeq));
     memcpy(escapeSeq,"SriKrishnaGovindHareMurari",sizeof("SriKrishnaGovindHareMurari"));
     writeToFile(escapeSeq,f);
     writeToFile("\n",f);
     getHuffmanCode(huffmanTree,"",0,f,escapeSeq);
    }
    else{
       if(!((strcmp(argv[1],"-c")==0)||(strcmp(argv[1],"-d")==0))){
        printf("Expected Flag: -c or d\n Flag inputed: %s",argv[1]);
       }
       if(strcmp(argv[1],"-c")==0){
        char *codebook = (char*)malloc(size *sizeof(char));
        memset(codebook,'\0',size);
        //char *path = (char*)malloc((size)*sizeof(char));
        readFile(argv[3],codebook,size,tracker,' ');
       codebook=finalBuffer;
        makeCtokens(codebook,'c');
        char *filePath = (char*)malloc(strlen(argv[2])*sizeof(char)+ 5*sizeof(char));
        memset(filePath,'\0',strlen(filePath));
        memcpy(filePath,argv[2],strlen(argv[2]));
        strcat(filePath,".hcz");
         size =100000; *tracker =0;
        readFile(argv[2],buffer,size,tracker,' ');
        buffer = finalBuffer;
        int f = createFile('c',filePath);
        size = 100000; *tracker =0;
        makeBSTtokens(buffer,f);
        
      }
      else if(strcmp(argv[1],"-d")==0){
        char *codebook = (char*)malloc(size *sizeof(char));
        memset(codebook,'\0',size);
        readFile(argv[3],codebook,size,tracker,' ');
        makeCtokens(codebook,'d');
        if(strstr(argv[2],".hcz")==NULL){
         printf("Expected a .hcz to decompress");
         exit(1);   
        }
        *tracker =0; size =100000;
        readFile(argv[2],buffer,size,tracker,' ');
        buffer = finalBuffer;
        char *path = (char*)malloc(strlen(argv[2])*sizeof(char));
        memset(path,'\0',strlen(argv[2]));
        memcpy(path,argv[2],strlen(argv[2])-4);
        int i =strlen(argv[2]);
        int fdis = createFile('d',path);
        decompress(buffer,fdis);
      }      
     }

  }
 }

else{
 printf("Too less arguements, expected at least 3\n");
 return 0;
}

return 0;
}

