#include "NetHandlers.h"
#include "cocos2d.h"

USING_NS_CC;

static NetHandlers* instance = nullptr;

NetHandlers* NetHandlers::getInstance()
{
	if (nullptr == instance)
	{
		instance = new NetHandlers();
	}
	return instance;
}

NetHandlers::NetHandlers(void)
{
	handler = new Handler();
}


NetHandlers::~NetHandlers(void)
{
	if (handler)
	{
		delete handler;
		handler = nullptr;
	}
}


void NetHandlers::parsePacket(Packet *p)
{
	handler->parse(p);
}

