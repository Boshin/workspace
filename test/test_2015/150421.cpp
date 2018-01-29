#include <stdio.h>

class base
{
    public:
    base() {}
    virtual void vfunc1()
    {
    }

    virtual void vfunc2(){}

    void func1()
    {
    }

    void func2(){}
};

int main(int argc, char *argv[])
{
    printf("vfunc1: %p\n", &base::vfunc1);
    printf("vfunc2: %p\n", &base::vfunc2);
    printf("func1: %p\n", &base::func1);
    printf("func2: %p\n", &base::func2);
    printf("func1: %p\n", &base::base);
}
