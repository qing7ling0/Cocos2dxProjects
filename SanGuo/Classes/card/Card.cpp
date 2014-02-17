#include "Card.h"
#include "battle\BattleControl.h"

Card::Card(void):
	_cardNode(nullptr),
	_hero(nullptr),
	_position(Point(0,0)),
	_state(State::normal),
	_cardVo(nullptr)
{
}


Card::~Card(void)
{
	if(_cardNode) _cardNode->removeFromParentAndCleanup(false);
	CC_SAFE_RELEASE(_cardNode);
	CC_SAFE_RELEASE(_cardVo);
}

void Card::dispose()
{

}

Card* Card::create(CardVo *cardVo)
{
	Card *card = new Card();
	card->init(cardVo);
	card->autorelease();

	return card;
}

void Card::init(CardVo *cardVo)
{
	_cardVo = cardVo;
	_cardVo->retain();

	createCardNode();
}

void Card::createCardNode()
{
	if (nullptr == _cardNode)
	{
		_cardNode = CardNode::create();
		_cardNode->retain();
		_cardNode->setCardVo(*_cardVo);
		_cardNode->updateLabels();
		_cardNode->setSelected(false);
	}
}

void Card::setState(Card::State state)
{
	_state = state;
}
	
Card::State Card::getState()
{
	return _state;
}

void Card::onDamage(int value)
{
}

void Card::onHand()
{
}

void Card::onEventHandle(int key)
{
}
void Card::onTiggerSkill(int key)
{
}
void Card::onAttack(Card card)
{
}
void Card::checkCanHand(bool tip)
{
}
	
/** 是否可以被攻击 */
bool Card::checkCanHitted(bool tip)
{
	return false;
}
	
/** 是否可以攻击 */
bool Card::checkCanAttack(bool tip)
{
	return false;
}
	
	/** 是否可以收到伤害 */
bool Card::checkCanDamage(bool tip)
{
	return false;
}
	
void Card::checkDeath()
{
}
void Card::addEffect()
{
}
void Card::updateLable()
{
}
void Card::setPosition(float x, float y)
{
	_position.x = x;
	_position.y = y;
}

Point Card::getPostition()
{
	return Point(_position.x, _position.y);
}

void Card::selected()
{
	if (_cardNode)
	{
		_cardNode->setScale(1.2f, 1.2f);
	}
}
void Card::unselected()
{
	if (_cardNode)
	{
		_cardNode->setScale(1, 1);
	}
}
void Card::doing(float dt)
{
}
bool Card::isEnableTouch()
{
	return false;
}
bool Card::checkCanSelected()
{
	return false;
}

CardNode* Card::getNode()
{
	return _cardNode;
}

CardVo* Card::getCardVo()
{
	return _cardVo;
}

void Card::setHero(HeroCard *hero)
{
	_hero = hero;
}

HeroCard* Card::getHero()
{
	return _hero;
}

bool Card::onTouched(Touch *touch)
{
	if (_cardNode && _cardNode->isVisible() && _cardNode->isTouchEnable())
	{
		Point local = _cardNode->convertToNodeSpace(touch->getLocation());
		Rect r = _cardNode->rect();
		r.origin = Point::ZERO;

		return r.containsPoint(local);
	}

	return false;
}


//
// HeroCard
//
HeroCard::HeroCard():
	_allCardVos(Array::createWithCapacity(5)),
	_readyCards(Array::createWithCapacity(10)),
	_handCards(Array::createWithCapacity(10)),
	_battle(BattleControl::getInstance())
{
    _allCardVos->retain();
    _readyCards->retain();
    _handCards->retain();
	_battle->retain();
}

HeroCard::~HeroCard()
{
    CC_SAFE_RELEASE(_allCardVos);
    CC_SAFE_RELEASE(_readyCards);
    CC_SAFE_RELEASE(_handCards);
    CC_SAFE_RELEASE(_battle);
}

HeroCard* HeroCard::create(CardVo *cardVo)
{
	HeroCard *card = new HeroCard();
	card->init(cardVo);
	card->autorelease();

	return card;
}

void HeroCard::init(CardVo *cardVo)
{
	Card::init(cardVo);
	for(int i=0; i<20; i++)
	{
		CardVo *temp = new CardVo();

		char tmp[15];
		sprintf(tmp,"%s%d", cardVo->name.c_str(), i);

		temp->name = tmp;
		temp->life = i;
		temp->maxLife = i;
		temp->attack = i/4 + 2;
		temp->fali = temp->attack;
		temp->maxFali = temp->fali;
		temp->isEnemy = cardVo->isEnemy;
		temp->type = CARD_SOLDIER;
		temp->icon = cardVo->isEnemy?i : (30+i);
		temp->id = temp->icon;

		_allCardVos->addObject(temp);
		temp->release();
	}
}

CardVo* HeroCard::chooseOneCard()
{
	int count = _allCardVos->count();
	if (count < 1) return nullptr;


	int index = RANDOM_0_TO(count);
	
	Object *obj = _allCardVos->getObjectAtIndex(index);
	if (obj)
	{
		_allCardVos->removeObjectAtIndex(index, false);
		CardVo* vo = static_cast<CardVo*>(obj);
		return vo;
	}

	return nullptr;
}

Array* HeroCard::getAllCardVos()
{
	return _allCardVos;
}
	
Array* HeroCard::getReadyCards()
{
	return _readyCards;
}

Array* HeroCard::getHandCards()
{
	return _handCards;
}
bool HeroCard::addAllCardVos(CardVo *card)
{
	if (_allCardVos)
	{
		_allCardVos->addObject(card);
		return true;
	}
	return false;
}

bool HeroCard::addReadyCards(Card *card)
{
	if (_readyCards && card)
	{
		card->setHero(this);
		_readyCards->addObject(card);
		card->setState(Card::State::reading);
		return true;
	}
	return false;
}
void HeroCard::removeCard(Card *card, Card::State state)
{
	if (state == Card::State::reading)
	{
		_readyCards->removeObject(card);
	}
	else if (state == Card::State::handed)
	{
		_handCards->removeObject(card);
	}
}
	
bool HeroCard::getHandCards(Card *card)
{
	if (_handCards)
	{
		_handCards->addObject(card);
		card->setHero(this);
		card->setState(Card::State::handed);
		return true;
	}
	return false;
}