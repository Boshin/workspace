#ifndef LINEAR__H_
#define LINEAR__H_

#include <stdio.h>
#include <stdlib.h>

#include "MicroDefine.h"

#define STACK_INIT_SIZE 100     //�洢�ռ��ʼ������
#define STACKINCREMENT 10       //�洢�ռ��������

template<typename SElemType>
struct linearStack
{
    SElemType *base;        //��ջ����֮ǰ������֮��base��ֵΪNULL
    SElemType *top;         //ջ��ָ��
    int stackSize;          //  ��ǰ�ѷ���Ĵ洢�ռ䣬��Ԫ��Ϊ��λ
};

typedef struct linearStack<int> SqStack;

//����һ����ջ
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

//����ջstack��stack���ٴ���
Status destoryStack(SqStack &stack);

//��stack��Ϊ��ջ
Status clearStack(SqStack &stack);

//��ջstackΪ��ջ���򷵻�TRUE�����򷵻�FALSE
Status stackEmpty(const SqStack &stack);

//����stack��Ԫ�ظ�������ջ�ĳ���
int stackLength(const SqStack &stack);

//��ջ��Ϊ�գ�����e����stack��ջ��Ԫ�أ�������OK�����򷵻�ERROR
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

//����Ԫ��eΪ�µ�ջ��Ԫ��
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

//��ջ���գ���ɾ��stack��ջ��Ԫ�أ���e������ֵ��
//������OK�����򷵻�ERROR
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

//��ջ�׵�ջ�����ζ�ջ�е�ÿ��Ԫ�ص��ú���visit(),һ��visit()ʧ��,
//�����ʧ��
Status stackTraverse(SqStack stack, Status(*visit)());

#endif
