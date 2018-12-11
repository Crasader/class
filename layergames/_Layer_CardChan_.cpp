#include "_Layer_CardChan_.h"
#include "mUtils.h"
#include "AllData.h"
#include "GameServer.h"
#include "Requests/ExtensionRequest.h"
#include "SimpleAudioEngine.h"
#include "data_manager.h"
#include "ButtonChan.h"
#include "BoBaiChan.h"
#include "PlayerChanIsMe.h"
#include "PlayerChanNormal.h"
#include "../SceneManager.h"
#include "PhomMessDef.h"
#define KTAG_BTN_XEMNOC 123
#define KTAG_LAYER_GIVE 124

using namespace Sfs2X;

_Layer_CardChan_::_Layer_CardChan_()
:LblCountNoc(NULL),
LayerAllCards(NULL),
DiaNoc(NULL),
LayerCardsOnHandWinner(NULL),
LayerCardsInNoc(NULL),
sizeScreen(Size(0, 0)),
disTouchBegan(Size(0, 0)),
oldPos(Vec2::ZERO),
m_callback(NULL),
m_callbackListener(NULL),
Toast(NULL),
ListCardsNoc(""),
BeginX(0),
BeginY(0),
scaleApp(0),
startLeft(0),
startTop(0),
IsDragDrop(false),
FlagCallback(false),
hasUSucsses(false),
FlagAnBao(false),
Card_Action_Duration(0.2) {
	this->arrCardsResult.clear();
	this->arrCardsNoc.clear();
	this->arrCardU.clear();
}

_Layer_CardChan_::~_Layer_CardChan_() {

	delete this->PlayerMe;
	delete this->PlayerLeft;
	delete this->PlayerRight;
	delete this->PlayerTop;
	delete this->PlayerBottom;

	BoBaiChan::Destroy();

	log("Layer card Chan Deconstructor !");
}

bool _Layer_CardChan_::init() {
	if (!CCLayer::init())
		return false;

	this->setAnchorPoint(Vec2(0, 0));
	this->setPosition(Vec2(0, 0));

	this->sizeScreen = Director::getInstance()->getVisibleSize();
	float VarX = sizeScreen.width / WIDTH_DESIGN;
	float VarY = sizeScreen.height / HEIGHT_DESIGN;
	float scaleMin = (VarX < VarY) ? VarX : VarY;
	scaleApp = scaleMin;

	startLeft = (sizeScreen.width - WIDTH_DESIGN) / 2;
	startTop = (sizeScreen.height - HEIGHT_DESIGN) / 2;

	VarX = WIDTH_DESIGN >> 1;
	VarY = HEIGHT_DESIGN >> 1;
	this->ListCardsNoc = "";

	Size SizeAdd = ChanUtils::getSizePos();

	this->BeginX = WIDTH_DESIGN >> 1;
	this->BeginY = 250.0 - SizeAdd.height;

	// Vi tri cac la bai cua tri
	// PlayerMe
	this->PlayerMe = new PlayerChanIsMe();
	this->PlayerMe->SetPosCardCuaChi(VarX + 370, 122 - SizeAdd.height);
	this->PlayerMe->SetPosCardDuoiTay(590, 140 - SizeAdd.height);
	this->PlayerMe->SetPosBeginVisible(this->BeginX, this->BeginY);
	this->PlayerMe->SetSigne(1, -1);

	// Player Left
	this->PlayerLeft = new PlayerChanNormal();
	this->PlayerLeft->SetPosCardCuaChi(200 + W_CARD - SizeAdd.width, 425);
	this->PlayerLeft->SetPosCardDuoiTay(60 - SizeAdd.width, 720);
	this->PlayerLeft->SetPosBeginVisible(40 - SizeAdd.width, 455);
	this->PlayerLeft->SetSigne(1, 1);

	//Player Right
	this->PlayerRight = new PlayerChanNormal();
	this->PlayerRight->SetPosCardCuaChi(1720 - W_CARD + SizeAdd.width, 425);
	this->PlayerRight->SetPosCardDuoiTay(WIDTH_DESIGN - 50 + SizeAdd.width, 720);
	this->PlayerRight->SetPosBeginVisible(WIDTH_DESIGN - 10 - W_CARD / 2 + SizeAdd.width, VarY - H_CARD / 2);
	this->PlayerRight->SetSigne(-1, -1);

	//Player Top
	this->PlayerTop = new PlayerChanNormal();
	this->PlayerTop->SetPosCardCuaChi(830, 728 + SizeAdd.height);
	this->PlayerTop->SetPosCardDuoiTay(1090, HEIGHT_DESIGN - H_CARD - 20 + SizeAdd.height);
	this->PlayerTop->SetPosBeginVisible(VarX, HEIGHT_DESIGN - H_CARD - 20 + SizeAdd.height);
	this->PlayerTop->SetSigne(-1, 1);

	//Player Bottom (For Guess)
	this->PlayerBottom = new PlayerChanNormal();
	this->PlayerBottom->SetPosCardCuaChi(WIDTH_DESIGN / 2 + 150, 122 - SizeAdd.height);
	this->PlayerBottom->SetPosCardDuoiTay(WIDTH_DESIGN / 2 - 150, 140 - SizeAdd.height);
	this->PlayerBottom->SetPosBeginVisible(VarX, 130 - SizeAdd.height);
	this->PlayerBottom->SetSigne(1, -1);

	// Layer Chứa các lá bài đánh
	this->LayerAllCards = Layer::create();
	this->LayerAllCards->setAnchorPoint(Vec2::ZERO);
	this->LayerAllCards->setPosition(Vec2::ZERO);
	this->addChild(this->LayerAllCards);

	// Tạo tất cả các lá bài
	this->CreateAllCards();

	// Layer Chứa các lá bài trên tay người Ù
	this->LayerCardsOnHandWinner = Layer::create();
	this->LayerCardsOnHandWinner->setAnchorPoint(Vec2::ZERO);
	this->LayerCardsOnHandWinner->setPosition(Vec2::ZERO);
	this->LayerCardsOnHandWinner->setLocalZOrder(0);
	this->addChild(this->LayerCardsOnHandWinner);

	// Layer chứa các lá bài còn trong nọc, và Button Xem Nọc
	this->LayerCardsInNoc = Layer::create();
	this->LayerCardsInNoc->setAnchorPoint(Vec2::ZERO);
	this->LayerCardsInNoc->setPosition(Vec2::ZERO);
	this->addChild(this->LayerCardsInNoc);

	this->DiaNoc = Button::create();
	this->DiaNoc->loadTextures("bat_noc.png", "bat_noc.png", "");
	this->DiaNoc->setPosition(Vec2(VarX + 10, VarY + 85));
	this->DiaNoc->setVisible(false);
	this->LayerCardsInNoc->addChild(this->DiaNoc);

	this->LblCountNoc = Label::createWithBMFont(RobotoFnt, "");
	this->LblCountNoc->setColor(Color3B::YELLOW);
	this->LblCountNoc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->LblCountNoc->setScale(1.15);
	this->LblCountNoc->setPosition(Vec2(this->DiaNoc->getContentSize().width - 35, this->DiaNoc->getContentSize().height - 45));
	this->DiaNoc->addChild(this->LblCountNoc);



	Button* BtnXemNoc = Button::create();
	BtnXemNoc->setTag(KTAG_BTN_XEMNOC);
	BtnXemNoc->loadTextures("noc.png", "noc.png", "", Widget::TextureResType::PLIST);
	BtnXemNoc->setPosition(Vec2(VarX, VarY + 75));
	BtnXemNoc->setVisible(false);
	BtnXemNoc->addTouchEventListener(CC_CALLBACK_2(_Layer_CardChan_::OnXemNocClick, this));

	Label* LblXemNoc = Label::createWithTTF(dataManager.GetSysString(624),kBoldFont, 30);
	LblXemNoc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	LblXemNoc->setColor(Color3B::RED);
	LblXemNoc->setScale(1.5);
	LblXemNoc->setPosition(Vec2(BtnXemNoc->getContentSize().width / 2, BtnXemNoc->getContentSize().height / 2 + 5));
	BtnXemNoc->addChild(LblXemNoc);
	this->LayerCardsInNoc->addChild(BtnXemNoc);

	//Button Xem noc

	if (this->LayerAllCards->getLocalZOrder() < this->LayerCardsInNoc->getLocalZOrder()) {
		int Temp = this->LayerAllCards->getLocalZOrder();
		this->LayerAllCards->setLocalZOrder(this->LayerCardsInNoc->getLocalZOrder());
		this->LayerCardsInNoc->setLocalZOrder(Temp);
	}

	// Init Callback
	this->m_callback = NULL;
	this->m_callbackListener = NULL;

	return true;
}

void _Layer_CardChan_::CreateAllCards() {
	BoBaiChan::Destroy();
	std::vector<CardChan*> arraycards = BoBaiChan::GetInstance()->GetArrayCards();
	for (int i = 0; i < arraycards.size(); ++i){
		this->LayerAllCards->addChild(arraycards.at(i));
	}
}

void _Layer_CardChan_::ResetAllCards() {

	this->PlayerMe->Reset();
	this->PlayerLeft->Reset();
	this->PlayerRight->Reset();
	this->PlayerTop->Reset();
	this->PlayerBottom->Reset();

	this->ListCardsNoc = "";

	BoBaiChan::GetInstance()->ResetAllCards();

	this->FlagCallback = false;
	this->IsDragDrop = false;

	this->LastCardObj.reset();

	this->LayerCardsOnHandWinner->setPosition(Vec2::ZERO);
	this->LayerCardsInNoc->getChildByTag(KTAG_BTN_XEMNOC)->setVisible(false);

	this->DeleteListCards(this->arrCardsNoc);
	this->DeleteListCards(this->arrCardsResult);

	this->LblCountNoc->setString("");
	this->DiaNoc->setVisible(false);

	this->arrCardU.clear();
	this->hasUSucsses = false;
}

void _Layer_CardChan_::CreateOrRestoreListCards(const int& pos, string& listCards) {
	vector<string> arrList = mUtils::splitString(listCards, '/');
	if (arrList.size() < 3)
		return;

	PlayerChan* player = this->GetPlayerChan(pos);
	if (player != NULL) {
		if (pos == kUserMe){
			this->SetMyListCards(listCards);
			if (this->m_callback && this->m_callbackListener && !this->FlagCallback) {
				(this->m_callback->*this->m_callbackListener)(this);
			}
		}
		else {

		}
		player->SetPlayerListCards(listCards);
	}
}

void _Layer_CardChan_::SetIsSpect(std::map<std::string, int> mapPlayers){

	vector<PlayerChan*> arrays;

	this->PlayerMe->ClearAIOwner();
	this->PlayerLeft->ClearAIOwner();
	this->PlayerRight->ClearAIOwner();
	this->PlayerTop->ClearAIOwner();
	this->PlayerBottom->ClearAIOwner();

	std::map<std::string, int>::iterator pos;
	for (pos = mapPlayers.begin(); pos != mapPlayers.end(); ++pos)
	{
		switch (pos->second){
		case kUserMe:
			this->PlayerMe->SetAI(pos->first);
			this->PlayerMe->SetIsPlayer(true);
			arrays.push_back(this->PlayerMe);
			break;

		case kUserRight:
			this->PlayerRight->SetAI(pos->first);
			this->PlayerRight->SetIsPlayer(true);
			arrays.push_back(this->PlayerRight);
			break;

		case kUserTop:
			this->PlayerTop->SetAI(pos->first);
			this->PlayerTop->SetIsPlayer(true);
			arrays.push_back(this->PlayerTop);
			break;

		case kUserLeft:
			this->PlayerLeft->SetAI(pos->first);
			this->PlayerLeft->SetIsPlayer(true);
			arrays.push_back(this->PlayerLeft);
			break;

		case kUserBot:
			this->PlayerBottom->SetAI(pos->first);
			this->PlayerBottom->SetIsPlayer(true);
			arrays.push_back(this->PlayerBottom);
			break;
		}
		cout << "Tên: " << pos->first << "\t" << "thứ tự : " << pos->second << endl;
	}

	log("arrays.size() = %ld", arrays.size());


	if (this->PlayerMe->GetIsPlayer())
	{
		log("I'm Player !");
		this->PlayerMe->SetOtherPlayer(this->PlayerRight, this->PlayerTop, this->PlayerLeft);
		this->PlayerRight->SetOtherPlayer(this->PlayerTop, this->PlayerLeft, this->PlayerMe);
		this->PlayerTop->SetOtherPlayer(this->PlayerLeft, this->PlayerMe, this->PlayerRight);
		this->PlayerLeft->SetOtherPlayer(this->PlayerMe, this->PlayerRight, this->PlayerTop);
	}
	else
	{
		log("I'm spectator !");
		this->PlayerBottom->SetOtherPlayer(this->PlayerRight, this->PlayerTop, this->PlayerLeft);
		this->PlayerRight->SetOtherPlayer(this->PlayerTop, this->PlayerLeft, this->PlayerBottom);
		this->PlayerTop->SetOtherPlayer(this->PlayerLeft, this->PlayerBottom, this->PlayerRight);
		this->PlayerLeft->SetOtherPlayer(this->PlayerBottom, this->PlayerRight, this->PlayerTop);
	}

	for (int i = 0; i < arrays.size(); ++i)
	{
		PlayerChan* player = arrays.at(i);
		log("player.aI = %s", player->GetAI().c_str());
		if (!player->GetPlayerLeft()->GetIsPlayer())
		{
			player->SetPlayerLeft(player->GetPlayerTop());
		}
		if (!player->GetPlayerRight()->GetIsPlayer())
		{
			player->SetPlayerRight(player->GetPlayerTop());
		}
		log("player : %s, left : %s, top : %s, right : %s ", player->GetAI().c_str(), player->GetPlayerLeft()->GetAI().c_str(), player->GetPlayerTop()->GetAI().c_str(), player->GetPlayerRight()->GetAI().c_str());
	}
}

void _Layer_CardChan_::SetMyListCards(string& listcards) {
	this->DeleteListCards(this->arrCardsResult);
	((PlayerChanIsMe*)this->PlayerMe)->SetListCardFirst(listcards);

	int res = this->PlayerMe->SetListCards(listcards);
	if (res == 1){
		this->FlagCallback = true;
	}
	else if (res == 0){
		this->DiaNoc->setVisible(true);
	}
	else if (res == -1){

	}
}

void _Layer_CardChan_::RemoveCardResult(){
	this->DeleteListCards(this->arrCardsResult);
}

void _Layer_CardChan_::SetPlayerListCards(const int& pos, string& lstCards){
	PlayerChan* player = this->GetPlayerChan(pos);
	if (player != NULL){
		player->SetPlayerListCards(lstCards, this->hasUSucsses);
	}
}

void _Layer_CardChan_::TakeCards(const CardOrg& card) {
	card.show();

	this->LastCardObj = card;

	switch (card.crdorg) {
	case CARD_ORIGINATION_CHIA_BAI:
		break;
	case CARD_ORIGINATION_BOC_NOC:
	{
									// log("Bốc nọc");
									 if (card.tpos == kUserMe) {
										 if (!this->FlagAnBao){
											 ((PlayerChanIsMe*)this->PlayerMe)->BeforeBocBai();
										 }
										 this->PlayerMe->RefreshListCards();
									 }
									 this->Action_BocNoc(card);
	}
		break;

	case CARD_ORIGINATION_AN_CUA_TREN:
		this->Action_AnCuaTren(card);
		break;

	case CARD_ORIGINATION_AN_CUA_TRI:
		this->Action_AnCuaTri(card);
		break;

	case CARD_ORIGINATION_CHIU:
		log("Chíu");
		Action_ChiuBai(card);
		break;

	case CARD_ORIGINATION_TRA_CUA:
		log("Trả cửa");
		this->Action_TraCua(card);
		break;

	case CARD_ORIGINATION_BY_DISCARD:
		log("Đánh bài");
		this->PlaySounds("Danh.mp3");
		this->ActionDiscards(card);
		break;

	case CARD_ORIGINATION_BY_DUOI:
		//playSounds("duoi.mp3");
		if (card.tpos == kUserMe) {
			if (!this->FlagAnBao){
				((PlayerChanIsMe*)this->PlayerMe)->BeforeBocBai();
			}
			this->PlayerMe->RefreshListCards();
		}
		log("Dưới");
		break;

	case CARD_ORIGINATION_BY_TRANSFER_TREN_2_DUOI:
		log("Chuyển bài");
		this->Action_ChuyenBai(card);
		break;

	case CARD_ORIGINATION_AN_BAI_U:
		this->Action_An_U(card);
		break;

	default:
		break;
	}
}

void _Layer_CardChan_::Action_BocNoc(const CardOrg& card) {
	card.show();
	PlayerChan* player = this->GetPlayerChan(card.tpos);
	if (player == NULL) return;

	CardChan *pCard = BoBaiChan::GetInstance()->PopCard();
	pCard->loadTexture(ChanUtils::FindTypeCard(card.cardnu, card.cardsu), Widget::TextureResType::PLIST);
	pCard->SetNumber(card.cardnu);
	pCard->SetSuite(card.cardsu);
	pCard->SetSizeCard(W_CARD, H_CARD);
	pCard->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
	pCard->setColor(Color3B(200, 255, 255));
	pCard->SetIsDraw(true);
	pCard->setVisible(true);

	this->PlaySounds("Boc.mp3");
	MoveTo* moveTo = MoveTo::create(Card_Action_Duration, player->PosCardCuaChi());
	CallFunc* callfun = CallFunc::create(CC_CALLBACK_0(_Layer_CardChan_::AddCard_toCuaTri, this, pCard, card.tpos));
	DelayTime* delay = DelayTime::create(Card_Action_Duration);

	player->AddCardToCuaChi(pCard);
	pCard->runAction(Sequence::create(delay, moveTo, callfun, NULL));
}

void _Layer_CardChan_::AddCard_toCuaTri(Ref* sender, int data) {
	PlayerChan* player = this->GetPlayerChan(data);
	if (player != NULL){
		player->ResortCardCuaChi(NULL, player);
	}
}

void _Layer_CardChan_::Action_AnCuaTren(const CardOrg& card) {
	log("Ăn cửa trên");
	this->PlaySounds("An.mp3");

	BoBaiChan::GetInstance()->RemoveElemsChiu(card);

	PlayerChan* player = this->GetPlayerChan(card.tpos);
	if (player == NULL) return;
	player->ActionAnCuaTrenByServer(card);
}

void _Layer_CardChan_::Action_AnCuaTri(const CardOrg& card) {

	log("Ăn cửa trì");
	this->PlaySounds("An.mp3");

	PlayerChan* player = this->GetPlayerChan(card.tpos);
	if (player == NULL) return;
	player->ActionAnCuaChiByServer(card);
}

void _Layer_CardChan_::Action_ChiuBai(const CardOrg& card) {
	PlayerChan* player = this->GetPlayerChan(card.tpos);
	if (player != NULL){
		player->ActionChiuBaiByServer(card);
	}
}

void _Layer_CardChan_::Action_TraCua(const CardOrg& card) {
	PlayerChan* player = this->GetPlayerChan(card.fpos);
	assert(player != NULL);
	if (player != NULL) {
		player->ActionTraCuaByServer(card);
	}
}

void _Layer_CardChan_::ActionDiscards(const CardOrg& card) {
	PlayerChan* player = this->GetPlayerChan(card.tpos);
	if (player != NULL)
	{
		player->ActionDiscardByServer(card);
	}
}

void _Layer_CardChan_::Action_ChuyenBai(const CardOrg& card) {

	PlayerChan* player = this->GetPlayerChan(card.tpos);
	if (player == NULL) return;

	// An binh thuong
	if (-1 != card.fpos && -1 != card.tpos) {
		player->ActionChuyenBaiByServer(card);
	}

	// Chiu bai
	if (-1 == card.fpos && 3 == card.count) {
		player->ActionChuyenBaiChiuByServer(card);
	}
}

void _Layer_CardChan_::Action_An_U(const CardOrg& card) {
	card.show();
}

void _Layer_CardChan_::ScaleCardsHand_WhenU() {
	((PlayerChanIsMe*)this->PlayerMe)->ScaleCardsHand_WhenU();
}

void _Layer_CardChan_::DisplayListCardsWinner(const string& uid, string& listCards) {

	if (this->LayerAllCards->getLocalZOrder() > this->LayerCardsInNoc->getLocalZOrder()) {
		int Temp = LayerAllCards->getLocalZOrder();
		this->LayerAllCards->setLocalZOrder(this->LayerCardsInNoc->getLocalZOrder());
		this->LayerCardsInNoc->setLocalZOrder(Temp);
	}

	vector<string> list = mUtils::splitString(listCards, ';');

	int i = 0;
	while (i <= list.size()) {
		if (i < list.size()) {
			try {
				vector<string> info = mUtils::splitString(list.at(i), ':');
				CardChan *pCard = CardChan::create();
				pCard->SetSTT(i);
				pCard->loadTexture(ChanUtils::FindTypeCard(info.at(1), info.at(2)), Widget::TextureResType::PLIST);
				pCard->SetSizeCard(W_CARD + 15, H_CARD + 15);
				pCard->setPosition(Vec2(WIDTH_DESIGN >> 1, 645));
				pCard->setTouchEnabled(true);
				pCard->setVisible(true);
				pCard->addTouchEventListener(CC_CALLBACK_2(_Layer_CardChan_::CardResultMove, this));
				this->arrCardsResult.push_back(pCard);
				this->LayerCardsOnHandWinner->addChild(pCard);
				i++;
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
		else {

			this->PlaySounds("Xoe.mp3");
			ExecuteInfo info;
			info.beginX = WIDTH_DESIGN / 2;
			info.beginY = 645;
			info.startPos = (float)(this->arrCardsResult.size() - 1);
			info.cornor = GOC;
			info.width = 0;
			info.height = H_CARD_HAND / 6;
			info.duration_ = 0.8;
			for_each(this->arrCardsResult.begin(), this->arrCardsResult.end(), std::bind2nd(mem_fun(&CardChan::RotateCardChan), info));

			this->DisplayButtonViewNoc();
			break;
		}
	}

	this->ShowLaBaiU(uid);
}

void _Layer_CardChan_::ShowLaBaiU(const string& uid){
	InfoCardU obj;
	obj.uid = "";
	for (int i = 0; i < this->arrCardU.size(); ++i) {
		InfoCardU t = this->arrCardU.at(i);
		if (t.uid == uid) {
			obj = t;
			break;
		}
	}

	if (obj.uid != "") {
		vector<string> info = mUtils::splitString(obj.cardu, ':');
		int id = atoi(info.at(0).c_str());
		BoBaiChan::GetInstance()->ShowU(id);
	}
}

void _Layer_CardChan_::DisplayButtonViewNoc(){
	//    BoBaiChan::GetInstance()->ShowCardU(this->CardU.cardnu, this->CardU.cardsu);
	this->LayerCardsInNoc->getChildByTag(KTAG_BTN_XEMNOC)->setVisible(true);
}

void _Layer_CardChan_::CardResultMove(Ref *pSender, ui::Widget::TouchEventType pType) {
	CardChan* pCard = (CardChan*)pSender;
	if (pCard == NULL)
		return;

	switch (pType) {
	case ui::Widget::TouchEventType::BEGAN: {
												IsDragDrop = true;
												Vec2 Tap = pCard->getTouchBeganPosition();
												Tap = ConvertPoint(Tap);
												oldPos = LayerCardsOnHandWinner->getPosition();
												disTouchBegan.setSize(Tap.x, Tap.y);
	}
		break;

	case ui::Widget::TouchEventType::MOVED: {
												if (IsDragDrop) {
													Vec2 Tap = pCard->getTouchMovePosition();
													Tap = ConvertPoint(Tap);
													Tap.x -= disTouchBegan.width;
													Tap.y -= disTouchBegan.height;

													Tap.x += oldPos.x;
													Tap.y += oldPos.y;
													LayerCardsOnHandWinner->setPosition(Tap);
												}
	}
		break;

	case ui::Widget::TouchEventType::CANCELED:
		IsDragDrop = false;
		break;

	case ui::Widget::TouchEventType::ENDED:
		IsDragDrop = false;
		break;

	default:
		break;
	}
}

void _Layer_CardChan_::MoveCardChi_WhenU() {
	this->PlayerMe->MoveCardCuaChi();
}

void _Layer_CardChan_::SetCountNoc(const int& count) {
	string StrCount = boost::to_string(count);
	this->LblCountNoc->setString(StrCount);
	this->DiaNoc->setVisible(true);
}

void _Layer_CardChan_::SetListCardsNoc(string& _list) {
	this->ListCardsNoc = _list;
}

void _Layer_CardChan_::ActionDisCards() {
	int res = this->PlayerMe->ActionDisCardByClient();
	if (res == CLIENT_RC_OK){
		ButtonChanNotification::GetInstance()->ClientDuoiCard();
		if (m_callback2 && m_callbackListener2) {
			(m_callback2->*m_callbackListener2)(this);
		}
	}
	else if (res == CLIENT_RC_CHUA_CHON_LABAI_DANH){
		this->Toast->ShowToast(dataManager.GetSysString(59), 3);
	}
	else if (res == CLIENT_RC_ACTION_PENDING){
		log("Client da danh, dang doi server xac nhan ....");
	}
}

int _Layer_CardChan_::ActionChiuCard() {
	int res = this->PlayerMe->ActionChiuCardByClient();
	if (res == CLIENT_RC_LASTROOM_NULL) {
		log("last room == NULL");
	}
	else if (res == CLIENT_RC_OK) {
		log("send chiu ok");
	}
	else if (res == CLIENT_RC_CHUA_CHON_LABAI_CHIU) {
		this->Toast->ShowToast(dataManager.GetSysString(60), 3);
	}
	return res;
}

void _Layer_CardChan_::ActionTakeCard() {
	int res = this->PlayerMe->ActionTakeCardByClient(this->LastCardObj);
	switch (res){
	case CLIENT_RC_ACTION_PENDING:
		log("Client da tu an, cho server xac nhan");
		break;
	case CLIENT_RC_FAILED:
		log("An bai failed - Khong an duoc");
		break;
	case CLIENT_RC_AN_KHONG_DUNG_CHAN_CA:
		this->Toast->ShowToast(dataManager.GetSysString(61));
		break;
	case CLIENT_RC_OK:{
						 // log("An bai thanh cong, pending...");
						  ButtonChanNotification::GetInstance()->ClientDuoiCard();
						  if (m_callback2 && m_callbackListener2) {
							  (m_callback2->*m_callbackListener2)(this);
						  }
	}
		break;
	case CLIENT_RC_CARD_NOT_IN_HAND:
		log("Khong tim thay la bai nay de an");
		break;
	case CLIENT_RC_CHUA_CHON_LABAI_AN:
		this->Toast->ShowToast(dataManager.GetSysString(62), 3);
		break;
	default:
		log("unknow here !");
		break;
	}
}

void _Layer_CardChan_::ActionDuoiCard() {
	//An cac button di, chi hien thi button U.

	ButtonChanNotification::GetInstance()->ClientDuoiCard();
	if (m_callback2 && m_callbackListener2) {
		(m_callback2->*m_callbackListener2)(this);
	}

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lstRooms = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_DUOI_CARD, params, lstRooms));
	GameServer::getSingleton().Send(request);
}


void _Layer_CardChan_::DisplayListCard_Noc(vector<CardChan*>& P) {
	if (0 == P.size())
		return;
	ExecuteInfo info;
	info.startPos = (WIDTH_DESIGN - P.size() * (W_CARD / 3 * 2)) / 2;
	info.cornor = 0;
	info.width = W_CARD / 3 * 2;
	info.height = H_CARD / 2;
	for_each(arrCardsNoc.begin(), arrCardsNoc.end(), std::bind2nd(mem_fun(&CardChan::DisplayCardInListNoc), info));
}

void _Layer_CardChan_::CardNoc_Touch(Ref *pSender, ui::Widget::TouchEventType pType) {
	if (pType == ui::Widget::TouchEventType::ENDED && this->arrCardsNoc.size() > 0)
		this->DeleteListCards(this->arrCardsNoc);
}

void _Layer_CardChan_::PlaySounds(const string& url) {
	if (!mUtils::isSoundOn()) return;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(("sounds/Chan/" + url).c_str());
}

Vec2& _Layer_CardChan_::ConvertPoint(Vec2& pPoint) {
	float xCenter = this->sizeScreen.width / 2;
	float yCenter = this->sizeScreen.height / 2;

	float xTap = pPoint.x;
	float yTap = pPoint.y;

	float x1, x2;
	float y1, y2;

	float A = ((powf(xCenter - xTap, 2) + powf(yCenter - yTap, 2))) / powf(scaleApp, 2);
	float B = powf((yCenter - yTap) / (xCenter - xTap), 2) + 1;

	x1 = xCenter + sqrtf(A / B);
	x2 = xCenter - sqrtf(A / B);

	y1 = yCenter + (yCenter - yTap) * (x1 - xCenter) / (xCenter - xTap);
	y2 = yCenter + (yCenter - yTap) * (x2 - xCenter) / (xCenter - xTap);

	if ((xTap < xCenter && x1 < xCenter) || (xTap > xCenter && x1 > xCenter)) {
		x1 -= startLeft;
		y1 -= startTop;

		pPoint.x = x1;
		pPoint.y = y1;
	}
	else if ((xTap < xCenter && x2 < xCenter) || (xTap > xCenter && x2 > xCenter)) {
		x2 -= startLeft;
		y2 -= startTop;

		pPoint.x = x2;
		pPoint.y = y2;
	}
	else {
		log("No define POINT CONVERT");
	}
	return pPoint;
}

void _Layer_CardChan_::SetCardCanChiu(string &ls)
{
	BoBaiChan::GetInstance()->SetCardCanChiu(ls);
}

void _Layer_CardChan_::OnXemNocClick(Ref *pSender, ui::Widget::TouchEventType pType) {
	if (pType == ui::Widget::TouchEventType::ENDED) {

		if (arrCardsNoc.size() > 0) {
			this->DeleteListCards(arrCardsNoc);
		}

		if (0 != ListCardsNoc.compare("")) {
			vector<string> arrNoc = mUtils::splitString(ListCardsNoc, ';');
			float startLeft = (WIDTH_DESIGN - (arrNoc.size() * (W_CARD / 3 * 2))) / 2;
			int i = 0;
			int listsize = (int)arrNoc.size();
			while (i <= listsize) {
				if (i < listsize) {
					try {
						vector<string> info = mUtils::splitString(arrNoc.at(i), ':');
						CardChan *pCard = CardChan::create();
						pCard->SetSTT(i);
						pCard->loadTexture(ChanUtils::FindTypeCard(info.at(1), info.at(2)), Widget::TextureResType::PLIST);
						pCard->SetSizeCard(W_CARD, H_CARD);
						pCard->setPosition(Vec2(startLeft, (HEIGHT_DESIGN >> 1) - H_CARD / 2));
						pCard->setVisible(true);
						pCard->setTouchEnabled(true);
						pCard->addTouchEventListener(CC_CALLBACK_2(_Layer_CardChan_::CardNoc_Touch, this));
						arrCardsNoc.push_back(pCard);
						LayerCardsInNoc->addChild(pCard);
						i++;
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
				else {
					this->DisplayListCard_Noc(arrCardsNoc);
					break;
				}
			}
			arrNoc.clear();
		}
	}
}

void _Layer_CardChan_::SetCallBackListCards(Ref* target, SEL_CallFuncO callfun) {
	this->m_callback = target;
	this->m_callbackListener = callfun;
}

void _Layer_CardChan_::SetCallBackDisCards(Ref* target, SEL_CallFuncO callfun){
	this->m_callback2 = target;
	this->m_callbackListener2 = callfun;
}

void _Layer_CardChan_::DeleteListCards(vector<CardChan*>& listCards) {
	if (0 == listCards.size())
		return;
	for_each(listCards.begin(), listCards.end(), std::bind2nd(mem_fun(&CardChan::removeFromParentAndCleanup), true));
	listCards.clear();
}

void _Layer_CardChan_::StartTurn()
{
	((PlayerChanIsMe*)this->PlayerMe)->StartTurn();
}

void _Layer_CardChan_::FinishTurn()
{
	((PlayerChanIsMe*)this->PlayerMe)->FinishTurn();
}

PlayerChan *_Layer_CardChan_::GetPlayerChan(const int &pos)
{
	switch (pos)
	{
	case kUserMe:
		return this->PlayerMe;
	case kUserLeft:
		return this->PlayerLeft;
	case kUserRight:
		return this->PlayerRight;
	case kUserTop:
		return this->PlayerTop;
	case kUserBot:
		return this->PlayerBottom;
	}
	return NULL;
}

PlayerChan *_Layer_CardChan_::GetPlayerChanByAI(const string &aI)
{
	if (0 != aI.compare("")){
		if (0 == this->PlayerMe->GetAI().compare(aI)){
			return this->PlayerMe;
		}

		else if (0 == this->PlayerLeft->GetAI().compare(aI)){
			return this->PlayerLeft;
		}

		else if (0 == this->PlayerRight->GetAI().compare(aI)){
			return this->PlayerRight;
		}

		else if (0 == this->PlayerTop->GetAI().compare(aI)){
			return this->PlayerTop;
		}

		else if (0 == this->PlayerBottom->GetAI().compare(aI)){
			return this->PlayerBottom;
		}
	}
	return NULL;
}

void _Layer_CardChan_::SetLayerChanToast(LayerChanToast* toast) {
	this->Toast = toast;
}

int _Layer_CardChan_::GetCountCardsMe() {
	return this->PlayerMe->GetCountCardTrenTay();
}

void _Layer_CardChan_::ChiuSuccess(const int& rescode)
{
	((PlayerChanIsMe*)this->PlayerMe)->ChiuSuccess(rescode);
}

void _Layer_CardChan_::TakeSuccess(const int& rc)
{
	((PlayerChanIsMe*)this->PlayerMe)->TakeSuccess(rc);
}

void _Layer_CardChan_::DisSuccess(const int& rescode){
	((PlayerChanIsMe*)this->PlayerMe)->DisSuccess(rescode);
}

bool _Layer_CardChan_::checkIsRunningCard(const int& pos, const int& cardnu, const int& cardsu) {
	PlayerChan* player = this->GetPlayerChan(pos);
	if (player != NULL){
		return player->CheckingRunningCuaChi();
	}
	return false;
}

bool _Layer_CardChan_::CheckCardRunning(const int& pos){
	PlayerChan* player = this->GetPlayerChan(pos);
	if (player == NULL)
		return false;
	return player->CheckCardRunning();
}

bool _Layer_CardChan_::checkU()
{
	return ((PlayerChanIsMe*)this->PlayerMe)->CheckU(this->LastCardObj);
}

void _Layer_CardChan_::SetInfoCardU(std::string& uid, std::string& card){
	log("Lá bài Ù: %s", card.c_str());
	InfoCardU obj;
	obj.uid = uid;
	obj.cardu = card;
	this->arrCardU.push_back(obj);
	this->hasUSucsses = true;
}

void _Layer_CardChan_::RemoveCardOnHandWinner(){
	if (this->arrCardsResult.size() > 0) {
		this->DeleteListCards(this->arrCardsResult);
	}
	this->LayerCardsInNoc->getChildByTag(KTAG_BTN_XEMNOC)->setVisible(false);
}

void _Layer_CardChan_::SetFlagAnBao(bool flag){
	this->FlagAnBao = flag;
}
