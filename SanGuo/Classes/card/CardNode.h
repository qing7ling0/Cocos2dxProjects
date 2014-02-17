#pragma once
#include "cocos2d.h"
#include "QConfig.h"
#include "CardVo.h"

USING_NS_CC;

class CardNode : public cocos2d::LayerRGBA
{
public:
	CardNode(void);
	~CardNode(void);

	virtual bool init();

	virtual void updateLabels();

	virtual void setCardVo(const CardVo &cardVo);

	virtual void setSelected(bool selected);

	virtual bool isTouchEnable();

	virtual void setTouchEnable(bool enable);
	
	virtual Rect rect() const;

	CREATE_FUNC(CardNode);

protected:
	LabelTTF *_lifeLabel;
	LabelTTF *_faliLabel;
	LabelTTF *_attackLabel;
	LabelTTF *_armorLabel;

	Sprite *_iconSprite;
	Sprite *_selectSprite;
	Sprite *_normalSprite;

	CardVo _card;

	bool _touchEnable;
};

