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
    int row;        //���Թ���ͼ�е��к�
    int col;        //���Թ���ͼ�е��к�

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
    int ord;        //ͨ������·���ϵġ���š�
    posType seat;   //ͨ�������Թ��еġ�����λ�á�
    int di;         //�Ӵ�ͨ����������һͨ����ġ�����
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
    mazeStackNode *base;        //��ջ����֮ǰ������֮��base��ֵΪNULL
    mazeStackNode *top;         //ջ��ָ��
    int stackSize;          //��ǰ�ѷ���Ĵ洢�ռ䣬��Ԫ��Ϊ��λ
};

//����һ����ջ
Status initStack(mazeLinkStack &stack);

//����ջstack��stack���ٴ���
Status destoryStack(mazeLinkStack &stack);

//��stack��Ϊ��ջ
Status clearStack(mazeLinkStack &stack);

//��ջstackΪ��ջ���򷵻�TRUE�����򷵻�FALSE
Status stackEmpty(const mazeLinkStack &stack);

//����stack��Ԫ�ظ�������ջ�ĳ���
int stackLength(const mazeLinkStack &stack);

//��ջ��Ϊ�գ�����e����stack��ջ��Ԫ�أ�������OK�����򷵻�ERROR
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

//����Ԫ��eΪ�µ�ջ��Ԫ��
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

//��ջ���գ���ɾ��stack��ջ��Ԫ�أ���e������ֵ��
//������OK�����򷵻�ERROR
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
