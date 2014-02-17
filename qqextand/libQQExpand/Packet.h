#pragma once
#include "StreamIO.h"
#include "QCommon.h"

class Packet : public StreamIO
{
protected:
	int type;
	char option;

public :
	int id;
	char callbackId;
	Packet(void);
	~Packet(void);

	int getType() {
		return type;
	}

	void setType(int type) {
		this->type = type;
	}

	q_byte getOption() {
		return option;
	}

	void setOption(q_byte option) {
		this->option = option;
	}

	void setBodyLength(int length) {
		this->length = length;
	}

	int getPacketLength() {
		return getLength()+12;
	}

	q_byte* encoder2Bytes();
};

