#include "TaiXiuBet.h"
#include "TaiXiuMessageDefs.h"
#include "mUtils.h"
#include "GameServer.h"
#include "../Common.h"

SuggestTaiXiu::SuggestTaiXiu()
: lblTotal(NULL), lblPlayerBet(NULL){

}

SuggestTaiXiu::~SuggestTaiXiu(){

}

bool SuggestTaiXiu::init(){
	if (!ImageView::init())
		return false;

	return true;
}

void SuggestTaiXiu::loadTexture(const std::string &fileName){
	ImageView::loadTexture(fileName);

	this->lblTotal = Label::createWithTTF("",kNormalFont , 30);
	this->lblTotal->setAnchorPoint(Vec2(0, 0));
	this->addChild(this->lblTotal);

	this->lblPlayerBet = Label::createWithTTF("",kNormalFont , 30);
	this->lblPlayerBet->setAnchorPoint(Vec2(0, 0));
	this->addChild(this->lblPlayerBet);
}

void SuggestTaiXiu::SetInfo(long totalbet, long playerbet){
	float x = this->getContentSize().width;
	float y = this->getContentSize().height;

	string s;
	if (totalbet < 1000000000) {
		s = dataManager.GetSysString(746)+": " + mUtils::convertMoneyEx(totalbet);
	}
	else {
		s = dataManager.GetSysString(659)+" : " + mUtils::convertMoneyEx(totalbet);
	}

	this->lblTotal->setPosition(Vec2(x / 2 - 150, y / 2 + 10));
	this->lblTotal->setString(s);

	string d;
	if (playerbet < 1000000000) {
		d = dataManager.GetSysString(513)+" " + mUtils::convertMoneyEx(playerbet);
	}
	else {
		d = dataManager.GetSysString(514)+" " + mUtils::convertMoneyEx(playerbet);
	}

	this->lblPlayerBet->setPosition(Vec2(x / 2 - 150, y / 2 - 35));
	this->lblPlayerBet->setString(d);
}

TaiXiuBet::TaiXiuBet(Button *item, std::string &name, const int &tag, std::string &type)
{
	this->pButton = item;
	this->Name = name;
	this->Tag = tag;
	this->Type = type;
	this->totalBet = 0;
	this->playerBet = 0;
	this->pButton->setTag(this->Tag);
	this->arrCoin.clear();
	this->Suggest = NULL;
	this->__Pos = Vec2(30, 30);
	this->__WorldPos = Vec2(0, 0);
	this->_target = NULL;
	this->_mySelected = false;
	//log("init container button %s", this->Name.c_str());
}

TaiXiuBet::~TaiXiuBet()
{
	this->arrCoin.clear();
	//log("destructor Tai Xiu %s", this->Type.c_str());
}

std::string TaiXiuBet::GetType() const
{
	return this->Type;
}

int TaiXiuBet::GetTag() const
{
	return this->Tag;
}

void TaiXiuBet::SetSuggest(SuggestTaiXiu* suggest) {
	this->Suggest = suggest;
}
void TaiXiuBet::DisplayCurrentBet() {
	this->Suggest->SetInfo(this->totalBet, this->playerBet);
	this->Suggest->setVisible(true);
}

void TaiXiuBet::HideCurrentBet() {
	this->Suggest->setVisible(false);
}

void TaiXiuBet::SetPositionCoin(const Vec2& pos){
	this->__Pos = pos;
}

Button *TaiXiuBet::GetMenu() const
{
	return this->pButton;
}

void TaiXiuBet::SetImage(const std::string &pNormal, const std::string &pSelect, const std::string &active, const std::string& eat)
{
	this->pImageNormal = pNormal;
	this->pImageSelected = pSelect;
	this->pImageActive = active;
	this->pImageEat = eat;
	this->pButton->loadTextureNormal(pNormal);
	this->pButton->loadTexturePressed(pSelect);
}

void TaiXiuBet::Selected()
{
	this->pButton->loadTextureNormal(this->pImageSelected);
}

void TaiXiuBet::CanEat(Vec2 _dest, Vec2 _pos){
	if (this->_mySelected){
		this->pButton->loadTextureNormal(this->pImageEat);
		//this->processAnimationWin(_dest, _pos);
		this->pButtonPos = this->pButton->getPosition();
	}
	else{
		this->pButton->loadTextureNormal(this->pImageSelected);
	}
}

void TaiXiuBet::CanEat()
{
	if (this->_mySelected){
		this->pButton->loadTextureNormal(this->pImageEat);
		this->setIsAnimation(true);
	}
	else{
		this->pButton->loadTextureNormal(this->pImageSelected);
	}
}

void TaiXiuBet::UnSelected()
{
	this->pButton->loadTextureNormal(this->pImageNormal);
}

void TaiXiuBet::MySelect()
{
	this->_mySelected = true;
	this->pButton->loadTextureNormal(this->pImageActive);
}

void TaiXiuBet::SetTotalBet(const long &totalbet)
{
	this->totalBet = totalbet;
	this->Suggest->SetInfo(this->totalBet, this->playerBet);
}

void TaiXiuBet::SetPlayerBet(const long &mybet)
{
	this->playerBet += mybet;
	if (this->totalBet < this->playerBet) {
		this->totalBet = this->playerBet;
	}
	this->Suggest->SetInfo(this->totalBet, this->playerBet);
}

void TaiXiuBet::IncreTotalBet(const long& add){
	this->totalBet += add;
	if (this->totalBet < this->playerBet) {
		this->totalBet = this->playerBet;
	}
	this->Suggest->SetInfo(this->totalBet, this->playerBet);
}

void TaiXiuBet::ClientBet(const long& money){
	this->playerBet += money;
	this->totalBet += money;
	this->Suggest->SetInfo(this->totalBet, this->playerBet);
}

void TaiXiuBet::SetTarget(Node* target){
	this->_target = target;
}

void TaiXiuBet::Reset()
{
	this->setIsAnimation(false);
	this->pButton->loadTextureNormal(this->pImageNormal);
	//this->pButton->setColor(Color3B::WHITE);
	this->playerBet = 0;
	this->totalBet = 0;
	this->_mySelected = false;
	for (int i = 0; i < this->arrCoin.size(); ++i){
		ui::ImageView* img = this->arrCoin.at(i);
		img->removeFromParentAndCleanup(true);
	}
	this->arrCoin.clear();
}

const std::vector<ui::ImageView*>& TaiXiuBet::GetArrayCoin() const{
	return this->arrCoin;
}

void TaiXiuBet::AddCoin(ImageView *img)
{
	//Vec2 pos = this->__WorldPos;
	this->arrCoin.push_back(img);
//    if (this->totalBet > 5){
//        this->ExecuteRefresh();
//    }
}

void TaiXiuBet::SetWorldPos(const Vec2& pos){
	this->__WorldPos.x = pos.x;
	this->__WorldPos.y = pos.y;
}

Vec2 TaiXiuBet::GetWorldPos(){
	//long ix = this->arrCoin.size();
	Vec2 pp = Vec2(0, 0);
	pp.x = this->__WorldPos.x;
    pp.y = this->__WorldPos.y ;//+ ix * 8;
	return pp;
}
Vec2 TaiXiuBet::getPointCoin(){
    Size s;
    string betType = this->GetType();
    if (betType == BET_TYPE_CHAN || betType == BET_TYPE_LE){
        s = Size(130,100);
    }
    if (betType == BET_TYPE_CAP_0_4 || betType == BET_TYPE_CAP_1_3 || betType == BET_TYPE_CAP_3_1 || betType == BET_TYPE_CAP_4_0){
        s = Size(110,110);
    }
    
    Vec2 p = Vec2(this->GetWorldPos().x-s.width/2,this->GetWorldPos().y-s.height/2);
    int width = s.width;
    int height = s.height;
    if (betType == BET_TYPE_CHAN || betType == BET_TYPE_LE){
        height = s.height+100;
    }
    int randomX = rand() % width + p.x;
    int randomY = rand() % height + p.y;
    
    return Vec2(randomX,randomY);
}
void TaiXiuBet::ExecuteRefresh() {
	std::for_each(this->arrCoin.begin(), this->arrCoin.end(), std::mem_fun(&ui::ImageView::removeFromParent));
	this->arrCoin.clear();

	Vec2 pos = this->__WorldPos;
	int count = 0;
    
    

	
	if (totalBet % 2000 == 0 && totalBet < 10000){
		int totalCount = totalBet / 2000;
		for (int i = 0; i < totalCount; i++)
		{
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-2k.png");
			count++;
		}
		
	}
	else{
		if (this->totalBet > 5 && this->totalBet < 100) {
			for (int i = 0; i < 5; i++) {
				this->reAddCoinMove(count, getPointCoin(), "tx-chip-01B.png");
				count++;
			}
			return;
		}


		int count5m = this->totalBet / 5000000;
		for (int i = 0; i < count5m; i++) {
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-08.png");
			count++;
		}

		int remain5m = this->totalBet % 5000000;
		int count1m = remain5m / 1000000;
		for (int i = 0; i < count1m; i++) {
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-07.png");
			count++;
		}

		int remain1m = remain5m % 1000000;
		int count500k = remain1m / 500000;
		for (int i = 0; i < count500k; i++) {
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-06.png");
			count++;
		}

		int remain500k = remain1m % 500000;
		int count100k = remain500k / 100000;
		for (int i = 0; i < count100k; i++) {
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-05.png");
			count++;
		}

		int remain100k = remain500k % 100000;
		int count50k = remain100k / 50000;
		for (int i = 0; i < count50k; i++) {
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-04.png");
			count++;
		}

		int remain50k = remain100k % 50000;
		int count10k = remain50k / 10000;
		for (int i = 0; i < count10k; i++) {
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-03.png");
			count++;
		}

		int remain10k = remain50k % 10000;
		int count5k = remain10k / 5000;
		for (int i = 0; i < count5k; i++) {
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-02.png");
			count++;
		}

		int remain5k = remain10k % 5000;
		int count1k = remain5k / 1000;
		for (int i = 0; i < count1k; i++) {
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-01.png");
			count++;
		}
		int remain1k = remain5k % 1000;
		int count100 = remain1k / 500;
		for (int i = 0; i < count100; i++) {
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-500B.png");
			count++;
		}
		int remain500 = remain1k % 500;
		int count500 = remain500 / 100;
		for (int i = 0; i < count500; i++) {
			this->reAddCoinMove(count, getPointCoin(), "tx-chip-100B.png");
			count++;
		}
	}
}

void TaiXiuBet::reAddCoinMove(const int& count, const Vec2& posfirst, const char* src) {
	ui::ImageView* chip = ui::ImageView::create();
	chip->loadTexture(src);
	chip->setScale(0.5);
	chip->setPosition(Vec2(posfirst.x, posfirst.y + count * 8));
	this->_target->addChild(chip);
	this->arrCoin.push_back(chip);
}

const long &TaiXiuBet::GetTotalBet() const
{
	return this->totalBet;
}

const long &TaiXiuBet::GetPlayerBet() const
{
	return this->playerBet;
}

//void TaiXiuBet::processAnimationWin(Vec2 _dest, Vec2 _pos)
//{
//	auto parent = this->getParent();
//
//	vector<Sprite*> coinsArr;
//	string pathImg = "moc3-icon-hao.png";
//	if (SceneManager::getSingleton().getCurrRoomType() == 0)//gold
//	{
//		pathImg = "moc3-icon-moc.png";
//	}
//	for (int i = 0; i < 5; i++)
//	{
//		Sprite* coins = Sprite::create(pathImg);
//		coins->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//		coins->setPosition(_pos);
//		coins->setLocalZOrder(244);
//		parent->addChild(coins);
//		coinsArr.push_back(coins);
//	}
//	CallFuncN* callBack = CallFuncN::create([&](Node* pSender){
//		pSender->removeFromParentAndCleanup(true);
//	});
//	for (unsigned int i = 0; i < coinsArr.size(); i++)
//	{
//		float _delay = 0.01f + 0.03f*i;
//		MoveTo * moveTo = MoveTo::create(0.2 + _delay, _dest);
//		ScaleTo* scaleTo = ScaleTo::create(0.2 + _delay, 0.5);
//		DelayTime* delayAction = DelayTime::create(_delay);
//		Spawn* spawnAction = Spawn::create(moveTo, scaleTo, NULL);
//		coinsArr.at(i)->runAction(Sequence::create(delayAction, spawnAction, callBack, NULL));
//	}
//}

cocos2d::Vec2 TaiXiuBet::getpButtonPos()
{
	return pButtonPos;
}

void TaiXiuBet::setIsAnimation(bool _animation)
{
	this->isAnimation = _animation;
}

bool TaiXiuBet::getIsAnimation()
{
	return isAnimation;
}

TXBetTaiXiu::TXBetTaiXiu(Button* item, std::string& name, const int& tag, std::string& type)
: TaiXiuBet(item, name, tag, type){
}

TXBetTaiXiu::~TXBetTaiXiu() {
}

Vec2 TXBetTaiXiu::GetPostoMoveCoin() {
	return Vec2(this->pButton->getPosition().x - 10, this->pButton->getPosition().y - 20);
}

TXBetBoDoi::TXBetBoDoi(Button* item, std::string& name, const int& tag, std::string& type)
: TaiXiuBet(item, name, tag, type){
}

TXBetBoDoi::~TXBetBoDoi() {
}

Vec2 TXBetBoDoi::GetPostoMoveCoin() {
	return Vec2(this->pButton->getPositionX() + 5, this->pButton->getPositionY() - 5);
}

TXBetBoBa::TXBetBoBa(Button* item, std::string& name, const int& tag, std::string& type)
: TaiXiuBet(item, name, tag, type){
}

TXBetBoBa::~TXBetBoBa() {
}

Vec2 TXBetBoBa::GetPostoMoveCoin() {
	return Vec2(this->pButton->getPositionX() - 5, this->pButton->getPositionY() - 5);
}

TXBetAnyBa::TXBetAnyBa(Button* item, std::string& name, const int& tag, std::string& type)
: TaiXiuBet(item, name, tag, type){
}

TXBetAnyBa::~TXBetAnyBa() {
}

Vec2 TXBetAnyBa::GetPostoMoveCoin() {
	return Vec2(this->pButton->getPositionX(), this->pButton->getPositionY());
}

TXBetDiem::TXBetDiem(Button* item, std::string& name, const int& tag, std::string& type)
: TaiXiuBet(item, name, tag, type){
}

TXBetDiem::~TXBetDiem() {
}

Vec2 TXBetDiem::GetPostoMoveCoin() {
	return Vec2(this->pButton->getPositionX() - 5, this->pButton->getPositionY());
}

TXBetCapDoi::TXBetCapDoi(Button* item, std::string& name, const int& tag, std::string& type)
: TaiXiuBet(item, name, tag, type){
}

TXBetCapDoi::~TXBetCapDoi() {
}

Vec2 TXBetCapDoi::GetPostoMoveCoin() {
	return Vec2(this->pButton->getPositionX(), this->pButton->getPositionY() - 5);
}

TXBetDonLe::TXBetDonLe(Button* item, std::string& name, const int& tag, std::string& type)
: TaiXiuBet(item, name, tag, type){
}

TXBetDonLe::~TXBetDonLe() {
}

Vec2 TXBetDonLe::GetPostoMoveCoin() {
	return Vec2(this->pButton->getPositionX() + this->pButton->getContentSize().width / 2, this->pButton->getPositionY() - this->pButton->getContentSize().height / 2);
}

TaiXiuBet *TaiXiuBetFactory::CreateTXBet(const int &group, Button *button, const char* name_, const int &tag, const char* type_)
{
	std::string name = boost::to_string(name_);
	std::string type = boost::to_string(type_);
	switch (group){
	case VALUE_TAIXIU:
		return new TXBetTaiXiu(button, name, tag, type);
	case VALUE_BODOI:
		return new TXBetBoDoi(button, name, tag, type);
	case VALUE_BOBA:
		return new TXBetBoBa(button, name, tag, type);
	case VALUE_ANYBA:
		return new TXBetAnyBa(button, name, tag, type);
	case VALUE_CAPDOI:
		return new TXBetCapDoi(button, name, tag, type);
	case VALUE_DIEM:
		return new TXBetDiem(button, name, tag, type);
	case VALUE_DONLE:
		return new TXBetDonLe(button, name, tag, type);
	}
	return NULL;
}
