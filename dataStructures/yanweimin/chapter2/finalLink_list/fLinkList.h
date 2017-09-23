#ifndef __FLINKLIST_H
#define __FLINKLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../MicroDefine.h"

template<class T>
struct LNode
{
    T data;
    struct LNode *next; 
};

typedef LNode<int>  linkNode;
typedef LNode<int> *Link;
typedef LNode<int> *Position;

typedef struct
{
    Link head;
    Link tail;
    int len;

}LinkList;

//打印线性链表list
inline void printList(LinkList list)
{
    if(NULL == list.head)
    {
        puts("list is null");

        return;
    }

    while(list.head->next)
    {
        printf("%d ", list.head->next->data);
        list.head = list.head->next;
    }

    puts("\nprint over");
}

//分配由node指向的值为e的结点，并返回OK;
//若分配失败，则返回ERROR
template<class T>
Status makeNode(Link &node, T e)
{
    node = (Link)malloc(sizeof(linkNode));
       
    if(NULL == node)
    {
        puts("allcoted memory failuer");

        return ERROR;
    }

    node->data = e;

    return OK;
}

//释放node所指的节点
void freeNode(Link &node);

//构造一个空的线性链表list
Status initList(LinkList &list);

//销毁线性表list
Status destoryList(LinkList &list);

//将线性表list重置为空表，并释放原链表的结点空间
Status clearList(LinkList &list);

//head指向线性链表的头结点，
//将s所指的结点插入在第一个结点之前
Status insertFirst(Link head, Link s);

//head指向线性链表的头结点，
//删除链表中的第一个结点并以q返回
Status deleteFirst(Link head, Link &q);

//将指针所指的一串结点链接在线性表list的最后一个结点
//之后，并改变链表list的尾指针指向新的尾结点
Status append(LinkList &list, Link s);

//删除线性链表中的尾结点并以q返回，
//改变链表list的尾指针指向新的尾结点
Status remove(LinkList &list, Link &q);

//p指向线性链表list中的的一个结点，将s
//所指结点插入在p所指结点之前，
//并修改指针p指向新插入的结点
Status insertBefore(LinkList &list, Link &p, Link s);

//已知p指向线性链表list中的一个结点，将s
//所指的结点插入在p所指结点之后，并修改
//指针p指向新插入的结点
Status insertAfter(LinkList &list, Link &p, Link s);

//已知p指向线性链表中的一个结点，用e更新p所指结点中的数据元素的指
template<class ElemType>
Status setCurElem(Link &p, ElemType e)
{
    if(NULL == p)
    {
        return ERROR;
    }

    p->data = e;

    return OK;
}

//已知p指向线性链表中的一个结点，返回p所指结点中的数据元素的值
template<class ElemType>
ElemType getCurElem(Link p)
{
    if(NULL == p)
    {
        return ERROR;
    }

    return p->data;
}

//若线性表list为空表，则返回TRUE否则返回FALSE
Status listEmpty(LinkList list);

//返回线性表list中元素个数
int listLength(LinkList list);

//返回线性链表中头节点的位置
Position getHead(LinkList list);

//返回线性链表中尾结点的位置
Position getLast(LinkList list);

//已知p指向线性链表list中的一个结点，返回p所指结点的直接前驱的位置
//若无前驱，则返回NULL
Position priorPos(LinkList list, Link p);

//已知p指向线性链表list中的一个结点，返回p所指结点的直接后记的位置
//若无后继，则返回NULL
Position nextPos(LinkList list, Link p);

//返回p指示线性链表list中第i个结点的位置并返回OK，i值不合法时返回ERROR
Status locatePos(LinkList list, int i, Link &p);

//返回线性链表list中第1个与e满足函数compare()判定关系的元素的位置，
//若不存在这样的元素，则返回NULL
template<class ElemType>
Position locateElem(LinkList list, ElemType e, Status(*compare)(ElemType, ElemType))
{
    if(NULL == list.head->next) 
    {
        return NULL;
    }

    Link ptr = list.head->next;

    while(ptr) 
    {
        if(compare(e, ptr->data))
        {
            break;
        }

        ptr = ptr->next;
    }

    return ptr;
}

template<typename ElemType>
Status listInsert(LinkList &list, int i, ElemType e)
{
    Position pos = NULL;
    Link newNode = NULL;

    if(!locatePos(list, i, pos))
    {
        return ERROR;
    }

    if(!makeNode(newNode, e))
    {
        return ERROR;
    }

    return insertBefore(list, pos, newNode);    
}

#if 0
//依次对list的每个元素调用函数visit，一旦visit()失败，则操作失败
Status listTracerse(LinkList L, Status(*visit)());

#endif

#endif  //__FLINKLIST_H

