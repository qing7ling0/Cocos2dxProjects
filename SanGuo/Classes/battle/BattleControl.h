#pragma once
#include "QConfig.h"
#include "CCSHead.h"
#include "BattleDatas.h"
#include "BattleRes.h"


extern const int ORDER_CARD;
extern const int ORDER_MENU;


class BattleCommand;
class RoleAction;
class RoleActions;
class Card;
class HeroCard;
class CardNode;
class CardVo;
class CardTouch;

class BattleControl : public Object
{
public:
	BattleControl(void);
	~BattleControl(void);

	static BattleControl* getInstance();

	void init();

	void load();

	void loadRes();

	void update(float dt);

	void dispose();

	bool isSelfRound();
	
	UILayout* getBattleUI();

	Node* getBattleLayer();

	void setBattleUI(UILayout *battleui);

	BattleCommand* getCommand();

	void setCommand(BattleCommand *command);

	RoleActions* getActions();

	BattleRound getBattleRound();

	Array* getHeros();

	CardTouch* getCardTouch();

	void addReadyCard(Card* card, HeroCard *hero);

	void addHandCard(Card* card, HeroCard *hero);

	void nextRound();

	void updateCards(bool self, bool ready);

protected:

	bool loaded;

	UILayout *_battleUI;

	BattleRound _round;

	Array *_heros;

	Scheduler *_scheduler;

	int step;

	BattleCommand *_curCommand;

	RoleActions *_actions;

	CardTouch *_cardTouch;
};

