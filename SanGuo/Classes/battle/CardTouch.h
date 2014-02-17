#pragma once
#include "QConfig.h"
#include "BattleControl.h"

class CardTouch : public Layer
{
public:
    enum class State
    {
		self_round,

        other_round,

    };

	CardTouch(void);
	~CardTouch(void);

	bool init();

	bool onTouchBegan(Touch *touch, Event *unused_event); 
    void onTouchMoved(Touch *touch, Event *unused_event); 
    void onTouchEnded(Touch *touch, Event *unused_event); 
    void onTouchCancelled(Touch *touch, Event *unused_event);

	Card* touchedCard(Touch *touch);

	void touchCallback();

	void handCard(HeroCard* hero, Card *card);

	void selectedCard(Card* card);

	void unselectedCard(Card* card);

	void setState(CardTouch::State state);

	CREATE_FUNC(CardTouch);

protected:
	Array *_heros;

	Card *_touchCard;

	HeroCard* _selectedHero;

	Card *_selectedCard;

	State _state;

	Card *_handCard;

	Card *_attackCard;

	BattleControl *_battle;
};

