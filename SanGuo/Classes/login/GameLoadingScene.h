#pragma once
#include "cocos2d.h"
#include "QConfig.h"
#include "CCSHead.h"
#include "game/CommonRes.h"

#include "../qqextand/libQQExpand/WaterWaveAction.h"

USING_NS_CC;

class GameLoading : public Layer
{
public:
	~GameLoading(void);

	virtual bool init();

	virtual void onEnter();
	
	void doing(float dt);

	void loadingFinsih(float dt);

	void startGameTouchListener(Object* pSender, int eventType);

	bool onTouchBegan(Touch *touch, Event *unused_event); 

    void onTouchMoved(Touch *touch, Event *unused_event);

	void dropStone(const Point& position);

	CREATE_SECNE_FUNC(GameLoading);
private:
	UILayer *loginUI;
	ActionObject *loadingBar;
	UIButton *startBtn;
	CommonRes *_res;

	WaterWaveAction *_waveAction;
	struct timeval *_lastUpdate;
};

