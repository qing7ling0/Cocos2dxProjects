#pragma once
class GameLauncher
{
public:

	static GameLauncher* getInstance();

	virtual void onLoad();

	virtual void onDispose();

protected:

	GameLauncher(void);

	~GameLauncher(void);
};

