#include <stdio.h>
#include <unistd.h>

int a[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 , 4, 5, 11 };
// int a[] = { 6, 3, 4, 2, 1, 5, 8, 7, 9 };
int arrSize = sizeof(a) / sizeof(a[0]);

void printArr(int sortCnt)
{
	for (int i = 0; i < arrSize; ++i) {
		printf("%d ", a[i]);
	}

	printf("\n sortCnt :%d\n", sortCnt);
}

//冒泡排序
void bubbleSort()
{
	int cnt = 0;

	for (int i = 0; i < arrSize - 2; ++i) {
		for (int j = 0; j < arrSize - 1 - i; ++j)
		{
			if (a[j] > a[j + 1]) {
				int tmp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = tmp;
				++cnt;
			}
		}
	}

	printArr(cnt);
}

//快速排序
void quickSortImp1(int li, int ri)
{
	if (li > ri)
		return;
	
	int basicLi = li;
	int basicRi = ri;
	int basicValue = a[li];

	printArr(0);
 	while (1) {
		if ((a[ri] >= basicValue) && (li < ri)) {
			ri--;
			continue;
		}

		if(li == ri) {
			a[basicLi] = a[li];
			a[li] = basicValue;
			if (basicLi < (li - 1))
				quickSortImp1(basicLi, li - 1);
			if ((ri + 1) < basicRi )
				quickSortImp1(ri + 1, basicRi); 
			return;
		}
		else {
			if ((a[li] > basicValue) && (li < ri)) {
				int tmp = a[li];
				a[li] = a[ri];
				a[ri] = tmp;
			}
			else {
				++li;
			}
		}
	}
}

void quickSortImp2(int li, int ri)
{
	if (li > ri)
		return;
	
	int basicLi = li;
	int basicRi = ri;
	int basicValue = a[li];

	while (li != ri) {
		while ((a[ri] >= basicValue) && (li < ri))
			--ri;

		while ((a[li] <= basicValue && (li < ri)))
			++li;

		if (li < ri) {
			int tmp = a[li];
			a[li] = a[ri];
			a[ri] = tmp;
		}
	} 
	
	a[basicLi] = a[li];
	a[li] = basicValue;

	quickSortImp2(basicLi, li - 1);
	quickSortImp2(ri + 1, basicRi);
	return;
}

int main (int argc, char *argv[])
{
	printArr(0);
	//bubbleSort();
	quickSortImp1(0, arrSize - 1);
	//quickSortImp2(0, arrSize - 1);
	printArr(0);
}

