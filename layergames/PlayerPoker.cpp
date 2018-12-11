#include "PlayerPoker.h"
#include "SimpleAudioEngine.h"
#include "_Number_.h"
#include "../Common.h"
#include "PhomMessDef.h"
#include "ChanUtils.h"
#define delay_show_money_time 2.5

PlayerPoker::PlayerPoker()
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

	log("PlayerPoker Constructor");
}

PlayerPoker::~PlayerPoker()
{
	this->ArrayCards.clear();
	log("PlayerPoker destructor");
}

bool PlayerPoker::init()
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

void PlayerPoker::ShowVicType(const string& type){
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

void PlayerPoker::AddCards(Card *pCard)
{
	this->ArrayCards.push_back(pCard);
}

void PlayerPoker::RemoveAllCards()
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

void PlayerPoker::ReceiveCardFirst()
{
	Card *pCard = new Card("card_back.png");
	pCard->autorelease();
	pCard->SetScaleCard(this->WithOrig / pCard->getContentSize().width, this->HeiOrig / pCard->getContentSize().height);
	pCard->setPosition(Vec2((WIDTH_DESIGN >> 1), HEIGHT_DESIGN /2+150));
	pCard->setVisible(true);
	this->addChild(pCard);

	MoveBy *newTo = MoveTo::create(0.1, Vec2(this->LeftCard + this->ArrayCards.size() * this->WidtdCard, this->BottomCard));
	ScaleBy* scaleTo = ScaleBy::create(0.1, this->WidtdCard / this->WithOrig, this->HeightCard / this->HeiOrig);

	this->ArrayCards.push_back(pCard);
	pCard->runAction(newTo);
	pCard->runAction(scaleTo);
	log("nhan 2 la bai dau tien");
}

void PlayerPoker::ReceiveCard(const std::string& allc, const string& url)
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
	CallFuncN* callfun = CallFuncN::create(CC_CALLBACK_1(PlayerPoker::RefreshListCards, this, a));
	pCard->runAction(Sequence::create(newTo, callfun, NULL));
}

void PlayerPoker::RestoreCard(string &listcards, bool hasopen)
{

		vector<string> ArrayIds = mUtils::splitString(listcards, ',');
		for (unsigned short i = 0; i < ArrayIds.size(); ++i) {
				int idcard = atoi(ArrayIds.at(i).c_str());
            if (idcard == -1){
                Card *pCard1 = new Card("card_back.png");
                pCard1->autorelease();
                pCard1->SetScaleCard(this->WidtdCard / pCard1->getContentSize().width, this->HeightCard / pCard1->getContentSize().height);
                pCard1->setPosition(this->PosCard());
                pCard1->setVisible(true);
                this->addChild(pCard1);
                this->ArrayCards.push_back(pCard1);
            }else{
                std::string url = XiToHelper::FindTypeCardPoker(idcard);
                Card* pCard = new Card(url.c_str());
                pCard->autorelease();
                pCard->SetScaleCard(this->WidtdCard / pCard->getContentSize().width, this->HeightCard / pCard->getContentSize().height);
                pCard->setPosition(this->PosCard());
                pCard->SetID(idcard);
                pCard->setVisible(true);
                this->addChild(pCard);
                this->ArrayCards.push_back(pCard);
            }
		}
}

void PlayerPoker::RefreshListCards(Ref* sender, string &listID)
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
				std::string url = XiToHelper::FindTypeCardPoker(atoi(arrID.at(this->ArrayCards.size() + i).c_str()));
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
			std::string url = XiToHelper::FindTypeCardPoker(idx);
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

bool PlayerPoker::CheckCardRunning()
{
	for (int i = 0; i < this->ArrayCards.size(); ++i){
		if (this->ArrayCards.at(i)->getNumberOfRunningActions() > 0)
			return true;
	}
	return false;
}

Vec2 PlayerPoker::PosCard()
{
	return Vec2(this->LeftCard + (this->ArrayCards.size() * this->WidtdCard ), this->BottomCard);
}

void PlayerPoker::SetIdListCards(string &lc)
{
	vector<string> arrID = mUtils::splitString(lc, ',');
	if (this->ArrayCards.size() >= arrID.size()) {
		for (int i = 0; i < this->ArrayCards.size(); ++i){
			try {
				int id = atoi(arrID.at(i).c_str());
				Card* pCard = this->ArrayCards.at(i);
				pCard->SetID(id);
				pCard->initWithSpriteFrameName(XiToHelper::FindTypeCardPoker(id));
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

void PlayerPoker::DisplayValueListCards(string &lc)
{
	if (this->IsFold)
		return;
//    if (0 != lc.compare("")) {
//        vector<string> arrID = mUtils::splitString(lc, ',');
//        for (std::vector<Card*>::iterator it = this->ArrayCards.begin(); it != this->ArrayCards.end(); ++it) {
//            for (unsigned short i = 0; i < arrID.size(); ++i) {
//                try {
//                    if ((*it)->GetID() == atoi(arrID.at(i).c_str())) {
//                        (*it)->setClicked(true);
//                    }
//                }
//                catch (std::out_of_range& e) {
//                   // log("out of range: %s", e.what());
//                }
//                catch (std::exception& e) {
//                   // log("exception: %s", e.what());
//                }
//                catch (...) {
//                }
//            }
//        }
//        arrID.clear();
//        for_each(this->ArrayCards.begin(), this->ArrayCards.end(), mem_fun(&Card::SetValuesXiTo));
//    }
//    auto lstC = mUtils::splitString(lc,',');
//    for (int i = 0; i< this->ArrayCards.size();i++){
//        this->ArrayCards[i]->setColor(Color3B(150, 150, 150));
//        ;
//    }
//    for (int i = 0 ; i < lstC.size();i++){
//        for (int j = 0; j< this->ArrayCards.size();j++){
//            if (atoi(lstC[i].c_str())==this->ArrayCards[j]->GetID())
//                this->ArrayCards[j]->setColor(Color3B::WHITE);
//        }
//    }
	//
}

void PlayerPoker::SetPosBeginCard(const float &left, const float &bottom)
{
    Size pos = ChanUtils::getSizePos();
	this->LeftCard = left;
	this->BottomCard = bottom+pos.height;
}

Vec2 PlayerPoker::GetPosBeginCard()
{
	return Vec2(this->LeftCard, this->BottomCard);
}

void PlayerPoker::SetSizeCard(const Size& size){
	this->WidtdCard = size.width;
	this->HeightCard = size.height;
}

float PlayerPoker::GetWitdCard() const
{
	return this->WidtdCard;
}

float PlayerPoker::GetHeightCard() const
{
	return this->HeightCard;
}

float PlayerPoker::GetLeftCard() const
{
	return this->LeftCard;
}

float PlayerPoker::GetBottomCard() const
{
	return this->BottomCard;
}

int PlayerPoker::GetCurrentTypeBet() const
{
	return this->CurrentTypeBet;
}

float PlayerPoker::GetCurrentValueBet() const
{
	return this->CurrentValueBet;
}

float PlayerPoker::GetTotalBet() const
{
	return this->TotalBet;
}

void PlayerPoker::SetAI(const string &aI)
{
	this->aI = aI;
}

string PlayerPoker::GetAI() const
{
	return this->aI;
}

Vec2 PlayerPoker::GetVicTypePosition()
{
	float x = this->LeftCard - this->WidtdCard / 2;
	float y = this->BottomCard + this->HeightCard / 2 + 4;
	return Vec2(x, y);
}

bool ReplaceStrings(std::string& str, const std::string& from, const std::string& to) {
	while (true){
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			break;
		str.replace(start_pos, from.length(), to);
	}
	return true;
}

void PlayerPoker::ShowMoney(const string &amf, bool b)
{
	string s = amf;
	ReplaceStrings(s, ",", ".");

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

	CallFuncN * callfun = CallFuncN::create(CC_CALLBACK_1(PlayerPoker::ShowMoneyCallFunc, this, "bet"));
	lbl->runAction(Sequence::create(MoveTo::create(0.5, Vec2(p.x, p.y + this->HeightCard + 40)),
		DelayTime::create(delay_show_money_time),
		callfun, NULL));

	this->arrAnimMoney.push_back(lbl);
}

void PlayerPoker::ShowMoneyCallFunc(Ref *sender, std::string data){
	Label* node = (Label*)sender;
	if (node != NULL){
		node->setVisible(false);
	}
}

void PlayerPoker::BoCuoc(){
	for (int i = 0; i < this->ArrayCards.size(); ++i){
		this->ArrayCards.at(i)->initWithSpriteFrameName("card_back.png");
	}
	this->lblVicType->setString(dataManager.GetSysString(511));
	this->lblVicType->setPosition(this->GetVicTypePosition());
	this->lblVicType->setVisible(true);
}

void PlayerPoker::ResetTurn()
{
	this->CurrentTypeBet = -1;
	this->CurrentValueBet = 0;
	this->TotalBet += this->CurrentValueBet;
}

PlayerPokerIsMe::PlayerPokerIsMe()
{
	log("PlayerPokerIsMe Constructor");
}

PlayerPokerIsMe::~PlayerPokerIsMe()
{
	log("PlayerPokerIsMe destructor");
}

bool PlayerPokerIsMe::init()
{
	if (!PlayerPoker::init()) return false;
	return true;
}

int PlayerPokerIsMe::DisplayTwoCard(string& listdeal)
{
	vector<string> deal = mUtils::splitString(listdeal, ',');
	try {
		int id = atoi(deal.at(0).c_str());
		log("id card 1 ======= %d", id);
		Card *pXiTo1 = this->ArrayCards.at(0);
		pXiTo1->initWithSpriteFrameName(XiToHelper::FindTypeCardPoker(id));
		pXiTo1->SetID(id);

		id = atoi(deal.at(1).c_str());
		log("id card 2 ======= %d", id);
		Card *pXiTo2 = this->ArrayCards.at(1);
		pXiTo2->initWithSpriteFrameName(XiToHelper::FindTypeCardPoker(id));
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

void PlayerPokerIsMe::MoveDealCard(const int &idCard)
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

void PlayerPokerIsMe::RestoreListCard(string &listcards, bool hasopen)
{
	this->RestoreCard(listcards, hasopen);
//    this->ArrayCards.at(0)->setColor(Color3B(150, 250, 250));
//    if (this->ArrayCards.size() == 5) {
//        try {
//            Card* c = this->ArrayCards.back();
//            if (c) {
//                c->setColor(Color3B(150, 250, 250));
//            }
//        }
//        catch (...) {
//
//        }
//    }
}

void PlayerPokerIsMe::UpBai()
{

}

void PlayerPokerIsMe::CreateFrameBet(const float&x, const float& y, const int& sign)
{
    Size pos = ChanUtils::getSizePos();

	this->spFrameBetMe = FrameBet::create();
    this->spFrameBetMe->setPosition(Vec2(630, 230+pos.height));

	this->spFrameBetMe->setVisible(false);
	this->addChild(this->spFrameBetMe);
}

void PlayerPokerIsMe::RefreshListCards(Ref* sender, std::string& listID){
	PlayerPoker::RefreshListCards(sender, listID);
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

void PlayerPokerIsMe::SetBet(const long &bettype, const double &betvalues, int pos)
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
			str = formatMoneySymbol(betvalues);
		}
		else {
			str =  mUtils::convertMoneyEx((long long)betvalues);
		}

        Vec2 posMoney;
        
        switch (pos) {
            case USER_ME:
                posMoney = Vec2(550, 270);
                break;
            case USER_LEFT_BOT:
                posMoney = Vec2(230, (HEIGHT_DESIGN >> 1) - 100);
                break;
            case USER_RIGHT_BOT:
                posMoney = Vec2(WIDTH_DESIGN - 230, (HEIGHT_DESIGN >> 1) - 120);
                break;
            case USER_LEFT_TOP:
                posMoney = Vec2(350, 830);
                break;
            case USER_RIGHT_TOP:
                posMoney = Vec2(WIDTH_DESIGN - 350, 830);
                break;
        }
        for (int i = 0; i < 4;i++){
            Sprite* chip = Sprite::create("zen-min.png");
            this->addChild(chip);
            chip->setPosition(posMoney);
            chip->runAction(Sequence::create(DelayTime::create(0.1*i), MoveTo::create(0.2, Vec2(posMoney.x,posMoney.y+120+3*i)),NULL));
            lstChip.push_back(chip);
        }
        
		this->spFrameBetMe->SetValueBet(str);
		this->spFrameBetMe->setVisible(true);
	}
}

void PlayerPokerIsMe::HideFrameBet()
{
	if (this->spFrameBetMe != NULL)
		this->spFrameBetMe->setVisible(false);
}

void PlayerPokerIsMe::SetBetBeforeServer(const double &bet)
{
	this->lblBetMe->setString((dataManager.GetSysString(32)+" " + mUtils::convertMoneyEx((long long)bet)).c_str());
	this->spFrameBetMe->setVisible(true);
    if (!mUtils::isSoundOn()) return;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/chip_moving.mp3");
}
void PlayerPokerIsMe::removeChip(){
    for (int i = 0 ; i < this->lstChip.size();i++){
        this->lstChip[i]->removeFromParent();
    }
    this->lstChip.clear();
}
void PlayerPokerIsMe::moveChipToPos(){
    for (int i = 0 ; i < this->lstChip.size();i++){
        this->lstChip[i]->runAction(Sequence::create( MoveTo::create(0.4, Vec2(960,700)),RemoveSelf::create(true),NULL));
    }
    this->lstChip.clear();

}
void PlayerPokerNormal::moveChipToPos(){
    for (int i = 0 ; i < this->lstChip.size();i++){
        this->lstChip[i]->runAction(Sequence::create( MoveTo::create(0.4, Vec2(960,750)),RemoveSelf::create(true),NULL));
    }
    this->lstChip.clear();
}
void PlayerPokerNormal::removeChip(){
    for (int i = 0 ; i < this->lstChip.size();i++){
        this->lstChip[i]->removeFromParent();
    }
    this->lstChip.clear();
}
PlayerPokerNormal::PlayerPokerNormal()
{
	log("PlayerPokerNormal Constructor");
}

PlayerPokerNormal::~PlayerPokerNormal()
{
	log("PlayerPokerNormal destructor");
}

bool PlayerPokerNormal::init()
{
	if (!PlayerPoker::init()) return false;
	return true;
}

void PlayerPokerNormal::MoveDealCard(const int &idCard)
{
	try {
		Card *pCard = this->ArrayCards.at(1);
		pCard->initWithSpriteFrameName(XiToHelper::FindTypeCardPoker(idCard));

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

void PlayerPokerNormal::RestoreListCard(string &listcards, bool hasopen)
{
	this->RestoreCard(listcards, hasopen);
}

void PlayerPokerNormal::UpBai()
{

}

void PlayerPokerNormal::CreateFrameBet(const float&x, const float& y, const int& sign)
{
    Size pos = ChanUtils::getSizePos();

	this->frameBet = FrameBet::create();
	switch (sign){

	case USER_LEFT_BOT:
		this->frameBet->setPosition(Vec2(290, 300+pos.height));
		break;

	case USER_LEFT_TOP:
		this->frameBet->setPosition(Vec2(420, 700+pos.height));
		break;

	case USER_RIGHT_BOT:
		this->frameBet->setPosition(Vec2(WIDTH_DESIGN - 310 - this->frameBet->GetKc_Width(), 300+pos.height));
		break;

	case USER_RIGHT_TOP:
		this->frameBet->setPosition(Vec2(WIDTH_DESIGN - 410 - this->frameBet->GetKc_Width() , 700+pos.height));
		break;
	}
    this->frameBet->setVisible(false);
	this->addChild(this->frameBet);
}

void PlayerPokerNormal::SetBet(const long &bettype, const double &betvalues, int pos)
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

        Vec2 posMoney;
        
        switch (pos) {
            case USER_ME:
                posMoney = Vec2(550, 270);
                break;
            case USER_LEFT_BOT:
                posMoney = Vec2(230, (HEIGHT_DESIGN >> 1) - 100);
                break;
            case USER_RIGHT_BOT:
                posMoney = Vec2(WIDTH_DESIGN - 230, (HEIGHT_DESIGN >> 1) - 120);
                break;
            case USER_LEFT_TOP:
                posMoney = Vec2(350, 830);
                break;
            case USER_RIGHT_TOP:
                posMoney = Vec2(WIDTH_DESIGN - 350, 830);
                break;
        }
        for (int i = 0; i < 4;i++){
            Sprite* chip = Sprite::create("zen-min.png");
            this->addChild(chip);
            chip->setPosition(posMoney);
            chip->runAction(Sequence::create(DelayTime::create(0.1*i), MoveTo::create(0.2, Vec2(posMoney.x,posMoney.y+120+3*i)),NULL));
            lstChip.push_back(chip);
        }
        
		this->frameBet->SetValueBet(str);
		this->frameBet->setVisible(true);
        if (!mUtils::isSoundOn()) return;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/chip_moving.mp3");
	}
}

void PlayerPokerNormal::HideFrameBet()
{
	if (this->frameBet != NULL)
		this->frameBet->setVisible(false);
}
