#pragma once
#include <thread>
#include <mutex>
#include "ODSocket.h"

class SocketThread
{
public:	
	~SocketThread(void);
	SocketThread(ODSocket *socket);

	virtual	void start(void);

	//函数中止当前线程。
	virtual void stop(void);

	virtual void run();

protected:

	// 0 表示连接成功 1 表示连接失败
	int state;

	ODSocket *socket;

	std::thread *mThread;

	bool isRun;
	std::mutex mMutex;
};

