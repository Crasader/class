#include "LayerPopupHotLine.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Requests/ExtensionRequest.h"
#include "../SceneManager.h"
#include "../scenes/SceneLogin.h"
#include "../data_manager.h"
#include "../scenes/ScenePickRoom.h"
#include "../mUtils.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../layergames/NativeHelper.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
using namespace cocos2d::ui;

LayerPopupHotLine::LayerPopupHotLine()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupHotLine::~LayerPopupHotLine()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupHotLine::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(false);
	this->setTag(POPUP_THONGBAO);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupHotline.csb");
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
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupHotLine::onBtnClose, this));
    }
    
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("lblTitle"));
    lblTitle->setString(dataManager.GetSysString(765));
    
    this->btnHotLine1 = static_cast<Button*>(pnlBg->getChildByName("btnHotLine1"));
    if(this->btnHotLine1){
       
        this->btnHotLine1->setTag(1);
        this->btnHotLine1->setPressedActionEnabled(true);
        this->btnHotLine1->addClickEventListener(CC_CALLBACK_1(LayerPopupHotLine::onBtnHotLine, this));
        if (dataManager.getAppConfig().callcenter == ""){
            this->btnHotLine1->setVisible(false);
        }else{
            this->btnHotLine1->setTitleText(dataManager.getAppConfig().callcenter);

        }
    }
    
    this->btnHotLine2 = static_cast<Button*>(pnlBg->getChildByName("btnHotLine2"));
    if(this->btnHotLine2){
        this->btnHotLine2->setTag(2);

        this->btnHotLine2->setPressedActionEnabled(true);
        this->btnHotLine2->addClickEventListener(CC_CALLBACK_1(LayerPopupHotLine::onBtnHotLine, this));
        if (dataManager.getAppConfig().callcenter2 == ""){
            this->btnHotLine2->setVisible(false);
        }else{
            this->btnHotLine2->setTitleText(dataManager.getAppConfig().callcenter2);
            
        }
    }
    this->btnFP = static_cast<Button*>(pnlBg->getChildByName("btnFP"));
    if(this->btnFP){
        if (SceneManager::getSingleton().isNagaNew){
            this->btnFP->setTitleFontSize(this->btnHotLine1->getTitleFontSize()-5);
            
        }
        this->btnFP->setPressedActionEnabled(true);
        this->btnFP->addClickEventListener(CC_CALLBACK_1(LayerPopupHotLine::onBtnFB, this));
        if (dataManager.getAppConfig().fanpage == ""){
            this->btnFP->setVisible(false);
        }else{
            this->btnFP->setTitleText(dataManager.getAppConfig().fanpage);
        }
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

void LayerPopupHotLine::onBtnClose(Ref* pSender){
    this->removeFromParent();
}

bool LayerPopupHotLine::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupHotLine::onEnter()
{
	Layer::onEnter();
    SceneManager::getSingleton().numPopup+=1;

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupHotLine::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerPopupHotLine::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
    SceneManager::getSingleton().numPopup-=1;

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
void LayerPopupHotLine::onBtnFB(Ref* pSender){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    NativeHelper::openUrl(dataManager.getAppConfig().fanpage.c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::openUrl(dataManager.getAppConfig().fanpage);
#endif
}
void LayerPopupHotLine::onBtnHotLine(Ref* pSender)
{
    auto node = (Node*)pSender;
    int tag = node->getTag();
    if (tag == 1){
        std::string strHotLine = dataManager.getAppConfig().callcenter + "";
        auto nums = mUtils::splitString(strHotLine, '.');
        strHotLine.erase(std::remove(strHotLine.begin(), strHotLine.end(), '.'), strHotLine.end());
        
        #if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        	NativeHelper::callPhone(strHotLine.c_str());
        #endif
        #if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        	IOS::tryToCall(strHotLine);
        #endif
    }
    if (tag == 2){
        std::string strHotLine = dataManager.getAppConfig().callcenter2 + "";
        auto nums = mUtils::splitString(strHotLine, '.');
        strHotLine.erase(std::remove(strHotLine.begin(), strHotLine.end(), '.'), strHotLine.end());
        
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        NativeHelper::callPhone(strHotLine.c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        IOS::tryToCall(strHotLine);
#endif
    }

}
