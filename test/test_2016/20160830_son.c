#include <stdio.h>

int main()
{
    // long long i = 100;
    // long long j = (long long)&i;

    // printf("&i: %p, j: %llX\n", &i, j);

    const int a = 5;
    int *ptr = &a;
    *ptr = 8;

    printf("a: %d\n", a);

    return 0;
}





