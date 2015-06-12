#include <stdio.h>
#include <stdlib.h>

#include "linkList.h"

Status iniList(linkList &list)
{
    list = (linkList)malloc(sizeof(lNode));

    if(NULL == list)
    {
        puts("allocate memory failure...");
        exit(OVERFLOW);
    }

    list->next = NULL;
    list->data.occurTime = -1;
    list->data.nType = -1;

    return OK;
}

Status destoryList(linkList &list)
{
    linkList ptr = NULL;

    while(list)
    {
        ptr = list->next;
        free(list);
        list = ptr;
    }

    ptr = NULL;
    list = NULL;

    return OK;
}

Status orderInsert(linkList &list, const Event &newEvent, cmpFunc cmp)
{
    if(NULL == list)
    {
        puts("list is not init");

        return ERROR;
    }

   lNode *newNode = (lNode*)malloc(sizeof(lNode)); 

   if(NULL == newNode)
   {
       puts("allocate memory failure...");
       exit(OVERFLOW);
   }

   newNode->next = NULL;
   newNode->data = newEvent;

   if(NULL == list->next)
   {
       list->next = newNode;
   }
   else
   {
        linkList ptr = list->next;

        while((NULL != ptr) && (-1 == cmp(ptr->data, newNode->data)))
        {
            ptr = ptr->next;
        }

        newNode->next = ptr->next;
        ptr->next = newNode;
   }
    
   return OK;
}


Status delFirst(linkList &list, Event &delEvent)
{
    if(listEmpty(list))
    {
        puts("list is empty");

        return ERROR;
    }

    lNode *ptr = list->next;
    list->next = ptr->next;

    delEvent.occurTime = ptr->data.occurTime;
    delEvent.nType = ptr->data.nType;

    free(ptr);
    ptr = NULL;

    return OK;
}


Status listEmpty(const linkList &list)
{
    if(NULL == list->next)
    {
        return TRUE;
    }
    
    return FALSE;
}
