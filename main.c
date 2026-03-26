#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
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
 fclose(f);
 enum TType{
   TT_ID,TT_DOT,TT_LPAR,TT_QT,TT_STR,TT_RPAR,TT_SEM
 };
 struct Token{
  int index;
  int l;
  int c;
  enum TType type;
  char* lex;
 };
 struct TList {
  struct Token* arr;
  int len;
 } *tokL;
 tokL = malloc(sizeof(struct TList));
 if(tokL==NULL){
  printf("TokenList malloc fail");
  return 8;
 }
 tokL->arr = malloc(10*sizeof(struct Token));
 if(tokL->arr==NULL){
  printf("TokenArray malloc fail");
  return 9;
 }
 tokL->len = 10;
 long pos=0;
 bool ts=false;
 long start=-1;
 int l=0,c=0,i=0,s=0,t=0;
 while(pos<size+1){
  bool isNum = fs[pos]>=48&&fs[pos]<=57;
  bool isAlC = fs[pos]>=65&&fs[pos]<=90;
  bool isAlS = fs[pos]>=97&&fs[pos]<=122;
  bool isAlp = isAlC||isAlS;
  bool isAlNu = isAlp||isNum;
  bool isNL = fs[pos]==10;
  bool isWs = fs[pos]==0||fs[pos]==32||fs[pos]==9||isNL;
  bool isQuo = fs[pos]==34;
  bool isPnc = fs[pos]==46||fs[pos]==40||fs[pos]==41||fs[pos]==59||isQuo;
  if(i>=tokL->len){
   tokL->arr = realloc(tokL->arr, 2*tokL->len*sizeof(struct Token));
   if(tokL->arr==NULL){
    printf("TokenArray Re malloc fail");
    return 10;
   }
   tokL->len = 2*tokL->len;
  }
  if(isAlNu){
   if(ts==false){
    ts=true;
    start=pos;
   }
  }else{
   if(ts==true){
    ts=false;
    s=pos-start+1;
    char* tok=malloc(s*sizeof(char));
    if(tok==NULL){
     printf("tok malloc fail");
     return 7;
    }
    memcpy(tok,fs+start,s);
    tok[s-1]='\0';
    tokL->arr[i].index=i;
    tokL->arr[i].l=l;
    tokL->arr[i].c=start;
    tokL->arr[i].type=TT_ID;
    tokL->arr[i].lex=tok;
    i++;
   }
   if (!isWs){
    if(fs[pos]==46){
     t=TT_DOT;
    }else if(fs[pos]==40){
     t=TT_LPAR;
    }else if(fs[pos]==34){
     t=TT_QT;
    }else if(fs[pos]==41){
     t=TT_RPAR;
    }else if(fs[pos]==59){
     t=TT_SEM;
    }
    tokL->arr[i].index=i;
    tokL->arr[i].l=l;
    tokL->arr[i].c=pos;
    tokL->arr[i].type=t;
    char* tok1=malloc(2*sizeof(char));
     if(tok1==NULL){
      printf("tok malloc fail");
      return 7;
     }
     tok1[0]=fs[pos];
     tok1[1]='\0';
    tokL->arr[i].lex=tok1;
    i++;
   }
  }
  if(isNL){
   l++;
   c=0;
  }
  pos++;
  c++;
 }
 free(fs);
 fs=NULL;
 constexpr int maxargs=10;  
 struct CallExp{
  struct Callee {
   struct Token object;
   struct Token property;
  } callee;
  struct Arguments {
    int p;
    struct Token arg;
  } arguments[maxargs];
  int l;
 };
 struct Stat{
  enum STType{ STT_NOOP, STT_CALL } type;
  union {
   struct CallExp cExp;
  };
 };
 struct Prog{
  struct Stat* st;
  int l;
 } *pr;
 pr=malloc(sizeof(struct Prog));
 if(pr==NULL){
  printf("program malloc fail");
  return 11;
 }
 pr->st=malloc(2*sizeof(struct Stat));
 if(pr->st==NULL){
  printf("statement list malloc fail");
  return 12;
 }
 pr->l=2;
 int tl = i,j=0;
 i=0,start=0;
 while(j<tl){
  if(i>=pr->l){
    pr->st=realloc(pr->st,2*pr->l*sizeof(struct Stat));
    if(pr->st==NULL){
     printf("statement list realloc fail");
     return 12;
    }
    pr->l=2*pr->l;
  }
  if(tokL->arr[j].type==TT_SEM){
   const int callPttr[] = {TT_ID,TT_DOT,TT_ID,TT_LPAR,TT_QT,TT_ID,TT_QT,TT_RPAR,TT_SEM};
   int k=start;
   bool patter=true;
   while(k<j){
    if(tokL->arr[k].type!=callPttr[k]){
      patter=false;
      break;
    }
    k++;
   }
   if(patter==false){
    printf("invalid call exp pattern from l%d:c%d to l%d:c%d",tokL->arr[start].l,tokL->arr[start].c,tokL->arr[j-1].l,tokL->arr[j-1].c);
    return 13;
   }else{
    pr->st[i].type=STT_CALL;
    memcpy(&(pr->st[i].cExp.callee.object), &(tokL->arr[start]), sizeof(struct Token));
    memcpy(&(pr->st[i].cExp.callee.property), &(tokL->arr[start+2]), sizeof(struct Token));
    memcpy(&(pr->st[i].cExp.arguments[0].arg), &(tokL->arr[start+5]), sizeof(struct Token));
    pr->st[i].cExp.arguments[0].p=0;
    pr->st[i].cExp.l=1;
   }
   i++;
   start=i;
  }
  j++;
 }
 free(tokL->arr);
 free(tokL);
 auto ee = pr->st[0].cExp;
 printf("%p->%p->{%d,%d,%d}\n",pr,pr->st,pr->l,pr->st[0].type,ee.l);
 auto cc = ee.callee;
 auto aa = ee.arguments;
 struct Token a[] = { cc.object, cc.property, aa->arg };
 for(j=0;j<3;j++){
  printf("");
  printf("i:%d l:%d c:%d t:%d lex:%s \n",a[j].index,a[j].l,a[j].c,a[j].type,a[j].lex);
 }
 free(pr->st);
 free(pr);
 return 0;
}
