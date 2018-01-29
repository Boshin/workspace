#include <iostream>
#include <stdio.h>
#include <string.h>

class fun
{
	public:
		fun();
		virtual ~fun();

		void print();

		void set(int a);

		virtual void fuck() {};

	private:
		int m_val;
};

fun::fun()
	: m_val(0)
{
	std::cout << "CC" << std::endl;
}

fun::~fun()
{
	std::cout << "dd" << std::endl;
}

void fun::print()
{
	std::cout << "print " << m_val << std::endl;
}

void fun::set(int a)
{
	m_val = a;
}

int main(int argc, char *argv[])
{
	printf("fun size: %d\n", sizeof(fun));
	fun *thisFun = new fun();
	thisFun->print();
	fun &ref = *thisFun;
	ref.set(10);
	thisFun->print();
	ref.print();
	delete thisFun;
	memset(thisFun, 0, sizeof(fun));
	//thisFun = NULL;
    fun* otherFn = new fun();	

	printf("ref: %08x, thisFun: %08x, otherFun: %08x\n", &ref, thisFun, otherFn);
	
	//thisFun->fuck();
	thisFun->print();
	ref.print();
	ref.set(20);
	ref.print();
	otherFn->print();

	return 0;
}
