#include <stdio.h>

struct bat
{
    unsigned char hight;
    unsigned char weight;
    unsigned char eye;
};

#define container_of(ptr, type, member) \
    (type *)((char *)(ptr) - (char *) &((type *)0)->member)
// #endif

int main(int argc, char *argv[])
{
   struct bat *ptr = 0; 
   struct bat batOne = { 1, 2, 3 };

   ptr = &batOne;

   unsigned char *pE = container_of(ptr, struct bat, eye);


   printf(" ptr: %p\n,  h: %p\n,  w: %p\n,  e: %p\n, pE: %p\n", ptr, &batOne.hight, &batOne.weight, &batOne.eye, pE);

   return 0;
}
