#pragma once
#include "QCommon.h"
#include "Packet.h"
#include "Handler.h"

class NetHandlers
{
public:
	NetHandlers(void);

	~NetHandlers(void);

	void parsePacket(Packet *p);

	static NetHandlers* getInstance();

private:
	Handler *handler;
};

