//
//  LayerPopupDauTruong.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/28/16.
//
//

#include "LayerPopupDauTruong.h"
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
#include "../layergames/_Chat_.h"
LayerPopupDauTruong::LayerPopupDauTruong()
{
}


LayerPopupDauTruong::~LayerPopupDauTruong()
{
    
}

bool LayerPopupDauTruong::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupDauTruong.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
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
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupDauTruong::onBtnClose, this));
    }
    
    Button* btnHuongDan = dynamic_cast<Button*>(pnlBg->getChildByName("btnHuongDan"));
    if(btnHuongDan){
        btnHuongDan->addClickEventListener(CC_CALLBACK_1(LayerPopupDauTruong::onBtnBaoTri, this));
        btnHuongDan->setPressedActionEnabled(true);
    }
    Button* btnXepHang = dynamic_cast<Button*>(pnlBg->getChildByName("btnXepHang"));
    if(btnXepHang){
        btnXepHang->addClickEventListener(CC_CALLBACK_1(LayerPopupDauTruong::onBtnBaoTri, this));
        btnXepHang->setPressedActionEnabled(true);

    }
    Button* btnDangKy1 = dynamic_cast<Button*>(pnlBg->getChildByName("btnDangKy1"));
    if(btnDangKy1){
        btnDangKy1->addClickEventListener(CC_CALLBACK_1(LayerPopupDauTruong::onBtnBaoTri, this));
        btnDangKy1->setPressedActionEnabled(true);

    }
    Button* btnDangKy3 = dynamic_cast<Button*>(pnlBg->getChildByName("btnDangKy3"));
    if(btnDangKy3){
        btnDangKy3->addClickEventListener(CC_CALLBACK_1(LayerPopupDauTruong::onBtnBaoTri, this));
        btnDangKy3->setPressedActionEnabled(true);

    }
    Button* btnLichSu2 = dynamic_cast<Button*>(pnlBg->getChildByName("btnLichSu2"));
    if(btnLichSu2){
        btnLichSu2->addClickEventListener(CC_CALLBACK_1(LayerPopupDauTruong::onBtnBaoTri, this));
        btnLichSu2->setPressedActionEnabled(true);

    }
    Button* btnLichSu1 = dynamic_cast<Button*>(pnlBg->getChildByName("btnLichSu1"));
    if(btnLichSu1){
        btnLichSu1->addClickEventListener(CC_CALLBACK_1(LayerPopupDauTruong::onBtnBaoTri, this));
        btnLichSu1->setPressedActionEnabled(true);

    }
    Button* btnLichSu3 = dynamic_cast<Button*>(pnlBg->getChildByName("btnLichSu3"));
    if(btnLichSu3){
        btnLichSu3->addClickEventListener(CC_CALLBACK_1(LayerPopupDauTruong::onBtnBaoTri, this));
        btnLichSu3->setPressedActionEnabled(true);

    }

    
    
    return true;
}
void LayerPopupDauTruong::onBtnBaoTri(Ref* pSender){
    auto toast = Chat::create("Chức năng đấu trường hiện tại chưa mở!", -1);
    this->addChild(toast);

}
void LayerPopupDauTruong::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}


bool LayerPopupDauTruong::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupDauTruong::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupDauTruong::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupDauTruong::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

