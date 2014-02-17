#pragma once

#include "cocos2d.h"
#include "core/SocketThread.h"

class TestScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Object* pSender);

    void menuThreadCallback(Object* pSender);
	
	void menuNetCallback(Object* pSender);
	
	void createSceneEditerTest();
	void createUIEditerTest();
	
	void uiBtnCallback(Object* pSender, int eventType);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TestScene);

	bool threadStart;
	SocketThread* s_thread;
};

