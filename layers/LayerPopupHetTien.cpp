//
//  LayerPopupHetTien.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/8/16.
//
//

#include "LayerPopupHetTien.h"
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
LayerPopupHetTien::LayerPopupHetTien()
{
}


LayerPopupHetTien::~LayerPopupHetTien()
{
    
}

bool LayerPopupHetTien::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupHetTien.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
    }else{
        sizeAdd = Size(0,0);
    }
    Size sizeAdd2;
    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
        sizeAdd2 = ChanUtils::getSizePos();
        
    }else{
        sizeAdd2 = Size(0,0);
    }
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
    pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupHetTien::onBtnClose, this));
    }
    

    this->btnUngTien = dynamic_cast<Button*>(pnlBg->getChildByName("btnUngTien"));
    if(this->btnUngTien){
        this->btnUngTien->addClickEventListener(CC_CALLBACK_1(LayerPopupHetTien::onBtnUngTien, this));
    }

    Button* btnNapTien = dynamic_cast<Button*>(pnlBg->getChildByName("btnNapTien"));
    if(btnNapTien){
        btnNapTien->addClickEventListener(CC_CALLBACK_1(LayerPopupHetTien::onBtnNapTien, this));
    }
    
    Button* btnThoat = dynamic_cast<Button*>(pnlBg->getChildByName("btnThoat"));
    if(btnThoat){
        btnThoat->addClickEventListener(CC_CALLBACK_1(LayerPopupHetTien::onBtnThoat, this));
        btnThoat->setVisible(true);
    }
    
    
//    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
//    int acctype = 0;
//    double money = 0;
//    if (mySelf != NULL)
//    {
//        boost::shared_ptr<UserVariable> acctype_ptr = mySelf->GetVariable("accVipType");
//        if (acctype_ptr != NULL)
//            acctype = *acctype_ptr->GetIntValue();
//        boost::shared_ptr<UserVariable> amf = mySelf->GetVariable("amf");
//        if (amf != NULL)
//            money = *amf->GetDoubleValue();
//    }
//    
//    int minVip = 10 - dataManager.getAppConfig().minungtien.size()+1;
//    
//    if (acctype >= minVip){
//        if (money <= dataManager.getAppConfig().minungtien[acctype-minVip]*1000)
//            btnUngTien->setEnabled(true);
//        else{
//            btnUngTien->setEnabled(false);
//        }
//    }
//    else{
//        btnUngTien->setEnabled(false);
//    }

    
    
    return true;
}
void LayerPopupHetTien::onBtnNapTien(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    
 /*   LayerPopupRecharge* popup = LayerPopupRecharge::create();
    _currScene->addChild(popup,10000);*/

	LayerPopupCuaHang* _popupCuaHang = LayerPopupCuaHang::create();
	_popupCuaHang->onBtnNapThe(NULL);
	_currScene->addChild(_popupCuaHang, 10000,9070);
    this->removeFromParentAndCleanup(true);

}
void LayerPopupHetTien::onBtnThoat(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;

    auto layerpoup = LayerPopupThongBao::create();
    layerpoup->setMessage(dataManager.GetSysString(567));
    layerpoup->setContentMess("Bạn có muốn thoát ứng dụng?");
    layerpoup->setCallBackFromButton(this, callfuncO_selector(LayerPopupHetTien::onPopupCallBack), 1);
    _currScene->addChild(layerpoup, 10000,9071);
}
void LayerPopupHetTien::onBtnUngTien(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    
    LayerPopupUngTien* popup = LayerPopupUngTien::create();
    _currScene->addChild(popup,10000,9072);
    this->removeFromParentAndCleanup(true);

}
void LayerPopupHetTien::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}


bool LayerPopupHetTien::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupHetTien::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupHetTien::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupHetTien::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
void LayerPopupHetTien::onPopupCallBack(Ref *pSender)
{
    auto node = (Node*)pSender;
    int tag = node->getTag();
    
        //Director::getInstance()->end();
        boost::shared_ptr<IRequest> request(new LogoutRequest());
        GameServer::getSingleton().Send(request);
        //
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
        return;
#endif
        
        Director::getInstance()->end();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
}

