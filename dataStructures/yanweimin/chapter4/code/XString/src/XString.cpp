#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xstring.h"

//chars是字符串常量，生成一个其值等于chars的串str
Status strAssign(XString &str, const char *chars)
{
	if(NULL == chars)
	{
		puts("chars == NULL");
		return ERROR;
	}

	if(NULL != str.ch)
	{
		free(str.ch);
		str.ch = NULL;
	}

	str.length = strlen(chars);

	if(0 == str.length)
	{
		puts("chars is empty");
		return ERROR;
	}

	str.ch = (char*)malloc((str.length + 1) * sizeof(char));
	memset(str.ch, 0, str.length + 1);

	if(NULL == str.ch)
	{
		puts("allocate memory failure");
		exit(OVERFLOW);
	}

	strncpy(str.ch, chars, str.length);

	 return OK;
}

//复制srcStr得到destStr
Status strCopy(XString &destStr, const XString &srcStr)
{
	if(NULL == destStr.ch || destStr.length <= 0 ||
	   NULL == srcStr.ch || srcStr.length <= 0)
	{
		return ERROR;
	}

	if(destStr.length >= srcStr.length)
	{
		strncpy(destStr.ch, srcStr.ch, srcStr.length);
	}
	else
	{
		strncpy(destStr.ch, srcStr.ch, destStr.length);
	}
	return OK;
}

//串str存在，若str为空串返回TRUE，否则返回FALSE
Status strEmpty(const XString &str)
{
	return (str.length == 0);
}

//返回字符串str的长度
int strLength(const XString &str)
{
	return (int)str.length;
}

//若srcStr1 > srcStr2，则返回> 0,
//若srcStr1 = srcStr2，则返回 ＝ 0，
//若srcStr1 < srcStr2, 则返回 < 0
int strCompare(XString &srcStr1, XString &srcStr2)
{
	while(srcStr1.ch && srcStr2.ch)
	{
		int diff = *srcStr1.ch++ - *srcStr2.ch++;

		if(0 != diff)
		{
			return diff;
		}
	}

	return srcStr1.length - srcStr2.length;
}

//将str清为空串，并释放str所占的空间
Status strClear(XString &str)
{
	if(NULL != str.ch)
	{
		free(str.ch);
		str.ch = NULL;
	}

	str.length = 0;
	
	return OK;
}

//用destStr返回由srcStr1和srcStr2联接而成的新串
Status strCat(XString &destStr, const XString &srcStr1,  const XString &srcStr2)
{
	if(NULL != destStr.ch)
	{
		free(destStr.ch);
		destStr.ch = NULL;
		destStr.length = 0;
	}

	destStr.ch = (char*)malloc(sizeof(char) * 
			(srcStr1.length + srcStr2.length));

	if(NULL == destStr.ch)
	{
		puts("allocate memory failure");
		exit(OVERFLOW);
	}

	memcpy(destStr.ch, srcStr1.ch, srcStr1.length);
	memcpy(destStr.ch + srcStr1.length, srcStr2.ch, srcStr2.length);
	destStr.length = srcStr1.length + srcStr2.length;

	return OK;
}

//返回串str的第pos个字符起长度为len的子串
Status subStr(XString &subStr, const XString &str, int pos, int len)
{
	if(pos < 0 || pos > str.length || len < 0 || len > str.length - pos)
	{
		return ERROR;
	}

	if(NULL != subStr.ch)
	{
		free(subStr.ch);
		subStr.ch = NULL;
		subStr.length = 0;
	}

	subStr.ch = (char*)malloc(sizeof(char) * len);

	if(NULL == subStr.ch)
	{
		puts("allocate memory failure");
		exit(OVERFLOW);
	}

	memcpy(subStr.ch, str.ch + pos, len);

	return OK;
	
}

int bruteForceFunc(const XString &mainStr, const XString &subStr, int pos)
{
	int i = pos;
	int j = 0;
	
	while(i < mainStr.length && j < subStr.length)
	{
		if(mainStr.ch[i] == subStr.ch[j])
		{
			++i;
			++j;
		}
		else
		{
			i = i -j + 1;
			j = 0;
		}
	}

	if( j >= subStr.length)
	{
		return i - j;
	}
	
	return 0;
}

void getNextVal(const XString subStr, int *nextVal)
{
	int i = 0;
	int j = -1;
	nextVal[0] = 0;

	while(i < subStr.length - 1)
	{
		if(-1 == j || subStr.ch[i] == subStr.ch[j])
		{
			++i;
			++j;
			if(subStr.ch[i] == subStr.ch[j])
			{
				nextVal[i] = nextVal[j];
			}
			else
			{
				nextVal[i] = j;
			}
		}
		else
		{
			j = nextVal[j];
		}
	}
}
void getNext(const XString subStr, int *next)
{
	//此算法在某些情况下存在缺陷
	//例如主串为"aaabaaaab"与模式串"aaaab"
	//修正的算法为getNextVal()
	int i = 1;
	int j = 0;
	
	while( j < subStr.length)
	{
		if(0 == j || subStr.ch[i] == subStr.ch[j])
		{
			++i;
			++j;
			
			next[i] = j;
		}
		else
		{
			j = next[j];
		}
	}
}

int kmpFunc(const XString &mainStr, const XString &subStr, int pos)
{
	int i = pos;
	int j = 0;
	int *nextVal = (int*)malloc(sizeof(int) * subStr.length);
	memset(nextVal, 0, subStr.length);

	if(NULL == nextVal)
	{
		puts("allocate mem failure");
		exit(OVERFLOW);
	}

	getNextVal(subStr, nextVal);

	while((j < subStr.length) && (i < mainStr.length))
	{
		if(mainStr.ch[i] == subStr.ch[j] || 0 == j)
		{
			++i;
			++j;
		}
		else
		{
			j = nextVal[j];
		}
	}

	if(j >= subStr.length)
	{
		return i - j;
	}
	
	return -1;

}

//若主串mainStr存在和串subStr值相同的子串，则返回它在主串中
//第pos个字符之后第一次出现的位置，否则返回0
int indexStr(const XString &mainStr, const XString &subStr, int pos)
{
	//return bruteForceFunc(mainStr, subStr, pos);
	return kmpFunc(mainStr, subStr, pos);
}

//用字符串str3替换字符串str中出现的所有与str2相等的不重叠的子串
Status strReplace(XString &str, const XString &str2, const XString &str3)
{
	if(NULL == str.ch || NULL == str2.ch || NULL == str3.ch ||
			0 == str.length || 0 == str2.length || 0 == str3.length)
	{
		return ERROR;
	}

	int diff = str3.length - str2.length;
	int pos = 0;
	
	while(-1 != (pos = indexStr(str, str2, 0)))
	{
		char *ptr = (char*)malloc((str.length + diff) * sizeof(char));
		memset(ptr, 0, str.length + diff);
		
		strncpy(ptr, str.ch, pos);
		strncpy(ptr + pos, str3.ch, str3.length);
		strncpy(ptr + pos + str3.length, str.ch + pos + str2.length, 
				str.length - pos - str2.length);

		free(str.ch);
		str.ch = ptr;
		str.length = strlen(ptr);
	}
	
	return OK;
}

//在字符串str的pos位置之前插入新字符串insertStr
Status strInsert(XString &str, int pos, const XString &insertStr)
{
	if(pos < 0 || pos > str.length - 1)
	{
		return ERROR;
	}

	if(NULL == insertStr.ch || 0  == insertStr.length)
	{
		return OK;
	}

	str.ch = (char*)realloc(str.ch, str.length + insertStr.length);

	if(NULL == str.ch)
	{
		puts("allocate memory failure");
		exit(OVERFLOW);
	}

	memcpy(str.ch + insertStr.length - 1, str.ch + pos, 
			str.length - pos -1);
	memcpy(str.ch + pos, insertStr.ch, insertStr.length);

	return OK;
}

//从字符串str中删除pos位置之后长度为len的子串
Status strDelete(XString &str, int pos, int len)
{
	if(NULL == str.ch || 0 == str.length || 
			pos < 0 || pos > str.length - 1 || 
			len < 0 || len > str.length - pos)
	{
		return ERROR;
	}

	XString tmpStr;
	tmpStr.ch = (char*)malloc(sizeof(char) * (str.length - len));
	tmpStr.length = str.length -len;
	
	memcpy(tmpStr.ch, str.ch, pos + 1);
	memcpy(tmpStr.ch + pos, str.ch + pos + len, str.length - pos - len);

	free(str.ch);

	str.ch = tmpStr.ch;
	str.length = tmpStr.length;

	return OK;

}

//销毁字符串str
Status strDestory(XString &str)
{
	return strClear(str);
}
