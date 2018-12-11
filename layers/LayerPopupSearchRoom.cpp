#include "LayerPopupSearchRoom.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "scenes/ScenePickRoom.h"
#include "mUtils.h"
#include "layergames/_Chat_.h"
#include "Requests/JoinRoomRequest.h"
#include "boost/smart_ptr/make_shared_object.hpp"
#include "../SceneManager.h"
using namespace cocos2d::ui;

LayerPopupSearchRoom::LayerPopupSearchRoom()
{

}

LayerPopupSearchRoom::~LayerPopupSearchRoom()
{

}

bool LayerPopupSearchRoom::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(false);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupSearchRoom.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));

	this->addChild(rootNode);
	auto btn_huy = dynamic_cast<Button*>(rootNode->getChildByName("btnCancel"));
	if (btn_huy != NULL)
	{
		btn_huy->addTouchEventListener(CC_CALLBACK_2(LayerPopupSearchRoom::onButtonHuy, this));
	}
	auto btn_dongy = dynamic_cast<Button*>(rootNode->getChildByName("btnOK"));
	if (btn_dongy != NULL)
	{
		btn_dongy->addTouchEventListener(CC_CALLBACK_2(LayerPopupSearchRoom::onButtonDongy, this));
	}
	txtRoomNumber = dynamic_cast<TextField*>(rootNode->getChildByName("txtRoomNumber"));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	txtRoomNumber->setInputMode(EditBox::InputMode::NUMERIC);
#endif
	return true;
}

bool LayerPopupSearchRoom::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupSearchRoom::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupSearchRoom::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerPopupSearchRoom::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}
void LayerPopupSearchRoom::onButtonHuy(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
		this->removeFromParentAndCleanup(true);
}

void LayerPopupSearchRoom::onButtonDongy(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
	}
}
