//
//  LayerPlayGamePhom.cpp
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//

#include "LayerPlayGamePhom.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "Requests/LeaveRoomRequest.h"
#include "xmllite/xmlParser.h"
#include "LayerListGa.h"
#include "PhomMessDef.h"
using namespace CocosDenshion;

/*
enum EXT_RESPONSE {
//
EXT_SRVNTF_PUBLIC_MESSAGE = 0,
EXT_SRVNTF_PRIVATE_MESSAGE,
EXT_SRVNTF_GAME_MASTER_INFO,
EXT_SRVNTF_READY,
EXT_SRVNTF_PLAYER_LIST,
EXT_SRVNTF_RECOMMENDED_CARDSETS,
EXT_EVENT_START,
EXT_EVENT_END,
EXT_EVENT_GAME_RESULT,
EXT_EVENT_LISTCARD_NTF,
EXT_EVENT_COIN_NTF,
EXT_SRVNTF_CURRENT_PLAYING_USER,
EXT_SRVNTF_END_PLAYING_GAME,
EXT_SRVNTF_GAME_INFO,
EXT_EVENT_READY_RES,
EXT_EVENT_READY_REQ,
EXT_EVENT_UNREADY_RES,
EXT_EVENT_UNREADY_REQ,
EXT_SRVNTF_USER_READY,
EXT_SRVNTF_CARD_ORIGINATION,
EXT_EVENT_RES_DRAW_CARD,
EXT_EVENT_RES_TAKE_CARD,
EXT_EVENT_RES_DISCARD,
EXT_EVENT_RES_HANDOFF_PHOM,
EXT_EVENT_RES_END_PLAYING,
EXT_EVENT_RES_SET_BET,
EXT_EVENT_RES_U,
EXT_SRVNTF_CAN_TAKE,
EXT_SRVNTF_PLAYER_REPLAY,
EXT_SRVNTF_U_9,
EXT_SRVNTF_U, // cac loai u
EXT_SRVNTF_COIN_CHANGE,
EXT_EVENT_RES_ORDER_CARDS,
EXT_EVENT_RES_PUSH_CARD,
EXT_EVENT_CHICKEN_UPDATE_NTF,//"chkntf"
EXT_EVENT_REQ_JOIN_GAME,
EXT_EVENT_RES_JOIN_GAME,
EXT_EVENT_REQ_LEAVE_GAME,
EXT_EVENT_RES_LEAVE_GAME,
EXT_SRVNTF_PLAYER_KICKOUT,
EXT_EVENT_REQ_ORDER_CARDS,
EXT_EVENT_REQ_DRAW_CARD,
EXT_EVENT_REQ_DISCARD,
EXT_EVENT_REQ_TAKE_CARD,
EXT_EVENT_REQ_HANDOFF_PHOM,
EXT_EVENT_REQ_PUSH_CARD,
EXT_EVENT_REQ_U,
EXT_EVENT_UNREADY_NTF, //rdtntf
EXT_EVENT_NTF_TIMER,// = "tupd";                    // timer update
EXT_EVENT_REQ_UPDATE_GAME_CONFIG,// "rqgcfg",//bat tat ga
EXT_EVENT_RES_UPDATE_GAME_CONFIG,// = "rsgcfg";     // reponse update game cfg
EXT_SRVNTF_NOC_COUNT,
EXT_EVENT_REGISTER_QUEUE_NTF, //"rqntf";//ban tin chau ria
EXT_SRVNTF_RECOMMENDED_PUSH_CARDS,// goi y gui phom ntfpucs

CARD_ORIGINATION_BY_TAKEN = 1, // ăn được
CARD_ORIGINATION_BY_DRAWN = 2, // bốc từ nọc
CARD_ORIGINATION_BY_TRANSFERED = 3, // quân bài chuyển
CARD_ORIGINATION_BY_DEALT = 4, // chia bài
CARD_ORIGINATION_BY_HANDOFF = 5, // chia bài
CARD_ORIGINATION_BY_PUSHED = 6, // chia bài
CARD_ORIGINATION_BY_UNKOWN = 7, // chia bài

};

int LayerPlayGamePhom::convertResponseToInt(string inString) {
if (inString == "ntfpubmsg") return  EXT_SRVNTF_PUBLIC_MESSAGE;
if (inString == "ntfprvmsg") return  EXT_SRVNTF_PRIVATE_MESSAGE;
if (inString == "ntfgminfo") return  EXT_SRVNTF_GAME_MASTER_INFO;
if (inString == "rsr") return  EXT_SRVNTF_READY;
if (inString == "ntfpllst") return  EXT_SRVNTF_PLAYER_LIST;
if (inString == "ntfrcrds") return  EXT_SRVNTF_RECOMMENDED_CARDSETS;
if (inString == "s") return  EXT_EVENT_START;
if (inString == "e") return  EXT_EVENT_END;
if (inString == "grs") return  EXT_EVENT_GAME_RESULT;
if (inString == "lcntf") return  EXT_EVENT_LISTCARD_NTF;
if (inString == "ntfcoinch") return  EXT_EVENT_COIN_NTF;
if (inString == "ntfcurpu") return  EXT_SRVNTF_CURRENT_PLAYING_USER;
if (inString == "ntfepla") return  EXT_SRVNTF_END_PLAYING_GAME;
if (inString == "ntfginfo") return  EXT_SRVNTF_GAME_INFO;
if (inString == "rr") return EXT_EVENT_READY_REQ;
if (inString == "rdres") return  EXT_EVENT_READY_RES;
if (inString == "ntfurd") return  EXT_SRVNTF_USER_READY;
if (inString == "rsur") return EXT_EVENT_UNREADY_RES;
if (inString == "urr") return EXT_EVENT_UNREADY_REQ;
if (inString == "ntfcorg") return  EXT_SRVNTF_CARD_ORIGINATION;
if (inString == "rsdrwc") return  EXT_EVENT_RES_DRAW_CARD;
if (inString == "rstkc") return  EXT_EVENT_RES_TAKE_CARD;
if (inString == "rshofc") return  EXT_EVENT_RES_DISCARD;
if (inString == "rshofp") return  EXT_EVENT_RES_HANDOFF_PHOM;
if (inString == "rsenpl") return  EXT_EVENT_RES_END_PLAYING;
if (inString == "rssbet") return  EXT_EVENT_RES_SET_BET;
if (inString == "rsU") return  EXT_EVENT_RES_U;
if (inString == "chkntf") return EXT_EVENT_CHICKEN_UPDATE_NTF;

if (inString == "ntfcantk") return  EXT_SRVNTF_CAN_TAKE;
if (inString == "ntfrpl") return  EXT_SRVNTF_PLAYER_REPLAY;
if (inString == "ntfu9") return  EXT_SRVNTF_U_9;
if (inString == "ntfu") return EXT_SRVNTF_U;
if (inString == "rsodrc") return  EXT_EVENT_RES_ORDER_CARDS;
if (inString == "rspusc") return EXT_EVENT_RES_PUSH_CARD;
//
if (inString == "rqjg") return EXT_EVENT_REQ_JOIN_GAME;
if (inString == "rsjg") return EXT_EVENT_RES_JOIN_GAME;
if (inString == "rqlg") return EXT_EVENT_REQ_LEAVE_GAME;
if (inString == "rslg")	return EXT_EVENT_RES_LEAVE_GAME;
if (inString == "ntfko") return EXT_SRVNTF_PLAYER_KICKOUT;
if (inString == "rodrc") return EXT_EVENT_REQ_ORDER_CARDS;
if (inString == "rqdrwc") return EXT_EVENT_REQ_DRAW_CARD;
if (inString == "rqhofc") return EXT_EVENT_REQ_DISCARD;
if (inString == "rqtkc") return EXT_EVENT_REQ_TAKE_CARD;
if (inString == "rqhofp") return EXT_EVENT_REQ_HANDOFF_PHOM;
if (inString == "rqpusc") return EXT_EVENT_REQ_PUSH_CARD;
if (inString == "rqU") return EXT_EVENT_REQ_U;
if (inString == "urntf") return EXT_EVENT_UNREADY_NTF;
if (inString == "tupd")		return EXT_EVENT_NTF_TIMER;
if (inString == "rqgcfg") return EXT_EVENT_REQ_UPDATE_GAME_CONFIG;
if (inString == "rsgcfg")	return EXT_EVENT_RES_UPDATE_GAME_CONFIG;
if (inString == "ntfnoccnt")			return EXT_SRVNTF_NOC_COUNT;
if (inString == "rqntf") return EXT_EVENT_REGISTER_QUEUE_NTF;
if (inString == "ntfpucs") return EXT_SRVNTF_RECOMMENDED_PUSH_CARDS;
return -1;
}

string LayerPlayGamePhom::convertResponseToString(int inInt) {
if (inInt == EXT_SRVNTF_PUBLIC_MESSAGE) return "ntfpubmsg";
if (inInt == EXT_SRVNTF_PRIVATE_MESSAGE) return "ntfprvmsg";
if (inInt == EXT_SRVNTF_GAME_MASTER_INFO) return "ntfgminfo";
if (inInt == EXT_SRVNTF_READY) return  "rsr";
if (inInt == EXT_SRVNTF_PLAYER_LIST) return  "ntfpllst";
if (inInt == EXT_SRVNTF_RECOMMENDED_CARDSETS) return  "ntfrcrds";
if (inInt == EXT_EVENT_START) return  "s";
if (inInt == EXT_EVENT_END) return  "e";
if (inInt == EXT_EVENT_GAME_RESULT) return  "grs";
if (inInt == EXT_EVENT_LISTCARD_NTF) return  "lcntf";
if (inInt == EXT_EVENT_COIN_NTF) return  "ntfcoinch";
if (inInt == EXT_SRVNTF_CURRENT_PLAYING_USER) return  "ntfcurpu";
if (inInt == EXT_SRVNTF_END_PLAYING_GAME) return  "ntfepla";
if (inInt == EXT_SRVNTF_GAME_INFO) return  "ntfginfo";
if (inInt == EXT_EVENT_READY_REQ) return "rr";
if (inInt == EXT_EVENT_READY_RES) return  "rdres";
if (inInt == EXT_SRVNTF_USER_READY) return  "ntfurd";
if (inInt == EXT_EVENT_UNREADY_REQ) return "urr";
if (inInt == EXT_SRVNTF_CARD_ORIGINATION) return  "ntfcorg";
if (inInt == EXT_EVENT_RES_DRAW_CARD) return  "rsdrwc";
if (inInt == EXT_EVENT_RES_TAKE_CARD) return  "rstkc";
if (inInt == EXT_EVENT_RES_DISCARD) return  "rshofc";
if (inInt == EXT_EVENT_RES_HANDOFF_PHOM) return  "rshofp";
if (inInt == EXT_EVENT_RES_END_PLAYING) return  "rsenpl";
if (inInt == EXT_EVENT_RES_SET_BET) return  "rssbet";
if (inInt == EXT_EVENT_RES_U) return  "rsU";
if (inInt == EXT_SRVNTF_CAN_TAKE) return  "ntfcantk";
if (inInt == EXT_SRVNTF_PLAYER_REPLAY) return  "ntfrpl";
if (inInt == EXT_SRVNTF_U_9) return  "ntfu9";
if (inInt == EXT_SRVNTF_U) return "ntfu";
if (inInt == EXT_EVENT_RES_ORDER_CARDS) return  "rsodrc";
if (inInt == EXT_EVENT_RES_PUSH_CARD) return "rspusc";
//
if (inInt == EXT_EVENT_REQ_JOIN_GAME) return  "rqjg";
if (inInt == EXT_EVENT_RES_JOIN_GAME) return "rsjg";
if (inInt == EXT_EVENT_REQ_LEAVE_GAME) return "rqlg";
if (inInt == EXT_EVENT_RES_LEAVE_GAME)	return "rslg";
if (inInt == EXT_SRVNTF_PLAYER_KICKOUT) return "ntfko";
if (inInt == EXT_EVENT_REQ_ORDER_CARDS) return "rodrc";
if (inInt == EXT_EVENT_REQ_DRAW_CARD) return "rqdrwc";
if (inInt == EXT_EVENT_REQ_DISCARD) return "rqhofc";
if (inInt == EXT_EVENT_REQ_TAKE_CARD) return "rqtkc";
if (inInt == EXT_EVENT_REQ_HANDOFF_PHOM) return "rqhofp";
if (inInt == EXT_EVENT_REQ_PUSH_CARD) return "rqpusc";
if (inInt == EXT_EVENT_REQ_U) return "rqU";
if (inInt == EXT_EVENT_UNREADY_NTF) return "urntf";
if (inInt == EXT_EVENT_CHICKEN_UPDATE_NTF) return "chkntf";
if (inInt == EXT_EVENT_NTF_TIMER)			return "tupd";
if (inInt == EXT_EVENT_REQ_UPDATE_GAME_CONFIG) return "rqgcfg";
if (inInt == EXT_EVENT_RES_UPDATE_GAME_CONFIG) return "rsgcfg";
if (inInt == EXT_SRVNTF_NOC_COUNT) return "ntfnoccnt";
if (inInt == EXT_EVENT_REGISTER_QUEUE_NTF) return "rqntf";
if (inInt == EXT_SRVNTF_RECOMMENDED_PUSH_CARDS) return "ntfpucs";
return "";
}
*/
LayerPlayGamePhom::LayerPlayGamePhom() {
	mThreadQueueMsg = NULL;
	layerTimer = NULL;
	layerGa = NULL;
}

LayerPlayGamePhom::~LayerPlayGamePhom() {
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGamePhom::updateEvent),this);
	arrName.clear();
	arrMoney.clear();
	arrMoneyDouble.clear();
	GameServer::getSingleton().removeListeners(this);
	if (mThreadQueueMsg != NULL) delete mThreadQueueMsg;
	log("~~~~LayerPlayGamePhom");

}
bool LayerPlayGamePhom::init()
{
	if (!Layer::init()) return false;
	myReady = false;// co de kiem tra trang thai ready
	isEndGame = false; /// co kiem tra da end game hay chua
	this->myName = SceneManager::getSingleton().getMyName();
	this->isRegistSittingDown = false;
	this->isRegistStandUp = false;
	this->isNuoiGa = false;
	this->isResume = false;
	this->isU9 = false;
	this->isHaPhom = false;
	lastHitCard = 0;
	countRequestJoin = 0;

	sizeAdd = mUtils::getSizePos();
	this->setAnchorPoint(Point(0.5, 0.5));
	this->setPosition(Vec2::ZERO);

	//
	/*this->levelGame = 0;*/
	isFirstReady = true;
	this->actionLast = 0;
	readyTimeOut = 15;
	discardTimeout = 15;
	turnTimeout = 15;
	endGameTimeout = 5;//hien tai default dang la 8s

	mThreadQueueMsg = new ThreadQueueMsg();
	mThreadQueueMsg->setFunCallBack(this, callfunc_selector(LayerPlayGamePhom::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGamePhom::updateEvent),this,1/60, false);

	GameServer::getSingleton().addListeners(this);
	//
	createBackgrounds();
	createCards();
	createAvatars();
	createLayerGa();
	createLayerTimer();
	createChats();
	createNumbers();
	createButtons();
	initParam();
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGamePhom::sendRequestJoinGameInterval),this,0.5, false);
	return true;
}

void LayerPlayGamePhom::createBackgrounds() {
	BackgroundInGame *bg = BackgroundInGame::create();
	//bg->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
	this->addChild(bg);

	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL)
		return;

	// thông tin bàn chơi và mức cược
	boost::shared_ptr<RoomVariable> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params");
	if (param != NULL){
		string paramString = param->GetStringValue()->c_str();
		vector<string> arrInfo = split(paramString, '@');
		string money = arrInfo.at(0);
		string name = *mUtils::getGameNameByID(kGamePhom);
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
		bg->setNameTable(name, roomID, money_bet);
	}
}

void LayerPlayGamePhom::createAvatars() {
	this->layerAvatars = LayerAvatarInGame52La::create();
	//this->layerAvatars->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
	this->addChild(layerAvatars);
}

void LayerPlayGamePhom::createButtons() {
	this->layerButtons = LayerButtonInGame::create();
	//this->layerButtons->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
	this->addChild(this->layerButtons);
    if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL){
        return;
    }
	boost::shared_ptr<RoomVariable> isVipRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("isviproom");
	if (isVipRoom != NULL){
		this->vipRoom = *isVipRoom->GetIntValue();
		if (vipRoom == 0){
			this->layerButtons->processDisplayButtonInvite(false);
		}
		else{
			this->layerButtons->processDisplayButtonInvite(true);
		}
	}
	//log("priority button %d", layerButtons->getTouchPriority());
	winSize = Director::getInstance()->getVisibleSize();
	int widButton1 = 157;
	int widButton2 = 157;
	int heiButton = 148;
	int space = 100;
	int w_btnChat = 82;
	createButtonWith_Tag_Title_Position(kTagButtonReady, "button_batdau.png",
		Point(winSize.width / 2, space), "");
	createButtonWith_Tag_Title_Position(kTagButtonSort, "button_xepbai.png",
		Point(winSize.width / 2 - widButton1 * 2, space), "");
	createButtonWith_Tag_Title_Position(kTagButtonHit, "button_danh.png",
		Point(winSize.width / 2, space), "");
	createButtonWith_Tag_Title_Position(kTagButtonGive, "groupCard.png",
		Point(winSize.width / 2, winSize.height / 2 + SIZE_ADD - 10), "");
	createButtonWith_Tag_Title_Position(kTagButtonPush, "button_gui.png",
		Point(winSize.width / 2 + widButton1 * 2, space),
		"");
	createButtonWith_Tag_Title_Position(kTagButtonEat, "button_an.png",
		Point(winSize.width / 2 + widButton1 * 3, space),
		"");
	createButtonWith_Tag_Title_Position(kTagButtonHaPhom, "button_ha.png",
		Point(winSize.width / 2 + widButton1 * 4, space),
		"");
	createButtonWith_Tag_Title_Position(kTagButtonU, "button_u.png",
		Point(winSize.width / 2 + widButton1 * 2, space),
		"");
	// 	createButtonWith_Tag_Title_Position(kTagButtonSitting, "btn-ngoichoi.png", Point(winSize.width - space, space));
	// 	createButtonWith_Tag_Title_Position(kTagButtonStandUp, "btn-stanup.png", Vec2(space * 2 + w_btnChat + widButton2, space));
	//

	this->arrowNoc = Sprite::create("arrow_phom.png");
	this->arrowNoc->setAnchorPoint(Vec2(0.5, 0.5));
	this->arrowNoc->setPosition(Vec2(WIDTH_DESIGN / 2 + sizeAdd.width,
		HEIGHT_DESIGN / 2 + 110 + sizeAdd.height + SIZE_ADD / 2));
	this->arrowNoc->setVisible(false);
	this->layerButtons->addChild(this->arrowNoc, 0, 24);

	labelNoc = Label::createWithSystemFont("", "", FONT_SIZE_NORMAL,
		Size(FONT_SIZE_NORMAL * 2, FONT_SIZE_NORMAL),
		TextHAlignment::CENTER,
		TextVAlignment::CENTER);
	labelNoc->setAnchorPoint(Vec2(0.5, 0.5));
	labelNoc->setColor(Color3B::BLACK);
	labelNoc->setPosition(Vec2(45, 55));

	getButtonByTag(kTagButtonGive)->addChild(labelNoc);

	getButtonByTag(kTagButtonReady)->addTouchEventListener(CC_CALLBACK_2(LayerPlayGamePhom::actionReady, this));
	getButtonByTag(kTagButtonSort)->addTouchEventListener(CC_CALLBACK_2(LayerPlayGamePhom::actionSortCards, this));
	getButtonByTag(kTagButtonHit)->addTouchEventListener(CC_CALLBACK_2(LayerPlayGamePhom::actionHitCards, this));
	getButtonByTag(kTagButtonGive)->addTouchEventListener(CC_CALLBACK_2(LayerPlayGamePhom::actionGiveCards, this));
	getButtonByTag(kTagButtonEat)->addTouchEventListener(CC_CALLBACK_2(LayerPlayGamePhom::actionEatCards, this));
	getButtonByTag(kTagButtonHaPhom)->addTouchEventListener(CC_CALLBACK_2(LayerPlayGamePhom::actionHaPhom, this));
	getButtonByTag(kTagButtonPush)->addTouchEventListener(CC_CALLBACK_2(LayerPlayGamePhom::actionPush, this));
	getButtonByTag(kTagButtonU)->addTouchEventListener(CC_CALLBACK_2(LayerPlayGamePhom::actionUUU, this));
}

void LayerPlayGamePhom::createNumbers() {
	layerNumbers = LayerNumberInGame::create();
	this->addChild(layerNumbers);
}

void LayerPlayGamePhom::createCards() {
	layerCards = LayerCardInGame::create();
	layerCards->setFunCallBack(this, callfuncO_selector(LayerPlayGamePhom::setFuncallBack));
	layerCards->initCardByPhom();
	this->addChild(layerCards);

}

void LayerPlayGamePhom::createChats() {
	//	layerChats = LayerChatToastInGame::create();
	//	this->addChild(layerChats);
}

void LayerPlayGamePhom::initGame() {
	// khởi tạo các giá trị ban đầu hoặc hiển thị các thông tin cần thiết
	this->actionLast = 0;

	// thông tin tiền hiện tại của Users
	for (int i = 0; i < arrName.size(); i++) {
		layerAvatars->setMoney(layerAvatars->getPosByName(arrName[i]), arrMoneyDouble[i]);
	}
	arrName.clear();
	arrMoney.clear();
	arrMoneyDouble.clear();

	// unready all
	layerAvatars->setUnReadyAllUser();
}

void LayerPlayGamePhom::resetGame() {
	isU9 = false;
	lastHitCard = 0;
	listCardHandOff = "";
	isHaPhom = false;
	myReady = false;
	isEndGame = false;
	this->currentPlaying = "";
	this->isResume = false;

	layerTimer->stopTimer();
	hideAllButton();
	initGame();
	layerCards->resetGame();
	layerAvatars->setUnReadyAllUser();

	Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(LayerPlayGamePhom::resetCard),this);
	Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(LayerPlayGamePhom::showButtonReady),this);

}

void LayerPlayGamePhom::playSound(int type)
{
	if (!mUtils::isSoundOn()) return;
	string soundPath;
	switch (type)
	{
	case S_EAT_CARD://an bai
		soundPath = "an_bai_4.mp3";
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
	{
				  int rand_int = rand() % 2;
				  if (rand_int == 1){
					  soundPath = "thua_roi_1.mp3";//hien thi thang thua
				  }
				  else{
					  soundPath = "thang_roi_1.mp3";//hien thi thang thua
				  }
	}

		break;
	case S_HIT_CARD:
	{
					   int rand_int = rand() % 2;
					   if (rand_int == 1){
						   soundPath = "thach_an_1.mp3";//hien thi thang thua
					   }
					   else{
						   soundPath = "an_di_1.mp3";//hien thi thang thua
					   }
	}
		break;
	case S_GIVE_CARD:
		soundPath = "boc_bai.mp3";//boc bai
		break;
	case S_HAND_OFF:
		soundPath = "ha_phom.mp3";//ha phom
		break;
	case S_U:
		soundPath = "u_1.mp3";//u
		break;
	case S_SORT:
		soundPath = "XepBai.mp3";
		break;
	default:
		break;
	}

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundPath.c_str());
}

void LayerPlayGamePhom::createButton_PushMulti_By_CardId(int cardid) {

	Card *card = layerCards->getCardByID(cardid);
	if (card == NULL) return;

	Button* button = Button::create();
	button->setTouchEnabled(true);
	button->setScale9Enabled(false);
	//button->setPressedActionEnabled(true);
	button->setTitleText(dataManager.GetSysString(217));
	button->setTitleFontSize(FONT_SIZE_SMALL);
	button->loadTextures("btn-batdau.png", "", "");
	button->setAnchorPoint(Point(0.5, 0.5));
	button->addTouchEventListener(CC_CALLBACK_2(LayerPlayGamePhom::actionPushMulti, this));
	button->setPosition(Point(card->getPositionX() + 60, card->getPositionY() + 30));
	button->setTag(kTagButtonPushMulti);
	layerButtons->addChild(button);
	arrButtonCanPush.push_back(button);
}

void LayerPlayGamePhom::createButtonWith_Tag_Title_Position(int tag, string imgPath, Point pPoint, string title) {
	// Create the button
	Button* button = Button::create();
	//button->setTitleFontName(kNormalFont);//neu khong co ten font, title hien thi se bi lech
	//button->setPressedActionEnabled(true);
	button->setTouchEnabled(true);
	button->setScale9Enabled(false);
	button->setTitleText(title);
	button->setTitleFontSize(FONT_SIZE_SMALL);
	button->setPosition(Vec2(pPoint.x + sizeAdd.width, pPoint.y));

	//	if (tag != kTagButtonGive && tag != kTagButtonReady)
	//	{
	//		button->setAnchorPoint(Vec2(1, 0));
	//		button->loadTextures("ingame-button.png", "ingame-button-active.png", "");
	//		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	//			button->setTitleColor(Color3B::WHITE);
	//	}
	//	else
	//	{
	button->setAnchorPoint(Vec2(0.5, 0.5));
	button->loadTextures(imgPath, imgPath, imgPath);
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
		button->setTitleColor(Color3B::BLACK);
	//	}
	button->setTag(tag);
	button->setVisible(false);
	layerButtons->addChild(button);
}

Button* LayerPlayGamePhom::getButtonByTag(int tag) {
	Button *button = (Button*) this->layerButtons->getChildByTag(tag);
	return button;
}

void LayerPlayGamePhom::hideAllButton() {
	int user_count = layerAvatars->getUserCount();
	int init_noc_count = 52 - ((user_count - 1) * 9 + 10);
	labelNoc->setString(StringUtils::format("%d", init_noc_count));
	showBocNoc(false);
	getButtonByTag(kTagButtonGive)->setVisible(false);
	getButtonByTag(kTagButtonSort)->setVisible(false);
	getButtonByTag(kTagButtonHit)->setVisible(false);
	getButtonByTag(kTagButtonEat)->setVisible(false);
	getButtonByTag(kTagButtonHaPhom)->setVisible(false);
	getButtonByTag(kTagButtonPush)->setVisible(false);
	getButtonByTag(kTagButtonU)->setVisible(false);

	getButtonByTag(kTagButtonReady)->setVisible(false);
	int size = arrButtonCanPush.size();
	if (size > 0) {
		for (int i = size - 1; i >= 0; i--)
		{
			arrButtonCanPush.at(i)->setVisible(false);
			arrButtonCanPush.at(i)->setTouchEnabled(false);
			layerButtons->removeChild(arrButtonCanPush.at(i), true);
		}
		arrButtonCanPush.clear();
	}
}

void LayerPlayGamePhom::actionReady(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		// 		layerTimer->startTimer();
		// 		return;
		// 		layerAvatars->getUserByPos(kUserMe)->startTimer(15);
		// 		return;
		// 		string lc = "9/34:9:3:0:4:-1;18:6:2:0:4:-1;35:10:3:0:4:-1;33:8:3:0:4:-1;19:7:2:0:4:-1;15:3:2:0:4:-1;27:2:3:0:4:-1;32:7:3:0:4:-1;6:7:1:0:2:-1/30:5:3:0:5:-1;25:13:2:0:5:-1;48:10:4:0:5:-1";
		// 		layerCards->eventListcardNTF(kUserMe, lc);
		// 		layerCards->actionGiveCard(kUserMe, 10);
		// 		return;
		// 		 		return;
		//layerCards->resetGame();
		//getButtonByTag(kTagButtonReady)->setVisible(false);
		//		string listcard = "7/26:1:3:0:4:-1;4:5:1:0:4:-1;30:5:3:0:4:-1;45:7:4:0:4:-1;7:8:1:0:4:-1;46:8:4:0:4:-1;16:4:2:0:2:-1/10:11:1:0:5:-1;38:13:3:0:5:-1;22:10:2:0:5:-1/2:3:1:1:2:1;15:3:2:1:4:1;28:3:3:1:4:1;";
		// 		string listcard4 = "4//12:13:1:0:5:-1;50:12:4:0:5:-1;9:10:1:0:5:-1/3:4:1:1:4:1;29:4:3:1:4:1;42:4:4:1:4:1;8:9:1:1:2:2;34:9:3:1:2:2;47:9:4:1:2:2;";
		// 		// 		//  		layerCards->actionEatCard(kUserTop, kUserBot, 5);
		// 		// 		//  		layerCards->actionEatCard(kUserTop, kUserBot, 6);
		// 		// 		//listcard for ha phom
		// 		string listcard = "9/27:2:3:1:4:-1;40:2:4:1:4:-1;28:3:3:1:4:-1;16:4:2:1:4:-1;30:5:3:1:4:-1;44:6:4:1:4:-1;33:8:3:1:2:-1;46:8:4:1:4:-1;38:13:3:1:2:-1/50:12:4:0:5:-1;32:7:3:0:5:-1;10:11:1:0:5:-1;23:11:2:0:3:-1";
		// 		string listcard2 = "6/13:1:2:1:4:-1;1:2:1:1:4:-1;15:3:2:1:4:-1;41:3:4:1:4:-1;4:5:1:1:4:-1;43:5:4:1:4:-1/12:13:1:0:5:-1;48:10:4:0:5:-1;47:9:4:0:5:-1;18:6:2:0:5:-1/6:7:1:1:1:1;19:7:2:1:2:1;45:7:4:1:4:1;";
		// 		string listcard3 = "6/29:4:3:1:4:-1;42:4:4:1:4:-1;5:6:1:1:4:-1;31:6:3:1:2:-1;8:9:1:1:4:-1;9:10:1:1:2:-1/51:13:4:0:5:-1;14:2:2:0:5:-1;17:5:2:0:5:-1;49:11:4:0:5:-1/11:12:1:1:2:1;24:12:2:1:4:1;37:12:3:1:1:1;";
		// 		layerCards->eventListcardNTF(kUserTop, listcard3);
		// 		layerCards->eventListcardNTF(kUserLeft, listcard);
		// 		layerCards->eventListcardNTF(kUserMe, listcard2);
		// 		layerCards->setIdCardCanEat(1);
		// 		layerCards->actionHitCard(kUserTop, 1);
		//  		layerCards->actionEatCard(kUserTop, kUserRight, 1);
		// 		layerCards->actionEatCard(kUserTop, kUserRight, 2);
		// 		layerCards->actionEatCard(kUserTop, kUserRight, 3);
		//  		layerCards->actionEatCard(kUserTop, kUserBot, 6);
		// 		layerCards->showCardOnHandAll();
		// 		return;
		//test 2
		// 		string listcard1 = "3/4:5:1:1:4:-1;6:7:1:1:2:-1;45:7:4:1:4:-1/48:10:4:0:5:-1;50:12:4:0:5:-1;11:12:1:0:5:-1;10:11:1:0:5:-1/3:4:1:1:4:1;29:4:3:1:4:1;42:4:4:1:1:1;25:13:2:1:1:2;38:13:3:1:4:2;51:13:4:1:4:2;";
		// 		string listcard2 = "5/39:1:4:1:4:-1;40:2:4:1:4:-1;2:3:1:1:4:-1;16:4:2:1:2:-1;12:13:1:1:4:-1/28:3:3:0:5:-1;36:11:3:0:3:-1;44:6:4:0:5:-1;47:9:4:0:3:-1/18:6:2:1:4:1;19:7:2:1:2:1;20:8:2:1:4:1;21:9:2:1:4:1;";
		// 		string listcard3 = "6/0:1:1:1:4:-1;26:1:3:1:4:-1;14:2:2:1:4:-1;15:3:2:1:4:-1;41:3:4:1:2:-1;30:5:3:1:4:-1/32:7:3:0:5:-1;8:9:1:0:3:-1;37:12:3:0:5:-1;43:5:4:0:5:-1/7:8:1:1:2:1;33:8:3:1:4:1;46:8:4:1:4:1;";
		// 		layerCards->eventListcardNTF(kUserLeft, listcard1);
		// 		layerCards->eventListcardNTF(kUserRight, listcard2);
		// 		layerCards->eventListcardNTF(kUserMe, listcard3);
		// 		return;
		// 		
		// 		return;
		// 		layerCards->setIdCardCanEat(10);
		// 		return;
		//layerCards->actionHitCard(kUserMe, 13);
		//return;
		//////this.layerCards.setIdCardCanEat(10);
		//this.layerCards.showCardOnHandAll();
		//return;
		// 		float time_vale = 30000;
		// 		layerAvatars->runTimerWithTime(kUserMe, time_vale / 1000);
		// 		bool isSoundOn = mUtils::isSoundOn();
		// 		return;
		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
		GameServer::getSingleton().Send(request);
	}
}

void LayerPlayGamePhom::actionSortCards(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		playSound(S_SORT);
		layerCards->actionSortCardPhom();
		// 		if (layerCards->getAllowSortCard() == false) {
		// 			log("khong duoc sort");
		// 			return;
		// 		};
		// 		log("duoc sort!");
		// 		int size = arrButtonCanPush.size();
		// 		if (size > 0) {
		// 			for (int i = size - 1; i >= 0; i--)
		// 			{
		// 				arrButtonCanPush.at(i)->setVisible(false);
		// 				arrButtonCanPush.at(i)->setTouchEnabled(false);
		// 				layerButtons->removeChild(arrButtonCanPush.at(i), true);
		// 			}
		// 			arrButtonCanPush.clear();
		// 		}
		// 		//
		// 		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		// 		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		// 		if (lastRoom == NULL)
		// 			return;
		// 		boost::shared_ptr<IRequest> request(new ExtensionRequest(convertResponseToString(EXT_EVENT_REQ_ORDER_CARDS), parameter, lastRoom));
		// 		GameServer::getSingleton().Send(request);
		// 		layerCards->setAllowSortCard(true);
	}
}

void LayerPlayGamePhom::actionHitCards(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		int size = arrButtonCanPush.size();
		if (size > 0) {
			for (int i = size - 1; i >= 0; i--)
			{
				arrButtonCanPush.at(i)->setVisible(false);
				arrButtonCanPush.at(i)->setTouchEnabled(false);
				layerButtons->removeChild(arrButtonCanPush.at(i), true);
			}
			arrButtonCanPush.clear();
		}
		//
		vector<int> arrID = layerCards->getIdCardByClicked();
		unsigned int length = arrID.size();

		if (length != 1) {
			layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(243));
			return;
		}
		else {
			if (layerCards->getCardByID(arrID[0])->getEat()) return; //neu la quan bai da an thi khong danh duoc
			layerCards->actionHitCard(kUserMe, arrID[0]);
			lastHitCard = arrID[0];
			////thu animation truoc, hien tai gui len sever sau:
			getButtonByTag(kTagButtonHit)->setVisible(false);
			layerAvatars->stopAllTimer();
			log("GUI BAN TIN DANH BAI");

			boost::shared_ptr<ISFSObject> params(new SFSObject());
			unsigned char cardidx = (unsigned char)arrID.at(0);
			params->PutByte(EXT_FIELD_CARD_INDEX, cardidx);
			boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom == NULL)
				return;
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_DISCARD, params, lastRoom));
			GameServer::getSingleton().Send(request);
		}
	}
}

void LayerPlayGamePhom::actionGiveCards(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		showBocNoc(false);
		layerCards->hideArrowCardCanEat();
		getButtonByTag(kTagButtonEat)->setVisible(false);
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_DRAW_CARD, params, lastRoom));
		GameServer::getSingleton().Send(request);
	}
}

void LayerPlayGamePhom::actionEatCards(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		// 		layerCards->actionEatCard(kUserMe, kUserTop, 10);
		// 		layerCards->actionEatCard(kUserMe, kUserTop, 11);
		// 		layerCards->actionEatCard(kUserMe, kUserTop, 12);
		// 		return;
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		getButtonByTag(kTagButtonEat)->setVisible(false);
		params->PutByte(EXT_FIELD_CARD_INDEX, layerCards->getIdCardCanEat());
		layerCards->hideArrowCardCanEat();
		showBocNoc(false);
		///hien tai thuc hien an bai truoc gui len sever sau
		layerAvatars->stopAllTimer();
		int cardID = layerCards->getIdCardCanEat();
		//layerCards->actionEatCard(layerCards->getCardByID(cardID)->getFromUser(), kUserMe, cardID);
		////
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_TAKE_CARD, params, lastRoom));

		GameServer::getSingleton().Send(request);
	}
}

void LayerPlayGamePhom::actionHaPhom(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED){
		int size = arrButtonCanPush.size();
		if (size > 0) {
			for (int i = size - 1; i >= 0; i--)
			{
				arrButtonCanPush.at(i)->setVisible(false);
				layerButtons->removeChild(arrButtonCanPush.at(i), true);
			}
			arrButtonCanPush.clear();
		}

		// Phòng trường hợp "xấu" nhất, ta cứ gửi các quân bài đang được chọn
		// rồi sau đó mới gừi các quân được xếp bởi sv
		vector<int> arrID = layerCards->getIdCardByClicked();
		string listID;
		int length = arrID.size();
		if (length < 3)
		{
			return;
		}
		for (int i = 0; i < length; i++) {
			listID += boost::to_string(arrID[i]);
			if (i != length - 1) listID += ":";
		}
		if (listCardHandOff.size() > 0){
			///neu list card hand off da co, thi phai check voi quan bai no chon, uu tien quan bai ng dung chon
			vector<string> arrIDPhomHaSever = mUtils::splitString(listCardHandOff, ';');
			vector<string> arrIDPhomClient = mUtils::splitString(listID, ';');
			if (arrIDPhomHaSever.size() == arrIDPhomClient.size())//thieu doan nay nen crash the moi nho
			{
				if ((equal(arrIDPhomHaSever.begin(), arrIDPhomHaSever.end(), arrIDPhomClient.begin()) == true))
					//neu trung nhau thi thuc hien luon con khong thi gui len sever do chua check duoc logic o client
				{
					log("ID HA PHOM TRUNG NHAU");
					getButtonByTag(kTagButtonHaPhom)->setVisible(false);
					log("LISTCARD_HANDOFF = %s", listCardHandOff.c_str());
					layerCards->actionHaPhomClient(listCardHandOff);
					this->isHaPhom = true;
					listCardHandOff = "";
				}
			}
		}

		log("callbackHaPhom listID= %s", listID.c_str());
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString(EXT_FIELD_LIST_CARD, listID);
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_HANDOFF_PHOM, params, lastRoom));
		GameServer::getSingleton().Send(request);
	}
}

void LayerPlayGamePhom::callbackHaPhom(float dt) {
	vector<string> arrListID = layerCards->getID_HaPhom();

	for (int i = 0; i < arrListID.size(); i++) {
		listID_HaPhom = arrListID[i];

		//this->scheduleOnce(schedule_selector(LayerPlayGamePhom::callbackHaPhom_stepByStep), 0.3 * i);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGamePhom::callbackHaPhom_stepByStep), this, 0.3 * i, 1, 0.3 * i, false);
	}
}

void LayerPlayGamePhom::callbackHaPhom_stepByStep(float dt){
	if (listID_HaPhom.length() < 2) return;

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString(EXT_FIELD_LIST_CARD, listID_HaPhom);

	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_HANDOFF_PHOM, params, lastRoom));

	GameServer::getSingleton().Send(request);

	listID_HaPhom = "";
}

void LayerPlayGamePhom::actionPush(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		//
		int size = arrButtonCanPush.size();
		if (size > 0) {
			for (int i = size - 1; i >= 0; i--)
			{
				arrButtonCanPush.at(i)->setVisible(false);
				arrButtonCanPush.at(i)->setTouchEnabled(false);
				layerButtons->removeChild(arrButtonCanPush.at(i), true);
			}
			arrButtonCanPush.clear();
		}
		//

		vector<int> arrID = layerCards->getIdCardByClicked();
		unsigned int length = arrID.size();
		//cai nay danh cho gui kieu moi
		if (length < 1) {
			///neu khong chon quan nao van gui len cho server de gui tat nhung con co the
			layerNumbers->showChatByPos(-1, dataManager.GetSysString(244));
			return;
		}
		{
			cardid_push = arrID.at(0);
			string listID;
			bool isEatCard = false;//danh cho viec check neu co quan da an
			for (int i = 0; i < arrID.size(); ++i)
			{
				if (layerCards->getCardByID(arrID.at(i))->getEat()) //neu la quan bai da an thi khong danh duoc
				{
					isEatCard = true;
					break;
				}
				if (i != 0)
					listID += PHOM_CHAR_SPLIT_2;
				listID += StringUtils::format("%d", arrID.at(i));
			}
			if (isEatCard)
			{
				layerNumbers->showChatByPos(-1, dataManager.GetSysString(264));
				return;
			}

			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutUtfString(EXT_FIELD_LIST_CARD, listID);
			boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom == NULL)
				return;
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_PUSH_CARD, params, lastRoom));
			log("Gui listcard = %s", listID.c_str());
			GameServer::getSingleton().Send(request);
			return;
		}

		// 		if (length != 1) {
		// 			///neu khong chon quan nao van gui len cho server de gui tat nhung con co the
		// 			layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(244));
		// 			// 			boost::shared_ptr<ISFSObject> params(new SFSObject());
		// 			// 			boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		// 			// 			if (lastRoom == NULL)
		// 			// 				return;
		// 			// 			boost::shared_ptr<IRequest> request(new ExtensionRequest(convertResponseToString(EXT_EVENT_REQ_PUSH_CARD), params, lastRoom));
		// 			// 			GameServer::getSingleton().Send(request);
		// 			return;
		// 		}
		// 		if (length == 1) {
		// 			this->cardid_push = arrID[0];
		// 			boost::shared_ptr<ISFSObject> params(new SFSObject());
		// 			params->PutByte("cardidx", arrID[0]);
		// 			boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		// 			if (lastRoom == NULL)
		// 				return;
		// 			boost::shared_ptr<IRequest> request(new ExtensionRequest(convertResponseToString(EXT_EVENT_REQ_PUSH_CARD), params, lastRoom));
		// 			log("Gui quan co id = %d", arrID[0]);
		// 			GameServer::getSingleton().Send(request);
		// 		}
	}
}

void LayerPlayGamePhom::actionUUU(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_U, params, lastRoom));

		GameServer::getSingleton().Send(request);
	}
}

void LayerPlayGamePhom::actionSitting(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		getButtonByTag(kTagButtonSitting)->setVisible(false);
		isSpector = layerAvatars->isSpectator();
		isStartedGame = layerAvatars->isStartedGame();
		if (isSpector) {
			if (isStartedGame) {
				if (!isRegistSittingDown){
					layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(246));
					isRegistSittingDown = true;
				}
			}
			else {
				// join game
				isRegistSittingDown = false;
			}
			sendRequestJoinGame();
		}
	}
}

void LayerPlayGamePhom::actionStandUp(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		getButtonByTag(kTagButtonStandUp)->setVisible(false);
		boost::shared_ptr<IRequest> request(new PlayerToSpectatorRequest());
		GameServer::getSingleton().Send(request);
		isRegistStandUp = false;
		sendRequestLeaveGame();
	}
}

void LayerPlayGamePhom::actionPushMulti(Ref *pSender, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		log("actionPushMulti");

		// Clicked vào 1 bộ phỏm để gửi tới đó
		Point positionBt = ((Button*)pSender)->getPosition();
		positionBt.setPoint(positionBt.x - 60, positionBt.y - 30);
		int size = arrButtonCanPush.size();
		if (size == 0) return;
		for (int i = size - 1; i >= 0; i--)
		{
			arrButtonCanPush.at(i)->setVisible(false);
			arrButtonCanPush.at(i)->setTouchEnabled(false);
			layerButtons->removeChild(arrButtonCanPush.at(i), true);
		}
		arrButtonCanPush.clear();
		//
		vector<string> arrPlayerPhom = mUtils::splitString(this->dstphsString, '/');
		for (int i = 0; i < arrPlayerPhom.size(); i++)
		{
			vector<string> arr = mUtils::splitString(arrPlayerPhom.at(i), PHOM_CHAR_SPLIT_1);
			if (arr.size() < 2) return;
			int idx1 = std::atoi(arr.at(0).c_str());
			Card *card = layerCards->getCardByID(idx1);
			log("card->getPositionX()= %.1f", card->getPositionX());
			log("card->getPositionY()= %.1f", card->getPositionY());
			//if (card->getPosition().equals(positionBt) == 0) {
			if (card->getPositionX() == positionBt.x && card->getPositionY() == positionBt.y){
				boost::shared_ptr<ISFSObject> params(new SFSObject());
				///for new push
				params->PutUtfString(EXT_FIELD_LIST_CARD, StringUtils::format("%d", cardid_push));
				//params->PutByte("cardidx", this->cardid_push);
				string phomidString = arr.at(arr.size() - 2);
				params->PutInt(EXT_FIELD_PHOM_ID, atol(phomidString.c_str()));
				params->PutUtfString(EXT_FIELD_USERNAME, arr.at(arr.size() - 1));

				log("phomid=%d, i=%d", std::atoi(phomidString.c_str()), i);

				boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
				if (lastRoom == NULL)
					return;
				boost::shared_ptr<IRequest> request
					(new ExtensionRequest(EXT_EVENT_REQ_PUSH_CARD, params, lastRoom));

				GameServer::getSingleton().Send(request);

				break;
			}
		}

	}
}

void LayerPlayGamePhom::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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

void LayerPlayGamePhom::OnExtensionResponseProcess() {
	this->param = mThreadQueueMsg->deQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
	//boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT);
	if (cmd == NULL) return;
	// 	if (rscode != NULL) {
	// 		int codeInt = (int)*rscode;
	// 		if (codeInt == PHOM_RSCODE_ERROR_GAME_FULL) {
	// 			layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(198));
	// 		}
	// 	}
	if (0 == cmd->compare(EXT_HEATBEAT)){
		return;
	}
	else if (0 == cmd->compare(EXT_SRVNTF_PUBLIC_MESSAGE))
	{
		this->event_EXT_SRVNTF_PUBLIC_MESSAGE();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_PRIVATE_MESSAGE))
	{
		this->event_EXT_SRVNTF_PRIVATE_MESSAGE();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_GAME_MASTER_INFO))
	{
		this->event_EXT_SRVNTF_GAME_MASTER_INFO();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_READY))
	{
		this->event_EXT_SRVNTF_READY();
	}
	else if (0 == cmd->compare(EXT_EVENT_READY_RES))
	{
		this->event_EXT_EVENT_READY_RES();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_PLAYER_LIST))
	{
		this->event_EXT_SRVNTF_PLAYER_LIST();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_RECOMMENDED_CARDSETS))
	{
		this->event_EXT_SRVNTF_RECOMMENDED_CARDSETS();
	}
	else if (0 == cmd->compare(EXT_EVENT_START))
	{
		this->event_EXT_EVENT_START();
	}
	else if (0 == cmd->compare(EXT_EVENT_END))
	{
		this->event_EXT_EVENT_END();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT))
	{
		this->event_EXT_EVENT_GAME_RESULT();
	}
	else if (0 == cmd->compare(EXT_EVENT_LISTCARD_NTF))
	{
		this->event_EXT_EVENT_LISTCARD_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_COIN_NTF))
	{
		this->event_EXT_EVENT_COIN_NTF();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_CURRENT_PLAYING_USER))
	{
		this->event_EXT_SRVNTF_CURRENT_PLAYING_USER();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_END_PLAYING_GAME))
	{
		this->event_EXT_SRVNTF_END_PLAYING_GAME();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_GAME_INFO))
	{
		this->event_EXT_SRVNTF_GAME_INFO();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_USER_READY))
	{
		this->event_EXT_SRVNTF_USER_READY();
	}
	else if (0 == cmd->compare(EXT_EVENT_UNREADY_RES))
	{
		this->event_EXT_SRVNTF_USER_UNREADY();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_CARD_ORIGINATION))// bản tin đánh, ăn .... bài public
	{
		this->event_EXT_SRVNTF_CARD_ORIGINATION();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_DRAW_CARD))
	{
		//List event response message
		this->event_EXT_EVENT_RES_DRAW_CARD();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_TAKE_CARD))
	{
		this->event_EXT_EVENT_RES_TAKE_CARD();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_DISCARD))
	{
		this->event_EXT_EVENT_RES_DISCARD();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_HANDOFF_PHOM))
	{
		this->event_EXT_EVENT_RES_HANDOFF_PHOM();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_END_PLAYING))
	{
		this->event_EXT_EVENT_RES_END_PLAYING();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_SET_BET))
	{
		this->event_EXT_EVENT_RES_SET_BET();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_U))
	{
		this->event_EXT_EVENT_RES_U();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_PUSH_CARD))
	{
		this->event_EXT_EVENT_RES_PUSH_CARD();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_CAN_TAKE))
	{
		this->event_EXT_SRVNTF_CAN_TAKE();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_PLAYER_REPLAY))
	{
		this->event_EXT_SRVNTF_PLAYER_REPLAY();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_U_9))
	{
		this->event_EXT_SRVNTF_U_9();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_U))
	{
		this->event_EXT_SRVNTF_U();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_PLAYER_KICKOUT))
	{
		this->event_EXT_SRVNTF_PLAYER_KICKOUT();
	}
	else if (0 == cmd->compare(EXT_EVENT_UNREADY_NTF))
	{
		layerTimer->startTimer();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_LEAVE_GAME))
	{
		this->event_EXT_EVENT_RES_LEAVE_GAME();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_JOIN_GAME))
	{
		this->event_EXT_EVENT_RES_JOIN_GAME();
	}
	else if (0 == cmd->compare(EXT_EVENT_CHICKEN_UPDATE_NTF))
	{
		this->event_EXT_EVENT_CHICKEN_UPDATE_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_NTF_TIMER))
	{
		this->event_EXT_EVENT_NTF_TIMER();
	}
	else if (0 == cmd->compare(EXT_EVENT_RES_UPDATE_GAME_CONFIG))
	{
		this->event_EXT_EVENT_RES_UPDATE_GAME_CONFIG();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_NOC_COUNT))
	{
		this->event_EXT_SRVNTF_NOC_COUNT();
	}
	else if (0 == cmd->compare(EXT_EVENT_REGISTER_QUEUE_NTF))
	{
		this->event_EXT_EVENT_REGISTER_QUEUE_NTF();
	}
	else if (0 == cmd->compare(EXT_SRVNTF_RECOMMENDED_PUSH_CARDS))
	{
		this->event_EXT_SRVNTF_RECOMMENDED_PUSH_CARDS();
	}
	else
	{
		log("LOG_ERROR %s", cmd->c_str());
	}
}

void LayerPlayGamePhom::sendRequestJoinGameInterval(float dt) {
	log("sendRequestJoinGame %d", countRequestJoin);
	if (countRequestJoin >= 3){
		countRequestJoin = 0;
		Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(LayerPlayGamePhom::sendRequestJoinGameInterval),this);
		return;
	}
	countRequestJoin++;
	sendRequestJoinGame();
}

void LayerPlayGamePhom::sendRequestJoinGame() {
	// join game
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_JOIN_GAME, params, lastRoom));
	GameServer::getSingleton().Send(request);
	//Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayGamePhom::sendRequestJoinGameInterval));
}

void LayerPlayGamePhom::sendRequestLeaveGame() {
	// leave game
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_LEAVE_GAME, params, lastRoom));
	GameServer::getSingleton().Send(request);
}

vector<string> LayerPlayGamePhom::split(string &S, const char &str) {
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

void LayerPlayGamePhom::event_EXT_SRVNTF_PUBLIC_MESSAGE(){

}

void LayerPlayGamePhom::event_EXT_SRVNTF_PRIVATE_MESSAGE() {

}

void LayerPlayGamePhom::event_EXT_SRVNTF_GAME_MASTER_INFO() {
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT);

	if (rscode != NULL) {
		int codeInt = (int)*rscode;
		if (codeInt != PHOM_RSCODE_OK) {
			//layerNumbers->showChatByPos(-1, "Đợi chờ là hạnh phúc :P");
		}
	}

}

void LayerPlayGamePhom::event_EXT_SRVNTF_READY() {
	log("event_EXT_EVENT_READY_RES");
}

void LayerPlayGamePhom::event_EXT_EVENT_READY_RES() {

}

void LayerPlayGamePhom::event_EXT_SRVNTF_PLAYER_LIST() {

	boost::shared_ptr<string> list = param->GetUtfString(EXT_FIELD_LIST_USER);
	std::string _lstUser = *list;
	//isSpector = this->isSpectator();

	if (list == NULL) return;
	lstUser = list;
	if (layerAvatars->getUserByPos(kUserBot)->isVisible()){
		layerCards->resetGame();
	}

	//neu ban full thi khong hien thi button invite
	int _size = mUtils::splitString(_lstUser, ';').size();
	if (_size >= 4){
		this->layerButtons->processDisplayButtonInvite(false);
	}
	else{
		if (this->vipRoom == 1){
			this->layerButtons->processDisplayButtonInvite(true);
		}
	}

	log("EXT_SRVNTF_PLAYER_LIST = %s", list->c_str());
	layerAvatars->setListUserByPhom(list->c_str());
	bool isChuphong = strcmp(find_ChuPhong(lstUser->c_str()).c_str(), myName.c_str()) == 0 ? true : false;
	//levelGame++;
	// 	if (levelGame == 0) {
	// 		// ban dau thi hien luon
	// 		// thông tin tiền hiện tại của Users
	// 		for (int i = 0; i < arrName.size(); i++) {
	// 			int pos = layerAvatars->getPosByName(arrName[i]);
	// 			if (pos > -1){
	// 				layerAvatars->setMoney(pos, arrMoneyDouble[i]);
	// 			}
	// 		}
	// 
	// 		arrName.clear();
	// 		arrMoney.clear();
	// 		arrMoneyDouble.clear();
	// 	}

	// Check: Vào lại bàn đang chơi dở (isStartedGame=true, isSpector=false)
	isStartedGame = layerAvatars->isStartedGame();
	isSpector = layerAvatars->isSpectator();
	if (isSpector)
	{

		layerButtons->SetIsGuess(true);
		if (isStartedGame == true)
		{
			layerTimer->stopTimer();
			getButtonByTag(kTagButtonGive)->setVisible(true);
			getButtonByTag(kTagButtonReady)->setVisible(false);
			if (layerAvatars->getUserCount() >= 4)
				layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(253));
		}
		else{
			getButtonByTag(kTagButtonReady)->setVisible(false);
			if (layerAvatars->getUserCount() == 1)
				layerTimer->stopTimer();
		}
	}
	else{
		Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(LayerPlayGamePhom::sendRequestJoinGameInterval),this);
		layerTimer->stopTimer();
		layerButtons->SetIsGuess(false);
		if (this->isStartedGame)
		{
			isResume = true;
			getButtonByTag(kTagButtonSort)->setVisible(true);
			getButtonByTag(kTagButtonGive)->setVisible(true);
			showBocNoc(false);
		}
		else{
			isResume = false;
			if (layerAvatars->getUserCount() > 1){
				if (isChuphong){
					getButtonByTag(kTagButtonReady)->setVisible(true);
				}
				layerTimer->startTimer();
			}
			else if (layerAvatars->getUserCount() == 1)
			{
				layerAvatars->setUnReadyAllUser();
				layerTimer->stopTimer();
				resetGame();
				hideAllButton();
				Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(LayerPlayGamePhom::showButtonReady),this);
			}
		}
	}

	// 	string str_list = "466:bot_134061111111111:1:0;465:bot_1340711:0:0;466:bot_134061111111:1:0;465:bot_13407111111:0:0";
	// 	layerAvatars->setListUserByPhom(str_list);
	// 	layerAvatars->setResult(kUserMe, 6, kGamePhom, false, 0);

}

void LayerPlayGamePhom::event_EXT_SRVNTF_RECOMMENDED_CARDSETS() {
	// Danh sách gợi ý các quân bài hạ phỏm dành riêng cho mình
	// chỉ nhảy vào đây khi trên tay có phỏm
	// ẩn các nút cho tới khi các quân bài được xếp gợi ý
	getButtonByTag(kTagButtonSort)->setVisible(false);
	getButtonByTag(kTagButtonHit)->setVisible(false);
	//    btnHaPhom->setVisible(true);
	//    btnPush->setVisible(true);

	boost::shared_ptr<string> listcard = param->GetUtfString(EXT_FIELD_RECOMMEND_CARDSET);
	if (listcard == NULL) return;
	log("EXT_SRVNTF_RECOMMENDED_CARDSETS listcard= %s", listcard->c_str());
	listCardHandOff = *listcard;

	//layerNumbers->showChatByPos(-1, "Hãy hạ phỏm trước khi đánh!");
	layerCards->event_EXT_SRVNTF_RECOMMENDED_CARDSETS(listcard->c_str());
}

void LayerPlayGamePhom::event_EXT_EVENT_START() {
	isFirstReady = false;
	log("EXT_EVENT_START");
	playSound(S_START_GAME);
	this->resetGame();
	getButtonByTag(kTagButtonGive)->setVisible(true);
	isStartedGame = true;
	layerGa->showToast(false);
	// 	string mes = "Bắt đầu";
	// 	if (isSpector) {
	// 		mes += ". Bạn là khách, hãy tham gia sau";
	// 		getButtonByTag(kTagButtonSitting)->setVisible(true);
	// 	}
	// 
	// 	layerNumbers->showChatByPos(-1, mes);
}

void LayerPlayGamePhom::event_EXT_EVENT_END() {
	isFirstReady = false;
	playSound(S_END_GAME);
	isSpector = this->isSpectator();
	isStartedGame = this->layerAvatars->isStartedGame();
	//this->scheduleOnce(CC_SCHEDULE_SELECTOR(LayerPlayGamePhom::resetCard), TIME_RESET_GAME);
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGamePhom::resetCard), this, TIME_RESET_GAME, 1, TIME_RESET_GAME, false);
	layerCards->setGameStarted(false);
	this->hideAllButton();
	layerAvatars->stopAllTimer();
	if (!isSpector) {
		if (layerAvatars->getUserCount() > 1){
			//if (strcmp(find_ChuPhong(lstUser->c_str()).c_str(), myName.c_str()) == 0)
			//this->scheduleOnce(CC_SCHEDULE_SELECTOR(LayerPlayGamePhom::showButtonReady), endGameTimeout);
                Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayGamePhom::showButtonReady), this, endGameTimeout, 1, endGameTimeout, false);
		}
	}
	if (lstUser == NULL || lstUser->find(PHOM_CHAR_SPLIT_1) == std::string::npos)return;
	layerTimer->startTimer();
	layerGa->showToast(true);
}

void LayerPlayGamePhom::event_EXT_EVENT_GAME_RESULT() {
	boost::shared_ptr<string> resultGame = param->GetUtfString(EXT_FIELD_RESULT_GAME);

	//0:28:4:1000:1401329992510:thanhhv4;1:36:5:-1000:1401330005039:thanhhv2;


	if (resultGame == NULL) {
		return;
	}
	log("EXT_EVENT_GAME_RESULT rg= %s.", resultGame->c_str());
	isEndGame = true;
	hideAllButton();
	layerAvatars->stopAllTimer();
	string rg = resultGame->c_str();

	// tam thoi chi hien thi diem ở ô tiền
	vector<string> listString = split(rg, PHOM_CHAR_SPLIT_1);
	for (int i = 0; i < listString.size(); i++) {
		if (listString[i].length() < 5) {
			break;
		}
		vector<string> arrInfo = split(listString[i], PHOM_CHAR_SPLIT_2);
		int size = arrInfo.size();
		if (size >= 6){
			int score = std::atoi(arrInfo[1].c_str());
			int rescode = std::atoi(arrInfo[2].c_str());
			string name = arrInfo[5];
			int pos = this->layerAvatars->getPosByName(name);
			string result = "";
			ostringstream oss;
			oss << score;
			string scoreString = oss.str() + " "+dataManager.GetSysString(556)+" - ";
			if (score < 0) scoreString = "";
			// 			switch (rescode) {
			// 			case 0:
			// 				scoreString = "Ù 0 điểm!";
			// 				result = "Ù 0 điểm!";
			// 				break;
			// 			case 1:
			// 				scoreString = "Ù tròn!";
			// 				result = "Ù tròn!";
			// 				break;
			// 			case 2:
			// 				scoreString = "Ù 9 cây!";
			// 				result = "Ù 9 cây!";
			// 				break;
			// 			case 3:
			// 				scoreString = "Ù khan!";
			// 				result = "Ù khan!";
			// 				break;
			// 			case 4:
			// 				scoreString += "Nhất";
			// 				result = "Nhất";
			// 
			// 				break;
			// 			case 5:
			// 				scoreString += "Nhì";
			// 				result = "Nhì";
			// 
			// 				break;
			// 			case 6:
			// 				scoreString += "Ba";
			// 				result = "Ba";
			// 				break;
			// 			case 7:
			// 				scoreString += "Bét";
			// 				result = "Bét";
			// 				break;
			// 			case 8:
			// 				scoreString += "Cháy!";
			// 				result = "Cháy";
			// 				break;
			// 			case 9:
			// 				scoreString = "Ù đền!";
			// 				result = "Ù đền";
			// 				break;
			// 			case 10:
			// 				scoreString = "Bỏ cuộc";
			// 				result = "Bỏ cuộc";
			// 				break;
			// 			case 11:
			// 				scoreString = "Ù láo";
			// 				result = "Ù láo";
			// 				break;
			// 			case 12:
			// 				scoreString = "Ù khan 10 cây";
			// 				result = "Ù khan 10 cây";
			// 				break;
			// 			case 13:
			// 				scoreString = "Ù đền 10 cây";
			// 				result = "Ù đền 10 cây";
			// 				break;
			// 			case 20: //chua co defined
			// 				break;
			// 			default:
			// 				scoreString += "Bét";
			// 				result = "Bét";
			// 				break;
			// 			}
			//this->layerAvatars->setMoney(pos, scoreString);
			layerAvatars->setResult(pos, rescode, kGamePhom, false, 0);
			if (strcmp(name.c_str(), myName.c_str()) == 0)
			{
				playSound(S_WIN);
			}
		}
	}
	layerCards->showCardOnHandAll();

}

void LayerPlayGamePhom::event_EXT_EVENT_LISTCARD_NTF() {
	boost::shared_ptr<string> fname = param->GetUtfString(EXT_FIELD_USERNAME);
	boost::shared_ptr<string> listString = param->GetUtfString(EXT_FIELD_LIST_CARD);
	if (fname == NULL || listString == NULL) return;
	log("EXT_EVENT_LISTCARD_NTF name= %s, lc= %s", fname->c_str(), listString->c_str());

	if (strcmp(fname->c_str(), myName.c_str()) == 0 && this->isHaPhom){
		log("it's mine, don't action");
		return;/// neu la minh thi khong ha phom nua
	}
	layerCards->setResume(isResume);
	layerCards->eventListcardNTF(layerAvatars->getPosByName(fname->c_str()), listString->c_str());
	if (this->currentPlaying == myName) {
		// khi tới lượt mình thì có 2 trường hợp:
		//      1. Đánh (lượt đầu và đc đánh trước)
		//      2. Không đánh (bốc hoặc ăn rồi mới đánh)        
		if (layerCards->getCountCardOnHand() == 10) {
			getButtonByTag(kTagButtonHit)->setVisible(true);
		}
		else if (this->actionLast != DANH_BAI && this->actionLast != BOC_BAI &&
			this->actionLast != AN_BAI && !getButtonByTag(kTagButtonPush)->isVisible()){
			getButtonByTag(kTagButtonGive)->setVisible(true);
			showBocNoc(true);
		}
	}

}

void LayerPlayGamePhom::event_EXT_EVENT_COIN_NTF() {
	log("EXT_EVENT_COIN_NTF");
	boost::shared_ptr<string> fname = param->GetUtfString(EXT_FIELD_USERNAME);
	boost::shared_ptr<long> coin = param->GetInt(EXT_FIELD_COIN_CHANGE);
	boost::shared_ptr<long> coinchr = param->GetInt(EXT_FIELD_COIN_CHANGE_REASON);

	if (fname == NULL || coin == NULL) {
		return;
	}

	if (coinchr != NULL)
	{
		log("coinchr= %ld", *coinchr);
		string ressonString = "";
		switch (*coinchr) {
			// 		case 0:
			// 			ressonString = "Bị ăn";
			// 			break;
			// 		case 1:
			// 			ressonString = "Được ăn";
			// 			break;
			// 		case 2:
			// 			//ressonString = "Kết thúc";
			// 			break;
		case 3:
			if (*coin < 0)
				ressonString = "Ăn láo";
			break;
		case 4:
			ressonString = "Bị đền làng";
			break;
		case 7:
			ressonString = "Bị ăn chốt";
			break;
		case 8:
			ressonString = "Được ăn chốt";
			break;
		case 9:// tru tien ghop ga
			ressonString = dataManager.GetSysString(479);
			break;
			// 		case 10:// cong tien khi an ga
			// 			//ressonString = "Ăn gà";
			// 			break;
		}
		if (ressonString.length() > 0) {
			//			layerNumbers->showChatByPos(layerAvatars->getPosByName(fname->c_str()), ressonString);
			if (*coinchr == 9)
			{
				Avatar52La* avatar = layerAvatars->getUserByPos(layerAvatars->getPosByName(fname->c_str()));
				if (avatar)
				{
					avatar->showChat(ressonString);
				}
			}
			//layerNumbers->showChatByPos(layerAvatars->getPosByName(fname->c_str()), ressonString);
			else
				layerAvatars->setResult(layerAvatars->getPosByName(fname->c_str()), *coinchr, kGamePhom, true, *coin);
		}
		if (*coinchr == 10 && isNuoiGa) // an ga, thi hien thi tien bay
		{
			auto userAva = layerAvatars->getUserByPos(layerAvatars->getPosByName(fname->c_str()));
			if (userAva != NULL)
				layerGa->runActionAnGa(userAva->getPosition(), 1);
		}
		// 		if (ressonString.length() > 0) {
		// 			layerNumbers->showChatByPos(layerAvatars->getPosByName(fname->c_str()), ressonString);
		// 		}
	}
	//playSound(S_WIN);
	// 	if (isEndGame && fname->c_str() == myName){
	// 		if (*coin > 0){
	// 			int numrd = rand() % 4 + 1;
	// 			string path = StringUtils::format("thang_roi_%d.mp3", numrd).c_str();
	// 			playSound(path);
	// 		}
	// 		else if (*coin < 0){
	// 			int numrd = rand() % 2 + 1;
	// 			string path = StringUtils::format("thua_roi_%d.mp3", numrd).c_str();
	// 			playSound(path);
	// 		}
	// 	}

	auto avatar = layerAvatars->getUserByPos(layerAvatars->getPosByName(fname->c_str()));
	if (avatar)
		avatar->showNumber(StringUtils::toString((int)*coin));
	//layerNumbers->showNumberByPos(layerAvatars->getPosByName(fname->c_str()), StringUtils::toString((int)*coin));
}

void LayerPlayGamePhom::event_EXT_SRVNTF_CURRENT_PLAYING_USER() {
	boost::shared_ptr<string> fname = param->GetUtfString(EXT_FIELD_USERNAME);
	if (fname == NULL) return;
	log("EXT_SRVNTF_CURRENT_PLAYING_USER name= %s", fname->c_str());

	//layerAvatars->runTimerWithTime(layerAvatars->getPosByName(fname->c_str()), discardTimeout);

	this->currentPlaying = fname->c_str();

	//
	if (this->currentPlaying == myName) {
		// khi tới lượt mình thì có 2 trường hợp:
		//      1. Đánh (lượt đầu và đc đánh trước)
		//      2. Không đánh (bốc hoặc ăn rồi mới đánh)
		log("getCountCardOnHand = %d", layerCards->getCountCardOnHand());

		if (layerCards->getCountCardOnHand() == 10) {
			getButtonByTag(kTagButtonHit)->setVisible(true);
		}
		else {
			showBocNoc(true);
			getButtonByTag(kTagButtonGive)->setVisible(true);
		}
	}
	else {
		getButtonByTag(kTagButtonU)->setVisible(false);
	}
}

void LayerPlayGamePhom::event_EXT_SRVNTF_END_PLAYING_GAME() {
	log("EXT_SRVNTF_END_PLAYING_GAME---------");
	if (isU9){
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_U, params, lastRoom));

		GameServer::getSingleton().Send(request);
		//return;
	}
	// 	if (isFirstHandOff)//neu la nguoi ha dau thi khong hien nut gui
	// 		getButtonByTag(kTagButtonPush)->setVisible(false);
	// 	else
	if (!listCardHandOff.empty())
		getButtonByTag(kTagButtonHaPhom)->setVisible(true);
	getButtonByTag(kTagButtonSort)->setVisible(true);
	getButtonByTag(kTagButtonHit)->setVisible(true);
	getButtonByTag(kTagButtonPush)->setVisible(true);
	//getButtonByTag(kTagButtonGive)->setVisible(false);
	showBocNoc(false);
}

void LayerPlayGamePhom::event_EXT_SRVNTF_GAME_INFO() {
	boost::shared_ptr<double> tienga = param->GetDouble(EXT_FIELD_CHICKEN_VALUE);
	boost::shared_ptr<double> lv_ga = param->GetDouble(EXT_FIELD_CHICKEN_LEVEL);
	boost::shared_ptr<string> listUserGa = param->GetUtfString(EXT_FIELD_LIST_USER_CHICKEN);
	if (tienga != NULL && lv_ga != NULL){
		int chicken_value = (int)*tienga;
		int chicken_lv = (int)*lv_ga;
		if (listUserGa != NULL){
			string lstGa = listUserGa->c_str();
			layerGa->setListUser(lstGa, PHOM_CHAR_SPLIT_1, PHOM_CHAR_SPLIT_2);
		}

		layerGa->setChickenLv(chicken_lv);
		layerGa->setChickenMoney(chicken_value);
	}
	//layerGa->setChickenMoney(10000);
}

void LayerPlayGamePhom::event_EXT_SRVNTF_USER_READY() {

	boost::shared_ptr<string> fname = param->GetUtfString(EXT_FIELD_USERNAME);

	if (fname != NULL) {
		log("event_EXT_SRVNTF_USER_READY %s", fname->c_str());

		layerAvatars->setReady(layerAvatars->getPosByName(fname->c_str()), true);
		if (fname->c_str() == this->myName) {
			myReady = true;
			getButtonByTag(kTagButtonReady)->setVisible(false);
			layerTimer->stopTimer();
			this->actionLast = SAN_SANG;
		}
	}
}

void LayerPlayGamePhom::event_EXT_SRVNTF_CARD_ORIGINATION() {
	boost::shared_ptr<unsigned char> fuid = param->GetByte(EXT_FIELD_FROM_USERID);
	boost::shared_ptr<unsigned char> tuid = param->GetByte(EXT_FIELD_TO_USERID);
	boost::shared_ptr<string> fname = param->GetUtfString(EXT_FIELD_FROM_USERNAME);
	boost::shared_ptr<string> tname = param->GetUtfString(EXT_FIELD_TO_USERNAME);
	boost::shared_ptr<unsigned char> localChar = param->GetByte(EXT_FIELD_CARD_ORIGINATION);
	boost::shared_ptr<unsigned char> cardid = param->GetByte(EXT_FIELD_CARD_INDEX);


	if (fname == NULL || tname == NULL || localChar == NULL || cardid == NULL) return;
	log("EXT_SRVNTF_CARD_ORIGINATION fname = %s / tname = %s / cardid = %d", fname->c_str(), tname->c_str(), *cardid.get());
	switch (*localChar.get()) {
	case CARD_ORIGINATION_BY_TAKEN:///an bai
		//layerAvatars->stopAllTimer();
		//layerAvatars->runTimerWithTime(layerAvatars->getPosByName(tname->c_str()), discardTimeout);
		break;

	case CARD_ORIGINATION_BY_DRAWN:
		// bốc bài
		layerCards->actionGiveCard(layerAvatars->getPosByName(tname->c_str()), *(cardid.get()));

		//layerAvatars->stopAllTimer();
		//layerAvatars->runTimerWithTime(layerAvatars->getPosByName(tname->c_str()), turnTimeout);
		break;

	case  CARD_ORIGINATION_BY_TRANSFERED:
		// chuyển bài
		log("CARD_ORIGINATION_BY_TRANSFERED");
		layerCards->actionTransferedCard(layerAvatars->getPosByName(fname->c_str()), layerAvatars->getPosByName(tname->c_str()), *(cardid.get()));
		break;

	case  CARD_ORIGINATION_BY_DEALT:
		break;

	case  CARD_ORIGINATION_BY_HANDOFF:
		log("CARD_ORIGINATION_BY_HANDOFF %d. name: %s", *(tuid.get()), fname->c_str());
		// tuid = -1
		if (*(tuid.get()) == 255){
			// đánh bài
			int numrd = rand() % 4 + 1;
			string path = StringUtils::format("thach_an_%d.mp3", numrd).c_str();
			playSound(S_HIT_CARD);
			if (fname->c_str() == myName){
				this->actionLast = DANH_BAI;
				if (*(cardid.get()) == lastHitCard) //neu bai danh ra sever gui xuong = client thi khong can nua
				{
					log("trung id card khong thuc hien animation");
					lastHitCard = 0;
					return;
				}
				else if (*(cardid.get()) != lastHitCard && lastHitCard > 0){ //neu khong dung roll back
					log("Rollback lai quan bai");
					layerCards->actionHitCardRollBack(lastHitCard);
					layerCards->actionHitCard(layerAvatars->getPosByName(fname->c_str()), *(cardid.get()));
					layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(247));
					return;
				}
			}
			layerCards->actionHitCard(layerAvatars->getPosByName(fname->c_str()), *(cardid.get()));
		}
		else{
			// ăn bài
			playSound(S_EAT_CARD);
			//if (strcmp(tname->c_str(), myName.c_str()) == 0) return; /// neu la minh thi thoi khoi thuc hien tiep
			int numrd = rand() % 5 + 1;
			string path = StringUtils::format("an_bai_%d.mp3", numrd).c_str();

			layerCards->actionEatCard(layerAvatars->getPosByName(fname->c_str()), layerAvatars->getPosByName(tname->c_str()), *(cardid.get()));
			//this->actionLast = AN_BAI;
		}

		break;

	case  CARD_ORIGINATION_BY_UNKOWN:
		break;

	case  CARD_ORIGINATION_BY_PUSHED:
		// gửi bài
		log("CARD_ORIGINATION_BY_PUSHED ");
		this->isHaPhom = false;///reset lai co - tranh truong hop gui quan khong thuc hien animation
		layerCards->actionPushCard(layerAvatars->getPosByName(fname->c_str()), layerAvatars->getPosByName(tname->c_str()), *(cardid.get()));
		break;

	}
}

void LayerPlayGamePhom::event_EXT_EVENT_RES_DRAW_CARD() {
	boost::shared_ptr<unsigned char> localChar = param->GetByte(EXT_FIELD_RESULT);
	if (localChar == NULL) return;
	log("EXT_EVENT_RES_DRAW_CARD rscode = %d", *(localChar.get()));

	if (*localChar.get() == PHOM_RSCODE_OK) {
		// bốc bài thành công
		//neu u va da co 3 con tren ban thi gui require u luon
		if (isU9 && layerCards->getCountCardTaken() >= 3)
		{
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom == NULL)
				return;
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_U, params, lastRoom));

			GameServer::getSingleton().Send(request);
			return;
		}

		showBocNoc(false);
		layerCards->hideArrowCardCanEat();
		getButtonByTag(kTagButtonEat)->setVisible(false);
		getButtonByTag(kTagButtonHit)->setVisible(true);
		layerCards->setAllowSortCard(true);
		this->actionLast = BOC_BAI;
		layerCards->actionRecommendCard();
	}
}

void LayerPlayGamePhom::event_EXT_EVENT_RES_TAKE_CARD() {
	boost::shared_ptr<unsigned char> localChar = param->GetByte(EXT_FIELD_RESULT);
	if (localChar == NULL) return;
	log("EXT_EVENT_RES_TAKE_CARD rscode = %d", *(localChar.get()));

	if (*localChar.get() == PHOM_RSCODE_OK) {
		//an bai ok
		//neu u va da co 3 con tren ban thi gui require u luon
		if (isU9 && layerCards->getCountCardTaken() >= 3)
		{
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom == NULL)
				return;
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_U, params, lastRoom));

			GameServer::getSingleton().Send(request);
			return;
		}

		showBocNoc(false);
		layerCards->hideArrowCardCanEat();
		getButtonByTag(kTagButtonHit)->setVisible(true);
		this->actionLast = AN_BAI;
		layerCards->setAllowSortCard(true);
		layerCards->actionRecommendCard();
	}
}

void LayerPlayGamePhom::event_EXT_EVENT_RES_DISCARD() {
	boost::shared_ptr<unsigned char> localChar = param->GetByte(EXT_FIELD_RESULT);
	if (localChar == NULL) return;
	log("EXT_EVENT_RES_DISCARD rscode = %d", *(localChar.get()));

	if (*localChar.get() == PHOM_RSCODE_OK) {
		// danh bài thành công
		showBocNoc(false);
		layerCards->hideArrowCardCanEat();
		getButtonByTag(kTagButtonEat)->setVisible(false);
		getButtonByTag(kTagButtonHit)->setVisible(false);
		getButtonByTag(kTagButtonHaPhom)->setVisible(false);
		getButtonByTag(kTagButtonPush)->setVisible(false);
		getButtonByTag(kTagButtonU)->setVisible(false);
		layerCards->setAllowSortCard(true);
		this->actionLast = DANH_BAI;
	}
	else if (*localChar.get() == 14){
		log("khong lam gi ca");
	}
	else
	{
		//neu quan bai cuoi van con, chung to danh khong duoc, hien thong bao khac
		if (lastHitCard > 0){
			lastHitCard = 0;

		}
		else{
			layerNumbers->showChatByPos(-1, dataManager.GetSysString(239));
		}

	}
}

void LayerPlayGamePhom::event_EXT_EVENT_RES_HANDOFF_PHOM() {
	// mỗi lần hạ phỏm thành công là sắp xếp lại
	boost::shared_ptr<unsigned char> localChar = param->GetByte(EXT_FIELD_RESULT);
	if (localChar == NULL) return;
	log("EXT_EVENT_RES_HANDOFF_PHOM rscode = %d", *(localChar.get()));
	if (*localChar.get() == PHOM_RSCODE_OK) {
		this->actionLast = HA_PHOM;
		listCardHandOff = "";
		//layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(240));
	}
	else {
		layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(943));
	}
}

void LayerPlayGamePhom::event_EXT_EVENT_RES_END_PLAYING() {
	log("event_EXT_EVENT_RES_END_PLAYING");
}

void LayerPlayGamePhom::event_EXT_EVENT_RES_SET_BET() {

}

void LayerPlayGamePhom::event_EXT_EVENT_RES_U() {

}

void LayerPlayGamePhom::event_EXT_EVENT_RES_PUSH_CARD() {
	boost::shared_ptr<unsigned char> localChar = param->GetByte(EXT_FIELD_RESULT);
	if (localChar == NULL) return;
	int rscode = *(localChar.get());
	log("event_EXT_EVENT_RES_PUSH_CARD rscode = %d", rscode);
	if (rscode == PHOM_RSCODE_OK) {
		// Gui thanh cong
	}
	else if (rscode == PHOM_RSCODE_VALID_MULTI_PUSHCARD) {

		// co nhieu bo phom thoa man quan bai gui di
		// EXT_FIELD_DESTINATION_PHOMS = "dstphs";      // ds các phỏm có thể gửi bài
		boost::shared_ptr<string> dstphs = param->GetUtfString(EXT_FIELD_DESTINATION_PHOMS);
		if (dstphs == NULL) return;
		this->dstphsString = (string)dstphs->c_str();
		// dstphs = playerphom1/playerphom2/.../playerphomN
		// playerphom = idx1:idx2:idxn;phomID;userName
		log("dstphsString= | %s |", dstphsString.c_str());

		/*
		Ở đây khi đã xác định được nhưng nơi có thể đc gửi tới,
		ta đặt ở đó những Button mà khi nhấn vào Button đó,
		tương ứng dữ liệu sẽ được gửi đi
		*/
		vector<string> arrPlayerPhom = mUtils::splitString(this->dstphsString, '/');
		for (int i = 0; i < arrPlayerPhom.size(); i++)
		{
			vector<string> arr = mUtils::splitString(arrPlayerPhom.at(i), PHOM_CHAR_SPLIT_1);
			if (arr.size() == 0) return;
			int idx1 = std::atoi(arr.at(0).c_str());
			createButton_PushMulti_By_CardId(idx1);
		}
	}


}

void LayerPlayGamePhom::event_EXT_SRVNTF_CAN_TAKE() {
	boost::shared_ptr<unsigned char> cardid = param->GetByte(EXT_FIELD_CARD_INDEX);
	if (cardid == NULL) return;
	layerCards->setIdCardCanEat(*(cardid.get()));
	getButtonByTag(kTagButtonEat)->setVisible(true);
}

void LayerPlayGamePhom::event_EXT_SRVNTF_PLAYER_REPLAY() {
	boost::shared_ptr<string> fname = param->GetUtfString(EXT_FIELD_USERNAME);//duoc tai ha thi reset co ha phom
	if (fname == NULL) return;
	if (strcmp(fname->c_str(), myName.c_str()) == 0) {
		layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(241));
		this->isHaPhom = false;
	}
}

void LayerPlayGamePhom::event_EXT_SRVNTF_U_9() {
	// 	log("U 9 - NGOC TRINH");
	// 	//layerCards->refreshCardOnHand();
	this->isU9 = true;
	getButtonByTag(kTagButtonU)->setVisible(true);
	//layerCards->actionRecommendCard();
}

void LayerPlayGamePhom::event_EXT_EVENT_NTF_TIMER()
{
	//public static final String EXT_FIELD_TIMER_VAL = "tval";     
	log("event_EXT_EVENT_NTF_TIMER");
	boost::shared_ptr<long> time_val = param->GetInt(EXT_FIELD_TIMER_VAL);
	if (time_val != NULL){
		float duration = (float)*time_val;
		log("TIMEVAL = %.1f - currentPlay = %s", duration, currentPlaying.c_str());
		if (isStartedGame)// phai them cai nay kiem tra, vi luc end game van thay gui ve
		{
			log("START TIMER");
			layerAvatars->runTimerWithTime(layerAvatars->getPosByName(currentPlaying), duration / 1000);
		}

	}
	else{
		layerAvatars->runTimerWithTime(layerAvatars->getPosByName(currentPlaying), discardTimeout);
	}
}

void LayerPlayGamePhom::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);
	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> ptrNotifiedMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);
	if (ptrNotifiedMessage == NULL || ptrNotifiedUser == NULL) return;
	//
    if (dataManager.getAppConfig().is24h)
    {
        return;
    }
	log("ptrNotifiedMessage: %s", ptrNotifiedMessage->c_str());
	//
	int vt = layerAvatars->getPosByName(ptrNotifiedUser->Name()->c_str());
	if (vt == -1) return;
	Avatar52La* avatar = layerAvatars->getUserByPos(vt);
	if (avatar)
	{
		avatar->showChat(ptrNotifiedMessage->c_str());
	}
}

void LayerPlayGamePhom::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	boost::shared_ptr<Room> room = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	//
	boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
	if (rv != NULL){
		boost::shared_ptr<string> param_ptr = rv->GetStringValue();
		if (param_ptr == NULL) return;
		string param_str = param_ptr->c_str();
		//
		vector<string> ArrParams = mUtils::splitString(param_str, '@');
		if (ArrParams.size() > 3){
			string xml = "<?xml version=\"1.0\"?>" + ArrParams.back();
			XMLResults xe;
			XMLNodeEx xMainNode = XMLNodeEx::parseString(xml.c_str(), NULL, &xe);
			if (xe.error != XMLError::eXMLErrorNone) return;
			const char* ready_str = xMainNode.getChildNodeByPath("tableconfig.timeout.playerReadyTimeout", true, '.').getText();
			const char* discard_str = xMainNode.getChildNodeByPath("tableconfig.timeout.discardTimeout", true, '.').getText();
			const char* turnTime_str = xMainNode.getChildNodeByPath("tableconfig.timeout.turnTimeout", true, '.').getText();
			const char* endGame_str = xMainNode.getChildNodeByPath("tableconfig.timeout.endGameTimeout", true, '.').getText();
			const char* isNuoi_str = xMainNode.getChildNodeByPath("tableconfig.playing_rule.choiNuoiGa", true, '.').getText();
			const char* playerFirstReadyTimeout = xMainNode.getChildNodeByPath("tableconfig.timeout.playerFirstReadyTimeout", true, '.').getText();
			if (ready_str == NULL || discard_str == NULL || turnTime_str == NULL ||
				endGame_str == NULL || isNuoi_str == NULL || playerFirstReadyTimeout == NULL)
				return;
			if (isFirstReady)
				readyTimeOut = std::atoi(playerFirstReadyTimeout);//timeout cho lan dau
			else
			{
				readyTimeOut = std::atoi(ready_str);//time out cho nhung van tiep theo
				if (layerTimer)
					layerTimer->setCount(readyTimeOut);
			}
			discardTimeout = std::atoi(discard_str);
			turnTimeout = std::atoi(turnTime_str);
			endGameTimeout = std::atoi(endGame_str);
			// 			isNuoiGa = strcmp(isNuoi_str, "true") == 0 ? true : false;
			// 			if (isNuoiGa)
			// 			{
			// 				layerGa->setConfigChicken(LayerGa::CONFIG::ON);
			// 			}
			// 			else
			// 			{
			// 
			// 				layerGa->setConfigChicken(LayerGa::CONFIG::OFF);
			// 			}
		}
	}
}

void LayerPlayGamePhom::event_EXT_SRVNTF_U()
{
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_U, params, lastRoom));

	GameServer::getSingleton().Send(request);
}

void LayerPlayGamePhom::event_EXT_SRVNTF_PLAYER_KICKOUT()
{
	log("event_EXT_SRVNTF_PLAYER_KICKOUT");
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERNAME);
	boost::shared_ptr<long int> localChar = param->GetInt(EXT_FIELD_KICKOUT_REASON);
	if (localChar == NULL) return;
	int rscode = *(localChar.get());
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf == NULL) return;
	if (uid->compare(mySelf->Name()->c_str()) == 0){
		log("event_EXT_SRVNTF_PLAYER_KICKOUT rscode = %d", rscode);
		switch (rscode)
		{
		case 0: /// het tien
		{
					SceneManager::getSingleton().setKickNem(true);
					layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(52));
					//hien popup nap tien
					//HoangDD
					//SceneManager::getSingleton().gotoChonBanChoi(atol(gameID.c_str()));
		}
			break;
		case 1: // het thoi gian ma khong san sang
		{
					layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(242));
					layerTimer->stopTimer();
					myReady = true;
					// 					boost::shared_ptr<IRequest> request(new PlayerToSpectatorRequest());
					// 					GameServer::getSingleton().Send(request);
		}
			break;
		case 2:
		{
				  SceneManager::getSingleton().gotoPickGame(kGamePhom);
		}
			break;
		case 3: // bi kich boi chu phong
			if (strcmp(uid->c_str(), myName.c_str()) == 0)
			{
				SceneManager::getSingleton().setKickRoom(true);
			}
			break;
		case 4://KICK_OUT_BY_OVER_STEP_TIMEOUT_LIMIT = 4; 
		{
				   layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(242));
		}
			break;
		default:
			break;
		}

	}

}

void LayerPlayGamePhom::actionUnReady(Ref *pSender, Widget::TouchEventType pType)
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

void LayerPlayGamePhom::event_EXT_SRVNTF_USER_UNREADY()
{

	boost::shared_ptr<string> fname = param->GetUtfString(EXT_FIELD_USERID);
	if (fname == NULL) return;
	if (strcmp(getButtonByTag(kTagButtonReady)->getTitleText().c_str(), dataManager.GetSysString(197).c_str()) == 0)
		layerTimer->stopTimer();
	if (strcmp(fname->c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0){
		getButtonByTag(kTagButtonUnReady)->setVisible(false);
		layerTimer->startTimer();
		// hiện unready
		getButtonByTag(kTagButtonReady)->setVisible(true);
		layerAvatars->getUserByPos(kUserMe)->setReady(false);
	}
	else{
		switch (layerAvatars->getPosByName(fname->c_str())) {
		case kUserLeft:
			layerAvatars->getUserByPos(kUserLeft)->setReady(false);
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
}

std::string LayerPlayGamePhom::find_ChuPhong(string listUser)
{
	if (listUser.size() <= 0) return "";
	vector<string> arrUser = split(listUser, PHOM_CHAR_SPLIT_1);
	vector<string> info = split(arrUser[0], PHOM_CHAR_SPLIT_2);
	string boosId = info[1];
	return boosId;
}

void LayerPlayGamePhom::createLayerTimer()
{
	layerTimer = LayerTimer::create();
	layerTimer->setCount(readyTimeOut);
	this->addChild(layerTimer);
}

void LayerPlayGamePhom::event_EXT_EVENT_RES_LEAVE_GAME()
{
}

void LayerPlayGamePhom::setFuncallBack(Ref *pSender)
{
	int tag = ((Node*)pSender)->getTag();
	switch (tag)
	{
	case 0:{ // hiển thị các button
			   getButtonByTag(kTagButtonSort)->setVisible(true);
			   getButtonByTag(kTagButtonHit)->setVisible(true);
			   getButtonByTag(kTagButtonHaPhom)->setVisible(true);
			   getButtonByTag(kTagButtonPush)->setVisible(true);
			   //getButtonByTag(kTagButtonGive)->setVisible(false);
	}
		break;
	case 1:{ ///ẩn nút sẵn sàng và hiện nút sắp xếp
			   getButtonByTag(kTagButtonReady)->setVisible(false);
			   getButtonByTag(kTagButtonSort)->setVisible(true);
	}
		break;
	case 2://click vao quan bai de an
		actionEatCards(NULL, Widget::TouchEventType::ENDED);
		break;
	default:
		break;
	}
}

void LayerPlayGamePhom::updateEvent(float dt)
{
	if (mThreadQueueMsg == NULL) return;
	if (mThreadQueueMsg->isQueueEmpty())
		return;
	mThreadQueueMsg->run();
}

void LayerPlayGamePhom::createLayerGa()
{
	this->layerGa = LayerGa::create();
	this->layerGa->resetAll();
	//this->layerGa->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));;
	this->layerGa->callBackFromActionGa(this, callfunc_selector(LayerPlayGamePhom::actionConfigGa));
	this->addChild(layerGa);
	// 	this->layerGa->setNuoiGa(isNuoiGa);
	// 	if (!isNuoiGa) this->layerGa->setVisible(false);
}

void LayerPlayGamePhom::event_EXT_EVENT_CHICKEN_UPDATE_NTF()
{
	boost::shared_ptr<long> tienga = param->GetInt(EXT_FIELD_CHICKEN_VALUE);
	boost::shared_ptr<long> lv_ga = param->GetInt(EXT_FIELD_CHICKEN_LEVEL);
	boost::shared_ptr<string> listUserGa = param->GetUtfString("lug");
	if (tienga != NULL && lv_ga != NULL){
		int chicken_value = (int)*tienga;
		int chicken_lv = (int)*lv_ga;
		if (listUserGa != NULL){
			string lstGa = listUserGa->c_str();
			layerGa->setListUser(lstGa, PHOM_CHAR_SPLIT_1, PHOM_CHAR_SPLIT_2);
		}
		layerGa->setChickenLv(chicken_lv);
		layerGa->setChickenMoney(chicken_value);
	}
}

void LayerPlayGamePhom::initParam()
{
	///1000@0@2@<timeout><playerReadyTimeout>30</playerReadyTimeout><discardTimeout>30</discardTimeout><turnTimeout>30</turnTimeout></timeout> </tableconfig>
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<RoomVariable> param = lastRoom->GetVariable("params");
	if (param == NULL){
		return;
	}
	string param_str = *param->GetStringValue();
	//
	vector<string> ArrParams = mUtils::splitString(param_str, '@');
	if (ArrParams.size() > 3){
		string xml = "<?xml version=\"1.0\"?>" + ArrParams.back();
		XMLResults xe;
		XMLNodeEx xMainNode = XMLNodeEx::parseString(xml.c_str(), NULL, &xe);
		if (xe.error != XMLError::eXMLErrorNone) return;
		const char* ready_str = xMainNode.getChildNodeByPath("tableconfig.timeout.playerReadyTimeout", true, '.').getText();
		const char* discard_str = xMainNode.getChildNodeByPath("tableconfig.timeout.discardTimeout", true, '.').getText();
		const char* turnTime_str = xMainNode.getChildNodeByPath("tableconfig.timeout.turnTimeout", true, '.').getText();
		const char* endGame_str = xMainNode.getChildNodeByPath("tableconfig.timeout.endGameTimeout", true, '.').getText();
		const char* isNuoi_str = xMainNode.getChildNodeByPath("tableconfig.playing_rule.choiNuoiGa", true, '.').getText();
		const char* playerFirstReadyTimeout = xMainNode.getChildNodeByPath("tableconfig.timeout.playerFirstReadyTimeout", true, '.').getText();
		if (ready_str == NULL || discard_str == NULL || turnTime_str == NULL ||
			endGame_str == NULL || isNuoi_str == NULL || playerFirstReadyTimeout == NULL)
			return;
		readyTimeOut = std::atoi(playerFirstReadyTimeout);
		discardTimeout = std::atoi(discard_str);
		turnTimeout = std::atoi(turnTime_str);
		endGameTimeout = std::atoi(endGame_str);
		isNuoiGa = isNuoi_str;
		layerTimer->setCount(readyTimeOut);
		if (isNuoiGa)
		{
			layerGa->setConfigChicken(LayerGa::CONFIG::ON);
		}
		else
		{
			layerGa->setConfigChicken(LayerGa::CONFIG::OFF);
		}
	}
}

void LayerPlayGamePhom::resetCard(float dt)
{
	layerCards->resetGame();
	//layerAvatars->setListUserByPhom(lstUser->c_str());
	layerAvatars->setUnReadyAllUser();
}

void LayerPlayGamePhom::callBackFromTimer()
{
	if (!getButtonByTag(kTagButtonHaPhom)->isVisible()) return;
	///het thoi gian timeout thi tu ha phom
	vector<int> arrID = layerCards->getIdCardByClicked();
	string listID;
	int length = arrID.size();
	log("callBackFromTimer ::LENGTH ARRID = %d", arrID.size());
	if (length < 3)
	{
		return;
	}
	for (int i = 0; i < length; i++) {
		listID += boost::to_string(arrID[i]);
		if (i != length - 1) listID += ":";
	}

	log("callBackFromTimer callbackHaPhom listID= %s", listID.c_str());
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString("lc", listID);
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_HANDOFF_PHOM, params, lastRoom));
	GameServer::getSingleton().Send(request);
}

bool LayerPlayGamePhom::isSpectator()
{
	if (lstUser == NULL) return true;
	vector<string> arr = mUtils::splitString(*lstUser, PHOM_CHAR_SPLIT_1);
	int size = arr.size();
	for (int i = 0; i < size; i++){
		vector<string> arrInfo = split(arr.at(i), PHOM_CHAR_SPLIT_2);
		if (arrInfo.size() < 2) continue;
		string name = arrInfo.at(1);
		if (name == myName) return false;
	}

	return true;
}

void LayerPlayGamePhom::actionConfigGa()
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
	// 		//vao choi roi thi khoi config hoac khong phai chu ban// tien ga lon hon 0 cung khong dc config
	// 		bool isChuPhong = false;
	// 		if (lstUser != NULL)
	// 			isChuPhong = strcmp(find_ChuPhong(*lstUser).c_str(), myName.c_str()) == 0;
	// 		if (layerGa->getChickenMoney() > 0 ||//danh cho chu ban
	// 			!isChuPhong)//danh cho thang choi
	// 
	// 		{
	// 			layerGa->showListChicken();
	// 			return;
	// 		}
	// 		//public static final String EXT_FIELD_GAME_CFG = "gcfg";        // game config xml
	// 		string isChickenOn;
	// 		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	// 		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	// 		if (lastRoom == NULL)
	// 			return;
	// 		if (layerGa->getNuoiGa() == false){ //trang thai da tat, gui ban tin bat ga
	// 			isChickenOn = "true";
	// 		}
	// 		else{// gui ban tin tat ga
	// 			isChickenOn = "false";
	// 		}
	// 		try
	// 		{
	// 			string xmlConfig = StringUtils::format("<tableconfig><playing_rule><U_0_Factor>5</U_0_Factor><U_9_Factor>5</U_9_Factor><U_KHAN_Factor>5</U_KHAN_Factor><U_10_Factor>10</U_10_Factor><rank2Factor>1</rank2Factor><rank3Factor>2</rank3Factor><rank4Factor>3</rank4Factor><momFactor>4</momFactor><fakeTaking_Factor>5</fakeTaking_Factor><discardBeforeHandoffPhom>false</discardBeforeHandoffPhom><take3MustEndPlaying>true</take3MustEndPlaying><endPlayingAfterU10>true</endPlayingAfterU10><choiNuoiGa>%s</choiNuoiGa></playing_rule><timeout><playerReadyTimeout>%d</playerReadyTimeout><discardTimeout>%d</discardTimeout><turnTimeout>%d</turnTimeout><endGameTimeout>%d</endGameTimeout></timeout></tableconfig>", isChickenOn.c_str(), readyTimeOut, discardTimeout, turnTimeout, endGameTimeout);
	// 			parameter->PutUtfString("gcfg", xmlConfig);
	// 			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_UPDATE_GAME_CONFIG, parameter, lastRoom));
	// 			GameServer::getSingleton().Send(request);
	// 		}
	// 		catch (boost::exception &e)
	// 		{
	// 			std::cerr << boost::diagnostic_information(e);
	// 		}
	// 	}
	// 	if (isStartedGame){
	// 		if (layerGa->getChickenMoney() <= 0) return;
	// 		layerGa->showListChicken();
	// 		return;
	// 	}
	// 	else{
	// 		//fix bat tat ga bay gio thang nao cung bat tat duoc tru thang khach
	// 		if (layerGa->getListGa().size() > 1 && isSpector)//danh cho thang xem
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
	//	}
}

void LayerPlayGamePhom::actionLeaveGame(Ref* pSender)
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

void LayerPlayGamePhom::event_EXT_EVENT_RES_UPDATE_GAME_CONFIG()
{
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT);

	if (rscode != NULL) {
		int codeInt = (int)*rscode;
		log("rescode = %d", codeInt);
		// 		if (codeInt == 0)// config thanh cong
		// 		{
		// 			if (layerGa->getNuoiGa()){
		// 				layerGa->setConfigChicken(LayerGa::CONFIG::OFF);//tat ga
		// 			}
		// 			else{
		// 				layerGa->setConfigChicken(LayerGa::CONFIG::ON);//bat ga
		// 			}
		// 		}

	}
}

void LayerPlayGamePhom::showListUserChicken()
{
	if (layerGa->getChickenMoney() <= 0) return;
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

void LayerPlayGamePhom::showButtonReady(float dt)
{
	getButtonByTag(kTagButtonReady)->setVisible(true);
}

void LayerPlayGamePhom::event_EXT_SRVNTF_NOC_COUNT()
{
	boost::shared_ptr<long> count_noc = param->GetInt(EXT_FIELD_NOC_COUNT);
	if (count_noc != NULL){
		labelNoc->setString(StringUtils::toString(*count_noc));
	}
}

void LayerPlayGamePhom::showBocNoc(bool isBoc)
{
	getButtonByTag(kTagButtonGive)->setEnabled(isBoc);
	if (isBoc){//co the boc
		this->arrowNoc->setVisible(true);
		this->arrowNoc->runAction(RepeatForever::create(
			Sequence::create(MoveBy::create(0.5, Vec2(0, 5)),
			MoveBy::create(0.5, Vec2(0, -5)), nullptr)));
	}
	else{
		this->arrowNoc->setPosition(WIDTH_DESIGN / 2 + sizeAdd.width, HEIGHT_DESIGN / 2 + 110 + sizeAdd.height + SIZE_ADD / 2);
		this->arrowNoc->setVisible(false);
		this->arrowNoc->stopAllActions();
	}
}

void LayerPlayGamePhom::event_EXT_EVENT_REGISTER_QUEUE_NTF()
{
	boost::shared_ptr<string> lu = this->param->GetUtfString(EXT_FIELD_LIST_USER);

	if (lu != NULL)
	{
		log("event_EXT_EVENT_REGISTER_QUEUE_NTF %s", lu->c_str());
		this->layerAvatars->setListUserGuess(lu->c_str(), ';');
		bool isChauRia = this->isNguoiChauRia(lu->c_str());
	}

}

bool LayerPlayGamePhom::isNguoiChauRia(string _list)
{
	vector<string> arrUser = mUtils::splitString(_list, PHOM_CHAR_SPLIT_1);
	for (int i = 0; i < arrUser.size(); ++i)
	{
		vector<string> info = mUtils::splitString(arrUser.at(i), PHOM_CHAR_SPLIT_2);
		if (info.size() >1)
		{
			string userName = info.at(1);
			if (strcmp(userName.c_str(), myName.c_str()) == 0)
				return true;
		}
	}
	return false;
}

void LayerPlayGamePhom::event_EXT_SRVNTF_RECOMMENDED_PUSH_CARDS()
{
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERNAME);
	boost::shared_ptr<string> listcard = this->param->GetUtfString(EXT_FIELD_PUSCARDS_DESTINATION);
	if (uid == NULL || listcard == NULL) return;
	if (strcmp(myName.c_str(), uid->c_str()) == 0)
		layerCards->actionRecommendPushCard(listcard->c_str());
}

void LayerPlayGamePhom::event_EXT_EVENT_RES_JOIN_GAME()
{
	Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(LayerPlayGamePhom::sendRequestJoinGameInterval),this);
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT);
	if (rscode != NULL) {
		int rc = (int)(*rscode);
		log("rscode join game: %d", rc);
		if (rc == PHOM_RSCODE_OK) {
			//getButtonByTag(kTagButtonSitting)->setVisible(false);
		}
		else if (rc == PHOM_RSCODE_ERROR_GAME_FULL)
		{
			layerNumbers->showChatByPos(kUserMe, dataManager.GetSysString(198));
		}
		else if (rc == PHOM_RSCODE_HAVE_JOINED_BEFRORE) {
			//getButtonByTag(kTagButtonSitting)->setVisible(false);
		}
		else if (rc == PHOM_RSCODE_UNDER_MINIMUM_BALANCE){
			layerButtons->showToast(dataManager.GetSysString(10));
			layerButtons->eventTouchBtnCoin(NULL, Widget::TouchEventType::ENDED);
		}
	}
}

void LayerPlayGamePhom::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	log("Update User Variables");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	if (ptrEventParams == NULL)
		return;
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
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
					setUckToUserDefault(uck == LayerGa::CONFIG::ON ? true : false);
				}
			}
		}
	}
}







