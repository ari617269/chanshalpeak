#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
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
 printf("%s",fs);
 fclose(f);
 const int toklen=51;
 char* tok=malloc(toklen*sizeof(char));
 if(tok==NULL){
  printf("tok malloc fail");
  return 7;
 }
 long pos=0;
 bool ts=false;
 while(pos<size+1){
  bool isNum = fs[pos]>=48&&fs[pos]<=57;
  bool isAlC = fs[pos]>=65&&fs[pos]<=90;
  bool isAlS = fs[pos]>=97&&fs[pos]<=122;
  bool isAlp = isAlC||isAlS;
  bool isAlNu = isAlp||isNum;
  bool isWs = fs[pos]==32||fs[pos]==9||fs[pos]==10;
  bool isQuo = fs[pos]==34;
  bool isPnc = fs[pos]==46||fs[pos]==40||fs[pos]==41||fs[pos]==59||isQuo;
  if(isAlNu)printf("A");
  if(isWs)printf("W");
  if(isPnc)printf("P");
  pos++;
 }
 free(fs);
 free(tok);
 return 0;
}
