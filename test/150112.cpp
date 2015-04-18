#include <iostream>
#include <memory>
#include <string>

struct st2
{
    char aa[9];
};

struct st
{
    char c1;
    char c2;
    char c3;
    double d1;
    st2   s2;
    //char a[128];
    //int i1;
};

int main () {
    //std::shared_ptr<int> foo (new int(10));
    //std::shared_ptr<int> bar (new int(20));

    //foo.swap(bar);

    //std::cout << "*foo: " << *foo << '\n';
    //std::cout << "*bar: " << *bar << '\n';

    std::string a = "I am a geninus";
    std::cout << "sizeof(a) " << sizeof(a) << '\n';
    std::cout << "st: " << sizeof(st) << "\n";

    return 0;
}
