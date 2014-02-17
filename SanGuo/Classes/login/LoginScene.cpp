#include "LoginScene.h"
#include "battle\BattleScene.h"


LoginScene::~LoginScene(void)
{
}


bool LoginScene::init(void)
{
	if (Layer::init())
	{
		UILayer *loginUI =UILayer::create(); 
		//将UILayer层加入到当前的场景 
		this->addChild(loginUI); 
		UILayout *m_pWidget = dynamic_cast<UILayout*>(cocostudio::GUIReader::shareReader()->widgetFromJsonFile("main/main.ExportJson"));
		loginUI->addWidget(m_pWidget);
		

		UIButton *startBtn = dynamic_cast<UIButton*> (loginUI->getWidgetByName("btn_battle"));

		startBtn->setPressedActionEnabled(true);
		startBtn->setBright(true);

		startBtn->addTouchEventListener(this, toucheventselector(LoginScene::startGameTouchListener));

		return true;
	}

	return false;
}

void LoginScene::startGameTouchListener(Object* pSender, int eventType)
{
	if (eventType == TOUCH_EVENT_ENDED)
	{
		Scene *sc = BattleScene::createScene();
        Director::getInstance()->replaceScene(
		TransitionFade::create(0.5f, sc));
		
	}
}
