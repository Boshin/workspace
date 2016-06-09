#include <map>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
    typedef map<int, int> mapType;
    mapType tmp;

    if (tmp.empty())
    {
        printf("map is empty");
    }

    mapType::iterator itBegin = tmp.begin();
    mapType::iterator itEnd = tmp.end();

    if (itBegin == itEnd)
    {
        printf("\n begin == end\n");
    }
    else 
    {
        printf("begin != end\n");
    }
    
}
