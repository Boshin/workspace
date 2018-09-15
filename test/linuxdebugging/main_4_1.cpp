#include <stdio.h>

class Parent
{
public:
    Parent()
        :numInParent(1111)
    {

    }
    virtual void Foo() { }
    virtual void Boo() { }

    void Aoo() { printf("Parent::Aoo\n"); }

    virtual void Coo() { printf("Parent::Coo\n"); }

private:
    int numInParent;
};

class Child: public Parent
{
public:
    Child():numInChild(2222) { }

    virtual void Foo() { }

    void Aoo() { Parent::Aoo(); printf("Child::Aoo\n"); }

    virtual void Coo() { Parent::Coo(); printf("Child::Coo\n"); }

    int numInChild;
};

int main(int argc, char* argv[])
{
  Parent *pObj = new Parent();
  pObj->Aoo();
  pObj->Coo();
  printf("\n");

  Child cObj;
  cObj.Foo();
  cObj.Aoo();
  cObj.Coo();
  printf("\n");

  pObj = &cObj;
  pObj->Aoo();
  pObj->Coo();
  printf("\n");

  return 0;
}
