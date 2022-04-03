#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Stack.h"

#define INITIAL_SIZE 10

/****V1.Implement by ARRAY(just the same as list implemented by array)****/
//disadvantage: need to enlarge from time to time. enlarge time O(n)

typedef struct Stack_t{
    int_type max_size;
    int_type len;
    Element* array;
    ElemCopy elemCopy;
    ElemFree elemFree;
    ///Not traditional member
    int_type current;
} Stack_e;

static StackResult stackExpand(Stack stack);

static StackResult stackExpand(Stack stack){
    if(stack==NULL) return S_NULL;
    Element* newarr=malloc((stack->len+INITIAL_SIZE)*sizeof(Element));
    if(newarr==NULL){
        return S_NO_MEM;
    }
    for(int_type i=stack->len;i<(stack->len+INITIAL_SIZE);i++){
        newarr[i]=NULL;
    }
    memcpy(newarr,(stack->array),(stack->len)*sizeof(Element));
    free(stack->array);
    stack->array=newarr;
    newarr=NULL;
    stack->max_size+=INITIAL_SIZE;
    return S_SUCCESS;
}
///End of static helper function

Stack stackCreate(ElemCopy elemCopy,ElemFree elemFree){
    if(elemCopy==NULL || elemFree==NULL){
        return NULL;
    }
    Stack stack=malloc(sizeof(*stack));
    if(stack==NULL){
        return NULL;
    }
    stack->array=malloc(INITIAL_SIZE*sizeof(Element));
    if(stack->array==NULL){
        free(stack);
        return NULL;
    }
    for(int_type i=0;i<INITIAL_SIZE;i++) stack->array[i]=NULL;        //initialization
    stack->current=-1;
    stack->max_size=INITIAL_SIZE;
    stack->len=0;
    stack->elemCopy = elemCopy;
    stack->elemFree = elemFree;
    return stack;
}

Stack stackCopy(Stack stack){
    if(stack==NULL) return NULL;
    Stack scopy=malloc(sizeof(*scopy));
    if(scopy==NULL) return NULL;
    int_type max_size=stack->max_size;
    scopy->array=malloc(max_size*sizeof(Element));
    if(scopy->array==NULL){
        free(scopy);
        return NULL;
    }
    for(int_type i=0;i<max_size;i++)
        scopy->array[i]=stack->elemCopy(stack->array[i]);
    ///elemCopy will return NULL if it get NULL
    scopy->current=stack->current;
    scopy->max_size=max_size;
    scopy->len=stack->len;
    scopy->elemCopy=stack->elemCopy;
    scopy->elemFree=stack->elemFree;
    return scopy;
}

void stackDestroy(Stack stack){
    if(stack==NULL) return;
    stackClear(stack);
    free(stack->array);
    free(stack);
} ///Don't forget to stack=NULL after destroy

StackResult stackPush(Stack stack,Element elem){
    if(stack==NULL || elem==NULL) return S_NULL;
    Element ecopy=stack->elemCopy(elem);
    if(ecopy==NULL) return S_NO_MEM;
    if(stack->len >= stack->max_size){
        if(stackExpand(stack)!=S_SUCCESS){
            stack->elemFree(ecopy);
            return S_NO_MEM;
        }
    }
    stack->array[stack->len]=ecopy;
    stack->len++;
    //stack->current=-1;
    return S_SUCCESS;
}

Element stackTop(Stack stack){
    if(stack==NULL) return NULL;
    if(stack->len<=0) return NULL; //if the stack is empty then return NULL
    return stack->array[stack->len-1];
}

Element stackPop(Stack stack){
    if(stack==NULL) return NULL;
    if(stack->len<=0) return NULL;//if list is empty,return NULL
    Element temp=stack->array[stack->len-1];
    stack->array[stack->len-1]=NULL;
    stack->len--;
    stack->current=-1;
    return temp;
}///don't forget to free the element got from this function

void stackClear(Stack stack){
    if(stack==NULL) return;
    for(int_type i=0;i<stack->len;i++){
        stack->elemFree(stack->array[i]);
    }
    stack->len=0;
    stack->current=-1;                           //undefine iterator
}

int_type stackGetSize(Stack stack){
    if(stack==NULL) return -1;
    return stack->len;
}

bool stackIsEmpty(Stack stack){
    if(stack==NULL) return true;
    return stack->len==0;
}

bool stackIsFull(Stack stack){
    if(stack==NULL) return true;
    return stack->len==stack->max_size;
}

///Not_traditional_functions
Element stackGetFirst(Stack stack){
    if(stack==NULL) return NULL;
    if(stack->len<=0) return NULL;
    stack->current=0;
    return stack->array[stack->current];
}

Element stackGetNext(Stack stack){
    if(stack==NULL) return NULL;
    if(stack->current<0) return NULL;  //need valid current
    if(stack->current>=(stack->len)-1) return NULL;//if curr is the last return NULL
    stack->current++;
    return stack->array[stack->current];
}

Element stackGetCurrent(Stack stack){
    if(stack==NULL) return NULL;
    if(stack->current<0) return NULL;
    return stack->array[stack->current];
}

Element stackGetLast(Stack stack){
    if(stack==NULL) return NULL;
    if(stack->len<=0) return NULL;
    stack->current=stack->len-1;
    return stack->array[stack->current];
}

int_type stackFind(Stack stack,Element elem,ElemEqual elemEqual){
    if(stack==NULL || elem==NULL || elemEqual==NULL) return -1;
    STACK_FOREACH(Element,iterator,stack){
        if(elemEqual(iterator,elem))
            return stack->current;
    }
    return -1;
}

Element stackAltCurr(Stack stack,Element newelem){
    if(stack==NULL || newelem==NULL) return NULL;
    if(stack->current<0) return NULL;  //need valid current
    Element newtemp=stack->elemCopy(newelem);
    if(newtemp==NULL) return NULL;
    stack->elemFree(stack->array[stack->current]);
    stack->array[stack->current]=newtemp;
    return stack->array[stack->current];
}

Element stackElemAlt(Stack stack,Element elem,Element newelem,ElemEqual elemEqual){
    if(stack==NULL || elem==NULL || newelem==NULL || elemEqual==NULL) return NULL;
    int_type pos=stackFind(stack,elem,elemEqual);
    if(pos<0 || pos>=stack->len) return NULL;
    return stackPosAlt(stack,pos,newelem);
}

Element stackPosAlt (Stack stack,int_type pos,Element newelem){
    if(stack==NULL || newelem==NULL) return NULL;
    if(pos<0 || pos>=stack->len) return NULL;
    Element newtemp=stack->elemCopy(newelem);
    if(newtemp==NULL) return NULL;
    stack->elemFree(stack->array[pos]);
    stack->array[pos]=newtemp;
    return stack->array[pos];
}

void stackDisplay(Stack stack,ElemOutput display){
    if(stack==NULL || display==NULL) return;
    STACK_FOREACH(Element,iterator,stack){
        display(iterator);
    }
    printf("\n");
}

/****V2.Implement by linked node(just the same as linked list)****/

/****V3.Implement by list(just the same as list)****/
