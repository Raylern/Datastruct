#ifndef ADT_H_
#define ADT_H_

#include<stdbool.h>

typedef void* Element;          //for generic element in data structure
typedef void* KeyElem;          //for generic key element in dictionary
typedef void* ValueElem;        //for generic value element in dictionary

typedef Element (*ElemCopy)     (Element);          //returns a copy of current element, NULL if error
typedef void    (*ElemFree)     (Element);          //free the memory of this element
typedef bool    (*ElemEqual)    (Element,Element);  //return True if Equal, else False
typedef int     (*ElemCmp)      (Element,Element);  //positive if A greater B, 0 if equal, negative if smaller
typedef bool    (*ElemCondition)(Element,void* key);//filter condition for filter
typedef Element (*ElemInput)    ();                 //Input an element from standard input
typedef void    (*ElemOutput)   (Element);          //Output an element from standard output

#endif // ADT_H_

