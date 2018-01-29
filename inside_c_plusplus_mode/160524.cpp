#include <iostream>
#include <stdio.h>

using namespace std;

enum Date
{
    Date_one,
    Date_two
};

class base
{
};

class derived1 : virtual public base
{
};

class derived2 : virtual public base
{
};

//class A : public derived1, public derived2
//class A : public base
class A : public derived1
{
    public:
        static int i;
};

int A::i = 0;

class Piont 
{
    public:
        Piont()
            : j(10)
        {
            i = j;
        }
    
        void print()
        //virtual void print()
        {
            cout << "i: " << i << "\nj: " << j << "\n";
        }

    // private:
    public:
        static Piont origin;
        int i;
        int j;
        int z;
        void* ptr;
};

class base1
{
    public:
        int val1;
};

class base2
{
    public:
        int val2;
};

class der : public base1, public base2 
{
    public:
        int val3;
};

class shape
{
    public:
        virtual ~shape(); // { }
        int x(); // { }
        int y(); // { }
        virtual int z(); // { }
};

Piont Piont::origin;

void testPiont()
{
    Piont tP;
    tP.print();

    printf("&Piont::i: %p\n", &Piont::i);
    printf("&Piont::j: %p\n", &Piont::j);
    printf("&Piont::z: %p\n", &Piont::z);
    printf("&Piont::ptr: %p\n", &Piont::ptr);

    int Piont::*ptr1 = 0;
    int Piont::*ptr2 = &Piont::i;

    if (ptr1 == ptr2)
    {
        printf("ptr1 == ptr2");
    }

    printf("&tP: %p\n", &tP);
    printf("&tP.i: %p\n", &tP.j);


    printf("&tP.origin: %p\n", &tP.origin);
    printf("&Piont::origin: %p\n", &Piont::origin);
}

void testder()
{
    printf("&base1::val1: %p\n", &base1::val1);
    printf("&base2::val2: %p\n", &base2::val2);
    printf("&der::val1: %p\n", &der::val1);
    printf("&der:val2: %p\n", &der::val2);
    // printf("&der:val3: %p\n", &der::val3);
}

void testShape()
{
    printf("&shape::z %d\n", &shape::z());
}

int main(int argc, char *argv[])
{
    testShape();
    testder();

    cout << endl;

    testPiont();

    cout << "sizeof(derived1): " << sizeof(derived1) << endl;
    cout << "sizeof(A): " << sizeof(A) << endl;
    cout << "sizeof(pointer): " << sizeof(char*) << endl;

    A a;
    printf("A i: %d\n", a.i);
    a.i = 10;
    printf("A i: %d\n", a.i);
    A::i = 20;
    printf("A i: %d\n", A::i);

    printf("Date_one: %d\n", Date_one);

    /*
    for (unsigned int i = 0; i < sizeof(A); ++i)
    {
        //printf("A[%d]: %d\n", i &A);
    }
    */

    return 0;
}
