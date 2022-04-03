#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"List.h"

#define INITIAL_SIZE 10

/****V1.Implement by ARRAY****/
//disadvantage:
//1.average insert time/remove time=O(n),insert/remove last=O(1)
//2.search time=O(n) linear search.
//advantage:
//1.constant time indexing

//here the position is index(start from 0)
typedef struct List_t
{
    int_type max_size;
    int_type len;
    Element* array;
    int_type current;
    ElemCopy elemCopy;
    ElemFree elemFree;
} List_e;

static ListResult listExpand(List list);
static void merge(Element *a,int_type na,Element *b,int_type nb,Element *c,ElemCmp elemCmp);

static ListResult listExpand(List list){
    if(list==NULL) return L_NULL;
    Element* newarr=malloc((list->len+INITIAL_SIZE)*sizeof(Element));
    if(newarr==NULL){
        return L_NO_MEM;
    }
    for(int_type i=list->len;i<(list->len+INITIAL_SIZE);i++){
        newarr[i]=NULL;
    }
    //memcpy(newarr,(list->array),(list->len)*sizeof(Element));
    for(int_type i=0;i<list->len;i++){
        newarr[i]=list->array[i];
    }
    free(list->array);
    list->array=newarr;
    newarr=NULL;
    list->max_size+=INITIAL_SIZE;
    return L_SUCCESS;
}


static void merge(Element *a,int_type na,Element *b,int_type nb,Element *c,ElemCmp elemCmp){
    if(a==NULL || b==NULL || c==NULL || elemCmp==NULL) return;
    int_type ia, ib, ic;
    for(ia = ib = ic = 0; (ia < na) && (ib < nb); ic++){
        if(elemCmp(a[ia] , b[ib])<0) {
            c[ic] = a[ia];  ia++;
        }
        else {
            c[ic] = b[ib];  ib++;
        }
    }
    for(;ia < na; ia++, ic++) c[ic] = a[ia];
    for(;ib < nb; ib++, ic++) c[ic] = b[ib];
    return;
}
/**End of static helper function**/

List listCreate(ElemCopy elemCopy,ElemFree elemFree){
    if(elemCopy==NULL || elemFree==NULL){
        return NULL;
    }
    List list=malloc(sizeof(*list));
    if(list==NULL){
        return NULL;
    }
    list->array=malloc(INITIAL_SIZE*sizeof(Element));
    if(list->array==NULL){
        free(list);
        return NULL;
    }
    for(int_type i=0;i<INITIAL_SIZE;i++)list->array[i]=NULL;        //initialization
    list->current=-1;
    list->max_size=INITIAL_SIZE;
    list->len=0;
    list->elemCopy = elemCopy;
    list->elemFree = elemFree;
    return list;
}

List listCopy(List list){
    if(list==NULL) return NULL;
    List lcopy=malloc(sizeof(*lcopy));
    if(lcopy==NULL) return NULL;
    int_type max_size=list->max_size;
    lcopy->array=malloc(max_size*sizeof(Element));
    if(lcopy->array==NULL){
        free(lcopy);
        return NULL;
    }
    for(int_type i=0;i<max_size;i++)
        lcopy->array[i]=list->elemCopy(list->array[i]);
    ///elemCopy will return NULL if it get NULL
    lcopy->current=list->current;
    lcopy->max_size=max_size;
    lcopy->len=list->len;
    lcopy->elemCopy=list->elemCopy;
    lcopy->elemFree=list->elemFree;
    return lcopy;
}

void listDestroy(List list){
    if(list==NULL) return;
    listClear(list);
    free(list->array);
    free(list);
}

Element listGetFirst(List list){
    if(list==NULL) return NULL;
    if(list->len<=0) return NULL;
    list->current=0;
    return list->array[list->current];   //return the exact element, not a copy
}

Element listGetNext(List list){
    if(list==NULL) return NULL;
    if(list->current<0) return NULL;  //need valid current
    if(list->current>=(list->len)-1) return NULL;//if curr is the last return NULL
    list->current++;
    return list->array[list->current];
}

Element listGetCurrent(List list){
    if(list==NULL) return NULL;
    if(list->current<0) return NULL;  //need valid current
    return list->array[list->current];
}

Element listGetLast(List list){        /**Here get last is not the one after the last!**/
    if(list==NULL) return NULL;
    if(list->len<=0) return NULL;
    list->current=list->len-1;
    return list->array[list->current];
}

void listGetNULL(List list){
    if(list==NULL) return;
    list->current=-1;
}

ListResult listInsertFirst(List list,Element elem){
    if(list==NULL || elem==NULL) return L_NULL;
    Element ecopy=list->elemCopy(elem);
    if(ecopy==NULL) return L_NO_MEM;
    if(list->len >= list->max_size){
        if(listExpand(list)!=L_SUCCESS){
            list->elemFree(ecopy);
            return L_NO_MEM;
        }
    }
    for(int_type i=list->len-1;i>=0;i--){     //move all element 1 after
        list->array[i+1]=list->array[i];
    }
    list->array[0]=ecopy;
    list->len++;
    if(list->current>=0) list->current++;/**If the iterator is enabled,it points to the orginal element**/
    return L_SUCCESS;
}

ListResult listInsertLast(List list,Element elem){
    if(list==NULL || elem==NULL) return L_NULL;
    Element ecopy=list->elemCopy(elem);
    if(ecopy==NULL) return L_NO_MEM;
    if(list->len >= list->max_size){
        if(listExpand(list)!=L_SUCCESS){
            list->elemFree(ecopy);
            return L_NO_MEM;
        }
    }
    list->array[list->len]=ecopy;
    list->len++;
    //list->current=-1;
    return L_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list,Element elem){
    if(list==NULL || elem==NULL) return L_NULL;
    if(list->current<0) return L_ILL_CURR;           //need legal curr
    Element ecopy=list->elemCopy(elem);
    if(ecopy==NULL) return L_NO_MEM;
    if(list->len >= list->max_size){
        if(listExpand(list)!=L_SUCCESS){
            list->elemFree(ecopy);
            return L_NO_MEM;
        }
    }
    for(int_type i=list->len-1;i>=list->current;i--){ //move all element 1 after
        list->array[i+1]=list->array[i];
    }
    list->array[list->current]=ecopy;
    list->len++;
    list->current++;
    return L_SUCCESS;
}

ListResult listInsertAfterCurrent(List list,Element elem){
    if(list==NULL || elem==NULL) return L_NULL;
    if(list->current<0) return L_ILL_CURR;
    Element ecopy=list->elemCopy(elem);
    if(ecopy==NULL) return L_NO_MEM;
    if(list->len >= list->max_size){
        if(listExpand(list)!=L_SUCCESS){
            list->elemFree(ecopy);
            return L_NO_MEM;
        }
    }
    for(int_type i=list->len-1;i>list->current;i--){ //move all element 1 after
        list->array[i+1]=list->array[i];
    }
    list->array[list->current+1]=ecopy;
    list->len++;
    return L_SUCCESS;
}

ListResult listRemoveFirst(List list){
    if(list==NULL) return L_NULL;
    if(listGetFirst(list)==NULL) return L_EMPTY;
    return listRemoveCurrent(list);
}

ListResult listRemoveLast(List list){
    if(list==NULL) return L_NULL;
    if(listGetLast(list)==NULL) return L_EMPTY;
    return listRemoveCurrent(list);
}

ListResult listRemoveCurrent(List list){
    if(list==NULL) return L_NULL;
    if(list->current<0) return L_ILL_CURR;
    int_type pos=list->current;
    if(list->array[pos]!=NULL) list->elemFree(list->array[pos]);
    list->array[pos]=NULL;
    for(int_type i=list->current;i<list->len;i++)
        list->array[i]=list->array[i+1];
    list->array[list->len-1]=NULL;
    list->len--;
    list->current=-1;
    return L_SUCCESS;
}

void listClear(List list){
    if(list==NULL) return;
    for(int_type i=0;i<list->len;i++){
        list->elemFree(list->array[i]);
    }
    list->len=0;
    list->current=-1;                           //undefine iterator
}

int_type listGetSize(List list){
    if(list==NULL) return -1;
    return list->len;
}

bool listIsEmpty(List list){
    if(list==NULL) return true;
    return list->len==0;
}

bool listIsFull(List list){
    if(list==NULL) return true;
    return list->len==list->max_size;
}

ListResult listSort(List list,ElemCmp elemCmp){//Minsort
    if(list==NULL || elemCmp==NULL) return L_NULL;
    Element temp=NULL;
    for(int_type i=0;i<list->len;i++){
        for(int_type j=i+1;j<list->len;j++){
            if(elemCmp(list->array[i],list->array[j])>0){
                temp=list->array[i];
                list->array[i]=list->array[j];
                list->array[j]=temp;
                temp=NULL;
            }
        }
    }
    list->current=-1;
    return L_SUCCESS;
}

ListResult listMergeSort(List list,ElemCmp elemCmp){//MergeSort
    if(list==NULL || elemCmp==NULL) return L_NULL;
    if(list->len<2) return L_SUCCESS;
    int_type cmplen, len=list->len;
    Element *temp=NULL, *base=NULL, *arr=list->array;
    temp = malloc((list->len)*sizeof(Element));
    if(temp == NULL) {
         return L_NO_MEM;
    }
    for (cmplen = 1; cmplen < len; cmplen *= 2) {
        for (base = arr; base < arr + (len/cmplen)*cmplen; base += 2 * cmplen) {
            merge(base, cmplen, base + cmplen, (base+2*cmplen> arr +len ? len%cmplen: cmplen), temp,elemCmp);
            memcpy(base, temp, (base+2*cmplen> arr+len ? cmplen+len%cmplen: 2*cmplen)*sizeof(Element));
        }
    }
    free(temp); temp=NULL; base=NULL;
    list->current=-1;
    return L_SUCCESS;
}

List listFilter(List list,ElemCondition cond,void* key){
    if(list==NULL || cond==NULL) return NULL;
    List flist=listCreate(list->elemCopy,list->elemFree);
    if(flist==NULL) return NULL;
    for(int_type i=0;i<list->len;i++){
        if(cond(list->array[i],key))
            if(listInsertLast(flist,list->array[i])!=L_SUCCESS){
                listDestroy(flist);
                return NULL;
            }
    }
    return flist;
}

/******Not_traditional_functions******/
int_type listFind(List list,Element elem,ElemEqual elemEqual){
    if(list==NULL || elem==NULL || elemEqual==NULL) return -1;
    LIST_FOREACH(Element,iterator,list){
        if(elemEqual(iterator,elem))
            return list->current;
    }
    return -1;
}

int_type listCount(List list,Element elem,ElemEqual elemEqual){
    if(list==NULL || elem==NULL || elemEqual==NULL) return -1;
    int_type count=0;
    LIST_FOREACH(Element,iterator,list){
        if(elemEqual(iterator,elem))
            count++;
    }
    return count;
}

List listReverse(List list){
    if(list==NULL) return NULL;
    List rlist=listCreate(list->elemCopy,list->elemFree);
    if(rlist==NULL) return NULL;
    LIST_FOREACH(Element,iterator,list){
        if(listInsertFirst(rlist,iterator)!=L_SUCCESS){
            listDestroy(rlist);
            return NULL;
        }
    }
    return rlist;
}

//for mutability
Element listAltCurr(List list,Element newelem){
    if(list==NULL || newelem==NULL) return NULL;
    if(list->current<0) return NULL;
    Element newtemp=list->elemCopy(newelem);
    if(newtemp==NULL) return NULL;
    list->elemFree(list->array[list->current]);
    list->array[list->current]=newtemp;
    return list->array[list->current];
}

Element listElemAlt(List list,Element elem,Element newelem,ElemEqual elemEqual){
    if(list==NULL || elem==NULL || newelem==NULL || elemEqual==NULL) return NULL;
    Element newtemp=list->elemCopy(newelem);
    if(newtemp==NULL) return NULL;
    for(int_type i=0;i<list->len;i++){
        if(elemEqual(elem,list->array[i])){
            list->elemFree(list->array[i]);
            list->array[i]=newtemp;
            return list->array[i];
        }
    }
    list->elemFree(newtemp);
    return NULL;
}

Element listPosAlt (List list,int_type pos,Element newelem){
    if(list==NULL || newelem==NULL) return NULL;
    if(pos<0 || pos>=list->len) return NULL;
    Element newtemp=list->elemCopy(newelem);
    if(newtemp==NULL) return NULL;
    list->elemFree(list->array[pos]);
    list->array[pos]=newtemp;
    return list->array[pos];
}

//For iteration
Element listGetPrev(List list){
    if(list==NULL) return NULL;
    if(list->current<=0) return NULL;//need valid current and if curr is the first return NULL
    list->current--;
    return list->array[list->current];
}

Element listGetPosition(List list,int_type position){
    if(list==NULL) return NULL;
    if(position<0 || list->len<=0) return NULL;
    if(position>=list->len) return listGetLast(list);
    list->current=position;
    return list->array[list->current];
}

//for stack
Element listPop(List list){
    if(list==NULL) return NULL;
    if(list->len<=0) return NULL;//if list is empty,return NULL
    Element temp=list->array[list->len-1];
    list->array[list->len-1]=NULL;
    list->len--;
    list->current=-1;
    return temp;
}///don't forget to free the element got from this function

Element listTop(List list){
    if(list==NULL) return NULL;
    if(list->len<=0) return NULL;
    return list->array[list->len-1];
}

ListResult listPush(List list,Element elem){
    return listInsertLast(list,elem);
}

//for queue
ListResult listEnqueueF(List list,Element elem){
    return listInsertFirst(list,elem);
}

ListResult listEnqueueB(List list,Element elem){
    return listInsertLast(list,elem);
}

Element listDequeueF(List list){
    if(list==NULL) return NULL;
    if(list->len<=0) return NULL;
    Element temp=list->array[0];
    list->array[0]=NULL;
    if(listRemoveFirst(list)!=L_SUCCESS){
        list->array[0]=temp;
        return NULL;
    }
    return temp;
}

Element listDequeueB(List list){
    return listPop(list);
}

Element listFront(List list){
    if(list==NULL) return NULL;
    if(list->len<=0) return NULL;
    return list->array[0];
}

Element listBack(List list){
    if(list==NULL) return NULL;
    if(list->len<=0) return NULL;
    return list->array[list->len-1];
}

//for more flexible insert or delete
bool listElemIn(List list,Element elem,ElemEqual elemEqual){
    if(list==NULL || elem==NULL || elemEqual==NULL) return false;
    for(int_type i=0;i<list->len;i++){
        if(elemEqual(elem,list->array[i])){
            return true;
        }
    }
    return false;
}

ListResult listDelElement(List list,Element elem,ElemEqual elemEqual){
    if(list==NULL || elem==NULL || elemEqual==NULL) return L_NULL;
    int_type pos=listFind(list,elem,elemEqual);
    if(pos<=0) return L_EDNE;
    return listDelPosition(list,pos);
}
ListResult listDelPosition(List list,int_type pos){
    if(list==NULL) return L_NULL;
    if(pos<0) return L_ILL_POS;
    if(pos>=list->len) return listRemoveLast(list);
    if(listGetPosition(list,pos)==NULL) return L_EMPTY;
    return listRemoveCurrent(list);
}

ListResult listInsertPosition(List list,Element elem,int_type pos){
    if(list==NULL || elem==NULL) return L_NULL;
    if(pos<0) return L_ILL_POS;
    if(pos>=list->len) return listInsertLast(list,elem);  //if pos too big, then insert last
    Element ecopy=list->elemCopy(elem);
    if(ecopy==NULL) return L_NO_MEM;
    if(list->len >= list->max_size){
        if(listExpand(list)!=L_SUCCESS){
            list->elemFree(ecopy);
            return L_NO_MEM;
        }
    }
    for(int_type i=list->len-1;i>=pos-1;i--){                //move all element 1 after
        list->array[i+1]=list->array[i];
    }
    list->array[pos-1]=ecopy;
    list->len++;
    if(list->current >= pos-1)list->current++;
    return L_SUCCESS;
}

//for display
void listDisplay(List list,ElemOutput display){
    if(list==NULL || display==NULL) return;
    LIST_FOREACH(Element,iterator,list){
        display(iterator);
    }
    printf("\n");
}
