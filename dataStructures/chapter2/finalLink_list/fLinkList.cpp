#include "fLinkList.h"

void freeNode(Link &node)
{
    if(NULL != node)
    {
        free(node);
    }

    node = NULL;

}

Status initList(LinkList &list)
{
    int listSize = 0;
    int nodeValue = 0;
    Link newNode = NULL;

    list.head = NULL;
    list.tail = NULL;
    list.len = 0;

    if(ERROR == makeNode(newNode, 0))
    {
        puts("allcoted node space failure");
        exit(ERROR);
    }

    list.head = list.tail = newNode;
    list.len++; 

    puts("please input the link list size: ");
    scanf("%d", &listSize);

    for(int i = 0; i < listSize; ++i)
    {
        newNode = NULL;

        puts("please input the link list node value: ");
        scanf("%d", &nodeValue);

        if(ERROR == makeNode(newNode, nodeValue))
        {
            puts("allcoted node space failure");
            exit(ERROR);
        }
        
        newNode->next = NULL;

        //if(NULL == list.tail)
        //{
        //    list.head = newNode;
        //}
        //else
        //{
            list.tail->next = newNode;
        //}

        list.tail = newNode;
        list.len++;
    }
}

Status destoryList(LinkList &list)
{
    if(NULL == list.head)
    {
        return OK;
    }

    Link ptr = list.head;

    while(NULL != list.head)
    {
       ptr = list.head->next;

       freeNode(list.head);

       list.head = ptr;
       list.len--;
    }

    list.head = NULL;
    list.tail = NULL;
    printf("--len: %d\n", list.len);
}

Status clearList(LinkList &list)
{
    if(NULL == list.head->next)
    {
        return OK;
    }

    Link ptr = NULL;

    while(list.head->next)
    {
        ptr = list.head->next;
        list.head->next = ptr->next;
        list.len--;

        free(ptr);
    }

    list.tail = list.head;
    printf("--len: %d\n", list.len);
}

Status insertFirst(Link head, Link s)
{
    if(NULL == head)
    {
        puts("head is NULL");

        return ERROR;
    }

    s->next = head->next;
    head->next = s;

    return OK;
}

Status deleteFirst(Link head, Link &q)
{
    if(NULL == head)
    {
        puts("head is NULL");

        return ERROR;
    }
     
    memcpy(q, head->next, sizeof(linkNode));

    Link nodePtr = head->next;
    head->next = head->next->next;

    free(nodePtr);

    nodePtr = NULL;

    return OK;

}

Status append(LinkList &list, Link s)
{
    if(NULL == s)
    {
        puts("s is NULL");
        return ERROR;
    }

    if(list.head == list.tail) //listÎª¿Õ±í
    {
        list.head->next= s;
    }
    else
    {
        list.tail->next = s;
    }

    while(s->next)
    {
        s = s->next;
        list.len++;
    }

    list.tail = s;

    return OK;
} 

Status remove(LinkList &list, Link &q)
{
    if(NULL == list.head->next)
    {
        puts("list is NULL");

        return ERROR;
    }

    Link nodePtr = list.head->next;

    while(nodePtr->next != list.tail)
    {
        nodePtr = nodePtr->next;
    }
    
    memcpy(q, list.tail, sizeof(linkNode));
    free(list.tail);

    list.tail = nodePtr;
}

Status insertBefore(LinkList &list, Link &p, Link s)
{
   if((NULL == list.head->next) ||
      (NULL == p) ||
      (NULL == s)) 
   {
       return ERROR;
   }

   Link ptr = list.head->next;

   while((ptr->next != p) && (NULL != ptr))
   {
        ptr = ptr->next;
   }

   if(NULL == ptr)
   {
        return ERROR;
   }

   s->next = p;
   ptr->next = s;
   list.len++;
   p = s;

   return OK;
}

Status insertAfter(LinkList &list, Link &p, Link s)
{
   if((NULL == list.head->next) ||
      (NULL == p) ||
      (NULL == s)) 
   {
       return ERROR;
   }

   s->next = p->next;
   p->next = s;
   list.len++;
   p = s;

   return OK;
}

Status listEmpty(LinkList list)
{
    if(list.head == list.tail)
    {
        return TRUE;
    }

    return FALSE;
}

int listLength(LinkList list)
{
    return list.len;
}


Position getHead(LinkList list)
{
    return list.head;
}

Position getLast(LinkList list)
{
    return list.tail;
}

Position priorPos(LinkList list, Link p)
{
   if((NULL == list.head->next) || (NULL == p) ||
      (list.head->next == p)) 
   {
       return NULL;
   }

   Link ptr = list.head->next;

   while((ptr->next != p) && (NULL != ptr))
   {
        ptr = ptr->next;
   }

   return ptr;;
}

Position nextPos(LinkList list, Link p)
{
   if((NULL == list.head->next) || (NULL == p) ||
      (list.tail == p)) 
   {
       return NULL;
   }


   return p->next;
}

Status locatePos(LinkList list, int i, Link &p)
{
    if((NULL == list.head->next) || i >= (list.len - 1))
    {
        return ERROR;
    }

    int index = 0;
    Link ptr = list.head->next;


    while((index != i) && (NULL != ptr))
    {
        ptr = ptr->next;
        index++;
    }

    if(NULL == ptr)
    {
        return ERROR;
    }

    p = ptr;

    return OK;
}

