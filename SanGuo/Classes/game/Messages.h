#pragma once
#include "QConfig.h"


class MessageItem : public Object
{
public:
	MessageItem(void);
	~MessageItem(void);

	void init(char *msg, int time);

	LayerRGBA* getNode();

	bool timeOver();

	void update(float dt);

	static MessageItem* create(char *msg, int time=2);

protected:
	char *_msg;

	short _time;

	float _curTime;

	LayerRGBA *_node;
};

class MessagesService : public Object
{
public:
	MessagesService(void);
	~MessagesService(void);

	Array* getMessages();

	void addMessage(MessageItem *item);

	static MessagesService* getInstance();

protected:
	Array *_messages;
};

class MessagesLayer : public Layer
{
public:
	MessagesLayer(void);
	~MessagesLayer(void);

	bool init() override;

	void doing(float dt);

	CREATE_FUNC(MessagesLayer);

protected:
	Array *_messages;
	MessageItem *_curShowItem;
};