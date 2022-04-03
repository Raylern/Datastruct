#ifndef LIST_H_
#define LIST_H_

#include"../ADT.h"

#define int_type long

/*
Unordered, mutable(in-place) data structure List
*/

typedef struct List_t *List;
typedef enum{
    L_SUCCESS , L_EAE ,L_EDNE , L_NO_MEM , L_NULL , L_FULL , L_EMPTY , L_ILL_CURR ,L_ILL_POS
} ListResult;

List listCreate(ElemCopy elemCopy,ElemFree elemFree);
List listCopy(List list);
void listDestroy(List list);

Element listGetFirst(List list);
Element listGetNext(List list);
Element listGetCurrent(List list);
Element listGetLast(List list);
void listGetNULL(List list);//disable iterator

ListResult listInsertFirst(List list,Element elem);
ListResult listInsertLast(List list,Element elem);
ListResult listInsertBeforeCurrent(List list,Element elem);
ListResult listInsertAfterCurrent(List list,Element elem);

ListResult listRemoveFirst(List list);
ListResult listRemoveLast(List list);
ListResult listRemoveCurrent(List list);

void listClear(List list);
int_type listGetSize(List list);

bool listIsEmpty(List list);
bool listIsFull(List list);

ListResult listSort(List list,ElemCmp elemcmp);
ListResult listMergeSort(List list,ElemCmp elemCmp);
List       listFilter(List list,ElemCondition cond,void* key);

#define LIST_FOREACH(Type,iterator,list) \
        for(Type iterator=listGetFirst(list);iterator!=NULL;iterator=listGetNext(list))

/******Not_traditional_functions******/
int_type listFind(List list,Element elem,ElemEqual elemEqual);
int_type listCount(List list,Element elem,ElemEqual elemEqual);
List     listReverse(List list);

//for mutability
Element listAltCurr(List list,Element newelem);
Element listElemAlt(List list,Element elem,Element newelem,ElemEqual elemEqual);
Element listPosAlt (List list,int_type pos,Element newelem);//Not for liked nodes implementation

//for iteration
Element listGetPrev(List list);
Element listGetPosition(List list,int_type pos);

//for stack
Element    listPop(List list);
Element    listTop(List list);
ListResult listPush(List list,Element elem);

//for queue
ListResult listEnqueueF(List list,Element elem);
ListResult listEnqueueB(List list,Element elem);
Element listDequeueF(List list);
Element listDequeueB(List list);
Element listFront(List list);
Element listBack(List list);

//for more flexible insert or delete
bool listElemIn(List list,Element elem,ElemEqual elemEqual);
ListResult listDelElement(List list,Element elem,ElemEqual elemEqual);
ListResult listDelPosition(List list,int_type pos);
ListResult listInsertPosition(List list,Element elem,int_type pos);

//for display
void listDisplay(List list,ElemOutput display);

#endif // LIST_H_
