#include "LauncherScene.h"
#include "login\GameLoadingScene.h"

USING_NS_CC;

Scene* LauncherLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LauncherLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LauncherLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	this->scheduleOnce(schedule_selector(LauncherLayer::next), 0.1f);

    return true;
}

void LauncherLayer::next(float dt)
{
	Director::getInstance()->replaceScene(GameLoading::createScene());
}