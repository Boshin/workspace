#include "linearStack.h"

Status destoryStack(SqStack &stack)
{
    if(NULL == stack.base)
    {
        puts("stack is NULL");
        return ERROR;
    }

    free(stack.base);
    stack.base = NULL;
    stack.top = NULL;
    stack.stackSize = 0;

    return OK;
}

Status clearStack(SqStack &stack)
{
    if(NULL == stack.base)
    {
        puts("stack is NULL");
        return ERROR;
    }

    stack.top = stack.base;

    return OK;
}

Status stackEmpty(const SqStack &stack)
{
    if(NULL == stack.base)
    {
        puts("stack is NULL");
        return ERROR;
    }

    if(stack.base == stack.top)
    {
        return TRUE;
    }

    return FALSE;
}

int stackLength(const SqStack &stack)
{
    if(NULL == stack.base)
    {
        puts("stack is NULL");
        return ERROR;
    }

    return stack.stackSize;
}

