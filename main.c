#include<stdio.h>
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
 printf("%ld",size);
 fclose(f);
 return 0;
}
