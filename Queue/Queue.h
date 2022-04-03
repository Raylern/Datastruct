#ifndef QUEUE_H_
#define QUEUE_H_

#include"../ADT.h"

#define int_type long

/*
Unordered, mutable(in-place) data structure Queue
*/

typedef struct Queue_t *Queue;
typedef enum{
    Q_SUCCESS, Q_NO_MEM, Q_NULL, Q_EMPTY, Q_FULL
} QueueResult;

Queue queueCreate(ElemCopy elemCopy,ElemFree elemFree);
Queue queueCopy(Queue queue);
void queueDestroy(Queue queue);

QueueResult enqueue(Queue queue,Element elem);
Element     dequeue(Queue queue);

Element queueFront(Queue queue);
Element queueBack (Queue queue);

void queueClear(Queue queue);
int_type queueGetSize(Queue queue);

bool queueIsEmpty(Queue queue);
bool queueIsFull(Queue queue);

/******Not_traditional_functions and MACROs******/
#define QUEUE_FOREACH(Type,iterator,queue) \
        for(Type iterator=queueGetFirst(queue);iterator;iterator=queueGetNext(queue))

Element queueGetFirst(Queue queue);
Element queueGetNext(Queue queue);
Element queueGetCurrent(Queue queue);
Element queueGetLast(Queue queue);

int_type queueFind(Queue queue,Element elem,ElemEqual elemEqual);
//Element queueFind (Queue queue,Element elem,ElemEqual elemEqual);//For linked nodes implementation

Element queueAltCurr(Queue queue,Element newelem);
Element queueElemAlt(Queue queue,Element elem,Element newelem,ElemEqual elemEqual);
Element queuePosAlt (Queue queue,int_type pos,Element newelem);//Not for liked nodes implementation

void queueDisplay(Queue queue,ElemOutput display);

#endif // QUEUE_H_
