#include "LayerPopupForgetPassword.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../data_manager.h"
using namespace cocos2d::ui;

LayerPopupForgetPassword::LayerPopupForgetPassword()
{

}

LayerPopupForgetPassword::~LayerPopupForgetPassword()
{

}

bool LayerPopupForgetPassword::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupForgetPassword.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);
    auto lblTitleTen = static_cast<Text*> (rootNode->getChildByName("Text_8"));
    lblTitleTen->setTextVerticalAlignment(cocos2d::TextVAlignment::TOP);
    lblTitleTen->setString(dataManager.GetSysString(876));
	auto imgEmailBg = static_cast<ImageView*>(rootNode->getChildByName("Image_5"));
	auto txtEmail = dynamic_cast<TextField*>(rootNode->getChildByName("txtEmail"));
	lblContact = dynamic_cast<Text*>(rootNode->getChildByName("Text_6"));
    lblContact->setString("");
	imgEmailBg->setVisible(false);
    txtEmail->setVisible(false);

#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
    txtEmail->setVisibleEditBox(false);
#endif

	auto btnGetPassword = dynamic_cast<Button*>(rootNode->getChildByName("btnLayLaiPassword"));
	if (btnGetPassword != NULL)
	{
		btnGetPassword->addClickEventListener(CC_CALLBACK_1(LayerPopupForgetPassword::onButtonGetPassword, this));
	}
	btnGetPassword->setVisible(false);

	auto btnClose = static_cast<ui::Button*>(rootNode->getChildByName("btnClose"));
	if (btnClose != NULL){
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupForgetPassword::onButtonClose, this));
	}
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupForgetPassword::scheduleUpdateContactInfo),this,1, false);
	return true;
}

bool LayerPopupForgetPassword::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupForgetPassword::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupForgetPassword::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerPopupForgetPassword::onExit()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupForgetPassword::scheduleUpdateContactInfo),this);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerPopupForgetPassword::onButtonClose(Ref* pSender){
	this->removeFromParentAndCleanup(true);

}
void LayerPopupForgetPassword::onButtonGetPassword(Ref* pSender){

}

void LayerPopupForgetPassword::scheduleUpdateContactInfo(float dt)
{
	if (dataManager.getAppConfig().callcenter.length() == 0 || !lblContact)
		return;
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupForgetPassword::scheduleUpdateContactInfo),this);
	lblContact->setString(dataManager.GetSysString(593)+" " + dataManager.getAppConfig().callcenter);
}
