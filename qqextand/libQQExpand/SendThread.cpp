#include "SendThread.h"
#include "Debug.h"

SendThread::~SendThread(void)
{
	isRun = false;
}

void SendThread::run(void)
{
	while(isRun) {
		mMutex.lock();
		std::queue<Packet*>* packQ = sendQueue;
		if (!packQ->empty())
		{
			Packet *p = packQ->front();
			packQ->pop();

			char* chs = p->encoder2Bytes();
			socket->QSend(chs, p->getPacketLength());
			StreamIO::free_bytes(chs);
			chs = nullptr;
			delete p;
			p = nullptr;
			QLog(" SendThread -------- send packet");
		}
		mMutex.unlock();
	}
}