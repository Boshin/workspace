#include "link_list.h"

Status createList(linkList &head, int n)
{
    head = (linkList)malloc(n * sizeof(LNode));

    if(NULL == head)
    {
        printf("allocate failure...\n");

        exit(OVERFLOW);
    }

    head->next = NULL;

    for(int i = n; i >0; --i)
    {
        linkList p = (linkList)malloc(n * sizeof(LNode));

        if(NULL == p)
        {
            printf("allocate failure...\n");

            exit(OVERFLOW);
        }

        puts("input list dataValue: ");
        scanf("%d", &(p->data));

        p->next = head->next;
        head->next = p;
    }

    return OK;
}

Status destroyList(linkList &head)
{
    if(NULL == (head->next))
    {
        puts("link is NULL");

        return ERROR;
    }

    linkList p = head;
    linkList tmp = NULL;

    while(NULL != p)
    {
        tmp = p->next;
        free(p);
        p = tmp;
    }

    head = NULL;

    return OK;
}

//单链线性表la和lb的元素按值非递减排列
//归并la和lb得到新的单链线性表lc，lc的元素也是按值的非递减排列
void mergeList(const linkList &la, const linkList &lb, linkList &lc)  //algorithem2.12
{
    linkList pa = la->next;
    linkList pb = lb->next;
    linkList pc = la;       //使用la的头节点作为lc的头节点
    lc = pc;

    while(pa && pb)
    {
        if(pa->data <= pb->data)
        {
            pc->next = pa;
            pc = pa;
            pa = pa->next;
        }
        else
        {
            pc->next = pb;
            pc = pb;
            pb = pb->next;
        }
    }

    pc->next = pa ? pa : pb;

    free(lb);
}
