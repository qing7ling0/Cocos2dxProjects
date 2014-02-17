#include "SocketConn.h"
#include "Debug.h"
#include "NetHandlers.h"


static SocketConn* instance = nullptr;

void SocketConn::create()
{
	if (nullptr == instance)
	{
		instance = new SocketConn();
	}
}

void SocketConn::dispose()
{
	if (nullptr == instance)
	{
		delete instance;
		instance = nullptr;
	}
}

SocketConn* SocketConn::getInstance()
{
	return instance;
}

void SocketConn::openConn(const char *ip, unsigned short port)
{
	const string key(ip);
	if (connect_key.compare(key) == 0)
	{
		CCLOG("SocketConn openConn ip=%s has open", ip);
		return;
	}
	connect_key = key;
	if (nullptr == odSocket)
	{
		odSocket = new ODSocket();
		odSocket->QInit();
		odSocket->QCreate(AF_INET, SOCK_STREAM, 0);
	}
	else
	{
		close();
	}

	connect(ip, port);
}

void SocketConn::connect(const char *ip, unsigned short port)
{
	if (!connected)
	{
		if (nullptr != thread)
		{
			delete thread;
			thread = nullptr;
		}
		thread = new std::thread([this, ip, port](){
			this->mMutex.lock();
			bool ok = this->odSocket->QConnect(ip, port);

			if (!ok)
			{
				connect_key = "";
				connected = false;
				this->connectFail();
			}
			else
			{
				sendThread = new SendThread(odSocket, sendQueue);
				sendThread->start();
				recvThread = new RecvThread(odSocket, recvQueue);
				recvThread->start();
				
				connected = true;
				QLog("SocketConn connect ok");
			}
		
			this->mMutex.unlock();
		});
		thread->detach();
	}
}

void SocketConn::send(Packet* p)
{
	mMutex.lock();
	sendQueue->push(p);
	mMutex.unlock();
}

void SocketConn::close()
{
	if (sendThread)
	{
		sendThread->stop();
		CC_SAFE_DELETE(sendThread);
	}
				
	if (recvThread)
	{
		recvThread->stop();
		CC_SAFE_DELETE(recvThread);
	}

	connected = false;
	odSocket->QClean();
	odSocket->QClose();
	cleanPacket();
}

void SocketConn::timeOut()
{
	close();
}

void SocketConn::connectFail()
{
	QLog(" SocketConn -------- connectFail");
	close();
}

SocketConn::SocketConn()
{
	Director::getInstance()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&SocketConn::update), this, 0.1f, false);

	sendQueue = new queue<Packet*>();
	recvQueue = new queue<Packet*>();

	connected = false;
	recvThread = nullptr;
	sendThread = nullptr;
	odSocket = nullptr;
	thread = nullptr;

}

void SocketConn::update(float dt)
{
	mMutex.lock();
	if (!recvQueue->empty())
	{
		Packet *p = recvQueue->front();
		recvQueue->pop();
		NetHandlers::getInstance()->parsePacket(p);
		p = nullptr;
	}
	mMutex.unlock();
}

void SocketConn::cleanPacket()
{
	while(!recvQueue->empty())
	{
		Packet* p = recvQueue->front();
		recvQueue->pop();
		CC_SAFE_DELETE(p);
	}

	while(!sendQueue->empty())
	{
		Packet* p = sendQueue->front();
		sendQueue->pop();
		CC_SAFE_DELETE(p);
	}
}

SocketConn::~SocketConn()
{
	close();

	CC_SAFE_DELETE(recvThread);
	CC_SAFE_DELETE(sendThread);
	CC_SAFE_DELETE(odSocket);
	CC_SAFE_DELETE(thread);
	CC_SAFE_DELETE(sendQueue);
	CC_SAFE_DELETE(recvQueue);
}