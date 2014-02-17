#pragma once
#include "GameLauncher.h"
#include "cocos2d.h"
#include "QConfig.h"

class SGLauncher : public GameLauncher
{
public:

	static void create();

	void onLoad() override;

	void onDispose() override;

protected:

	SGLauncher(void);
	~SGLauncher(void);
};

