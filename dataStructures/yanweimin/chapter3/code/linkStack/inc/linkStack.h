#ifndef LINKSTACK__H_
#define LINKSTACK__H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MicroDefine.h"

template<typename ElemType>
struct node
{
    ElemType data;
    struct node *next;
};

typedef struct node<int> stackNode;

struct linkStack
{
    stackNode *base;        //在栈构造之前和销毁之后，base的值为NULL
    stackNode *top;         //栈顶指针
    int stackSize;          //当前已分配的存储空间，以元素为单位
};


//构造一个空栈
Status initStack(linkStack &stack);

//销毁栈stack，stack不再存在
Status destoryStack(linkStack &stack);

//把stack置为空栈
Status clearStack(linkStack &stack);

//若栈stack为空栈，则返回TRUE，否则返回FALSE
Status stackEmpty(const linkStack &stack);

//返回stack的元素个数，即栈的长度
int stackLength(const linkStack &stack);

//若栈不为空，则用e返回stack的栈顶元素，并返回OK，否则返回ERROR
template <typename SElemType>
Status getTop(const linkStack &stack, SElemType &e)
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
Status push(linkStack &stack, SElemType e)
{

    stackNode* newNode = (stackNode*)malloc(sizeof(stackNode));

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
Status pop(linkStack &stack, SElemType &e)
{
    if(stack.base == stack.top)
    {
        puts("stack is empty");
        return ERROR;
    }

    stackNode* ptr = stack.top;
    e = ptr->data;
    free(ptr);

    stack.top = stack.top->next;
    stack.stackSize--;

    ptr = NULL;

    return OK;
}

//打印栈内的元素
inline void printStackElem(const linkStack &stack)   
{
    int len = stackLength(stack);
    char buf[64] = {0};

    if(stackEmpty(stack))
    {
        puts("stack is empty");
    }

    stackNode* ptr = stack.top;


    while(NULL != ptr && len > 0)
    {
        buf[len - 1] = ptr->data;
        ptr = ptr->next;
        len--;
    }

    //len = strlen(buf); 

    //for(unsigned int i = 0; i < len; ++i)
    //{
    //    printf("%c", buf[i]);
    //}

    printf("%s\n", buf);
    puts("\nprint over\n");
}

//从栈底到栈顶依次对栈中的每个元素调用函数visit(),一旦visit()失败,
//则操作失败
Status stackTraverse(linkStack stack, Status(*visit)());

#endif
