#include "Handler.h"

Handler::Handler(void)
{
}

void Handler::parse(Packet *p)
{
	int type = p->getType();
	switch (type)
	{
	case 0x00f3:
		QLog(" Handler recv ping");
		break;
	default:
		break;
	}
}


Handler::~Handler(void)
{
}
