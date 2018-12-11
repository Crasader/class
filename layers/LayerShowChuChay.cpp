//
//  LayerShowChuChay.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/27/16.
//
//

#include "LayerShowChuChay.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupRecharge.h"
#include "LayerPopupUngTien.h"
#include "LayerPopupThongBao.h"
#include "Requests/LogoutRequest.h"
#include "../data_manager.h"
LayerShowChuChay::LayerShowChuChay()
{
}


LayerShowChuChay::~LayerShowChuChay()
{
    
}

bool LayerShowChuChay::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerShowChuChay.csb");
    Size sizeAdd;
   // if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    if (SceneManager::getSingleton().currLang == 2 || SceneManager::getSingleton().currLang == 4){
        auto txtChay = static_cast<TextBMFont*>(rootNode->getChildByName("txtContent"));
        this->txtChuChay2 = Label::createWithSystemFont("", "", 50);
        this->txtChuChay2->setColor(Color3B::YELLOW);
        this->txtChuChay2->setPosition(Vec2(txtChay->getPositionX(),txtChay->getPositionY()+sizeAdd.height));
        this->addChild(this->txtChuChay2);

    }else{
        this->txtChuChay = static_cast<TextBMFont*>(rootNode->getChildByName("txtContent"));
        this->txtChuChay->setPosition(Vec2(this->txtChuChay->getPositionX(),this->txtChuChay->getPositionY()+sizeAdd.height));

    }
    
    
    //this->txtChuChay->setScale(5);
    return true;
}
void LayerShowChuChay::setContent(string str){
    this->lstChuChay.push_back(str);
    if (this->lstChuChay.size() == 1){
        this->chayChu(1);
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerShowChuChay::chayChu),this,20, false);
    }
    

}
void LayerShowChuChay::chayChu(float dt){
    
    if (SceneManager::getSingleton().currLang == 2 || SceneManager::getSingleton().currLang == 4){
        if (this->lstChuChay.size() == currentshow) {
            Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerShowChuChay::chayChu),this);
            this->actionFinished();
            
            return;
        }
        this->txtChuChay2->setString(this->lstChuChay[currentshow]);
        currentshow++;
        this->txtChuChay2->setPositionX(2000);
        this->txtChuChay2->runAction(Sequence::create(MoveTo::create(20,Vec2(-this->txtChuChay2->getContentSize().width*2,this->txtChuChay2->getPositionY())),NULL));
    }
    else{
        if (this->lstChuChay.size() == currentshow) {
            Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerShowChuChay::chayChu),this);
            this->actionFinished();
            
            return;
        }
        this->txtChuChay->setString(this->lstChuChay[currentshow]);
        currentshow++;
        this->txtChuChay->setPositionX(2000);
        this->txtChuChay->runAction(Sequence::create(MoveTo::create(20,Vec2(-this->txtChuChay->getContentSize().width*2,this->txtChuChay->getPositionY())),NULL));
    }

   // this->lstChuChay.erase(this->lstChuChay.begin());

}
void LayerShowChuChay::actionFinished(){

    this->removeFromParentAndCleanup(true);
}
bool LayerShowChuChay::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerShowChuChay::onEnter()
{
    Layer::onEnter();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = CC_CALLBACK_2(LayerShowChuChay::onTouchBegan, this);
//    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerShowChuChay::onExit()
{
    Layer::onExit();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

