#include "MicroDefine.h"

typedef struct
{
    int occurTime;      //�¼������¼�
    int nType;          //�¼����ͣ�0��ʾ�����¼���1��4��ʾ4�����ڵ��뿪�¼�
}Event, ElemType;

typedef struct Node 
{
   Event data;
   struct Node *next; 
}lNode, *linkList;

typedef int (*cmpFunc)(const Event &a, const Event &b);

Status iniList(linkList &list);
Status destoryList(linkList &list);
Status orderInsert(linkList &list, const Event &newEvent, cmpFunc cmp);

//ɾ��list�����еĵ�һ���ڵ㲢��delEvent����ɾ���Ľڵ�����
Status delFirst(linkList &list, Event &delEvent);

//�ж�list�����Ƿ�Ϊ��,���Ϊ�շ���TRUE�����߷���FALSE
Status listEmpty(const linkList &list);
