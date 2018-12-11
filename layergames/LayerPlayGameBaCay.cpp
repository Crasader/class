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

#include "LayerPlayGameBaCay.h"
#include "_Background_inGame_.h"
#include "_Number_Chat_inGame7u.h"
#include "SliderCustomLoader.h"
#include "LayerBet_BaCayChuong.h"
#include "LayerBet_Lieng.h"
#include "Nan3Cay.h"
#include "_Chat_.h"

#include "mUtils.h"
#include "AllData.h"
#include "data_manager.h"
#include "ClientMsgDefs.h"
#include "ChanUtils.h"
#include "PhomMessDef.h"
vector<string> LayerPlayGameBaCay::split(string &S, const char &str) {
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
LayerPlayGameBaCay::LayerPlayGameBaCay()
:LayerAvatars(NULL),
LayerChat(NULL),
LayerButton(NULL),
LayerCards(NULL),
LayerBet(NULL),
mLayerCount(NULL),
nameGame(NULL),
layerGa(NULL),
ListCards(""),
ListUsers(""),
MyAI(""),
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
	this->valueBet = 0;
}

LayerPlayGameBaCay::~LayerPlayGameBaCay() {
	if (mThreadQueueMsg != NULL) {
		delete mThreadQueueMsg;
	}
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameBaCay::updateEvent),this);
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPlayGameBaCay::init() {
	if (!Layer::init())
		return false;

	//test unit
	//this->event_EXT_EVENT_DEAL_CARD_BOSS_NOTIF("vodanh9x|8 Co(36); hehe_keke|3 Co(16)");

	mCurrGameID = SceneManager::getSingleton().getGameID();
	log("mCurrentGameId: %d", mCurrGameID);
	if (GameServer::getSingleton().getSmartFox()->MySelf() == NULL) return false;
	this->myName = SceneManager::getSingleton().getMyName();
	this->MyAI = *GameServer::getSingleton().getSmartFox()->MySelf()->Name();
	// this->IsSpector = GameServer::getSingleton().getSmartFox()->MySelf()->IsSpectator();
	//	log("%s", IsSpector ? "Khách" : "Người chơi");

	///*moc3-add buttons thuc hien chuc nang danh bien*/
	this->btnNhanBienAll1 = CreateButtonWithTitle_Pos("ingame-button.png", Vec2(WIDTH_DESIGN / 2 + 157 * 2, 20));
	this->btnBienTat1 = CreateButtonWithTitle_Pos("ingame-button.png", Vec2(WIDTH_DESIGN / 2 + 157 * 3, 20));
	this->btnBienTatx2_1 = CreateButtonWithTitle_Pos("ingame-button.png", Vec2(WIDTH_DESIGN / 2 + 157 * 4, 20));
	//this->btnDat = CreateButtonWithTitle_Pos("ingame-button.png", Vec2(WIDTH_DESIGN / 2 + 157 * 5, 20));
	/*auto labelDat = Label::createWithTTF("ĐẶT CƯỢC", "__Roboto-Bold.ttf", 30);
	labelDat->setAnchorPoint(Vec2(0.5, 0.5));
	this->btnDat->addChild(labelDat);
	labelDat->setPosition(Vec2(this->btnDat->getContentSize().width / 2, this->btnDat->getContentSize().height / 2));
	*/
	this->layerBetBaCay = LayerDatCuocBaCay::create();
	//this->btnDat = this->layerBetBaCay->getButtonConfirm();
	auto btnDatCua = this->layerBetBaCay->getButtonConfirm();
	if (btnDatCua != NULL){
		btnDatCua->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameBaCay::processBtnDatCuaTouched, this));
	}
	auto labelNhanBien = Label::createWithSystemFont(dataManager.GetSysString(627), "", 30);
	labelNhanBien->setAnchorPoint(Vec2(0.5, 0.5));
	auto labelALL = Label::createWithSystemFont(dataManager.GetSysString(670), "", 30);
	labelALL->setAnchorPoint(Vec2(0.5, 0.5));
	Sprite *checkBg = Sprite::create("moc3-bg-uncheck.png");
	checkBg->setAnchorPoint(Vec2(0.5, 0.5));
	this->checkedBg = Sprite::create("moc3-checked.png");
	checkedBg->setAnchorPoint(Vec2(0.5, 0.5));
	checkedBg->setVisible(false);
	this->btnNhanBienAll1->addChild(labelNhanBien);
	this->btnNhanBienAll1->addChild(labelALL);
	this->btnNhanBienAll1->addChild(checkBg);
	this->btnNhanBienAll1->addChild(this->checkedBg);

	labelNhanBien->setPosition(Vec2(btnNhanBienAll1->getContentSize().width / 2, btnNhanBienAll1->getContentSize().height - labelNhanBien->getContentSize().height / 2 - 15));
	checkBg->setPosition(Vec2(checkBg->getContentSize().width * 3 / 2, labelNhanBien->getPositionY() - labelNhanBien->getContentSize().height - 10));
	labelALL->setPosition(Vec2(checkBg->getPositionX() + checkBg->getContentSize().width * 2, checkBg->getPositionY()));
	this->checkedBg->setPosition(Vec2(checkBg->getPosition()));

	auto labelBienAll = Label::createWithSystemFont(dataManager.GetSysString(628), "", 30);
	labelBienAll->setAnchorPoint(Vec2(0.5, 0.5));

	this->labelBienAllMoney = Label::createWithSystemFont("...", "", 30);
	this->labelBienAllMoney->setAnchorPoint(Vec2(0.5, 0.5));
	this->labelBienAllMoney->setColor(Color3B::YELLOW);

	this->btnBienTat1->addChild(labelBienAll);
	this->btnBienTat1->addChild(this->labelBienAllMoney);
	labelBienAll->setPosition(Vec2(btnBienTat1->getContentSize().width / 2, btnBienTat1->getContentSize().height - labelBienAll->getContentSize().height / 2 - 15));
	this->labelBienAllMoney->setPosition(Vec2(btnBienTat1->getContentSize().width / 2, btnBienTat1->getContentSize().height / 2 - this->labelBienAllMoney->getContentSize().height / 2));

	auto labelBienAllx2 = Label::createWithSystemFont(dataManager.GetSysString(628), "", 30);
	labelBienAllx2->setAnchorPoint(Vec2(0.5, 0.5));

	this->labelBienAllMoneyx2 = Label::createWithSystemFont("...", "", 30);
	this->labelBienAllMoneyx2->setAnchorPoint(Vec2(0.5, 0.5));
	this->labelBienAllMoneyx2->setColor(Color3B::YELLOW);

	this->btnBienTatx2_1->addChild(labelBienAllx2);
	this->btnBienTatx2_1->addChild(this->labelBienAllMoneyx2);
	labelBienAllx2->setPosition(Vec2(btnBienTatx2_1->getContentSize().width / 2, btnBienTatx2_1->getContentSize().height - labelBienAllx2->getContentSize().height / 2 - 15));
	this->labelBienAllMoneyx2->setPosition(Vec2(btnBienTatx2_1->getContentSize().width / 2, btnBienTatx2_1->getContentSize().height / 2 - this->labelBienAllMoneyx2->getContentSize().height / 2));
	/*moc3-end change*/

	this->CreateBackgrounds();
	this->CreateTimer();
	this->CreateCards();
	this->CreateButtons();
	sizeAd = mUtils::getSizePos();
	this->CreateLayerBet();
	this->createLayerGa();
	this->LayerChat = Number_Chat_inGame7u::create();
	this->addChild(this->LayerChat);

	if (mCurrGameID == kGameBaCayChuong){
		//this->btnDat->setTag(TAG_BTN_DAT_CUA);
		this->btnNhanBienAll1->setTag(TAG_BTN_NHAN_BIEN_ALL);
		this->btnBienTat1->setTag(TAG_BTN_BIEN_ALL_X1);
		this->btnBienTatx2_1->setTag(TAG_BTN_BIEN_ALL_X2);
		this->addChild(this->layerBetBaCay);
		this->addChild(this->btnNhanBienAll1);
		this->addChild(this->btnBienTat1);
		this->addChild(this->btnBienTatx2_1);
		//this->addChild(this->btnDat);

		this->hideUIDanhBien();
	}
	this->CreateAvatars();
	this->createLayerBien();

	//test
	this->mThreadQueueMsg = new ThreadQueueMsg();
	this->mThreadQueueMsg->setFunCallBack(this, callfunc_selector(LayerPlayGameBaCay::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameBaCay::updateEvent),this,1/60, false);
	this->ResetGame();
	this->createLoadingBet();
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

	//test
	/*string ListCardsTest = "8_2-4_3-3_1";
	Nan3Cay* m_NanBai = Nan3Cay::create();
	m_NanBai->SetCallbackFunc(this, callfuncN_selector(LayerPlayGameBaCay::CallFuncTurnUpAll));
	m_NanBai->InitListCardHand(ListCardsTest);
	m_NanBai->setTag(TAG_NAN_BACAY_LAYER);
	this->addChild(m_NanBai);*/
	/*LayerCardInGame *cardInGame = LayerCardInGame::create();
	cardInGame->getURL_byID_3CayChuong(35);*/

	return true;
}

void LayerPlayGameBaCay::CreateBackgrounds() {

	BackgroundInGame *bg = BackgroundInGame::create();
	this->addChild(bg);

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
			std::string moneyConvert = mUtils::convertMoneyEx(atoi(money.c_str()));

			this->money_bet = atol(money.c_str());
			this->labelBienAllMoney->setString(moneyConvert);
			this->labelBienAllMoneyx2->setString(mUtils::convertMoneyEx(money_bet * 2));
			int roomID = 101;
			boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom != NULL){
				roomID = lastRoom->Id();
				//HoangDD Change
				string sRoomIdDisplay = mUtils::getRoomIDByName(*lastRoom->Name());
				if (sRoomIdDisplay != "")
					roomID = atoi(sRoomIdDisplay.c_str());
			}
			bg->setNameTable(name, roomID, money_bet);
		}
	}

	// this->//nameGame = Label::createWithTTF(this->StringResult, kNormalFont, 16);
	// this->//nameGame->setPosition(Vec2(400 - 5, 213 + 10));
	// this->//nameGame->setColor(Color3B::WHITE);
	// this->//nameGame->setOpacity(150);
	// bg->addChild(this->//nameGame);
}

void LayerPlayGameBaCay::CreateCards() {
	this->LayerCards = CardBaCay::create();
	this->LayerCards->SetMyName(this->MyAI);
	this->LayerCards->SetCallbackFunc(this, callfuncN_selector(LayerPlayGameBaCay::CallFuncEndGive));
	this->addChild(this->LayerCards);
}

void LayerPlayGameBaCay::CreateAvatars() {
	this->LayerAvatars = LayerBaCayAvatar::create();
	this->LayerAvatars->setTag(1309);
	this->LayerAvatars->ResetAll();
	this->addChild(this->LayerAvatars);
}
void LayerPlayGameBaCay::createLayerGa() {
	this->layerGa = LayerGa::create();
	this->layerGa->resetAll();
	layerGa->callBackFromActionGa(this, callfunc_selector(LayerPlayGameBaCay::actionConfigGa));
	this->addChild(layerGa);
	if (SceneManager::getSingleton().getIsBotZone()
		&& CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	{
		layerGa->setVisible(false);
	}
}
void LayerPlayGameBaCay::actionConfigGa()
{
	if (layerGa->getListGa().size() > 1)
	{
		layerGa->showListChicken();
		return;
	}

	// 	if (isStartedGame){
	// 		if (layerGa->getChickenMoney() <= 0) return;
	// 		layerGa->showListChicken();
	// 		return;
	// 	}
	// 	else{
	// 		if (layerGa->getListGa().size() > 1 && IsSpector)//danh cho thang xem
	// 		{
	// 			layerGa->showListChicken();
	// 			return;
	// 		}
	// 		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	// 		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	// 		if (lastRoom == NULL)
	// 			return;
	// 		if (layerGa->getNuoiGa() == false){ //trang thai da tat, gui ban tin bat ga
	// 			parameter->PutInt(EXT_FIELD_CONFIRM, LayerGa::CONFIG::ON);
	// 		}
	// 		else{// gui ban tin tat ga
	// 			parameter->PutInt(EXT_FIELD_CONFIRM, LayerGa::CONFIG::OFF);
	// 		}
	// 		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_USE_CHICKEN_REQ, parameter, lastRoom));
	// 		GameServer::getSingleton().Send(request);
	// 	}
}
std::string LayerPlayGameBaCay::find_ChuPhong(string listUser)
{
	if (listUser == "") return "";
	vector<string> arrUser = split(listUser, ';');
	vector<string> info = split(arrUser[0], '|');
	if (info.size() <= 1) return "";
	string boosId = info[0];
	return boosId;
}
void LayerPlayGameBaCay::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
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
				this->mLayerCount->setCount((*endTime->GetDoubleValue()) / 1000);
			}
		}

	}
}
void LayerPlayGameBaCay::CreateLayerBet() {
	this->LayerBet = BetGame3Cay::create();
	this->addChild(this->LayerBet);
}

void LayerPlayGameBaCay::CreateButtons() {
	this->LayerButton = LayerButtonInGame::create();
	this->addChild(this->LayerButton);
	this->LayerButton->setLocalZOrder(10000);
    if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL){
        return;
    }
	boost::shared_ptr<RoomVariable> isVipRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("isviproom");
	if (isVipRoom != NULL){
		this->vipRoom = *isVipRoom->GetIntValue();
		if (vipRoom == 0){
			this->LayerButton->processDisplayButtonInvite(false);
		}
		else{
			this->LayerButton->processDisplayButtonInvite(true);
		}
	}

	//Size convert
	Size _size = Size(0, 0);

	int w_Button = 157;
	int h_Button = 157;
	int _space = 20;

	Button* btn_Ready = CreateButtonWithTitle_Pos("button_batdau.png", Vec2(WIDTH_DESIGN / 2 + w_Button * 2, _space + _size.height));
	Button* btn_Unready = CreateButtonWithTitle_Pos(dataManager.GetSysString(196), Vec2(WIDTH_DESIGN - w_Button - _space + _size.width, _space + _size.height));
	Button* btn_TurnUpAll = CreateButtonWithTitle_Pos("button_latbai.png", Vec2(WIDTH_DESIGN / 2 + w_Button * 3, _space + _size.height));
	Button* btn_View = CreateButtonWithTitle_Pos("button_xembai.png", Vec2(WIDTH_DESIGN / 2 + w_Button * 2, _space + _size.height));
	Button* btn_Squeezing = CreateButtonWithTitle_Pos("button_nanbai.png", Vec2(WIDTH_DESIGN / 2 - w_Button * 2, _space + _size.height));

	btn_Ready->setTag(DTAG_BTN_READY);
	btn_Unready->setTag(DTAG_BTN_UNREADY);
	btn_TurnUpAll->setTag(DTAG_BTN_TURNALL);
	btn_View->setTag(DTAG_BTN_VIEW);
	btn_Squeezing->setTag(DTAG_BTN_SQUEEZ);

	btn_Ready->setVisible(false);
	btn_Unready->setVisible(false);
	btn_TurnUpAll->setVisible(false);
	btn_View->setVisible(false);
	btn_Squeezing->setVisible(false);

	this->LayerButton->addChild(btn_Ready);
	this->LayerButton->addChild(btn_Unready);
	this->LayerButton->addChild(btn_TurnUpAll);
	this->LayerButton->addChild(btn_View);
	this->LayerButton->addChild(btn_Squeezing);
}

Button* LayerPlayGameBaCay::CreateButtonWithTitle_Pos(string imgPath, const Vec2& pPoint) {
	Button* m_Button = Button::create();
	m_Button->setTouchEnabled(true);
	m_Button->setScale9Enabled(false);
	m_Button->loadTextures(imgPath, "", "");
	//m_Button->setTitleText(pName);
	// m_Button->setTitleColor(Color3B::RED);
	m_Button->setTitleFontSize(40);
	//m_Button->setTitleFontName(kNormalFont);
	m_Button->setAnchorPoint(Vec2(0, 0));
	m_Button->setPosition(pPoint);
	m_Button->setScale(0.8);

	m_Button->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameBaCay::OnButtonClick, this));
	return m_Button;
}

Button* LayerPlayGameBaCay::GetButtonByTag(const int& pTag) {
	Button* button = (Button*) this->LayerButton->getChildByTag(pTag);
	return button;
}

void LayerPlayGameBaCay::createLayerChat() {
	this->LayerChat = Number_Chat_inGame7u::create();
	this->addChild(this->LayerChat);
}

void LayerPlayGameBaCay::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	pObjs->PutUtfString("_cmd_", *cmd);
	this->mThreadQueueMsg->addQueue(pObjs);
}


void LayerPlayGameBaCay::OnExtensionResponseProcess() {
	this->param = mThreadQueueMsg->deQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");

	log("cmd in LayerPlayGameBaCay: %s", cmd->c_str());

	if (0 == cmd->compare(EXT_EVENT_LIST_USER_UPDATE)) {
		if (mCurrGameID == kGameBaCayChuong)
			this->event_EXT_EVENT_LIST_USER_UPDATE_3CAYCHUONG();
		else if (mCurrGameID == kGameBaCay)
			this->event_EXT_EVENT_LIST_USER_UPDATE_3CAYNORMAL();
	}
	else if (0 == cmd->compare(EXT_EVENT_READY_RES)){
		if (mCurrGameID == kGameBaCayChuong){
			//this->displayUIBet(true);
			log("ready");
		}
	}
	else if (0 == cmd->compare(EXT_EVENT_READY_NTF)) {
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong){
			this->event_EXT_EVENT_READY_NTF();
		}
	}
	else if (0 == cmd->compare(EXT_EVENT_START_GAME_NOTIF)){
		if (mCurrGameID == kGameBaCayChuong){
			this->event_EXT_EVENT_START_GAME_NOTIF();
		}
	}
	else if (0 == cmd->compare(EXT_EVENT_UNREADY_NTF)) {
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong){
			this->event_EXT_EVENT_UNREADY_NTF();
		}
	}

	else if (0 == cmd->compare(EXT_EVENT_ERROR_READY_NTF)) {
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong){
			this->event_EXT_EVENT_ERROR_READY_NTF();
		}
	}

	else if (0 == cmd->compare(EXT_EVENT_START)) {
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong){
			this->event_EXT_EVENT_START();
		}
	}

	else if (0 == cmd->compare(EXT_EVENT_LISTCARD_NTF)) {
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong)
			this->event_EXT_EVENT_LISTCARD_NTF();
	}

	else if (0 == cmd->compare(EXT_EVENT_END)) {
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong)
			this->event_EXT_EVENT_END();
	}

	else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT)) {
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong)
			this->event_EXT_EVENT_GAME_RESULT();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_CHANGE_NOTIF)){
		boost::shared_ptr<string> lu = param->GetUtfString("ginf");
        boost::shared_ptr<string> lc = param->GetUtfString("lc");
        boost::shared_ptr<long> isdeal = param->GetInt("isdeal");
        int isdealcard = 1;
        if (isdeal)
            isdealcard = *isdeal;
		this->UserRejoinGame(*lu,isdealcard);
        if (lc){
            auto lscard = mUtils::splitString(*lc, ';');
            for (int i = 0 ; i< lscard.size(); i++){
                auto ui = mUtils::splitString(lscard[i], '|');
                if (ui.size()< 2) return;
                int Pos = this->LayerCards->GetPosUserByName(ui[0]);
                this->LayerCards->TurnUpAllCards(ui[1], Pos);
            }
        }
	}

	// Bản tin đặt cược của người chơi
	else if (0 == cmd->compare(EXT_EVENT_GAME_BET_NTF)) {
		this->event_EXT_EVENT_GAME_BET_NTF();
	}

	// Bản tin nhận được sẽ bắt đầu đếm 15s, hết 15 không sẵn sàng sẽ bị kick
	else if (0 == cmd->compare(EXT_EVENT_START_READY_TIMER_NOTIF)) {
		this->mLayerCount->startTimer();
		this->ResetGame();
		//        this->GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
		//this->//mLayerCount->StartCountDownReady();
	}

	else if (0 == cmd->compare(EXT_EVENT_USER_KICKED_NOTIF)) {
		boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
		if (0 == uid->compare(this->MyAI)) {
			this->LayerButton->BackToChooseDesk();
		}
	}
	else if (0 == cmd->compare(EXT_EVENT_CHICKEN_UPDATE_NTF)) {
		boost::shared_ptr<double> tienga = param->GetDouble(EXT_FIELD_CHICKEN_VALUE);
		boost::shared_ptr<long> lv_ga = param->GetInt(EXT_FIELD_CHICKEN_LEVEL);
		boost::shared_ptr<string> listUserGa = param->GetUtfString("lu");
		if (tienga != NULL && lv_ga != NULL && listUserGa != NULL){
			double chicken_value = (double)*tienga;
			int chicken_lv = (int)*lv_ga;
			layerGa->setChickenLv(chicken_lv);
			layerGa->setChickenMoney(chicken_value);
			vector<string> list_user = mUtils::splitString(*listUserGa, ',');
			layerGa->setListDirect(list_user);
		}
		// 		string listUser = "48:bot_16:1:1:0-50:bot_171:0:1:0-48:bot_16:1:1:0-50:bot_171:0:1:0-48:bot_16:1:1:0-50:bot_171:0:1:0-48:bot_16:1:1:0";
		// 		layerGa->setChickenMoney(100000);
		// 		layerGa->setChickenLv(10);
		// 		layerGa->setListUser(listUser, TL_CHAR_SPLIT_1, TL_CHAR_SPLIT_2);
	}
	else if (0 == cmd->compare(EXT_EVENT_CHANGE_BALANCE_NOTIF))
	{
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong){
			this->event_EXT_EVENT_AMF_TEST_NOTIF();
		}
	}
	else if (0 == cmd->compare(EXT_EVENT_REGISTER_QUEUE_NTF)){
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong)
			this->event_EXT_EVENT_REGISTER_QUEUE_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY)){
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong)
			this->event_EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY();
	}
	else if (0 == cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NTF)) {
		this->LayerButton->BackToChooseDesk();
	}
	//Events 4 Lieng
	else if (0 == cmd->compare(EXT_EVENT_NEXT_TURN)) {
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong)
			this->event_EXT_EVENT_NEXT_TURN();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_BET_NTF)) {
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong){
			this->event_EXT_EVENT_GAME_BET_NTF();
		}
	}
	else if (0 == cmd->compare(EXT_EVENT_WAIT_BET_TIMER_NTF)) {
		if (mCurrGameID == kGameBaCay || mCurrGameID == kGameBaCayChuong)
			this->event_EXT_EVENT_WAIT_BET_TIMER_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_ENROLL_BET_RESPONSE)){
		log("ebres");
		int rc = 0;
		std::string rd = "";
		boost::shared_ptr<long> _rc = param->GetInt("rscode");
		boost::shared_ptr<string> _rd = param->GetUtfString("rd");
		if (_rc != NULL){
			rc = *_rc;
		}
		if (_rd != NULL){
			rd = *_rd;
		}
		log("rc= %d - rd= %s", rc, rd.c_str());
	}
	else if (0 == cmd->compare(EXT_EVENT_ENROLL_ALL_INFO_NOTIF)){
		log("eaintf");
		//hehe_keke_1_0; vodanh9x_0_1;
		std::string lstDanhBien = "";
		boost::shared_ptr<string> _lstInfo = param->GetUtfString("list_info");
		if (_lstInfo != NULL)
			lstDanhBien = *_lstInfo; //acc_status_money;acc_status_money;..
		log("list: %s", lstDanhBien.c_str());
		this->event_EXT_EVENT_ENROLL_ALL_INFO_NOTIF(lstDanhBien);
	}
	else if (0 == cmd->compare("ebntf")){
		log("ebntf");
		std::string uid = "";
		double betValue = 0;
		boost::shared_ptr<long> _betvl = param->GetInt("betvl");
		boost::shared_ptr<string> _uid = param->GetUtfString("uid");
		if (_betvl != NULL)
			betValue = *_betvl; //acc_money;acc_money;..
		if (_uid != NULL)
			uid = *_uid;
		log("list: %s - bet: %d", uid.c_str(), betValue);
		this->event_EXT_EVENT_ENROLL_BET_NTF(uid, betValue);
	}
	else if (0 == cmd->compare(EXT_EVENT_DEAL_CARD_BOSS_NOTIF)){
		if (mCurrGameID == kGameBaCayChuong){
			log("dcbntf");
			this->mLayerCount->stopTimer();
			std::string lc = *param->GetUtfString("lc");
			this->event_EXT_EVENT_DEAL_CARD_BOSS_NOTIF(lc);
		}
	}
	else if (0 == cmd->compare(EXT_EVENT_NEW_BOSS_NOTIF)){
		log("nbntf");
		this->event_EXT_EVENT_NEW_BOSS_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_USER_LEAVE_NOTIF)){
		this->event_EXT_EVENT_USER_LEAVE_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_USER_JOIN_NOTIF)){
		log("jrntf");
		this->event_EXT_EVENT_USER_JOIN_NOTIF();
	}
}


void LayerPlayGameBaCay::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	log("Update User Variables");
	////boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	////boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	////boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	////if (ptrNotifiedUser->GetVariable("amf") == NULL)
	////    return;
	//// boost::shared_ptr<double> money = ptrNotifiedUser->GetVariable("amf")->GetDoubleValue();
	//// boost::shared_ptr<string> uid = (ptrNotifiedUser->Name());
	////HoangDDs
	//boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	//if (ptrEventParams == NULL)
	//	return;
	//boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	//boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	//if (ptrNotifiedUser != NULL){
	//	boost::shared_ptr<string> name_ptr = ptrNotifiedUser->Name();
	//	if (name_ptr)
	//	{
	//		if (0 == name_ptr->compare(myName))
	//		{
	//			boost::shared_ptr<UserVariable> uck_ptr = ptrNotifiedUser->GetVariable("uck");//gia tri bat tat ga cua tung user
	//			if (uck_ptr)
	//			{
	//				int uck = *uck_ptr->GetIntValue();
	//				layerGa->setConfigChicken(uck);
	//				setUckToUserDefault(uck == LayerGa::CONFIG::ON ? true : false);
	//			}
	//		}
	//	}

	//	// 		boost::shared_ptr<UserVariable> amf = ptrNotifiedUser->GetVariable("amf");
	//	//
	//	// 		if (amf== NULL) return;
	//	// 		//string money = boost::to_string(*ptrNotifiedUser->GetVariable("amf")->GetDoubleValue());
	//	// 		int    money = (int)(*amf->GetDoubleValue());
	//	// 		double moneyDouble = (*amf->GetDoubleValue());
	//	// 		string name = boost::to_string(*ptrNotifiedUser->Name());
	//	//
	//	// 		log("OnSmartFoxUserVariableUpdate: name= %s, money= %d", name.c_str(), money);
	//	// 		int pos = layerAvatars->getPosByName(name.c_str());
	//	// 		if (pos == -1) return;
	//	// 		// 	layerAvatars->getUserByPos(pos)->setMoney(money);
	//	// 		// 	layerAvatars->getUserByPos(pos)->setMoney(moneyDouble);
	//	// 
	//	// 		//     arrName.push_back(name);
	//	// 		//     arrMoney.push_back(money);
	//	// 		// 	arrMoneyDouble.push_back(moneyDouble);
	//	// 		log("size: %d", arrName.size());
	//}
}

void LayerPlayGameBaCay::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
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

void LayerPlayGameBaCay::OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {

}

// // //Tìm chương - chia bai tim chuong (ba cay chuong) 
string LayerPlayGameBaCay::FindChuong(string& pListUsers) {
	// username1|id_card1;username1|id_card1
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

void LayerPlayGameBaCay::event_EXT_EVENT_START() {
	this->IsTurnUpAll = false;
	this->isStartedGame = true;
	this->LayerCards->ResetGame();
	this->LayerBet->GetLayerResult()->removeAllChildrenWithCleanup(true);
	this->LayerAvatars->UnReadyAllUser();
	this->mLayerCount->stopTimer();
	layerGa->showToast(false);

	this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_UNREADY)->setVisible(false);
}

void LayerPlayGameBaCay::event_EXT_EVENT_END() {
	this->LayerAvatars->StopAllTimer();
	if (this->LayerAvatars->IsSpect())
		this->SpecMode();
	if (!this->IsTurnUpAll)
	{
		this->LayerCards->ResetGame();
		this->LayerBet->GetLayerResult()->removeAllChildrenWithCleanup(true);
		this->mLayerCount->stopTimer();
	}
	this->FlagDatCuoc = false;
	this->FlagChiaBai = false;
	this->IsTurnUpAll = false;
	this->ListCards = "";
	layerGa->showToast(true);

	if (this->LayerAvatars->IsSpect()) {
		this->LayerCards->ResetGame();
		this->LayerBet->GetLayerResult()->removeAllChildrenWithCleanup(true);
		this->JoinGame();
	}
}

void LayerPlayGameBaCay::OnButtonClick(Ref *sender, ui::Widget::TouchEventType type) {
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
		case DTAG_BTN_TURNALL:
			this->TurnUpAll();
			break;
		case DTAG_BTN_VIEW:
			this->ViewCards();
			break;
		case DTAG_BTN_SQUEEZ:
			this->SqueezCards();
			break;
		case TAG_BTN_NHAN_BIEN_ALL:
			this->processBtnNhanBienAllTouched();
			break;
		case TAG_BTN_BIEN_ALL_X1:
			this->processBtnBienTatTouched();
			break;
		case TAG_BTN_BIEN_ALL_X2:
			this->processBtnBienTatx2Touched();
			break;
		}
	}
}

void LayerPlayGameBaCay::JoinGame() {
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

void LayerPlayGameBaCay::ActionReady() {
	//this->LayerCards->ResetGame();
	this->LayerBet->GetLayerResult()->removeAllChildrenWithCleanup(true);
	int money = Current_My_Bet == 0 ? this->mBetDefault : Current_My_Bet;
	if (mCurrGameID != kGameBaCayChuong)
		this->LayerBet->resetAllBets(money);
	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
	GameServer::getSingleton().Send(request);
	this->mLayerCount->stopTimer();
	this->LayerButton->getButtonByTag(DTAG_BTN_READY)->setVisible(false);

}

void LayerPlayGameBaCay::UnReady() {
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_UNREADY_REQ, params, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerPlayGameBaCay::ActionBet() {
	if (mCurrGameID == kGameBaCayChuong){
		log("valueBet= %d", this->layerBetBaCay->getValueBet());
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL) return;
		params->PutInt(EXT_FIELD_BET_TYPE, 3);
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
		GameServer::getSingleton().Send(request);
	}
}

void LayerPlayGameBaCay::TurnUpAll() {
	this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
	this->LayerAvatars->GetUserByPos(0)->stopTimer();
	if (this->ListCards.compare("") != 0){
		this->LayerCards->TurnUpAllCards(this->ListCards, kuser0);
	}

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TURNUP_CARD, params, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerPlayGameBaCay::ViewCards() {
	if (this->getChildByTag(TAG_NAN_BACAY_LAYER) != NULL)
		this->removeChildByTag(TAG_NAN_BACAY_LAYER);

	if (0 != this->ListCards.compare("")) {
		this->LayerCards->FlipEachCard(this->ListCards);
		this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
		this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
	}

	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom != NULL)
	{
		boost::shared_ptr<IRequest> request(new ExtensionRequest("atntf", parameter, lastRoom));//gui ban tin chau ria
		GameServer::getSingleton().Send(request);

	}
}

void LayerPlayGameBaCay::SqueezCards() {
	this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
	log("Listcard: %s", this->ListCards.c_str());
	Nan3Cay* m_NanBai = Nan3Cay::create();
	m_NanBai->SetCallbackFunc(this, callfuncN_selector(LayerPlayGameBaCay::CallFuncTurnUpAll));
	m_NanBai->InitListCardHand(ListCards);
	m_NanBai->setTag(TAG_NAN_BACAY_LAYER);
	this->addChild(m_NanBai);

	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom != NULL)
	{
		boost::shared_ptr<IRequest> request(new ExtensionRequest("atntf", parameter, lastRoom));//gui ban tin chau ria
		GameServer::getSingleton().Send(request);

	}
}

//
void LayerPlayGameBaCay::CallFuncTurnUpAll(Node *pSend) {
	if (0 != this->ListCards.compare("")) {
		this->LayerCards->TurnUpAllCards(this->ListCards, kuser0);
		this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
		this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
		if (mCurrGameID == kGameLieng)
			this->MoveButtonToRight();
	}
}

// Sau Khi chia bài xong, vào đây để hiển thị các Button và count down timer
void LayerPlayGameBaCay::CallFuncEndGive(Node *pSend) {
	this->FlagChiaBai = true;
	if (this->LayerAvatars->IsSpect()) {
		this->SpecMode();
	}
	else {
		//this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(true);
		//this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(true);
		if (mCurrGameID != kGameLieng){
			//this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(true);
			//this->LayerAvatars->RunAllTimer();
			//this->//mLayerCount->StartCountDownTurnUp();
		}
	}
}

// Nếu là khách
void LayerPlayGameBaCay::SpecMode() {
	// Không chat được
	//this->LayerButton->getButtonByTag(TAG_LB_BTN_CHAT)->setTouchEnabled(false);
	this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_UNREADY)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
}

void LayerPlayGameBaCay::CreateTimer() {
	this->mLayerCount = LayerTimer::create();
	this->addChild(mLayerCount);
}

void LayerPlayGameBaCay::OnSmartFoxSpectatorToPlayerError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {

}


void LayerPlayGameBaCay::OnSmartFoxSpectatorToPlayer(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	//
	if (ptrNotifiedUser->IsItMe() && !this->LayerAvatars->IsStartedGame()) {
		this->mLayerCount->startTimer();
		this->LayerButton->getButtonByTag(TAG_LB_BTN_CHAT)->setTouchEnabled(true);
	}
}

void LayerPlayGameBaCay::OnSmartFoxPlayerToSpectator(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	if (ptrNotifiedUser->IsItMe() && !this->LayerAvatars->IsStartedGame()) {
		this->LayerCards->ResetGame();
		this->LayerBet->GetLayerResult()->removeAllChildrenWithCleanup(true);
		this->LayerButton->getButtonByTag(TAG_LB_BTN_CHAT)->setTouchEnabled(false);
	}
}

void LayerPlayGameBaCay::updateEvent(float dt) {
	//neu la chu phong thi an ui danh bien
	/*boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	if (0 != this->FindChuong(this->ListUsers).compare(*uid)){
	this->hideUIDanhBien();
	}*/
	if (this->mThreadQueueMsg->isQueueEmpty())
		return;
	if (this->LayerCards->isRunningAction())
		return;
	this->mThreadQueueMsg->run();
}

void LayerPlayGameBaCay::event_EXT_EVENT_ERROR_READY_NTF() {
	boost::shared_ptr<long> errc = param->GetInt(EXT_FIELD_ERROR_CODE);
	if (errc != NULL) {
		if (*errc == 30) {
			Chat *toast = Chat::create(dataManager.GetSysString(57), -1);
			this->addChild(toast);
		}
	}
}

void LayerPlayGameBaCay::event_EXT_EVENT_UNREADY_NTF() {
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	if (uid != NULL) {
		int _pos = this->LayerAvatars->GetPosByName(*uid);

		if (_pos == -1) return;

		//this->LayerAvatars->GetUserByPos(_pos)->setReady(false);

		if (0 == uid->compare(this->MyAI)) {
			this->FlagDatCuoc = false;
			this->GetButtonByTag(DTAG_BTN_UNREADY)->setVisible(false);
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
			this->mLayerCount->startTimer();
			if (0 == strcmp(this->GetButtonByTag(kTagButtonReady)->getTitleText().c_str(), dataManager.GetSysString(197).c_str())) {
				this->mLayerCount->stopTimer();
			}
			else
			{

			}

		}
	}
}

void LayerPlayGameBaCay::event_EXT_EVENT_READY_NTF() {
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	if (uid != NULL) {
		int _pos = this->LayerAvatars->GetPosByName(*uid);
		if (_pos == -1) return;
		//this->LayerAvatars->GetUserByPos(_pos)->setReady(true);
		log("ready ntf");
		if (0 == uid->compare(this->MyAI)) {
			this->FlagDatCuoc = true;
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
			this->GetButtonByTag(DTAG_BTN_UNREADY)->setVisible(true);
			this->mLayerCount->stopTimer();
		}
	}
}
void LayerPlayGameBaCay::event_EXT_EVENT_LIST_USER_UPDATE_3CAYNORMAL()
{
	boost::shared_ptr<string> lu = param->GetUtfString(EXT_FIELD_LIST_USER);
	if (lu != NULL) {
		this->ListUsers = *lu;
		log("list user Ba Cay Thuong: %s", lu->c_str());
		this->PlaySound("sounds/game/ring_ring.mp3");

		vector<string> arrUser = mUtils::splitString(ListUsers, ';');
		this->SizeListUser = arrUser.size();


		bool isPlaying = this->LayerAvatars->IsPlaying(ListUsers);
		log("%s", isPlaying ? "Đang chơi" : "Chưa chơi");
		if (isPlaying && !this->LayerAvatars->IsSpect())
			return;

		this->LayerCards->SetMyName(this->MyAI);
		this->LayerCards->setListUser(*lu);
		this->LayerAvatars->SetListUser(this->ListUsers);
		//isStartedGame = LayerAvatars->IsStartedGame();
		if (this->SizeListUser == 0) return;

		if (isPlaying)
			this->FlagChiaBai = true;
		else
		if (this->SizeListUser > 1){
			this->ResetGame();
			this->mLayerCount->startTimer();
		}

		if (this->SizeListUser == 1){
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
			this->ResetGame();
			this->mLayerCount->stopTimer();
		}

		vector<string> first = mUtils::splitString(arrUser[0], '|');
		if (first.size() == 0) return;
		if (this->SizeListUser == 1){
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
		}
		if (0 == this->MyAI.compare(first[0])) {
			//this->GetButtonByTag(DTAG_BTN_READY)->setTitleText(dataManager.GetSysString(197));
			//this->mLayerCount->stopTimer();
		}
		//	this->GetButtonByTag(DTAG_BTN_READY)->setTitleText(dataManager.GetSysString(197));

		for (int i = 0; i < arrUser.size(); i++) {
			vector<string> info = mUtils::splitString(arrUser[i], '|');
			if (0 == this->MyAI.compare(info[0])) {
				if (0 == info[1].compare("1") && 0 == LayerCards->getListCards_0().size()) {
					this->LayerCards->CreateCardBack(kuser0);
					auto avatar = this->LayerAvatars->GetUserByPos(kuser0);
					avatar->startTimer();
				}
			}

			else {
				int Pos = this->LayerAvatars->GetPosByName(info[0]);

				switch (Pos) {
				case kuser0:
					if (this->LayerAvatars->IsSpect()) {
						if (0 == info[1].compare("1") && 0 == LayerCards->getListCards_0().size())
							this->LayerCards->CreateCardBack(kuser0);
						auto avatar = this->LayerAvatars->GetUserByPos(kuser0);
						avatar->startTimer();
					}
					break;

				case kuser1:
					if (0 == info[1].compare("1") && 0 == LayerCards->getListCards_1().size()){
						this->LayerCards->CreateCardBack(kuser1);
						auto avatar = this->LayerAvatars->GetUserByPos(kuser1);
						avatar->startTimer();
					}

					break;

				case kuser2:
					if (0 == info[1].compare("1") && 0 == LayerCards->getListCards_2().size()){
						this->LayerCards->CreateCardBack(kuser2);
						auto avatar = this->LayerAvatars->GetUserByPos(kuser2);
						avatar->startTimer();
					}
					break;

				case kuser3:
					if (0 == info[1].compare("1") && 0 == LayerCards->getListCards_3().size()){
						this->LayerCards->CreateCardBack(kuser3);
						auto avatar = this->LayerAvatars->GetUserByPos(kuser3);
						avatar->startTimer();
					}
					break;

				case kuser4:
					if (0 == info[1].compare("1") && 0 == LayerCards->getListCards_4().size()){
						this->LayerCards->CreateCardBack(kuser4);
						auto avatar = this->LayerAvatars->GetUserByPos(kuser4);
						avatar->startTimer();
					}
					break;

				case kuser5:
					if (0 == info[1].compare("1") && 0 == LayerCards->getListCards_5().size()){
						this->LayerCards->CreateCardBack(kuser5);
						auto avatar = this->LayerAvatars->GetUserByPos(kuser5);
						avatar->startTimer();
					}
					break;

				case kuser6:
					if (0 == info[1].compare("1") && 0 == LayerCards->getListCards_6().size()){
						this->LayerCards->CreateCardBack(kuser6);
						auto avatar = this->LayerAvatars->GetUserByPos(kuser6);
						avatar->startTimer();
					}
					break;

				default:
					break;
				}
			}
		}
		//! nếu là khách
		if (this->LayerAvatars->IsSpect()) {
			//nameGame->setString("Bạn đang xem...");
			SpecMode();
			mLayerCount->stopTimer();
			//LayerButton->SetIsGuess(true);
		}
		else {
			LayerButton->SetIsGuess(false);
			//nameGame->setString(StringResult);
			log("game::%s", StringResult.c_str());
			if (arrUser.size() > 1 && !isPlaying) {
				vector<string> arrUser = mUtils::splitString(ListUsers, ';');
				for (int i = 0; i < arrUser.size(); i++) {
					vector<string>info = mUtils::splitString(arrUser[i], '|');
					if (0 == info.size()) continue;
					if (0 == MyAI.compare(info[0])) {
						if (0 == info[2].compare("1") || 0 == i) {
							//this->mLayerCount->stopTimer();
						}
					}
				}
			}

			if (isPlaying) {
				GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
				GetButtonByTag(DTAG_BTN_UNREADY)->setVisible(false);
				if (!IsTurnUpAll) {
					// Hiện các Button: Lật bài, Xem bài và nặn bài
					//					GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(true);
					//					GetButtonByTag(DTAG_BTN_VIEW)->setVisible(true);
					//					GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(true);
					mLayerCount->stopTimer();
					for (int i = 0; i < arrUser.size(); i++){
						vector<string> first = mUtils::splitString(arrUser[i], '|');
						auto pos = this->LayerAvatars->GetPosByName(first[0]);
						auto avatar = this->LayerAvatars->GetUserByPos(pos);
						if (avatar == NULL) return;
						avatar->startTimer();
					}
				}
				else {
					GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
					GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
					GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
					LayerCards->ResetGame();
					LayerBet->GetLayerResult()->removeAllChildrenWithCleanup(true);
				}
				mLayerCount->stopTimer();

			}
			else {
				GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
				GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
				GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
				//mLayerCount->stopTimer();
				GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
				if (this->SizeListUser == 1){
					this->ResetGame();
					this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
				}
			}
		}
	}
	this->LayerBet->VisibleAllFrameBet();
}


void LayerPlayGameBaCay::event_EXT_EVENT_LIST_USER_UPDATE_3CAYCHUONG() {
	//3caythuong: p.getUser().getName() | p.m_isPlaying | p.m_isReady
	//3caychuong: p.getUser().getName() | p.m_isLeader  | p.m_nBet      | p.m_isPlaying | p.m_isReady
	//Lieng     : p.getUser().getName() | p.m_nBet      | p.m_isPlaying | p.m_isReady
	//this->LayerAvatars->UnReadyAllUser();
	boost::shared_ptr<string> lu = param->GetUtfString(EXT_FIELD_LIST_USER);
	if (lu != NULL) {

		log("list user Ba Cay Chuong: %s", lu->c_str());
		this->ListUsers = *lu;//"test3caychuong3|0|0|0|1;test3caychuong1|0|0|0|1;test3caychuong2|0|0|0|1;test3caychuong4|0|0|0|1;test3caychuong5|0|0|0|1;test3caychuong6|0|0|0|1;test3caychuong7|0|0|0|1";

		int _size = mUtils::splitString(this->ListUsers, ';').size();
		if (_size >= 4){
			this->LayerButton->processDisplayButtonInvite(false);
		}
		else{
			if (this->vipRoom == 1){
				this->LayerButton->processDisplayButtonInvite(true);
			}
		}

		std::string m_IdChuong = this->FindChuong(this->ListUsers);
		this->uidChuong = m_IdChuong;

		//test
		//this->ListUsers += ";test3caychuong3|1|0|0|;test3caychuong1|0|0|0|1;test3caychuong2|0|0|0|1;test3caychuong4|0|0|0|1;test3caychuong5|0|0|0|1";
		vector<string> arrPlayes = mUtils::splitString(this->ListUsers, ';');
		this->SizeListUser = arrPlayes.size();

		//reset image Chuong- duyet list user: neu chuong khong con trong ban thi reset 
		bool isBossExist = false;
		for (int i = 0; i < arrPlayes.size(); ++i){
			std::string username = mUtils::splitString(arrPlayes[i], '|')[0];
			if (username.compare(this->uidChuong) == 0){
				isBossExist = true;
			}
		}
		if (isBossExist == false){
			//auto name = mUtils::splitString(arrPlayes[0], '|')[0];
			this->LayerAvatars->SetPosChuong(100);
		}

		bool isPlaying = LayerAvatars->IsStartedGame();
		//this->FlagChiaBai = isPlaying;
		log("%s", isPlaying ? "Đang chơi" : "Chưa chơi");
		if (isPlaying && !this->LayerAvatars->IsSpect())
			return;

		this->LayerAvatars->SetListUser(this->ListUsers);
		this->LayerCards->setListUser(this->ListUsers);
		this->LayerCards->setPlayerMap(LayerAvatars->GetMapPlayer());
		this->bien3Cay->setListUser(this->ListUsers);

		//isStartedGame = LayerAvatars->IsStartedGame();
		this->IsSpector = this->LayerAvatars->IsSpect();

		this->LayerAvatars->SetPosChuong(this->LayerAvatars->GetPosByName(m_IdChuong));
		if (this->SizeListUser == 0) return;

		if (isPlaying)
			this->FlagChiaBai = true;
		else if (this->SizeListUser > 1 && this->isStartedGame == false){
			this->ResetGame();
			this->mLayerCount->startTimer();
		}

		if (this->SizeListUser == 1){
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
			this->ResetGame();
			this->mLayerCount->stopTimer();
		}
		else if (this->isStartedGame == false){
			//chua chia bai tim chuong - hien thi button "Start"
			if (this->uidChuong.compare("") == 0){
				this->GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
			}
			else{
				if (!isPlaying) {
					if (0 == MyAI.compare(m_IdChuong)) {
						this->GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
						this->GetButtonByTag(DTAG_BTN_UNREADY)->setVisible(false);
					}
					else {
						this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
					}
				}
				else{

				}
			}
		}

		//Hiển thị các FrameBet, LayerBien của người chơi
		this->LayerBet->VisibleAllFrameBet();

		for (unsigned int i = 0; i < arrPlayes.size(); ++i) {
			vector<string> info = mUtils::splitString(arrPlayes[i], '|');
			if (info.size() == 0) continue;
			int pos = this->LayerAvatars->GetPosByName(info[0]);
			//Show bet
			string _bet = mUtils::convertMoneyEx(atoi(info[2].c_str()));
			this->LayerBet->GetFrameBetByPos(pos)->SetValueBet(_bet);
			this->LayerBet->GetFrameBetByPos(pos)->setVisible(true);
			if (info[0].compare(m_IdChuong) == 0)
				this->LayerBet->GetFrameBetByPos(pos)->setVisible(false);
		}

		for (unsigned int i = 0; i < arrPlayes.size(); ++i) {
			vector<string> info = mUtils::splitString(arrPlayes[i], '|');
			if (info.size() == 0) continue;
			if (0 == this->MyAI.compare(info[0])) {
				if (0 == info[3].compare("1") && 0 == this->LayerCards->getListCards_0().size()) {
					log("Me Rejoin");
					this->LayerCards->CreateCardBack(kuser0);
				}
			}
			else {
				int pos = this->LayerAvatars->GetPosByName(info[0]);
				if (0 == info[3].compare("1") && 0 == this->LayerCards->getListCardsByPos(pos).size())
					this->LayerCards->CreateCardBack(pos);
				if (pos == kuser0 && this->IsSpector) {
					if (0 == info[3].compare("1") && 0 == this->LayerCards->getListCards_0().size())
						this->LayerCards->CreateCardBack(kuser0);
				}
			}
		}

		//! nếu là khách
		if (this->IsSpector) {
			//this->//nameGame->setString("Bạn đang xem...");
			this->SpecMode();
			this->layerBetBaCay->setVisible(false);
			this->mLayerCount->stopTimer();
			this->LayerButton->SetIsGuess(true);
		}
		//! Neu la player
		else {
			this->LayerButton->SetIsGuess(false);
			//this->//nameGame->setString(StringResult);
			if (arrPlayes.size() > 1 && !isPlaying) {
				for (int i = 0; i < arrPlayes.size(); ++i) {
					vector<string>info = mUtils::splitString(arrPlayes[i], '|');
					if (0 == this->MyAI.compare(info[0])) {
						if (0 == info[4].compare("1") && 0 == info[1].compare("0")) {
							//this->mLayerCount->startTimer();
						}
					}
				}
			}

			if (isPlaying) {
				//! Ẩn Các Button Sẵn Sàng, Hủy Sẵn Sàng, Đặt Cược
				this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
				this->GetButtonByTag(DTAG_BTN_UNREADY)->setVisible(false);

				if (!this->IsTurnUpAll) {
					//! Hiện các Button: Lật bài, Xem bài và nặn bài
					//					this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(true);
					//					this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(true);
					//					this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(true);
					//this->mLayerCount->startTimer();
				}
				else {
					this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
					this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
					this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
					this->LayerBet->GetLayerResult()->removeAllChildrenWithCleanup(true);
				}
				this->mLayerCount->stopTimer();
                //this->LayerAvatars->StopAllTimer();
			}
			else {
				/*this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
				this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
				this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);*/
			}
		}//!else kết thúc ktra người chơi
		//hide layerBet
		for (int i = 0; i < arrPlayes.size(); ++i)
		{
			std::string username = mUtils::splitString(arrPlayes[i], '|')[0];
			FrameBet* userFrameBet = this->LayerBet->GetFrameBetByPos(this->LayerAvatars->GetPosByName(username));
			userFrameBet->setVisible(false);
		}

		//test
		//this->LayerCards->GivePocker();
		/*for (int i = 0; i < 7; ++i)
			this->LayerAvatars->setPosBien(i, "10K");

			if (this->LayerAvatars->getChildByTag(kuser0 + 100)){
			this->LayerAvatars->removeChildByTag(kuser0 + 100);
			}*/
		//this->bien3Cay->displayBien3cay(true);
		/*LayerBien *bienUser1 = (LayerBien*)this->bien3Cay->getChildByTag(this->LayerAvatars->GetPosByName("test3caychuong3"));
		bienUser1->buttonBienx1LoadTexture();*/
	}
}
void LayerPlayGameBaCay::event_EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY() {
	log("event_EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY");
	boost::shared_ptr<long> chks = param->GetInt("chks");
	if (chks != NULL){
		layerGa->setConfigChicken(*(chks.get()));
	}
}

void LayerPlayGameBaCay::event_EXT_EVENT_LISTCARD_NTF() {
	if (param == NULL) return;

	if (mCurrGameID == kGameBaCayChuong){
		this->hideUIDanhBien();
		this->bien3Cay->displayBien3cay(false);
		if (!this->FlagChiaBai) {
			float posX = WIDTH_DESIGN / 2 - WIDTH_DESIGN / 7 + W_MONEY_BG / 2 + 45;
			float posY = HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 4 - H_MONEY_BG / 2 + 35;
			this->LayerCards->setPositionCardMe(posX, posY);
			this->LayerCards->GivePocker();
		}
	}

	bool isPlaying = LayerAvatars->IsStartedGame();
	if (isPlaying)
		this->FlagChiaBai = true;
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> lc = this->param->GetUtfString(EXT_FIELD_LIST_CARD);

	if (uid != NULL && lc != NULL) {
		if (0 == uid->compare(this->MyAI)) {
			this->ListCards = *lc;
		}

		if (this->FlagChiaBai) {
			log("uid turn up cards = %s lc = %s", uid->c_str(), lc->c_str());
			//else, 3 cay chuong + 3 cay thuong process
			boost::shared_ptr<bool> tua = this->param->GetBool(EXT_FIELD_IS_TURN_UP_ALL);
			if (0 == uid->compare(this->MyAI) && tua != NULL) {
				if (*tua) {
					this->IsTurnUpAll = true;
					auto Pos = this->LayerAvatars->GetPosByName(*uid);
					auto _avatar = this->LayerAvatars->GetUserByPos(Pos);
					_avatar->stopTimer();
					//this->//mLayerCount->StopCountDownTurnUp();

					if (this->getChildByTag(TAG_NAN_BACAY_LAYER) != NULL)
						this->removeChildByTag(TAG_NAN_BACAY_LAYER);
					this->LayerCards->TurnUpAllCards(*lc, kuser0);
					this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
					this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
					this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
				}
				else {
					//this->LayerCards->TurnUpAllCards("0_0-0_0-0_0", kuser0);
					this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(true);
					this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(true);
					this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(true);
					this->LayerAvatars->RunAllTimer();
				}

			}
			else {
				if (*tua) {
					auto Posa = this->LayerAvatars->GetPosByName(*uid);
					auto _avatar = this->LayerAvatars->GetUserByPos(Posa);
					if (_avatar == NULL) return;
					_avatar->stopTimer();
					if (*tua) {
						int Pos = this->LayerCards->GetPosUserByName(*uid);
						this->LayerCards->TurnUpAllCards(*lc, Pos);
					}
					else {
						int Pos = this->LayerCards->GetPosUserByName(*uid);
						this->LayerCards->TurnUpAllCards(*lc, Pos);
					}
				}
			}
		}
	}
}
void LayerPlayGameBaCay::event_EXT_EVENT_AMF_TEST_NOTIF(){
	//    // sự kiện công/trừ tiền
	//    var uid = params.uid;
	//    var money = params.amf;
	log("event_EXT_EVENT_AMF_TEST_NOTIF");
	boost::shared_ptr<string> name = param->GetUtfString("uid");
	boost::shared_ptr<double> money_ptr;
	//if (SceneManager::getSingleton().getCurrRoomType() == 0){

	money_ptr = param->GetDouble("amf");
	//}
	boost::shared_ptr<double> betvl = param->GetDouble("betvl");
	boost::shared_ptr<long> cbt = param->GetInt("cbt");
	//int money = (int)(*(param->GetDouble("amf")));
	//int betvl = (int)(*(param->GetDouble("betvl")));

	int resson = -1;
	long money = 0;
	// 	if ((param->GetInt("cbt")) != NULL){
	// 		resson = (int)(*(param->GetInt("cbt")));
	// 		log("resson = %d", resson);
	// 	}

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
		//layerNumbers->showNumberByPos(pos, res);
		auto avatar = LayerAvatars->GetUserByPos(pos);
		if (avatar)
			avatar->showNumber(res);
		if (resson < 0) return;
		string ressonString = "";
		switch (resson) {
		case 1:
			if (money < 0)
				ressonString = "Thối 2";
			break;
		case 2:
			ressonString = "Ăn thối 2";
			break;
		case 3:
			ressonString = "Chặn được 2";
			break;
		case 4:
			ressonString = "Bị bắt 2";
			break;
		case 5:
			ressonString = "Chặn lại 2";
			break;
		case 6:
			if (money < 0)
				ressonString = "Đền làng";
			break;
		case 7:
			ressonString = "Đền báo";
			break;
		case 8:// tru tien ghop ga
                ressonString = dataManager.GetSysString(479);
			//this->LayerChat->ShowNumberByPos(pos, res);
			break;
		case 9:// cong tien khi an ga
			ressonString = "Ăn gà";
			//this->LayerChat->ShowNumberByPos(pos, res);
			break;
		case 10:
			ressonString = "Tới trắng";
			break;
		case 11:
			ressonString = "Thua cóng";
			break;
		case 12:
			ressonString = "Đứt mù";
			break;
		case 13:
			ressonString = "Thắng cóng";
			break;
		case 14:
			ressonString = "Bỏ cuộc";//khi xin ra
			break;
		case 15:
			ressonString = "Thối tứ quý";
			break;
		case 16:
			ressonString = "Thối 3 đôi thông";
			break;
		case 17:
			ressonString = "Thối 4 đôi thông";
			break;
		case 18:
			ressonString = "Ăn thối tứ quý";
			break;
		case 19:
			ressonString = "Ăn thối 3 thông";
			break;
		case 20:
			ressonString = "Ăn thối 4 thông";
			break;
		}
		log("resson=%d, ressonString=%s", resson, ressonString.c_str());
		if (resson == 8){
			auto _avatar = LayerAvatars->GetUserByPos(pos);
			if (_avatar != NULL)
				_avatar->showChat(ressonString);
			log("gop ga");
		}
		else if (resson == 14){
			this->LayerAvatars->setResult(pos, resson, kGameBaCay, false, money);
		}
		else if (resson == 9) // an ga, thi hien thi tien bay
		{
			auto userAva = LayerAvatars->GetUserByPos(pos);
			if (userAva != NULL)
			{
				Vec2 position = userAva->getPosition();
				layerGa->runActionAnGa(position, 1);
			}

		}
	}
}
void LayerPlayGameBaCay::event_EXT_EVENT_GAME_RESULT() {
	this->isStartedGame = false;
	boost::shared_ptr<string> rg = param->GetUtfString(EXT_FIELD_GAME_RESULT);
	if (rg != NULL) {
		log("resuilt game: %s", rg->c_str()); // co result thi an buttons action luon
		this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
		this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
		this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
		//thanhhv3|3|3|2|1000;dautv3|2|1|1|-1000

		if (this->getChildByTag(TAG_NAN_BACAY_LAYER) != NULL)
			this->removeChildByTag(TAG_NAN_BACAY_LAYER);

		if (!this->IsTurnUpAll && !this->IsSpector) {
			this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
			this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
			this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
			this->LayerCards->ResetGame();
			this->IsTurnUpAll = true;
			return;
		}

		// 		Number_Chat_inGame7u *layerNumbers = Number_Chat_inGame7u::create();
		// 		this->addChild(layerNumbers);

		vector<string> _result = mUtils::splitString(*rg, ';');
		std::string varStr = "1";
		//tên user | điểm | giá trị quân bài to nhất| giá trị chất to nhất| số tiền cộng hoặc trừ | loại quân bài
		for (int i = 0; i < _result.size(); ++i) {
			vector<string> info = mUtils::splitString(_result[i], '|');

			if (info.size() == 0) continue;

			string strScore = atoi(info[4].c_str()) > 0 ? ("+" + info[4]) : info[4];

			string strResuilt = info[1] + "|" + info[2] + "|" + info[3] + "|" + info[5];
			auto pos = this->LayerAvatars->GetPosByName(info[0]);
			auto avatar = this->LayerAvatars->GetUserByPos(pos);
			if (avatar)
			{
				avatar->stopTimer();
				string result = info[2] + info[3];
				if (atoi(info[4].c_str()) > 0){
					if (0 != this->uidChuong.compare(info[0]))
						avatar->setResult(152, kGameBaCay, true, 100);
				}

				if (info[1].compare("10") == 0){
					if (0 == info[5].compare("1")){
						avatar->setResult(102, kGameBaCay, true, 100);
					}
					else if (result.compare("12") == 0)
						avatar->setResult(101, kGameBaCay, true, 100);
					else{
						if (atoi(info[4].c_str()) > 0)
							avatar->setResult(0, kGameBaCay, true, 100);
						else
							avatar->setResult(1, kGameBaCay, true, 100);
					}
					avatar->SetResult4AllUser(varStr, strResuilt);
				}
				else{
					if (0 == info[5].compare("3")){
						avatar->setResult(103, kGameBaCay, true, 100);
					}
					else if (atoi(info[4].c_str()) > 0)
						avatar->setResult(0, kGameBaCay, true, 100);
					else
						avatar->setResult(1, kGameBaCay, true, 100);
					avatar->SetResult4AllUser(varStr, strResuilt);
				}
			}
		}
	}
}

void LayerPlayGameBaCay::event_EXT_EVENT_GAME_BET_NTF() {
	//gbv":1000,"uid":"thanhhv3

	/*if (mCurrGameID == kGameBaCayChuong){
		this->hideUIDanhBien();
		this->bien3Cay->displayBien3cay(false);
		if (!this->FlagChiaBai) {
		float posX = WIDTH_DESIGN / 2 - WIDTH_DESIGN / 7 + W_MONEY_BG / 2 + 45;
		float posY = HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 4 - H_MONEY_BG / 2 + 35;
		this->LayerCards->setPositionCardMe(posX, posY);
		this->LayerCards->GivePocker();
		}
		}*/

	int _size = mUtils::splitString(this->ListUsers, ';').size();
	if (_size <= 2){
		this->hideLoadingBet();
	}
	boost::shared_ptr<long> gbv = param->GetInt(EXT_FIELD_GAME_BET_VALUE);
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	//3 cay chuong
	if (gbv != NULL && uid != NULL) {

		int pos = this->LayerAvatars->GetPosByName(*uid);
		string _gbv = mUtils::convertMoneyEx(*gbv);
		log("gbv %s", _gbv.c_str());
		this->LayerBet->GetFrameBetByPos(pos)->SetValueBet(_gbv);
		this->LayerBet->GetFrameBetByPos(pos)->setVisible(true);
		if (uid->compare(MyAI) == 0)
			this->Current_My_Bet = *gbv;
	}
}


void LayerPlayGameBaCay::PlaySound(const string& soundPath)
{
	if (mUtils::isSoundOn())
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundPath.c_str());
}

//////////////////////////////////////////////////////////////////////////
//Functions 4 Lieng
//////////////////////////////////////////////////////////////////////////
void LayerPlayGameBaCay::UserRejoinGame(string& listplayers, int isdeal)
{
	if (0 == listplayers.compare(""))
		return;

	this->mLayerCount->stopTimer();
	this->LayerAvatars->SetListUser(listplayers);
	bool m_IsPlaying = this->LayerAvatars->IsPlaying(listplayers);
	this->LayerCards->setListUser(listplayers);
	if (m_IsPlaying && 0 == this->LayerCards->getListCards_0().size()) {
		//this->MoveButtonToRight();
	}
	if (mCurrGameID == kGameBaCay){
		vector<string> list = mUtils::splitString(listplayers, ';');
		for (unsigned short i = 0; i < list.size(); i++) {
			vector<string> info = mUtils::splitString(list[i], '|');
			if (0 == this->MyAI.compare(info[0])) {
				auto pos = this->LayerAvatars->GetPosByName(info[0]);
				auto avatar = this->LayerAvatars->GetUserByPos(pos);
				if (avatar != NULL){

					if (info[1].compare("0") != 0)
						avatar->startTimer();
					else
						avatar->stopTimer();
				}

				if (m_IsPlaying) {
					this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
                    if (isdeal == 1)
                        this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(true);
				}
				if (3 != LayerCards->getListCards_0().size() && 0 != info[2].compare("0") && isdeal == 1) {
					this->LayerCards->CreateCardBack(kuser0);
				}
                if (isdeal == 1)
                    this->FlagChiaBai = true;
			}
			else {

				auto pos = this->LayerAvatars->GetPosByName(info[0]);


				auto avatar = this->LayerAvatars->GetUserByPos(pos);
				if (avatar != NULL){

					if (info[1].compare("1") == 0)
						avatar->startTimer();
					else
						avatar->stopTimer();
				}

                if (isdeal == 0) return;
                
				this->FlagChiaBai = true;

				switch (pos) {
				case kuser0:
					if (0 != info[1].compare("0")) {
						this->LayerCards->CreateCardBack(kuser0);
					}
					break;
				case kuser1:
					if (0 != info[1].compare("0")) {
						this->LayerCards->CreateCardBack(kuser1);
					}
					break;
				case kuser2:
					if (0 != info[1].compare("0")) {
						this->LayerCards->CreateCardBack(kuser2);
					}
					break;
				case kuser3:
					if (0 != info[1].compare("0")) {
						this->LayerCards->CreateCardBack(kuser3);
					}
					break;
				case kuser4:
					if (0 != info[1].compare("0")) {
						this->LayerCards->CreateCardBack(kuser4);
					}
					break;
				case kuser5:
					if (0 != info[1].compare("0")) {
						this->LayerCards->CreateCardBack(kuser5);
					}
					break;
				case kuser6:
					if (0 != info[1].compare("0")) {
						this->LayerCards->CreateCardBack(kuser6);
					}
					break;
				}
			}
		}

	}
	else if (mCurrGameID == kGameBaCayChuong){
		/*
		hehe_keke|0|0_0-0_0-0_0|0|50;vodanh9x|1|8_1-5_1-3_1|0|0;
		String.format("%s|%d|%s|%d|%d", user.getName(), isBoss ? 1 : 0
		, toStateCardString(turn), this.getTotalEnrollBet(), this.getCurrentbet());
		*/
		log("rejoin game %s", listplayers.c_str());
		vector<string> list = mUtils::splitString(listplayers, ';');
		for (unsigned short i = 0; i < list.size(); i++) {
			vector<string> info = mUtils::splitString(list[i], '|');

			std::string name = info[0];
			std::string _boss = info[1];
			double betValue = atoi(info[4].c_str());
			if (_boss == "1"){
				this->uidChuong = name;
				this->LayerAvatars->SetPosChuong(this->LayerAvatars->GetPosByName(name));
			}
			else{

			}
			//neu la chinh minh
			if (0 == this->MyAI.compare(name)) {
				auto pos = this->LayerAvatars->GetPosByName(name);
				auto avatar = this->LayerAvatars->GetUserByPos(pos);
				//avatar->setBetValue(betValue);
				/*if (avatar != NULL){
					if (info[2].compare("0") != 0)
					avatar->startTimer();
					else
					avatar->stopTimer();
					}*/
				if (m_IsPlaying) {
					avatar->startTimer();
					this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
					this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(true);
				}
				else{
					avatar->stopTimer();
					if (_boss == "0"){
						this->LayerButton->getButtonByTag(DTAG_BTN_READY)->setVisible(false);
					}
				}
				//if (3 != LayerCards->getListCards_0().size() && 0 != info[4].compare("0")) {
				if (3 != LayerCards->getListCards_0().size()) {
					this->LayerCards->CreateCardBack(kuser0);
				}
				this->FlagChiaBai = true;
			}
			else {
				auto pos = this->LayerAvatars->GetPosByName(name);
				auto avatar = this->LayerAvatars->GetUserByPos(pos);
				//avatar->setBetValue(betValue);
				//if (avatar != NULL){
				//	if (info[2].compare("1") == 0)
				//		avatar->startTimer();
				//	else
				//		avatar->stopTimer();
				//}

				if (m_IsPlaying){
					avatar->startTimer();
				}
				else{
					avatar->stopTimer();
					if (_boss == "0"){
						this->LayerButton->getButtonByTag(DTAG_BTN_READY)->setVisible(false);
					}
				}

				this->FlagChiaBai = true;

				switch (pos) {
				case kuser0:
					//if (0 != info[4].compare("0")) {
					this->LayerCards->CreateCardBack(kuser0);
					//}
					break;
				case kuser1:
					//if (0 != info[4].compare("0")) {
					this->LayerCards->CreateCardBack(kuser1);
					//}
					break;
				case kuser2:
					//if (0 != info[4].compare("0")) {
					this->LayerCards->CreateCardBack(kuser2);
					//}
					break;
				case kuser3:
					//if (0 != info[4].compare("0")) {
					this->LayerCards->CreateCardBack(kuser3);
					//}
					break;
				case kuser4:
					//if (0 != info[4].compare("0")) {
					this->LayerCards->CreateCardBack(kuser4);
					//}
					break;
				case kuser5:
					//if (0 != info[4].compare("0")) {
					this->LayerCards->CreateCardBack(kuser5);
					//}
					break;
				case kuser6:
					//if (0 != info[4].compare("0")) {
					this->LayerCards->CreateCardBack(kuser6);
					//}
					break;
				}
			}
            
		}

	}
}

void LayerPlayGameBaCay::BetGame(const string& uid, string& betal)
{
	this->LayerAvatars->StopAllTimer();

	if (0 == this->MyAI.compare(uid)) {

		//!
		this->LayerAvatars->GetUserByPos(kuser0)->startTimer();
		this->GetButtonByTag(DTAG_BTN_FOLLOW)->setVisible(false);

		std::vector<std::string> typeBet = mUtils::splitString(betal, ',');
		for (int i = 0; i < typeBet.size(); ++i) {
			if (0 == typeBet[i].compare("1")) {
				/*this->GetButtonByTag(DTAG_BTN_BET)->setVisible(true);
				this->GetButtonByTag(DTAG_BTN_BET)->setTouchEnabled(true);*/
			}
			else if (0 == typeBet[i].compare("2")) {
				this->GetButtonByTag(DTAG_BTN_FOLD)->setVisible(true);
				this->GetButtonByTag(DTAG_BTN_FOLD)->setTouchEnabled(true);
			}
			else if (0 == typeBet[i].compare("3")) {
				this->GetButtonByTag(DTAG_BTN_FOLLOW)->setVisible(true);
				this->GetButtonByTag(DTAG_BTN_FOLLOW)->setTouchEnabled(true);
			}
		}
	}

	else {
		if (this->getChildByTag(TAG_BET_LIENG_LAYER) != NULL) {
			this->removeChildByTag(TAG_BET_LIENG_LAYER);
		}
		this->GetButtonByTag(DTAG_BTN_FOLD)->setVisible(false);
		this->GetButtonByTag(DTAG_BTN_FOLLOW)->setVisible(false);

		//!Count Down
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

void LayerPlayGameBaCay::MoveButtonToRight()
{
	//Size convert
	Size _size = ChanUtils::getSizePos();
	int w_Button = 129;
	int h_Button = 44;
	this->GetButtonByTag(DTAG_BTN_FOLLOW)->setPosition(Vec2(WIDTH_DESIGN - w_Button + _size.width, h_Button + 20 - _size.height));
	this->GetButtonByTag(DTAG_BTN_FOLD)->setPosition(Vec2(WIDTH_DESIGN - w_Button - 110 + _size.width, 10 - _size.height));
}

void LayerPlayGameBaCay::ResetButtonLocation()
{
	//Size convert
	Size _size = ChanUtils::getSizePos();
	int w_Button = 129;
	int h_Button = 44;
	this->GetButtonByTag(DTAG_BTN_FOLLOW)->setPosition(Vec2(WIDTH_DESIGN - w_Button - 105 + _size.width, h_Button + 20 - _size.height));
	this->GetButtonByTag(DTAG_BTN_FOLD)->setPosition(Vec2(WIDTH_DESIGN - w_Button - 200 + _size.width, 15 - _size.height));
}

void LayerPlayGameBaCay::event_EXT_EVENT_NEXT_TURN()
{
	boost::shared_ptr<string> uid = param->GetUtfString("uid");
	boost::shared_ptr<long> mb = param->GetInt("mb");
	boost::shared_ptr<string> betal = param->GetUtfString("betal");

	if (uid != NULL)
		CurrentPlayerBet = *uid;
	if (betal != NULL)
		CurrentBetal = *betal;
	if (mb != NULL)
		MinBetValue = (int)*mb;
	log("uid: %s, Curentbeal: %s, MinbetValue: %d", CurrentPlayerBet.c_str(), CurrentBetal.c_str(), MinBetValue);
	BetGame(CurrentPlayerBet, CurrentBetal);
	this->mReal = true;
}

std::string LayerPlayGameBaCay::FindBossRoom(string listUser)
{
	vector<string> arrUser = mUtils::splitString(listUser, ';');
	if (arrUser.size() == 0) return "";
	vector<string> info = mUtils::splitString(arrUser[0], '|');
	return info[0];
}

void LayerPlayGameBaCay::event_EXT_EVENT_GAME_RESULT_4_LIENG()
{
	boost::shared_ptr<string> rg = this->param->GetUtfString(EXT_FIELD_GAME_RESULT);
	//OPen my list cards
	int pos = LayerAvatars->GetPosByName(this->MyAI);
	if (pos != -1)
		this->LayerCards->TurnUpAllCards(this->ListCards, pos);

	if (rg != NULL) {
		log("result game is: %s", rg->c_str());

		this->GetButtonByTag(DTAG_BTN_FOLLOW)->setVisible(false);
		this->GetButtonByTag(DTAG_BTN_FOLD)->setVisible(false);
		this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
		this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);

		this->LayerAvatars->StopAllTimer();

		if (this->getChildByTag(TAG_BET_LIENG_LAYER) != NULL) {
			this->removeChildByTag(TAG_BET_LIENG_LAYER);
		}
		if (this->getChildByTag(TAG_NAN_BACAY_LAYER) != NULL) {
			this->removeChildByTag(TAG_NAN_BACAY_LAYER);
		}

		Number_Chat_inGame7u *layerNumbers = Number_Chat_inGame7u::create();
		this->addChild(layerNumbers);

		vector<string> result = mUtils::splitString(*rg, ';');
		for (unsigned int i = 0; i < result.size(); ++i) {
			vector<string> info = mUtils::splitString(result[i], '|');
			string StrMoney = "";
			string StrResuilt = info[2] + "|" + info[3] + "|" + info[4];
			if (0 == i) {
				StrMoney = ("+" + info[5]);
			}
			else {
				StrMoney = ("-" + info[5]);
			}

			int Pos = LayerAvatars->GetPosByName(info[0]);
			layerNumbers->ShowNumberByPos(Pos, StrMoney);
			this->LayerBet->SetResult4AllUser(Pos, info[1], StrResuilt);
		}
	}
}
void LayerPlayGameBaCay::event_EXT_EVENT_REGISTER_QUEUE_NTF()
{
	boost::shared_ptr<string> lu = this->param->GetUtfString("lu");
	if (lu != NULL)
	{
		log("DS Chau ria: %s", lu->c_str());
		this->queueList = lu->c_str();
		this->LayerAvatars->setListUserGuess(lu->c_str(), '-');
		//queueList = lu->c_str();
		// bool isChauRia = this->isNguoiChauRia(lu->c_str());
	}
}
void LayerPlayGameBaCay::event_EXT_EVENT_WAIT_BET_TIMER_NTF(){
	this->LayerButton->getButtonByTag(DTAG_BTN_READY)->setVisible(false);
	if (this->param->GetInt("drt") == NULL)
		return;
	auto timerForBet = this->param->GetInt("drt");
	vector<string> arrPlayes = mUtils::splitString(this->ListUsers, ';');

	this->IsSpector = this->LayerAvatars->IsSpect();
	if (this->IsSpector){
		this->layerBetBaCay->setVisible(false);
	}

	this->timerBet3Cay = *timerForBet / 1000;
	this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
	this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);

	this->LayerAvatars->RunAllTimer();

	std::string IdChuong = this->FindChuong(this->ListUsers);
	auto pos = this->LayerAvatars->GetPosByName(IdChuong);
	auto avatar = this->LayerAvatars->GetUserByPos(pos);
	if (avatar == NULL) return;
	avatar->stopTimer();
	auto avatarMe = this->LayerAvatars->GetUserByPos(0);
	if (avatar == NULL) return;
	avatarMe->stopTimer();

	log("valueBetOnWbntf= %d", this->layerBetBaCay->getValueBet());

	this->showLoadingBet();
	if (this->uidChuong.compare(this->myName) != 0)
	{
		if (this->IsSpector == false)
			this->displayUIBet(true);
	}
	else{
		this->displayUIBet(false);
		SceneManager::getSingleton().showToast(dataManager.GetSysString(114));
	}

	// 	for (int i = 0; i < arrPlayes.size(); ++i) {
	// 		auto info = mUtils::splitString(arrPlayes[i], '|');
	// 		if (this->myName.compare(info[0]) == 0) {
	// 			if (info[info.size() - 1].compare("1") == 0 && IdChuong.compare(info[0]) != 0){
	// 				auto toast = Chat::create("Hãy đặt cược!", -1);
	// 				this->addChild(toast);
	// 				if (0 == this->uidChuong.compare(this->myName)){
	// 					this->displayUIBet(true);
	// 					this->layerBetBaCay->_setLocalZOrder(this->LayerAvatars->getLocalZOrder() + 1);
	// 				}
	// 				this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
	// 			}
	// 			else{
	// 				auto toast = Chat::create("Đợi người chơi khác đặt cược!", -1);
	// 				this->addChild(toast);
	// 			}
	// 		}
	// 	}
	//this->scheduleOnce(schedule_selector(LayerPlayGameBaCay::onScheduleEndBet), this->timerBet3Cay);
}
void LayerPlayGameBaCay::onScheduleEndBet(float dt){
	this->hideLoadingBet();
	int _size = mUtils::splitString(this->ListUsers, ';').size();
	if (_size > 2){
		this->bien3Cay->displayBien3cay(false);
		log("betValue %d", this->valueBet);
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL) return;
		//params->PutInt(EXT_FIELD_BET_TYPE, 2);
		params->PutInt(EXT_FIELD_GAME_BET_VALUE, this->valueBet);
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
		GameServer::getSingleton().Send(request);

		this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(true);
		this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(true);
		this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(true);
		this->LayerAvatars->RunAllTimer();
		//this->removeChildByTag(1048);
	}
}
void LayerPlayGameBaCay::createLoadingBet()
{
	Size winSize = Director::getInstance()->getVisibleSize();
	bgLoading = Sprite::create("timer.png");
	bgLoading->setAnchorPoint(Vec2(0.5, 0.5));
	bgLoading->setPosition(Vec2(WIDTH_DESIGN / 2,
		winSize.height / 2));

	Sprite *time = Sprite::create("frame_time.png");
	timerBet = ProgressTimer::create(time);
	timerBet->setPercentage(100);
	/*change*/
	/*timerBet->setAnchorPoint(Vec2(0, 0));
	timerBet->setPosition(Vec2(0, 0));*/
	timerBet->setAnchorPoint(Vec2(0.5, 0.5));
	timerBet->setPosition(Vec2(bgLoading->getContentSize().width / 2, bgLoading->getContentSize().height / 2));
	/*end change*/
	timerBet->setType(ProgressTimer::Type::RADIAL);

    labelTimerBet = Label::createWithSystemFont("15", "",
		40, Size(40 * 2, 40),
		TextHAlignment::CENTER, TextVAlignment::CENTER);
	labelTimerBet->setAnchorPoint(Vec2(0.5, 0.5));
	labelTimerBet->setPosition(Vec2(bgLoading->getContentSize().width / 2,
		bgLoading->getContentSize().height / 2));
	bgLoading->addChild(timerBet);
	bgLoading->addChild(labelTimerBet);
	this->addChild(bgLoading);
	bgLoading->setVisible(false);
}

void LayerPlayGameBaCay::showLoadingBet()
{
	labelTimerBet->setString("15");
	bgLoading->setVisible(true);
	timerBet->setPercentage(100);
	timerBet->runAction(ProgressTo::create(TIME_BAO_SAM, 0));
	Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(LayerPlayGameBaCay::updateTimerBet), this,1,false);
}

void LayerPlayGameBaCay::hideLoadingBet()
{
	this->displayButtonDanhBien(false);
	this->bien3Cay->displayBien3cay(false);
	timerBet3Cay = 15;
	bgLoading->setVisible(false);
	timerBet->stopAllActions();
	Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(LayerPlayGameBaCay::updateTimerBet),this);
}

void LayerPlayGameBaCay::updateTimerBet(float dt)
{
	labelTimerBet->setString(StringUtils::format("%d", timerBet3Cay));
	timerBet3Cay--;
	if (timerBet3Cay == 0){
		this->hideLoadingBet();
	}
}
void LayerPlayGameBaCay::event_EXT_EVENT_LIST_USER_UPDATE_4_LIENG()
{
	boost::shared_ptr<string> lu = param->GetUtfString(EXT_FIELD_LIST_USER);
	if (lu != NULL) {
		this->ListUsers = *lu;
		log("List Users Lieng %s", ListUsers.c_str());

		vector<string> list = mUtils::splitString(ListUsers, ';');
		this->SizeListUser = list.size();
		this->LayerAvatars->SetListUser(ListUsers);
		this->LayerCards->setPlayerMap(LayerAvatars->GetMapPlayer());

		if (this->LayerAvatars->IsPlaying(this->ListUsers)) {
			this->mReal = true;
			this->FlagChiaBai = true;
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
			//this->//mLayerCount->StopCountDownReady();
		}
		else {
			log("Game ko choi");
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
			this->GetButtonByTag(DTAG_BTN_READY)->setTouchEnabled(true);
		}

		string IdBoss = FindBossRoom(ListUsers);
		if (0 == MyAI.compare(IdBoss)) {
			//this->GetButtonByTag(DTAG_BTN_READY)->setTitleText(dataManager.GetSysString(197));
			//this->//mLayerCount->StopCountDownReady();
		}
		else {
			//this->GetButtonByTag(DTAG_BTN_READY)->setTitleText(dataManager.GetSysString(195));
		}

		this->UserRejoinGame(ListUsers,0);

		this->LayerBet->VisibleAllFrameBet();

		if (this->LayerAvatars->IsSpect()) {
			//this->//nameGame->setString("Bạn đang xem...");
			this->SpecMode();
			//this->//mLayerCount->StopCountDownReady();
			this->LayerButton->SetIsGuess(true);
			if (!this->LayerAvatars->IsPlaying(this->ListUsers)) {
				this->LayerCards->ResetGame();
				this->LayerBet->GetLayerResult()->removeAllChildrenWithCleanup(true);
			}
		}
		else {
			this->LayerButton->SetIsGuess(false);
			//this->//nameGame->setString(StringResult.c_str());
			this->LayerButton->getButtonByTag(103)->setTouchEnabled(true);
			this->GetButtonByTag(DTAG_BTN_READY)->setTouchEnabled(true);
			this->GetButtonByTag(DTAG_BTN_UNREADY)->setTouchEnabled(true);
			this->GetButtonByTag(DTAG_BTN_VIEW)->setTouchEnabled(true);
			this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setTouchEnabled(true);
			this->GetButtonByTag(DTAG_BTN_FOLD)->setTouchEnabled(true);
			//this->GetButtonByTag(DTAG_BTN_COMPLETE)->setTouchEnabled(true);

			if (list.size() > 1 && !this->LayerAvatars->IsPlaying(this->ListUsers)) {
				vector<string> arrUser = mUtils::splitString(ListUsers, ';');
				for (int i = 0; i < arrUser.size(); i++) {
					vector<string> info = mUtils::splitString(arrUser[i], '|');
					if (info.size() == 0) continue;
					if (0 == MyAI.compare(info[0]) && 0 == info[3].compare("1")) {
						//this->//mLayerCount->StopCountDownReady();
					}
				}
			}
		}

		for (int i = 0; i < list.size(); i++) {
			vector<string> info = mUtils::splitString(list[i], '|');
			int pos = LayerAvatars->GetPosByName(info[0]);
			string _minbet = mUtils::convertMoneyEx(this->mBetDefault);
			this->LayerBet->GetFrameBetByPos(pos)->SetValueBet(_minbet);
			this->LayerBet->GetFrameBetByPos(pos)->setVisible(true);
		}
	}
}
void LayerPlayGameBaCay::ResetGame(){
	this->FlagChiaBai = false;
	this->LayerCards->ResetGame();
	this->LayerBet->resetAllBets(0);
	this->LayerAvatars->UnReadyAllUser();
	//reset bien active
	this->bien3Cay->resetAllBien();
	this->LayerAvatars->resetBienButton();
	this->statusNhanBienAll = 0;
	this->checkedBg->setVisible(false);
	this->layerBetBaCay->resetSlider();
	this->queueList = "";
}

void LayerPlayGameBaCay::processBtnNhanBienAllTouched()
{
	/*if (type == ui::Widget::TouchEventType::ENDED){*/
	this->statusNhanBienAll = 0;
	int betvl = 0;
	if (this->checkedBg->isVisible()){
		this->statusNhanBienAll = 1;
		this->checkedBg->setVisible(false);
		this->btnBienTat1->setVisible(true);
		this->btnBienTatx2_1->setVisible(true);
	}
	else{
		this->statusNhanBienAll = 0;
		this->checkedBg->setVisible(true);
		this->btnBienTat1->setVisible(false);
		this->btnBienTatx2_1->setVisible(false);
	}
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL) return;
	params->PutInt("status", this->statusNhanBienAll);
	params->PutInt("betvl", betvl);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_SET_ACCEPT_ENROLL_ALL_REQUEST, params, lastRoom));
	GameServer::getSingleton().Send(request);
	//}
}

void LayerPlayGameBaCay::processBtnBienTatTouched()
{
	log("button bien tat X1 touched");
	this->btnBienTat1->setVisible(false);
	int status = 0;
	int betvl = this->money_bet;
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL) return;
	params->PutInt("status", status);
	params->PutInt("betvl", betvl);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_SET_ACCEPT_ENROLL_ALL_REQUEST, params, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerPlayGameBaCay::processBtnBienTatx2Touched()
{
	log("button bien tat X2 touched");
	this->btnBienTatx2_1->setVisible(false);
	int status = 0;
	int betvl = 2 * this->money_bet;
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL) return;
	params->PutInt("status", status);
	params->PutInt("betvl", betvl);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_SET_ACCEPT_ENROLL_ALL_REQUEST, params, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerPlayGameBaCay::processBtnDatCuaTouched(Ref *sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		/*
		button Dat Cua touched - hide ui bet and show ui danh bien
		*/
		this->displayUIBet(false);
		this->layerBetBaCay->setLocalZOrder(this->LayerAvatars->getLocalZOrder() - 1);
		int _size = mUtils::splitString(this->ListUsers, ';').size();
		log("size %d", _size);

		log("valueBet= %d", this->layerBetBaCay->getValueBet());
		if (this->layerBetBaCay->getValueBet() == 0){
			this->valueBet = this->money_bet;
		}
		else{
			this->valueBet = this->layerBetBaCay->getValueBet();
		}

		//neu chi co 2 nguoi thi khong hien thi danh bien va bet xong thi gui ban tin bet luon
		if (_size <= 2){
			this->displayButtonDanhBien(false);
		}
		else{
			//hien thi danh bien - neu la chu phong thi khong hien thi
			this->showBienUser();
			this->displayButtonDanhBien(true);
		}
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL) return;
		//params->PutInt(EXT_FIELD_BET_TYPE, 2);
		params->PutInt(EXT_FIELD_GAME_BET_VALUE, this->valueBet);
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
		GameServer::getSingleton().Send(request);
	}
}

void LayerPlayGameBaCay::hideUIDanhBien()
{
	this->btnNhanBienAll1->setVisible(false);
	this->btnBienTat1->setVisible(false);
	this->btnBienTatx2_1->setVisible(false);
	//this->btnDat->setVisible(false);
	this->layerBetBaCay->setVisible(false);
}

void LayerPlayGameBaCay::displayUIBet(bool _display)
{
	//this->btnDat->setVisible(_display);
	this->layerBetBaCay->setVisible(_display);
}

void LayerPlayGameBaCay::displayButtonDanhBien(bool _display)
{
	this->btnNhanBienAll1->setVisible(_display);
	this->btnBienTat1->setVisible(_display);
	this->btnBienTatx2_1->setVisible(_display);
}

void LayerPlayGameBaCay::event_EXT_EVENT_DEAL_CARD_BOSS_NOTIF(std::string _lc)
{
	this->isStartedGame = true;
	log("list card: %s", _lc.c_str()); //vodanh9x|29;hehe_keke|12;
	vector<std::string> arrPlayes = mUtils::splitString(_lc, ';');

	bool isBossExist = false;
	for (int i = 0; i < arrPlayes.size(); ++i){
		std::string username = mUtils::splitString(arrPlayes[i], '|')[0];
		if (username.compare(this->uidChuong) == 0){
			isBossExist = true;
		}
	}
	if (isBossExist == false){
		this->LayerAvatars->SetPosChuong(100);
	}

	this->LayerAvatars->SetListUser(_lc);
	this->LayerCards->setListUser(_lc);
	this->LayerCards->setPlayerMap(LayerAvatars->GetMapPlayer());

	std::string _cardId = "";
	std::string _lcCardId = "";
	this->LayerButton->getButtonByTag(DTAG_BTN_READY)->setVisible(false);
	vector<std::string> _player = mUtils::splitString(_lc, ';');
	for (int i = 0; i < _player.size(); ++i){
		std::string _cardParse = mUtils::splitString(_player.at(i), '|')[1];
		_cardId = this->getURL_byID_3CayChuong(atoi(_cardParse.c_str()));

		if (i < _player.size() - 1)
			_cardId += ";";
		_lcCardId += _cardId;
	}
	log("_lc= %s", _lcCardId.c_str());
	this->LayerCards->GivePokerFindChuong(_lcCardId);
}

void LayerPlayGameBaCay::event_EXT_EVENT_START_GAME_NOTIF()
{
	this->IsTurnUpAll = false;
	this->isStartedGame = true;
	this->LayerCards->ResetGame();
	this->LayerBet->GetLayerResult()->removeAllChildrenWithCleanup(true);
	this->LayerAvatars->UnReadyAllUser();
	this->mLayerCount->stopTimer();
	layerGa->showToast(false);

	this->GetButtonByTag(DTAG_BTN_UNREADY)->setVisible(false);
	//hien thi dat cuoc: la chuong thi khong hien thi
	if (mCurrGameID == kGameBaCayChuong){
		if (0 == this->uidChuong.compare(this->MyAI)){
			this->displayUIBet(false);
			this->layerBetBaCay->setLocalZOrder(this->LayerAvatars->getLocalZOrder() - 1);
			this->LayerBet->getChildByTag(this->LayerAvatars->GetPosByName(this->uidChuong))->setVisible(false);
		}
		else{
			if (this->IsSpector == false)
				this->displayUIBet(true);
			this->layerBetBaCay->setLocalZOrder(this->LayerAvatars->getLocalZOrder() + 1);
		}
	}
	this->LayerCards->resetCardChuong();
	this->FlagChiaBai = false;
}

double LayerPlayGameBaCay::getMoneyBet()
{
	return this->money_bet;
}

std::string LayerPlayGameBaCay::getURL_byID_3CayChuong(int pID)
{
	ostringstream oss;
	int num = (pID / 4);
	int type = (pID % 4);
	if (num > 9)
	{
		num = 9;
		type = pID - num * 4;
	}
	if (type == 0){
		num--;
		type = 4;
	}
	string url = "card_";
	if (num == 1){
		url += "a";
	}
	else{
		oss << num;
		url += oss.str();
	}

	url += "_";
	// [1, 2, 3, 4] = ['s', 'c', 'd', 'h']
	switch (type) {
	case 1:
		url += "s.png";
		break;
	case 2:
		url += "c.png";
		break;
	case 3:
		url += "d.png";
		break;
	case 4:
		url += "h.png";
		break;
	}
	return url;
}

void LayerPlayGameBaCay::event_EXT_EVENT_USER_LEAVE_NOTIF()
{
	boost::shared_ptr<string> listUser = param->GetUtfString("lu");
	boost::shared_ptr<string> uid = param->GetUtfString("uid");

	if (listUser != NULL){
		std::string _uid = *uid;
		if (this->uidChuong.compare(_uid) == 0){
			this->LayerAvatars->getImageChuong()->setVisible(false);
		}
		this->ListUsers = listUser->c_str();
		log("event_EXT_EVENT_USER_LEAVE_NOTIF");
		log("listUser= %s", listUser->c_str());
		this->LayerAvatars->SetListUser(this->ListUsers);
		this->LayerCards->setListUser(this->ListUsers);
		this->LayerCards->setPlayerMap(LayerAvatars->GetMapPlayer());
		this->bien3Cay->setListUser(this->ListUsers);

		this->LayerAvatars->setListUserGuess(this->queueList, '-');
		vector<string> listStr = mUtils::splitString(this->ListUsers, ';');
		if (listStr.size() <= 1)
		{
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
			this->mLayerCount->stopTimer();

		}
	}
}

void LayerPlayGameBaCay::event_EXT_EVENT_NEW_BOSS_NOTIF()
{
	std::string _mess = "";
	string myName = GameUtils::getMyName();
	std::string userName = GameUtils::getNamePlayer(myName);
	log("myName: %s - myAi: %s", myName.c_str(), this->MyAI.c_str());
	std::string uid = *param->GetUtfString("uid");
	int drt = *param->GetInt("drt");
	std::string rd = *param->GetUtfString("rd");
	log("Chu phong: %s - drt: %d", uid.c_str(), drt);
	this->uidChuong = uid;
	//neu la chu phong thi hien thi mess
	if (0 == this->uidChuong.compare(this->MyAI)){
		_mess = dataManager.GetSysString(115);
	}
	else{
		_mess = dataManager.GetSysString(116)+" " + GameUtils::getNamePlayer(uid);
	}
	SceneManager::getSingleton().showToast(_mess);
	this->LayerAvatars->SetPosChuong(this->LayerAvatars->GetPosByName(this->uidChuong));
}

void LayerPlayGameBaCay::createLayerBien()
{
	this->bien3Cay = Bien3Cay::create(money_bet);
	this->addChild(this->bien3Cay);
}

void LayerPlayGameBaCay::event_EXT_EVENT_ENROLL_ALL_INFO_NOTIF(std::string _lstBien)
{
	/*lstBien = user1_status_betValue;user2_status_betvalue;..
		status = 0: chap nhan - 1: khong chap nhan
		*/
	vector<std::string> lstUserBien = mUtils::splitString(_lstBien, ';');
	for (int i = 0; i < lstUserBien.size(); ++i){
		std::string username = mUtils::splitString(lstUserBien[i], '|')[0];
		int _statusBien = atoi(mUtils::splitString(lstUserBien[i], '|')[1].c_str());
		int betValue = atoi(mUtils::splitString(lstUserBien[i], '|')[2].c_str());
		LayerBien* bienUser = this->bien3Cay->GetLayerBienByPos(this->LayerAvatars->GetPosByName(username));
		bienUser->resetBien();
		if (_statusBien == 0){
			if (betValue == this->money_bet){
				bienUser->buttonBienx1LoadTexture();
			}
			else if (betValue == 0){
				bienUser->buttonBienx1LoadTexture();
				bienUser->buttonBienx2LoadTexture();
			}
			else{
				bienUser->buttonBienx2LoadTexture();
			}
		}
		else{
		}
	}
}

void LayerPlayGameBaCay::showBienUser()
{
	vector<std::string> arrLst = mUtils::splitString(this->ListUsers, ';');
	for (int i = 0; i < arrLst.size(); ++i){
		std::string name = mUtils::splitString(arrLst[i], '|')[0];
		int _tag = this->LayerAvatars->GetPosByName(name);
		log("Tag=%d name=%s", _tag, name.c_str());
		if (this->bien3Cay->GetLayerBienByPos(_tag)){
			LayerBien* bienUser = this->bien3Cay->GetLayerBienByPos(_tag);
			bienUser->setVisible(true);
		}
	}
	/*if (this->LayerAvatars->GetPosByName(this->myName)){
		LayerBien *bienMe = (LayerBien*) this->bien3Cay->GetLayerBienByPos(this->LayerAvatars->GetPosByName(this->MyAI));
		bienMe->setVisible(false);
		}*/
	this->bien3Cay->GetLayerBienByPos(0)->setVisible(false);
	if (this->LayerAvatars->GetPosByName(this->uidChuong)){
		LayerBien *bienBoss = this->bien3Cay->GetLayerBienByPos(this->LayerAvatars->GetPosByName(this->uidChuong));
		bienBoss->setVisible(false);
	}
}

void LayerPlayGameBaCay::event_EXT_EVENT_USER_JOIN_NOTIF()
{
	boost::shared_ptr<string> lu = this->param->GetUtfString(EXT_FIELD_LIST_USER);
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);

	log("uid join room: %s", (*uid).c_str());
	//if (lu != NULL){
	//	this->ListUsers = lu->c_str();

	//	int _size = mUtils::splitString(this->ListUsers, ';').size();
	//	if (_size >= 4){
	//		this->LayerButton->processDisplayButtonInvite(false);
	//	}
	//	else{
	//		if (this->vipRoom == 1){
	//			this->LayerButton->processDisplayButtonInvite(true);
	//		}
	//	}
	//	log("event_EXT_EVENT_USER_JOIN_NOTIF");
	//	log("listUser join room= %s", lu->c_str());
	//	this->LayerAvatars->SetListUser(this->ListUsers);
	//	this->LayerCards->setListUser(this->ListUsers);
	//	this->LayerCards->setPlayerMap(LayerAvatars->GetMapPlayer());
	//	isStartedGame = this->LayerAvatars->IsStartedGame();
	//	IsSpector = this->LayerAvatars->IsSpect();
	//	
	//	std::string m_IdChuong = this->FindChuong(this->ListUsers);
	//	this->LayerAvatars->SetPosChuong(this->LayerAvatars->GetPosByName(m_IdChuong));
	//	
	//	//neu la khach
	//	if (IsSpector){
	//		log("spector");
	//		this->LayerButton->SetIsGuess(true);
	//		this->layerBetBaCay->setVisible(false);
	//	}
	//	if (!isStartedGame && !IsSpector && _size > 1 && mLayerCount->isVisible()){
	//		mLayerCount->startTimer();
	//	}
	//	if (isStartedGame && !IsSpector) {
	//		log("JOIN SUCCESS");
	//		LayerButton->SetIsGuess(false);
	//	}
	//}

	if (lu != NULL) {

		log("list user Ba Cay Chuong: %s", lu->c_str());
		this->ListUsers = *lu;//"test3caychuong3|0|0|0|1;test3caychuong1|0|0|0|1;test3caychuong2|0|0|0|1;test3caychuong4|0|0|0|1;test3caychuong5|0|0|0|1;test3caychuong6|0|0|0|1;test3caychuong7|0|0|0|1";

		int _size = mUtils::splitString(this->ListUsers, ';').size();
		if (_size >= 4){
			this->LayerButton->processDisplayButtonInvite(false);
		}
		else{
			if (this->vipRoom == 1){
				this->LayerButton->processDisplayButtonInvite(true);
			}
		}

		//test
		//this->ListUsers += ";test3caychuong3|1|0|0|;test3caychuong1|0|0|0|1;test3caychuong2|0|0|0|1;test3caychuong4|0|0|0|1;test3caychuong5|0|0|0|1;test3caychuong6|0|0|0|1";
		vector<string> arrPlayes = mUtils::splitString(this->ListUsers, ';');
		this->SizeListUser = arrPlayes.size();

		//reset image Chuong- duyet list user: neu chuong khong con trong ban thi reset 
		bool isBossExist = false;
		for (int i = 0; i < arrPlayes.size(); ++i){
			std::string username = mUtils::splitString(arrPlayes[i], '|')[0];
			if (username.compare(this->uidChuong) == 0){
				isBossExist = true;
			}
		}
		if (isBossExist == false){
			//auto name = mUtils::splitString(arrPlayes[0], '|')[0];
			this->LayerAvatars->SetPosChuong(100);
		}

		bool isPlaying = LayerAvatars->IsStartedGame();
		//this->FlagChiaBai = isPlaying;
		log("%s", isPlaying ? "Đang chơi" : "Chưa chơi");
		if (isPlaying && !this->LayerAvatars->IsSpect())
			return;

		this->LayerAvatars->SetListUser(this->ListUsers);
		this->LayerCards->setListUser(this->ListUsers);
		this->LayerCards->setPlayerMap(LayerAvatars->GetMapPlayer());
		this->bien3Cay->setListUser(this->ListUsers);

		//isStartedGame = LayerAvatars->IsStartedGame();
		this->IsSpector = this->LayerAvatars->IsSpect();

		std::string m_IdChuong = this->FindChuong(this->ListUsers);

		this->LayerAvatars->SetPosChuong(this->LayerAvatars->GetPosByName(m_IdChuong));
		if (this->SizeListUser == 0) return;

		if (isPlaying)
			this->FlagChiaBai = true;
		else if (this->SizeListUser > 1 && this->isStartedGame == false){
			this->ResetGame();
			this->mLayerCount->startTimer();
		}

		if (this->SizeListUser == 1){
			this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
			this->ResetGame();
			this->mLayerCount->stopTimer();
		}
		else if(this->isStartedGame == false){
			if (this->uidChuong.compare("") == 0){
				this->GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
			}
			else{
				if (!isPlaying) {
					if (0 == MyAI.compare(m_IdChuong)) {
						this->GetButtonByTag(DTAG_BTN_READY)->setVisible(true);
						this->GetButtonByTag(DTAG_BTN_UNREADY)->setVisible(false);
					}
					else {
						this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
					}
				}
				else{

				}
			}
		}

		//Hiển thị các FrameBet, LayerBien của người chơi
		this->LayerBet->VisibleAllFrameBet();

		for (unsigned int i = 0; i < arrPlayes.size(); ++i) {
			vector<string> info = mUtils::splitString(arrPlayes[i], '|');
			if (info.size() == 0) continue;
			int pos = this->LayerAvatars->GetPosByName(info[0]);
			//Show bet
			string _bet = mUtils::convertMoneyEx(atoi(info[2].c_str()));
			this->LayerBet->GetFrameBetByPos(pos)->SetValueBet(_bet);
			this->LayerBet->GetFrameBetByPos(pos)->setVisible(true);
			if (info[0].compare(m_IdChuong) == 0)
				this->LayerBet->GetFrameBetByPos(pos)->setVisible(false);
		}

		for (unsigned int i = 0; i < arrPlayes.size(); ++i) {
			vector<string> info = mUtils::splitString(arrPlayes[i], '|');
			if (info.size() == 0) continue;
			if (0 == this->MyAI.compare(info[0])) {
				if (0 == info[3].compare("1") && 0 == this->LayerCards->getListCards_0().size()) {
					log("Me Rejoin");
					this->LayerCards->CreateCardBack(kuser0);
				}
			}
			else {
				int pos = this->LayerAvatars->GetPosByName(info[0]);
				if (0 == info[3].compare("1") && 0 == this->LayerCards->getListCardsByPos(pos).size())
					this->LayerCards->CreateCardBack(pos);
				if (pos == kuser0 && this->IsSpector) {
					if (0 == info[3].compare("1") && 0 == this->LayerCards->getListCards_0().size())
						this->LayerCards->CreateCardBack(kuser0);
				}
			}
		}

		//! nếu là khách
		if (this->IsSpector) {
			//this->//nameGame->setString("Bạn đang xem...");
			this->SpecMode();
			this->mLayerCount->stopTimer();
			this->LayerButton->SetIsGuess(true);
			this->layerBetBaCay->setVisible(false);
		}
		//! Neu la player
		else {
			this->LayerButton->SetIsGuess(false);
			//this->//nameGame->setString(StringResult);
			if (arrPlayes.size() > 1 && !isPlaying) {
				for (int i = 0; i < arrPlayes.size(); ++i) {
					vector<string>info = mUtils::splitString(arrPlayes[i], '|');
					if (0 == this->MyAI.compare(info[0])) {
						if (0 == info[4].compare("1") && 0 == info[1].compare("0")) {
							//this->mLayerCount->startTimer();
						}
					}
				}
			}

			if (isPlaying) {
				//! Ẩn Các Button Sẵn Sàng, Hủy Sẵn Sàng, Đặt Cược
				this->GetButtonByTag(DTAG_BTN_READY)->setVisible(false);
				this->GetButtonByTag(DTAG_BTN_UNREADY)->setVisible(false);

				if (!this->IsTurnUpAll) {
					//! Hiện các Button: Lật bài, Xem bài và nặn bài
					//					this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(true);
					//					this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(true);
					//					this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(true);
					//this->mLayerCount->startTimer();
				}
				else {
					this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
					this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
					this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);
					this->LayerBet->GetLayerResult()->removeAllChildrenWithCleanup(true);
				}
				this->mLayerCount->stopTimer();
			}
			else {
				/*this->GetButtonByTag(DTAG_BTN_TURNALL)->setVisible(false);
				this->GetButtonByTag(DTAG_BTN_VIEW)->setVisible(false);
				this->GetButtonByTag(DTAG_BTN_SQUEEZ)->setVisible(false);*/
			}
		}//!else kết thúc ktra người chơi
		//hide layerBet
		for (int i = 0; i < arrPlayes.size(); ++i)
		{
			std::string username = mUtils::splitString(arrPlayes[i], '|')[0];
			FrameBet* userFrameBet = this->LayerBet->GetFrameBetByPos(this->LayerAvatars->GetPosByName(username));
			userFrameBet->setVisible(false);
		}
	}
}

void LayerPlayGameBaCay::event_EXT_EVENT_ENROLL_BET_NTF(std::string _uid, double _bet)
{
	std::string betvlStr = StringUtils::format("%d", _bet);
	this->LayerAvatars->setPosBien(this->LayerAvatars->GetPosByName(_uid), betvlStr);
}
