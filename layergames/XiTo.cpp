//
//  XiTo.cpp
//  iCasinov2
//
//  Created by DauA on 5/28/14.
//
//

#include "XiTo.h"
#include "_Background_inGame_.h"
#include "LayerOpenCard_Xito.h"
#include "SliderCustomLoader.h"
#include "Requests/ExtensionRequest.h"
#include "_Chat_.h"
#include "mUtils.h"
#include "SceneManager.h"
#include "data_manager.h"
#include "PhomMessDef.h"
#include <boost/algorithm/string/trim.hpp>
#include "XiToPopupChonTien.h"
#define kTag_PopupOpenCard 444

#define z_order_timer_ready 900
#define z_order_layer_opencard 901
#define z_order_layer_button 902
#define z_tag_popup_chontien 9898

XiTo::XiTo() :
PlayerMe(NULL),
PlayerLeftBottom(NULL),
PlayerLeftTop(NULL),
PlayerRightTop(NULL),
PlayerRightBottom(NULL),
//object
spFrameBetTotal(NULL),
lblBetTotal(NULL),
LayerFrameBets(NULL),
layerLabelVictype(NULL),
LayerAvatars(NULL),
LayerButtons(NULL),
TimerToReady(NULL),
//string
MyAI(""),
s_TwoCardsFirst(""),
s_ListPlayers(""),

//int
CountDownToReady(15),
m_CountGive2CardFirst(0),
m_CountGiveCardNext(0),
m_TimeDelayToEndGame(0),
m_Duration(0.3),
f_RoomBet(0),
f_TotalCurrentRoomBet(0),
f_CurrentTurnBet(0),
//boolean
mThreadQueueMsg(NULL),
IsSpect(true),
_pTimerReady(10),
_pTurnTime(30),
_pTimeTurnOne(15),
StepTimeout(30),
isRegistLeave(false)
{
	//vector
	this->posgive.clear();
	this->mapPlayers.clear();
	this->listBet.clear();
	log("Constructor Xi To Game Layer");
}

bool XiTo::init() {
	if (!Layer::init())
		return false;

	if (GameServer::getSingleton().getSmartFox()->MySelf() == NULL)
		return false;

	this->MyAI = *GameServer::getSingleton().getSmartFox()->MySelf()->Name();

	this->GetRoomVar();

	Size _cardsize = Size(106, 139);
	this->PlayerMe = PlayerXiToIsMe::create();
	this->PlayerMe->SetPosBeginCard(890, 200);
	this->PlayerMe->SetSizeCard(_cardsize);

	this->PlayerLeftBottom = PlayerXiToNormal::create();
	this->PlayerLeftBottom->SetPosBeginCard(400, (HEIGHT_DESIGN >> 1) - 150);
	this->PlayerLeftBottom->SetSizeCard(_cardsize);

	this->PlayerLeftTop = PlayerXiToNormal::create();
	this->PlayerLeftTop->SetPosBeginCard(500, 830);
	this->PlayerLeftTop->SetSizeCard(_cardsize);

	this->PlayerRightTop = PlayerXiToNormal::create();
	this->PlayerRightTop->SetPosBeginCard(WIDTH_DESIGN - 780, 830);
	this->PlayerRightTop->SetSizeCard(_cardsize);

	this->PlayerRightBottom = PlayerXiToNormal::create();
	this->PlayerRightBottom->SetPosBeginCard(WIDTH_DESIGN - 650, (HEIGHT_DESIGN >> 1) - 150);
	this->PlayerRightBottom->SetSizeCard(_cardsize);

	this->PlayerMe->setAnchorPoint(Vec2::ZERO);
	this->PlayerLeftBottom->setAnchorPoint(Vec2::ZERO);
	this->PlayerLeftTop->setAnchorPoint(Vec2::ZERO);
	this->PlayerRightTop->setAnchorPoint(Vec2::ZERO);
	this->PlayerRightBottom->setAnchorPoint(Vec2::ZERO);

	this->PlayerMe->setPosition(Vec2::ZERO);
	this->PlayerLeftBottom->setPosition(Vec2::ZERO);
	this->PlayerLeftTop->setPosition(Vec2::ZERO);
	this->PlayerRightTop->setPosition(Vec2::ZERO);
	this->PlayerRightBottom->setPosition(Vec2::ZERO);

	this->CreateBackground();
	this->CreateAvatar();
	this->CreateButtons();
	this->CreateFrameBets();

	this->addChild(this->PlayerMe);
	this->addChild(this->PlayerLeftBottom);
	this->addChild(this->PlayerLeftTop);
	this->addChild(this->PlayerRightTop);
	this->addChild(this->PlayerRightBottom);

	this->mThreadQueueMsg = new ThreadQueueMsg();
	this->mThreadQueueMsg->setFunCallBack(this, callfunc_selector(XiTo::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(XiTo::updateEvent),this,1/60, false);

	/*test*/
	this->OnButtonDisplay(NULL, ui::Widget::TouchEventType::ENDED);

	GameServer::getSingleton().addListeners(this);
	SceneManager::getSingleton().hideLoading();
	//string typeallow = "0:0.0, 5:0.0, 6:228000.0, 7:309600.0, 3:84000.0, 4:168000.0";
	//SetButtonBet("mrtuan2", typeallow);
	return true;
}

XiTo::~XiTo() {
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(XiTo::updateEvent),this);
	if (this->getChildByTag(HEIGHT_DESIGN) != NULL)
		this->removeChildByTag(HEIGHT_DESIGN);
	if (this->getChildByTag(WIDTH_DESIGN) != NULL)
		this->removeChildByTag(WIDTH_DESIGN);
	if (this->getChildByTag(HEIGHT_DESIGN + 1) != NULL)
		this->removeChildByTag(HEIGHT_DESIGN + 1);
	GameServer::getSingleton().removeListeners(this);

	if (this->mThreadQueueMsg != NULL) delete this->mThreadQueueMsg;
}

void XiTo::CreateAvatar() {
	this->LayerAvatars = LayerAvatarXiTo::create();
	this->addChild(this->LayerAvatars);
}

void XiTo::CreateBackground() {

	Size SizeAdd = mUtils::getSizePos();
	Size winsize = Director::getInstance()->getVisibleSize();

    Sprite* bg = Sprite::create("moc3-bg-game.jpg");
    bg->setScaleX(winsize.width / bg->getContentSize().width);
    bg->setScaleY(winsize.height / bg->getContentSize().height);
    bg->setAnchorPoint(Vec2(0, 0));
    bg->setPosition(Vec2(0, 0));
    this->addChild(bg);
    
    
    Sprite* table = Sprite::create("girl-table.png");
    table->setAnchorPoint(Vec2(0.5, 0.5));
    table->setPosition(Vec2(winsize.width / 2, winsize.height / 2));
    
    this->addChild(table);

	/*moc3-remove*/
	/*Sprite* bg_top = Sprite::create("bg_top.png");
	bg_top->setAnchorPoint(Vec2(0,1));
	bg_top->setPosition(Vec2(0,winsize.height + SizeAdd.height));
	this->addChild(bg_top);


	Sprite* realbg = Sprite::create("xito-table.png");
	realbg->setAnchorPoint(Vec2(0.5, 0.5));
	realbg->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN /2+100+ SizeAdd.height));
	this->addChild(realbg);

	this->bgButton = Sprite::create("xito-to-bg.png");
	this->bgButton->setAnchorPoint(Vec2(1, 0));
	this->bgButton->setPosition(Vec2(WIDTH_DESIGN, 0));
	this->addChild(this->bgButton);*/

	/*moc3-end change*/
	string paramString = "";
	boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (room != NULL) {
		boost::shared_ptr<RoomVariable> param = room->GetVariable("params");
		if (param != NULL)
			paramString = *param->GetStringValue();
	}

	log("paramString = %s", paramString.c_str());

	if (paramString.size() < 2 || paramString == "")
		paramString == "500@0@1";

	string moneyConvert = "500";
	vector<string> arrInfo = mUtils::splitString(paramString, '@');

	try {
		log("arrInfo[0] = %s", arrInfo.at(0).c_str());
		int n = atoi(arrInfo.at(0).c_str());
		this->f_RoomBet = atof(arrInfo.at(0).c_str());
		moneyConvert = mUtils::convertMoneyEx(n);
	}
	catch (...) {

	}

	string result = "";
	if (moneyConvert.size() > 0)
		result = moneyConvert;

	// Hien thi ten phong choi
	Label* pGameName = Label::createWithSystemFont(result,"", 25);
	pGameName->setAnchorPoint(Vec2(0, 1));
	pGameName->setPosition(Vec2(180 - SizeAdd.width, HEIGHT_DESIGN - 30 + SizeAdd.height*2));
	pGameName->setColor(_colorTextNormal);
	pGameName->setScale(1.35);
	pGameName->setVisible(true);
	this->addChild(pGameName);

	if (room != NULL){
		string sRoomIdDisplay = mUtils::getRoomIDByName(*room->Name());
		if (sRoomIdDisplay == "")
			sRoomIdDisplay = boost::to_string(room->Id());

		log("sRoomIdDisplay = %s", sRoomIdDisplay.c_str());
		pGameName->setString( dataManager.GetSysString(747) + sRoomIdDisplay);
	}

	if (result.size() > 0) {
		Sprite *iconCoin = Sprite::create("moc3-icon-hao.png");
		int roomType = SceneManager::getSingleton().getCurrRoomType();
		if (roomType == 1){ //1- hao 0- moc
			iconCoin->setTexture("moc3-icon-hao.png");
		}
		else{
			iconCoin->setTexture("moc3-icon-moc.png");
		}
		iconCoin->setAnchorPoint(Vec2(0, 1));
		iconCoin->setPosition(Vec2(180 - SizeAdd.width, HEIGHT_DESIGN - 80 + SizeAdd.height*2));
		this->addChild(iconCoin);
		Label* pTableBet = Label::createWithSystemFont(result,"", 25);
		pTableBet->setAnchorPoint(Vec2(0, 1));
		pTableBet->setPosition(Vec2(iconCoin->getPositionX() + 50, HEIGHT_DESIGN - 80 + SizeAdd.height*2));
		pTableBet->setColor(_colorTextNormal);
		pTableBet->setScale(1.25);
		pTableBet->setVisible(true);
		this->addChild(pTableBet);
	}
}

void XiTo::CreateButtons() {
	this->LayerButtons = LayerButtonInGame::create();
	this->LayerButtons->setLocalZOrder(z_order_layer_button);
	this->addChild(this->LayerButtons);
    
    if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL){
        return;
    }

	boost::shared_ptr<RoomVariable> isVipRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("isviproom");
	if (isVipRoom != NULL){
		this->vipRoom = *isVipRoom->GetIntValue();
		if (vipRoom == 0){
			this->LayerButtons->processDisplayButtonInvite(false);
		}
		else{
			this->LayerButtons->processDisplayButtonInvite(true);
		}
	}
	Size SizeAdd = mUtils::getSizePos();
	/*moc3-remove*/
	//Button * btnChat = (Button*)this->LayerButtons->getChildByTag(TAG_LB_BTN_CHAT);
	//if (btnChat != NULL) {
	//	btnChat->setAnchorPoint(Vec2(0, 0));
	//	btnChat->setPosition(Vec2(-SizeAdd.width + 30, -SizeAdd.height + 30));
	//	btnChat->setVisible(true);
	//}

	//Button* btnCoin = (Button*)this->LayerButtons->getChildByTag(TAG_LB_BTN_COIN);
	//if (btnCoin != NULL) {
	//	btnCoin->setAnchorPoint(Vec2(0, 0));
	//	btnCoin->setPosition(Vec2(-SizeAdd.width + 183, -SizeAdd.height + 30));
	//	btnCoin->setVisible(true);
	//}

	//Button* btnBack = (Button*)this->LayerButtons->getChildByTag(TAG_LB_BTN_BACK);
	//if (btnBack != NULL){
	//	btnBack->setAnchorPoint(Vec2(0, 1));
	//	btnBack->setPosition(Vec2(20 - SizeAdd.width, HEIGHT_DESIGN - 20 + SizeAdd.height));
	//	//Vec2 pos = Vec2(20 - SizeAdd.width, HEIGHT_DESIGN - 20 + SizeAdd.height - btnBack->getContentSize().width);
	//	//this->LayerButtons->SetPosLayerSetting(pos);
	//}

	//Sprite* signal = (Sprite*)this->LayerButtons->getChildByTag(120);
	//if (signal != NULL){
	//	signal->setAnchorPoint(Vec2(1, 1));
	//	signal->setPosition(Vec2(WIDTH_DESIGN - 20 + SizeAdd.width, HEIGHT_DESIGN - 20 + SizeAdd.height));
	//}
	/*moc3-end Change*/
	float w_Button = 186;
	float h_Button = 148;
	// Button Ready, Start game
	this->btnDis = Button::create();
	this->btnDis->loadTextures("xito-start.png", "xito-start.png");
	this->btnDis->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
	this->btnDis->setTouchEnabled(true);
	//btnDis->setTag(DTAG_BTN_READY);
	//btnDis->addTouchEventListener(CC_CALLBACK_2(XiTo::OnBtnReadyClick, this));
	this->btnDis->setPressedActionEnabled(true);
	this->addChild(this->btnDis);

	this->btnReady = this->createButtonType1(1000, dataManager.GetSysString(29));
	this->btnReady->setAnchorPoint(Vec2(1, 0));
	this->btnReady->setPosition(Vec2(WIDTH_DESIGN - this->btnReady->getContentSize().width - 20, 20));
	this->btnReady->addTouchEventListener(CC_CALLBACK_2(XiTo::OnBtnReadyClick, this));
	this->btnReady->setTag(DTAG_BTN_READY);
	this->btnReady->setPressedActionEnabled(true);
	this->addChild(this->btnReady);

	// Timer count to start game.
	string s = boost::to_string(this->CountDownToReady);
	this->TimerToReady = Label::createWithBMFont(f_font_sfuStaff, s);
	this->TimerToReady->setLocalZOrder(z_order_timer_ready);
	this->TimerToReady->setVisible(false);
	this->TimerToReady->setPosition(Vec2((WIDTH_DESIGN >> 1) + 130, (HEIGHT_DESIGN >> 1) - 20));
	this->addChild(this->TimerToReady);

	Size _size = Size(w_Button, h_Button);
	this->btnToDisplay = this->createButtonType1(1000, dataManager.GetSysString(31));
	this->btnToDisplay->setAnchorPoint(Vec2(1, 0));
	this->btnToDisplay->setPosition(Vec2(this->btnReady->getPosition()));
	this->btnToDisplay->addTouchEventListener(CC_CALLBACK_2(XiTo::OnButtonDisplay, this));
	this->addChild(this->btnToDisplay);

	int _count = 1;
	ButtonXiTo* btnNhuong = this->createButtonType1(DTAG_BTN_GIVE, dataManager.GetSysString(672));
	btnNhuong->setAnchorPoint(Vec2(1, 0));
	btnNhuong->setPosition(Vec2(this->btnReady->getPositionX() - (20 + _size.width) * _count++, 20));
	this->addChild(btnNhuong);
	this->pArrayButton2.push_back(btnNhuong);

	ButtonXiTo* btnTheo = this->createButtonType1(DTAG_BTN_FOLLOW, dataManager.GetSysString(32));
	btnTheo->setAnchorPoint(Vec2(1, 0));
	btnTheo->setPosition(Vec2(this->btnReady->getPositionX() - (20 + _size.width) * _count++, 20));
	this->addChild(btnTheo);
	this->pArrayButton2.push_back(btnTheo);

	ButtonXiTo* btnUpBo = this->createButtonType1(DTAG_BTN_FOLD, dataManager.GetSysString(673));
	btnUpBo->setAnchorPoint(Vec2(1, 0));
	btnUpBo->setPosition(Vec2(this->btnReady->getPositionX() - (20 + _size.width) * _count++, 20));
	this->addChild(btnUpBo);
	this->pArrayButton2.push_back(btnUpBo);

	typedef boost::shared_ptr<TagName> _tagName;
	vector<_tagName> _arr;

	_arr.push_back(_tagName(new TagName(DTAG_BTN_BET, dataManager.GetSysString(31))));
	_arr.push_back(_tagName(new TagName(DTAG_BTN_BET_QUATAR, "1/4")));
	_arr.push_back(_tagName(new TagName(DTAG_BTN_BET_HAFT, "1/2")));
	_arr.push_back(_tagName(new TagName(DTAG_BTN_BET_X2, "X2")));
	_arr.push_back(_tagName(new TagName(DTAG_BTN_BET_ALL, dataManager.GetSysString(31)+" \n"+dataManager.GetSysString(670))));

	_size = Size(148, 118);

	int _zIndex = 1001;
	for (int i = 0; i < _arr.size(); ++i) {
		ButtonXiTo* btn1 = ButtonXiTo::create(_arr.at(i)->name);
		btn1->setAnchorPoint(Vec2(1, 0));
		btn1->setPosition(Vec2(0, 120 + (_size.height + 20) * i));
		btn1->setTag(_arr.at(i)->tag);
		btn1->loadTextures("xito-to-btn.png", "xito-to-btn.png");
		btn1->setVisible(false);
		btn1->addTouchEventListener(CC_CALLBACK_2(XiTo::OnButtonBetClick, this));
		btn1->createSuggest();
		btn1->setLocalZOrder(_zIndex++);
		this->addChild(btn1);

        Label* title = Label::createWithSystemFont(_arr.at(i)->name,"",25);
		title->setColor(Color3B::WHITE);
		title->setPosition(Vec2(_size.width / 2, _size.height / 2));
		btn1->addChild(title);
		this->pArrayButton1.push_back(btn1);
	}
	_arr.clear();

	this->VisibleButtonPlay();
}

ButtonXiTo* XiTo::createButtonType1(const int& tag, const string& name){

	// 	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	// 		_size = Size(184, 111);
	ButtonXiTo* btn1 = ButtonXiTo::create("");
	btn1->loadTextures("ingame-button.png", "ingame-button-active.png", "");
	btn1->setTag(tag);
	Size _size = btn1->getContentSize();

	Label* title = Label::createWithSystemFont(name,"",30);
	title->setColor(_colorTextNormal);
	title->setPosition(Vec2(_size.width / 2, _size.height / 2));
	btn1->addChild(title);

	//
	btn1->addTouchEventListener(CC_CALLBACK_2(XiTo::OnButtonBetClick, this));
	return btn1;
}

void XiTo::OnButtonDisplay(Ref *sender, ui::Widget::TouchEventType pType){
	if (pType == ui::Widget::TouchEventType::ENDED) {
		//Hien background
		//this->bgButton->setVisible(true);

		Size _size = Size(148, 118);
		int _count = 0;
		for (int i = 0; i < this->pArrayButton1.size(); ++i) {
			try {
				ButtonXiTo* btn = this->pArrayButton1.at(i);
				if (btn != NULL && btn->isAllow()) {
					btn->setPosition(Vec2(this->btnReady->getPositionX(), 138 + (_size.height + 20) * _count++));
					btn->setVisible(true);
				}
			}
			catch (...) {
				log("exception in onButtonDisplay");
			}
		}
	}
}

void XiTo::OnButtonBetClick(Ref *sender, ui::Widget::TouchEventType pType){
	if (pType == ui::Widget::TouchEventType::ENDED) {
		Node* btn = (Node*)sender;
		if (btn != NULL) {
			switch (btn->getTag()) {
			case DTAG_BTN_FOLD:
				this->SendBetNTF(GAME_TABLE_STATUS_BET_FOLD);
				break;
			case DTAG_BTN_FOLLOW:
				this->SendBetNTF(GAME_TABLE_STATUS_BET_FOLLOW);
				break;
			case DTAG_BTN_GIVE:
				this->SendBetNTF(GAME_TABLE_STATUS_BET_NONE);
				break;
			case DTAG_BTN_BET:
				this->SendBetNTF(GAME_TABLE_STATUS_BET_RAISE);
				break;
			case DTAG_BTN_BET_HAFT:
				this->SendBetNTF(GAME_TABLE_STATUS_BET_HAFT);
				break;
			case DTAG_BTN_BET_QUATAR:
				this->SendBetNTF(GAME_TABLE_STATUS_BET_QUATER);
				break;
			case DTAG_BTN_BET_ALL:
				this->SendBetNTF(GAME_TABLE_STATUS_BET_ALL);
				break;
			case DTAG_BTN_BET_X2:
				this->SendBetNTF(GAME_TABLE_STATUS_BET_DOUBLE);
				break;
			default:
				break;
			}
		}//end if button
	}
}

void XiTo::CreateFrameBets() {

	float left_FB_LT = 550;
	float left_FB_LB = 260;
	float bottom_TOP = 200;
	float bottom_BOTTOM = (HEIGHT_DESIGN >> 1) - 50;

	//Layer frame bet
	this->LayerFrameBets = Layer::create();
	this->LayerFrameBets->setAnchorPoint(Vec2(0, 0));
	this->LayerFrameBets->setPosition(Vec2(0, 0));
	this->addChild(this->LayerFrameBets);

	//layer victory type
	this->layerLabelVictype = Layer::create();
	this->layerLabelVictype->setAnchorPoint(Vec2(0, 0));
	this->layerLabelVictype->setPosition(Vec2(0, 0));
	this->addChild(this->layerLabelVictype);

	Size size = Size(235, 64);
	this->spFrameBetTotal = Sprite::create("xito_total_center.png");
	this->spFrameBetTotal->setPosition(Vec2((WIDTH_DESIGN >> 1), (HEIGHT_DESIGN >> 1) + 180));

    this->lblBetTotal = Label::createWithSystemFont("","",25);
	this->lblBetTotal->setColor(Color3B::BLACK);
	this->lblBetTotal->setPosition(Vec2(size.width / 2, size.height / 2));

	this->spFrameBetTotal->addChild(this->lblBetTotal);
	this->spFrameBetTotal->setVisible(false);
	this->LayerFrameBets->addChild(this->spFrameBetTotal);

	// Tao cac framebet cho nguoi choi
	this->PlayerMe->CreateFrameBet(WIDTH_DESIGN >> 1, this->spFrameBetTotal->getPositionY() - HEIGHT_DESIGN / 15, USER_ME);
	this->PlayerLeftTop->CreateFrameBet(left_FB_LT, bottom_TOP, USER_LEFT_TOP);
	this->PlayerLeftBottom->CreateFrameBet(left_FB_LB, bottom_BOTTOM, USER_LEFT_BOT);
	this->PlayerRightTop->CreateFrameBet(left_FB_LT, bottom_TOP, USER_RIGHT_TOP);
	this->PlayerRightBottom->CreateFrameBet(left_FB_LB, bottom_BOTTOM, USER_RIGHT_BOT);
}

void XiTo::HideAllFrameBet() {
	this->LayerAvatars->hideAllBetType();
	this->PlayerMe->HideFrameBet();
	this->PlayerLeftTop->HideFrameBet();
	this->PlayerLeftBottom->HideFrameBet();
	this->PlayerRightTop->HideFrameBet();
	this->PlayerRightBottom->HideFrameBet();
}

void XiTo::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	pObjs->PutUtfString("_cmd_", *cmd);
	//only add to queues
	this->mThreadQueueMsg->addQueue(pObjs);
}

void XiTo::OnExtensionResponseProcess() {
	this->param = this->mThreadQueueMsg->deQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");

	log("cmd in xi to = %s", cmd->c_str());
	if (0 == cmd->compare(EXT_EVENT_LEAVE_GAME_RES)){
		this->event_EXT_EVENT_LEAVE_GAME_RES();
	}

	//Bản tin khi có 1 người chơi vào phòng
	if (0 == cmd->compare(EXT_EVENT_USER_JOIN_NOTIF)) {
		this->event_EXT_EVENT_USER_JOIN_NOTIF();
	}

	if (0 == cmd->compare(EXT_EVENT_LIST_USER_UPDATE)){
		this->event_EXT_EVENT_LIST_USER_UPDATE();
	}

	//Khi có 1 người chơi rời phòng
	else if (0 == cmd->compare(EXT_EVENT_USER_LEAVE_NOTIF)) {
		this->event_EXT_EVENT_USER_LEAVE_NOTIF();
	}

	else if (0 == cmd->compare(EXT_EVENT_REGISTER_QUEUE_NTF)){
		this->event_EXT_EVENT_REGISTER_QUEUE_NTF();
	}

	//Khi người chơi Sắn Sàng
	else if (0 == cmd->compare(EXT_EVENT_USER_READY_NOTIF)) {
		this->event_EXT_EVENT_USER_READY_NOTIF();
	}

	// Tổng số tiền tố khi kết thúc game
	else if (0 == cmd->compare(EXT_EVENT_SUB_BALANCE_FIRST_NOTIF)) {
		this->event_EXT_EVENT_SUB_BALANCE_FIRST_NOTIF();
	}

	//Bắt đầu ván chơi
	else if (0 == cmd->compare(EXT_EVENT_START_GAME_NOTIF)) {
		this->event_EXT_EVENT_START_GAME_NOTIF();
	}

	// Sự kiện nhận được các lá bài của các người chơi
	// + Khi bắt đầu chỉ nhận được 2 lá của mình.
	// + Sau lượt 2 lá bài, thì cũng nhận các lá bài được chia thêm của tất cả các người chơi ở đây.
	else if (0 == cmd->compare(EXT_EVENT_DEAL_CARD_NOTIF)) {
		this->event_EXT_EVENT_DEAL_CARD_NOTIF();
	}

	//Một người mở deal card (1 trong 2 lá bài đầu tiên)
	else if (0 == cmd->compare(EXT_EVENT_OPEN_CARD_NOTIF)) {
		this->event_EXT_EVENT_OPEN_CARD_NOTIF();
	}

	// Đến lượt 1 người chơi Tố.
	// Note:
	// Sau khi cả 2 người chơi gửi lá bài tẩy của mình lên server:
	// + Server trả về bản tin EXT_EVENT_OPEN_CARD_NOTIF để client hiển thị các lá bài.
	// Ngay sau đó server trả về bản tin: EXT_EVENT_DEAL_CARD_NOTIF:
	// + Lúc này ta có danh sách "list_cards_all_player"
	// + Ngay ở lúc đó, ta chỉ tổng hợp thành 1 list để tạo list chia bài.
	// Khi xác định được người tố ở bản tin dưới đây thì ta mới thực hiện
	// các hiệu ứng chia bài dựa trên "list_cards_all_player"
	else if (0 == cmd->compare(EXT_EVENT_NEAD_RAISE_NOTIF)) {
		this->event_EXT_EVENT_NEAD_RAISE_NOTIF();
	}

	//Khi 1 người chơi Sẵn Sàng không thành công, server sẽ trả về bản tin này
	else if (0 == cmd->compare(EXT_EVENT_READY_RES)) {
		this->event_EXT_EVENT_READY_RES();
	}

	//Thông tin về lượt Tố của 1 người chơi
	else if (0 == cmd->compare(EXT_EVENT_RAISE_NOTIF)) {
		this->event_EXT_EVENT_RAISE_NOTIF();
	}

	//Bản tin nhận được cho biết giá trị bộ bài hiện tại trên tay người chơi (Mậu Thầu, Sám Cô, Cù Lũ, Thùng...)
	else if (0 == cmd->compare(EXT_EVENT_USER_HAVE_SET_NOTIF)) {
		event_EXT_EVENT_USER_HAVE_SET_NOTIF();
	}

	//Mở toàn bộ các lá bài của người chơi
	else if (0 == cmd->compare(EXT_EVENT_OPEN_ALL_CARD_NOTIF)) {
		this->event_EXT_EVENT_OPEN_ALL_CARD_NOTIF();
	}

	//Bản tin kết quả game: danh sách các lá bài, và kiểu chiến thắng
	else if (0 == cmd->compare(EXT_EVENT_VICTORY_NOTIF)) {
		this->event_EXT_EVENT_VICTORY_NOTIF();
	}

	//Bản tin cộng trừ tiền sau mỗi ván chơi
	else if (0 == cmd->compare(EXT_EVENT_CHANGE_BALANCE_NOTIF)) {
		this->event_EXT_EVENT_CHANGE_BALANCE_NOTIF();
	}

	// Có người vào lại phòng hoặc có người vào xem
	else if (0 == cmd->compare(EXT_EVENT_GAME_CHANGE_NOTIF)) {
		this->event_EXT_EVENT_GAME_CHANGE_NOTIF();
	}

	//Bản tin kết thúc ván chơi
	else if (0 == cmd->compare(EXT_EVENT_END_GAME_NOTIF)) {
		this->event_EXT_EVENT_END_GAME_NOTIF();
	}

	//Khi kết thúc ván, nếu người chơi hết tiền hoặc thiếu tiền thì nhận được bản tin này và bị đá ra ngoài
	else if (0 == cmd->compare(EXT_EVENT_USER_EXPIRE_MONEY_NOTIF)) {
		this->event_EXT_EVENT_USER_EXPIRE_MONEY_NOTIF();
	}

	// Nhận được bản tin này.
	// -> Bắt đầu đếm timer đến 15s, không ready sẽ bị out ra.
	else if (0 == cmd->compare(EXT_EVENT_START_READY_TIMER_NOTIF)) {
		this->event_EXT_EVENT_START_READY_TIMER_NOTIF();
	}

	//Bản tin bị kick ra khỏi phòng do không ready
	else if (0 == cmd->compare(EXT_EVENT_USER_KICKED_NOTIF)) {
		this->event_EXT_EVENT_USER_KICKED_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_EXPIRE_CHIP_NTF)){
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
			XiToPopupChonTien* chontien = XiToPopupChonTien::create();
			chontien->setMinMaxValue(nBetValue, bet_ratio*nBetValue);
			chontien->setJoinType(2);
			this->addChild(chontien, ZORDER_POPUP_CHONTIEN, z_tag_popup_chontien);
		}



	}
}

void XiTo::event_EXT_EVENT_LEAVE_GAME_RES(){
	boost::shared_ptr<long> rslt = this->param->GetInt(EXT_FIELD_RESULT);
	if (rslt != NULL) {
		if (0 == *rslt) {
			this->isRegistLeave = !this->isRegistLeave;
			this->LayerAvatars->showIconLeaveGame(this->isRegistLeave);
		}
	}
}

void XiTo::event_EXT_EVENT_USER_JOIN_NOTIF() {
	boost::shared_ptr<string> lu = this->param->GetUtfString(EXT_FIELD_LIST_USER);
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	if (lu && uid) {
		log("list user update: %s", lu->c_str());
		log("Nguoi moi vao phong: %s", uid->c_str());
		this->s_ListPlayers = *lu;
		this->EventListPlayers(this->s_ListPlayers);

		int _size = mUtils::splitString(this->s_ListPlayers, ';').size();
		if (_size >= 4){
			this->LayerButtons->processDisplayButtonInvite(false);
		}
		else{
			if (this->vipRoom == 1){
				this->LayerButtons->processDisplayButtonInvite(true);
			}
		}
	}
}

void XiTo::event_EXT_EVENT_LIST_USER_UPDATE(){
	boost::shared_ptr<string> lu = this->param->GetUtfString(EXT_FIELD_LIST_USER);
	if (lu != NULL) {
		this->s_ListPlayers = *lu;
		int _size = mUtils::splitString(this->s_ListPlayers, ';').size();
		if (_size >= 4){
			this->LayerButtons->processDisplayButtonInvite(false);
		}
		else{
			if (this->vipRoom == 1){
				this->LayerButtons->processDisplayButtonInvite(true);
			}
		}

		this->EventListPlayers(this->s_ListPlayers);
	}
}

void XiTo::event_EXT_EVENT_REGISTER_QUEUE_NTF(){
	boost::shared_ptr<string> lu = this->param->GetUtfString(EXT_FIELD_LIST_USER);
	if (lu != NULL) {
		log("danh sach chau ria: %s", lu->c_str());
		this->LayerAvatars->SetListSpectToEmpty(*lu, this->MyAI);
		//99:tuyennh8:0:0:19900.000000:0.000000: -78:dautv1993:0:0:25878.000000:0.000000:
	}
}

void XiTo::event_EXT_EVENT_USER_LEAVE_NOTIF() {
	boost::shared_ptr<string> lu = this->param->GetUtfString(EXT_FIELD_LIST_USER);
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	log("list user update: %s", lu->c_str());
	if (lu != NULL) {
		this->s_ListPlayers = *lu;
		this->EventListPlayers(this->s_ListPlayers);
	}
}

void XiTo::event_EXT_EVENT_USER_READY_NOTIF() {
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	if (uid != NULL) {
		int pos = GetPosPlayer(*uid);
		if (pos == -1)
			return;

		if (0 == uid->compare(this->MyAI)) {
			this->ResetTimeoutReady();
			this->HideButtonReady();
		}
		this->LayerAvatars->GetUserByPos(pos)->setReady(true);
	}
}

void XiTo::event_EXT_EVENT_SUB_BALANCE_FIRST_NOTIF() {
	boost::shared_ptr<double> bettt = this->param->GetDouble(EXT_FIELD_BET_TOTAL);
	if (bettt != NULL) {
		this->f_TotalCurrentRoomBet = *bettt;
		this->SetTotalMoney(this->f_TotalCurrentRoomBet);
	}
}

void XiTo::event_EXT_EVENT_START_GAME_NOTIF() {
	log("Game Xi To Start");
	// Nếu mình là khách mà khi delay endgame chưa qua 10s thì gọi hàm này.
	if (this->IsSpect || this->m_TimeDelayToEndGame >= 10) {
		this->ActionEndGame();
	}
}

void XiTo::event_EXT_EVENT_DEAL_CARD_NOTIF() {
	this->ResetTimeoutReady();

	boost::shared_ptr<string> allc = this->param->GetUtfString(EXT_FIELD_ALL_PLAYER_CARD);

	this->f_CurrentTurnBet = this->f_TotalCurrentRoomBet;

	if (allc != NULL) {
		//! allc = bot_300007|-1, -1|-1, -1;bot_200009|26, 32|26, 32;
		log("allc = %s", allc->c_str());
		std::vector<std::string> info = mUtils::splitString(*allc, ';');
		this->posgive.clear();
		bool b_Give = false;

		for (int i = 0; i < info.size(); ++i) {
			//            try {
			if (0 == info.at(i).compare("")) continue;
			std::vector<std::string> p = mUtils::splitString(info.at(i), '|');
			if (2 == mUtils::splitString(p.at(1), ',').size()) {
				if (!b_Give) b_Give = true;
				if (0 == p.at(0).compare(this->MyAI)) {
					this->s_TwoCardsFirst = p.at(1);
				}
				int pos = this->GetPosPlayer(p.at(0));
				this->posgive.push_back(Give(pos, "", ""));
			}
			else {
				int pos = this->GetPosPlayer(p.at(0));
				string img = XiToHelper::FindTypeCard(atoi(p.at(1).c_str()));
				this->posgive.push_back(Give(pos, img, p.at(2)));
			}
			//            } catch (std::out_of_range &e) {
			//               // log("event_EXT_EVENT_DEAL_CARD_NOTIF out_of_range exception: %s", e.what());
			//            } catch (std::exception &e) {
			//               // log("event_EXT_EVENT_DEAL_CARD_NOTIF std::exception %s", e.what());
			//            } catch (...) {
			//               // log("event_EXT_EVENT_DEAL_CARD_NOTIF unknown exception");
			//            }
		}

		if (this->posgive.size() > 0) {
			if (b_Give) {
				//! Chia 2 La bai
				this->LayerAvatars->UnReadyAllUser();
				this->HideButtonReady();
				this->Chia2LaBaiDauTien_1();
			}
			else {
				//! Chia 1 La bai
				this->listBet.clear();
				this->PlayerMe->ResetTurn();
				this->ChiaThem1LaBai_1();
			}
		}
	}
}

void XiTo::event_EXT_EVENT_USER_KICKED_NOTIF() {
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	if (uid != NULL) {
		if (0 == uid->compare(this->MyAI)) {
			this->LayerButtons->BackToChooseDesk();
		}
		else {
			log("%s da bi da ra khoi phong", uid->c_str());
		}
	}
}

void XiTo::event_EXT_EVENT_START_READY_TIMER_NOTIF() {
	//if (1 == this->mapPlayers.size()) return;
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	if (uid != NULL) {
		if (0 == uid->compare(this->MyAI)) {
			this->ActionEndGame();
			this->btnReady->setVisible(true);
			this->btnDis->setVisible(true);
			this->LayerAvatars->StopAllTimer();
			Director::getInstance()->getScheduler()->schedule(schedule_selector(XiTo::TimerToReadyFunc),this,1, false);
		}
	}
}

void XiTo::event_EXT_EVENT_USER_EXPIRE_MONEY_NOTIF() {
	log("User not enought money !");
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	if (uid != NULL) {
		if (0 == uid->compare(this->MyAI)) {
			this->LayerButtons->BackToChooseDesk();
		}
	}
}

void XiTo::event_EXT_EVENT_END_GAME_NOTIF() {
	Director::getInstance()->getScheduler()->schedule(schedule_selector(XiTo::DelayToEndGame),this,1, false);
	this->VisibleButtonPlay();
	if (!this->IsSpect) {
		this->LayerAvatars->StopAllTimer();
	}
}

void XiTo::event_EXT_EVENT_GAME_CHANGE_NOTIF() {
	boost::shared_ptr<string> ginf = this->param->GetUtfString(EXT_FIELD_GAME_INFO);
	boost::shared_ptr<string> betal = this->param->GetUtfString(EXT_FIELD_BET_TYPE_ALLOW);

	if (ginf == NULL) return;

	vector<string> btr = mUtils::splitString(*ginf, '|');

	//    try {
	this->s_ListPlayers = btr.at(3);

	this->SetMapPlayer(this->s_ListPlayers);
	this->EventListPlayers(this->s_ListPlayers);

	btr.clear();
	log("ListPlayer = %s", this->s_ListPlayers.c_str());
	log("Info Game = %s", ginf->c_str());

	this->EventReJoinGame(*ginf);

	if (betal != NULL) {
		log("betal != NULL -----------------------------------------------------");
		this->SetButtonBet(this->MyAI, *betal);
	}
	else
	{
		log("betal == NULL -----------------------------------------------------");
	}
	//    } catch (std::out_of_range &e) {
	//       // log("out_of_range exception: %s", e.what());
	//    } catch (std::exception &e) {
	//       // log("std::exception %s", e.what());
	//    } catch (...) {
	//       // log("unknown exception");
	//    }
}

void XiTo::event_EXT_EVENT_CHANGE_BALANCE_NOTIF() {
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<double> amf = this->param->GetDouble(EXT_VAL_AMOUNT_BALANCE);
	boost::shared_ptr<long> cbt = this->param->GetInt(EXT_VAL_CBT);

	this->LayerAvatars->StopAllTimer();

	if (uid != NULL && amf != NULL){
		string money = "0";
		int pos = this->GetPosPlayer(*uid);

		bool b_ret = true;
		if (*amf > 0) {
			money = "+";
		}
		else {
			money = "-";
			b_ret = false;
		}

		long ti = abs((long)(*amf));
		money += mUtils::convertMoneyEx(ti); // boost::to_string(ti);

		PlayerXiTo* p = this->GetPlayer(pos);
		if (p != NULL) {
			p->ShowMoney(money, b_ret);
			if (cbt != NULL && *cbt == 14){
				p->BoCuoc();
			}
		}
	}
}

void XiTo::event_EXT_EVENT_VICTORY_NOTIF() {
	this->LayerAvatars->StopAllTimer();
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> lc = this->param->GetUtfString(EXT_FIELD_LIST_CARD);
	boost::shared_ptr<long> vicType = this->param->GetInt(EXT_FIELD_VICTORY_TYPE);

	if (uid != NULL && lc != NULL && vicType != NULL) {

		this->LayerAvatars->StopAllTimer();
		this->VisibleButtonPlay();

		int pos = this->GetPosPlayer(*uid);
		PlayerXiTo* player = this->GetPlayer(pos);
		if (player != NULL){
			player->DisplayValueListCards(*lc);
			player->HideFrameBet();

			string s = XiToHelper::GetTypeListCards(*vicType);
			if (0 == vicType) {
				s = dataManager.GetSysString(230);
			}
			player->ShowVicType(s);
		}
	}
}
void XiTo::event_EXT_EVENT_OPEN_ALL_CARD_NOTIF() {
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> lc = this->param->GetUtfString(EXT_FIELD_LIST_CARD);
	if (uid != NULL && lc != NULL) {
		log("Mo tat ca la bai cua: %s la: %s", uid->c_str(), lc->c_str());
		int pos = this->GetPosPlayer(*uid);
		PlayerXiTo* player = this->GetPlayer(pos);
		if (player != NULL){
			player->SetIdListCards(*lc);
		}
	}
}
void XiTo::event_EXT_EVENT_USER_HAVE_SET_NOTIF() {
	log("Gia tri bo bai !");
	boost::shared_ptr<long> vttp = this->param->GetInt(EXT_FIELD_VICTORY_TYPE);
	if (vttp != NULL) {
		string s = XiToHelper::GetTypeListCards(*vttp);
		log("Values List Cards: %s", s.c_str());
		this->PlayerMe->ShowVicType(s);
	}
}

void XiTo::event_EXT_EVENT_RAISE_NOTIF() {
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<long> bet = this->param->GetInt(EXT_FIELD_BET_TYPE);
	boost::shared_ptr<double> betvl = this->param->GetDouble(EXT_FIELD_BET_VALUE);
	boost::shared_ptr<double> bettt = this->param->GetDouble(EXT_FIELD_BET_TOTAL);

	string Uid_ = "";
	long Bet_ = -1;
	double BetValue = 0;
	double BetTotal = 0;

	if (uid != NULL)
		Uid_ = uid->c_str();

	if (bet != NULL)
		Bet_ = *bet;

	if (betvl != NULL)
		BetValue = *betvl;

	if (bettt != NULL)
		BetTotal = *bettt;

	log("uid: %s typeBet: %ld betValue: %f betTotal %f", Uid_.c_str(), Bet_, BetValue, BetTotal);

	std::string BetTypeString = XiToHelper::GetTypeBet(Bet_);

	if (Bet_ != 0 && Bet_ != 2) {
		this->f_TotalCurrentRoomBet = BetTotal;
		this->SetTotalMoney(this->f_TotalCurrentRoomBet);
	}

	int PosUser = GetPosPlayer(Uid_);
	this->LayerAvatars->ShowTypeBetInPos(PosUser, BetTypeString);

	PlayerXiTo* player = this->GetPlayer(PosUser);
	if (player != NULL){
		player->SetBet(Bet_, BetValue);
		this->listBet.push_back(player);
	}
}

void XiTo::event_EXT_EVENT_OPEN_CARD_NOTIF() {
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<long> crdvl = param->GetInt(EXT_FIELD_CARD_ID);

	if (uid != NULL && crdvl != NULL) {

		if (0 == uid->compare(this->MyAI)) {
			if (this->getChildByTag(kTag_PopupOpenCard) != NULL) {
				this->removeChildByTag(kTag_PopupOpenCard);
			}
		}

		int pos = this->GetPosPlayer(*uid);
		PlayerXiTo* player = this->GetPlayer(pos);
		if (player != NULL){
			player->MoveDealCard((int)*crdvl);
		}
	}
}

void XiTo::event_EXT_EVENT_READY_RES() {
	boost::shared_ptr<long> rslt = param->GetInt(EXT_FIELD_RESULT);
	if (rslt != NULL) {
		if (7 == *rslt) {
			Chat *toast = Chat::create(dataManager.GetSysString(407), -1);
			this->addChild(toast);
		}
	}
}

void XiTo::event_EXT_EVENT_NEAD_RAISE_NOTIF() {
	this->HideButtonReady();
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> typeAllow = param->GetUtfString(EXT_FIELD_BET_TYPE_ALLOW);

	if (uid != NULL && typeAllow != NULL) {
		this->SetButtonBet(*uid, *typeAllow);
	}
}

void XiTo::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	this->GetRoomVar();
}

void XiTo::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	if (ptrEventParamValueUser == NULL) return;
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);

	if (ptrNotifiedUser == NULL) return;

	double money = 0;
	if (ptrNotifiedUser->GetVariable(EXT_FIELD_GAME_CHIP) != NULL)
		money = *ptrNotifiedUser->GetVariable(EXT_FIELD_GAME_CHIP)->GetDoubleValue();

	std::string uid = *ptrNotifiedUser->Name();

	log("Money of %s is %f", uid.c_str(), money);

	int pos = this->GetPosPlayer(uid);
	switch (pos) {
	case USER_ME:
		this->LayerAvatars->GetUserByPos(USER_ME)->setMoney(money);
		break;
	case USER_LEFT_BOT:
		this->LayerAvatars->GetUserByPos(USER_LEFT_BOT)->setMoney(money);
		break;
	case USER_LEFT_TOP:
		this->LayerAvatars->GetUserByPos(USER_LEFT_TOP)->setMoney(money);
		break;
	case USER_RIGHT_BOT:
		this->LayerAvatars->GetUserByPos(USER_RIGHT_BOT)->setMoney(money);
		break;
	case USER_RIGHT_TOP:
		this->LayerAvatars->GetUserByPos(USER_RIGHT_TOP)->setMoney(money);
		break;
	default:
		break;
	}
}

void XiTo::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> Players = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);

	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> p_Messages = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);
    if (dataManager.getAppConfig().is24h)
    {
        return;
    }
	//
	if (Players != NULL && p_Messages != NULL) {
		int pos = this->GetPosPlayer(*Players->Name());
		if (-1 == pos) return;
		this->LayerAvatars->ShowChatByPos(pos, *p_Messages);
	}
}

void XiTo::OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {

}

void XiTo::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	//   // log("User ExitRoom On Room");
	//    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	//    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	//    boost::shared_ptr<User> uUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	//    //
	//    if (uUser != NULL && uUser->IsItMe()) {
	//       // log("User Exit: %s", uUser->Name()->c_str());
	//        if (this->IsSpect)
	//            this->LayerButtons->SetIsGuess(true);
	//        this->LayerButtons->eventTouchBtnBack(NULL, Widget::TouchEventType::ENDED);
	//    }
}

void XiTo::EventListPlayers(string& listplayers) {
	this->LayerAvatars->ResetAll();

	this->IsSpect = true;
	vector<string> arrPlayers;

	if (listplayers != "") {
		arrPlayers = mUtils::splitString(listplayers, ';');
	}

	if (0 == arrPlayers.size()) return;

	// Set position users, isSpectator here
	this->SetMapPlayer(listplayers);

	this->LayerButtons->SetIsGuess(this->IsSpect);

	if (this->IsSpect || 1 == arrPlayers.size()) {
		this->HideButtonReady();
	}

	boost::shared_ptr<Room> LastJoinRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (LastJoinRoom == NULL) return;

	bool IsStartedGame = false;
	vector<string> s_ = mUtils::splitString(arrPlayers.at(0), ':');
	try {
		string st = s_.at(6);
		boost::algorithm::trim(st);
		if (st != "") {
			IsStartedGame = true;
		}
	}
	catch (...) {
		IsStartedGame = false;
	}

	if (IsStartedGame) {
		this->HideButtonReady();
	}

	bool IsBossRoom = false;
	vector<string>::iterator it = arrPlayers.begin();
	if (0 != (*it).compare("0")) {
		vector<string> Info = mUtils::splitString(*it, ':');
		try {
			if (0 == MyAI.compare(Info.at(1)))
				IsBossRoom = true;
		}
		catch (std::out_of_range& e) {
			log("%s", e.what());
		}
		catch (...){}
	}

	unsigned short CountPos = 0;
	for (vector<string>::iterator it = arrPlayers.begin(); it != arrPlayers.end(); ++it) {
		if (0 == (*it).compare("0")) continue;
		//        try {
		vector<string> n = mUtils::splitString(*it, ':');
		double Tien = 0;
		string Ten = n.at(1);
		string UrlAva = "";

		boost::shared_ptr<User> user = LastJoinRoom->GetUserByName(n.at(1));
		if (user != NULL) {
			boost::shared_ptr<double> money;
			boost::shared_ptr<string> name;
			boost::shared_ptr<string> url;
			if (user->GetVariable(EXT_VAL_ACCOUNT_NAME) != NULL)
				name = user->GetVariable("adn")->GetStringValue();
			if (user->GetVariable(EXT_FIELD_GAME_CHIP) != NULL)
				money = user->GetVariable(EXT_FIELD_GAME_CHIP)->GetDoubleValue();
			if (user->GetVariable(EXT_VAL_AMOUNT_AVATAR_LINK) != NULL)
				url = user->GetVariable(EXT_VAL_AMOUNT_AVATAR_LINK)->GetStringValue();
			if (name)
				Ten = *name;
			if (money)
				Tien = *money;
			if (url)
				UrlAva = *url;
		}

		int Pos = GetPosPlayer(n.at(1));
		bool IsReady = IsStartedGame ? false : n.at(2) == "1";

		boost::shared_ptr<XiToInfoPlayer> p(new XiToInfoPlayer(Pos, n.at(1), Ten, Tien, UrlAva, IsReady, 0 == CountPos));

		this->SetInfoAllPlayers(p);
		this->LayerAvatars->SetIsBoss(Pos, IsBossRoom);
		CountPos++;

		//        } catch (std::out_of_range &e) {
		//           // log("out_of_range exception: %s", e.what());
		//        } catch (std::exception &e) {
		//           // log("std::exception %s", e.what());
		//        } catch (...) {
		//           // log("unknown exception");
		//        }
	}
	arrPlayers.clear();
	this->LayerAvatars->checkSameIp();
}

void XiTo::SetInfoAllPlayers(boost::shared_ptr<XiToInfoPlayer> info) {
	if (-1 == info->pos)
		return;

	AvatarXiTo* avatar = this->LayerAvatars->GetUserByPos(info->pos);
	if (avatar != NULL){
		avatar->SetInfo(info);
	}

	//    if (info->pos == USER_ME && info->isBoss) {
	//        this->ResetTimeoutReady();
	//    }
}

void XiTo::EventReJoinGame(string& roomInfo) {

	std::vector<std::string> info;
	std::vector<std::string> ArrayUsers;
	std::vector<std::string> str;

	try {
		info = mUtils::splitString(roomInfo, '|');
		ArrayUsers = mUtils::splitString(info.at(3), ';');
		str = mUtils::splitString(ArrayUsers.at(0), ':');
	}
	catch (std::out_of_range& e) {
		log("exception %s", e.what());
		return;
	}

	//! arr[0] = 436_dautv1_1_979440.000000_1000.000000_4, -1, 12.
	//! Thông tin người chơi đầu tiên luôn có 7 trường, dựa vào đó để xác định
	//! có đang chơi hay không.
	if (str.size() >= 7) {

		this->HideButtonReady();

		vector<string> InfoPlayes;
		for (unsigned short i = 0; i < ArrayUsers.size(); ++i) {
			//            try {
			InfoPlayes = mUtils::splitString(ArrayUsers.at(i), ':');
			if (InfoPlayes.size() >= 7) {
				vector<string> L_Cards = mUtils::splitString(InfoPlayes.at(6), ',');
				//! Đếm số lượng card chưa được mở.
				//! chưa mở -> id = -1.
				int NotOpenCard = 0;

				for (unsigned short t = 0; t < L_Cards.size(); ++t) {
					//                        // Đưa những card có id = -1 về đầu dãy.
					//                        try {
					//                            if (atoi(L_Cards.at(t).c_str()) < 0) {
					//                                NotOpenCard++;
					//                                std::string StrTemp = L_Cards.at(0);
					//                                L_Cards.at(0) = L_Cards.at(t);
					//                                L_Cards.at(t) = StrTemp;
					//                            }
					//                        } catch (std::out_of_range &e) {
					//                           // log("out_of_range exception: %s", e.what());
					//                        } catch (std::exception &e) {
					//                           // log("ex: %s", e.what());
					//                        } catch (...) {
					//                        }
				}//end for t

				//! Tạo 1 list_card đúng thứ tự.
				std::string StrNewCards = InfoPlayes.at(6);
				//                    for (unsigned short t1 = 0; t1 < L_Cards.size(); ++t1) {
				//                        try {
				//                            StrNewCards += L_Cards.at(t1) + ",";
				//                        } catch(std::out_of_range& e) {

				//                        } catch(std::exception& e) {

				//                        }
				//                    }

				//! Vẽ lại bàn chơi.
				int PosUser = this->GetPosPlayer(InfoPlayes.at(1));
				PlayerXiTo* player = this->GetPlayer(PosUser);
				if (player != NULL){
					player->RestoreListCard(StrNewCards, 2 == NotOpenCard);
				}
			}//end if

			InfoPlayes.clear();
			//            } catch (std::out_of_range &e) {
			//               // log("out_of_range exception: %s", e.what());
			//            } catch (std::exception &e) {
			//               // log("std::exception %s", e.what());
			//            } catch (...) {
			//               // log("unknown exception");
			//            }// end try
		}//end for
	}//end if
}

void XiTo::Chia2LaBaiDauTien_1() {
	//    try {
	int solanchia = this->posgive.size() * 2;
	int pos = this->posgive.at(this->m_CountGive2CardFirst % this->posgive.size()).pos;
	PlayerXiTo* player = this->GetPlayer(pos);
	if (player != NULL){
		this->Chia2LaBaiDauTien_2(player, solanchia);
	}
	//    } catch (std::out_of_range &e) {
	//       // log("Chia2LaBaiDauTien_1 out_of_range exception: %s", e.what());
	//    } catch (std::exception &e) {
	//       // log("Chia2LaBaiDauTien_1 std::exception %s", e.what());
	//    } catch (...) {
	//       // log("Chia2LaBaiDauTien_1 unknown exception");
	//    }
}

void XiTo::Chia2LaBaiDauTien_2(PlayerXiTo* player, const int& totalchia){
    if (mUtils::isSoundOn())
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/deal_card.mp3");
	this->m_CountGive2CardFirst++;

	player->ReceiveCardFirst();

	if (this->m_CountGive2CardFirst < totalchia) {
		DelayTime* delay = DelayTime::create(m_Duration);
		CallFunc* func = CallFunc::create(CC_CALLBACK_0(XiTo::Chia2LaBaiDauTien_1, this));
		this->runAction(Sequence::create(delay, func, NULL));
	}
	else
	{
		int res = ((PlayerXiToIsMe*)PlayerMe)->DisplayTwoCard(this->s_TwoCardsFirst);
		if (res == 0)
		{
			DelayTime* delay = DelayTime::create(0.3);
			CallFunc* callfun = CallFunc::create(CC_CALLBACK_0(XiTo::SendOpenCard, this));
			this->runAction(Sequence::create(delay, callfun, NULL));
		}
		else
		{

		}
	}
}

void XiTo::SendOpenCard() {
   // log("2 la bai dau tien: %s", this->s_TwoCardsFirst.c_str());
    LayerOpenCard_Xito* layerOpen = LayerOpenCard_Xito::create(this->s_TwoCardsFirst, this->_pTimeTurnOne);
    this->addChild(layerOpen, z_order_layer_opencard, kTag_PopupOpenCard);
}

void XiTo::ChiaThem1LaBai_1() {
	this->HideAllFrameBet();
	int solanchia = this->posgive.size();
	//    try {
	Give give = this->posgive.at(m_CountGiveCardNext);
	std::string url = give.img;
	std::string allc = give.allc;
	int pos = give.pos;
	PlayerXiTo* player = this->GetPlayer(pos);
	if (player != NULL){
		this->ChiaThem1LaBai_2(player, allc, url, solanchia);
	}
	//    } catch (std::out_of_range &e) {
	//       // log("out_of_range exception: %s", e.what());
	//    } catch (std::exception &e) {
	//       // log("std::exception %s", e.what());
	//    } catch (...) {
	//       // log("unknown exception");
	//    }
}

void XiTo::ChiaThem1LaBai_2(PlayerXiTo* player, const string& allc, const string& url, const int& totalchia){

	this->m_CountGiveCardNext++;
    if (mUtils::isSoundOn())
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/deal_card.mp3");

	//Player nhan 1 la bai o day.
	player->ReceiveCard(allc, url);

	if (this->m_CountGiveCardNext < totalchia)
	{
		DelayTime* delay = DelayTime::create(this->m_Duration);
		CallFunc* func = CallFunc::create(CC_CALLBACK_0(XiTo::ChiaThem1LaBai_1, this));
		this->runAction(Sequence::create(delay, func, NULL));
	}
	else
	{
		this->m_CountGiveCardNext = 0;
	}
}

void XiTo::SetButtonBet(const string& uid, string& lsBet) {
	this->LayerAvatars->StopAllTimer();
	//0:0.0, 1:500.0, 7:26500.0, 3:6625.0, 4:13250.0
	/*GAME_TABLE_STATUS_BET_FOLD,
	 GAME_TABLE_STATUS_BET_RAISE,
	 GAME_TABLE_STATUS_BET_NONE,
	 GAME_TABLE_STATUS_BET_QUATER,
	 GAME_TABLE_STATUS_BET_HAFT,
	 GAME_TABLE_STATUS_BET_FOLLOW,
	 GAME_TABLE_STATUS_BET_DOUBLE,
	 GAME_TABLE_STATUS_BET_ALL,*/
	if (uid == this->MyAI) {

		this->HideButtonReady();

		vector<string> listBetType = mUtils::splitString(lsBet, ',');
		this->VisibleButtonPlay();

		Size _size = Size(148, 118);
		bool b_Display = false;

		for (int i = 0; i < listBetType.size(); ++i) {
			if (listBetType.at(i) == "")
				continue;

			vector<string> info = mUtils::splitString(listBetType.at(i), ':');

			try {
				int a = atoi(info.at(0).c_str());
				long _money = atol(info.at(1).c_str());
				for (int j = 0; j < this->pArrayButton1.size(); ++j) {
					try {
						ButtonXiTo* btn = this->pArrayButton1.at(j);
						if (btn != NULL && btn->getTag() == this->GetTagButton(a)) {
							btn->setAllow(true);
							bool b = _money > 999999999 ? true : false;
							string s = mUtils::convertMoneyEx(_money);
							btn->setMoney(s, b);

							if (!b_Display) {
								b_Display = true;
							}

							break;
						}
					}
					catch (...) {

					}
				}
			}
			catch (...) {

			}
		}

		int _count1 = b_Display ? 1 : 0;
		for (int i = 0; i < listBetType.size(); ++i) {
			try {
				int a = atoi(listBetType.at(i).c_str());
				for (int k = 0; k < this->pArrayButton2.size(); ++k) {
					try {
						ButtonXiTo* btn = this->pArrayButton2.at(k);
						if (btn != NULL && btn->getTag() == this->GetTagButton(a)) {
							//btn->setPosition(Vec2(WIDTH_DESIGN - 20 - (20 + _size.width) * _count1++, 20));
							btn->setAllow(true);
						}
						btn->setVisible(true);
					}
					catch (...) {

					}
				}
			}
			catch (...) {

			}
		}

		for (int k = 0; k < this->pArrayButton2.size(); ++k) {
			try {
				ButtonXiTo* btn = this->pArrayButton2.at(k);

				if (btn->isAllow()) {
					btn->setColor(Color3B::WHITE);
					btn->setTouchEnabled(true);
				}
				else {
					btn->setColor(Color3B::GRAY);
					btn->setTouchEnabled(false);
				}

			}
			catch (...) {

			}
		}

		if (b_Display) {
			this->btnToDisplay->setColor(Color3B::WHITE);
			this->btnToDisplay->setTouchEnabled(true);
			this->btnToDisplay->setVisible(true);
		}
		else {
			this->btnToDisplay->setColor(Color3B::GRAY);
			this->btnToDisplay->setTouchEnabled(false);
			this->btnToDisplay->setVisible(true);
		}

		this->LayerAvatars->GetUserByPos(USER_ME)->startTimer(this->StepTimeout);
	}
	else {
		this->VisibleButtonPlay();
		int pos = this->GetPosPlayer(uid);
		if (pos == -1)
			return;
		this->LayerAvatars->GetUserByPos(pos)->startTimer(this->StepTimeout);
	}
}

void XiTo::ActionEndGame() {

	this->PlayerMe->RemoveAllCards();
	this->PlayerLeftBottom->RemoveAllCards();
	this->PlayerLeftTop->RemoveAllCards();
	this->PlayerRightBottom->RemoveAllCards();
	this->PlayerRightTop->RemoveAllCards();

	this->spFrameBetTotal->setVisible(false);
	this->removeChildByTag(kTag_PopupOpenCard);

	this->layerLabelVictype->removeAllChildrenWithCleanup(true);
	if (this->getChildByTag(333) != NULL)
		this->removeChildByTag(333);

	//! Kết thúc một ván thì reset lại các biến để thiết lập ván chơi mới
	this->m_CountGive2CardFirst = 0;
	this->CountDownToReady = (unsigned short)this->_pTimerReady;

	this->isRegistLeave = false;

	this->TimerToReady->setString(boost::to_string(this->CountDownToReady));

	Director::getInstance()->getScheduler()->unschedule(schedule_selector(XiTo::DelayToEndGame),this);
	this->m_TimeDelayToEndGame = 0;
}

void XiTo::VisibleButtonPlay() {
	//this->bgButton->setVisible(false);
	this->btnToDisplay->setVisible(false);

	for (int i = 0; i < this->pArrayButton1.size(); ++i) {
		try {
			ButtonXiTo* btn = this->pArrayButton1.at(i);
			btn->resetState();
		}
		catch (...) {

		}
	}
	for (int i = 0; i < this->pArrayButton2.size(); ++i) {
		try {
			ButtonXiTo* btn = this->pArrayButton2.at(i);
			btn->resetState();
			btn->setColor(Color3B::WHITE);
			btn->setTouchEnabled(true);
		}
		catch (...) {

		}
	}
}

void XiTo::OnBtnReadyClick(Ref *sender, ui::Widget::TouchEventType  pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		Button* btn = (Button*)sender;
		if (btn) {
			switch (btn->getTag()) {
			case DTAG_BTN_READY: {
									 this->ActionEndGame();
									 boost::shared_ptr<ISFSObject> parameter(new SFSObject());
									 boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
									 if (lastRoom == NULL)
										 return;
									 boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
									 GameServer::getSingleton().Send(request);
			}
				break;
			case DTAG_BTN_JOIN:
				break;
			case DTAG_BTN_STANDUP:
				break;
			default:
				break;
			}
		}
	}
}

void XiTo::SendBetNTF(const int& typeBet) {
	boost::shared_ptr<ISFSObject> param(new SFSObject());
	switch (typeBet) {
	case GAME_TABLE_STATUS_BET_FOLD:
		param->PutInt(EXT_FIELD_BET_TYPE, GAME_TABLE_STATUS_BET_FOLD);
		break;
	case GAME_TABLE_STATUS_BET_RAISE:
		param->PutInt(EXT_FIELD_BET_TYPE, GAME_TABLE_STATUS_BET_RAISE);
		break;
	case GAME_TABLE_STATUS_BET_NONE:
		param->PutInt(EXT_FIELD_BET_TYPE, GAME_TABLE_STATUS_BET_NONE);
		break;
	case GAME_TABLE_STATUS_BET_QUATER:
		param->PutInt(EXT_FIELD_BET_TYPE, GAME_TABLE_STATUS_BET_QUATER);
		break;
	case GAME_TABLE_STATUS_BET_HAFT:
		param->PutInt(EXT_FIELD_BET_TYPE, GAME_TABLE_STATUS_BET_HAFT);
		break;
	case GAME_TABLE_STATUS_BET_FOLLOW:
		param->PutInt(EXT_FIELD_BET_TYPE, GAME_TABLE_STATUS_BET_FOLLOW);
		break;
	case GAME_TABLE_STATUS_BET_DOUBLE:
		param->PutInt(EXT_FIELD_BET_TYPE, GAME_TABLE_STATUS_BET_DOUBLE);
		break;
	case GAME_TABLE_STATUS_BET_ALL:
		param->PutInt(EXT_FIELD_BET_TYPE, GAME_TABLE_STATUS_BET_ALL);
		break;
	}

	log("so luong bet truoc do: %ld", this->listBet.size());
	log("Tinh tien ra: %f --------------------------------------------------------------------------------", this->CalcMoneyBet(typeBet));

	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL) return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_RAISE_REQ, param, lastRoom));
	GameServer::getSingleton().Send(request);
	this->VisibleButtonPlay();
}

double XiTo::CalcMoneyBet(const int &type)
{
	if (type == GAME_TABLE_STATUS_BET_FOLD || type == GAME_TABLE_STATUS_BET_NONE){
		return 0;
	}
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL)
		return 0;

	boost::shared_ptr<UserVariable> var;
	if (SceneManager::getSingleton().getCurrRoomType() == 0){

		var = myself->GetVariable("amf");
	}
	else{
		var = myself->GetVariable("amfs");

	}
	if (var == NULL)
		return 0;

	double mymoney = *var->GetDoubleValue();

	if (mymoney < this->f_RoomBet)
		return mymoney;

	double current_me = this->PlayerMe->GetCurrentValueBet();
	double prev_player_bet = 0;

	for (int i = this->listBet.size() - 1; i >= 0; --i){
		PlayerXiTo* player = this->listBet.at(i);
		if (player->GetCurrentTypeBet() != GAME_TABLE_STATUS_BET_FOLD && player->GetCurrentTypeBet() != GAME_TABLE_STATUS_BET_NONE){
			prev_player_bet = player->GetCurrentValueBet();
			break;
		}
	}

	log("So tien to nguoi truoc: %f", prev_player_bet);

	double Add = 0;
	double Ret = 0;

	switch (type){
	case GAME_TABLE_STATUS_BET_RAISE:{
										 Add = this->f_RoomBet;
										 Ret = this->f_RoomBet;
										 break;
	}

	case GAME_TABLE_STATUS_BET_QUATER:{
										  //Tố 1/4: mức cược thêm = theo bằng người trươc + tổng tiền sau khi đã theo / 4
										  Add = prev_player_bet - current_me;
										  Ret = Add + this->f_TotalCurrentRoomBet;
										  Ret = (Add + Ret / 4);
										  break;
	}

	case GAME_TABLE_STATUS_BET_HAFT:{
										Add = prev_player_bet - current_me;
										Ret = Add + this->f_TotalCurrentRoomBet;
										Ret = (Add + Ret / 2);
										break;
	}

	case GAME_TABLE_STATUS_BET_FOLLOW:{
										  Add = prev_player_bet - current_me;
										  Ret = Add;
										  break;
	}

	case GAME_TABLE_STATUS_BET_DOUBLE:{
										  double _cur = this->f_TotalCurrentRoomBet - this->f_CurrentTurnBet;
										  //Add = this->f_TotalCurrentRoomBet * 2;
										  Add = _cur * 2;
										  Ret = Add;
										  break;
	}

	case GAME_TABLE_STATUS_BET_ALL:{
									   Add = prev_player_bet - current_me;
									   Ret = Add + this->f_TotalCurrentRoomBet;
									   Ret += Add;
									   break;
	}

	}

	if (mymoney < Ret) {
		Ret = mymoney;
	}

	//((PlayerXiToIsMe*)this->PlayerMe)->SetBetBeforeServer(Ret);
	//float m = mymoney - Add;
	//this->LayerAvatars->GetUserByPos(USER_ME)->setMoney(m);

	//    Ret = Ret - current_me;
	//    if(Ret < 0)
	//      Ret = 0;

	return Ret;
}

int XiTo::GetTagButton(const int& iTag) {
	switch (iTag) {
	case GAME_TABLE_STATUS_BET_FOLD:
		return DTAG_BTN_FOLD;
	case GAME_TABLE_STATUS_BET_RAISE:
		return DTAG_BTN_BET;
	case GAME_TABLE_STATUS_BET_NONE:
		return DTAG_BTN_GIVE;
	case GAME_TABLE_STATUS_BET_QUATER:
		return DTAG_BTN_BET_QUATAR;
	case GAME_TABLE_STATUS_BET_HAFT:
		return DTAG_BTN_BET_HAFT;
	case GAME_TABLE_STATUS_BET_FOLLOW:
		return DTAG_BTN_FOLLOW;
	case GAME_TABLE_STATUS_BET_DOUBLE:
		return DTAG_BTN_BET_X2;
	case GAME_TABLE_STATUS_BET_ALL:
		return DTAG_BTN_BET_ALL;
	default:
		return -1;
	}
}

void XiTo::HideButtonReady()
{
	this->btnReady->setVisible(false);
	this->TimerToReady->setVisible(false);
	this->btnDis->setVisible(false);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(XiTo::TimerToReadyFunc),this);
}

void XiTo::DelayToEndGame(float dt) {
	this->m_TimeDelayToEndGame++;
	if (10 == this->m_TimeDelayToEndGame) {
		ActionEndGame();
	}
}

void XiTo::TimerToReadyFunc(float dt) {
	this->CountDownToReady--;
	if (this->CountDownToReady > 0) {
		this->TimerToReady->setVisible(true);
		this->TimerToReady->setString(boost::to_string(this->CountDownToReady));
	}
	else {
		this->TimerToReady->setVisible(false);
		this->ResetTimeoutReady();
	}
}

void XiTo::ResetTimeoutReady() {
	this->TimerToReady->setVisible(false);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(XiTo::TimerToReadyFunc),this);
	this->CountDownToReady = (unsigned short)this->_pTimerReady;
	this->TimerToReady->setString(boost::to_string(this->CountDownToReady));
	this->removeChildByTag(z_tag_popup_chontien);
}

int XiTo::GetPosPlayer(const std::string& uid) {
	return this->mapPlayers.find(uid) != this->mapPlayers.end() ? this->mapPlayers[uid] : -1;
}

PlayerXiTo *XiTo::GetPlayer(const int &pos)
{
	switch (pos){
	case USER_ME:
		return this->PlayerMe;
	case USER_RIGHT_BOT:
		return this->PlayerRightBottom;
	case USER_RIGHT_TOP:
		return this->PlayerRightTop;
	case USER_LEFT_TOP:
		return this->PlayerLeftTop;
	case USER_LEFT_BOT:
		return this->PlayerLeftBottom;
	}
	return NULL;
}

void XiTo::updateEvent(float dt) {
	if (this->mThreadQueueMsg->isQueueEmpty()) return;

	if (this->PlayerMe->CheckCardRunning()
		|| this->PlayerLeftTop->CheckCardRunning()
		|| this->PlayerLeftBottom->CheckCardRunning()
		|| this->PlayerRightTop->CheckCardRunning()
		|| this->PlayerRightBottom->CheckCardRunning()){
		return;
	}

	mThreadQueueMsg->run();
}

void XiTo::SetMapPlayer(string& listplayers) {
	int vt = -1;
	vector<string> arrPlayers;

	if (listplayers != "") {
		arrPlayers = mUtils::splitString(listplayers, ';');
	}

	if (0 == arrPlayers.size()) return;

	std::string IdPlayers = "";
	for (unsigned int i = 0; i < arrPlayers.size(); ++i) {
		//        try {
		IdPlayers = mUtils::splitString(arrPlayers.at(i), ':').at(1);
		if (0 == this->MyAI.compare(IdPlayers)) {
			vt = i;
			this->IsSpect = false;
			break;
		}
		//        } catch (std::out_of_range &e) {
		//           // log("SetMapPlayer find vt out_of_range exception: %s", e.what());
		//        } catch (std::exception &e) {
		//           // log("SetMapPlayer find vt std::exception %s", e.what());
		//        } catch (...) {
		//           // log("SetMapPlayer find vt unknown exception");
		//        }
	}

	if (this->IsSpect){
		vt = 0;
	}

	vector<string> player;
	this->mapPlayers.clear();

	for (unsigned int i = 0; i < arrPlayers.size(); ++i) {
		if (0 == arrPlayers.at(i).compare("")) continue;
		//        try {
		player = mUtils::splitString(arrPlayers.at(i), ':');
		if (i == vt)
		{
			this->mapPlayers[player.at(1)] = USER_ME;
			this->PlayerMe->SetAI(player.at(1));
		}
		else if (i == (vt + 1) % 5)
		{
			this->mapPlayers[player.at(1)] = USER_RIGHT_BOT;
			this->PlayerRightBottom->SetAI(player.at(1));
		}
		else if (i == (vt + 2) % 5)
		{
			this->mapPlayers[player.at(1)] = USER_RIGHT_TOP;
			this->PlayerRightTop->SetAI(player.at(1));
		}
		else if (i == (vt + 3) % 5)
		{
			this->mapPlayers[player.at(1)] = USER_LEFT_TOP;
			this->PlayerLeftTop->SetAI(player.at(1));
		}
		else if (i == (vt + 4) % 5)
		{
			this->mapPlayers[player.at(1)] = USER_LEFT_BOT;
			this->PlayerLeftBottom->SetAI(player.at(1));
		}
		player.clear();
		//        } catch (std::out_of_range &e) {
		//           // log("Set map out_of_range exception: %s", e.what());
		//        } catch (std::exception &e) {
		//           // log("Set map std::exception %s", e.what());
		//        } catch (...) {
		//           // log("Set map unknown exception");
		//        }
	}
}

void XiTo::GetRoomVar(){
	this->_pTimerReady = 10;
	this->_pTimeTurnOne = TIMER_CHOOSE_CARD;

	//opencardtime, playtime, endtime
	boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (room != NULL) {
		boost::shared_ptr<RoomVariable> opencardtime = room->GetVariable("opencardtime");
		if (opencardtime != NULL) {
			this->_pTimeTurnOne = (*opencardtime->GetDoubleValue()) / 1000;
		}

		boost::shared_ptr<RoomVariable> playtime = room->GetVariable("playtime");
		if (playtime != NULL) {
			this->StepTimeout = (*playtime->GetDoubleValue()) / 1000;
		}

		boost::shared_ptr<RoomVariable> endtime = room->GetVariable("endtime");
		if (endtime != NULL) {
			this->_pTimerReady = (*endtime->GetDoubleValue()) / 1000;
		}
	}
	this->CountDownToReady = this->_pTimerReady;
}

void XiTo::SetTotalMoney(const double& money){
	if (money > 1000000) {
		this->lblBetTotal->setString((dataManager.GetSysString(659)+" " + formatMoneySymbol(money)));
	}
	else {
		this->lblBetTotal->setString((dataManager.GetSysString(659)+" " + mUtils::convertMoneyEx((long)money)));
	}
	this->spFrameBetTotal->setVisible(true);
}
