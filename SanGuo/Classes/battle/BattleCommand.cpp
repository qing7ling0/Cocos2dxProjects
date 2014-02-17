#include "BattleCommand.h"
#include "BattleControl.h"

BattleCommand::BattleCommand(void) 
	: _battleControl(nullptr)
{
	_battleControl = BattleControl::getInstance();
	_battleControl->retain();
}

BattleCommand::~BattleCommand(void)
{
	CC_SAFE_RELEASE(_battleControl);
}

void BattleCommand::init()
{
}

void BattleCommand::doing(float dt)
{
}

void BattleCommand::over()
{
}

void BattleCommand::dispose()
{
}


//
// GameStartCommand
//
GameStartCommand::GameStartCommand(void)
{
}

GameStartCommand::~GameStartCommand(void)
{
}

void GameStartCommand::init()
{
	BattleCommand::init();

	Sprite *sp = Sprite::create(s_pathBattleVs);
	sp->setScale(2);
	sp->setOpacity(0);
	sp->setTag(10);
	sp->setPosition(Point(D_display.cx, D_display.cy));

	EaseOut *ease = EaseOut::create(ScaleTo::create(0.3f, 1, 1), 2);
	EaseOut *ease2 = EaseOut::create(FadeIn::create(0.3f), 2);

	Sequence *se = Sequence::create(ease2,
		DelayTime::create(2),
		CallFunc::create(std::bind(&GameStartCommand::over, this)),
		NULL);

	sp->runAction(ease);
	sp->runAction(se);

	_battleControl->getBattleLayer()->addChild(sp);
}

void GameStartCommand::doing(float dt)
{
}

void GameStartCommand::over()
{
	_battleControl->setCommand(new SelfRoundCommand());
}

void GameStartCommand::dispose()
{
	BattleCommand::dispose();
	_battleControl->getBattleLayer()->removeChildByTag(10);
}



//
// SelfRoundCommand
//
SelfRoundCommand::SelfRoundCommand(void):_start(false)
{
}

SelfRoundCommand::~SelfRoundCommand(void)
{
}

void SelfRoundCommand::init()
{
	BattleCommand::init();

	Sprite *sp = Sprite::create(s_pathBattleYourTurn);
	sp->setScale(2);
	sp->setOpacity(0);
	sp->setPosition(Point(D_display.cx, D_display.cy));
	sp->setTag(10);

	EaseOut *ease = EaseOut::create(ScaleTo::create(0.3f, 1, 1), 2);
	EaseOut *ease2 = EaseOut::create(FadeIn::create(0.3f), 2);
	
	Sequence *se = Sequence::create(ease2,
		DelayTime::create(2),
		FadeOut::create(0.2f), 
		CallFunc::create([&](){
							_start = true;
							_battleControl->getBattleLayer()->removeChildByTag(10);
                        }),
		NULL);

	sp->runAction(ease);
	sp->runAction(se);

	_battleControl->getBattleLayer()->addChild(sp);
}

void SelfRoundCommand::start()
{
	_start = true;
	_battleControl->getBattleLayer()->removeChildByTag(10);
}

void SelfRoundCommand::doing(float dt)
{
	if (_start)
	{

	}
}

void SelfRoundCommand::over()
{
}

void SelfRoundCommand::dispose()
{
	BattleCommand::dispose();
}



//
// OtherRoundCommand
//
OtherRoundCommand::OtherRoundCommand(void)
{
}

OtherRoundCommand::~OtherRoundCommand(void)
{
}

void OtherRoundCommand::init()
{
	BattleCommand::init();
}

void OtherRoundCommand::doing(float dt)
{
}

void OtherRoundCommand::over()
{
}

void OtherRoundCommand::dispose()
{
	BattleCommand::dispose();
}



//
// WaitNextRoundCommand
//
WaitNextRoundCommand::WaitNextRoundCommand(void)
{
}

WaitNextRoundCommand::~WaitNextRoundCommand(void)
{
}

void WaitNextRoundCommand::init()
{
	BattleCommand::init();
}

void WaitNextRoundCommand::doing(float dt)
{
}

void WaitNextRoundCommand::over()
{
}

void WaitNextRoundCommand::dispose()
{
	BattleCommand::dispose();
}

