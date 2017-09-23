#ifndef BANKSIMULATION_H_
#define BANKSIMULATION_H_

#include "linkList.h"
#include "linkQueue.h"

typedef linkList EventList;

Event en;               //�¼�
linkQueue q[5];         //4���ͻ�����
QElemType customer;     //�ͻ���¼
EventList ev = NULL;           //ʱ���
int totalTime = 0;      //�ۼƿͻ�����ʱ��
int customerNum = 0;    //�ͻ���

int cmp(const Event &a, const Event &b)
{
    if(a.occurTime < b.occurTime)
    {
        return -1;
    }
    else if(a.occurTime > b.occurTime)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void bank_simulation(int closeTime);

#endif
