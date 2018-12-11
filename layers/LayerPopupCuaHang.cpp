//
//  LayerPopupCuaHang.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerPopupCuaHang.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/_Chat_.h"
#include "../scenes/ScenePickRoom.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "LayerPopupDoiMoc.h"
#include "LayerPopupChinhSach.h"

LayerPopupCuaHang::LayerPopupCuaHang()
{
}


LayerPopupCuaHang::~LayerPopupCuaHang()
{
    
}

bool LayerPopupCuaHang::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupCuaHang.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
 //   if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }
//    else{
//        sizeAdd = Size(0, 0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));

    auto btn_close = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if (btn_close != NULL){
        btn_close->addClickEventListener(CC_CALLBACK_1(LayerPopupCuaHang::onBtnClose, this));
        btn_close->setPressedActionEnabled(true);
    }
    
    this->btnNapThe = dynamic_cast<Button*>(pnlBg->getChildByName("btnNapThe"));
    if (this->btnNapThe != NULL){
        this->btnNapThe->setVisible(!dataManager.getAppConfig().isHideTheCao
);
        this->btnNapThe->addClickEventListener(CC_CALLBACK_1(LayerPopupCuaHang::onBtnNapThe, this));
        this->btnNapThe->setPressedActionEnabled(true);
        this->btnNapThe->setTitleText(dataManager.GetSysString(1029));
    }
    this->btnIAP = dynamic_cast<Button*>(pnlBg->getChildByName("btnIAP"));
    if (this->btnIAP != NULL){
        this->btnIAP->addClickEventListener(CC_CALLBACK_1(LayerPopupCuaHang::onBtnNapHao, this));
        this->btnIAP->setPressedActionEnabled(true);
        this->btnIAP->setTitleText(dataManager.GetSysString(805));

    }
    this->btnSMS = dynamic_cast<Button*>(pnlBg->getChildByName("btnSMS"));
    if (this->btnSMS != NULL){
//        //this->btnSMS->setVisible(!dataManager.getAppConfig().isHideSMS
//                                    );
        this->btnSMS->addClickEventListener(CC_CALLBACK_1(LayerPopupCuaHang::onBtnSMS, this));
        this->btnSMS->setPressedActionEnabled(true);
        this->btnSMS->setTitleText(dataManager.GetSysString(806));

    }
    
    this->btnChuyenMoc = dynamic_cast<Button*>(pnlBg->getChildByName("btnChuyenMoc"));
    if (this->btnChuyenMoc != NULL){
        this->btnChuyenMoc->addClickEventListener(CC_CALLBACK_1(LayerPopupCuaHang::onBtnChuyenMoc, this));
        this->btnChuyenMoc->setPressedActionEnabled(true);
        this->btnChuyenMoc->setTitleText(dataManager.GetSysString(807));

    }
    this->btnDoiMoc = dynamic_cast<Button*>(pnlBg->getChildByName("btnDoiMoc"));
    if (this->btnDoiMoc != NULL){
        this->btnDoiMoc->setVisible(!dataManager.getAppConfig().isHideExchange);
        this->btnDoiMoc->addClickEventListener(CC_CALLBACK_1(LayerPopupCuaHang::onBtnDoiMoc, this));
        this->btnDoiMoc->setPressedActionEnabled(true);
        this->btnDoiMoc->setTitleText(dataManager.GetSysString(808));


    }
    
    this->btnNapCellCard = dynamic_cast<Button*>(pnlBg->getChildByName("btnNapCellCard"));
    if (this->btnNapCellCard != NULL){
        this->btnNapCellCard->addClickEventListener(CC_CALLBACK_1(LayerPopupCuaHang::onBtnNapCellCard, this));
        this->btnNapCellCard->setPressedActionEnabled(true);
        this->btnNapCellCard->setTitleText(dataManager.GetSysString(1030));
        this->btnNapCellCard->setVisible(dataManager.getAppConfig().cellcard);
        
    }
    
    
    this->txtNhan = dynamic_cast<Text*>(pnlBg->getChildByName("txtNhan"));
    
    auto pnlContent = dynamic_cast<Layout*>(pnlBg->getChildByName("pnlContent"));
    
    this->napmoc = LayerNapZen::create();
    this->addChild(this->napmoc);
    this->napmoc->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    this->napmoc->hideLayer();
    
      
    this->napsms = LayerNapSMS::create();
    this->addChild(this->napsms);
    this->napsms->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    this->napsms->hideLayer();

    this->iap = LayerIAP2::create();
    this->iap->loadTyGia(0);
    this->addChild(this->iap);
    this->iap->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    this->iap->hideLayer();
    
    this->chuyenmoc = LayerChuyenMoc::create();
    this->addChild(this->chuyenmoc);
    this->chuyenmoc->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    this->chuyenmoc->hideLayer();
    
    this->napcellcard = LayerNapCellCard::create();
    this->addChild(this->napcellcard);
    this->napcellcard->setPosition(Vec2(pnlContent->getPositionX(),pnlContent->getPositionY()+sizeAdd.height));
    this->napcellcard->hideLayer();
    this->onBtnNapThe(NULL);
    
    auto btnChinhSach = dynamic_cast<Button*>(pnlBg->getChildByName("btnChinhSach"));
    if(btnChinhSach){
        //  this->btnDaiLy->setVisible(!dataManager.getAppConfig().isHideExchange);
        btnChinhSach->addClickEventListener(CC_CALLBACK_1(LayerPopupCuaHang::onBtnChinhSach, this));
        btnChinhSach->setTitleText(dataManager.GetSysString(975));
        btnChinhSach->setVisible(!dataManager.getAppConfig().isHideBilling);

    }
    if (SceneManager::getSingleton().isNagaWin){
        
        this->stickWing = dynamic_cast<ImageView*>(pnlBg->getChildByName("stickWing"));
        this->stickCellCard = dynamic_cast<ImageView*>(pnlBg->getChildByName("stickCellCard"));
        this->txtKhuyenMaiWing = dynamic_cast<Text*>(stickWing->getChildByName("txtKhuyenMain"));
        this->txtKhuyenMaiCellCard = dynamic_cast<Text*>(stickCellCard->getChildByName("txtKhuyenMain"));
        this->schedule(schedule_selector(LayerPopupCuaHang::showNapTien));
        this->stickWing->setVisible(false);
        this->stickCellCard->setVisible(false);
        this->stickWing = dynamic_cast<ImageView*>(pnlBg->getChildByName("stickWing"));
        this->stickCellCard = dynamic_cast<ImageView*>(pnlBg->getChildByName("stickCellCard"));
        this->txtKhuyenMaiWing = dynamic_cast<Text*>(stickWing->getChildByName("txtKhuyenMain"));
        this->txtKhuyenMaiCellCard = dynamic_cast<Text*>(stickCellCard->getChildByName("txtKhuyenMain"));

    }
    
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
//    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
//    if (_scene)
//    {
//        _scene->hideWebView();
//    }
    if (dataManager.getAppConfig().isHideBilling ){
        this->btnNapThe->setVisible(false);
        this->btnSMS->setVisible(false);
        this->btnIAP->setVisible(false);
        this->btnChuyenMoc->setVisible(false);
        this->btnDoiMoc->setVisible(false);
        this->btnNapCellCard->setVisible(false);
        this->onBtnNapHao(NULL);
    }
    if (dataManager.getAppConfig().isHideTheCao){
        this->onBtnNapHao(NULL);
    }
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));


    return true;
}
void LayerPopupCuaHang::showNapTien(float dt){
    if (!SceneManager::getSingleton().isLogin || dataManager.getAppConfig().isHideBilling){
        this->stickWing->setVisible(false);
        this->stickCellCard->setVisible(false);
        return;
    }
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    if (myself->GetVariable("am") == NULL) return;
    auto amf = myself->GetVariable("am")->GetDoubleValue();
    if (amf){
        int am = *amf;
        if (am <=0 && atoi(dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR.c_str()) > 0){
            this->stickWing->setVisible(true);
            this->stickCellCard->setVisible(true);
            this->txtKhuyenMaiWing->setString(dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR+"%");
            this->txtKhuyenMaiCellCard->setString(dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR+"%");

        }else{
            this->stickWing->setVisible(false);
            this->stickCellCard->setVisible(false);

        }
    }
}
void LayerPopupCuaHang::onBtnChinhSach(Ref* pSender){
    LayerPopupChinhSach* pop = LayerPopupChinhSach::create();
    auto currscene = Director::getInstance()->getRunningScene();
    if (currscene == NULL)
        return;
    currscene->addChild(pop,10000);
}
void LayerPopupCuaHang::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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
            if (SceneManager::getSingleton().getSoTienNap()>0){
                this->btnDoiMoc->setVisible(true);
            }
        }
    }

}
void LayerPopupCuaHang::onBtnNapCellCard(Ref* pSender){
    
    if (dataManager.getAppConfig().isHideBilling || dataManager.getAppConfig().isHideTheCao){
        this->onBtnNapHao(NULL);
        return;
    }
    
    this->btnNapCellCard->setEnabled(false);
    this->btnNapCellCard->setBright(false);
    this->napcellcard->showLayer();

    this->btnNapThe->setEnabled(true);
    this->btnNapThe->setBright(true);
    this->btnIAP->setEnabled(true);
    this->btnIAP->setBright(true);
    //    this->btnSMS->setEnabled(true);
    //    this->btnSMS->setBright(true);
    this->btnChuyenMoc->setEnabled(true);
    this->btnChuyenMoc->setBright(true);
    this->btnDoiMoc->setEnabled(true);
    this->btnDoiMoc->setBright(true);
    this->txtNhan->setString(dataManager.GetSysString(800));
    this->napmoc->hideLayer();
    this->napsms->hideLayer();
    this->chuyenmoc->hideLayer();
    this->iap->hideLayer();
    
    SceneManager::getSingleton().showLoading();
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    params->PutInt("cfk", 19);//CONFIG_KEY = "cfk"; 1: thuế chuyển tiền giữa các accounts
    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("reqgcf", params)); //REQUEST_GET_CONFIG = "reqgcf";
    GameServer::getSingleton().Send(request);
}
void LayerPopupCuaHang::onBtnNapThe(Ref* pSender){
    
    if (dataManager.getAppConfig().isHideBilling || dataManager.getAppConfig().isHideTheCao){
        this->onBtnNapHao(NULL);
        return;
    }
    
    this->btnNapCellCard->setEnabled(true);
    this->btnNapCellCard->setBright(true);
    this->napcellcard->hideLayer();
    
    this->btnNapThe->setEnabled(false);
    this->btnNapThe->setBright(false);
    this->btnIAP->setEnabled(true);
    this->btnIAP->setBright(true);
//    this->btnSMS->setEnabled(true);
//    this->btnSMS->setBright(true);
    this->btnChuyenMoc->setEnabled(true);
    this->btnChuyenMoc->setBright(true);
    this->btnDoiMoc->setEnabled(true);
    this->btnDoiMoc->setBright(true);
    this->txtNhan->setString(dataManager.GetSysString(800));
    this->napmoc->showLayer();

    this->napsms->hideLayer();
    this->chuyenmoc->hideLayer();
    this->iap->hideLayer();
    
    SceneManager::getSingleton().showLoading();
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    params->PutInt("cfk", 19);//CONFIG_KEY = "cfk"; 1: thuế chuyển tiền giữa các accounts
    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("reqgcf", params)); //REQUEST_GET_CONFIG = "reqgcf";
    GameServer::getSingleton().Send(request);
}
void LayerPopupCuaHang::onBtnNapHao(Ref* pSender){
    this->btnNapThe->setEnabled(true);
    this->btnNapThe->setBright(true);
    this->btnIAP->setEnabled(false);
    this->btnIAP->setBright(false);
//    this->btnSMS->setEnabled(true);
//    this->btnSMS->setBright(true);
    this->btnChuyenMoc->setEnabled(true);
    this->btnChuyenMoc->setBright(true);
    this->btnDoiMoc->setEnabled(true);
    this->btnDoiMoc->setBright(true);
    this->txtNhan->setString(dataManager.GetSysString(801));
    
    this->napmoc->hideLayer();
    this->napsms->hideLayer();
    this->chuyenmoc->hideLayer();
    this->iap->showLayer();
    this->btnNapCellCard->setEnabled(true);
    this->btnNapCellCard->setBright(true);
    this->napcellcard->hideLayer();

}
void LayerPopupCuaHang::onBtnSMS(Ref* pSender){
    this->btnNapCellCard->setEnabled(true);
    this->btnNapCellCard->setBright(true);
    this->napcellcard->hideLayer();
    this->btnNapThe->setEnabled(true);
    this->btnNapThe->setBright(true);
    this->btnIAP->setEnabled(true);
    this->btnIAP->setBright(true);
//    this->btnSMS->setEnabled(false);
//    this->btnSMS->setBright(false);
    this->btnChuyenMoc->setEnabled(true);
    this->btnChuyenMoc->setBright(true);
    this->btnDoiMoc->setEnabled(true);
    this->btnDoiMoc->setBright(true);
    this->txtNhan->setString(dataManager.GetSysString(802));
    
    this->napmoc->hideLayer();
    this->napsms->showLayer();
    this->chuyenmoc->hideLayer();
    this->iap->hideLayer();
    
    SceneManager::getSingleton().showLoading();
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    params->PutInt("cfk", 20);//CONFIG_KEY = "cfk"; 1: thuế chuyển tiền giữa các accounts
    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("reqgcf", params)); //REQUEST_GET_CONFIG = "reqgcf";
    GameServer::getSingleton().Send(request);

}
void LayerPopupCuaHang::onBtnChuyenMoc(Ref* pSender){
    if (dataManager.getAppConfig().isHideTransfer){
        auto toast = Chat::create(dataManager.GetSysString(456),-1);
        auto curentScene = Director::getInstance()->getRunningScene();
        curentScene->addChild(toast,10000);
        return;
    }
    this->btnNapCellCard->setEnabled(true);
    this->btnNapCellCard->setBright(true);
    this->napcellcard->hideLayer();
    this->btnNapThe->setEnabled(true);
    this->btnNapThe->setBright(true);
    this->btnIAP->setEnabled(true);
    this->btnIAP->setBright(true);
//    this->btnSMS->setEnabled(true);
//    this->btnSMS->setBright(true);
    this->btnChuyenMoc->setEnabled(false);
    this->btnChuyenMoc->setBright(false);
    this->btnDoiMoc->setEnabled(true);
    this->btnDoiMoc->setBright(true);
    this->txtNhan->setString(dataManager.GetSysString(803));
    
    this->napmoc->hideLayer();
    this->napsms->hideLayer();
    this->chuyenmoc->showLayer();
    this->iap->hideLayer();
}
void LayerPopupCuaHang::onBtnDoiMoc(Ref* pSender){

//    this->btnNapThe->setEnabled(true);
//    this->btnNapThe->setBright(true);
//    this->btnIAP->setEnabled(true);
//    this->btnIAP->setBright(true);
//    this->btnSMS->setEnabled(true);
//    this->btnSMS->setBright(true);
//    this->btnChuyenMoc->setEnabled(true);
//    this->btnChuyenMoc->setBright(true);
//    this->btnDoiMoc->setEnabled(false);
//    this->btnDoiMoc->setBright(false);
    this->txtNhan->setString(dataManager.GetSysString(596));
    
//    this->napmoc->hideLayer();
//    this->naphao->hideLayer();
//    this->napsms->hideLayer();
//    this->chuyenmoc->hideLayer();
   // this->removeFromParent();
    auto currscene = Director::getInstance()->getRunningScene();
    if (currscene == NULL)
        return;
    LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
    currscene->addChild(popup,2000);

}
void LayerPopupCuaHang::onBtnClose(Ref* pSender){
    
    this->removeFromParentAndCleanup(true);
}
bool LayerPopupCuaHang::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupCuaHang::onEnter()
{
    Layer::onEnter();
    SceneManager::getSingleton().numPopup+=1;

    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupCuaHang::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupCuaHang::onExit()
{
    Layer::onExit();
    SceneManager::getSingleton().numPopup-=1;
    this->unschedule(schedule_selector(LayerPopupCuaHang::showNapTien));

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

