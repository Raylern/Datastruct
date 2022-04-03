#ifndef STACK_H_
#define STACK_H_

#include"../ADT.h"

#define int_type long

/*
Unordered, mutable(in-place) data structure Stack
*/

typedef struct Stack_t *Stack;
typedef enum{
    S_SUCCESS, S_NO_MEM, S_NULL, S_EMPTY, S_FULL, S_ILL_CURR
} StackResult;

Stack stackCreate(ElemCopy elemCopy,ElemFree elemFree);
Stack stackCopy(Stack stack);
void stackDestroy(Stack stack);

StackResult stackPush(Stack stack,Element elem);
Element stackTop(Stack stack);
Element stackPop(Stack stack);

void stackClear(Stack stack);
int_type stackGetSize(Stack stack);

bool stackIsEmpty(Stack stack);
bool stackIsFull(Stack stack);

/******Not_traditional_functions and MACROs******/
#define STACK_FOREACH(Type,iterator,stack) \
        for(Type iterator=stackGetFirst(stack);iterator;iterator=stackGetNext(stack))

Element stackGetFirst  (Stack stack);
Element stackGetNext   (Stack stack);
Element stackGetCurrent(Stack stack);
Element stackGetLast   (Stack stack);

int_type stackFind(Stack stack,Element elem,ElemEqual elemEqual);
//Element stackFind (Stack stack,Element elem,ElemEqual elemEqual);//for linked node implementation

Element stackAltCurr(Stack stack,Element newelem);
Element stackElemAlt(Stack stack,Element elem,Element newelem,ElemEqual elemEqual);
Element stackPosAlt (Stack stack,int_type pos,Element newelem);//Not for liked nodes implementation
void stackDisplay(Stack stack,ElemOutput display);

#endif // STACK_H_
