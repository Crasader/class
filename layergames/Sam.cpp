//
//  Sam.cpp
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//
//// public static final String EXT_EVENT_GAME_LEAVE_REQ     = "lvgrq"; khi muon thoat khoi phong dang choi
// public static final String EXT_EVENT_GAME_LEAVE_RES = "lvgrs";/// thi gui ban tin nay
// public static final String EXT_EVENT_GAME_LEAVE_NOTIF = "lvgntf";

#include "Sam.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "data_manager.h"
#include "LayerListGa.h"
#include "PhomMessDef.h"
#define TAG_ARROW 1392
vector<string> Sam::split(string &S, const char &str) {
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
/*
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
EXT_EVENT_START_READY_TIMER_NOTIF, //rdtntf
EXT_EVENT_SAM_REQ,// "smrq",
EXT_EVENT_SAM_RES,// "smrs",
EXT_EVENT_SAM_NTF,// "smntf",
EXT_EVENT_BAO_NTF,// "baontf",
EXT_EVENT_CHICKEN_UPDATE_NTF,//"chkntf"
EXT_EVENT_REGISTER_QUEUE_NTF,//rqntf
//khi huy sam se co notify
EXT_EVENT_UNSAM_REQ,// = "usmrq";
EXT_EVENT_UNSAM_NTF,// = "usmntf"; //	EXT_EVENT_UNSAM_REQ,// = "usmrq";EXT_EVENT_UNSAM_RES,// = "usmrs";
EXT_EVENT_UNSAM_RES, //usmrs
EXT_EVENT_GAME_CHIKKEN_CONFIG_REQ,// = "ckcrq" - require config ga
EXT_EVENT_GAME_CHIKKEN_CONFIG_RES,// = "ckcrs" - result config ga
EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY, //= "ckcntf" - name extions config ga: 1 la bat, 0 la tat, chi tat dc khi tien ga dang = 0
EXT_EVENT_GAME_LEAVE_REQ,// = "lvgrq";
EXT_EVENT_GAME_LEAVE_RES,// "lvgrs";/// th
EXT_EVENT_GAME_LEAVE_NOTI, //= "lvgntf"; F
EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF,//: "expmntf",
EXT_EVENT_AUTO_KICK// "kkntf",
};

int Sam::convertResponseToInt(string inString) {
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
if (inString == "smrq")    return EXT_EVENT_SAM_REQ;
if (inString == "smrs")    return EXT_EVENT_SAM_RES;
if (inString == "smntf")    return EXT_EVENT_SAM_NTF;
if (inString == "baontf")    return EXT_EVENT_BAO_NTF;
if (inString == "chkntf")	return EXT_EVENT_CHICKEN_UPDATE_NTF;
if (inString == "usmrq")	return EXT_EVENT_UNSAM_REQ;
if (inString == "usmrs")	return EXT_EVENT_UNSAM_RES;
if (inString == "usmntf")	return EXT_EVENT_UNSAM_NTF;
if (inString == "ckcrq")	return EXT_EVENT_GAME_CHIKKEN_CONFIG_REQ;
if (inString == "ckcrs")	return EXT_EVENT_GAME_CHIKKEN_CONFIG_RES;
if (inString == "ckcntf")	return EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY;
if (inString == "lvgrq")	return EXT_EVENT_GAME_LEAVE_REQ;
if (inString == "lvgrs")	return EXT_EVENT_GAME_LEAVE_RES;
if (inString == "lvgntf")	return EXT_EVENT_GAME_LEAVE_NOTI;
if (inString == "expmntf")	return EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF;
if (inString == "rqntf")	return EXT_EVENT_REGISTER_QUEUE_NTF;
if (inString == "kkntf")	return EXT_EVENT_AUTO_KICK;
return -1;
}

string Sam::convertResponseToString(int inInt) {
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
if (inInt == EXT_EVENT_SAM_REQ)       return "smrq";
if (inInt == EXT_EVENT_SAM_RES)       return "smrs";
if (inInt == EXT_EVENT_SAM_NTF)      return "smntf";
if (inInt == EXT_EVENT_BAO_NTF)      return "baontf";
if (inInt == EXT_EVENT_CHICKEN_UPDATE_NTF) return "chkntf";
if (inInt == EXT_EVENT_UNSAM_REQ)		return "usmrq";
if (inInt == EXT_EVENT_UNSAM_RES)		return "usmrs";
if (inInt == EXT_EVENT_UNSAM_NTF)		return "usmntf";
if (inInt == EXT_EVENT_GAME_CHIKKEN_CONFIG_REQ) return "ckcrq";
if (inInt == EXT_EVENT_GAME_CHIKKEN_CONFIG_RES)	return "ckcrs";
if (inInt == EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY) return"ckcntf";
if (inInt == EXT_EVENT_GAME_LEAVE_REQ) return"lvgrq";
if (inInt == EXT_EVENT_GAME_LEAVE_RES) return"lvgrs";
if (inInt == EXT_EVENT_GAME_LEAVE_NOTI) return"lvgntf";
if (inInt == EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF)	return "expmntf";
if (inInt == EXT_EVENT_REGISTER_QUEUE_NTF)		return "rqntf";
if (inInt == EXT_EVENT_AUTO_KICK)		return "kkntf";
return "";
}
*/
Sam::Sam() {
	layerTimer = NULL;
	mThreadQueueMsg = NULL;
}

Sam::~Sam() {
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(Sam::updateEvent),this);
	GameServer::getSingleton().removeListeners(this);
	if (mThreadQueueMsg != NULL) delete mThreadQueueMsg;
	log("~Sam");
	arrName.clear();
	arrMoney.clear();
	arrMoneyDouble.clear();
	arrIDTurn.clear();
}

void Sam::createBackgrounds() {
	layerBackGround = BackgroundInGame::create();
	this->addChild(layerBackGround);

	//////////////////////////////////////////////////////////////////////////
	// thông tin bàn chơi và mức cược
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() != NULL){
		int id = std::atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0, 3).c_str());
		boost::shared_ptr<RoomVariable> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params");
		if (param != NULL){
			string paramString = param->GetStringValue()->c_str();
			vector<string> arrInfo = split(paramString, '@');
			string money = arrInfo.at(0);

			string *name = mUtils::getGameNameByID(gameID);
			double money_bet = atol(money.c_str());
			int roomID = 101;
			boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom != NULL){
				roomID = lastRoom->Id();
				//HoangDD Change
				log("room Name:%s", lastRoom->Name()->c_str());
				string sRoomIdDisplay = mUtils::getRoomIDByName(*lastRoom->Name());
				if (sRoomIdDisplay != "")
					roomID = atoi(sRoomIdDisplay.c_str());
			}
			layerBackGround->setNameTable(*name, roomID, money_bet);
		}
	}
}

void Sam::createAvatars() {
	layerAvatars = LayerAvatarInGame52La::create();
	layerAvatars->resetAll();
	this->addChild(layerAvatars);
}

void Sam::createButtons() {
	layerButtons = LayerButtonInGame::create();
	this->addChild(layerButtons);

    if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL){
        return;
    }
	boost::shared_ptr<RoomVariable> isVipRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("isviproom");
	if (isVipRoom != NULL){
		this->vipRoom = *isVipRoom->GetIntValue();
		if (this->vipRoom == 0){
			this->layerButtons->processDisplayButtonInvite(false);
		}
		else{
			this->layerButtons->processDisplayButtonInvite(true);
		}
	}

	int widButton = 157;
	int widButton2 = 157;
	int heiButton = 148;
	int space = 100;
	int w_btnChat = 323;
	sizeAd = mUtils::getSizePos();
	Size winSize = Director::getInstance()->getVisibleSize();
	Button *btnReady = createButtonWithTitle_Position("button_batdau.png",
		Vec2(WIDTH_DESIGN / 2, space), "");
	Button *btnBaoSam = createButtonWithTitle_Position("button_baosam.png",
		Vec2(WIDTH_DESIGN / 2 + widButton * 2, space)
		, "");
	btnBaoSam->setAnchorPoint(Vec2(0.5, 0.5));
	Button *btnHuySam = createButtonWithTitle_Position("button_huysam.png",
		Vec2(WIDTH_DESIGN / 2 - widButton * 2, space)
		, "");

	Button *btnSortCard = createButtonWithTitle_Position("button_xepbai.png",
		Vec2(WIDTH_DESIGN / 2, space)
		, "");
	Button *btnHitCard = createButtonWithTitle_Position("button_danh.png",
		Vec2(WIDTH_DESIGN / 2 + widButton * 2, space)
		, "");
	Button *btnNextTurn = createButtonWithTitle_Position("button_boluot.png",
		Vec2(WIDTH_DESIGN / 2 - widButton * 2, space)
		, "");

	btnBaoSam->addTouchEventListener(CC_CALLBACK_2(Sam::actionBaoSam, this));
	btnHuySam->addTouchEventListener(CC_CALLBACK_2(Sam::actionHuySam, this));
	btnReady->addTouchEventListener(CC_CALLBACK_2(Sam::actionReady, this));
	btnSortCard->addTouchEventListener(CC_CALLBACK_2(Sam::actionSortCards, this));
	btnHitCard->addTouchEventListener(CC_CALLBACK_2(Sam::actionHitCards, this));
	btnNextTurn->addTouchEventListener(CC_CALLBACK_2(Sam::actionNextTurn, this));

	btnBaoSam->setTag(kTagButtonBaoSam);
	btnHuySam->setTag(kTagButtonHuySam);
	btnReady->setTag(kTagButtonReady);
	btnSortCard->setTag(kTagButtonSort);
	btnHitCard->setTag(kTagButtonHit);
	btnNextTurn->setTag(kTagButtonNextTurn);

	btnReady->setVisible(false);
	btnBaoSam->setVisible(false);
	btnHuySam->setVisible(false);
	btnSortCard->setVisible(false);
	btnHitCard->setVisible(false);
	btnNextTurn->setVisible(false);

	layerButtons->addWidgetEx(btnReady);
	layerButtons->addWidgetEx(btnHuySam);
	layerButtons->addWidgetEx(btnSortCard);
	layerButtons->addWidgetEx(btnHitCard);
	layerButtons->addWidgetEx(btnNextTurn);
	layerButtons->addWidgetEx(btnBaoSam);

}

void Sam::createNumbers() {
	layerNumbers = LayerNumberInGame::create();
	this->addChild(layerNumbers);
}

void Sam::createCards() {
	layerCards = LayerCardInGame::create();
	layerCards->initCardByTienLen();
	layerCards->setFunCallBack(this, callfuncO_selector(Sam::cardInGameCallBack));
	this->addChild(layerCards);
}

void Sam::createChats() {
	//	layerChats = LayerChatInGame::create();
	//	this->addChild(layerChats);
}

void Sam::initGame() {
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


void Sam::playSound(int type)
{
	if (!mUtils::isSoundOn()) return;
	string soundPath;
	switch (type)
	{
	case S_EAT_CARD://an bai
		soundPath = "an_bai.mp3";
		break;
	case S_UPDATE_LIST: //co nguoi moi vao - update list user
		soundPath = "ring_ring.mp3";
		break;
	case S_START_GAME:
		soundPath = "start_game.mp3"; //bat dau game
		break;
	case S_END_GAME:
		soundPath = "end_game.mp3";//ket thuc game
		break;
	case S_WIN:
		soundPath = "thang_roi_1.mp3";//hien thi thang thua
		break;
	case S_DEAL:
		soundPath = "chia_bai.mp3";
		break;
	case S_SORT:
		soundPath = "XepBai.mp3";
		break;
	case S_HIT_CARD_TL:
	{
						  int rand_int = rand() % 2;
						  if (rand_int == 1){
							  soundPath = "con_nua_ne.mp3";//hien thi thang thua
						  }
						  else{
							  soundPath = "chay_di_dau.mp3";//hien thi thang thua
						  }
	}
		break;
	case S_NEXT_TURN:
		soundPath = "em_tha_do.mp3";
		break;
	default:
		break;
	}

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundPath.c_str());
}


Button* Sam::createButtonWithTitle_Position(string imgPath, Point pPoint, string title) {
	// Create the button
	//Change for game P2
	Button* button = Button::create();
	button->setTouchEnabled(true);
	button->setScale9Enabled(false);
	button->setTitleText(title);
	button->setTitleFontSize(FONT_SIZE_SMALL);
	//button->setTitleFontName(kNormalFont);

	if (0 != imgPath.compare(""))
	{
		button->setPressedActionEnabled(true);
		button->loadTextures(imgPath, imgPath, "");
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
			button->setTitleColor(Color3B::BLACK);
	}
	else{

		button->setAnchorPoint(Point(1, 0));
		button->loadTextures("ingame-button.png", "ingame-button-active.png", "");
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
			button->setTitleColor(Color3B::WHITE);
	}

	button->setPosition(Vec2(pPoint.x + sizeAd.width, pPoint.y));

	return button;
}

Button* Sam::getButtonByTag(int tag) {
	Button *button = (Button*) this->layerButtons->getChildByTag(tag);
	return button;
}

bool Sam::isSpectator() {
	vector<string> arr = split(myListUser, TL_CHAR_SPLIT_1);
	int size = arr.size();
	for (int i = 0; i < size; i++){
		vector<string> arrInfo = split(arr.at(i), TL_CHAR_SPLIT_2);
		if (arrInfo.size() < 2) continue;
		string name = arrInfo.at(1);
		log("Sam::isSpectator(): name= %s", name.c_str());
		if (name == myName) return false;
	}

	return true;
}

void Sam::actionReady(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		// 		layerAvatars->getUserByPos(kUserMe)->startTimer(10);
		// 		return;
		// 		layerAvatars->getUserByPos(kUserMe)->StartAnimExp("20");
		// 		return;
		//layerTimer->startTimer();
		//test for roll back : MY LISTCARD = 27,30,36,38,40,42 LISTCARD_SEVER = 27
		// 		string listCard = "27";
		// 		string listCardClient = "27,30,36,38,40,42";
		// 		vector<int> arrID = layerCards->getIDFromString_TienLen(listCard);
		// 
		// 		vector<int> arrHitID = layerCards->getIDFromString_TienLen(listCardClient);
		// 		//so sanh 2 mang voi nhau, neu trung thi khoi animation, neu khong dung thi phai rollback lai
		// 		if (arrHitID.size() != arrID.size()){//so sanh khac length thi cung rollback luon
		// 			///rollback
		// 			log("KHONG TRUNG - ROLLBACK");
		// 			layerCards->actionHitCardTLRollBack(arrHitID);
		// 			layerCards->actionHitCardByTienLen(kUserMe, arrID);
		// 		}
		// 		return;
		// 		string list_id = "61,62,59,58,56,55";
		// 		logicTienLenMN->setPreviousCard("");
		// 		if (!logicTienLenMN->checkPlay(list_id)){
		// 			layerNumbers->showChatByPos(-1, dataManager.GetSysString(239));
		// 			return;
		// 		}
		// 		return;
		// 		vector<int> listcard = layerCards->getIDFromString_TienLen(list_id);
		// 		layerCards->actionHitCardByTienLen(kUserMe, listcard);
		// 		return;
		//resetGame();
		// 		string list_id[] = { "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25" };
		// 		vector<int> listcard = layerCards->getIDFromString_TienLen(list_id[demotest]);
		// 		layerCards->actionHitCardByTienLen(kUserMe, listcard);
		// 		if (demotest >= 5)
		// 			demotest = 0;
		// 		//layerNumbers->showNumberByPos(demotest, "200000");
		// 		demotest++;
		// 		return;
		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
		GameServer::getSingleton().Send(request);

	}
}
void Sam::actionBaoSam(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		hideBaoSam();
		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_SAM_REQ, parameter, lastRoom));
		GameServer::getSingleton().Send(request);

	}
}
void Sam::actionSortCards(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		log("sort!");
        showArrowInButton(getButtonByTag(kTagButtonNextTurn), false);
		playSound(S_SORT);
		layerCards->actionSortCardByTienLen();
	}
}
void Sam::showBaoSam(){
	if (isSpectator()) return;
	this->getButtonByTag(kTagButtonBaoSam)->setVisible(true);
	this->getButtonByTag(kTagButtonHuySam)->setVisible(true);
	this->getButtonByTag(kTagButtonSort)->setVisible(true);
	showLoadingSam();
}
void Sam::hideBaoSam(){
	this->getButtonByTag(kTagButtonBaoSam)->setVisible(false);
	this->getButtonByTag(kTagButtonHuySam)->setVisible(false);
}
void Sam::actionHitCards(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
        layerCards->removeArrow();

		vector<int> arrID = layerCards->getIdCardByClicked();
		if (arrID.size() == 0){
			layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(243));
			return;
		}
		string listID = "";
		myListCards = "";
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
		this->myListCards = listID;
		logicTienLenMN->setPreviousCard(previousCards);
		if (!logicTienLenMN->checkPlay(myListCards)){
			myListCards = "";
			layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(239));
			return;
		}
		showButtonOtherTurn();
		layerAvatars->stopAllTimer();

		vector<int> listcard = layerCards->getIDFromString_TienLen(listID);
		layerCards->actionHitCardByTienLen(kUserMe, listcard);
		///gui len sever
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString(EXT_FIELD_LIST_CARD, myListCards);
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PLAY_CARD_REQ, params, lastRoom));

		GameServer::getSingleton().Send(request);

	}
}

void Sam::actionNextTurn(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
        layerCards->removeArrow();

		showButtonOtherTurn();
		layerAvatars->stopAllTimer();
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PASS_CARD_REQ, params, lastRoom));

		GameServer::getSingleton().Send(request);
	}
}

void Sam::actionSitting(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		getButtonByTag(kTagButtonSitting)->setVisible(false);
		if (layerAvatars->isStartedGame()){
			isRegistSittingDown = true;//dang trong van thi chua lam ngay
			layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(246));
		}
		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom != NULL)
		{
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PLAY_ROOM_REGISTER, parameter, lastRoom));//gui ban tin chau ria
			GameServer::getSingleton().Send(request);

		}
	}
}

void Sam::actionStandUp(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		getButtonByTag(kTagButtonStandUp)->setVisible(false);
		boost::shared_ptr<IRequest> request(new PlayerToSpectatorRequest());
		GameServer::getSingleton().Send(request);
		isRegistStandUp = false;
	}
}

void Sam::actionConfigGa()
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
	/*
	//vao choi roi thi khoi config hoac khong phai chu ban// tien ga lon hon 0 cung khong dc config
	if (layerGa->getChickenMoney() > 0 && layerGa->getListGa().size() > 1 ||//danh cho chu ban
	(strcmp(find_ChuPhong(myListUser).c_str(), myName.c_str()) != 0))//danh cho thang choi

	{
	layerGa->showListChicken();
	//showListUserChicken();
	return;
	}
	//public static final String EXT_FIELD_CHICKKEN_STATUS = "chks";
	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
	return;
	if (layerGa->getNuoiGa() == false){ //trang thai da tat, gui ban tin bat ga
	parameter->PutInt("chks", LayerGa::CONFIG::ON);
	}
	else{// gui ban tin tat ga
	parameter->PutInt("chks", LayerGa::CONFIG::OFF);
	}
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_CHIKKEN_CONFIG_REQ, parameter, lastRoom));
	GameServer::getSingleton().Send(request);*/

	//fix bat tat ga bay gio thang nao cung bat tat duoc tru thang khach
	// 		if (layerGa->getListGa().size() > 1 )//danh cho thang xem
	// 		{
	// 			layerGa->showListChicken();
	// 			return;
	// 		}
	// // 		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	// // 		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	// // 		if (lastRoom == NULL)
	// // 			return;
	// // 		if (layerGa->getNuoiGa() == false){ //trang thai da tat, gui ban tin bat ga
	// // 			parameter->PutInt(EXT_FIELD_CONFIRM, LayerGa::CONFIG::ON);
	// // 		}
	// // 		else{// gui ban tin tat ga
	// // 			parameter->PutInt(EXT_FIELD_CONFIRM, LayerGa::CONFIG::OFF);
	// // 		}
	// // 		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_USE_CHICKEN_REQ, parameter, lastRoom));
	// // 		GameServer::getSingleton().Send(request);
	// 	}
}
void Sam::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if (0 != cmd->compare(EXT_EVENT_USER_JOIN_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_LISTCARD_NTF)
        && 0 != cmd->compare(EXT_EVENT_ERROR_READY_NTF)
        && 0 != cmd->compare(EXT_EVENT_USER_READY_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_UNREADY_NTF)
        && 0 != cmd->compare(EXT_EVENT_READY_RES)
        && 0 != cmd->compare(EXT_EVENT_LIST_USER_UPDATE)
        && 0 != cmd->compare(EXT_EVENT_START_GAME_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_DEAL_CARD_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_USER_LEAVE_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_END_GAME_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_CHANGE_BALANCE_NOTIFY)
        && 0 != cmd->compare(EXT_EVENT_VICTORY_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_NEAD_PLAY_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_PLAY_CARD_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_GAME_CHANGE_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_PASS_CARD_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_PLAY_CARD_RES)
        && 0 != cmd->compare(EXT_EVENT_HIT_CARD_ERROR)
        && 0 != cmd->compare(EXT_EVENT_START_READY_TIMER_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_SAM_RES)
        && 0 != cmd->compare(EXT_EVENT_BAO_NTF)
        && 0 != cmd->compare(EXT_EVENT_SAM_NTF)
        && 0 != cmd->compare(EXT_EVENT_CHICKEN_UPDATE_NTF)
        && 0 != cmd->compare(EXT_EVENT_UNSAM_NTF)
        && 0 != cmd->compare(EXT_EVENT_UNSAM_RES)
        && 0 != cmd->compare(EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY)
        && 0 != cmd->compare(EXT_EVENT_GAME_CHIKKEN_CONFIG_RES)
        && 0 != cmd->compare(EXT_EVENT_GAME_LEAVE_NOTI)
        && 0 != cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_REGISTER_QUEUE_NTF)
        && 0 != cmd->compare(EXT_EVENT_AUTO_KICK))
    {
        return;
    }
	pObjs->PutUtfString("_cmd_", *cmd);
	//only add to queues
	mThreadQueueMsg->addQueue(pObjs);
}

void Sam::OnExtensionResponseProcess() {
	this->param = mThreadQueueMsg->deQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
	if (cmd == NULL) return;
	if (strcmp("hbc", cmd->c_str()) == 0)
		return;
	else
		log("CMD: %s", cmd->c_str());

	// 	boost::shared_ptr<string> listCard = param->GetUtfString("lc");
	// 	if (listCard != NULL){
	// 		if (this->layerCards->checkCardIsRunning(listCard->c_str()))
	// 			return;
	// 	}
	if (0 == cmd->compare(EXT_HEATBEAT))
	{
		return;
	}
	else if (0 == cmd->compare(EXT_EVENT_USER_JOIN_NOTIF))
	{
		event_EXT_EVENT_USER_JOIN_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_LISTCARD_NTF))
	{
		event_EXT_EVENT_LISTCARD_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_ERROR_READY_NTF))
	{
		event_EXT_EVENT_ERROR_READY_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_USER_READY_NOTIF))
	{
		event_EXT_EVENT_USER_READY_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_UNREADY_NTF))
	{
		event_EXT_EVENT_UNREADY_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_READY_RES))
	{
		event_EXT_EVENT_READY_RES();
	}
	else if (0 == cmd->compare(EXT_EVENT_LIST_USER_UPDATE))
	{
		event_EXT_EVENT_LIST_USER_UPDATE();
	}
	else if (0 == cmd->compare(EXT_EVENT_START_GAME_NOTIF))
	{
		event_EXT_EVENT_START_GAME_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_DEAL_CARD_NOTIF))
	{
		event_EXT_EVENT_DEAL_CARD_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_USER_LEAVE_NOTIF))
	{
		event_EXT_EVENT_USER_LEAVE_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_END_GAME_NOTIF))
	{
		event_EXT_EVENT_END_GAME_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_CHANGE_BALANCE_NOTIFY))
	{
		event_EXT_EVENT_CHANGE_BALANCE_NOTIFY();
	}
	else if (0 == cmd->compare(EXT_EVENT_VICTORY_NOTIF))
	{
		event_EXT_EVENT_VICTORY_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_NEAD_PLAY_NOTIF))
	{
		event_EXT_EVENT_NEAD_PLAY_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_PLAY_CARD_NOTIF))
	{
		event_EXT_EVENT_PLAY_CARD_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_CHANGE_NOTIF))
	{
		event_EXT_EVENT_GAME_CHANGE_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_PASS_CARD_NOTIF))
	{
		event_EXT_EVENT_PASS_CARD_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_PLAY_CARD_RES))
	{
		event_EXT_EVENT_PLAY_CARD_RES();
	}
	else if (0 == cmd->compare(EXT_EVENT_HIT_CARD_ERROR))
	{
		event_EXT_EVENT_HIT_CARD_ERROR();
	}
	else if (0 == cmd->compare(EXT_EVENT_START_READY_TIMER_NOTIF))
	{
		event_EXT_EVENT_START_READY_TIMER_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_SAM_RES))
	{
		event_EXT_EVENT_SAM_RES();
	}
	else if (0 == cmd->compare(EXT_EVENT_BAO_NTF))
	{
		event_EXT_EVENT_BAO_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_SAM_NTF))
	{
		event_EXT_EVENT_SAM_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_CHICKEN_UPDATE_NTF))
	{
		this->event_EXT_EVENT_CHICKEN_UPDATE_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_UNSAM_NTF))
	{
		this->event_EXT_EVENT_UNSAM_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_UNSAM_RES))
	{
		this->event_EXT_EVENT_UNSAM_RES();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY))
	{
		this->event_EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_CHIKKEN_CONFIG_RES))
	{
		this->event_EXT_EVENT_GAME_CHIKKEN_CONFIG_RES();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_LEAVE_NOTI))
	{
		this->event_EXT_EVENT_GAME_LEAVE_NOTI();
	}
	else if (0 == cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF))
	{
		this->event_EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_REGISTER_QUEUE_NTF))
	{
		this->event_EXT_EVENT_REGISTER_QUEUE_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_AUTO_KICK))
	{
		this->event_EXT_EVENT_AUTO_KICK();
	}
	else{
		log("LOG_ERROR %s", cmd->c_str());
	}
}
void Sam::event_EXT_EVENT_SAM_NTF() {
	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
	if (name != NULL){
		log("user bao sam: %s", name->c_str());
		int pos = layerAvatars->getPosByName(name->c_str());
		layerAvatars->setResult(pos, 100, kGameSam, false, 0);
		isBaoSam = true;//danh dau co nguoi bao sam
	}
}
void Sam::event_EXT_EVENT_SAM_RES() {
	//this->hideBaoSam();
}
void Sam::event_EXT_EVENT_BAO_NTF() {
	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
	if (name != NULL)
	{
		int pos = layerAvatars->getPosByName(name->c_str());
		layerAvatars->setResult(pos, 101, kGameSam, false, 0);
	}
}
void Sam::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	log("Update User Variables");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	if (SceneManager::getSingleton().getGameID() != kGameDemLaSolo && SceneManager::getSingleton().getGameID() != kGameSamSolo)
	if (ptrNotifiedUser != NULL){
		boost::shared_ptr<string> name_ptr = ptrNotifiedUser->Name();
		if (name_ptr)
		{
			if (0 == name_ptr->compare(myName))
			{
				boost::shared_ptr<UserVariable> uck_ptr = ptrNotifiedUser->GetVariable("uck");//gia tri bat tat ga cua tung user
				if (uck_ptr)
				{
					int uck = *uck_ptr->GetIntValue();
					layerGa->setConfigChicken(uck);
					//isNuoiGa = uck == LayerGa::CONFIG::ON ? true : false;
					setUckToUserDefault(uck == LayerGa::CONFIG::ON ? true : false);
				}
			}
		}

	}

}

void Sam::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
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
	if (ptrNotifiedMessage == NULL || ptrNotifiedUser == NULL) return;
	log("ptrNotifiedMessage: %s", ptrNotifiedMessage->c_str());
	//
	int vt = layerAvatars->getPosByName(ptrNotifiedUser->Name()->c_str());
	if (vt == -1) return;
	Avatar52La* avatar = layerAvatars->getUserByPos(vt);
	if (avatar)
	{
		avatar->showChat(ptrNotifiedMessage->c_str());
	}
	//layerNumbers->showChatByPos(vt, ptrNotifiedMessage->c_str());
}

void Sam::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	boost::shared_ptr<Room> room = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	//
	if (room != NULL){
		boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
		int id = std::atoi(room->GroupId()->substr(0, 3).c_str());
		boost::shared_ptr<RoomVariable> param = room->GetVariable("params");
		if (param != NULL){
			string paramString = param->GetStringValue()->c_str();
			vector<string> arrInfo = split(paramString, '@');
			string money = arrInfo.at(0);

			string *name = mUtils::getGameNameByID(gameID);
			double money_bet = atol(money.c_str());
			int roomID = room->Id();
			//HoangDD Change
			string sRoomIdDisplay = mUtils::getRoomIDByName(*room->Name());
			if (sRoomIdDisplay != "")
				roomID = atoi(sRoomIdDisplay.c_str());
			layerBackGround->setNameTable(*name, roomID, money_bet);

			// 			boost::shared_ptr<RoomVariable> chicken_value = room->GetVariable("chicken");

			// 			if (chicken_value != NULL)
			// 			{
			// 				if (*chicken_value->GetBoolValue() == true)
			// 				{
			// 					layerGa->setNuoiGa(true);
			// 					layerGa->setConfigChicken(LayerGa::CONFIG::ON);
			// 				}
			// 				else{
			// 					layerGa->setNuoiGa(false);
			// 					layerGa->setConfigChicken(LayerGa::CONFIG::OFF);
			// 				}
			// 			}
			boost::shared_ptr<RoomVariable> endtime_value = room->GetVariable("endtime");
			if (endtime_value != NULL)
			{
				if (endtime_value->GetIntValue())
				{
					int end_time = *endtime_value->GetDoubleValue();
					if (layerTimer)
						layerTimer->setCount(end_time / 1000);
				}
			}
		}
	}
}

void Sam::event_EXT_EVENT_USER_JOIN_NOTIF(){
	log("EXT_EVENT_USER_JOIN_NOTIF");
	boost::shared_ptr<double> tienga = param->GetDouble(EXT_FIELD_CHICKEN_VALUE);
	boost::shared_ptr<long> lv_ga = param->GetInt(EXT_FIELD_CHICKEN_LEVEL);
	boost::shared_ptr<string> listUser = param->GetUtfString(EXT_FIELD_LIST_USER);
	if (listUser != NULL) {
		myListUser = listUser->c_str();
		log("listUser= %s", listUser->c_str());
		int maxPlayer = 4;
		if (SceneManager::getSingleton().getGameID() == kGameSamSolo || SceneManager::getSingleton().getGameID() == kGameDemLaSolo)
		{
			maxPlayer = 2;
		}
		int _size = mUtils::splitString(myListUser, '-').size();
		if (_size >= maxPlayer){
			this->layerButtons->processDisplayButtonInvite(false);
		}
		else{
			if (this->vipRoom == 1){
				this->layerButtons->processDisplayButtonInvite(true);
			}
		}
		//lay thong tin ve ga
		queueList = "";
		layerAvatars->setListUserByTienLen(myListUser);
		isStartedGame = layerAvatars->isStartedGame();
		isSpector = layerAvatars->isSpectator();

		if (tienga != NULL && lv_ga != NULL && *tienga > 0 && *lv_ga > 0 && listUser != NULL){
			layerGa->setChickenMoney(*tienga);
			layerGa->setChickenLv(*lv_ga);
		}


		if (isSpector){
			layerButtons->SetIsGuess(true);
			if (layerAvatars->getUserCount() >= 4)
				layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(253));
		}
		if (!isStartedGame && !isSpector && layerAvatars->getUserCount() > 1 && layerTimer->isVisible()){
			layerTimer->startTimer();
		}
		if (isStartedGame && !isSpector) {
			log("JOIN SUCCESS");
			layerButtons->SetIsGuess(false);
			getButtonByTag(kTagButtonSort)->setVisible(true);
		}

		//	else {
		//layerNumbers->showChatByPos(-1, "Cập nhật lại bàn chơi");
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

void Sam::event_EXT_EVENT_LISTCARD_NTF(){
	log("EXT_EVENT_LISTCARD_NTF");
}

void Sam::event_EXT_EVENT_ERROR_READY_NTF(){
	log("event_EXT_EVENT_ERROR_READY_NTF");
}

void Sam::event_EXT_EVENT_USER_READY_NOTIF(){
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
	// 		layerAvatars->getUserByPos(kUserMe)->setReady(true);
	// // 		// hiện unready
	// // 		layerAvatars->getUserByPos(kUserMe)->setReady(true);
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

void Sam::event_EXT_EVENT_UNREADY_NTF(){
	boost::shared_ptr<string> fname = param->GetUtfString(EXT_FIELD_USERID);
	//    // log("EXT_EVENT_UNREADY_NTF name= %s", fname->c_str());

	if (strcmp(fname->c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0){
		// hiện unready
		//getButtonByTag(kTagButtonReady)->setVisible(true);
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
		layerTimer->stopTimer();
}

void Sam::event_EXT_EVENT_READY_RES(){
	boost::shared_ptr<long> rs = param->GetInt(EXT_FIELD_RESULT);

	if (rs != NULL) {
		log("event_EXT_EVENT_READY_RES %ld", *rs.get());

		int rscode = *rs.get();
		if (rscode != PHOM_RSCODE_OK) {
			//			layerNumbers->showChatByPos(-1, dataManager.GetSysString(57));
		}
	}
}

void Sam::event_EXT_EVENT_LIST_USER_UPDATE(){
	boost::shared_ptr<string> listUser = param->GetUtfString(EXT_FIELD_LIST_USER);
	if (listUser == NULL) return;
	myListUser = listUser->c_str();
	log("event_EXT_EVENT_LIST_USER_UPDATE %s", listUser->c_str());
	//if (this->listUser == listUser->c_str()) return;
	int maxPlayer = 4;
	if (SceneManager::getSingleton().getGameID() == kGameSamSolo || SceneManager::getSingleton().getGameID() == kGameDemLaSolo)
	{
		maxPlayer = 2;
	}
	int _size = mUtils::splitString(myListUser, '-').size();
	if (_size >= maxPlayer){
		this->layerButtons->processDisplayButtonInvite(false);
	}
	else{
		if (this->vipRoom == 1){
			this->layerButtons->processDisplayButtonInvite(true);
		}
	}
	myListUser = (listUser->c_str());
	if (myListUser.find(leaveUser) == std::string::npos || 0 == leaveUser.compare("")) //neu khong co thang roi ban trong list moi cap nhat lai listuser
	{
		log("nhay vao day");
		layerAvatars->setListUserByTienLen(myListUser);
	}

	layerAvatars->setListUserGuess(queueList, TL_CHAR_SPLIT_1);
	if (layerAvatars->getUserCount() == 1)
	{
		layerTimer->stopTimer();
	}
}

void Sam::event_EXT_EVENT_START_GAME_NOTIF(){
	log("event_EXT_EVENT_START_GAME_NOTIF");
	this->resetGame();
	layerTimer->stopTimer();
	isStartedGame = true;
	hideAllButton();
	layerGa->showToast(false);
	// hiển thị số bài trên tay của mỗi người (default=13) voi tien len, con lai sam la 10 - hien tai dang bo
	int gameID = SceneManager::getSingleton().getGameID();
	vector<string> lst = mUtils::splitString(myListUser, TL_CHAR_SPLIT_1);
	int first_card = 13;
	if (gameID == kGameSam || gameID == kGameSamSolo)
		first_card = 10;
	for (int i = 0; i < lst.size(); i++)
	{
		vector<string> info = mUtils::splitString(lst[i], TL_CHAR_SPLIT_2);
		if (info.size()>1)
			layerCards->setCountCardByPos(layerAvatars->getPosByName(info[1]), first_card);
	}
}

void Sam::event_EXT_EVENT_DEAL_CARD_NOTIF(){
	//    // Danh sách bài (chỉ sử dụng cho lần đầu)
	//    //Ti.App.EXT_FIELD_CARD_OPEN = "cropn";
	//    var isCardOpen = params.cropn;
	//    //EXT_FIELD_LIST_CARD = "lc";
	//    var lc = params.lc;
	//    var uid = params.uid;
	log("event_EXT_EVENT_DEAL_CARD_NOTIF");
	boost::shared_ptr<string> listCard = param->GetUtfString(EXT_FIELD_LIST_CARD);

	if (listCard != NULL) {
		log("listCard= %s", listCard->c_str());
		layerCards->actionDealCard(layerCards->getIDFromString_TienLen(listCard->c_str()));
		if (!isSpector && (this->gameID == kGameSam || gameID == kGameSamSolo)) {
			playSound(S_DEAL);
			showBaoSam();
		}

	}
}


void Sam::event_EXT_EVENT_USER_LEAVE_NOTIF(){
	boost::shared_ptr<string> listUser = param->GetUtfString(EXT_FIELD_LIST_USER);
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);

	if (listUser != NULL){
		myListUser = listUser->c_str();

		int maxPlayer = 4;
		if (SceneManager::getSingleton().getGameID() == kGameSamSolo || SceneManager::getSingleton().getGameID() == kGameDemLaSolo)
		{
			maxPlayer = 2;
		}
		int _size = mUtils::splitString(myListUser, '-').size();
		if (_size >= maxPlayer){
			this->layerButtons->processDisplayButtonInvite(false);
		}
		else{
			if (this->vipRoom == 1){
				this->layerButtons->processDisplayButtonInvite(true);
			}
		}

		log("event_EXT_EVENT_USER_LEAVE_NOTIF");
		log("listUser= %s", listUser->c_str());
		if (isStartedGame == false)
		{
			layerAvatars->setListUserGuess(queueList, TL_CHAR_SPLIT_1);
			layerAvatars->setListUserByTienLen(listUser->c_str());
		}


		if (layerAvatars->getUserCount() == 1){ //neu chi co 1 thang trong phong, dung ready
			layerTimer->stopTimer();
			hideAllButton();
		}
	}

}

void Sam::event_EXT_EVENT_END_GAME_NOTIF(){
	log("event_EXT_EVENT_END_GAME_NOTIF");
	isStartedGame = false;
	layerAvatars->stopAllTimer();
	hideLoadingSam();
	layerCards->setGameStarted(false);
	hideAllButton();
	arrIDTurn.clear();
	//this->scheduleOnce(CC_SCHEDULE_SELECTOR(Sam::resetCard), TIME_RESET_GAME);
        Director::getInstance()->getScheduler()->schedule(schedule_selector(Sam::resetCard), this, TIME_RESET_GAME, 1, TIME_RESET_GAME, false);
	isSpector = layerAvatars->isSpectator();
	layerGa->showToast(true);
}

void Sam::event_EXT_EVENT_CHANGE_BALANCE_NOTIFY(){
	//    // sự kiện công/trừ tiền
	//    var uid = params.uid;
	//    var money = params.amf;
	log("event_EXT_EVENT_AMF_TEST_NOTIF");
	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<double> money_ptr = param->GetDouble(EXT_FIELD_USER_AMF_GOLD);
	boost::shared_ptr<double> betvl = param->GetDouble(EXT_FIELD_BET_VALUE);
	boost::shared_ptr<long> cbt = param->GetInt(EXT_FIELD_CHANGE_BALANCE_TYPE);

	int resson = -1;
	long money = 0;
	if (name != NULL && money_ptr != NULL && cbt != NULL) {
		resson = (int)(*cbt.get());
		money = (long)(*money_ptr.get());
		log("name= %s, money= %d, reason= %d", name->c_str(), money, resson);

		int pos = layerAvatars->getPosByName(name->c_str());
		std::string res;
		if (money > 0)
			res = "+" + StringUtils::toString(money);
		else
			res = StringUtils::toString(money);
		//layerNumbers->showNumberByPos(pos, res);
		auto avatar = layerAvatars->getUserByPos(pos);
		if (avatar)
			avatar->showNumber(res);
		if (resson < 0) return;
		string ressonString = "";
		switch (resson) {
		case SAM_RESULT_RSCODE_THOI_2:
			if (money < 0)
				ressonString = "Thối 2";
			break;
		case SAM_RESULT_RSCODE_AN_THOI_2:
			ressonString = "Ăn thối 2";
			break;
		case SAM_RESULT_RSCODE_CHAN_DUOC_2:
			ressonString = "Chặn được 2";
			break;
		case SAM_RESULT_RSCODE_BI_BAT_2:
			ressonString = "Bị bắt 2";
			break;
		case SAM_RESULT_RSCODE_CHAN_LAI_2:
			ressonString = "Chặn lại 2";
			break;
		case SAM_RESULT_RSCODE_DEN_LANG:
			if (money < 0)
				ressonString = "Đền làng";
			break;
		case SAM_RESULT_RSCODE_DEN_BAO:
			ressonString = "Đền báo";
			break;
		case SAM_RESULT_RSCODE_GOP_GA:// tru tien ghop ga
			ressonString = dataManager.GetSysString(479);
			break;
			// 		case 9:// cong tien khi an ga
			// 			ressonString = "";
			// 			break;
		case SAM_RESULT_RSCODE_TOI_TRANG:
			ressonString = "Tới trắng";
			break;
		case SAM_RESULT_RSCODE_THUA_CONG:
			ressonString = "Thua cóng";
			break;
		case SAM_RESULT_RSCODE_DUT_MU:
			ressonString = "Đứt mù";
			break;
		case SAM_RESULT_RSCODE_THANG_CONG:
			ressonString = "Thắng cóng";
			break;
		case SAM_RESULT_RSCODE_BO_CUOC:
			ressonString = "Bỏ cuộc";//khi xin ra
			leaveUser = name->c_str();
			break;
		case SAM_RESULT_RSCODE_THOI_TU_QUY:
			ressonString = "Thối tứ quý";
			break;
		case SAM_RESULT_RSCODE_THOI_3_THONG:
			ressonString = "Thối 3 đôi thông";
			break;
		case SAM_RESULT_RSCODE_THOI_4_THONG:
			ressonString = "Thối 4 đôi thông";
			break;
		case SAM_RESULT_RSCODE_AN_THOI_TU_QUY:
			ressonString = "Ăn thối tứ quý";
			break;
		case SAM_RESULT_RSCODE_AN_THOI_3_THONG:
			ressonString = "Ăn thối 3 thông";
			break;
		case SAM_RESULT_RSCODE_AN_THOI_4_THONG:
			ressonString = "Ăn thối 4 thông";
			break;
		}
		log("resson=%d, ressonString=%s", resson, ressonString.c_str());
		if (resson == 8){
			//layerNumbers->showChatByPos(pos, ressonString);
			Avatar52La* avatar = layerAvatars->getUserByPos(pos);
			if (avatar)
			{
				avatar->showChat(ressonString);
			}
		}
		else{
			layerAvatars->setResult(pos, resson, kGameSam, false, money);
		}

		if (resson == 9) // an ga, thi hien thi tien bay
		{
			auto userAva = layerAvatars->getUserByPos(pos);
			if (userAva != NULL)
			{
				Vec2 position = userAva->getPosition();
				layerGa->runActionAnGa(position, 1);
			}

		}
	}
}

void Sam::event_EXT_EVENT_VICTORY_NOTIF(){
	//    var uid = params.uid;
	//    var vicPos = params.vtps;

	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<long> vicPos = param->GetInt(EXT_FIELD_VIC_POS);
	boost::shared_ptr<string> listCard = param->GetUtfString(EXT_FIELD_LIST_CARD);
	boost::shared_ptr<long> toitrang_type = param->GetInt(EXT_FIELD_TOITRANG_TYPE);
	log("event_EXT_EVENT_VICTORY_NOTIF");
	hideAllButton();
	if (name != NULL && listCard != NULL) {

		if (strcmp(name->c_str(), myName.c_str()) == 0){
			getButtonByTag(kTagButtonSort)->setVisible(false);
			getButtonByTag(kTagButtonReady)->setVisible(false);
		}

		if (strcmp(name->c_str(), myName.c_str()) == 0);
		{
			playSound(S_WIN);

		}
		string lc = listCard->c_str();
		if (lc.size() <= 0) return;
		if (strcmp(name->c_str(), myName.c_str()) != 0){
			//layerCards->hideCardByArr(arrIDTurn);
			arrIDTurn.clear();
			int pos = layerAvatars->getPosByName(name->c_str());
			layerCards->showCardOnHandByPos_List(pos, lc);
		}
	}
	if (toitrang_type != NULL){
		if (gameID == kGameSam || gameID == kGameSamSolo)//toi trang danh cho 2 game
		{
			layerAvatars->setResult(layerAvatars->getPosByName(name->c_str()),
				*toitrang_type, 24, false, 0);
		}
		else if (gameID == kGameTLMNDemLa || gameID == kGameDemLaSolo)
		{
			layerAvatars->setResult(layerAvatars->getPosByName(name->c_str()),
				*toitrang_type, 25, false, 0);
		}
	}
}

void Sam::event_EXT_EVENT_NEAD_PLAY_NOTIF(){
	// Lượt chơi
	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<long> isfr = param->GetInt(EXT_FIELD_IS_FIRST_ROUND); /// bien kiem tra het 1 luot danh hay chua
	boost::shared_ptr<long> duration = param->GetInt(EXT_FIELD_DURATION_TIME);
	int dr_time = 10;
	log("event_EXT_EVENT_NEAD_PLAY_NOTIF");

	if (name != NULL && isfr != NULL) {
		if (duration != NULL) dr_time = *duration.get() / 1000;
		log("-----TURN_PLAY : name= %s, isfr= %ld %d", name->c_str(), *isfr.get(), dr_time);
		layerAvatars->runTimerWithTime(layerAvatars->getPosByName(name->c_str()), dr_time);
		currentPlaying = name->c_str();
		//recommend card
		layerCards->setLastLengthChoose(0);
		if (strcmp(name->c_str(), myName.c_str()) == 0) {
			flagRollBack = false;
			//recommend card
			layerCards->setPreviousCard(previousCards);
			layerCards->setMyTurn(true);
			if (*isfr.get() == 0) {
				// duoc phep bo luot
				this->showButtonMyTurn(true);

				if (isRecommendOn())//check config flag
				{
					//recommend card
                    layerCards->isRecommend = false;

					string listcardonhand = layerCards->getListCardOnHand();
					logicTienLenMN->setPreviousCard(previousCards);
					vector<CardInfo*> cards = logicTienLenMN->checkRecommendCard(listcardonhand);
					if (cards.size()<=0)
					{
                        layerCards->setCanRecommend(false);
						log("khong chan duoc");
						showArrowInButton(getButtonByTag(kTagButtonNextTurn), true);
					}
					else{
                        layerCards->setCanRecommend(true);
						log("chan duoc");
                        this->layerCards->showArrowInCard(cards[0]->getIdCard());
					}
				}
			}
			else {
				this->showButtonMyTurn(false);
                if (isRecommendOn())//check config flag
                {
                    //recommend card
                    layerCards->isRecommend = false;

                    string listcardonhand = layerCards->getListCardOnHand();
                    logicTienLenMN->setPreviousCard(previousCards);
                    layerCards->setCanRecommend(true);
                }
			}

		}
		else {
			this->showButtonOtherTurn();

			//recommend card
			layerCards->setMyTurn(false);
		}

		// lượt mới: ẩn các quân đang có trên bàn và release mảng chứa
		isNewTurn = false;
		if (*isfr.get() == 1) {
			if (isBaoSam){ //co nguoi bao sam thi an tat ca nhung thang con lai
				int posBaoSam = layerAvatars->getPosByName(name->c_str());
				for (int i = 0; i < 5; i++){
					if (i != posBaoSam)
						layerAvatars->getUserByPos(i)->removeResult();
				}
				isBaoSam = false;
			}
			hideLoadingSam();
			previousCards = "";
			//recommend card
			layerCards->setPreviousCard(previousCards);
			logicTienLenMN->resetGame();
			isNewTurn = true;
			layerCards->hideCardByArr(arrIDTurn);
			layerCards->hideLastCard();
			arrIDTurn.clear();
		}
	}
}

void Sam::event_EXT_EVENT_PLAY_CARD_NOTIF(){
	// Danh sach quan bai danh ra
	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> listCard = param->GetUtfString(EXT_FIELD_LIST_CARD);
	boost::shared_ptr<long> countCard = param->GetInt(EXT_FIELD_COUNT_CARD);

	log("event_EXT_EVENT_PLAY_CARD_NOTIF");

	if (name != NULL && listCard != NULL && countCard != NULL) {
		log("name= %s, listCard= %s, countCard= %ld", name->c_str(), listCard->c_str(), *countCard.get());
		previousCards = listCard->c_str();
		int pos = layerAvatars->getPosByName(*name);
		// cap nhat so bai tren tay
		layerCards->setCountCardByPos(pos, *countCard.get());
		vector<int> arrID = layerCards->getIDFromString_TienLen(listCard->c_str());
		// mỗi khi có danh sách các quân bài đánh ra
		// đều đẩy ID vào mảng lưu trữ tạm thời (để dễ xử lý về sau)
		for (int i = 0; i < arrID.size(); i++) {
			arrIDTurn.push_back(arrID[i]);
		}
		playSound(S_HIT_CARD_TL);
		if (strcmp(name->c_str(), myName.c_str()) == 0 && myListCards.size() > 0) //neu la minh thi kiem tra xem co bai danh tren ban co trung voi bai sever gui
		{
			flagRollBack = true;
			log("MY LISTCARD = %s LISTCARD_SEVER = %s", myListCards.c_str(), listCard->c_str());
			vector<int> arrHitID = layerCards->getIDFromString_TienLen(myListCards);
			myListCards = "";
			//so sanh 2 mang voi nhau, neu trung thi khoi animation, neu khong dung thi phai rollback lai
			if (arrHitID.size() != arrID.size()){//so sanh khac length thi cung rollback luon
				///rollback
				log("KHONG TRUNG - ROLLBACK");
				layerCards->actionHitCardTLRollBack(arrHitID, false);
				layerCards->actionHitCardByTienLen(pos, arrID);
				return;
			}
			else{
				if (equal(arrHitID.begin(), arrHitID.end(), arrID.begin()) == true){ //neu trung
					log("TRUNG ID CARD");
					return;
				}
				else{
					///rollback
					log("KHONG TRUNG - ROLLBACK");
					layerCards->actionHitCardTLRollBack(arrHitID, false);
					layerCards->actionHitCardByTienLen(pos, arrID);
					return;
				}
			}
		}
		else{
			log("action hit card nhu binh thuong");
			std::sort(arrID.begin(), arrID.end());
			layerCards->actionHitCardByTienLen(pos, arrID);
		}

	}

	// 	// 	// am thanh
	// 	if (isNewTurn) {
	// 		playSound("danh_di.mp3");
	// 	}
	// 	else {
	// 		playSound("con_nua_ne.mp3");
	// 	}

}

void Sam::event_EXT_EVENT_GAME_CHANGE_NOTIF(){
	// khi trở lại bàn chơi
	//1000|botbot11|0|42|55_botbot111_1_36, 14, 23, 24, 46, 21, 27, 31, 13, 55, 48;53_botbot11_0_-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1|4000.000000|1
	boost::shared_ptr<string> ginf = param->GetUtfString(EXT_FIELD_GAME_INFO_NOTIFY);
	getButtonByTag(kTagButtonReady)->setVisible(false);
	log("event_EXT_EVENT_GAME_CHANGE_NOTIF");
	if (ginf != NULL) {
		log("%s", ginf->c_str());
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
		string isfr = arrList[2];
        vector<string> arr = split(list, '|');
        if (strcmp(arr.at(3).c_str(), "") != 0 && atoi(isfr.c_str()) != 1){//neu khong phai luot dau danh- tuc la no la thang dau tien
            previousCards = arr.at(3);
            layerCards->showLastCard(list);
        }
        else{
            //neu la thang dau tien thi hien bai lam gi
        }
		if (arrList.size() > 3)
		{
			int index = 3;
			if (arrList.size() >= 5) {
				index = 4;
			}
			string tienCuoc = arrList[0];
			string tenUserToiLuot = arrList[1];
			string listUser_Card = arrList[index];
            layerTimer->stopTimer();
			layerAvatars->runTimerWithTime(layerAvatars->getPosByName(tenUserToiLuot), 15);
			vector<string> arrInfo = split(listUser_Card, ';');
			for (int i = 0; i < arrInfo.size(); i++) {
				vector<string> arr = split(arrInfo[i], TL_CHAR_SPLIT_2);
				if (arr.size() < 4) break;

				string name = arr[1];
				string flag = arr[2];
				string listID = arr[3];
				arrListUser.push_back(name);
				// chu phong
				//layerAvatars->setFlag(layerAvatars->getPosByName(name), (std::atoi(flag.c_str()) == 1));
				vector<int> arrID = layerCards->getIDFromString_TienLen(listID);
				int pos = layerAvatars->getPosByAccountID(name);
				layerCards->setCountCardByPos(layerAvatars->getPosByName(name), arrID.size());
				// chia bai cho minh
				if (strcmp(name.c_str(), myName.c_str()) == 0){
					layerCards->actionDealCardTL(arrID);
					if (strcmp(tenUserToiLuot.c_str(), myName.c_str()) == 0) {

						if (arrID.size() > 0)
							showButtonMyTurn(false);
						if (std::atoi(isfr.c_str()) == 0) {
							// dc phep bo luot
							//getButtonByTag(kTagButtonNextTurn)->setVisible(true);
                            layerCards->setPreviousCard(previousCards);
							showButtonMyTurn(true);
                            layerCards->setMyTurn(true);
							//recommend card
							if (isRecommendOn())//check config flag
							{
                                layerCards->isRecommend = false;
								string listcardonhand = layerCards->getListCardOnHand();
								logicTienLenMN->setPreviousCard(previousCards);
                                vector<CardInfo*> cards = logicTienLenMN->checkRecommendCard(listcardonhand);
                                if (cards.size()<=0)
                                {
                                    layerCards->setCanRecommend(false);
                                   // log("khong chan duoc");
                                    showArrowInButton(getButtonByTag(kTagButtonNextTurn), true);
                                }
                                else{
                                    layerCards->setCanRecommend(true);
                                   // log("chan duoc");
                                    this->layerCards->showArrowInCard(cards[0]->getIdCard());
                                }
							}
						}
					}
				}
			}
		}

		//}

		///set previous card for logic
		
	}
	else{
		//phong truong hop khong lay duoc gi tu sever
		showButtonOtherTurn();
	}

}

void Sam::event_EXT_EVENT_PASS_CARD_NOTIF(){
	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);

	log("event_EXT_EVENT_PASS_CARD_NOTIF");
	if (name != NULL) {
		log("%s", name->c_str());
		//layerNumbers->showChatByPos(layerAvatars->getPosByName(name->c_str()), dataManager.GetSysString(210));
		int pos = layerAvatars->getPosByName(name->c_str());
		Avatar52La* avatar = layerAvatars->getUserByPos(pos);
        layerCards->removeArrow();
		if (avatar)
			avatar->showChat(dataManager.GetSysString(210));
		    playSound(S_NEXT_TURN);
		// 		if (name->c_str() == myName) {
		// 			// 			if (myListCards.size() > 0){//kiem tra xem neu listcard van con thi rollback
		// 			// 				vector<int> arrHitID = layerCards->getIDFromString_TienLen(myListCards);
		// 			// 				log("KHONG TRUNG - ROLLBACK");
		// 			// 				layerCards->actionHitCardTLRollBack(arrHitID);
		// 			// 				//layerNumbers->showChatByPos(-1, "Mạng lag không đánh được");
		// 			// 				myListCards = "";
		// 			// 			}
		// 			showButtonOtherTurn();
		// 		}
	}
}

void Sam::event_EXT_EVENT_PLAY_CARD_RES(){
	boost::shared_ptr<long> rs = param->GetInt(EXT_FIELD_RESULT);

	log("event_EXT_EVENT_PLAY_CARD_RES");
	if (rs != NULL) {
		log("%ld", *rs.get());
		if (*rs.get() != 0) {
			log("MYLISTCARD = %s", myListCards.c_str());
			vector<int> arrHitID = layerCards->getIDFromString_TienLen(myListCards);
			if (myListCards.size() > 0){
				layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(247));
				myListCards = "";
				if (strcmp(currentPlaying.c_str(), myName.c_str()) == 0)//phong truong hop check logic client loi
				{
					showButtonMyTurn(true);
				}
			}
			if (!flagRollBack){
				log("ROLL BACK khi bi bo luot");
				layerCards->actionHitCardTLRollBack(arrHitID, true);
			}
		}
	}
}

std::string Sam::find_ChuPhong(string listUser)
{
	if (listUser == "") return "";
	vector<string> arrUser = split(listUser, TL_CHAR_SPLIT_1);
	vector<string> info = split(arrUser[0], TL_CHAR_SPLIT_2);
	if (info.size() <= 1) return "";
	string boosId = info[1];
	return boosId;
}

void Sam::isWinner()
{
	ui::ImageView* win = ui::ImageView::create();
	win->loadTexture("pic/win.jpg");
	this->addChild(win);
	win->setPosition(this->getContentSize() / 2);
	win->runAction(Repeat::create(Sequence::create(FadeOut::create(0.5), FadeIn::create(0.5), NULL), 5));
}

void Sam::event_EXT_EVENT_HIT_CARD_ERROR()
{
	//	layerNumbers->showChatByPos(-1, "Bài đánh không hợp lệ");
}

void Sam::actionUnReady(Ref *pSender, Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED){
		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_UNREADY_REQ, parameter, lastRoom));
		GameServer::getSingleton().Send(request);

	}

}

void Sam::resetGame()
{
	queueList = "";
	isRegistSittingDown = false;
	myListCards = "";
	previousCards = "";
	currentPlaying = "";
	flagRollBack = false;
	isBaoSam = false;
	Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(Sam::resetCard),this);
	layerCards->resetGame();
	layerAvatars->setUnReadyAllUser();
	//layerTimer->startTimer();
	leaveUser = "";
}

void Sam::createTimer()
{
	layerTimer = LayerTimer::create();
	layerTimer->setCount(15);
	this->addChild(layerTimer);
}

bool Sam::init()
{
	if (!Layer::init())
		return false;
	log("init Sam");
	demotest = 0;
	timeBaoSam = TIME_BAO_SAM;
	this->myName = SceneManager::getSingleton().getMyName();
	this->doTogether = new DoTogether();
	this->isRegistSittingDown = false;
	this->isRegistStandUp = false;
	flagRollBack = false;
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL)
		return false;
	gameID = SceneManager::getSingleton().getGameID();

	this->setAnchorPoint(Point(0.5, 0.5));
	this->setPosition(Vec2::ZERO);
	logicTienLenMN = new LogicTienLenMN();
	logicTienLenMN->resetGame();
	logicTienLenMN->setGameID(gameID);

	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom != NULL)
	{
		log("gui ban tin chau ria");
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PLAY_ROOM_REGISTER, parameter, lastRoom));//gui ban tin chau ria
		GameServer::getSingleton().Send(request);

	}
	//
	createBackgrounds();
	createLoadingBaoSam();
	createCards();
	createAvatars();
	createLayerGa();
	createTimer();
	createChats();
	createNumbers();
	createButtons();
	// 	if (lastRoom != NULL){
	// 		boost::shared_ptr<RoomVariable> param = lastRoom->GetVariable("params");
	// 		if (param != NULL){
	// 			string param_str = *param->GetStringValue();
	// 			if (mUtils::splitString(param_str, '@').size() >= 4) {
	// 				layerGa->setNuoiGa(true);
	// 				//isNuoiGa = true;
	// 				layerGa->setConfigChicken(LayerGa::CONFIG::ON);
	// 			}
	// 			else{
	// 				//isNuoiGa = false;
	// 				layerGa->setNuoiGa(false);
	// 				layerGa->setConfigChicken(LayerGa::CONFIG::OFF);
	// 			}
	// 		}
	// 	}

	mThreadQueueMsg = new ThreadQueueMsg();
	mThreadQueueMsg->setFunCallBack(this, callfunc_selector(Sam::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(Sam::updateEvent),this,1/60, false);

	GameServer::getSingleton().addListeners(this);
	SceneManager::getSingleton().hideLoading();

	return true;
}

void Sam::createLayerGa()
{
	layerGa = LayerGa::create();
	layerGa->resetAll();
	layerGa->callBackFromActionGa(this, callfunc_selector(Sam::actionConfigGa));
	this->addChild(layerGa);
}

void Sam::event_EXT_EVENT_CHICKEN_UPDATE_NTF()
{
	boost::shared_ptr<double> tienga = param->GetDouble(EXT_FIELD_CHICKEN_VALUE);
	boost::shared_ptr<long> lv_ga = param->GetInt(EXT_FIELD_CHICKEN_LEVEL);
	boost::shared_ptr<string> listUserGa = param->GetUtfString(EXT_FIELD_LIST_USER);
	if (tienga != NULL && lv_ga != NULL && listUserGa != NULL){
		double chicken_value = (double)*tienga;
		int chicken_lv = (int)*lv_ga;
		layerGa->setChickenLv(chicken_lv);
		layerGa->setChickenMoney(chicken_value);
		vector<string> list_user = mUtils::splitString(*listUserGa, ',');
		layerGa->setListDirect(list_user);
	}
	// 
	// 	string listUser = "48:bot_16:1:1:0-50:bot_171:0:1:0";
	// 	layerGa->setChickenMoney(100000);
	// 	layerGa->setChickenLv(10);
	// 	layerGa->setListUser(listUser, TL_CHAR_SPLIT_1, TL_CHAR_SPLIT_2);
}
void Sam::updateEvent(float dt)
{
	if (mThreadQueueMsg == NULL) return;
	if (mThreadQueueMsg->isQueueEmpty())
		return;
	mThreadQueueMsg->run();
}

void Sam::resetCard(float dt)
{
	this->resetGame();
}

void Sam::event_EXT_EVENT_UNSAM_NOTIF()
{
	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
	if (name == NULL) return;
	log("event_EXT_EVENT_UNSAM_NOTIF: %s", name->c_str());
	int pos = layerAvatars->getPosByName(name->c_str());
	Avatar52La *avatar = layerAvatars->getUserByPos(pos);
	if (avatar)
		avatar->showChat(dataManager.GetSysString(232));
	//layerNumbers->showChatByPos(pos, dataManager.GetSysString(232));
}

void Sam::actionHuySam(Ref *pSender, Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED){
		hideBaoSam();
		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_UNSAM_REQ, parameter, lastRoom));
		GameServer::getSingleton().Send(request);

	}
}

void Sam::event_EXT_EVENT_UNSAM_RES()
{
	boost::shared_ptr<long> res = param->GetInt(EXT_FIELD_RESULT);
	if (res == NULL) return;
	int rs_code = (int)(*res);
	switch (rs_code)
	{
	case 5:
		//		layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(234));
		break;
	default:
		this->hideBaoSam();
		break;
	}
}

void Sam::showButtonMyTurn(bool isAllow)
{
	//danh, xep, bo luot
	int widButton = 129;
	int space = 10;
	log("showButtonMyTurn isAllow");
	getButtonByTag(kTagButtonHit)->setVisible(true);
	getButtonByTag(kTagButtonSort)->setVisible(true);
	getButtonByTag(kTagButtonBaoSam)->setVisible(false);
	if (isAllow){
		getButtonByTag(kTagButtonNextTurn)->setVisible(true);
	}
	else{
		getButtonByTag(kTagButtonNextTurn)->setVisible(false);
	}
}

void Sam::showButtonOtherTurn()
{

	if (isSpectator()) return;//neu la khach thi thoi
	log("showButtonOtherTurn");
	//chi co xep
	int widButton = 129;
	int space = 10;
	//getButtonByTag(kTagButtonSort)->setVisible(true);
	getButtonByTag(kTagButtonHit)->setVisible(false);
	getButtonByTag(kTagButtonNextTurn)->setVisible(false);
	getButtonByTag(kTagButtonBaoSam)->setVisible(false);
	getButtonByTag(kTagButtonSort)->setVisible(true);
	//getButtonByTag(kTagButtonSort)->setPositionX(WIDTH_DESIGN - space + sizeAd.width);
	//recommend card
	showArrowInButton(getButtonByTag(kTagButtonNextTurn), false);
	showArrowInButton(getButtonByTag(kTagButtonHit), false);
}

void Sam::hideAllButton()
{
	log("hideAllButton");
	getButtonByTag(kTagButtonBaoSam)->setVisible(false);
	getButtonByTag(kTagButtonHuySam)->setVisible(false);
	getButtonByTag(kTagButtonReady)->setVisible(false);
	getButtonByTag(kTagButtonSort)->setVisible(false);
	getButtonByTag(kTagButtonHit)->setVisible(false);
	getButtonByTag(kTagButtonNextTurn)->setVisible(false);
}

void Sam::event_EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY()
{
	log("event_EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY");
	boost::shared_ptr<long> chks = param->GetInt(EXT_FIELD_CHICKEN_SET);
	if (chks != NULL){
		layerGa->setConfigChicken(*(chks.get()));
	}
}

void Sam::event_EXT_EVENT_GAME_CHIKKEN_CONFIG_RES()
{
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT);
	if (rscode != NULL){
		if (*(rscode.get()) == SAM_RSCODE_OK){ //thanh cong
			string mess = "";
			if (layerGa->getNuoiGa()){
				mess.append("Bật gà thành công");
			}
			else{
				mess.append("Tắt gà thành công");
			}
			//			layerNumbers->showChatByPos(-1, mess);
		}
	}
}

void Sam::event_EXT_EVENT_START_READY_TIMER_NOTIF()
{
	getButtonByTag(kTagButtonReady)->setVisible(true);
	layerTimer->startTimer();
}

void Sam::event_EXT_EVENT_GAME_LEAVE_NOTI()
{

}

void Sam::actionLeaveGame(Ref* pSender)
{
	//HoangDD
	// 	LayerNotification* layer = SceneManager::getSingleton().getLayerNotification();
	// 	if (!SceneManager::getSingleton().showNotification()){
	// 		log("NTF Dialog already open!");
	// 		return;
	// 	}
	// 
	// 	layer->setNotificationOptions(dataManager.GetSysString(235),
	// 		dataManager.GetSysString(236), true, dataManager.GetSysString(14), 0, this);
}

void Sam::showListUserChicken()
{
	//HoangDD
	// 	NodeLoaderLibrary* NodeLoaderLibrary = SceneManager::getSingleton().getNodeLoaderLibrary();
	// 	NodeLoaderLibrary->unregisterNodeLoader("LayerSMS");
	// 	NodeLoaderLibrary->registerNodeLoader("LayerSMS", LayerListGaLoader::loader());
	// 	CCBReader* ccbReader = new cocosbuilder::CCBReader(NodeLoaderLibrary);
	// 	if (ccbReader)
	// 	{
	// 		LayerListGa* layerListGa;
	// 		layerListGa = (LayerListGa *)ccbReader->readNodeGraphFromFile("LayerSMS.ccbi");
	// 		layerListGa->setListUser(layerGa->getListGa());
	// 		this->addChild(layerListGa, 10000, HEIGHT_DESIGN);
	// 		ccbReader->release();
	// 	}
}

void Sam::createLoadingBaoSam()
{
	Size winSize = Director::getInstance()->getVisibleSize();
	bgLoading = Sprite::create("timer.png");
	bgLoading->setAnchorPoint(Vec2(0.5, 0.5));
	bgLoading->setPosition(Vec2(WIDTH_DESIGN / 2 + sizeAd.width,
		winSize.height / 2 + sizeAd.height + SIZE_ADD));

	Sprite *time = Sprite::create("timer-loaded.png");
	timerSam = ProgressTimer::create(time);
	timerSam->setPercentage(100);
	timerSam->setAnchorPoint(Vec2(0, 0));
	timerSam->setPosition(Vec2(0, 0));
	timerSam->setType(ProgressTimer::Type::RADIAL);

	labelTimerSam = Label::createWithSystemFont("15", "",
		FONT_SIZE_NORMAL, Size(FONT_SIZE_NORMAL * 2, FONT_SIZE_NORMAL),
		TextHAlignment::CENTER, TextVAlignment::CENTER);
	labelTimerSam->setAnchorPoint(Vec2(0.5, 0.5));
	labelTimerSam->setPosition(Vec2(bgLoading->getContentSize().width / 2,
		bgLoading->getContentSize().height / 2));
	bgLoading->addChild(labelTimerSam);
	bgLoading->addChild(timerSam);
	this->addChild(bgLoading);
	bgLoading->setVisible(false);
}

void Sam::showLoadingSam()
{
	labelTimerSam->setString("15");
	bgLoading->setVisible(true);
	timerSam->setPercentage(100);
	timerSam->runAction(ProgressTo::create(TIME_BAO_SAM, 0));
	Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(Sam::updateTimerSam), this,1,false);
}

void Sam::hideLoadingSam()
{
	hideBaoSam();
	timeBaoSam = 15;
	bgLoading->setVisible(false);
	timerSam->stopAllActions();
	Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(Sam::updateTimerSam),this);
}

void Sam::updateTimerSam(float dt)
{
	labelTimerSam->setString(StringUtils::format("%d", timeBaoSam));
	timeBaoSam--;
	if (timeBaoSam == 0){
		this->hideLoadingSam();
	}
}

void Sam::event_EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF()
{
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	log("event_EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF");
	if (uid != NULL)
	{
		if (strcmp(uid->c_str(), myName.c_str()) == 0){
			//khong du tien hien popup nap tien
			SceneManager::getSingleton().setKickNem(true);
			layerButtons->showToast(dataManager.GetSysString(10));
			layerButtons->eventTouchBtnCoin(NULL, Widget::TouchEventType::ENDED);
		}
	}

}

void Sam::event_EXT_EVENT_REGISTER_QUEUE_NTF()
{
	boost::shared_ptr<string> lu = this->param->GetUtfString(EXT_FIELD_LIST_USER);
	if (lu != NULL)
	{
		log("DS Chau ria: %s", lu->c_str());
		this->layerAvatars->setListUserGuess(lu->c_str(), TL_CHAR_SPLIT_1);
		queueList = lu->c_str();
		bool isChauRia = this->isNguoiChauRia(lu->c_str());
	}
}

bool Sam::isNguoiChauRia(string _list)
{
	vector<string> arrUser = mUtils::splitString(_list, TL_CHAR_SPLIT_1);
	for (int i = 0; i < arrUser.size(); ++i)
	{
		vector<string> info = mUtils::splitString(arrUser.at(i), TL_CHAR_SPLIT_2);
		if (info.size() >1)
		{
			string userName = info.at(1);
			if (strcmp(userName.c_str(), myName.c_str()) == 0)
				return true;
		}
	}
	return false;
}

void Sam::event_EXT_EVENT_AUTO_KICK()
{
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	if (uid != NULL)
	{
		if (strcmp(uid->c_str(), myName.c_str()) == 0){
			SceneManager::getSingleton().setKickRoom(true);
		}
	}
}

void Sam::cardInGameCallBack(Ref *pSender)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
		return;
	int tag = ((Node*)pSender)->getTag();
	switch (tag)
	{
	case 4:
		showArrowInButton(getButtonByTag(kTagButtonHit), true);
		break;
	case 5:
		showArrowInButton(getButtonByTag(kTagButtonHit), false);
		break;
	default:
		break;
	}
}

void Sam::showArrowInButton(Button* btn, bool isShow)
{
	btn->removeChildByTag(TAG_ARROW);
	Sprite* arrow = Sprite::create("arrow_phom.png");
	arrow->setTag(TAG_ARROW);
	arrow->setPosition(Vec2(btn->getContentSize().width / 2, btn->getContentSize().height));
	btn->addChild(arrow);
	if (isShow)
	{
		arrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(0, 10)), MoveBy::create(0.5, Vec2(0, -10)), nullptr)));
	}
	else{
		btn->removeChildByTag(TAG_ARROW);
	}
}

















