#include <stdio.h>

typedef unsigned char u8_t;

union Band{
	struct
	{
		 u8_t ch1:2;
		 u8_t ch2:2;
		 u8_t ch3:2;
		 u8_t ch4:2;
	}struct_ch;

	u8_t byte;
};

int main(int argc, char *argv[])
{
	Band info;
	info.struct_ch.ch4 = 3;
	
	printf("%lu\n", sizeof(Band));
	printf("%u\n", info.byte);

	return 0;
}
