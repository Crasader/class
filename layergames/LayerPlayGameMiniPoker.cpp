//
//  LayerPlayGameMiniPoker.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 8/31/16.
//
//

#include "LayerPlayGameMiniPoker.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"
#include "_Chat_.h"
#include "TaiXiuMessageDefs.h"
#include "../layers/LayerPopupThongBao.h"
#include "../layers/LayerGuideInGame.h"
#include "LayerXepHangMiniPoker.h"
#include "LayerLichSuMiniPoker.h"
#include "../scenes/ScenePickRoom.h"
#include "../layers/LayerPopupReconnect.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerPlayGameMiniPoker::LayerPlayGameMiniPoker()
{
    
}


LayerPlayGameMiniPoker::~LayerPlayGameMiniPoker()
{
    
}

bool LayerPlayGameMiniPoker::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPlayMiniPoker.csb");
    
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
    //rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    //auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    //pnlB->setSwallowTouches(false);

    //pnlB->setContentSize(Size(visibleSize.width, visibleSize.height));
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
//    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }
//    else{
//        sizeAdd = Size(0, 0);
//    }
    Size sizeAdd2 = Size(0,0);
    //rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    
    this->mQueueMsg = new ThreadQueueMsg();
    SceneManager::getSingleton().showLoading();

    pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    pnlBg->setSwallowTouches(false);
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    if (lblTitle)
        lblTitle->setString(dataManager.GetSysString(855));
    
    auto Image_1 = static_cast<ImageView*>(pnlBg->getChildByName("Image_1"));
    Image_1->loadTexture(StringUtils::format("%s%sbg.png",PATH_PK.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    this->btnPlay = static_cast<Button*>(pnlBg->getChildByName("btnPlay"));
    if (this->btnPlay) {
        this->btnPlay->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnPlay, this));
    }
    Button* btnClose = static_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if (btnClose) {
        btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnClose, this));
        btnClose->setPressedActionEnabled(true);
    }
    
    Button* btnXepHang = static_cast<Button*>(pnlBg->getChildByName("btnXepHang"));
    if (btnXepHang) {
        btnXepHang->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnXepHang, this));
        btnXepHang->setPressedActionEnabled(true);
    }
    
    Button* btnInfo = static_cast<Button*>(pnlBg->getChildByName("btnInfo"));
    if (btnInfo) {
        btnInfo->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnLichSu, this));
        btnInfo->setPressedActionEnabled(true);
    }
    
    Button* btnHuongDan = static_cast<Button*>(pnlBg->getChildByName("btnHuongDan"));
    if (btnHuongDan) {
        btnHuongDan->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnHuongDan, this));
        btnHuongDan->setPressedActionEnabled(true);
    }
    
    Button* btnSuKien = static_cast<Button*>(pnlBg->getChildByName("btnSuKien"));
    if (btnSuKien) {
        btnSuKien->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnSuKien, this));
        btnSuKien->setPressedActionEnabled(true);
    }
    
    btnAutoPlay = static_cast<Button*>(pnlBg->getChildByName("btnAutoPlay"));
    if (btnAutoPlay) {
        btnAutoPlay->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnAutoPlay, this));
        // btnAutoPlay->setPressedActionEnabled(true);
        btnAutoPlay->setEnabled(false);
    }
    
    
//    this->txtMyMoney = static_cast<Text*>(pnlBg->getChildByName("txtMyMoney"));
//    this->txtMyMoney->setString("");
    
    this->imgResult = static_cast<ImageView*>(pnlBg->getChildByName("imgResult"));
    this->imgResult->setZOrder(100000);
    this->imgResult->setVisible(false);
    
    

    
    
    this->btn100 = static_cast<Button*>(pnlBg->getChildByName("btnBet100"));
    if (this->btn100) {
        this->btn100->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnBet100, this));
        // this->btn100->setPressedActionEnabled(true);
    }
    this->btn1k = static_cast<Button*>(pnlBg->getChildByName("btn1k"));
    if (this->btn1k) {
        this->btn1k->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnBet1K, this));
        //this->btn100->setPressedActionEnabled(true);
    }
    this->btn10k = static_cast<Button*>(pnlBg->getChildByName("btn10k"));
    if (this->btn10k) {
        this->btn10k->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnBet10K, this));
        //this->btn10k->setPressedActionEnabled(true);
    }
    
    this->txtTongHu = static_cast<Text*>(pnlBg->getChildByName("txtTongHu"));
    this->txtTongHu->setString("");
    
    this->txtPhien = static_cast<Text*>(pnlBg->getChildByName("txtPhien"));
    this->txtPhien->setString("");
    
    this->imgAutoPlay = static_cast<ImageView*>(pnlBg->getChildByName("imgAutoPlay"));
    
    this->imgAnimMachine = static_cast<ImageView*>(pnlBg->getChildByName("imgAnimMachine"));
    this->imgAnimPress = static_cast<ImageView*>(pnlBg->getChildByName("imgAnimPress"));
    this->imgAnimPress->setVisible(false);

    this->sprRob = Sprite::create("1_play.png");
    this->sprRob->setPosition(this->imgAnimPress->getPosition());
    this->sprRob->setAnchorPoint(this->imgAnimPress->getAnchorPoint());
    if (SceneManager::getSingleton().isNagaWin){
        this->sprRob->setPosition(Vec2(0,238));
        this->sprRob->setAnchorPoint(Vec2(0,0.5));
        this->imgAnimPress->addChild(this->sprRob,0,0);
        this->imgAnimPress->setVisible(true);

    }else{
        pnlBg->addChild(this->sprRob);
    }
    
    this->imgAnimMachine->setVisible(false);
    this->sprDown = Sprite::create("1_down.png");
    this->sprDown->setPosition(this->imgAnimMachine->getPosition());
    this->sprDown->setAnchorPoint(this->imgAnimMachine->getAnchorPoint());
    pnlBg->addChild(this->sprDown);
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
    {
        ImageView* img5 = static_cast<ImageView*>(pnlBg->getChildByName("img5"));
        img5->setVisible(false);
        this->round5 = RoundLayer::create();
        this->round5->setTag(11);
        this->round5->setPosition(Vec2(img5->getPositionX(), img5->getPositionY()+7 ));
        pnlBg->addChild(this->round5, 3);
        
        ImageView* img1 = static_cast<ImageView*>(pnlBg->getChildByName("img1"));
        img1->setVisible(false);
        this->round1 = RoundLayer::create();
        this->round1->setTag(11);
        this->round1->setPosition(Vec2(img1->getPositionX() , img1->getPositionY() +7));
        pnlBg->addChild(this->round1, 3);
        
        ImageView* img2 = static_cast<ImageView*>(pnlBg->getChildByName("img2"));
        img2->setVisible(false);
        this->round2 = RoundLayer::create();
        this->round2->setTag(11);
        this->round2->setPosition(Vec2(img2->getPositionX() , img2->getPositionY()+7 ));
        pnlBg->addChild(this->round2, 3);
        
        ImageView* img3 = static_cast<ImageView*>(pnlBg->getChildByName("img3"));
        img3->setVisible(false);
        this->round3 = RoundLayer::create();
        this->round3->setTag(11);
        this->round3->setPosition(Vec2(img3->getPositionX() , img3->getPositionY()+7 ));
        pnlBg->addChild(this->round3, 3);
        
        ImageView* img4 = static_cast<ImageView*>(pnlBg->getChildByName("img4"));
        img4->setVisible(false);
        this->round4 = RoundLayer::create();
        this->round4->setTag(11);
        //this->round4->setStopCallBackFunc(this, this->stopRound4);
        this->round4->setPosition(Vec2(img4->getPositionX() , img4->getPositionY()+7));
        pnlBg->addChild(this->round4, 3);
        
        this->imgLucky1 = static_cast<ImageView*>(pnlBg->getChildByName("imgLucky1"));
        this->imgLucky2 = static_cast<ImageView*>(pnlBg->getChildByName("imgLucky2"));
        this->imgLucky3 = static_cast<ImageView*>(pnlBg->getChildByName("imgLucky3"));

    }else{
        ImageView* img5 = static_cast<ImageView*>(pnlBg->getChildByName("img5"));
        img5->setVisible(false);
        this->round5 = RoundLayer::create();
        this->round5->setTag(11);
        this->round5->setPosition(Vec2(img5->getPositionX() - 65, img5->getPositionY() - 85));
        pnlBg->addChild(this->round5, 3);
        
        ImageView* img1 = static_cast<ImageView*>(pnlBg->getChildByName("img1"));
        img1->setVisible(false);
        this->round1 = RoundLayer::create();
        this->round1->setTag(11);
        this->round1->setPosition(Vec2(img1->getPositionX() - 65, img1->getPositionY() - 85));
        pnlBg->addChild(this->round1, 3);
        
        ImageView* img2 = static_cast<ImageView*>(pnlBg->getChildByName("img2"));
        img2->setVisible(false);
        this->round2 = RoundLayer::create();
        this->round2->setTag(11);
        this->round2->setPosition(Vec2(img2->getPositionX() - 65, img2->getPositionY() - 85));
        pnlBg->addChild(this->round2, 3);
        
        ImageView* img3 = static_cast<ImageView*>(pnlBg->getChildByName("img3"));
        img3->setVisible(false);
        this->round3 = RoundLayer::create();
        this->round3->setTag(11);
        this->round3->setPosition(Vec2(img3->getPositionX() - 65, img3->getPositionY() - 85));
        pnlBg->addChild(this->round3, 3);
        
        ImageView* img4 = static_cast<ImageView*>(pnlBg->getChildByName("img4"));
        img4->setVisible(false);
        this->round4 = RoundLayer::create();
        this->round4->setTag(11);
        //this->round4->setStopCallBackFunc(this, this->stopRound4);
        this->round4->setPosition(Vec2(img4->getPositionX() - 65, img4->getPositionY() - 85));
        pnlBg->addChild(this->round4, 3);

    }
    
    this->playAnimationMachine();
    this->createAnimPlay();
    this->onBtnBet1K(this, ui::Widget::TouchEventType::ENDED);
    
    this->btnPlay->setEnabled(false);
    this->sprDown->setVisible(false);
    
    
    this->round1->setVisible(false);
    this->round2->setVisible(false);
    this->round3->setVisible(false);
    this->round4->setVisible(false);
    this->round5->setVisible(false);

    
    
    this->autoPlay = false;
    if (this->autoPlay)
        this->imgAutoPlay->setVisible(true);
    else
        this->imgAutoPlay->setVisible(false);
    
  
    this->mQueueMsg->setFunCallBack(this, callfunc_selector(LayerPlayGameMiniPoker::OnExtensionResponseProcess));
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameMiniPoker::updateEvent),this,1/60, false);
    
    GameServer::getSingleton().addListeners(this);

    
    this->getMinMoney();
    this->getHuThuong(0);

    
    this->btnGold = static_cast<Button*>(pnlBg->getChildByName("btnGold"));
    if (this->btnGold){
        this->btnGold->setPressedActionEnabled(true);
        this->btnGold->addClickEventListener(CC_CALLBACK_1(LayerPlayGameMiniPoker::onBtnGold, this));
    }
    this->btnSilver = static_cast<Button*>(pnlBg->getChildByName("btnSilver"));
    if (this->btnSilver){
        this->btnSilver->setPressedActionEnabled(true);
        this->btnSilver->setVisible(false);
        this->btnSilver->addClickEventListener(CC_CALLBACK_1(LayerPlayGameMiniPoker::onBtnSilver, this));
    }
    
    imgBonusQT = static_cast<ImageView*>(pnlBg->getChildByName("imgBonusQT"));
    imgBonusQT->setVisible(false);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt("tm",0);
    boost::shared_ptr<IRequest> req(new ExtensionRequest("pkj", params));
    GameServer::getSingleton().Send(req);
    
    
    
//    //Cheat
    //txtNhanQT = static_cast<TextField*>(pnlBg->getChildByName("txtNhanQT"));
        txtKetqua = static_cast<TextField*>(pnlBg->getChildByName("txtKetqua"));
   // txtQT = static_cast<TextField*>(pnlBg->getChildByName("txtQT"));
    

    
    Button* btnKetQua = static_cast<Button*>(pnlBg->getChildByName("btnDoiTien"));
    if (btnKetQua) {
        btnKetQua->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnKetQua, this));
        //this->btn10k->setPressedActionEnabled(true);
    }
//    Button* btnNhanQT = static_cast<Button*>(pnlBg->getChildByName("btnNhanQT"));
//    if (btnNhanQT) {
//        btnNhanQT->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnNhanQT, this));
//        //this->btn10k->setPressedActionEnabled(true);
//    }
////
//    Button* btnSetQT = static_cast<Button*>(pnlBg->getChildByName("btnSetQT"));
//    if (btnSetQT) {
//        btnSetQT->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnSetQT, this));
//        //this->btn10k->setPressedActionEnabled(true);
//    }
   // this->setScale(0.8);
        SceneManager::getSingleton().currRoomTypePK = 0;
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }
    this->fakeHuPK = SceneManager::getSingleton().huPoker;
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameMiniPoker::tangHuPK),this,1/60, false);

    return true;
}
void LayerPlayGameMiniPoker::tangHuPK(float dt){
//    static int count = 0;
//    static int count2 = 0;
//    static int step = 37;
//    this->fakeHuPK+=step;
//    count+=37;
//    if (count>12443){
//        count2+=34;
//        step= 0;
//        if (count2>6244){
//            step = 37;
//            count2 = 0 ;
//            count = 0;
//
//        }
//
//    }
    this->txtTongHu->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().huPoker));
    
//    if (this->fakeHuPK > this->huPK){
//        this->fakeHuPK = 683471;
//        this->txtTongHu->setString(mUtils::convertMoneyEx(this->fakeHuPK));
//    }
//     = this->fakeHuPK;

}
void LayerPlayGameMiniPoker::onBtnGold(Ref *sender){
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(effect);

    if (this->autoPlay){
        auto toast = Chat::create(dataManager.GetSysString(420),-1);
        this->addChild(toast);
        return;
    }
    SceneManager::getSingleton().showLoading();
Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameMiniPoker::updateTimerStart),this);
//    this->btnGold->setEnabled(false);
//    this->btnGold->setBright(false);
//    
//    this->btnSilver->setEnabled(true);
//    this->btnSilver->setBright(true);
    
    this->btnSilver->setVisible(true);
    this->btnGold->setVisible(false);
    
    this->currHuThuong = 0;
    this->txtTongHu->setString(mUtils::convertMoneyEx(this->currHuThuong));
    
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req(new ExtensionRequest("pkl", params));
    GameServer::getSingleton().Send(req);
    this->resetGame();
    
}
void LayerPlayGameMiniPoker::onBtnSilver(Ref *sender){
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(effect);

    if (this->autoPlay){
        auto toast = Chat::create(dataManager.GetSysString(420),-1);
        this->addChild(toast);
        return;
    }
    SceneManager::getSingleton().showLoading();
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameMiniPoker::updateTimerStart),this);
//
//    this->btnGold->setEnabled(true);
//    this->btnGold->setBright(true);
//    
//    this->btnSilver->setEnabled(false);
//    this->btnSilver->setBright(false);
    this->btnSilver->setVisible(false);
    this->btnGold->setVisible(true);
    
    this->currHuThuong = 0;
    this->txtTongHu->setString(mUtils::convertMoneyEx(this->currHuThuong));
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req(new ExtensionRequest("pkl", params));
    GameServer::getSingleton().Send(req);
    this->resetGame();
    
}
////Cheat
void LayerPlayGameMiniPoker::onBtnSetQT(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        
        string strCheat = this->txtQT->getString();
        if (strCheat == "" || atoi(strCheat.c_str()) > 10000000)
            return;        
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt(EXT_FIELD_GAME_BET_VALUE, atoi(strCheat.c_str()));
        params->PutInt("gbr", this->currBetMoney);
        boost::shared_ptr<IRequest> req(new ExtensionRequest("pksmqtreq", params));
        GameServer::getSingleton().Send(req);
    }
}
//void LayerPlayGameMiniPoker::onBtnNhanQT(Ref *sender, ui::Widget::TouchEventType pType){
//    if (pType == ui::Widget::TouchEventType::ENDED) {
//        int xqt = 0;
//        bool isXQT = false;
//        xqt = atoi(txtNhanQT->getString().c_str());
//        if (xqt == 2 || xqt == 3 || xqt == 4){
//            boost::shared_ptr<ISFSObject> params(new SFSObject());
//            params->PutBool("ibqt", true);
//            params->PutInt(EXT_FIELD_GAME_BET_VALUE, xqt);
//            boost::shared_ptr<IRequest> req(new ExtensionRequest("pkcqtreq", params));
//            GameServer::getSingleton().Send(req);
//        }
//        else if (xqt == 0){
//            boost::shared_ptr<ISFSObject> params(new SFSObject());
//            params->PutBool("ibqt", false);
//            params->PutInt(EXT_FIELD_GAME_BET_VALUE, 3);
//            boost::shared_ptr<IRequest> req(new ExtensionRequest("pkcqtreq", params));
//            GameServer::getSingleton().Send(req);
//        }
//    }
//}
void LayerPlayGameMiniPoker::onBtnKetQua(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
    
        string strCheat = this->txtKetqua->getString();
        if (strCheat == "")
            return;
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("glstc", strCheat);
        params->PutUtfString("uid", mySelf->Name());
        boost::shared_ptr<IRequest> req(new ExtensionRequest("pkctreq", params));
        GameServer::getSingleton().Send(req);
    }
}
//
void LayerPlayGameMiniPoker::getHuThuong(float dt){
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt(EXT_FIELD_GAME_BET_VALUE,this->currBetMoney);
    boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_PK_GET_QUY_THUONG_REQ, params));
    GameServer::getSingleton().Send(req);
}
void LayerPlayGameMiniPoker::event_EXT_EVENT_GAME_STATE(){
    SceneManager::getSingleton().hideLoading();
    boost::shared_ptr<long> phien = this->param->GetInt("mid");
    boost::shared_ptr<long> number = this->param->GetInt(EXT_FIELD_GAME_AUTO_AMOUNT);
    boost::shared_ptr<long> gbv = this->param->GetInt("gbv");
    boost::shared_ptr<string> ghs = this->param->GetUtfString("ghs");
    boost::shared_ptr<bool> isBonusQT = this->param->GetBool("ibqt");
    boost::shared_ptr<long> bonusQTValue = this->param->GetInt(EXT_FIELD_GAME_BET_VALUE);
    boost::shared_ptr<string> lst7lucky = this->param->GetUtfString("lst7");

    this->getHuThuong(0);
    if (this->currTypeMoney == 0){
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        
        if (mySelf != NULL){
            if (mySelf->GetVariable("amf") != NULL) {
                auto myMoney = *mySelf->GetVariable("amf")->GetDoubleValue();
               // this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
                this->currMyMoney = myMoney;
            }
        }
    }else{
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        
        if (mySelf != NULL){
            if (mySelf->GetVariable("amfs") != NULL) {
                auto myMoney = *mySelf->GetVariable("amfs")->GetDoubleValue();
                //this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
                this->currMyMoney = myMoney;
            }
        }
    }
    if (gbv != NULL){
        int a = *gbv;
        switch (a){
            case 100:
                this->onBtnBet100(this,ui::Widget::TouchEventType::ENDED);
                break;
            case 1000:
                this->onBtnBet1K(this,ui::Widget::TouchEventType::ENDED);
                break;
            case 10000:
                this->onBtnBet10K(this,ui::Widget::TouchEventType::ENDED);
                break;
        }
    }
    if (phien != NULL)
        this->txtPhien->setString(StringUtils::format("#%d",*phien));
    
    if (number != NULL){
        this->numberAuto = *number;
        if (this->numberAuto > 0){
            this->autoPlay = true;
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
        }
    }
    if (this->autoPlay)
        this->imgAutoPlay->setVisible(true);
    else
        this->imgAutoPlay->setVisible(false);
    this->btnAutoPlay->setEnabled(true);
    
    this->resetGame();
    
    if (ghs != NULL){
        if (0 == ghs->compare(""))
            this->showResultPrevMatch("1_4;7_2;3_1;4_4;10_2");
        else
            this->showResultPrevMatch(*ghs);
    }
    
    if (isBonusQT!=NULL){
        bool isQT = *isBonusQT;
        if (isQT){
            if (bonusQTValue!=NULL){
                int value = *bonusQTValue;
                imgBonusQT->setVisible(true);
                switch (value) {
                    case 2:
                        //
                        imgBonusQT->loadTexture("x2_qt_minpoker.png");
                        break;
                    case 3:
                        imgBonusQT->loadTexture("x3_qt_minpoker.png");
                        //
                        break;
                    case 4:
                        imgBonusQT->loadTexture("x4_qt_minpoker.png");
                        
                        //
                        break;
                    default:
                        break;
                }
            }
        }
        else{
            imgBonusQT->setVisible(false);
        }
    }
    
    if (lst7lucky != NULL){
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
        {
            string lucky = *lst7lucky;
            this->imgLucky1->loadTexture("777-normal.png");
            this->imgLucky2->loadTexture("777-normal.png");
            this->imgLucky3->loadTexture("777-normal.png");
            
            if (lucky.size() == 1) {
                this->imgLucky1->loadTexture("777-active.png");
            }else if (lucky.size() == 2) {
                this->imgLucky1->loadTexture("777-active.png");
                this->imgLucky2->loadTexture("777-active.png");
            }
        }
    }
    
    this->round1->setVisible(true);
    this->round2->setVisible(true);
    this->round3->setVisible(true);
    this->round4->setVisible(true);
    this->round5->setVisible(true);
    this->timerStart = 0;

}
void LayerPlayGameMiniPoker::showResultPrevMatch(string str){
    this->arrTagCard.clear();
    auto arrCard = mUtils::splitString(str,';');
    if (arrCard.size() < 5)
        return;
    auto k = arrCard.size();
    for (int i = 0; i < k; i++) {
        int tag = this->round1->getTagCardFromStr(arrCard[i]);
        this->arrTagCard.push_back(tag);
    }
    if (this->arrTagCard.size() < 5)
        return;

    this->round1->stopRound(this->arrTagCard[0]);
    this->round1->stopUpdate();
    this->round2->stopRound(this->arrTagCard[1]);
    this->round2->stopUpdate();
    this->round3->stopRound(this->arrTagCard[2]);
    this->round3->stopUpdate();
    this->round4->stopRound(this->arrTagCard[3]);
    this->round4->stopUpdate();
    this->round5->stopRound(this->arrTagCard[4]);
    this->round5->stopUpdate();
    
}
void LayerPlayGameMiniPoker::event_PK_GET_QUY_THUONG(){
    
    boost::shared_ptr<long> money = this->param->GetInt("gbv");
    if (money == NULL)
        return;
    if (this->currHuThuong < *money || this->currHuThuong > *money) {
        this->txtTongHu->setVisible(true);
        this->currHuThuong = *money;
//        if (this->isNoHu)
//            return;
        this->huPK = *money;
       // this->txtTongHu->setString(mUtils::convertMoneyEx(this->currHuThuong));
        //this->chenhlechHuThuong = money - this->currHuThuong;
        //Director::getInstance()->getScheduler()->schedule(this->updateMoneyHuThuong);
       // this->txtTongHu->runAction(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 1),NULL));
    }
}
void LayerPlayGameMiniPoker::event_EXT_EVENT_GAME_START() {
    //phien
    //this->timerStart = 0;
    boost::shared_ptr<string> phien = this->param->GetUtfString("mid");
    if (phien != NULL){
        string mid = *phien;
        this->txtPhien->setString(StringUtils::format("#%s",mid.c_str()));
    }
    //ket qua quay
    boost::shared_ptr<string> result = this->param->GetUtfString("glstc");
    if (result != NULL)
        this->strResult = *result;
    
    if (!this->Animating){
        this->startGame();
        auto toast = Chat::create(dataManager.GetSysString(420),-1);
        this->addChild(toast);
    }
}
void LayerPlayGameMiniPoker::startGame(){
    this->playAnimStartGame();
    this->btnPlay->setEnabled(false);
    this->sprDown->setVisible(false);
    this->Animating = true;
    this->CurrentGameState = true;
}
void LayerPlayGameMiniPoker::event_EXT_EVENT_GAME_RESULT() {
    this->timerStart = 0;

    boost::shared_ptr<long> winValue = this->param->GetInt("gwv");
    boost::shared_ptr<string> rc = this->param->GetUtfString("gbt");
    boost::shared_ptr<string> lst7lucky = this->param->GetUtfString("lst7");
    boost::shared_ptr<long> has7 = this->param->GetInt("has7");
    
    if (winValue!= NULL)
        this->winMoney = *winValue;
    if (rc != NULL)
        this->currCodeRS = atof((*rc).c_str());
    this->stopRound(this->strResult);
    if (lst7lucky != NULL){
        this->strlst7 = *lst7lucky;
    }
    if (has7 != NULL){
        if (*has7 == 1) {
            this->ishas7 = true;
        }
    }
}

void LayerPlayGameMiniPoker::updateEvent(float dt){
    if (this->mQueueMsg->isQueueEmpty())
        return;
//    if (this->Animating)
//        return;
    
    this->mQueueMsg->run();
}
void LayerPlayGameMiniPoker::onBtnClose(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
//        LayerGameTaiXiuMini* parent = (LayerGameTaiXiuMini*)this->getParent();
//        if (parent)
//            parent->imgBGTimer->setVisible(true);
        this->removeFromParentAndCleanup(true);
    }
}
void LayerPlayGameMiniPoker::onBtnPlay(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {

        this->getMinMoney();
        if (this->Animating){
            auto toast = Chat::create(dataManager.GetSysString(421),-1);
            this->addChild(toast);
            return;
        }
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }

        
        //this->showResult();
        if (this->currBetMoney > this->currMyMoney  ) {
            if (this->currTypeMoney == 0)
                SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
            else{
                SceneManager::getSingleton().showPopupNapTienGold(dataManager.GetSysString(978));

            }
            return;
        }
        if (SceneManager::getSingleton().getCurrRoomType() == this->currTypeMoney){
            if (this->currBetMoney > (this->currMyMoney - this->MinBetValue)){
                if (this->currTypeMoney == 0)
                    SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
                else{
                    SceneManager::getSingleton().showPopupNapTienGold(dataManager.GetSysString(978));
                    
                }
                return;
            }
        }
        this->timerStart = 0;

        this->currMyMoney -=this->currBetMoney;
        //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->currMyMoney));
        SceneManager::getSingleton().setLastGameID(129);

        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString(EXT_FIELD_GAME_BET_VALUE,StringUtils::format("%d",this->currBetMoney));
        params->PutUtfString(EXT_FIELD_GAME_BET_TYPE,StringUtils::format("%d",""));
        boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ_PKMINI, params));
        GameServer::getSingleton().Send(req);
        
        this->startGame();
        
        ///Cheat
        
//        string strCheat = "10_1;14_1;13_1;12_1;11_1";
//        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
//        
//        boost::shared_ptr<ISFSObject> paramscheat(new SFSObject());
//        paramscheat->PutUtfString("glstc", strCheat);
//        paramscheat->PutUtfString("uid", mySelf->Name());
//        boost::shared_ptr<IRequest> reqcheat(new ExtensionRequest("pkctreq", paramscheat));
//        GameServer::getSingleton().Send(reqcheat);
    }
}
void LayerPlayGameMiniPoker::onBtnSuKien(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        LayerPopupWebView* _layer = LayerPopupWebView::create();
        string url = LINK_EVENT_MINIPOKER;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
            url = SKTX_P2;
        _layer->setWebView(url);
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(_layer, 2000);
    }
}
void LayerPlayGameMiniPoker::onBtnHuongDan(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        LayerGuideInGame* layer = LayerGuideInGame::create();
        string url = GUIDE_LINK;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
            url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+GUIDE_P2_LINK;
        layer->setLink(StringUtils::format("%s%d", url.c_str(), 1381));
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(layer, 2000);
        layer->setTag(1504);
    }
}
void LayerPlayGameMiniPoker::onBtnLichSu(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
        LayerLichSuMiniPoker* lichsu = LayerLichSuMiniPoker::create();
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(lichsu);
        lichsu->setLocalZOrder(2000);
    }
}
void LayerPlayGameMiniPoker::onBtnXepHang(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
        LayerXepHangMiniPoker* xephang = LayerXepHangMiniPoker::create();
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(xephang);
        xephang->setLocalZOrder(2000);
    }
}
void LayerPlayGameMiniPoker::onBtnAutoPlay(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        
        
        if (this->Animating && !this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(421),-1);
            this->addChild(toast);
            return;
        }
        
        if (this->autoPlay){
            
            
            this->autoPlay = false;
            
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutUtfString(EXT_FIELD_GAME_BET_VALUE,StringUtils::format("%d",this->currBetMoney));
            params->PutInt(EXT_FIELD_GAME_IS_AUTO,0);
            boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_PK_AUTO_PLAY_REQ, params));
            GameServer::getSingleton().Send(req);

            auto toast = Chat::create(dataManager.GetSysString(422),-1);
            this->addChild(toast);
            
        }
        else{
            
            if (this->currBetMoney > this->currMyMoney ) {
                if (this->currTypeMoney == 0)
                    SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
                else{
                    SceneManager::getSingleton().showPopupNapTienGold(dataManager.GetSysString(978));
                    
                }
                return;
            }
            if (SceneManager::getSingleton().getCurrRoomType() == this->currTypeMoney){
                if (this->currBetMoney > (this->currMyMoney - this->MinBetValue)){
                    if (this->currTypeMoney == 0)
                        SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
                    else{
                        SceneManager::getSingleton().showPopupNapTienGold(dataManager.GetSysString(978));
                        
                    }
                    return;
                }
            }

            this->timerStart = 0;

            this->autoPlay = true;
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutUtfString(EXT_FIELD_GAME_BET_VALUE,StringUtils::format("%d",this->currBetMoney));
            params->PutInt(EXT_FIELD_GAME_IS_AUTO,1);
            boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_PK_AUTO_PLAY_REQ, params));
            GameServer::getSingleton().Send(req);

        }
        if (this->autoPlay)
            this->imgAutoPlay->setVisible(true);
        else
            this->imgAutoPlay->setVisible(false);
        this->btnAutoPlay->setEnabled(false);
    }
}
void LayerPlayGameMiniPoker::onBtnBet100(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        
        if (this->Animating){
            auto toast = Chat::create(dataManager.GetSysString(421),-1);
            this->addChild(toast);
            return;
        }
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }

        
        //this->btn100->setBright(false);
        this->btn100->setEnabled(false);
        
        //this->btn1k->setBright(true);
        this->btn1k->setEnabled(true);
        
        //this->btn10k->setBright(true);
        this->btn10k->setEnabled(true);
        
        this->currBetMoney = 1000;
        this->currHuThuong = 0;
        this->txtTongHu->setString("0");
        this->getHuThuong(0);
    }
}
void LayerPlayGameMiniPoker::onBtnBet1K(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        
        if (this->Animating){
            auto toast = Chat::create(dataManager.GetSysString(421),-1);
            this->addChild(toast);
            return;
        }
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }

        
        //this->btn100->setBright(true);
        this->btn100->setEnabled(true);
        
        //this->btn1k->setBright(false);
        this->btn1k->setEnabled(false);
        
        //this->btn10k->setBright(true);
        this->btn10k->setEnabled(true);
        
        this->currBetMoney = 5000;
        this->txtTongHu->setString("0");
        this->currHuThuong = 0;
        this->getHuThuong(0);
        
    }
}
void LayerPlayGameMiniPoker::onBtnBet10K(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        
        if (this->Animating){
            auto toast = Chat::create(dataManager.GetSysString(421),-1);
            this->addChild(toast);
            return;
        }
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }

        
        //this->btn100->setBright(true);
        this->btn100->setEnabled(true);
        
        //this->btn1k->setBright(true);
        this->btn1k->setEnabled(true);
        
        //this->btn10k->setBright(false);
        this->btn10k->setEnabled(false);
        
        this->currBetMoney = 10000;
        this->txtTongHu->setString("0");
        this->getHuThuong(0);
        this->currHuThuong = 0;
        
    }
}
void LayerPlayGameMiniPoker::startRound() {
    if (mUtils::isSoundOn()){
        effect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("bm-slot.mp3");
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
    }
    this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::startRound1, this)), DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::startRound2, this)), DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::startRound3, this)), DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::startRound4, this)), DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::startRound5, this)),NULL));
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameMiniPoker::updateTimerStart),this,1, false);
}
void LayerPlayGameMiniPoker::updateTimerStart(float dt){
    timerStart+=1;
    
    if (timerStart>20){
        this->resetGame();
    
        auto toast = Chat::create(dataManager.GetSysString(49),-1);
        this->addChild(toast);
        this->showResultPrevMatch("1_4;7_2;3_1;4_4;10_2");
        
        auto _currScene = Director::getInstance()->getRunningScene();
        if (_currScene) {
            if (_currScene->getChildByTag(tag_SceneLogin) != NULL){
                return;
            }
        }
        if (_currScene->getChildByTag(10) != NULL)
            return;
        _currScene->removeChildByTag(1504);

        LayerPopupReconnect* layerReconn = LayerPopupReconnect::create();
        _currScene->addChild(layerReconn, ZORDER_LIST::ZORDER_POPUP_RECONNECT, 9010);
        layerReconn->showReconnect();
    }
}
void LayerPlayGameMiniPoker::stopRound(string str){
    
 
    
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameMiniPoker::updateTimerStart),this);

    this->arrTagCard.clear();
    auto arrCard = mUtils::splitString(str,';');
    if (arrCard.size() < 5)
        return;
    auto k = arrCard.size();
    for (int i = 0; i < k; i++) {
        int tag = this->round1->getTagCardFromStr(arrCard[i]);
        this->arrTagCard.push_back(tag);
    }
    if (this->arrTagCard.size() < 5)
        return;
   // log("result:%s",str.c_str());
    if (timerStart < 3){
    //this->round1->stopRound(this->arrTagCard[0]);
        this->runAction(Sequence::create(DelayTime::create(2-timerStart),CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound1,this,this->arrTagCard[0])), DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound2, this,this->arrTagCard[1])), DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound3, this,this->arrTagCard[2])), DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound4, this,this->arrTagCard[3])), DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound5, this,this->arrTagCard[4])), DelayTime::create(0.5),  CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::showResult, this)),NULL));
        
    }else{
        this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound1,this,this->arrTagCard[0])), DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound2, this,this->arrTagCard[1])), DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound3, this,this->arrTagCard[2])), DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound4, this,this->arrTagCard[3])), DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound5, this,this->arrTagCard[4])), DelayTime::create(0.5),  CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::showResult, this)),NULL));
    }
    timerStart = 0;
}
void LayerPlayGameMiniPoker::startRound1() {
    this->round1->startRound();
}
void LayerPlayGameMiniPoker::startRound2() {
    this->round2->startRound();
}
void LayerPlayGameMiniPoker::startRound3() {
    this->round3->startRound();
}
void LayerPlayGameMiniPoker::startRound4() {
    this->round4->startRound();
}
void LayerPlayGameMiniPoker::startRound5() {
    this->round5->startRound();
}
void LayerPlayGameMiniPoker::stopRound1(int tag) {
    this->round1->stopRound(tag);
    //sender.round2.stopRound(this->arrTagCard[1]);
}
void LayerPlayGameMiniPoker::stopRound2(int tag) {
    this->round2->stopRound(tag);
    // sender.round3.stopRound(this->arrTagCard[2]);
}
void LayerPlayGameMiniPoker::stopRound3(int tag) {
    this->round3->stopRound(tag);
    // sender.round4.stopRound(this->arrTagCard[3]);
}
void LayerPlayGameMiniPoker::stopRound4(int tag) {
    this->round4->stopRound(tag);
    //sender.round5.stopRound(this->arrTagCard[4]);
}
void LayerPlayGameMiniPoker::stopRound5(int tag) {
    this->round5->stopRound(tag);
    //if (this->winMoney > 0){
    //    this->showResult();
    //}
    if (mUtils::isSoundOn()){
        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(effect);
    }
}
void LayerPlayGameMiniPoker::showResult() {
    if (this->currCodeRS>0) {
        auto imgRS = ImageView::create(StringUtils::format("%s%s%s",PATH_PK.c_str(),SceneManager::getSingleton().getFolderLang().c_str(),this->getStrImageRS(this->currCodeRS).c_str()));
        imgRS->setPosition(this->imgResult->getPosition());
        imgRS->setAnchorPoint(this->imgResult->getAnchorPoint());
        imgRS->setTag(this->TAG_RESULT_IMG);
        pnlBg->addChild(imgRS,10000000);
        imgRS->runAction(Sequence::create(ScaleTo::create(0.5,1.3),ScaleTo::create(0.5,1),DelayTime::create(2),FadeOut::create(0.2),RemoveSelf::create(true), NULL));
    }
    
    

        string lucky = this->strlst7;
        this->imgLucky1->loadTexture("777-normal.png");
        this->imgLucky2->loadTexture("777-normal.png");
        this->imgLucky3->loadTexture("777-normal.png");
        
        if (lucky.size() == 1) {
            this->imgLucky1->loadTexture("777-active.png");
        }else if (lucky.size() == 2) {
            this->imgLucky1->loadTexture("777-active.png");
            this->imgLucky2->loadTexture("777-active.png");
        }else if (lucky.size() == 3) {
            this->imgLucky1->loadTexture("777-active.png");
            this->imgLucky2->loadTexture("777-active.png");
            this->imgLucky3->loadTexture("777-active.png");
        }
//        if (this->ishas7){
//            Chat* toast = Chat::create("Chúc mừng bạn đã tích luỹ được 01 \"Số 7 May Mắn\"!",-1);
//            this->addChild(toast);
//            
//        }
        
    
    
    if (this->winMoney <= 0) {
        this->btnPlay->setEnabled(true);
        this->sprDown->setVisible(true);
        this->resetGame();
        return;
    }
    
    //string font_name = "font_win-font.fnt";
    string money = StringUtils::format("+%s",mUtils::convertMoneyEx(this->winMoney).c_str());
    Label* labelResult = Label::createWithSystemFont(money, "", 65);
    labelResult->setColor(Color3B::YELLOW);
    labelResult->setAnchorPoint(Vec2(0.5,0.5));
    
    labelResult->setPosition(Vec2(612,160));
    if (SceneManager::getSingleton().isNagaWin){
        labelResult->setPosition(Vec2(412,160));
        labelResult->setColor(Color3B::RED);

    }
    pnlBg->addChild(labelResult);
    labelResult->setTag(this->TAG_RESULT_MONEY);
    labelResult->runAction(Sequence::create(MoveBy::create(1,Vec2(0,50)),DelayTime::create(2),FadeOut::create(0.2),RemoveSelf::create(true), NULL));
    
    this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::resetGame, this)),NULL));
    //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->currMyMoney));
    if (this->isNoHu)
        this->txtTongHu->setString(mUtils::convertMoneyEx(this->currHuThuong));
    
    this->isNoHu = false;
    
    
}
string LayerPlayGameMiniPoker::getStrImageRS(int rsc) {
    switch (rsc) {
        case POKER_THUNG_PHA_SANH_LON:
            this->isNoHu = true;
            return "1_rs.png";
        case POKER_THUNG_PHA_SANH_BE:
            return "2_rs.png";
        case POKER_TU_QUY:
            return "3_rs.png";
        case POKER_CU_LU:
            return "4_rs.png";
        case POKER_THUNG:
            return "5_rs.png";
        case POKER_SANH:
            return "6_rs.png";
        case POKER_SAM:
            return "7_rs.png";
        case POKER_HAI_DOI:
            return "8_rs.png";
        case POKER_MOT_DOI_J:
            return "9_rs.png";
        case POKER_DOI_NHO:
            return "10_rs.png";
        case POKER_BA_SO_7:
            return "11_rs.png";
        case POKER_MOT_SO_7:
//            Chat* toast = Chat::create("Chúc mừng bạn đã tích luỹ được 01 \"Số 7 May Mắn\"!",-1);
//            this->addChild(toast);
            return "";
    };
}
void LayerPlayGameMiniPoker::playAnimationMachine() {
    // if (cc.animationCache._clear)
    // if (cc.animationCache._clear())
    //    cc.animationCache._clear();
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
    {
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 3; i++) {
            str = StringUtils::format("%d_down.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 122, 290));
            animFrames.pushBack(spriteFrame);
        }
        
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        Animate* animN = Animate::create(animation);
        this->sprDown->runAction(RepeatForever::create(animN));

    }else{
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 3; i++) {
            str = StringUtils::format("%d_down.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 35, 158));
            animFrames.pushBack(spriteFrame);
        }
        
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        Animate* animN = Animate::create(animation);
        this->sprDown->runAction(RepeatForever::create(animN));

    }
}
void LayerPlayGameMiniPoker::createAnimPlay() {
    // if (cc.animationCache._clear())
    //    cc.animationCache._clear();
    if (SceneManager::getSingleton().isNagaWin)
    {
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("%d_play.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 270, 476));
            animFrames.pushBack(spriteFrame);
        }
        this->anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        this->anim->retain();
    }else{
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 5; i++) {
            str = StringUtils::format("%d_play.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 122, 290));
            animFrames.pushBack(spriteFrame);
        }
        this->anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        this->anim->retain();
    }
    
   
}
void LayerPlayGameMiniPoker::getMinMoney(){
	if (SceneManager::getSingleton().getGameID() == kGameLieng || SceneManager::getSingleton().getGameID() == kGameXiTo || SceneManager::getSingleton().getGameID() == kGamePoker)
	{
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf)
		{
			boost::shared_ptr<UserVariable> gameChip_ptr = mySelf->GetVariable(EXT_FIELD_GAME_CHIP);
			if (gameChip_ptr)
			{
				this->MinBetValue = *gameChip_ptr->GetDoubleValue();
			}
		}
	}
	else{
		vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
		int gameID = SceneManager::getSingleton().getGameID();
		int bet_ratio = 1;
		for (int i = 0; i < arrBet.size(); ++i)
		{
			if (arrBet.at(i).gameid == gameID)
			{
				bet_ratio = arrBet.at(i).ratioBet;
				break;
			}
		}
		string money = "0";
		boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (room != NULL && room->GetVariable("params") != NULL) {
			std::string paramString = *room->GetVariable("params")->GetStringValue();
			std::vector<std::string> arrInfo = mUtils::splitString(paramString, '@');
			if (arrInfo.size() > 0) {
				money = arrInfo.at(0);
				if (SceneManager::getSingleton().getGameID() == kGameTaiXiu || SceneManager::getSingleton().getGameID() == kGameXocDia)
					money = "0";
			}
		}
		this->MinBetValue = bet_ratio*(atoi(money.c_str()));
	}
}
void LayerPlayGameMiniPoker::playAnimStartGame() {
    // if (cc.animationCache._clear)
    
    this->sprRob->setAnchorPoint(Vec2(0, 0.5));
    Animate* animN = Animate::create(this->anim);
    this->sprRob->runAction(Sequence::create(animN,CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::startRound, this)),NULL));
}
void LayerPlayGameMiniPoker::resetGame() {
    this->stopAllActions();
    this->CurrentGameState = false;
    this->strResult = "";
    this->winMoney = 0;
    this->arrTagCard.clear();
    this->currCodeRS = 0;
    this->imgResult->setVisible(false);
    this->btnPlay->setEnabled(true);
    this->sprDown->setVisible(true);
    this->Animating = false;
    this->removeChildByTag(this->TAG_RESULT_IMG);
    this->removeChildByTag(this->TAG_RESULT_MONEY);
    this->ishas7 = false;
    
}
bool LayerPlayGameMiniPoker::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto rect = this->pnlBg->getBoundingBox();
    auto locationInNode = pTouch->getLocation();
    if (rect.containsPoint(locationInNode)){
        return true;
    }
    //var s = target.getContentSize();
    return false;
}
//Listen Server Event
void LayerPlayGameMiniPoker::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF_MINIPOKER)) {
        this->param = pObjs;
        this->event_EXT_EVENT_GAME_STATE();
        this->Animating = false;
        return;
    }
    if (0 == cmd->compare("pke_bntf")) {
        this->param = pObjs;
        boost::shared_ptr<long> errc = this->param->GetInt("errc");
       // log("errc minipoker :%d",*errc);
        string result = "2_1;5_4;10_2;14_1;5_3";
        this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create(CC_CALLBACK_0(LayerPlayGameMiniPoker::stopRound,this,result)),NULL));
        auto toast = Chat::create(dataManager.GetSysString(423),-1);
        this->addChild(toast);
        return;
    }
    if (0 == cmd->compare(EXT_EVENT_PK_CONFIG_QUY_THUONG_NTF)) {
        this->param = pObjs;
        boost::shared_ptr<bool> isBonusQT = this->param->GetBool("ibqt");
        boost::shared_ptr<long> bonusQTValue = this->param->GetInt(EXT_FIELD_GAME_BET_VALUE);
        
        if (isBonusQT!=NULL){
            bool isQT = *isBonusQT;
            if (isQT){
                if (bonusQTValue!=NULL){
                    int value = *bonusQTValue;
                    imgBonusQT->setVisible(true);
                    switch (value) {
                        case 2:
                            //
                            imgBonusQT->loadTexture("x2_qt_minpoker.png");
                            break;
                        case 3:
                            imgBonusQT->loadTexture("x3_qt_minpoker.png");
                            //
                            break;
                        case 4:
                            imgBonusQT->loadTexture("x4_qt_minpoker.png");
                            
                            //
                            break;
                        default:
                            break;
                    }
                }
            }
            else{
                imgBonusQT->setVisible(false);
            }
        }
        return;
    }
    if (0 != cmd->compare(EXT_EVENT_PK_GET_QUY_THUONG_RESP)
        && 0 != cmd->compare(EXT_EVENT_GAME_START_PKMINI)
        && 0 != cmd->compare(EXT_EVENT_GAME_RESULT_PKMINI)
        && 0 != cmd->compare(EXT_EVENT_GAME_END_PKMINI)
        && 0 != cmd->compare(EXT_EVENT_PK_AUTO_PLAY_RESP)
        && 0 != cmd->compare("pklrs")){
        return;
    }
    
    pObjs->PutUtfString("_cmd_", *cmd);
    this->mQueueMsg->addQueue(pObjs);
}

void LayerPlayGameMiniPoker::OnExtensionResponseProcess(){
    this->param = this->mQueueMsg->frontQueue();
    boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");

    if (0 == cmd->compare(EXT_EVENT_GAME_START_PKMINI)) {
        this->event_EXT_EVENT_GAME_START();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT_PKMINI)) {
        this->event_EXT_EVENT_GAME_RESULT();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_END_PKMINI)){
        this->event_EXT_EVENT_GAME_END();
    }
    else if (0 == cmd->compare(EXT_EVENT_PK_AUTO_PLAY_RESP)){
        this->event_EXT_EVENT_GAME_AUTO_RESP();
    }
    else if (0 == cmd->compare("pklrs")){
        if (this->currTypeMoney == 0){
            //SceneManager::getSingleton().hideLoading();

            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutInt("tm",1);
            boost::shared_ptr<IRequest> req(new ExtensionRequest("pkj", params));
            GameServer::getSingleton().Send(req);
            this->currTypeMoney  = 1;
            SceneManager::getSingleton().currRoomTypePK = 1;
        }else{
           // SceneManager::getSingleton().hideLoading();

            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutInt("tm",0);
            boost::shared_ptr<IRequest> req(new ExtensionRequest("pkj", params));
            GameServer::getSingleton().Send(req);
            
            this->currTypeMoney  = 0;
            SceneManager::getSingleton().currRoomTypePK = 0;
            
        }
    }
    this->mQueueMsg->deQueue();
}

void LayerPlayGameMiniPoker::event_EXT_EVENT_GAME_END(){
    boost::shared_ptr<long> number = this->param->GetInt(EXT_FIELD_GAME_AUTO_AMOUNT);
    if (number != NULL){
        this->numberAuto = *number;
        if (this->numberAuto > 0){
            this->autoPlay = true;
        }
        else{
            this->autoPlay = false;
            this->imgAutoPlay->setVisible(false);
        }
    }

}

void LayerPlayGameMiniPoker::event_EXT_EVENT_GAME_AUTO_RESP(){
    boost::shared_ptr<long> number = this->param->GetInt(EXT_FIELD_GAME_AUTO_AMOUNT);
    boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);

    if (number != NULL){
        this->numberAuto = *number;
        if (this->numberAuto > 0){
            this->autoPlay = true;
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
        }
        else{
            if (errc != NULL){
                int ec = *errc;
                if (ec == 1){
                    auto toast = Chat::create(dataManager.GetSysString(82),-1);
                    this->addChild(toast);
                }
            }
            this->autoPlay = false;
            this->imgAutoPlay->setVisible(false);
        }
    }
    this->btnAutoPlay->setEnabled(true);
}
void LayerPlayGameMiniPoker::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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
            // url
            
            boost::shared_ptr<UserVariable> aalptr = user->GetVariable("aal");
            if (this->currTypeMoney == 0){
                boost::shared_ptr<UserVariable> amfptr = user->GetVariable("amf");
                if (amfptr)
                {
                    boost::shared_ptr<double> amf = amfptr->GetDoubleValue();
                    if (amf)
                    {
                        this->currMyMoney = *amf;
                        string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
//                        if (!this->Animating && !this->CurrentGameState)
//                            this->txtMyMoney->setString(_money);
                        
                        //this->amfUser = this->currMyMoney;
                    }
                }

            }
            else{
                boost::shared_ptr<UserVariable> amfptr = user->GetVariable("amfs");
                if (amfptr)
                {
                    boost::shared_ptr<double> amf = amfptr->GetDoubleValue();
                    if (amf)
                    {
                        this->currMyMoney = *amf;
                        string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
//                        if (!this->Animating && !this->CurrentGameState)
//                            this->txtMyMoney->setString(_money);
                        
                        //this->amfUser = this->currMyMoney;
                    }
                }

            }
        }
    }
}
void LayerPlayGameMiniPoker::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    auto target = pnlBg;
    auto delta = pTouch->getDelta();
    //  if (target.x ==0 || target.x == WIDTH_DESIGN-143 || target.y ==0 || target.y == HEIGHT_DESIGN-149)
    //     return;
    auto x = target->getPositionX() + delta.x;
    auto y = target->getPositionY() + delta.y;
    
    this->pnlBg->setPosition(Vec2(x,y));
    //    if (abs(delta.x) > 10 || abs(delta.y) > 10)
    //        this->isTouchMove =true;
    //    //listener->setSwallowTouches(true);
}
void LayerPlayGameMiniPoker::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPlayGameMiniPoker::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerPlayGameMiniPoker::onTouchMoved, this);

    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton()->addListeners(this);
}
void LayerPlayGameMiniPoker::onExit()
{
    Layer::onExit();
    SceneManager::getSingleton().setIsShowPK(false);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(effect);

    //this->anim->release();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(listener);
    GameServer::getSingleton().removeListeners(this);
    SceneManager::getSingleton().setIsShowPK(false);
    if (!SceneManager::getSingleton().getIsShowTX() && !SceneManager::getSingleton().getIsShowXD()&& !SceneManager::getSingleton().getIsShowPK()&& !SceneManager::getSingleton().getIsShowVQ()&& !SceneManager::getSingleton().getIsShowCT()){
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return ;
        ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
        if (_scene)
        {
            _scene->showWebView();
        }
    }
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req(new ExtensionRequest("pkl", params));
    GameServer::getSingleton().Send(req);

    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameMiniPoker::updateEvent),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameMiniPoker::tangHuPK),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameMiniPoker::updateTimerStart),this);

}
