#include <string>
#include <iostream>
#include <typeinfo>

class base
{
	public:
		base()
		{
			std::cout << "base constructor" << std::endl;
		}

		virtual ~base()
		{
			std::cout << "base destructor" << std::endl;
		}
};

class derived : base
{
	public:
		explicit derived()
			: base()
		{
			std::cout << "derived constructor" << std::endl;
		}

		virtual ~derived()
		{
			std::cout << "derived destructor" << std::endl;
		}
};

class newC
{
	public:
		newC(derived* subPtr){};
		~newC(){};
};

int main(int argc, char *argv[])
{
	std::string str1 = "hello world";

	base baseObject;

	newC tmm(&baseObject);
	//derived derivedObject;

	//std::cout << typeid(str1).name() << std::endl;
	//std::cout << typeid(baseObject).name() << std::endl;
	//std::cout << typeid(derivedObject).name() << std::endl;

	{
		//std::string &str = NULL;
	}

	return 0;
}
