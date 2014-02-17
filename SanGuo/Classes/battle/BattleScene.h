#pragma once
#include "cocos2d.h"
#include "QConfig.h"
#include "CCSHead.h"
#include "BattleRes.h"

class BattleScene : public LayerRGBA
{
public:
	~BattleScene(void);

	virtual bool init();

	virtual void onEnter();

	virtual void onExit();

	void endRound();

	void loadBattleUI();

	void doing(float dt);

	bool loadOK();

	void CCSUIBtnTouchListener(Object* pSender, TouchEventType eventType);

	CREATE_SECNE_FUNC(BattleScene);

private:
	UILayout *battleUI;
	UIButton *endRoundBtn;
	BattleRes *_res;
	bool loadOk;
	float _oldPer;
	LabelTTF *_labelPercent;
};

