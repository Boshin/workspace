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

//�������Ա�la��lb��Ԫ�ذ�ֵ�ǵݼ�����
//�鲢la��lb�õ��µĵ������Ա�lc��lc��Ԫ��Ҳ�ǰ�ֵ�ķǵݼ�����
void mergeList(const linkList &la, const linkList &lb, linkList &lc)  //algorithem2.12
{
    linkList pa = la->next;
    linkList pb = lb->next;
    linkList pc = la;       //ʹ��la��ͷ�ڵ���Ϊlc��ͷ�ڵ�
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
