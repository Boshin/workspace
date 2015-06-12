#include "linkStack.h"

Status initStack(linkStack &stack)
{
    stack.top = stack.base = NULL;
    stack.stackSize = 0;

    return OK;
}

Status destoryStack(linkStack &stack)
{
    if(stack.base == stack.top)
    {
        puts("stack is empty");
        return ERROR;
    }

    stackNode* ptr = NULL;

    while(stack.top)
    {
        ptr = stack.top->next;
        free(stack.top);
        stack.top = ptr;
    }

    stack.top = NULL;
    stack.stackSize = 0;

    return OK;
}

Status clearStack(linkStack &stack)
{
    return destoryStack(stack);
}

int stackLength(const linkStack &stack)
{
    return stack.stackSize;
}

Status stackEmpty(const linkStack &stack)
{
    if(stack.base == stack.top)
    {
        return TRUE;
    }

    return FALSE;
}
