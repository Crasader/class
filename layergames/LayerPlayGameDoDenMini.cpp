//
//  LayerPlayGameDoDenMini.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 6/22/16.
//
//

#include "LayerPlayGameDoDenMini.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"
#include "_Chat_.h"
#include "TaiXiuMessageDefs.h"
#include "LayerLichSuDoDenMini.h"
#include "LayerLichSuPhienDoDenMini.h"
#include "LayerXepHangDoDenMini.h"
#include "../layers/LayerPopupThongBao.h"
#include "../layers/LayerGuideInGame.h"
#include "LayerThongKeDoDenMini.h"
#include "../scenes/ScenePickRoom.h"
#include "../cotuong/gameUtils.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerPlayGameDoDenMini::LayerPlayGameDoDenMini()
{
    
}


LayerPlayGameDoDenMini::~LayerPlayGameDoDenMini()
{
    
}

bool LayerPlayGameDoDenMini::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerDoDenMini.csb");
    
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
    //rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    //auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    //pnlB->setSwallowTouches(false);
    //pnlB->setScale(visibleSize.width / pnlB->getContentSize().width, visibleSize.height / pnlB->getContentSize().height);
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
 //   if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }
//    else{
//        sizeAdd = Size(0, 0);
//    }
    Size sizeAdd2;
   if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
        sizeAdd2 = ChanUtils::getSizePos();
        
    }
    else{
        sizeAdd2 = Size(0, 0);
    }
    //rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    
    
    this->pnlBg1 = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    this->pnlBg1->setSwallowTouches(false);
    auto Image_2 = static_cast<ImageView*>(this->pnlBg1->getChildByName("Image_2"));
    Image_2->loadTexture(StringUtils::format("%s%sbg.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));

    Button* btnClose = static_cast<Button*>(this->pnlBg1->getChildByName("btnClose"));
    if (btnClose){
        btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameDoDenMini::onButtonClose, this));
        btnClose->setPressedActionEnabled(true);
    }
    
    Button* btnLichSu = static_cast<Button*>(this->pnlBg1->getChildByName("btnLichSu"));
    if (btnLichSu){
        btnLichSu->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameDoDenMini::onButtonLichSu, this));
        btnLichSu->setPressedActionEnabled(true);
    }
    Button* btnXepHang = static_cast<Button*>(this->pnlBg1->getChildByName("btnXepHang"));
    if (btnXepHang){
        btnXepHang->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameDoDenMini::onButtonXepHang, this));
        btnXepHang->setPressedActionEnabled(true);
    }
    
    Button* btnSuKien = static_cast<Button*>(this->pnlBg1->getChildByName("btnSuKien"));
    if (btnSuKien){
        btnSuKien->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameDoDenMini::onButtonSukien, this));
        btnSuKien->setPressedActionEnabled(true);
    }
    
    Button* btnHuongDan = static_cast<Button*>(this->pnlBg1->getChildByName("btnHuongDan"));
    if (btnHuongDan){
        btnHuongDan->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameDoDenMini::onButtonHuongDan, this));
        btnHuongDan->setPressedActionEnabled(true);
    }
    Button* btnThongKe = static_cast<Button*>(this->pnlBg1->getChildByName("btnThongKe"));
    if (btnThongKe){
        btnThongKe->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onButtonThongKe, this));
        btnThongKe->setPressedActionEnabled(true);
    }
    
    
    this->imgTai = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgTai"));
    this->imgTai->loadTexture(StringUtils::format("%s%stxt-tai.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));

    this->imgXiu = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgXiu"));
    this->imgXiu->loadTexture(StringUtils::format("%s%sxiu-txt.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));

    this->txtTongBacTai = static_cast<Text*>(this->pnlBg1->getChildByName("txtTongBacTai"));
    this->txtTongBacXiu = static_cast<Text*>(this->pnlBg1->getChildByName("txtTongBacXiu"));
    
    this->txtSoNguoiTai = static_cast<Text*>(this->pnlBg1->getChildByName("txtSoNguoiTai"));
    this->txtSoNguoiXiu = static_cast<Text*>(this->pnlBg1->getChildByName("txtSoNguoiXiu"));
    
    this->btnDatTai = static_cast<Button*>(this->pnlBg1->getChildByName("btnDatTai"));
    if (this->btnDatTai){
        this->btnDatTai->setPressedActionEnabled(true);
        this->btnDatTai->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameDoDenMini::onBtnBetTai, this));
        this->btnDatTai->setTitleText(dataManager.GetSysString(587));

    }
    
    
    arrayValueBtnCuoc.push_back(1000);
    arrayValueBtnCuoc.push_back(10000);
    arrayValueBtnCuoc.push_back(100000);
    arrayValueBtnCuoc.push_back(500000);
    arrayValueBtnCuoc.push_back(1000000);
    
    
    this->firstTimeOpen = true;
    this->btnDatXiu = static_cast<Button*>(this->pnlBg1->getChildByName("btnDatXiu"));
    if (this->btnDatXiu){
        this->btnDatXiu->setPressedActionEnabled(true);
        this->btnDatXiu->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameDoDenMini::onBtnBetXiu, this));
        this->btnDatXiu->setTitleText(dataManager.GetSysString(587));
    }
    
    this->txtDaDatTai = static_cast<Text*>(this->pnlBg1->getChildByName("txtDaDatTai"));
    this->txtDaDatXiu = static_cast<Text*>(this->pnlBg1->getChildByName("txtDaDatXiu"));
    
    this->mQueueMsg = new ThreadQueueMsg();
    
    for (int i = 1; i <= 16; i++){
        auto imgHis = static_cast<ImageView*>(this->pnlBg1->getChildByName(StringUtils::format("imghis%d", i)));
        if (imgHis)
        {
            imgHis->setTag(i-1);
            imgHis->setVisible(false);
            imgHis->setTouchEnabled(true);
            imgHis->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameDoDenMini::onLichSuPhien, this));
            this->lstimgHis.push_back(imgHis);
        }
        
    }
    
    for (int i = 1; i <= 16; i++){
        auto detailHis = static_cast<ImageView*>(this->pnlBg1->getChildByName(StringUtils::format("imgStatusHis%d", i)));
        if (detailHis)
        {
            detailHis->setVisible(false);
            detailHis->setTag(i);
            this->lstHisDetails.push_back(detailHis);
        }
        
    }
    
    for (int i = 0; i < this->lstHisDetails.size(); i++){
        auto detailHis = this->lstHisDetails[i];
        if (detailHis)
        {
            auto txtDetail = static_cast<Text*>(detailHis->getChildByName("txtDetailHis"));
            if (txtDetail)
            {
                // txtDetail->setString(i);
                this->lstTxtDetails.push_back(txtDetail);
            }
        }
        
    }
    
    this->getMinMoney();
    
    
    this->imgHisNow = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgNow"));
    this->imgHisNow->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 0.8), ScaleTo::create(0.5, 0.7), ScaleTo::create(0.5, 0.8), NULL)));
    //moc3
    
    //	this->txtMoneyBetPlayer = static_cast<TextField*>(this->pnlBg1->getChildByName("txtMoneyBetPlayer"));
    //#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //	this->txtMoneyBetPlayer->setInputMode(ui::EditBox::InputMode::NUMERIC);
    //	this->txtMoneyBetPlayer->setMaxLength(9);
    //#endif
    //moc3-end
    this->resultDice1 = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgdice1"));
    this->resultDice2 = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgdice2"));
    this->resultDice3 = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgdice3"));
    this->resultDice1->setVisible(false);
    this->resultDice2->setVisible(false);
    this->resultDice3->setVisible(false);
    
    this->imgBgDo = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgBgDo"));
    this->imgBgDen = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgBgDen"));

    
    auto btnXoaCuoc = static_cast<Button*>(this->pnlBg1->getChildByName("btnXoaCuoc"));
    if (btnXoaCuoc){
        btnXoaCuoc->setPressedActionEnabled(true);
        btnXoaCuoc->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameDoDenMini::onBtnResetTienCuoc, this));
    }
    
    for (int i = 1; i <= 6; i++){
        auto chonTienCuoc = static_cast<Button*>(this->pnlBg1->getChildByName(StringUtils::format("btnMucCuoc%d", i)));
        if (chonTienCuoc){
            chonTienCuoc->setPressedActionEnabled(true);
            chonTienCuoc->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameDoDenMini::onBtnChonTienCuoc, this));
            chonTienCuoc->setTag(i);
        }
    }
    this->imgBat = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgDia"));
    
    this->txtTimer = static_cast<Text*>(this->pnlBg1->getChildByName("txtTimer"));
    this->txtTimer->setVisible(false);
    this->txtTimer->setString("");
    this->txtPhienChoi = static_cast<Text*>(this->pnlBg1->getChildByName("txtPhienChoi"));
    this->txtPhienChoi->setString("");
    
    this->txtTongHu = static_cast<Text*>(this->pnlBg1->getChildByName("txtTongHu"));
    if (this->txtTongHu)
        this->txtTongHu->setString("0");
    
    this->imgTongDiem = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgTongDiem"));
    this->imgTongDiem->setVisible(false);
    this->txtTongDiem = static_cast<Text*>(this->imgTongDiem->getChildByName("txtTongDiem"));
    this->imgNextPlay = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgNextPlay"));
    this->imgNextPlay->setVisible(false);
    auto txtTimerEnd = static_cast<Text*>(this->imgNextPlay->getChildByName("Text_4"));
    txtTimerEnd->setVisible(false);
    this->lblEndTimer = Label::createWithSystemFont("", "", 100);
    lblEndTimer->setColor(Color3B::WHITE);
    this->lblEndTimer->setAnchorPoint(Vec2(0.5, 0.5));
    this->lblEndTimer->setScale(0.8);
    this->lblEndTimer->setPosition(txtTimerEnd->getPosition().x, txtTimerEnd->getPosition().y);
    this->imgNextPlay->addChild(this->lblEndTimer);
    
    
    
    //	this->txtMyMoney = static_cast<Text*>(this->pnlBg1->getChildByName("txtMyMoney"));
    //	this->txtMyMoney->setString("");
    //	this->imgAvatar = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgAvatar"));
    //	this->createAvatar();
    
    
    
    
    this->imgCanCua = static_cast<ImageView*>(this->pnlBg1->getChildByName("imgCanCua"));
    this->imgCanCua->setVisible(false);
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        auto txtMoney = static_cast<Text*>(this->imgCanCua->getChildByName("Text_1"));
        txtMoney->setVisible(false);
        this->lblMoneyTraThuong = Label::createWithBMFont("fontDoDenMini.fnt", "20", TextHAlignment::CENTER);
        this->lblMoneyTraThuong->setAnchorPoint(Vec2(0.5, 0.5));
        this->lblMoneyTraThuong->setScale(1);
        this->lblMoneyTraThuong->setPosition(txtMoney->getPosition().x, txtMoney->getPosition().y + 10);
        this->imgCanCua->addChild(this->lblMoneyTraThuong);
        
    }
    
    //    // Smartfox event init
    //    var self = this;
    //    this->_pUpdateEvt = setInterval(function () {
    //        self.updateEvent();
    //    }, (1000/ 60));
    
    this->sendRequestInfoGame();
    
    this->mQueueMsg->setFunCallBack(this, callfunc_selector(LayerPlayGameDoDenMini::OnExtensionResponseProcess));
    GameServer::getSingleton().addListeners(this);

    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameDoDenMini::updateEvent),this,1/60, false);
//   // if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
//        this->GetQuy(0);
//        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameDoDenMini::GetQuy), 5);
//   // }
    
    
//    //Cheat
//        this->txtFieldCheat = static_cast<TextField*>(this->pnlBg1->getChildByName("txtCheat"));
//        Button* btnCheat = static_cast<Button*>(this->pnlBg1->getChildByName("btnCheat"));
//        if (btnCheat) {
//            btnCheat->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onBtnSet, this));
//            btnCheat->setPressedActionEnabled(true);
//        }
    //
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt("tm", 0);
    boost::shared_ptr<IRequest> req(new ExtensionRequest("xdj", params));
    GameServer::getSingleton().Send(req);
    
    //New GB 2.0
    this->btnGold = static_cast<Button*>(this->pnlBg1->getChildByName("btnGold"));
    if (this->btnGold){
        this->btnGold->setPressedActionEnabled(true);
        this->btnGold->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onBtnGold, this));
    }
    this->btnSilver = static_cast<Button*>(this->pnlBg1->getChildByName("btnSilver"));
    if (this->btnSilver){
        this->btnSilver->setPressedActionEnabled(true);
        this->btnSilver->setVisible(false);
        this->btnSilver->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onBtnSilver, this));
    }
    auto pnlChat = static_cast<Layout*>(this->pnlBg1->getChildByName("pnlChat"));
    
    
    this->layerChat = LayerChatMiniGame::create();
    //this->layerChat->setPosition(Vec2(-320,350));
    this->layerChat->loadResByID(0);
    pnlChat->addChild(this->layerChat);
    this->layerChat->setPosition(Vec2(40,230));
    this->layerChat->setVisible(false);
    
    this->btnChatOpen = (Button*)this->pnlBg1->getChildByName("btnChatOpen");
    this->btnChatOpen->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onButtonChat, this));
    
    this->btnChatClose = (Button*)this->pnlBg1->getChildByName("btnChatClose");
    this->btnChatClose->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onButtonChat, this));
    this->onButtonChat(NULL);
    this->onButtonChat(NULL);
    if (dataManager.getAppConfig().is24h)
    {
        this->btnChatOpen->setVisible(false);
        this->btnChatClose->setVisible(false);
    }
    this->currTypeMoney = 0;
    SceneManager::getSingleton().currRoomTypeDD = 0;

    //this->setScale(0.8);
    
    //moc3
    this->btnNan = static_cast<Button*>(this->pnlBg1->getChildByName("btnNan"));
    if (this->btnNan){
        this->btnNan->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onBtnNan, this));
        this->btnNan->loadTextures(StringUtils::format("%s%sbtn-nan-disable.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("%s%sbtn-nan-enable.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("%s%sbtn-nan-enalbe.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));

    }
    
    this->pnlDatCuoc = (Layout*)this->pnlBg1->getChildByName("pnlDatCuoc");
    if (this->pnlDatCuoc){
        this->pnlDatCuoc->setVisible(false);
        
        for (int i = 1; i<=6;i++ ){
            Button* btnCuoc = static_cast<Button*>(this->pnlDatCuoc->getChildByName(StringUtils::format("btnMucCuoc%d",i)));
            if (btnCuoc){
                btnCuoc->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onBtnChonTien, this));
                btnCuoc->setPressedActionEnabled(true);
            }
        }
        Button* btnDongY = static_cast<Button*>(this->pnlDatCuoc->getChildByName("btnDongY"));
        if (btnDongY){
            btnDongY->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onBtnDatCuoc, this));
            btnDongY->setPressedActionEnabled(true);
            btnDongY->setTitleText(dataManager.GetSysString(889));

        }
        Button* btnTatCa = static_cast<Button*>(this->pnlDatCuoc->getChildByName("btnTatCa"));
        if (btnTatCa){
            btnTatCa->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onBtnCuocTatCa, this));
            btnTatCa->setPressedActionEnabled(true);
            btnTatCa->setTitleText(dataManager.GetSysString(888));

        }
        Button* btnHuy = static_cast<Button*>(this->pnlDatCuoc->getChildByName("btnHuy"));
        if (btnHuy){
            btnHuy->addClickEventListener(CC_CALLBACK_1(LayerPlayGameDoDenMini::onBtnHuyCuoc, this));
            btnHuy->setPressedActionEnabled(true);
            btnHuy->setTitleText(dataManager.GetSysString(890));

        }
        
    }
    
    this->imgTimeNanBat  = static_cast<ImageView*>(rootNode->getChildByName("imgTimeNanBat"));
    this->txtTimeNanBat  = static_cast<Text*>(this->imgTimeNanBat->getChildByName("txtTimeNanBat"));
    this->imgBatNan = static_cast<ImageView*>(rootNode->getChildByName("imgBatNan"));
    this->pnlKetQua = static_cast<Layout*>(rootNode->getChildByName("pnlKetQua"));
    this->pnlKetQua->setSwallowTouches(false);
    this->ResetGame(false);
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }
    return true;
}
void LayerPlayGameDoDenMini::timeNanBat(float dt){
    static int i = 0;
    i++;
    if (i == 5){
        //this->imgTongDiem->setVisible(true);
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameDoDenMini::timeNanBat),this);
        this->imgBatNan->runAction(Sequence::create(MoveTo::create(2,Vec2(this->imgBatNan->getPosition().x ,this->imgBatNan->getPosition().y-300)),CallFunc::create(CC_CALLBACK_0(LayerPlayGameDoDenMini::showKetQua,this)),CallFunc::create(CC_CALLBACK_0(LayerPlayGameDoDenMini::showResultAfter, this)),NULL));
        i = 0;
    }
    
}
void LayerPlayGameDoDenMini::showKetQua(){
    vector<string> arrRg = mUtils::splitString(this->rg, '-');
    
    if (arrRg.size() < 3)
        return;
    int total = atoi(arrRg.at(0).c_str()) + atoi(arrRg.at(1).c_str()) + atoi(arrRg.at(2).c_str());
    
    
    
    //this->imgTongDiem->setVisible(true);
    this->txtTongDiem->setString(StringUtils::format("%d", total));
    
    if (total >= 2){
        this->currResult = "DO";
        this->imgHisNow->loadTexture("do-icon.png");
        this->imgBgDo->loadTexture("LayerDoDenMini/do-active.png");

        this->imgTai->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 0.8), ScaleTo::create(0.5, 1), NULL)));

    }
    else{
        this->currResult = "DEN";
        this->imgBgDen->loadTexture("LayerDoDenMini/do-active.png");

        this->imgHisNow->loadTexture("icon-den.png");
     this->imgXiu->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 0.8), ScaleTo::create(0.5, 1), NULL)));
    }
    
}
void LayerPlayGameDoDenMini::onBtnNan(Ref* pSender){
    if (this->IsNanBat == false){
        this->IsNanBat = true;
        this->btnNan->loadTextures(StringUtils::format("%s%sbtn-nan-enable.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("%s%sbtn-nan-disable.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("%s%sbtn-nan-disable.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    }else{
        this->IsNanBat = false;
        this->btnNan->loadTextures(StringUtils::format("%s%sbtn-nan-disable.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("%s%sbtn-nan-enable.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("%s%sbtn-nan-enalbe.png",PATH_DD.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    }
}
void LayerPlayGameDoDenMini::chucnangNan(){
    vector<string> arrRg = mUtils::splitString(this->rg, '-');
    
    if (arrRg.size() < 3)
        return;
    int total = atoi(arrRg.at(0).c_str()) + atoi(arrRg.at(1).c_str()) + atoi(arrRg.at(2).c_str());
    
    
    
    //this->imgTongDiem->setVisible(true);
    this->txtTongDiem->setString(StringUtils::format("%d", total));
    
    if (total >= 2){
        this->currResult = "DO";
        this->imgHisNow->loadTexture("do-icon.png");
        this->imgBgDo->loadTexture("LayerDoDenMini/do-active.png");

        this->imgTai->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 0.8), ScaleTo::create(0.5, 1), NULL)));
    }
    else{
        this->currResult = "DEN";
        this->imgBgDen->loadTexture("LayerDoDenMini/den-active.png");

        this->imgHisNow->loadTexture("icon-den.png");
        this->imgXiu->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 0.8), ScaleTo::create(0.5, 1), NULL)));
    }
    
    this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create(CC_CALLBACK_0(LayerPlayGameDoDenMini::showResultAfter, this)), NULL));
}
void LayerPlayGameDoDenMini::onButtonThongKe(Ref* pSender){
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene)
        return;
    LayerThongKeDoDenMini* popup = LayerThongKeDoDenMini::create();
    if (this->ghs != NULL)
        popup->loadThongKe(*this->ghs);
    scene->addChild(popup,2000);
}
void LayerPlayGameDoDenMini::onBtnHuyCuoc(Ref* pSender){
    this->tongSoTienDinhCuoc= 0;
    this->btnDatTai->setTitleText(mUtils::convertMoneyEx(this->tongSoTienDinhCuoc));
    this->btnDatXiu->setTitleText(mUtils::convertMoneyEx(this->tongSoTienDinhCuoc));
    
}
void LayerPlayGameDoDenMini::onBtnDatCuoc(Ref* pSender){
    if (this->tongSoTienDinhCuoc  == 0){
        auto toast = Chat::create(dataManager.GetSysString(411), -1);
        this->addChild(toast);
        return;
    }
    if (this->currMyMoney>=0) {
        if ((this->tongSoTienDinhCuoc) > this->currMyMoney ){
            if (this->currTypeMoney == 0)
                SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
            else{
                SceneManager::getSingleton().showPopupNapTienGold(dataManager.GetSysString(978));

            }
            return;
        }
        
        if (SceneManager::getSingleton().getCurrRoomType() == this->currTypeMoney){
            if ((this->tongSoTienDinhCuoc ) > (this->currMyMoney - this->MinBetValue)){
                if (this->currTypeMoney == 0)
                    SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
                else{
                    SceneManager::getSingleton().showPopupNapTienGold(dataManager.GetSysString(978));
                    
                }
                return;
            }
        }
        
        
        //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->currMyMoney));
        if (this->isBetTai){
            
            auto betNum = this->tongSoTienDinhCuoc;//atoi(this->txtMoneyBetPlayer->getString().c_str());
            this->currBetTai += betNum;
            this->txtDaDatTai->setString(mUtils::convertMoneyEx(this->currBetTai));
            
            this->currTongBetTai += betNum;
            this->txtTongBacTai->setString(mUtils::convertMoneyEx(this->currTongBetTai));
            
            if (!this->isAddMeBetTai){
                this->isAddMeBetTai = true;
                this->currSonguoiTai += 1;
                this->txtSoNguoiTai->setString(StringUtils::format("%d", this->currSonguoiTai));
            }
            this->currMyMoney -= betNum;
            SceneManager::getSingleton().setLastGameID(131);

            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutUtfString(EXT_FIELD_GAME_BET_VALUE, StringUtils::format("%d", betNum));
            params->PutUtfString(EXT_FIELD_GAME_BET_TYPE, BET_TYPE_DO);
            
            boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ_MINI_XD, params));
            GameServer::getSingleton().Send(req);
            
            this->tongSoTienDinhCuoc= 0;
            this->btnDatTai->setTitleText(mUtils::convertMoneyEx(this->tongSoTienDinhCuoc));
            
        }
        if (this->isBetXiu){
            
            auto betNum = this->tongSoTienDinhCuoc;//atoi(this->txtMoneyBetPlayer->getString().c_str());
            this->currBetXiu += betNum;
            this->txtDaDatXiu->setString(mUtils::convertMoneyEx(this->currBetXiu));
            
            this->currTongBetXiu += betNum;
            this->txtTongBacXiu->setString(mUtils::convertMoneyEx(this->currTongBetXiu));
            
            if (!this->isAddMeBetXiu){
                this->isAddMeBetXiu = true;
                this->currSonguoiXiu += 1;
                this->txtSoNguoiXiu->setString(StringUtils::format("%d", this->currSonguoiXiu));
            }
            this->currMyMoney -= betNum;
            
            SceneManager::getSingleton().setLastGameID(131);

            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutUtfString(EXT_FIELD_GAME_BET_VALUE, StringUtils::format("%d", betNum));
            params->PutUtfString(EXT_FIELD_GAME_BET_TYPE, BET_TYPE_DEN);
            
            boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ_MINI_XD, params));
            GameServer::getSingleton().Send(req);
            this->tongSoTienDinhCuoc= 0;
            this->btnDatXiu->setTitleText(mUtils::convertMoneyEx(this->tongSoTienDinhCuoc));
            
        }
        this->pnlDatCuoc->setVisible(false);
        
        
    }
}

void LayerPlayGameDoDenMini::onBtnCuocTatCa(Ref* pSender){
    if (this->currMyMoney >= 100000000){
        auto toast = Chat::create(dataManager.GetSysString(412), -1);
        this->addChild(toast);
        return;
    }
    this->tongSoTienDinhCuoc = this->currMyMoney;
    if (this->isBetTai){
        this->btnDatTai->setTitleText(mUtils::convertMoneyEx(this->tongSoTienDinhCuoc));
    }else{
        this->btnDatXiu->setTitleText(mUtils::convertMoneyEx(this->tongSoTienDinhCuoc));
        
    }
    
}

void LayerPlayGameDoDenMini::onBtnChonTien(Ref* pSender){
    auto node = (Node*)pSender;
    int tag = node->getTag();
    
    if ((this->tongSoTienDinhCuoc + tag) >= 100000000){
        auto toast = Chat::create(dataManager.GetSysString(412), -1);
        this->addChild(toast);
        return;
    }
    this->tongSoTienDinhCuoc += tag;
    if (this->isBetTai){
        this->btnDatTai->setTitleText(mUtils::convertMoneyEx(this->tongSoTienDinhCuoc));
    }
    if (this->isBetXiu){
        this->btnDatXiu->setTitleText(mUtils::convertMoneyEx(this->tongSoTienDinhCuoc));
        
    }
}
void LayerPlayGameDoDenMini::onBtnGold(Ref *sender){
    SceneManager::getSingleton().showLoading();
    //	this->btnGold->setEnabled(false);
    //	this->btnGold->setBright(false);
    //
    //	this->btnSilver->setEnabled(true);
    //	this->btnSilver->setBright(true);
    
    this->btnGold->setVisible(false);
    this->btnSilver->setVisible(true);
    
    
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req(new ExtensionRequest("xdl", params));
    GameServer::getSingleton().Send(req);
    this->ResetGame(false);
    
}
void LayerPlayGameDoDenMini::onBtnSilver(Ref *sender){
    SceneManager::getSingleton().showLoading();
    
    //    this->btnGold->setVisible(false);
    //    this->btnSilver->setVisible(true);
    //
    //	this->btnGold->setEnabled(true);
    //	this->btnGold->setBright(true);
    
    this->btnSilver->setVisible(false);
    this->btnGold->setVisible(true);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req(new ExtensionRequest("xdl", params));
    GameServer::getSingleton().Send(req);
    this->ResetGame(false);
    
}
void LayerPlayGameDoDenMini::onBtnSet(Ref *sender){
//    boost::shared_ptr<ISFSObject> params(new SFSObject());
//    params->PutUtfString("rg", this->txtFieldCheat->getString());
//    params->PutUtfString("tk", mUtils::getMD5KeyInApp(*(GameServer::getSingleton().getSmartFox()->SessionToken())+SceneManager::getSingleton().getMyName()+"DTS@123"));
//    boost::shared_ptr<IRequest> req(new ExtensionRequest("xdsetrrq", params));
//    GameServer::getSingleton().Send(req);
    
}
void LayerPlayGameDoDenMini::GetQuy(float dt){
//    boost::shared_ptr<ISFSObject> params(new SFSObject());
//    params->PutInt("gbv", 0);
//    boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_XD_GET_QUY_THUONG_REQ, params));
//    GameServer::getSingleton().Send(req);
}
void LayerPlayGameDoDenMini::updateEvent(float dt){
    if (this->mQueueMsg->isQueueEmpty())
        return;
    
    if (this->Animating)
        return;
    
    this->mQueueMsg->run();
}
void LayerPlayGameDoDenMini::onButtonClose(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
        //		LayerGameDoDenMini* parent = (LayerGameDoDenMini*)this->getParent();
        //		if (parent)
        //			parent->imgBGTimer->setVisible(true);
        this->removeFromParentAndCleanup(true);
    }
}
void LayerPlayGameDoDenMini::onButtonSukien(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
//        LayerPopupWebView* _layer = LayerPopupWebView::create();
//        //url = SKXD_P2;
//        _layer->setWebView(url);
//        Scene *scene = Director::getInstance()->getRunningScene();
//        if (!scene)
//            return;
//        scene->addChild(_layer, 1000);
    }
}
void LayerPlayGameDoDenMini::onButtonHuongDan(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        
        LayerGuideInGame* layer = LayerGuideInGame::create();
        string url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+GUIDE_P2_LINK;
        layer->setLink(StringUtils::format("%s%d", url.c_str(), 134));
        scene->addChild(layer, 2000);
    }
}
void LayerPlayGameDoDenMini::onButtonLichSu(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        // this->getParent().imgBGTimer->setVisible(true);
        LayerLichSuDoDenMini* lichsu = LayerLichSuDoDenMini::create();
        scene->addChild(lichsu);
        lichsu->setLocalZOrder(2000);
    }
}
void LayerPlayGameDoDenMini::onButtonXepHang(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        LayerXepHangDoDenMini* xephang = LayerXepHangDoDenMini::create();
        scene->addChild(xephang);
        xephang->setLocalZOrder(2000);
    }
}
void LayerPlayGameDoDenMini::onBtnBetTai(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (!this->CurrentGameState)
        {
            auto toast = Chat::create(dataManager.GetSysString(413), -1);
            this->addChild(toast);
            return;
        }
        if (this->nguoimobat&& !dataManager.getAppConfig().isBetWhenOpenDish){
            auto toast = Chat::create(dataManager.GetSysString(414), -1);
            this->addChild(toast);
            return;
        }
        if (!this->pnlDatCuoc->isVisible()){
            this->pnlDatCuoc->setVisible(true);
        }
        //
        this->isBetTai = true;
        this->isBetXiu = false;
        this->btnDatXiu->setTitleText(dataManager.GetSysString(587));
        this->tongSoTienDinhCuoc= 0;
        this->btnDatXiu->setTitleText(mUtils::convertMoneyEx(this->tongSoTienDinhCuoc));
        
        if (this->pnlBg1->getPositionY() < 540){
            auto posX = this->pnlBg1->getPositionX();
            auto posY = 540;
            this->pnlBg1->runAction(MoveTo::create(0.2,Vec2(posX,posY)));
        }
        
    }
}
void LayerPlayGameDoDenMini::onBtnBetXiu(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (!this->CurrentGameState)
        {
            auto toast = Chat::create(dataManager.GetSysString(413), -1);
            this->addChild(toast);
            return;
        }
        if (this->nguoimobat&& !dataManager.getAppConfig().isBetWhenOpenDish){
            auto toast = Chat::create(dataManager.GetSysString(414), -1);
            this->addChild(toast);
            return;
        }
        if (!this->pnlDatCuoc->isVisible()){
            this->pnlDatCuoc->setVisible(true);
        }
        
        this->isBetTai = false;
        this->isBetXiu = true;
        this->btnDatTai->setTitleText(dataManager.GetSysString(587));
        this->tongSoTienDinhCuoc= 0;
        this->btnDatTai->setTitleText(mUtils::convertMoneyEx(this->tongSoTienDinhCuoc));
        
        if (this->pnlBg1->getPositionY() < 540){
            auto posX = this->pnlBg1->getPositionX();
            auto posY = 540;
            this->pnlBg1->runAction(MoveTo::create(0.2,Vec2(posX,posY)));
        }
    }
}

void LayerPlayGameDoDenMini::onLichSuPhien(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        LayerLichSuPhienDoDenMini* lsphien = LayerLichSuPhienDoDenMini::create();
        lsphien->setLstHistory(this->listHistory);
        lsphien->reloadCoin(this->currTypeMoney);
        Node* img = (Node*)sender;
        lsphien->setCurrentItem(img->getTag());
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(lsphien);
        lsphien->setLocalZOrder(5000);
    }
}
void LayerPlayGameDoDenMini::onBtnResetTienCuoc(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        //this->txtMoneyBetPlayer->setString("");
    }
}
void LayerPlayGameDoDenMini::onBtnChonTienCuoc(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        Button* pSender = (Button*)sender;
        int numMoney = 0;
        //string strMoney = this->txtMoneyBetPlayer->getString();
        
        //		if (strcmp(strMoney.c_str(), "") == 0)
        //			numMoney = 0;
        //		else
        //			numMoney = atoi(strMoney.c_str());
        
        if (numMoney >= 100000000){
            auto toast = Chat::create(dataManager.GetSysString(412), -1);
            this->addChild(toast);
            return;
        }
        
        if (pSender->getTag() == 6){
            numMoney *= 2;
        }
        else{
            float money = this->arrayValueBtnCuoc[pSender->getTag() - 1];
            numMoney += money;
        }
        
        //this->txtMoneyBetPlayer->setString(StringUtils::format("%d", numMoney));
    }
}
void LayerPlayGameDoDenMini::createAvatar() {
    this->circleAvatar = CircleAvatar::create();
    //this->circleAvatar->setScale(0.7);
    this->circleAvatar->setAnchorPoint(this->imgAvatar->getAnchorPoint());
    this->circleAvatar->setPosition(this->imgAvatar->getPosition());
    this->pnlBg1->addChild(this->circleAvatar);
    this->circleAvatar->setVisible(false);
}
void LayerPlayGameDoDenMini::ResetGame(bool state){
    //moc3
    this->imgBgDen->loadTexture("LayerDoDenMini/den-normal.png");
    this->imgBgDo->loadTexture("LayerDoDenMini/do-normal.png");

    this->txtSoNguoiTai->setString("0");
    this->txtSoNguoiXiu->setString("0");
    this->txtTongBacTai->setString("0");
    this->txtTongBacXiu->setString("0");
    this->txtTimer->setVisible(true);
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        this->txtTimerP2->setVisible(true);
        this->txtTimer->setVisible(false);
    }
    this->txtTimer->setString("");
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        this->txtTimerP2->setString("");
        this->txtTimer->setVisible(false);
    }
    this->txtDaDatTai->setString("0");
    this->txtDaDatXiu->setString("0");
    this->currBetTai = 0;
    this->currBetXiu = 0;
    this->currTongBetTai = 0;
    this->currTongBetXiu = 0;
    this->imgTai->stopAllActions();
    this->imgXiu->stopAllActions();
    this->imgTai->setScale(1);
    this->imgXiu->setScale(1);
    this->imgXiu->setVisible(true);
    this->imgTai->setVisible(true);
    this->pnlBg1->removeChildByTag(144);
    this->isAddMeBetXiu = false;
    this->isAddMeBetTai = false;
    this->imgHisNow->loadTexture("now.png");
    this->currSonguoiTai = 0;
    this->currSonguoiXiu = 0;
    this->rg = "";
    this->rt10 = "";
    this->resultDice1->setVisible(false);
    this->resultDice2->setVisible(false);
    this->resultDice3->setVisible(false);
    this->imgTongDiem->setVisible(false);
    this->imgNextPlay->setVisible(false);
    this->imgCanCua->setVisible(false);
    this->money = 0;
    //    //this->removeChildByTag(TAG_RESULT_MONEY);
    //    if (b) {
    //        this->Animating = true;
    //    } else {
    //        this->Animating = false;
    //    }
    this->isShowMoney = false;
    this->nguoimobat = false;
    this->pnlDatCuoc->setVisible(false);
    this->btnDatTai->setTitleText(dataManager.GetSysString(587));
    this->btnDatXiu->setTitleText(dataManager.GetSysString(587));
    this->imgBat->setVisible(false);
    this->isTouchBat = false;
    this->imgTimeNanBat->setVisible(false);
    this->txtTimeNanBat->setVisible(false);
    this->imgBatNan->setVisible(false);
    this->imgBatNan->setPosition(this->pnlBg1->getPosition());
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameDoDenMini::timeNanBat),this);

}
void LayerPlayGameDoDenMini::sendRequestInfoGame(){
    //    boost::shared_ptr<ISFSObject> params(new SFSObject());
    //    boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_GET_GAME_INFO_REQ_XD_MINI, params));
    //    GameServer::getSingleton().Send(req);
    
}
bool LayerPlayGameDoDenMini::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto rect = this->pnlBg1->getBoundingBox();
    auto locationInNode = pTouch->getLocation();
    auto rectbat = this->imgBatNan->getBoundingBox();
    
    if (this->IsNanBat && this->CurrentGameState == false){
        if (rectbat.containsPoint(locationInNode)){
            this->isTouchBat = true;
        }
    }
    if (rect.containsPoint(locationInNode)){
        return true;
    }
    //var s = target.getContentSize();
    return false;
}
//Listen Server Event
void LayerPlayGameDoDenMini::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if (0 != cmd->compare(EXT_EVENT_START_XDMINI)
        && 0 != cmd->compare(EXT_EVENT_END_XDMINI)
        && 0 != cmd->compare(EXT_EVENT_GAME_RESULT_XDMINI)
        && 0 != cmd->compare(EXT_EVENT_GAME_BET_ERROR_NOTIFY_XDMINI)
        && 0 != cmd->compare(EXT_EVENT_GAME_INFO_NTF_XDMINI)
        && 0 != cmd->compare(EXT_EVENT_GAME_HIS_NTF_XDMINI)
        && 0 != cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NTF_XDMINI)
        && 0 != cmd->compare(EXT_EVENT_GAME_STATE_NTF_XDMINI)
        && 0 != cmd->compare(EXT_EVENT_GAME_BET_NTF_XDMINI)
        && 0 != cmd->compare(EXT_EVENT_XD_REFUND_NTF)
        && 0 != cmd->compare(EXT_EVENT_XD_GET_QUY_THUONG_RESP)
        && 0 != cmd->compare("xdlrs")){
        return;
    }
    pObjs->PutUtfString("_cmd_", *cmd);
    this->mQueueMsg->addQueue(pObjs);
}

void LayerPlayGameDoDenMini::OnExtensionResponseProcess(){
    this->param = this->mQueueMsg->frontQueue();
    boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
    
    if (0 == cmd->compare(EXT_EVENT_START_XDMINI)) {
        this->event_EXT_EVENT_START();
    }
    else if (0 == cmd->compare(EXT_EVENT_END_XDMINI)) {
        this->event_EXT_EVENT_END();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT_XDMINI)) {
        this->event_EXT_EVENT_GAME_RESULT();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_BET_ERROR_NOTIFY_XDMINI)) {
        this->event_EXT_EVENT_GAME_BET_ERROR_NOTIFY();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_INFO_NTF_XDMINI)) {
        this->event_EXT_EVENT_GAME_INFO_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_HIS_NTF_XDMINI)) {
        this->event_EXT_EVENT_GAME_HIS_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NTF_XDMINI)) {
        this->event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF_XDMINI)) {
        this->event_EXT_EVENT_GAME_STATE_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_BET_NTF_XDMINI)) {
        this->event_EXT_EVENT_GAME_BET_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_XD_REFUND_NTF)) {
        this->event_EXT_EVENT_SHOW_CANCUA();
    }
    else if (0 == cmd->compare(EXT_EVENT_XD_GET_QUY_THUONG_RESP)) {
        boost::shared_ptr<long> gbv = this->param->GetInt("gbv");
        
        if (gbv){
            if (this->txtTongHu) {
                this->txtTongHu->setString(StringUtils::format("%s", mUtils::convertMoneyEx(*gbv).c_str()));
            }
        }
        
    }
    else if (0 == cmd->compare("xdlrs")){
        if (this->currTypeMoney == 0){
            SceneManager::getSingleton().hideLoading();
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutInt("tm", 1);
            boost::shared_ptr<IRequest> req(new ExtensionRequest("xdj", params));
            GameServer::getSingleton().Send(req);
            this->currTypeMoney = 1;
                SceneManager::getSingleton().currRoomTypeDD = 1;
        }
        else{
            SceneManager::getSingleton().hideLoading();
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutInt("tm", 0);
            boost::shared_ptr<IRequest> req(new ExtensionRequest("xdj", params));
            GameServer::getSingleton().Send(req);
            
            this->currTypeMoney = 0;
                SceneManager::getSingleton().currRoomTypeDD = 0;
            
        }
    }
    this->mQueueMsg->deQueue();
}

void LayerPlayGameDoDenMini::event_EXT_EVENT_SHOW_CANCUA(){
    boost::shared_ptr<double> moneyrt = this->param->GetDouble(EXT_FIELD_GAME_RETURN_VALUE);
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
        if (moneyrt != NULL){
            if (*moneyrt > 0){
                auto money = "+" + mUtils::convertMoneyEx(*moneyrt);
                string font_name = "font_win-font.fnt";
                if (money.find('-') != std::string::npos) //contain string '-'
                    font_name = "font_lose-font.fnt";
                Label* labelResult = Label::createWithSystemFont(money, "", 50);
                labelResult->setColor(Color3B::YELLOW);
                labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                if (this->isCanCua == 0){
                    labelResult->setPositionX(this->txtDaDatTai->getPositionX() + sizeAdd.width);
                    labelResult->setPositionY(this->txtDaDatTai->getPositionY() + sizeAdd.height);
                    this->currBetTai -= *moneyrt;
                    if (this->currBetTai < 0)
                        this->currBetTai = 0;
                    
                    this->txtDaDatTai->setString(mUtils::convertMoneyEx(this->currBetTai));
                }
                else{
                    labelResult->setPositionX(this->txtDaDatXiu->getPositionX() + sizeAdd.width);
                    labelResult->setPositionY(this->txtDaDatXiu->getPositionY() + sizeAdd.height);
                    this->currBetXiu -= *moneyrt;
                    if (this->currBetXiu < 0)
                        this->currBetXiu = 0;
                    this->txtDaDatXiu->setString(mUtils::convertMoneyEx(this->currBetXiu));
                    
                }
                
                this->pnlBg1->addChild(labelResult);
                labelResult->setTag(TAG_RESULT_MONEY);
                labelResult->runAction(Sequence::create(MoveBy::create(1, Vec2(0, labelResult->getContentSize().height)),
                                                        DelayTime::create(1),
                                                        FadeOut::create(0.2), RemoveSelf::create(true), NULL));
                
                
            }
        }
    }
}
void LayerPlayGameDoDenMini::event_EXT_EVENT_START(){
    int phien = *this->param->GetInt("mid");
    long endTime = *this->param->GetLong("et");
    long playerTime = *this->param->GetInt("pt");
    this->PlayTurnTime = playerTime / 1000;
    this->BetweentwoPlayTime = endTime / 1000;
    // if(phien!=NULL){
    this->txtPhienChoi->setString(StringUtils::format("#%d", phien));
    // }
    this->showHistory();
    this->ResetGame(false);
    this->UpdateTimeStartGame();
    this->CurrentGameState = true;
}
void LayerPlayGameDoDenMini::event_EXT_EVENT_END(){
    this->StopMyTimer();
    // Đặt lại trạng thái game.
    this->CurrentGameState = false;
    
    this->UpdateTimeEndGame();
    
    this->rg = *this->param->GetUtfString(EXT_FIELD_RESULT_GAME);
    this->rt10 = *this->param->GetUtfString(EXT_FIELD_RESULT_TOP_10);
    
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
        int currTienTraLai = 0;
        int currCuaTraLai = 0; //0 la tai, 1 la xiu
        if (this->currTongBetTai > this->currTongBetXiu){
            currCuaTraLai = 0;
            currTienTraLai = this->currTongBetTai - this->currTongBetXiu;
        }
        else if (this->currTongBetXiu > this->currTongBetTai){
            currCuaTraLai = 1;
            currTienTraLai = this->currTongBetXiu - this->currTongBetTai;
        }
        if (currTienTraLai > 0){
            string font_name = "font_lose-font.fnt";
            string strMoneyCanCua = "-" + mUtils::convertMoneyEx(currTienTraLai);
            Label* labelResult = Label::createWithSystemFont(strMoneyCanCua, "", 50);
            labelResult->setColor(Color3B::YELLOW);
            labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            if (currCuaTraLai == 0){
                labelResult->setPositionX(this->txtTongBacTai->getPositionX() + sizeAdd.width);
                labelResult->setPositionY(this->imgTai->getPositionY() + sizeAdd.height);
            }
            else{
                labelResult->setPositionX(this->txtTongBacXiu->getPositionX() + sizeAdd.width);
                labelResult->setPositionY(this->imgXiu->getPositionY() + sizeAdd.height);
            }
            
            this->pnlBg1->addChild(labelResult);
            labelResult->setTag(TAG_RESULT_MONEY);
            labelResult->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, labelResult->getContentSize().height)),
                                                    DelayTime::create(1),
                                                    FadeOut::create(0.2), RemoveSelf::create(true), CallFunc::create(CC_CALLBACK_0(LayerPlayGameDoDenMini::showAnimationAfterCanCua, this)), NULL));
            if (currCuaTraLai == 0){
                this->isCanCua = 0;
                this->currTongBetTai -= currTienTraLai;
                if (this->currTongBetTai < 0)
                    this->currTongBetTai = 0;
                this->txtTongBacTai->setString(mUtils::convertMoneyEx(this->currTongBetTai));
            }
            else{
                this->isCanCua = 1;
                this->currTongBetXiu -= currTienTraLai;
                if (this->currTongBetXiu < 0)
                    this->currTongBetXiu = 0;
                
                this->txtTongBacXiu->setString(mUtils::convertMoneyEx(this->currTongBetXiu));
                
            }
        }
        else{
            this->showAnimationAfterCanCua();
        }
    }
}
void LayerPlayGameDoDenMini::showAnimationAfterCanCua(){
    this->imgBat->setVisible(false);
    this->imgCanCua->setVisible(false);
    this->Animating = true;
    
    Vec2 pos = Vec2(Vec2(this->imgBat->getPosition().x, this->imgBat->getPosition().y));
    auto animatea = LayerAnimationDoDenMini::createInstance(pos);
    animatea->SetCallBackResult(this, callfuncO_selector(LayerPlayGameDoDenMini::CallBackResult));
    this->pnlBg1->addChild(animatea, 999, TAG_ANIM_END_GAME);
}
void LayerPlayGameDoDenMini::event_EXT_EVENT_GAME_RESULT(){
    this->imgCanCua->setVisible(false);
    boost::shared_ptr<long> gbv = this->param->GetInt(EXT_FIELD_GAME_BET_VALUE);
    boost::shared_ptr<long> gwv = this->param->GetInt(EXT_FIELD_GAME_WIN_VALUE);
    boost::shared_ptr<long> grv = this->param->GetInt(EXT_FIELD_GAME_RETURN_VALUE);
    this->wincell = *this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
    //Số tiền đã đặt cược và số tiền thu về
    if (grv != NULL && gwv != NULL && strcmp(this->wincell.c_str(), "") != 0) {
        //Hiển thị animation
        
        this->money = 0;
        if (*gwv > 0){
            this->money += *gwv;
            if (this->currTypeMoney == 0){
                if (this->money > dataManager.getAppConfig().notifyZen){
                    string winCell = "";
                    
                    this->msg = StringUtils::format("%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s%s|%s|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(1234).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(983).c_str(),mUtils::convertMoneyEx(this->money).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(12341).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9831).c_str(),mUtils::convertMoneyEx(this->money).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(12342).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9832).c_str(),mUtils::convertMoneyEx(this->money).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(12343).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9833).c_str(),mUtils::convertMoneyEx(this->money).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str());
                    
                    
                }
            }else{
                if (this->money > dataManager.getAppConfig().notifyGold){
                    string winCell = "";
                    
                           this->msg = StringUtils::format("%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(1234).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(983).c_str(),mUtils::convertMoneyEx(this->money).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(12341).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9831).c_str(),mUtils::convertMoneyEx(this->money).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(12342).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9832).c_str(),mUtils::convertMoneyEx(this->money).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(12343).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9833).c_str(),mUtils::convertMoneyEx(this->money).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str());
                }
            }
            if (!this->Animating){
                if (!this->IsNanBat){
                this->isShowMoney = true;
                this->showResultGame("+" + mUtils::convertMoneyEx(this->money), this->wincell);
                //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->amfUser));
                }
            }
  
        }
        else{
            this->money = 0;
            if (*grv > 0){
                this->money += *grv;
                if (!this->Animating){
                     if (!this->IsNanBat){
                         this->isShowMoney = true;
                         this->showResultGame("+" + mUtils::convertMoneyEx(this->money), this->wincell);
                     }
                    //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->amfUser));
                }
            }
        }
    }
}
void LayerPlayGameDoDenMini::event_EXT_EVENT_GAME_BET_ERROR_NOTIFY(){
    boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);
    boost::shared_ptr<string> bettype = this->param->GetUtfString("gbt");
    boost::shared_ptr<string> betvalue = this->param->GetUtfString("gbv");
    if (errc != NULL) {
        if (*errc == RESCODE_NOT_ENOUGH_MONEY) {
            if (this->currTypeMoney == 0)
                SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
            else{
                SceneManager::getSingleton().showPopupNapTienGold(dataManager.GetSysString(978));

            }
            this->RollBackBetValue(*bettype, *betvalue);
        }
        
        if (*errc == RESCODE_GAME_IS_ENDED) {
            // this->Toast.ShowToast("Đã hết thời gian đặt cược");
            auto toast = Chat::create(dataManager.GetSysString(410), -1);
            this->addChild(toast);
            this->RollBackBetValue(*bettype, *betvalue);
        }
        if (*errc == RESCODE_NOT_BET_MORE_TYPE) {
            // this->Toast.ShowToast("Đã hết thời gian đặt cược");
            auto toast = Chat::create(dataManager.GetSysString(419), -1);
            this->addChild(toast);
            this->RollBackBetValue(*bettype, *betvalue);
        }
    }
    
}
void LayerPlayGameDoDenMini::RollBackBetValue(string betType, string betValue){
    if (strcmp(betType.c_str(), BET_TYPE_DO) == 0){
        this->currMyMoney += atoi(betValue.c_str());
        this->currBetTai -= atoi(betValue.c_str());
        this->currTongBetTai -= atoi(betValue.c_str());
        if (this->currBetTai == 0 && this->currSonguoiTai > 0){
            this->isAddMeBetTai = false;
            this->currSonguoiTai -= 1;
            this->txtSoNguoiTai->setString(StringUtils::format("%d", this->currSonguoiTai));
            
        }
        this->txtDaDatTai->setString(mUtils::convertMoneyEx(this->currBetTai));
        this->txtTongBacTai->setString(mUtils::convertMoneyEx(this->currTongBetTai));
        //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->currMyMoney));
    }
    else{
        this->currMyMoney += atoi(betValue.c_str());
        this->currBetXiu -= atoi(betValue.c_str());
        this->currTongBetXiu -= atoi(betValue.c_str());
        if (this->currBetXiu == 0 && this->currSonguoiXiu > 0){
            this->isAddMeBetXiu = false;
            this->currSonguoiXiu -= 1;
            this->txtSoNguoiXiu->setString(StringUtils::format("%d", this->currSonguoiXiu));
            
        }
        this->txtDaDatXiu->setString(mUtils::convertMoneyEx(this->currBetXiu));
        this->txtTongBacXiu->setString(mUtils::convertMoneyEx(this->currTongBetXiu));
        //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->currMyMoney));
    }
}
void LayerPlayGameDoDenMini::event_EXT_EVENT_GAME_INFO_NTF(){
    
    boost::shared_ptr<string> ghtp = this->param->GetUtfString(EXT_FIELD_GAME_HISTORY_TOP_PLAYER);
    this->ghs = this->param->GetUtfString(EXT_FIELD_GAME_HISTORY_RESULT);
    
    if (ghtp != NULL) {
        this->listTopWinner.clear();
        auto arr = mUtils::splitString(*ghtp, '|');
        for (int i = arr.size() - 1; i >= 0; --i) {
            if (arr.at(i) != "") {
                this->listTopWinner.push_back(arr.at(i));
            }
        }
        // this->GetInfoTopWinner();
    }
    if (this->firstTimeOpen)
        this->showHistory();
    
}
void LayerPlayGameDoDenMini::event_EXT_EVENT_GAME_HIS_NTF(){
    
}
void LayerPlayGameDoDenMini::event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF(){
    
}
void LayerPlayGameDoDenMini::event_EXT_EVENT_GAME_STATE_NTF(){
    
    boost::shared_ptr<long> gtime = this->param->GetInt(EXT_FIELD_GAME_TIME);
    boost::shared_ptr<long> gstate = this->param->GetInt(EXT_FIELD_GAME_PLAY_STATE);
    boost::shared_ptr<string> gbt = this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
    boost::shared_ptr<string> gbex = this->param->GetUtfString(EXT_FIELD_GAME_BET_EXCHANGE);
    boost::shared_ptr<long> phien = this->param->GetInt("mid");
    boost::shared_ptr<long> endTime = this->param->GetInt("et");
    boost::shared_ptr<long> playerTime = this->param->GetInt("pt");
    boost::shared_ptr<string> totalPlayer = this->param->GetUtfString("gtbuc");
    boost::shared_ptr<string> ghs = this->param->GetUtfString("ghs");
    boost::shared_ptr<string> od = this->param->GetUtfString("od");
    
    this->layerChat->loadResByID(0);
    
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    
    if (mySelf != NULL){
        if (this->currTypeMoney == 0){
            if (mySelf->GetVariable("amf") != NULL) {
                auto myMoney = *mySelf->GetVariable("amf")->GetDoubleValue();
                //this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
                this->currMyMoney = myMoney;
            }
        }
        else{
            if (mySelf->GetVariable("amfs") != NULL) {
                auto myMoney = *(mySelf->GetVariable("amfs")->GetDoubleValue());
                //this->txtMyMoney->setString(mUtils::convertMoneyEx(myMoney));
                this->currMyMoney = myMoney;
            }
        }
        
        if (mySelf->GetVariable("aal") != NULL) {
            this->urlAvatar = *mySelf->GetVariable("aal")->GetStringValue();
            //this->circleAvatar->setAvatarImage(this->urlAvatar);
        }
    }
    
    if (totalPlayer != NULL)
        this->showTotalPlayer(*totalPlayer);
    this->PlayTurnTime = *playerTime / 1000;
    this->BetweentwoPlayTime = *endTime / 1000;
    if (phien != NULL){
        // string phienstr = *phien;
        this->txtPhienChoi->setString(StringUtils::format("#%ld", *phien));
    }
    this->firstTimeOpen = true;
    // Thoi gian con lai cua van choi
    if (gtime != NULL) {
        this->TimerCountDown = *gtime;
        // cc.log("gtime = " + this->TimerCountDown);
        //        if (this->txtTimer != null) {
        //            //  this->txtTimer.setString(this->TimerCountDown.toString());
        //            // this->txtTimer.setVisible(true);
        //        }
    }
    
    if (gbt != NULL) {
        // cc.log("gbt =  " + gbt);
        this->ReloadGameWhenReturn(*gbt);
    }
    
    if (gbex != NULL) {
        //this->LoadGameExchange(*gbex);
    }
    if (od != NULL){
        if (od->compare(SceneManager::getSingleton().getMyName()) == 0){
            this->nguoimobat = true;
            // this->Toast.ShowToast("Bạn không đủ tiền để chơi");
            auto currscene = Director::getInstance()->getRunningScene();
            if (currscene == NULL)
                return;
            auto toast = Chat::create(dataManager.GetSysString(417), -1);
            currscene->addChild(toast,2000);
            
        }else{
            auto currscene = Director::getInstance()->getRunningScene();
            if (currscene == NULL)
                return;
            auto toast = Chat::create(StringUtils::format("%s %s",dataManager.GetSysString(418).c_str(),ChanUtils::GetNamePlayer(*od).c_str()), -1);
            currscene->addChild(toast,2000);
        }
    }
    if (gstate != NULL) {
        if (0 == *gstate) {
            
            if (ghs != NULL){
                if (0 != strcmp(ghs->c_str(), "")){
                    
                    auto arr = mUtils::splitString(*ghs, '|');
                    
                    if (arr.size() == 14){
                        this->showPrevMatchHis(arr[3]);
                        this->txtTongBacTai->setString(mUtils::convertMoneyEx(atof(arr[7].c_str())));
                        this->txtTongBacXiu->setString(mUtils::convertMoneyEx(atof(arr[8].c_str())));
                        this->txtDaDatTai->setString(mUtils::convertMoneyEx(atof(arr[10].c_str())));
                        this->txtDaDatXiu->setString(mUtils::convertMoneyEx(atof(arr[11].c_str())));
                        this->txtSoNguoiTai->setString(mUtils::convertMoneyEx(atof(arr[12].c_str())));
                        this->txtSoNguoiXiu->setString(mUtils::convertMoneyEx(atof(arr[13].c_str())));
                    }
                    
                    this->CurrentGameState = false;
                    //this->txtTimer.setVisible(false);
                    this->BetweentwoPlayTime = *gtime;
                    this->txtTimeNanBat->setString(StringUtils::format("%d", this->BetweentwoPlayTime));
                    
                    
                    this->lblEndTimer->setString(StringUtils::format("%d", this->BetweentwoPlayTime));
                    this->UpdateTimeEndGame();
                    this->imgNextPlay->setVisible(true);
                    
                }
                else{
                    this->CurrentGameState = false;
                    //this->txtTimer.setVisible(false);
                    this->BetweentwoPlayTime = *gtime;
                    this->txtTimeNanBat->setString(StringUtils::format("%d", this->BetweentwoPlayTime));
                    
                    this->lblEndTimer->setString(StringUtils::format("%d", this->BetweentwoPlayTime));
                    this->UpdateTimeEndGame();
                    this->imgNextPlay->setVisible(true);
                    
                }
            }
            else{
                this->CurrentGameState = false;
                //this->txtTimer.setVisible(false);
                this->BetweentwoPlayTime = *gtime;
                this->txtTimeNanBat->setString(StringUtils::format("%d", this->BetweentwoPlayTime));
                
                this->lblEndTimer->setString(StringUtils::format("%d", this->BetweentwoPlayTime));
                this->UpdateTimeEndGame();
                this->imgNextPlay->setVisible(true);
            }
            
        }
        else {
            this->CurrentGameState = true;
            
            // Director::getInstance()->getScheduler()->schedule(this->UpdateHayDatcuoc, 0.5);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameDoDenMini::updateTimer),this,1, false);
            
        }
    }
}
void LayerPlayGameDoDenMini::showPrevMatchHis(string str){
    vector<string> arrRg = mUtils::splitString(str, '_');
    
    if (arrRg.size() < 3)
        return;
    int total = atoi(arrRg.at(0).c_str()) + atoi(arrRg.at(1).c_str()) + atoi(arrRg.at(2).c_str());
    
    this->imgBat->setVisible(false);
    string url = this->GetUrlXiNgau(atoi(arrRg.at(0).c_str()));
    this->resultDice1->loadTexture(url);
    this->resultDice1->setVisible(true);
    
    url = this->GetUrlXiNgau(atoi(arrRg.at(1).c_str()));
    this->resultDice2->loadTexture(url);
    this->resultDice2->setVisible(true);
    
    url = this->GetUrlXiNgau(atoi(arrRg.at(2).c_str()));
    this->resultDice3->loadTexture(url);
    this->resultDice3->setVisible(true);
    
    if (total >= 2){
        this->currResult = "DO";
        this->imgHisNow->loadTexture("do-icon.png");
        this->imgBgDo->loadTexture("LayerDoDenMini/do-active.png");

        this->imgTai->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 0.8), ScaleTo::create(0.5, 1), NULL)));
    }
    else{
        this->currResult = "DEN";
        this->imgHisNow->loadTexture("icon-den.png");
        this->imgBgDen->loadTexture("LayerDoDenMini/den-active.png");

        this->imgXiu->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 0.8), ScaleTo::create(0.5, 1), NULL)));
    }
    //this->imgTongDiem->setVisible(true);
    this->txtTongDiem->setString(StringUtils::format("%d", total));
    
}
void LayerPlayGameDoDenMini::event_EXT_EVENT_GAME_BET_NTF(){
    //    var uid = this->param[EXT_FIELD_USERID];
    //    var gbv = this->param[EXT_FIELD_GAME_BET_VALUE];
    //    var gbt = this->param[EXT_FIELD_GAME_BET_TYPE];
    //    var gtbv = this->param[EXT_FIELD_GAME_TOTAL_BET_VALUE];
    boost::shared_ptr<string> gtbvc = this->param->GetUtfString(EXT_FIELD_GAME_TOTAL_BET_VALUE_CELL);
    boost::shared_ptr<string> ttp = this->param->GetUtfString("gtbuc");
    if (ttp != NULL) {
        this->showPlayer(*ttp);
    }
    if (gtbvc != NULL){
        this->showTongTienBet(*gtbvc);
    }
    
}
void LayerPlayGameDoDenMini::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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
            if (aalptr)
            {
                boost::shared_ptr<string> url = aalptr->GetStringValue();
                if (url && 0 == this->urlAvatar.compare(""))
                {
                    //chi can set 1 lan
                    this->urlAvatar = url->c_str();
                    //circleAvatar->setAvatarImage(url->c_str());
                }
            }
            if (this->currTypeMoney == 0){
                boost::shared_ptr<UserVariable> amfptr = user->GetVariable("amf");
                if (amfptr)
                {
                    boost::shared_ptr<double> amf = amfptr->GetDoubleValue();
                    if (amf)
                    {
                        this->currMyMoney = *amf;
                        string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
                        //						if (!this->Animating && !this->CurrentGameState)
                        //							this->txtMyMoney->setString(_money);
                        
                        this->amfUser = this->currMyMoney;
                    }
                }
                
            }
            else{
                boost::shared_ptr<UserVariable> amfptrs = user->GetVariable("amfs");
                if (amfptrs)
                {
                    boost::shared_ptr<double> amfs = amfptrs->GetDoubleValue();
                    if (amfs)
                    {
                        this->currMyMoney = *amfs;
                        string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
                        //						if (!this->Animating && !this->CurrentGameState)
                        //							this->txtMyMoney->setString(_money);
                        
                        this->amfUser = this->currMyMoney;
                    }
                }
                
            }
        }
    }
}

void LayerPlayGameDoDenMini::UpdateTimeStartGame() {
    // Tat dem nguoc den van tiep theo
    //this->lblCountNextTime.setVisible(false);
    //var str = "Ván tiếp: " + this->BetweentwoPlayTime;
    //this->lblCountNextTime.setString(str);
    //this->TimerCountNextTime = this->BetweentwoPlayTime;
    //Director::getInstance()->getScheduler()->unschedule(this->updateToNextTurn);
    
    // Dem timer thoi gian van choi
    //moc3
    //this->imgBat->setVisible(true);
    //moc3-end
    this->txtTimer->setVisible(true);
    this->TimerCountDown = this->PlayTurnTime;
    
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameDoDenMini::updateTimer),this,1, false);
}
void LayerPlayGameDoDenMini::updateTimer(float dt) {
    
    this->TimerCountDown -= 1;
    if (this->TimerCountDown < 0) {
        this->StopMyTimer();
        return;
    }
    string str = StringUtils::format("%d", this->TimerCountDown);
    // if (this->txtTimer != null) {
    this->txtTimer->setString(str);
    //moc3
    //this->imgBat->setVisible(true);
    //  }
    //moc3-end
    
    if (this->TimerCountDown == 0) {
        this->imgCanCua->setVisible(true);
        this->CurrentGameState = false;
    }
}
void LayerPlayGameDoDenMini::updateMoneyTraThuong(float dt){
    if (MoneyTraThuong <= 0){
        this->lblMoneyTraThuong->setString("0");
        return;
    }
    MoneyTraThuong -= 9000;
    this->lblMoneyTraThuong->setString(mUtils::convertMoneyEx(MoneyTraThuong));
    this->lblMoneyTraThuong->setVisible(true);
}
void LayerPlayGameDoDenMini::StopMyTimer(){
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameDoDenMini::updateTimer),this);
    this->TimerCountDown = this->PlayTurnTime;
}
void LayerPlayGameDoDenMini::CallBackResult(Ref *pSend){
    this->CallBackFromAnim();
}

void LayerPlayGameDoDenMini::CallBackFromAnim(){
    
    this->Animating = false;
    this->HoverFlag = true;
    
    this->showResult();
    this->txtTimer->setVisible(false);
    
    if (this->IsNanBat ) {
        
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameDoDenMini::timeNanBat),this,1, false);
        
        
        this->imgTimeNanBat->setVisible(true);
        this->txtTimeNanBat->setVisible(true);
        this->imgBatNan->setVisible(true);
        return;
    }
    
    this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create(CC_CALLBACK_0(LayerPlayGameDoDenMini::showResultAfter, this)), NULL));
    
}
void LayerPlayGameDoDenMini::showResultAfter(){
    
    
    
    if (!this->CurrentGameState)
        this->imgNextPlay->setVisible(true);
    if (this->money > 0){
        if (!this->isShowMoney)
        {
            
            this->showResultGame("+" + mUtils::convertMoneyEx(this->money), this->wincell);
            //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->amfUser));
        }
    }
}
void LayerPlayGameDoDenMini::showResultGame(string money, string result){
    
    string font_name = "font_win-font.fnt";
    if (money.find('-') != std::string::npos) //contain string '-'
        font_name = "font_lose-font.fnt";
    Label* labelResult = Label::createWithSystemFont(money, "", 50);
    labelResult->setColor(Color3B::YELLOW);
    labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    if (result == BET_TYPE_DO) {
        labelResult->setPosition(Vec2(this->txtDaDatTai->getPositionX(),this->txtDaDatTai->getPositionY()));
    }
    else {
        labelResult->setPosition(Vec2(this->txtDaDatXiu->getPositionX(),this->txtDaDatXiu->getPositionY()));
        
    }
    this->pnlBg1->addChild(labelResult);
    labelResult->setTag(TAG_RESULT_MONEY);
    labelResult->runAction(Sequence::create(MoveBy::create(1, Vec2(0, labelResult->getContentSize().height)),
                                            DelayTime::create(2),
                                            FadeOut::create(0.2), RemoveSelf::create(true), NULL));
    
    if (this->msg.length()>0 || this->msg != ""){
        SceneManager::getSingleton().sendEventNotify(this->msg);
    }
    this->msg = "";
}
void LayerPlayGameDoDenMini::showResult(){
    vector<string> arrRg = mUtils::splitString(this->rg, '-');
    
    if (arrRg.size() < 3)
        return;
    int total = atoi(arrRg.at(0).c_str()) + atoi(arrRg.at(1).c_str()) + atoi(arrRg.at(2).c_str());
    
    this->imgBat->setVisible(false);
    string url = this->GetUrlXiNgau(atoi(arrRg.at(0).c_str()));
    this->resultDice1->loadTexture(url);
    this->resultDice1->setVisible(true);
    
    url = this->GetUrlXiNgau(atoi(arrRg.at(1).c_str()));
    this->resultDice2->loadTexture(url);
    this->resultDice2->setVisible(true);
    
    url = this->GetUrlXiNgau(atoi(arrRg.at(2).c_str()));
    this->resultDice3->loadTexture(url);
    this->resultDice3->setVisible(true);
    
    
    if (this->IsNanBat) {
        //        //this->imgTongDiem->setVisible(true);
        this->txtTongDiem->setString(StringUtils::format("%d", total));
        
        return;
    }
    
    //this->imgTongDiem->setVisible(true);
    this->txtTongDiem->setString(StringUtils::format("%d", total));
    
    if (total >= 2){
        this->currResult = "DO";
        this->imgHisNow->loadTexture("do-icon.png");
        this->imgBgDo->loadTexture("LayerDoDenMini/do-active.png");

             this->imgTai->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 0.8), ScaleTo::create(0.5, 1), NULL)));
    }
    else{
        this->currResult = "DEN";
        this->imgBgDen->loadTexture("LayerDoDenMini/den-active.png");

        this->imgHisNow->loadTexture("icon-den.png");
         this->imgXiu->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 0.8), ScaleTo::create(0.5, 1), NULL)));
    }
    
}
string LayerPlayGameDoDenMini::GetUrlXiNgau(int number) {
    string num = "";
    switch (number){
        case 0:
            num = "den-show.png";
            break;
        case 1:
            num = "do-show.png";
    }
    return num;
}
void LayerPlayGameDoDenMini::UpdateTimeEndGame() {
    this->CurrentGameState = false;
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameDoDenMini::updateToNextTurn),this);
    
    this->TimerCountNextTime = this->BetweentwoPlayTime;
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameDoDenMini::updateToNextTurn),this,1, false);
    
}
void LayerPlayGameDoDenMini::updateToNextTurn(float dt){
    if (this->TimerCountNextTime == 0) {
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameDoDenMini::updateToNextTurn),this);
        this->imgNextPlay->setVisible(false);
        return;
    }
    if (this->TimerCountNextTime > 0) {
        this->TimerCountNextTime--;
    }
    else {
        //this->lblCountNextTime.setVisible(false);
        this->TimerCountNextTime = this->BetweentwoPlayTime;
        this->imgNextPlay->setVisible(false);
    }
    //var str = "Ván tiếp: " + this->TimerCountNextTime;
    this->txtTimeNanBat->setString(StringUtils::format("%d", this->TimerCountNextTime));
    
    this->lblEndTimer->setString(StringUtils::format("%d", this->TimerCountNextTime));
}
void LayerPlayGameDoDenMini::showTotalPlayer(string str){
    auto arrStr = mUtils::splitString(str, '|');
    for (int i = 0; i < arrStr.size(); i++){
        auto arrTotal = mUtils::splitString(arrStr[i], ':');
        if (arrTotal.size() < 2)
            return;
        if (strcmp(arrTotal[0].c_str(), BET_TYPE_DO) == 0){
            this->txtSoNguoiTai->setString(arrTotal[1]);
        }
        else{
            this->txtSoNguoiXiu->setString(arrTotal[1]);
        }
    }
}
void LayerPlayGameDoDenMini::ReloadGameWhenReturn(string gbt) {
    
    auto arrGbt = mUtils::splitString(gbt, ';');
    vector<string> info;
    
    //for (var i = 0; i < this->arrBet.length; ++i) {
    //    var pTaiXiu = this->arrBet.at(i);
    //    pTaiXiu.Reset();
    //}
    this->txtDaDatTai->setString("0");
    this->txtDaDatXiu->setString("0");
    int yourBet = 0;
    for (int i = 0; i < arrGbt.size(); ++i) {
        if (strcmp(arrGbt.at(i).c_str(), "") == 0) continue;
        info = mUtils::splitString(arrGbt[i], ':');
        if (strcmp(info[0].c_str(), BET_TYPE_DO) == 0){
            if (this->currTongBetTai < atoi(info[1].c_str()))
                this->currTongBetTai = atoi(info[1].c_str());
            this->txtTongBacTai->setString(mUtils::convertMoneyEx(atoi(info[1].c_str())));
            this->txtDaDatTai->setString(mUtils::convertMoneyEx(atoi(info[2].c_str())));
            this->currTongBetTai = atoi(info[1].c_str());
            this->currBetTai = atoi(info[2].c_str());
            int m = atoi(info[2].c_str());
            if (m > 0) {
                this->isAddMeBetTai = true;
            }
        }
        else{
            if (this->currTongBetXiu < atoi(info[1].c_str()))
                this->currTongBetXiu = atoi(info[1].c_str());
            this->txtTongBacXiu->setString(mUtils::convertMoneyEx(atoi(info[1].c_str())));
            this->currTongBetXiu = atoi(info[1].c_str());
            this->txtDaDatXiu->setString(mUtils::convertMoneyEx(atoi(info[2].c_str())));
            this->currBetXiu = atoi(info[2].c_str());
            int m = atoi(info[2].c_str());
            if (m > 0) {
                this->isAddMeBetXiu = true;
            }
        }
    }
    
    if (yourBet > 0) {
        //this->playerInfo.SoTienDatCuocServer = yourBet;
        //this->playerInfo.SoTienDatCuocClient = yourBet;
        //this->DisplayInfoMoney(this->playerInfo.SoTienThuc, yourBet);
    }
    
    arrGbt.clear();
}
void LayerPlayGameDoDenMini::showHistory(){
    
    if (this->ghs != NULL) {
        auto arr = mUtils::splitString(*this->ghs, ';');
        this->listHistory.clear();
        
        if (arr.size()<=16){
            for (int i = 0; i < arr.size(); ++i) {
                this->listHistory.push_back(arr.at(i));
            }
        }else{
            for (int i = arr.size()-17; i < arr.size(); ++i) {
                this->listHistory.push_back(arr.at(i));
            }
        }
        
    }
    
    for (int i = 0; i < this->lstimgHis.size(); i++){
        this->lstimgHis[i]->setVisible(false);
    }
    this->firstTimeOpen = false;
    if (this->listHistory.size() < 0)
        return;
    if (this->listHistory.size() <= 16){
        for (int i = 0; i < this->listHistory.size(); i++){
            auto arrHis = mUtils::splitString(this->listHistory[i], '|');
            if (arrHis.size() < 2)
                return;
            auto arrDice = mUtils::splitString(arrHis[1], '-');
            if (arrDice.size() < 3)
                return;
            int totalResult = atoi(arrDice[0].c_str()) + atoi(arrDice[1].c_str()) + atoi(arrDice[2].c_str());
            string strResult = "";
            if (totalResult >= 2){
                strResult = dataManager.GetSysString(591);
                this->lstimgHis[16 - this->listHistory.size() + i]->loadTexture("do-icon.png");
                this->lstimgHis[16 - this->listHistory.size() + i]->setVisible(true);
            }
            else{
                this->lstimgHis[16 - this->listHistory.size() + i]->loadTexture("icon-den.png");
                this->lstimgHis[16 - this->listHistory.size() + i]->setVisible(true);
                strResult = dataManager.GetSysString(592);
            }
            //
            //            var strResultDetail = "#" + arrHis[0]+"\n"+strResult +" "+ totalResult+ " ("+parseFloat(arrDice[0]) +" " +parseFloat(arrDice[1])+" "+parseFloat(arrDice[2])+")";
            //            this->lstTxtDetails[20- this->listHistory.length+i].setString(strResultDetail);
            
        }
    }
    else{
        for (int i = 1; i < this->listHistory.size(); i++){
            auto arrHis = mUtils::splitString(this->listHistory[i], '|');
            if (arrHis.size() < 2)
                return;
            auto arrDice = mUtils::splitString(arrHis[1], '-');
            if (arrDice.size() < 3)
                return;
            if (this->lstimgHis.size() < 0)
                return;
            auto totalResult = atoi(arrDice[0].c_str()) + atoi(arrDice[1].c_str()) + atoi(arrDice[2].c_str());
            string strResult = "";
            if (totalResult >= 2){
                strResult = dataManager.GetSysString(591);
                if (this->lstimgHis[i - 1] == NULL || i > this->lstimgHis.size())
                    return;
                this->lstimgHis[i - 1]->loadTexture("do-icon.png");
                this->lstimgHis[i - 1]->setVisible(true);
            }
            else{
                if (this->lstimgHis[i - 1] == NULL || i > this->lstimgHis.size())
                    return;
                this->lstimgHis[i - 1]->loadTexture("icon-den.png");
                this->lstimgHis[i - 1]->setVisible(true);
                strResult = dataManager.GetSysString(592);
            }
            //
            //            var strResultDetail = "#" + arrHis[0]+"\n"+strResult +" "+ totalResult+ " ("+parseFloat(arrDice[0]) +" " +parseFloat(arrDice[1])+" "+parseFloat(arrDice[2])+")";
            //            this->lstTxtDetails[i-1].setString(strResultDetail);
            //
        }
    }
    
}
void LayerPlayGameDoDenMini::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    //this->pnlBg1->setSwallowTouches(true);
    this->isTouchBat = false;
}
void LayerPlayGameDoDenMini::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    auto target = pnlBg1;
    auto delta = pTouch->getDelta();
    //  if (target.x ==0 || target.x == WIDTH_DESIGN-143 || target.y ==0 || target.y == HEIGHT_DESIGN-149)
    //     return;
    if (this->isTouchBat){
        //this->pnlBg1->setSwallowTouches(false);
        auto x = this->imgBatNan->getPositionX() + delta.x;
        auto y = this->imgBatNan->getPositionY() + delta.y;
        
        this->imgBatNan->setPosition(Vec2(x,y));
        
        auto rectBat = this->imgBatNan->getBoundingBox();
        auto rectKetQua = this->pnlKetQua->getBoundingBox();
        
        if (!rectBat.intersectsRect(rectKetQua)){
            this->chucnangNan();
            Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameDoDenMini::timeNanBat),this);

        }
        
        return ;
    }
    auto x = target->getPositionX() + delta.x;
    auto y = target->getPositionY() + delta.y;
    
    this->pnlBg1->setPosition(Vec2(x,y));
    this->imgBatNan->setPosition(Vec2(this->imgBatNan->getPositionX() + delta.x,this->imgBatNan->getPositionY() + delta.y));
    this->imgTimeNanBat->setPosition(Vec2(this->imgTimeNanBat->getPositionX() + delta.x,this->imgTimeNanBat->getPositionY() + delta.y));
    
    //    if (abs(delta.x) > 10 || abs(delta.y) > 10)
    //        this->isTouchMove =true;
    //    //listener->setSwallowTouches(true);
}
void LayerPlayGameDoDenMini::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPlayGameDoDenMini::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerPlayGameDoDenMini::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerPlayGameDoDenMini::onTouchEnded, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}
void LayerPlayGameDoDenMini::showPlayer(string str){
    auto arrNUM = mUtils::splitString(str, '|');
    for (int i = 0; i < arrNUM.size(); i++){
        auto arr = mUtils::splitString(arrNUM[i], ':');
        if (arr.size() < 2)
            return;
        if (strcmp(arr[0].c_str(), BET_TYPE_DO) == 0){
            if (this->currSonguoiTai < atoi(arr[1].c_str()))
                this->currSonguoiTai = atoi(arr[1].c_str());
            this->txtSoNguoiTai->setString(mUtils::convertMoneyEx(this->currSonguoiTai));
        }
        else{
            if (this->currSonguoiXiu < atoi(arr[1].c_str()))
                this->currSonguoiXiu = atoi(arr[1].c_str());
            this->txtSoNguoiXiu->setString(mUtils::convertMoneyEx(this->currSonguoiXiu));
            
        }
    }
}
void LayerPlayGameDoDenMini::showTongTienBet(string str){
    
    //log("bet tai xiu:%s", str.c_str());
    auto arrNUM = mUtils::splitString(str, '|');
    for (int i = 0; i < arrNUM.size(); i++){
        auto arr = mUtils::splitString(arrNUM[i], ':');
        if (arr.size() < 2)
            return;
        if (strcmp(arr[0].c_str(), BET_TYPE_DO) == 0){
            if (this->currTongBetTai < atoi(arr[1].c_str()))
                this->currTongBetTai = atoi(arr[1].c_str());
            this->txtTongBacTai->setString(mUtils::convertMoneyEx(this->currTongBetTai));
        }
        else{
            if (this->currTongBetXiu < atoi(arr[1].c_str()))
                this->currTongBetXiu = atoi(arr[1].c_str());
            this->txtTongBacXiu->setString(mUtils::convertMoneyEx(this->currTongBetXiu));
            
        }
    }
    
}
void LayerPlayGameDoDenMini::getMinMoney(){
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
void LayerPlayGameDoDenMini::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(listener);
    GameServer::getSingleton().removeListeners(this);
    SceneManager::getSingleton().setIsShowXD(false);
    if (!SceneManager::getSingleton().getIsShowTX() && !SceneManager::getSingleton().getIsShowXD()&& !SceneManager::getSingleton().getIsShowPK()&& !SceneManager::getSingleton().getIsShowVQ()&& !SceneManager::getSingleton().getIsShowCT()){
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return ;
        ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
        if (_scene)
        {
            _scene->showWebView();
        }
    }
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameDoDenMini::timeNanBat),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameDoDenMini::updateEvent),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameDoDenMini::GetQuy),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameDoDenMini::updateTimer),this);
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req(new ExtensionRequest("xdl", params));
    // GameServer::getSingleton().Send(req);
    Layer::onExit();
}

void LayerPlayGameDoDenMini::onButtonChat(Ref* pSender)
{
    bool isVisible = this->layerChat->isVisible();
    if (isVisible)
    {
        this->btnChatOpen->setVisible(true);
        this->btnChatClose->setVisible(false);
        this->layerChat->setVisible(false);
    }
    else{
        this->btnChatOpen->setVisible(false);
        this->btnChatClose->setVisible(true);
        this->layerChat->setVisible(true);
    }
}

LayerAnimationDoDenMini::LayerAnimationDoDenMini(){
    
}
LayerAnimationDoDenMini::LayerAnimationDoDenMini(Vec2& pos)
{
    this->m_callback2 = NULL;
    this->m_callbackListener2 = NULL;
    this->posFrame.x = pos.x;
    this->posFrame.y = pos.y;
}

LayerAnimationDoDenMini *LayerAnimationDoDenMini::createInstance(Vec2 &pos)
{
    LayerAnimationDoDenMini* layer = new LayerAnimationDoDenMini(pos);
    if (layer && layer->init()){
        layer->autorelease();
        return layer;
    }
    else{
        delete layer;
        layer = 0;
        return NULL;
    }
}

bool LayerAnimationDoDenMini::init(){
    if (!Layer::init()) {
        return false;
    }
    
    SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("animationddmini.plist");
    
    Vector<SpriteFrame*> animFrames(15);
    char str[100] = { 0 };
    for (int i = 1; i <= 8; ++i)
    {
        sprintf(str, "moc3-dia-%d.png", i);
        auto frame = frameCache->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1, 3);
    AnimationCache::getInstance()->addAnimation(animation, "dance");
    
    AnimationCache* animCache = AnimationCache::getInstance();
    Animation* normal = animCache->getAnimation("dance");
    normal->setRestoreOriginalFrame(true);
    
    Animate* animN = Animate::create(normal);
    
    auto _sprite = Sprite::create();
    auto frame = frameCache->getSpriteFrameByName("moc3-dia-1.png");
    _sprite->setSpriteFrame(frame);
    _sprite->setAnchorPoint(Vec2(0.5, 0.5));
    _sprite->setPosition(Vec2(this->posFrame.x, this->posFrame.y));
   // _sprite->setScale(1);
    this->addChild(_sprite);
    CallFunc* call = CallFunc::create(CC_CALLBACK_0(LayerAnimationDoDenMini::CallFuncRemove, this));
    
    _sprite->runAction(Sequence::create(animN, call, NULL));
    
    return true;
}

void LayerAnimationDoDenMini::CallFuncRemove(){
    if (this->m_callback2 && this->m_callbackListener2) {
        (this->m_callback2->*this->m_callbackListener2)(this);
    }
    this->stopAllActions();
    this->removeFromParent();
}

void LayerAnimationDoDenMini::SetCallBackResult(Ref* target, SEL_CallFuncO callfun){
    this->m_callback2 = target;
    this->m_callbackListener2 = callfun;
}


