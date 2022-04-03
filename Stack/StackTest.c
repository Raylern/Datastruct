#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Stack.h"
#include"Stack.c"

Element stringCopy(Element);
void stringFree   (Element);
bool stringEqual  (Element,Element);
int  stringCmp    (Element,Element);
bool longerThan   (Element,void* key);
void stringPrint  (Element);
Element stringInput();
//Element stringAlter  (Element,Element);//Not good enough, see implementation

int main(){
    char* p1="Ray";
    //char* p2="Fang";
    char* p2=stringInput();
    Stack stack=stackCreate(stringCopy,stringFree);
    stringPrint(stackPop(stack)); printf("\n");
    stackPush(stack,(Element)p2);
    stackPush(stack,(Element)p2);
    stackPush(stack,(Element)p1);
    stackPush(stack,(Element)p2);
    stackPush(stack,(Element)p1);
    stackPush(stack,(Element)p2);
    free(p2);
    char* p3="OMG";
    stackPush(stack,(Element)p3);
    stackPush(stack,"Happy");
    stackDisplay(stack,stringPrint);

    Stack cstack=stackCopy(stack);
    stackDisplay(cstack,stringPrint);

    for(int i=0;i<10;i++){
        stackPush(stack,(Element)p1);
        stackPush(stack,(Element)p3);
    }
    stackDisplay(stack,stringPrint);
    stackDestroy(stack);  stack=NULL;
    stackDisplay(cstack,stringPrint);


    printf("\"Ray\" in rlist is at index %ld.\n",stackFind(cstack,"Ray",stringEqual));
    //printf("\"Ray\" in rlist is at index %s.\n",(char*)stackFind(cstack,"Ray",stringEqual));
    printf("Current stack len is %ld.\n",stackGetSize(cstack));
    printf("Deleted stack len is %ld.\n",stackGetSize(stack));

    Element str=stackTop(cstack);     //str is the exact pointer stored in the strcuture
    stackDisplay(cstack,stringPrint);
    /*
    stringFree(str);                  //the memory that pointer str is pointing to is freed, but the data structure don't know
    str=stringCopy("Hacking");
    stackDisplay(cstack,stringPrint);
    //This gives Error because the cell in the struct is holding the address of &str
    //when we do str=stringCopy, we actually altered the value(an address) holding in variable str
    //And the string written in previous location &str is NOT changed, but freed
    //so when stackDisplay(stringPrint) try to dereference the freed pointer, Error occurs.
    */

    /*
    stringAlter(str,"Oh");
    //this is also not good enough, although we don't malloc new space to avoid doing nothing,
    //we can't change the string to be bigger without memory leak.
    //So, we need the stack structure to be present in the argument, and we need its help
    */

    //Finally, we have a new method to support mutability (in-place)
    printf("the string \'%s\'",(char*)str);
    printf("is changed with \'%s\'\n",(char*)stackElemAlt(cstack,str,"OMG,You change the string!!!",stringEqual));
    ///Can NOT put str and stackElemAlt in the same line, because str is the exact pointer in structure, and stackElemAlt will free it.
    int_type pos=3;
    printf("string at index %ld is changed with \'%s\'\n",pos,(char*)stackPosAlt(cstack,pos,"Ray,you need smile and courage!!!"));
    stackDisplay(cstack,stringPrint);

    Element str2=stackPop(cstack);
    stringPrint(str2); printf("\n");
    stringFree(str2);
    Element str3=stackPop(cstack);
    stringPrint(str3); printf("\n");
    stringFree(str3);
    stackDisplay(cstack,stringPrint);

    printf("Current stack len is %ld.\n",stackGetSize(cstack));

    stackPush(stack,(Element)p1);
    stackDisplay(stack,stringPrint);

    stackPush(cstack,(Element)p1);
    stackPush(cstack,(Element)p1);
    stackDisplay(cstack,stringPrint);

    stackDestroy(cstack);
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

/*
//Still can not be mutable fully, for example, if the string is larger, then strcpy may write into illegal place.
//When alter need to malloc new space, this will fail
//So, need method inside the data structure to support mutability
Element stringAlter(Element str1,Element str2){
    if(str1==NULL || str2==NULL) return NULL;
    char* strto=str1, *strfrom=str2;
    strcpy(strto,strfrom);
    return str1;
}*/
