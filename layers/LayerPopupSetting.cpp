#include "LayerPopupSetting.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;


LayerPopupSetting::LayerPopupSetting()
{
}


LayerPopupSetting::~LayerPopupSetting()
{
}

bool LayerPopupSetting::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupSetting.csb");

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());

	//add event listenner
	auto btn_setting_popup_cauhinh = dynamic_cast<Button*>(rootNode->getChildByName("btn_setting_popup_cauhinh"));
	if (btn_setting_popup_cauhinh != NULL){
		btn_setting_popup_cauhinh->addClickEventListener(CC_CALLBACK_1(LayerPopupSetting::onButtonConfig, this));
	}
	auto btn_setting_popup_huongdan = dynamic_cast<Button*>(rootNode->getChildByName("btn_setting_popup_huongdan"));
	if (btn_setting_popup_huongdan != NULL){
		btn_setting_popup_huongdan->addClickEventListener(CC_CALLBACK_1(LayerPopupSetting::onButtonGuide, this));
	}

	return true;
}

bool LayerPopupSetting::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupSetting::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupSetting::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerPopupSetting::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerPopupSetting::onButtonConfig(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

void LayerPopupSetting::onButtonGuide(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

void LayerPopupSetting::onButtonSoNguoiChoi(Ref* pSender)
{

}
