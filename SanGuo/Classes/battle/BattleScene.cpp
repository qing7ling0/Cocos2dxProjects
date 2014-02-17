#include "BattleScene.h"
#include "BattleControl.h"
#include "game\CommonRes.h"
#include "BattleCommand.h"
#include "game\Messages.h"

BattleScene::~BattleScene(void)
{
	BattleControl::getInstance()->dispose();

	CC_SAFE_RELEASE(battleUI);
	CC_SAFE_RELEASE(endRoundBtn);
	CC_SAFE_RELEASE(_res);
}

bool BattleScene::init(void)
{
	if (LayerRGBA::init())
	{
		log("BattleScene init");
		auto *spBg = Sprite::create(s_pathloadingbg);
		spBg->setPosition(Point(D_display.cx, D_display.cy));
		addChild(spBg, -1);
		spBg->setTag(10);

		auto _labelLoading = LabelTTF::create("loading...", "Arial", 25);

		_labelLoading->setPosition(Point(D_display.cx, 100));
		_labelLoading->setAnchorPoint(Point(1, 0.5f));
		spBg->addChild(_labelLoading);
		
		_labelPercent = LabelTTF::create("%0", "Arial", 25);
		_labelPercent->setPosition(Point(D_display.cx, 100));
		_labelPercent->setAnchorPoint(Point(0, 0.5f));
		spBg->addChild(_labelPercent);

		_oldPer = 0;
		this->schedule(schedule_selector(BattleScene::doing));


		MessagesLayer *ml = MessagesLayer::create();
		addChild(ml);

		_res = new BattleRes();
		_res->load();

		loadOk = false;

		return true;
	}

	return false;
}

void BattleScene::doing(float dt)
{
	if (!loadOk && _res)
	{
		log("BattleScene doing");
		float newPer = _res->loadProgress();
		if (newPer != _oldPer && newPer<2)
		{
			_oldPer = newPer;
			 char tmp[10];
			sprintf(tmp,"%%%d", (int)(((float)_oldPer) * 100));
			_labelPercent->setString(tmp);
			log("BattleScene doing load per=%s", tmp);
			if (newPer >= 1)
			{
				newPer = 2;
			}
		}
		if (newPer == 2)
		{
			loadOk = true;
			removeChildByTag(10);

			loadBattleUI();
		}
	}
	if (loadOk)
	{
		BattleControl::getInstance()->update(dt);
	}
}

void BattleScene::loadBattleUI()
{
	UILayer *layer = UILayer::create(); 
	//将UILayer层加入到当前的场景 
	this->addChild(layer); 
	UILayout *m_pWidget = dynamic_cast<UILayout*>(cocostudio::GUIReader::shareReader()->widgetFromJsonFile("battle_1.json"));
	layer->addWidget(m_pWidget);

	battleUI = dynamic_cast<UILayout*> (layer->getWidgetByName("battle_ui"));
	battleUI->retain();

	endRoundBtn = dynamic_cast<UIButton*> (layer->getWidgetByName("btn_end"));
	endRoundBtn->retain();

	endRoundBtn->addTouchEventListener(this, SEL_TouchEvent(&BattleScene::CCSUIBtnTouchListener));
	
	BattleControl::getInstance()->setBattleUI(battleUI);
	BattleControl::getInstance()->setCommand(new GameStartCommand());
	BattleControl::getInstance()->load();
}


void BattleScene::onEnter(void)
{
	LayerRGBA::onEnter();
}

void BattleScene::onExit(void)
{
	LayerRGBA::onExit();
}

void BattleScene::endRound(void)
{
	log("battle scene end round");
	BattleControl::getInstance()->nextRound();
	MessagesService::getInstance()->addMessage(MessageItem::create("下一回合"));
}

bool BattleScene::loadOK()
{
	return loadOk;
}

void BattleScene::CCSUIBtnTouchListener(Object* pSender, TouchEventType eventType)
{
	if (eventType == TOUCH_EVENT_ENDED)
	{
		this->endRound();
	}
}
