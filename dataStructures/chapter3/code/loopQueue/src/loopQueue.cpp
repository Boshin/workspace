#include <stdio.h>
#include <stdlib.h>

#include "loopQueue.h"

Status initQueue(sqQueue &q)
{
  q.base = (qElemType*)malloc(sizeof(qElemType) * MAXSIZE);

  if(NULL == q.base)
  {
      puts("allocate memory failure");

      exit(OVERFLOW);
  }

  q.front = q.rear = 0;

  return OK;
}

Status destroyQueue(sqQueue &q)
{
    if(NULL != q.base)
    {
        free(q.base);
        q.base = NULL;
    }
    
    q.front = q.rear = 0;

    return OK;
}

Status clearQueue(sqQueue &q)
{
    if(NULL == q.base)
    {
        puts("queue is NULL");
        return ERROR;
    }
    
    q.front = q.rear = 0;

    return OK;
}

Status queueEmpty(const sqQueue &q)
{
    return q.front == q.rear;
}

int queueLength(const sqQueue &q)
{
    return (q.rear - q.front + MAXSIZE) % MAXSIZE;
}

Status enQueue(sqQueue &q, qElemType e)
{
    if((q.rear + 1) % MAXSIZE == q.front)
    {
        puts("queue is full");
        return ERROR;
    }

    q.base[q.rear] = e;
    q.rear = (q.rear +1 ) % MAXSIZE;

    return OK;
}

Status deQueue(sqQueue &q, qElemType &e)
{
    if(queueEmpty(q))
    {
        puts("queue is empty");
        return ERROR;
    }

    e = q.base[q.front];
    q.front = (q.front + 1) % MAXSIZE;

    return OK;
}
