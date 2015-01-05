#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

class MutexLock
{
    public:
        MutexLock():m_bLocked(0) {};

        void lock()
        {
            while(m_bLocked)
            {
                sleep(0);
            }
            m_bLocked = 1;
        }

        void unlock()
        {
            m_bLocked = 0;
        }

    private:
        volatile int m_bLocked;   
};

static MutexLock g_lock;
static int aaa = 0;
static pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;

#if 1
#define LOCK g_lock.lock()
#define UNLOCK g_lock.unlock()
#else
#define LOCK pthread_mutex_lock(&mymutex);
#define UNLOCK  pthread_mutex_unlock(&mymutex);
#endif 


void* test_func_a(void*)
{
    for(int i=0; i<1000; ++i)
    {
        LOCK;
        printf("a--->after lock---\n");
        aaa = i;
        if(aaa > 1000)
        {
            printf("Lock Error!!!!");
        }
        printf("a--->before unlock---\n");
        UNLOCK;
    }
    return 0;
}

void* test_func_b(void*)
{
    for(int i=1000; i<2000; ++i)
    {
        LOCK;
        printf("b--->after lock---\n");
        aaa = i;
        if(aaa < 1000)
        {
            printf("Lock Error!!!!");
        }
        printf("b--->before unlock---\n");
        UNLOCK;
    }
    return 0;
}


int main()
{
    int err;
    pthread_t ntid;
    err = pthread_create(&ntid, NULL, test_func_a, NULL);
    if (err != 0)
        printf("can't create thread: %s\n", strerror(err));

    err = pthread_create(&ntid, NULL, test_func_b, NULL);
    if (err != 0)
        printf("can't create thread: %s\n", strerror(err));

    while(1)
        sleep(0);

    return 0;
}
