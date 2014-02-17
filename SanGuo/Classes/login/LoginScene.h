#pragma once
#include "cocos2d.h"
#include "QConfig.h"
#include "CCSHead.h"

class LoginScene : public cocos2d::Layer
{
public:
	~LoginScene(void);

	virtual bool init();

	void startGameTouchListener(Object* pSender, int eventType);

	CREATE_SECNE_FUNC(LoginScene);
};

