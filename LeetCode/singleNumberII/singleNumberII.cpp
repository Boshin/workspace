#include <stdio.h>
#include <unordered_map>

typedef unsigned char u8_t;

union funNum
{
	typedef struct
	{
		u8_t bit0  : 1;
		u8_t bit1  : 1;
		u8_t bit2  : 1;
		u8_t bit3  : 1;
		u8_t bit4  : 1;
		u8_t bit5  : 1;
		u8_t bit6  : 1;
		u8_t bit7  : 1;
		u8_t bit8  : 1;
		u8_t bit9  : 1;
		u8_t bit10 : 1;
		u8_t bit11 : 1;
		u8_t bit12 : 1;
		u8_t bit13 : 1;
		u8_t bit14 : 1;
		u8_t bit15 : 1;
		u8_t bit16 : 1;
		u8_t bit17 : 1;
		u8_t bit18 : 1;
		u8_t bit19 : 1;
		u8_t bit20 : 1;
		u8_t bit21 : 1;
		u8_t bit22 : 1;
		u8_t bit23 : 1;
		u8_t bit24 : 1;
		u8_t bit25 : 1;
		u8_t bit26 : 1;
		u8_t bit27 : 1;
		u8_t bit28 : 1;
		u8_t bit29 : 1;
		u8_t bit30 : 1;
		u8_t bit31 : 1;
		u8_t bit32 : 1;
	}typeIntBit;

	typeIntBit 	bitData;
	int 		intNum;
};

class Solution {
	public:
		int singleNumberII(int A[], int n) {
			int bitArr[32] = {0};
			funNum tmp;
			funNum tar;

			tar.intNum = 0;
			tmp.intNum = 0;

			for(int i = 0; i < n; ++i)
			{
				tmp.intNum = A[i];
				printf("before bitArr[0]: %d, tmp%d.bitData.bit0: %d\n", bitArr[0], i, tmp.bitData.bit0);
				bitArr[0]  += tmp.bitData.bit0;
				bitArr[1]  += tmp.bitData.bit1;
				bitArr[2]  += tmp.bitData.bit2;
				bitArr[3]  += tmp.bitData.bit3;
				bitArr[4]  += tmp.bitData.bit4;
				bitArr[5]  += tmp.bitData.bit5;
				bitArr[6]  += tmp.bitData.bit6;
				bitArr[7]  += tmp.bitData.bit7;
				bitArr[8]  += tmp.bitData.bit8;
				bitArr[9]  += tmp.bitData.bit9;
				bitArr[10] += tmp.bitData.bit10;
				bitArr[11] += tmp.bitData.bit11;
				bitArr[12] += tmp.bitData.bit12;
				bitArr[13] += tmp.bitData.bit13;
				bitArr[14] += tmp.bitData.bit14;
				bitArr[15] += tmp.bitData.bit15;
				bitArr[16] += tmp.bitData.bit16;
				bitArr[17] += tmp.bitData.bit17;
				bitArr[18] += tmp.bitData.bit18;
				bitArr[19] += tmp.bitData.bit19;
				bitArr[20] += tmp.bitData.bit20;
				bitArr[21] += tmp.bitData.bit21;
				bitArr[22] += tmp.bitData.bit22;
				bitArr[23] += tmp.bitData.bit23;
				bitArr[24] += tmp.bitData.bit24;
				bitArr[25] += tmp.bitData.bit25;
				bitArr[26] += tmp.bitData.bit26;
				bitArr[27] += tmp.bitData.bit27;
				bitArr[28] += tmp.bitData.bit28;
				bitArr[29] += tmp.bitData.bit29;
				bitArr[30] += tmp.bitData.bit30;
				bitArr[31] += tmp.bitData.bit31;
				printf("before bitArr[0]: %d, tmp%d.bitData.bit0: %d\n\n", bitArr[0], i, tmp.bitData.bit0);
			}

			//for(int i = 0; i < sizeof(bitArr) / sizeof(bitArr[0]); ++i)
			{
				tar.bitData.bit0  = bitArr[0] % 3;
				tar.bitData.bit1  = bitArr[1] % 3;  
				tar.bitData.bit2  = bitArr[2] % 3;  
				tar.bitData.bit3  = bitArr[3] % 3;  
				tar.bitData.bit4  = bitArr[4] % 3;  
				tar.bitData.bit5  = bitArr[5] % 3;  
				tar.bitData.bit6  = bitArr[6] % 3;  
				tar.bitData.bit7  = bitArr[7] % 3;  
				tar.bitData.bit8  = bitArr[8] % 3;  
				tar.bitData.bit9  = bitArr[9] % 3;  
				tar.bitData.bit10 = bitArr[10] % 3;
				tar.bitData.bit11 = bitArr[11] % 3;
				tar.bitData.bit12 = bitArr[12] % 3;
				tar.bitData.bit13 = bitArr[13] % 3;
				tar.bitData.bit14 = bitArr[14] % 3;
				tar.bitData.bit15 = bitArr[15] % 3;
				tar.bitData.bit16 = bitArr[16] % 3;
				tar.bitData.bit17 = bitArr[17] % 3;
				tar.bitData.bit18 = bitArr[18] % 3;
				tar.bitData.bit19 = bitArr[19] % 3;
				tar.bitData.bit20 = bitArr[20] % 3;
				tar.bitData.bit21 = bitArr[21] % 3;
				tar.bitData.bit22 = bitArr[22] % 3;
				tar.bitData.bit23 = bitArr[23] % 3;
				tar.bitData.bit24 = bitArr[24] % 3;
				tar.bitData.bit25 = bitArr[25] % 3;
				tar.bitData.bit26 = bitArr[26] % 3;
				tar.bitData.bit27 = bitArr[27] % 3;
				tar.bitData.bit28 = bitArr[28] % 3;
				tar.bitData.bit29 = bitArr[29] % 3;
				tar.bitData.bit30 = bitArr[30] % 3;
				tar.bitData.bit31 = bitArr[31] % 3;
			}

			printf("tarNum: %d\n", tar.intNum);
			return tar.intNum;
		}
};

int main(int argc, char *argv[])
{
	int arr[] = {0, 0, 0, 1, 1, 1, 3, 3};
	//int arr[] = {1, 2, 3, 4, 5, 4, 3, 2, 1, 6, 8, 7, 8, 7, 5, 4, 6, 3, 6, 2, 7, 1, 8};

	Solution sou;
	sou.singleNumberII(arr, sizeof(arr) / sizeof(arr[0]));
	
	//funNum fn;
	//printf("%lu\n", sizeof(fn.bitData));
	
	return 0;
}
