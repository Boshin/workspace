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
    stackNode *base;        //��ջ����֮ǰ������֮��base��ֵΪNULL
    stackNode *top;         //ջ��ָ��
    int stackSize;          //��ǰ�ѷ���Ĵ洢�ռ䣬��Ԫ��Ϊ��λ
};


//����һ����ջ
Status initStack(linkStack &stack);

//����ջstack��stack���ٴ���
Status destoryStack(linkStack &stack);

//��stack��Ϊ��ջ
Status clearStack(linkStack &stack);

//��ջstackΪ��ջ���򷵻�TRUE�����򷵻�FALSE
Status stackEmpty(const linkStack &stack);

//����stack��Ԫ�ظ�������ջ�ĳ���
int stackLength(const linkStack &stack);

//��ջ��Ϊ�գ�����e����stack��ջ��Ԫ�أ�������OK�����򷵻�ERROR
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

//����Ԫ��eΪ�µ�ջ��Ԫ��
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

//��ջ���գ���ɾ��stack��ջ��Ԫ�أ���e������ֵ��
//������OK�����򷵻�ERROR
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

//��ӡջ�ڵ�Ԫ��
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

//��ջ�׵�ջ�����ζ�ջ�е�ÿ��Ԫ�ص��ú���visit(),һ��visit()ʧ��,
//�����ʧ��
Status stackTraverse(linkStack stack, Status(*visit)());

#endif
