#pragma once
#include "core\GameLauncher.h"
#include "cocos2d.h"

class SGLauncher : public GameLauncher
{
public:

	static void create();

	virtual void onLoad() override;

	virtual void onDispose() override;

protected:

	SGLauncher(void);
	~SGLauncher(void);
};

