#ifndef LINKQUEUE_H_
#define LINKQUEUE_H_

#include "MicroDefine.h"

typedef int qElemType;

typedef struct qNode
{
    qElemType data;
    struct qNode *next;
}qNode, *queuePtr;

typedef struct
{
    queuePtr front;    //队头指针
    queuePtr rear;     //队尾指针
}linkQueue;

//构造一个空队列q
Status initQueue(linkQueue &q);

//销毁队列q，q不再存在
Status destroyQueue(linkQueue &q);

//将q清为空队列
Status clearQueue(linkQueue &q);

//若队列q为空队列，则返回TRUE，否则返回FALSE
Status queueEmpty(const linkQueue &q);

//返回q得元素个数，既为队列的长度
int queueLength(const linkQueue &q);

//若列队不空，则用e返回的头元素，并返回OK，否则返回ERROR
Status getHead(const linkQueue &q, qElemType &e);

//插入元素e为q的新的队尾元素
Status enQueue(linkQueue &q, qElemType e);

//若队列不为空，则删除q的队头元素，用e返回其值，并返回OK
Status deQueue(linkQueue &q, qElemType e);

//从队头到队尾依次对队列q中的每个元素调用函数visit()，
//一旦visit失败，则操作失败
Status queueTraverse(const linkQueue &q, Status (*visit)());

#endif
