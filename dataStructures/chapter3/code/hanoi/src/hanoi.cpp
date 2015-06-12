#include "hanoi.h"

int cnt = 0;

void move(char x, int n, char z)
{
    printf("cnt: %d, Move disk %d from %c to %c\n", ++cnt, n, x, z);
}

void hanoi(int n, char x, char y, char z)
{
    if(1 == n)
    {
        move(x, 1, z);  //�����Ϊ1��Բ�̴�x�ƶ���z
    }
    else
    {
        //printf("hanoi(n - 1: %d, x: %c, z: %c, y: %c)\n", n - 1, x, z, y);
        hanoi(n - 1, x, z, y);  //��x�ϱ��Ϊ1��n-1��Բ���ƶ���y��z��������
        //printf("move(x: %c, n: %d, z: %c)\n", x, n, z);
        move(x, n, z);          //�����Ϊn��Բ�̴�x�ƶ���z
        //printf("hanoi(n - 1: %d, y: %c, x: %c, z: %c)\n", n - 1, y, x, z);
        hanoi(n - 1, y, x, z);  //��y�ϱ��1��n��1��Բ���ƶ���z��x��Ϊ������
    }
}
