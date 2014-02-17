#ifndef __ENCODER__
#define __ENCODER__

#include <string>
#include <vector>
#include "QCommon.h"

using namespace std;

static int bytes2int(const char* s)
{
    return ((s[0]&0xff)
            +((s[1]&0xff)<<8)
            +((s[2]&0xff)<<16)
            +((s[3]&0xff)<<24));
}
    
static short bytes2short(const char*s)
{
    return ((s[0]&0xff)
            +((s[1]&0xff)<<8));
}

/*
  整数到字节数组的转换..
*/
static char* int2Bytes(int number)
{
	int temp = number;
	//char* ret = (char*)malloc(4);
    static int intbuf;
    char* ret = (char*)&intbuf;
	for (int i = 0; i < 4; i++) {
		ret[i] = (char) (temp & 0xff);
		temp = temp >> 8; // 向右移8位
	}
	return ret;
}

/*
  将一个short整数转换为一个byte数组
*/
static char* short2Bytes(short input)
{
    static short shortbuf;
    char* ret = (char*)&shortbuf;
	ret[0] = (char) (input & 0xFF);
	ret[1] = (char) ((input >> 8) & 0xFF);
	return ret;
}

static string bytes2String(char* bytes)
{
	return string(bytes);
}

static char* string2bytes(string str)
{
	const char* chs = str.data();
	int len = str.length()+3;
	char* ret = (char *) malloc(len * CHAR_LENGTH); 

	strncpy (ret+2, chs, len-2);
	ret[0] = len & 0xff;
	ret[1] = (len>>8) & 0xff;

	return ret;
}

/*
	把两个字节组合为一个short数字
	high8bit short数字的高8位
	low8bit short数字的低8位
*/
static short getShortFrom2Byte(char high8bit, char low8bit) 
{
	return (short) ((high8bit << 8) | (low8bit & 0xFF));
}

static short readShort(char* bytes, int offset)
{
	return getShortFrom2Byte(bytes[offset+1], bytes[offset]);
}

static char* readBytes(char* bytes, int offset, int length)
{
	char* ret = (char *) malloc(length);
	memcpy(ret, bytes+offset, length);

	return ret;
}

static string readString(char* bytes, int offset)
{
	short length = readShort(bytes, offset);
	offset += 2;

	char* str_bytes = readBytes(bytes, offset, length-2);
	str_bytes[length-3] = '\0';
	return bytes2String(str_bytes);
}

static short readInt(char* bytes, int offset)
{
	int result = 0;
	int t1 = bytes[offset++] & 0xff;
	int t2 = (bytes[offset++] << 8) & 0xff00;
	int t3 = (bytes[offset++] << 16) & 0xff0000;
	int t4 = (bytes[offset++] << 24) & 0xff000000;
	result = t1 | t2 | t3 | t4;

	return result;
}

#endif