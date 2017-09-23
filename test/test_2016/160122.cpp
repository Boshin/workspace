#include <iostream>

class A {};

class B : public virtual A {};

class C : public virtual A {};

class D : public B, public C {};

int main(int argc, char *argv[])
{
    std::cout << "sizeof A: " << sizeof(A) << "\n";
    std::cout << "sizeof B: " << sizeof(B) << "\n";
    std::cout << "sizeof C: " << sizeof(C) << "\n";
    std::cout << "sizeof D: " << sizeof(D) << "\n";
}
