#include "doubleLink_list.h"

void clearListTest(pDLinkList dList)
{
    printf("%d\n", listEmpty(dList));
    clearList(dList);
    printf("%d\n", listEmpty(dList));
}

void getListElem(pDLinkList dList)
{
    int data = -1;
    int priorData = -1;
    int nextData = -1;

    listLength(dList);
    getElem(dList, 3, data);
    printf("--data: %d\n", data);

    priorElem(dList, data, priorData);
    printf("priorData: %d\n", priorData);

    nextElem(dList, data, nextData);
    printf("nextData: %d\n", nextData);

}

int main(int argc, char *argv[])
{
    pDLinkList dList = NULL;

    createList(dList, 4);
    
    printList(dList);

//    getListElem(dList);

    listInsert(dList, 4, 11);
    printf("list len: %d\n", listLength(dList));
    printList(dList);

    int deleteElem = -1;
    listDelete(dList, 4, deleteElem);
    printf("list len: %d, deleteElem: %d\n", listLength(dList), deleteElem);
    printList(dList);

    pDLinkList dList2 = NULL;
    createList(dList2, 3);
    printList(dList2);

    mergeList(dList, dList2);
    printf("list len: %d, deleteElem: %d\n", listLength(dList), deleteElem);
    printList(dList);

    destroyList(dList);
    destroyList(dList2);

    return 0;
}

