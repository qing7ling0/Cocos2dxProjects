#include "Messages.h"

MessageItem::MessageItem(void):_node(nullptr),
	_time(0),
	_curTime(0),
	_msg("")
{
}

MessageItem::~MessageItem(void)
{
	CC_SAFE_RELEASE(_node);
}

void MessageItem::init(char *msg, int time)
{
	_msg = msg;
	_time = time;
	_curTime = 0;

	Size size = Size(400, 0);

	_node = LayerRGBA::create();
	_node->retain();

	LayerColor *bg = LayerColor::create(Color4B(0x0, 0x0, 0x0, 0xaa), 20, 20);
    bg->setAnchorPoint(Point(0.5f, 0.5f));
	bg->ignoreAnchorPointForPosition(false);
	_node->addChild(bg);
	
	//Marker Felt
	auto label1 = LabelTTF::create(_msg, "Marker Felt", 20, size, 
		TextHAlignment::CENTER, 
		TextVAlignment::TOP);
	size.height = label1->getContentSize().height + 20;
	bg->setContentSize(size);

    label1->setAnchorPoint(Point(0.5f, 0.5f));
	_node->addChild(label1);

}

bool MessageItem::timeOver(void)
{
	return _curTime >= _time;
}

LayerRGBA* MessageItem::getNode()
{
	return _node;
}

void MessageItem::update(float dt)
{
	_curTime += dt;
}

MessageItem* MessageItem::create(char *msg, int time/*time=2*/)
{
	MessageItem *mi = new MessageItem();
	mi->autorelease();
	mi->init(msg, time);

	return mi;
}

static MessagesService *_instance = nullptr;

MessagesService::MessagesService(void):_messages(nullptr)
{
	_messages = Array::createWithCapacity(10);
	_messages->retain();
}

MessagesService::~MessagesService(void)
{
	CC_SAFE_RELEASE(_messages);
}

void MessagesService::addMessage(MessageItem *item)
{
	if (_messages)
	{
		_messages->addObject(item);
	}
}

Array* MessagesService::getMessages()
{
	return _messages;
}


MessagesService* MessagesService::getInstance()
{
	if (!_instance)
	{
		_instance = new MessagesService();
	}
	return _instance;
}

MessagesLayer::MessagesLayer(void):_messages(nullptr),
	_curShowItem(nullptr)
{
}

MessagesLayer::~MessagesLayer(void)
{
	CC_SAFE_RELEASE(_curShowItem);
}

bool MessagesLayer::init()
{
	if (Layer::init())
	{
		_messages = MessagesService::getInstance()->getMessages();

		schedule(schedule_selector(MessagesLayer::doing));

		setZOrder(NodeOrder::messagebox);

		return true;
	}

	return false;
}

void MessagesLayer::doing(float dt)
{
	if (!_messages) return;

	int count = _messages->count();


	if (_curShowItem)
	{
		_curShowItem->update(dt);
	}

	if (_curShowItem==nullptr || _curShowItem->timeOver())
	{
		LayerRGBA* _node = nullptr;
		if (_curShowItem)
		{
			_node = _curShowItem->getNode();
			_node->stopAllActions();
			auto action = Sequence::create(
				MoveBy::create(0.3f, Point(0, 100)),
				CallFuncN::create(
					std::bind([&](Node *node){ node->removeFromParent();}, std::placeholders::_1)
					),
				NULL
				);
			_node->runAction(action);
			auto action2 = FadeOut::create(0.3f);
			_node->runAction(action2);
			_node = nullptr;
			CC_SAFE_RELEASE_NULL(_curShowItem);
		}

		if (count > 0)
		{
			Object *obj = _messages->getLastObject();
			if (obj)
			{
				MessageItem *item = static_cast<MessageItem*>(obj);
				if (item)
				{
				
					_node = item->getNode();
					_node->setOpacity(0);
					_node->setPosition(Point(_contentSize.width/2, _contentSize.height/2-40));
					addChild(_node);

					auto action = MoveBy::create(0.2f, Point(0, 40));
					_node->runAction(action);
					auto action2 = FadeIn::create(0.2f);
					_node->runAction(action2);
					_node = nullptr;

					_curShowItem = item;
					_curShowItem->retain();
					_messages->removeObject(obj);
				}
			}
		}
	}

}
