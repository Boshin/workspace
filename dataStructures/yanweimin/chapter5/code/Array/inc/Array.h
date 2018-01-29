#include "MicroDefine.h"

#define MAX_ARRAY_DIM 8

typedef int ElemType;

typedef struct
{
	ElemType *base;     //数组元素基址，由initArray分配
	int dim;            //数组维数
	int *bounds;        //数组维界基址，由initArray分配
	int *constants;     //数组映像函数常量基址，由initArray分配
}Array;

//若维数dim和随后的各维长度合法，
//则构造相应的数组element，并返回OK
Status initArray(Array &A, int dim, ...);

//销毁数组
Status destoryArray(Array &A);

//A是n维数组，e为元素变量，随后是n个下标值
//若下标不越界，则将e赋值为所指定的A的元素，并返回OK
Status value(const Array &A, ElemType &e, ...);

//A是n维数组，e为元素变量，随后是n个下标值
//若下标不越界，则将e的值赋给所指定的A的元素，并返回OK
Status assign(Array &A, const ElemType &e, ...);
