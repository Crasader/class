//
//  LayerChanGame.cpp
//  iCasinov2
//
//  Created by DauA on 6/4/14.
//
//

#include "LayerChanGame.h"
#include "_Background_inGame_.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/SetRoomVariablesRequest.h"
#include "Requests/SpectatorToPlayerRequest.h"

#include "CardChan.h"
#include "LayerGameChan_XuongU.h"
#include "SceneManager.h"
#include "LayerGameChan_KetQua.h"
#include "Layer_GivePocker_Chan.h"
#include "LayerConfigRoomChan.h"
#include "../layers/LayerGuideInGame.h"
#include "AllData.h"
#include "xmllite/xmlParser.h"
#include "LayerConnecting.h"
#include <stdexcept>
#include "PhomMessDef.h"
#include "../logfile.h"

#define TIMEOUT_CHIU 5

#define KTAG_LAYER_GIVE		200
#define KTAG_LAYER_RESUILT	201
#define KTAG_LAYER_XUONG_U	202
#define KTAG_LABEL_CUOC		203
#define KTAG_LAYER_CONFIG	204
#define KTAG_LAYER_CONNECTING 9981

#define z_Order_layer_setting 107
#define z_Order_layer_huongdan 106
#define z_Order_layer_chan_toast 105
#define z_Order_lblCuoc 104
#define z_Order_layer_ketqua 103
#define z_Order_layer_xuong_u 102
#define z_Order_layer_button 101
#define z_Order_layer_give 100
#define z_Order_layer_avatar 99
#define z_Order_layer_card  98
#define z_Order_layer_connecting 100000

LayerChanGame::LayerChanGame()
:LayerAvatars(NULL),
layerButtons(NULL),
LayerCardsChan(NULL),
Toast(NULL),
LblDetail(NULL),
LblRoomName(NULL),
LblSuggestU(NULL),
TimerMe(NULL),
_ga(NULL),
_bg(NULL),
LblCoinGa(NULL),
MyAI(""),
Step_Timeout(30),
ListPlayers(""),
CurrentPlayer(""),
StrResonAnBao(""),
CountBocCai(0),
CountPlayers(0),
CountNocs(0),
HasGiveCard(false),
HasNotifyU(false),
FlagAnBao(false),
IsGamePlaying(false),
FlagSitDown(true),
IsNuoiGa(false),
IsUXuong(false),
IsSpect(true),
showAvatarMe(false),
isRegistLeave(false),
mThreadQueueMsg(NULL),
isChiuPending(false),
_countPendingChiu(0) {
	this->mapPlayers.clear();
	this->State = PlayingState::BEGIN;
	this->SizeAdd = ChanUtils::getSizePos();
}

LayerChanGame::~LayerChanGame() {
	log("I am comment test git ubuntu here");
	if (this->getChildByTag(HEIGHT_DESIGN) != NULL)
		this->removeChildByTag(HEIGHT_DESIGN);
	if (this->getChildByTag(WIDTH_DESIGN) != NULL)
		this->removeChildByTag(WIDTH_DESIGN);

	if (SceneManager::getSingletonPtr() != NULL) {
		//HoangDD
		//SceneManager::getSingleton().setBackgroundScreen(false);
	}

	// Huy singletons
	ButtonChanNotification::GetInstance()->RemoveAllObserver();
	ButtonChanNotification::DestroyInstance();

	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerChanGame::updateEvent),this);

	GameServer::getSingleton().removeListeners(this);

	this->CauHinh.reset();

	if (mThreadQueueMsg != NULL) delete mThreadQueueMsg;
	log("Deconstructor Game Chan");
}

bool LayerChanGame::init() {
	if (!Layer::init()) return false;

	if (GameServer::getSingleton().getSmartFox()->MySelf() == NULL) return false;
	this->MyAI = *GameServer::getSingleton().getSmartFox()->MySelf()->Name();

	int KieuBan = SceneManager::getSingleton().GetKieuBanChan();
	string ss = "chan_bg_1.jpg";
	//     if (KieuBan == 2) ss = "chan_bg_2.jpg";
	//     else if (KieuBan == 3) ss = "chan_bg_3.jpg";

	Size winsize = Director::getInstance()->getVisibleSize();
	this->_bg = ImageView::create();
	this->_bg->loadTexture(ss);
	this->_bg->setTouchEnabled(false);
	this->_bg->setScaleX(winsize.width / this->_bg->getContentSize().width);
	this->_bg->setScaleY(winsize.height / this->_bg->getContentSize().height);
	this->_bg->setAnchorPoint(Vec2(0, 0));
	this->_bg->setPosition(Vec2(-this->SizeAdd.width, -this->SizeAdd.height));
	this->addChild(this->_bg);

	//    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
	//        Sprite* bgchieu = Sprite::create("bg-chieu.png");
	//        bgchieu->setPosition(Vec2(WIDTH_DESIGN/2-this->SizeAdd.width, HEIGHT_DESIGN/2-this->SizeAdd.height));
	//        this->addChild(bgchieu);
	//    }

	this->CreateCards();
	this->CreateAvatars();
	this->CreateButtons();

	this->LblDetail = Label::createWithBMFont(LucidaGrandeFnt, "C:BD:C:Q");
	this->LblDetail->setScale(1.6);
	this->LblDetail->setAnchorPoint(Vec2(1, 0));
	this->LblDetail->setPosition(Vec2(WIDTH_DESIGN - 20, HEIGHT_DESIGN - 50 + this->SizeAdd.height));
	this->LblDetail->setColor(Color3B::RED);
	this->addChild(this->LblDetail);
	this->LblDetail->setVisible(false);

	this->LblRoomName = Label::createWithSystemFont("","", 25);
	this->LblRoomName->setColor(_colorTextNormal);
	this->LblRoomName->setScale(1);
	this->LblRoomName->setString(dataManager.GetSysString(107)+ " ");
	this->LblRoomName->setAnchorPoint(Vec2(0, 1));
	this->LblRoomName->setPosition(Vec2(350 - this->SizeAdd.width, HEIGHT_DESIGN - 20 + this->SizeAdd.height));
	this->addChild(this->LblRoomName);

	this->LblSuggestU = Label::createWithSystemFont("","", 25);
	this->LblSuggestU->setColor(_colorTextNormal);
	this->LblSuggestU->setScale(1);
	this->LblSuggestU->setAnchorPoint(Vec2(0, 1));
	this->LblSuggestU->setPosition(Vec2(this->LblRoomName->getPositionX(), HEIGHT_DESIGN - 60 + SizeAdd.height));
	this->addChild(this->LblSuggestU);

	Sprite* scoin = Sprite::create("moc3-icon-moc.png");
	int roomType = SceneManager::getSingleton().getCurrRoomType();

	if (roomType == 0){
		scoin->setTexture("moc3-icon-moc.png");
	}
	else{
		scoin->setTexture("moc3-icon-hao.png");

	}
	scoin->setAnchorPoint(Vec2(0, 1));
	scoin->setPosition(Vec2(175, HEIGHT_DESIGN - 20 + this->SizeAdd.height));
	scoin->setScale(0.75);
	this->addChild(scoin);

	this->LblCuocBan = Label::createWithSystemFont("","", 25);
	this->LblCuocBan->setColor(_colorTextNormal);
	this->LblCuocBan->setScale(1);
	this->LblCuocBan->setString("1,000");
	this->LblCuocBan->setAnchorPoint(Vec2(0, 1));
	this->LblCuocBan->setPosition(Vec2(scoin->getPositionX() + 50, HEIGHT_DESIGN - 20 + this->SizeAdd.height));
	this->addChild(this->LblCuocBan);

	this->_ga = ImageView::create();
	this->_ga->loadTexture("ga_enable.png", Widget::TextureResType::PLIST);
	this->_ga->setAnchorPoint(Vec2(0, 1));
	this->_ga->setPosition(Vec2(175, HEIGHT_DESIGN - 50 + this->SizeAdd.height));
	this->_ga->setScale(0.6);
	this->addChild(this->_ga);

	this->LblCoinGa = Label::createWithSystemFont("","", 25);
	this->LblCoinGa->setAnchorPoint(Vec2(0, 1));
	this->LblCoinGa->setColor(_colorTextNormal);
	this->LblCoinGa->setPosition(Vec2(this->_ga->getPositionX() + 50, HEIGHT_DESIGN - 65 + this->SizeAdd.height));
	this->LblCoinGa->setScale(1.1);
	this->addChild(this->LblCoinGa);

	this->mThreadQueueMsg = new ThreadQueueMsg();
	this->mThreadQueueMsg->setFunCallBack(this, callfunc_selector(LayerChanGame::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerChanGame::updateEvent),this,1/60, false);

	GameServer::getSingleton().addListeners(this);
	//this->scheduleOnce(schedule_selector(LayerChanGame::SendRequestJoinGame), 0.5f);
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerChanGame::SendRequestJoinGame), this, 0.5f, 1, 0.5f, false);

	// Tạo LayerToast để hiển thị các thông báo.
	this->Toast = LayerChanToast::create();
	this->Toast->setLocalZOrder(z_Order_layer_chan_toast);
	this->addChild(this->Toast);

	// set this->Toast cho Layer Card Chan
	this->LayerCardsChan->SetLayerChanToast(this->Toast);

	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();

	this->SetCauHinhRoom(lastRoom);
	this->SetRoomVar(lastRoom);

	std::string stringThongBao = "";

	if (this->CauHinh.choiUXuong) {
		stringThongBao = dataManager.GetSysString(104);
	}

	if (this->CauHinh.uDuoi4Diem == 4) {
		if (stringThongBao != "") {
			stringThongBao += ", "+dataManager.GetSysString(105);
		}
		else {
			stringThongBao = dataManager.GetSysString(106);
		}
	}

	if (stringThongBao != "")
	{
		//this->Toast->ShowToast(stringThongBao);
	}

	log("after init : %s", this->MyAI.c_str());

	return true;
}

void LayerChanGame::SetCauHinhRoom(boost::shared_ptr<Room> lastRoom){
	this->CauHinh = this->SetRoomVar(lastRoom);
	this->CauHinh.commentU = true;
	this->CauHinh.commentChiu = true;
}

CauHinhChanRoom LayerChanGame::SetRoomVar(boost::shared_ptr<Room> lastRoom){

	CauHinhChanRoom __config;

	if (lastRoom != NULL) {

		std::string r = boost::to_string(lastRoom->Id());
		//HoangDD Change
		string sRoomIdDisplay = mUtils::getRoomIDByName(*lastRoom->Name());
		this->LblRoomName->setString(dataManager.GetSysString(107)+" " + sRoomIdDisplay);

		boost::shared_ptr<RoomVariable> rv = lastRoom->GetVariable("params");

		if (rv != NULL) {
			string StrParam = *rv->GetStringValue();

			log("params %s", StrParam.c_str());

			vector<string> ArrParams = mUtils::splitString(StrParam, '@');

			try {
				long f = atol(ArrParams.at(0).c_str());
				this->LblCuocBan->setString(mUtils::convertMoneyEx(f));

				std::string typeRoom = NewCommon::getTypeRoom(f);
				typeRoom = dataManager.GetSysString(107)+ " ";
				typeRoom += sRoomIdDisplay;
				//this->LblRoomName->setString(typeRoom);

			}
			catch (std::out_of_range& e) {
				log("e = %s", e.what());
			}
			catch (std::exception& e){
				log("e = %s", e.what());
			}

			if (ArrParams.size() == 3) return __config;

			string xml = "<?xml version=\"1.0\"?>" + ArrParams.back();
			log("config: %s", xml.c_str());

			XMLResults xe;
			XMLNodeEx xMainNode = XMLNodeEx::parseString(xml.c_str(), NULL, &xe);

			if (xe.error != XMLError::eXMLErrorNone)
				return __config;

			const char* StrVar = "";
			bool BoolVar = false;

			StrVar = xMainNode.getChildNodeByPath("tableconfig.timeout.stepTimeout", true, '.').getText();
			if (StrVar == NULL)
				StrVar = "30";
			this->Step_Timeout = atof(StrVar);
			log("Step time out = %f", this->Step_Timeout);

			__config.timeout = this->Step_Timeout;

			StrVar = xMainNode.getChildNodeByPath("tableconfig.playing_rule.choiNuoiGa", true, '.').getText();
			if (StrVar == NULL)
				StrVar = "1";
			log("choiNuoiGa = %s", StrVar);
			BoolVar = 0 == strcmp(StrVar, "1");

			__config.isNuoiGa = BoolVar;

			string strToast = this->FindBossRoom();
			if (0 == strToast.compare(""))
				strToast = "";

			if (0 == strToast.compare(this->MyAI)) {
				strToast = dataManager.GetSysString(108);
			}
			else {
				strToast = ChanUtils::GetNamePlayer(strToast);
			}

			if (BoolVar) {
				this->_ga->loadTexture("ga_enable.png", Widget::TextureResType::PLIST);
				strToast = strToast + " "+dataManager.GetSysString(109);
			}

			if (!BoolVar) {
				this->_ga->loadTexture("ga_disable.png", Widget::TextureResType::PLIST);
				strToast = strToast + " "+dataManager.GetSysString(110);
			}
			if (this->IsNuoiGa != BoolVar) {
				this->IsNuoiGa = BoolVar;
				this->Toast->ShowToast(strToast.c_str());
			}

			StrVar = xMainNode.getChildNodeByPath("tableconfig.playing_rule.choiUXuong", true, '.').getText();
			if (StrVar == NULL)
				StrVar = "0";
			log("choiUXuong = %s", StrVar);
			BoolVar = 0 == strcmp(StrVar, "0");

			__config.choiUXuong = BoolVar;

			if (BoolVar) {
				this->LblSuggestU->setString(dataManager.GetSysString(111));
			}
			else {
				this->LblSuggestU->setString(dataManager.GetSysString(112));
			}

			if (BoolVar && !IsUXuong) {
				//this->Toast->ShowToast(dataManager.GetSysString(113));
			}

			this->IsUXuong = BoolVar;

			StrVar = xMainNode.getChildNodeByPath("tableconfig.playing_rule.choiDiemToiThieu", true, '.').getText();
			if (StrVar == NULL)
				StrVar = "0";

			log("choiDiemToiThieu = %s", StrVar);
			BoolVar = 0 == strcmp(StrVar, "4");
			__config.uDuoi4Diem = BoolVar;

			if (BoolVar) {
				this->LblSuggestU->setString(dataManager.GetSysString(84));
				//this->Toast->ShowToast("Không chơi ù dưới 4 điểm");
			}
		}
	}
	return __config;
}

void LayerChanGame::DisPlayAvatarMe(bool show) {
	Avatar* me = this->LayerAvatars->getUserByPos(kUserMe);
	Avatar* bot = this->LayerAvatars->getUserByPos(kUserBot);

	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL)
		return;

	std::string name = this->MyAI;
	float money = 0;
	std::string _url = "";

	boost::shared_ptr<UserVariable> aal = myself->GetVariable("aal");
	boost::shared_ptr<UserVariable> amf;
	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		amf = myself->GetVariable("amf");
	}
	else{
		amf = myself->GetVariable("amfs");
	}
	/*moc3-change aN=>adn*/
	boost::shared_ptr<UserVariable> aN = myself->GetVariable("adn");

	if (aal != NULL) {
		_url = *aal->GetStringValue();
	}
	if (amf != NULL) {
		money = *amf->GetDoubleValue();
	}
	if (aN != NULL) {
		name = *aN->GetStringValue();
	}
	if (0 == name.compare(""))
		name = *myself->Name();

	if (!this->IsSpect){
		if (!show){
			me->setVisibleLayerInvite(false);
			me->setAI(this->MyAI);
			me->setMoney(money);
			me->setIcon(_url);
			me->setName(name);
			me->setVisible(true);

			bot->setPositionY(-HEIGHT_DESIGN >> 1);
			bot->setVisible(true);
			this->showAvatarMe = true;
		}
		else
		{
			me->setVisibleLayerInvite(true);
			me->setVisible(false);
			bot->setPositionY(200 - this->SizeAdd.height);
			bot->setVisible(true);
			this->showAvatarMe = false;
		}
	}
	else
	{
		me->setVisibleLayerInvite(false);
		me->setAI(this->MyAI);
		me->setMoney(money);
		me->setIcon(_url);
		me->setName(name);
		me->setVisible(true);

		bot->setPositionY(200 - this->SizeAdd.height);
		bot->setVisible(true);
		this->showAvatarMe = true;
	}
}

bool LayerChanGame::checkConfigChanged(CauHinhChanRoom config){
	bool ret = false;

	if (this->CauHinh.timeout != config.timeout) {
		this->CauHinh.timeout = config.timeout;
		ret = true;
	}

	if (this->CauHinh.isNuoiGa != config.isNuoiGa) {
		this->CauHinh.isNuoiGa = config.isNuoiGa;
		ret = true;
	}

	if (this->CauHinh.choiUXuong != config.choiUXuong) {
		this->CauHinh.choiUXuong = config.choiUXuong;
		ret = true;
	}

	if (this->CauHinh.uDuoi4Diem != config.uDuoi4Diem) {
		this->CauHinh.uDuoi4Diem = config.uDuoi4Diem;
		ret = true;
	}

	return ret;
}

void LayerChanGame::DisplayLayerKetQua(string& result) {
	LayerMocChatInGame* chat = this->layerButtons->getLayerChat();
	LayerGameChan_KetQua *kq = LayerGameChan_KetQua::createInstance(result, chat);
	if (kq != NULL) {
		kq->SetCallBack_Close(this, callfuncO_selector(LayerChanGame::CallBackFunc_ResetGame));
		this->addChild(kq, z_Order_layer_ketqua, KTAG_LAYER_RESUILT);
	}
}

void LayerChanGame::DisPlayOnlyBtnU() {
	if (!this->IsSpect && this->HasGiveCard && !this->HasNotifyU) {
		this->BtnDisCard->Disable();
		this->BtnTakeCard->Disable();
		this->BtnDrawCard->Disable();
		this->BtnDuoiCard->Disable();
		this->BtnBaoU->Enable();
	}
}

void LayerChanGame::CreateAvatars() {
	this->LayerAvatars = LayerAvatarInGame::create();
	this->LayerAvatars->resetAll();

	Avatar* me = this->LayerAvatars->getUserByPos(kUserMe);
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf != NULL){
		if (SceneManager::getSingleton().getCurrRoomType() == 0){

			double amf = *mySelf->GetVariable("amf")->GetDoubleValue();
			me->setMoney(amf);
		}
		else{
			double amf = *mySelf->GetVariable("amfs")->GetDoubleValue();
			me->setMoney(amf);
		}
		//set aal
		//set exp
	}
	me->setVisible(false);

	this->DisPlayAvatarMe(true);

	this->addChild(this->LayerAvatars, z_Order_layer_avatar);
}

void LayerChanGame::CreateCards() {
	this->LayerCardsChan = _Layer_CardChan_::create();
	this->LayerCardsChan->SetCallBackListCards(this, callfuncO_selector(LayerChanGame::CallBackFunc_ListCards));
	this->LayerCardsChan->SetCallBackDisCards(this, callfuncO_selector(LayerChanGame::CallBackAfterDisCard));
	this->addChild(this->LayerCardsChan, z_Order_layer_card);
}

void LayerChanGame::CreateButtons() {
	this->layerButtons = LayerButtonInGame::create();
	this->layerButtons->hideButtonInChanGame();
	this->addChild(layerButtons, z_Order_layer_button);

    if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL){
        return;
    }
    
	boost::shared_ptr<RoomVariable> isVipRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("isviproom");
	if (isVipRoom != NULL){
		int vipRoom = *isVipRoom->GetIntValue();
		if (vipRoom == 0){
			this->layerButtons->processDisplayButtonInvite(false);
		}
		else{
			this->layerButtons->processDisplayButtonInvite(true);
		}
	}

	Button* btnSetting = Button::create();
	btnSetting->setTouchEnabled(true);
	btnSetting->setScale9Enabled(false);
	btnSetting->setAnchorPoint(Vec2(1, 1));
	btnSetting->loadTextures("button_setting.png", "button_setting.png", "");
	btnSetting->setPosition(Vec2(WIDTH_DESIGN - 20 + SizeAdd.width, HEIGHT_DESIGN - 20 + SizeAdd.height));
	btnSetting->addTouchEventListener(CC_CALLBACK_2(LayerChanGame::OnBtnSetting_Chan, this));
	this->layerButtons->addWidgetEx(btnSetting);

	Button* btnHuongDan = Button::create();
	btnHuongDan->setTouchEnabled(true);
	btnHuongDan->setScale9Enabled(false);
	btnHuongDan->setAnchorPoint(Vec2(1, 1));
	btnHuongDan->loadTextures("top-chan-tutorial.png", "top-chan-tutorial.png", "");
	btnHuongDan->setPosition(Vec2(WIDTH_DESIGN - 40 - btnSetting->getContentSize().width + SizeAdd.width, HEIGHT_DESIGN - 20 + SizeAdd.height));
	btnHuongDan->setPressedActionEnabled(true);
	btnHuongDan->addTouchEventListener(CC_CALLBACK_2(LayerChanGame::OnBtnHuongDanChan, this));
	this->layerButtons->addChild(btnHuongDan);

	Button* btnChat = (Button*)this->layerButtons->getChildByTag(TAG_LB_BTN_CHAT);
	if (btnChat != NULL){
		btnChat->setAnchorPoint(Vec2(1, 1));
		btnChat->loadTextures("button_chat_chan.png", "button_chat_chan.png", "");
		btnChat->setPosition(Vec2(btnHuongDan->getPositionX() - btnHuongDan->getContentSize().width - 15.0, btnHuongDan->getPositionY()));
		btnChat->setVisible(true);
        if (dataManager.getAppConfig().is24h) {
            btnChat->setVisible(false);
        }
	}
    

	Button* btnMission = (Button*)this->layerButtons->getChildByTag(TAG_LB_BTN_MISSION);
	if (btnMission != NULL){

		btnMission->setAnchorPoint(Vec2(1, 0));
		btnMission->setPosition(Vec2(WIDTH_DESIGN - 30 + this->SizeAdd.width - btnMission->getContentSize().width, 100 - this->SizeAdd.height + btnChat->getContentSize().height / 2 * 3));
		btnMission->setVisible(true);
	}


	Sprite* signal = (Sprite*)this->layerButtons->getChildByTag(120);
	if (signal != NULL){
		signal->setAnchorPoint(Vec2(1, 1));
		signal->setPosition(Vec2(WIDTH_DESIGN - 60 - btnSetting->getContentSize().width * 2 + SizeAdd.width, HEIGHT_DESIGN - 50 + SizeAdd.height));
		//signal->setVisible(false);
	}

	BagItem* bagitem = (BagItem*)this->layerButtons->getChildByTag(982);
	if (bagitem != NULL) {
		Vec2 pos = btnChat->getPosition();
		bagitem->setAnchorPoint(Vec2(1, 0));
		bagitem->setPosition(Vec2(pos.x - 40, pos.y + 40));
	}

	Button* btnBack = (Button*)this->layerButtons->getChildByTag(TAG_LB_BTN_BACK);
	if (btnBack != NULL){
		btnBack->loadTextures("button_menu_chan.png", "button_menu_chan.png", "");

		btnBack->setAnchorPoint(Vec2(0, 1));
		btnBack->setPosition(Vec2(20 - SizeAdd.width, HEIGHT_DESIGN - 20 + SizeAdd.height));
		btnBack->setScale(0.75);
	}

	/*   Button* btnCoin = (Button*)this->layerButtons->getChildByTag(TAG_LB_BTN_COIN);
	   if(btnCoin != NULL && btnBack != NULL){
	   btnCoin->loadTextures("button_naptien_chan.png", "button_naptien_chan.png");
	   btnCoin->setAnchorPoint(Vec2(0, 1));
	   btnCoin->setPosition(Vec2(120 - SizeAdd.width, HEIGHT_DESIGN - 25 + SizeAdd.height));
	   btnCoin->setVisible(true);
	   btnCoin->setPressedActionEnabled(true);
	   if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
	   btnCoin->setScale(0.6);
	   }
	   }*/

	int haftWidth = WIDTH_DESIGN >> 1;

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		Sprite * time = Sprite::createWithSpriteFrameName("timerme.png");
		this->TimerMe = CCProgressTimer::create(time);
		this->TimerMe->setAnchorPoint(Vec2(0.5, 0));
		this->TimerMe->setType(ProgressTimer::Type::RADIAL);
		this->TimerMe->setPosition(Vec2(haftWidth, -3 - SizeAdd.height));
	}
	else{
		Sprite * time = Sprite::createWithSpriteFrameName("timerme_new.png");
		this->TimerMe = CCProgressTimer::create(time);
		this->TimerMe->setAnchorPoint(Vec2(0.5, 0));
		this->TimerMe->setType(ProgressTimer::Type::RADIAL);
		this->TimerMe->setPosition(Vec2(haftWidth, 12 - SizeAdd.height));
		this->TimerMe->setLocalZOrder(101);
		//this->TimerMe->setScale(1.5);
	}
	this->BtnReady = Button::create();
	this->BtnReady->setTouchEnabled(true);
	this->BtnReady->setPressedActionEnabled(true);
	this->BtnReady->setScale9Enabled(false);
	this->BtnReady->loadTextures("btn-chiabai.png", "", "");
	this->BtnReady->setAnchorPoint(Vec2(0.5, 0.5));
	this->BtnReady->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
	this->BtnReady->setVisible(false);

	this->BtnSitDown = Button::create();
	this->BtnSitDown->setTouchEnabled(true);
	this->BtnSitDown->setPressedActionEnabled(true);
	this->BtnSitDown->setScale9Enabled(false);
	this->BtnSitDown->loadTextures("btn-ngoichoi-chan.png", "btn-ngoichoi-chan-press.png", "");
	this->BtnSitDown->setAnchorPoint(Vec2(1, 0));
	this->BtnSitDown->setPosition(Vec2(WIDTH_DESIGN - 150 + SizeAdd.width, 20 - SizeAdd.height));

	ButtonChanNotification* ntf = ButtonChanNotification::GetInstance();
	ntf->RemoveAllObserver();
	//Create Button Chan, don't Modify
	this->BtnDrawCard = new ButtonBoc(dataManager.GetSysString(85), "noc.png", "noc_press.png", "noc_Disable.png", Vec2(haftWidth, (HEIGHT_DESIGN >> 1) + 8), -1, ntf);
	this->BtnDrawCard->setScale(0.8);

	this->BtnDrawCard->autorelease();
	this->BtnTakeCard = new ButtonEate(dataManager.GetSysString(86), "an.png", "an_press.png", "an_Disable.png", Vec2(haftWidth, 135 - SizeAdd.height), -1, ntf);

	this->BtnTakeCard->autorelease();
	this->BtnBaoU = new ButtonU("", "U.png", "U_press.png", "U_Disable.png", Vec2(haftWidth, 15 - SizeAdd.height), -1, ntf);
	this->BtnBaoU->setScale(1.2);

	this->BtnBaoU->autorelease();
	this->BtnChiuCard = new ButtonChiu(dataManager.GetSysString(68), "chiu.png", "chiu_press.png", "chiu_Disable.png", Vec2(haftWidth - this->BtnBaoU->getContentSize().width / 2 - 75, 25 - SizeAdd.height), 1, ntf);

	this->BtnChiuCard->autorelease();
	this->BtnDisCard = new ButtonTake(dataManager.GetSysString(87), "danh.png", "danh_press.png", "danh_Disable.png", Vec2(haftWidth + this->BtnBaoU->getContentSize().width / 2 + 75, 25 - SizeAdd.height), 2, ntf);

	this->BtnDisCard->autorelease();
	this->BtnDuoiCard = new ButtonDuoi(dataManager.GetSysString(88), "danh.png", "danh_press.png", "danh_Disable.png", Vec2(haftWidth + this->BtnBaoU->getContentSize().width / 2 + 75, 25 - SizeAdd.height), 2, ntf);

	this->BtnDuoiCard->autorelease();

	this->BtnReady->setVisible(false);
	this->BtnSitDown->setVisible(false);

	this->BtnDrawCard->setVisible(false);
	this->BtnTakeCard->setVisible(false);
	this->BtnBaoU->setVisible(false);
	this->BtnChiuCard->setVisible(false);
	this->BtnDisCard->setVisible(false);
	this->BtnDuoiCard->setVisible(false);

	this->BtnReady->setTag(CTAG_BTN_READY);
	this->BtnSitDown->setTag(CTAG_BTN_SIT_DOWN);

	this->BtnDrawCard->setTag(CTAG_BTN_DRAWCARD);
	this->BtnTakeCard->setTag(CTAG_BTN_TAKECARD);
	this->BtnBaoU->setTag(CTAG_BTN_BAO_U);
	this->BtnChiuCard->setTag(CTAG_BTN_CHIUCARD);
	this->BtnDisCard->setTag(CTAG_BTN_DISCARD);
	this->BtnDuoiCard->setTag(CTAG_BTN_DUOICARD);

	this->BtnReady->addTouchEventListener(CC_CALLBACK_2(LayerChanGame::OnBtnReadyClick, this));
	this->BtnSitDown->addTouchEventListener(CC_CALLBACK_2(LayerChanGame::OnBtnSitDownClick, this));
	this->BtnDisCard->addTouchEventListener(CC_CALLBACK_2(LayerChanGame::OnBtnTakeClick, this));
	this->BtnDrawCard->addTouchEventListener(CC_CALLBACK_2(LayerChanGame::OnBtnBocClick, this));
	this->BtnDuoiCard->addTouchEventListener(CC_CALLBACK_2(LayerChanGame::OnBtnDuoiClick, this));
	this->BtnTakeCard->addTouchEventListener(CC_CALLBACK_2(LayerChanGame::OnBtnEateClick, this));
	this->BtnBaoU->addTouchEventListener(CC_CALLBACK_2(LayerChanGame::OnBtnUClick, this));
	this->BtnChiuCard->addTouchEventListener(CC_CALLBACK_2(LayerChanGame::OnBtnChiuClick, this));

	this->layerButtons->addChild(this->BtnSitDown);
	this->layerButtons->addChild(this->BtnReady);

	this->layerButtons->addChild(this->BtnDisCard);
	this->layerButtons->addChild(this->BtnDrawCard);
	this->layerButtons->addChild(this->BtnDuoiCard);
	this->layerButtons->addChild(this->BtnTakeCard);
	this->layerButtons->addChild(this->BtnBaoU);
	this->layerButtons->addChild(this->BtnChiuCard);
	this->layerButtons->addChild(this->TimerMe, 110);
}

void LayerChanGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	pObjs->PutUtfString("_cmd_", *cmd);
	//only add to queues
	this->mThreadQueueMsg->addQueue(pObjs);
}


void LayerChanGame::OnExtensionResponseProcess() {
	this->param = this->mThreadQueueMsg->frontQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");

	if (this->_countPendingChiu >= TIMEOUT_CHIU && this->isChiuPending) {
		this->stopChiuPending();
	}

	//! List player
	if (0 == cmd->compare(EXT_SRVNTF_PLAYER_LIST)) {
		this->event_EXT_SRVNTF_PLAYER_LIST();
	}

	if (0 == cmd->compare(EXT_EVENT_LEAVE_GAME_RES)){
		this->event_EXT_EVENT_LEAVE_GAME_RES();
	}

	if (0 == cmd->compare(EXT_EVENT_GAME_LEAVE_VIP_RES)){
		this->event_EXT_EVENT_GAME_LEAVE_RES();
	}
	//! user ready
	else if (0 == cmd->compare(EXT_SRVNTF_USER_READY)) {
		this->event_EXT_SRVNTF_USER_READY();
	}

	//! Bản tin nhận được nếu người chơi sẵn sàng
	else if (0 == cmd->compare(EXT_EVENT_READY_RES)) {
		this->event_EXT_EVENT_READY_RES();
	}

	//! Bốc cái, nhận được bản tin này thì bắt đầu hiệu ứng chia bài
	else if (0 == cmd->compare(EXT_SRVNTF_BOC_CAI)) {
		this->event_EXT_SRVNTF_BOC_CAI();
	}

	//! Ban tin nhan duoc xem con bai nao co the chiu
	else if (0 == cmd->compare(EXT_SRVNTF_CHIU_CARD_LIST)){
		boost::shared_ptr<string> chcl = this->param->GetUtfString(EXT_FIELD_CHIU_CARDS);
		if (chcl != NULL){
			log("list cards can chiu: %s", chcl->c_str());
			//27:4:0:0;
			this->LayerCardsChan->SetCardCanChiu(*chcl);
		}
	}

	//! Bắt đầu game
	else if (0 == cmd->compare(EXT_EVENT_START)) {
		this->event_EXT_EVENT_START();
	}

	//! Bản tin list card của người chơi
	else if (0 == cmd->compare(EXT_EVENT_LISTCARD_NTF)) {
		boost::shared_ptr<string> usrn = this->param->GetUtfString(EXT_FIELD_USERNAME);
		if (usrn != NULL) {
			int pos = this->GetPosPlayer(*usrn);
			if (this->LayerCardsChan->CheckCardRunning(pos)) {
				return;
			}
		}
		this->event_EXT_EVENT_LISTCARD_NTF();
	}

	//! Nếu nhận được bản tin này có nghĩa là đang chơi, không phải khách
	if (0 == cmd->compare(EXT_EVENT_RES_BOC_CAI)) {
		this->event_EXT_EVENT_RES_BOC_CAI();
	}

	//! Bản tin trả về khi có sự thay đổi về số lượng còn trong nọc
	else if (0 == cmd->compare(EXT_SRVNTF_NOC_COUNT)) {
		this->event_EXT_SRVNTF_NOC_COUNT();
	}

	//! Bản tin trả về chi tiết bộ bài trên tay: Chắn:Cạ:Ba Đầu:Què
	else if (0 == cmd->compare(EXT_SRVNTF_ONHAND_DETAILS)) {
		this->event_EXT_SRVNTF_ONHAND_DETAILS();
	}

	//! Bản tin trả về người chơi hiện tại - current player
	else if (0 == cmd->compare(EXT_SRVNTF_CURRENT_PLAYING_USER)) {
		this->event_EXT_SRVNTF_CURRENT_PLAYING_USER();
	}

	//! Bản tin cho biết nguồn gốc và đích đến của lá bài
	//! Bản tin này xác định những người chơi khác đang chơi.
	//! Các tao tác ăn đánh dưới của mình đã có bản tin riêng, đây là nơi xác định với các người chơi khác
	else if (0 == cmd->compare(EXT_SRVNTF_CARD_ORIGINATION)) {
		boost::shared_ptr<string> f_user = param->GetUtfString(EXT_FIELD_FROM_USERNAME);
		boost::shared_ptr<string> t_user = param->GetUtfString(EXT_FIELD_TO_USERNAME);

		boost::shared_ptr<unsigned char> cardnu = param->GetByte(EXT_FIELD_CARD_NUMBER);
		boost::shared_ptr<unsigned char> cardsu = param->GetByte(EXT_FIELD_CARD_SUIT);
		boost::shared_ptr<unsigned char> crdorg = param->GetByte(EXT_FIELD_CARD_ORIGINATION);
		boost::shared_ptr<unsigned char> crdco = param->GetByte(EXT_FIELD_CARD_COUNT);

		if (f_user != nullptr /*&& 0 != f_user->compare(*t_user)*/) {
			int pos = this->GetPosPlayer(*f_user);
			if (this->LayerCardsChan->checkIsRunningCard(pos, *cardnu, *cardsu))
				return;
		}

		string _f_user = "";
		string _t_user = "";
		int _cardnu = -1;
		int _cardsu = -1;
		int _crdorg = -1;
		int _count = 1;

		if (f_user != NULL)
			_f_user = *f_user;
		if (t_user != NULL)
			_t_user = *t_user;

		if (cardnu != NULL) {
			_cardnu = *cardnu;
			log("cardnu: %d", _cardnu);
		}

		if (cardsu != NULL) {
			_cardsu = *cardsu;
			log("cardsu: %d", _cardsu);
		}

		if (crdorg != NULL) {
			_crdorg = *crdorg;
			log("crdorg: %d", _crdorg);
		}

		if (crdco != NULL) {
			_count = *crdco;
			log("crdorg: %d", _count);
		}

		this->EventTakeCards(_f_user, _t_user, _cardnu, _cardsu, _crdorg, _count);
	}

	//! Bản tin trả về nếu có 1 người chơi ăn báo
	else if (0 == cmd->compare(EXT_SRVNTF_ANBAO)) {
		this->event_EXT_SRVNTF_ANBAO();
	}

	//! Báo có người chíu
	//! Thong tin nguoi chiu (not recomment)
	else if (0 == cmd->compare(EXT_SRVNTF_ONE_EXPECTING_CHIU)) {
		this->event_EXT_SRVNTF_ONE_EXPECTING_CHIU();
	}

	//Ban tin nhan duoc khi dang ky lam nguoi choi.
	else if (0 == cmd->compare(EXT_EVENT_PLAY_REGISTER_RES)){
		log("Dang ky lam nguoi choi");
	}

	//Danh Sach nguoi dang chau ria:
	else if (0 == cmd->compare(EXT_EVENT_REGISTER_QUEUE_NTF)){
		this->event_EXT_EVENT_REGISTER_QUEUE_NTF();
	}

	//! Chíu thành công, Nếu mình chíu mới nhận được
	//! Tra ve rc, neu rc = 0. -> Chiu thanh cong.
	else if (0 == cmd->compare(EXT_EVENT_RES_CHIU_CARD)) {
		this->event_EXT_EVENT_RES_CHIU_CARD();
	}

	//! Bản tin tra ve rc bốc nọc, nhận được khi chính mình bốc
	//! rc = 0 -> Boc noc ok
	else if (0 == cmd->compare(EXT_EVENT_RES_DRAW_CARD)) {
		this->event_EXT_EVENT_RES_DRAW_CARD();
	}

	//! Dưới bài, nhận được khi chính mình Dưới
	//! Tra ve rc, neu rc = 0 -> Duoi thanh cong
	else if (0 == cmd->compare(EXT_EVENT_RES_DUOI_CARD)) {
		this->event_EXT_EVENT_RES_DUOI_CARD();
	}

	//! Ăn bài, nhận được khi mình ăn
	//! Tra ve rc, neu rc = 0 -> Danh thanh cong
	else if (0 == cmd->compare(EXT_EVENT_RES_TAKE_CARD)) {
		this->event_EXT_EVENT_RES_TAKE_CARD();
	}

	//! Đánh bài, nhận được khi đánh thành công
	//! Tra ve rc, neu rc = 0 -> danh thanh cong.
	else if (0 == cmd->compare(EXT_EVENT_RES_DISCARD)) {
		this->event_EXT_EVENT_RES_DISCARD();
	}

	//! Bản tin này nhận về khi mình báo Ù.
	//! Nếu rs = 0 -> Báo Ù thành công
	else if (0 == cmd->compare(EXT_EVENT_RES_U)) {
		this->event_EXT_EVENT_RES_U();
	}

	//! ban tin thông báo có người báo ù thành công
	//! Thong tin nguoi U, hien thi o day
	else if (0 == cmd->compare(EXT_SRVNTF_U_SUCCESS)) {
		this->event_EXT_SRVNTF_U_SUCCESS();
	}

	//! Báo có thể Ù (not recommnent)
	else if (0 == cmd->compare(EXT_SRVNTF_ONE_EXPECTING_U)) {
		this->event_EXT_SRVNTF_ONE_EXPECTING_U();
	}

	//! Bản tin trả về nếu bị ù đè
	//! not recomment
	else if (0 == cmd->compare(EXT_SRVNTF_U_DE)) {
		this->event_EXT_SRVNTF_U_DE();
	}

	//! Bản tin này trả về các lá bài trên tay người Ù
	else if (0 == cmd->compare(EXT_SRVNTF_TRENTAY_DETAIL)) {
		this->event_EXT_SRVNTF_TRENTAY_DETAIL();
	}

	//! Bản tin này trả về các lá bài còn trong nọc
	else if (0 == cmd->compare(EXT_SRVNTF_NOC_DETAIL)) {
		this->event_EXT_SRVNTF_NOC_DETAIL();
	}

	//! Khi nhận được bản tin này thì có thể xướng cước
	//! Chỉ nhận được nếu mình báo Ù trước đó
	else if (0 == cmd->compare(EXT_SRVNTF_BEGIN_ANNOUCING)) {
		this->event_EXT_SRVNTF_BEGIN_ANNOUCING();
	}

	//! Bản tin trả về kết quả game
	else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT)) {
		this->event_EXT_EVENT_GAME_RESULT();
	}

	//! Nhận được bản tin này là kết thúc ván chơi
	else if (0 == cmd->compare(EXT_EVENT_END)) {
		this->event_EXT_EVENT_END();
	}

	//! Khi nhận được bản tin này thì người chơi có thể gửi ready
	else if (0 == cmd->compare(EXT_EVENT_READY_NTF)) {
		this->event_EXT_EVENT_READY_NTF();
	}

	//! Nhận lại thông tin bàn chơi khi vào xem hoặc reconnect
	else if (0 == cmd->compare(EXT_SRVNTF_GAME_INFO)) {
		this->event_EXT_SRVNTF_GAME_INFO();
	}

	//! Nhận được event này khi có sự thay đổi về gà nuôi
	else if (0 == cmd->compare(EXT_SRVNTF_GA_NUOI_SCORE)) {
		this->event_EXT_SRVNTF_GA_NUOI_SCORE();
	}

	//! Bản tin coin change: Số tiền của từng người chơi
	else if (0 == cmd->compare(EXT_SRVNTF_COIN_CHANGE)) {
		this->event_EXT_SRVNTF_COIN_CHANGE();
	}

	//! Bản tin trả về khi chính mình gửi bản tin join game
	//! Tra ve rc
	else if (0 == cmd->compare(EXT_EVENT_RES_JOIN_GAME)) {
		this->event_EXT_EVENT_RES_JOIN_GAME();
	}

	//! Nguoi choi khong ready, bi kick thanh nguoi xem
	else if (0 == cmd->compare(EXT_SRVNTF_PLAYER_KICKOUT)) {
		this->event_EXT_SRVNTF_PLAYER_KICKOUT();
	}

	//! Bản tin nhận về khi update room variable
	else if (0 == cmd->compare(EXT_EVENT_RES_UPDATE_GAME_CONFIG)) {
		this->event_EXT_EVENT_RES_UPDATE_GAME_CONFIG();
	}

	this->mThreadQueueMsg->deQueue();
	//this->param.reset();

}//end extensions

void LayerChanGame::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	if (ptrEventParamValueUser == NULL) return;
	boost::shared_ptr<User> m_Player = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);

	if (m_Player == NULL) return;
	double money = 0;

	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		if (m_Player->GetVariable(EXT_VAL_AMOUNT_BALANCE) != NULL) {
			money = *m_Player->GetVariable(EXT_VAL_AMOUNT_BALANCE)->GetDoubleValue();
		}
	}
	else{
		if (m_Player->GetVariable("amfs") != NULL) {
			money = *m_Player->GetVariable("amfs")->GetDoubleValue();
		}
	}


	std::string uid = *m_Player->Name();

	int pos = this->GetPosPlayer(uid);
	if (pos == -1) return;

	if (pos == kUserMe)
	{
		CCASSERT(this->LayerAvatars->getUserByPos(kUserBot), "User Bottom NULL");
		this->LayerAvatars->getUserByPos(kUserBot)->setMoney(money);
		this->LayerAvatars->getUserByPos(kUserMe)->setMoney(money);
	}
	else
	{
		CCASSERT(this->LayerAvatars->getUserByPos(pos), "Avatar NULL");
		this->LayerAvatars->getUserByPos(pos)->setMoney(money);
	}

	log("user variables update %s : %f", uid.c_str(), money);
}

void LayerChanGame::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> Players = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);
	boost::shared_ptr<string> _UserName = Players->Name();
	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> p_Messages = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);
    if (dataManager.getAppConfig().is24h)
    {
        return;
    }
	if (_UserName != NULL && p_Messages != NULL){
		int pos = this->GetPosPlayer(*_UserName);
		if (pos == kUserMe && !this->showAvatarMe){
			pos = kUserBot;
		}
		this->LayerAvatars->SetEmotionInPos(pos, *p_Messages);
	}
}

void LayerChanGame::OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {

}

void LayerChanGame::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {

}

void LayerChanGame::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	log("OnSmartFoxRoomVariableUpdate");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	if (ptrEventParamValueRoom == NULL) return;
	boost::shared_ptr<Room> room = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	//
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL || room == NULL) return;
	if (lastRoom == room) {
		CauHinhChanRoom sc = this->SetRoomVar(room);
		bool changed = this->checkConfigChanged(sc);
		if (changed) {
			this->Toast->ShowToast(dataManager.GetSysString(66));
		}
	}
	else {
		log("Lastroom != room");
	}
}

void LayerChanGame::OnSmartFoxRoomDestroy(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL) {
		log("OnSmartFoxRoomDestroy Your Room Destroy");
		layerButtons->BackToChooseDesk();
	}
}

//Send Request join game
void LayerChanGame::SendRequestJoinGame(float dt) {
	SendRequestJoinGame();
}

void LayerChanGame::SendRequestJoinGame() {
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL) return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_JOIN_GAME, params, lastRoom));
	GameServer::getSingleton().Send(request);
	log("join game request !!!!");
}

void LayerChanGame::event_EXT_EVENT_START() {
	LayerAvatars->setUnReadyAllUser();
}

//! Nhận được khi có 1 người chơi ready
//! Bản tin thông báo người chơi có thể ready
void LayerChanGame::event_EXT_EVENT_READY_NTF() {
	if (this->getChildByTag(KTAG_LAYER_XUONG_U) != NULL)
		this->removeChildByTag(KTAG_LAYER_XUONG_U);
	log("user can request ready !!!!!!!!");

	//! Ẩn hiện button ready
	if (0 == this->FindBossRoom().compare(this->MyAI)){
		this->SetEndGame();
	}

}

void LayerChanGame::event_EXT_EVENT_END() {

	this->StrResonAnBao = "";
	this->CurrentPlayer = "";
	this->CountBocCai = 0;
	this->CountNocs = 0;
	this->FlagAnBao = false;
	this->HasGiveCard = false;
	this->HasNotifyU = false;
	this->IsGamePlaying = false;
	this->FlagSitDown = true;

	this->LayerCardsChan->SetFlagAnBao(false);

	this->State = PlayingState::BEGIN;

	if (this->getChildByTag(KTAG_LAYER_GIVE) != NULL) {
		this->removeChildByTag(KTAG_LAYER_GIVE);
	}

	this->StopTimer_Me();
	//this->LblDetail->setString("C:0 BD:0 C:0 Q:0");
}

void LayerChanGame::event_EXT_SRVNTF_GA_NUOI_SCORE() {
	boost::shared_ptr<long> diemganuoi = param->GetInt(EXT_FIELD_GA_NUOI_SCORE);
	if (diemganuoi != NULL) {
		LblCoinGa->setString(boost::to_string(*diemganuoi));
		log("Diem ga nuoi: %ld", *diemganuoi);
	}
}

void LayerChanGame::event_EXT_SRVNTF_COIN_CHANGE() {
	//!"uid":0,"coinchr":9,"coinch":-10000
	boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);
	boost::shared_ptr<long> coinchr = param->GetInt(EXT_FIELD_COIN_CHANGE_REASON);
	boost::shared_ptr<long> coinch = param->GetInt(EXT_FIELD_COIN_CHANGE);
	if (usrn != NULL && coinchr != NULL && coinch != NULL) {
		log("coin change: %s - %ld - %d", usrn->c_str(), *coinchr, (int)*coinch);

		int icoin = (int)(*coinch);
		std::string o = this->ConvertMoneyEx(abs(icoin));

		string strToast = ChanUtils::GetNamePlayer(*usrn) + " "+dataManager.GetSysString(67)+" -" + o + " " + SceneManager::getSingleton().getCoinUnit();
		log("coin: %s", strToast.c_str());

		switch (*coinchr) {
		case CCR_BY_GOP_GA:
			this->Toast->ShowToast(strToast, 3);
			break;
		case CCR_BY_NGUOI_KHAC_CHIU:
			strToast = dataManager.GetSysString(68)+", " + strToast;
			this->Toast->ShowToast(strToast, 3);
			break;
		case CCR_BY_NHAI:
			strToast = dataManager.GetSysString(89)+", " + strToast;
			this->Toast->ShowToast(strToast, 3);
			break;
		default:
			log("Coinchange Reson: %ld", *coinchr);
			break;
		}
	}
}

//! Hàm này được gọi khi có 1 người vào phòng.
//! Phòng đó đang chơi.
//! Người đó có thể là khách vào xem, hoặc có thể là người chơi trong phòng mất kết nối quay lại.
void LayerChanGame::event_EXT_SRVNTF_GAME_INFO() {
	log("Event Game Info");
	boost::shared_ptr<string> ginf = param->GetUtfString(EXT_FIELD_GAME_INFO);
    if (ginf == NULL) return;
	log("list cards: %s", ginf->c_str());
	if (ginf != NULL) {

		this->SetEndGame();

		vector<string> room_infos = mUtils::splitString(*ginf, '|');
		vector<string>::iterator player_info;
		for (player_info = room_infos.begin(); player_info != room_infos.end(); ++player_info) {
			vector<string> str = mUtils::splitString(*player_info, '-');
			if (str.size() <= 1) return;
		}

		//! Nếu đã đi được đến đây thì có nghĩa là bàn chơi đang chơi.
		this->IsGamePlaying = true;
		this->BtnReady->setVisible(false);

		//! Xử lý nếu mình là khách vào xem (Chầu rìa)
		if (this->IsSpect) {
			if (this->getChildByTag(KTAG_LAYER_GIVE) != NULL) {
				this->removeChildByTag(KTAG_LAYER_GIVE);
			}

			for (player_info = room_infos.begin(); player_info != room_infos.end(); ++player_info) {

				try {
					vector<string> p = mUtils::splitString(*player_info, '-');

					int pos = this->GetPosPlayer(p.at(0));
					if (-1 == pos) continue;

					this->HasGiveCard = true;
					this->LayerCardsChan->CreateOrRestoreListCards(pos, p.at(1));

					//! Kiểm tra xem người này có bị báo hay không.
					//! Bị báo thì dán chữ "Bị Báo" lên Avatar
					if (0 != p.at(2).compare("0")) {
						long ErrorCode = (long)atol(p.at(2).c_str());
						string note = "";
						this->DisplayAnBaoError(ErrorCode, p.at(0), note);
					}

					//! Kiểm tra xem đây có phải người đang chơi hiện tại hay không
					//! Nếu đang chơi thì start timer.
					if (0 == p.at(3).compare("1")) {
						this->CurrentPlayer = p.at(1);
						if (pos >= 0) {
							this->LayerAvatars->stopAllTimer();
							this->LayerAvatars->getUserByPos(pos)->startTimer(this->Step_Timeout);
						}
					}
				}
				catch (std::out_of_range &e) {
					log("Caught an out_of_range exception: %s", e.what());
				}
				catch (std::exception &e) {
					log("Caught an exception of an unexpected type %s", e.what());
				}
				catch (...) {
					log("Caught an unknown exception");
				}
			}
		}

		//!Nếu mình là người chơi bị mất kết nối và vào lại bàn chơi
		else {
			log("Mat ket noi, vao lai");
			this->HasGiveCard = true;
			this->DisPlayAvatarMe(false);

			for (player_info = room_infos.begin(); player_info != room_infos.end(); ++player_info) {
				try {

					vector<string> p = mUtils::splitString(*player_info, '-');

					int pos = this->GetPosPlayer(p.at(0));
					if (-1 == pos) continue;

					if (pos == kUserMe) {
						//! Xác định xem mình có phải là người đánh bài đầu tiên hay ko. dựa vào count_discards.
						if (mUtils::splitString(mUtils::splitString(p.at(1), '/').at(0), ';').size() < 20) {
							this->State = PlayingState::DISCARD;
						}
					}

					this->LayerCardsChan->CreateOrRestoreListCards(pos, p.at(1));

					//!Tạm fix ở Client khi server chưa fix
					if (pos == kUserMe) {
						if (mUtils::splitString(p.at(1), '/').at(0) == "0") {
							this->IsGamePlaying = false;
							this->HasGiveCard = false;
							this->DisPlayAvatarMe(true);
							this->BtnReady->setVisible(true);
						}
					}

					//check ăn báo
					if (0 != p.at(2).compare("0")) {
						long rscode = (long)atol(p.at(2).c_str());
						string note = "";
						this->DisplayAnBaoError(rscode, p.at(0), note);
					}

					//check current player
					if (0 == p.at(3).compare("1")) {
						log("Nguoi dang choi la: %s", p.at(0).c_str());
						this->CurrentPlayer = p.at(0);
						if (0 == this->CurrentPlayer.compare(this->MyAI)){
							int size = this->LayerCardsChan->GetCountCardsMe();
							if (0 == size % 2)
							{
								// truoc khi thoat da an bai, gio phai danh di.
								if (this->State != PlayingState::BEGIN){
									this->State = PlayingState::ANBAI;
								}
							}
							else
							{
								// Gio co the boc, an hoac duoi.
							}
						}
						else
						{
							this->BtnChiuCard->Enable();
						}
						this->SetCurrentPlayer(1.0);
					}

				}
				catch (std::out_of_range &e) {
					log("Caught an out_of_range exception: %s", e.what());
				}
				catch (std::exception &e) {
					log("Caught an exception of an unexpected type %s", e.what());
				}
				catch (...) {
					log("Caught an unknown exception");
				}
			}
		}//end else
	}
}

void LayerChanGame::event_EXT_EVENT_GAME_RESULT() {
	boost::shared_ptr<string> rg = param->GetUtfString(EXT_FIELD_RESULT_GAME);
	if (rg != NULL) {
		log("resuilt game: %s", rg->c_str());
		this->LayerAvatars->stopAllTimer();
		if (this->getChildByTag(KTAG_LAYER_XUONG_U) != NULL)
		{
			this->removeChildByTag(KTAG_LAYER_XUONG_U);
		}

		try {
			vector<string> arrResults = mUtils::splitString(*rg, ';');
			string StrCuocs = (0 != arrResults.at(4).compare("")) ? arrResults.at(4) : arrResults.at(5);

			arrResults.clear();
			arrResults = mUtils::splitString(StrCuocs, ':');

			std::string trdgsg = StrCuocs;

			StrCuocs = "";
			for (unsigned short i = 0; i < arrResults.size(); ++i) {
				StrCuocs += ChanUtils::GetNameCuocSacById(arrResults.at(i));
				if (ChanUtils::GetNameCuocSacById(arrResults.at(i)) != "")
					StrCuocs += " ";
			}

			StrCuocs = ChanUtils::TestGameCuoc(trdgsg);

			log("here, before ");
			log("ket cuoc: %s", StrCuocs.c_str());

			if (this->getChildByTag(KTAG_LABEL_CUOC) != NULL) {
				this->removeChildByTag(KTAG_LABEL_CUOC);
			}

			Label *lblCuoc = Label::createWithSystemFont(StrCuocs,"", 25);
			lblCuoc->setAnchorPoint(Vec2(0.5, 0.5));
			lblCuoc->setColor(Color3B::RED);
			lblCuoc->setPosition(Vec2(WIDTH_DESIGN >> 1, (HEIGHT_DESIGN >> 1) + 300));
			this->addChild(lblCuoc, z_Order_lblCuoc, KTAG_LABEL_CUOC);

			ScaleTo *scale = ScaleTo::create(0.5, 3.5);
			lblCuoc->runAction(scale);

			CallFuncN* callfun = CallFuncN::create(CC_CALLBACK_1(LayerChanGame::DisplayWinnerResultCuoc, this, *rg));
			DelayTime *delay = DelayTime::create(2.0);
			this->runAction(Sequence::create(delay, callfun, NULL));
		}

		catch (std::out_of_range &e) {
			log("Caught an out_of_range exception: %s", e.what());
		}
		catch (std::exception &e) {
			log("Caught an exception of an unexpected type %s", e.what());
		}
		catch (...) {
			log("Caught an unknown exception");
		}
	}
}

//Bản tin Bốc cái (Nhận đc -> animation chia bài)
void LayerChanGame::event_EXT_SRVNTF_BOC_CAI() {
	this->Toast->RemoveAllToast();
	this->SetEndGame();

	if (!this->IsSpect){
		this->BtnSitDown->setVisible(false);
	}

	boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);
	if (usrn != NULL) {
		log("Nguoi Boc Bai: %s", usrn->c_str());
		CountBocCai++;

		//! remove layer result
		if (this->getChildByTag(KTAG_LAYER_RESUILT) != NULL) {
			this->removeChildByTag(KTAG_LAYER_RESUILT);
		}

		if (1 == CountBocCai) {
			this->BtnReady->setVisible(false);
			if (this->getChildByTag(KTAG_LAYER_GIVE) != NULL) {
				this->removeChildByTag(KTAG_LAYER_GIVE);
			}

			Layer_GivePocker_Chan *LayerGive = Layer_GivePocker_Chan::createInstance();
			// Kiểm tra xem mình có phải là người được bốc bài hay không.
			bool iBoc = (0 == usrn->compare(MyAI));
			LayerGive->SetLayerChanToast(this->Toast);
			LayerGive->SetMapPlayer(this->mapPlayers);
			LayerGive->SetIsPlayerBocCai(iBoc);
			LayerGive->SetPlayerBocCai(*usrn);
			this->addChild(LayerGive, z_Order_layer_give, KTAG_LAYER_GIVE);
			LayerGive->StartGivePocker();
		}
		else if (CountBocCai > 1) {
			bool iBoc = (0 == usrn->compare(MyAI));
			Layer_GivePocker_Chan *LayerGive = (Layer_GivePocker_Chan*)this->getChildByTag(KTAG_LAYER_GIVE);
			if (LayerGive != NULL) {
				LayerGive->SetMapPlayer(this->mapPlayers);
				LayerGive->SetIsPlayerBocCai(iBoc);
				LayerGive->SetPlayerBocCai(*usrn);
			}
		}
	}
}

//Event list cards from server
void LayerChanGame::event_EXT_EVENT_LISTCARD_NTF() {
	boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);
	boost::shared_ptr<string> lc = param->GetUtfString(EXT_FIELD_LIST_CARD);

	this->BtnReady->setVisible(false);

	if (usrn != NULL && lc != NULL) {
		log("event Listcards uid: %s", usrn->c_str());
		log("event Listcards lc: %s", lc->c_str());

		//! Nếu lần đầu tiên nhận được list lá bài thì delay 1s để
		//! hiệu ứng xòe bài.
		log("flagChiaBai = %s", HasGiveCard ? "true" : "false");

		if (!HasGiveCard) {

			if (this->IsSpect) {
				if (this->getChildByTag(KTAG_LAYER_GIVE) != NULL) {
					this->removeChildByTag(KTAG_LAYER_GIVE);
				}
				this->HasGiveCard = true;
			}

			if (0 == usrn->compare(MyAI)) {
				CallFuncN* callfun = CallFuncN::create(CC_CALLBACK_1(LayerChanGame::DelayListCardFirst, this, *lc));
				DelayTime *delay = DelayTime::create(0.9);
				this->runAction(Sequence::create(delay, callfun, NULL));
			}
		}

		//! Từ lần 2 thì chỉ cần lấy list card để resort lại các lá bài trên tay.
		else
		{
			int pos = this->GetPosPlayer(*usrn);
			if (pos == kUserMe) {
				this->LayerCardsChan->RemoveCardResult();
			}
			this->LayerCardsChan->SetPlayerListCards(pos, *lc);
		}
	}
}

void LayerChanGame::CheckAndRemovePlayBefore(){
	this->LayerAvatars->stopAllBlinkAvatar();
	this->LayerCardsChan->RemoveCardOnHandWinner();
	if (this->getChildByTag(KTAG_LAYER_RESUILT) != NULL) {
		this->removeChildByTag(KTAG_LAYER_RESUILT);
	}
	if (this->getChildByTag(KTAG_LAYER_GIVE) != NULL) {
		this->removeChildByTag(KTAG_LAYER_GIVE);
	}
}

void LayerChanGame::DelayListCardFirst(Ref *sender, std::string& data) {

	this->DisPlayAvatarMe(false);

	if (this->getChildByTag(KTAG_LAYER_GIVE) != NULL) {
		this->removeChildByTag(KTAG_LAYER_GIVE);
	}

	// Khi bat dau van choi, chi enable Button U
	this->BtnDrawCard->Disable();
	this->BtnChiuCard->Disable();
	this->BtnDuoiCard->Disable();
	this->BtnDisCard->Disable();
	this->BtnTakeCard->Disable();
	this->BtnBaoU->Enable();

	this->LayerCardsChan->SetMyListCards(data);
	this->HasGiveCard = true;
	this->SetCurrentPlayer(1.0);
}

// Bản tin lá bài được đánh ra.
void LayerChanGame::event_EXT_EVENT_RES_DISCARD() {
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT_CODE);
	if (rscode != NULL) {
		int rc = (int)(*rscode);
		if (rc != GAME_RSCODE_OK) {
			log("Không đánh được !");
			ButtonChanNotification::GetInstance()->ClientDisCard();
		}
		else {

		}
		this->LayerCardsChan->DisSuccess(rc);
	}
}

// Kết quả trả về khi mình báo Ù
void LayerChanGame::event_EXT_EVENT_RES_U() {
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT_CODE);
	if (rscode != NULL) {
		int rc = (int)(*rscode);
		log("rescode Bao u = %d", rc);
		if (rc == GAME_RSCODE_OK) {
			this->HideAllButton();
			this->BtnDrawCard->Disable();
			this->StopTimer_Me();
		}

		else if (rc == GAME_RSCODE_U_BUT_LOW_PRIORITY) {
			//this->Toast->ShowToast("Người báo ù trước với thứ tự cao hơn", 3);
		}
		else if (rc == GAME_RSCODE_COULDNOT_ANNOUCING_U) {
			//! Chua U duoc
			this->Toast->ShowToast(dataManager.GetSysString(69), 3);
		}

		else if (rc == GAME_RSCODE_COULDNOT_U_OVER_U_INTERVAL) {
			this->Toast->ShowToast(dataManager.GetSysString(69), 3);
		}

		else
		{
			log("Bao U khong thanh cong, res code = %ld", rc);
		}
	}
}

// Nhận được khi 1 người chờ Ù
void LayerChanGame::event_EXT_SRVNTF_ONE_EXPECTING_U() {
	boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);
	// return;
	if (usrn != NULL) {
		log("nguoi cho U: %s", usrn->c_str());

		this->StopTimer_Me();
		this->LayerAvatars->stopAllTimer();

		if (0 == usrn->compare(this->MyAI)) {
			if (this->CauHinh.commentU){
				this->Toast->ShowToast(dataManager.GetSysString(70), 3);
			}
			if (0 == this->MyAI.compare(this->CurrentPlayer)) {

			}
			else {

			}
		}

		else
		{
			string strToast = "" + ChanUtils::GetNamePlayer(*usrn) + " "+dataManager.GetSysString(71);
			this->Toast->ShowToast(strToast, 3);

			int pos = this->GetPosPlayer(*usrn);
			if (pos == -1) return;
			//start timer của người chơi chờ Ù
			if (pos == kUserMe)
			{

			}
			else
			{
				this->LayerAvatars->getUserByPos(pos)->startTimer(this->Step_Timeout);
			}
		}
	}
}

// Bản tin list cards trên tay người Ù
void LayerChanGame::event_EXT_SRVNTF_TRENTAY_DETAIL() {
	boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);
	boost::shared_ptr<string> lc = param->GetUtfString(EXT_FIELD_LIST_CARD);

	if (usrn != NULL || lc != NULL) {
		if (0 != usrn->compare(MyAI)) {
			//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sounds/Chan/uroi.mp3");
			std::string strRe = *usrn + "|" + *lc;
			CallFuncN* CallFunc = CallFuncN::create(CC_CALLBACK_1(LayerChanGame::Delay10s, this, strRe));
			DelayTime* Delay = DelayTime::create(2.0f);
			this->runAction(Sequence::create(Delay, CallFunc, NULL));
		}
	}
}

// Bản tin list cards còn trong Nọc
void LayerChanGame::event_EXT_SRVNTF_NOC_DETAIL() {
	boost::shared_ptr<string> nocdetl = param->GetUtfString(EXT_FIELD_NOC_REMAIN_DETAIL);
	if (nocdetl != NULL) {
		StopTimer_Me();
		log("Các lá bài còn trong nọc: %s", nocdetl->c_str());
		this->LayerCardsChan->SetListCardsNoc(*nocdetl);
		this->BtnDrawCard->DisableAndHide();
		this->BtnDisCard->DisableAndHide();
		this->BtnDuoiCard->DisableAndHide();
		this->BtnChiuCard->DisableAndHide();
		this->BtnTakeCard->DisableAndHide();
		this->BtnBaoU->DisableAndHide();
	}
}

int LayerChanGame::GetPosPlayer(const string& uid) {
	return this->mapPlayers.find(uid) != this->mapPlayers.end() ? this->mapPlayers.at(uid) : -1;
}

//Update list users
void LayerChanGame::EventListPlayers(string& list) {

	this->mapPlayers.clear();
	this->IsSpect = true;

	int vt = -1;
	vector<string> arrPlayers;
	if (list.c_str() != NULL && list != "") {
		arrPlayers = mUtils::splitString(list, ';');
	}

	// Set count user
	this->CountPlayers = arrPlayers.size();

	// Find position of mine
	for (unsigned short i = 0; i < arrPlayers.size(); ++i) {
		try {
			string id_ = mUtils::splitString(arrPlayers.at(i), ':').at(1);
			if (0 == id_.compare(this->MyAI)) {
				vt = i;
				this->IsSpect = false;
				break;
			}
		}
		catch (std::out_of_range &e) {
			log("Caught an out_of_range exception: %s", e.what());
		}
		catch (std::exception &e) {
			log("Caught an exception of an unexpected type %s", e.what());
		}
		catch (...) {
			log("Caught an unknown exception");
		}
	}

	this->LayerAvatars->resetAll();
	bool isboss_ = this->MyAI.compare(this->FindBossRoom()) == 0;
	this->LayerAvatars->SetMeIsBoss(isboss_);
	this->LayerAvatars->SetIsSpect(this->IsSpect);
	this->layerButtons->SetIsGuess(this->IsSpect);

	this->mapPlayers.clear();
	if (this->IsSpect)
	{
		vt = 0;
	}

	for (unsigned short i = 0; i < arrPlayers.size(); ++i) {
		if (0 == arrPlayers.at(i).compare("")) continue;
		try {
			vector<string> player = mUtils::splitString(arrPlayers.at(i), ':');
			if (i == vt) {
				if (this->IsSpect)
				{
					this->mapPlayers[player.at(1)] = kUserBot;
				}
				else
				{
					this->mapPlayers[player.at(1)] = kUserMe;
				}
			}
			else if (i == (vt + 1) % 4)
			{
				this->mapPlayers[player.at(1)] = kUserRight;
			}
			else if (i == (vt + 2) % 4)
			{
				this->mapPlayers[player.at(1)] = kUserTop;
			}
			else if (i == (vt + 3) % 4)
			{
				this->mapPlayers[player.at(1)] = kUserLeft;
			}
		}
		catch (std::out_of_range &e) {
			log("Caught an out_of_range exception EventListPlayers: %s", e.what());
		}
		catch (std::exception &e) {
			log("Caught an exception of an unexpected type EventListPlayers %s", e.what());
		}
		catch (...) {
			log("Caught an unknown exception EventListPlayers");
		}
	}

	// Neu chi co 2 nguoi choi.
	if (this->CountPlayers == 2)
	{
		std::map<std::string, int>::iterator pos;
		for (pos = this->mapPlayers.begin(); pos != this->mapPlayers.end(); ++pos)
		{
			if (pos->second != kUserBot && pos->second != kUserMe)
			{
				pos->second = kUserTop;
			}
		}
	}

	this->LayerCardsChan->SetIsSpect(this->mapPlayers);

	//
	this->layerButtons->ResetStatus();
	this->isRegistLeave = false;
	this->LayerAvatars->showIconLeaveGame(false);

	/////////
	// Neu minh la khach
	if (this->IsSpect)
	{
		this->BtnReady->setVisible(false);
		this->BtnSitDown->setVisible(true);

		for (vector<string>::iterator it = arrPlayers.begin(); it != arrPlayers.end(); ++it) {
			PlayerChanInfo pInfo = this->GetPlayerInfo(*it);
			this->SetInfoAvatar(pInfo);
		}
	}

	// Neu minh la nguoi choi
	else
	{
		std::string IdBoss = this->FindBossRoom();
		bool MeIsBoss = 0 == this->MyAI.compare(IdBoss);

		for (vector<string>::iterator it = arrPlayers.begin(); it != arrPlayers.end(); ++it) {
			PlayerChanInfo pInfo = this->GetPlayerInfo(*it);
			if (pInfo.pos == kUserMe) {
				if (0 == this->CountBocCai || this->IsGamePlaying) {
					this->VisibleButtonReady();
				}
				else {
					this->BtnReady->setVisible(false);
				}
				pInfo.pos = kUserBot;
				pInfo.isBoss = MeIsBoss;
			}
			this->SetInfoAvatar(pInfo);
		}
	}

	if (1 == this->CountPlayers && !this->IsSpect) {
		this->BtnReady->setVisible(true);
		this->BtnSitDown->setVisible(false);
	}

	this->LayerAvatars->checkSameIp();
	this->DisPlayAvatarMe(true);
}

PlayerChanInfo LayerChanGame::GetPlayerInfo(std::string args){
	PlayerChanInfo ret;

	boost::shared_ptr< Room > pLastJoinedRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (pLastJoinedRoom == NULL)
		return ret;

	std::vector<std::string> info = mUtils::splitString(args, ':');
	if (0 == info.size())
		return ret;

	try{
		ret.aI = info.at(1);
		ret.pos = this->GetPosPlayer(ret.aI);
		ret.money = atof(info.at(4).c_str());
		ret.isReady = info.at(3) == "1";
		ret.name = info.at(1);

		if (pLastJoinedRoom->GetUserByName(info.at(1)) != NULL) {
			boost::shared_ptr<UserVariable> ptrName = pLastJoinedRoom->GetUserByName(info.at(1))->GetVariable(EXT_VAL_ACCOUNT_NAME);
			if (ptrName != NULL) {
				std::string _name = (*ptrName->GetStringValue());
				boost::algorithm::trim(_name);
				if (_name.compare("") != 0)
					ret.name = _name;
			}
			if (SceneManager::getSingleton().getCurrRoomType() == 0){
				boost::shared_ptr<UserVariable> ptrMoney = pLastJoinedRoom->GetUserByName(info.at(1))->GetVariable(EXT_VAL_AMOUNT_BALANCE);
				if (ptrMoney != NULL) {
					ret.money = *ptrMoney->GetDoubleValue();
				}

			}
			else{
				boost::shared_ptr<UserVariable> ptrMoney = pLastJoinedRoom->GetUserByName(info.at(1))->GetVariable("amfs");
				if (ptrMoney != NULL) {
					ret.money = *ptrMoney->GetDoubleValue();
				}

			}
			boost::shared_ptr<UserVariable> ptrUrl = pLastJoinedRoom->GetUserByName(info.at(1))->GetVariable(EXT_VAL_AMOUNT_AVATAR_LINK);
			if (ptrUrl != NULL) {
				ret.aal = *ptrUrl->GetStringValue();
			}
		}
	}
	catch (std::out_of_range& e){
		log("GetPlayerInfo out of range %s", e.what());
	}
	catch (std::exception& e){
		log("GetPlayerInfo exception %s", e.what());
	}

	return ret;
}

void LayerChanGame::SetInfoAvatar(const PlayerChanInfo& info) {
	if (info.pos == -1) return;
	Avatar* avatar = this->LayerAvatars->getUserByPos(info.pos);
	if (avatar == NULL) return;
	if (info.pos >= 0) {
		avatar->setVisibleLayerInvite(false);
		avatar->setAI(info.aI);
		std::string name_ = ChanUtils::GetNamePlayer(info.name);
		avatar->setName(name_);
		avatar->setMoney(info.money);
		avatar->setIcon(info.aal);
		avatar->setMeIsBoss(info.isBoss);
	}

	if (info.pos == kUserMe || info.pos == kUserBot) {
		if (0 == this->MyAI.compare(info.name)){
			this->LayerAvatars->getUserByPos(kUserMe)->setMoney(info.money);
			this->LayerAvatars->getUserByPos(kUserMe)->setAI(info.aI);
			std::string name_ = ChanUtils::GetNamePlayer(info.name);
			this->LayerAvatars->getUserByPos(kUserMe)->setName(name_);
			this->LayerAvatars->getUserByPos(kUserMe)->setIcon(info.aal);
		}
		avatar->setPositionY(200 - this->SizeAdd.height);
	}

}
//Các sự kiện đánh bài của người chơi
void LayerChanGame::EventTakeCards(const string& f_user, const string& t_user, const int& cardnu, const int& cardsu, const int& crdorg, const int& count) {

	int fpos = this->GetPosPlayer(f_user);
	int tpos = this->GetPosPlayer(t_user);

	log("Event take cards: from user: %s to user: %s", f_user.c_str(), t_user.c_str());

	if (crdorg == CARD_ORIGINATION_BOC_NOC || crdorg == CARD_ORIGINATION_BY_DISCARD){
		this->CheckAndRemovePlayBefore();
	}

	CardOrg card(f_user, t_user, fpos, tpos, cardnu, cardsu, crdorg, count);
	if (this->IsSpect) {
		this->EventTakeCards_Showing(card);
	}
	else {
		this->EventTakeCards_Playing(card);
	}
}

//! các sự kiện đánh bài nếu mình đang xem (là khách)
void LayerChanGame::EventTakeCards_Showing(CardOrg& card) {

	this->HideAllButton();
	this->LayerCardsChan->TakeCards(card);

	if (-1 == card.tpos) return;

	this->LayerAvatars->stopAllTimer();

	switch (card.crdorg)
	{
	case CARD_ORIGINATION_BOC_NOC:
	case CARD_ORIGINATION_AN_CUA_TREN:
	case CARD_ORIGINATION_AN_CUA_TRI:
	{
										this->LayerAvatars->getUserByPos(card.tpos)->startTimer(this->Step_Timeout);
	}
		break;

	case CARD_ORIGINATION_CHIU:
	{
								  this->LayerAvatars->getUserByPos(card.tpos)->startTimer(this->Step_Timeout);
	}
		break;

	case CARD_ORIGINATION_TRA_CUA:
	{
									 int _pos = this->GetPosPlayer(this->CurrentPlayer);
									 if (_pos > -1)
									 {
										 this->LayerAvatars->getUserByPos(_pos)->startTimer(this->Step_Timeout);
									 }
	}
		break;
	}
}

//! các sự kiện đánh bài nếu mình đang chơi (là người chơi)
void LayerChanGame::EventTakeCards_Playing(CardOrg& card) {

	log("From %d to %d", card.fpos, card.tpos);
	this->LayerCardsChan->TakeCards(card);

	if (this->CountNocs == NOCCOUNT_STOP_GAME) return;

	switch (card.crdorg){
		// An cua tren hoac cua chi
	case CARD_ORIGINATION_AN_CUA_TRI:
	case CARD_ORIGINATION_AN_CUA_TREN:
	{
										 if (card.tpos == kUserMe){
											 this->BtnDuoiCard->setVisible(false);
											 this->BtnDisCard->Enable();
											 this->State = PlayingState::DISCARD;
										 }
	}
		break;

		// Tra cua
	case CARD_ORIGINATION_TRA_CUA:
	{
									 if (card.fpos != kUserMe && card.tpos == kUserMe) {
										 this->State = PlayingState::TRACUA;
									 }
									 if (this->FlagAnBao){
										 this->BtnChiuCard->Disable();
									 }
									 else{
										 this->BtnChiuCard->Enable();
									 }
	}
		break;

		// Danh bai
	case CARD_ORIGINATION_BY_DISCARD:
	{
										if (card.fpos == kUserMe) {
											this->BtnChiuCard->Disable();
										}
										else {
											if (this->FlagAnBao){
												this->BtnChiuCard->Disable();
											}
											else{
												this->BtnChiuCard->Enable();
											}
										}
										this->State = PlayingState::DISCARD;
	}
		break;

		// Co nguoi boc noc
	case CARD_ORIGINATION_BOC_NOC:
	{
									 if (this->FlagAnBao){
										 this->BtnChiuCard->Disable();
									 }
									 else{
										 this->BtnChiuCard->Enable();
									 }
									 if (card.tpos == kUserMe) {
										 this->State = PlayingState::DRAWCARD;
										 if (!this->FlagAnBao)
										 {
											 this->BtnDisCard->Disable();
											 this->BtnTakeCard->Enable();
											 this->BtnDuoiCard->Enable();
											 this->BtnChiuCard->Enable();
											 this->BtnBaoU->Enable();
										 }
									 }
	}
		break;

		// Duoi bai
	case CARD_ORIGINATION_BY_DUOI:
	{
									 if (card.tpos == kUserMe) {
										 this->BtnChiuCard->Disable();
									 }
									 this->State = PlayingState::DISCARD;
	}
		break;

		// Neu minh an 1 la bai
	case CARD_ORIGINATION_BY_TRANSFER_TREN_2_DUOI:
	{
													 if (card.tpos == kUserMe){
														 this->BtnChiuCard->Disable();
													 }
	}
		break;

	case CARD_ORIGINATION_CHIU:
		if (card.fpos == kUserMe){
			this->BtnTakeCard->Disable();
			this->BtnDuoiCard->Disable();
			this->BtnTakeCard->Disable();
			this->BtnChiuCard->Disable();
		}
		break;

	default:
		break;
	}

	// Neu tuser = me
	if (0 == MyAI.compare(card.tuser)) {

		//! Hien timer cua me
		if (card.crdorg == CARD_ORIGINATION_BOC_NOC ||
			card.crdorg == CARD_ORIGINATION_AN_CUA_TREN ||
			card.crdorg == CARD_ORIGINATION_AN_CUA_TRI) {
			if (!this->FlagAnBao)
				this->StartTimerMe(this->Step_Timeout);
		}

		//! Neu chiu thi hien timer cua nguoi chiu
		if (card.crdorg == CARD_ORIGINATION_CHIU) {
			if (0 == this->MyAI.compare(this->CurrentPlayer)) {
				this->StartTimerMe(this->Step_Timeout);
			}
			else {
				this->StopTimer_Me();
			}
		}
	}

	// tuser != me
	else {
		if (-1 == card.tpos) return;

		//! hien timer cua tuser
		if (card.crdorg == CARD_ORIGINATION_BOC_NOC ||
			card.crdorg == CARD_ORIGINATION_AN_CUA_TREN ||
			card.crdorg == CARD_ORIGINATION_AN_CUA_TRI) {
			this->LayerAvatars->getUserByPos(card.tpos)->startTimer(this->Step_Timeout);
		}

		// Neu chiu thi hien timer cua nguoi chiu
		if (card.crdorg == CARD_ORIGINATION_CHIU) {
			//! Người đang chơi mà chíu. Đếm lại timer
			if (0 == card.tuser.compare(this->CurrentPlayer)) {
				int _pos = this->GetPosPlayer(this->CurrentPlayer);
				if (_pos >= 0) {
					this->LayerAvatars->getUserByPos(_pos)->startTimer(this->Step_Timeout);
				}
			}
			else {
				// Người đang chơi không được chíu. Người khác chíu -> đợi chíu xong đã.
				this->LayerAvatars->stopAllTimer();
				this->LayerAvatars->getUserByPos(card.tpos)->startTimer(this->Step_Timeout);
			}
		}// end if
	}

	//! Trả cửa xong thì đếm lại timer cho current Player
	if (card.crdorg == CARD_ORIGINATION_TRA_CUA) {
		int _pos = this->GetPosPlayer(this->CurrentPlayer);
		if (_pos >= 0) {
			this->LayerAvatars->getUserByPos(_pos)->startTimer(this->Step_Timeout);
		}
	}
}

//! Tạo các button chức năng
Button* LayerChanGame::CreateButtonWithTitle_Pos(const char *pName, const Vec2& pPoint) {
	Button* button = Button::create();
	button->setTouchEnabled(true);
	button->setScale9Enabled(false);
	button->loadTextures("ready.png", "ready_selected.png", "");
	button->setTitleText(pName);
	button->setTitleColor(Color3B::RED);
	button->setTitleFontSize(20);
	button->setTitleFontSize(button->getContentSize().height / 2);
	button->setAnchorPoint(Vec2(0, 0));
	button->setPosition(pPoint);
	return button;
}

void LayerChanGame::PlaySounds(const string& url) {
	if (!mUtils::isSoundOn()) return;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(("sounds/Chan/" + url).c_str());
}

// set current player
void LayerChanGame::SetCurrentPlayer(float dt) {
	if (!this->HasGiveCard) return;
	this->LayerAvatars->stopAllTimer();

	log("Current user %s", this->CurrentPlayer.c_str());
	this->StopTimer_Me();

	if (0 == this->CurrentPlayer.compare(this->MyAI))
	{
		this->BtnReady->setVisible(false);
		this->BtnSitDown->setVisible(false);
		// Nếu đã bị báo trước đó.
		if (this->FlagAnBao) {
			string strToast = dataManager.GetSysString(72)+" " + this->StrResonAnBao + " "+dataManager.GetSysString(73)+" !";
			this->Toast->ShowToast(strToast, 3);
			this->BtnBaoU->Disable();
			this->BtnDisCard->Disable();
			this->BtnTakeCard->Disable();
			this->BtnDrawCard->Disable();
			this->BtnDuoiCard->Disable();
			this->LayerCardsChan->StartTurn();
			return;
		}

		//! Bắt đầu đếm timer của mình - neu khong bi bao
		//!
		this->StartTimerMe(this->Step_Timeout);
		this->LayerCardsChan->StartTurn();

		switch (this->State){
			// Vua an bai xong
		case PlayingState::ANBAI:
			this->BtnDrawCard->Disable();
			this->BtnDuoiCard->Disable();
			this->BtnChiuCard->Disable();
			this->BtnTakeCard->Disable();
			this->BtnDisCard->Enable();
			break;

			// Vua boc bai xong
		case PlayingState::DRAWCARD:
		{
									   this->BtnDrawCard->Disable();
									   this->BtnDisCard->Disable();
									   this->BtnDisCard->Disable();
									   this->BtnDuoiCard->Enable();
									   this->BtnChiuCard->Enable();
									   this->BtnBaoU->Enable();
		}
			break;

			//Khi van bat dau
		case PlayingState::BEGIN:
		{
									this->BtnDuoiCard->setVisible(false);
									this->BtnDisCard->Disable();
									this->BtnTakeCard->Disable();
									this->BtnDrawCard->Disable();
									this->BtnDisCard->Enable();
									this->BtnBaoU->Enable();
		}
			break;

			//Den luot minh danh (truoc do la cua tren danh, duoi)
		case PlayingState::DISCARD:
		{
									  this->BtnDuoiCard->Disable();
									  this->BtnDisCard->Disable();

									  this->BtnBaoU->Enable();
									  this->BtnChiuCard->Enable();
									  this->BtnTakeCard->Enable();
									  this->BtnDrawCard->Enable();
		}
			break;

			// Minh duoc tra cua
		case PlayingState::TRACUA:
		{
									 this->BtnDrawCard->Disable();
									 this->BtnDisCard->Disable();

									 this->BtnDuoiCard->Enable();
									 this->BtnChiuCard->Enable();
									 this->BtnTakeCard->Enable();
									 this->BtnDuoiCard->Enable();
		}
			break;
		}
	}

	//!Lượt chơi của người khác
	else
	{
		log("luot choi cua nguoi khac: %s", this->CurrentPlayer.c_str());
		this->LayerCardsChan->FinishTurn();

		if (!this->IsSpect)
		{
			if (this->State == PlayingState::BEGIN)
			{

			}
			this->DisPlayOnlyBtnU();
		}

		int pos = this->GetPosPlayer(this->CurrentPlayer);
		if (-1 != pos)
		{
			this->LayerAvatars->getUserByPos(pos)->startTimer(this->Step_Timeout);
		}
	}
}

// Hiển thị lỗi ăn báo.
void LayerChanGame::DisplayAnBaoError(const long& rscode, string& uid, string& note, const bool& enter) {
	log("rc error an bao = %ld", rscode);
	string str = ChanUtils::GetStringAnBao(rscode);

	//    if (0 != note.compare("")) {
	//        if(!enter) {
	//            str += " " + note;
	//        } else {
	//            str += "\n" + note;
	//        }
	//    }

	str += " " + note;

	log("User bi bao: %s", ChanUtils::GetNamePlayer(uid).c_str());
	log("Nguyen nhan: %s", str.c_str());

	//! Nếu người bỏ ù không phải là mình
	if (0 != uid.compare(this->MyAI)) {

		if (rscode == ANBAO_REASON_U_BAO){
			return;
		}
		int pos = this->GetPosPlayer(uid);
		if (pos != -1) {
			this->LayerAvatars->ShowBiBao(pos, true);
		}

		if (0 == str.compare("")) return;
		std::string strToast = "" + ChanUtils::GetNamePlayer(uid) + " "+dataManager.GetSysString(74)+" " + str;

		this->Toast->ShowToast(strToast, 3);
	}

	//Nếu người bao Ù là mình.
	else {
		// Những lỗi sau đây vẫn có thể chơi tiếp. Nếu kết thúc ván mà Ù sẽ bị làng check
		// thì sẽ bị đền làng.
		if (rscode != ANBAO_REASON_AN_CA_DOI_CHO &&
			rscode != ANBAO_TREOTRANH &&
			rscode != ANBAO_REASON_CO_CHAN_CAU_CA &&
			rscode != ANBAO_REASON_AN_CHON_CA &&
			rscode != ANBAO_REASON_CHIUDUOC_NHUNG_LAI_ANTHUONG) {

			// Set An Lao
			this->FlagAnBao = true;
			this->LayerCardsChan->SetFlagAnBao(true);
			this->StrResonAnBao = str;
			this->Toast->ShowToast(str, 3);

			this->LayerAvatars->ShowBiBao(kUserMe, true);

			// bi an bao thi disable tat ca cac button
			this->BtnBaoU->Disable();
			this->BtnChiuCard->Disable();
			this->BtnDuoiCard->Disable();
			this->BtnDisCard->Disable();
			this->BtnTakeCard->Disable();
			this->BtnDrawCard->Disable();
		}
	}
}

//Xuong U
void LayerChanGame::ActionXuongU() {

	this->LayerCardsChan->ScaleCardsHand_WhenU();
	this->LayerCardsChan->MoveCardChi_WhenU();

	//Add Xuong u o day
	LayerGameChan_XuongU *layerXuongU = LayerGameChan_XuongU::create();
	if (layerXuongU != NULL) {
		layerXuongU->setPosition(Vec2(0, 0));
		this->addChild(layerXuongU, z_Order_layer_xuong_u, KTAG_LAYER_XUONG_U);
	}

	this->LayerCardsChan->DisplayButtonViewNoc();
	this->LayerCardsChan->ShowLaBaiU(this->MyAI);
}

void LayerChanGame::SetEndGame() {

	this->LayerAvatars->SetEndGame();
	this->StopTimer_Me();

	this->LayerCardsChan->ResetAllCards();

	this->DisPlayAvatarMe(true);

	this->BtnDrawCard->Reset();
	this->BtnDuoiCard->Reset();
	this->BtnTakeCard->Reset();
	this->BtnDisCard->Reset();
	this->BtnChiuCard->Reset();
	this->BtnBaoU->Reset();

	this->State = PlayingState::BEGIN;
	this->HasGiveCard = false;
	this->isRegistLeave = false;

	this->isChiuPending = false;

	this->VisibleButtonReady();

	if (this->getChildByTag(KTAG_LABEL_CUOC) != NULL)
		this->removeChildByTag(KTAG_LABEL_CUOC);
}

void LayerChanGame::DisplayWinnerResultCuoc(Ref *sender, std::string& data) {
	this->DisplayLayerKetQua(data);
}

void LayerChanGame::Delay10s(Ref *sender, std::string& data) {
	this->LayerAvatars->stopAllTimer();
	vector<string> arrRe = mUtils::splitString(data, '|');

	try{
		std::string uid = arrRe.at(0);
		int pos = this->GetPosPlayer(uid);
		if (pos > 0)
		{
			if (pos == kUserBot || pos == kUserMe)
			{
				this->LayerAvatars->getUserByPos(kUserBot)->setBlinkAvatar();
			}
			else
			{
				this->LayerAvatars->getUserByPos(pos)->setBlinkAvatar();
			}
			if (!this->IsSpect)
			{
				this->LayerCardsChan->ScaleCardsHand_WhenU();
			}
		}
		std::string ListcardsResult = arrRe.at(1);
		this->LayerCardsChan->DisplayListCardsWinner(uid, ListcardsResult);
	}
	catch (std::out_of_range& e)
	{
		log("%s", e.what());
	}

	this->BtnChiuCard->setVisible(false);
	this->BtnBaoU->setVisible(false);
	this->BtnDisCard->setVisible(false);
	this->BtnDuoiCard->setVisible(false);
	this->BtnTakeCard->setVisible(false);
	this->BtnDrawCard->setVisible(false);
}

//btn Ready
void LayerChanGame::OnBtnReadyClick(Ref *sender, ui::Widget::TouchEventType  type) {
	if (type == ui::Widget::TouchEventType::ENDED) {

		//        long *a = new long;
		//        boost::shared_ptr<long> a_ptr(a);
		//        *a_ptr = 20000;

		//        string s = mUtils::convertMoneyEx (*a_ptr);

		//        cout << a_ptr << " val: " << *a_ptr << " s = " << s << "\n";

		//        return;

		//        LogFile::Write("test write file");

		//        return;


		//        LayerGameChan_XuongU *layerXuongU = LayerGameChan_XuongU::create();
		//        if (layerXuongU != NULL) {
		//            layerXuongU->setPosition(Vec2(0, 0));
		//            this->addChild(layerXuongU, z_Order_layer_xuong_u, KTAG_LAYER_XUONG_U);
		//        }
		//        return;

		//        string result = "82;1;net124;;2:16:22:8:5:7:20;;35;xuandung4:6:-15/hung_hien:6:-15/net124:0:65/hathu7272:6:-15;;10:2:3;20;;0";

		//        LayerChatInGame* chat = this->layerButtons->getLayerChat();
		//        LayerGameChan_KetQua *kq = LayerGameChan_KetQua::createInstance(result, chat);
		//        if(kq != NULL) {
		//            kq->SetCallBack_Close(this, callfuncO_selector(LayerChanGame::CallBackFunc_ResetGame));
		//            this->addChild(kq, z_Order_layer_ketqua, KTAG_LAYER_RESUILT);
		//        }

		//        return;

		//        this->startChiuPending();

		if (0 == this->CountBocCai) {
			this->SetEndGame();
		}

		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL) {
			log("last room == NULL");
			return;
		}
		log("Send ready");
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
		GameServer::getSingleton().Send(request);
	}
}

void LayerChanGame::call2Test(Ref *pSender, ui::Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		Button* b = (Button*)pSender;
		if (b != NULL) {
			log("Get Tag Ok and Tag = %d", b->getTag());
		}
		else {
			log("Canot parse");
		}
	}
}

void LayerChanGame::callbackTest(Ref* sender, std::string name) {
	log("name is: %s", name.c_str());
	Sprite* s = (Sprite*)sender;
	if (s != NULL) {
		log("Tag is %d", s->getTag());
	}
}

//btn Take Cards
void LayerChanGame::OnBtnTakeClick(Ref *sender, ui::Widget::TouchEventType  type) {
	if (type == ui::Widget::TouchEventType::ENDED)
	if (!this->FlagAnBao){
		this->LayerCardsChan->ActionDisCards();
	}
}

//btn Bốc bài
void LayerChanGame::OnBtnBocClick(Ref *sender, ui::Widget::TouchEventType  type) {
	if (type == ui::Widget::TouchEventType::ENDED) {

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lstRooms = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lstRooms == NULL) return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_DRAW_CARD, params, lstRooms));
		GameServer::getSingleton().Send(request);

		this->BtnDrawCard->Disable();
		this->BtnTakeCard->Disable();
		this->BtnDuoiCard->Disable();
		this->BtnDisCard->Disable();
		this->BtnChiuCard->Disable();
	}
}

//Btn Dưới bài
void LayerChanGame::OnBtnDuoiClick(Ref *sender, ui::Widget::TouchEventType  type) {
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->LayerCardsChan->ActionDuoiCard();
	}
}

//btn Ăn bài
void LayerChanGame::OnBtnEateClick(Ref *sender, ui::Widget::TouchEventType  type) {
	if (type == ui::Widget::TouchEventType::ENDED)
		this->LayerCardsChan->ActionTakeCard();
}

void LayerChanGame::OnBtnUClick(Ref *sender, ui::Widget::TouchEventType  type) {
	if (type == ui::Widget::TouchEventType::ENDED) {
		bool ret = this->LayerCardsChan->checkU();
		if (!ret)
		{
			log("client check ! ko u duoc");
			this->Toast->ShowToast(dataManager.GetSysString(69));
		}
		else
		{
			boost::shared_ptr<Room> lstRooms = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			if (lstRooms == NULL) return;
			boost::shared_ptr<IRequest> resObj(new ExtensionRequest(EXT_EVENT_REQ_U, params, lstRooms));
			GameServer::getSingleton().Send(resObj);
		}
	}
}

void LayerChanGame::OnBtnChiuClick(Ref *sender, ui::Widget::TouchEventType  type) {
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (!this->isChiuPending){
			int res = this->LayerCardsChan->ActionChiuCard();
			if (res == CLIENT_RC_OK) {
				this->startChiuPending();
			}
		}
	}
}

void LayerChanGame::HideAllButton() {
	this->BtnReady->setVisible(false);
	this->BtnDrawCard->setVisible(false);
	this->BtnTakeCard->setVisible(false);
	this->BtnDuoiCard->setVisible(false);
	this->BtnDisCard->setVisible(false);
	this->BtnBaoU->setVisible(false);
	this->BtnChiuCard->setVisible(false);
}

void LayerChanGame::CallBackFunc_LatBai(Ref *pSend) {
	if (0 == this->CurrentPlayer.compare(this->MyAI)) {
		this->BtnDrawCard->setVisible(false);
		this->BtnTakeCard->setVisible(true);
		this->BtnDuoiCard->setVisible(true);
	}
}

void LayerChanGame::StartTimerMe(const float& duration) {
	this->StopTimer_Me();
	ProgressTo* ProgressTimer = ProgressTo::create(duration, 100);
	CallFunc* callfun = CallFunc::create(CC_CALLBACK_0(LayerChanGame::StopTimer_Me, this));
	Sequence* seq = Sequence::create(ProgressTimer, callfun, NULL);
	this->TimerMe->runAction(seq);
}

void LayerChanGame::StopTimer_Me() {
	if (this->TimerMe == NULL)
		return;

	if (this->TimerMe->getNumberOfRunningActions() > 0)
		this->TimerMe->stopAllActions();

	this->TimerMe->setPercentage(0);
}

//Khi chia xong bài thì call back tới hàm này để hiện thị các Button chơi.
void LayerChanGame::CallBackFunc_ListCards(Ref *pSend) {
	this->BtnDrawCard->setVisible(true);
	this->BtnChiuCard->setVisible(true);
	this->BtnDisCard->setVisible(true);
	this->BtnBaoU->setVisible(true);
	this->BtnTakeCard->setVisible(true);
}

void LayerChanGame::CallBackFunc_ResetGame(Ref *pSend) {
	//this->SetEndGame();
}

void LayerChanGame::CallBackAfterDisCard(Ref *pSend)
{
	this->StopTimer_Me();
}

// Nếu 1 người vào xem, khi đang chia bài.
// Khi chia bài xong, callback lại để xóa layer chia bài.
void LayerChanGame::CallBackFunc_BocBaiIfGuess(Ref* pSend) {
	if (this->IsSpect && this->getChildByTag(KTAG_LAYER_GIVE) != NULL)
		this->removeChildByTag(KTAG_LAYER_GIVE);
}

string LayerChanGame::NguonGocLaBai(const int& orig) {
	switch (orig) {
	case CARD_ORIGINATION_CHIA_BAI:
		return dataManager.GetSysString(90);
	case CARD_ORIGINATION_BOC_NOC:
		return dataManager.GetSysString(91);
	case CARD_ORIGINATION_AN_CUA_TREN:
		return dataManager.GetSysString(92);
	case CARD_ORIGINATION_AN_CUA_TRI:
		return dataManager.GetSysString(93);
	case CARD_ORIGINATION_CHIU:
		return dataManager.GetSysString(94);
	case CARD_ORIGINATION_TRA_CUA:
		return dataManager.GetSysString(95);
	case CARD_ORIGINATION_BY_DISCARD:
		return dataManager.GetSysString(96);
		// discard
	case CARD_ORIGINATION_BY_DUOI:
		return dataManager.GetSysString(97);
		// đánh đi cây bài vừa bốc nọc ở cửa trì
	case CARD_ORIGINATION_BY_AN_DUOI:
		return dataManager.GetSysString(98);
	case CARD_ORIGINATION_BY_TRANSFER_TREN_2_DUOI:
		return dataManager.GetSysString(99);
	}
	return "";
}

void LayerChanGame::VisibleButtonReady(const bool& isDisplay /*= true*/) {
	if (0 == this->FindBossRoom().compare(this->MyAI)){
		this->BtnReady->setVisible(true);
		if (this->getChildByTag(KTAG_LAYER_RESUILT) != NULL) {
			this->removeChildByTag(KTAG_LAYER_RESUILT);
		}
	}
}

void LayerChanGame::OnBtnSetting_Chan(Ref *pObject, ui::Widget::TouchEventType  pType) {
	if (pType == ui::Widget::TouchEventType::ENDED) {

		log("reference bg : %d", this->_bg->getReferenceCount());
		string bossId = this->FindBossRoom();
		bool IsBoss = false;
		if (0 == MyAI.compare(bossId))
			IsBoss = true;
		LayerConfigRoomChan* LayerConfig = LayerConfigRoomChan::create(IsBoss);
		LayerConfig->setParentBG(this->_bg);
		this->addChild(LayerConfig, z_Order_layer_setting, KTAG_LAYER_CONFIG);
	}
}

void LayerChanGame::OnBtnSitDownClick(Ref* pObject, ui::Widget::TouchEventType  pType) {
	if (pType == ui::Widget::TouchEventType::ENDED) {
		this->FlagSitDown = false;
		this->SendRequestJoinGame();
	}
}

void LayerChanGame::OnBtnHuongDanChan(Ref* pObject, ui::Widget::TouchEventType  pType){
	if (pType == ui::Widget::TouchEventType::ENDED) {
		LayerGuideInGame* layer = LayerGuideInGame::create();
		this->addChild(layer, z_Order_layer_huongdan);
	}
}

std::string LayerChanGame::FindBossRoom() {
	std::string ret = "";
	std::vector<std::string> arrPlayers = mUtils::splitString(this->ListPlayers, ';');
	try{
		std::vector<std::string> info = mUtils::splitString(arrPlayers.at(0), ':');
		ret = info.at(1);
	}
	catch (std::out_of_range& e){
		log("exception %s", e.what());
	}
	catch (std::exception& e){
		log("exception %s", e.what());
	}
	return ret;
}

void LayerChanGame::event_EXT_EVENT_RES_DUOI_CARD() {
	boost::shared_ptr<long> rescode = param->GetInt(EXT_FIELD_RESULT_CODE);
	if (rescode != NULL) {
		int rc = (int)(*rescode);
		log("rscode duoi cards: %d", rc);
		if (rc == GAME_RSCODE_OK){
			this->State = PlayingState::DISCARD;
		}
	}
}

void LayerChanGame::event_EXT_EVENT_RES_TAKE_CARD() {
	log("Ăn bài : event_EXT_EVENT_RES_TAKE_CARD");
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT_CODE);
	if (rscode != NULL) {
		int rc = (int)(*rscode);
		log("rscode take cards: %d", rc);
		if (rc == GAME_RSCODE_OK) {
			ButtonChanNotification::GetInstance()->ClientDisCard();
		}

		else if (rc == GAME_RSCODE_GAME_IN_CHIU){
			log("Co nguoi chiu");
		}

		else {
			log("Ăn không đúng Chắn, Cạ");
			this->Toast->ShowToast(dataManager.GetSysString(45), 3);
		}
		this->LayerCardsChan->TakeSuccess(rc);
	}
}

void LayerChanGame::event_EXT_EVENT_RES_DRAW_CARD() {
	boost::shared_ptr<long> rescode = param->GetInt(EXT_FIELD_RESULT_CODE);
	if (rescode != NULL) {
		int rc = (int)(*rescode);
		log("rscode draw cards: %d", rc);
		if (rc == GAME_RSCODE_OK) {
			this->State = PlayingState::DRAWCARD;
			// Noc con 1 con thi khong boc nua, chi U.
			if (CountNocs == NOCCOUNT_STOP_GAME) {
				this->DisPlayOnlyBtnU();
			}
			else {
				// Neu da an bao thi khong hien gi len nua.
				if (!this->FlagAnBao){
					// Khi boc noc len, thi hien thi an, duoi, chiu, u. hide button discards, draw
					this->BtnDisCard->setVisible(false);

					this->BtnBaoU->Enable();
					this->BtnChiuCard->Enable();
					this->BtnTakeCard->Enable();
					this->BtnDuoiCard->Enable();
				}
			}

			this->BtnDrawCard->Disable();
		}
		else
		{
			if (0 == this->CurrentPlayer.compare(this->MyAI) && !this->FlagAnBao){
				this->BtnDuoiCard->Disable();
				this->BtnDisCard->Disable();
				this->BtnChiuCard->Enable();
				this->BtnDrawCard->Enable();
				this->BtnTakeCard->Enable();
			}
		}
	}
}

void LayerChanGame::event_EXT_SRVNTF_CURRENT_PLAYING_USER() {
	boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);
	if (usrn != NULL) {
		this->CurrentPlayer = *usrn;
		this->SetCurrentPlayer(1.0);
	}
}

void LayerChanGame::event_EXT_SRVNTF_ONHAND_DETAILS() {
	boost::shared_ptr<string> ohdetl = param->GetUtfString(EXT_FIELD_ONHANDCARD_DETAIL);
	boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);
	if (ohdetl != NULL && usrn != NULL) {
		if (0 == usrn->compare(this->MyAI)) {
			vector<string> detail = mUtils::splitString(*ohdetl, ':');
		}
	}
}

void LayerChanGame::event_EXT_SRVNTF_NOC_COUNT() {
	boost::shared_ptr<long> noccount = param->GetInt(EXT_FIELD_NOC_REMAIN_COUNT);
	if (noccount != NULL) {
		log("noc count: %ld", *noccount);
		this->CountNocs = (int)*noccount;
		this->LayerCardsChan->SetCountNoc(this->CountNocs);

		if (this->CountNocs == NOCCOUNT_STOP_GAME) {
			if (!this->IsSpect)
			{
				this->BtnDrawCard->Disable();
				this->BtnChiuCard->Disable();
				this->BtnDuoiCard->Disable();
				this->BtnDisCard->Disable();
				this->BtnTakeCard->Disable();
			}

			this->Toast->ShowToast(dataManager.GetSysString(76), 3);
			this->LayerAvatars->stopAllTimer();
			this->StopTimer_Me();
		}
	}
}

void LayerChanGame::event_EXT_EVENT_RES_BOC_CAI() {
	this->LayerAvatars->setUnReadyAllUser();
}

void LayerChanGame::event_EXT_SRVNTF_USER_READY() {
	boost::shared_ptr<string> user = param->GetUtfString(EXT_FIELD_USERNAME);
	if (user == NULL) return;

	if (0 == user->compare(this->MyAI)) {
		if (this->getChildByTag(KTAG_LAYER_RESUILT) != NULL)
		{
			this->removeChildByTag(KTAG_LAYER_RESUILT);
		}
	}

	int pos = this->GetPosPlayer(*user);
	if (-1 != pos)
	{
		switch (pos) {
		case kUserMe:
		case kUserBot:
			this->BtnReady->setVisible(false);
			//            this->LayerAvatars->getUserByPos(kUserBot)->setReady(true);
			break;
		case kUserLeft:
			//            this->LayerAvatars->getUserByPos(kUserLeft)->setReady(true);
			break;
		case kUserRight:
			//            this->LayerAvatars->getUserByPos(kUserRight)->setReady(true);
			break;
		case kUserTop:
			//            this->LayerAvatars->getUserByPos(kUserTop)->setReady(true);
			break;
		default:
			break;
		}
	}
}

void LayerChanGame::event_EXT_EVENT_LEAVE_GAME_RES(){
	boost::shared_ptr<long> rslt = this->param->GetInt(EXT_FIELD_RESULT_CODE);
	if (rslt != NULL) {
		log("event_EXT_EVENT_LEAVE_GAME_RES %ld", *rslt);
		if (0 == *rslt) {
			this->isRegistLeave = !this->isRegistLeave;
			if (!this->IsSpect)
				this->LayerAvatars->showIconLeaveGame(this->isRegistLeave);
		}
	}
}

void LayerChanGame::event_EXT_EVENT_GAME_LEAVE_RES(){
	//    boost::shared_ptr<long> rslt = this->param->GetInt(EXT_FIELD_RESULT);
	//    if (rslt != NULL) {
	//       // log("event_EXT_EVENT_LEAVE_GAME_RES %ld", *rslt);
	//        if (0 == *rslt) {
	//            this->isRegistLeave = !this->isRegistLeave;
	//            this->LayerAvatars->showIconLeaveGame(this->isRegistLeave);
	//        }
	//    }
}

void LayerChanGame::event_EXT_SRVNTF_PLAYER_LIST() {
	boost::shared_ptr<string> lu = param->GetUtfString(EXT_FIELD_LIST_USER);
	boost::shared_ptr<string> rp = this->param->GetUtfString(EXT_FIELD_REG_PLAY);
	if (lu != NULL) {
		log("list player: %s", lu->c_str());
		this->SetEndGame();

		this->ListPlayers = *lu;

		int _size = mUtils::splitString(this->ListPlayers, ';').size();
		if (_size >= 4){
			this->layerButtons->processDisplayButtonInvite(false);
		}

		this->HideAllButton();
		this->EventListPlayers(this->ListPlayers);
	}

	if (rp != NULL){
		log("list player chau ria: %s", rp->c_str());
	}
}

void LayerChanGame::event_EXT_SRVNTF_ANBAO() {
	boost::shared_ptr<long> anbaores = param->GetInt(EXT_FIELD_ANBAO_REASON);
	boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);
	std::string resString = "";
	bool b_Enter = true;

	if (anbaores != NULL && usrn != NULL) {
		switch (*anbaores) {
		case ANBAO_REASON_AN_CA_DOI_CHO: {
											 boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
											 boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);
											 if (takecard && hacard) {
												 std::string c1 = ChanUtils::GetNameCard(*takecard);
												 std::string c2 = ChanUtils::GetNameCard(*hacard);
												// log("Cap ca an ca doi cho: %s, %s", c1.c_str(), c2.c_str());
												 resString += "(" + c1 + ", " + c2 + ")";
												 b_Enter = false;
											 }
		}
			break;

		case ANBAO_REASON_ANCA_DANHCA: {
										   // Cặp cạ vừa ăn
										   boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
										   boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);

										   // Lấy cặp cạ đã đánh đi
										   boost::shared_ptr<string> discard1 = param->GetUtfString(EXT_FIELD_DIS_DUOI_CARD_1);
										   boost::shared_ptr<string> discard2 = param->GetUtfString(EXT_FIELD_DIS_DUOI_CARD_2);

										   if (takecard && hacard && discard1 && discard2) {
											   std::string c1 = ChanUtils::GetNameCard(*takecard);
											   std::string c2 = ChanUtils::GetNameCard(*hacard);

											   std::string d1 = ChanUtils::GetNameCard(*discard1);
											   std::string d2 = ChanUtils::GetNameCard(*discard2);

											  // log("Cap ca vua an");
											  // log("%s", c1.c_str());
											  // log("%s", c2.c_str());

											  // log("Cap ca da danh di");
											  // log("%s", d1.c_str());
											  // log("%s", d2.c_str());

											   resString += "("+dataManager.GetSysString(87)+" " + d1 + ", " + d2;
											   resString += " "+dataManager.GetSysString(86)+" " + c1 + ", " + c2 + ")";
										   }
		}
			break;

		case ANBAO_TREOTRANH: {
								  boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
								  boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);
								  if (takecard && hacard) {
									  std::string c1 = ChanUtils::GetNameCard(*takecard);
									  std::string c2 = ChanUtils::GetNameCard(*hacard);
									 // log("Treo tranh");
									 // log("Cap ca ha xuong duoi tay: %s, %s", c1.c_str(), c2.c_str());
									  resString += dataManager.GetSysString(100)+" " + c1 + ", " + c2;
									  b_Enter = false;
								  }
		}
			break;

		case ANBAO_REASON_BOCHAN_ANCA: {
										   boost::shared_ptr<string> igcard = param->GetUtfString(EXT_FIELD_IGNORE_CARD);
										   boost::shared_ptr<long> sres = param->GetInt(EXT_FIELD_ANBAO_SUBREASON);

										   boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
										   boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);


										   if (takecard && hacard && igcard && sres) {
											   string c1 = ChanUtils::GetNameCard(*takecard);
											   string c2 = ChanUtils::GetNameCard(*hacard);
											  // log("Bo chan an ca");
											  // log("Cap ca vua an: %s, %s", c1.c_str(), c2.c_str());

											   string ig = ChanUtils::GetNameCard(*igcard);
											  // log("La bai bo an chan %s", ig.c_str());
											  // log("%s", GetValueSubReson(*sres).c_str());

											   resString += "("+dataManager.GetSysString(101)+" " + ig;
											   resString += " "+dataManager.GetSysString(86)+" " + c1 + ", " + c2 + ")";
											   b_Enter = false;
										   }
		}
			break;

		case ANBAO_REASON_BOCHAN_DANHCHAN: {
											   boost::shared_ptr<string> discard1 = param->GetUtfString(EXT_FIELD_DIS_DUOI_CARD_1);
											   if (discard1)
											   {
												   std::string d1 = ChanUtils::GetNameCard(*discard1);
												  // log("Bo chan danh chan con: %s", d1.c_str());
												   resString += "(" + d1 + ")";
											   }
											   boost::shared_ptr<long> sres = param->GetInt(EXT_FIELD_ANBAO_SUBREASON);
											   if (sres)
											   {
												  // log("%s", this->GetValueSubReson(*sres).c_str());
											   }
		}
			break;

		case ANBAO_REASON_BOCHAN_ANCHAN: {
											 boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
											 boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);

											 if (takecard && hacard)
											 {
												 string c1 = ChanUtils::GetNameCard(*takecard);
												 string c2 = ChanUtils::GetNameCard(*hacard);
												// log("Bo chan an an chan con %s", c1.c_str());
												 resString += "(" + c1 + ")";
											 }
											 boost::shared_ptr<long> sres = param->GetInt(EXT_FIELD_ANBAO_SUBREASON);
											 if (sres)
											 {
												// log("%s", GetValueSubReson(*sres).c_str());
											 }

		}
			break;

		case ANBAO_REASON_BOCA_ANCA: {
										 boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
										 boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);
										 boost::shared_ptr<string> igcard = param->GetUtfString(EXT_FIELD_IGNORE_CARD);

										 if (takecard && hacard && igcard)
										 {
											 string c1 = ChanUtils::GetNameCard(*takecard);
											 string c2 = ChanUtils::GetNameCard(*hacard);
											 string icard = ChanUtils::GetNameCard(*igcard);
											// log("Bo an ca con %s", icard.c_str());
											// log("Cap ca vua an: %s, %s", c1.c_str(), c2.c_str());
											 resString += "("+dataManager.GetSysString(101)+" " + icard;
											 resString += " "+dataManager.GetSysString(86)+" " + c1 + ", " + c2 + ")";
										 }
										 boost::shared_ptr<long> sres = param->GetInt(EXT_FIELD_ANBAO_SUBREASON);
										 if (sres)
										 {
											// log("%s", GetValueSubReson(*sres).c_str());
										 }
		}
			break;

		case ANBAO_REASON_DANHCA_ANCA: {
										   // Cặp cạ vừa ăn
										   boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
										   boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);

										   // Lấy cặp cạ đã đánh đi
										   boost::shared_ptr<string> discard1 = param->GetUtfString(EXT_FIELD_DIS_DUOI_CARD_1);
										   boost::shared_ptr<string> discard2 = param->GetUtfString(EXT_FIELD_DIS_DUOI_CARD_2);
										  // log("Danh ca an ca");
										   if (takecard && hacard && discard1 && discard2) {
											   string c1 = ChanUtils::GetNameCard(*takecard);
											   string c2 = ChanUtils::GetNameCard(*hacard);

											   string d1 = ChanUtils::GetNameCard(*discard1);
											   string d2 = ChanUtils::GetNameCard(*discard2);

											  // log("Cap ca vua an");
											  // log("%s", c1.c_str());
											  // log("%s", c2.c_str());

											  // log("Cap ca da danh di");
											  // log("%s", d1.c_str());
											  // log("%s", d2.c_str());

											   resString += "("+dataManager.GetSysString(87)+" " + d1 + ", " + d2;
											   resString += " "+dataManager.GetSysString(86)+" " + c1 + ", " + c2 + ")";
										   }
		}
			break;

		case ANBAO_REASON_XECA_ANCA: {
										 boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
										 boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);
										 boost::shared_ptr<string> discard1 = param->GetUtfString(EXT_FIELD_DIS_DUOI_CARD_1);

										// log("Xe ca an ca");
										 if (takecard && hacard && discard1) {
											 string c1 = ChanUtils::GetNameCard(*takecard);
											 string c2 = ChanUtils::GetNameCard(*hacard);
											 string d1 = ChanUtils::GetNameCard(*discard1);
											// log("Con bai xe ca danh di %s", d1.c_str());
											// log("Cap ca vua an: %s, %s", c1.c_str(), c2.c_str());
											 resString += "("+dataManager.GetSysString(87)+" " + d1;
											 resString += " "+dataManager.GetSysString(86)+" " + c1 + ", " + c2 + ")";
										 }
		}
			break;

		case ANBAO_REASON_XECHAN_ANCA: {
										   boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
										   boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);
										   boost::shared_ptr<string> discard1 = param->GetUtfString(EXT_FIELD_DIS_DUOI_CARD_1);

										  // log("Xe chan an ca");
										   if (takecard && hacard && discard1) {
											   string c1 = ChanUtils::GetNameCard(*takecard);
											   string c2 = ChanUtils::GetNameCard(*hacard);
											   string d1 = ChanUtils::GetNameCard(*discard1);
											  // log("Con bai xe chan %s", d1.c_str());
											  // log("Cap ca vua an: %s, %s", c1.c_str(), c2.c_str());
											   resString += "("+dataManager.GetSysString(87)+"" + d1;
											   resString += " "+dataManager.GetSysString(86)+" " + c1 + ", " + c2 + ")";
										   }
		}
			break;

		case ANBAO_REASON_DANH_ROILAI_AN: {
											  boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
											 // log("Xe danh roi lai an");
											  if (takecard) {
												  string c1 = ChanUtils::GetNameCard(*takecard);
												 // log("Con bai da danh di %s", c1.c_str());
												  resString += "(" + c1 + ")";
											  }
		}
			break;

		case ANBAO_REASON_DANH_DI_DOI_CHAN: {
												boost::shared_ptr<string> discard1 = param->GetUtfString(EXT_FIELD_DIS_DUOI_CARD_1);
												log("Danh di doi chan");
												if (discard1) {
													string d1 = ChanUtils::GetNameCard(*discard1);
													log("Doi chan danh di %s", d1.c_str());
													resString += "(" + d1 + ")";
												}
		}
			break;

		case ANBAO_REASON_AN_ROILAI_DANH: {
											  boost::shared_ptr<string> discard1 = param->GetUtfString(EXT_FIELD_DIS_DUOI_CARD_1);
											  if (discard1) {
												  string d1 = ChanUtils::GetNameCard(*discard1);
												 // log("An roi lai danh ra con: %s", d1.c_str());
												  resString += "(" + d1 + ")";
											  }
		}
			break;

		case ANBAO_REASON_ANCA_ROILAI_DANH_QUAN_CUNG_HANG: {
															   boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
															   boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);
															   boost::shared_ptr<string> discard1 = param->GetUtfString(EXT_FIELD_DIS_DUOI_CARD_1);
															   if (takecard && hacard && discard1) {
																   string c1 = ChanUtils::GetNameCard(*takecard);
																   string c2 = ChanUtils::GetNameCard(*hacard);
																   string d1 = ChanUtils::GetNameCard(*discard1);

																  // log("Cap ca vua an: ");
																  // log("%s", c1.c_str());
																  // log("%s", c2.c_str());

																  // log("Bai cung hang vua danh: ");
																  // log("%s", d1.c_str());

																   resString += "("+dataManager.GetSysString(86)+" " + c1 + ", " + c2;
																   resString += " "+dataManager.GetSysString(87)+" " + d1 + ")";
															   }
		}
			break;

		case ANBAO_REASON_CHIUDUOC_NHUNG_LAI_ANTHUONG: {
														   boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
														   if (takecard) {
															   string c1 = ChanUtils::GetNameCard(*takecard);
															  // log("Bai chiu duoc nhung lai an thuong %s", c1.c_str());
															   resString += "(" + c1 + ")";
														   }
		}
			break;

		case ANBAO_REASON_AN_CHON_CA: {
										  boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
										  boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);

										 // log("An chon ca");
										  if (takecard && hacard) {
											  string c1 = ChanUtils::GetNameCard(*takecard);
											  string c2 = ChanUtils::GetNameCard(*hacard);
											 // log("Cap ca vua an: %s, %s", c1.c_str(), c2.c_str());
											  resString += "(" + c1 + ", " + c2 + ")";
										  }
		}
			break;

		case ANBAO_REASON_CO_CHAN_CAU_CA: {
											  boost::shared_ptr<string> takecard = param->GetUtfString(EXT_FIELD_TAKEN_CARD);
											  boost::shared_ptr<string> hacard = param->GetUtfString(EXT_FIELD_HA_CARD);

											 // log("Co chan cau ca");
											  if (takecard && hacard) {
												  string c1 = ChanUtils::GetNameCard(*takecard);
												  string c2 = ChanUtils::GetNameCard(*hacard);
												 // log("Cap ca vua an: %s, %s", c1.c_str(), c2.c_str());
												  resString += dataManager.GetSysString(102)+" " + c1 + ", " + c2;
											  }
		}
			break;

		case ANBAO_REASON_BO_U_ROI_LAI_U: {
											  boost::shared_ptr<string> takecard = this->param->GetUtfString(EXT_FIELD_TAKEN_CARD);
											 // log("Bo U");
											  if (takecard != NULL) {
												  string c1 = ChanUtils::GetNameCard(*takecard);
												 // log("Bo U con: %s", c1.c_str());
												  resString += dataManager.GetSysString(103)+" " + c1;
												  b_Enter = false;
											  }
		}
			break;

		case ANBAO_REASON_ANCA_ROILAI_AN_QUAN_CUNG_HANG:
		{
														   boost::shared_ptr<string> takecard2 = this->param->GetUtfString(EXT_FIELD_TAKEN_CARD2);
														   if (takecard2 != NULL) {
															   string c1 = ChanUtils::GetNameCard(*takecard2);
															   resString += "(" + c1 + ")";
														   }
		}
			break;

		}

		this->DisplayAnBaoError(*anbaores, *usrn, resString, b_Enter);
	}
}

void LayerChanGame::event_EXT_SRVNTF_ONE_EXPECTING_CHIU() {
	boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);
	if (usrn != NULL) {
		if (0 == usrn->compare(this->MyAI))
		{
			if (this->CauHinh.commentChiu){
				this->Toast->ShowToast(dataManager.GetSysString(77), 3);
			}
		}
		else {
			if (0 == this->MyAI.compare(this->CurrentPlayer)) {
				//string toast = ChanUtils::GetNamePlayer(*usrn) + " chíu lá bài này";
				//this->Toast->ShowToast(toast, 3);
			}
		}
	}
}

void LayerChanGame::event_EXT_EVENT_REGISTER_QUEUE_NTF(){
	boost::shared_ptr<string> lu = this->param->GetUtfString(EXT_FIELD_LIST_USER);
	if (lu != NULL){
		log("Danh sach nguoi choi chau ria: %s", lu->c_str());
		this->LayerAvatars->SetListSpectToEmpty(*lu, this->MyAI);
	}
}

void LayerChanGame::event_EXT_SRVNTF_BEGIN_ANNOUCING() {
	DelayTime* delay = DelayTime::create(2.0);
	CallFunc* call = CallFunc::create(CC_CALLBACK_0(LayerChanGame::ActionXuongU, this));
	this->runAction(Sequence::create(delay, call, NULL));
}

void LayerChanGame::event_EXT_SRVNTF_PLAYER_KICKOUT() {
	boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);
	boost::shared_ptr<long> koutr = param->GetInt(EXT_FIELD_KICKOUT_REASON);

	if (usrn && koutr) {
		std::string StrReson = ChanUtils::GetNamePlayer(*usrn) + " bi kickout do: ";
		switch (*koutr) {
		case KICK_OUT_BY_UNDER_MINIMUM_BALANCE:
			if (0 == usrn->compare(MyAI))
				layerButtons->BackToChooseDesk();
			StrReson += "Khong du tien choi";
			break;
		case KICK_OUT_BY_READY_TIMEOUT:
			if (0 == usrn->compare(MyAI)) {
				this->HideAllButton();
				this->BtnSitDown->setVisible(true);
			}
			StrReson += "Khong San Sang";
			break;
		case KICK_OUT_DISCONECT_BY_CLIENT:
			if (0 == usrn->compare(MyAI))
				layerButtons->BackToChooseDesk();
			StrReson += "Mat ket noi";
			break;
		case KICK_OUT_BY_MASTER:
			if (0 == usrn->compare(MyAI))
				layerButtons->BackToChooseDesk();
			StrReson += "Chu phong kick";
			break;
		}
		log("%s", StrReson.c_str());
	}
}

void LayerChanGame::event_EXT_SRVNTF_U_DE() {
	log("Ban da bi u de !");
	boost::shared_ptr<string> udun = param->GetUtfString(EXT_FIELD_UDE_USERNAME);
	if (udun != NULL) {
		log("Nguoi U de: %s", udun->c_str());
		//string StrToast = "Bạn bị " + ChanUtils::GetNamePlayer(*udun) + " Ù đè ván này !";
		//this->Toast->ShowToast(StrToast, 3);
	}
}

void LayerChanGame::event_EXT_SRVNTF_U_SUCCESS() {
	log("Co nguoi bao u thanh cong !");
	boost::shared_ptr<string> usrn = this->param->GetUtfString(EXT_FIELD_USERNAME);
	boost::shared_ptr<string> takecard = this->param->GetUtfString(EXT_FIELD_TAKEN_CARD);

	if (usrn != NULL && takecard != NULL) {

		std::string cardU = ChanUtils::GetNameCard(*takecard);
		//        this->LayerCardsChan->SetCardU(*takecard);
		this->LayerCardsChan->SetInfoCardU(*usrn, *takecard);

		log("Nguoi u la: %s", usrn->c_str());
		log("La bai U: %s", cardU.c_str());

		std::string s = "";
		if (0 == usrn->compare(MyAI)) {
			this->HasNotifyU = true;
			s.append(dataManager.GetSysString(78)+ " ");
			s.append(cardU);
			s.append(", "+dataManager.GetSysString(79)+" !");
			this->Toast->ShowToast(s, 3);
		}
		else {
			this->PlaySounds("uroi.mp3");
			s = ChanUtils::GetNamePlayer(*usrn);
			s.append(" "+dataManager.GetSysString(80)+" ");
			s.append(cardU);
			s.append(", "+dataManager.GetSysString(79)+" !");
			this->Toast->ShowToast(s, 3);
			this->LayerAvatars->stopAllTimer();
			this->DisPlayOnlyBtnU();
		}
	}
}

void LayerChanGame::event_EXT_EVENT_RES_CHIU_CARD() {
	log("Chiu thanh cong ! current user %s", CurrentPlayer.c_str());
	//Nếu mình là người chíu, mình sẽ nhận được bản tin này.
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT_CODE);
	if (rscode != NULL) {
		int rc = (int)(*rscode);
		log("rscode = %d", rc);
		if (rc == GAME_RSCODE_OK) {
			this->BtnDuoiCard->setVisible(false);
			this->BtnDrawCard->Disable();
			this->BtnTakeCard->Disable();
			this->BtnDisCard->Enable();
		}
		this->LayerCardsChan->ChiuSuccess(rc);
	}
	this->isChiuPending = false;
}

void LayerChanGame::event_EXT_EVENT_RES_JOIN_GAME() {
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT_CODE);
	if (rscode != NULL) {
		int rc = (int)(*rscode);
		log("rscode join game: %d", rc);
		if (rc == GAME_RSCODE_OK) {
			if (!FlagSitDown)
				this->Toast->ShowToast(dataManager.GetSysString(81), 3);
			this->BtnSitDown->setVisible(false);
			Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerChanGame::SendRequestJoinGame),this);
		}
		else if (rc == GAME_RSCODE_HAVE_JOINED_BEFRORE) {
			this->BtnSitDown->setVisible(false);
			Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerChanGame::SendRequestJoinGame),this);
		}
		else if (rc == GAME_RSCODE_UNDER_MINIMUM_BALANCE){
			this->Toast->ShowToast(dataManager.GetSysString(82), 3);
			this->BtnSitDown->setVisible(false);
			Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerChanGame::SendRequestJoinGame),this);
        }
		else {
			log("Join game failed");
			this->BtnSitDown->setVisible(true);
			this->BtnReady->setVisible(false);
			Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerChanGame::SendRequestJoinGame),this);
		}
	}
}

void LayerChanGame::event_EXT_EVENT_READY_RES() {
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT_CODE);
	if (rscode) {
		int rc = (int)(*rscode);
		log("EXT_EVENT_READY_RES = %d", rc);
		if (rc == GAME_RSCODE_INVALIDE_STATE)
			this->Toast->ShowToast(dataManager.GetSysString(83), 3);
	}
}

void LayerChanGame::event_EXT_EVENT_RES_UPDATE_GAME_CONFIG() {
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT_CODE);
	if (rscode){
		int rc = (int)(*rscode);
		log("rscode = %d", rc);
	}

	boost::shared_ptr<string> gcf = param->GetUtfString(EXT_FIELD_GAME_CFG);
	if (gcf)
		log("Game Config update = %s", gcf->c_str());
}

std::string LayerChanGame::GetValueSubReson(const long& sres) {
	switch (sres) {
	case SUBREASON_NO_PROBLEM:
		return "";
	case SUBREASON_BO_CARD_CUA_TRI:
		return "Bo khong an tai cua tri";
	case SUBREASON_BO_CARD_CUA_TREN:
		return "Bo khong an cua tren";
	case SUBREASON_BO_CARD_DA_AN_LAM_CA:
		return "Bo card da an lam ca";
	}
	return "";
}

string LayerChanGame::ConvertMoneyEx(int money){
	char sMoneyTmp[30], sMoney[30], s[30];

	sprintf(sMoneyTmp, "%d", money);
	int len = strlen(sMoneyTmp);

	int i = len - 1, counterSeg = 0, counter = 0;

	while (i >= 0){
		sMoney[counter++] = sMoneyTmp[i];
		counterSeg++;
		if (counterSeg == 3){
			counterSeg = 0;
			if (i != 0)
				sMoney[counter++] = ',';
		}
		i--;
	}

	for (i = counter - 1; i >= 0; i--){
		s[counter - i - 1] = sMoney[i];
	}
	s[counter] = 0;

	return string(s);
}

void LayerChanGame::updateEvent(float dt) {
	if (mThreadQueueMsg->isQueueEmpty())
		return;
	// 	if(LayerCardsChan->isRunningAction())
	// 		return;

	if (!this->isChiuPending && this->getChildByTag(KTAG_LAYER_CONNECTING) != NULL) {
		this->removeChildByTag(KTAG_LAYER_CONNECTING);
	}

	mThreadQueueMsg->run();
}

void LayerChanGame::startChiuPending(){
	if (this->isChiuPending)
		return;
	this->isChiuPending = true;
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerChanGame::executeChiuPending),this,1, false);
}

void LayerChanGame::executeChiuPending(float dt){
	log("Time out chiu: %d", this->_countPendingChiu);
	if (!this->isChiuPending) {
		this->stopChiuPending();
	}
	if (this->_countPendingChiu > TIMEOUT_CHIU) {
		if (this->getChildByTag(KTAG_LAYER_CONNECTING) == NULL) {
			LayerConnecting* connect = LayerConnecting::create();
			this->addChild(connect, z_Order_layer_connecting, KTAG_LAYER_CONNECTING);
		}
	}
	this->_countPendingChiu++;
}

void LayerChanGame::stopChiuPending(){
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerChanGame::executeChiuPending),this);
	this->_countPendingChiu = 0;
	this->isChiuPending = false;

	if (this->getChildByTag(KTAG_LAYER_CONNECTING) != NULL) {
		this->removeChildByTag(KTAG_LAYER_CONNECTING);
	}
}
