//
//  LayerPopupChinhSach.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerPopupChinhSach.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "LayerPopupDaiLy.h"
#include "../layergames/_Chat_.h"
#include "../scenes/ScenePickRoom.h"
LayerPopupChinhSach::LayerPopupChinhSach()
{
    GameServer::getSingleton().addListeners(this);

}


LayerPopupChinhSach::~LayerPopupChinhSach()
{
    GameServer::getSingleton().removeListeners(this);

}

bool LayerPopupChinhSach::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupChinhSach.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
  //  if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupChinhSach::onBtnClose, this));
    }

    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto txtNhan = dynamic_cast<Text*>(pnlBg->getChildByName("txtNhan"));
    txtNhan->setString(dataManager.GetSysString(975));
    auto pnlContent = dynamic_cast<Layout*>(pnlBg->getChildByName("pnlContent"));
    
    
    this->btnTyLeNap = dynamic_cast<Button*>(pnlBg->getChildByName("btnTyLeNap"));
    if(this->btnTyLeNap ){
        this->btnTyLeNap->setVisible(!dataManager.getAppConfig().isHideExchange);

        this->btnTyLeNap->addClickEventListener(CC_CALLBACK_1(LayerPopupChinhSach::onBtnTyLeNap, this));
        this->btnTyLeNap->setTitleText(dataManager.GetSysString(1031));

    }
    

    this->btnTyLeRut = dynamic_cast<Button*>(pnlBg->getChildByName("btnTyLeRut"));
    if(this->btnTyLeRut ){
        this->btnTyLeRut->addClickEventListener(CC_CALLBACK_1(LayerPopupChinhSach::onBtnTyLeRut, this));
        this->btnTyLeRut->setTitleText(dataManager.GetSysString(973));

    }
    this->btnQuyenLoiVip = dynamic_cast<Button*>(pnlBg->getChildByName("btnQuyenLoiVip"));
    if(this->btnQuyenLoiVip ){
        this->btnQuyenLoiVip->addClickEventListener(CC_CALLBACK_1(LayerPopupChinhSach::onBtnQuyenLoiVip, this));
        this->btnQuyenLoiVip->setTitleText(dataManager.GetSysString(974));

    }

    this->btnTyLePhe = dynamic_cast<Button*>(pnlBg->getChildByName("btnTyLePhe"));
    if(this->btnTyLePhe ){
        this->btnTyLePhe->addClickEventListener(CC_CALLBACK_1(LayerPopupChinhSach::onBtnTyLePhe, this));
        this->btnTyLePhe->setTitleText(dataManager.GetSysString(976));
        
    }
    
    this->btnTyLeCellCard = dynamic_cast<Button*>(pnlBg->getChildByName("btnTyLeCellCard"));
    if(this->btnTyLeCellCard ){
        this->btnTyLeCellCard->addClickEventListener(CC_CALLBACK_1(LayerPopupChinhSach::onBtnTyLeCellCard, this));
        this->btnTyLeCellCard->setTitleText(dataManager.GetSysString(1032));
        
    }
    
    this->webview = LayerWebView::create();
    pnlBg->addChild(this->webview);
    this->webview->showLayer();
    this->webview->setPosition(Vec2(pnlContent->getPositionX()-38,pnlContent->getPositionY()+sizeAdd.height));
    
    this->onBtnTyLeNap(NULL);
    //this->txtChuChay->setScale(5);
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
    return true;
}
void LayerPopupChinhSach::onBtnTyLeCellCard(Ref* pSender){
    this->btnTyLeCellCard->setEnabled(false);
    this->btnTyLeCellCard->setBright(false);
    this->btnTyLeRut->setEnabled(true);
    this->btnTyLeRut->setBright(true);
    this->btnTyLePhe->setEnabled(true);
    this->btnTyLePhe->setBright(true);
    this->btnQuyenLoiVip->setEnabled(true);
    this->btnQuyenLoiVip->setBright(true);
    this->btnTyLeNap->setEnabled(true);
    this->btnTyLeNap->setBright(true);
    
    string url = "";
    url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+"ty_le_cellcard.html";
    //    }
    this->webview->setWebView(url);
    
    SceneManager::getSingleton().showLoading();
}
void LayerPopupChinhSach::onBtnTyLePhe(Ref* pSender){
    this->btnTyLeCellCard->setEnabled(true);
    this->btnTyLeCellCard->setBright(true);
    this->btnTyLeRut->setEnabled(true);
    this->btnTyLeRut->setBright(true);
    this->btnTyLePhe->setEnabled(false);
    this->btnTyLePhe->setBright(false);
    this->btnQuyenLoiVip->setEnabled(true);
    this->btnQuyenLoiVip->setBright(true);
    this->btnTyLeNap->setEnabled(true);
    this->btnTyLeNap->setBright(true);
    
    string url = "";
    url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+"ty_le_phe.html";
    //    }
    this->webview->setWebView(url);
    
    SceneManager::getSingleton().showLoading();
}
void LayerPopupChinhSach::onBtnTyLeNap(Ref* pSender){
    this->btnTyLeCellCard->setEnabled(true);
    this->btnTyLeCellCard->setBright(true);
    this->btnTyLeRut->setEnabled(true);
    this->btnTyLeRut->setBright(true);
    this->btnTyLePhe->setEnabled(true);
    this->btnTyLePhe->setBright(true);

    this->btnQuyenLoiVip->setEnabled(true);
    this->btnQuyenLoiVip->setBright(true);

    this->btnTyLeNap->setEnabled(false);
    this->btnTyLeNap->setBright(false);
    
    string url = "";
    url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+"ty_le_nap.html";
    //    }
    this->webview->setWebView(url);
    
    SceneManager::getSingleton().showLoading();
}
void LayerPopupChinhSach::onBtnTyLeRut(Ref* pSender){
    this->btnTyLeRut->setEnabled(false);
    this->btnTyLeRut->setBright(false);
    this->btnTyLeCellCard->setEnabled(true);
    this->btnTyLeCellCard->setBright(true);
    this->btnQuyenLoiVip->setEnabled(true);
    this->btnQuyenLoiVip->setBright(true);
    this->btnTyLePhe->setEnabled(true);
    this->btnTyLePhe->setBright(true);
    this->btnTyLeNap->setEnabled(true);
    this->btnTyLeNap->setBright(true);
    
    string url = "";
    url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+"ty_le_rut.html";
    //    }
    this->webview->setWebView(url);
    SceneManager::getSingleton().showLoading();
}
void LayerPopupChinhSach::onBtnQuyenLoiVip(Ref* pSender){
    this->btnTyLeRut->setEnabled(true);
    this->btnTyLeRut->setBright(true);
    this->btnTyLePhe->setEnabled(true);
    this->btnTyLePhe->setBright(true);
    this->btnQuyenLoiVip->setEnabled(false);
    this->btnQuyenLoiVip->setBright(false);
    this->btnTyLeCellCard->setEnabled(true);
    this->btnTyLeCellCard->setBright(true);
    this->btnTyLeNap->setEnabled(true);
    this->btnTyLeNap->setBright(true);
    string url = "";
    url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+"chinh_sach_vip.html";
    //    }
    this->webview->setWebView(url);
    SceneManager::getSingleton().showLoading();
}
void LayerPopupChinhSach::hideLayer(){
    this->setVisible(false);
}
void LayerPopupChinhSach::showLayer(){
    this->setVisible(true);
}
bool LayerPopupChinhSach::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerPopupChinhSach::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerPopupChinhSach::onEnter()
{
    Layer::onEnter();
    SceneManager::getSingleton().numPopup+=1;

    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupChinhSach::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupChinhSach::onExit()
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

