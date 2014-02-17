#include "RoleAction.h"

RoleAction::RoleAction(void):_hero(nullptr),
	_over(false),
	_start(false),
	_battle(BattleControl::getInstance()),
	step(0)
{
}

RoleAction::~RoleAction(void)
{
	_hero = nullptr;
}

void RoleAction::start()
{
}

void RoleAction::doing(float dt)
{
}

bool RoleAction::isOver()
{
	return _over;
}

void RoleAction::over()
{
	_over = true;
}

bool RoleAction::isStarted()
{
	return _start;
}

HeroCard* RoleAction::getHero()
{
	return _hero;
}


//
// ChooseCard
//
ChooseCard* ChooseCard::create(HeroCard *hero)
{
	ChooseCard* action = new ChooseCard();
    if (action && action->init(hero))
    {
        action->autorelease();
        return action;
    }

    CC_SAFE_DELETE(action);
    return nullptr;
}

bool ChooseCard::init(HeroCard *hero)
{
	_hero = hero;
	return true;
}

void ChooseCard::start()
{
	_start = true;
}

void ChooseCard::doing(float dt)
{
	if (_hero)
	{
		if (step == 0)
		{
			CardVo* vo = _hero->chooseOneCard();
			if (vo)
			{
				//Card *card = Card::create(vo);
				_battle->getActions()->addAction(GetOneCard::create(_hero, vo));
				//if (card && _hero->addReadyCards(card))
				//{
				//	_battle->addReadyCard(card, _hero);
				//}
				
			}
			step++;
		}
		else if (step == 1)
		{
			_over = true;
		}
	}
}


//
// ChooseCard
//
GetOneCard* GetOneCard::create(HeroCard *hero, CardVo *cardvo)
{
	GetOneCard* action = new GetOneCard();
    if (action && action->init(hero, cardvo))
    {
        action->autorelease();
        return action;
    }

    CC_SAFE_DELETE(action);
    return nullptr;
}

bool GetOneCard::init(HeroCard *hero, CardVo *cardvo)
{
	_hero = hero;
	_getCardVo = cardvo;
	_getCard = nullptr;
	_start = false;
	return true;
}

void GetOneCard::start()
{
	_start = true;
	if (_hero && _getCardVo)
	{
		Card *card = Card::create(_getCardVo);
		if (_hero->addReadyCards(card))
		{
			_getCard = card;
		}
	}
}

void GetOneCard::doing(float dt)
{
	if (_start && !_over && _getCard)
	{
		if (step == 0)
		{
			_battle->addReadyCard(_getCard, _hero);
			step++;
		}
		else if (step == 1)
		{
			_over = true;
		}
	}
}

//
// HandCard
//
HandCard* HandCard::create(HeroCard *hero, Card *card)
{
	HandCard* action = new HandCard();
    if (action && action->init(hero, card))
    {
        action->autorelease();
        return action;
    }

    CC_SAFE_DELETE(action);
    return nullptr;
}

bool HandCard::init(HeroCard *hero, Card *card)
{
	_handCard = card;
	_hero = hero;

	return true;
}

void HandCard::start()
{
	_start = true;
	_over = false;
	step = 0;
}

void HandCard::doing(float dt)
{
	if (!_over && _start)
	{
		if (step == 0)
		{
		}
		else
		{

		}
	}
}


//
// ObtainBodyguard
//
ObtainBodyguard* ObtainBodyguard::create(HeroCard *hero, CardVo *card)
{
	ObtainBodyguard* action = new ObtainBodyguard();
    if (action && action->init(hero, card))
    {
        action->autorelease();
        return action;
    }

    CC_SAFE_DELETE(action);
    return nullptr;
}

bool ObtainBodyguard::init(HeroCard *hero, CardVo *card)
{
	_hero = hero;
	_bodyguardCard = card;

	return true;
}

void ObtainBodyguard::start()
{
	_start = true;
	_over = false;
	if (_hero)
	{

	}
}

void ObtainBodyguard::doing(float dt)
{
	if (!_over && _start)
	{
		if (step == 0)
		{
			Card *card = Card::create(_bodyguardCard);
			if (_hero->getHandCards(card))
			{
				_battle->addHandCard(card, _hero);
				card->getNode()->setScale(0.2f);
				card->getNode()->runAction(EaseBackInOut::create(ScaleTo::create(0.5f, 1, 1)));
			}
			step++;
		}
		else if (step == 1)
		{
			_over = true;
		}
	}
}


//
// NormalAttackAction
//
NormalAttackAction* NormalAttackAction::create(HeroCard *hero, Card* _attackSrc, Card *_attackDesc)
{
	NormalAttackAction* action = new NormalAttackAction();
	if (action && action->init(hero, _attackSrc, _attackDesc))
    {
        action->autorelease();
        return action;
    }

    CC_SAFE_DELETE(action);
    return nullptr;
}

bool NormalAttackAction::init(HeroCard *hero, Card* attackSrc, Card *attackDesc)
{
	_hero = hero;
	_attackSrc = attackSrc;
	_attackDesc = attackDesc;

	return true;
}

void NormalAttackAction::start()
{
	_start = true;
	_over = false;
	step = 0;
}

void NormalAttackAction::doing(float dt)
{
	if(_start && _over)
	{
		if (step == 0)
		{
			Point p = _attackDesc->getPostition();
			Point oldP = _attackSrc->getPostition();
			auto *node = _attackSrc->getNode();
			auto *action = EaseOut::create(MoveTo::create(0.5f, p), 2);
			auto *action2 = EaseOut::create(ScaleTo::create(0.5f, 0.2f, 0.2f), 2);
			node->runAction(Sequence::create(
				action,
				DelayTime::create(2),
				action->reverse(),
				NULL
				));
			node->runAction(Sequence::create(
				action2,
				DelayTime::create(2),
				action2->reverse(),
				CallFunc::create(std::bind([&](){
						step++;
					})),
				NULL
				));
		}
		else if (step==2)
		{
			over();
		}
	}
}


//
//RoleActions
//
RoleActions::RoleActions(void):_actions(Array::createWithCapacity(10))
{
	_actions->retain();
}

RoleActions::~RoleActions(void)
{
	CC_SAFE_RELEASE(_actions);
}

void RoleActions::doing(float dt)
{
	if (_actions)
	{
		Object* obj;
		if (_actions->count() > 0)
		{
			obj = _actions->getObjectAtIndex(0);
			RoleAction* action = static_cast<RoleAction*>(obj);
			if (action)
			{
				if (action->isOver())
				{
					_actions->removeObject(action);
				}
				else
				{
					if (!action->isStarted()) action->start();
					action->doing(dt);
				}
			}
		}

	}
}

void RoleActions::addAction(RoleAction *action)
{
	_actions->addObject(action);
}