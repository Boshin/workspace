#include "unp.h"

int main (int argc, char *argv[])
{
    union {
        short val;
        char  c[sizeof(short)];
    }un;

    un.val = 0x102;

    //printf("%s\n", CPU_VENDOR_OS);
    // printf("un.c[0]: %d, un.c[1]: %d\n", un.c[0], un.c[1]);

    if (2 == sizeof(short)) {
        if ((1 == un.c[0]) && (2 == un.c[1])) {
            printf("big endign\n");
        }
        else if ((2 == un.c[0]) && 1 == un.c[1]) {
            printf ("little endign\n");
        }
        else {
            printf("unkown\n");
        }
    }
    else {
        printf("sizeof(short): %lu", sizeof(short));
    }

    return 0;
}

