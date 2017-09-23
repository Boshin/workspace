#include <stdio.h>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
   vector<int> vec;
   printf("vec size1: %d\n", vec.size()); 
   vec.reserve(6);
   printf("vec size2: %d\n", vec.size()); 
   vec.push_back(100);
   printf("vec size3: %d\n", vec.size()); 
   printf("vec capacity: %d\n", vec.capacity()); 
   
    
   
}
