//
//  LayerPlayGameSlotMachine.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 8/31/16.
//
//

#include "LayerPlaySlotMachine.h"
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
#include "Requests/LeaveRoomRequest.h"
#include "Requests/JoinRoomRequest.h"
#include "../layers/LayerPopupCuaHang.h"
#include "../layers/LayerPopupDoiMoc.h"
#include "../layers/LayerWinSlot.h"
#include "../cotuong/GameUtils.h"
#include "../scenes/SceneMain.h"
#include "../layers/LayerPopupReconnect.h"
#include "../layers/LayerBangXepHang.h"

USING_NS_CC;
using namespace cocos2d::ui;

LayerPlayGameSlotMachine::LayerPlayGameSlotMachine()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerPlayGameSlotMachine::~LayerPlayGameSlotMachine()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameSlotMachine::updateEvent),this);
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPlayGameSlotMachine::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerSlotMachine.csb");
    
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
    //
    
    this->colors = {
        Color3B(0,255,255),
        Color3B(0,255,0),
        Color3B(255,0,255),
        Color3B(255,13,19),
        Color3B(246,150,121),
        Color3B(255,242,0),
        Color3B(255,255,255),
        Color3B(176,0,241),
        Color3B(0,255,0),
        Color3B(255,242,0),
        Color3B(176,0,241),
        Color3B(246,150,121),
        Color3B(0,255,0),
        Color3B(255,13,19),
        Color3B(0,255,255),
        Color3B(255,0,255),
        Color3B(255,255,255)};
    
    ui::Helper::doLayout(rootNode);
    //auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    //pnlB->setSwallowTouches(false);
    auto bg = static_cast<ImageView*>(rootNode->getChildByName("bg"));
    bg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    bg->setPosition(Point(visibleSize / 2));
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
    
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    
    
    pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    
    //    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
    sizeAdd = ChanUtils::getSizePos();
    
    //    }
    //    else{
    //        sizeAdd = Size(0, 0);
    //    }
    Size sizeAdd2 = Size(0,0);
    pnlBg->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    
    pnlBg->setSwallowTouches(false);
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    this->currTypeMoney = SceneManager::getSingleton().getCurrRoomType();
    
    auto Image_2 = static_cast<ImageView*>(pnlBg->getChildByName("Image_2"));
    Image_2->loadTexture(StringUtils::format("%s%sbg-machine.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    this->imgMoneyTable = static_cast<ImageView*>(pnlBg->getChildByName("imgMoneyTable"));
    this->imgMoneyType = static_cast<ImageView*>(pnlBg->getChildByName("imgMoneyType"));
    
    if (this->currTypeMoney ==0 ){
        this->imgMoneyTable->loadTexture(PATH_SLOT+"dongmoc-min.png");
        this->imgMoneyType->loadTexture(PATH_SLOT+"dongmoc-min.png");
        
        
    }else{
        this->imgMoneyTable->loadTexture(PATH_SLOT+"donghao-min.png");
        this->imgMoneyType->loadTexture(PATH_SLOT+"donghao-min.png");
        
    }
    
    auto imgLine = static_cast<ImageView*>(pnlBg->getChildByName("imgLine"));
    imgLine->loadTexture(StringUtils::format("%s%sline-bg.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    this->btnPlay = static_cast<Button*>(pnlBg->getChildByName("btnSpin"));
    if (this->btnPlay) {
        this->btnPlay->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnPlay, this));
        this->btnPlay->loadTextures(StringUtils::format("%s%sspin-en.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()), StringUtils::format("%s%sspin-dis.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
        
    }
    Button* btnClose = static_cast<Button*>(pnlBg->getChildByName("btnExit"));
    if (btnClose) {
        btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnClose, this));
        btnClose->setPressedActionEnabled(true);
        btnClose->setPositionY(btnClose->getPositionY()+sizeAdd.height);
    }
    
    Button* btnHuongDan = static_cast<Button*>(pnlBg->getChildByName("btnHuongDan"));
    if (btnHuongDan) {
        btnHuongDan->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnHuongDan, this));
        btnHuongDan->setPressedActionEnabled(true);
        btnHuongDan->setPositionY(btnHuongDan->getPositionY()+sizeAdd.height);
    }
    
    Button* btnRecharge = static_cast<Button*>(pnlBg->getChildByName("btnRecharge"));
    if (btnRecharge) {
        btnRecharge->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnRecharge, this));
        btnRecharge->setPressedActionEnabled(true);
    }
    
    btnAutoPlay = static_cast<Button*>(pnlBg->getChildByName("btnTuQuay"));
    if (btnAutoPlay) {
        btnAutoPlay->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnAutoPlay, this));
        // btnAutoPlay->setPressedActionEnabled(true);
        btnAutoPlay->loadTextures(StringUtils::format("%s%sbtn-tuquay.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()), StringUtils::format("%s%sbtn-tuquay.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
        btnAutoPlay->setEnabled(false);
    }
    
    
    this->txtMyMoney = static_cast<Text*>(pnlBg->getChildByName("txtMyMoney"));
    this->txtMyMoney->setString("");
    
    
    
    
    
    
    //    this->txtTongHu = static_cast<Text*>(pnlBg->getChildByName("txtTongHu"));
    //    this->txtTongHu->setString("");
    //
    this->txtPhien = static_cast<Text*>(pnlBg->getChildByName("txtPhien"));
    this->txtPhien->setString("---");
    
    this->imgAutoPlay = static_cast<ImageView*>(pnlBg->getChildByName("imgTuQuay"));
    ImageView* img5 = static_cast<ImageView*>(pnlBg->getChildByName("imgSlot5"));
    img5->setVisible(false);
    this->round5 = RoundLayerSlot::create();
    this->round5->setTag(11);
    this->round5->setPosition(Vec2(img5->getPositionX(), img5->getPositionY()+7 ));
    pnlBg->addChild(this->round5, 3);
    
    ImageView* img1 = static_cast<ImageView*>(pnlBg->getChildByName("imgSlot1"));
    img1->setVisible(false);
    this->round1 = RoundLayerSlot::create();
    this->round1->setTag(11);
    this->round1->setPosition(Vec2(img1->getPositionX() , img1->getPositionY() +7));
    pnlBg->addChild(this->round1, 3);
    
    ImageView* img2 = static_cast<ImageView*>(pnlBg->getChildByName("imgSlot2"));
    img2->setVisible(false);
    this->round2 = RoundLayerSlot::create();
    this->round2->setTag(11);
    this->round2->setPosition(Vec2(img2->getPositionX() , img2->getPositionY()+7 ));
    pnlBg->addChild(this->round2, 3);
    
    ImageView* img3 = static_cast<ImageView*>(pnlBg->getChildByName("imgSlot3"));
    img3->setVisible(false);
    this->round3 = RoundLayerSlot::create();
    this->round3->setTag(11);
    this->round3->setPosition(Vec2(img3->getPositionX() , img3->getPositionY()+7 ));
    pnlBg->addChild(this->round3, 3);
    
    ImageView* img4 = static_cast<ImageView*>(pnlBg->getChildByName("imgSlot4"));
    img4->setVisible(false);
    this->round4 = RoundLayerSlot::create();
    this->round4->setTag(11);
    //this->round4->setStopCallBackFunc(this, this->stopRound4);
    this->round4->setPosition(Vec2(img4->getPositionX() , img4->getPositionY()+7));
    pnlBg->addChild(this->round4, 3);
    
    this->imgLucky1 = static_cast<ImageView*>(pnlBg->getChildByName("imgLucky1"));
    this->imgLucky2 = static_cast<ImageView*>(pnlBg->getChildByName("imgLucky2"));
    this->imgLucky3 = static_cast<ImageView*>(pnlBg->getChildByName("imgLucky3"));
    
    //    }else{
    //        ImageView* img5 = static_cast<ImageView*>(pnlBg->getChildByName("img5"));
    //        img5->setVisible(false);
    //        this->round5 = RoundLayerSlot::create();
    //        this->round5->setTag(11);
    //        this->round5->setPosition(Vec2(img5->getPositionX() - 65, img5->getPositionY() - 85));
    //        pnlBg->addChild(this->round5, 3);
    //
    //        ImageView* img1 = static_cast<ImageView*>(pnlBg->getChildByName("img1"));
    //        img1->setVisible(false);
    //        this->round1 = RoundLayerSlot::create();
    //        this->round1->setTag(11);
    //        this->round1->setPosition(Vec2(img1->getPositionX() - 65, img1->getPositionY() - 85));
    //        pnlBg->addChild(this->round1, 3);
    //
    //        ImageView* img2 = static_cast<ImageView*>(pnlBg->getChildByName("img2"));
    //        img2->setVisible(false);
    //        this->round2 = RoundLayerSlot::create();
    //        this->round2->setTag(11);
    //        this->round2->setPosition(Vec2(img2->getPositionX() - 65, img2->getPositionY() - 85));
    //        pnlBg->addChild(this->round2, 3);
    //
    //        ImageView* img3 = static_cast<ImageView*>(pnlBg->getChildByName("img3"));
    //        img3->setVisible(false);
    //        this->round3 = RoundLayerSlot::create();
    //        this->round3->setTag(11);
    //        this->round3->setPosition(Vec2(img3->getPositionX() - 65, img3->getPositionY() - 85));
    //        pnlBg->addChild(this->round3, 3);
    //
    //        ImageView* img4 = static_cast<ImageView*>(pnlBg->getChildByName("img4"));
    //        img4->setVisible(false);
    //        this->round4 = RoundLayerSlot::create();
    //        this->round4->setTag(11);
    //        //this->round4->setStopCallBackFunc(this, this->stopRound4);
    //        this->round4->setPosition(Vec2(img4->getPositionX() - 65, img4->getPositionY() - 85));
    //        pnlBg->addChild(this->round4, 3);
    //
    //    }
    
    // this->playAnimationMachine();
    //    this->createAnimPlay();
    //    this->onBtnBet1K(this, ui::Widget::TouchEventType::ENDED);
    
    //    this->btnPlay->setEnabled(false);
    //    this->sprDown->setVisible(false);
    
    
    this->round1->setVisible(false);
    this->round2->setVisible(false);
    this->round3->setVisible(false);
    this->round4->setVisible(false);
    this->round5->setVisible(false);
    
    
    
    this->autoPlay = false;
    if (this->autoPlay)
        this->imgAutoPlay->loadTexture(PATH_SLOT+"auto-play-min.png");
    else
        this->imgAutoPlay->loadTexture(PATH_SLOT+"check-p-min.png");
    
    
    
    this->countLine = this->GameLine;

    this->bgChooseLine = static_cast<ImageView*>(pnlBg->getChildByName("bgChooseLine"));
    this->bgChooseLine->setVisible(false);
    this->bgChooseLine->setLocalZOrder(5);
    for (int i = 1;i<=this->countLine;i++){
        Button* btn = static_cast<Button*>(bgChooseLine->getChildByName(StringUtils::format("btn%d",i)));
        btn->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnChooseLine, this));
        btn->loadTextures(StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),i),StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),i));
        btn->setTag(i);
        this->lstChooseLine.push_back(btn);
    }
    
    this->txtLineCount= static_cast<Text*>(pnlBg->getChildByName("txtLineCount"));
    this->txtLineCount->setString(StringUtils::format("%d",this->countLine));
    
    for (int i = 1;i<=5;i++){
        ImageView* img = static_cast<ImageView*>(pnlBg->getChildByName(StringUtils::format("pointX%d",i)));
        img->setVisible(false);
        this->lstpointXLines.push_back(img);
    }
    
    for (int i = 1;i<=this->countLine;i++){
        ImageView* img = static_cast<ImageView*>(pnlBg->getChildByName(StringUtils::format("line%dLeft",i)));
        img->setTag(i);
        this->lstlineLeft.push_back(img);
    }
    
    for (int i = 1;i<=this->countLine;i++){
        ImageView* img = static_cast<ImageView*>(pnlBg->getChildByName(StringUtils::format("line%dRight",i)));
        img->setTag(i);
        this->lstlineRight.push_back(img);
    }
    
    this->btnLineBack = static_cast<Button*>(pnlBg->getChildByName("btnLineBack"));
    if (this->btnLineBack) {
        this->btnLineBack->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnLineBack, this));
        this->btnLineBack->setPressedActionEnabled(true);
    }
    
    this->btnLineNext = static_cast<Button*>(pnlBg->getChildByName("btnLineNext"));
    if (this->btnLineNext) {
        this->btnLineNext->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnLineNext, this));
        this->btnLineNext->setPressedActionEnabled(true);
        this->btnLineNext->setEnabled(false);
        this->btnLineNext->setBright(false);
        //this->btnLineNext->setVisible(false);
    }
    
    this->btnBet = static_cast<Button*>(pnlBg->getChildByName("btnBet"));
    if (this->btnBet) {
        this->btnBet->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnBet, this));
        this->btnBet->setPressedActionEnabled(true);
        this->btnBet->loadTextures(StringUtils::format("%s%scuoc-btn.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()), StringUtils::format("%s%scuoc-btn-dis.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    }
    
    this->btnMax = static_cast<Button*>(pnlBg->getChildByName("btnMax"));
    if (this->btnMax) {
        this->btnMax->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnBetMax, this));
        this->btnMax->setPressedActionEnabled(true);
        this->btnMax->loadTextures(StringUtils::format("%s%smax-en.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()), StringUtils::format("%s%smax-dis.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
        
    }
    
    this->btnMulti = static_cast<Button*>(pnlBg->getChildByName("btnMulti"));
    if (this->btnMulti) {
        this->btnMulti->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnMulti, this));
        this->btnMulti->setPressedActionEnabled(true);
        this->btnMulti->loadTextures(StringUtils::format("%s%smulti-en.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()), StringUtils::format("%s%smulti-dis.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
        
    }
    
    for (int i = 1; i <= this->countLine;i++){
        if (i<this->countLine)
            lsLines+=StringUtils::format("%d,",i);
        else
            lsLines+=StringUtils::format("%d",i);
        
    }
    auto lst = mUtils::splitString(this->lsLines, ',');
    
    for (int i = 0; i< lst.size();i++){
        for (int j = 0; j < this->lstChooseLine.size();j++)
            if (this->lstChooseLine[j]->getTag() == atoi(lst[i].c_str())){
                this->lstChooseLine[j]->loadTextures(StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),atoi(lst[i].c_str())),StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),atoi(lst[i].c_str())));
                
            }
    }
    this->txtTienThang = static_cast<Text*>(pnlBg->getChildByName("txtTienThang"));
    this->txtTienThang->setString("");
    this->txtTongDat = static_cast<Text*>(pnlBg->getChildByName("txtTongDat"));
    this->txtTongDat->setString("");
    this->txtMucCuoc = static_cast<Text*>(pnlBg->getChildByName("txtMucCuoc"));
    this->txtMucCuoc->setString("");
    
    
    
    this->getMinMoney();
    //    this->getHuThuong(0);
    ////    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1) {
    ////        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameSlotMachine::getHuThuong),5);
    ////    }
    //
    //    this->btnGold = static_cast<Button*>(pnlBg->getChildByName("btnGold"));
    //    if (this->btnGold){
    //        this->btnGold->setPressedActionEnabled(true);
    //        this->btnGold->addClickEventListener(CC_CALLBACK_1(LayerPlayGameSlotMachine::onBtnGold, this));
    //    }
    //    this->btnSilver = static_cast<Button*>(pnlBg->getChildByName("btnSilver"));
    //    if (this->btnSilver){
    //        this->btnSilver->setPressedActionEnabled(true);
    //        this->btnSilver->setVisible(false);
    //        this->btnSilver->addClickEventListener(CC_CALLBACK_1(LayerPlayGameSlotMachine::onBtnSilver, this));
    //    }
    
    //    imgBonusQT = static_cast<ImageView*>(pnlBg->getChildByName("imgBonusQT"));
    //    imgBonusQT->setVisible(false);
    //
    //    boost::shared_ptr<ISFSObject> params(new SFSObject());
    //    params->PutInt("tm",0);
    //    boost::shared_ptr<IRequest> req(new ExtensionRequest("pkj", params));
    //    GameServer::getSingleton().Send(req);
    
    
    
    //    //Cheat
    //    txtNhanQT = static_cast<TextField*>(pnlBg->getChildByName("txtNhanQT"));
//        txtKetqua = static_cast<TextField*>(pnlBg->getChildByName("txtOTP"));
//    //    txtQT = static_cast<TextField*>(pnlBg->getChildByName("txtQT"));
//    //
//    //    Button* btnNhanQT = static_cast<Button*>(pnlBg->getChildByName("btnNhanQT"));
//    //    if (btnNhanQT) {
//    //        btnNhanQT->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnNhanQT, this));
//    //        //this->btn10k->setPressedActionEnabled(true);
//    //    }
//    //
//        Button* btnKetQua = static_cast<Button*>(pnlBg->getChildByName("btnSet"));
//        if (btnKetQua) {
//            btnKetQua->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnKetQua, this));
//            //this->btn10k->setPressedActionEnabled(true);
//        }
    ////
    //    Button* btnSetQT = static_cast<Button*>(pnlBg->getChildByName("btnSetQT"));
    //    if (btnSetQT) {
    //        btnSetQT->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnSetQT, this));
    //        //this->btn10k->setPressedActionEnabled(true);
    //    }
    // this->setScale(0.8);
    //    auto _currScene = Director::getInstance()->getRunningScene();
    //    if (!_currScene) return true;
    //    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    //    if (_scene)
    //    {
    //        _scene->hideWebView();
    //    }
    Button* btnChuyenDongTien = static_cast<Button*>(pnlBg->getChildByName("btnChuyenDongTien"));
    if (btnChuyenDongTien) {
        btnChuyenDongTien->addClickEventListener(CC_CALLBACK_1(LayerPlayGameSlotMachine::onBtnChuyenDongTien, this));
        //this->btn10k->setPressedActionEnabled(true);
    }
    
    
    this->txtMoneyHuThuong = (Text*)pnlBg->getChildByName("txtMoneyHuThuong");
    if (this->txtMoneyHuThuong){
        this->txtMoneyHuThuong->setString("---");
    }
    
    
    this->btnBetUp = static_cast<Button*>(pnlBg->getChildByName("upbet"));
    if (this->btnBetUp) {
        this->btnBetUp->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnBetUp, this));
        this->btnBetUp->setPressedActionEnabled(true);
    }
    this->btnBetDown = static_cast<Button*>(pnlBg->getChildByName("downbet"));
    if (this->btnBetDown) {
        this->btnBetDown->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnBetDown, this));
        this->btnBetDown->setPressedActionEnabled(true);
        this->btnBetDown->setEnabled(false);
        this->btnBetDown->setBright(false);

    }
    
    this->imgCuoc = static_cast<ImageView*>(pnlBg->getChildByName("imgCuoc"));
    if (this->imgCuoc){
        this->imgCuoc->loadTexture(StringUtils::format("%s%sbet-bg.png",PATH_SLOT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    }
    
    this->bgWin = static_cast<ImageView*>(pnlBg->getChildByName("bgWin"));
    this->txtMoneyWinBig = static_cast<Text*>(this->bgWin->getChildByName("txtMoneyWinBig"));
    
    this->bgWin->setLocalZOrder(100000);
    this->bgWin->setVisible(false);
    
    this->mQueueMsg = new ThreadQueueMsg();
    this->mQueueMsg->setFunCallBack(this, callfunc_selector(LayerPlayGameSlotMachine::OnExtensionResponseProcess));
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameSlotMachine::updateEvent),this,1/60, false);
    

    
    auto btnRank= static_cast<Button*>(pnlBg->getChildByName("btnRank"));
    if (btnRank) {
       btnRank->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameSlotMachine::onBtnRank, this));
        btnRank->setPressedActionEnabled(true);
    }
    
    this->bgNoHu = static_cast<ImageView*>(pnlBg->getChildByName("bgNoHu"));
    auto item1 = static_cast<ImageView*>(this->bgNoHu->getChildByName("item1"));
    auto item2 = static_cast<ImageView*>(this->bgNoHu->getChildByName("item2"));
    auto item3 = static_cast<ImageView*>(this->bgNoHu->getChildByName("item3"));
    auto item4 = static_cast<ImageView*>(this->bgNoHu->getChildByName("item4"));
    auto item5 = static_cast<ImageView*>(this->bgNoHu->getChildByName("item5"));
    this->lstItemNoHu.push_back(item1);
    this->lstItemNoHu.push_back(item2);
    this->lstItemNoHu.push_back(item3);
    this->lstItemNoHu.push_back(item4);
    this->lstItemNoHu.push_back(item5);

    this->txtMoneyNoHu = static_cast<Text*>(this->bgNoHu->getChildByName("txtMoney"));
    this->bgNoHu->setLocalZOrder(100001);
    this->bgNoHu->setVisible(false);
    SceneManager::getSingleton().showLoading();

    return true;
}
void LayerPlayGameSlotMachine::onBtnRank(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        LayerPopupXepHangSlot* xh = LayerPopupXepHangSlot::create();
        this->addChild(xh,1000000);
    }
}
void LayerPlayGameSlotMachine::onBtnBetUp(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }
        this->btnBetDown->setEnabled(true);
        this->btnBetDown->setBright(true);
        if (this->currPosBet == this->lstBet.size()-1){
            this->btnBetUp->setEnabled(false);
            this->btnBetUp->setBright(false);
        }
        else{
            this->currPosBet++;
            if (this->currPosBet == this->lstBet.size()-1){
                this->btnBetUp->setEnabled(false);
                this->btnBetUp->setBright(false);
            }
        }
        this->currBetMoney = this->lstBet[this->currPosBet];
        this->getHuThuong(0);
        this->txtTongDat->setString(StringUtils::format("%s: %s",dataManager.GetSysString(886).c_str(),mUtils::convertMoneyEx(long(this->currBetMoney*this->countLine)).c_str()));
        this->txtMucCuoc->setString(mUtils::formatMoneySymbol(long(this->currBetMoney)));
    }
}
void LayerPlayGameSlotMachine::onBtnBetDown(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }
        
        
        
        this->btnBetUp->setEnabled(true);
        this->btnBetUp->setBright(true);
        if (this->currPosBet == 0){
            this->btnBetDown->setEnabled(false);
            this->btnBetDown->setBright(false);
            
        }else{
            this->currPosBet--;
            if (this->currPosBet == 0){
                this->btnBetDown->setEnabled(false);
                this->btnBetDown->setBright(false);
                
            }
        }
        this->currBetMoney = this->lstBet[this->currPosBet];
        this->getHuThuong(0);
        this->txtTongDat->setString(StringUtils::format("%s: %s",dataManager.GetSysString(886).c_str(),mUtils::convertMoneyEx(long(this->currBetMoney*this->countLine)).c_str()));
        this->txtMucCuoc->setString(mUtils::formatMoneySymbol(long(this->currBetMoney)));
        this->getHuThuong(0);
    }
}
void LayerPlayGameSlotMachine::onBtnChuyenDongTien(Ref* sender){
    this->isJoin = false;
    if (this->CurrentGameState){
        auto toast = Chat::create(dataManager.GetSysString(421),-1);
        this->addChild(toast);
        return;
    }
    boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (NULL == lastRoom)
    {
        //SceneManager::getSingleton().gotoPickGame(this->gameID);
        SceneManager::getSingleton().gotoMain();
        
    }
    else
    {
        // SceneManager::getSingleton().setCurrRoomType(-1);
        this->lsLines = "";
        this->countLine = this->GameLine;
        for (int i = 1; i <= this->countLine;i++){
            if (i<this->countLine)
                lsLines+=StringUtils::format("%d,",i);
            else
                lsLines+=StringUtils::format("%d",i);
            
        }
        auto lst = mUtils::splitString(this->lsLines, ',');
        
        for (int i = 0; i< lst.size();i++){
            for (int j = 0; j < this->lstChooseLine.size();j++)
                if (this->lstChooseLine[j]->getTag() == atoi(lst[i].c_str())){
                    this->lstChooseLine[j]->loadTextures(StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),atoi(lst[i].c_str())),StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),atoi(lst[i].c_str())));
                    
                }
        }
        this->txtTongDat->setString(StringUtils::format("%s: %s",dataManager.GetSysString(886).c_str(),mUtils::convertMoneyEx(long(this->currBetMoney*this->countLine)).c_str()));
        this->txtLineCount->setString(StringUtils::format("%d",this->GameLine));
        this->btnLineNext->setEnabled(false);
        this->btnLineNext->setBright(false);
        
        this->btnLineBack->setEnabled(true);
        this->btnLineBack->setBright(true);

        this->autoPlay = false;
        this->resetGame();
        this->txtTienThang->setString("");
        //this->showResultPrevMatch("4,7,9,7,7,|5,9,4,3,2,|3,6,2,3,4,|");
        for (int i = 0 ; i< this->lstDrawLine.size();i++){
            this->lstDrawLine[i]->removeFromParent();
        }
        this->bgChooseLine->setVisible(false);
        
        this->lstDrawLine.clear();
        this->round1->resetRound();
        this->round2->resetRound();
        this->round3->resetRound();
        this->round4->resetRound();
        this->round5->resetRound();
        SceneManager::getSingleton().setIsChuyenTab(true);
        boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
        GameServer::getSingleton().Send(request);
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameSlotMachine::updateTimerStart),this);
    }
    SceneManager::getSingleton().showLoading();
    
}
void LayerPlayGameSlotMachine::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    // Get the user parameter of the event
    boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    // Get the room parameter of the event
    boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
    boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
    //
    if (ptrNotifiedUser == NULL || ptrNotifiedRoom == NULL) return;
    
    if (ptrNotifiedRoom->GroupId()->compare(SERVICE_CHAT_ROOM) == 0
        || ptrNotifiedRoom->Id() == 118 || ptrNotifiedRoom->GroupId()->compare("200") == 0
        || ptrNotifiedRoom->Id() == 200)
        return;
    //if it is me
    if (!ptrNotifiedUser->IsItMe()){
        return;
    }
    if (this->isJoin) return;
    if (this->currTypeMoney == -1) return;
    if (SceneManager::getSingleton().getGameID() != kGameSlot) return;
    if (this->currTypeMoney == 0){
        boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(kGameSlot));
       // log("list room : %d", list_room->size());
        for (int i = 0; i < list_room->size(); i++)
        {
            boost::shared_ptr<Room> room = list_room->at(i);
            if (room != NULL)
            {
                if (room->GetVariable("roomType") != NULL){
                    int roomType = *room->GetVariable("roomType")->GetIntValue();
                    /*if (getCurrRoomType() == 0){
                     if (this->getMoneyByType(0) < this->getMinMoney(kGameXocDia))
                     setCurrRoomType(1);
                     }
                     
                     if (getCurrRoomType() == 1){
                     if (this->getMoneyByType(1) < this->getMinMoney(kGameXocDia))
                     setCurrRoomType(0);
                     }*/
                    
                    if (roomType == 1){
                        this->currTypeMoney = 1;
                        boost::shared_ptr<long> lastRoomID(new long(-1));
                        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
                        if (lastRoom)
                            lastRoomID = boost::shared_ptr<long>(new long(lastRoom->Id()));
                       // log("Room Tai Xiu != NULL");
                        boost::shared_ptr<IRequest> request(new JoinRoomRequest(room, "", lastRoomID, false));
                        GameServer::getSingleton().Send(request);
                    }
                }
            }
        }
    }else{
        boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(kGameSlot));
       // log("list room : %d", list_room->size());
        for (int i = 0; i < list_room->size(); i++)
        {
            boost::shared_ptr<Room> room = list_room->at(i);
            if (room != NULL)
            {
                if (room->GetVariable("roomType") != NULL){
                    int roomType = *room->GetVariable("roomType")->GetIntValue();
                    //log("roomType: %d - currRoom: %d", roomType, getCurrRoomType());
                    /*if (getCurrRoomType() == 0){
                     if (this->getMoneyByType(0) < this->getMinMoney(kGameXocDia))
                     setCurrRoomType(1);
                     }
                     
                     if (getCurrRoomType() == 1){
                     if (this->getMoneyByType(1) < this->getMinMoney(kGameXocDia))
                     setCurrRoomType(0);
                     }*/
                    
                    if (roomType == 0){
                        this->currTypeMoney = 0;
                        boost::shared_ptr<long> lastRoomID(new long(-1));
                        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
                        if (lastRoom)
                            lastRoomID = boost::shared_ptr<long>(new long(lastRoom->Id()));
                       // log("Room Tai Xiu != NULL");
                        boost::shared_ptr<IRequest> request(new JoinRoomRequest(room, "", lastRoomID, false));
                        GameServer::getSingleton().Send(request);
                    }
                }
            }
        }
    }
}
void LayerPlayGameSlotMachine::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    
    
    this->btnBetUp->setEnabled(true);
    this->btnBetDown->setEnabled(false);
    
    //SceneManager::getSingleton().hideLoading();
    if (this->currTypeMoney ==0 ){
        this->imgMoneyTable->loadTexture(PATH_SLOT+"dongmoc-min.png");
        this->imgMoneyType->loadTexture(PATH_SLOT+"dongmoc-min.png");
        
        
    }else{
        this->imgMoneyTable->loadTexture(PATH_SLOT+"donghao-min.png");
        this->imgMoneyType->loadTexture(PATH_SLOT+"donghao-min.png");
        
    }
}
void LayerPlayGameSlotMachine::updateMoneyWin(float dt){
    
    //return;
    static int i = 1000;
    if (this->winvalue/10000000 > 0){
        i = 500;
    }
    if (this->winvalue/100000000 > 0){
        i = 100;
    }
    this->winvalue2-=(24 + 24*this->winvalue/i);
    
    if (this->winvalue2 <= 0){
        this->winMoney = 0;
        this->winvalue = 0;
        this->winvalue2 = 0;
        this->Animating = false;
        if (this->currTypeMoney == 0){
            boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
            
            if (mySelf != NULL){
                if (mySelf->GetVariable("amf") != NULL) {
                    auto myMoney = *mySelf->GetVariable("amf")->GetDoubleValue();
                    this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
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
        ParticleSystemQuad* ps = (ParticleSystemQuad*)this->pnlBg->getChildByTag(278123);
        if (ps){
            ps->stop();
        }
        string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
        this->txtMyMoney->setString(_money);
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameSlotMachine::updateMoneyWin),this);
        return;
    }else{
        
        this->currMyMoney+=(24 + 24*this->winvalue2/1000);
        double moneyReal = 0;
        if (this->currTypeMoney == 0){
            boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
            
            if (mySelf != NULL){
                if (mySelf->GetVariable("amf") != NULL) {
                    moneyReal = *mySelf->GetVariable("amf")->GetDoubleValue();
                    
                }
            }
        }else{
            boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
            
            if (mySelf != NULL){
                if (mySelf->GetVariable("amfs") != NULL) {
                    moneyReal = *mySelf->GetVariable("amfs")->GetDoubleValue();
                }
            }
        }
        if (this->currMyMoney<=moneyReal){
            string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
            this->txtMyMoney->setString(_money);
        }else{
            ParticleSystemQuad* ps = (ParticleSystemQuad*)this->pnlBg->getChildByTag(278123);
            if (ps){
                ps->stop();
            }
        }
    }
}
void LayerPlayGameSlotMachine::updatemotion(float dt){
}
void LayerPlayGameSlotMachine::onBtnChooseLine(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        Button* btn = (Button*)sender;
        int tag = btn->getTag();
        auto lst = mUtils::splitString(this->lsLines,',');
        bool isChoose = false;
        for (int i = 0; i< lst.size();i++){
            if (tag == atoi(lst[i].c_str())){
                isChoose = true;
            }
        }
        this->resetGame();
        if (isChoose){
            btn->loadTextures(StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),tag),StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),tag));
            this->lsLines = "";
            for (int i = 0; i< lst.size();i++){
                if (tag == atoi(lst[i].c_str())){
                    lst.erase(lst.begin() + i);
                }
            }
            for (int i = 0; i< lst.size();i++){
                if (i<lst.size()-1)
                    this->lsLines+=StringUtils::format("%s,",lst[i].c_str());
                else
                    this->lsLines+=StringUtils::format("%s",lst[i].c_str());
            }
        }
        else{
            btn->loadTextures(StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),tag),StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),tag));
            if (this->lsLines == "")
                this->lsLines+=StringUtils::format("%d",tag);
            else
                this->lsLines+=StringUtils::format(",%d",tag);
            
        }
        this->drawChooseLine(this->lsLines);
    }
}

void LayerPlayGameSlotMachine::drawChooseLine(string str){
    for (int i = 0 ; i< this->lstDrawLine.size();i++){
        this->lstDrawLine[i]->removeFromParent();
    }
    this->lstDrawLine.clear();
    auto lstLine = mUtils::splitString(str,',');
    if (lstLine.size() == 0 || lstLine.size() == 1){
        this->btnLineNext->setEnabled(true);
        this->btnLineNext->setBright(true);
        this->btnLineBack->setEnabled(false);
        this->btnLineBack->setBright(false);
    }
    
    if (lstLine.size() > 1 && lstLine.size() < this->GameLine){
        this->btnLineNext->setEnabled(true);
        this->btnLineNext->setBright(true);
        this->btnLineBack->setEnabled(true);
        this->btnLineBack->setBright(true);
    }
    
    if (lstLine.size() == this->GameLine ){
        this->btnLineNext->setEnabled(false);
        this->btnLineNext->setBright(false);
        this->btnLineBack->setEnabled(true);
        this->btnLineBack->setBright(true);
    }
    this->countLine = lstLine.size();
    
    this->txtLineCount->setString(StringUtils::format("%d",this->countLine));
    this->txtTongDat->setString(StringUtils::format("%s: %s",dataManager.GetSysString(886).c_str(),mUtils::convertMoneyEx(long(this->currBetMoney*this->countLine)).c_str()));
    
    for (int i = 0; i<lstLine.size();i++){
        int row1 = 1;
        int row2 = 1;
        int row3 = 1;
        int row4 = 1;
        int row5= 1;
        
        switch (atoi(lstLine[i].c_str())) {
            case 1:
                row1 = 2;
                row2 = 2;
                row3 = 2;
                row4 = 2;
                row5 = 2;
                
                break;
            case 2:
                row1 = 3;
                row2 = 3;
                row3 = 3;
                row4 = 3;
                row5 = 3;
                
                break;
            case 3:
                row1 = 1;
                row2 = 1;
                row3 = 1;
                row4 = 1;
                row5 = 1;
                break;
            case 4:
                row1 = 3;
                row2 = 2;
                row3 = 3;
                row4 = 2;
                row5 = 3;
                
                break;
            case 5:
                row1 = 1;
                row2 = 2;
                row3 = 1;
                row4 = 2;
                row5 = 1;
                
                break;
            case 6:
                row1 = 2;
                row2 = 3;
                row3 = 2;
                row4 = 3;
                row5 = 2;
                
                break;
            case 7:
                row1 = 2;
                row2 = 1;
                row3 = 2;
                row4 = 1;
                row5 = 2;
                
                break;
            case 8:
                row1 = 3;
                row2 = 2;
                row3 = 1;
                row4 = 2;
                row5 = 3;
                
                break;
            case 9:
                row1 = 1;
                row2 = 2;
                row3 = 3;
                row4 = 2;
                row5 = 1;
                
                break;
            case 10:
                row1 = 3;
                row2 = 3;
                row3 = 3;
                row4 = 3;
                row5 = 2;
                
                break;
            case 11:
                row1 = 2;
                row2 = 2;
                row3 = 1;
                row4 = 2;
                row5 = 2;
                
                break;
            case 12:
                row1 = 2;
                row2 = 3;
                row3 = 2;
                row4 = 1;
                row5 = 2;
                
                break;
                
            case 13:
                row1 = 2;
                row2 = 1;
                row3 = 2;
                row4 = 3;
                row5 = 2;
                
                break;
            case 14:
                row1 = 1;
                row2 = 1;
                row3 = 1;
                row4 = 1;
                row5 = 2;
                
                break;
            case 15:
                row1 = 2;
                row2 = 3;
                row3 = 3;
                row4 = 3;
                row5 = 3;
                
                break;
            case 16:
                row1 = 2;
                row2 = 2;
                row3 = 3;
                row4 = 2;
                row5 = 2;
                
                break;
            case 17:
                row1 = 2;
                row2 = 1;
                row3 = 1;
                row4 = 1;
                row5 = 1;
                
                break;
            default:
                break;
        }
        
        this->drawLineNew(atoi(lstLine[i].c_str()),{row1,row2,row3,row4,row5});
    }
}
void LayerPlayGameSlotMachine::onBtnRecharge(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (this->currTypeMoney ==0){
            LayerPopupCuaHang* popupCuaHang = LayerPopupCuaHang::create();
            popupCuaHang->onBtnNapThe(NULL);
            auto _scene = Director::getInstance()->getRunningScene();
            _scene->addChild(popupCuaHang,2000);
        }
        else{
            LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
            popup->onBtnNapHao(NULL);
            auto _scene = Director::getInstance()->getRunningScene();
            _scene->addChild(popup,2000);
        }
        
    }
}
void LayerPlayGameSlotMachine::onBtnMulti(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }
        if (this->bgChooseLine->isVisible()){
            this->bgChooseLine->setVisible(false);
        }else{
            this->bgChooseLine->setVisible(true);
        }
    }
}
void LayerPlayGameSlotMachine::onBtnBetMax(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }
        this->resetGame();
        this->currPosBet = this->lstBet.size()-1;
        this->countLine = this->GameLine;
        this->lsLines = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17";
        this->btnLineNext->setEnabled(false);
        this->btnLineNext->setBright(false);
        this->btnLineBack->setEnabled(true);
        this->btnLineBack->setBright(true);
        
        
        this->btnBetUp->setEnabled(false);
        this->btnBetUp->setBright(false);
        this->btnBetDown->setEnabled(true);
        this->btnBetDown->setBright(true);
        
        this->currBetMoney = this->lstBet[this->currPosBet];
        this->getHuThuong(0);
        this->txtMucCuoc->setString(mUtils::formatMoneySymbol(long(this->currBetMoney)));
        
        this->txtTongDat->setString(StringUtils::format("%s: %s",dataManager.GetSysString(886).c_str(),mUtils::convertMoneyEx(long(this->currBetMoney*this->countLine)).c_str()));
        this->txtLineCount->setString(StringUtils::format("%d",this->countLine));
        
        this->drawChooseLine(this->lsLines);
        
        for (int i = 0; i< this->lstChooseLine.size();i++){
            this->lstChooseLine[i]->loadTextures(StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),i+1),StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),i+1));
        }
        
        auto lst = mUtils::splitString(this->lsLines, ',');
        for (int i = 0; i< lst.size();i++){
            for (int j = 0; j < this->lstChooseLine.size();j++)
                if (this->lstChooseLine[j]->getTag() == atoi(lst[i].c_str())){
                    this->lstChooseLine[j]->loadTextures(StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),atoi(lst[i].c_str())),StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),atoi(lst[i].c_str())));
                    
                }
        }
        
    }
}

void LayerPlayGameSlotMachine::onBtnBet(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }
        if (this->currPosBet == this->lstBet.size()-1)
            this->currPosBet = 0;
        else
            this->currPosBet++;
        this->currBetMoney = this->lstBet[this->currPosBet];
        this->getHuThuong(0);
        this->txtTongDat->setString(StringUtils::format("%s: %s",dataManager.GetSysString(886).c_str(),mUtils::convertMoneyEx(long(this->currBetMoney*this->countLine)).c_str()));
        this->txtMucCuoc->setString(mUtils::formatMoneySymbol(long(this->currBetMoney)));
        
    }
}
void LayerPlayGameSlotMachine::onBtnLineBack(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }
        this->resetGame();

        this->countLine--;
        this->btnLineNext->setEnabled(true);
        this->btnLineNext->setBright(true);
        this->txtLineCount->setString(StringUtils::format("%d",this->countLine));
        if (this->countLine==1){
            this->btnLineBack->setEnabled(false);
            this->btnLineBack->setBright(false);
            
        }
        this->lsLines = "";
        for (int i = 1; i <= this->countLine;i++){
            if (i<this->countLine)
                this->lsLines+=StringUtils::format("%d,",i);
            else
                this->lsLines+=StringUtils::format("%d",i);
            
        }
        this->drawChooseLine(this->lsLines);
        
        this->txtTongDat->setString(StringUtils::format("%s: %s",dataManager.GetSysString(886).c_str(),mUtils::convertMoneyEx(long(this->currBetMoney*this->countLine)).c_str()));
        
        for (int i = 0; i< this->lstChooseLine.size();i++){
            this->lstChooseLine[i]->loadTextures(StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),i+1),StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),i+1));
        }
        
        auto lst = mUtils::splitString(this->lsLines, ',');
        for (int i = 0; i< lst.size();i++){
            for (int j = 0; j < this->lstChooseLine.size();j++)
                if (this->lstChooseLine[j]->getTag() == atoi(lst[i].c_str())){
                    this->lstChooseLine[j]->loadTextures(StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),atoi(lst[i].c_str())),StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),atoi(lst[i].c_str())));
                    
                }
        }
    }
}
void LayerPlayGameSlotMachine::loadChooseLine(string str){
    auto lstID = mUtils::splitString(str,',');
}
void LayerPlayGameSlotMachine::onBtnLineNext(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(420),-1);
            this->addChild(toast);
            return;
        }
        this->resetGame();

        this->countLine++;
        this->btnLineBack->setBright(true);
        this->btnLineBack->setEnabled(true);

        this->txtLineCount->setString(StringUtils::format("%d",this->countLine));
        if (this->countLine==this->GameLine){
            this->btnLineNext->setEnabled(false);
            this->btnLineNext->setBright(false);

        }
        this->lsLines = "";
        
        for (int i = 1; i <= this->countLine;i++){
            if (i<this->countLine)
                lsLines+=StringUtils::format("%d,",i);
            else
                lsLines+=StringUtils::format("%d",i);
            
        }
        this->drawChooseLine(this->lsLines);
        this->txtTongDat->setString(StringUtils::format("%s: %s",dataManager.GetSysString(886).c_str(),mUtils::convertMoneyEx(long(this->currBetMoney*this->countLine)).c_str()));
        for (int i = 0; i< this->lstChooseLine.size();i++){
            this->lstChooseLine[i]->loadTextures(StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),i+1),StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),i+1));
        }
        auto lst = mUtils::splitString(this->lsLines, ',');
        for (int i = 0; i< lst.size();i++){
            for (int j = 0; j < this->lstChooseLine.size();j++)
                if (this->lstChooseLine[j]->getTag() == atoi(lst[i].c_str())){
                    this->lstChooseLine[j]->loadTextures(StringUtils::format("%sbtn-line%d-en.png",PATH_SLOT.c_str(),atoi(lst[i].c_str())),StringUtils::format("%sbtn-line%d-dis.png",PATH_SLOT.c_str(),atoi(lst[i].c_str())));
                    
                }
        }
    }
}

void LayerPlayGameSlotMachine::onBtnGold(Ref *sender){
    if (this->autoPlay){
        auto toast = Chat::create(dataManager.GetSysString(420),-1);
        this->addChild(toast);
        return;
    }
    SceneManager::getSingleton().showLoading();
    
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
void LayerPlayGameSlotMachine::onBtnSilver(Ref *sender){
    if (this->autoPlay){
        auto toast = Chat::create(dataManager.GetSysString(420),-1);
        this->addChild(toast);
        return;
    }
    SceneManager::getSingleton().showLoading();
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
void LayerPlayGameSlotMachine::onBtnSetQT(Ref *sender, ui::Widget::TouchEventType pType){
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
//void LayerPlayGameSlotMachine::onBtnNhanQT(Ref *sender, ui::Widget::TouchEventType pType){
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
//void LayerPlayGameSlotMachine::onBtnKetQua(Ref *sender, ui::Widget::TouchEventType pType){
//    if (pType == ui::Widget::TouchEventType::ENDED) {
//        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
//        if (lastRoom == NULL)
//            return;
//        string strCheat = this->txtKetqua->getString();
//                if (strCheat == "")
//                    return;
//                boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
//
//                boost::shared_ptr<ISFSObject> params(new SFSObject());
//                params->PutUtfString("rg", strCheat);
//                params->PutUtfString("uid", mySelf->Name());
//                boost::shared_ptr<IRequest> req(new ExtensionRequest("srltreq", params,lastRoom));
//                GameServer::getSingleton().Send(req);
//    }
//}
//
void LayerPlayGameSlotMachine::getHuThuong(float dt){
    
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt(EXT_FIELD_GAME_BET_VALUE,this->currBetMoney);
    boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_GET_JACKPOT_REQ, params,lastRoom));
    GameServer::getSingleton().Send(req);
}
void LayerPlayGameSlotMachine::event_EXT_EVENT_GAME_STATE(){
    
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
                this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
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
        this->imgAutoPlay->loadTexture(PATH_SLOT+"auto-play-min.png");
    else
        this->imgAutoPlay->loadTexture(PATH_SLOT+"check-p-min.png");
    this->btnAutoPlay->setEnabled(true);
    
    this->resetGame();
    
    if (ghs != NULL){
        if (0 == ghs->compare(""))
            this->showResultPrevMatch("4,7,9,7,7,|5,9,4,3,2,|3,6,2,3,4,|");
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
    
    
}
void LayerPlayGameSlotMachine::showResultPrevMatch(string str){
    auto lstRow = mUtils::splitString(str, '|');
    if (lstRow.size()<3) {
        return;
    }
    
    auto lstRow1 = mUtils::splitString(lstRow[0], ',');
    auto lstRow2 = mUtils::splitString(lstRow[1], ',');
    auto lstRow3 = mUtils::splitString(lstRow[2], ',');
    
    if (lstRow1.size()<5||lstRow2.size()<5||lstRow2.size()<5)
        return;
    
    string strs = "";
    for (int i = 0; i<5;i++){
        strs+=lstRow1[i]+"_"+lstRow2[i]+"_"+lstRow3[i]+";";
    }
    this->itemResult.clear();
    auto arrItem = mUtils::splitString(strs,';');
    if (arrItem.size() < 5)
        return;
    auto k = arrItem.size();
    for (int i = 0; i < k; i++) {
        this->itemResult.push_back(arrItem[i]);
    }
    if (this->itemResult.size() < 5)
        return;
    this->stopRound1(this->itemResult[0]);
    this->round1->stopUpdate();
    this->stopRound2(this->itemResult[1]);
    this->round2->stopUpdate();
    this->stopRound3(this->itemResult[2]);
    this->round3->stopUpdate();
    this->stopRound4(this->itemResult[3]);
    this->round4->stopUpdate();
    this->stopRound5(this->itemResult[4]);
    this->round5->stopUpdate();
    
}
void LayerPlayGameSlotMachine::event_PK_GET_QUY_THUONG(){
    
    boost::shared_ptr<long> money = this->param->GetInt("gbv");
    if (money == NULL)
        return;
    if (this->currHuThuong < *money || this->currHuThuong > *money) {
        this->txtTongHu->setVisible(true);
        this->currHuThuong = *money;
        if (this->isNoHu)
            return;
        this->txtTongHu->setString(mUtils::convertMoneyEx(this->currHuThuong));
        //this->chenhlechHuThuong = money - this->currHuThuong;
        //Director::getInstance()->getScheduler()->schedule(this->updateMoneyHuThuong);
        this->txtTongHu->runAction(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 1),NULL));
    }
}
void LayerPlayGameSlotMachine::event_EXT_EVENT_GAME_START() {
    //phien
    this->timerStart = 0;
    boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);
    if (errc != NULL){
        int ec = *errc;
        if (ec == 1){
            auto toast = Chat::create(dataManager.GetSysString(82),-1);
            this->addChild(toast);
            this->autoPlay = false;
            this->imgAutoPlay->loadTexture(PATH_SLOT+"check-p-min.png");
            return;
        }
    }
    if (!this->Animating){
        this->startGame();
        auto toast = Chat::create(dataManager.GetSysString(420),-1);
        this->addChild(toast);
        for (int i = 0 ; i< this->lstDrawLine.size();i++){
            this->lstDrawLine[i]->removeFromParent();
        }
        this->lstDrawLine.clear();
    }
}
void LayerPlayGameSlotMachine::startGame(){
    //this->playAnimStartGame();
    this->resetGame();
    this->isResult = false;
    this->currShow = 0;
    this->startRound();
    this->btnPlay->setEnabled(false);
    this->btnMax->setEnabled(false);
    this->btnBetUp->setEnabled(false);
    this->btnBetDown->setEnabled(false);
    this->btnMulti->setEnabled(false);
    this->btnLineNext->setEnabled(false);
    this->btnLineBack->setEnabled(false);
    
    //this->sprDown->setVisible(false);
    this->Animating = true;
    this->CurrentGameState = true;
    this->txtTienThang->setString("");
}
void LayerPlayGameSlotMachine::event_EXT_EVENT_GAME_RESULT() {
    this->isResult = true;
    boost::shared_ptr<long> winValue = this->param->GetInt("gwv");
    boost::shared_ptr<string> rc = this->param->GetUtfString("gbt");
    boost::shared_ptr<string> lst7lucky = this->param->GetUtfString("lst7");
    boost::shared_ptr<long> has7 = this->param->GetInt("has7");
    
    if (winValue!= NULL){
        this->winMoney = *winValue;
        
    }
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

void LayerPlayGameSlotMachine::updateEvent(float dt){
    if (this->mQueueMsg->isQueueEmpty())
        return;
    //    if (this->Animating)
    //        return;
    
    this->mQueueMsg->run();
}
void LayerPlayGameSlotMachine::onBtnClose(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
        //        LayerGameTaiXiuMini* parent = (LayerGameTaiXiuMini*)this->getParent();
        //        if (parent)
        //            parent->imgBGTimer->setVisible(true);
        boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
        if (NULL == lastRoom)
        {
            SceneManager::getSingleton().gotoMain();
            SceneMain::getSingleton().prepareToPickGame();

        }
        else
        {

            SceneManager::getSingleton().setIsChuyenTab(false);
            SceneManager::getSingleton().setCurrRoomType(-1);
            this->currTypeMoney = -1;
            boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
            GameServer::getSingleton().Send(request);
            SceneMain::getSingleton().prepareToPickGame();
        }
    }
}
void LayerPlayGameSlotMachine::onHieuUngCallBack(Ref* pSender){
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameSlotMachine::updateMoneyWin),this,1/60, false);
}
void LayerPlayGameSlotMachine::onBtnPlay(Ref *sender, ui::Widget::TouchEventType pType) {
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
        
        if (this->countLine == 0 || this->lsLines == ""){
            auto toast = Chat::create(dataManager.GetSysString(896),-1);
            this->addChild(toast);
            return;
        }
        //this->showResult();
        if (this->currBetMoney*this->countLine > this->currMyMoney  ) {
            if (this->currTypeMoney == 0)
                SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
            else{
                SceneManager::getSingleton().showPopupNapTienGold(dataManager.GetSysString(978));
                
            }
            return;
        }
        //        if (SceneManager::getSingleton().getCurrRoomType() == this->currTypeMoney){
        //            if (this->currBetMoney > (this->currMyMoney - this->MinBetValue)){
        //                auto toast = Chat::create(dataManager.GetSysString(409), -1);
        //                this->addChild(toast);
        //                return;
        //            }
        //        }
        this->currMyMoney -=this->currBetMoney*this->countLine;
        this->txtMyMoney->setString(mUtils::convertMoneyEx(this->currMyMoney));
        this->bgChooseLine->setVisible(false);
        
        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
        if (lastRoom == NULL)
            return;
        SceneManager::getSingleton().setLastGameID(133);
        
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString(EXT_FIELD_SLOT_LINE_COUNT,this->lsLines);
        params->PutInt(EXT_FIELD_GAME_BET_VALUE,this->currBetMoney);
        boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_GAME_ROTATE_REQ, params,lastRoom));
        GameServer::getSingleton().Send(req);
        //
        this->startGame();
        
        for (int i = 0 ; i< this->lstDrawLine.size();i++){
            this->lstDrawLine[i]->removeFromParent();
            
        }
        this->lstDrawLine.clear();
        
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
void LayerPlayGameSlotMachine::onBtnSuKien(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        LayerPopupWebView* _layer = LayerPopupWebView::create();
        string url = LINK_EVENT_MINIPOKER;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
            url = SKTX_P2;
        _layer->setWebView(url);
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(_layer, 1000);
    }
}
void LayerPlayGameSlotMachine::onBtnHuongDan(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        LayerGuideInGame* layer = LayerGuideInGame::create();
        string url = GUIDE_LINK;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
            url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+GUIDE_P2_LINK;
        layer->setLink(StringUtils::format("%s%d", url.c_str(), 139));
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(layer, 1000);
        
        layer->setTag(1504);
    }
}
void LayerPlayGameSlotMachine::onBtnLichSu(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
        LayerLichSuMiniPoker* lichsu = LayerLichSuMiniPoker::create();
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(lichsu);
        lichsu->setLocalZOrder(1000);
    }
}
void LayerPlayGameSlotMachine::onBtnXepHang(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
        LayerXepHangMiniPoker* xephang = LayerXepHangMiniPoker::create();
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(xephang);
        xephang->setLocalZOrder(1000);
    }
}
void LayerPlayGameSlotMachine::onBtnAutoPlay(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        
        
        if (this->Animating && !this->autoPlay){
            auto toast = Chat::create(dataManager.GetSysString(421),-1);
            this->addChild(toast);
            return;
        }
        
        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
        if (lastRoom == NULL)
            return;
        
        if (this->autoPlay){
            
            
            this->autoPlay = false;
            
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutUtfString(EXT_FIELD_SLOT_LINE_COUNT,this->lsLines);
            params->PutInt(EXT_FIELD_GAME_BET_VALUE,this->currBetMoney);
            params->PutInt(EXT_FIELD_GAME_IS_AUTO,0);
            boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_SLOT_AUTO_PLAY_REQ, params,lastRoom));
            GameServer::getSingleton().Send(req);
            
            auto toast = Chat::create(dataManager.GetSysString(422),-1);
            this->addChild(toast);
            
        }
        else{
            
            if (this->currBetMoney*this->countLine > this->currMyMoney ) {
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
            if (this->countLine == 0 || this->lsLines == ""){
                auto toast = Chat::create(dataManager.GetSysString(896),-1);
                this->addChild(toast);
                return;
            }
            
            this->autoPlay = true;
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutUtfString(EXT_FIELD_SLOT_LINE_COUNT,this->lsLines);
            params->PutInt(EXT_FIELD_GAME_BET_VALUE,this->currBetMoney);
            params->PutInt(EXT_FIELD_GAME_IS_AUTO,1);
            boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_SLOT_AUTO_PLAY_REQ, params,lastRoom));
            GameServer::getSingleton().Send(req);
            
            this->bgChooseLine->setVisible(false);
            
            
        }
        if (this->autoPlay)
            this->imgAutoPlay->loadTexture(PATH_SLOT+"auto-play-min.png");
        else
            this->imgAutoPlay->loadTexture(PATH_SLOT+"check-p-min.png");
    }
}
void LayerPlayGameSlotMachine::onBtnBet100(Ref *sender, ui::Widget::TouchEventType pType) {
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
void LayerPlayGameSlotMachine::onBtnBet1K(Ref *sender, ui::Widget::TouchEventType pType) {
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
void LayerPlayGameSlotMachine::onBtnBet10K(Ref *sender, ui::Widget::TouchEventType pType) {
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
void LayerPlayGameSlotMachine::startRound() {
    if (mUtils::isSoundOn()){
        effect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("bm-slot.mp3");
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
    }
    //    this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::startRound1, this)), DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::startRound2, this)), DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::startRound3, this)), DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::startRound4, this)), DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::startRound5, this)),NULL));
    //
    this->startRound1();
    this->startRound2();
    this->startRound3();
    this->startRound4();
    this->startRound5();
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameSlotMachine::updateTimerStart),this,1, false);
}
void LayerPlayGameSlotMachine::updateTimerStart(float dt){
    timerStart+=1;
    
    if (timerStart>20){
        this->resetGame();
        this->txtTienThang->setString("");
        for (int i = 0 ; i< this->lstDrawLine.size();i++){
            this->lstDrawLine[i]->removeFromParent();
        }
        this->bgChooseLine->setVisible(false);
        
        this->lstDrawLine.clear();
        this->round1->resetRound();
        this->round2->resetRound();
        this->round3->resetRound();
        this->round4->resetRound();
        this->round5->resetRound();
        auto toast = Chat::create(dataManager.GetSysString(49),-1);
        this->addChild(toast);
        this->showResultPrevMatch("4,7,9,7,7,|5,9,4,3,2,|3,6,2,3,4,|");
        
        auto _currScene = Director::getInstance()->getRunningScene();
        if (_currScene) {
            if (_currScene->getChildByTag(tag_SceneLogin) != NULL){
                return;
            }
        }
        if (_currScene->getChildByTag(19) != NULL)
            return;
        _currScene->removeChildByTag(1504);
        
        LayerPopupReconnect* layerReconn = LayerPopupReconnect::create();
        _currScene->addChild(layerReconn, ZORDER_LIST::ZORDER_POPUP_RECONNECT, 9019);
        layerReconn->showReconnect();
    }
}
void LayerPlayGameSlotMachine::stopRound(string str){
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameSlotMachine::updateTimerStart),this);
    auto lstRow = mUtils::splitString(str, '|');
    if (lstRow.size()<3) {
        this->resetGame();
        this->txtTienThang->setString("");
        this->showResultPrevMatch("4,7,9,7,7,|5,9,4,3,2,|3,6,2,3,4,|");
        for (int i = 0 ; i< this->lstDrawLine.size();i++){
            this->lstDrawLine[i]->removeFromParent();
        }
        this->bgChooseLine->setVisible(false);
        
        this->lstDrawLine.clear();
        this->round1->resetRound();
        this->round2->resetRound();
        this->round3->resetRound();
        this->round4->resetRound();
        this->round5->resetRound();
        auto toast = Chat::create(dataManager.GetSysString(49),-1);
        this->addChild(toast);
        return;
    }
    
    auto lstRow1 = mUtils::splitString(lstRow[0], ',');
    auto lstRow2 = mUtils::splitString(lstRow[1], ',');
    auto lstRow3 = mUtils::splitString(lstRow[2], ',');
    
    if (lstRow1.size()<5||lstRow2.size()<5||lstRow2.size()<5){
        this->resetGame();
        this->txtTienThang->setString("");
        this->showResultPrevMatch("4,7,9,7,7,|5,9,4,3,2,|3,6,2,3,4,|");
        for (int i = 0 ; i< this->lstDrawLine.size();i++){
            this->lstDrawLine[i]->removeFromParent();
        }
        this->bgChooseLine->setVisible(false);
        
        this->lstDrawLine.clear();
        this->round1->resetRound();
        this->round2->resetRound();
        this->round3->resetRound();
        this->round4->resetRound();
        this->round5->resetRound();
        auto toast = Chat::create(dataManager.GetSysString(49),-1);
        this->addChild(toast);
        return;
    }
    
    string strs = "";
    for (int i = 0; i<5;i++){
        strs+=lstRow1[i]+"_"+lstRow2[i]+"_"+lstRow3[i]+";";
    }
    
    
    this->itemResult.clear();
    auto arrItem = mUtils::splitString(strs,';');
    if (arrItem.size() < 5){
        this->resetGame();
        this->txtTienThang->setString("");
        this->showResultPrevMatch("4,7,9,7,7,|5,9,4,3,2,|3,6,2,3,4,|");
        for (int i = 0 ; i< this->lstDrawLine.size();i++){
            this->lstDrawLine[i]->removeFromParent();
        }
        this->bgChooseLine->setVisible(false);
        
        this->lstDrawLine.clear();
        this->round1->resetRound();
        this->round2->resetRound();
        this->round3->resetRound();
        this->round4->resetRound();
        this->round5->resetRound();
        auto toast = Chat::create(dataManager.GetSysString(49),-1);
        this->addChild(toast);
        return;
    }
    auto k = arrItem.size();
    for (int i = 0; i < k; i++) {
        this->itemResult.push_back(arrItem[i]);
    }
    if (this->itemResult.size() < 5)
        return;
    if (timerStart < 1.5){
        //this->round1->stopRound(this->arrTagCard[0]);
        this->runAction(Sequence::create(DelayTime::create(1.5-timerStart),CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopRound1,this,this->itemResult[0])), DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopRound2, this,this->itemResult[1])), DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopRound3, this,this->itemResult[2])), DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopRound4, this,this->itemResult[3])), DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopRound5, this,this->itemResult[4])), DelayTime::create(0.2),  CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::showResult, this)),CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::showWinLine, this)),NULL));
        
    }else{
        this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopRound1,this,this->itemResult[0])), DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopRound2, this,this->itemResult[1])), DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopRound3, this,this->itemResult[2])), DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopRound4, this,this->itemResult[3])), DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopRound5, this,this->itemResult[4])), DelayTime::create(0.5),  CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::showResult, this)), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::showWinLine, this)),NULL));
    }
    timerStart = 0;
}
void LayerPlayGameSlotMachine::startRound1() {
    this->round1->startRound();
}
void LayerPlayGameSlotMachine::startRound2() {
    this->round2->startRound();
}
void LayerPlayGameSlotMachine::startRound3() {
    this->round3->startRound();
}
void LayerPlayGameSlotMachine::startRound4() {
    this->round4->startRound();
}
void LayerPlayGameSlotMachine::startRound5() {
    this->round5->startRound();
}
void LayerPlayGameSlotMachine::stopRound1(string tag) {
    this->round1->stopRound(tag);
    //sender.round2.stopRound(this->arrTagCard[1]);
}
void LayerPlayGameSlotMachine::stopRound2(string tag) {
    this->round2->stopRound(tag);
    // sender.round3.stopRound(this->arrTagCard[2]);
}
void LayerPlayGameSlotMachine::stopRound3(string tag) {
    this->round3->stopRound(tag);
    // sender.round4.stopRound(this->arrTagCard[3]);
}
void LayerPlayGameSlotMachine::stopRound4(string tag) {
    this->round4->stopRound(tag);
    //sender.round5.stopRound(this->arrTagCard[4]);
}
void LayerPlayGameSlotMachine::stopRound5(string tag) {
    this->round5->stopRound(tag);
    //if (this->winMoney > 0){
    //    this->showResult();
    //}
    if (mUtils::isSoundOn()){
        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(effect);
    }
}
void LayerPlayGameSlotMachine::showResult() {
    //    if (this->currCodeRS>0) {
    //        auto imgRS = ImageView::create(this->getStrImageRS(this->currCodeRS));
    //        imgRS->setPosition(this->imgResult->getPosition());
    //        imgRS->setAnchorPoint(this->imgResult->getAnchorPoint());
    //        imgRS->setTag(this->TAG_RESULT_IMG);
    //        pnlBg->addChild(imgRS,10000000);
    //        imgRS->runAction(Sequence::create(ScaleTo::create(0.5,1.3),ScaleTo::create(0.5,1),DelayTime::create(2),FadeOut::create(0.2),RemoveSelf::create(true), NULL));
    //    }
    //
    //
    //
    //    string lucky = this->strlst7;
    //
    //
    //    if (this->winMoney <= 0) {
    //        this->btnPlay->setEnabled(true);
    //        this->resetGame();
    //        return;
    //    }
    //
    //    //string font_name = "font_win-font.fnt";
    //    string money = StringUtils::format("+%s",mUtils::convertMoneyEx(this->winMoney).c_str());
    //    Label* labelResult = Label::createWithTTF(money, "__Roboto-Bold.ttf", 45);
    //    labelResult->setColor(Color3B::YELLOW);
    //    labelResult->setAnchorPoint(Vec2(0.5,0.5));
    //
    //    labelResult->setPosition(Vec2(612,260));
    //    pnlBg->addChild(labelResult);
    //    labelResult->setTag(this->TAG_RESULT_MONEY);
    //    labelResult->runAction(Sequence::create(MoveBy::create(1,Vec2(0,50)),DelayTime::create(2),FadeOut::create(0.2),RemoveSelf::create(true), NULL));
    //
    //    this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::resetGame, this)),NULL));
    //    //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->currMyMoney));
    //    if (this->isNoHu)
    //        this->txtTongHu->setString(mUtils::convertMoneyEx(this->currHuThuong));
    //
    //    this->isNoHu = false;
    //
    
}
string LayerPlayGameSlotMachine::getStrImageRS(int rsc) {
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
void LayerPlayGameSlotMachine::playAnimationMachine() {
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
void LayerPlayGameSlotMachine::createAnimPlay() {
    // if (cc.animationCache._clear())
    //    cc.animationCache._clear();
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
    {
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 5; i++) {
            str = StringUtils::format("%d_play.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 122, 290));
            animFrames.pushBack(spriteFrame);
        }
        this->anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        this->anim->retain();
    }else{
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 9; i++) {
            str = StringUtils::format("%d_play.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 89, 580));
            animFrames.pushBack(spriteFrame);
        }
        this->anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        this->anim->retain();
    }
    
    
}
void LayerPlayGameSlotMachine::getMinMoney(){
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
void LayerPlayGameSlotMachine::playAnimStartGame() {
    // if (cc.animationCache._clear)
    
    this->sprRob->setAnchorPoint(Vec2(0, 0.5));
    Animate* animN = Animate::create(this->anim);
    this->bgChooseLine->setVisible(false);
    this->sprRob->runAction(Sequence::create(animN,CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::startRound, this)),NULL));
}
void LayerPlayGameSlotMachine::resetGame() {
    this->CurrentGameState = false;
    this->stopAllActions();
    this->strLineWin = "";
    this->strResult = "";
    //this->winMoney = 0;
    this->itemResult.clear();
    this->currCodeRS = 0;
    this->btnPlay->setEnabled(true);
    this->Animating = false;
    this->removeChildByTag(this->TAG_RESULT_IMG);
    this->removeChildByTag(this->TAG_RESULT_MONEY);
    this->bgWin->setVisible(false);
    this->currShow = 0;
    this->currLineDraws.clear();
    this->currLines.clear();
    this->currItems.clear();
    this->bgNoHu->setVisible(false);
    this->moneyNoHu = 0;
    this->lineNoHu= "";
    for (int i = 0 ; i< this->lstDrawLine.size();i++){
        this->lstDrawLine[i]->removeFromParent();
    }
    this->lstDrawLine.clear();
    
    this->round1->resetRoundToshow();
    this->round2->resetRoundToshow();
    this->round3->resetRoundToshow();
    this->round4->resetRoundToshow();
    this->round5->resetRoundToshow();
    this->stopNoHuAnimmation();
    
}
bool LayerPlayGameSlotMachine::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
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
void LayerPlayGameSlotMachine::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if (0 != cmd->compare(EXT_EVENT_SLOT_GAME_START)
        && 0 != cmd->compare(EXT_EVENT_GAME_ROTATE_RES)
        && 0 != cmd->compare(EXT_EVENT_GAME_INFO_NTF)
        && 0 != cmd->compare(EXT_EVENT_GAME_AUTO_RES)
        && 0 != cmd->compare(EXT_EVENT_GET_JACKPOT_RES)
        && 0 != cmd->compare(EXT_EVENT_CHANGE_JACKPOT_NTF)
        && 0 != cmd->compare(EXT_EVENT_EAT_JACKPOT_NTF)){
        return;
    }
    pObjs->PutUtfString("_cmd_", *cmd);
    this->mQueueMsg->addQueue(pObjs);
}
void LayerPlayGameSlotMachine::OnExtensionResponseProcess(){
    this->param = this->mQueueMsg->frontQueue();
    boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
    
    if (0 == cmd->compare(EXT_EVENT_SLOT_GAME_START)) {
        this->event_EXT_EVENT_GAME_START();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_ROTATE_RES)){
        this->event_EXT_EVENT_GAME_ROTATE_RES();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_INFO_NTF)){
        this->event_EXT_FIELD_GAME_INFO();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_AUTO_RES)){
        this->event_EXT_EVENT_GAME_AUTO_RES();
    }
    else if (0 == cmd->compare(EXT_EVENT_GET_JACKPOT_RES)){
        auto huthuong = this->param->GetLong(EXT_FIELD_JACKPOT_VALUE);
        //EXT_FIELD_GAME_BET_VALUE,this->currBetMoney
        auto betvl = this->param->GetInt(EXT_FIELD_GAME_BET_VALUE);
        if (betvl){
            if (*betvl == this->currBetMoney){
                if (huthuong){
                    if (this->txtMoneyHuThuong){
                        this->txtMoneyHuThuong->setString(mUtils::convertMoneyEx(*huthuong));
                    }
                }
            }
        }
        
    }
    else if (0 == cmd->compare(EXT_EVENT_CHANGE_JACKPOT_NTF)){
        auto huthuong = this->param->GetLong(EXT_FIELD_JACKPOT_VALUE);
        //EXT_FIELD_GAME_BET_VALUE,this->currBetMoney
        auto betvl = this->param->GetInt(EXT_FIELD_GAME_BET_VALUE);
        if (betvl){
            if (*betvl == this->currBetMoney){
                if (huthuong){
                    if (this->txtMoneyHuThuong){
                        this->txtMoneyHuThuong->setString(mUtils::convertMoneyEx(*huthuong));
                    }
                }
            }
        }
    }
    else if (0 == cmd->compare(EXT_EVENT_EAT_JACKPOT_NTF)){
        auto line = this->param->GetUtfString(EXT_FIELD_SLOT_JACKPOT_LINE);
        auto money = this->param->GetDouble(EXT_FIELD_AMOUNT_MONEY);
        if (money)
            this->moneyNoHu = *money;
        if (line)
            this->lineNoHu=*line;
    }
    this->mQueueMsg->deQueue();
}
void LayerPlayGameSlotMachine::event_EXT_EVENT_GAME_AUTO_RES(){
    boost::shared_ptr<long> isAuto = this->param->GetInt("isAuto");
    
    boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);
    if (errc != NULL){
        int ec = *errc;
        if (ec == 1){
            auto toast = Chat::create(dataManager.GetSysString(82),-1);
            this->addChild(toast);
            this->autoPlay = false;
            this->imgAutoPlay->loadTexture(PATH_SLOT+"check-p-min.png");
            return;
        }
    }
    
    if (isAuto){
        this->autoPlay = *isAuto ? 1 : 0;
        if (this->autoPlay)
            this->imgAutoPlay->loadTexture(PATH_SLOT+"auto-play-min.png");
        else
            this->imgAutoPlay->loadTexture(PATH_SLOT+"check-p-min.png");
    }
    
}
void LayerPlayGameSlotMachine::event_EXT_FIELD_GAME_INFO(){
    this->isJoin = true;
    this->round1->setVisible(true);
    this->round2->setVisible(true);
    this->round3->setVisible(true);
    this->round4->setVisible(true);
    this->round5->setVisible(true);
    boost::shared_ptr<string> ginfo = this->param->GetUtfString(EXT_FIELD_GAME_INFO_VQ);
    boost::shared_ptr<string> slsid = this->param->GetUtfString(EXT_FIELD_SLOT_SESSION_ID);
    boost::shared_ptr<string> slbt = this->param->GetUtfString(EXT_FIELD_SLOT_BET_LIST);
    if (this->currTypeMoney == 0){
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        
        if (mySelf != NULL){
            if (mySelf->GetVariable("amf") != NULL) {
                auto myMoney = *mySelf->GetVariable("amf")->GetDoubleValue();
                this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
                this->currMyMoney = myMoney;
            }
        }
    }else{
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        
        if (mySelf != NULL){
            if (mySelf->GetVariable("amfs") != NULL) {
                auto myMoney = *mySelf->GetVariable("amfs")->GetDoubleValue();
                this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
                this->currMyMoney = myMoney;
            }
        }
    }
    this->timerStart = 0;
    if (ginfo){
        if (0 != ginfo->compare(""))
            this->showResultPrevMatch(*ginfo);
        else
            this->showResultPrevMatch("4,7,9,7,7,|5,9,4,3,2,|3,6,2,3,4,|");
       // log("result: %s",ginfo->c_str());
    }else{
        this->showResultPrevMatch("4,7,9,7,7,|5,9,4,3,2,|3,6,2,3,4,|");
    }
    if (slsid){
        this->txtPhien->setString(StringUtils::format("#%s",slsid->c_str()));
    }
    if (slbt){
        this->lstBet.clear();
        auto lstBet = mUtils::splitString(*slbt,',');
        for (int i = 0 ; i < lstBet.size(); i++){
            this->lstBet.push_back(atoi(lstBet[i].c_str()));
        }
        this->currPosBet = 0;
        this->currBetMoney = this->lstBet[this->currPosBet];
        this->txtTongDat->setString(StringUtils::format("%s: %s",dataManager.GetSysString(886).c_str(),mUtils::convertMoneyEx(long(this->currBetMoney*this->countLine)).c_str()));
        this->txtMucCuoc->setString(mUtils::formatMoneySymbol(long(this->currBetMoney)));
    }
    this->autoPlay = false;
    this->btnAutoPlay->setEnabled(true);
    this->imgAutoPlay->loadTexture(PATH_SLOT+"check-p-min.png");
    this->getHuThuong(0);
    SceneManager::getSingleton().hideLoading();

}
void LayerPlayGameSlotMachine::event_EXT_EVENT_GAME_ROTATE_RES(){
    this->timerStart = 0;
    boost::shared_ptr<string> ginfo = this->param->GetUtfString(EXT_FIELD_GAME_INFO_VQ);
    boost::shared_ptr<string> slwll = this->param->GetUtfString(EXT_FIELD_SLOT_WIN_LINE_LIST);
    auto slwvl = this->param->GetDouble(EXT_FIELD_SLOT_WIN_VALUE);
    boost::shared_ptr<string> slsid = this->param->GetUtfString(EXT_FIELD_SLOT_SESSION_ID);
    if (slsid){
        this->txtPhien->setString(StringUtils::format("#%s",slsid->c_str()));
    }
    if (ginfo){
        this->stopRound(*ginfo);
        //this->stopRound("9,5,2,8,9,|3,3,3,5,7,|7,7,9,8,7,|");
        
        
       // log("result: %s",ginfo->c_str());
    }
    if (slwll){
       // log("line win: %s",slwll->c_str());
        this->strLineWin = *slwll;
        //this->strLineWin = "1;3, 3, 3, 0, 0|";
    }
    if (slwll){
        this->winvalue = *slwvl;
        if (this->winvalue < 0)
            this->winvalue = 0;
        if (this->currTypeMoney == 0){
            if (this->winvalue > dataManager.getAppConfig().notifyZen){
                
                string msg = "";
                msg = StringUtils::format("%s|Slot Machine|%s|%s|%s|%s|%s/%s|Slot Machine|%s|%s|%s|%s|%s/%s|Slot Machine|%s|%s|%s|%s|%s/%s|Slot Machine|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(983).c_str(),mUtils::convertMoneyEx(this->winvalue).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9831).c_str(),mUtils::convertMoneyEx(this->winvalue).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9832).c_str(),mUtils::convertMoneyEx(this->winvalue).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9833).c_str(),mUtils::convertMoneyEx(this->winvalue).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str());
                SceneManager::getSingleton().sendEventNotify(msg);
                
            }
        }else{
            if (this->winvalue > dataManager.getAppConfig().notifyGold){
                
                string msg = "";
                msg = StringUtils::format("%s|Slot Machine|%s|%s|%s|%s|%s/%s|Slot Machine|%s|%s|%s|%s|%s/%s|Slot Machine|%s|%s|%s|%s|%s/%s|Slot Machine|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(983).c_str(),mUtils::convertMoneyEx(this->winvalue).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9831).c_str(),mUtils::convertMoneyEx(this->winvalue).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9832).c_str(),mUtils::convertMoneyEx(this->winvalue).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9833).c_str(),mUtils::convertMoneyEx(this->winvalue).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str());
                SceneManager::getSingleton().sendEventNotify(msg);
            }
        }
    }
   // log("abc");
}
void LayerPlayGameSlotMachine::resetLine(int currLine){
    int count = 0;
    for (int i = 0 ; i< this->lstDrawLine.size();i++){
        this->lstDrawLine[i]->removeFromParent();
    }
    this->lstDrawLine.clear();

    this->round1->resetRoundToshow();
    this->round2->resetRoundToshow();
    this->round3->resetRoundToshow();
    this->round4->resetRoundToshow();
    this->round5->resetRoundToshow();
}
void LayerPlayGameSlotMachine::pushItemEat1(int item,int row){
    this->round1->pushItemEat(item,row);
}
void LayerPlayGameSlotMachine::pushItemEat2(int item,int row){
    this->round2->pushItemEat(item,row);

}
void LayerPlayGameSlotMachine::pushItemEat3(int item,int row){
    this->round3->pushItemEat(item,row);

}
void LayerPlayGameSlotMachine::pushItemEat4(int item,int row){
    this->round4->pushItemEat(item,row);

}
void LayerPlayGameSlotMachine::pushItemEat5(int item,int row){
    this->round5->pushItemEat(item,row);

}
void LayerPlayGameSlotMachine::animSlotMachineWinLine(){
    for (int i = 0; i < this->currLineDraws.size();i++){
        this->runAction(Sequence::create(DelayTime::create(2.5*i),
                                         CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::drawLineNew, this,this->currLineDraws[i],this->currLines[i])),
                                         CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::pushItemEat1, this,this->currItems[i][0],this->currLines[i][0])),
                                         CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::pushItemEat2, this,this->currItems[i][1],this->currLines[i][1])),
                                         CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::pushItemEat3, this,this->currItems[i][2],this->currLines[i][2])),
                                         CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::pushItemEat4, this,this->currItems[i][3],this->currLines[i][3])),
                                         CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::pushItemEat5, this,this->currItems[i][4],this->currLines[i][4])),
                                         DelayTime::create(2),
                                         CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::resetLine, this,this->currLineDraws[i])),NULL));
    }
    this->currShow++;
    CallFunc* call = CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::animSlotMachineWinLine, this));
    this->runAction(Sequence::create(DelayTime::create(2.5*this->currLineDraws.size()),call, NULL));
}
void LayerPlayGameSlotMachine::showWinLine(){
    //test
    //this->strLineWin = "4;9, 9, 100, 0, 0|9;9, 9, 100, 0, 0|16;9, 9, 100, 0, 0|";
    auto lstLine = mUtils::splitString(this->strLineWin,'|');
    for (int i = 0;i<lstLine.size();i++){
        auto it = mUtils::splitString(lstLine[i],';');
        if (it.size()<2) continue;

        auto lstItem = mUtils::splitString(it[1],',');
        if (lstItem.size()<5) continue;
        int row1 = 1;
        int row2 = 1;
        int row3 = 1;
        int row4 = 1;
        int row5 = 1;
        
        switch (atoi(it[0].c_str())) {
            case 1:
                row1 = 2;
                row2 = 2;
                row3 = 2;
                row4 = 2;
                row5 = 2;
                
                break;
            case 2:
                row1 = 3;
                row2 = 3;
                row3 = 3;
                row4 = 3;
                row5 = 3;
                
                break;
            case 3:
                row1 = 1;
                row2 = 1;
                row3 = 1;
                row4 = 1;
                row5 = 1;
                break;
            case 4:
                row1 = 3;
                row2 = 2;
                row3 = 3;
                row4 = 2;
                row5 = 3;
                
                break;
            case 5:
                row1 = 1;
                row2 = 2;
                row3 = 1;
                row4 = 2;
                row5 = 1;
                
                break;
            case 6:
                row1 = 2;
                row2 = 3;
                row3 = 2;
                row4 = 3;
                row5 = 2;
                
                break;
            case 7:
                row1 = 2;
                row2 = 1;
                row3 = 2;
                row4 = 1;
                row5 = 2;
                
                break;
            case 8:
                row1 = 3;
                row2 = 2;
                row3 = 1;
                row4 = 2;
                row5 = 3;
                
                break;
            case 9:
                row1 = 1;
                row2 = 2;
                row3 = 3;
                row4 = 2;
                row5 = 1;
                
                break;
            case 10:
                row1 = 3;
                row2 = 3;
                row3 = 3;
                row4 = 3;
                row5 = 2;
                
                break;
            case 11:
                row1 = 2;
                row2 = 2;
                row3 = 1;
                row4 = 2;
                row5 = 2;
                
                break;
            case 12:
                row1 = 2;
                row2 = 3;
                row3 = 2;
                row4 = 1;
                row5 = 2;
                
                break;

            case 13:
                row1 = 2;
                row2 = 1;
                row3 = 2;
                row4 = 3;
                row5 = 2;
                
                break;
            case 14:
                row1 = 1;
                row2 = 1;
                row3 = 1;
                row4 = 1;
                row5 = 2;
                
                break;
            case 15:
                row1 = 2;
                row2 = 3;
                row3 = 3;
                row4 = 3;
                row5 = 3;
                
                break;
            case 16:
                row1 = 2;
                row2 = 2;
                row3 = 3;
                row4 = 2;
                row5 = 2;
                
                break;
            case 17:
                row1 = 2;
                row2 = 1;
                row3 = 1;
                row4 = 1;
                row5 = 1;
            
                break;
            default:
                break;
        }
        
        
        this->currLineDraws.push_back(atoi(it[0].c_str()));
        this->currLines.push_back({row1,row2,row3,row4,row5});
        this->currItems.push_back({atoi(lstItem[0].c_str()),atoi(lstItem[1].c_str()),atoi(lstItem[2].c_str()),atoi(lstItem[3].c_str()),atoi(lstItem[4].c_str())});
        
        this->drawLineNew(atoi(it[0].c_str()),{row1,row2,row3,row4,row5});
        
        this->round1->pushItemEat(atoi(lstItem[0].c_str()),row1);
        this->round2->pushItemEat(atoi(lstItem[1].c_str()),row2);
        this->round3->pushItemEat(atoi(lstItem[2].c_str()),row3);
        this->round4->pushItemEat(atoi(lstItem[3].c_str()),row4);
        this->round5->pushItemEat(atoi(lstItem[4].c_str()),row5);

    }

    if (this->currLineDraws.size()>0){
        this->animSlotMachineWinLine();
    }

    this->CurrentGameState = false;
    //this->winvalue+= this->moneyNoHu;
    this->txtTienThang->setString(StringUtils::format("%s: %s",dataManager.GetSysString(660).c_str(),mUtils::convertMoneyEx(long(this->winvalue)).c_str()));
    //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->currMyMoney));
    if (this->moneyNoHu>0){
        this->bgNoHu->setVisible(true);
        this->bgNoHu->runAction(Sequence::create(DelayTime::create(6),CallFunc::create([this](){this->bgNoHu->setVisible(false);}),CallFunc::create(CC_CALLBACK_0(LayerPlayGameSlotMachine::stopNoHuAnimmation, this)),NULL));
        
        this->txtMoneyNoHu->setString(mUtils::convertMoneyEx(this->moneyNoHu));
        auto it = mUtils::splitString(this->lineNoHu, ',');
        for (int i = 0; i < it.size();i++){
            this->lstItemNoHu[i]->loadTexture(StringUtils::format("design/ResSlot/item/%d.png",atoi(it[i].c_str())));
        }
        
        auto emitter = ParticleSystemQuad::create("particles/coin_water.plist");
        auto visibleSize = Director::getInstance()->getVisibleSize();
        
        emitter->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
        emitter->setAutoRemoveOnFinish(true);
        //emitter->setScale(3);
        emitter->setTag(278122);
        emitter->setAutoRemoveOnFinish(true);
        emitter->setScale(2.5);
        emitter->setLife(3);
        if (this->currTypeMoney == 0){
            TextureCache* tc = Director::getInstance()->getTextureCache();
            Texture2D* coin = tc->addImage(PATH_SLOT+"dongmoc-min.png");
            emitter->setTexture(coin);
        }
        else{
            TextureCache* tc = Director::getInstance()->getTextureCache();
            Texture2D* coin = tc->addImage(PATH_SLOT+"donghao-min.png");
            emitter->setTexture(coin);
        }
        this->pnlBg->addChild(emitter, 10000);
        
        auto emitter2 = ParticleSystemQuad::create("particles/coin_water.plist");
        
        emitter2->setPosition(Vec2(visibleSize.width+100,visibleSize.height/2));
        emitter2->setAutoRemoveOnFinish(true);
        //emitter->setScale(3);
        emitter2->setTag(278129);
        emitter2->setAutoRemoveOnFinish(true);
        emitter2->setScale(2.5);
        emitter2->setLife(3);
        if (this->currTypeMoney == 0){
            TextureCache* tc = Director::getInstance()->getTextureCache();
            Texture2D* coin = tc->addImage(PATH_SLOT+"dongmoc-min.png");
            emitter2->setTexture(coin);
        }
        else{
            TextureCache* tc = Director::getInstance()->getTextureCache();
            Texture2D* coin = tc->addImage(PATH_SLOT+"donghao-min.png");
            emitter2->setTexture(coin);
        }
        this->pnlBg->addChild(emitter2, 10000);
        
        auto emitter3 = ParticleSystemQuad::create("particles/coin_water.plist");
        
        emitter3->setPosition(Vec2(-100,visibleSize.height/2));
        emitter3->setAutoRemoveOnFinish(true);
        //emitter->setScale(3);
        emitter3->setTag(278124);
        emitter3->setAutoRemoveOnFinish(true);
        emitter3->setScale(2.5);
        emitter3->setLife(3);
        if (this->currTypeMoney == 0){
            TextureCache* tc = Director::getInstance()->getTextureCache();
            Texture2D* coin = tc->addImage(PATH_SLOT+"dongmoc-min.png");
            emitter3->setTexture(coin);
        }
        else{
            TextureCache* tc = Director::getInstance()->getTextureCache();
            Texture2D* coin = tc->addImage(PATH_SLOT+"donghao-min.png");
            emitter3->setTexture(coin);
        }
        this->pnlBg->addChild(emitter3, 10000);
        
        
        
    }
    if (this->winvalue> 0){
        if (this->moneyNoHu<=0){
            this->bgWin->setVisible(true);
            this->bgWin->runAction(Sequence::create(DelayTime::create(4),CallFunc::create([this](){this->bgWin->setVisible(false);}), NULL));
            this->txtMoneyWinBig->setString(StringUtils::format("WIN %s",mUtils::convertMoneyEx(long(this->winvalue)).c_str()));
        }
        LayerWinSlot* win = LayerWinSlot::create();
        win->setCallBack(this, callfuncO_selector(LayerPlayGameSlotMachine::onHieuUngCallBack));
        win->setRoomType(this->currTypeMoney);
        this->addChild(win);
        win->startAnim();
        auto emitter = ParticleSystemQuad::create("particles/coin_water.plist");
        auto visibleSize = Director::getInstance()->getVisibleSize();
        
        emitter->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
        emitter->setAutoRemoveOnFinish(true);
        //emitter->setScale(3);
        emitter->setTag(278123);
        emitter->setAutoRemoveOnFinish(true);
        emitter->setScale(2.5);
        emitter->setLife(3);
        if (this->currTypeMoney == 0){
            TextureCache* tc = Director::getInstance()->getTextureCache();
            Texture2D* coin = tc->addImage(PATH_SLOT+"dongmoc-min.png");
            emitter->setTexture(coin);
        }
        else{
            TextureCache* tc = Director::getInstance()->getTextureCache();
            Texture2D* coin = tc->addImage(PATH_SLOT+"donghao-min.png");
            emitter->setTexture(coin);
        }
        this->pnlBg->addChild(emitter, 10000);
    }
    else{
        this->winMoney = 0;
        this->winvalue = 0;
        this->winvalue2 = 0;
        this->Animating = false;
        if (this->currTypeMoney == 0){
            boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
            
            if (mySelf != NULL){
                if (mySelf->GetVariable("amf") != NULL) {
                    auto myMoney = *mySelf->GetVariable("amf")->GetDoubleValue();
                    this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
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
        this->winMoney = 0;
        this->winvalue = 0;
        this->winvalue2 = 0;
        this->Animating = false;
        string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
        this->txtMyMoney->setString(_money);
        ParticleSystemQuad* ps = (ParticleSystemQuad*)this->pnlBg->getChildByTag(278123);
        if (ps){
            ps->stop();
        }
    }
    this->btnPlay->setEnabled(true);
    this->btnMax->setEnabled(true);
    this->btnMulti->setEnabled(true);
    if (this->currPosBet == this->lstBet.size()-1){
        this->btnBetUp->setEnabled(false);
        this->btnBetUp->setBright(false);
        
        this->btnBetDown->setEnabled(true);
        this->btnBetDown->setBright(true);
    }
    if (this->currPosBet == 0){
        this->btnBetUp->setEnabled(true);
        this->btnBetUp->setBright(true);
        
        this->btnBetDown->setEnabled(false);
        this->btnBetDown->setBright(false);
    }
    if (this->currPosBet < this->lstBet.size()-1 && this->currPosBet > 0){
        this->btnBetUp->setEnabled(true);
        this->btnBetUp->setBright(true);
        
        this->btnBetDown->setEnabled(true);
        this->btnBetDown->setBright(true);
    }
    
    if (this->countLine == 1){
        this->btnLineBack->setEnabled(false);
        this->btnLineBack->setBright(false);
        this->btnLineNext->setEnabled(true);
        this->btnLineNext->setBright(true);
    }
    if (this->countLine == this->GameLine){
        this->btnLineBack->setEnabled(true);
        this->btnLineBack->setBright(true);
        this->btnLineNext->setEnabled(false);
        this->btnLineNext->setBright(false);
    }
    if (this->countLine > 1 && this->countLine < this->GameLine){
        this->btnLineBack->setEnabled(true);
        this->btnLineBack->setBright(true);
        this->btnLineNext->setEnabled(true);
        this->btnLineNext->setBright(true);
    }
    
    
    
    if (this->currTypeMoney == 0){
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        
        if (mySelf != NULL){
            if (mySelf->GetVariable("amf") != NULL) {
                auto myMoney = *mySelf->GetVariable("amf")->GetDoubleValue();
                if (this->winvalue <=0)
                    this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
                this->lastMoney = myMoney;
            }
        }
    }else{
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        
        if (mySelf != NULL){
            if (mySelf->GetVariable("amfs") != NULL) {
                auto myMoney = *mySelf->GetVariable("amfs")->GetDoubleValue();
                if (this->winvalue <=0)
                    this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
                this->lastMoney = myMoney;
            }
        }
    }
    
}
void LayerPlayGameSlotMachine::stopNoHuAnimmation(){
    ParticleSystemQuad* ps1 = (ParticleSystemQuad*)this->pnlBg->getChildByTag(278122);
    if (ps1){
        ps1->stop();
    }
    ParticleSystemQuad* ps2 = (ParticleSystemQuad*)this->pnlBg->getChildByTag(278129);
    if (ps2){
        ps2->stop();
    }
    ParticleSystemQuad* ps3 = (ParticleSystemQuad*)this->pnlBg->getChildByTag(278124);
    if (ps3){
        ps3->stop();
    }
}
void LayerPlayGameSlotMachine::drawLineNew(int line, vector<int> item){
    //new draw
    
    vector<float> posY {this->lstlineLeft[2]->getPositionY(),this->lstlineLeft[0]->getPositionY(),this->lstlineLeft[1]->getPositionY()};
    
    auto fromPos = Vec2(this->lstlineLeft[line-1]->getPositionX()+15,this->lstlineLeft[line-1]->getPositionY());
    auto toPos = Vec2(this->lstlineRight[line-1]->getPositionX()-15,this->lstlineRight[line-1]->getPositionY());
    
    auto posMiddle1 = Vec2(this->lstpointXLines[0]->getPositionX(),posY[item[0]-1]);
    auto posMiddle2 = Vec2(this->lstpointXLines[1]->getPositionX(),posY[item[1]-1]);
    auto posMiddle3 = Vec2(this->lstpointXLines[2]->getPositionX(),posY[item[2]-1]);
    auto posMiddle4 = Vec2(this->lstpointXLines[3]->getPositionX(),posY[item[3]-1]);
    auto posMiddle5 = Vec2(this->lstpointXLines[4]->getPositionX(),posY[item[4]-1]);
    
    Color3B color = this->colors[line-1];
    
    auto drawChart1 = DrawNode::create();
    drawChart1->drawSegment(fromPos, posMiddle1,3,Color4F(color,1));
    this->pnlBg->addChild(drawChart1,3);
    this->lstDrawLine.push_back(drawChart1);
    
    auto drawChart2 = DrawNode::create();
    drawChart2->drawSegment(posMiddle1, posMiddle2,3,Color4F(color,1));
    this->pnlBg->addChild(drawChart2,3);
    this->lstDrawLine.push_back(drawChart2);
    
    auto drawChart3 = DrawNode::create();
    drawChart3->drawSegment(posMiddle2, posMiddle3,3,Color4F(color,1));
    this->pnlBg->addChild(drawChart3,3);
    this->lstDrawLine.push_back(drawChart3);
    
    auto drawChart4 = DrawNode::create();
    drawChart4->drawSegment(posMiddle3, posMiddle4,3,Color4F(color,1));
    this->pnlBg->addChild(drawChart4,3);
    this->lstDrawLine.push_back(drawChart4);
    
    auto drawChart5 = DrawNode::create();
    drawChart5->drawSegment(posMiddle4, posMiddle5,3,Color4F(color,1));
    this->pnlBg->addChild(drawChart5,3);
    this->lstDrawLine.push_back(drawChart5);
    
    auto drawChart6 = DrawNode::create();
    drawChart6->drawSegment(posMiddle5, toPos,3,Color4F(color,1));
    this->pnlBg->addChild(drawChart6,3);
    this->lstDrawLine.push_back(drawChart6);
    
    drawChart1->setTag(line);
    drawChart2->setTag(line);
    drawChart3->setTag(line);
    drawChart4->setTag(line);
    drawChart5->setTag(line);
    drawChart6->setTag(line);

}
void LayerPlayGameSlotMachine::drawLine(int line){
    
    
  
    
    
    
    
    
    if (line ==1 ){
        auto fromPos = Vec2(this->lstlineLeft[line-1]->getPositionX()+15,this->lstlineLeft[line-1]->getPositionY());
        auto toPos = Vec2(this->lstlineRight[line-1]->getPositionX()-15,this->lstlineRight[line-1]->getPositionY());
        
        auto drawChart = DrawNode::create();
        drawChart->drawSegment(fromPos, toPos,3,Color4F::RED);
        this->pnlBg->addChild(drawChart,3);
        this->lstDrawLine.push_back(drawChart);
    }
    else if (line ==2 ){
        auto fromPos = Vec2(this->lstlineLeft[line-1]->getPositionX()+15,this->lstlineLeft[line-1]->getPositionY());
        auto toPos = Vec2(this->lstlineRight[line-1]->getPositionX()-15,this->lstlineRight[line-1]->getPositionY());
        
        auto drawChart = DrawNode::create();
        drawChart->drawSegment(fromPos, toPos,3,Color4F(Color3B(0,255,0),1));
        this->pnlBg->addChild(drawChart,3);
        this->lstDrawLine.push_back(drawChart);
    }
    else if (line ==3 ){
        auto fromPos = Vec2(this->lstlineLeft[line-1]->getPositionX()+15,this->lstlineLeft[line-1]->getPositionY());
        auto toPos = Vec2(this->lstlineRight[line-1]->getPositionX()-15,this->lstlineRight[line-1]->getPositionY());
        
        auto drawChart = DrawNode::create();
        drawChart->drawSegment(fromPos, toPos,3,Color4F(Color3B(1,4,255),1));
        this->pnlBg->addChild(drawChart,3);
        this->lstDrawLine.push_back(drawChart);
        
    }
    else if (line ==4 ){
        auto fromPos = Vec2(this->lstlineLeft[line-1]->getPositionX()+15,this->lstlineLeft[line-1]->getPositionY());
        auto toPos = Vec2(this->lstlineRight[line-1]->getPositionX()-15,this->lstlineRight[line-1]->getPositionY());
        
        auto posMiddle1 = Vec2(this->lstpointXLines[1]->getPositionX(),this->lstlineLeft[0]->getPositionY());
        auto posMiddle2 = Vec2(this->lstpointXLines[2]->getPositionX(),this->lstlineLeft[1]->getPositionY());
        auto posMiddle3 = Vec2(this->lstpointXLines[3]->getPositionX(),this->lstlineLeft[0]->getPositionY());
        
        auto drawChart1 = DrawNode::create();
        drawChart1->drawSegment(fromPos, posMiddle1,3,Color4F(Color3B(176,0,241),1));
        this->pnlBg->addChild(drawChart1,3);
        this->lstDrawLine.push_back(drawChart1);
        
        auto drawChart2 = DrawNode::create();
        drawChart2->drawSegment(posMiddle1, posMiddle2,3,Color4F(Color3B(176,0,241),1));
        this->pnlBg->addChild(drawChart2,3);
        this->lstDrawLine.push_back(drawChart2);
        
        auto drawChart3 = DrawNode::create();
        drawChart3->drawSegment(posMiddle2, posMiddle3,3,Color4F(Color3B(176,0,241),1));
        this->pnlBg->addChild(drawChart3,3);
        this->lstDrawLine.push_back(drawChart3);
        
        auto drawChart4 = DrawNode::create();
        drawChart4->drawSegment(posMiddle3, toPos,3,Color4F(Color3B(176,0,241),1));
        this->pnlBg->addChild(drawChart4,3);
        this->lstDrawLine.push_back(drawChart4);
    }
    else if (line ==5 ){
        auto fromPos = Vec2(this->lstlineLeft[line-1]->getPositionX()+15,this->lstlineLeft[line-1]->getPositionY());
        auto toPos = Vec2(this->lstlineRight[line-1]->getPositionX()-15,this->lstlineRight[line-1]->getPositionY());
        
        auto posMiddle1 = Vec2(this->lstpointXLines[1]->getPositionX(),this->lstlineLeft[0]->getPositionY());
        auto posMiddle2 = Vec2(this->lstpointXLines[2]->getPositionX(),this->lstlineLeft[4]->getPositionY());
        auto posMiddle3 = Vec2(this->lstpointXLines[3]->getPositionX(),this->lstlineLeft[0]->getPositionY());
        
        auto drawChart1 = DrawNode::create();
        drawChart1->drawSegment(fromPos, posMiddle1,3,Color4F(Color3B(255,0,255),1));
        this->pnlBg->addChild(drawChart1,3);
        this->lstDrawLine.push_back(drawChart1);
        
        auto drawChart2 = DrawNode::create();
        drawChart2->drawSegment(posMiddle1, posMiddle2,3,Color4F(Color3B(255,0,255),1));
        this->pnlBg->addChild(drawChart2,3);
        this->lstDrawLine.push_back(drawChart2);
        
        auto drawChart3 = DrawNode::create();
        drawChart3->drawSegment(posMiddle2, posMiddle3,3,Color4F(Color3B(255,0,255),1));
        this->pnlBg->addChild(drawChart3,3);
        this->lstDrawLine.push_back(drawChart3);
        
        auto drawChart4 = DrawNode::create();
        drawChart4->drawSegment(posMiddle3, toPos,3,Color4F(Color3B(255,0,255),1));
        this->pnlBg->addChild(drawChart4,3);
        this->lstDrawLine.push_back(drawChart4);
        
        
    }
    else if (line ==6 ){
        auto fromPos = Vec2(this->lstlineLeft[line-1]->getPositionX()+15,this->lstlineLeft[line-1]->getPositionY());
        auto toPos = Vec2(this->lstlineRight[line-1]->getPositionX()-15,this->lstlineRight[line-1]->getPositionY());
        
        auto posMiddle1 = Vec2(this->lstpointXLines[0]->getPositionX(),this->lstlineLeft[line-1]->getPositionY());
        auto posMiddle2 = Vec2(this->lstpointXLines[1]->getPositionX(),this->lstlineLeft[1]->getPositionY());
        auto posMiddle3 = Vec2(this->lstpointXLines[2]->getPositionX(),this->lstlineLeft[0]->getPositionY());
        auto posMiddle4 = Vec2(this->lstpointXLines[3]->getPositionX(),this->lstlineLeft[1]->getPositionY());
        auto posMiddle5 = Vec2(this->lstpointXLines[4]->getPositionX(),this->lstlineLeft[5]->getPositionY());
        
        auto drawChart1 = DrawNode::create();
        drawChart1->drawSegment(fromPos, posMiddle1,3,Color4F(Color3B(255,242,0),1));
        this->pnlBg->addChild(drawChart1,3);
        this->lstDrawLine.push_back(drawChart1);
        
        auto drawChart2 = DrawNode::create();
        drawChart2->drawSegment(posMiddle1, posMiddle2,3,Color4F(Color3B(255,242,0),1));
        this->pnlBg->addChild(drawChart2,3);
        this->lstDrawLine.push_back(drawChart2);
        
        auto drawChart3 = DrawNode::create();
        drawChart3->drawSegment(posMiddle2, posMiddle3,3,Color4F(Color3B(255,242,0),1));
        this->pnlBg->addChild(drawChart3,3);
        this->lstDrawLine.push_back(drawChart3);
        
        auto drawChart4 = DrawNode::create();
        drawChart4->drawSegment(posMiddle3, posMiddle4,3,Color4F(Color3B(255,242,0),1));
        this->pnlBg->addChild(drawChart4,3);
        this->lstDrawLine.push_back(drawChart4);
        
        auto drawChart5 = DrawNode::create();
        drawChart5->drawSegment(posMiddle4, posMiddle5,3,Color4F(Color3B(255,242,0),1));
        this->pnlBg->addChild(drawChart5,3);
        this->lstDrawLine.push_back(drawChart5);
        
        auto drawChart6 = DrawNode::create();
        drawChart6->drawSegment(posMiddle5, toPos,3,Color4F(Color3B(255,242,0),1));
        this->pnlBg->addChild(drawChart6,3);
        this->lstDrawLine.push_back(drawChart6);
    }
    
    else if (line ==7 ){
        auto fromPos = Vec2(this->lstlineLeft[line-1]->getPositionX()+15,this->lstlineLeft[line-1]->getPositionY());
        auto toPos = Vec2(this->lstlineRight[line-1]->getPositionX()-15,this->lstlineRight[line-1]->getPositionY());
        
        auto posMiddle1 = Vec2(this->lstpointXLines[0]->getPositionX(),this->lstlineLeft[line-1]->getPositionY());
        auto posMiddle2 = Vec2(this->lstpointXLines[1]->getPositionX(),this->lstlineLeft[4]->getPositionY()-15);
        auto posMiddle3 = Vec2(this->lstpointXLines[2]->getPositionX(),this->lstlineLeft[0]->getPositionY()-15);
        auto posMiddle4 = Vec2(this->lstpointXLines[3]->getPositionX(),this->lstlineLeft[4]->getPositionY());
        auto posMiddle5 = Vec2(this->lstpointXLines[4]->getPositionX(),this->lstlineLeft[line-1]->getPositionY());
        
        auto drawChart1 = DrawNode::create();
        drawChart1->drawSegment(fromPos, posMiddle1,3,Color4F(Color3B(246,150,121),1));
        this->pnlBg->addChild(drawChart1,3);
        this->lstDrawLine.push_back(drawChart1);
        
        auto drawChart2 = DrawNode::create();
        drawChart2->drawSegment(posMiddle1, posMiddle2,3,Color4F(Color3B(246,150,121),1));
        this->pnlBg->addChild(drawChart2,3);
        this->lstDrawLine.push_back(drawChart2);
        
        auto drawChart3 = DrawNode::create();
        drawChart3->drawSegment(posMiddle2, posMiddle3,3,Color4F(Color3B(246,150,121),1));
        this->pnlBg->addChild(drawChart3,3);
        this->lstDrawLine.push_back(drawChart3);
        
        auto drawChart4 = DrawNode::create();
        drawChart4->drawSegment(posMiddle3, posMiddle4,3,Color4F(Color3B(246,150,121),1));
        this->pnlBg->addChild(drawChart4,3);
        this->lstDrawLine.push_back(drawChart4);
        
        auto drawChart5 = DrawNode::create();
        drawChart5->drawSegment(posMiddle4, posMiddle5,3,Color4F(Color3B(246,150,121),1));
        this->pnlBg->addChild(drawChart5,3);
        this->lstDrawLine.push_back(drawChart5);
        
        auto drawChart6 = DrawNode::create();
        drawChart6->drawSegment(posMiddle5, toPos,3,Color4F(Color3B(246,150,121),1));
        this->pnlBg->addChild(drawChart6,3);
        this->lstDrawLine.push_back(drawChart6);
    }
    else if (line ==8 ){
        auto fromPos = Vec2(this->lstlineLeft[line-1]->getPositionX()+15,this->lstlineLeft[line-1]->getPositionY());
        auto toPos = Vec2(this->lstlineRight[line-1]->getPositionX()-15,this->lstlineRight[line-1]->getPositionY());
        
        auto posMiddle1 = Vec2(this->lstpointXLines[0]->getPositionX(),this->lstlineLeft[line-1]->getPositionY());
        auto posMiddle2 = Vec2(this->lstpointXLines[2]->getPositionX(),this->lstlineLeft[4]->getPositionY());
        auto posMiddle3 = Vec2(this->lstpointXLines[4]->getPositionX(),this->lstlineLeft[line-1]->getPositionY());
        
        auto drawChart1 = DrawNode::create();
        drawChart1->drawSegment(fromPos, posMiddle1,3,Color4F(Color3B(0,255,255),1));
        this->pnlBg->addChild(drawChart1,3);
        this->lstDrawLine.push_back(drawChart1);
        
        auto drawChart2 = DrawNode::create();
        drawChart2->drawSegment(posMiddle1, posMiddle2,3,Color4F(Color3B(0,255,255),1));
        this->pnlBg->addChild(drawChart2,3);
        this->lstDrawLine.push_back(drawChart2);
        
        auto drawChart3 = DrawNode::create();
        drawChart3->drawSegment(posMiddle2, posMiddle3,3,Color4F(Color3B(0,255,255),1));
        this->pnlBg->addChild(drawChart3,3);
        this->lstDrawLine.push_back(drawChart3);
        
        auto drawChart4 = DrawNode::create();
        drawChart4->drawSegment(posMiddle3, toPos,3,Color4F(Color3B(0,255,255),1));
        this->pnlBg->addChild(drawChart4,3);
        this->lstDrawLine.push_back(drawChart4);
        
    }
    else if (line ==9 ){
        auto fromPos = Vec2(this->lstlineLeft[line-1]->getPositionX()+15,this->lstlineLeft[line-1]->getPositionY());
        auto toPos = Vec2(this->lstlineRight[line-1]->getPositionX()-15,this->lstlineRight[line-1]->getPositionY());
        
        auto posMiddle1 = Vec2(this->lstpointXLines[0]->getPositionX(),this->lstlineLeft[line-1]->getPositionY());
        auto posMiddle2 = Vec2(this->lstpointXLines[2]->getPositionX(),this->lstlineLeft[1]->getPositionY());
        auto posMiddle3 = Vec2(this->lstpointXLines[4]->getPositionX(),this->lstlineLeft[line-1]->getPositionY());
        
        auto drawChart1 = DrawNode::create();
        drawChart1->drawSegment(fromPos, posMiddle1,3,Color4F::BLACK);
        this->pnlBg->addChild(drawChart1,3);
        this->lstDrawLine.push_back(drawChart1);
        
        auto drawChart2 = DrawNode::create();
        drawChart2->drawSegment(posMiddle1, posMiddle2,3,Color4F::BLACK);
        this->pnlBg->addChild(drawChart2,3);
        this->lstDrawLine.push_back(drawChart2);
        
        auto drawChart3 = DrawNode::create();
        drawChart3->drawSegment(posMiddle2, posMiddle3,3,Color4F::BLACK);
        this->pnlBg->addChild(drawChart3,3);
        this->lstDrawLine.push_back(drawChart3);
        
        auto drawChart4 = DrawNode::create();
        drawChart4->drawSegment(posMiddle3, toPos,3,Color4F::BLACK);
        this->pnlBg->addChild(drawChart4,3);
        this->lstDrawLine.push_back(drawChart4);
        
    }
}
void LayerPlayGameSlotMachine::event_EXT_EVENT_GAME_END(){
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

void LayerPlayGameSlotMachine::event_EXT_EVENT_GAME_AUTO_RESP(){
    boost::shared_ptr<long> number = this->param->GetInt(EXT_FIELD_GAME_AUTO_AMOUNT);
    boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);
    this->timerStart = 0;
    
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
void LayerPlayGameSlotMachine::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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
                        this->lastMoney = *amf;
                        this->winvalue2 = this->lastMoney-this->currMyMoney;
                        
                       // log("money slot 3 : %d",this->lastMoney);
                        
                        if (this->winvalue2 > 0 && this->winvalue > 0) return;
                        this->currMyMoney = *amf;
                       // log("tien slot %d",this->currMyMoney);
                        string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
                        if (!this->Animating && !this->CurrentGameState)
                            this->txtMyMoney->setString(_money);
                        ParticleSystemQuad* ps = (ParticleSystemQuad*)this->pnlBg->getChildByTag(278123);
                        if (ps){
                            ps->stop();
                        }
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
                        this->lastMoney = *amf;
                        this->winvalue2 = this->lastMoney-this->currMyMoney;
                       // log("money slot 3 : %d",this->lastMoney);
                        
                        if (this->winvalue2 > 0 && this->winvalue > 0) return;
                        this->currMyMoney = *amf;
                        string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
                        if (!this->Animating && !this->CurrentGameState)
                            this->txtMyMoney->setString(_money);
                        ParticleSystemQuad* ps = (ParticleSystemQuad*)this->pnlBg->getChildByTag(278123);
                        if (ps){
                            ps->stop();
                        }
                    }
                }
                
            }
        }
    }
}
void LayerPlayGameSlotMachine::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
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
void LayerPlayGameSlotMachine::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPlayGameSlotMachine::onTouchBegan, this);
    //    listener->onTouchMoved = CC_CALLBACK_2(LayerPlayGameSlotMachine::onTouchMoved, this);
    //
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton()->addListeners(this);
}
void LayerPlayGameSlotMachine::onExit()
{
    Layer::onExit();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(effect);
    
    // this->anim->release();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(listener);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameSlotMachine::updateEvent),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameSlotMachine::updateMoneyWin),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameSlotMachine::updateTimerStart),this);

}
