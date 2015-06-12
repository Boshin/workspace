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
    //��ʼ������
    totalTime = 0;              //��ʼ���ۼ�ʱ��Ϊ0
    customerNum = 0;            //��ʼ���ͻ���Ϊ0

    iniList(ev);                //��ʼ���¼�����λ�ձ�

    en.occurTime = 0;           //�趨һ���ͻ������¼�
    en.nType = 0;

    orderInsert(ev, en, cmp);   //�����¼���

    for(int i = 0; i < 5; ++i)
    {
        initQueue(q[i]);         //�ÿն��У�
    }
}

void customerArrived(int closeTime)
{
    int durTime = -1;               //�ͻ�������������ʱ��
    int interTime = -1;             //��һ���ͻ������ʱ����
    int nextArrivalTime = -1;        //��һ���ͻ������ʱ��
    ++customerNum;

    random(durTime, interTime);           //���������

    nextArrivalTime = en.occurTime + interTime;

    if(nextArrivalTime < closeTime)
    {
        Event tmpEn;
        tmpEn.occurTime = nextArrivalTime;
        tmpEn.nType = 0;

        orderInsert(ev, tmpEn, cmp);
    }

    int i = minimum(q);         //��ȡ������̵Ķ���

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

//����ͻ��뿪�¼�
void customerDeparture()
{
  int i = en.nType;

  deQueue(q[i], customer);

  totalTime += en.occurTime - customer.arrivalTime;    //�ۼƿͻ�����ʱ��

  //�趨��i���е�һ���뿪�¼��������¼���
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
