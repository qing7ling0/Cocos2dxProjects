#pragma once
// 此类主要 接受服务器推送过来的消息
#include <queue>
#include "SocketThread.h"
#include "Packet.h"

class RecvThread : public SocketThread
{
public:	
	~RecvThread(void);
	RecvThread(ODSocket *socket, std::queue<Packet*>* recvQueue):SocketThread(socket)
	{
		this->recvQueue = recvQueue;
	};

	void start(void) override {
		SocketThread::start();
	};

protected:
	void run(void) override;
	void recvPacket();
	std::queue<Packet*>* recvQueue;
};
