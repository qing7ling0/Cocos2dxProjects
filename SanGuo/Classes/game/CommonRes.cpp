#include "CommonRes.h"


void CommonRes::load(void)
{
	totalCount = 1;
	loadCount = 0;

	Director::getInstance()->getTextureCache()->addImageAsync(s_pathloadingbg, this, callfuncO_selector(CommonRes::loadCallback));
}

void CommonRes::loadCallback(Object *obj)
{
	loadCount++;
}

float CommonRes::loadProgress()
{
	return loadCount *1.0f / totalCount;
}

