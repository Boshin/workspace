#include <stdio.h>

typedef unsigned char   u1Byte;
typedef unsigned short  u2Byte; 
typedef unsigned int    u4Byte; 

#pragma pack(push, 1)
typedef struct 
{
    u1Byte val1;
    u4Byte val2;
    u1Byte val3;
    u2Byte val4;

}BC;
#pragma pack(pop)

int main(int argc, char* argv[])
{
    printf("sizeof(short): %u\n", sizeof(short));
    printf("sizeof(int): %u\n", sizeof(int));
    printf("sizeof(long): %u\n", sizeof(long));
    BC ob;
    BC arr[10];

    printf("sizeof(BC): %u\n", sizeof(BC));
    printf("sizeof(ob): %u\n", sizeof(ob));
    printf("sizeof(arr): %u\n", sizeof(arr));
    //printf("sizeof(long): %d", sizeof(long));
}

