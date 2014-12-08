#include <stdio.h>
#include <unordered_map>

class Solution {
	public:
		int singleNumber(int A[], int n) {
			int tarNum = A[0];

			for(int i = 1; i < n; ++i)
			{
				printf("before tarNum: %d, A[i]: %d\n", tarNum, A[i]);
				tarNum ^= A[i];
				printf("after tarNum: %d\n", tarNum);
			}

			return tarNum;
		}
};

int main(int argc, char *argv[])
{
	int arr[] = {1, 2, 3, 4, 5, 4, 3, 2, 1, 6, 8, 7, 8, 7, 5};

	Solution sou;
	sou.singleNumber(arr, sizeof(arr) / sizeof(arr[0]));
	
	return 0;
}
