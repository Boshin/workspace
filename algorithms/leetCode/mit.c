#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;
	int n = 20;

	//for(i = 0; i + n; i--)
	//for(i = 0; i < n; n--)
	for(i = 0; -i < n; i--)
	{
		printf("- %d\n", i);
	}

	return 0;
}
