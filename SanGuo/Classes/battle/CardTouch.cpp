#include "CardTouch.h"
#include "card\Card.h"
#include "BattleControl.h"
#include "RoleAction.h"


CardTouch::CardTouch(void):
	_heros(nullptr),
	_selectedCard(nullptr),
	_selectedHero(nullptr),
	_attackCard(nullptr),
	_handCard(nullptr),
	_state(CardTouch::State::self_round),
	_battle(BattleControl::getInstance()),
	_touchCard(nullptr)
{
}


CardTouch::~CardTouch(void)
{
	CC_SAFE_RELEASE(_heros);
}

	
bool CardTouch::init()
{
	if (Layer::init())
	{
		setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
		setTouchEnabled(true);
		BattleControl *battle = BattleControl::getInstance();
		_heros = battle->getHeros();
		if (_heros) _heros->retain();

		return true;
	}

	return false;
}
	
bool CardTouch::onTouchBegan(Touch *touch, Event *unused_event)
{
	_touchCard = touchedCard(touch);
	if (_touchCard)
	{
		selectedCard(_touchCard);
		return true;
	}

	return false;
}

void CardTouch::onTouchMoved(Touch *touch, Event *unused_event)
{
	Card* card = touchedCard(touch);
	if (card && _touchCard != card)
	{
		unselectedCard(_touchCard);
		selectedCard(card);
		_touchCard =card;
	}
} 

void CardTouch::onTouchEnded(Touch *touch, Event *unused_event)
{
	Card* card = touchedCard(touch);
	unselectedCard(_touchCard);
	if (card && _touchCard == card)
	{
		_selectedCard = _touchCard;
		touchCallback();
	}
} 

void CardTouch::onTouchCancelled(Touch *touch, Event *unused_event)
{
	unselectedCard(_touchCard);
}

void CardTouch::touchCallback()
{
	if (_selectedCard)
	{
		bool isHero = _touchCard->getCardVo()->type == CARD_HERO;
		if (isHero)
		{
		}
		else
		{
			if(_state == State::self_round)
			{
				HeroCard *hero = _touchCard->getHero();
				if (_selectedCard->getState() == Card::State::reading && !hero->getCardVo()->isEnemy)
				{
					handCard(hero, _selectedCard);
					_selectedCard = nullptr;
				}
				else if (_selectedCard->getState() == Card::State::handed)
				{
					if (!hero->getCardVo()->isEnemy)
					{
						bool _hand = false;
						if (_handCard)
						{
							if (_handCard != _selectedCard)
							{
								_handCard->getNode()->setSelected(false);
								_hand = true;
							}
						}
						else
						{
							_hand = true;
						}
						if (_hand)
						{
							_handCard = _selectedCard;
							_handCard->getNode()->setSelected(true);
						}
					}
					else
					{
						if (_handCard)
						{
							_attackCard = _selectedCard;
							_battle->getActions()->addAction(NormalAttackAction::create(hero, _handCard, _attackCard));
						}
					}
				}
			}
		}
	}
}

void CardTouch::selectedCard(Card* card)
{
	if (card && card->getNode())
	{
		card->getNode()->stopActionByTag(11);
		ActionInterval *action = EaseElasticInOut::create(ScaleTo::create(0.2f, 1.2f, 1.2f));
		action->setTag(10);
		card->getNode()->runAction(action);
	}
}

void CardTouch::unselectedCard(Card* card)
{
	if (card && card->getNode())
	{
		card->getNode()->stopActionByTag(10);
		ActionInterval *action = EaseElasticInOut::create(ScaleTo::create(0.2f, 1, 1));
		action->setTag(11);
		card->getNode()->runAction(action);
	}
}


Card* CardTouch::touchedCard(Touch *touch)
{
	Object* obj;
	CCARRAY_FOREACH(_heros, obj)
	{
		HeroCard* he = static_cast<HeroCard*>(obj);
		if (he)
		{
			if (he->onTouched(touch))
			{
				return he;
			}
			else
			{
				Object* obj2;
				CCARRAY_FOREACH_REVERSE(he->getReadyCards(), obj2)
				{
					Card* card = static_cast<Card*>(obj2);
					if (card && card->onTouched(touch))
					{
						return card;
					}
				}

				obj2 = nullptr;
				CCARRAY_FOREACH_REVERSE(he->getHandCards(), obj2)
				{
					Card* card = static_cast<Card*>(obj2);
					if (card && card->onTouched(touch))
					{
						return card;
					}
				}
			}
		}
	}

	return nullptr;
}


void CardTouch::handCard(HeroCard* hero, Card* card)
{
	card->getNode()->runAction(Sequence::create(
		ScaleTo::create(0.3f, 0, 0),
		CallFunc::create(std::bind([&, hero, card](){
			CardVo* cv = card->getCardVo();
			cv->retain();
			RoleAction *action = ObtainBodyguard::create(hero, cv);

			hero->removeCard(card, Card::State::reading);
			BattleControl::getInstance()->updateCards(!hero->getCardVo()->isEnemy, true);

			BattleControl::getInstance()->getActions()->addAction(action);
		})),
		NULL
	));
}

void CardTouch::setState(CardTouch::State state)
{
	_state = state;
}