#include <string.h>

#include "verifyBrackets.h"

void verifyBrackets()
{
    char brackets[10] = {0};
    linkStack stack;

   initStack(stack); 

    puts("please input some brackets: ");
    scanf("%s", brackets);
    printf("strlen(brackets): %ld, brackets: %s\n", strlen(brackets), brackets);

    for(unsigned int i = 0; i < strlen(brackets); ++i)
    {

        int getElem = -1;

        switch(brackets[i])
        {
            case '[':
                printf("it's [\n");
                push(stack, brackets[i]);
                break;
            case ']':
                printf("it's ]\n");
                getTop(stack, getElem);

                if(stackEmpty(stack))
                {
                    puts("stakc is NULL, inputs elem err");
                    exit(-1);
                }

                if(2 == brackets[i] - getElem)
                {
                    pop(stack, getElem);
                    printf("input elem: %c, pop elem: %c\n", brackets[i], getElem);
                }
                
                break;
            case '(':
                printf("it's (\n");
                push(stack, brackets[i]);
                break;
            case ')':
                printf("it's )\n");
                getTop(stack, getElem);

                if(stackEmpty(stack))
                {
                    puts("stakc is NULL, inputs elem err");
                    exit(-1);
                }

                if(1 == brackets[i] - getElem)
                {
                    pop(stack, getElem);
                    printf("input elem: %c, pop elem: %c\n", brackets[i], getElem);
                }

                break;

            default:
                printf("err elem: %c, we will exit\n", brackets[i]);
                destoryStack(stack);
                exit(-1);
        }

    }

    if(!stackEmpty(stack))
    {
        puts("inputs error");

        destoryStack(stack);
    }
    else
        puts("process bracktes success!!!");

}
