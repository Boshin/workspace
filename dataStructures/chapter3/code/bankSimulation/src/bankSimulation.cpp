#include <time.h>
#include <stdlib.h>

#include "bankSimulation.h"

void random(int &durTime, int &interTime);
int minimum(linkQueue *ptrQ);
void openForDay();
void customerArrived(int closeTime);
void customerDeparture();

void random(int &durTime, int &interTime)
{
   srand((int)time(0));
   durTime = 1 + (int)30.0 * rand() / (RAND_MAX + 1.0);
   interTime = 1 + (int)15.0 * rand() / (RAND_MAX + 1.0);
}

int minimum(linkQueue *ptrQ)
{
  int minQueue = queueLength(ptrQ[0]);
  int minIndex = 0;

  for(int i = 1; i < 5; ++i)
  {
      if(queueLength(ptrQ[i]) < minQueue)
      {
          minIndex = i;
      }
  }

  return minIndex;
}

void openForDay()
{
    //初始化操作
    totalTime = 0;              //初始化累计时间为0
    customerNum = 0;            //初始化客户数为0

    iniList(ev);                //初始化事件链表位空表

    en.occurTime = 0;           //设定一个客户到达事件
    en.nType = 0;

    orderInsert(ev, en, cmp);   //插入事件表

    for(int i = 0; i < 5; ++i)
    {
        initQueue(q[i]);         //置空队列：
    }
}

void customerArrived(int closeTime)
{
    int durTime = -1;               //客户办理事务所需时间
    int interTime = -1;             //下一个客户到达的时间间隔
    int nextArrivalTime = -1;        //下一个客户到达的时刻
    ++customerNum;

    random(durTime, interTime);           //生成随机数

    nextArrivalTime = en.occurTime + interTime;

    if(nextArrivalTime < closeTime)
    {
        Event tmpEn;
        tmpEn.occurTime = nextArrivalTime;
        tmpEn.nType = 0;

        orderInsert(ev, tmpEn, cmp);
    }

    int i = minimum(q);         //获取长度最短的队列

    customer.arrivalTime = nextArrivalTime;
    customer.duration = durTime;
    enQueue(q[i], customer);

    if(1 == queueLength(q[i]))
    {
        Event tmpEn2;
        tmpEn2.occurTime = nextArrivalTime + durTime;
        tmpEn2.nType = i;

        orderInsert(ev, tmpEn2, cmp);
    }
}

//处理客户离开事件
void customerDeparture()
{
  int i = en.nType;

  deQueue(q[i], customer);

  totalTime += en.occurTime - customer.arrivalTime;    //累计客户逗留时间

  //设定第i队列的一个离开事件并插入事件表
  if(queueEmpty(q[i]))
  {
      Event tmpEn;
      tmpEn.occurTime = en.occurTime + customer.duration;
      tmpEn.nType = i;
      getHead(q[i], customer);
      orderInsert(ev, en, cmp);
  }

}

void bank_simulation(int closeTime)
{
    openForDay();

    while(!listEmpty(ev))
    {
        Event tmpEn;
        delFirst(ev, tmpEn);

        if(0 == tmpEn.nType)
        {
            customerArrived(closeTime);
        }
        else
        {
            customerDeparture();
        }
    }
}
