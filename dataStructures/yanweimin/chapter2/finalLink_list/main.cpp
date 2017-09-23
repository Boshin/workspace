#include "fLinkList.h"

void testInsert(LinkList &list)
{
    Link newNode = NULL;
    makeNode(newNode, 9);
   
    Link tmpPtr = list.head->next->next;

    printf("tmpPrt: %p, newNode: %p\n", tmpPtr, newNode);
    insertBefore(list, tmpPtr, newNode);
    printf("tmpPrt: %p\n", tmpPtr);
    newNode = NULL;
    tmpPtr = NULL;
    printList(list);

    makeNode(newNode, 10);
    tmpPtr = list.tail;
    printf("tmpPrt: %p, newNode: %p\n", tmpPtr, newNode);
    insertAfter(list, tmpPtr, newNode);
    printf("tmpPrt: %p\n", tmpPtr);
    newNode = NULL;
    tmpPtr = NULL;
    printList(list);
}

int main(int argc, char* argv[])
{
    LinkList finalList;
    
    puts("init link list");
    initList(finalList);
    printList(finalList);

    testInsert(finalList);

    puts("\nclear link list");
    clearList(finalList);
    printList(finalList);

    puts("\ndestory link list");
    destoryList(finalList);
    printList(finalList);

    return 0;
}
