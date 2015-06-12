#ifndef LINEAR__H_
#define LINEAR__H_

#include <stdio.h>
#include <stdlib.h>

#include "MicroDefine.h"

#define STACK_INIT_SIZE 100     //存储空间初始化分配
#define STACKINCREMENT 10       //存储空间分配增量

template<typename SElemType>
struct linearStack
{
    SElemType *base;        //在栈构造之前和销毁之后，base的值为NULL
    SElemType *top;         //栈顶指针
    int stackSize;          //  当前已分配的存储空间，以元素为单位
};

typedef struct linearStack<int> SqStack;

//构造一个空栈
template <typename SElemType>
Status initStack(SqStack &stack)
{
    stack.base = (SElemType*)malloc(sizeof(SElemType) * STACK_INIT_SIZE);

    if(NULL == stack.base)
    {
        exit(OVERFLOW);
    }

    stack.top = stack.base;
    stack.stackSize = STACK_INIT_SIZE;

    return OK;
}

//销毁栈stack，stack不再存在
Status destoryStack(SqStack &stack);

//把stack置为空栈
Status clearStack(SqStack &stack);

//若栈stack为空栈，则返回TRUE，否则返回FALSE
Status stackEmpty(const SqStack &stack);

//返回stack的元素个数，即栈的长度
int stackLength(const SqStack &stack);

//若栈不为空，则用e返回stack的栈顶元素，并返回OK，否则返回ERROR
template <typename SElemType>
Status getTop(const SqStack &stack, SElemType &e)
{
    if(stack.base == stack.top)
    {
        puts("stack is empty");
        return ERROR;
    }

    e = *(stack.top - 1);

    return OK;
}

//插入元素e为新的栈顶元素
template <typename SElemType>
Status push(SqStack &stack, SElemType e)
{
    if(NULL == stack.base)
    {
        puts("stack is NULL");
        return ERROR;
    }

    if((stack.top - stack.base) >= STACK_INIT_SIZE)
    {
        stack.base = (SElemType)realloc(stack.base, 
                (stack.stackSize + STACKINCREMENT) * sizeof(SElemType));
    }

    if(NULL == stack.base)
    {
        exit(OVERFLOW);
    }

    stack.top = stack.base + stack.stackSize;
    stack.stackSize += STACKINCREMENT;

    *stack.top++ = e;

    return OK;
}

//若栈不空，则删除stack的栈顶元素，用e返回其值，
//并返回OK，否则返回ERROR
template <typename SElemType>
Status pop(SqStack &stack, SElemType e)
{
    if(stack.base == stack.top)
    {
        puts("stack is empty");
        return ERROR;
    }

    e = *--stack.top;

    return OK;
}

//从栈底到栈顶依次对栈中的每个元素调用函数visit(),一旦visit()失败,
//则操作失败
Status stackTraverse(SqStack stack, Status(*visit)());

#endif
