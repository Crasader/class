//
//  LayerPlayGameTLMN.cpp
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//var GAME_CHANGE_BALANCE_TYPE_TRU_GA   = 8; //tru tien ghop ga
//var GAME_CHANGE_BALANCE_TYPE_AN_GA   = 9; //cong tien khi an ga
//[10:22:01 AM] mrdung Do Tien Dung: public static final String EXT_FIELD_CHANGE_BALANCE_TYPE = "cbt";
//[10:22:10 AM] mrdung Do Tien Dung: la ly do tru tien/cong tien
// public static final int GAME_CHANGE_BALANCE_TYPE_TOI_TRANG  = 10; //Thay doi dien khi toi trang
// public static final int GAME_CHANGE_BALANCE_TYPE_DUT_MU   = 12; //tru tien khi bi dut mu
///public static final int GAME_CHANGE_BALANCE_TYPE_THUA_CONG  = 11; //tru tien khi bi thua cong
//public static final int GAME_CHANGE_BALANCE_TYPE_BAT_CONG_LANG = 13; //cong
/*
[10:34:21 AM] mrdung Do Tien Dung: public static final String EXT_EVENT_CHICKEN_UPDATE_NTF    = "chkntf"; ban tin update gia tri ga
[10:34:25 AM] mrdung Do Tien Dung: va co 2 truong nay nhe
[10:34:38 AM] mrdung Do Tien Dung:     public static final String EXT_FIELD_CHICKEN_VALUE = "chkvl";
public static final String EXT_FIELD_CHICKEN_LEVEL = "chklv";
[10:34:43 AM] mrdung Do Tien Dung: tong tien va muc ga
*/
#include "LayerPlayGameTLMN.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "data_manager.h"
vector<string> LayerPlayGameTLMN::split(string &S, const char &str) {
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

enum EXT_REPONSE {
	EXT_EVENT_USER_JOIN_NOTIF,      // jrntf
	EXT_EVENT_LISTCARD_NTF,         //"lcntf";
	EXT_EVENT_ERROR_READY_NTF,      // = "e_rntf";
	EXT_EVENT_USER_READY_NOTIF,     // = "rdyntf";
	EXT_EVENT_UNREADY_NTF,          // = "urntf";
	EXT_EVENT_READY_RES,            // = "rdres";
	EXT_EVENT_LIST_USER_UPDATE,     // = "luu";
	EXT_EVENT_START_GAME_NOTIF,     // = "strntf";
	EXT_EVENT_DEAL_CARD_NOTIF,      // = "dlcntf";
	EXT_EVENT_USER_LEAVE_NOTIF,     // = "lrntf";
	EXT_EVENT_END_GAME_NOTIF,       // = "endntf";
	EXT_EVENT_AMF_TEST_NOTIF,       // = "cblltf";
	EXT_EVENT_VICTORY_NOTIF,        // = "vicntf";
	EXT_EVENT_NEAD_PLAY_NOTIF,      // = "ndpntf";
	EXT_EVENT_PLAY_CARD_NOTIF,      // = "plcntf";
	EXT_EVENT_GAME_CHANGE_NOTIF,    // = "gchntf";
	EXT_EVENT_PASS_CARD_NOTIF,      // = "pscntf";
	EXT_EVENT_PLAY_CARD_RES,        // = "plcrs";
	EXT_EVENT_UNREADY_REQ,
	EXT_EVENT_READY_REQ,            // = "rr";
	EXT_EVENT_PLAY_CARD_REQ,        // = "plcrq"
	EXT_EVENT_PASS_CARD_REQ,       // = "pscrq";
	EXT_EVENT_HIT_CARD_ERROR,		//pscrs
	EXT_EVENT_START_READY_TIMER_NOTIF, //rdtntf,
	EXT_EVENT_CHICKEN_UPDATE_NTF//"chkntf"
};

int LayerPlayGameTLMN::convertResponseToInt(string inString) {
	if (inString == "jrntf")    return EXT_EVENT_USER_JOIN_NOTIF;
	if (inString == "lcntf")    return EXT_EVENT_LISTCARD_NTF;
	if (inString == "e_rntf")   return EXT_EVENT_ERROR_READY_NTF;
	if (inString == "rdyntf")   return EXT_EVENT_USER_READY_NOTIF;
	if (inString == "urntf")    return EXT_EVENT_UNREADY_NTF;
	if (inString == "rdres")    return EXT_EVENT_READY_RES;
	if (inString == "luu")      return EXT_EVENT_LIST_USER_UPDATE;
	if (inString == "strntf")   return EXT_EVENT_START_GAME_NOTIF;
	if (inString == "dlcntf")   return EXT_EVENT_DEAL_CARD_NOTIF;
	if (inString == "lrntf")    return EXT_EVENT_USER_LEAVE_NOTIF;
	if (inString == "endntf")   return EXT_EVENT_END_GAME_NOTIF;
	if (inString == "cblltf")   return EXT_EVENT_AMF_TEST_NOTIF;
	if (inString == "vicntf")   return EXT_EVENT_VICTORY_NOTIF;
	if (inString == "ndpntf")   return EXT_EVENT_NEAD_PLAY_NOTIF;
	if (inString == "plcntf")   return EXT_EVENT_PLAY_CARD_NOTIF;
	if (inString == "gchntf")   return EXT_EVENT_GAME_CHANGE_NOTIF;
	if (inString == "pscntf")   return EXT_EVENT_PASS_CARD_NOTIF;
	if (inString == "plcrs")    return EXT_EVENT_PLAY_CARD_RES;
	if (inString == "urr")      return EXT_EVENT_UNREADY_REQ;
	if (inString == "rr")       return EXT_EVENT_READY_REQ;
	if (inString == "plcrq")    return EXT_EVENT_PLAY_CARD_REQ;
	if (inString == "pscrq")    return EXT_EVENT_PASS_CARD_REQ;
	if (inString == "pscrs")    return EXT_EVENT_HIT_CARD_ERROR;
	if (inString == "rdtntf")    return EXT_EVENT_START_READY_TIMER_NOTIF;
	if (inString == "chkntf")	return EXT_EVENT_CHICKEN_UPDATE_NTF;
	return -1;
}

string LayerPlayGameTLMN::convertResponseToString(int inInt) {
	if (inInt == EXT_EVENT_USER_JOIN_NOTIF)     return "jrntf";
	if (inInt == EXT_EVENT_LISTCARD_NTF)        return "lcntf";
	if (inInt == EXT_EVENT_ERROR_READY_NTF)     return "e_rntf";
	if (inInt == EXT_EVENT_USER_READY_NOTIF)    return "rdyntf";
	if (inInt == EXT_EVENT_UNREADY_NTF)         return "urntf";
	if (inInt == EXT_EVENT_READY_RES)           return "rdres";
	if (inInt == EXT_EVENT_LIST_USER_UPDATE)    return "luu";
	if (inInt == EXT_EVENT_START_GAME_NOTIF)    return "strntf";
	if (inInt == EXT_EVENT_DEAL_CARD_NOTIF)     return "dlcntf";
	if (inInt == EXT_EVENT_USER_LEAVE_NOTIF)    return "lrntf";
	if (inInt == EXT_EVENT_END_GAME_NOTIF)      return "endntf";
	if (inInt == EXT_EVENT_AMF_TEST_NOTIF)      return "cblltf";
	if (inInt == EXT_EVENT_VICTORY_NOTIF)       return "vicntf";
	if (inInt == EXT_EVENT_NEAD_PLAY_NOTIF)     return "ndpntf";
	if (inInt == EXT_EVENT_PLAY_CARD_NOTIF)     return "plcntf";
	if (inInt == EXT_EVENT_GAME_CHANGE_NOTIF)   return "gchntf";
	if (inInt == EXT_EVENT_PASS_CARD_NOTIF)     return "pscntf";
	if (inInt == EXT_EVENT_PLAY_CARD_RES)       return "plcrs";
	if (inInt == EXT_EVENT_UNREADY_REQ)         return "urr";
	if (inInt == EXT_EVENT_READY_REQ)           return "rr";
	if (inInt == EXT_EVENT_PLAY_CARD_REQ)       return "plcrq";
	if (inInt == EXT_EVENT_PASS_CARD_REQ)       return "pscrq";
	if (inInt == EXT_EVENT_HIT_CARD_ERROR)      return "pscrs";
	if (inInt == EXT_EVENT_START_READY_TIMER_NOTIF)      return "rdtntf";
	if (inInt == EXT_EVENT_CHICKEN_UPDATE_NTF)	return "chkntf";
	return "";
}

LayerPlayGameTLMN::LayerPlayGameTLMN() {


}

LayerPlayGameTLMN::~LayerPlayGameTLMN() {
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGameTLMN::updateEvent),this);
	GameServer::getSingleton().removeListeners(this);
	if (mThreadQueueMsg != NULL) delete mThreadQueueMsg;
	log("~LayerPlayGameTLMN");
	arrName.clear();
	arrMoney.clear();
	arrMoneyDouble.clear();
	arrIDTurn.clear();
	//this->removeAllComponents();
	//layerButtons->removeFromParentAndCleanup(true);
	this->removeChild(layerCards);
	this->removeAllChildrenWithCleanup(true);
}

void LayerPlayGameTLMN::createBackgrounds() {
	layerBackGround = BackgroundInGame::create();
	this->addChild(layerBackGround);

	nameGame = Label::createWithSystemFont("", "", kNormalFontSize);
	nameGame->setPosition(Vec2(400 - 5, 213 + 10));
	nameGame->setColor(Color3B::WHITE);
	nameGame->setOpacity(150);
	layerBackGround->addChild(nameGame);

	//////////////////////////////////////////////////////////////////////////
	// thông tin bàn chơi và mức cược
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() != NULL){
		int id = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0, 3).c_str());
		boost::shared_ptr<RoomVariable> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params");
		if (param != NULL){
			string paramString = param->GetStringValue()->c_str();
			vector<string> arrInfo = split(paramString, '@');
			string money = arrInfo.at(0);

			string *name = mUtils::getGameNameByID(gameID);
			string moneyConvert = mUtils::convertMoneyEx(atoi(money.c_str()));

			string result = "";
			if (name->length() > 0 && moneyConvert.length() > 0)
			{
				result = *name;
				result += (" - "+dataManager.GetSysString(745)+":" + moneyConvert);
			}
			nameGame->setString(result.c_str());
		}
	}
}

void LayerPlayGameTLMN::createAvatars() {
	layerAvatars = LayerAvatarInGame::create();
	layerAvatars->getUserByPos(kUserBot)->setCanTouch(false);
	this->addChild(layerAvatars);
}

void LayerPlayGameTLMN::createButtons() {
	layerButtons = LayerButtonInGame::create();
	this->addChild(layerButtons);

	int widButton = 129;
	int heiButton = 44;
	int space = 10;

	Button *btnReady = createButtonWithTitle_Position(dataManager.GetSysString(197).c_str(), Point(WIDTH_DESIGN - space, space));
	Button *btnUnReady = createButtonWithTitle_Position(dataManager.GetSysString(196).c_str(), Point(WIDTH_DESIGN - space, space));
	Button *btnSortCard = createButtonWithTitle_Position(dataManager.GetSysString(208).c_str(), Point(WIDTH_DESIGN - space * 2 - widButton, space));
	Button *btnHitCard = createButtonWithTitle_Position(dataManager.GetSysString(209).c_str(), Point(WIDTH_DESIGN - space * 3 - widButton * 2, space));
	Button *btnNextTurn = createButtonWithTitle_Position(dataManager.GetSysString(210).c_str(), Point(WIDTH_DESIGN - space, space));
	// Button *btnSitting = createButtonWithTitle_Position("Ngồi chơi", btnReady->getPosition());
	//Button *btnStandUp = createButtonWithTitle_Position("Đứng xem", Point(WIDTH_DESIGN - space, HEIGHT_DESIGN - space - heiButton));
	// 	Button *btnStandUp = Button::create();
	// 	btnStandUp->setTouchEnabled(true);
	// 	btnStandUp->setScale9Enabled(false);
	// 	btnStandUp->loadTextures("menu_standup.png", "menu_standup_hover.png", "");
	// 	btnStandUp->setAnchorPoint(Point(1, 0));
	// 	btnStandUp->setPosition(Point(WIDTH_DESIGN - space, HEIGHT_DESIGN - space - heiButton));
	// 	btnStandUp->setScale(0.5);


	btnReady->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameTLMN::actionReady, this));
	btnUnReady->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameTLMN::actionUnReady, this));
	btnSortCard->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameTLMN::actionSortCards, this));
	btnHitCard->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameTLMN::actionHitCards, this));
	btnNextTurn->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameTLMN::actionNextTurn, this));
	// btnSitting->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameTLMN::actionSitting));
	//btnStandUp->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameTLMN::actionStandUp));

	btnReady->setTag(kTagButtonReady);
	btnUnReady->setTag(kTagButtonUnReady);
	btnSortCard->setTag(kTagButtonSort);
	btnHitCard->setTag(kTagButtonHit);
	btnNextTurn->setTag(kTagButtonNextTurn);
	//	btnSitting->setTag(kTagButtonSitting);
	//btnStandUp->setTag(kTagButtonStandUp);

	btnSortCard->setVisible(false);
	btnUnReady->setVisible(false);
	btnHitCard->setVisible(false);
	btnNextTurn->setVisible(false);
	btnReady->setVisible(false);
	//btnSitting->setVisible(false);
	//btnStandUp->setVisible(false);

	layerButtons->addWidgetEx(btnReady);
	layerButtons->addWidgetEx(btnSortCard);
	layerButtons->addWidgetEx(btnHitCard);
	layerButtons->addWidgetEx(btnNextTurn);
	//	layerButtons->addWidgetEx(btnSitting);
	//layerButtons->addWidgetEx(btnStandUp);
	layerButtons->addWidgetEx(btnUnReady);

}

void LayerPlayGameTLMN::createNumbers() {
	layerNumbers = LayerNumberInGame::create();
	this->addChild(layerNumbers);
}

void LayerPlayGameTLMN::createCards() {
	layerCards = LayerCardInGame::create();
	layerCards->initCardByTienLen();
	this->addChild(layerCards);
}

void LayerPlayGameTLMN::createChats() {

//	layerChats = LayerChatInGame::create();
//	this->addChild(layerChats);

}

void LayerPlayGameTLMN::initGame() {
	// khởi tạo các giá trị ban đầu hoặc hiển thị các thông tin cần thiết

	layerCards->resetGame();
	// thông tin tiền hiện tại của Users
	//     for (int i = 0; i < arrName.size(); i++) {
	//         layerAvatars->setMoney(layerAvatars->getPosByName(arrName[i]), arrMoneyDouble[i]);
	// 		// log("name: %s,pos: %d",arrName[i].c_str(),layerAvatars->getPosByName(arrName[i]));
	// 	}
	// 	arrName.clear();
	// 	arrMoney.clear();
	// 	arrMoneyDouble.clear();
}

void LayerPlayGameTLMN::playSound(string soundPath)
{
	if (mUtils::isSoundOn())
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundPath.c_str());
}

Button* LayerPlayGameTLMN::createButtonWithTitle_Position(const char *title, Point pPoint) {
	// Create the button
	Button* button = Button::create();
	button->setTouchEnabled(true);
	button->setScale9Enabled(false);
	button->loadTextures("ready.png", "ready_selected.png", "");
	//button->setTitleFontName(kNormalFont);
	button->setTitleText(title);
	button->setTitleColor(Color3B::BLACK);
	button->setTitleFontSize(button->getContentSize().height / 2);
	button->setAnchorPoint(Point(1, 0));
	button->setTitleAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	button->setPosition(Vec2(pPoint.x + sizeAd.width, pPoint.y - sizeAd.height));

	return button;
}

Button* LayerPlayGameTLMN::getButtonByTag(int tag) {
	Button *button = (Button*) this->layerButtons->getChildByTag(tag);
	return button;
}

bool LayerPlayGameTLMN::isSpectator() {
	vector<string> arr = split(this->myListUser, TL_CHAR_SPLIT_1);
	int size = arr.size();
	for (int i = 0; i < size; i++){
		vector<string> arrInfo = split(arr.at(i), TL_CHAR_SPLIT_2);
		if (arrInfo.size() < 2) continue;
		string name = arrInfo.at(1);
		log("LayerPlayGameTLMN::isSpectator(): name= %s", name.c_str());
		if (name == myName) return false;
	}

	return true;
}

void LayerPlayGameTLMN::actionReady(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
// 		string listID = "18,22,25";
// 		logicTienLenMN->setPreviousCard("13,17,21");
// 		if (!logicTienLenMN->checkPlay(listID)){
// 			layerChats->showChatByPos(-1, "Bài đánh không hợp lệ - client");
// 			return;
// 		}
// 		else{
// 			layerChats->showChatByPos(-1, "Bài đánh hợp lệ - client");
// 			return;
// 		}
// 		return;
		//resetGame();
		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(convertResponseToString(EXT_EVENT_READY_REQ), parameter, lastRoom));
		GameServer::getSingleton().Send(request);

	}
}

void LayerPlayGameTLMN::actionSortCards(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		layerCards->actionSortCardByTienLen();
	}
}

void LayerPlayGameTLMN::actionHitCards(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		log("danh bai clicked");
		vector<int> arrID = layerCards->getIdCardByClicked();

		string listID = "";
		for (int i = 0; i < arrID.size(); i++) {
			if (i != 0) {
				listID += ",";
			}
			ostringstream os;
			log("ID quan bai danh la: %d", arrID[i]);
			os << arrID[i];
			listID += os.str();
		}

		// dang thu danh truoc
		//if (previousCards.size() > 0){
		//neu la nguoi danh sau moi kiem tra
		logicTienLenMN->setPreviousCard(previousCards);
		if (!logicTienLenMN->checkPlay(listID)){
			//layerChats->showChatByPos(-1, dataManager.GetSysString(239));
//			layerChats->showChatByPos(-1, "Bài đánh ko hợp lệ client");
			return;
		}
		//	}
		getButtonByTag(kTagButtonHit)->setVisible(false);
		vector<int> listcard = layerCards->getIDFromString_TienLen(listID);
		layerCards->actionHitCardByTienLen(kUserMe, listcard);

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString("lc", listID);
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(convertResponseToString(EXT_EVENT_PLAY_CARD_REQ), params, lastRoom));

		GameServer::getSingleton().Send(request);

	}
}

void LayerPlayGameTLMN::actionNextTurn(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		//getButtonByTag(kTagButtonNextTurn)->setVisible(false);
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(convertResponseToString(EXT_EVENT_PASS_CARD_REQ), params, lastRoom));

		GameServer::getSingleton().Send(request);
	}
}

void LayerPlayGameTLMN::actionSitting(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		//((Button*) pSender)->setVisible(false);
		boost::shared_ptr<IRequest> request(new SpectatorToPlayerRequest());
		GameServer::getSingleton().Send(request);
	}
}

void LayerPlayGameTLMN::actionStandUp(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		getButtonByTag(kTagButtonStandUp)->setVisible(false);
		isSpector = layerAvatars->isSpectator();
		isStartedGame = layerAvatars->isStartedGame();
		if (!isSpector) {
			if (isStartedGame) {
				if (!isRegistStandUp){
//					layerChats->showChatByPos(-1, dataManager.GetSysString(237));
					isRegistStandUp = true;
				}

			}
			// 			else {
			// 				boost::shared_ptr<IRequest> request(new PlayerToSpectatorRequest());
			// 				GameServer::getSingleton().Send(request);
			// 				isRegistStandUp = false;
			// 				layerTimer->StopCountDownReady();
			// 			}
		}

	}
}

void LayerPlayGameTLMN::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	pObjs->PutUtfString("_cmd_", *cmd);
	//only add to queues
	mThreadQueueMsg->addQueue(pObjs);
}

void LayerPlayGameTLMN::OnExtensionResponseProcess() {
	this->param = mThreadQueueMsg->deQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");

	if (strcmp("hbc", cmd->c_str()) == 0)
		return;
	else
		log("CMD: %s", cmd->c_str());

	// 	boost::shared_ptr<string> listCard = param->GetUtfString("lc");
	// 	if (listCard != NULL){
	// 		if (this->layerCards->checkCardIsRunning(listCard->c_str()))
	// 			return;
	// 	}

	switch (convertResponseToInt(cmd->c_str())) {
	case EXT_EVENT_USER_JOIN_NOTIF:
		event_EXT_EVENT_USER_JOIN_NOTIF();
		break;

	case EXT_EVENT_LISTCARD_NTF:
		event_EXT_EVENT_LISTCARD_NTF();
		break;

	case EXT_EVENT_ERROR_READY_NTF:
		event_EXT_EVENT_ERROR_READY_NTF();
		break;

	case EXT_EVENT_USER_READY_NOTIF:
		event_EXT_EVENT_USER_READY_NOTIF();
		break;

	case EXT_EVENT_UNREADY_NTF:
		event_EXT_EVENT_UNREADY_NTF();
		break;

	case EXT_EVENT_READY_RES:
		event_EXT_EVENT_READY_RES();
		break;

	case EXT_EVENT_LIST_USER_UPDATE:
		event_EXT_EVENT_LIST_USER_UPDATE();
		break;

	case EXT_EVENT_START_GAME_NOTIF:
		event_EXT_EVENT_START_GAME_NOTIF();
		break;

	case EXT_EVENT_DEAL_CARD_NOTIF:
		event_EXT_EVENT_DEAL_CARD_NOTIF();
		break;

	case EXT_EVENT_USER_LEAVE_NOTIF:
		event_EXT_EVENT_USER_LEAVE_NOTIF();
		break;

	case EXT_EVENT_END_GAME_NOTIF:
		event_EXT_EVENT_END_GAME_NOTIF();
		break;

	case EXT_EVENT_AMF_TEST_NOTIF:
		event_EXT_EVENT_AMF_TEST_NOTIF();
		break;

	case EXT_EVENT_VICTORY_NOTIF:
		event_EXT_EVENT_VICTORY_NOTIF();
		break;

	case EXT_EVENT_NEAD_PLAY_NOTIF:
		event_EXT_EVENT_NEAD_PLAY_NOTIF();
		break;

	case EXT_EVENT_PLAY_CARD_NOTIF:
		event_EXT_EVENT_PLAY_CARD_NOTIF();
		break;

	case EXT_EVENT_GAME_CHANGE_NOTIF:
		event_EXT_EVENT_GAME_CHANGE_NOTIF();
		break;

	case EXT_EVENT_PASS_CARD_NOTIF:
		event_EXT_EVENT_PASS_CARD_NOTIF();
		break;

	case EXT_EVENT_PLAY_CARD_RES:
		event_EXT_EVENT_PLAY_CARD_RES();
		break;
	case EXT_EVENT_HIT_CARD_ERROR:
		event_EXT_EVENT_HIT_CARD_ERROR();
		break;
	case EXT_EVENT_START_READY_TIMER_NOTIF:
		layerTimer->StartCountDownReady();
		break;
	case EXT_EVENT_CHICKEN_UPDATE_NTF:
		this->event_EXT_EVENT_CHICKEN_UPDATE_NTF();
		break;
	}
	if (strcmp("expmntf", cmd->c_str()) == 0)
	{
		boost::shared_ptr<string> uid = param->GetUtfString("uid");
		if (uid != NULL)
		{
			if (uid->compare(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0)
			{
//				layerChats->showChatByPos(-1, "Không đủ tiền để chơi!");
				layerButtons->eventTouchBtnBack(NULL, Widget::TouchEventType::ENDED);

			}
		}
	}
	if (strcmp("kkntf", cmd->c_str()) == 0){ ////EXT_EVENT_USER_KICKED_NOTIF    = "kkntf";
		boost::shared_ptr<string> uid = param->GetUtfString("uid");
		if (uid->compare(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0)
			layerButtons->BackToChooseDesk();
	}

	//mThreadQueueMsg->deQueue();

}

void LayerPlayGameTLMN::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	log("Update User Variables");
	// 	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	// 	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	// 	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	// 	if (ptrNotifiedUser != NULL){
	// 		boost::shared_ptr<UserVariable> amf = ptrNotifiedUser->GetVariable("amf");
	// 
	// 		if (amf == NULL) return;
	// 		//string money = boost::to_string(*ptrNotifiedUser->GetVariable("amf")->GetDoubleValue());
	// 		int    money = (int)(*amf->GetDoubleValue());
	// 		double moneyDouble = (*amf->GetDoubleValue());
	// 		string name = boost::to_string(*ptrNotifiedUser->Name());
	// 
	// 		log("OnSmartFoxUserVariableUpdate: name= %s, money= %d", name.c_str(), money);
	// 		int pos = layerAvatars->getPosByName(name.c_str());
	// 		if (pos == -1) return;
	// 		// 	layerAvatars->getUserByPos(pos)->setMoney(money);
	// 		// 	layerAvatars->getUserByPos(pos)->setMoney(moneyDouble);
	// 
	// 		//     arrName.push_back(name);
	// 		//     arrMoney.push_back(money);
	// 		// 	arrMoneyDouble.push_back(moneyDouble);
	// 		log("size: %d", arrName.size());
	// 	}

}

void LayerPlayGameTLMN::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);
	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> ptrNotifiedMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);
    if (dataManager.getAppConfig().is24h)
    {
        return;
    }
	//
	log("ptrNotifiedMessage: %s", ptrNotifiedMessage->c_str());
	//
	int vt = layerAvatars->getPosByName(ptrNotifiedUser->Name()->c_str());
//	layerChats->showChatByPos(vt, ptrNotifiedMessage->c_str());
}

void LayerPlayGameTLMN::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	//
	log("user leave!");
	if (ptrNotifiedUser->IsItMe()){
		layerButtons->eventTouchBtnBack(NULL, Widget::TouchEventType::ENDED);
	}
}

void LayerPlayGameTLMN::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	boost::shared_ptr<Room> room = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	//
	boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
	if (rv != NULL){
		string s = *rv->GetStringValue();
		log("Room %s update RoomVariables: %s", room->Name()->c_str(), s.c_str());

		vector<string> lstBet = mUtils::splitString(s, '@');
		lstBet.at(1).compare("1") == 0 ? (isStartedGame = true) : (isStartedGame = false);
	}

}

void LayerPlayGameTLMN::event_EXT_EVENT_USER_JOIN_NOTIF(){
	log("EXT_EVENT_USER_JOIN_NOTIF");
	boost::shared_ptr<double> tienga = param->GetDouble("chkvl");
	boost::shared_ptr<long> lv_ga = param->GetInt("chklv");
	boost::shared_ptr<string> listUser = param->GetUtfString("lu");
	myListUser = listUser->c_str();
	log("listUser= %s", listUser->c_str());
	if (listUser != NULL) {
		//lay thong tin ve ga
		layerAvatars->setListUserByTienLen(myListUser);
		isStartedGame = layerAvatars->isStartedGame();
		isSpector = layerAvatars->isSpectator();

		if (tienga != NULL && lv_ga != NULL && *tienga > 0 && isNuoiGa && *lv_ga > 0 && listUser != NULL){
			layerGa->setChickenMoney(*tienga);
			layerGa->setChickenLv(*lv_ga);
			layerGa->setListUser(*listUser, TL_CHAR_SPLIT_1, TL_CHAR_SPLIT_2);
		}
		if (strcmp(find_ChuPhong(myListUser).c_str(), myName.c_str()) == 0 && !isStartedGame ){// neu la chu phong thi moi hien nut bat dau
			getButtonByTag(kTagButtonReady)->setVisible(true);
		}

		if (isSpector) nameGame->setString("Bạn đang xem...");

		if (isStartedGame && !isSpector) {
			getButtonByTag(kTagButtonSort)->setVisible(true);
		}
		//	else {
		//layerChats->showChatByPos(-1, "Cập nhật lại bàn chơi");
		//if (!isSpector) {
		//getButtonByTag(kTagButtonReady)->setVisible(true);
		// 				vector<string> lst = mUtils::splitString(myListUser, TL_CHAR_SPLIT_1);
		// 				for (int i = 0; i < lst.size(); i++)
		// 				{
		// 					vector<string> info = mUtils::splitString(lst[i], TL_CHAR_SPLIT_2);
		// 					if (strcmp(info[1].c_str(), myName.c_str()) == 0)
		// 					if (strcmp(info[3].c_str(), "0") == 0 && strcmp(find_ChuPhong(myListUser).c_str(), myName.c_str()) != 0){
		// 						layerTimer->StartCountDownReady();
		// 					}
		// 				}
		// 				if (strcmp(myName.c_str(), (find_ChuPhong(myListUser).c_str())) == 0)
		// 				{
		// 					getButtonByTag(kTagButtonReady)->setTitleText(dataManager.GetSysString(197));
		// 					layerTimer->StopCountDownReady();
		// 				}
		// 				else
		// 					getButtonByTag(kTagButtonReady)->setTitleText(dataManager.GetSysString(195));
		// 			}
		// 			else {
		// 				boost::shared_ptr<IRequest> request(new PlayerToSpectatorRequest());
		// 				GameServer::getSingleton().Send(request);
		// 				isRegistStandUp = false;
		// 				layerButtons->SetIsGuess(true);
		// 				getButtonByTag(kTagButtonReady)->setVisible(false);
		// 				if (isStartedGame)
		// 					layerAvatars->setUnReadyAllUser();
		// 
		// 			}
		//	}
		//if (isStartedGame || isSpector) layerTimer->StopCountDownReady();
	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_LISTCARD_NTF(){
	log("EXT_EVENT_LISTCARD_NTF");
}

void LayerPlayGameTLMN::event_EXT_EVENT_ERROR_READY_NTF(){
	log("event_EXT_EVENT_ERROR_READY_NTF");
}

void LayerPlayGameTLMN::event_EXT_EVENT_USER_READY_NOTIF(){
	// 	boost::shared_ptr<string> fname = param->GetUtfString("uid");
	// 
	// 	//     if (fname != NULL) {
	// 	//        // log("EXT_EVENT_USER_READY_NOTIF name= %s", fname->c_str());
	// 	//         
	// 	//         layerAvatars->setReady(layerAvatars->getPosByName(fname->c_str()), true);
	// 	//         if (fname->c_str() == this->myName && strcmp("Sẵn sàng", getButtonByTag(kTagButtonReady)->getTitleText()) == 0) {
	// 	//             getButtonByTag(kTagButtonReady)->setTitleText("Đã sẵn sàng");
	// 	//         }
	// 	//     }
	// 	if (strcmp(fname->c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0){
	// 		getButtonByTag(kTagButtonReady)->setVisible(false);
	// 		layerTimer->StopCountDownReady();
	// 		// 		// hiện unready
	// 		// 		getButtonByTag(kTagButtonUnReady)->setVisible(true);
	// 		layerAvatars->getUserByPos(kUserMe)->setReady(true);
	// 	}
	// 	else{
	// 		switch (layerAvatars->getPosByName(fname->c_str())) {
	// 		case kUserLeft:
	// 			layerAvatars->getUserByPos(kUserLeft)->setReady(true);
	// 			break;
	// 		case kUserRight:
	// 			layerAvatars->getUserByPos(kUserRight)->setReady(true);
	// 			break;
	// 		case kUserTop:
	// 			layerAvatars->getUserByPos(kUserTop)->setReady(true);
	// 			break;
	// 		default:
	// 			break;
	// 		}
	// 	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_UNREADY_NTF(){
	boost::shared_ptr<string> fname = param->GetUtfString("uid");
	//    // log("EXT_EVENT_UNREADY_NTF name= %s", fname->c_str());

	if (strcmp(fname->c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0){
		getButtonByTag(kTagButtonUnReady)->setVisible(false);
		layerTimer->StartCountDownReady();
		// hiện unready
		getButtonByTag(kTagButtonReady)->setVisible(true);
		layerAvatars->getUserByPos(kUserMe)->setReady(false);
	}
	else{
		switch (layerAvatars->getPosByName(fname->c_str())) {
		case kUserLeft:
			layerAvatars->getUserByPos(kuser3)->setReady(false);
			break;
		case kUserRight:
			layerAvatars->getUserByPos(kUserRight)->setReady(false);
			break;
		case kUserTop:
			layerAvatars->getUserByPos(kUserTop)->setReady(false);
			break;
		default:
			break;
		}
	}
	if (strcmp(getButtonByTag(kTagButtonReady)->getTitleText().c_str(), dataManager.GetSysString(197).c_str()) == 0)
		layerTimer->StopCountDownReady();
}

void LayerPlayGameTLMN::event_EXT_EVENT_READY_RES(){
	boost::shared_ptr<long> rs = param->GetInt("rslt");

	if (rs != NULL) {
		log("event_EXT_EVENT_READY_RES %ld", *rs.get());

		int rscode = *rs.get();
		if (rscode != 0) {
//			layerChats->showChatByPos(-1, dataManager.GetSysString(57));
		}
	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_LIST_USER_UPDATE(){
	// 	boost::shared_ptr<string> listUser = param->GetUtfString("lu");
	// 	myListUser = listUser->c_str();
	// 	if (listUser == NULL) return;
	// 
	// 	log("event_EXT_EVENT_LIST_USER_UPDATE %s", listUser->c_str());
	// 	//if (this->listUser == listUser->c_str()) return;
	// 
	// 	this->listUser = (listUser->c_str());
	// 	layerAvatars->setListUserByTienLen(this->listUser);
	// 
	// 	//layerCards->resetGame();
	// 
	// 	if (strcmp(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str(), (find_ChuPhong(listUser->c_str()).c_str())) == 0)
	// 	{
	// 		getButtonByTag(kTagButtonReady)->setTitleText(dataManager.GetSysString(197));
	// 		layerTimer->StopCountDownReady();
	// 	}
	// 	else
	// 	{
	// 		getButtonByTag(kTagButtonReady)->setTitleText(dataManager.GetSysString(195));
	// 		layerTimer->StartCountDownReady();
	// 	}
	// 	// Check: Vào lại bàn đang chơi dở (isStartedGame=true, isSpector=false)
	// 	isStartedGame = layerAvatars->isStartedGame();
	// 	isSpector = layerAvatars->isSpectator();
	// 	if (isStartedGame && !isSpector) {
	// 		//layerChats->showChatByPos(-1, "Xây dựng lại bàn chơi");
	// 		getButtonByTag(kTagButtonReady)->setVisible(false);
	// 		getButtonByTag(kTagButtonSort)->setVisible(true);
	// 		layerTimer->StopCountDownReady();
	// 	}
	// 	else {
	// 		//layerChats->showChatByPos(-1, "Cập nhật lại bàn chơi");
	// 		if (!isSpector) {
	// 			getButtonByTag(kTagButtonReady)->setVisible(true);
	// 			layerAvatars->getUserByPos(kUserBot)->setCanTouch(false);
	// 		}
	// 		else {
	// 
	// 			layerAvatars->getUserByPos(kUserBot)->setCanTouch(true);
	// 		}
	// 	}
	// 	if (isSpector){
	// 		getButtonByTag(kTagButtonReady)->setVisible(false);
	// 		getButtonByTag(kTagButtonSort)->setVisible(false);
	// 		getButtonByTag(kTagButtonUnReady)->setVisible(false);
	// 	}
	// 	if (!isStartedGame)
	// 		getButtonByTag(kTagButtonSort)->setVisible(false);

}

void LayerPlayGameTLMN::event_EXT_EVENT_START_GAME_NOTIF(){
	log("event_EXT_EVENT_START_GAME_NOTIF");
	Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(LayerPlayGameTLMN::resetCard),this);
	this->resetGame();
	layerTimer->StopCountDownReady();
	// hiển thị số bài trên tay của mỗi người (default=13)
	vector<string> lst = mUtils::splitString(myListUser, TL_CHAR_SPLIT_1);
	for (int i = 0; i < lst.size(); i++)
	{
		vector<string> info = mUtils::splitString(lst[i], TL_CHAR_SPLIT_2);
		layerCards->setCountCardByPos(layerAvatars->getPosByName(info[1].c_str()), 13);
	}
	// unready all
	layerAvatars->setListUserByTienLen(myListUser);
	//layerAvatars->setUnReadyAllUser();
	isStartedGame = true;
	getButtonByTag(kTagButtonReady)->setVisible(false);
	if (!isSpector) {
		getButtonByTag(kTagButtonSort)->setVisible(true);
	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_DEAL_CARD_NOTIF(){
	//    // Danh sách bài (chỉ sử dụng cho lần đầu)
	//    //Ti.App.EXT_FIELD_CARD_OPEN = "cropn";
	//    var isCardOpen = params.cropn;
	//    //EXT_FIELD_LIST_CARD = "lc";
	//    var lc = params.lc;
	//    var uid = params.uid;
	log("event_EXT_EVENT_DEAL_CARD_NOTIF");
	boost::shared_ptr<string> listCard = param->GetUtfString("lc");

	if (listCard != NULL) {
		log("listCard= %s", listCard->c_str());
		layerCards->actionDealCard(layerCards->getIDFromString_TienLen(listCard->c_str()));
		playSound("e_card.mp3");
	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_USER_LEAVE_NOTIF(){
	boost::shared_ptr<string> listUser = param->GetUtfString("lu");
	boost::shared_ptr<string> uid = param->GetUtfString("uid");

	if (listUser != NULL){
		myListUser = listUser->c_str();
		log("event_EXT_EVENT_USER_LEAVE_NOTIF");
		log("listUser= %s", listUser->c_str());

		layerAvatars->setListUserByTienLen(listUser->c_str());
		// 		if (strcmp(myName.c_str(), (find_ChuPhong(myListUser).c_str())) == 0)
		// 		{
		// 			// 			layerAvatars->getUserByPos(kUserMe)->setReady(false);
		// 			// 			getButtonByTag(kTagButtonUnReady)->setVisible(false);
		// 			getButtonByTag(kTagButtonReady)->setVisible(true);
		// 			// 			getButtonByTag(kTagButtonReady)->setTitleText(dataManager.GetSysString(197));
		// 			// 			layerTimer->StopCountDownReady();
		// 		}
		// 		else
		// 			getButtonByTag(kTagButtonReady)->setTitleText(dataManager.GetSysString(195));
	}
	//layerCards->resetGame();

	// 		Check: Vào lại bàn đang chơi dở (isStartedGame=true, isSpector=false)
	// 			isStartedGame = layerAvatars->isStartedGame();
	// 			isSpector = layerAvatars->isSpectator();
	// 			if (isStartedGame && !isSpector) {
	// 				getButtonByTag(kTagButtonSort)->setVisible(true);
	// 			}
	// 			else {
	// 				//layerChats->showChatByPos(-1, "Cập nhật lại bàn chơi");
	// 				if (!isSpector) {
	// 					getButtonByTag(kTagButtonReady)->setVisible(true);
	// 					layerAvatars->getUserByPos(kUserBot)->setCanTouch(false);
	// 				}
	// 				else {
	// 					nameGame->setString("Bạn đang xem...");
	// 					getButtonByTag(kTagButtonReady)->setVisible(false);
	// 					layerTimer->StopCountDownReady();
	// 					layerButtons->SetIsGuess(true);
	// 					layerAvatars->getUserByPos(kUserBot)->setCanTouch(true);
	// 					if (isStartedGame)
	// 						layerAvatars->setUnReadyAllUser();
	// 				}
	// 			}
	// 		}
	// 		if (isNuoiGa){
	// 			const char c1 = TL_CHAR_SPLIT_1;
	// 			const char c2 = TL_CHAR_SPLIT_2;
	// 			layerGa->setVisible(true);
	// 			layerGa->setListUser(myListUser, c1, c2);
	// 		}
}

void LayerPlayGameTLMN::event_EXT_EVENT_END_GAME_NOTIF(){
	log("event_EXT_EVENT_END_GAME_NOTIF");
	// 	if (strcmp(myName.c_str(), (find_ChuPhong(myListUser).c_str())) == 0)
	// 	{
	// 		getButtonByTag(kTagButtonReady)->setTitleText(dataManager.GetSysString(197));
	// 	}
	// 	else
	// 	{
	// 		getButtonByTag(kTagButtonReady)->setTitleText(dataManager.GetSysString(195));
	// 	}
	isStartedGame = false;

	layerAvatars->stopAllTimer();
	layerCards->setGameStarted(false);

	getButtonByTag(kTagButtonSort)->setVisible(false);
	getButtonByTag(kTagButtonHit)->setVisible(false);
	getButtonByTag(kTagButtonNextTurn)->setVisible(false);
	//getButtonByTag(kTagButtonReady)->setVisible(true);
	getButtonByTag(kTagButtonUnReady)->setVisible(false);
	//getButtonByTag(kTagButtonStandUp)->setVisible(false);

	arrIDTurn.clear();
	//this->scheduleOnce(CC_SCHEDULE_SELECTOR(LayerPlayGameTLMN::resetCard), TIME_RESET_GAME);
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameTLMN::resetCard), this, TIME_RESET_GAME, 1, TIME_RESET_GAME, false);
	isSpector = layerAvatars->isSpectator();
	layerTimer->StartCountDownReady();
	// 	if (isSpectator())
	// 		resetGame();
	// 	else{
	// 		//getButtonByTag(kTagButtonReady)->setVisible(true);
	// // 		if (strcmp(getButtonByTag(kTagButtonReady)->getTitleText().c_str(),
	// // 			dataManager.GetSysString(197)) == 0)
	// // 			layerTimer->StopCountDownReady();
	// // 		else
	// 			
	// 	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_AMF_TEST_NOTIF(){
	//    // sự kiện công/trừ tiền
	//    var uid = params.uid;
	//    var money = params.amf;

	boost::shared_ptr<string> name = param->GetUtfString("uid");
	int money = (int)(*(param->GetDouble("amf")));
	int betvl = (int)(*(param->GetDouble("betvl")));
	log("BETVL:%d money:%d", betvl, money);
	int resson = -1;

	if ((param->GetInt("cbt")) != NULL){
		resson = (int)(*(param->GetInt("cbt")));
		log("resson = %d", resson);
	}

	log("event_EXT_EVENT_AMF_TEST_NOTIF");
	if (name != NULL && money != NULL) {
		log("name= %s, money= %d", name->c_str(), money);

		int pos = layerAvatars->getPosByName(name->c_str());
		std::string res;
		if (money > 0)
			res = "+" + boost::to_string(money);
		else
			res = boost::to_string(money);
		layerNumbers->showNumberByPos(pos, res);

		if (resson <= 0) return;
		string ressonString = "";
		switch (resson) {
		case 1:
			ressonString = "thối 2";
			break;
		case 2:
			//ressonString = "người thua bị thối 2";
			break;
		case 3:
			ressonString = "chặn được 2";
			break;
		case 4:
			ressonString = "bị bắt 2";
			break;
		case 5:
			ressonString = "chặn lại 2";
			break;
		case 8:// tru tien ghop ga
			ressonString = "Góp gà";
			break;
			// 		case 9:// cong tien khi an ga
			// 			ressonString = "";
			// 			break;
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
		}
		log("resson=%d, ressonString=%s", resson, ressonString.c_str());
		if (ressonString.length() > 0) {
			if (resson == 10 || resson == 11 || resson == 12 || resson == 13){
				if (strcmp(name->c_str(), this->myName.c_str()) == 0 && money > 0
					|| strcmp(name->c_str(), this->myName.c_str()) != 0 && money > 0){
//					layerChats->showChatByPos(pos, ressonString);
				}

			}
			else{
//				layerChats->showChatByPos(pos, ressonString);
			}
		}
		if (resson == 9 && isNuoiGa) // an ga, thi hien thi tien bay
		{
			layerGa->runActionAnGa(layerAvatars->getUserByPos(pos)->getPosition(), 1);
		}
	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_VICTORY_NOTIF(){
	//    var uid = params.uid;
	//    var vicPos = params.vtps;

	boost::shared_ptr<string> name = param->GetUtfString("uid");
	boost::shared_ptr<long> vicPos = param->GetInt("vtps");
	boost::shared_ptr<string> listCard = param->GetUtfString("lc");

	log("event_EXT_EVENT_VICTORY_NOTIF");

	if (name != NULL && vicPos != NULL) {
		log("name= %s, vicpos= %ld", name->c_str(), *vicPos.get());

		if (name->c_str() == myName){
			getButtonByTag(kTagButtonSort)->setVisible(false);
			getButtonByTag(kTagButtonReady)->setVisible(false);
		}
		string result = "";
		switch (*vicPos.get()) {
		case -1:
			result = dataManager.GetSysString(477);
			break;
		case 1:
			result = dataManager.GetSysString(629);

			if (name->c_str() == myName)
			{
				playSound("thang_roi.mp3");

			}

			break;

		case 2:
			result = "Về nhì!";
			break;

		case 3:
			result = "Về ba!";
			break;

		case 4:
			result = dataManager.GetSysString(630);
			break;
		default:
			break;
		}

		layerAvatars->setMoney(layerAvatars->getPosByName(name->c_str()), result);
	}

	if (listCard != NULL && name != NULL)
	{
		// hiển thị quân bài trên tay của người chơi cuối
		string lc = listCard->c_str();
		if (lc.size() <= 0) return;
		if (name->c_str() != myName){
			//layerCards->hideCardByArr(arrIDTurn);
			arrIDTurn.clear();
			int pos = layerAvatars->getPosByName(name->c_str());
			layerCards->showCardOnHandByPos_List(pos, lc);
		}
	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_NEAD_PLAY_NOTIF(){
	// Lượt chơi
	boost::shared_ptr<string> name = param->GetUtfString("uid");
	boost::shared_ptr<long> isfr = param->GetInt("isfr"); /// bien kiem tra het 1 luot danh hay chua

	log("event_EXT_EVENT_NEAD_PLAY_NOTIF");

	if (name != NULL && isfr != NULL) {
		log("name= %s, isfr= %ld", name->c_str(), *isfr.get());

		// timer
		layerAvatars->runTimer(layerAvatars->getPosByName(name->c_str()));
		//set first round for logic client
		if (name->c_str() == myName) {
			// 			getButtonByTag(kTagButtonHit)->setVisible(true);
			// 
			if (*isfr.get() == 0) {
				// duoc phep bo luot
				//getButtonByTag(kTagButtonNextTurn)->setVisible(true);
				this->showButtonMyTurn(true);
			}
			else {
				//getButtonByTag(kTagButtonNextTurn)->setVisible(false);
				this->showButtonMyTurn(false);
			}

		}
		else {
			this->showButtonOtherTurn();
			// 			getButtonByTag(kTagButtonHit)->setVisible(false);
			// 			getButtonByTag(kTagButtonNextTurn)->setVisible(false);
		}

		// lượt mới: ẩn các quân đang có trên bàn và release mảng chứa
		isNewTurn = false;
		if (*isfr.get() == 1) {
			previousCards = "";
			logicTienLenMN->resetGame();
			isNewTurn = true;
			layerCards->hideCardByArr(arrIDTurn);
			layerCards->hideLastCard();
			arrIDTurn.clear();
		}
	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_PLAY_CARD_NOTIF(){
	// Danh sach quan bai danh ra
	boost::shared_ptr<string> name = param->GetUtfString("uid");
	boost::shared_ptr<string> listCard = param->GetUtfString("lc");
	boost::shared_ptr<long> countCard = param->GetInt("cc");

	log("event_EXT_EVENT_PLAY_CARD_NOTIF");

	if (name != NULL && listCard != NULL && countCard != NULL) {
		log("name= %s, listCard= %s, countCard= %ld", name->c_str(), listCard->c_str(), *countCard.get());
		previousCards = listCard->c_str();
		int pos = layerAvatars->getPosByName(name->c_str());
		// cap nhat so bai tren tay
		layerCards->setCountCardByPos(pos, *countCard.get());
		vector<int> arrID = layerCards->getIDFromString_TienLen(listCard->c_str());
		layerCards->actionHitCardByTienLen(pos, arrID);

		// mỗi khi có danh sách các quân bài đánh ra
		// đều đẩy ID vào mảng lưu trữ tạm thời (để dễ xử lý về sau)
		for (int i = 0; i < arrID.size(); i++) {
			arrIDTurn.push_back(arrID[i]);
		}
	}

	// am thanh
	if (isNewTurn) {
		vector<string> arr;
		arr.push_back("danh_di.mp3");
		arr.push_back("thach_danh.mp3");
		int index = rand() % arr.size();
		playSound(arr.at(index));
		arr.clear();
	}
	else {
		vector<string> arr;
		arr.push_back("chay_dang_troi.mp3");
		arr.push_back("chay_di_dau.mp3");
		arr.push_back("chay_di_dau_2.mp3");
		arr.push_back("con_nua_ne.mp3");
		int index = rand() % arr.size();
		playSound(arr.at(index));
		arr.clear();
	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_GAME_CHANGE_NOTIF(){
	// khi trở lại bàn chơi
	//1000|botbot11|0|42|55_botbot111_1_36, 14, 23, 24, 46, 21, 27, 31, 13, 55, 48;53_botbot11_0_-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1|4000.000000|1
	boost::shared_ptr<string> ginf = param->GetUtfString("ginf");
	getButtonByTag(kTagButtonReady)->setVisible(false);
	log("event_EXT_EVENT_GAME_CHANGE_NOTIF");
	if (ginf != NULL) {
		log("%s", ginf->c_str());
		// neu da nhay vao day
		//layerChats->showChatByPos(-1, "Dựng lại bàn chơi đã lưu");

		// bất chấp "bỏ lượt" :D
		//boost::shared_ptr<ISFSObject> params (new SFSObject());
		//boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		//boost::shared_ptr<IRequest> request (new ExtensionRequest(convertResponseToString(EXT_EVENT_PASS_CARD_REQ), params, lastRoom));
		//
		//GameServer::getSingleton().Send(request);

		string list = ginf->c_str();
		vector<string> arrList = layerCards->split(list, '|');
		vector<string> arrGa = layerCards->split(list, '@');
		vector<string> arrListUser;
		if (arrGa.size() >= 3){
			double tien_ga = std::atof(arrGa.at(1).c_str());
			int lv_ga = std::atoi(arrGa.at(2).c_str());
			layerGa->setChickenLv(lv_ga);
			layerGa->setChickenMoney(tien_ga);
		}

		if (arrList.size() >= 5) {
			string tienCuoc = arrList[0];
			string tenUserToiLuot = arrList[1];
			string isfr = arrList[2];
			string listUser_Card = arrList[4];
			layerAvatars->runTimer(layerAvatars->getPosByName(tenUserToiLuot));
			vector<string> arrInfo = split(listUser_Card, ';');
			//doTogether->split(listUser_Card, ';');
			//
			for (int i = 0; i < arrInfo.size(); i++) {
				vector<string> arr = split(arrInfo[i], TL_CHAR_SPLIT_2);
				//doTogether->split(arrInfo[i], TL_CHAR_SPLIT_2);
				//

				if (arr.size() < 4) break;

				string name = arr[1];
				string flag = arr[2];
				string listID = arr[3];
				arrListUser.push_back(name);
				// chu phong
				layerAvatars->setFlag(layerAvatars->getPosByName(name), (atoi(flag.c_str()) == 1));
				vector<int> arrID = layerCards->getIDFromString_TienLen(listID);
				int pos = layerAvatars->getPosByAccountID(name);

				// chia bai cho minh
				if (name == myName){
					layerCards->actionDealCardTL(arrID);
					if (tenUserToiLuot == myName) {

						if (arrID.size() > 0)
							getButtonByTag(kTagButtonHit)->setVisible(true);
						if (atoi(isfr.c_str()) == 0) {
							// dc phep bo luot
							getButtonByTag(kTagButtonNextTurn)->setVisible(true);
						}
					}
				}
				else {
					layerCards->setCountCardByPos(pos, arrID.size());
				}
			}
		}
		layerCards->showLastCard(list);
		///set previous card for logic
		vector<string> arr = split(list, '|');
		if (strcmp(arr.at(3).c_str(), "") != 0){
			previousCards = arr.at(3);
		}

		//
		if (isNuoiGa){
			layerGa->setVisible(true);
			if (this->isSpector) // neu la khach phai push mang nay
				layerGa->setListDirect(arrListUser);
			else
				layerGa->setListUser(myListUser, TL_CHAR_SPLIT_1, TL_CHAR_SPLIT_2);
		}
	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_PASS_CARD_NOTIF(){
	boost::shared_ptr<string> name = param->GetUtfString("uid");

	log("event_EXT_EVENT_PASS_CARD_NOTIF");
	if (name != NULL) {
		log("%s", name->c_str());
//		layerChats->showChatByPos(layerAvatars->getPosByName(name->c_str()), dataManager.GetSysString(210));
		playSound("em_tha_do.mp3");
		// 		if (name->c_str() == myName) {
		// 			getButtonByTag(kTagButtonNextTurn)->setVisible(false);
		// 			getButtonByTag(kTagButtonHit)->setVisible(false);
		// 		}
	}
}

void LayerPlayGameTLMN::event_EXT_EVENT_PLAY_CARD_RES(){
	boost::shared_ptr<long> rs = param->GetInt("rslt");

	log("event_EXT_EVENT_PLAY_CARD_RES");
	if (rs != NULL) {
		log("%ld", *rs.get());
		if (*rs.get() != 0) {
			//layerChats->showChatByPos(-1, dataManager.GetSysString(239));
//			layerChats->showChatByPos(-1, "Bài đánh ko hợp lệ sever");
		}
	}
}
void LayerPlayGameTLMN::event_EXT_EVENT_CHICKEN_UPDATE_NTF()
{
	// 	public static final String EXT_FIELD_CHICKEN_VALUE = "chkvl";
	// 	public static final String EXT_FIELD_CHICKEN_LEVEL = "chklv";
	if (isNuoiGa){
		double chicken_value = *param->GetDouble("chkvl");
		int chicken_lv = (int)(*param->GetInt("chklv"));
		layerGa->setChickenLv(chicken_lv);
		layerGa->setChickenMoney(chicken_value);
		layerGa->setVisible(true);
		layerGa->setListUser(myListUser, TL_CHAR_SPLIT_1, TL_CHAR_SPLIT_2);
	}
}

std::string LayerPlayGameTLMN::find_ChuPhong(string listUser)
{
	if (listUser == "") return "";
	vector<string> arrUser = split(listUser, TL_CHAR_SPLIT_1);
	vector<string> info = split(arrUser[0], TL_CHAR_SPLIT_2);
	if (info.size() <= 1) return "";
	string boosId = info[1];
	return boosId;
}

void LayerPlayGameTLMN::isWinner()
{
	ui::ImageView* win = ui::ImageView::create();
	win->loadTexture("pic/win.jpg");
	this->addChild(win);
	win->setPosition(this->getContentSize() / 2);
	win->runAction(Repeat::create(Sequence::create(FadeOut::create(0.5), FadeIn::create(0.5), NULL), 5));
}

void LayerPlayGameTLMN::event_EXT_EVENT_HIT_CARD_ERROR()
{
//	layerChats->showChatByPos(-1, "Bài đánh không hợp lệ");
}

void LayerPlayGameTLMN::actionUnReady(Ref *pSender, Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED){
		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(convertResponseToString(EXT_EVENT_UNREADY_REQ), parameter, lastRoom));
		GameServer::getSingleton().Send(request);

	}

}

void LayerPlayGameTLMN::resetGame()
{
	// khởi tạo các giá trị ban đầu hoặc hiển thị các thông tin cần thiết

	layerCards->resetGame();
	previousCards = "";
	// thông tin tiền hiện tại của Users
	for (int i = 0; i < arrName.size(); i++) {
		layerAvatars->setMoney(layerAvatars->getPosByName(arrName[i]), arrMoneyDouble[i]);
		log("name: %s,pos: %d", arrName[i].c_str(), layerAvatars->getPosByName(arrName[i]));
	}
}

void LayerPlayGameTLMN::createTimer()
{
	layerTimer = LayerCount::create();
	layerTimer->setTitleReady(dataManager.GetSysString(238));
	this->addChild(layerTimer);
	//layerTimer->StartCountDownReady();
}

bool LayerPlayGameTLMN::init()
{
	if (!Layer::init())
		return false;
	this->myName = SceneManager::getSingleton().getMyName();
	this->doTogether = new DoTogether();
	this->isRegistSittingDown = false;
	this->isRegistStandUp = false;



	sizeAd = mUtils::getSizePos();
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL)
		return false;
	gameID = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0, 3).c_str());

	logicTienLenMN = new LogicTienLenMN();
	logicTienLenMN->resetGame();
	logicTienLenMN->setGameID(gameID);

	this->setAnchorPoint(Point(0, 0));
	this->setPosition(Point(-WIDTH_DESIGN / 2, -HEIGHT_DESIGN / 2));

	//
	createBackgrounds();
	createAvatars();
	createCards();
	createButtons();
	createNumbers();
	createChats();
	createLayerGa();
	createTimer();

	///lay thong tin va kiem tra xem co choi ga hay khong
	boost::shared_ptr<RoomVariable> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params");
	if (param != NULL){
		string param_str = *param->GetStringValue();
		if (mUtils::splitString(param_str, '@').size() >= 4) {
			layerGa->setNuoiGa(true);
			isNuoiGa = true;
			layerGa->setVisible(true);
		}
		else{
			isNuoiGa = false;
			layerGa->setNuoiGa(false);
			layerGa->setVisible(false);
		}

	}


	mThreadQueueMsg = new ThreadQueueMsg();
	mThreadQueueMsg->setFunCallBack(this, callfunc_selector(LayerPlayGameTLMN::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGameTLMN::updateEvent),this,1/60, false);

	GameServer::getSingleton().addListeners(this);
	SceneManager::getSingleton().hideLoading();
	return true;
}

void LayerPlayGameTLMN::createLayerGa()
{
	layerGa = LayerGa::create();
	layerGa->resetAll();
	this->addChild(layerGa);

}

void LayerPlayGameTLMN::updateEvent(float dt)
{
	if (mThreadQueueMsg->isQueueEmpty())
		return;
	// 	if (layerCards->isRunningAction())
	// 		return;
	mThreadQueueMsg->run();
}

void LayerPlayGameTLMN::resetCard(float dt)
{
	this->resetGame();
	layerAvatars->setListUserByTienLen(myListUser);
}

void LayerPlayGameTLMN::showButtonMyTurn(bool isAllow)
{
	//danh, xep, bo luot
	int widButton = 129;
	int space = 10;
	getButtonByTag(kTagButtonHit)->setVisible(true);
	getButtonByTag(kTagButtonSort)->setVisible(true);

	if (isAllow){
		getButtonByTag(kTagButtonNextTurn)->setVisible(true);
		getButtonByTag(kTagButtonHit)->setPositionX(WIDTH_DESIGN - space * 3 - widButton * 2);
		getButtonByTag(kTagButtonSort)->setPositionX(WIDTH_DESIGN - space * 2 - widButton);
		getButtonByTag(kTagButtonNextTurn)->setPositionX(WIDTH_DESIGN - space);
	}
	else{
		getButtonByTag(kTagButtonNextTurn)->setVisible(false);
		getButtonByTag(kTagButtonHit)->setPositionX(WIDTH_DESIGN - space * 2 - widButton);
		getButtonByTag(kTagButtonSort)->setPositionX(WIDTH_DESIGN - space);
	}
}

void LayerPlayGameTLMN::showButtonOtherTurn()
{
	//chi co xep
	int widButton = 129;
	int space = 10;
	//getButtonByTag(kTagButtonSort)->setPositionX(WIDTH_DESIGN - space);
	getButtonByTag(kTagButtonHit)->setVisible(false);
	getButtonByTag(kTagButtonNextTurn)->setVisible(false);
	getButtonByTag(kTagButtonSort)->setVisible(true);

}















