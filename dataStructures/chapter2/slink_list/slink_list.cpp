#include "slink_list.h"

void initSpace(sLinkList &space)
{
    for(int i = 0; i < MAXSIZE - 1; ++i)
    {
        space[i].cur = i + 1;
    }

    space[MAXSIZE - 1].cur = 0;
}

int mallocNode(sLinkList &space)
{
    int i = space[0].cur;

    if(space[0].cur)
    {
        space[0].cur = space[i].cur;

        return i;
    }

    return 0;
}

void freeNode(sLinkList &space, int k)
{
    if(k > (MAXSIZE - 1))
    {
        return;
    }

    space[k].cur = space[0].cur;
    space[0].cur = k;
}

void difference(sLinkList &space, int &S)
{
    initSpace(space);           //初始化备用空间
    S = mallocNode(space);      //生成S的头节点
    //printf("S: %d\n", S);
    int tall = S;               //tall指向S的当前最后的结点

    if(!S)
    {
        exit(-1);
    }

    int setACnt = 0;
    int setBCnt = 0;
    puts("please input set A count");
    scanf("%d", &setACnt);              //输入A的元素个数
    puts("please input set B count");
    scanf("%d", &setBCnt);             //输入B的元素个数

    //建立集合A的链表
    for(int j = 0; j < setACnt; ++j)
    {
        int i = mallocNode(space);
        int setAValue = 0;
        puts("输入A的元素值:");
        scanf("%d", &setAValue);
    //    printf("--i: %d, setAValue: %d\n", i, setAValue);
        space[i].data = setAValue;
        space[tall].cur = i;               //插入到表尾
        tall = i;
    }

    space[tall].cur = 0;                   //尾结点的指针为空
    //printList(space);

    for(int j = 0; j < setBCnt; ++j)
    {
        int setBValue = 0;
        puts("输入B的元素值:");
        scanf("%d", &setBValue);

        int p = 0;
        int k = space[S].cur;   //k指向集合A中第一个结点

        while((k != space[tall].cur) &&
              (space[k].data != setBValue)
             )
        {
            p = k;
            k = space[k].cur;
        }

        //当前表中不存在该元素，插入在tall所指结点之后，
        //且tall的位置不变
        if(k == space[tall].cur)   
        {
            int tmp = mallocNode(space);
            printf("tmp: %d\n", tmp);
            space[tmp].data = setBValue;
            space[tmp].cur = space[tall].cur;
            space[tall].cur = tmp;
        }
        else            //该元素已在表中，删除之
        {
            space[p].cur = space[k].cur;
            freeNode(space, k);

            if(tall == k)          //若删除的是r所指结点，则需修改尾指针
            {
                tall = p;
            }
        }
    }

    printList(space);
}

void printList(sLinkList &sl)
{
    int index = sl[1].cur;

    while(index)
    {
        printf("index: %d, data:%d\n", index, sl[index].data);
        index = sl[index].cur;
    }

    puts("\nprint over");
}
