#include "LayerRank.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/PhomMessDef.h"

USING_NS_CC;
using namespace cocos2d::ui;

LayerRank::LayerRank()
{
}

LayerRank::~LayerRank()
{
}

bool LayerRank::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(true);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		this->isWebView = false;
	}
	else{
		this->isWebView = true;
	}
	Sprite* sprBg;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		sprBg = Sprite::create("background-in-game.jpg");
	}
	else{
		sprBg = Sprite::create("background-sanh.jpg");
	}
	sprBg->setPosition(visibleSize / 2);
	sprBg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	this->addChild(sprBg);

	this->setContentSize(visibleSize);
	//default
	layerRankContent = LayerRankContent::create();
	layerRankContent->setPosition(Vec2(0, 0));
	layerRankContent->isWebView = this->isWebView;
	layerRankContent->setTag(TAG_CHILD);
	this->addChild(layerRankContent);

	layerRankTop = LayerRankTop::create();
	layerRankTop->setAnchorPoint(Vec2(0, 0));
	layerRankTop->setTag(0);
	layerRankTop->isWebView = this->isWebView;
	layerRankTop->setLocalZOrder(100);
	layerRankTop->setPosition(Point(0, visibleSize.height - layerRankTop->getContentSize().height));
	this->addChild(layerRankTop);

	return true;
}

bool LayerRank::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerRank::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerRank::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerRank::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerRank::showTopCaoThu()
{
	layerRankContent->nodeTopCaoThu->setVisible(true);
	layerRankContent->nodeTopDaigia->setVisible(false);
	layerRankContent->loadFirst();
}

void LayerRank::showTopDaiGia()
{
	layerRankContent->nodeTopCaoThu->setVisible(false);
	layerRankContent->nodeTopDaigia->setVisible(true);
	layerRankContent->loadTopDaiGia();
}

void LayerRank::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		layerRankTop->onButtonBack(this, ui::Widget::TouchEventType::ENDED);
	}
}
