#ifndef BANKSIMULATION_H_
#define BANKSIMULATION_H_

#include "linkList.h"
#include "linkQueue.h"

typedef linkList EventList;

Event en;               //事件
linkQueue q[5];         //4个客户队列
QElemType customer;     //客户纪录
EventList ev = NULL;           //时间表
int totalTime = 0;      //累计客户逗留时间
int customerNum = 0;    //客户数

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
