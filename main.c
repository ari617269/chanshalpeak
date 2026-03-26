#include<stdio.h>
#include<stdlib.h>
int main(int argc, char* argv[]){
 if(argc<2){
  printf("./main ./test.js");
  return 1;
 }
 FILE* f=fopen(argv[1],"r");
 if(!f){
  printf("%s not found",argv[1]);
  return 2;
 }
 int seek=fseek(f,0,SEEK_END);
 if(seek!=0){
  printf("EOF seek fail");
  return 3;
 }
 long size=ftell(f);
 if(size==-1L){
  printf("fsize tell fail");
  return 4;
 }
 rewind(f);
 char* fs=malloc((size+1)*sizeof(char));
 if(fs==NULL){
  printf("malloc fail");
  return 5;
 }
 fs[size+1]='\0';
 size_t rc=fread(fs,sizeof(char),size,f);
 if(rc!=size){
  printf("fread fail");
  return 6;
 }
 printf("%ld",rc);
 printf("%s",fs);
 free(fs);
 fclose(f);
 return 0;
}
