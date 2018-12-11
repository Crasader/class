//
//  LayerAlertBuyTheme.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/24/18.
//

#include "LayerAlertBuyTheme.hpp"
#include "AppMacros.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "GameManager.hpp"
#include "Common.h"
#include "ThemeManager.hpp"
#include "cocostudio/CocoStudio.h"
#include "../Scene/MainScene.hpp"


using namespace cocos2d;
using namespace cocostudio::timeline;
USING_NS_CC;
bool LayerAlertBuyTheme::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    //    CCLOG(std::string(visibleSize));
    CCLOG("%s", std::to_string(glview->getFrameSize().width).c_str());
    CCLOG("%s", std::to_string(glview->getFrameSize().height).c_str());
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerAlertBuyTheme.csb");
    rootNode->setContentSize(Size(visibleSize.width, rootNode->getContentSize().height));
    ui::Helper::doLayout(rootNode);
    
    ActionTimeline* action = CSLoader::createTimeline("LayerAlertBuyTheme.csb");
    action->gotoFrameAndPlay(0, false);
    this->addChild(rootNode);
    this->setContentSize(glview->getFrameSize());
    
    auto pnlMain = (Layout *)rootNode->getChildByName("pnlMain");
    if (pnlMain) {
        
        lblNoti = (Text*)pnlMain->getChildByName("lblNoti");
        
        auto btnCancel = (Button *)pnlMain->getChildByName("btnCancel");
        btnCancel->addClickEventListener(CC_CALLBACK_1(LayerAlertBuyTheme::tapCancel, this));
        
        auto btnUnlock = (Button *)pnlMain->getChildByName("btnUnlock");
        btnUnlock->addClickEventListener(CC_CALLBACK_1(LayerAlertBuyTheme::tapUnlock, this));
    }
    
    return true;
}
LayerAlertBuyTheme::LayerAlertBuyTheme()
{
    
}

LayerAlertBuyTheme::~LayerAlertBuyTheme()
{
    
}

void LayerAlertBuyTheme::setInfoTheme(int idTheme)
{
    
    string name = ThemeManager::getInstance()->getNameThemeUnlock(idTheme);
    int price = ThemeManager::getInstance()->getPriceThemeUnlock(idTheme);
    lblNoti->setString("Do you want unlock theme: " + name + " with price: " + std::to_string(price) +"$?");
    
}

void LayerAlertBuyTheme::setNoti(std::string noti)
{
    lblNoti->setString(noti);
}


void LayerAlertBuyTheme::tapUnlock(Ref *pSender){
    if (mCallBack && mCallBackListener) {
        Node *param = Node::create();
        (mCallBack->*mCallBackListener)(param);
        this->removeFromParent();
    }
}

void LayerAlertBuyTheme::tapCancel(Ref *pSender){
    if (mCallBack && mCallBackListener2) {
        Node *param = Node::create();
        (mCallBack->*mCallBackListener2)(param);
        this->removeFromParent();
    }
    
}
void LayerAlertBuyTheme::setCallBackCancel(Ref *target, SEL_CallFuncO listen){
    mCallBack = target;
    mCallBackListener2 = listen;
}


void LayerAlertBuyTheme::setCallBack(Ref *target, SEL_CallFuncO listen){
    mCallBack = target;
    mCallBackListener = listen;
}
