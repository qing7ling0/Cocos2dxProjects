#include "RecvThread.h"
#include "Debug.h"

RecvThread::~RecvThread(void)
{
}

void RecvThread::run(void)
{
	while(isRun) {
		mMutex.lock();
		int ret = socket->QSelect();
		if (ret == QSELECT_READ)
		{
			recvPacket();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		mMutex.unlock();
	}
}

void RecvThread::recvPacket()
{
	char* headBytes = (char*) malloc(12*CHAR_LENGTH);
	int ret = socket->QRecv(headBytes, 12);
	if (ret > 0)
	{
		Packet *p = new Packet();
		p->setType(readShort(headBytes, 0));
		p->setOption(headBytes[10]);

		int bodyLength = readInt(headBytes, 2);
		if (bodyLength > 0)
		{
			char* body = (char*) malloc(bodyLength*CHAR_LENGTH);
			p->setBody(body);
			p->setBodyLength(bodyLength);
			body = nullptr;
		}

		recvQueue->push(p);
	}

	delete (headBytes);
	headBytes = nullptr;
}