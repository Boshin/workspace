#include <stdio.h>

#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

int main(int argc, char *argv[])
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    vector<int*> intVec;

    intVec.push_back(&a);
    intVec.push_back(&b);
    intVec.push_back(&c);
    intVec.push_back(&d);

    //vector<int*>::iterator it = std::find(intVec.begin(), intVec.end(), &b);
    vector<int*>::iterator it = std::find(begin(intVec), end(intVec), &b);

    if(it != intVec.end())
    {
       printf("find it before: %d\n", intVec.size()); 
       intVec.erase(it);
       printf("find it after: %d\n", intVec.size()); 
    }
    
}
