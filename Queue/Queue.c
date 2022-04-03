#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Queue.h"

#define MAX_SIZE  -1
#define INITIAL_SIZE 10

/****V1.1 Implement by circular Array with %****/
//disadvantage: 1.queue->head and queue->tail will be very large
//              2.frequently access queue->length
//remark : to solve 1, when dequeue, they will be resetted
//advantage: no need to judge and reset queue->head and queue->tail when out of range


typedef struct Queue_t{
    int_type max_size;
    //no need len because len=tail-head
    Element* array;
    int_type head;     //index of first element, empty when
    int_type tail;     //index of
    ElemCopy elemCopy;
    ElemFree elemFree;
    ///Not traditional member
    int_type current;
} Queue_e;

static QueueResult queueExpand(Queue queue);

static QueueResult queueExpand(Queue queue){
    if(queue==NULL) return Q_NULL;
    Element* newarr=malloc((queue->max_size+INITIAL_SIZE)*sizeof(Element));
    if(newarr==NULL){
        return Q_NO_MEM;
    }
    int_type i=0;
    for(i = 0;i < queue->tail-queue->head;i++){
        newarr[i]=queue->array[(i+queue->head)%queue->max_size];
    }
    queue->head=0;
    queue->tail=i;
    for(;i<queue->max_size+INITIAL_SIZE;i++){
        newarr[i]=NULL;
    }
    free(queue->array);
    queue->array=newarr;
    newarr=NULL;
    queue->max_size+=INITIAL_SIZE;
    queue->current=-1; //disable current
    return Q_SUCCESS;
}
///End of static helper function

Queue queueCreate(ElemCopy elemCopy,ElemFree elemFree){
    if(elemCopy==NULL || elemFree==NULL){
        return NULL;
    }
    Queue queue=malloc(sizeof(*queue));
    if(queue==NULL){
        return NULL;
    }
    queue->array=malloc(INITIAL_SIZE*sizeof(Element));
    if(queue->array==NULL){
        free(queue);
        return NULL;
    }
    for(int_type i=0;i<INITIAL_SIZE;i++) queue->array[i]=NULL;        //initialization
    queue->current=-1;
    queue->max_size=INITIAL_SIZE;
    queue->head=0;
    queue->tail=0;
    queue->elemCopy = elemCopy;
    queue->elemFree = elemFree;
    return queue;
}

Queue queueCopy(Queue queue){
    if(queue==NULL) return NULL;
    Queue qcopy=malloc(sizeof(*qcopy));
    if(qcopy==NULL) return NULL;
    int_type max_size=queue->max_size;
    qcopy->array=malloc(max_size*sizeof(Element));
    if(qcopy->array==NULL){
        free(qcopy);
        return NULL;
    }
    for(int_type i=0;i<max_size;i++){
        if(queue->array[i]!=NULL)
            qcopy->array[i]=queue->elemCopy(queue->array[i]);
        else qcopy->array[i]=NULL;
    }
    qcopy->current=queue->current;
    qcopy->max_size=max_size;
    qcopy->head=queue->head;
    qcopy->tail=queue->tail;
    qcopy->elemCopy=queue->elemCopy;
    qcopy->elemFree=queue->elemFree;
    return qcopy;
}

void queueDestroy(Queue queue){
    if(queue==NULL) return;
    queueClear(queue);
    free(queue->array);
    free(queue);
} ///Don't forget to queue=NULL after destroy

QueueResult enqueue(Queue queue,Element elem){
    if(queue==NULL || elem==NULL) return Q_NULL;
    Element ecopy=queue->elemCopy(elem);
    if(ecopy==NULL) return Q_NO_MEM;
    if(queue->tail-queue->head>=queue->max_size){
        if(queueExpand(queue)!=Q_SUCCESS){
            queue->elemFree(ecopy);
            return Q_NO_MEM;
        }
    }
    queue->array[queue->tail%queue->max_size]=ecopy;
    queue->tail++;
    //stack->current=-1;
    return Q_SUCCESS;
}

Element dequeue(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->head==queue->tail) return NULL;
    Element temp=queue->array[queue->head%queue->max_size];
    queue->array[queue->head%queue->max_size]=NULL;
    queue->head++;
    ///can NOT do the following, because we use % is to ensure to make tail always larger than head
    ///if do this, tail may get in front of head
    //queue->head=queue->head%queue->max_size;
    //queue->tail=queue->tail%queue->max_size;
    queue->current=-1;
    return temp;
}

Element queueFront(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->head==queue->tail) return NULL;
    return queue->array[queue->head];
}

Element queueBack(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->head==queue->tail) return NULL;
    return queue->array[queue->tail-1];
}

void queueClear(Queue queue){
    if(queue==NULL) return;
    for(int_type i=queue->head;i<queue->tail;i++){
        queue->elemFree(queue->array[i%queue->max_size]);
    }
    queue->head=0;
    queue->tail=0;
    queue->current=-1;
}

int_type queueGetSize(Queue queue){
    if(queue==NULL) return -1;
    return queue->tail-queue->head;
}

bool queueIsEmpty(Queue queue){
    return queue->tail==queue->head;
}

bool queueIsFull(Queue queue){
    return queue->tail-queue->head>=queue->max_size;
}

///Not_traditional_functions
Element queueGetFirst(Queue queue){
    if(queue==NULL)
    if(queue->head==queue->tail) return NULL;
    queue->current=queue->head;
    return queue->array[queue->current%queue->max_size];
}

Element queueGetNext(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->current<0) return NULL;  //need valid current
    if(queue->current>=queue->tail) return NULL;//if curr is the last return NULL
    queue->current++;
    return queue->array[queue->current%queue->max_size];
}

Element queueGetCurrent(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->current<0) return NULL;
    return queue->array[queue->current%queue->max_size];
}

Element queueGetLast(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->head==queue->tail) return NULL;
    queue->current=queue->tail-1;
    return queue->array[queue->current%queue->max_size];
}

int_type queueFind(Queue queue,Element elem,ElemEqual elemEqual){
    if(queue==NULL || elem==NULL || elemEqual==NULL) return -1;
    QUEUE_FOREACH(Element,iterator,queue){
        if(elemEqual(iterator,elem))
            return queue->current%queue->max_size;
    }
    return -1;
}

Element queueAltCurr(Queue queue,Element newelem){
    if(queue==NULL || newelem==NULL) return NULL;
    if(queue->current<0) return NULL;  //need valid current
    Element newtemp=queue->elemCopy(newelem);
    if(newtemp==NULL) return NULL;
    int_type pos=queue->current%queue->max_size;
    queue->elemFree(queue->array[pos]);
    queue->array[pos]=newtemp;
    return queue->array[pos];
}

Element queueElemAlt(Queue queue,Element elem,Element newelem,ElemEqual elemEqual){
    if(queue==NULL || elem==NULL || newelem==NULL || elemEqual==NULL) return NULL;
    int_type pos=queueFind(queue,elem,elemEqual);
    if(pos<0) return NULL;
    return queuePosAlt(queue,pos,newelem);
}

Element queuePosAlt(Queue queue,int_type pos,Element newelem){
    if(queue==NULL || newelem==NULL) return NULL;
    if(pos<0 || pos+queue->head-(queue->head%queue->max_size)>=queue->tail)
        return NULL;
    Element newtemp=queue->elemCopy(newelem);
    if(newtemp==NULL) return NULL;
    queue->elemFree(queue->array[pos]);
    queue->array[pos]=newtemp;
    return queue->array[pos];
}

void queueDisplay(Queue queue,ElemOutput display){
    if(queue==NULL || display==NULL) return;
    int i=0;
    QUEUE_FOREACH(Element,iterator,queue){
        printf("%d\n",i++);
        display(iterator);
    }
    printf("\n");
}


/****V2.Implement by Knot with Dummy****/
//disadvantage: no indexing(or not O(1)indexing)
//advantage:
//Remark: I think dummy doesn't help for one-way linked list

/*
typedef struct Node_t{
    Element element;
    struct Node_t* next;
} Node_e, *Node;

typedef struct Queue_t{
    int_type len;
    Node head;
    Node last;
    ElemCopy elemCopy;
    ElemFree elemFree;
    ///Not traditional member
    Node current;
} Queue_e;


///No need static queueExpand, but need more helping construct node

static Node nodeCreate(Element elem,Node next);
static void nodeFree(Node node,ElemFree elemFree);
inline static Node queueGetFirstN(Queue queue);
inline static Node queueGetLastN(Queue queue);
inline static Node queueGetNextN(Queue queue);
static Node queueFind_N(Queue queue,Element elem,ElemEqual elemEqual);
//static void merge(Element *a,int_type na,Element *b,int_type nb,Element *c,ElemCmp elemCmp);

static Node nodeCreate(Element elem,Node next){ //here the elem must already be a copy
    if(elem==NULL) return NULL;                 //next can be NULL when constructing the last node
    Node newnode=malloc(sizeof(*newnode));
    if(newnode==NULL) return NULL;
    newnode->element=elem;
    newnode->next=next;
    return newnode;
}

static void nodeFree(Node node,ElemFree elemFree){ //remember to do something to prev and next node before using this
    if(node==NULL || elemFree==NULL) return;
    elemFree(node->element);
    free(node);
}

inline static Node queueGetFirstN(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->head==NULL) return NULL;//if list is empty,return NULL
    queue->current=queue->head;
    return queue->current;
}

inline static Node queueGetLastN(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->last==NULL) return NULL;//if list is empty,return NULL
    queue->current=queue->last;
    return queue->current;
}

inline static Node queueGetNextN(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->current==NULL) return NULL; //need valid current
    if(queue->current->next==NULL) return NULL;//if curr is the last element return NULL
    queue->current=queue->current->next;
    return queue->current;
}

#define QUEUE_FOREACH_N(iterator,queue) \
        for(Node iterator=queueGetFirstN(queue);iterator;iterator=queueGetNextN(queue))

static Node queueFind_N(Queue queue,Element elem,ElemEqual elemEqual){
    if(queue==NULL || elem==NULL || elemEqual==NULL) return NULL;
    QUEUE_FOREACH_N(iterator,queue){
        if(elemEqual(elem,iterator->element)){
            return iterator;
        }
    }
    return NULL;
}
///End of static helper function

Queue queueCreate(ElemCopy elemCopy,ElemFree elemFree){
    if(elemCopy==NULL || elemFree==NULL) return NULL;
    Queue queue=malloc(sizeof(*queue));
    if(queue==NULL){
        return NULL;
    }
    queue->head=NULL;
    queue->last=NULL;
    queue->len=0;
    queue->elemCopy = elemCopy;
    queue->elemFree = elemFree;
    queue->current=NULL;
    return queue;
}

Queue queueCopy(Queue queue){
    if(queue==NULL) return NULL;
    Queue qcopy=queueCreate(queue->elemCopy,queue->elemFree);
    if(qcopy==NULL) return NULL;
    QUEUE_FOREACH(Element,iterator,queue){
        if(enqueue(qcopy,iterator)!=Q_SUCCESS){
            queueDestroy(qcopy);
            return NULL;
        }
    }
    return qcopy; ///Different from List,the iterator/current of the lcopy is not set
}

void queueDestroy(Queue queue){
    if(queue==NULL) return;
    queueClear(queue);
    free(queue);
} ///Don't forget to queue=NULL after destroy

QueueResult enqueue(Queue queue,Element elem){   //insert last,if we have dummy here, we can dummy->next=node
    if(queue==NULL || elem==NULL) return Q_NULL;//but we don't, so need to take care to insert the first element
    Element ecopy=queue->elemCopy(elem);
    if(ecopy==NULL) return Q_NO_MEM;
    Node newnode=nodeCreate(ecopy,NULL);
    if(newnode==NULL){
        queue->elemFree(ecopy);
        return Q_NO_MEM;
    }
    if(queue->last==NULL)  //the queue is empty
        queue->head=newnode;
    else queue->last->next=newnode;
    queue->last=newnode;
    queue->len++;
    return Q_SUCCESS;
}

Element dequeue(Queue queue){  //remove first
    if(queue==NULL) return NULL;
    if(queue->head==NULL) return NULL; //if the queue is empty
    Node first=queue->head;
    queue->head=first->next;
    Element temp=first->element;
    free(first);
    queue->len--;
    if(queue->len<=0) queue->last=NULL;//if the first is also the last
    queue->current=NULL; //disable current
    return temp;
} ///don't forget to free the element dequeued by this function

Element queueFront(Queue queue){ //return the first element
    if(queue==NULL) return NULL;
    if(queue->head==NULL) return NULL; //if the queue is empty
    return queue->head->element;
}
Element queueBack(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->last==NULL) return NULL; //if the queue is empty
    return queue->last->element;
}

void queueClear(Queue queue){
    if(queue==NULL) return;
    Node curr=NULL, next=NULL;
    for(curr=queue->head;curr!=NULL;curr=next){
        next=curr->next;
        nodeFree(curr,queue->elemFree);
        //list->elemFree(curr->element);
        //free(curr);
    }
    queue->head=NULL;
    queue->last=NULL;
    queue->current=NULL;
    queue->len=0;
}

int_type queueGetSize(Queue queue){
    if(queue==NULL) return -1;
    return queue->len;
}

bool queueIsEmpty(Queue queue){
    if(queue==NULL) return true;
    return queue->len==0;
}

bool queueIsFull(Queue queue){
    if(queue==NULL) return true;
    if(MAX_SIZE<0) return true;
    return queue->len>=MAX_SIZE;
}

///Not_traditional_functions
Element queueFind(Queue queue,Element elem,ElemEqual elemEqual){
    if(queue==NULL || elem==NULL || elemEqual==NULL) return NULL;
    Node temp=queueFind_N(queue,elem,elemEqual);
    if(temp==NULL) return NULL;
    return temp->element;
}

Element queueGetFirst(Queue queue){
    if(queue==NULL) return NULL;
    Node first=queueGetFirstN(queue);
    if(first==NULL) return NULL;
    return first->element;
}

Element queueGetNext(Queue queue){
    if(queue==NULL) return NULL;
    Node next=queueGetNextN(queue);
    if(next==NULL) return NULL;
    return next->element;
}

Element queueGetCurrent(Queue queue){
    if(queue==NULL) return NULL;
    if(queue->current==NULL) return NULL;
    return queue->current->element;
}

Element queueGetLast(Queue queue){
    if(queue==NULL) return NULL;
    Node last=queueGetLastN(queue);
    if(last==NULL) return NULL;
    return last->element;
}

Element queueAltCurr(Queue queue,Element newelem){
    if(queue==NULL || newelem==NULL) return NULL;
    if(queue->current==NULL) return NULL;
    Element newtemp=queue->elemCopy(newelem);
    if(newtemp==NULL) return NULL;
    queue->elemFree(queue->current->element);
    queue->current->element=newtemp;
    return queue->current->element;
}

Element queueElemAlt(Queue queue,Element elem,Element newelem,ElemEqual elemEqual){
    if(queue==NULL || elem==NULL || newelem==NULL || elemEqual==NULL) return NULL;
    Element newtemp=queue->elemCopy(newelem);
    if(newtemp==NULL) return NULL;
    for(Node temp=queue->head; temp!=NULL; temp=temp->next){
        if(elemEqual(elem,temp->element)){
            queue->elemFree(temp->element);
            temp->element=newtemp;
            return temp->element;
        }
    }
    queue->elemFree(newtemp);
    return NULL;
}


void queueDisplay(Queue queue,ElemOutput display){
    if(queue==NULL || display==NULL) return;
    QUEUE_FOREACH(Element,iterator,queue){
        display(iterator);
    }
    printf("\n");
}
*/
