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
                case '#':       //����ջ�ǿ�ʱ��ջ
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

        printStackElem(bufStack);   //��ӡջ�ڵ�Ԫ��
        clearStack(bufStack);       //����bufStackΪ��ջ

        if(ED != ch)
        {
            ch = getchar();
        }
    }

    destoryStack(bufStack);
}
