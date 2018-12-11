//
//  BaCayChuong.cpp
//  iCasinov2
//
//  Created by DauA on 5/23/14.
//
//

#include "Requests/ExtensionRequest.h"
#include "Requests/SpectatorToPlayerRequest.h"
#include "SceneManager.h"
#include "LayerGameLieng.h"
#include "_Chat_.h"
#include "mUtils.h"
#include "AllData.h"
#include "data_manager.h"
#include "ClientMsgDefs.h"
#include "ChanUtils.h"
#include "PhomMessDef.h"
#include "layers/LayerPopupChonTienLieng.h"
vector<string> LayerGameLieng::split(string &S, const char &str) {
	vector<string> arrStr;
	string::iterator t, t2;

	for (t = S.begin(); t < S.end();){
		// Lặp từ vị trí bắt đầu
		t2 = find(t, S.end(), str);

		//kể từ vị trí t
		if (t != t2){
			arrStr.push_back(string(t, t2));
		}
		else if (t2 != S.end()) arrStr.push_back("");

		if (t2 == S.end())
		{
			break;
		}


		t = t2 + 1;
	}

	return arrStr;
}
LayerGameLieng::LayerGameLieng()
:LayerAvatars(NULL),
LayerButton(NULL),
LayerCards(NULL),
layerTimer(NULL),
ListCards(""),
ListUsers(""),
MyName(""),
StringResult(""),
SizeListUser(0),
FlagChiaBai(false),
FlagDatCuoc(false),
IsTurnUpAll(false),
IsSpector(false),
isStartedGame(false),
//4Lieng
CurrentPlayerBet(""),
CurrentBetal(""),
MinBetValue(0),
Current_My_Bet(0),
mReal(false),

mThreadQueueMsg(NULL) {
}

LayerGameLieng::~LayerGameLieng() {
	if (mThreadQueueMsg != NULL) {
		delete mThreadQueueMsg;
	}
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameLieng::updateEvent),this);
	GameServer::getSingleton().removeListeners(this);
}

bool LayerGameLieng::init() {
	if (!Layer::init())
		return false;

	mCurrGameID = 100;
	if (GameServer::getSingleton().getSmartFox()->MySelf() == NULL) return false;
	this->MyName = *GameServer::getSingleton().getSmartFox()->MySelf()->Name();
	// this->IsSpector = GameServer::getSingleton().getSmartFox()->MySelf()->IsSpectator();
	//	log("%s", IsSpector ? "Khách" : "Người chơi");

	this->CreateBackgrounds();
	this->CreateTimer();
	this->CreateCards();
	this->CreateAvatars();
	this->CreateButtons4Lieng();

	sizeAd = mUtils::getSizePos();

	this->mThreadQueueMsg = new ThreadQueueMsg();
	this->mThreadQueueMsg->setFunCallBack(this, callfunc_selector(LayerGameLieng::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameLieng::updateEvent),this,1/60, false);
	this->resetGame();
	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom != NULL)
	{
		log("gui ban tin chau ria");
		boost::shared_ptr<IRequest> request(new ExtensionRequest("prr", parameter, lastRoom));//gui ban tin chau ria
		GameServer::getSingleton().Send(request);

	}
	GameServer::getSingleton().addListeners(this);
	//HoangDD
	//SceneManager::getSingleton().hideLoading();

	//	this->LayerButton->getButtonByTag(TAG_LB_BTN_CHAT)->setTouchEnabled(!IsSpector);
	return true;
}

void LayerGameLieng::CreateBackgrounds() {

	layerBackGround = BackgroundInGame::create();
	this->addChild(layerBackGround);

	boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (room != NULL && room->GetVariable("params") != NULL) {
		std::string paramString = *room->GetVariable("params")->GetStringValue();
		mCurrGameID = atoi(room->GroupId()->substr(0, 3).c_str());
		std::vector<std::string> arrInfo = mUtils::splitString(paramString, '@');
		if (arrInfo.size() > 0) {
			std::string money = arrInfo.at(0);
			this->mBetDefault = atoi(money.c_str());

			std::string name = dataManager.GetSysString(169);
			if (mCurrGameID == kGameBaCay)
				name = dataManager.GetSysString(168);
			else if (mCurrGameID == kGameLieng)
				name = dataManager.GetSysString(177);
			std::string moneyConvert = mUtils::convertMoneyEx(atoi(money.c_str()));

			double money_bet = atol(money.c_str());
			int roomID = 101;
			boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom != NULL){
				roomID = lastRoom->Id();
				//HoangDD Change
				string sRoomIdDisplay = mUtils::getRoomIDByName(*lastRoom->Name());
				if (sRoomIdDisplay != "")
					roomID = atoi(sRoomIdDisplay.c_str());
			}
			layerBackGround->setNameTable(name, roomID, money_bet);
		}
	}
}

void LayerGameLieng::CreateCards() {
	this->LayerCards = LayerCardLieng::create();
	this->LayerCards->SetMyName(this->MyName);
	this->LayerCards->SetCallbackFunc(this, callfuncN_selector(LayerGameLieng::CallFuncEndGive));
	this->addChild(this->LayerCards);
}

void LayerGameLieng::CreateAvatars() {
	this->LayerAvatars = LayerBaCayAvatar::create();
	this->LayerAvatars->setTag(1309);
	this->LayerAvatars->ResetAll();
	this->addChild(this->LayerAvatars);

	// 	LayerDatCuocBaCay* betLayerSlider = LayerDatCuocBaCay::create();
	// 	betLayerSlider->setLocalZOrder(5000);
	// 	betLayerSlider->setTag(1048);
	// 	betLayerSlider->setInfoTest(1000, 100000);
	// 	betLayerSlider->setCallBack(CC_CALLBACK_0(LayerGameLieng::callBackLayerDatCuoc, this));
	// 	this->addChild(betLayerSlider);
}
std::string LayerGameLieng::find_ChuPhong(string listUser)
{
	if (listUser == "") return "";
	vector<string> arrUser = split(listUser, ';');
	vector<string> info = split(arrUser[0], '|');
	if (info.size() <= 1) return "";
	string boosId = info[0];
	return boosId;
}
void LayerGameLieng::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	if (ptrEvetnParams == NULL) return;
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	boost::shared_ptr<Room> room = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	//
	boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
	if (room != NULL){

		// 		boost::shared_ptr<RoomVariable> chicken_value = room->GetVariable("chicken");
		// 		if (chicken_value != NULL)
		// 		{
		// 			if (*chicken_value->GetBoolValue() == true)
		// 			{
		// 				isNuoiGa = true;
		// 				layerGa->setNuoiGa(true);
		// 				layerGa->setConfigChicken(LayerGa::CONFIG::ON);
		// 			}
		// 			else{
		// 				isNuoiGa = false;
		// 				layerGa->setNuoiGa(false);
		// 				layerGa->setConfigChicken(LayerGa::CONFIG::OFF);
		// 			}
		// 		}

		boost::shared_ptr<RoomVariable> endTime = room->GetVariable("endtime");
		if (endTime != NULL){
			if (endTime->GetDoubleValue() != NULL){
				this->layerTimer->setCount((*endTime->GetDoubleValue()) / 1000);
			}
		}

	}
}

void LayerGameLieng::CreateButtons4Lieng()
{
	this->LayerButton = LayerButtonInGame::create();
	this->addChild(this->LayerButton);

    if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL){
        return;
    }
    
	boost::shared_ptr<RoomVariable> isVipRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("isviproom");
	if (isVipRoom != NULL){
	this->vipRoom = *isVipRoom->GetIntValue();
		if (this->vipRoom == 0){
			this->LayerButton->processDisplayButtonInvite(false);
		}
		else{
			this->LayerButton->processDisplayButtonInvite(true);
		}
	}

	int w_Button = 184;
	int h_Button = 111;
	int _space = 5;
	//Size convert
	Size _size = ChanUtils::getSizePos();

	Button* btn_Ready = CreateButtonWithTitle_Pos(dataManager.GetSysString(29).c_str(), Vec2(WIDTH_DESIGN - w_Button * 2 - _space * 2, _space * 2));

	Button* btn_Squeezing = CreateButtonWithTitle_Pos(dataManager.GetSysString(28).c_str(), Vec2(WIDTH_DESIGN - w_Button* 2 - _space * 2, _space * 2));

	Button* btn_Fold = CreateButtonWithTitle_Pos(dataManager.GetSysString(30).c_str(), Vec2(WIDTH_DESIGN - w_Button * 4 - _space * 4, _space * 2));

	Button* btn_Bet = CreateButtonWithTitle_Pos(dataManager.GetSysString(31).c_str(), Vec2(WIDTH_DESIGN - w_Button* 2 - _space * 2, _space * 2));

	Button* btn_Follow = CreateButtonWithTitle_Pos(dataManager.GetSysString(32).c_str(), Vec2(WIDTH_DESIGN - w_Button * 3 - _space * 3, _space * 2));

	btn_Ready->setTag(DTAG_BTN_READY);
	btn_Squeezing->setTag(DTAG_BTN_SQUEEZ);
	btn_Fold->setTag(DTAG_BTN_FOLD);
	btn_Bet->setTag(DTAG_BTN_BET);
	btn_Follow->setTag(DTAG_BTN_FOLLOW);

	btn_Ready->setVisible(false);
	btn_Squeezing->setVisible(false);
	btn_Fold->setVisible(false);
	btn_Bet->setVisible(false);
	btn_Follow->setVisible(false);

	this->LayerButton->addChild(btn_Ready);
	this->LayerButton->addChild(btn_Squeezing);
	this->LayerButton->addChild(btn_Fold);
	this->LayerButton->addChild(btn_Follow);
	this->LayerButton->addChild(btn_Bet);
}

void LayerGameLieng::CreateTimer() {
	this->layerTimer = LayerTimer::create();
	this->addChild(layerTimer);
}

Button* LayerGameLieng::CreateButtonWithTitle_Pos(const char *pName, const Vec2& pPoint) {
	Color3B colorButton = Color3B(255, 227, 160);
	Size size_add = ChanUtils::getSizePos();
	Button* m_Button = Button::create();
	m_Button->setTouchEnabled(true);
	m_Button->setScale9Enabled(false);
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
		m_Button->loadTextures("ingame-button.png", "ingame-button-active.png", "");
	else if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
		m_Button->loadTextures("ingame-button.png", "ingame-button-active.png", "ingame-button-active.png");
		colorButton = Color3B::WHITE;
	}
	m_Button->setTitleText(pName);
	m_Button->setTitleFontSize(FONT_SIZE_SMALL);
	//m_Button->setTitleFontName(kNormalFont);
	m_Button->setAnchorPoint(Vec2(0, 0));
	m_Button->setTitleColor(colorButton);
	m_Button->setPosition(Vec2(pPoint.x, pPoint.y + size_add.height / 2));
	m_Button->addTouchEventListener(CC_CALLBACK_2(LayerGameLieng::OnButtonClick, this));
	return m_Button;
}

Button* LayerGameLieng::GetButtonByTag(const int& pTag) {
	Button* button = (Button*) this->LayerButton->getChildByTag(pTag);
	return button;
}

void LayerGameLieng::OnButtonClick(Ref *sender, ui::Widget::TouchEventType type) {
	Button* btn = (Button*)sender;
	if (btn && type == ui::Widget::TouchEventType::ENDED) {
		int Tag_ = btn->getTag();
		switch (Tag_) {
		case DTAG_BTN_READY:
			this->ActionReady();
			break;
		case DTAG_BTN_UNREADY:
			this->UnReady();
			break;
		case DTAG_BTN_BET:
			this->onButtonBet();
			break;
		case DTAG_BTN_TURNALL:
			this->TurnUpAll();
			break;
		case DTAG_BTN_VIEW:
			this->ViewCards();
			break;
		case DTAG_BTN_SQUEEZ:
			this->SqueezCards();
			break;
		case DTAG_BTN_JOIN:
			this->JoinGame();
			break;
			//Only 4 Lieng
		case DTAG_BTN_FOLD:
			this->FoldCards();
			break;

		case DTAG_BTN_FOLLOW:
			this->Follow();
			break;
		}
	}
}

void LayerGameLieng::JoinGame() {
	if (this->SizeListUser == 7)
		return;

	boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (room == NULL) {
		LayerButton->BackToChooseDesk();
		return;
	}

	boost::shared_ptr<IRequest> request(new SpectatorToPlayerRequest());
	GameServer::getSingleton().Send(request);
	boost::shared_ptr<ISFSObject> params(new SFSObject());

	boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_REQ_JOIN_GAME, params, room));
	GameServer::getSingleton().Send(req2);
}

void LayerGameLieng::ActionReady() {
	//for test
	// 	this->LayerAvatars->setBetInfo(100000, kuser0);
	// 	return;

	this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
	this->LayerAvatars->resetAllBets();

	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerGameLieng::UnReady() {
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_UNREADY_REQ, params, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerGameLieng::onButtonBet() {
	if (!this->FlagDatCuoc) {
		//this->GetButtonByTag(DTAG_BTN_BET)->setVisible(false);
		auto _visible = Director::getInstance()->getVisibleSize();
		this->betLayerSlider = LayerDatCuocBaCay::create();
		this->betLayerSlider->setLocalZOrder(5000);
		this->betLayerSlider->setTag(1048);
		this->betLayerSlider->setInfoBet(this->MinBetValue);
		this->addChild(this->betLayerSlider);
		//this->betLayerSlider->setCallBack(CC_CALLBACK_0(LayerGameLieng::callBackLayerDatCuoc, this));
		Button *btnTo = this->betLayerSlider->getButtonConfirm();
		btnTo->addTouchEventListener(CC_CALLBACK_2(LayerGameLieng::processButtonToTouched, this));

		/*LayerDatCuocBaCay* betLayerSlider = LayerDatCuocBaCay::create();
		betLayerSlider->setLocalZOrder(5000);
		betLayerSlider->setTag(1048);
		betLayerSlider->setInfoBet(this->MinBetValue);
		betLayerSlider->setCallBack(CC_CALLBACK_0(LayerGameLieng::callBackLayerDatCuoc, this));
		this->addChild(betLayerSlider);*/
	}
	else {
		Chat *toast = Chat::create(dataManager.GetSysString(408), -1);
		this->addChild(toast);
	}
}

void LayerGameLieng::TurnUpAll() {
	this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
	this->LayerAvatars->GetUserByPos(0)->stopTimer();
	if (this->ListCards.compare("") != 0){
		this->LayerCards->TurnUpAllCards(this->ListCards, kuser0);
	}

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TURNUP_CARD, params, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerGameLieng::ViewCards() {
	if (this->getChildByTag(TAG_NAN_BACAY_LAYER) != NULL)
		this->removeChildByTag(TAG_NAN_BACAY_LAYER);

	if (0 != this->ListCards.compare("")) {
		this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
	}

	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom != NULL)
	{
		boost::shared_ptr<IRequest> request(new ExtensionRequest("atntf", parameter, lastRoom));//gui ban tin chau ria
		GameServer::getSingleton().Send(request);

	}

}

void LayerGameLieng::SqueezCards() {
}

void LayerGameLieng::CallFuncTurnUpAll(Node *pSend) {
	if (0 != this->ListCards.compare("")) {
		this->LayerCards->TurnUpAllCards(this->ListCards, kuser0);
		this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
	}
}

// Sau Khi chia bài xong, vào đây để hiển thị các Button và count down timer
void LayerGameLieng::CallFuncEndGive(Node *pSend) {
	this->FlagChiaBai = true;
	if (this->LayerAvatars->IsSpect()) {
		this->SpecMode();
	}
	else {
		if (mCurrGameID != kGameLieng){
			//this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(true);
			//this->LayerAvatars->RunAllTimer();
			//this->//mLayerCount->StartCountDownTurnUp();
		}
	}
}

// Nếu là khách
void LayerGameLieng::SpecMode() {
	this->hideAllButton();
}

void LayerGameLieng::FoldCards()
{
	this->disableForNextTurn();
	LayerAvatars->StopAllTimer();
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL) return;
	params->PutInt(EXT_FIELD_GAME_BET_VALUE, MinBetValue);
	params->PutInt(EXT_FIELD_BET_TYPE, GAME_LIENG_FOLD);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerGameLieng::Follow()
{
	this->disableForNextTurn();
	LayerAvatars->StopAllTimer();
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL) return;
	params->PutInt(EXT_FIELD_GAME_BET_VALUE, 0);
	params->PutInt(EXT_FIELD_BET_TYPE, GAME_LIENG_FOLLOW);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerGameLieng::Complete()
{
	//GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
	//GetButtonByTag(DTAG_BTN_COMPLETE)->setVisible(false);
}

void LayerGameLieng::PlaySound(const string& soundPath)
{
	if (mUtils::isSoundOn())
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundPath.c_str());
}

void LayerGameLieng::nextTurnHandle(const string& uid, string& betal)
{
	this->LayerAvatars->StopAllTimer();
	if (0 == this->MyName.compare(uid)) {
		this->LayerAvatars->GetUserByPos(kuser0)->startTimer();
		this->disableForNextTurn();
		std::vector<std::string> typeBet = mUtils::splitString(betal, ',');
		if (typeBet.size() == 0)
		{
			this->setEnabledButton(DTAG_BTN_BET, true);
		}
		else{
			boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
			double amfUser = 0;
			if (mySelf != NULL)
			{
				boost::shared_ptr<UserVariable> amf_ptr = mySelf->GetVariable(EXT_FIELD_GAME_CHIP);
				if (amf_ptr){
					amfUser = *amf_ptr->GetDoubleValue();
				}
			}

			for (int i = 0; i < typeBet.size(); ++i) {
				if (0 == typeBet[i].compare("1")) {
					if (this->MinBetValue >amfUser)
						this->setEnabledButton(DTAG_BTN_FOLLOW, true);
					else
						this->setEnabledButton(DTAG_BTN_BET, true);
				}
				else if (0 == typeBet[i].compare("2")) {
					this->setEnabledButton(DTAG_BTN_FOLD, true);
				}
				else if (0 == typeBet[i].compare("3")) {
					this->setEnabledButton(DTAG_BTN_FOLLOW, true);
				}
			}
		}
	}

	else {
		this->disableForNextTurn();
		switch (LayerAvatars->GetPosByName(uid)) {
		case kuser1:
			this->LayerAvatars->GetUserByPos(kuser1)->startTimer();
			break;

		case kuser2:
			this->LayerAvatars->GetUserByPos(kuser2)->startTimer();
			break;

		case kuser3:
			this->LayerAvatars->GetUserByPos(kuser3)->startTimer();
			break;

		case kuser4:
			this->LayerAvatars->GetUserByPos(kuser4)->startTimer();
			break;

		case kuser5:
			this->LayerAvatars->GetUserByPos(kuser5)->startTimer();
			break;

		case kuser6:
			this->LayerAvatars->GetUserByPos(kuser6)->startTimer();
			break;
		}
	}
}

void LayerGameLieng::resetGame(){
	this->FlagChiaBai = false;
	this->LayerCards->ResetGame();
	this->LayerAvatars->UnReadyAllUser();
	this->LayerAvatars->resetAllBets();
	this->LayerAvatars->hideAllBet(true);
	this->LayerAvatars->removeResult();
}

std::string LayerGameLieng::FindBossRoom(string listUser)
{
	vector<string> arrUser = mUtils::splitString(listUser, ';');
	if (arrUser.size() == 0) return "";
	vector<string> info = mUtils::splitString(arrUser[0], '|');
	return info[0];
}

void LayerGameLieng::updateEvent(float dt) {
	if (this->mThreadQueueMsg->isQueueEmpty())
		return;
	if (this->LayerCards->isRunningAction())
		return;
	this->mThreadQueueMsg->run();
}

void LayerGameLieng::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	pObjs->PutUtfString("_cmd_", *cmd);
	this->mThreadQueueMsg->addQueue(pObjs);
}

void LayerGameLieng::OnExtensionResponseProcess() {
	param = mThreadQueueMsg->deQueue();
	boost::shared_ptr<string> cmd = param->GetUtfString("_cmd_");

	log("cmd in LayerGameLieng: %s", cmd->c_str());
	if (cmd == NULL)
		return;

	if (0 == cmd->compare(EXT_EVENT_LIST_USER_UPDATE)) {
		this->event_EXT_EVENT_LIST_USER_UPDATE_4_LIENG(param);
	}

	else if (0 == cmd->compare(EXT_EVENT_READY_NTF)) {
		this->event_EXT_EVENT_READY_NTF(param);
	}

	else if (0 == cmd->compare(EXT_EVENT_ERROR_READY_NTF)) {
		this->event_EXT_EVENT_ERROR_READY_NTF(param);
	}

	else if (0 == cmd->compare(EXT_EVENT_START)) {
		this->event_EXT_EVENT_START(param);
	}

	else if (0 == cmd->compare(EXT_EVENT_LISTCARD_NTF)) {
		this->event_EXT_EVENT_LISTCARD_NTF(param);
	}

	else if (0 == cmd->compare(EXT_EVENT_END)) {
		this->event_EXT_EVENT_END(param);
	}

	else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT)) {
		this->event_EXT_EVENT_GAME_RESULT_4_LIENG(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_CHANGE_NOTIF)){
		this->event_EXT_EVENT_GAME_CHANGE_NOTIF(param);
	}
	// Bản tin nhận được sẽ bắt đầu đếm 15s, hết 15 không sẵn sàng sẽ bị kick
	else if (0 == cmd->compare(EXT_EVENT_START_READY_TIMER_NOTIF)) {
		this->event_EXT_EVENT_START_READY_TIMER_NOTIF(param);
	}

	else if (0 == cmd->compare(EXT_EVENT_USER_KICKED_NOTIF)) {
		this->event_EXT_KICK_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_CHANGE_BALANCE_NOTIF))
	{
		this->event_EXT_EVENT_CHANGE_BALANCE_NOTIF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_REGISTER_QUEUE_NTF)){
		this->event_EXT_EVENT_REGISTER_QUEUE_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NTF)) {
		this->event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF(param);
	}
	//Events 4 Lieng
	else if (0 == cmd->compare(EXT_EVENT_NEXT_TURN)) {
		this->event_EXT_EVENT_NEXT_TURN(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_BET_NTF)) {
		this->event_EXT_EVENT_GAME_BET_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_EXPIRE_CHIP_NTF))
	{
		this->event_EXT_EVENT_EXPIRE_CHIP_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_SUB_BALANCE_FIRST_NOTIF))
	{
		this->event_event_EXT_EVENT_SUB_BALANCE_FIRST_NOTIF(param);
	}
}

void LayerGameLieng::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	//boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	//boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	//boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	//if (ptrNotifiedUser->GetVariable("amf") == NULL)
	//    return;
	// boost::shared_ptr<double> money = ptrNotifiedUser->GetVariable("amf")->GetDoubleValue();
	// boost::shared_ptr<string> uid = (ptrNotifiedUser->Name());
	//HoangDDs
	// 	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	// 	if (ptrEventParams == NULL)
	// 		return;
	// 	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	// 	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	// 	if (ptrNotifiedUser != NULL){
	// 		boost::shared_ptr<string> name_ptr = ptrNotifiedUser->Name();
	// 		if (name_ptr)
	// 		{
	// 			if (0 == name_ptr->compare(MyName))
	// 			{
	// 				boost::shared_ptr<UserVariable> uck_ptr = ptrNotifiedUser->GetVariable("uck");//gia tri bat tat ga cua tung user
	// 				if (uck_ptr)
	// 				{
	// 					int uck = *uck_ptr->GetIntValue();
	// 				}
	// 			}
	// 		}
	// 
	// 		// 		boost::shared_ptr<UserVariable> amf = ptrNotifiedUser->GetVariable("amf");
	// 		//
	// 		// 		if (amf== NULL) return;
	// 		// 		//string money = boost::to_string(*ptrNotifiedUser->GetVariable("amf")->GetDoubleValue());
	// 		// 		int    money = (int)(*amf->GetDoubleValue());
	// 		// 		double moneyDouble = (*amf->GetDoubleValue());
	// 		// 		string name = boost::to_string(*ptrNotifiedUser->Name());
	// 		//
	// 		// 		log("OnSmartFoxUserVariableUpdate: name= %s, money= %d", name.c_str(), money);
	// 		// 		int pos = layerAvatars->getPosByName(name.c_str());
	// 		// 		if (pos == -1) return;
	// 		// 		// 	layerAvatars->getUserByPos(pos)->setMoney(money);
	// 		// 		// 	layerAvatars->getUserByPos(pos)->setMoney(moneyDouble);
	// 		// 
	// 		// 		//     arrName.push_back(name);
	// 		// 		//     arrMoney.push_back(money);
	// 		// 		// 	arrMoneyDouble.push_back(moneyDouble);
	// 		// 		log("size: %d", arrName.size());
	// 	}

}

void LayerGameLieng::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);
	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> ptrNotifiedMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);
	//
    if (dataManager.getAppConfig().is24h)
    {
        return;
    }
	log("ptrNotifiedMessage: %s", ptrNotifiedMessage->c_str());
	int pos = LayerAvatars->GetPosByName(ptrNotifiedUser->Name()->c_str());
	if (pos == -1)
		return;
	Avatar52La* avatar = LayerAvatars->GetUserByPos(pos);
	if (avatar)
		avatar->showChat(ptrNotifiedMessage->c_str());
	// LayerChat->ShowChatByPos(pos, ptrNotifiedMessage->c_str());
}

// // //Tìm chương
string LayerGameLieng::FindChuong(string& pListUsers) {
	string IdChuong = "";
	if (0 != pListUsers.compare("")) {
		vector<string> arrUser = mUtils::splitString(pListUsers, ';');
		for (unsigned int i = 0; i < arrUser.size(); ++i) {
			if (0 == arrUser[i].compare(""))
				continue;
			vector<string> info = mUtils::splitString(arrUser[i], '|');
			if (0 == info[1].compare("1")) {
				IdChuong = info[0];
				break;
			}
		}
		arrUser.clear();
	}
	return IdChuong;
}

////******* Xử lý sự kiện nhận được ************///

void LayerGameLieng::event_EXT_EVENT_START(boost::shared_ptr<ISFSObject> param)
{
	this->IsTurnUpAll = false;
	this->LayerCards->ResetGame();
	this->removeChildByTag(1048);
	this->LayerAvatars->UnReadyAllUser();
	this->isStartedGame = true;
	this->layerTimer->stopTimer();

	this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_BET)->setVisible(false);

	this->layerBackGround->hideTotalBet(false);
	this->layerBackGround->setTotalBet(0);
	// Nếu Chưa chia bài thì thực hiện hiệu ứng chia bài.
	if (!this->FlagChiaBai) {
		this->LayerCards->GivePocker();
	}
}

void LayerGameLieng::event_EXT_EVENT_END(boost::shared_ptr<ISFSObject> param)
{
	this->Current_My_Bet = 0;
	this->FlagChiaBai = false;
	this->FlagDatCuoc = false;
	this->IsTurnUpAll = false;
	this->ListCards = "";
	this->hideAllButton();
	this->removeChildByTag(1048);
}

void LayerGameLieng::event_EXT_EVENT_ERROR_READY_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<long> errc = param->GetInt(EXT_FIELD_ERROR_CODE);
	if (errc != NULL) {
		if (*errc == 30) {
			Chat *toast = Chat::create(dataManager.GetSysString(407), -1);
			this->addChild(toast);
		}
	}
}

void LayerGameLieng::event_EXT_EVENT_READY_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	if (uid != NULL) {
		int _pos = this->LayerAvatars->GetPosByName(*uid);
		if (_pos == -1) return;
		if (0 == uid->compare(this->MyName)) {
			this->FlagDatCuoc = true;
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
			this->GetButtonByTag(DTAG_BTN_BET)->setVisible(false);
			this->layerTimer->stopTimer();
		}
	}
}

void LayerGameLieng::event_EXT_EVENT_LISTCARD_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> lc = param->GetUtfString(EXT_FIELD_LIST_CARD);
	if (uid != NULL && lc != NULL) {
		if (0 == uid->compare(this->MyName)) {
			this->ListCards = *lc;
		}
		this->LayerAvatars->hideAllBet(false);
		if (this->FlagChiaBai) {
			log("uid turn up cards = %s lc = %s", uid->c_str(), lc->c_str());
			if (mCurrGameID == kGameLieng){ //only 4 lieng
				if (0 == uid->compare(MyName)) {
					this->LayerCards->TurnUpAllCards(ListCards, kuser0);
				}
				else{
					int pos = LayerAvatars->GetPosByName(*uid);
					this->LayerCards->TurnUpAllCards(*lc, pos);
				}
			}
		}
	}
}

void LayerGameLieng::event_EXT_EVENT_GAME_BET_NTF(boost::shared_ptr<ISFSObject> param)
{
	//gbv":1000,"uid":"thanhhv3
	boost::shared_ptr<long> gabv = param->GetInt(EXT_FIELD_GAME_ACCOUNT_BET_VALUE);
	boost::shared_ptr<long> gbv = param->GetInt(EXT_FIELD_GAME_BET_VALUE);
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<long> bet = param->GetInt(EXT_FIELD_BET_TYPE);
	if (bet != NULL && gbv != NULL && uid != NULL && gabv != NULL) {
		layerBackGround->hideTotalBet(false);
		layerBackGround->setTotalBet(*gbv);

		int pos = LayerAvatars->GetPosByName(uid->c_str());
		int bettype = *bet;
		string a = "5";
		string b = "";
		switch (bettype)
		{
		case GAME_LIENG_FOLD:
			b = "5";
			break;
		case GAME_LIENG_BET:
			b = "106";
			break;
		case GAME_LIENG_FOLLOW:
			b = "107";
			break;
		}
		Avatar52La* avatar = this->LayerAvatars->GetUserByPos(pos);
		if (avatar != NULL)
		{
			avatar->setResultLieng(a, b, "", "");
		}
		if (0 == uid->compare(this->MyName)) {
			this->Current_My_Bet = *gabv;
		}
		if (bettype != GAME_LIENG_FOLD)
			LayerAvatars->setBetInfo(*gabv, pos);
	}
}

void LayerGameLieng::event_EXT_EVENT_NEXT_TURN(boost::shared_ptr<ISFSObject> param)
{
	this->layerBackGround->hideTotalBet(false);
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<long> mb = param->GetInt(EXT_FIELD_MIN_BET);
	boost::shared_ptr<string> betal = param->GetUtfString(EXT_FIELD_BET_TYPE_ALLOW);

	if (mb != NULL)
		this->MinBetValue = *mb;
	if (uid != NULL && betal != NULL)
	{
		this->nextTurnHandle(*uid, *betal);
	}
}

void LayerGameLieng::event_EXT_EVENT_GAME_RESULT_4_LIENG(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> rg = param->GetUtfString(EXT_FIELD_GAME_RESULT);
	if (rg != NULL)
	{
		this->hideAllButton();
		this->LayerAvatars->StopAllTimer();
		vector<string> result = mUtils::splitString(*rg, ';');
		for (unsigned int i = 0; i < result.size(); i++)
		{
			vector<string> info = mUtils::splitString(result.at(i), '|');
			if (info.size() == 0) continue;
			string listcard = info.at(6);
			int typeCard = atoi(info.at(2).c_str());
			int posUser = this->LayerAvatars->GetPosByName(info.at(0));
			Avatar52La* avatar = this->LayerAvatars->GetUserByPos(posUser);
			if (avatar != NULL)
				avatar->setResultLieng(info[1], info[2], info[3], info[4]);
			//neu la chin no ko can thuc hien
			if (0 != info.at(0).compare(this->MyName))
				LayerCards->TurnUpAllCards(listcard, posUser);
		}
	}
}

void LayerGameLieng::event_EXT_EVENT_REGISTER_QUEUE_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> lu = param->GetUtfString("lu");
	if (lu != NULL)
	{
		log("DS Chau ria: %s", lu->c_str());
		this->LayerAvatars->setListUserGuess(lu->c_str(), '-');
	}
}

void LayerGameLieng::event_EXT_EVENT_LIST_USER_UPDATE_4_LIENG(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> lu = param->GetUtfString(EXT_FIELD_LIST_USER);
	if (lu != NULL) {
		this->ListUsers = *lu;
		log("List Users Lieng %s", ListUsers.c_str());
		//this->ListUsers = "vodanh9x|0.000000|0|0;vodanh9x|0.000000|0|0;vodanh9x|0.000000|0|0";
		//test
		//this->ListUsers += ";test3caychuong3|0|0|0|1;test3caychuong1|0|0|0|1;test3caychuong2|0|0|0|1;test3caychuong4|0|0|0|1;test3caychuong5|0|0|0|1;test3caychuong6|0|0|0|1";
		int _size = mUtils::splitString(this->ListUsers, ';').size();
		if (_size >= 4){
			this->LayerButton->processDisplayButtonInvite(false);
		}
		else{
			if (this->vipRoom == 1){
				this->LayerButton->processDisplayButtonInvite(true);
			}
		}

		vector<string> list = mUtils::splitString(ListUsers, ';');
		this->SizeListUser = list.size();
		this->LayerAvatars->SetListUser(ListUsers);
		this->LayerCards->setListUser(ListUsers);
		//this->LayerCards->setPlayerMap(LayerAvatars->GetMapPlayer());

		layerBackGround->hideTotalBet(true);
		//layerBackGround->hideTotalBet(false);

		if (this->IsPlaying()) {
			this->FlagChiaBai = true;
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
			layerTimer->stopTimer();
		}
		else {
			vector<string> arrUser = mUtils::splitString(this->ListUsers, ';');
			if (arrUser.size() > 1){
				if (!this->isStartedGame)
				{
					layerTimer->startTimer();
					this->GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
				}
			}
			else{
				layerTimer->stopTimer();
				this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
				this->resetGame();
			}
		}
		if (this->LayerAvatars->IsSpect())
		{
			this->SpecMode();
			layerTimer->stopTimer();
			if (!this->IsPlaying())
			{
				LayerCards->ResetGame();
			}
		}

		//for test bet action
		//this->GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
		// 		ListUsers = "daptraicogisai2|0.000000|0|1;tester17|0.000000|0|1;tester18|0.000000|0|1;tester19|0.000000|0|1;tester20|0.000000|0|1;tester21|0.000000|0|1;tester16|0.000000|0|1";
		// 		this->LayerAvatars->SetListUser(ListUsers);
		// 		this->LayerCards->setListUser(ListUsers);
		// 		for (int i = 0; i < 7; i++) {
		// 			LayerCards->TurnUpAllCards("9_2-13_3-14_2", i);
		// 		}
		//test
		//this->LayerCards->GivePocker();
	}
}

void LayerGameLieng::event_EXT_KICK_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	if (uid != NULL)
	{
		if (0 == uid->compare(this->MyName)) {
			this->LayerButton->BackToChooseDesk();
		}
	}
}

void LayerGameLieng::event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF(boost::shared_ptr<ISFSObject> param)
{
	this->LayerButton->BackToChooseDesk();
}

void LayerGameLieng::event_EXT_EVENT_START_READY_TIMER_NOTIF(boost::shared_ptr<ISFSObject> param)
{
	this->LayerButton->getButtonByTag(DTAG_BTN_READY)->setVisible(true);
	this->resetGame();
	this->layerTimer->startTimer();
}

void LayerGameLieng::event_EXT_EVENT_CHANGE_BALANCE_NOTIF(boost::shared_ptr<ISFSObject> param)
{
	//    // sự kiện công/trừ tiền
	//    var uid = params.uid;
	//    var money = params.amf;
	log("event_EXT_EVENT_AMF_TEST_NOTIF");
	boost::shared_ptr<string> name = param->GetUtfString("uid");

	boost::shared_ptr<double> money_ptr;
	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		money_ptr = param->GetDouble("amf");
	}
	else{
		money_ptr = param->GetDouble("amfs");
	}
	boost::shared_ptr<double> betvl = param->GetDouble("betvl");
	boost::shared_ptr<long> cbt = param->GetInt("cbt");
	int resson = -1;
	long money = 0;
	if (name != NULL && money_ptr != NULL && cbt != NULL) {
		resson = (int)(*cbt.get());
		money = (long)(*money_ptr.get());
		log("name= %s, money= %d, reason= %d", name->c_str(), money, resson);

		int pos = LayerAvatars->GetPosByName(name->c_str());
		std::string res;
		if (money > 0)
			res = "+" + StringUtils::toString(money);
		else
			res = StringUtils::toString(money);
		auto avatar = LayerAvatars->GetUserByPos(pos);
		if (avatar)
			avatar->showNumber(res);
	}
}

void LayerGameLieng::event_EXT_EVENT_EXPIRE_CHIP_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> uid_ptr = param->GetUtfString(EXT_FIELD_USERID);
	if (uid_ptr != NULL)
	{
		if (0 == uid_ptr->compare(MyName))
		{
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
			vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
			int bet_ratio = 1;
			for (int i = 0; i < arrBet.size(); ++i)
			{
				if (arrBet.at(i).gameid == kGameXiTo)
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
				int nBetValue = atoi(money.c_str());
				LayerPopupChonTienLieng* chontien = LayerPopupChonTienLieng::create();
				chontien->setMinMaxValue(nBetValue, bet_ratio*nBetValue);
				chontien->setJoinType(2);
				this->addChild(chontien, ZORDER_LIST::ZORDER_POPUP_WEBVIEW, ZORDER_LIST::ZORDER_POPUP_WEBVIEW);
			}
		}
	}
}

void LayerGameLieng::event_EXT_EVENT_GAME_CHANGE_NOTIF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> listplayers = param->GetUtfString(EXT_FIELD_GAME_INFO_NOTIFY);
	if (0 == listplayers->compare(""))
		return;

	///"ginf":"bokinhvan114|200.000000|1|1|8_0-4_1-2_1;bokinhvan113|800.000000|0|1|0_0-0_0-0_0;"} ] to user bokinhvan114
	//return String.format("%s|%f|%d|%d", user.getName(), currentbet ,needplay?1:0, isReady?1:0);
	this->layerTimer->stopTimer();
	 	this->LayerAvatars->SetListUser(*listplayers);
	 	this->LayerCards->setListUser(*listplayers);

	vector<string> list = mUtils::splitString(*listplayers, ';');
	for (unsigned int i = 0; i < list.size(); i++) {
		vector<string> info = mUtils::splitString(list[i], '|');
		if (info.size() != 5)
			continue;
		string userName = info[0];
		string listcard = info[4];
		double betvalue = atof(info[1].c_str());
		int neadPlay = atoi(info[2].c_str());
		int isReady = atoi(info[3].c_str());

		if (0 == this->MyName.compare(userName)) {
			auto pos = this->LayerAvatars->GetPosByName(userName);
			auto avatar = this->LayerAvatars->GetUserByPos(pos);

			if (avatar != NULL){

				if (isReady != 0)
				{

					this->LayerAvatars->setBetInfo(betvalue, pos);
					this->LayerCards->setRejoinSizeCardMe();
					this->LayerCards->TurnUpAllCards(listcard, kuser0);
					if (neadPlay)
					{
						avatar->startTimer();
						this->GetButtonByTag(DTAG_BTN_BET)->setVisible(true);
						this->GetButtonByTag(DTAG_BTN_FOLD)->setVisible(true);
						this->GetButtonByTag(DTAG_BTN_FOLLOW)->setVisible(true);
						this->setEnabledButton(DTAG_BTN_BET, true);
						this->setEnabledButton(DTAG_BTN_FOLLOW, true);
						this->setEnabledButton(DTAG_BTN_FOLD, true);
					}
				}
				else
				{
					avatar->stopTimer();
				}
			}

			if (isReady != 0) {
				this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
			}
			if (3 != LayerCards->getListCards_0().size() && neadPlay != 0) {
				this->LayerCards->CreateCardBack(kuser0);
			}
			this->FlagChiaBai = true;
		}
		else {

			int pos = this->LayerAvatars->GetPosByName(userName);
			Avatar52La* avatar = this->LayerAvatars->GetUserByPos(pos);
			if (avatar != NULL){

				if (isReady != 0)
				{
					this->LayerAvatars->setBetInfo(betvalue, pos);
					this->LayerCards->CreateCardBack(pos);
					if (neadPlay)
					{
						avatar->startTimer();
					}
				}
				else
				{
					avatar->stopTimer();
				}
			}

			this->FlagChiaBai = true;
		}
	}
}

void LayerGameLieng::hideAllButton()
{
	this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_FOLD)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_BET)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_FOLLOW)->setVisible(false);
}

void LayerGameLieng::disableForNextTurn()
{
	this->GetButtonByTag(DTAG_BTN_BET)->setVisible(true);
	this->GetButtonByTag(DTAG_BTN_FOLD)->setVisible(true);
	this->GetButtonByTag(DTAG_BTN_FOLLOW)->setVisible(true);
	this->setEnabledButton(DTAG_BTN_BET, false);
	this->setEnabledButton(DTAG_BTN_FOLD, false);
	this->setEnabledButton(DTAG_BTN_FOLLOW, false);
}

void LayerGameLieng::setEnabledButton(int tag, bool enable)
{
	this->GetButtonByTag(tag)->setTouchEnabled(enable);
	int opacity = 255;
	if (!enable)
		opacity = 100;
	this->GetButtonByTag(tag)->setOpacity(opacity);
}

void LayerGameLieng::event_event_EXT_EVENT_SUB_BALANCE_FIRST_NOTIF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<double> bet_total = param->GetDouble(EXT_FIELD_BET_TOTAL);
	if (bet_total)
	{
		//run animation total bet
		//! splitString
		Vec2 dest = Vec2(WIDTH_DESIGN / 2, HEIGHT_DESIGN / 2 + sizeAd.height);
		vector<string> arrUser = mUtils::splitString(this->ListUsers, ';');
		if (arrUser.size() > 1){
			double bet_vl = *bet_total / arrUser.size();
			for (int i = 0; i < arrUser.size(); ++i) {
				vector<string> info = mUtils::splitString(arrUser[i], '|');
				if (info.size() > 0)
				{
					int pos = LayerAvatars->GetPosByName(info[0]);
					Avatar52La* avatar = LayerAvatars->GetUserByPos(pos);
					if (avatar != NULL)
					{
						avatar->runActionTotalBet(dest);
						avatar->runActionBet(bet_vl);
					}
				}
			}

		}
		layerBackGround->setTotalBet(*bet_total);
		this->PlaySound("chip_moving.mp3");
	}
}

void LayerGameLieng::callBackLayerDatCuoc()
{
	//this->GetButtonByTag(DTAG_BTN_BET)->setVisible(true);
	this->disableForNextTurn();
}

bool LayerGameLieng::IsPlaying()
{
	if (0 == ListUsers.compare(""))
		return false;
	vector<string> info = mUtils::splitString(ListUsers, ';');
	if (info.size() > 0)
	{
		vector<string> firt = mUtils::splitString(info.at(0), '|');
		if (firt.size() > 0)
		{
			if (0 == firt.at(2).compare("1"))
				return true;
			return false;
		}
	}
}

void LayerGameLieng::processButtonToTouched(Ref *sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		this->betLayerSlider->setVisible(false);
		log("ButtonTo touched: %d", this->betLayerSlider->getValueBet());
		//Send Request Bet Game
		/*if (valueBet > userAmf) {
			this->showToast("Hiện tại bạn không đủ tiền đặt cược");
			return;
			}
			else if (valueBet < minValue) {
			this->showToast("Số tiền đặt không hợp lệ");
			return;
			}*/
		int valueBet = this->betLayerSlider->getValueBet();
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutInt(EXT_FIELD_BET_TYPE, GAME_LIENG_BET);
		params->PutInt(EXT_FIELD_GAME_BET_VALUE, valueBet);
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
		GameServer::getSingleton().Send(request);
		/*this->runAction(Sequence::create(CallFunc::create(this->listener),
			DelayTime::create(0.2), RemoveSelf::create(), NULL));*/
	}
}

