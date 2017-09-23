#include <string.h>
#include "digitConv.h"

void dec2oct()
{
    linkStack octStack;
    int num = -1;
    int popElem = -1;

    initStack(octStack);

    puts("please input dec num: ");
    scanf("%d", &num);

    while(num)
    {
        push(octStack, num % 8);
        num = num / 8;
    }

    printf("octNum: o");
    while(!stackEmpty(octStack))
    {
        pop(octStack, popElem);
        printf("%d", popElem);
    }

    puts("\n");
    destoryStack(octStack);
}

void dec2hex()
{
    linkStack hexStack;
    int num = -1;
    int popElem = -1;

    initStack(hexStack);

    puts("please input dec num: ");
    scanf("%d", &num);

    while(num)
    {
        push(hexStack, num % 16);
        num = num / 16;
    }

    printf("hexNum: 0x");
    while(!stackEmpty(hexStack))
    {
        pop(hexStack, popElem);
        printf("%d", popElem);
    }

    puts("\n");
    destoryStack(hexStack);
}

void dec2binary()
{
    linkStack binaryStack;
    int num = -1;
    int popElem = -1;

    initStack(binaryStack);

    puts("please input dec num: ");
    scanf("%d", &num);

    while(num)
    {
        push(binaryStack, num % 2);
        num = num / 2;
    }

    printf("binaryNum: ");
    while(!stackEmpty(binaryStack))
    {
        pop(binaryStack, popElem);
        printf("%d", popElem);
    }

    puts("\n");
    destoryStack(binaryStack);
}

void oct2dec()
{
    char str[20] = {0};
    int n = 0;
    int strLen = 0;

    puts("please input oct num: ");
    scanf("%s", str);
    printf("strlen(str): %ld, str: %s\n", strlen(str), str);

    strLen = strlen(str);
    n = str[0] - '0';

    for(int i = 1; i < strLen; ++i)
    {
        printf("str: %c, --n: %d\n", str[i], n);
        n = n * 8 + str[i] - '0';
    }

    /*
    char *s = "2504";
    int n = 0;
    n = *s - '0';
    s++;

    while(*s != '\0')
    {
        printf("*s: %c, --n: %d\n", *s, n);
        n = n * 8 + *s - '0';
        s++;
    }
    */
   
    printf("n = %d\n", n);
}

void hex2dec()
{
    //方法同oct2dec()
}

void binary2dec()
{
    //方法同oct2dec();
}
