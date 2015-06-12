#include "linearList.h"

void printList(SqList &list)
{
    for(int i = 0; i < list.length; ++i)
    {
        printf("%d ", list.elem[i]);
    }

    puts("");
}

void callAlgorithem2_1(SqList &la, SqList &lb)
{
    listUnion(la, lb);
    printf("la length: %d\n", la.length);
    printList(la);
}

void callAlgorithem2_2(SqList &la, SqList &lb, SqList &lc)
{
    mergeList(la, lb, lc);
    printf("lc length: %d\n", lc.length);
    printList(lc);
}

void callAlgorithem2_7(SqList &la, SqList &lb, SqList &lc)
{
    mergeList_2_7(la, lb, lc, int(0));
    printf("lc length: %d\n", lc.length);
    printList(lc);
}

int main(int argc, char *charv[])
{

    SqList la;
    SqList lb;
    SqList lc;
    int tmpElem = -1;

    initList(la, int(0));
    initList(lb, int(0));

    listInsert(la, 1, 3);
    listInsert(la, 2, 5);
    listInsert(la, 3, 8);
    listInsert(la, 4, 11);

    listInsert(lb, 1, 2);
    listInsert(lb, 2, 6);
    listInsert(lb, 3, 8);
    listInsert(lb, 4, 9);
    listInsert(lb, 5, 11);
    listInsert(lb, 6, 15);
    listInsert(lb, 7, 20);

    printf("la length: %d\n", la.length);
    printf("lb length: %d\n", lb.length);
    printList(la);
    printList(lb);

    //callAlgorithem2_1(la, lb);
    //callAlgorithem2_2(la, lb, lc);
    callAlgorithem2_7(la, lb, lc);

    listDelete(lc, 0, tmpElem); 
    printf("tmpElem: %d\n", tmpElem);
    printList(lc);

    destroyList(la); 
    destroyList(lb); 
    destroyList(lc); 

    return 0;
}

#if 0
int main()
{
    int i = 11;
    SqList mylist;    
    mylist.elem = &i;
    return 0;
}
#endif
