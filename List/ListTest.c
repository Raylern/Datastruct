#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"List.h"
#include"List.c"

#define TEST(a) printf("\ntest"#a"\n")

Element stringCopy(Element);
void stringFree   (Element);
bool stringEqual  (Element,Element);
int  stringCmp    (Element,Element);
bool longerThan   (Element,void* key);
void stringPrint  (Element);
Element stringInput();

int main(){
    TEST(1);//listCreation
    List list=listCreate(stringCopy,stringFree);
    listDisplay(list,stringPrint);

    TEST(2);//insertion
    char* p1="Ray";
    char* p2=stringInput();
    char* p3="OMG";
    listInsertFirst(list,(Element)p1);
    listInsertFirst(list,(Element)p2);
    listInsertLast(list,"Happy");
    stringFree(p2);
    listDisplay(list,stringPrint);

    TEST(2.2);
    listInsertPosition(list,(Element)p3,4);
    listInsertPosition(list,(Element)p3,2);
    listInsertPosition(list,(Element)p3,55);
    listInsertPosition(list,(Element)p3,-10);
    listDisplay(list,stringPrint);

    TEST(2.3);
    listInsertFirst(list,"head");
    listInsertLast(list,"tail");
    listGetFirst(list);
    listGetNext(list);
    listInsertBeforeCurrent(list,"2ndfirst");
    listGetLast(list);
    listGetPrev(list);
    listGetPrev(list);
    listInsertAfterCurrent(list,"3rdlast");
    listDisplay(list,stringPrint);

    TEST(2.4);//massive insert
    listGetPosition(list,4);
    for(int i=0;i<20;i++){
        listInsertAfterCurrent(list,"a");
    }
    listDisplay(list,stringPrint);

    TEST(3);//deletion
    listDelPosition(list,1);
    listDelElement(list,"3rdlast",stringEqual);
    listDisplay(list,stringPrint);

    listFind(list,"a",stringEqual);
    listRemoveCurrent(list);
    listRemoveFirst(list);
    listRemoveLast(list);
    listDisplay(list,stringPrint);

    TEST(4);//sort
    List templist=listCopy(list);
    listDisplay(list,stringPrint);
    listSort(templist,stringCmp);
    listMergeSort(list,stringCmp);
    listDisplay(list,stringPrint);
    listDisplay(templist,stringPrint);
    listDestroy(templist);

    TEST(5);//filter
    int testkey=2;
    List flist=listFilter(list,longerThan,(void*)&testkey);
    listDisplay(flist,stringPrint);

    TEST(6);//mutability
    stringPrint(listElemAlt(flist,"OMG","amOMG",stringEqual));
    stringPrint(listPosAlt(flist,0,"Head"));
    printf("\n");
    listDisplay(flist,stringPrint);
    listDestroy(flist);

    TEST(9);//copy & reverse
    List clist=listCopy(list);
    List rlist=listReverse(list);
    listDestroy(list);
    listDisplay(rlist,stringPrint);
    listDisplay(clist,stringPrint);

    TEST(10);//find & count
    printf("\"Ray\" in rlist is at index %ld.\n",listFind(rlist,"Ray",stringEqual));
    printf("\"Ray\" in clist is at index %ld.\n",listFind(clist,"Ray",stringEqual));
    long count=listCount(clist,"OMG",stringEqual);
    printf("\"OMG\" in clist appears %ld time%s.\n",count,count>1? "s":"");

    listDestroy(rlist);
    listDestroy(clist);
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
    if(str==NULL) return;
    free((char*)str);
}

bool stringEqual(Element str1,Element str2){
    if(str1==NULL || str2==NULL) return false;
    return strcmp((char*)str1,(char*)str2)==0;
}

int stringCmp(Element str1,Element str2){
    if(str1==NULL || str2==NULL) return 0;
    return strcmp((char*)str1,(char*)str2);
}

bool longerThan(Element str,void* key){
    if(str==NULL || key==NULL) return false;
    return strlen((char*)str) > *(int*)key;
}

void stringPrint(Element str){
    if(str==NULL) return;
    printf("  %s  ",(char*)str);
}

#define STRM_LEN 50
Element stringInput(){
    char* p=malloc((STRM_LEN+1)*sizeof(char));
    printf("Please input a string(<=%d):" ,STRM_LEN);
    scanf("%s",p);
    return p;
}
