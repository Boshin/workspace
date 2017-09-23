#include "linearList.h"

Status equal(int e1, int e2)
{
    if(e1 == e2)
    {
        return TRUE;
    }

    return FALSE;
}

Status destroyList(SqList &list)
{
    if(NULL != list.elem)
    {
        free(list.elem);
        list.elem = NULL;
    }

    return OK;
}

Status clearLis(SqList &list)
{
    if(NULL != list.elem)
    {
        memset(list.elem, 0, LIST_INIT_SIZE);
    }

    return OK;
}

Status listEmpty(SqList list)
{
    if(0 == list.length)
    {
        return TRUE;
    }

    return FALSE;
}

Status listLength(SqList list)
{
    return list.length;
}

void listUnion(SqList &la, SqList &lb)
{
    int laLen = listLength(la);
    int lbLen = listLength(lb);
    int elem = -1;

    for(int i = 0; i < lbLen; ++i)
    {
        getElem(lb, i, elem);

        if(!locateElem(la, elem, equal))
        {
            listInsert(la, ++laLen, elem);
        }
    }
}

void mergeList(const SqList &la, const SqList &lb, SqList &lc)
{
	dd;
    initList(lc, (int)0);

    int laLen = listLength(la);
    int lbLen = listLength(lb);
    int i = 0;
    int j = 0;
    int k = 0;
    int laElem = -1;
    int lbElem = -1;

    while((i < laLen) && (j < lbLen))
    {
        getElem(la, i, laElem);
        getElem(lb, j, lbElem);
        printf("i: %d  laElem: %d, j: %d  lbElem: %d\n", i, laElem, j, lbElem);
        
        if(laElem <= lbElem)
        {
            listInsert(lc, ++k, laElem);
            ++i;
        }
        else
        {
            listInsert(lc, ++k, lbElem);
            ++j;
        }
    }

    while(i < laLen)
    {
        getElem(la, i, laElem);
        listInsert(lc, ++k, laElem);
        ++i;
    }

    while(j < lbLen)
    {
        getElem(lb, j, lbElem);
        listInsert(lc, ++k, lbElem);
        ++j;
    }
}

