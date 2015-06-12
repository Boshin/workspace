#ifndef LOOPQUEUE_H_
#define LOOPQUEUE_H_

#include "MicroDefine.h"

#define MAXSIZE 100     //最大队列长度

typedef int qElemType;

typedef struct qNode
{
    qElemType *base;    //初始化的动态分配存储空间
    int front;          //头指针，若队列不空，指向队列头元素
    int rear;           //尾指针，若队列不空，指向队列尾元素的下一个位置

}sqQueue;

//构造一个空队列q
Status initQueue(sqQueue &q);

//销毁队列q，q不再存在
Status destroyQueue(sqQueue &q);

//将q清为空队列
Status clearQueue(sqQueue &q);

//若队列q为空队列，则返回TRUE，否则返回FALSE
Status queueEmpty(const sqQueue &q);

//返回q得元素个数，既为队列的长度
int queueLength(const sqQueue &q);

//若列队不空，则用e返回的头元素，并返回OK，否则返回ERROR
Status getHead(const sqQueue &q, qElemType &e);

//插入元素e为q的新的队尾元素
Status enQueue(sqQueue &q, qElemType e);

//若队列不为空，则删除q的队头元素，用e返回其值，并返回OK
Status deQueue(sqQueue &q, qElemType e);

//从队头到队尾依次对队列q中的每个元素调用函数visit()，
//一旦visit失败，则操作失败
Status queueTraverse(const sqQueue &q, Status (*visit)());

#endif


