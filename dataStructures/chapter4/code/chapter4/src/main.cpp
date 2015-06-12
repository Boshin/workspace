#include <stdio.h>
#include "XString.h"

int main(int argc, char *[])
{
	char srcStr[] = {"aaabaaaab"};
	XString mainStr;
	mainStr.ch = NULL;
	mainStr.length = 0;
	
	char desStr[] = {"aaaab"};
	XString subStr;
	subStr.ch = NULL;
	subStr.length = 0;

	char repStr[] = {"cd"};
	XString rep;
	rep.ch = NULL;
	rep.length = 0;

	strAssign(mainStr, srcStr);
	strAssign(subStr, desStr);
	strAssign(rep, repStr);
	printf("mainStr len: %d, mainStr: %s\n", mainStr.length, mainStr.ch);
	printf("subStr len: %d, subStr: %s\n", subStr.length, subStr.ch);
	printf("rep len: %d, rep: %s\n", rep.length, rep.ch);

	int index = indexStr(mainStr, subStr, 0);
	printf("index: %d\n", index);

	strReplace(mainStr, subStr, rep);

	printf("new len: %d, new mainStr: %s\n", mainStr.length, mainStr.ch);

	return 0;
}
