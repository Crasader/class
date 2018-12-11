//
//  LayerPopupBuyItem.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 5/9/18.
//

#include "LayerPopupBuyItem.hpp"
#include "AppMacros.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "GameManager.hpp"
#include "Common.h"
#include "ThemeManager.hpp"
#include "cocostudio/CocoStudio.h"
#include "../Scene/MainScene.hpp"
#include "LayerAlert.hpp"

using namespace cocos2d;
using namespace cocostudio::timeline;
USING_NS_CC;
bool LayerPopupBuyItem::init()
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
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopUpBuyItem.csb");
    rootNode->setContentSize(Size(visibleSize.width, rootNode->getContentSize().height));
    ui::Helper::doLayout(rootNode);
    
//    ActionTimeline* action = CSLoader::createTimeline("LayerPopUpBuyItem.csb");
//    action->gotoFrameAndPlay(0, false);
    this->addChild(rootNode);
    this->setContentSize(glview->getFrameSize());
    
    pnlMain = (Layout *)rootNode->getChildByName("pnlMain");
    if (pnlMain) {
        
//        lblNoti = (Text*)pnlMain->getChildByName("lblNoti");
        
        auto btnCancel = (Button *)pnlMain->getChildByName("btnCancel");
        btnCancel->addClickEventListener(CC_CALLBACK_1(LayerPopupBuyItem::tapCancel, this));
        
        auto btnUnlock = (Button *)pnlMain->getChildByName("btnBuy");
        btnUnlock->addClickEventListener(CC_CALLBACK_1(LayerPopupBuyItem::tapBuy, this));
        
        lblTitle = (Text *)pnlMain->getChildByName("lblTitle");
        lblDes = (Text *)pnlMain->getChildByName("lblDes");
        lblMoney = (Text *)pnlMain->getChildByName("lblMoney");
        
        img = (Sprite *)pnlMain->getChildByName("img");
        
    }
    
    return true;
}
LayerPopupBuyItem::LayerPopupBuyItem()
{
    
}

LayerPopupBuyItem::~LayerPopupBuyItem()
{
    
}

void LayerPopupBuyItem::setContent(std::string title, std::string des,std::string money, std::string imgLink, std::string typeItem)
{
    lblTitle->setString(title);
    lblMoney->setString(money);
    lblDes->setString(des);
    img->setTexture(imgLink);
    this->typeItem = typeItem;
}


void LayerPopupBuyItem::tapBuy(Ref *pSender){

    auto result = GameManager::getInstance()->boughtItem(this->typeItem);
    if (result > 0) {
        CCLOG("Mua thanh cong");
            if (mCallBack && mCallBackListener) {
                Node *param = Node::create();
                (mCallBack->*mCallBackListener)(param);
            }
        return;
    }
    showNotifi();
    
}

void LayerPopupBuyItem::showNotifi(){
    LayerAlert *alert = LayerAlert::create();
    alert->setNoti(NOTI_NOT_MONEY.c_str());
    alert->setCallBack(this, callfuncO_selector(LayerPopupBuyItem::removeLayer));
    alert->btnOkBuyItem();
    this->addChild(alert);
    this->pnlMain->setVisible(false);
}
void LayerPopupBuyItem::removeLayer(Ref *pSender){
    this->removeFromParent();
}

void LayerPopupBuyItem::tapCancel(Ref *pSender){
    if (mCallBack && mCallBackListener2) {
        Node *param = Node::create();
        (mCallBack->*mCallBackListener2)(param);
        this->removeFromParent();
    }
    
}

void LayerPopupBuyItem::setCallBackCancel(Ref *target, SEL_CallFuncO listen){
    mCallBack = target;
    mCallBackListener2 = listen;
}


void LayerPopupBuyItem::setCallBack(Ref *target, SEL_CallFuncO listen){
    mCallBack = target;
    mCallBackListener = listen;
}
