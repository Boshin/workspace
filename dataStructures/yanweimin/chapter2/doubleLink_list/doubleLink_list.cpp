#include "doubleLink_list.h"

Status printList(pDLinkList &list)
{
    if(NULL == list)
    {
        puts("double link list is NULL");

        return ERROR;
    }

   pDLinkList p = NULL;
   pDLinkList tmp = list;

    while(list)
    {
       p = list->next; 

       if('*' != list->data)
       {
           printf("%d ", list->data);
       }

       list = p;
    }

    p = NULL;
    list = tmp;

    puts("\nprint over...");

    return OK;
}

Status equal(int e1, int e2)
{
    if(e1 == e2)
    {
        return TRUE;
    }

    return FALSE;
}

Status createList(pDLinkList &head, int n)
{
    head = (pDLinkList)malloc(sizeof(DLinkList));

    if(NULL == head)
    {
        puts("allocate memory space failure...");
        exit(OVERFLOW);
    }

    head->prior = NULL;
    head->next = NULL;
    head->data = '*';

    //若顺序建立双向链表则链接顺序是相反的，
    //按照常规链接顺顺序则需要从表尾到表头逆向建立双向链表
    //for(int i = 0; i < n; ++i) 
    for(int i = n; i > 0; --i)
    {
        pDLinkList newNode = (pDLinkList)malloc(sizeof(DLinkList));
        
        if(NULL == newNode)
        {
            puts("new node allocate memory failure...");
            exit(OVERFLOW);
        }

        puts("please input new node value: ");
        scanf("%d", &(newNode->data));

        if(NULL != head->next)
        {
           head->next->prior = newNode; 
        }

        newNode->next = head->next;
        newNode->prior = head;
        head->next = newNode;

    }

    return OK;
}

Status destroyList(pDLinkList &list)
{
    if(NULL == list)
    {
        puts("double link list is NULL");

        return OK;
    }

   pDLinkList p = NULL;

    while(list)
    {
       p = list->next; 
       free(list);
       list = p;
    }

    p = NULL;
    list = NULL;

    return OK;
}

Status clearList(pDLinkList &list)
{
    if(NULL == list)
    {
        puts("list is NULL...");
        
        return ERROR;
    }

    pDLinkList p = list->next;

    while(p)
    {
        p->data = '*';
        p = p->next;
    }

    return OK;
}

Status listEmpty(pDLinkList list)
{
    if(NULL == list)
    {
        puts("list is NULL");

        return TRUE;
    }

    pDLinkList p = list->next;

    while(p)
    {
        if('*' != p->data)
        {
            return FALSE;
        }

        p = p->next;
    }

    return TRUE;
}

Status listLength(pDLinkList list)
{
    int i = 0;
    pDLinkList p = list->next;

    while(p)
    {
        if('*' != p->data)
        {
            ++i;
            p = p->next;
        }
    }

    printf("double list length: %d\n", i);

    return i;
}

Status mergeList(const pDLinkList &la, pDLinkList &lb)
{
    if((NULL == la) || (NULL == lb))
    {
        puts("double link list is NULL");
        
        return ERROR;
    }

    if(NULL == lb->next)
    {
        puts("double link list is NULL");

        return ERROR;
    }

    pDLinkList p = la->next;

    while(p->next)
    {
        p = p->next;
    }

    p->next = lb->next;
    lb->next->prior = p;
    lb->next = NULL;

    return OK;
}
