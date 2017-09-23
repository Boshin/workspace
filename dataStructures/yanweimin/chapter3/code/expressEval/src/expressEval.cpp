#include "linkStack.h"
#include "expressEval.h"

//OP为运算符集合，len为集合的大小
Status isOptr(char ch, const char *OP, int len)
{

    for(int i = 0; i < len; ++i)
    {
        if(ch == OP[i])
        {
            return TRUE;
        }
    }

    return FALSE;
}

Status precede(linkStack const &optrStack, int ch)
{
    return TRUE;
}

Status operate(int value1, char optr, int value2)
{
    return TRUE;
}

OperandType evaluateExpression()
{
    char ch = 0;
    char getElem = 0;
    char OP[] = {'+', '-', '*', '/', '(', ')'};

    linkStack optrStack;
    linkStack opndStack;

    initStack(optrStack);
    initStack(opndStack);

    push(optrStack, '#');

    ch = getchar();
    getTop(optrStack, getElem);

    while('#' != ch || '#' != getElem)
    {
        
        if(!isOptr(ch, OP, sizeof(OP)))
        {
            push(optrStack, ch);
            ch = getchar();
        }
        else
        {
            switch(precede(optrStack, ch))
            {
                case '<':
                    {
                        push(optrStack, ch);
                        ch = getchar();
                        break;
                    }
                case '=':
                    {
                        int tmpElem = 0;
                        pop(optrStack, tmpElem);
                        ch = getchar();
                        break;
                    }
                case '>':
                    {
                        int optrElem = 0;
                        int opndElem1 = 0;
                        int opndElem2 = 0;

                        pop(optrStack, optrElem);
                        pop(opndStack, opndElem1);
                        pop(opndStack, opndElem2);

                        push(opndStack, operate(opndElem1, optrElem, opndElem2));
                        break;
                    }
            } //switch
        }
    } //while

    getTop(opndStack, getElem);
    return getElem;
}
