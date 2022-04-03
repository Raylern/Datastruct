#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Queue.h"
#include"Queue.c"

Element stringCopy(Element);
void stringFree   (Element);
bool stringEqual  (Element,Element);
int  stringCmp    (Element,Element);
bool longerThan   (Element,void* key);
void stringPrint  (Element);
Element stringInput();

int main(){
    char* p1="Ray";
    //char* p2="Fang";
    char* p2=stringInput();
    Queue queue=queueCreate(stringCopy,stringFree);
    stringPrint(dequeue(queue)); printf("\n");
    enqueue(queue,(Element)p2);
    enqueue(queue,(Element)p2);
    enqueue(queue,(Element)p1);
    enqueue(queue,(Element)p2);
    enqueue(queue,(Element)p1);
    enqueue(queue,(Element)p2);
    free(p2);
    char* p3="OMG";
    enqueue(queue,(Element)p3);
    enqueue(queue,"Happy");
    queueDisplay(queue,stringPrint);

    Queue cqueue=queueCopy(queue);
    queueDisplay(cqueue,stringPrint);

    for(int i=0;i<10;i++){
        enqueue(queue,(Element)p1);
        enqueue(queue,(Element)p3);
    }
    queueDisplay(queue,stringPrint);
    queueDestroy(queue);  queue=NULL;
    queueDisplay(cqueue,stringPrint);

    printf("\"Ray\" in cqueue is at position %ld.\n",queueFind(cqueue,"Ray",stringEqual));
    //printf("\"Ray\" in cqueue is at position %s.\n",(char*)queueFind(cqueue,"Ray",stringEqual));
    printf("Current queue len is %ld.\n",queueGetSize(cqueue));
    printf("Deleted queue len is %ld.\n",queueGetSize(queue));

    queueDisplay(cqueue,stringPrint);
    Element strf=queueFront(cqueue);
    printf("the string \'%s\'",(char*)strf);
    printf("is changed with \'%s\'\n",(char*)queueElemAlt(cqueue,strf,"OMG,You change the string!!!",stringEqual));
    ///Can NOT put str and stackElemAlt in the same line, because str is the exact pointer in structure, and stackElemAlt will free it.

    Element strb=queueBack(cqueue);
    printf("the string \'%s\'",(char*)strb);
    printf("is changed with \'%s\'\n",(char*)queueElemAlt(cqueue,strb,"Ray,you need smile and courage!!!",stringEqual));
    queueDisplay(cqueue,stringPrint);

    int_type pos=3;
    printf("string at index %ld is changed with \'%s\'\n",pos,(char*)queuePosAlt(cqueue,pos,"Ray,you need smile and courage!!!"));
    queueDisplay(cqueue,stringPrint);

    char* str2=dequeue(cqueue);
    printf("dequeue \'%s\'\n",str2);
    stringFree(str2);
    char* str3=dequeue(cqueue);
    printf("dequeue \'%s\'\n",str3);
    stringFree(str3);
    queueDisplay(cqueue,stringPrint);

    printf("Current queue len is %ld.\n",queueGetSize(cqueue));

    enqueue(queue,(Element)p1);
    queueDisplay(queue,stringPrint);

    enqueue(cqueue,(Element)p1);
    enqueue(cqueue,(Element)p1);
    queueDisplay(cqueue,stringPrint);

    queueDestroy(cqueue);
    return 0;
}

Element stringCopy(Element str){
    if(str==NULL) return NULL;
    char* string=str;
    char* p=malloc((strlen(string)+1)*sizeof(char));
    strcpy(p,string);
    return p;
}

void stringFree(Element str){
    free((char*)str);
}

bool stringEqual(Element str1,Element str2){
    return strcmp((char*)str1,(char*)str2)==0;
}

int stringCmp(Element str1,Element str2){
    return strcmp((char*)str1,(char*)str2);
}

bool longerThan(Element str,void* key){
    return strlen((char*)str) > *(int*)key;
}

void stringPrint(Element str){
    printf("  %s  ",(char*)str);
}

#define STRM_LEN 50
Element stringInput(){
    char* p=malloc((STRM_LEN+1)*sizeof(char));
    printf("Please input a string(<=%d):" ,STRM_LEN);
    scanf("%s",p);
    return p;
}
