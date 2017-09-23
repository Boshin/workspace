#include <stdio.h>

/*
 * 将塔座X上的按直径由小到大且自上而下的n个圆盘
 * 按规则搬到塔座Z上，y可用作辅助塔座。
 * 搬动操作move(x, n, z)可可定义为(cnt是初值为0的全局变量，对搬动计数):
 * printf("cnt: %d, MOve disk %d from %c to %c\n", ++cnt, n, x, z);
 */
void hanoi(int n, char x, char y, char z);
