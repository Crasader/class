//
//  LayerPopupDoiMoc.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerPopupDoiMoc.h"
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
#include "LayerPopupChinhSach.h"
LayerPopupDoiMoc::LayerPopupDoiMoc()
{
    GameServer::getSingleton().addListeners(this);

}


LayerPopupDoiMoc::~LayerPopupDoiMoc()
{
    GameServer::getSingleton().removeListeners(this);

}

bool LayerPopupDoiMoc::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupDoiMoc.csb");
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
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiMoc::onBtnClose, this));
    }

    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlContent = dynamic_cast<Layout*>(pnlBg->getChildByName("pnlContent"));
    
    this->muathe = LayerDoiWing::create();
    this->addChild(this->muathe);
    this->muathe->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    this->muathe->hideLayer();
    
    this->btnMuaMaThe = dynamic_cast<Button*>(pnlBg->getChildByName("btnMuaMaThe"));
    if(this->btnMuaMaThe ){
        this->btnMuaMaThe->setVisible(!dataManager.getAppConfig().isHideExchange);

        this->btnMuaMaThe->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiMoc::onBtnMaThe, this));
        this->btnMuaMaThe->setTitleText("WING");

    }
    
    this->txtNhan = dynamic_cast<Text*>(pnlBg->getChildByName("txtNhan"));
    this->txtNhan->setString(dataManager.GetSysString(596));

    this->naphao = LayerNapHao::create();
    this->addChild(this->naphao);
    this->naphao->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    this->naphao->hideLayer();

    this->iap2 = LayerIAP2::create();
    this->addChild(this->iap2);
    this->iap2->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    this->iap2->hideLayer();
    this->iap2->loadTyGia(1);
    
    this->doithe = LayerDoiThe::create();
    this->addChild(this->doithe);
    this->doithe->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    this->doithe->hideLayer();

    
    this->btnDaiLy = dynamic_cast<Button*>(pnlBg->getChildByName("btnDaiLy"));
    if(this->btnDaiLy ){
      //  this->btnDaiLy->setVisible(!dataManager.getAppConfig().isHideExchange);
        this->btnDaiLy->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiMoc::onBtnDaiLy, this));
        this->btnDaiLy->setTitleText(dataManager.GetSysString(832));


    }
    
    auto btnChinhSach = dynamic_cast<Button*>(pnlBg->getChildByName("btnChinhSach"));
    if(btnChinhSach){
        //  this->btnDaiLy->setVisible(!dataManager.getAppConfig().isHideExchange);
        btnChinhSach->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiMoc::onBtnChinhSach, this));
        btnChinhSach->setTitleText(dataManager.GetSysString(975));
        btnChinhSach->setVisible(!dataManager.getAppConfig().isHideBilling);

    }
    
    this->btnIAP = dynamic_cast<Button*>(pnlBg->getChildByName("btnIAP"));
    if(this->btnIAP){
        //  this->btnDaiLy->setVisible(!dataManager.getAppConfig().isHideExchange);
        this->btnIAP->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiMoc::onBtnIAP, this));
        this->btnIAP->setVisible(!dataManager.getAppConfig().isHideBilling);
    }
    
    this->btnDoiThe = dynamic_cast<Button*>(pnlBg->getChildByName("btnDoiThe"));
    if(this->btnDoiThe){
        this->btnDoiThe->setTitleText(dataManager.GetSysString(1040));
        //  this->btnDaiLy->setVisible(!dataManager.getAppConfig().isHideExchange);
        this->btnDoiThe->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiMoc::onBtnDoiThe, this));
        this->btnDoiThe->setVisible(!dataManager.getAppConfig().isHideBilling);
        this->btnDoiThe->setVisible(dataManager.getAppConfig().isDoiThe);
    }
    
    
    this->btnNhanThuong = dynamic_cast<Button*>(pnlBg->getChildByName("btnNhanThuong"));
    if(this->btnNhanThuong ){
        this->btnNhanThuong->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiMoc::onBtnNhanThuong, this));
        this->btnNhanThuong->setTitleText(dataManager.GetSysString(833));

    }
    this->btnNapHao = dynamic_cast<Button*>(pnlBg->getChildByName("btnNapHao"));
    if(this->btnNapHao ){
        this->btnNapHao->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiMoc::onBtnNapHao, this));
        this->btnNapHao->setTitleText(dataManager.GetSysString(834));

    }

    if (dataManager.getAppConfig().isHideBilling){
        this->btnNhanThuong->setVisible(false);
        this->btnNapHao->setVisible(false);
        this->btnDaiLy->setVisible(false);
        this->btnMuaMaThe->setVisible(false);
    }
    
        this->webview = LayerWebView::create();
        pnlBg->addChild(this->webview);
        this->webview->hideLayer();
        this->webview->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    
    this->onBtnMaThe(NULL);
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }
    //this->txtChuChay->setScale(5);
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
    return true;
}
void LayerPopupDoiMoc::onBtnDoiThe(Ref* pSender){
    this->btnMuaMaThe->setEnabled(true);
    this->btnMuaMaThe->setBright(true);
    
    this->btnDaiLy->setEnabled(true);
    this->btnDaiLy->setBright(true);
    
    this->btnNhanThuong->setEnabled(true);
    this->btnNhanThuong->setBright(true);
    
    this->btnIAP->setEnabled(true);
    this->btnIAP->setBright(true);
    
    this->btnNapHao->setEnabled(true);
    this->btnNapHao->setBright(true);
    
    this->btnDoiThe->setEnabled(false);
    this->btnDoiThe->setBright(false);
    this->doithe->showLayer();

    this->iap2->hideLayer();
    this->naphao->hideLayer();
    this->muathe->hideLayer();
    this->webview->hideLayer();
}
void LayerPopupDoiMoc::onBtnIAP(Ref* pSender){
    this->btnMuaMaThe->setEnabled(true);
    this->btnMuaMaThe->setBright(true);
    
    this->btnDaiLy->setEnabled(true);
    this->btnDaiLy->setBright(true);
    
    this->btnNhanThuong->setEnabled(true);
    this->btnNhanThuong->setBright(true);
    
    this->btnIAP->setEnabled(false);
    this->btnIAP->setBright(false);
    
    this->btnNapHao->setEnabled(true);
    this->btnNapHao->setBright(true);
    this->btnDoiThe->setEnabled(true);
    this->btnDoiThe->setBright(true);
    this->doithe->hideLayer();
    this->iap2->showLayer();
    this->naphao->hideLayer();
    this->muathe->hideLayer();
    this->webview->hideLayer();
}
void LayerPopupDoiMoc::onBtnChinhSach(Ref* pSender){
    LayerPopupChinhSach* pop = LayerPopupChinhSach::create();
    auto currscene = Director::getInstance()->getRunningScene();
    if (currscene == NULL)
        return;
    currscene->addChild(pop,10000);
}
void LayerPopupDoiMoc::onBtnNapHao(Ref* pSender){
    this->btnMuaMaThe->setEnabled(true);
    this->btnMuaMaThe->setBright(true);
    
    this->btnDaiLy->setEnabled(true);
    this->btnDaiLy->setBright(true);
    
    this->btnNhanThuong->setEnabled(true);
    this->btnNhanThuong->setBright(true);
    
    this->btnIAP->setEnabled(true);
    this->btnIAP->setBright(true);
    
    this->btnNapHao->setEnabled(false);
    this->btnNapHao->setBright(false);
    this->btnDoiThe->setEnabled(true);
    this->btnDoiThe->setBright(true);
    this->doithe->hideLayer();
    this->txtNhan->setString(dataManager.GetSysString(828));
    if (dataManager.getAppConfig().isHideBilling){
        this->txtNhan->setString(dataManager.GetSysString(829));
    }
    
    this->naphao->showLayer();
    this->muathe->hideLayer();
    this->webview->hideLayer();
    this->iap2->hideLayer();

    
    SceneManager::getSingleton().showLoading();
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    params->PutInt("cfk", 16);//CONFIG_KEY = "cfk"; 1: thuế chuyển tiền giữa các accounts
    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("reqgcf", params)); //REQUEST_GET_CONFIG = "reqgcf";
    GameServer::getSingleton().Send(request);
    
}
void LayerPopupDoiMoc::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    if (ptrEvetnParams == NULL)
        return;
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEvetnParams)["user"];
    boost::shared_ptr<User> user = ((boost::static_pointer_cast<User>(ptrEventParamValueUser)));
    //
    if (user != NULL)
    {
        string myName = SceneManager::getSingleton().getMyName();
        if (0 == myName.compare(*user->Name()))
        {
            if (SceneManager::getSingleton().getSoTienNap()>=3000000){
                this->btnDaiLy->setVisible(true);
            }
        }
    }
}
void LayerPopupDoiMoc::onBtnMaThe(Ref* pSender){
    if (dataManager.getAppConfig().isHideBilling){
        return;
    }
//    if (SceneManager::getSingleton().getSoTienNap() < dataManager.getAppConfig().minVipDoiMoc){
//        auto toast = Chat::create(StringUtils::format("%s %s ",dataManager.GetSysString(457).c_str(),formatMoneySymbol(dataManager.getAppConfig().minVipDoiMoc).c_str()),-1);
//        auto curentScene = Director::getInstance()->getRunningScene();
//        curentScene->addChild(toast,10000);
//        this->webview->hideLayer();
//        this->btnMuaMaThe->setEnabled(true);
//        this->btnMuaMaThe->setBright(true);
//        
//        this->btnDaiLy->setEnabled(true);
//        this->btnDaiLy->setBright(true);
//        
//        this->btnNhanThuong->setEnabled(true);
//        this->btnNhanThuong->setBright(true);
//        
//        this->btnNapHao->setEnabled(true);
//        this->btnNapHao->setBright(true);
//        return;
//    }
    this->btnMuaMaThe->setEnabled(false);
    this->btnMuaMaThe->setBright(false);
    
    this->btnDaiLy->setEnabled(true);
    this->btnDaiLy->setBright(true);
    
    this->btnNhanThuong->setEnabled(true);
    this->btnNhanThuong->setBright(true);
    
    this->btnNapHao->setEnabled(true);
    this->btnNapHao->setBright(true);
    
    this->btnIAP->setEnabled(true);
    this->btnIAP->setBright(true);
    
    this->txtNhan->setString(dataManager.GetSysString(596));
    this->btnDoiThe->setEnabled(true);
    this->btnDoiThe->setBright(true);
    this->doithe->hideLayer();
    this->muathe->showLayer();
    this->webview->hideLayer();
    this->naphao->hideLayer();
    this->iap2->hideLayer();



}
void LayerPopupDoiMoc::onBtnDaiLy(Ref* pSender){
    if (SceneManager::getSingleton().getSoTienNap() < dataManager.getAppConfig().minVipBanMoc){
        auto toast = Chat::create(StringUtils::format("%s %s",dataManager.GetSysString(457).c_str(),formatMoneySymbol(dataManager.getAppConfig().minVipBanMoc).c_str()),-1);
        auto curentScene = Director::getInstance()->getRunningScene();
        curentScene->addChild(toast,10000);
        this->webview->hideLayer();
        this->btnMuaMaThe->setEnabled(true);
        this->btnMuaMaThe->setBright(true);
        
        this->btnDaiLy->setEnabled(true);
        this->btnDaiLy->setBright(true);
        
        this->btnNhanThuong->setEnabled(true);
        this->btnNhanThuong->setBright(true);
        this->btnIAP->setEnabled(true);
        this->btnIAP->setBright(true);
        this->btnNapHao->setEnabled(true);
        this->btnNapHao->setBright(true);
        return;
    }
    this->removeFromParent();

    auto currscene = Director::getInstance()->getRunningScene();
    if (currscene == NULL)
        return;
    LayerPopupDaiLy* popup = LayerPopupDaiLy::create();
    currscene->addChild(popup,2000);

}
void LayerPopupDoiMoc::onBtnNhanThuong(Ref* pSender){
    this->btnMuaMaThe->setEnabled(true);
    this->btnMuaMaThe->setBright(true);
    
    this->btnDaiLy->setEnabled(true);
    this->btnDaiLy->setBright(true);
    
    this->btnNapHao->setEnabled(true);
    this->btnNapHao->setBright(true);
    this->btnIAP->setEnabled(true);
    this->btnIAP->setBright(true);
    this->txtNhan->setString(dataManager.GetSysString(833));

    
    this->btnNhanThuong->setEnabled(false);
    this->btnNhanThuong->setBright(false);
    this->muathe->hideLayer();
    this->webview->showLayer();
    this->naphao->hideLayer();
    this->iap2->hideLayer();
    this->btnDoiThe->setEnabled(true);
    this->btnDoiThe->setBright(true);
    this->doithe->hideLayer();
    string myName = SceneManager::getSingleton().getMyName();
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL || myself->Name() == NULL)
        return;
    string url = "";
//    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
//        url = LS_DOI_THE_P2;
//    }
//    else{
        url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+LS_DOI_THE_P2;
//    }
    url += myName + "&checksum=" + mUtils::getMD5KeyHis(myName);
    this->webview->setWebView(url);

}
void LayerPopupDoiMoc::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp("rg", cmd->c_str()) == 0){//OK
    }
}
void LayerPopupDoiMoc::hideLayer(){
    this->setVisible(false);
}
void LayerPopupDoiMoc::showLayer(){
    this->setVisible(true);
}
bool LayerPopupDoiMoc::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerPopupDoiMoc::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerPopupDoiMoc::onEnter()
{
    Layer::onEnter();
    SceneManager::getSingleton().numPopup+=1;

    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupDoiMoc::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupDoiMoc::onExit()
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

