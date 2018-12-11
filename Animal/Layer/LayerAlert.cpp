//
//  LayerAlert.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/22/18.
//

#include "LayerAlert.hpp"
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
bool LayerAlert::init()
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
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerAlert.csb");
    rootNode->setContentSize(Size(visibleSize.width, rootNode->getContentSize().height));
    ui::Helper::doLayout(rootNode);
    
    ActionTimeline* action = CSLoader::createTimeline("LayerAlert.csb");
    action->gotoFrameAndPlay(0, false);
    this->addChild(rootNode);
    this->setContentSize(glview->getFrameSize());
    
    auto pnlMain = (Layout *)rootNode->getChildByName("pnlMain");
    if (pnlMain) {
        lblNoti = (Text*)pnlMain->getChildByName("lblNoti");
        
        btnBack = (Button *)pnlMain->getChildByName("Button_1");
        btnBack->addClickEventListener(CC_CALLBACK_1(LayerAlert::backMain, this));
    }
    
    return true;
}

LayerAlert::LayerAlert()
{
    
}

LayerAlert::~LayerAlert()
{
    
}

void LayerAlert::setNoti(std::string noti)
{
    lblNoti->setString(noti);
    
}
void LayerAlert::btnOk(){
    btnBack->addClickEventListener(CC_CALLBACK_1(LayerAlert::tapOk, this));
}

void LayerAlert::btnOkBuyItem(){
    btnBack->addClickEventListener(CC_CALLBACK_1(LayerAlert::tapOkBuy, this));
}

void LayerAlert::backMain(Ref *pSender)
{
    this->removeFromParent();
}

void LayerAlert::tapOkBuy(Ref *pSender){
    if (mCallBack && mCallBackListener) {
        Node *param = Node::create();
        (mCallBack->*mCallBackListener)(param);
    
    }
}

void LayerAlert::tapOk(Ref *pSender){
    if (mCallBack && mCallBackListener) {
        Node *param = Node::create();
        (mCallBack->*mCallBackListener)(param);
        this->removeFromParent();
    }
}

void LayerAlert::setCallBack(Ref * target, SEL_CallFuncO listener){
    mCallBack = target;
    mCallBackListener = listener;
}

