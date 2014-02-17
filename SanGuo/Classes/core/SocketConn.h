#pragma once
#include <string.h>
#include <thread>
#include <queue>
#include <mutex>
#include <stdlib.h>
#include "ODSocket.h"
#include "SocketThread.h"
#include "RecvThread.h"
#include "SendThread.h"
#include "Packet.h"
#include "cocos2d.h"

USING_NS_CC;


class SocketConn : public Object
{

public:
	static void create();

	static void dispose();

	void update(float dt);

	static SocketConn* getInstance();

	void openConn(const char *ip, unsigned short port);
	
	void send(Packet *p);

	void prasePacket(Packet *p);

private:
	RecvThread *recvThread;
	SendThread *sendThread;
	ODSocket *odSocket;
	std::thread *thread;
	std::mutex mMutex;
	std::string connect_key;
	bool connected;
	std::queue<Packet*>* sendQueue;
	std::queue<Packet*>* recvQueue;

public:
	
	void timeOut();

private:

	void cleanPacket();

	void close();

	void connectFail();

	void connect(const char *ip, unsigned short port);

	SocketConn();

	~SocketConn();
};
