#pragma once
#include <string>
#include "QCommon.h"


class Debug
{
public:
	Debug(void);
	virtual ~Debug(void);
	
	static void log(std::string info, ...);
};

