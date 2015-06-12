#include <stdio.h>
#include <stdlib.h>

#include "link_list.h"

void printList(linkList &head)
{
    if(NULL == (head->next))
    {
        puts("link is NULL");

        return;
    }

    linkList p = head->next;

    while(NULL != p)
    {
        printf("%d ", p->data);

        p = p->next;
    }

    puts("");
}

int main(int argc, char *argv[])
{
    linkList head_a;
    linkList head_b;
    linkList head_c;

    //3 5 8 11
    createList(head_a, 4);
    printList(head_a);

    //2 6 8 9 11 15 20
    createList(head_b, 7);
    printList(head_b);

    puts("-------------getElem start--------------------");
    int elem = -1;
    getElem(head_a, 2, elem);
    printf("elem: %d\n", elem);
    puts("--------------getElem end-------------------\n");

    puts("--------------listInsert start-------------------");
    listInsert(head_a, 3, 7);
    printList(head_a);
    puts("---------------listInsert end------------------\n");

    puts("----------------listDelete start-----------------");
    listDelete(head_a, 3, elem);
    printf("elem: %d\n", elem);
    printList(head_a);
    puts("-----------------listDelete end----------------\n");

    puts("------------------mergeList start---------------");
    mergeList(head_a, head_b, head_c);
    printList(head_c);
    puts("------------------mergeList end---------------\n");

    //destroyList(head_a);
    printf("head_a: %p\n", head_a);
    printf("head_c: %p\n", head_c);
    
    //注意此时head_c和head_a指向的内存区域相同，
    //不可做重复释放操作，只可释放其中一个，
    //另一个赋为NULL
    head_a = NULL;
    destroyList(head_c);        

    printf("head_a: %p\n", head_a);
    printf("head_c: %p\n", head_c);

    return 0;
}
