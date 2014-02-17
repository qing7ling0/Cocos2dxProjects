#include "extensions\cocos-ext.h"
#include "cocostudio\CocoStudio.h"
#include "gui\CocosGUI.h"

#include "test\TestScene.h"
#include "core\RecvThread.h"
#include "core\SendThread.h"
#include "core\ODSocket.h"
#include "core\SocketConn.h"
#include "core\Debug.h"
#include "core\Packet.h"
#include "QConfig.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace gui;

Scene* TestScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(TestScene::menuCloseCallback, this));
    
	closeItem->setPosition(Point(D_display.x + D_display.w - closeItem->getContentSize().width/2 ,
                                D_display.y + closeItem->getContentSize().height/2));

    auto webItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(TestScene::menuNetCallback, this));
    
	webItem->setPosition(Point(D_display.x + D_display.w/2 - closeItem->getContentSize().width/2 ,
                                D_display.y + closeItem->getContentSize().height/2));

    auto threadItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(TestScene::menuThreadCallback, this));
    
	threadItem->setPosition(Point(D_display.x + D_display.w/4 - closeItem->getContentSize().width/2 ,
                                D_display.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, webItem, threadItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(D_display.x + D_display.w/2,
                            D_display.y + D_display.h - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
    auto web = LabelTTF::create("websocket", "Arial", 40);
    
    // position the label on the center of the screen
    web->setPosition(Point(D_display.x + D_display.w/2,
                            D_display.y + D_display.h/2 - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(web, 1);

    // add "TestScene" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(D_display.w/2 + D_display.x, D_display.h/2 + D_display.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    //ui action
	//auto actionObject = ActionManagerEx::shareManager()->playActionByName("startMenu_1.json","Animation1");
 //   return pNode;

	s_thread = nullptr;

	createUIEditerTest();

    return true;
}

void TestScene::createSceneEditerTest()
{
	Node *pNode = SceneReader::getInstance()->createNodeWithSceneFile("FightScene.json");
	if (pNode == nullptr)
	{
		return;
	}

	this->addChild(pNode);
	ComRender *hero = (ComRender *) (pNode->getComponent("hero"));
	ComRender *heroAnimi= (ComRender *)(pNode->getChildByTag(10005)->getComponent("CCArmature"));
	Armature * animi = (Armature*) hero->getNode();
	ArmatureAnimation *animi2 = animi->getAnimation();
	animi2->play("attack", -1, -1, 1);
}

void TestScene::createUIEditerTest()
{
	UILayer* ul =UILayer::create(); 
    //将UILayer层加入到当前的场景 
    this->addChild(ul); 
	UILayout *m_pWidget = dynamic_cast<UILayout*>(cocostudio::GUIReader::shareReader()->widgetFromJsonFile("ui/DemoShop/DemoShop.ExportJson"));
	ul->addWidget(m_pWidget);

	/* 动画播放
	ActionObject *action = ActionManagerEx::shareManager()->getActionByName("DemoShop.ExportJson", "Animation0");
	action->setLoop(true);
	action->play();*/
	UIScrollView *leftS = dynamic_cast<UIScrollView*> (ul->getWidgetByName("shop_ScrollView"));
	//leftS->addEventListenerScrollView(this, SEL_ScrollViewEvent(&TestScene::uiBtnCallback));
	//leftS->addTouchEventListener(this, SEL_TouchEvent(&TestScene::uiBtnCallback));
	UIButton *uBtn = dynamic_cast<UIButton*> (ul->getWidgetByName("Button_225"));
	uBtn->addTouchEventListener(this, SEL_TouchEvent(&TestScene::uiBtnCallback));
	//uBtn->getRenderer()->call
}

void TestScene::uiBtnCallback(Object* pSender, int eventType)
{
	UIWidget *node = dynamic_cast<UIWidget*>(pSender);
	log(" btn %s select buy eventType=%d", node->getName(), eventType);

}

void TestScene::menuCloseCallback(Object* pSender)
{
	SocketConn::dispose();
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void TestScene::menuThreadCallback(Object* pSender)
{
	if (s_thread)
	{
		if (threadStart)
		{
			s_thread->stop();
			threadStart = false;
		}
		else
		{
			s_thread->start();
			threadStart = true;
		}
	}
	else
	{
		s_thread = new SocketThread(NULL);
		s_thread->start();
		threadStart = true;
	}
}

void TestScene::menuNetCallback(Object* pSender)
{
	SocketConn::getInstance()->openConn("61.147.117.149", 6666);
	log("net work contact!");

	Packet* p = new Packet();
	p->id = 20;
	p->setType(0x00f2);
	p->enter(-20);
	string str("HH");
	p->enter(str);
	p->enter((short)-3000);
	p->enter((q_byte)-21);
	p->enter(true);
	p->setPointer(0);

	Packet* p2 = new Packet();
	p2->setType(0x00f2);
	SocketConn::getInstance()->send(p2);
	
	int temp = p->decodeInt();
	string st = p->decodeString();
	short sh = p->decodeShort();
	q_byte by = p->decodeByte();
	bool bl = p->decodeBoolean();
	
	log("id=%d, temp=%d, str=%s", p->id, temp, st.c_str());
	
	log("short=%d, byte=%d", sh, by);

	if (bl)
	{
		log("boolean=true");
	}
	else
	{
		log("boolean=false");
	}
}