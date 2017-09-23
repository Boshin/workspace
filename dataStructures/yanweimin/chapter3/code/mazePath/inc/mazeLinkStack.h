#ifndef MAZELINKSTACK__H_
#define MAZELINKSTACK__H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MicroDefine.h"
#include "mazeLinkStack.h"

typedef struct maze_Type
{
    int row;
    int col;
    char (*pMap)[10];

    maze_Type()
        :row(0), col(0), pMap(NULL)
    {
    }

}mazeType;

typedef struct pos_Type
{
    int row;        //在迷宫地图中的行号
    int col;        //在迷宫地图中的列号

    Status operator==(const pos_Type &other)
    {
        if((this->row == other.row) &&
           (this->col == other.col))
        {
            return TRUE;
        }

        return FALSE;
    }
}posType;

typedef struct
{
    int ord;        //通道块在路径上的“序号”
    posType seat;   //通道块在迷宫中的“坐标位置”
    int di;         //从此通道块走向下一通道块的“方向”
}elemType;

template<typename ElemType>
struct mazeNode
{
    ElemType data;
    struct mazeNode *next;
};

typedef struct mazeNode<elemType> mazeStackNode;

struct mazeLinkStack
{
    mazeStackNode *base;        //在栈构造之前和销毁之后，base的值为NULL
    mazeStackNode *top;         //栈顶指针
    int stackSize;          //当前已分配的存储空间，以元素为单位
};

//构造一个空栈
Status initStack(mazeLinkStack &stack);

//销毁栈stack，stack不再存在
Status destoryStack(mazeLinkStack &stack);

//把stack置为空栈
Status clearStack(mazeLinkStack &stack);

//若栈stack为空栈，则返回TRUE，否则返回FALSE
Status stackEmpty(const mazeLinkStack &stack);

//返回stack的元素个数，即栈的长度
int stackLength(const mazeLinkStack &stack);

//若栈不为空，则用e返回stack的栈顶元素，并返回OK，否则返回ERROR
template <typename SElemType>
Status getTop(const mazeLinkStack &stack, SElemType &e)
{
    if(stack.base == stack.top)
    {
        puts("stack is empty");
        return ERROR;
    }

    e = stack.top->data;

    return OK;
}

//插入元素e为新的栈顶元素
template <typename SElemType>
Status push(mazeLinkStack &stack, SElemType e)
{

    mazeStackNode* newNode = (mazeStackNode*)malloc(sizeof(mazeStackNode));

    if(NULL == newNode)
    {
        exit(OVERFLOW);
    }

    newNode->data = e;
    newNode->next = stack.top;
    stack.top = newNode;
    stack.stackSize++;

    return OK;
}

//若栈不空，则删除stack的栈顶元素，用e返回其值，
//并返回OK，否则返回ERROR
template <typename SElemType>
Status pop(mazeLinkStack &stack, SElemType &e)
{
    if(stack.base == stack.top)
    {
        puts("stack is empty");
        return ERROR;
    }

    mazeStackNode* ptr = stack.top;
    e = ptr->data;
    free(ptr);

    stack.top = stack.top->next;
    stack.stackSize--;

    ptr = NULL;

    return OK;
}

#endif
