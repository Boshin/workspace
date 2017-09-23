#include <stdio.h>

int main(int argc, char *argv[])
{
//	printf("%d\n", 0b11);
 printf("int: %d, long: %d\n", sizeof(int), sizeof(long));
 
 int a = 100;
 int &refA = a;
}
