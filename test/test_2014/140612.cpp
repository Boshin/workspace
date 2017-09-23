#include <iostream>

using namespace std;

class base
{
	public:
	base()
	{
		cout << "call base constructor" << endl;
	}

	virtual ~base()
	{
		cout << "call base destory" << endl;
	}

    void print()
	{
		cout << "hello world" << endl;
	}
};

class ch : public base
{
	public:
	ch()
	{
		cout << "call ch constructor" << endl;
	}

	virtual ~ch()
	{
		cout << "call ch dectory" << endl;
	}

    void print()
	{
        base::print();
	}
};

int main(int argc, char *argv[])
{
	ch myCH;

	myCH.print();
}
