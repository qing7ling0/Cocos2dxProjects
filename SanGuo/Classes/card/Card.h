#pragma once
#include "cocos2d.h"
#include "QConfig.h"
#include "CardNode.h"
#include "CardVo.h"

class HeroCard;

class Card : public Object
{
public:
    enum class State
    {
        normal,

		// 在准备区
        reading,

		// 已经使用了
        handed,

    };


	Card(void);

	~Card(void);

	virtual void dispose();
	
	virtual void onDamage(int value);

	virtual void onHand();

	virtual void onEventHandle(int key);
	
	virtual void onTiggerSkill(int key);
	
	virtual void onAttack(Card card);
	
	virtual void checkCanHand(bool tip);
	
	/** 是否可以被攻击 */
	virtual bool checkCanHitted(bool tip);
	
	/** 是否可以攻击 */
	virtual bool checkCanAttack(bool tip);
	
	/** 是否可以收到伤害 */
	virtual bool checkCanDamage(bool tip);
	
	virtual void checkDeath();

	virtual void addEffect();

	virtual void updateLable();

	virtual void selected();

	virtual void unselected();

	virtual void doing(float dt);
	
	virtual bool isEnableTouch();

	virtual bool checkCanSelected();
	
	virtual CardNode* getNode();

	virtual CardVo* getCardVo();

	virtual void setHero(HeroCard *hero);

	virtual HeroCard* getHero();

	bool onTouched(Touch *touch);

	static Card* create(CardVo *cardVo);

	void setPosition(float x, float y);

	Point getPostition();

	void setState(Card::State state);
	
	Card::State getState();

protected:

	virtual void createCardNode();

	virtual void init(CardVo *cardVo);

protected:
	CardNode *_cardNode;
	CardVo *_cardVo;

	Point _position;

	HeroCard *_hero;

	State _state;
};


class Soldier : public Card
{
public:
	Soldier();
	~Soldier();
};

class BattleControl;

class HeroCard : public Card
{
public:
	HeroCard();
	~HeroCard();

	virtual void init(CardVo *cardVo) override;

	static HeroCard* create(CardVo *cardVo);

	CardVo* chooseOneCard();

	Array* getAllCardVos();
	
	Array* getReadyCards();

	Array* getHandCards();

	bool addAllCardVos(CardVo *card);

	bool addReadyCards(Card *card);
	
	bool getHandCards(Card *card);

	void removeCard(Card *card, Card::State state);

protected:
	/** 玩家所携带的牌 */
	Array *_allCardVos;
	
	/** 玩家可以被使用的牌 */
	Array *_readyCards;

	/** 玩家已出的牌 */
	Array *_handCards;

	BattleControl *_battle;


};
