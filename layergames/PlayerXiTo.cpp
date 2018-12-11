#include "PlayerXiTo.h"
#include "SimpleAudioEngine.h"
#include "_Number_.h"
#include "../Common.h"
#include "PhomMessDef.h"

#define delay_show_money_time 2.5

PlayerXiTo::PlayerXiTo()
{
	this->WithOrig = 67;
	this->HeiOrig = 87;

	this->LeftCard = 0;
	this->BottomCard = 0;

	this->WidtdCard = 0;
	this->HeightCard = 0;

	this->lblVicType = NULL;

	this->IsFold = false;

	this->ArrayCards.clear();
	this->arrAnimMoney.clear();

	log("PlayerXiTo Constructor");
}

PlayerXiTo::~PlayerXiTo()
{
	this->ArrayCards.clear();
	log("PlayerXiTo destructor");
}

bool PlayerXiTo::init()
{
	if (!Layer::init()) return false;
	this->bgVicType = ImageView::create("moc3-bg-victype.png");
	this->bgVicType->setAnchorPoint(Vec2(0, 0));

	this->lblVicType = Label::createWithSystemFont("","", 25);
	this->lblVicType->setColor(Color3B::WHITE);
	this->lblVicType->setAnchorPoint(Vec2(0.5, 0.5));
	this->lblVicType->setScale(1.2);
	this->lblVicType->setVisible(false);
	this->addChild(this->bgVicType);
	this->addChild(this->lblVicType);
	this->bgVicType->setVisible(false);

	return true;
}

void PlayerXiTo::ShowVicType(const string& type){
	if (this->IsFold)
	{
		this->lblVicType->setString(dataManager.GetSysString(230));
	}
	else
	{
		this->lblVicType->setString(type);
	}

	this->bgVicType->setPosition(this->GetVicTypePosition());
	this->lblVicType->setPosition(Point(this->bgVicType->getPositionX() + this->bgVicType->getContentSize().width / 2, this->bgVicType->getPositionY() + this->bgVicType->getContentSize().height / 2));
	this->lblVicType->setVisible(true);
	this->bgVicType->setVisible(true);
}

void PlayerXiTo::AddCards(Card *pCard)
{
	this->ArrayCards.push_back(pCard);
}

void PlayerXiTo::RemoveAllCards()
{
	if (0 == this->ArrayCards.size()) return;
	for_each(this->ArrayCards.begin(), this->ArrayCards.end(), std::bind2nd(mem_fun(&Card::removeFromParentAndCleanup), true));
	this->ArrayCards.clear();
	//An frame bet
	this->HideFrameBet();
	this->CurrentTypeBet = -1;
	this->CurrentValueBet = 0;
	this->TotalBet = 0;
	this->IsFold = false;
	this->lblVicType->setVisible(false);
	this->bgVicType->setVisible(false);

	for (int i = 0; i < this->arrAnimMoney.size(); ++i){
		Label* label = this->arrAnimMoney.at(i);
		if (label != NULL){
			label->removeFromParentAndCleanup(true);
		}
	}
	this->arrAnimMoney.clear();
}

void PlayerXiTo::ReceiveCardFirst()
{
	Card *pCard = new Card("card_back.png");
	pCard->autorelease();
	pCard->SetScaleCard(this->WithOrig / pCard->getContentSize().width, this->HeiOrig / pCard->getContentSize().height);
	pCard->setPosition(Vec2((WIDTH_DESIGN >> 1), HEIGHT_DESIGN - this->HeiOrig / 2));
	pCard->setVisible(true);
	this->addChild(pCard);

	MoveBy *newTo = MoveTo::create(0.3, Vec2(this->LeftCard + this->ArrayCards.size() * this->WidtdCard, this->BottomCard));
	ScaleBy* scaleTo = ScaleBy::create(0.3, this->WidtdCard / this->WithOrig, this->HeightCard / this->HeiOrig);

	this->ArrayCards.push_back(pCard);
	pCard->runAction(newTo);
	pCard->runAction(scaleTo);
	log("nhan 2 la bai dau tien");
}

void PlayerXiTo::ReceiveCard(const std::string& allc, const string& url)
{
	Card *pCard = new Card(url.c_str());
	pCard->autorelease();
	pCard->SetScaleCard(this->WithOrig / pCard->getContentSize().width, this->HeiOrig / pCard->getContentSize().height);
	pCard->setPosition(Vec2((WIDTH_DESIGN >> 1), HEIGHT_DESIGN - this->HeiOrig / 2));
	pCard->setVisible(true);
	pCard->setLocalZOrder(6);
	this->addChild(pCard);

	MoveTo* newTo = MoveTo::create(0.3, this->PosCard());
	ScaleBy* scaleTo = ScaleBy::create(0.3, this->WidtdCard / this->WithOrig, this->HeightCard / this->HeiOrig);

	// Add tai vi tri nay
	this->ArrayCards.push_back(pCard);

	string a = allc;
	this->sListCardMe = allc;

	pCard->runAction(scaleTo);
	CallFuncN* callfun = CallFuncN::create(CC_CALLBACK_1(PlayerXiTo::RefreshListCards, this, a));
	pCard->runAction(Sequence::create(newTo, callfun, NULL));
}

void PlayerXiTo::RestoreCard(string &listcards, bool hasopen)
{
	if (hasopen) {
		//Card 1
		Card *pCard1 = new Card("card_back.png");
		pCard1->autorelease();
		pCard1->SetScaleCard(this->WidtdCard / pCard1->getContentSize().width, this->HeightCard / pCard1->getContentSize().height);
		pCard1->setPosition(Vec2(this->LeftCard, this->BottomCard));
		pCard1->setVisible(true);
		this->addChild(pCard1);
		this->ArrayCards.push_back(pCard1);

		//Card 2
		Card *pCard2 = new Card("card_back.png");
		pCard2->autorelease();
		pCard2->SetScaleCard(this->WidtdCard / pCard2->getContentSize().width, this->HeightCard / pCard2->getContentSize().height);
		pCard2->setPosition(Vec2(this->LeftCard + this->WidtdCard / 3 * 2, this->BottomCard));
		pCard2->setVisible(true);
		this->addChild(pCard2);
		this->ArrayCards.push_back(pCard2);
	}

	//! Đã lật con bài tẩy
	else {
		vector<string> ArrayIds = mUtils::splitString(listcards, ',');
		for (unsigned short i = 0; i < ArrayIds.size(); ++i) {
			try {
				int idcard = atoi(ArrayIds.at(i).c_str());
				std::string url = XiToHelper::FindTypeCard(idcard);
				Card* pCard = new Card(url.c_str());
				pCard->autorelease();
				pCard->SetScaleCard(this->WidtdCard / pCard->getContentSize().width, this->HeightCard / pCard->getContentSize().height);
				pCard->setPosition(this->PosCard());
				pCard->SetID(idcard);
				pCard->setVisible(true);
				this->addChild(pCard);
				this->ArrayCards.push_back(pCard);
			}
			catch (std::out_of_range &e) {
				log("RestoreListCard_Reconnected out_of_range exception: %s", e.what());
			}
			catch (std::exception &e) {
				log("RestoreListCard_Reconnected std::exception %s", e.what());
			}
			catch (...) {
				log("RestoreListCard_Reconnected unknown exception");
			}
		}
	}
}

void PlayerXiTo::RefreshListCards(Ref* sender, string &listID)
{
	vector<string> arrID = mUtils::splitString(listID, ',');
	if (0 == arrID.size()) return;

	//! Neu list card > list id server tra ve
	if (this->ArrayCards.size() > arrID.size()) {
		while (this->ArrayCards.size() > arrID.size()) {
			Card* pCard = this->ArrayCards.back();
			this->ArrayCards.pop_back();
			if (pCard){
				pCard->setVisible(false);
				pCard->removeFromParentAndCleanup(true);
			}
		}
	}

	//! Neu List Card nho < List Id tra ve tu server
	if (this->ArrayCards.size() > 0 && this->ArrayCards.size() < arrID.size()) {
		float scaleX = -1;
		float scaleY = -1;
		try {
			Card* pCard = this->ArrayCards.at(0);
			scaleX = pCard->GetScaleCardX();
			scaleY = pCard->GetScaleCardY();
		}
		catch (std::out_of_range &e) {
			log("out_of_range exception: %s", e.what());
			return;
		}
		catch (std::exception &e) {
			return;
		}
		catch (...) {
			return;
		}

		if (scaleX == -1 && scaleY == -1){
			return;
		}

		int n = arrID.size() - this->ArrayCards.size();
		for (int i = 0; i < n; ++i) {
			try {
				std::string url = XiToHelper::FindTypeCard(atoi(arrID.at(this->ArrayCards.size() + i).c_str()));
				Card* pCard = new Card(url.c_str());
				pCard->autorelease();
				pCard->SetScaleCard(scaleX, scaleY);
				pCard->setPosition(Vec2(this->LeftCard + (this->ArrayCards.size() + i) * this->WidtdCard / 3 * 2, this->BottomCard));
				this->addChild(pCard);
				this->ArrayCards.push_back(pCard);
			}
			catch (std::out_of_range &e) {
				log("22 out_of_range exception: %s", e.what());
			}
			catch (std::exception &e) {
				log("22 std::exception %s", e.what());
			}
			catch (...) {
				log("22 unknown exception");
			}
		}
	}

	//!Refresh tai day.
	for (int i = 0; i < arrID.size(); ++i) {
		try {
			int idx = atoi(arrID.at(i).c_str());
			std::string url = XiToHelper::FindTypeCard(idx);
			Card* pCard = this->ArrayCards.at(i);
			pCard->initWithSpriteFrameName(url);
			pCard->setLocalZOrder(i);
			pCard->setPositionX(this->LeftCard + i * this->WidtdCard / 3 * 2);
			pCard->SetID(idx);
		}
		catch (std::out_of_range &e) {
			log("out_of_range exception: %s", e.what());
		}
		catch (std::exception &e) {
			log("std::exception %s", e.what());
		}
		catch (...) {
			log("unknown exception");
		}
	}

	//! Set Opacity cho la bai dau tien
	if (0 != arrID.at(0).compare("-1") && this->ArrayCards.size() > 0) {
		this->ArrayCards.at(0)->setColor(Color3B(150, 250, 250));
	}
}

bool PlayerXiTo::CheckCardRunning()
{
	for (int i = 0; i < this->ArrayCards.size(); ++i){
		if (this->ArrayCards.at(i)->getNumberOfRunningActions() > 0)
			return true;
	}
	return false;
}

Vec2 PlayerXiTo::PosCard()
{
	return Vec2(this->LeftCard + (this->ArrayCards.size() * this->WidtdCard / 3 * 2), this->BottomCard);
}

void PlayerXiTo::SetIdListCards(string &lc)
{
	vector<string> arrID = mUtils::splitString(lc, ',');
	if (this->ArrayCards.size() >= arrID.size()) {
		for (int i = 0; i < this->ArrayCards.size(); ++i){
			try {
				int id = atoi(arrID.at(i).c_str());
				Card* pCard = this->ArrayCards.at(i);
				pCard->SetID(id);
				pCard->initWithSpriteFrameName(XiToHelper::FindTypeCard(id));
			}
			catch (std::out_of_range& e) {
				log("out of range: %s", e.what());
			}
			catch (std::exception& e) {
				log("exception: %s", e.what());
			}
			catch (...) {
			}
		}
	}
}

void PlayerXiTo::DisplayValueListCards(string &lc)
{
	if (this->IsFold)
		return;
	if (0 != lc.compare("")) {
		vector<string> arrID = mUtils::splitString(lc, ',');
		for (std::vector<Card*>::iterator it = this->ArrayCards.begin(); it != this->ArrayCards.end(); ++it) {
			for (unsigned short i = 0; i < arrID.size(); ++i) {
				try {
					if ((*it)->GetID() == atoi(arrID.at(i).c_str())) {
						(*it)->setClicked(true);
					}
				}
				catch (std::out_of_range& e) {
					log("out of range: %s", e.what());
				}
				catch (std::exception& e) {
					log("exception: %s", e.what());
				}
				catch (...) {
				}
			}
		}
		arrID.clear();
		for_each(this->ArrayCards.begin(), this->ArrayCards.end(), mem_fun(&Card::SetValuesXiTo));
	}
	//
}

void PlayerXiTo::SetPosBeginCard(const float &left, const float &bottom)
{
	this->LeftCard = left;
	this->BottomCard = bottom;
}

Vec2 PlayerXiTo::GetPosBeginCard()
{
	return Vec2(this->LeftCard, this->BottomCard);
}

void PlayerXiTo::SetSizeCard(const Size& size){
	this->WidtdCard = size.width;
	this->HeightCard = size.height;
}

float PlayerXiTo::GetWitdCard() const
{
	return this->WidtdCard;
}

float PlayerXiTo::GetHeightCard() const
{
	return this->HeightCard;
}

float PlayerXiTo::GetLeftCard() const
{
	return this->LeftCard;
}

float PlayerXiTo::GetBottomCard() const
{
	return this->BottomCard;
}

int PlayerXiTo::GetCurrentTypeBet() const
{
	return this->CurrentTypeBet;
}

float PlayerXiTo::GetCurrentValueBet() const
{
	return this->CurrentValueBet;
}

float PlayerXiTo::GetTotalBet() const
{
	return this->TotalBet;
}

void PlayerXiTo::SetAI(const string &aI)
{
	this->aI = aI;
}

string PlayerXiTo::GetAI() const
{
	return this->aI;
}

Vec2 PlayerXiTo::GetVicTypePosition()
{
	float x = this->LeftCard - this->WidtdCard / 2;
	float y = this->BottomCard + this->HeightCard / 2 + 4;
	return Vec2(x, y);
}

bool replaceString(std::string& str, const std::string& from, const std::string& to) {
	while (true){
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			break;
		str.replace(start_pos, from.length(), to);
	}
	return true;
}

void PlayerXiTo::ShowMoney(const string &amf, bool b)
{
	string s = amf;
	replaceString(s, ",", ".");

	Label* lbl;
	if (b) {
		lbl = Label::createWithBMFont("font_win-font.fnt", s);
	}
	else {
		lbl = Label::createWithBMFont("font_lose-font.fnt", s);
	}

	lbl->setLocalZOrder(100);
	lbl->setAnchorPoint(Vec2(0, 0));

	Vec2 p = Vec2(this->LeftCard - this->WidtdCard / 2, this->BottomCard - this->HeightCard / 2);

	lbl->setPosition(p);
	this->addChild(lbl);

	for (int i = 0; i < this->arrAnimMoney.size(); ++i) {
		try {
			Label* obj = this->arrAnimMoney.at(i);
			if (obj != NULL) {
				obj->setVisible(false);
			}
		}
		catch (...) {

		}
	}

	CallFuncN * callfun = CallFuncN::create(CC_CALLBACK_1(PlayerXiTo::ShowMoneyCallFunc, this, "bet"));
	lbl->runAction(Sequence::create(MoveTo::create(0.5, Vec2(p.x, p.y + this->HeightCard + 40)),
		DelayTime::create(delay_show_money_time),
		callfun, NULL));

	this->arrAnimMoney.push_back(lbl);
}

void PlayerXiTo::ShowMoneyCallFunc(Ref *sender, std::string data){
	Label* node = (Label*)sender;
	if (node != NULL){
		node->setVisible(false);
	}
}

void PlayerXiTo::BoCuoc(){
	for (int i = 0; i < this->ArrayCards.size(); ++i){
		this->ArrayCards.at(i)->initWithSpriteFrameName("card_back.png");
	}
    this->lblVicType->setString(dataManager.GetSysString(511));
	this->lblVicType->setPosition(this->GetVicTypePosition());
	this->lblVicType->setVisible(true);
}

void PlayerXiTo::ResetTurn()
{
	this->CurrentTypeBet = -1;
	this->CurrentValueBet = 0;
	this->TotalBet += this->CurrentValueBet;
}

PlayerXiToIsMe::PlayerXiToIsMe()
{
	log("PlayerXiToIsMe Constructor");
}

PlayerXiToIsMe::~PlayerXiToIsMe()
{
	log("PlayerXiToIsMe destructor");
}

bool PlayerXiToIsMe::init()
{
	if (!PlayerXiTo::init()) return false;
	return true;
}

int PlayerXiToIsMe::DisplayTwoCard(string& listdeal)
{
	vector<string> deal = mUtils::splitString(listdeal, ',');
	try {
		int id = atoi(deal.at(0).c_str());
		log("id card 1 ======= %d", id);
		Card *pXiTo1 = this->ArrayCards.at(0);
		pXiTo1->initWithSpriteFrameName(XiToHelper::FindTypeCard(id));
		pXiTo1->SetID(id);

		id = atoi(deal.at(1).c_str());
		log("id card 2 ======= %d", id);
		Card *pXiTo2 = this->ArrayCards.at(1);
		pXiTo2->initWithSpriteFrameName(XiToHelper::FindTypeCard(id));
		pXiTo2->SetID(id);

	}
	catch (std::out_of_range &e) {
		log("Chia2LaBaiDauTien_2 out_of_range exception: %s", e.what());
		return -1;
	}
	catch (std::exception &e) {
		log("Chia2LaBaiDauTien_2 std::exception %s", e.what());
		return -1;
	}
	catch (...) {
		log("Chia2LaBaiDauTien_2 unknown exception");
		return -1;
	}
	return 0;
}

void PlayerXiToIsMe::MoveDealCard(const int &idCard)
{
	if (2 != this->ArrayCards.size()) return;

	// Nếu lá bài thứ nhất mở thì hoán đổi ra để hiện thị.
	if (idCard == (*this->ArrayCards.begin())->GetID()) {
		iter_swap(this->ArrayCards.begin(), this->ArrayCards.begin() + 1);
	}

	Card* card1st = this->ArrayCards.at(0);
	Card* card2nd = this->ArrayCards.at(1);

	card1st->setColor(Color3B(250, 250, 250));
	card2nd->setColor(Color3B(250, 250, 250));

	card1st->setColor(Color3B(150, 250, 250));
	card1st->setLocalZOrder(0);
	card2nd->setLocalZOrder(1);

	MoveTo* move1 = MoveTo::create(0.4, Vec2(this->LeftCard, this->BottomCard));
	MoveTo* move2 = MoveTo::create(0.4, Vec2(this->LeftCard + this->WidtdCard / 3 * 2, this->BottomCard));

	card1st->runAction(move1);
	card2nd->runAction(move2);
}

void PlayerXiToIsMe::RestoreListCard(string &listcards, bool hasopen)
{
	this->RestoreCard(listcards, hasopen);
	this->ArrayCards.at(0)->setColor(Color3B(150, 250, 250));
	if (this->ArrayCards.size() == 5) {
		try {
			Card* c = this->ArrayCards.back();
			if (c) {
				c->setColor(Color3B(150, 250, 250));
			}
		}
		catch (...) {

		}
	}
}

void PlayerXiToIsMe::UpBai()
{

}

void PlayerXiToIsMe::CreateFrameBet(const float&x, const float& y, const int& sign)
{
	Size size = Size(235, 64);
	this->spFrameBetMe = Sprite::create("xito_total_center.png");
	this->spFrameBetMe->setPosition(Vec2(WIDTH_DESIGN >> 1, (HEIGHT_DESIGN >> 1) + 100));

	this->lblBetMe = Label::createWithSystemFont("","", 25);
	this->lblBetMe->setColor(Color3B::BLACK);

	this->lblBetMe->setPosition(Vec2(size.width / 2, size.height / 2));
	this->spFrameBetMe->addChild(this->lblBetMe);

	this->spFrameBetMe->setVisible(false);
	this->addChild(this->spFrameBetMe);
}

void PlayerXiToIsMe::RefreshListCards(Ref* sender, std::string& listID){
	PlayerXiTo::RefreshListCards(sender, listID);
	if (this->ArrayCards.size() == 5){
		try{
			Card* c = this->ArrayCards.back();
			if (c){
				c->setColor(Color3B(150, 250, 250));
			}
		}
		catch (...){

		}
	}
}

void PlayerXiToIsMe::SetBet(const long &bettype, const double &betvalues)
{
	this->CurrentTypeBet = (int)bettype;
	this->CurrentValueBet = betvalues;

	if (0 == bettype) {
		//for_each(this->ArrayCards.begin(), this->ArrayCards.end(), std::bind2nd(mem_fun(&Card::setOpacity), 180));
		for (int i = 0; i < this->ArrayCards.size(); ++i){
			this->ArrayCards.at(i)->initWithSpriteFrameName("card_back.png");
		}
		this->spFrameBetMe->setVisible(false);
		this->lblVicType->setString(dataManager.GetSysString(230));

		this->IsFold = true;

	}
	else if (bettype == GAME_TABLE_STATUS_BET_NONE) {
		this->spFrameBetMe->setVisible(false);
	}

	else {

		std::string str;
        if (betvalues > 999999) {
            str = (dataManager.GetSysString(32)+" " + formatMoneySymbol(betvalues));
        }
        else {
            str = (dataManager.GetSysString(32)+" " + mUtils::convertMoneyEx((long long)betvalues));
        }

		this->lblBetMe->setString(str);
		this->spFrameBetMe->setVisible(true);
	}
}

void PlayerXiToIsMe::HideFrameBet()
{
	if (this->spFrameBetMe != NULL)
		this->spFrameBetMe->setVisible(false);
}

void PlayerXiToIsMe::SetBetBeforeServer(const double &bet)
{
    this->lblBetMe->setString((dataManager.GetSysString(32)+" " + mUtils::convertMoneyEx((long long)bet)).c_str());
	this->spFrameBetMe->setVisible(true);
    if (mUtils::isSoundOn())
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/chip_moving.mp3");
}

PlayerXiToNormal::PlayerXiToNormal()
{
	log("PlayerXiToNormal Constructor");
}

PlayerXiToNormal::~PlayerXiToNormal()
{
	log("PlayerXiToNormal destructor");
}

bool PlayerXiToNormal::init()
{
	if (!PlayerXiTo::init()) return false;
	return true;
}

void PlayerXiToNormal::MoveDealCard(const int &idCard)
{
	try {
		Card *pCard = this->ArrayCards.at(1);
		pCard->initWithSpriteFrameName(XiToHelper::FindTypeCard(idCard));

		for (int i = 0; i < this->ArrayCards.size(); ++i){
			Card* pCard = this->ArrayCards.at(i);
			MoveTo* move = MoveTo::create(0.3, Vec2(this->LeftCard + i * (this->WidtdCard / 3 * 2), this->BottomCard));
			pCard->runAction(move);
		}
	}
	catch (std::out_of_range &e) {
		log("MoveDealCard normal out_of_range exception: %s", e.what());
	}
	catch (std::exception &e) {
		log("MoveDealCard normal std::exception %s", e.what());
	}
	catch (...) {
		log("MoveDealCard normal unknown exception");
	}
}

void PlayerXiToNormal::RestoreListCard(string &listcards, bool hasopen)
{
	this->RestoreCard(listcards, hasopen);
}

void PlayerXiToNormal::UpBai()
{

}

void PlayerXiToNormal::CreateFrameBet(const float&x, const float& y, const int& sign)
{
	this->frameBet = FrameBet::create();
	switch (sign){

	case USER_LEFT_BOT:
		this->frameBet->setPosition(Vec2(x, y));
		break;

	case USER_LEFT_TOP:
		this->frameBet->setPosition(Vec2(x - 50, y + 20));
		break;

	case USER_RIGHT_BOT:
		this->frameBet->setPosition(Vec2(WIDTH_DESIGN - x - this->frameBet->GetKc_Width(), y));
		break;

	case USER_RIGHT_TOP:
		this->frameBet->setPosition(Vec2(WIDTH_DESIGN - x - this->frameBet->GetKc_Width() + 50, y + 20));
		break;
	}
	this->frameBet->setVisible(false);
	this->addChild(this->frameBet);
}

void PlayerXiToNormal::SetBet(const long &bettype, const double &betvalues)
{
	this->CurrentTypeBet = (int)bettype;
	this->CurrentValueBet = betvalues;
	if (0 == bettype) {
		std::string back = "card_back.png";
		for_each(this->ArrayCards.begin(), this->ArrayCards.end(), std::bind2nd(mem_fun(&Card::initWithSpriteFrameName), back));

		this->frameBet->setVisible(false);
		this->lblVicType->setString(dataManager.GetSysString(230));

		this->IsFold = true;

	}
	else if (bettype == GAME_TABLE_STATUS_BET_NONE) {
		this->frameBet->setVisible(false);
	}
	else {

		std::string str;
		if (betvalues > 999999) {
			str = (formatMoneySymbol(betvalues));
		}
		else {
			str = (mUtils::convertMoneyEx((long long)betvalues));
		}

		this->frameBet->SetValueBet(str);
		this->frameBet->setVisible(true);
        if (mUtils::isSoundOn())
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/chip_moving.mp3");
	}
}

void PlayerXiToNormal::HideFrameBet()
{
	if (this->frameBet != NULL)
		this->frameBet->setVisible(false);
}
