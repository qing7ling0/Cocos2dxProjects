#pragma once
#include <string>
#include "core\Core.h"

#define SCREEN_WIDTH			960
#define SCREEN_HEIGHT			640
#define FPS_SHOW				true
#define FPS						60
#define PATH_COUNT				2

/** 配置的资源搜寻路径 */
const std::string CONFIG_PATHS[PATH_COUNT] = {
	"images", "particle"
};

#define CREATE_SECNE_FUNC(__TYPE__) \
CREATE_FUNC(__TYPE__) \
static cocos2d::Scene* createScene() \
{ \
	auto scene = Scene::create(); \
	auto layer = __TYPE__::create(); \
    scene->addChild(layer); \
    return scene; \
}

struct Display
{
	float x;
	float y;
	float w;
	float h;
	float cx;
	float cy;
};

extern Display D_display;


//#define _COCOS2DX_WIN32_ //win32下使用icovnlib转换中文
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define UTF8_STR(str) GBKToUTF8(str,"gb2312","utf-8")
#define UTF8_CHAR(chs) GBKToUTF8(std::string(chs),"gb2312","utf-8")
#else
#define UTF8_STR(str) str
#define UTF8_CHAR(chs) chs
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "iconv.h"
static char* GBKToUTF8(std::string & gbkStr, const char* toCode, const char* fromCode)
{
	iconv_t iconvH;
	iconvH = iconv_open(fromCode, toCode);
	if (iconvH == 0) { return ""; }

	const char* strChar = gbkStr.c_str();
	const char** pin = &strChar;
	size_t strLength = gbkStr.length();
	char* outbuf = (char*) malloc(strLength*4);
	char* pBuff = outbuf;

	memset( outbuf, 0, strLength*4);
	size_t outLength = strLength*4;

	if (-1 == iconv(iconvH, pin, &strLength, &outbuf, &outLength)){
		iconv_close(iconvH);
		return pBuff;
	}

	iconv_close(iconvH);

	return pBuff;
}
#endif


#include "XStrings.h"


enum NodeOrder
{
    normal=0,

	// 在准备区
    messagebox=1000,

};

