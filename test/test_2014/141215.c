#include <stdio.h>

typedef unsigned long long u64_t;
typedef unsigned long long uint64_t;
#define _BIT(m, n) (((uint64_t)(m) >> (uint64_t)(n)) & (unsigned char)(1))
//#define GETBIT(m, n) (((u64_t)(m) >> (u64_t)(n)) & (1))


unsigned char getBit(int m, int n)
{
    return ((m >> n) & 1);
}

int main()
{
    //printf("%d\n", _BIT(2));

    //printf("%d\n", getBit(15, 2));
    //printf("%d\n", GETBIT(15, 2));
    printf("%d\n", _BIT(15, 2));
}
