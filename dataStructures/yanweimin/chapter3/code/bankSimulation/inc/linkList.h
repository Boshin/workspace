#include "MicroDefine.h"

typedef struct
{
    int occurTime;      //事件发生事件
    int nType;          //事件类型，0表示到达事件，1至4表示4个窗口的离开事件
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

//删除list链表中的第一个节点并用delEvent返回删除的节点内容
Status delFirst(linkList &list, Event &delEvent);

//判断list链表是否为空,如果为空返回TRUE，否者返回FALSE
Status listEmpty(const linkList &list);
