//
//  _CardBaCay_.cpp
//  iCasino_v2
//
//  Created by DauA on 6/24/14.
//
//

#include "SimpleAudioEngine.h"
#include "GameServer.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "_CardBaCay_.h"
#include "ChanUtils.h"
#include "PhomMessDef.h"

CardBaCay::CardBaCay()
:m_callback(NULL),
m_callbackListener(NULL),
w_card_me(50),
w_card_notme(50),
h_card_me(65),
h_card_notme(65),
Duration_Action(0.05),
Count_Give_Card(0),
MyAI(""),
ListUsers(""){

	auto _visiblesize = Director::getInstance()->getVisibleSize();
	//Size convert
	Size _size = ChanUtils::getSizePos();
	if (SceneManager::getSingleton().getGameID() != kGameBaCayChuong){
		xCardPos_me = (WIDTH_DESIGN / 2) - 30;
		yCardPos_me = 270 + _size.height;
		/*xCardPos_me = _visiblesize.width / 2 - 71,
		yCardPos_me = _visiblesize.height / 4,*/
		xCardPos_u1 = WIDTH_DESIGN / 6 + 150;
		yCardPos_u1 = 320 + _size.height;
		xCardPos_u2 = WIDTH_DESIGN / 9 + 130;
		yCardPos_u2 = (HEIGHT_DESIGN / 2) + _size.height - 30;
		xCardPos_u3 = (WIDTH_DESIGN / 6) + 150;
		yCardPos_u3 = (HEIGHT_DESIGN - 410) + _size.height;
		xCardPos_u4 = WIDTH_DESIGN / 6 * 5 - 210;
		yCardPos_u4 = (HEIGHT_DESIGN - 370) + _size.height;
		xCardPos_u5 = (WIDTH_DESIGN / 9 * 8) - 220;
		yCardPos_u5 = (HEIGHT_DESIGN / 2) + _size.height - 30;
		xCardPos_u6 = WIDTH_DESIGN / 6 * 5 - 200;
		yCardPos_u6 = 320 + _size.height;
	}
	else if (SceneManager::getSingleton().getGameID() == kGameBaCayChuong){
		xCardPos_me = WIDTH_DESIGN / 2 - WIDTH_DESIGN / 7 + W_MONEY_BG / 2 + 45;
		yCardPos_me = HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 4 - H_MONEY_BG / 2 + 35;
		/*xCardPos_me = _visiblesize.width / 2 - 71,
		yCardPos_me = _visiblesize.height / 4,*/
		xCardPos_u1 = WIDTH_DESIGN / 8 + W_MONEY_BG / 2 + H_MONEY_BG / 2 + 25;
		yCardPos_u1 = HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 7 - H_MONEY_BG / 2;
		xCardPos_u2 = WIDTH_DESIGN / 8 + W_MONEY_BG / 2 + 45;
		yCardPos_u2 = HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 7 - H_MONEY_BG / 2 + 10.0;
		xCardPos_u3 = WIDTH_DESIGN / 2 - WIDTH_DESIGN / 4 + W_MONEY_BG + 15.0;
		yCardPos_u3 = HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 3 - H_MONEY_BG / 2 + 45;
		xCardPos_u4 = WIDTH_DESIGN / 2 + WIDTH_DESIGN / 4 - W_MONEY_BG * 3 / 2;
		yCardPos_u4 = HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 3 - H_MONEY_BG / 2 + 45;
		xCardPos_u5 = WIDTH_DESIGN / 9 * 8 - H_MONEY_BG / 2 - W_MONEY_BG - 10.0;
		yCardPos_u5 = HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 7 - H_MONEY_BG / 2 + 10.0;
		xCardPos_u6 = WIDTH_DESIGN / 9 * 8 - W_MONEY_BG - 45.0;
		yCardPos_u6 = HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 7 - H_MONEY_BG / 2;
	}


	this->CARD_0.clear();
	this->CARD_1.clear();
	this->CARD_2.clear();
	this->CARD_3.clear();
	this->CARD_4.clear();
	this->CARD_5.clear();
	this->CARD_6.clear();

	this->arrCardTypes.clear();
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		this->arrCardTypes.push_back("s");
		this->arrCardTypes.push_back("c");
		this->arrCardTypes.push_back("d");
		this->arrCardTypes.push_back("h");
	}
	else{
		this->arrCardTypes.push_back("bich");
		this->arrCardTypes.push_back("tep");
		this->arrCardTypes.push_back("ro");
		this->arrCardTypes.push_back("co");
	}

	this->mapPlayers.clear();
}

CardBaCay::~CardBaCay() {
	this->CARD_0.clear();
	this->CARD_1.clear();
	this->CARD_2.clear();
	this->CARD_3.clear();
	this->CARD_4.clear();
	this->CARD_5.clear();
	this->CARD_6.clear();
	log("Deconstructor Layer Cards Ba Cay");
}

bool CardBaCay::init() {
	if (!Layer::init()) return false;
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2::ZERO);

	this->posxChiabai = WIDTH_DESIGN / 2;
	this->posYChiabai = HEIGHT_DESIGN / 2;
	xCardPos_me = WIDTH_DESIGN / 2 - WIDTH_DESIGN / 7 + W_MONEY_BG / 2 + 45;
	yCardPos_me = HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 4 - H_MONEY_BG / 2 + 35;
	return true;
}

void CardBaCay::ResetGame() {
	this->Count_Give_Card = 0;
	this->deleteAllCards_FromArray(this->CARD_0);
	this->deleteAllCards_FromArray(this->CARD_1);
	this->deleteAllCards_FromArray(this->CARD_2);
	this->deleteAllCards_FromArray(this->CARD_3);
	this->deleteAllCards_FromArray(this->CARD_4);
	this->deleteAllCards_FromArray(this->CARD_5);
	this->deleteAllCards_FromArray(this->CARD_6);
}
void CardBaCay::setListUser(string lu){
	this->ListUsers = lu;
}
void CardBaCay::SetMyName(string name) {
	this->MyAI = name;
}

void CardBaCay::PlaySound(const string& pathSound) {
	if (mUtils::isSoundOn()) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pathSound.c_str());
	}
}

void CardBaCay::GivePocker() {
	this->PlaySound("sounds/deal_card.mp3");
	//! Tạo 1 danh sách người chơi ảo để chia 3 lượt
	log("listusers: %s", this->ListUsers.c_str());
	string virtualList = this->ListUsers;// + this.ListUsers + this.ListUsers;
	auto arrVirtuals = mUtils::splitString(virtualList, ';');
	if (arrVirtuals.size() == 0)
		return;


	//! Số lần thực hiện chia bài.

	//! Chia bai
	for (int i = 0; i < 3; i++){
		int Count_Virtual_Users = i + 1;
		for (int j = 0; j < arrVirtuals.size(); j++){
			auto info = mUtils::splitString(arrVirtuals[j], '|');
			this->poskUser = this->GetPosUserByName(info[0]);
			switch (this->poskUser) {
			case kuser0:
				this->GivePockerNext(CARD_0, w_card_me, h_card_me, xCardPos_me, yCardPos_me, Count_Virtual_Users);
				break;
			case kuser1:
				this->GivePockerNext(CARD_1, w_card_notme, h_card_notme, xCardPos_u1, yCardPos_u1, Count_Virtual_Users);
				break;
			case kuser2:
				this->GivePockerNext(CARD_2, w_card_notme, h_card_notme, xCardPos_u2, yCardPos_u2, Count_Virtual_Users);
				break;
			case kuser3:
				this->GivePockerNext(CARD_3, w_card_notme, h_card_notme, xCardPos_u3, yCardPos_u3, Count_Virtual_Users);
				break;
			case kuser4:
				this->GivePockerNext(CARD_4, w_card_notme, h_card_notme, xCardPos_u4, yCardPos_u4, Count_Virtual_Users);
				break;
			case kuser5:
				this->GivePockerNext(CARD_5, w_card_notme, h_card_notme, xCardPos_u5, yCardPos_u5, Count_Virtual_Users);
				break;
			case kuser6:
				this->GivePockerNext(CARD_6, w_card_notme, h_card_notme, xCardPos_u6, yCardPos_u6, Count_Virtual_Users);
				break;
			default:
				break;
			}
		}
	}
	//
	DelayTime* delay = DelayTime::create(Duration_Action * 3);
	CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(CardBaCay::callbackWhenGivePokerDone, this));
	this->runAction(Sequence::create(delay, callfunc, NULL));
}


void CardBaCay::GivePockerNext(std::vector<Card*>&P, const float& _width, const float& _height, const float& _left, const float& _bottom, const int& count) {
	this->Count_Give_Card++;
	float posY;
	auto _visibleSize = Director::getInstance()->getVisibleSize();

	Card* pCard = new Card("card_back.png");
	pCard->autorelease();
	pCard->setPosition(Vec2(posxChiabai, posYChiabai));
	pCard->SetScaleCard(w_card_notme / pCard->getContentSize().width, h_card_notme / pCard->getContentSize().height);
	if (count == 1){
		pCard->setRotation(-20);
		posY = _bottom;
	}
	else if (count == 3){
		pCard->setRotation(20);
		posY = _bottom - 15.0;
	}
	else{
		posY = _bottom;
	}
	pCard->setVisible(true);
	this->addChild(pCard);

	MoveTo *newTo = MoveTo::create((Duration_Action)*count, Vec2(_left + P.size() *(_width - 25.0), posY));
	ScaleBy *scaleTo = ScaleBy::create(Duration_Action*count, 1.7);

	pCard->runAction(newTo);
	pCard->runAction(scaleTo);
	P.push_back(pCard);
}
void CardBaCay::FlipEachCard(string& lc){
	if (this->CARD_0.size() != 3)
		this->createListCard_Back(this->CARD_0, this->w_card_me, this->h_card_me, this->xCardPos_me, this->yCardPos_me);
	else {
		for (int i = 0; i < this->CARD_0.size(); ++i) {
			this->CARD_0[i]->setPosition(this->xCardPos_me + i * this->w_card_me, this->yCardPos_me);
		}
	}

	vector<string> arrCards = mUtils::splitString(lc, '-');
	if (0 == arrCards.size())
		return;

	auto pCard1 = this->CARD_0[0];
	if (arrCards[0] == "0_0")
		pCard1->initWithSpriteFrameName("card_back.png");
	else {
		auto info1 = mUtils::splitString(arrCards[0], '_');
		string str1;
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
			str1 = "card_" + FindTypeCardByID(info1[0]) + "_" + arrCardTypes[atoi(info1[1].c_str())] + ".png";
		else
			str1 = FindTypeCardByID(info1[0]) + "-" + arrCardTypes[atoi(info1[1].c_str())] + ".png";
		//str1 = "card_" + FindTypeCardByID(info1[0]) + "_" + arrCardTypes[atoi(info1[1].c_str())] + ".png";
		int skewAmount = 40;
		auto firstSkew1 = SkewBy::create(0.1, 0, skewAmount);
		auto initFrame1 = CallFunc::create([pCard1, str1](){pCard1->initWithSpriteFrameName(str1.c_str()); });
		// flip the skew value after the initial half of the animation for a more credible 3d effect
		auto instantFlipSkew1 = SkewTo::create(0, 0, -skewAmount);
		auto resetSkew1 = SkewTo::create(0.1, 0, 0);
		auto delayTime1 = DelayTime::create(0);
		// var firtRotate = cc.Rotate3.create(0.5,cc.math.vec3(0,90,0));
		pCard1->runAction(Sequence::create(delayTime1, firstSkew1, instantFlipSkew1, initFrame1, resetSkew1, NULL));
	}
	auto pCard2 = this->CARD_0[1];
	if (arrCards[1] == "0_0")
		pCard2->initWithSpriteFrameName("card_back.png");
	else {
		auto info2 = mUtils::splitString(arrCards[1], '_');
		string str2;
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
			str2 = "card_" + FindTypeCardByID(info2[0]) + "_" + arrCardTypes[atoi(info2[1].c_str())] + ".png";
		else
			str2 = FindTypeCardByID(info2[0]) + "-" + arrCardTypes[atoi(info2[1].c_str())] + ".png";
		//string str2 = "card_" + FindTypeCardByID(info2[0]) + "_" + arrCardTypes[atoi(info2[1].c_str())] + ".png";
		int skewAmount = 40;
		auto firstSkew2 = SkewBy::create(0.1, 0, skewAmount);
		auto initFrame2 = CallFunc::create([pCard2, str2](){pCard2->initWithSpriteFrameName(str2.c_str()); });
		// flip the skew value after the initial half of the animation for a more credible 3d effect
		auto instantFlipSkew2 = SkewTo::create(0, 0, -skewAmount);
		auto resetSkew2 = SkewTo::create(0.1, 0, 0);
		auto delayTime2 = DelayTime::create(1);
		// var firtRotate = cc.Rotate3.create(0.5,cc.math.vec3(0,90,0));
		pCard2->runAction(Sequence::create(delayTime2, firstSkew2, instantFlipSkew2, initFrame2, resetSkew2, NULL));
	}
	auto pCard3 = this->CARD_0[2];
	if (arrCards[2] == "0_0")
		pCard3->initWithSpriteFrameName("card_back.png");
	else {
		auto info3 = mUtils::splitString(arrCards[2], '_');
		string str3;
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
			str3 = "card_" + FindTypeCardByID(info3[0]) + "_" + arrCardTypes[atoi(info3[1].c_str())] + ".png";
		else
			str3 = FindTypeCardByID(info3[0]) + "-" + arrCardTypes[atoi(info3[1].c_str())] + ".png";
		//string str3 = "card_" + FindTypeCardByID(info3[0]) + "_" + arrCardTypes[atoi(info3[1].c_str())] + ".png";
		int skewAmount = 40;
		auto firstSkew3 = SkewBy::create(0.1, 0, skewAmount);
		auto initFrame3 = CallFunc::create([pCard3, str3](){pCard3->initWithSpriteFrameName(str3.c_str()); });
		// flip the skew value after the initial half of the animation for a more credible 3d effect
		auto instantFlipSkew3 = SkewTo::create(0, 0, -skewAmount);
		auto resetSkew3 = SkewTo::create(0.1, 0, 0);
		auto delayTime3 = DelayTime::create(2);
		// var firtRotate = cc.Rotate3.create(0.5,cc.math.vec3(0,90,0));
		pCard3->runAction(Sequence::create(delayTime3, firstSkew3, instantFlipSkew3, initFrame3, resetSkew3, NULL));
	}
}

//create list card
void CardBaCay::TurnUpAllCards(string lc, const int& pos) {
	log("Lc in layer cards: %s", lc.c_str());
	log("Pos = %d", pos);

	switch (pos) {
	case kuser0:
		TurnUpAllCardsAtPos(lc, CARD_0, w_card_me, h_card_me, xCardPos_me, yCardPos_me);
		break;

	case kuser1:
		TurnUpAllCardsAtPos(lc, CARD_1, w_card_notme, h_card_notme, xCardPos_u1, yCardPos_u1);
		break;

	case kuser2:
		TurnUpAllCardsAtPos(lc, CARD_2, w_card_notme, h_card_notme, xCardPos_u2, yCardPos_u2);
		break;

	case kuser3:
		TurnUpAllCardsAtPos(lc, CARD_3, w_card_notme, h_card_notme, xCardPos_u3, yCardPos_u3);
		break;

	case kuser4:
		TurnUpAllCardsAtPos(lc, CARD_4, w_card_notme, h_card_notme, xCardPos_u4, yCardPos_u4);
		break;

	case kuser5:
		TurnUpAllCardsAtPos(lc, CARD_5, w_card_notme, h_card_notme, xCardPos_u5, yCardPos_u5);
		break;

	case kuser6:
		TurnUpAllCardsAtPos(lc, CARD_6, w_card_notme, h_card_notme, xCardPos_u6, yCardPos_u6);
		break;

	default:
		break;
	}
}

void CardBaCay::TurnUpAllCardsAtPos(string& lc, std::vector<Card*>& P, const float& _width, const float& _height, const float& _left, const float& _bottom) {

	float posY;
	if (P.size() != 3)
		createListCard_Back(P, _width, _height, _left, _bottom);
	else {
		for (int i = 0; i < P.size(); ++i) {
			if (i == P.size() - 1){
				posY = _bottom - 10.0;
			}
			else{
				posY = _bottom;
			}
			P[i]->setPosition(Vec2(_left + i * (_width - 25.0), posY));
		}
	}

	vector<string> arrCards = mUtils::splitString(lc, '-');
	if (0 == arrCards.size())
		return;

	int n = arrCards.size();
	if (P.size() < arrCards.size())
		n = P.size();

	for (int i = 0; i < n; ++i) {
		Card *pCard = P.at(i);
		if (0 == arrCards[i].compare("0_0"))
			pCard->initWithSpriteFrameName("card_back.png");
		else {
			vector<string> info = mUtils::splitString(arrCards[i], '_');
			string str;
			str = "card_" + FindTypeCardByID(info[0]) + "_" + arrCardTypes[atoi(info[1].c_str())] + ".png";
			log("str = %s", str.c_str());

			pCard->initWithSpriteFrameName(str.c_str());
		}
	}
}

string CardBaCay::FindTypeCardByID(string& card) {
	if (0 == card.compare("11")) {
		card = "j";
	}
	else if (0 == card.compare("1")) {
		card = "a";
	}
	else if (0 == card.compare("12")) {
		card = "q";
	}
	else if (0 == card.compare("13")) {
		card = "k";
	}
	return card;
}

void CardBaCay::CreateCardBack(const int& pos) {
	switch (pos) {
	case kuser0:
		createListCard_Back(CARD_0, w_card_me, h_card_me, xCardPos_me, yCardPos_me);
		break;
	case kuser1:
		createListCard_Back(CARD_1, w_card_notme, h_card_notme, xCardPos_u1, yCardPos_u1);
		break;
	case kuser2:
		createListCard_Back(CARD_2, w_card_notme, h_card_notme, xCardPos_u2, yCardPos_u2);
		break;
	case kuser3:
		createListCard_Back(CARD_3, w_card_notme, h_card_notme, xCardPos_u3, yCardPos_u3);
		break;
	case kuser4:
		createListCard_Back(CARD_4, w_card_notme, h_card_notme, xCardPos_u4, yCardPos_u4);
		break;
	case kuser5:
		createListCard_Back(CARD_5, w_card_notme, h_card_notme, xCardPos_u5, yCardPos_u5);
		break;
	case kuser6:
		createListCard_Back(CARD_6, w_card_notme, h_card_notme, xCardPos_u6, yCardPos_u6);
		break;
	}
}

void CardBaCay::createListCard_Back(std::vector<Card*>&P, const float& _width, const float& _height, const float& _left, const float& _bottom) {

	if (P.size() > 0) {
		for_each(P.begin(), P.end(), bind2nd(mem_fun(&Card::removeFromParentAndCleanup), true));
		P.clear();
	}

	for (int i = 0; i < 3; ++i) {
		Card *pCard = new Card("card_back.png");
		float posY;
		if (i == 0){
			pCard->setRotation(-20);
			posY = _bottom;
		}
		else if (i == 2){
			pCard->setRotation(20);
			posY = _bottom - 15.0;
		}
		else{
			posY = _bottom;
		}
		pCard->autorelease();
		pCard->SetScaleCard(_width / pCard->getContentSize().width, _height / pCard->getContentSize().height);
		pCard->setPosition(Vec2(_left + i * (_width - 25.0), posY));
		pCard->setVisible(true);
		pCard->setScale(0.8);
		this->addChild(pCard);
		P.push_back(pCard);
	}
}

void CardBaCay::deleteAllCards_FromArray(std::vector<Card*>& P) {
	if (0 == P.size()) return;
	for_each(P.begin(), P.end(), bind2nd(mem_fun(&Card::removeFromParentAndCleanup), true));
	P.clear();
}

void CardBaCay::SetCallbackFunc(Ref* target, SEL_CallFuncN callfun) {
	m_callback = target;
	m_callbackListener = callfun;
}

std::vector<Card*>& CardBaCay::getListCards_0() {
	return CARD_0;
}

std::vector<Card*>& CardBaCay::getListCards_1() {
	return CARD_1;
}

std::vector<Card*>& CardBaCay::getListCards_2() {
	return CARD_2;
}

std::vector<Card*>& CardBaCay::getListCards_3() {
	return CARD_3;
}

std::vector<Card*>& CardBaCay::getListCards_4() {
	return CARD_4;
}

std::vector<Card*>& CardBaCay::getListCards_5() {
	return CARD_5;
}

std::vector<Card*>& CardBaCay::getListCards_6() {
	return CARD_6;
}

bool CardBaCay::IsSpect() {
	if (this->ListUsers == "")
		return false;
	auto arr = mUtils::splitString(this->ListUsers, ';');
	auto size = arr.size();
	for (auto i = 0; i < size; i++) {
		auto arrInfo = mUtils::splitString(arr[i], '|');
		if (arrInfo.size() < 2) continue;
		auto name = arrInfo[0];
		if (name == this->MyAI) return false;
	}
	return true;
}

bool CardBaCay::isRunningAction() {
	for (int i = 0; i <= 6; i++) {
		std::vector<Card*> _lstCards;
		switch (i) {
		case 0:
			_lstCards = CARD_0;
			break;
		case 1:
			_lstCards = CARD_1;
			break;
		case 2:
			_lstCards = CARD_2;
			break;
		case 3:
			_lstCards = CARD_3;
			break;
		case 4:
			_lstCards = CARD_4;
			break;
		case 5:
			_lstCards = CARD_5;
			break;
		case 6:
			_lstCards = CARD_6;
			break;
		}
		//
		for (int i = 0; i<_lstCards.size(); i++) {
			Card* pcard = _lstCards.at(i);
			if (pcard->getNumberOfRunningActions() > 0) {
				return true;
			}
		}
	}
	return false;
}

void CardBaCay::setPlayerMap(const std::map<std::string, int>& mmap) {
	this->mapPlayers.clear();
	this->mapPlayers = mmap;
	this->posgive.clear();
	// for(int i = 0; i < 3; ++i) 
	{
		for (std::map<std::string, int>::iterator it = mapPlayers.begin(); it != mapPlayers.end(); ++it) {
			this->posgive.push_back(it->second);
		}
	}
}

void CardBaCay::callbackWhenGivePokerDone()
{
	if (m_callback && m_callbackListener) {
		(m_callback->*m_callbackListener)(this);
	}
}

std::vector<Card*>& CardBaCay::getListCardsByPos(int pos)
{
	std::vector<Card*> _lstCards;
	switch (pos)
	{
	case 0:
		_lstCards = CARD_0;
		break;
	case 1:
		_lstCards = CARD_1;
		break;
	case 2:
		_lstCards = CARD_2;
		break;
	case 3:
		_lstCards = CARD_3;
		break;
	case 4:
		_lstCards = CARD_4;
		break;
	case 5:
		_lstCards = CARD_5;
		break;
	case 6:
		_lstCards = CARD_6;
		break;
	}
	return _lstCards;
}
int CardBaCay::GetPosUserByName(string uid) {
	if (this->ListUsers == "")
		return -1;

	auto vt = -1;
	auto list = mUtils::splitString(ListUsers, ';');
	for (auto i = 0; i < list.size(); ++i) {
		auto info = mUtils::splitString(list[i], '|');
		if (MyAI.compare(info[0]) == 0 || this->IsSpect()) {
			vt = i;
			break;
		}
	}

	if (vt != -1) {
		for (auto k = 0; k < list.size(); ++k) {
			if (list[k] == "")
				continue;

			auto n = mUtils::splitString(list[k], '|');
			if (uid.compare(n[0]) == 0) {

				if (k == vt) {
					return kuser0;
				}
				else if (k == (vt + 1) % 7) {
					return kuser1;
				}
				else if (k == (vt + 2) % 7) {
					return kuser2;
				}
				else if (k == (vt + 3) % 7) {
					return kuser3;
				}
				else if (k == (vt + 4) % 7) {
					return kuser4;
				}
				else if (k == (vt + 5) % 7) {
					return kuser5;
				}
				else if (k == (vt + 6) % 7) {
					return kuser6;
				}
				break;
			}
		}
	}
	else
		return -1;
}


//update for new version lieng

LayerCardLieng::LayerCardLieng()
:m_callback(NULL),
m_callbackListener(NULL),
Duration_Action(0.1),
Count_Give_Card(0),
MyAI(""),
ListUsers(""){

	//Size convert
	Size _size = ChanUtils::getSizePos();
	xCardPos_me = WIDTH_DESIGN / 2 - WIDTH_DESIGN / 7 + W_MONEY_BG / 2 + 45;
	yCardPos_me = HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 4 - H_MONEY_BG / 2 + 35;
	/*xCardPos_me = _visiblesize.width / 2 - 71,
	yCardPos_me = _visiblesize.height / 4,*/
	xCardPos_u1 = WIDTH_DESIGN / 8 + W_MONEY_BG / 2 + H_MONEY_BG / 2 + 35;
	yCardPos_u1 = HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 7 - H_MONEY_BG / 2 + 35.0;
	xCardPos_u2 = WIDTH_DESIGN / 8 + W_MONEY_BG / 2 + H_MONEY_BG / 2 + 35;
	yCardPos_u2 = HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 7 - H_MONEY_BG / 2 + 35.0;
	xCardPos_u3 = WIDTH_DESIGN / 2 - WIDTH_DESIGN / 4 + W_MONEY_BG + 15.0;
	yCardPos_u3 = HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 3 - H_MONEY_BG / 2 + 75;
	xCardPos_u4 = WIDTH_DESIGN / 2 + WIDTH_DESIGN / 4 - W_MONEY_BG * 3 / 2 - 15.0;
	yCardPos_u4 = HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 3 - H_MONEY_BG / 2 + 75;
	xCardPos_u5 = WIDTH_DESIGN / 9 * 8 - H_MONEY_BG / 2 - W_MONEY_BG - 25.0;
	yCardPos_u5 = HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 7 - H_MONEY_BG / 2 + 35.0;
	xCardPos_u6 = WIDTH_DESIGN / 9 * 8 - H_MONEY_BG / 2 - W_MONEY_BG - 25.0;
	yCardPos_u6 = HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 7 - H_MONEY_BG / 2 + 35;
	w_card_me = 90;
	h_card_me = 110;

	w_card_notme = 60;
	h_card_notme = 75;

	this->CARD_0.clear();
	this->CARD_1.clear();
	this->CARD_2.clear();
	this->CARD_3.clear();
	this->CARD_4.clear();
	this->CARD_5.clear();
	this->CARD_6.clear();

	this->arrCardTypes.clear();
	// 	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	// 	{
	this->arrCardTypes.push_back("s");
	this->arrCardTypes.push_back("c");
	this->arrCardTypes.push_back("d");
	this->arrCardTypes.push_back("h");
	// 	}
	// 	else{
	// 		this->arrCardTypes.push_back("bich");
	// 		this->arrCardTypes.push_back("tep");
	// 		this->arrCardTypes.push_back("ro");
	// 		this->arrCardTypes.push_back("co");
	// 	}

	this->mapPlayers.clear();
}

LayerCardLieng::~LayerCardLieng() {
	this->CARD_0.clear();
	this->CARD_1.clear();
	this->CARD_2.clear();
	this->CARD_3.clear();
	this->CARD_4.clear();
	this->CARD_5.clear();
	this->CARD_6.clear();
	log("Deconstructor Layer Cards Ba Cay");
}

bool LayerCardLieng::init() {
	if (!Layer::init()) return false;
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2::ZERO);
	return true;
}

void LayerCardLieng::ResetGame() {
	this->Count_Give_Card = 0;
	this->deleteAllCards_FromArray(this->CARD_0);
	this->deleteAllCards_FromArray(this->CARD_1);
	this->deleteAllCards_FromArray(this->CARD_2);
	this->deleteAllCards_FromArray(this->CARD_3);
	this->deleteAllCards_FromArray(this->CARD_4);
	this->deleteAllCards_FromArray(this->CARD_5);
	this->deleteAllCards_FromArray(this->CARD_6);
}
void LayerCardLieng::setListUser(string lu){
	this->ListUsers = lu;
}
void LayerCardLieng::SetMyName(string name) {
	this->MyAI = name;
}

void LayerCardLieng::PlaySound(const string& pathSound) {
	if (mUtils::isSoundOn()) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pathSound.c_str());
	}
}

void LayerCardLieng::GivePocker() {
	this->PlaySound("sounds/deal_card.mp3");
	//! Tạo 1 danh sách người chơi ảo để chia 3 lượt

	string virtualList = this->ListUsers;// + this.ListUsers + this.ListUsers;
	auto arrVirtuals = mUtils::splitString(virtualList, ';');
	if (arrVirtuals.size() == 0)
		return;

	//! Số lần thực hiện chia bài.

	//! Chia bai
	for (int i = 0; i < 3; i++){
		int Count_Virtual_Users = i + 1;
		for (int j = 0; j < arrVirtuals.size(); j++){
			auto info = mUtils::splitString(arrVirtuals[j], '|');
			int pos = this->GetPosUserByName(info[0]);
			switch (pos) {
			case kuser0:
				this->GivePockerNext(CARD_0, w_card_me, h_card_me, xCardPos_me, yCardPos_me, Count_Virtual_Users);
				break;
			case kuser1:
				this->GivePockerNext(CARD_1, w_card_notme, h_card_notme, xCardPos_u1, yCardPos_u1, Count_Virtual_Users);
				break;
			case kuser2:
				this->GivePockerNext(CARD_2, w_card_notme, h_card_notme, xCardPos_u2, yCardPos_u2, Count_Virtual_Users);
				break;
			case kuser3:
				this->GivePockerNext(CARD_3, w_card_notme, h_card_notme, xCardPos_u3, yCardPos_u3, Count_Virtual_Users);
				break;
			case kuser4:
				this->GivePockerNext(CARD_4, w_card_notme, h_card_notme, xCardPos_u4, yCardPos_u4, Count_Virtual_Users);
				break;
			case kuser5:
				this->GivePockerNext(CARD_5, w_card_notme, h_card_notme, xCardPos_u5, yCardPos_u5, Count_Virtual_Users);
				break;
			case kuser6:
				this->GivePockerNext(CARD_6, w_card_notme, h_card_notme, xCardPos_u6, yCardPos_u6, Count_Virtual_Users);
				break;
			default:
				break;
			}
		}
	}
	//
	DelayTime* delay = DelayTime::create(Duration_Action * 3);
	CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(LayerCardLieng::callbackWhenGivePokerDone, this));
	this->runAction(Sequence::create(delay, callfunc, NULL));
}


void LayerCardLieng::GivePockerNext(std::vector<Card*>&P, const float& _width, const float& _height, const float& _left, const float& _bottom, const int& count) {
	this->Count_Give_Card++;
	float posY;

	Size _size = ChanUtils::getSizePos();

	Card* pCard = new Card("card_back.png");
	pCard->autorelease();
	pCard->setPosition(Vec2(WIDTH_DESIGN / 2, HEIGHT_DESIGN / 2));
	pCard->SetScaleCard(w_card_notme / pCard->getContentSize().width, h_card_notme / pCard->getContentSize().height);
	if (count == 1){
		pCard->setRotation(-20);
		posY = _bottom;
	}
	else if (count == 3){
		pCard->setRotation(20);
		posY = _bottom - 10.0;
	}
	else{
		posY = _bottom;
	}
	pCard->setVisible(true);
	this->addChild(pCard);

	MoveTo *newTo = MoveTo::create(Duration_Action*count, Vec2(_left + P.size() *(_width - 25.0), posY));
	ScaleBy *scaleTo = ScaleBy::create(Duration_Action*count, 1.7);

	pCard->runAction(newTo);
	pCard->runAction(scaleTo);
	P.push_back(pCard);

	//     if (this->Count_Give_Card < count) {
	//         DelayTime* delay = DelayTime::create(Duration_Action);
	//         CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(LayerCardLieng::GivePocker, this));
	//         this->runAction(Sequence::create(delay, callfunc, NULL));
	//     } else {
	//         if (m_callback && m_callbackListener) {
	//             (m_callback->*m_callbackListener)(this);
	//         }
	//     }
}

//create list card
void LayerCardLieng::TurnUpAllCards(string lc, const int& pos) {
	log("Lc in layer cards: %s", lc.c_str());
	log("Pos = %d", pos);

	switch (pos) {
	case kuser0:
		TurnUpAllCardsAtPos(lc, CARD_0, w_card_me, h_card_me, xCardPos_me, yCardPos_me);
		break;

	case kuser1:
		TurnUpAllCardsAtPos(lc, CARD_1, w_card_notme, h_card_notme, xCardPos_u1, yCardPos_u1);
		break;

	case kuser2:
		TurnUpAllCardsAtPos(lc, CARD_2, w_card_notme, h_card_notme, xCardPos_u2, yCardPos_u2);
		break;

	case kuser3:
		TurnUpAllCardsAtPos(lc, CARD_3, w_card_notme, h_card_notme, xCardPos_u3, yCardPos_u3);
		break;

	case kuser4:
		TurnUpAllCardsAtPos(lc, CARD_4, w_card_notme, h_card_notme, xCardPos_u4, yCardPos_u4);
		break;

	case kuser5:
		TurnUpAllCardsAtPos(lc, CARD_5, w_card_notme, h_card_notme, xCardPos_u5, yCardPos_u5);
		break;

	case kuser6:
		TurnUpAllCardsAtPos(lc, CARD_6, w_card_notme, h_card_notme, xCardPos_u6, yCardPos_u6);
		break;

	default:
		break;
	}
}

void LayerCardLieng::TurnUpAllCardsAtPos(string& lc, std::vector<Card*>& P, const float& _width, const float& _height, const float& _left, const float& _bottom) {

	float posY;
	if (P.size() != 3)
		createListCard_Back(P, _width, _height, _left, _bottom);
	else {
		for (int i = 0; i < P.size(); ++i) {
			if (i == P.size() - 1){
				posY = _bottom - 10.0;
			}
			else{
				posY = _bottom;
			}
			P[i]->setPosition(Vec2(_left + i * (_width - 25.0), posY));
		}
	}

	vector<string> arrCards = mUtils::splitString(lc, '-');
	if (0 == arrCards.size())
		return;

	int n = arrCards.size();
	if (P.size() < arrCards.size())
		n = P.size();

	for (int i = 0; i < n; ++i) {
		Card *pCard = P.at(i);
		if (0 == arrCards[i].compare("0_0"))
			pCard->initWithSpriteFrameName("card_back.png");
		else {
			vector<string> info = mUtils::splitString(arrCards[i], '_');
			string str = this->getURL_byID_TienLen(info.at(0), info.at(1));
			pCard->initWithSpriteFrameName(str);
		}
	}
}

string LayerCardLieng::FindTypeCardByID(string& card) {
	if (0 == card.compare("11")) {
		card = "j";
	}
	else if (0 == card.compare("1")) {
		card = "a";
	}
	else if (0 == card.compare("12")) {
		card = "q";
	}
	else if (0 == card.compare("13")) {
		card = "k";
	}
	return card;
}

void LayerCardLieng::CreateCardBack(const int& pos) {
	switch (pos) {
	case kuser0:
		createListCard_Back(CARD_0, w_card_me, h_card_me, xCardPos_me, yCardPos_me);
		break;
	case kuser1:
		createListCard_Back(CARD_1, w_card_notme, h_card_notme, xCardPos_u1, yCardPos_u1);
		break;
	case kuser2:
		createListCard_Back(CARD_2, w_card_notme, h_card_notme, xCardPos_u2, yCardPos_u2);
		break;
	case kuser3:
		createListCard_Back(CARD_3, w_card_notme, h_card_notme, xCardPos_u3, yCardPos_u3);
		break;
	case kuser4:
		createListCard_Back(CARD_4, w_card_notme, h_card_notme, xCardPos_u4, yCardPos_u4);
		break;
	case kuser5:
		createListCard_Back(CARD_5, w_card_notme, h_card_notme, xCardPos_u5, yCardPos_u5);
		break;
	case kuser6:
		createListCard_Back(CARD_6, w_card_notme, h_card_notme, xCardPos_u6, yCardPos_u6);
		break;
	}
}

void LayerCardLieng::createListCard_Back(std::vector<Card*>&P, const float& _width, const float& _height, const float& _left, const float& _bottom) {

	if (P.size() > 0) {
		for_each(P.begin(), P.end(), bind2nd(mem_fun(&Card::removeFromParentAndCleanup), true));
		P.clear();
	}

	for (int i = 0; i < 3; ++i) {
		Card *pCard = new Card("card_back.png");
		float posY;
		if (i == 0){
			pCard->setRotation(-20);
			posY = _bottom;
		}
		else if (i == 2){
			pCard->setRotation(20);
			posY = _bottom - 15.0;
		}
		else{
			posY = _bottom;
		}
		pCard->autorelease();
		pCard->SetScaleCard(_width / pCard->getContentSize().width, _height / pCard->getContentSize().height);
		pCard->setPosition(Vec2(_left + i * (_width - 25.0), posY));
		pCard->setVisible(true);
		pCard->setScale(0.8);
		this->addChild(pCard);
		P.push_back(pCard);
	}
}

void LayerCardLieng::deleteAllCards_FromArray(std::vector<Card*>& P) {
	if (0 == P.size()) return;
	for_each(P.begin(), P.end(), bind2nd(mem_fun(&Card::removeFromParentAndCleanup), true));
	P.clear();
}

void LayerCardLieng::SetCallbackFunc(Ref* target, SEL_CallFuncN callfun) {
	m_callback = target;
	m_callbackListener = callfun;
}

std::vector<Card*>& LayerCardLieng::getListCards_0() {
	return CARD_0;
}

std::vector<Card*>& LayerCardLieng::getListCards_1() {
	return CARD_1;
}

std::vector<Card*>& LayerCardLieng::getListCards_2() {
	return CARD_2;
}

std::vector<Card*>& LayerCardLieng::getListCards_3() {
	return CARD_3;
}

std::vector<Card*>& LayerCardLieng::getListCards_4() {
	return CARD_4;
}

std::vector<Card*>& LayerCardLieng::getListCards_5() {
	return CARD_5;
}

std::vector<Card*>& LayerCardLieng::getListCards_6() {
	return CARD_6;
}

bool LayerCardLieng::IsSpect() {
	if (this->ListUsers == "")
		return false;
	auto arr = mUtils::splitString(this->ListUsers, ';');
	auto size = arr.size();
	for (auto i = 0; i < size; i++) {
		auto arrInfo = mUtils::splitString(arr[i], '|');
		if (arrInfo.size() < 2) continue;
		auto name = arrInfo[0];
		if (name == this->MyAI) return false;
	}
	return true;
}

bool LayerCardLieng::isRunningAction() {
	for (int i = 0; i <= 6; i++) {
		std::vector<Card*> _lstCards;
		switch (i) {
		case 0:
			_lstCards = CARD_0;
			break;
		case 1:
			_lstCards = CARD_1;
			break;
		case 2:
			_lstCards = CARD_2;
			break;
		case 3:
			_lstCards = CARD_3;
			break;
		case 4:
			_lstCards = CARD_4;
			break;
		case 5:
			_lstCards = CARD_5;
			break;
		case 6:
			_lstCards = CARD_6;
			break;
		}
		//
		for (int i = 0; i<_lstCards.size(); i++) {
			Card* pcard = _lstCards.at(i);
			if (pcard->getNumberOfRunningActions() > 0) {
				return true;
			}
		}
	}
	return false;
}

void LayerCardLieng::setPlayerMap(const std::map<std::string, int>& mmap) {
	this->mapPlayers.clear();
	this->mapPlayers = mmap;
	this->posgive.clear();
	// for(int i = 0; i < 3; ++i) 
	{
		for (std::map<std::string, int>::iterator it = mapPlayers.begin(); it != mapPlayers.end(); ++it) {
			this->posgive.push_back(it->second);
		}
	}
}

void LayerCardLieng::callbackWhenGivePokerDone()
{
	if (m_callback && m_callbackListener) {
		(m_callback->*m_callbackListener)(this);
	}
}

std::vector<Card*>& LayerCardLieng::getListCardsByPos(int pos)
{
	std::vector<Card*> _lstCards;
	switch (pos)
	{
	case 0:
		_lstCards = CARD_0;
		break;
	case 1:
		_lstCards = CARD_1;
		break;
	case 2:
		_lstCards = CARD_2;
		break;
	case 3:
		_lstCards = CARD_3;
		break;
	case 4:
		_lstCards = CARD_4;
		break;
	case 5:
		_lstCards = CARD_5;
		break;
	case 6:
		_lstCards = CARD_6;
		break;
	}
	return _lstCards;
}
int LayerCardLieng::GetPosUserByName(string uid) {
	if (this->ListUsers == "")
		return -1;

	auto vt = -1;
	auto list = mUtils::splitString(ListUsers, ';');
	for (auto i = 0; i < list.size(); ++i) {
		auto info = mUtils::splitString(list[i], '|');
		if (MyAI.compare(info[0]) == 0 || this->IsSpect()) {
			vt = i;
			break;
		}
	}

	if (vt != -1) {
		for (auto k = 0; k < list.size(); ++k) {
			if (list[k] == "")
				continue;

			auto n = mUtils::splitString(list[k], '|');
			if (uid.compare(n[0]) == 0) {

				if (k == vt) {
					return kuser0;
				}
				else if (k == (vt + 1) % 7) {
					return kuser1;
				}
				else if (k == (vt + 2) % 7) {
					return kuser2;
				}
				else if (k == (vt + 3) % 7) {
					return kuser3;
				}
				else if (k == (vt + 4) % 7) {
					return kuser4;
				}
				else if (k == (vt + 5) % 7) {
					return kuser5;
				}
				else if (k == (vt + 6) % 7) {
					return kuser6;
				}
				break;
			}
		}
	}
	else
		return -1;
}

std::string LayerCardLieng::getURL_byID_TienLen(string pid, string pType)
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
		case 0:
			url.append("s.png");
			break;
		case 1:
			url.append("c.png");
			break;
		case 2:
			url.append("d.png");
			break;
		case 3:
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
		case 0:
			url.append("bich");
			break;
		case 1:
			url.append("tep");
			break;
		case 2:
			url.append("ro");
			break;
		case 3:
			url.append("co");
			break;
		}
		url.append(".png");
		return url;
	}
}

void LayerCardLieng::setRejoinSizeCardMe()
{
	w_card_me = 90;
	h_card_me = 110;
}

void CardBaCay::GivePokerFindChuong(std::string _lcChuong)
{
	this->PlaySound("sounds/deal_card.mp3");
	//card_8_h;card_3_h
	this->_cardChuong = mUtils::splitString(_lcChuong, ';');
	log("Size %d", _cardChuong.size());

	string virtualList = this->ListUsers;// + this.ListUsers + this.ListUsers;
	auto arrVirtuals = mUtils::splitString(virtualList, ';');
	if (arrVirtuals.size() == 0)
		return;


	//! Số lần thực hiện chia bài.

	//! Chia bai
	for (int i = 0; i < 1; i++){
		int Count_Virtual_Users = i + 1;
		for (int j = 0; j < arrVirtuals.size(); j++){
			auto info = mUtils::splitString(arrVirtuals[j], '|');
			int pos = this->GetPosUserByName(info[0]);
			switch (pos) {
			case kuser0:
				this->GivePockerNextFindChuong(this->_cardChuong[j], w_card_me, h_card_me, xCardPos_me, yCardPos_me, Count_Virtual_Users);
				break;
			case kuser1:
				this->GivePockerNextFindChuong(this->_cardChuong[j], w_card_notme, h_card_notme, xCardPos_u1, yCardPos_u1, Count_Virtual_Users);
				break;
			case kuser2:
				this->GivePockerNextFindChuong(this->_cardChuong[j], w_card_notme, h_card_notme, xCardPos_u2, yCardPos_u2, Count_Virtual_Users);
				break;
			case kuser3:
				this->GivePockerNextFindChuong(this->_cardChuong[j], w_card_notme, h_card_notme, xCardPos_u3, yCardPos_u3, Count_Virtual_Users);
				break;
			case kuser4:
				this->GivePockerNextFindChuong(this->_cardChuong[j], w_card_notme, h_card_notme, xCardPos_u4, yCardPos_u4, Count_Virtual_Users);
				break;
			case kuser5:
				this->GivePockerNextFindChuong(this->_cardChuong[j], w_card_notme, h_card_notme, xCardPos_u5, yCardPos_u5, Count_Virtual_Users);
				break;
			case kuser6:
				this->GivePockerNextFindChuong(this->_cardChuong[j], w_card_notme, h_card_notme, xCardPos_u6, yCardPos_u6, Count_Virtual_Users);
				break;
			default:
				break;
			}
		}
	}
	//
	DelayTime* delay = DelayTime::create(Duration_Action * 3);
	CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(CardBaCay::callbackWhenGivePokerDone, this));
	this->runAction(Sequence::create(delay, callfunc, NULL));
}

void CardBaCay::GivePockerNextFindChuong(std::string _card, const float& _width, const float& _height, const float& _left, const float& _bottom, const int& count)
{
	Size _size = ChanUtils::getSizePos();
	//Card* pCard = new Card("card_back.png");
	Card* pCard = new Card(_card.c_str());
	pCard->autorelease();
	pCard->setPosition(Vec2((WIDTH_DESIGN / 2) - this->w_card_notme / 2, (HEIGHT_DESIGN / 4 * 3) - this->h_card_notme / 2 + _size.height - 30));
	pCard->SetScaleCard(w_card_notme / pCard->getContentSize().width, h_card_notme / pCard->getContentSize().height);
	pCard->setVisible(true);
	this->addChild(pCard);

	MoveTo *newTo = MoveTo::create(Duration_Action*count, Vec2(_left + _width, _bottom));
	ScaleBy *scaleTo = ScaleBy::create(Duration_Action*count, 1.7);

	pCard->runAction(newTo);
	pCard->runAction(scaleTo);
	//P.push_back(pCard);
	this->CardChuong.push_back(pCard);
}

void CardBaCay::resetCardChuong()
{
	if (0 == this->CardChuong.size()) return;
	for_each(this->CardChuong.begin(), this->CardChuong.end(), bind2nd(mem_fun(&Card::removeFromParentAndCleanup), true));
	this->CardChuong.clear();
}

void CardBaCay::setPositionCardMe(float posX, float posY)
{
	this->xCardPos_me = posX;
	this->yCardPos_me = posY;
}


