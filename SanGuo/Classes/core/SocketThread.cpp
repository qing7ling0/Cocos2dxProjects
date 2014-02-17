#include "SocketThread.h"
#include <functional>
#include "Debug.h"

void fun(SocketThread *sthread)
{
	sthread->run();
}

void SocketThread::start(void)
{
	mMutex.lock();
	isRun = true;
	if (mThread)
	{
		delete(mThread);
		mThread = nullptr;
	}
	mThread = new std::thread(fun, this);
	mThread->detach();
	mMutex.unlock();
	QLog(" socket thread -------- start");
} 

void SocketThread::stop(void)
{
	mMutex.lock();
	isRun = false;
	mMutex.unlock();
	QLog(" socket thread -------- stop");
}

void SocketThread::run()
{
	while(isRun)
	{
		QLog(" socket thread -------- doing");
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

SocketThread::SocketThread(ODSocket *socket)
{
	mThread = nullptr;
	this->socket = socket;
}

SocketThread::~SocketThread(void)
{
	if (NULL == mThread)
	{
		delete mThread;
		mThread = NULL;
	}
}
