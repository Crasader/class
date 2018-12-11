#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	count = 0;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Layer *layer_login = Layer::create();
	layer_login->setPosition(Point(visibleSize / 2));
	layer_login->setAnchorPoint(Point(0, 0));
	auto topMain = CSLoader::getInstance()->createNode("LayerTaiXiuTop.csb");
	topMain->setPosition(Point(visibleSize.width / 2, visibleSize.height - topMain->getContentSize().height / 2));
	auto bottomMain = CSLoader::getInstance()->createNode("LayerTaiXiuBottom.csb");
	bottomMain->setPosition(Point(visibleSize.width / 2, visibleSize.height - bottomMain->getContentSize().height / 2));
	auto rootNode = CSLoader::getInstance()->createNode("LayerTaiXiuMain.csb");
	rootNode->setContentSize(visibleSize);
	topMain->setContentSize(visibleSize);
	bottomMain->setContentSize(visibleSize);
	
	//rootNode->setPosition(Point(visibleSize.width * 4 / 5, visibleSize.height / 2));
	ui::Helper::doLayout(rootNode);
	ui::Helper::doLayout(topMain);
	ui::Helper::doLayout(bottomMain);
	//layer_login->addChild(rootNode);

	this->addChild(topMain);
	this->addChild(bottomMain);
	this->addChild(rootNode);

	/*auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	*/
	return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
	count++;
	return true;
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event){

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (count == 1){
		/*auto rootNode = CSLoader::getInstance()->createNode("Layer_Setting.csb");
		rootNode->setContentSize(visibleSize);
		ui::Helper::doLayout(rootNode);
		this->addChild(rootNode);*/

		auto rootNode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("LayerPopupSetting.json");
		rootNode->setContentSize(visibleSize);
		ui::Helper::doLayout(rootNode);
		this->addChild(rootNode);

		//auto button = static_cast<cocos2d::ui::Button*>(rootNode->getChildByName("btn_tuchoi"));
		
		//if (button != NULL){
		//	button->addClickEventListener([](Ref*){
		//		CCLOG("Pressed");
		//		//this->removeChild((Node*)rootNode, true);
		//	});
		//}
	}
}