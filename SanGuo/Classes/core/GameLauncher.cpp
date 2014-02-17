#include "GameLauncher.h"
#include "SocketConn.h"

static GameLauncher* instance = nullptr;

GameLauncher::GameLauncher(void)
{
	instance = this;
}


GameLauncher::~GameLauncher(void)
{
	SocketConn::dispose();
	
}

void GameLauncher::onLoad(void)
{
	SocketConn::create();
}

void GameLauncher::onDispose(void)
{
	do
	{
		if (nullptr != instance)
		{
			delete instance;
			instance = nullptr;
		}
	} while (0);
}

GameLauncher* GameLauncher::getInstance(void)
{
	return instance;
}

