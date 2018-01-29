// #include <iostream>
//#include <stdio.h>

// using namespace std;

/*
class A
{
	virtual void a()
	{
	}
};
class A1
{
	virtual void a()
	{
	}
};

class B : public A , virtual public A1
{
};

int main()
{
	cout<<sizeof(B)<<endl;     // 
    
    return 0;
}
*/

/*
class A
{
    int a;
};

class B
{
    int b;
};

class C
{
};

class D
{
};

class E: public virtual A , public virtual B , public virtual C , public virtual D
{
};

int main()
{
    cout << "sizeof(E): " <<sizeof(E)<<endl;      // 

    return 0;
}
*/

/*
class A
{
};

class B
{
    int b;
};

class C
{
};

class D
{
};

class E: public virtual A , public virtual B , public virtual C , public virtual D
{
};

int main()
{
    cout<< "sizeof(E1): "<<sizeof(E)<<endl;      // 

    return 0;
}
*/


class A
{
    // virtual void func();
    // int a;
};

class B
{
    // int b;
};

class C
{
};

class E: public virtual A , public virtual B , public virtual C
{
};

class F : public virtual B { };

int main()
{
    //cout << "sizeof(E3): "<<sizeof(E)<<endl;      // 
   
    return 0;
}
