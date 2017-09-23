#include "MicroDefine.h"

typedef struct
{
	char *ch;
	int length;
}XString;

//chars是字符串常量，生成一个其值等于chars的串str
Status strAssign(XString &str, const char *chars);

//复制srcStr得到destStr
Status strCopy(XString &destStr, const XString &srcStr);

//串str存在，若str为空串返回TRUE，否则返回FALSE
Status strEmpty(const XString &str);

//返回字符串str的长度
int strLength(const XString &str);

//若srcStr1 > srcStr2，则返回> 0,
//若srcStr1 = srcStr2，则返回 ＝ 0，
//若srcStr1 < srcStr2, 则返回 < 0
int strCompare(XString &srcStr1, XString &srcStr2);

//将str清为空串，并释放str所占的空间
Status strClear(XString &str);

//用destStr返回由srcStr1和srcStr2联接而成的新串
Status strCat(XString &destStr, const XString &srcStr1,  const XString &srcStr2);

//返回串str的第pos个字符起长度为len的子串
Status subStr(XString &subStr, const XString &str, int pos, int len);

//若主串mainStr存在和串subStr值相同的子串，则返回它在主串中
//第pos个字符之后第一次出现的位置，否则返回0
int indexStr(const XString &mainStr, const XString &subStr, int pos);

//用字符串str3替换字符串str中出现的所有与str2相等的不重叠的子串
Status strReplace(XString &str, const XString &str2, const XString &str3);

//在字符串str的pos位置之前插入新字符串insertStr
Status strInsert(XString &str, int pos, const XString &insertStr);

//从字符串str中删除pos位置之后长度为len的子串
Status strDelete(XString &str, int pos, int len);

//销毁字符串str
Status strDestory(XString &str);
