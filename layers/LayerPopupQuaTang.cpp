//
//  LayerPopupQuaTang.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerPopupQuaTang.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "LayerPopupEvent.h"
#include "../scenes/ScenePickRoom.h"
LayerPopupQuaTang::LayerPopupQuaTang()
{
}


LayerPopupQuaTang::~LayerPopupQuaTang()
{
    
}

bool LayerPopupQuaTang::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupQuaTang.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
//    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    auto txtNhan = static_cast<Text*> (pnlBg->getChildByName("txtNhan"));
    txtNhan->setString(dataManager.GetSysString(761));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupQuaTang::onBtnClose, this));
    }
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    
    auto pnlContent = dynamic_cast<Layout*>(pnlBg->getChildByName("pnlContent"));
    
    pnlContent->setSwallowTouches(false);
    this->btnSuKien = dynamic_cast<Button*>(pnlBg->getChildByName("btnSuKien"));
    if(this->btnSuKien ){
        this->btnSuKien ->addClickEventListener(CC_CALLBACK_1(LayerPopupQuaTang::onBtnSuKien, this));
        this->btnSuKien->setTitleText(dataManager.GetSysString(762));
    }
    this->btnQuaTang = dynamic_cast<Button*>(pnlBg->getChildByName("btnQuaTang"));
    if(this->btnQuaTang ){
        this->btnQuaTang ->addClickEventListener(CC_CALLBACK_1(LayerPopupQuaTang::onBtnQuaTang, this));
        this->btnQuaTang->setTitleText(dataManager.GetSysString(763));

    }
    this->btnGiftcode = dynamic_cast<Button*>(pnlBg->getChildByName("btnGiftcode"));
    if(this->btnGiftcode ){
        this->btnGiftcode ->addClickEventListener(CC_CALLBACK_1(LayerPopupQuaTang::onBtnGiftCode, this));
        this->btnGiftcode->setTitleText(dataManager.GetSysString(764));

    }
    
    this->btnCongDong = dynamic_cast<Button*>(pnlBg->getChildByName("btnCongDong"));
    if(this->btnCongDong ){
        this->btnCongDong ->addClickEventListener(CC_CALLBACK_1(LayerPopupQuaTang::onBtnCongDong, this));
    }
    
    if (dataManager.getAppConfig().isHideBilling || dataManager.getAppConfig().is24h){
        this->btnGiftcode->setVisible(false);
        this->btnSuKien->setVisible(false);
        this->btnQuaTang->setVisible(false);
        Text* txtMau = dynamic_cast<Text*>(pnlBg->getChildByName("txtMau"));
        txtMau->setVisible(true);
    }
    
    this->offer = LayerOffer::create();
    pnlBg->addChild(offer);
    this->offer->hideLayer();
    this->offer->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    //
    this->giftcode = LayerGiftCode::create();
    pnlBg->addChild(this->giftcode);
    this->giftcode->hideLayer();
    this->giftcode->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    
    this->congdong = LayerCongDong::create();
    pnlBg->addChild(this->congdong);
    this->congdong->hideLayer();
    this->congdong->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    
    
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }
    
    //    this->sukien = LayerWebView::create();
    //    pnlBg->addChild(this->sukien);
    //    this->sukien->hideLayer();
    //    this->sukien->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    
    //
    //this->txtChuChay->setScale(5);
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

    return true;
}
void LayerPopupQuaTang::onBtnCongDong(Ref* pSender){
    this->btnQuaTang->setEnabled(true);
    this->btnQuaTang->setBright(true);
    
    this->btnCongDong->setEnabled(false);
    this->btnCongDong->setBright(false);
    this->congdong->showLayer();
    
    this->btnGiftcode->setEnabled(true);
    this->btnGiftcode->setBright(true);
    
    this->btnSuKien->setEnabled(true);
    this->btnSuKien->setBright(true);
    this->offer->hideLayer();
    this->giftcode->hideLayer();
    // this->sukien->hideLayer();
    
}
void LayerPopupQuaTang::onBtnSuKien(Ref* pSender){
    if (SceneManager::getSingleton().lstEvent.size()<=0){
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return;
        
        Chat* popup = Chat::create(dataManager.GetSysString(436),-1);
        _currScene->addChild(popup,10000,9085);
        return;
    }
    this->removeFromParent();
    auto currScene = Director::getInstance()->getRunningScene();
    LayerPopupEvent* popup = LayerPopupEvent::create();
    currScene->addChild(popup,2000,9086);
    //    this->btnQuaTang->setEnabled(true);
    //    this->btnQuaTang->setBright(true);
    //
    //    this->btnGiftcode->setEnabled(true);
    //    this->btnGiftcode->setBright(true);
    //
    //    this->btnSuKien->setEnabled(false);
    //    this->btnSuKien->setBright(false);
    //    this->offer->hideLayer();
    //    this->giftcode->hideLayer();
    //
    //    this->btnCongDong->setEnabled(true);
    //    this->btnCongDong->setBright(true);
    //    this->congdong->hideLayer();
    //    this->sukien->showLayer();
    
}
void LayerPopupQuaTang::onBtnQuaTang(Ref* pSender){
    this->btnQuaTang->setEnabled(false);
    this->btnQuaTang->setBright(false);
    
    this->btnGiftcode->setEnabled(true);
    this->btnGiftcode->setBright(true);
    
    this->btnCongDong->setEnabled(true);
    this->btnCongDong->setBright(true);
    this->congdong->hideLayer();
    
    this->btnSuKien->setEnabled(true);
    this->btnSuKien->setBright(true);
    this->offer->showLayer();
    this->giftcode->hideLayer();
    //this->sukien->hideLayer();
}
void LayerPopupQuaTang::onBtnGiftCode(Ref* pSender){
    this->btnQuaTang->setEnabled(true);
    this->btnQuaTang->setBright(true);
    
    this->btnCongDong->setEnabled(true);
    this->btnCongDong->setBright(true);
    this->congdong->hideLayer();
    
    this->btnGiftcode->setEnabled(false);
    this->btnGiftcode->setBright(false);
    
    this->btnSuKien->setEnabled(true);
    this->btnSuKien->setBright(true);
    this->offer->hideLayer();
    this->giftcode->showLayer();
    //this->sukien->hideLayer();
}
void LayerPopupQuaTang::hideLayer(){
    this->setVisible(false);
}
void LayerPopupQuaTang::showLayer(){
    this->setVisible(true);
    
}
bool LayerPopupQuaTang::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerPopupQuaTang::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerPopupQuaTang::onEnter()
{
    Layer::onEnter();
    SceneManager::getSingleton().numPopup+=1;

    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupQuaTang::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupQuaTang::onExit()
{
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

    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

