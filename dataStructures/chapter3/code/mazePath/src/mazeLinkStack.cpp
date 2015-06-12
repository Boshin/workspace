#include "mazeLinkStack.h"

Status initStack(mazeLinkStack &stack)
{
    stack.top = stack.base = NULL;
    stack.stackSize = 0;

    return OK;
}

Status destoryStack(mazeLinkStack &stack)
{
    if(stack.base == stack.top)
    {
        puts("stack is empty");
        return ERROR;
    }

    mazeStackNode* ptr = NULL;

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

Status clearStack(mazeLinkStack &stack)
{
    return destoryStack(stack);
}

int stackLength(const mazeLinkStack &stack)
{
    return stack.stackSize;
}

Status stackEmpty(const mazeLinkStack &stack)
{
    if(stack.base == stack.top)
    {
        return TRUE;
    }

    return FALSE;
}
