#pragma once
#include "QConfig.h"
#include "card\Card.h"
#include "BattleControl.h"

class RoleAction : public Object
{
public:
	RoleAction(void);

	~RoleAction(void);

	virtual void start();

	virtual void doing(float dt);

	virtual bool isOver();

	virtual void over();

	virtual bool isStarted();

	virtual HeroCard* getHero();

protected:
	HeroCard *_hero;
	bool _over;
	int step;
	bool _start;
	BattleControl *_battle;
};

//
// ChooseCard
//
class ChooseCard : public RoleAction
{
public:
	static ChooseCard* create(HeroCard *hero);

	bool init(HeroCard *hero);

	void start() override;

	void doing(float dt) override;
};

//
// GetOneCard
//
class GetOneCard : public RoleAction
{
public:
	static GetOneCard* create(HeroCard *hero, CardVo* cardVo);

	bool init(HeroCard *hero, CardVo* cardVo);

	void start() override;

	void doing(float dt) override;
protected:
	CardVo *_getCardVo;
	Card *_getCard;
};


//
// HandCard
//
class HandCard : public RoleAction
{
public:
	static HandCard* create(HeroCard *hero, Card *card);

	bool init(HeroCard *hero, Card *card);

	void start() override;

	void doing(float dt) override;

protected:
	Card *_handCard;
};


//
// ObtainBodyguard
//
class ObtainBodyguard : public RoleAction
{
public:
	static ObtainBodyguard* create(HeroCard *hero, CardVo *card);

	bool init(HeroCard *hero, CardVo *card);

	void start() override;

	void doing(float dt) override;

protected:
	CardVo *_bodyguardCard;
};


//
// NormalAttackAction
//
class NormalAttackAction : public RoleAction
{
public:
	static NormalAttackAction* create(HeroCard *hero, Card* _attackSrc, Card *_attackDesc);

	bool init(HeroCard *hero, Card* _attackSrc, Card *_attackDesc);

	void start() override;

	void doing(float dt) override;

protected:
	Card *_attackSrc;
	Card *_attackDesc;
};


//
//RoleActions
//
class RoleActions : public Object
{
public:
	RoleActions(void);

	~RoleActions(void);

	void doing(float dt);

	void addAction(RoleAction *action);

protected:
	/** 玩家所有的动作 */
	Array *_actions;
};