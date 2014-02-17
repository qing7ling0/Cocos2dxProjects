#pragma once
#include "QConfig.h"

static const char s_pathloadingbg[]						= "images/loading_bg.jpg";


class CommonRes : public Object
{
public:
	void load();

	void loadCallback(Object *obj);

	float loadProgress();

protected:
	int totalCount;
	int loadCount;
};

