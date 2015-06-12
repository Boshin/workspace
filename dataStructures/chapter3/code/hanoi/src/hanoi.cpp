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
        move(x, 1, z);  //将编号为1的圆盘从x移动到z
    }
    else
    {
        //printf("hanoi(n - 1: %d, x: %c, z: %c, y: %c)\n", n - 1, x, z, y);
        hanoi(n - 1, x, z, y);  //将x上编号为1至n-1的圆盘移动到y，z作辅助塔
        //printf("move(x: %c, n: %d, z: %c)\n", x, n, z);
        move(x, n, z);          //将编号为n的圆盘从x移动到z
        //printf("hanoi(n - 1: %d, y: %c, x: %c, z: %c)\n", n - 1, y, x, z);
        hanoi(n - 1, y, x, z);  //将y上编号1至n－1的圆盘移动到z，x作为辅助塔
    }
}
