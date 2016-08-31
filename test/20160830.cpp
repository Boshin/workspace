#include <stdio.h>

int main()
{
    long long i = 100;
    long long j = (long long)&i;

    printf("&i: %p, j: %llX\n", &i, j);

    return 0;
}





