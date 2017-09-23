#ifndef __FLINKLIST_H
#define __FLINKLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../MicroDefine.h"

template<class T>
struct LNode
{
    T data;
    struct LNode *next; 
};

typedef LNode<int>  linkNode;
typedef LNode<int> *Link;
typedef LNode<int> *Position;

typedef struct
{
    Link head;
    Link tail;
    int len;

}LinkList;

//��ӡ��������list
inline void printList(LinkList list)
{
    if(NULL == list.head)
    {
        puts("list is null");

        return;
    }

    while(list.head->next)
    {
        printf("%d ", list.head->next->data);
        list.head = list.head->next;
    }

    puts("\nprint over");
}

//������nodeָ���ֵΪe�Ľ�㣬������OK;
//������ʧ�ܣ��򷵻�ERROR
template<class T>
Status makeNode(Link &node, T e)
{
    node = (Link)malloc(sizeof(linkNode));
       
    if(NULL == node)
    {
        puts("allcoted memory failuer");

        return ERROR;
    }

    node->data = e;

    return OK;
}

//�ͷ�node��ָ�Ľڵ�
void freeNode(Link &node);

//����һ���յ���������list
Status initList(LinkList &list);

//�������Ա�list
Status destoryList(LinkList &list);

//�����Ա�list����Ϊ�ձ����ͷ�ԭ����Ľ��ռ�
Status clearList(LinkList &list);

//headָ�����������ͷ��㣬
//��s��ָ�Ľ������ڵ�һ�����֮ǰ
Status insertFirst(Link head, Link s);

//headָ�����������ͷ��㣬
//ɾ�������еĵ�һ����㲢��q����
Status deleteFirst(Link head, Link &q);

//��ָ����ָ��һ��������������Ա�list�����һ�����
//֮�󣬲��ı�����list��βָ��ָ���µ�β���
Status append(LinkList &list, Link s);

//ɾ�����������е�β��㲢��q���أ�
//�ı�����list��βָ��ָ���µ�β���
Status remove(LinkList &list, Link &q);

//pָ����������list�еĵ�һ����㣬��s
//��ָ��������p��ָ���֮ǰ��
//���޸�ָ��pָ���²���Ľ��
Status insertBefore(LinkList &list, Link &p, Link s);

//��֪pָ����������list�е�һ����㣬��s
//��ָ�Ľ�������p��ָ���֮�󣬲��޸�
//ָ��pָ���²���Ľ��
Status insertAfter(LinkList &list, Link &p, Link s);

//��֪pָ�����������е�һ����㣬��e����p��ָ����е�����Ԫ�ص�ָ
template<class ElemType>
Status setCurElem(Link &p, ElemType e)
{
    if(NULL == p)
    {
        return ERROR;
    }

    p->data = e;

    return OK;
}

//��֪pָ�����������е�һ����㣬����p��ָ����е�����Ԫ�ص�ֵ
template<class ElemType>
ElemType getCurElem(Link p)
{
    if(NULL == p)
    {
        return ERROR;
    }

    return p->data;
}

//�����Ա�listΪ�ձ��򷵻�TRUE���򷵻�FALSE
Status listEmpty(LinkList list);

//�������Ա�list��Ԫ�ظ���
int listLength(LinkList list);

//��������������ͷ�ڵ��λ��
Position getHead(LinkList list);

//��������������β����λ��
Position getLast(LinkList list);

//��֪pָ����������list�е�һ����㣬����p��ָ����ֱ��ǰ����λ��
//����ǰ�����򷵻�NULL
Position priorPos(LinkList list, Link p);

//��֪pָ����������list�е�һ����㣬����p��ָ����ֱ�Ӻ�ǵ�λ��
//���޺�̣��򷵻�NULL
Position nextPos(LinkList list, Link p);

//����pָʾ��������list�е�i������λ�ò�����OK��iֵ���Ϸ�ʱ����ERROR
Status locatePos(LinkList list, int i, Link &p);

//������������list�е�1����e���㺯��compare()�ж���ϵ��Ԫ�ص�λ�ã�
//��������������Ԫ�أ��򷵻�NULL
template<class ElemType>
Position locateElem(LinkList list, ElemType e, Status(*compare)(ElemType, ElemType))
{
    if(NULL == list.head->next) 
    {
        return NULL;
    }

    Link ptr = list.head->next;

    while(ptr) 
    {
        if(compare(e, ptr->data))
        {
            break;
        }

        ptr = ptr->next;
    }

    return ptr;
}

template<typename ElemType>
Status listInsert(LinkList &list, int i, ElemType e)
{
    Position pos = NULL;
    Link newNode = NULL;

    if(!locatePos(list, i, pos))
    {
        return ERROR;
    }

    if(!makeNode(newNode, e))
    {
        return ERROR;
    }

    return insertBefore(list, pos, newNode);    
}

#if 0
//���ζ�list��ÿ��Ԫ�ص��ú���visit��һ��visit()ʧ�ܣ������ʧ��
Status listTracerse(LinkList L, Status(*visit)());

#endif

#endif  //__FLINKLIST_H

