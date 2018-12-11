//
//  LayerWinSlot.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerWinSlot.h"
#include "Requests/ExtensionRequest.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "LayerPopupThongBao.h"
LayerWinSlot::LayerWinSlot()
{

}


LayerWinSlot::~LayerWinSlot()
{

}

bool LayerWinSlot::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto rootNode = CSLoader::getInstance()->createNode("LayerWinSlot.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
 //   if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    

    
    for (int i = 1; i <= 22; i ++){
        ImageView* img = dynamic_cast<ImageView*>(pnlBg->getChildByName(StringUtils::format("img1_%d",i)));
        img->setVisible(false);
        this->coin.push_back(img);
        this->pos.push_back(img->getPosition());
        img->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));

    }
    this->imgDich= dynamic_cast<ImageView*>(pnlBg->getChildByName("imgDich"));
    this->imgDich->setVisible(false);
    //this->txtChuChay->setScale(5);
    return true;
}
void LayerWinSlot::setRoomType(int type){
    if (type == 0){
         for (int i = 0 ; i< this->coin.size();i++){
             this->coin[i]->loadTexture("moc3-icon-moc.png");
         }
    }else{
        for (int i = 0 ; i< this->coin.size();i++){
            this->coin[i]->loadTexture("moc3-icon-hao.png");
        }
    }
}
void LayerWinSlot::startAnim(){
    this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create(CC_CALLBACK_0(LayerWinSlot::actionDone, this)),NULL));
}
void LayerWinSlot::actionDone(){
    if (mCallBack && mCallBackListener){
        //mCallBack
        Node *param = Node::create();
        (mCallBack->*mCallBackListener)(param);
    }
    this->removeFromParent();
}
bool LayerWinSlot::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerWinSlot::setCallBack(Ref* target,SEL_CallFuncO listener){
    mCallBack = target;
    mCallBackListener = listener;
}
void LayerWinSlot::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerWinSlot::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerWinSlot::onExit()
{
    Layer::onExit();
   
}

