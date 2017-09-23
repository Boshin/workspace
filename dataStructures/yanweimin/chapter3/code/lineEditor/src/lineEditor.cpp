#include "lineEditor.h"

#define ED 27

void lineEditor()
{
    linkStack bufStack;
    initStack(bufStack);

    char ch = getchar();
    int getElem = -1;

    while(ED != ch)
    {
        while((ED != ch) && ('\n' != ch))
        {
            switch(ch)
            {
                case '#':       //仅当栈非空时退栈
                    pop(bufStack, getElem);
                    break;
                case '@':
                    clearStack(bufStack);
                    break;
                default:
                    push(bufStack, ch);
                    break;
            }

            ch = getchar();
        }

        printStackElem(bufStack);   //打印栈内的元素
        clearStack(bufStack);       //重置bufStack为空栈

        if(ED != ch)
        {
            ch = getchar();
        }
    }

    destoryStack(bufStack);
}
