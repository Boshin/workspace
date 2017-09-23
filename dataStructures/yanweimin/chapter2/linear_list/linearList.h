#ifndef _LINEARLIST_H
#define _LINEARLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typeinfo>

#include "MicroDefine.h"

#define LIST_INIT_SIZE 100  //线性表存储空间的初始分配量
#define LISTINCREMENT 10    //线性表存储空间的分配增量

//template <class T>
//typedef Status (*CompareFunc)(T elem1, T elem2);
typedef Status (*CompareFunc)(int elem1, int elem2);

template <class T>
struct List
{
    T elem;            //存储空间基址
    int length;         //当前长度
    int listSize;       //当前分配的存储空间容量(以sizeof（T)为单位）
};

typedef struct List<int*> SqList;

//构造一个空的线性表
template<class ElemType>
Status initList(SqList &list, ElemType)         //algorithem2.3
{
    //printf("type: %s\n", typeid(ElemType).name());
    list.elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));

    if(NULL == list.elem)
    {
        printf("allocate failure...\n");

        exit(OVERFLOW);
    }

    list.length = 0;        //空表长度为0
    list.listSize = LIST_INIT_SIZE;      //初始存储容量

    //printf("list: %p\n", list);

    return OK;
}

//线性表list已存在，销毁线性表
Status destroyList(SqList &list);

//线性表l已存在，将l置为空表
Status clearLis(SqList &list);

//线性表l已存在，若l为空表，则返回TRUE，否则返回FALSE
Status listEmpty(SqList list);

//线性表l已存在， 返回l中的数据元素个数
Status listLength(SqList list);

//线性表已存在，1 <= i <= listLength(l)
//用e返回l中第i个数据元素的值
template <class ElemType>
Status getElem(SqList list, int i, ElemType &e)
{
    if(NULL != list.elem && i >= 0 && i <= list.length)
    {
        e = list.elem[i];

        return  TRUE;
    }

    return FALSE;
}

//线性表已存在，compare是数据元素判定函数；
//返回l中第1个与e满足关系compare的数据元素的位序，若不在，则返回值为0
template <class ElemType>
Status locateElem(SqList l, ElemType e, CompareFunc compare)    //algorithem2.6
{
    //自己实现的方法
#if 0
    int listLen = listLength(l);
    int tmpElem = -1;

    for(int i = 0; i < listLen; ++i)
    {
        getElem(l, i, tmpElem);
        
        if(compare(tmpElem, e))
        {
            return i;
        }
    }
#endif

    //参照书本算法2.6，
    //经过比较一下自己想的算法性能上还是略差思想也较一般；多思考多动脑
#if 1
    int i = 0;
    ElemType *pos = l.elem;

    while(i < l.length && !compare(*pos++, e))
    {
        ++i;
    }

    if(i < l.length)
    {
        return i;
    }
    else
    {
        return 0;
    }
#endif

    return FALSE;
}

//线性表l已存在，若curElem是l的数据元素，且不是第一个，
//则使用preEle返回它的前驱，否则操作失败，preElem无定义
template <class T, class ElemType>
Status priorElem(T l, ElemType curElem, ElemType &preElem);

//线性表l已存在，若curElem是l的数据元素，且不是最后一个，
//则使用nextElem返回它的后继，否则操作失败，nextElem无定义
template <class T, class ElemType>
Status nextElem(T l, ElemType curElem, ElemType &nextElem);

//线性表已存在，1 <= i <= listLength(l) + 1
//在l中第i个位置之前插入新的元素e，l的长度加1
template <class ElemType>
Status listInsert(SqList &list, int i, ElemType e)          //algorithem2.4
{
    if(NULL == list.elem || ((i < 1) && (i > list.length + 1)))
    {
        return ERROR;
    }

    if(list.length >= list.listSize)
    {
        ElemType* newAddr = (ElemType *)realloc(list.elem, (list.listSize + LISTINCREMENT) * sizeof(ElemType));

        if(NULL == newAddr)
        {
            exit(OVERFLOW);
        }

        list.elem = newAddr;
        list.listSize += LISTINCREMENT;
    }

    ElemType *insertPos = &(list.elem[i - 1]);
    ElemType *startPos = &(list.elem[list.length - 1]);

    for(; startPos >= insertPos; --startPos) //插入位置及之后的元素右移
    {
        *(startPos + 1) = *startPos;
    }
     
     *insertPos = e;
     ++list.length;

     return OK;
}

//线性表l已存在，且非空，1 <= i <= listLength(l)
//删除l的第i个数据元素，并且利用e返回其值，l的长度减1
template <class ElemType>
Status listDelete(SqList &l, int i, ElemType &e)
{
    if((i < 0) || i >= listLength(l))
    {
        return ERROR;
    }

    e = l.elem[i];

    ElemType *tmpPos = l.elem;

    ElemType *startPos = &(l.elem[i]) + 1;
    ElemType *endPos = tmpPos + l.length - 1;

    //原来自己真的有脑抽的毛病，为什么要经常多加分号呢，二货真心伤不起。。。。。
    //for(++startPos; startPos <= endPos; ++startPos);
    for(; startPos <= endPos; ++startPos)//;
    {
        *(startPos - 1) = *startPos;
    }

    --l.length;

    return OK;
}

//线性表la，lb已存在，扩大la线性表，将存在于lb中而不存在于la中
//的数据元素插入到la中
void listUnion(SqList &la, SqList &lb);                         //algorithem2.1

//将线性表la和lb归并为一个新的线性表lc，la和lb的数据元素
//按值非递减有序排列，且lc中的数据元素仍按值非递减有序排列
void mergeList(const SqList &la, const SqList &lb, SqList &lc);  //algorithem2.2

template<class ElemType>
void mergeList_2_7(const SqList &la, const SqList &lb, SqList &lc, ElemType)  //algorithem2.7
{
    ElemType *pa = la.elem;
    ElemType *pb = lb.elem;
    lc.listSize = lc.length = la.length + lb.length;

    ElemType *pc = lc.elem = (ElemType*)malloc(lc.listSize * sizeof(ElemType));

    if(NULL == lc.elem)
    {
        exit(OVERFLOW);
    }

    ElemType *paEnd = la.elem + la.length - 1;
    ElemType *pbEnd = lb.elem + lb.length - 1;

    while((pa <= paEnd) && (pb <= pbEnd))
    {
        if(*pa <= *pb)
        {
            *pc++ = *pa++;
        }
        else
        {
            *pc++ = *pb++;
        }
    }

    while(pa <= paEnd)
    {
        *pc++ = *pa++;
    }

    while(pb <= pbEnd)
    {
        *pc++ = *pb++;
    }
}

#endif
