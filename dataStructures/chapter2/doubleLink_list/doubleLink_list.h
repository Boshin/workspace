#include <stdio.h>
#include <stdlib.h>

#include "../../MicroDefine.h"

template <class ElemType>
struct DuLinkList
{
   ElemType data;
   struct DuLinkList *prior;
   struct DuLinkList *next;
};

typedef struct DuLinkList<int> DLinkList;
typedef struct DuLinkList<int> *pDLinkList;

typedef Status (*CompareFunc)(int elem1, int elem2);

//打印双向链表
Status printList(pDLinkList &list);

//逆向构造一个双向链表
Status createList(pDLinkList &list, int n);

//双向表表list已存在，销毁线性表
Status destroyList(pDLinkList &list);

//双向表l已存在，将l置为空表
Status clearList(pDLinkList &list);

//双向表l已存在，若l为空表，则返回TRUE，否则返回FALSE
Status listEmpty(pDLinkList list);

//双向表l已存在， 返回l中的数据元素个数
Status listLength(pDLinkList list);

//双向表已存在，1 <= i <= listLength(l)
//用e返回l中第i个数据元素的值
template <class ElemType>
pDLinkList getElem(pDLinkList list, int i, ElemType &e)
{
    pDLinkList p = list->next; //初始化p指向第一个节点，j为计数器

    int j = 1;

    while((NULL != p) && (j < i))
    {
        p = p->next;
        ++j;

    }

    if((NULL == p) || j > i)
    {
        return NULL; 
    }

    e = p->data;

    return p;
}

Status equal(int e1, int e2);

//双向表已存在，compare是数据元素判定函数；
//返回l中第1个与e满足关系compare的数据元素的位序，若不在，则返回值为0
template <class ElemType>
pDLinkList locateElem(pDLinkList list, ElemType e, CompareFunc compare)    
{
    if(NULL == list)
    {
        puts("double is NULL");

        return 0;
        
    }

    pDLinkList p = list->next; 

    while(p && !compare(p->data, e))
    {
        p = p->next;
    }

    return p;
}

//双向表l已存在，若curElem是list的数据元素，且不是第一个，
//则使用preEle返回它的前驱，否则操作失败，preElem无定义
template <class ElemType>
Status priorElem( pDLinkList list, ElemType curElem, ElemType &preElem)
{
    pDLinkList p = NULL;

    if(NULL == list)
    {
        puts("double link list is NULL");

        return FALSE;
    }

    p = locateElem(list, curElem, equal);
    
    if((NULL == p) || (p == list->next))
    {
        printf("In current double link list can't find curElem: %d\n", curElem);

        return FALSE;
    }

    preElem = p->prior->data;
    

    return TRUE;
}

//双向表l已存在，若curElem是list的数据元素，且不是最后一个，
//则使用nextElem返回它的后继，否则操作失败，nextElem无定义
template <class ElemType>
Status nextElem(pDLinkList list, ElemType curElem, ElemType &nextElem)
{
    pDLinkList p = NULL;

    if(NULL == list)
    {
        puts("double link list is NULL");

        return FALSE;
    }

    p = locateElem(list, curElem, equal);

    if((NULL == p) || (NULL == p->next))
    {
        printf("In current double link list can't find curElem: %d\n", curElem);

        return FALSE;
    }

    nextElem = p->next->data;
    

    return TRUE;
}

//双向表已存在，1 <= i <= listLength(l) + 1
//在l中第i个位置之前插入新的元素e
template <class ElemType>
Status listInsert(pDLinkList &list, int i, ElemType e)          //algorithem2.18
{

    int tmp = -1;
    pDLinkList p = getElem(list, i, tmp);  //寻找i对应在双向链表中的位置

    if(NULL == p)
    {
        return ERROR;
    }

    pDLinkList newNode = (pDLinkList)malloc(sizeof(DLinkList));

    if(NULL == newNode)
    {
        exit(OVERFLOW);
    }

    newNode->data = e;
    newNode->prior = p->prior;
    p->prior->next = newNode;
    newNode->next = p;
    p->prior = newNode;

    return OK;
}

//双向表l已存在，且非空，1 <= i <= listLength(l)
//删除l的第i个数据元素，并且利用e返回其值，l的长度减1
template <class ElemType>
Status listDelete(pDLinkList &list, int i, ElemType &e)     //algorithem2.19
{
    int tmp = 0;
    pDLinkList p = getElem(list, i, tmp);;


    if(NULL == p)
    {
        return ERROR;
    }

    e = p->data;
    p->prior->next = p->next;
    p->next->prior = p->prior;

    free(p);
    p = NULL;
    
    return OK;
}

//将双向表la和lb归并到双向表la,lb的节点插入在la表尾处
Status mergeList(const pDLinkList &la, pDLinkList &lb);
