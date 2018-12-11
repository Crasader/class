//
//  LayerCaoThapPopupGame.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 11/14/16.
//
//

#include "LayerCaoThapPopupGame.h"
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
#include "LayerCaoThapPopupLichSu.h"
#include "LayerCaoThapPopupXepHang.h"
#include <time.h>
#include "../scenes/ScenePickRoom.h"

USING_NS_CC;
using namespace cocos2d::ui;

LayerCaoThapPopupGame::LayerCaoThapPopupGame()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerCaoThapPopupGame::~LayerCaoThapPopupGame()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerCaoThapPopupGame::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerGameCaoThap.csb");
    
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
//    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }
//    else{
//        sizeAdd = Size(0, 0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    
    this->mQueueMsg = new ThreadQueueMsg();
    
    this->pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    this->pnlBg->setSwallowTouches(false);
    auto imgBg = static_cast<ImageView*>(this->pnlBg->getChildByName("imgBg"));
    imgBg->loadTexture(StringUtils::format("%s%smoc3-caothap-bg-popup.png",PATH_CT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));

    Button* btnClose = static_cast<Button*>(this->pnlBg->getChildByName("btnClose"));
    if (btnClose) {
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnClose, this));
        btnClose->setPressedActionEnabled(true);
    }
    Button* btnSuKien = static_cast<Button*>(this->pnlBg->getChildByName("btnSuKien"));
    if (btnSuKien) {
        btnSuKien->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnSuKien, this));
        btnSuKien->setPressedActionEnabled(true);
    }
    Button* btnXepHang = static_cast<Button*>(this->pnlBg->getChildByName("btnXepHang"));
    if (btnXepHang) {
        btnXepHang->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnXepHang, this));
        btnXepHang->setPressedActionEnabled(true);
    }
    
    Button* btnLichSu = static_cast<Button*>(this->pnlBg->getChildByName("btnLichSu"));
    if (btnLichSu) {
        btnLichSu->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnLichSu, this));
        btnLichSu->setPressedActionEnabled(true);
    }
    
    Button* btnHuongDan = static_cast<Button*>(this->pnlBg->getChildByName("btnHuongDan"));
    if (btnHuongDan) {
        btnHuongDan->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnHuongDan, this));
        btnHuongDan->setPressedActionEnabled(true);
    }
    
    this->txtPhien = static_cast<Text*>(this->pnlBg->getChildByName("txtPhien"));
    if (this->txtPhien)
        this->txtPhien->setString("#0");
    
    this->txtTongHu = static_cast<Text*>(this->pnlBg->getChildByName("txtTongHu"));
    if (this->txtTongHu)
        this->txtTongHu->setString("0");
    
    
    this->btnPlayPhienMoi = static_cast<Button*>(this->pnlBg->getChildByName("btnPlayPhienMoi"));
    if (this->btnPlayPhienMoi) {
        this->btnPlayPhienMoi->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnPlayPhienMoi, this));
        this->btnPlayPhienMoi->setPressedActionEnabled(true);
    }
    
    this->btnBetCao = static_cast<Button*>(this->pnlBg->getChildByName("btnBetCao"));
    if (this->btnBetCao) {
        this->btnBetCao->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnBetCao, this));
        this->btnBetCao->setPressedActionEnabled(true);
    }
    
    this->btnBetThap = static_cast<Button*>(this->pnlBg->getChildByName("btnBetThap"));
    if (this->btnBetThap) {
        this->btnBetThap->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnBetThap, this));
        this->btnBetThap->setPressedActionEnabled(true);
    }


    this->btnEndGame = static_cast<Button*>(this->pnlBg->getChildByName("btnEndGame"));
    if (this->btnEndGame) {
        this->btnEndGame->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnEndGame, this));
        this->btnEndGame->setPressedActionEnabled(true);
                this->btnEndGame->loadTextures(StringUtils::format("%s%smoc3-btn-luotmoi.png",PATH_CT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()), StringUtils::format("%s%smoc-3btn-luotmoi-disable.png",PATH_CT.c_str(),SceneManager::getSingleton().getFolderLang().c_str(),SceneManager::getSingleton().getFolderLang().c_str()), StringUtils::format("%s%smoc-3btn-luotmoi-disable.png",PATH_CT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    }
    
    this->btnMucCuoc1000 = static_cast<Button*>(this->pnlBg->getChildByName("btnMucCuoc1000"));
    if (this->btnMucCuoc1000) {
        this->btnMucCuoc1000->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnMucCuoc1000, this));
        this->btnMucCuoc1000->setPressedActionEnabled(true);
    }
    
    this->btnMucCuoc5000 = static_cast<Button*>(this->pnlBg->getChildByName("btnMucCuoc5000"));
    if (this->btnMucCuoc5000) {
        this->btnMucCuoc5000->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnMucCuoc5000, this));
        this->btnMucCuoc5000->setPressedActionEnabled(true);
    }
    
    this->btnMucCuoc10000 = static_cast<Button*>(this->pnlBg->getChildByName("btnMucCuoc10000"));
    if (this->btnMucCuoc10000) {
        this->btnMucCuoc10000->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnMucCuoc10000, this));
        this->btnMucCuoc10000->setPressedActionEnabled(true);
    }

    this->sprOpenCard = static_cast<Sprite*>(this->pnlBg->getChildByName("sprOpenCard"));
    this->sprOpenCard->setTexture(StringUtils::format("%s%smoc3-card-play.png",PATH_CT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    this->sprOpenCard->setScale(1);
    
    this->txtTienBetCao = static_cast<Text*>(this->pnlBg->getChildByName("txtTienBetCao"));
    this->txtTienNhan = static_cast<Text*>(this->pnlBg->getChildByName("txtTienNhan"));
    this->txtTienBetThap = static_cast<Text*>(this->pnlBg->getChildByName("txtTienBetThap"));
//    this->imgshowtiencao = static_cast<ImageView*>(this->pnlBg->getChildByName("imgshowtiencao"));
//    this->imgshowtien = static_cast<ImageView*>(this->pnlBg->getChildByName("imgshowtien"));
//    this->imgshowtienthap = static_cast<ImageView*>(this->pnlBg->getChildByName("imgshowtienthap"));
//
    this->txtThongBao = static_cast<Text*>(this->pnlBg->getChildByName("txtThongBao"));
    this->txtThongBao->setString(dataManager.GetSysString(897));
    this->imgStatus = static_cast<ImageView*>(this->pnlBg->getChildByName("imgStatus"));
    
    this->txtTimer = static_cast<Text*>(this->pnlBg->getChildByName("txtTimer"));
    
    
    this->imgbgQuanAt1 = static_cast<ImageView*>(this->pnlBg->getChildByName("imgbgQuanAt1"));
    this->imgbgQuanAt2 = static_cast<ImageView*>(this->pnlBg->getChildByName("imgbgQuanAt2"));
    this->imgbgQuanAt3 = static_cast<ImageView*>(this->pnlBg->getChildByName("imgbgQuanAt3"));

    this->lstA.push_back(this->imgbgQuanAt1);
    this->lstA.push_back(this->imgbgQuanAt2);
    this->lstA.push_back(this->imgbgQuanAt3);
    SceneManager::getSingleton().currRoomTypeCT = 0;
    //this->pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    this->onBtnMucCuoc1000(NULL);
    this->onGameReset();
    this->btnPlayPhienMoi->setVisible(false);

    this->mQueueMsg->setFunCallBack(this, callfunc_selector(LayerCaoThapPopupGame::OnExtensionResponseProcess));
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerCaoThapPopupGame::updateEvent),this,1/60, false);
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerCaoThapPopupGame::GetQuy),this,1/60, false);
    
    
    this->btnGold = static_cast<Button*>(this->pnlBg->getChildByName("btnGold"));
    if (this->btnGold){
        this->btnGold->setPressedActionEnabled(true);
        this->btnGold->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnGold, this));
    }
    this->btnSilver = static_cast<Button*>(this->pnlBg->getChildByName("btnSilver"));
    if (this->btnSilver){
        this->btnSilver->setPressedActionEnabled(true);
        this->btnSilver->setVisible(false);
        this->btnSilver->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnSilver, this));
    }

    SceneManager::getSingleton().showLoading();

    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt("tm", 0);
    boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_CAOTHAP_JOIN_GAME, params));
    GameServer::getSingleton().Send(req);
    
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    
    if (mySelf != NULL){
        if (mySelf->GetVariable("amf") != NULL) {
            auto myMoney = *mySelf->GetVariable("amf")->GetDoubleValue();
            this->currMyMoney = myMoney;
        }
    }

    
    //Cheat
//    this->txtFieldCheat = static_cast<TextField*>(this->pnlBg->getChildByName("txtCheat"));
//    Button* btnCheat = static_cast<Button*>(this->pnlBg->getChildByName("btnCheat"));
//    if (btnCheat) {
//        btnCheat->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupGame::onBtnSet, this));
//        btnCheat->setPressedActionEnabled(true);
//    }
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }
    return true;
}
void LayerCaoThapPopupGame::onBtnGold(Ref *sender){

    SceneManager::getSingleton().showLoading();
    
    //    this->btnGold->setEnabled(false);
    //    this->btnGold->setBright(false);
    //
    //    this->btnSilver->setEnabled(true);
    //    this->btnSilver->setBright(true);
    
    this->btnSilver->setVisible(true);
    this->btnGold->setVisible(false);
    
    this->txtTongHu->setString("0");
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req(new ExtensionRequest("ctl", params));
    GameServer::getSingleton().Send(req);
    this->onGameReset();
    
    
}
void LayerCaoThapPopupGame::onBtnSilver(Ref *sender){
 
    SceneManager::getSingleton().showLoading();
    //
    //    this->btnGold->setEnabled(true);
    //    this->btnGold->setBright(true);
    //
    //    this->btnSilver->setEnabled(false);
    //    this->btnSilver->setBright(false);
    this->btnSilver->setVisible(false);
    this->btnGold->setVisible(true);
    
    this->txtTongHu->setString("0");

    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req(new ExtensionRequest("ctl", params));
    GameServer::getSingleton().Send(req);
    this->onGameReset();
    
}
void LayerCaoThapPopupGame::updateEvent(float dt){
    if (this->mQueueMsg->isQueueEmpty())
        return;
    //    if (this->Animating)
    //        return;
    
    this->mQueueMsg->run();
}
void LayerCaoThapPopupGame::onBtnSet(Ref *sender){
//    boost::shared_ptr<ISFSObject> params(new SFSObject());
//    params->PutUtfString("glstc", this->txtFieldCheat->getString());
//    params->PutUtfString("uid", SceneManager::getSingleton().getMyName());
//    boost::shared_ptr<IRequest> req(new ExtensionRequest("ctctreq" , params));
//    GameServer::getSingleton().Send(req);

}
void LayerCaoThapPopupGame::onBtnBetCao(Ref *sender){
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("gbt", "0");
    boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_CAOTHAP_BET_CARD , params));
    GameServer::getSingleton().Send(req);
    this->PlayAnimlatBai();
    this->StopTimer();
    this->btnBetCao->setVisible(false);
    this->btnBetThap->setVisible(false);
    

}
void LayerCaoThapPopupGame::onBtnBetThap(Ref *sender){
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("gbt", "1");
    boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_CAOTHAP_BET_CARD , params));
    GameServer::getSingleton().Send(req);
    this->PlayAnimlatBai();
    this->StopTimer();
    this->btnBetCao->setVisible(false);
    this->btnBetThap->setVisible(false);
}
void LayerCaoThapPopupGame::onBtnHuongDan(Ref *sender) {
        LayerGuideInGame* layer = LayerGuideInGame::create();
       // string url = GUIDE_LINK;
       // if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
         string url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+GUIDE_P2_LINK;
        layer->setLink(StringUtils::format("%s%d", url.c_str(), 148));
        this->addChild(layer, 1000);
}
void LayerCaoThapPopupGame::onBtnLichSu(Ref *sender) {
    //
    LayerCaoThapPopupLichSu* popup = LayerCaoThapPopupLichSu::create();
    this->addChild(popup);
}
void LayerCaoThapPopupGame::updateAnimLatBat(float dt){
    int a = rand() % 14 + 2 ;
    int b = rand() % 4+1 ;
    if (a > 14)
        return;
    string str = this->getURL_byID_TienLen(StringUtils::format("%d",a), StringUtils::format("%d",b));
    this->sprOpenCard->initWithSpriteFrameName(str);
     this->sprOpenCard->setOpacity(128);
    this->sprOpenCard->setScale(2);
}
void LayerCaoThapPopupGame::PlayAnimlatBai(){
    this->sprOpenCard->setOpacity(128);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerCaoThapPopupGame::updateAnimLatBat),this,0.1, false);
}
void LayerCaoThapPopupGame::StopAnimlatBai(){
    //this->sprOpenCard->setOpacity(256);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCaoThapPopupGame::updateAnimLatBat),this);
}
void LayerCaoThapPopupGame::GetQuy(float dt){
    this->txtTongHu->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().huCaoThap));
}
void LayerCaoThapPopupGame::ShowQuanBaiLat(string strCard,string strType){
    this->StopAnimlatBai();
    string str = this->getURL_byID_TienLen(strCard,strType);
    this->sprOpenCard->initWithSpriteFrameName(str);
    this->sprOpenCard->setOpacity(225);
    this->sprOpenCard->setVisible(true);
    this->sprOpenCard->setScale(2);
}
std::string LayerCaoThapPopupGame::getURL_byID_TienLen(string pid, string pType)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		int num = atoi(pid.c_str());
		int type = atoi(pType.c_str());
		if (0 == pid.compare("0")) {
			num = 0;
			type = 0;
		}

		string url = "card_";
		switch (num) {
		case 11:
			url.append("j");
			break;
		case 12:
			url.append("q");
			break;
		case 13:
			url.append("k");
			break;
		case 14:
			url.append("a");
			break;
		default:
			url.append(StringUtils::format("%d", num));
			break;
		}

		url.append("_");

		// [0, 1, 2, 3] = ['s', 'c', 'd', 'h']
		switch (type) {
		case 4:
			url.append("s.png");
			break;
		case 3:
			url.append("c.png");
			break;
		case 2:
			url.append("d.png");
			break;
		case 1:
			url.append("h.png");
			break;
		}

		return url;
	}
	else
	{
		int num = atoi(pid.c_str());
		int type = atoi(pType.c_str());
		if (0 == pid.compare("0")) {
			num = 0;
			type = 0;
		}

		string url;
		switch (num) {
		case 11:
			url.append("j");
			break;
		case 12:
			url.append("q");
			break;
		case 13:
			url.append("k");
			break;
		case 14:
			url.append("a");
			break;
		default:
			url.append(StringUtils::format("%d", num));
			break;
		}

		url.append("-");

		// [0, 1, 2, 3] = ['s', 'c', 'd', 'h']
		switch (type) {
		case 4:
			url.append("bich");
			break;
		case 3:
			url.append("tep");
			break;
		case 2:
			url.append("ro");
			break;
		case 1:
			url.append("co");
			break;
		}
		url.append(".png");
		return url;
	}
}

void LayerCaoThapPopupGame::onBtnXepHang(Ref *sender) {
    //
    LayerCaoThapPopupXepHang* popup = LayerCaoThapPopupXepHang::create();
    this->addChild(popup);
}

void LayerCaoThapPopupGame::StartTimer(){
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerCaoThapPopupGame::CountDownTimer),this,1, false);
    //this->PlayAnimlatBai();
}
void LayerCaoThapPopupGame::StopTimer(){
    this->txtTimer->setVisible(false);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCaoThapPopupGame::CountDownTimer),this);
    //this->PlayAnimlatBai();
}

void LayerCaoThapPopupGame::CountDownTimer(float dt){
    this->currTimer -=1;
    this->txtTimer->setVisible(true);
    if (this->currTimer == 0)
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCaoThapPopupGame::CountDownTimer),this);
    int second=this->currTimer % 3600;
    int minute=second/60;
    second %= 60;
    if (second >= 10){
        if (minute >= 10)
            this->txtTimer->setString(StringUtils::format("%d:%d",minute,second));
        else
            this->txtTimer->setString(StringUtils::format("0%d:%d",minute,second));
    }
    else{
        if (minute >= 10)
            this->txtTimer->setString(StringUtils::format("%d:0%d",minute,second));
        else
            this->txtTimer->setString(StringUtils::format("0%d:0%d",minute,second));
    }
}
void LayerCaoThapPopupGame::onGameReset(){
    
    this->txtTienBetCao->setVisible(false);
    this->txtTienNhan->setString(mUtils::convertMoneyEx(this->currMucCuoc));
    this->txtTienBetThap->setVisible(false);
//    this->imgshowtiencao->setVisible(false);
//    this->imgshowtien->setVisible(true);
//    this->imgshowtienthap->setVisible(false);
    
    this->txtThongBao->setVisible(true);
    this->imgStatus->removeAllChildren();
    this->txtTimer->setVisible(false);
    
    this->sprOpenCard->setTexture(StringUtils::format("%s%smoc3-card-play.png",PATH_CT.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    this->sprOpenCard->stopAllActions();
    this->sprOpenCard->setScale(1);
    this->sprOpenCard->setOpacity(255);
    this->StopAnimlatBai();

    this->btnPlayPhienMoi->setEnabled(true);
    this->btnEndGame->setEnabled(false);
    
    this->isGameState = false;
    
    this->imgStatus->removeAllChildren();
    
    for (int j = 0;j<lstA.size();j++){
        this->lstA[j]->loadTexture("moc3-A-normal.png");
    }
    this->btnBetCao->setVisible(false);
    this->btnBetThap->setVisible(false);
    this->StopTimer();
    this->btnPlayPhienMoi->setVisible(true);

}
void LayerCaoThapPopupGame::onBtnMucCuoc1000(Ref *sender){
    
   //
        if (this->isGameState)
        {
            auto toast = Chat::create(dataManager.GetSysString(421),-1);
            this->addChild(toast);
            return;
        }
    
        this->btnMucCuoc1000->setEnabled(false);
        this->btnMucCuoc1000->setBright(false);

        this->btnMucCuoc5000->setEnabled(true);
        this->btnMucCuoc5000->setBright(true);

        this->btnMucCuoc10000->setEnabled(true);
        this->btnMucCuoc10000->setBright(true);

        this->currMucCuoc = 1000;
    
        this->GetQuy(0);
        
        onGameReset();
   
}
void LayerCaoThapPopupGame::onBtnMucCuoc5000(Ref *sender){
    
    if (this->isGameState)
    {
        auto toast = Chat::create(dataManager.GetSysString(421),-1);
        this->addChild(toast);
        return;
    }
   
        this->btnMucCuoc5000->setEnabled(false);
        this->btnMucCuoc5000->setBright(false);
        
        this->btnMucCuoc1000->setEnabled(true);
        this->btnMucCuoc1000->setBright(true);
        
        this->btnMucCuoc10000->setEnabled(true);
        this->btnMucCuoc10000->setBright(true);

        this->currMucCuoc = 5000;
        this->GetQuy(0);
        onGameReset();
    
}
void LayerCaoThapPopupGame::onBtnMucCuoc10000(Ref *sender){
    
    if (this->isGameState)
    {
        auto toast = Chat::create(dataManager.GetSysString(421),-1);
        this->addChild(toast);
        return;
    }
    
        this->btnMucCuoc10000->setEnabled(false);
        this->btnMucCuoc10000->setBright(false);
        
        this->btnMucCuoc1000->setEnabled(true);
        this->btnMucCuoc1000->setBright(true);
        
        this->btnMucCuoc5000->setEnabled(true);
        this->btnMucCuoc5000->setBright(true);

        this->currMucCuoc = 10000;
        this->GetQuy(0);
        onGameReset();
}
void LayerCaoThapPopupGame::getMinMoney(){
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
void LayerCaoThapPopupGame::onBtnPlayPhienMoi(Ref *sender) {
    this->getMinMoney();
    if (this->isGameState)
        return;
    if (this->currMucCuoc > this->currMyMoney){
        if (this->currTypeMoney == 0)
            SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
        else{
            SceneManager::getSingleton().showPopupNapTienGold(dataManager.GetSysString(978));
            
        }
        return;
    }
    if (SceneManager::getSingleton().getCurrRoomType() == this->currTypeMoney){
        if (this->currMucCuoc > (this->currMyMoney - this->MinBetValue)){
            if (this->currTypeMoney == 0)
                SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
            else{
                SceneManager::getSingleton().showPopupNapTienGold(dataManager.GetSysString(978));

            }
            return;
        }
    }
    SceneManager::getSingleton().setLastGameID(130);

    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("gbv",StringUtils::format("%d",this->currMucCuoc));
    boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_CAOTHAP_START, params));
    GameServer::getSingleton().Send(req);
    this->PlayAnimlatBai();
    this->btnPlayPhienMoi->setEnabled(false);
    this->isGameState = true;
}
void LayerCaoThapPopupGame::onBtnEndGame(Ref *sender) {
    this->btnBetCao->setVisible(false);
    this->btnBetThap->setVisible(false);
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_CAOTHAP_END, params));
    GameServer::getSingleton().Send(req);
    this->txtTimer->setVisible(false);
    this->StopTimer();
}
void LayerCaoThapPopupGame::onBtnClose(Ref *sender) {
    
        this->removeFromParentAndCleanup(true);
    
}

void LayerCaoThapPopupGame::onBtnSuKien(Ref *sender) {
    
        LayerPopupWebView* _layer = LayerPopupWebView::create();
        string url = LINK_EVENT_MINIPOKER;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        url = SKTX_P2;
        _layer->setWebView(url);
        this->addChild(_layer, 1000);

}
bool LayerCaoThapPopupGame::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
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
void LayerCaoThapPopupGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    pObjs->PutUtfString("_cmd_", *cmd);
    this->mQueueMsg->addQueue(pObjs);
}

void LayerCaoThapPopupGame::OnExtensionResponseProcess(){
    this->param = this->mQueueMsg->frontQueue();
    boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
    
    if (0 == cmd->compare(EXT_NOTIFY_CAOTHAP_GAME_STATE)) {
        this->event_EXT_EVENT_GAME_STATE_NTF();
    }
    else if (0 == cmd->compare(EXT_NOTIFY_CAOTHAP_START_GAME )) {
        this->event_EXT_EVENT_GAME_START();
    }
    else if (0 == cmd->compare(EXT_EVENT_CAOTHAP_GAME_RESULT)) {
        this->event_EXT_EVENT_GAME_RESULT();
    }
    else if (0 == cmd->compare(EXT_NOTIFY_CAOTHAP_END_GAME)){
        this->event_EXT_EVENT_GAME_END();
    }
    else if (0 == cmd->compare("cte_bntf")) {
        boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);
        boost::shared_ptr<string> bettype = this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
        boost::shared_ptr<string> betvalue = this->param->GetUtfString(EXT_FIELD_GAME_BET_VALUE);
        int i = *errc;
        this->ShowQuanBaiLat("6","4");
        auto toast = Chat::create(dataManager.GetSysString(423),-1);
        this->addChild(toast);
        this->onGameReset();
    }
    else if (0 == cmd->compare(EXT_EVENT_CAOTHAP_GET_QUY_RESP)) {
//        boost::shared_ptr<long> gbv = this->param->GetInt("gbv");
//
//        if (gbv){
//            this->txtTongHu->setString(StringUtils::format("%s",mUtils::convertMoneyEx(*gbv).c_str()));
//        }
        
    }
    else if (0 == cmd->compare("ctlrs")){
        if (this->currTypeMoney == 0){
           // SceneManager::getSingleton().hideLoading();
            
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutInt("tm",1);
            boost::shared_ptr<IRequest> req(new ExtensionRequest("ctj", params));
            GameServer::getSingleton().Send(req);
            this->currTypeMoney  = 1;
            SceneManager::getSingleton().currRoomTypeCT = 1;
        }else{
           // SceneManager::getSingleton().hideLoading();
            
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutInt("tm",0);
            boost::shared_ptr<IRequest> req(new ExtensionRequest("ctj", params));
            GameServer::getSingleton().Send(req);
            
            this->currTypeMoney  = 0;
            SceneManager::getSingleton().currRoomTypeCT = 0;

            
        }
    }
    this->mQueueMsg->deQueue();
}
void LayerCaoThapPopupGame::event_EXT_EVENT_GAME_RESULT(){
    boost::shared_ptr<long> mid = this->param->GetInt("mid");
    this->gwv= this->param->GetInt("gwv");
    this->glstc = this->param->GetUtfString("glstc");
    this->ibqt = this->param->GetInt("ibqt");
    if (*this->gwv <= 0){
        this->PlayAnimlatBai();
    }
    
}
void LayerCaoThapPopupGame::ShowLoseGame(string card,string type){
    
    
    
}
void LayerCaoThapPopupGame::event_EXT_EVENT_GAME_END(){
    if (!this->isGameState){
        return;
    }
    if (this->gwv){
        if (*this->gwv > 0){
            string money = StringUtils::format("+%s",mUtils::convertMoneyEx(*this->gwv).c_str());
            Label* labelResult = Label::createWithSystemFont(money, "", 50);
            labelResult->setColor(Color3B::YELLOW);
            labelResult->setAnchorPoint(Vec2(0.5,0.5));
            
            labelResult->setPosition(Vec2(219,185));
            this->pnlBg->addChild(labelResult);
            labelResult->setTag(this->TAG_RESULT_MONEY);
            labelResult->runAction(Sequence::create(MoveBy::create(1,Vec2(0,50)),DelayTime::create(2),FadeOut::create(0.2),RemoveSelf::create(true), NULL));
            this->StopTimer();
            if (this->glstc)
            {
                string strcard = *this->glstc;
                vector<string> lstcard = mUtils::splitString(strcard, ';');
                string strcardtype = lstcard[lstcard.size()-1];
                auto lstcardtype = mUtils::splitString(strcardtype, '_');
                if (lstcardtype.size() >=2)
                    this->ShowQuanBaiLat(lstcardtype[0], lstcardtype[1]);
                this->txtThongBao->setVisible(false);
                int anum = 0;
                this->imgStatus->removeAllChildren();
                for (int i = 0 ; i<lstcard.size();i++){
                    auto lstcardtypes = mUtils::splitString(lstcard[i], '_');
                    if (lstcardtypes.size()<2)
                        continue;
                    ItemKetQuaCaoThap* it = ItemKetQuaCaoThap::create();
                    it->setAnchorPoint(Vec2(0,0.5));
                    if (atoi(lstcardtypes[0].c_str()) == 14){
                        anum++;
                    }
                    it->setDatas(lstcardtypes[0], lstcardtypes[1]);
                    it->setPosition(Vec2(18+i*60+5,7));
                    this->imgStatus->addChild(it);
                }
                for (int j = 0;j<anum;j++){
                    this->lstA[j]->loadTexture("moc3-A-active.png");
                }
            }
            
            this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create(CC_CALLBACK_0(LayerCaoThapPopupGame::onGameReset, this)),NULL));
            
        }else{
            if (this->glstc)
            {
                string strcard = *this->glstc;
                vector<string> lstcard = mUtils::splitString(strcard, ';');
                string strcardtype = lstcard[lstcard.size()-1];
                auto lstcardtype = mUtils::splitString(strcardtype, '_');
                if (lstcardtype.size() >=2)
                    this->ShowQuanBaiLat(lstcardtype[0], lstcardtype[1]);
                this->txtThongBao->setVisible(false);
                int anum = 0;
                this->imgStatus->removeAllChildren();
                for (int i = 0 ; i<lstcard.size();i++){
                    auto lstcardtypes = mUtils::splitString(lstcard[i], '_');
                    if (lstcardtypes.size()<2)
                        continue;
                    ItemKetQuaCaoThap* it = ItemKetQuaCaoThap::create();
                    it->setAnchorPoint(Vec2(0,0.5));
                    if (atoi(lstcardtypes[0].c_str()) == 14){
                        anum++;
                    }
                    it->setDatas(lstcardtypes[0], lstcardtypes[1]);
                    it->setPosition(Vec2(18+i*60+5,7));
                    this->imgStatus->addChild(it);
                }
                for (int j = 0;j<anum;j++){
                    this->lstA[j]->loadTexture("moc3-A-active.png");
                }

                
                this->StopTimer();
                
                this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create(CC_CALLBACK_0(LayerCaoThapPopupGame::onGameReset, this)),NULL));
            }
            
        }
    }
    if (this->ibqt)
    {
        if (*this->ibqt == 1){
           // log("no hu roi");
        }
    }

}
void LayerCaoThapPopupGame::event_EXT_EVENT_GAME_START(){
    if (!this->isGameState){
        return;
    }
    boost::shared_ptr<long> mid = this->param->GetInt("mid");
    boost::shared_ptr<long> gtime = this->param->GetInt("gtime");
    boost::shared_ptr<string> glstc = this->param->GetUtfString("glstc");
    boost::shared_ptr<string> glsg = this->param->GetUtfString("glsg");
    bool isHideCao = false;
    bool isHideThap = false;
    if (mid)
        this->txtPhien->setString(StringUtils::format("#%d",*mid));
    if (gtime)
    {
     
        if (glstc)
        {
            string strcard = *glstc;
           // log("list card : %s",strcard.c_str());
            vector<string> lstcardOrg = mUtils::splitString(strcard, ';');
            vector<string> lstcard;
            int k = 0;
            if (lstcardOrg.size()<=15)
                k = 0;
            else
                k = lstcardOrg.size() - 15;
            for (int i = k ; i<lstcardOrg.size();i++){
                lstcard.push_back(lstcardOrg[i]);
            }
            string strcardtype = lstcard[lstcard.size()-1];
            auto lstcardtype = mUtils::splitString(strcardtype, '_');
            if (lstcardtype.size() >=2)
                this->ShowQuanBaiLat(lstcardtype[0], lstcardtype[1]);
            this->btnBetCao->setVisible(true);
            this->btnBetThap->setVisible(true);
            if (atoi(lstcardtype[0].c_str()) == 2){
                this->btnBetThap->setVisible(false);
                isHideThap = true;
            }
            if (atoi(lstcardtype[0].c_str()) == 14){
                this->btnBetCao->setVisible(false);
                isHideCao = true;
            }
            if (lstcard.size()>1)
                this->btnEndGame->setEnabled(true);
            this->txtThongBao->setVisible(false);
            int anum = 0;
            this->imgStatus->removeAllChildren();
       
            for (int i = 0 ; i<lstcard.size();i++){
                auto lstcardtypes = mUtils::splitString(lstcard[i], '_');
                if (lstcardtypes.size()<2)
                    continue;
                ItemKetQuaCaoThap* it = ItemKetQuaCaoThap::create();
                it->setAnchorPoint(Vec2(0,0.5));
                if (atoi(lstcardtypes[0].c_str()) == 14){
                    anum++;
                }
                it->setDatas(lstcardtypes[0], lstcardtypes[1]);
                it->setPosition(Vec2(18+i*60+5,7));
                this->imgStatus->addChild(it);
            }
            for (int j = 0;j<anum;j++){
                this->lstA[j]->loadTexture("moc3-A-active.png");
            }
        }
        if (glsg){
            string strbet = *glsg;
           // log("bet cao thap : %s",strbet.c_str());
            vector<string> lst = mUtils::splitString(strbet,';');
            if (lst.size()<3)
                return;
            this->txtTienBetCao->setString(mUtils::convertMoneyEx(atol(lst[1].c_str())));
            this->txtTienNhan->setString(mUtils::convertMoneyEx(atol(lst[0].c_str())));
            this->txtTienBetThap->setString(mUtils::convertMoneyEx(atol(lst[2].c_str())));
//            this->imgshowtiencao->setVisible(!isHideCao);
//            this->imgshowtien->setVisible(true);
//            this->imgshowtienthap->setVisible(!isHideThap);
            this->txtTienNhan->setVisible(true);
            this->txtTienBetCao->setVisible(!isHideCao);
            this->txtTienBetThap->setVisible(!isHideThap);

        }
        this->StopTimer();
        this->currTimer = *gtime;
        this->StartTimer();

        
    }
}
void LayerCaoThapPopupGame::event_EXT_EVENT_GAME_STATE_NTF(){
    SceneManager::getSingleton().hideLoading();
    boost::shared_ptr<long> mid = this->param->GetInt("mid");
    boost::shared_ptr<long> gs = this->param->GetInt("gstate");
    boost::shared_ptr<long> gtime = this->param->GetInt("gtime");
    boost::shared_ptr<string> ghs = this->param->GetUtfString("ghs");
    boost::shared_ptr<long> gbv = this->param->GetInt("gbv");
    this->GetQuy(0);
    bool isHideCao = false;
    bool isHideThap = false;
    this->btnPlayPhienMoi->setVisible(false);

    this->currMyMoney = SceneManager::getSingleton().getMoneyByType(this->currTypeMoney);
    if (mid)
        this->txtPhien->setString(StringUtils::format("#%d",*mid));
    if (gs)
    {
        log ("trang thai game : %d",*gs);
        if (*gs == 1){
            if (gtime){
                this->currTimer = *gtime;
                this->btnPlayPhienMoi->setEnabled(false);
                if (gbv){
                    if (*gbv == 1000){
                        this->onBtnMucCuoc1000(NULL);
                    }else if (*gbv == 5000){
                        this->onBtnMucCuoc5000(NULL);
                    }
                    else if (*gbv == 10000){
                        this->onBtnMucCuoc10000(NULL);
                    }
                }
                this->StartTimer();
                this->isGameState = true;
                this->btnPlayPhienMoi->setVisible(false);

            }

            if (ghs)
            {
                log ("trang thai game ve : %s",ghs->c_str());
                vector<string> lst = mUtils::splitString(*ghs, '|');
                if (lst.size() < 7)
                    return;
                string strcard = lst[3];
                vector<string> lstcardOrg = mUtils::splitString(strcard, ';');
                vector<string> lstcard;
                int k = 0;
                if (lstcardOrg.size()<=15)
                    k = 0;
                else
                    k = lstcardOrg.size() - 15;
                for (int i = k ; i<lstcardOrg.size();i++){
                    lstcard.push_back(lstcardOrg[i]);
                }
                string strcardtype = lstcard[lstcard.size()-1];
                auto lstcardtype = mUtils::splitString(strcardtype, '_');
                if (lstcardtype.size() >=1)
                    this->ShowQuanBaiLat(lstcardtype[0], lstcardtype[1]);
                this->btnBetCao->setVisible(true);
                this->btnBetThap->setVisible(true);
                if (atoi(lstcardtype[0].c_str()) == 2){
                    this->btnBetThap->setVisible(false);
                    isHideThap = true;
                }
                if (atoi(lstcardtype[0].c_str()) == 14){
                    this->btnBetCao->setVisible(false);
                    isHideCao = true;
                }
                if (lstcard.size()>=2)
                    this->btnEndGame->setEnabled(true);

                this->txtThongBao->setVisible(false);
                int anum = 0;
                this->imgStatus->removeAllChildren();
           
                for (int i = 0 ; i<lstcard.size();i++){
                    auto lstcardtypes = mUtils::splitString(lstcard[i], '_');
                    if (lstcardtypes.size()<2)
                        continue;
                    ItemKetQuaCaoThap* it = ItemKetQuaCaoThap::create();
                    it->setAnchorPoint(Vec2(0,0.5));
                    if (atoi(lstcardtypes[0].c_str()) == 14){
                        anum++;
                    }
                    it->setDatas(lstcardtypes[0], lstcardtypes[1]);
                    it->setPosition(Vec2(18+i*60+5,7));
                    this->imgStatus->addChild(it);
                }
                for (int j = 0;j<anum;j++){
                    this->lstA[j]->loadTexture("moc3-A-active.png");
                }
                
                    string strbet = lst[6];
                   // log("bet cao thap : %s",strbet.c_str());
                    vector<string> lstbet = mUtils::splitString(strbet,';');
                    if (lst.size()<3)
                        return;
                    this->txtTienBetCao->setString(mUtils::convertMoneyEx(atol(lstbet[1].c_str())));
                    this->txtTienNhan->setString(mUtils::convertMoneyEx(atol(lstbet[0].c_str())));
                    this->txtTienBetThap->setString(mUtils::convertMoneyEx(atol(lstbet[2].c_str())));
//                    this->imgshowtiencao->setVisible(!isHideCao);
//                    this->imgshowtien->setVisible(true);
//                    this->imgshowtienthap->setVisible(!isHideThap);
                    this->txtTienNhan->setVisible(true);
                    this->txtTienBetCao->setVisible(!isHideCao);
                    this->txtTienBetThap->setVisible(!isHideThap);
                  this->btnPlayPhienMoi->setVisible(false);

            }
        }else{
            this->txtThongBao->setVisible(true);
            this->btnPlayPhienMoi->setVisible(true);
            this->onGameReset();
        }
    }
}
void LayerCaoThapPopupGame::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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
            if (this->currTypeMoney  ==  0){
                boost::shared_ptr<UserVariable> amfptr = user->GetVariable("amf");
                if (amfptr)
                {
                    boost::shared_ptr<double> amf = amfptr->GetDoubleValue();
                    if (amf)
                    {
                        this->currMyMoney = *amf;
                        //                    string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
                        //                    if (!this->Animating && !this->CurrentGameState)
                        //                    this->txtMyMoney->setString(_money);
                        //
                        //                    //this->amfUser = this->currMyMoney;
                    }
                }

            }else{
                boost::shared_ptr<UserVariable> amfptr = user->GetVariable("amfs");
                if (amfptr)
                {
                    boost::shared_ptr<double> amf = amfptr->GetDoubleValue();
                    if (amf)
                    {
                        this->currMyMoney = *amf;
                        //                    string _money = mUtils::convertMoneyEx(long(this->currMyMoney));
                        //                    if (!this->Animating && !this->CurrentGameState)
                        //                    this->txtMyMoney->setString(_money);
                        //
                        //                    //this->amfUser = this->currMyMoney;
                    }
                }

            }
        }
    }
}
void LayerCaoThapPopupGame::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    auto target = this->pnlBg;
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
void LayerCaoThapPopupGame::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerCaoThapPopupGame::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerCaoThapPopupGame::onTouchMoved, this);

    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton()->addListeners(this);
}
void LayerCaoThapPopupGame::onExit()
{
    Layer::onExit();
    SceneManager::getSingleton().setIsShowCT(false);
    if (!SceneManager::getSingleton().getIsShowTX() && !SceneManager::getSingleton().getIsShowXD()&& !SceneManager::getSingleton().getIsShowPK()&& !SceneManager::getSingleton().getIsShowVQ()&& !SceneManager::getSingleton().getIsShowCT()){
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return ;
        ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
        if (_scene)
        {
            _scene->showWebView();
        }
    }
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(listener);
    boost::shared_ptr<ISFSObject> params(new SFSObject());

    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCaoThapPopupGame::updateEvent),this);
    
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCaoThapPopupGame::GetQuy),this);
    
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCaoThapPopupGame::updateAnimLatBat),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCaoThapPopupGame::CountDownTimer),this);

}
ItemKetQuaCaoThap::ItemKetQuaCaoThap(){
    
}
ItemKetQuaCaoThap::~ItemKetQuaCaoThap(){
    
}
bool ItemKetQuaCaoThap::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("ItemKetQuaCaoThap.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
    
    this->imgCard = static_cast<ImageView*>(root->getChildByName("imgCard"));
    this->txtCard = static_cast<Text*>(root->getChildByName("txtCard"));
    this->txtCard->setFontSize(this->txtCard->getFontSize());
    return true;
}
void ItemKetQuaCaoThap::setDatas(string pCard, string pType){
    string card= pCard;
    switch (atoi(pCard.c_str())) {
        case 11:
            card = "J";
            break;
        case 12:
            card = "Q";
            break;
        case 13:
            card = "K";
            break;
        case 14:
            card = "A";
            break;
    }
    
    this->txtCard->setString(card);
    
    string url = "";
    switch (atoi(pType.c_str())) {
        case 1:
            url = "moc3-chatco.png";
            break;
            
        case 2:
            url = "moc3-chatro.png";
            break;
        case 3:
            url = "moc3-chattep.png";
            break;
        case 4:
            url = "moc3-chatbich.png";
            break;
    }
    this->imgCard->loadTexture(url);

}
