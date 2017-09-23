#ifndef __SLINK_LIST_H
#define __SLINK_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "../../MicroDefine.h"

#define MAXSIZE 100

template <class ElemType>
struct staticLinkList
{
    ElemType data;
    int cur;
};

typedef struct staticLinkList<int> sLinkList[MAXSIZE];

//将一维数组space中各分量链成一个备用链表，
//space[0].cur为头指针
void initSpace(sLinkList &space);   //algorithem2.14

//若备用空间链表非空，则从备用链表上取得第一个节点 i 作为待
//插入的新结点，否则返回0
int mallocNode(sLinkList &space);   //algorithem2.15


//将下表为k的空闲结点回收到备用链表,并重新作为备用链表的第一个结点
void freeNode(sLinkList &space, int k);     //algorithem2.16

//依次输入集合A和B的元素，在一维数组space中建立表示集合（A - B）U (B -A )
//的静态链表，S为其头指针。假设备用空间足够大，space[0].cur为其头指针
void difference(sLinkList &space, int &S);

//在静态单链线性表中查找第1个值为e的元素
//若找到，则返回在在sl中的位序，否则返回0
template<class ElemType>
int locateElem(sLinkList &sl, ElemType e)       //algorithem2.13
{
    int i = sl[0].cur;

    while(i && (sl[i].data != e))
    {
        i = sl[i].cur;
    }

    return i;
}

void printList(sLinkList &sl);

#endif
