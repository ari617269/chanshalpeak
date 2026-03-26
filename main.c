#include<stdio.h>
int main(int argc, char* argv[]){
 if(argc<2){
  printf("./main ./test.js");
  return 1;
 }
 printf("%d: %s",argc, argv[1]);
 return 0;
}
