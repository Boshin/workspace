#include <vector>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
    int arr[10] = { 0 };

    for (int i = 0; i < 10; ++i)
    {
        arr[i] = i;
    }

    vector<int> vec;
    printf("111 vec size: %d\n", vec.size());
    printf("111 vec capacity: %d\n", vec.capacity());

    vec.insert(vec.begin(), arr, arr + 10);

    printf("222 vec size: %d\n", vec.size());
    printf("222 vec capacity: %d\n", vec.capacity());

    vec.resize(12);

    printf("333 vec size: %d\n", vec.size());
    printf("333 vec capacity: %d\n", vec.capacity());
}

