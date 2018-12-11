#include "LayerSettingInGameMain.h"
#include "cocostudio/CocoStudio.h"
#include "LayerConfigInGame.h"
#include "LayerGuideInGame.h"
#include "../data_manager.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerSettingInGameMain::LayerSettingInGameMain()
{

}


LayerSettingInGameMain::~LayerSettingInGameMain()
{

}

bool LayerSettingInGameMain::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupSettingInGameMain.csb");
	if (gameID == kGameChan || gameID == kGameXiTo){
		rootNode->setAnchorPoint(Point::ZERO);
		rootNode->setPosition(Point::ZERO);
	}
	else
	{
		rootNode->setAnchorPoint(Point(0.5, 0.5));
		rootNode->setPosition(Point(visibleSize / 2));
	}
	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());
    
    
    

    
	btnCauhinh = static_cast<Button*>(rootNode->getChildByName("btnCauHinh"));
	if (btnCauhinh != NULL){
		btnCauhinh->addTouchEventListener(CC_CALLBACK_2(LayerSettingInGameMain::onBtnCauhinh, this));
	}

	btnHuongdan = static_cast<ui::Button*>(rootNode->getChildByName("btnHuongDan"));
	if (btnHuongdan != NULL){
		btnHuongdan->addTouchEventListener(CC_CALLBACK_2(LayerSettingInGameMain::onBtnHuongDan, this));
	}
	Button *btnClose = static_cast<Button*>(rootNode->getChildByName("btnClose"));
	if (btnClose)
		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerSettingInGameMain::onBtnClose, this));

	return true;
}

bool LayerSettingInGameMain::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}
void LayerSettingInGameMain::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
	auto target = pEvent->getCurrentTarget();
	auto locationInNode = target->convertToNodeSpace(pTouch->getLocation());
	auto s = this->getContentSize();
	auto rect = Rect(this->getPositionX(), this->getPositionY(), s.width, s.height);
	if (rect.containsPoint(locationInNode)) {
	}
	else {
		this->hide();
	}
}

void LayerSettingInGameMain::onBtnCauhinh(Ref* pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::ENDED)
	{
		auto layer = LayerConfigInGame::create();
		this->getParent()->addChild(layer);
		this->hide();
	}
}

void LayerSettingInGameMain::onBtnHuongDan(Ref* pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::ENDED)
	{
		auto layer = LayerGuideInGame::create();
		this->getParent()->addChild(layer);
		this->hide();
	}
}
void LayerSettingInGameMain::show(bool isDisableCuoc){
	setVisible(true);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerSettingInGameMain::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(LayerSettingInGameMain::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerSettingInGameMain::hide(){
	setVisible(false);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
}

void LayerSettingInGameMain::onBtnClose(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{

	}
}
