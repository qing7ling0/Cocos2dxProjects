#include "Debug.h"
#include <stdarg.h>

Debug::Debug(void)
{
}

void Debug::log(std::string info, ...)
{
	va_list args;
    va_start(args, info);
	CCLOG(info.c_str(), args);
    va_end(args);
}


Debug::~Debug(void)
{
}
