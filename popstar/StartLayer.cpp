#include "GameScene.h"
#include "GameLevel.h"
#include "StartLayer.h"

USING_NS_CC;


Scene* StartLayer::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	StartLayer *layer = StartLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StartLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float scale = visibleSize.height/1080;
    Sprite* popStarBg = Sprite::create("backgrounds/bg.jpg");
    popStarBg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    popStarBg->setScale(scale);
    this->addChild(popStarBg, 0);


	MenuItemImage* item = MenuItemImage::create("buttons/default/00010.png",
		"buttons/default/00010.png",
		"buttons/default/00010.png",
		this,
		menu_selector(StartLayer::onClickMenu));
    
//    MenuItemImage* item2 = MenuItemImage::create("buttons/default/quit.png",
//                                                "buttons/default/quit.png",
//                                                "buttons/default/quit.png",
//                                                this,
//                                                menu_selector(StartLayer::onClickMenu2));

	item->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
   // item2->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2-50));
    item->setScale(1);
   // item2->setScale(1);

	Menu* menu = Menu::create(item, NULL);
    menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void StartLayer::onClickMenu(Ref* obj)
{
	changeLayer( this, GameLevel::create() );
}
void StartLayer::onClickMenu2(Ref* obj)
{
   Director::getInstance()->end();
}
