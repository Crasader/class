//
//  LayerGameMauBinh.cpp

#include "LayerGameMauBinh.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "data_manager.h"
#include "LayerListGa.h"
#include "PhomMessDef.h"
#include "ClientMsgDefs.h"
#include "LayerKetQuaMauBinh.h"

#define TAG_ARROW 1392
vector<string> LayerGameMauBinh::split(string &S, const char &str) {
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
 EXT_EVENT_LayerGameMauBinh_REQ,// "smrq",
 EXT_EVENT_LayerGameMauBinh_RES,// "smrs",
 EXT_EVENT_LayerGameMauBinh_NTF,// "smntf",
 EXT_EVENT_BAO_NTF,// "baontf",
 EXT_EVENT_CHICKEN_UPDATE_NTF,//"chkntf"
 EXT_EVENT_REGISTER_QUEUE_NTF,//rqntf
 //khi huy LayerGameMauBinh se co notify
 EXT_EVENT_UNLayerGameMauBinh_REQ,// = "usmrq";
 EXT_EVENT_UNLayerGameMauBinh_NTF,// = "usmntf"; //	EXT_EVENT_UNLayerGameMauBinh_REQ,// = "usmrq";EXT_EVENT_UNLayerGameMauBinh_RES,// = "usmrs";
 EXT_EVENT_UNLayerGameMauBinh_RES, //usmrs
 EXT_EVENT_GAME_CHIKKEN_CONFIG_REQ,// = "ckcrq" - require config ga
 EXT_EVENT_GAME_CHIKKEN_CONFIG_RES,// = "ckcrs" - result config ga
 EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY, //= "ckcntf" - name extions config ga: 1 la bat, 0 la tat, chi tat dc khi tien ga dang = 0
 EXT_EVENT_GAME_LEAVE_REQ,// = "lvgrq";
 EXT_EVENT_GAME_LEAVE_RES,// "lvgrs";/// th
 EXT_EVENT_GAME_LEAVE_NOTI, //= "lvgntf"; F
 EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF,//: "expmntf",
 EXT_EVENT_AUTO_KICK// "kkntf",
 };

 int LayerGameMauBinh::convertResponseToInt(string inString) {
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
 if (inString == "smrq")    return EXT_EVENT_LayerGameMauBinh_REQ;
 if (inString == "smrs")    return EXT_EVENT_LayerGameMauBinh_RES;
 if (inString == "smntf")    return EXT_EVENT_LayerGameMauBinh_NTF;
 if (inString == "baontf")    return EXT_EVENT_BAO_NTF;
 if (inString == "chkntf")	return EXT_EVENT_CHICKEN_UPDATE_NTF;
 if (inString == "usmrq")	return EXT_EVENT_UNLayerGameMauBinh_REQ;
 if (inString == "usmrs")	return EXT_EVENT_UNLayerGameMauBinh_RES;
 if (inString == "usmntf")	return EXT_EVENT_UNLayerGameMauBinh_NTF;
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

 string LayerGameMauBinh::convertResponseToString(int inInt) {
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
 if (inInt == EXT_EVENT_LayerGameMauBinh_REQ)       return "smrq";
 if (inInt == EXT_EVENT_LayerGameMauBinh_RES)       return "smrs";
 if (inInt == EXT_EVENT_LayerGameMauBinh_NTF)      return "smntf";
 if (inInt == EXT_EVENT_BAO_NTF)      return "baontf";
 if (inInt == EXT_EVENT_CHICKEN_UPDATE_NTF) return "chkntf";
 if (inInt == EXT_EVENT_UNLayerGameMauBinh_REQ)		return "usmrq";
 if (inInt == EXT_EVENT_UNLayerGameMauBinh_RES)		return "usmrs";
 if (inInt == EXT_EVENT_UNLayerGameMauBinh_NTF)		return "usmntf";
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
LayerGameMauBinh::LayerGameMauBinh() {
	layerTimer = NULL;
	mThreadQueueMsg = NULL;

}

LayerGameMauBinh::~LayerGameMauBinh() {
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameMauBinh::updateEvent),this);
    if (mThreadQueueMsg != NULL) delete mThreadQueueMsg;

    GameServer::getSingleton().removeListeners(this);

	log("~LayerGameMauBinh");
	arrName.clear();
	arrMoney.clear();
	arrMoneyDouble.clear();
	//this->removeAllComponents();
	//layerButtons->removeFromParentAndCleanup(true);
	this->removeChild(layerCards);
	this->removeAllChildrenWithCleanup(true);
}
bool LayerGameMauBinh::init()
{
	if (!Layer::init())
		return false;
	//demotest = 0;
	this->myName = SceneManager::getSingleton().getMyName();
	this->isRegistSittingDown = false;
	this->isRegistStandUp = false;
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL)
		return false;
	gameID = SceneManager::getSingleton().getGameID();

	this->setAnchorPoint(Point(0.5, 0.5));
	this->setPosition(Vec2::ZERO);

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
	createAvatars();
	createTimer();
	createChats();
	createNumbers();
	createCards();
	createButtons();

	mThreadQueueMsg = new ThreadQueueMsg();
	mThreadQueueMsg->setFunCallBack(this, callfunc_selector(LayerGameMauBinh::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameMauBinh::updateEvent),this,1/60, false);
    GameServer::getSingleton().addListeners(this);

	SceneManager::getSingleton().hideLoading();
	return true;
}


void LayerGameMauBinh::createBackgrounds() {
	layerBackGround = BackgroundInGame::create();
	layerBackGround->setLocalZOrder(70);
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
               // log("room Name:%s", lastRoom->Name()->c_str());
                string sRoomIdDisplay = mUtils::getRoomIDByName(*lastRoom->Name());
                if (sRoomIdDisplay != "")
                    roomID = atoi(sRoomIdDisplay.c_str());
            }
			layerBackGround->setNameTable(*name, roomID, money_bet);

		}
	}
}
void LayerGameMauBinh::createAvatars() {
	layerAvatars = LayerAvatarInGame52La::create();
	layerAvatars->setTag(100);
	layerAvatars->resetAll();
	layerAvatars->setLocalZOrder(80);

	this->addChild(layerAvatars);
}

void LayerGameMauBinh::createButtons() {
	layerButtons = LayerButtonInGame::create();
	layerButtons->setLocalZOrder(140);

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
    
	this->addChild(layerButtons);

	int widButton = 230;
	int widButton2 = 230;
	int heiButton = 148;
	int space = 30;
	int w_btnChat = 323;
	sizeAd = mUtils::getSizePos();
	Size winSize = Director::getInstance()->getVisibleSize();
	Button *btnReady = createButtonWithTitle_Position("button_batdau.png",
		Vec2(winSize.width / 2, winSize.height / 2 - widButton), "");
	Button *btnBaoLayerGameMauBinh = createButtonWithTitle_Position("btn-batdau.png",
		Vec2(WIDTH_DESIGN / 2, HEIGHT_DESIGN / 2 - widButton / 3 + sizeAd.height)
		, dataManager.GetSysString(221));
	btnBaoLayerGameMauBinh->setAnchorPoint(Vec2(0.5, 0.5));
	Button *btnHuyLayerGameMauBinh = createButtonWithTitle_Position("",
		Vec2(WIDTH_DESIGN - space * 2 - widButton2, space)
		, dataManager.GetSysString(232));

	Button *btnSortCard = createButtonWithTitle_Position("",
		Vec2(WIDTH_DESIGN - space, space)
		, dataManager.GetSysString(208));
	Button *btnHitCard = createButtonWithTitle_Position("",
		Vec2(WIDTH_DESIGN / 2 + widButton / 2, space)
		, dataManager.GetSysString(209));
	Button *btnNextTurn = createButtonWithTitle_Position("",
		Vec2(WIDTH_DESIGN - widButton2 - space * 2, space)
		, dataManager.GetSysString(210));

	btnReady->addTouchEventListener(CC_CALLBACK_2(LayerGameMauBinh::actionReady, this));
	btnSortCard->addTouchEventListener(CC_CALLBACK_2(LayerGameMauBinh::actionSortCards, this));
	btnHitCard->addTouchEventListener(CC_CALLBACK_2(LayerGameMauBinh::actionHitCards, this));
	btnNextTurn->addTouchEventListener(CC_CALLBACK_2(LayerGameMauBinh::actionNextTurn, this));

	btnReady->setTag(kTagButtonReady);
	btnSortCard->setTag(kTagButtonSort);
	btnHitCard->setTag(kTagButtonHit);
	btnNextTurn->setTag(kTagButtonNextTurn);

	btnReady->setVisible(false);
	btnBaoLayerGameMauBinh->setVisible(false);
	btnHuyLayerGameMauBinh->setVisible(false);
	btnSortCard->setVisible(false);
	btnHitCard->setVisible(false);
	btnNextTurn->setVisible(false);

	layerButtons->addWidgetEx(btnReady);
	layerButtons->addWidgetEx(btnHuyLayerGameMauBinh);
	layerButtons->addWidgetEx(btnSortCard);
	layerButtons->addWidgetEx(btnHitCard);
	layerButtons->addWidgetEx(btnNextTurn);
	layerButtons->addWidgetEx(btnBaoLayerGameMauBinh);

}

void LayerGameMauBinh::createNumbers() {
	layerNumbers = LayerNumberInGame::create();
	layerNumbers->setLocalZOrder(100);

	this->addChild(layerNumbers);
}

void LayerGameMauBinh::createCards() {
	layerCards = LayerBoBaiMauBinh::create();
	layerCards->setLocalZOrder(110);
	this->addChild(layerCards);
}

void LayerGameMauBinh::createChats() {
	//	layerChats = LayerChatInGame::create();
	//	this->addChild(layerChats);
}

void LayerGameMauBinh::initGame() {
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


void LayerGameMauBinh::playSound(int type)
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


Button* LayerGameMauBinh::createButtonWithTitle_Position(string imgPath, Point pPoint, string title) {
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
		button->setTitleColor(Color3B::WHITE);
	}
	else{

		button->setAnchorPoint(Point(1, 0));
		button->loadTextures("ingame-button.png", "ingame-button-active.png", "");
		button->setTitleColor(Color3B::WHITE);
	}

	button->setPosition(Vec2(pPoint.x + sizeAd.width, pPoint.y));
    if (SceneManager::getSingleton().isNagaWin){
        button->setTitleColor(Color3B::BLACK);
    }
	return button;
}

Button* LayerGameMauBinh::getButtonByTag(int tag) {
	Button *button = (Button*) this->layerButtons->getChildByTag(tag);
	return button;
}

bool LayerGameMauBinh::isSpectator() {
	vector<string> arr = split(myListUser, TL_CHAR_SPLIT_1);
	int size = arr.size();
	for (int i = 0; i < size; i++){
		vector<string> arrInfo = split(arr.at(i), TL_CHAR_SPLIT_2);
		if (arrInfo.size() < 2) continue;
		string name = arrInfo.at(1);
		log("LayerGameMauBinh::isSpectator(): name= %s", name.c_str());
		if (name == myName) return false;
	}

	return true;
}

void LayerGameMauBinh::actionReady(Ref *pSender, Widget::TouchEventType pType) {
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

void LayerGameMauBinh::actionSortCards(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		log("sort!");
		//playSound(S_SORT);
		layerCards->showXepBaiMauBinh();

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutInt("act", 0);
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_FINISH_CARD_LINK_REQ, params, lastRoom));
		GameServer::getSingleton().Send(request);
		this->hideAllButton();
	}
}

void LayerGameMauBinh::actionHitCards(Ref *pSender, Widget::TouchEventType pType) {
}

void LayerGameMauBinh::actionNextTurn(Ref *pSender, Widget::TouchEventType pType) {
}

void LayerGameMauBinh::actionSitting(Ref *pSender, Widget::TouchEventType pType) {
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

void LayerGameMauBinh::actionStandUp(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		getButtonByTag(kTagButtonStandUp)->setVisible(false);
		boost::shared_ptr<IRequest> request(new PlayerToSpectatorRequest());
		GameServer::getSingleton().Send(request);
		isRegistStandUp = false;
	}
}

void LayerGameMauBinh::actionConfigGa()
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
void LayerGameMauBinh::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if (0 != cmd->compare(EXT_EVENT_USER_JOIN_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_START_READY_TIMER_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_START_GAME_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_DEAL_CARD_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_FINISH_CARD_LINK_NOTIFY)
        && 0 != cmd->compare(EXT_EVENT_END_GAME_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_GAME_RESULT)
        && 0 != cmd->compare(EXT_EVENT_LIST_USER_UPDATE)
        && 0 != cmd->compare(EXT_EVENT_REGISTER_QUEUE_NTF)
        && 0 != cmd->compare(EXT_EVENT_CHANGE_BALANCE_NOTIFY)
        && 0 != cmd->compare(EXT_EVENT_VICTORY_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_USER_LEAVE_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_GAME_CHANGE_NOTIF)
        && 0 != cmd->compare("drwntf")){
        return;
    }
	pObjs->PutUtfString("_cmd_", *cmd);
	//only add to queues
	mThreadQueueMsg->addQueue(pObjs);
}

void LayerGameMauBinh::OnExtensionResponseProcess() {
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
		this->event_EXT_EVENT_USER_JOIN_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_START_READY_TIMER_NOTIF))
	{
		this->event_EXT_EVENT_START_READY_TIMER_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_START_GAME_NOTIF))
	{
		this->event_EXT_EVENT_START_GAME_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_DEAL_CARD_NOTIF))
	{
		this->event_EXT_EVENT_DEAL_CARD_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_FINISH_CARD_LINK_NOTIFY))
	{
		this->event_EXT_EVENT_FINISH_CARD_LINK_NOTIFY();
	}
	else if (0 == cmd->compare(EXT_EVENT_END_GAME_NOTIF))
	{
		this->event_EXT_EVENT_END_GAME_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT))
	{
		this->event_EXT_EVENT_GAME_RESULT();
	}
	else if (0 == cmd->compare(EXT_EVENT_LIST_USER_UPDATE))
	{
		this->event_EXT_EVENT_LIST_USER_UPDATE();
	}
	else if (0 == cmd->compare(EXT_EVENT_REGISTER_QUEUE_NTF))
	{
		this->event_EXT_EVENT_REGISTER_QUEUE_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_CHANGE_BALANCE_NOTIFY))
	{
		this->event_EXT_EVENT_CHANGE_BALANCE_NOTIFY();
	}
	else if (0 == cmd->compare(EXT_EVENT_VICTORY_NOTIF))
	{
		this->event_EXT_EVENT_VICTORY_NOTIF();
	}

	else if (0 == cmd->compare(EXT_EVENT_USER_LEAVE_NOTIF))
	{
		event_EXT_EVENT_USER_LEAVE_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_CHANGE_NOTIF))
	{
		event_EXT_EVENT_GAME_CHANGE_NOTIF();
	}
    else if (0 == cmd->compare("drwntf")){
        this->hideAllButton();
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameMauBinh::updateTimeXepBai),this);
        layerCards->setLocalZOrder(115);
        layerAvatars->setLocalZOrder(116);
        boost::shared_ptr<string> listCard = param->GetUtfString("rg");
        if (listCard){
           // log(" game end %s", listCard->c_str());
            this->layerCards->hideXepBaiMauBinh();
            auto lstCards = mUtils::splitString(*listCard, '|');
            string list = "";
            for (int i = 0; i<lstCards.size();i++){
                auto itCards = mUtils::splitString(lstCards[i], ';');
                if (itCards.size()<3) return;

                list+=(itCards[0]+"|"+itCards[2] + ";");
                int pos = layerAvatars->getPosByName(itCards[0].c_str());
                layerAvatars->setResult(pos, atoi(itCards[1].c_str()), kGameMauBinh, false, 0);
            }
            this->layerCards->LatBaiHoa(list);
            this->layerAvatars->stopAllTimer();
            string pathImg = "3doi-hoa.png";
            Size winSize = Director::getInstance()->getVisibleSize();
            Sprite* imghoa = Sprite::create(StringUtils::format("icon_result/%s%s",SceneManager::getSingleton().getFolderLang().c_str(),pathImg.c_str()));
            this->addChild(imghoa,this->HOA_IMG_TAG);
            imghoa->setTag(this->HOA_IMG_TAG);
            imghoa->setPosition(winSize/2);
            this->isHoa = true;
        }
    }
}
void LayerGameMauBinh::event_EXT_EVENT_GAME_CHANGE_NOTIF(){
	// khi trở lại bàn chơi
	//"time":621,"gs":0,
	//"ginf":"daptraicogisai2|52,58,50,59,43,45,64,36,23,38,41,47,18|0;abcd3090117|-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1|0;"
	boost::shared_ptr<string> ginf = param->GetUtfString(EXT_FIELD_GAME_INFO_NOTIFY);
	boost::shared_ptr<long> gamestate_ptr = param->GetInt("gs");
	boost::shared_ptr<long> timeCountDownPtr = param->GetInt("time");
	getButtonByTag(kTagButtonReady)->setVisible(false);
	log("event_EXT_EVENT_GAME_CHANGE_NOTIF");

	if (gamestate_ptr != NULL)
	{
		int state = *gamestate_ptr;//0 la het van, 1 la dang choi
		if (state == 0)
		{
            string list = ginf->c_str();
            this->layerCards->setCardRejoin(list);
            this->layerCards->latBai(list);
            this->layerAvatars->stopAllTimer();
			return;
		}
	}

	if (timeCountDownPtr){
		this->timeXepbai = *timeCountDownPtr / 1000;
		if (this->timeXepbai > 0)
		{
			Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameMauBinh::updateTimeXepBai),this, 1, false);
		}
	}


	if (ginf != NULL) {
		log("%s", ginf->c_str());
		string list = ginf->c_str();
		vector<string> listInfo = mUtils::splitString(list, ';');
		this->layerCards->actionDealCard(list);
		this->layerCards->setTimeXepBai(this->timeXepbai);
        layerTimer->stopTimer();
		for (ssize_t i = 0; i < listInfo.size(); i++)
		{
			vector<string> infos = mUtils::splitString(listInfo.at(i), '|');

			if (infos.size() == 3)
			{
				string username = infos.at(0);
				string listcard = infos.at(1);
				int cs = atoi(infos.at(2).c_str());//0 la dang xep, 1 la xep xong
                
				int pos = this->layerAvatars->getPosByName(username);
				BaiMauBinh* bobai = this->layerCards->getBoBaiByPos(pos);
				if (bobai){
					if (cs == 0) {
						bobai->updateStatus(false);
						layerAvatars->runTimerWithTime(pos, this->timeXepbai);
						if (this->layerCards->IsMee(username)) {
							this->getButtonByTag(kTagButtonSort)->setVisible(false);

						}
					}
					else{
						bobai->updateStatus(true);
						Avatar52La* avatar = layerAvatars->getUserByPos(pos);
						if (avatar)
						{
							avatar->stopTimer();
						}
						if (this->layerCards->IsMee(username)) {
							this->getButtonByTag(kTagButtonSort)->setVisible(false);

						}
					}
				}
			}
		}

	}
}
void LayerGameMauBinh::reloadCuoc(int linkChi1, int linkChi2, int linkChi3){
	this->layerButtons->loadCuoc(linkChi1, linkChi2, linkChi3);
}
void LayerGameMauBinh::hideCuoc(){
	this->layerButtons->hideBangCuoc();
}

void LayerGameMauBinh::event_EXT_EVENT_USER_LEAVE_NOTIF(){
	boost::shared_ptr<string> listUser = param->GetUtfString(EXT_FIELD_LIST_USER);
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);

	if (listUser != NULL){
		myListUser = listUser->c_str();
		log("event_EXT_EVENT_USER_LEAVE_NOTIF");
		log("listUser= %s", listUser->c_str());
		if (isStartedGame == false)
		{
			layerAvatars->setListUserGuess(queueList, TL_CHAR_SPLIT_1);
			layerAvatars->setListUserByTienLen(listUser->c_str());
		}

		if (isStartedGame == true){
			int pos = layerAvatars->getPosByName(uid->c_str());
			layerAvatars->setResult(pos, SAM_RESULT_RSCODE_BO_CUOC, kGameSam, false, -1);
			layerCards->resetGameByPos(pos);
		}

		if (layerAvatars->getUserCount() == 1){ //neu chi co 1 thang trong phong, dung ready
			layerTimer->stopTimer();
			hideAllButton();
			this->layerCards->removeTimer();

		}
	}
}

void LayerGameMauBinh::event_EXT_EVENT_VICTORY_NOTIF(){
    if (this->isHoa) return;
	boost::shared_ptr<string> gi = this->param->GetUtfString(EXT_FIELD_GAME_INFO);
    if (gi){
		this->layerCards->showKetQua(*gi);
        LayerKetQuaMauBinh* kq = LayerKetQuaMauBinh::create();
        kq->setTag(1430);
        kq->setLocalZOrder(130);
        kq->setKetQua(*gi);
        this->addChild(kq);
        this->layerAvatars->setLocalZOrder(119);

    }

}
void LayerGameMauBinh::event_EXT_EVENT_REGISTER_QUEUE_NTF()
{
	boost::shared_ptr<string> lu = this->param->GetUtfString(EXT_FIELD_LIST_USER);
	if (lu != NULL)
	{
		log("DS Chau ria: %s", lu->c_str());
		this->layerAvatars->setListUserGuess(lu->c_str(), TL_CHAR_SPLIT_1);
		queueList = lu->c_str();
		bool isChauRia = this->isNguoiChauRia(lu->c_str());
		this->layerCards->setIsSpector(isChauRia);

	}
}
void LayerGameMauBinh::event_EXT_EVENT_LIST_USER_UPDATE(){
	boost::shared_ptr<string> listUser = param->GetUtfString(EXT_FIELD_LIST_USER);
	if (listUser == NULL) return;
	myListUser = listUser->c_str();
	log("event_EXT_EVENT_LIST_USER_UPDATE %s", listUser->c_str());
	//if (this->listUser == listUser->c_str()) return;
    int _size = mUtils::splitString(myListUser, '-').size();
    if (_size >= 4){
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
		this->resetGame();
		this->layerCards->removeTimer();

	}
}
void LayerGameMauBinh::event_EXT_EVENT_GAME_RESULT(){
    if (this->isHoa) return;
	boost::shared_ptr<string> rg = param->GetUtfString("rg");
	if (rg){
		log("result game %s", rg->c_str());
		string strrg = *rg;
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameMauBinh::updateTimeXepBai),this);
        this->hideAllButton();
		this->layerCards->showSochi(strrg);
		layerCards->setLocalZOrder(115);
        layerAvatars->setLocalZOrder(116);
	}
}
void LayerGameMauBinh::event_EXT_EVENT_END_GAME_NOTIF(){
    if (this->isHoa) return;
	boost::shared_ptr<string> listCard = param->GetUtfString("lc");
	if (listCard){
		log(" game end %s", listCard->c_str());
        
		this->layerCards->hideXepBaiMauBinh();
		this->layerCards->latBai(*listCard);
		this->layerAvatars->stopAllTimer();
	}
}
void LayerGameMauBinh::event_EXT_EVENT_CHANGE_BALANCE_NOTIFY(){
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
		std::string res = "";
		if (money > 0)
			res = "+" + StringUtils::toString(money);
		else if (money < 0)
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
			ressonString = "Góp gà";
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
			layerAvatars->setResult(pos, resson, kGameMauBinh, false, money);

			if (this->layerCards->IsMee(name->c_str())) {
				if (money > 0){
					this->layerCards->playSound(103);

				}
				else if (money < 0){
					this->layerCards->playSound(104);
				}

			}
		}

	}
}
void LayerGameMauBinh::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	log("Update User Variables");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	if (ptrNotifiedUser != NULL){
		boost::shared_ptr<string> name_ptr = ptrNotifiedUser->Name();
		if (name_ptr)
		{
			if (0 == name_ptr->compare(myName))
			{
				//                boost::shared_ptr<UserVariable> uck_ptr = ptrNotifiedUser->GetVariable("uck");//gia tri bat tat ga cua tung user
				//                if (uck_ptr)
				//                {
				//                    int uck = *uck_ptr->GetIntValue();
				//                    layerGa->setConfigChicken(uck);
				//                    //isNuoiGa = uck == LayerGa::CONFIG::ON ? true : false;
				//                    setUckToUserDefault(uck == LayerGa::CONFIG::ON ? true : false);
				//                }
			}
		}

		// 		boost::shared_ptr<UserVariable> amf = ptrNotifiedUser->GetVariable("amf");
		//
		// 		if (amf== NULL) return;
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
	}

}

void LayerGameMauBinh::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);
	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> ptrNotifiedMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);
	//
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

void LayerGameMauBinh::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
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
            string sRoomIdDisplay = mUtils::getRoomIDByName(*room->Name());
            if (sRoomIdDisplay != "")
                roomID = atoi(sRoomIdDisplay.c_str());
            
			//HoangDD Change
			//			string sRoomIdDisplay = mUtils::getRoomIDByName(*room->Name());
			//			if (sRoomIdDisplay != "")
			//				roomID = atoi(sRoomIdDisplay.c_str());
			layerBackGround->setNameTable(*name, roomID, money_bet);

			boost::shared_ptr<RoomVariable> chicken_value = room->GetVariable("chicken");
			boost::shared_ptr<RoomVariable> endtime_value = room->GetVariable("endtime");
            boost::shared_ptr<RoomVariable> timesochi = room->GetVariable("timesochi");

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
			if (endtime_value != NULL)
			{
				if (endtime_value->GetIntValue())
				{
					int end_time = *endtime_value->GetDoubleValue();
					if (layerTimer)
						layerTimer->setCount(end_time / 1000);
					this->timerReady = end_time / 1000;

				}
			}
		}
	}
}

std::string LayerGameMauBinh::find_ChuPhong(string listUser)
{
	if (listUser == "") return "";
	vector<string> arrUser = split(listUser, TL_CHAR_SPLIT_1);
	vector<string> info = split(arrUser[0], TL_CHAR_SPLIT_2);
	if (info.size() <= 1) return "";
	string boosId = info[1];
	return boosId;
}

void LayerGameMauBinh::isWinner()
{
	ui::ImageView* win = ui::ImageView::create();
	win->loadTexture("pic/win.jpg");
	this->addChild(win);
	win->setPosition(this->getContentSize() / 2);
	win->runAction(Repeat::create(Sequence::create(FadeOut::create(0.5), FadeIn::create(0.5), NULL), 5));
}


void LayerGameMauBinh::actionUnReady(Ref *pSender, Widget::TouchEventType pType)
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

void LayerGameMauBinh::resetGame()
{
    this->numDone = 0;
	queueList = "";
	isRegistSittingDown = false;
	//myListCards = "";
	//previousCards = "";
	currentPlaying = "";
	//flagRollBack = false;
	//isBaoLayerGameMauBinh = false;
	// Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(LayerGameMauBinh::resetCard));
	layerCards->resetGame();
	layerAvatars->setUnReadyAllUser();
	layerAvatars->setLocalZOrder(80);
	layerCards->setLocalZOrder(110);
	layerButtons->setLocalZOrder(140);
    this->removeChildByTag(this->HOA_IMG_TAG);
    this->removeChildByTag(1430);
	this->layerButtons->resetBangCuoc(-1, -1, -1);

	//layerTimer->startTimer();
	leaveUser = "";
	this->hideAllButton();
    this->isHoa = false;
}

void LayerGameMauBinh::createTimer()
{
	layerTimer = LayerTimer::create();
	layerTimer->setCount(15);
	layerTimer->setLocalZOrder(90);
	this->addChild(layerTimer);
}

void LayerGameMauBinh::createLayerGa()
{
	layerGa = LayerGa::create();
	layerGa->resetAll();
	layerGa->callBackFromActionGa(this, callfunc_selector(LayerGameMauBinh::actionConfigGa));
	this->addChild(layerGa);
}

void LayerGameMauBinh::updateEvent(float dt)
{
	if (mThreadQueueMsg == NULL) return;
	if (mThreadQueueMsg->isQueueEmpty())
		return;
	mThreadQueueMsg->run();
}


void LayerGameMauBinh::hideAllButton()
{
	log("hideAllButton");
	getButtonByTag(kTagButtonReady)->setVisible(false);
	getButtonByTag(kTagButtonSort)->setVisible(false);
	getButtonByTag(kTagButtonHit)->setVisible(false);
	getButtonByTag(kTagButtonNextTurn)->setVisible(false);
}



void LayerGameMauBinh::actionLeaveGame(Ref* pSender)
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

void LayerGameMauBinh::showListUserChicken()
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



bool LayerGameMauBinh::isNguoiChauRia(string _list)
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



void LayerGameMauBinh::cardInGameCallBack(Ref *pSender)
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

void LayerGameMauBinh::showArrowInButton(Button* btn, bool isShow)
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
void LayerGameMauBinh::event_EXT_EVENT_START_GAME_NOTIF(){
	log("event_EXT_EVENT_START_GAME_NOTIF");
	this->resetGame();
	layerTimer->stopTimer();
	isStartedGame = true;
	hideAllButton();
	this->layerCards->removeTimer();

	//layerGa->showToast(false);
	// hiển thị số bài trên tay của mỗi người (default=13) voi tien len, con lai sam la 10 - hien tai dang bo
	//int gameID = SceneManager::getSingleton().getGameID();
	//vector<string> lst = mUtils::splitString(myListUser, TL_CHAR_SPLIT_1);
	//    int first_card = 13;
	//    if (gameID == kGameSam || gameID == kGameSamSolo)
	//        first_card = 10;
	//    for (int i = 0; i < lst.size(); i++)
	//    {
	//        vector<string> info = mUtils::splitString(lst[i], TL_CHAR_SPLIT_2);
	//        if (info.size()>1)
	//            layerCards->setCountCardByPos(layerAvatars->getPosByName(info[1]), first_card);
	//    }

}
void LayerGameMauBinh::updateTimeXepBai(float dt){

	if (this->timeXepbai > 0)
		this->timeXepbai--;
    else{
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameMauBinh::updateTimeXepBai),this);
        this->hideAllButton();
    }

	this->layerCards->setTimeXepBai(this->timeXepbai);
}
void LayerGameMauBinh::event_EXT_EVENT_FINISH_CARD_LINK_NOTIFY(){
	boost::shared_ptr<string> uid = param->GetUtfString("uid");
	boost::shared_ptr<long> cs = param->GetInt("cs");
	if (uid != NULL && cs != NULL){
		int pos = this->layerAvatars->getPosByName(*uid);
		BaiMauBinh* bobai = this->layerCards->getBoBaiByPos(pos);
		if (bobai){
			if (*cs == 0) {
                if (this->numDone > 0){
                    this->numDone--;
                }
				bobai->updateStatus(false);
				layerAvatars->runTimerBinh(pos, this->timeXepbai);
				if (this->layerCards->IsMee(*uid)) {
					this->getButtonByTag(kTagButtonSort)->setVisible(false);

				}
				else{
					this->layerCards->setIsXepBai(false);

				}
			}
			else{
                this->numDone++;
                
				bobai->updateStatus(true);
				Avatar52La* avatar = layerAvatars->getUserByPos(pos);
				if (avatar)
				{
					avatar->stopTimer();
				}
				if (this->layerCards->IsMee(*uid)) {
					this->getButtonByTag(kTagButtonSort)->setVisible(true);
				}
				else{
					this->layerCards->setIsXepBai(true);

				}
			}
            if (this->numDone == this->layerAvatars->getUserCount()){
                this->hideAllButton();
            }
		}
	}
}
void LayerGameMauBinh::event_EXT_EVENT_DEAL_CARD_NOTIF(){
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
		layerCards->actionDealCard(*listCard);
		//        if (!isSpector && (this->gameID == kGameSam || gameID == kGameSamSolo)) {
		//            playSound(S_DEAL);
		//            showBaoSam();
		//        }

	}
	boost::shared_ptr<long> drt = param->GetInt(EXT_FIELD_DURATION_TIME);
	if (drt){
		this->timeXepbai = *drt / 1000;
		Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameMauBinh::updateTimeXepBai),this, 1, false);
	}
}

void LayerGameMauBinh::event_EXT_EVENT_START_READY_TIMER_NOTIF()
{
	this->resetGame();
	getButtonByTag(kTagButtonReady)->setVisible(true);
	this->layerCards->runTimerReady(this->timerReady);
	layerTimer->startTimer();
}
void LayerGameMauBinh::event_EXT_EVENT_USER_JOIN_NOTIF(){
	log("EXT_EVENT_USER_JOIN_NOTIF");
	boost::shared_ptr<double> tienga = param->GetDouble(EXT_FIELD_CHICKEN_VALUE);
	boost::shared_ptr<long> lv_ga = param->GetInt(EXT_FIELD_CHICKEN_LEVEL);
	boost::shared_ptr<string> listUser = param->GetUtfString(EXT_FIELD_LIST_USER);
	if (listUser != NULL) {
		this->myListUser = listUser->c_str();
		log("listUser= %s", listUser->c_str());
		//lay thong tin ve ga
		queueList = "";
        
		layerAvatars->setListUserByTienLen(this->myListUser);
		isStartedGame = layerAvatars->isStartedGame();
		isSpector = layerAvatars->isSpectator();

		//        if (tienga != NULL && lv_ga != NULL && *tienga > 0 && *lv_ga > 0 && listUser != NULL){
		//            layerGa->setChickenMoney(*tienga);
		//            layerGa->setChickenLv(*lv_ga);
		//        }


		if (isSpector){
			layerButtons->SetIsGuess(true);
			if (layerAvatars->getUserCount() >= 4)
				layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(253));
            if (layerAvatars->getUserCount() >= 4){
                this->layerButtons->processDisplayButtonInvite(false);
            }
            else{
                if (this->vipRoom == 1){
                    this->layerButtons->processDisplayButtonInvite(true);
                }
            }
		}
		if (!isStartedGame && !isSpector && layerAvatars->getUserCount() > 1 && layerTimer->isVisible()){
			this->resetGame();
			layerTimer->startTimer();
			getButtonByTag(kTagButtonReady)->setVisible(true);
		}
		if (isStartedGame && !isSpector) {
			log("JOIN SUCCESS");
			layerButtons->SetIsGuess(false);
			//getButtonByTag(kTagButtonSort)->setVisible(true);
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















