#include <stdio.h>
#include <stdlib.h>

#include "linkQueue.h"

Status initQueue(linkQueue &q)
{
    //构造一个空队列
    q.front = q.rear = (queuePtr)malloc(sizeof(qNode));

    if(NULL == q.front)
    {
        puts("allocate memory failure");
        exit(OVERFLOW);
    }

    q.front->next = NULL;

    return OK;
}

Status destoryQueue(linkQueue &q)
{
    //销毁队列q

    while(q.front)
    {
        q.rear = q.front->next;
        free(q.front);
        q.front = q.rear;
    }

    return OK;
}

Status clearQueue(linkQueue &q)
{
    //清空队列q

    queuePtr ptr = q.front->next;

    while(ptr)
    {
        q.rear = ptr->next;
        free(ptr);
        ptr = q.rear;
    }

    q.rear = q.front;
}

Status queueEmpty(const linkQueue &q)
{
    return q.front == q.rear;
}

Status getHead(const linkQueue &q, qElemType &e)
{
    if(queueEmpty(q))
    {
        puts("queue is NULL");

        return ERROR;
    }

    e = q.front->next->data;

    return OK;
}

Status enQueue(linkQueue &q, qElemType e)
{
    queuePtr ptr = (queuePtr)malloc(sizeof(qNode));

    if(NULL == ptr)
    {
        puts("allocate memory failure");
        exit(OVERFLOW);
    }

    ptr->data = e;
    q.rear->next = ptr;
    q.rear = ptr;

    return OK;

}

Status deQueue(linkQueue &q, qElemType &e)
{
    if(queueEmpty(q))
    {
        puts("queue is NULL");
        return ERROR;
    }

    queuePtr ptr = q.front->next;
    e = ptr->data;
    q.front->next = ptr->next;

    if(ptr == q.rear)
    {
        q.rear = q.front;
    }

    free(ptr);
    ptr = NULL;

    return OK;
}

