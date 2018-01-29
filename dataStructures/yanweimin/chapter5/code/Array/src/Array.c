#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Array.h"

Status initArray(Array &A, int dim, ...)
{
	if(dim < 1 || dim > MAX_ARRAY_DIM)
	{
		return ERROR;
	}
	
	A.dim = dim;
	A.bounds = (int*)malloc(dim * sizeof(int));

	if(NULL == A.bounds)
	{
		puts("allocate mem fail");
		exit(OVERFLOW);
	}

	int elemtotal = 1;
	va_list ap;
	va_start(ap, dim);

	for(int i = 0; i < dim; ++i)
	{
		A.bounds[i] = va_arg(ap, int);

		if(A.bounds[i] < 0)
		{
			return UNDERFLOW;
		}

		elemtotal *= A.bounds[i];
	}

	va_end(ap);

	A.base = (ElemType*)malloc(elemtotal * sizeof(ElemType));

	if(NULL == A.base)
	{
		puts("allocate mem fail");
		exit(OVERFLOW);
	}

	//求映像函数的常数c，并存入A.constants[i - 1], i = 1, ..., dim
	A.constants = (int*)malloc(dim * sizeof(int));

	if(NULL == A.constants)
	{
		puts("allocate mem fail");
		exit(OVERFLOW);
	}

	A.constants[dim - 1] = 1; 	//L=1,指针的增减以元素的大小为单位

	for(int i = dim - 2; i >= 0; --i)
	{
		A.constants[i] = A.bounds[i + 1] * A.constants[i + 1];
	}

	return OK;
}

Status destoryArray(Array &A)
{
	if(NULL == A.base)
	{
		return ERROR;
	}

	free(A.base);
	A.base = NULL;

	if(NULL == A.bounds)
	{
		return ERROR;
	}

	free(A.bounds);
	A.bounds = NULL;
	
	if(NULL == A.constants)
	{
		return ERROR;
	}

	free(A.constants);
	A.constants = NULL;

	return OK;
}

//若ap指示的各下标值合法，则求出该元素在A中的相对位置
Status locate(const Array &A, va_list ap, int &offset)
{
	offset = 0;

	for(int i = 0; i < A.dim; ++i)
	{
		int tmpValue = va_arg(ap, int);

		if(tmpValue < 0 || tmpValue >= A.bounds[i])
		{
			return OVERFLOW;
		}

		offset += A.constants[i] * tmpValue;
	}
}

Status value(const Array &A, ElemType &e, ...)
{
	int result = 0;
	int offset = 0;
	va_list ap;
	va_start(ap, e);
	
	if((result = locate(A, ap, offset)) <= 0)
	{
		return result;
	}

	e = *(A.base + offset);

	return OK;
}

Status assign(Array &A, const ElemType &e, ...)
{
	int result = 0;
	int offset = 0;
	va_list ap;
	va_start(ap, e);
	
	if((result = locate(A, ap, offset)) <= 0)
	{
		return result;
	}

	*(A.base + offset) = e;

	return OK;
}
