#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

static int x = 0;
static volatile int f = 0;

void* test_func_a(void*)
{
    while(f == 0);
    int b = x;
    printf("x=%d\n",b);
    return 0;
}

void* test_func_b(void*)
{
    //sleep(1);
    volatile int xxx = 0;
    while(xxx++ < 999999999);
    x = 42;
    f = 1;
    return 0;
}


int main()
{
    int err;
    pthread_t ntid1, ntid2;
    err = pthread_create(&ntid1, NULL, test_func_a, NULL);
    if (err != 0)
        printf("can't create thread: %s\n", strerror(err));

    err = pthread_create(&ntid2, NULL, test_func_b, NULL);
    if (err != 0)
        printf("can't create thread: %s\n", strerror(err));

    pthread_join(ntid1, 0);
    pthread_join(ntid2, 0);    
    return 0;
}
