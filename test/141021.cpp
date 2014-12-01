#include <stdio.h>
#include <stddef.h>
#include <typeinfo>
typedef bool (stt)(float id, char **ptr);

/*
typedef struct STUDENT
{
	typedef bool (type)(int id, char **ptr);
	int id;
	type *ptr2;
}stu;

*/
int main()
{
	stt x;
	printf(":%d\n", sizeof(main));
	//printf("size: %d\n", sizeof(x));
	//printf("size: %u\n", sizeof(x));

//	stu mystu;
}
