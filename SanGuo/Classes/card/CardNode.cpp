#include "CardNode.h"
#include "battle\BattleControl.h"
#include "battle\BattleRes.h"

CardNode::CardNode(void):
	_lifeLabel(nullptr),
	_faliLabel(nullptr),
	_attackLabel(nullptr),
	_armorLabel(nullptr),
	_iconSprite(nullptr),
	_selectSprite(nullptr),
	_touchEnable(true),
	_normalSprite(nullptr)
{
}

CardNode::~CardNode(void)
{
	_lifeLabel = nullptr;
	_faliLabel = nullptr;
	_attackLabel = nullptr;
	_armorLabel = nullptr;
	_iconSprite = nullptr;
	_selectSprite = nullptr;
}

bool CardNode::init()
{
	if (LayerRGBA::init())
	{
		setContentSize(Size(116, 143));
		setAnchorPoint(Point(0.5f, 0.5f));
		setZOrder(ORDER_CARD);

		return true;
	}
	return false;
}


void CardNode::setCardVo(const CardVo &cardVo)
{
	_card = cardVo;
}

void CardNode::setSelected(bool selected)
{
	if (_selectSprite)
	{
		_selectSprite->setVisible(selected);
	}
	if (_normalSprite)
	{
		_normalSprite->setVisible(!selected);
	}
}

bool CardNode::isTouchEnable()
{
	return _touchEnable;
}

void CardNode::setTouchEnable(bool enable)
{
	_touchEnable = enable;
}


Rect CardNode::rect() const
{
    return Rect( _position.x - _contentSize.width * _anchorPoint.x,
                      _position.y - _contentSize.height * _anchorPoint.y,
                      _contentSize.width, _contentSize.height);
}

void CardNode::updateLabels()
{
	if (&_card)
	{
		if (_card.type == CARD_HERO)
		{
			if (!_iconSprite)
			{
				setContentSize(Size(140, 140));
				char paths[25];
				sprintf(paths,"%s%d%s", "images/card/", _card.icon, ".png");
				_iconSprite = Sprite::create(paths);
				_iconSprite->setPosition(Point(this->_contentSize.width/2, this->_contentSize.height/2));
				_iconSprite->setScale(this->_contentSize.width/_iconSprite->getContentSize().width);
				this->addChild(_iconSprite, -2);

				char tmp[5];
				sprintf(tmp,"%d", _card.life);
		
				if (_lifeLabel)
				{
					_lifeLabel->setString(tmp);
				}
				else
				{
					_lifeLabel = LabelTTF::create(tmp, "Arial", 20);
					_lifeLabel->setAnchorPoint(Point(0.5f, 0.5f));
					_lifeLabel->setPosition(Point(20, 88));
					this->addChild(_lifeLabel);
				}
			}
		}
		else
		{
			float sc = 1;
			if (!_iconSprite)
			{
				char paths[25];
				sprintf(paths,"%s%d%s", "images/card/", _card.icon, ".png");
				_iconSprite = Sprite::create(paths);
				sc = this->_contentSize.width/_iconSprite->getContentSize().width;
				_iconSprite->setPosition(Point(this->_contentSize.width/2, this->_contentSize.height/2));
				_iconSprite->setScale(sc);
				this->addChild(_iconSprite, -2);
			}
			if (!_normalSprite)
			{
				_normalSprite = Sprite::create(s_pathBattleNormal_1_1);

				_normalSprite->setPosition(Point(this->_contentSize.width/2, this->_contentSize.height/2));
				_normalSprite->setScale(sc);
				
				this->addChild(_normalSprite, -1);
			}
			if (!_selectSprite)
			{
				_selectSprite = Sprite::create(s_pathBattleNormal_1_2);
				_selectSprite->setPosition(Point(this->_contentSize.width/2, this->_contentSize.height/2));
				_selectSprite->setScale(sc);
				_selectSprite->setVisible(false);
				
				this->addChild(_selectSprite, -1);
			}

			char tmp[5];
			sprintf(tmp,"%d", _card.life);
		
			if (_lifeLabel)
			{
				_lifeLabel->setString(tmp);
			}
			else
			{
				_lifeLabel = LabelTTF::create(tmp, "Arial", 20);
				_lifeLabel->setAnchorPoint(Point(0.5f, 0.5f));
				_lifeLabel->setPosition(Point(30, 130));
				this->addChild(_lifeLabel);
			}
		
			char tmp2[5];
			sprintf(tmp2,"%d", _card.attack);
			if (_attackLabel)
			{
				_attackLabel->setString(tmp2);
			}
			else
			{
				_attackLabel = LabelTTF::create(tmp2, "Arial", 20);
				_attackLabel->setAnchorPoint(Point(0.5f, 0.5f));
				_attackLabel->setPosition(Point(40, 18));
				this->addChild(_attackLabel);
			}

			char tmp3[5];
			sprintf(tmp3,"%d", _card.fali);
			if (_faliLabel)
			{
				_faliLabel->setString(tmp3);
			}
			else
			{
				_faliLabel = LabelTTF::create(tmp3, "Arial", 20);
				_faliLabel->setAnchorPoint(Point(0.5f, 0.5f));
				_faliLabel->setPosition(Point(75, 18));
				this->addChild(_faliLabel);
			}
		
			char tmp4[5];
			sprintf(tmp4,"%d", _card.armor);
			if (_armorLabel)
			{
				_armorLabel->setString(tmp4);
			}
			else
			{
				_armorLabel = LabelTTF::create(tmp4, "Arial", 20);
				_armorLabel->setAnchorPoint(Point(0.5f, 0.5f));
				_armorLabel->setPosition(Point(18, 16));
				//this->addChild(armorLabel);
			}
		}
	}
}
