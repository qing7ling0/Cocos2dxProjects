#include "Packet.h"
#include "Encoder.h"

Packet::Packet(void)
{
	option = 0;
	callbackId = 0;
}

q_byte* Packet::encoder2Bytes()
{
	char* ret = (char *) malloc((12+getLength())*CHAR_LENGTH);
	int index = 0;
	char* temp = short2Bytes((short)type);
	memcpy(ret, temp, 2);
	index += 2;

	temp = int2Bytes(getLength());
	memcpy(ret+index, temp, 4);
	index += 4;
	
	temp = int2Bytes(id);
	memcpy(ret+index, temp, 4);
	index += 4;
	
	ret[10] = option;
	ret[11] = callbackId;
	index = 12;

	if (getLength() > 0)
	{
		memcpy(ret+index, getBody(), getLength());
	}

	return ret;
}


Packet::~Packet(void)
{
}
