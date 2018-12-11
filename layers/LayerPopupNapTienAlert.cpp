#include "LayerPopupNapTienAlert.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Requests/ExtensionRequest.h"
#include "../SceneManager.h"
#include "../scenes/SceneLogin.h"
#include "../data_manager.h"
#include "../scenes/ScenePickRoom.h"
#include "LayerPopupDoiMoc.h"
#include "../mUtils.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../layergames/NativeHelper.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
using namespace cocos2d::ui;

LayerPopupNapTienAlert::LayerPopupNapTienAlert()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupNapTienAlert::~LayerPopupNapTienAlert()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupNapTienAlert::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(false);
	this->setTag(POPUP_THONGBAO);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupNapTienAlert.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    auto Panel_1_0 = static_cast<Layout*>(rootNode->getChildByName("Panel_1_0"));

	Panel_1_0->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));

	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(Panel_1_0->getContentSize());

    auto btnClose = static_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->setPressedActionEnabled(true);
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupNapTienAlert::onBtnClose, this));
    }

    
    this->txtMoTa = static_cast<Text*> (pnlBg->getChildByName("txtMoTa"));
    
    this->btnNapTien = dynamic_cast<Button*>(pnlBg->getChildByName("btnNapTien"));
    if (this->btnNapTien != NULL){
        this->btnNapTien->addClickEventListener(CC_CALLBACK_1(LayerPopupNapTienAlert::onBtnNapTien, this));
        this->btnNapTien->setTitleText(dataManager.GetSysString(962));
    }
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
	return true;
}
void LayerPopupNapTienAlert::setText(string msg){
    this->txtMoTa->setString(msg);
}
void LayerPopupNapTienAlert::onBtnClose(Ref* pSender){
    this->removeFromParent();
}

bool LayerPopupNapTienAlert::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupNapTienAlert::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupNapTienAlert::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerPopupNapTienAlert::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
    if (!SceneManager::getSingleton().getIsShowTX() && !SceneManager::getSingleton().getIsShowXD()&& !SceneManager::getSingleton().getIsShowPK()&& !SceneManager::getSingleton().getIsShowVQ()&& !SceneManager::getSingleton().getIsShowCT()){

    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->showWebView();
    }
    }
}
void LayerPopupNapTienAlert::onBtnNapTien(Ref* pSender)
{
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    if (this->type == 1){
        LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
        popup->onBtnNapHao(NULL);
        _currScene->addChild(popup,2000,9100);
        this->removeFromParent();
        return;
    }

    LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
    popup->onBtnNapThe(NULL);
    if (dataManager.getAppConfig().isHideBilling){
        popup->onBtnNapHao(NULL);
    }
    _currScene->addChild(popup,2000,9101);
    this->removeFromParent();
}
void LayerPopupNapTienAlert::setType(int _type){
    this->type = _type;
}
