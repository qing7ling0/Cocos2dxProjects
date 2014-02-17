#pragma once
#include "QCommon.h"
#include "Packet.h"

class Handler
{
public:
	Handler(void);
	~Handler(void);
	void parse(Packet *p);
};

