#pragma once
#include <string>
#include "QConfig.h"

/** ¿¨ÅÆ¤Î¼¼ÄÜ */
static const short CARD_SKILL = 1;

/** ¿¨ÅÆ¤ÎÊ¿±ø */
static const short CARD_SOLDIER = 2;

/** ¿¨ÅÆ¤ÎÓ¢ÐÛ */
static const short CARD_HERO = 3;

/** ¿¨ÅÆ¤ÎÎäÆ÷ */
static const short CARD_WEAPON = 4;

class CardVo : public Object
{
public:
	CardVo(void);
	~CardVo(void);
	std::string name;
	int attack;
	int life;
	int maxLife;
	int icon;
	short type;
	bool isEnemy;
	int fali;
	int maxFali;
	int armor;
	int id;
};