#include "BattleControl.h"
#include "card\Card.h"
#include "BattleCommand.h"
#include "RoleAction.h"
#include "CardTouch.h"

const int ORDER_CARD = 100;
const int ORDER_MENU = 200;

static BattleControl *instance = nullptr;

static int MAX_STEP = 100;

static char hero_self_ui[] = "hero_self";
static char hero_enemy_ui[] = "hero_enemy";

static char cards_self_ui[] = "self_cards";
static char cards_enemy_ui[] = "enemy_cards";

BattleControl::BattleControl(void):
	_heros(NULL),
	_battleUI(nullptr),
	_scheduler(nullptr),
	_curCommand(nullptr),
	_actions(nullptr),
	_cardTouch(nullptr),
	loaded(false),
	step(0)
{
	//this->set
}

BattleControl::~BattleControl(void)
{
	_battleUI = nullptr;
    CC_SAFE_RELEASE(_heros);
    CC_SAFE_RELEASE(_scheduler);
    CC_SAFE_RELEASE(_curCommand);
    CC_SAFE_RELEASE(_actions);
    CC_SAFE_RELEASE(_cardTouch);

	instance = nullptr;
}

BattleControl* BattleControl::getInstance()
{
	if (nullptr == instance)
	{
		instance = new BattleControl();
		instance->init();
	}

	return instance;
}

void BattleControl::dispose()
{
	if (_scheduler)
	{
		_scheduler->unscheduleAllForTarget(this);
	}

	release();
}

void BattleControl::init()
{
	_heros = Array::createWithCapacity(2);
    _heros->retain();

	_scheduler = Director::getInstance()->getScheduler();
	_scheduler->retain();
	_scheduler->scheduleUpdateForTarget(this, 0, false);

	_actions = new RoleActions();

	_round.roundIndex = 0;
	_round.currentRoundHeroID = 491;
}


UILayout* BattleControl::getBattleUI()
{
	return _battleUI;
}

Node* BattleControl::getBattleLayer()
{
	return _battleUI->getRenderer();
}

void BattleControl::setBattleUI(UILayout *battleui)
{
	_battleUI = battleui;
}

BattleCommand* BattleControl::getCommand()
{
	return _curCommand;
}

CardTouch* BattleControl::getCardTouch()
{
	return _cardTouch;
}

void BattleControl::setCommand(BattleCommand *command)
{
	if (_curCommand)
	{
		_curCommand->dispose();
		CC_SAFE_RELEASE_NULL(_curCommand);
	}

	_curCommand = command;
	if (_curCommand)
	{
		_curCommand->init();
	}
}

RoleActions* BattleControl::getActions()
{
	return _actions;
}

BattleRound BattleControl::getBattleRound()
{
	return _round;
}

Array* BattleControl::getHeros()
{
	return _heros;
}

void BattleControl::update(float dt)
{
	if (!loaded) return;
	if (step <= MAX_STEP)
	{
		loadRes();
	}
	else
	{
		_curCommand->doing(dt);
		_actions->doing(dt);
	}
}

void BattleControl::load()
{
	loaded = true;

	_cardTouch = CardTouch::create();
	_cardTouch->setContentSize(_battleUI->getContentSize());
	_battleUI->addRenderer(_cardTouch, -1);
}

void BattleControl::loadRes()
{
	if (step == 0)
	{
		CardVo *card = new CardVo();
		card->name = "Íæ¼Ò";
		card->life = 20;
		card->maxLife = 20;
		card->attack = 2;
		card->fali = card->attack;
		card->maxFali = card->fali;
		card->isEnemy = false;
		card->type = CARD_HERO;
		card->icon = 9001;
		card->id = card->icon;
		HeroCard *hero = HeroCard::create(card);
		_heros->addObject(hero);
		card->release();
	
		card = new CardVo();
		card->name = "µÐÈË";
		card->life = 20;
		card->maxLife = 20;
		card->attack = 2;
		card->fali = card->attack;
		card->maxFali = card->fali;
		card->isEnemy = true;
		card->type = CARD_HERO;
		card->icon = 9002;
		card->id = card->icon;
		HeroCard *enemy = HeroCard::create(card);
		_heros->addObject(enemy);
		card->release();

		step++;
	}
	else if (step == 1)
	{
		if (_heros)
		{
			Object* hero;
			CCARRAY_FOREACH(_heros, hero)
			{
				HeroCard* heroCard = static_cast<HeroCard*>(hero);
				if (heroCard)
				{
					CardNode *node = heroCard->getNode();
					char *ui_name = (heroCard->getCardVo()->isEnemy) ? hero_enemy_ui : hero_self_ui;
					UILayout *layer = dynamic_cast<UILayout*>(_battleUI->getChildByName(ui_name));

					node->ignoreAnchorPointForPosition(false);
					node->setAnchorPoint(Point(0.5, 0.5));
					node->setPosition(Point(layer->getPosition().x+layer->getContentSize().width/2, layer->getPosition().y+layer->getContentSize().height/2+20));

					_battleUI->addRenderer(node, node->getZOrder());
				}
			}
		}
		step++;
	}
	else if (step == 2)
	{
		step = MAX_STEP+1;
	}
}

bool BattleControl::isSelfRound()
{
	if (_heros)
	{
        Object* hero;
		CCARRAY_FOREACH(_heros, hero)
        {
            HeroCard* heroCard = static_cast<HeroCard*>(hero);
            if (heroCard)
            {
				return heroCard->getCardVo()->id == _round.currentRoundHeroID;
            }
        }
	}

	return false;
}

void BattleControl::nextRound()
{
	_round.roundIndex++;
	_round.currentRoundHeroID = 9001;


	int index = _round.roundIndex % 2;
	HeroCard *hero = static_cast<HeroCard*> (_heros->getObjectAtIndex(index));
	_actions->addAction(ChooseCard::create(hero));
}

void BattleControl::addReadyCard(Card* card, HeroCard *hero)
{
	if (card && hero)
	{
		_battleUI->addRenderer(card->getNode(), ORDER_CARD);
		updateCards(!hero->getCardVo()->isEnemy, true);
	}
	else
	{
		log("addReadyCard card nil or hero nil");
	}
}


void BattleControl::addHandCard(Card* card, HeroCard *hero)
{
	if (card && hero)
	{
		_battleUI->addRenderer(card->getNode(), ORDER_CARD);
		updateCards(!hero->getCardVo()->isEnemy, false);
	}
	else
	{
		log("addHandCard card nil or hero nil");
	}
}

void BattleControl::updateCards(bool self, bool ready)
{
	HeroCard *hero = nullptr;
	
    Object* obj;
	CCARRAY_FOREACH(_heros, obj)
    {
        HeroCard* heroCard = static_cast<HeroCard*>(obj);
		if (heroCard->getCardVo()->isEnemy != self)
		{
			hero = heroCard;
			break;
		}
    }

	Point p = Point();
	float cw=0, sx=0;
	Array *cards = nullptr;
	if (ready)
	{
		char *ui_name = self ? cards_self_ui : cards_enemy_ui;
		UILayout *layer = dynamic_cast<UILayout*>(_battleUI->getChildByName(ui_name));
		
		int count = hero->getReadyCards()->count();
		p.x = layer->getPosition().x;
		p.y = layer->getPosition().y;
		cw = layer->getContentSize().width / count;
		sx = p.x;

		cards = hero->getReadyCards();
	}
	else
	{
		int count = hero->getHandCards()->count();
		float cy = _battleUI->getContentSize().height/2;
		float cx = _battleUI->getContentSize().width/2-88;
		p.y = self ?  (cy-115) : (cy+75);
		cw = 80;
		sx = cx - count*cw / 2;

		cards = hero->getHandCards();
	}

	int i = 0;
	obj = nullptr;
	CCARRAY_FOREACH(cards, obj)
    {
        Card* card = static_cast<Card*>(obj);
        if (card)
        {
			p.x = sx + i*cw + cw/2; 
			card->setPosition(p.x, p.y);
			card->getNode()->setPosition(p);
			i++;
        }
    }
}
