#include <stdio.h>

class base
{
    public:
        virtual ~base(); // = 0;
        virtual void interface() const = 0;
        virtual const char* meld() const { return _meld; }

    protected:
        char* _meld;
};

// inline base::~base() { }

inline void base::interface() const
{
    printf("base::interface\n");
}

class derived : public base
{
    public:
        derived() { }

    public:
        void interface() const; 
};

inline void derived::interface() const
{
    printf("derived::interface\n");
    base::interface();
}

void test()
{
    derived test;
    test.interface();
}

int main(int argc, char* argv[])
{
    test();

    return 0;
}
