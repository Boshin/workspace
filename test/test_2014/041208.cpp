#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	//unsigned long long maxsize = atoi(argv[1]) * 10000 * 10000;

	//printf("start maxsize: %llu\n", maxsize);

//	for(unsigned long long i = 0; i < maxsize; ++i);

	//printf("end\n");

	int j = 1;
	printf("j :%d\n", j);

	j ^= 1;
	printf("j^=1 :%d\n", j);

	j ^= 2;
	printf("j^=2 :%d\n", j);

	j ^= 2;
	printf("j^=2 :%d\n", j);

}
