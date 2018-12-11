//
//  _LayerBet_.cpp
//  iCasino_v2
//
//  Created by DauA on 6/26/14.
//
//

#include "_LayerBet_.h"
#include "AllData.h"
#include "mUtils.h"
#include "ChanUtils.h"

BetGame3Cay::BetGame3Cay()
:LayerResult(NULL)
{
}

BetGame3Cay::~BetGame3Cay()
{
}

bool BetGame3Cay::init()
{
	if (!Layer::init())
		return false;

	this->setAnchorPoint(Point(0, 0));
	this->setPosition(Point(0, 0));

	FrameBet *bme = FrameBet::create();
	FrameBet *spec = FrameBet::create();
	FrameBet *buser1 = FrameBet::create();
	FrameBet *buser2 = FrameBet::create();
	FrameBet *buser3 = FrameBet::create();
	FrameBet *buser4 = FrameBet::create();
	FrameBet *buser5 = FrameBet::create();
	FrameBet *buser6 = FrameBet::create();
	//Size convert
	Size _size = ChanUtils::getSizePos();
	bme->setPosition(WIDTH_DESIGN / 2 - WIDTH_DESIGN / 7 - W_AVATAR / 3, HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 5 + 25.0);
	//bme->setScale(1);
	spec->setPosition(0, 0);
	buser1->setPosition(Vec2(WIDTH_DESIGN / 8 + W_MONEY_BG / 4, HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 7 + H_MONEY_BG / 2 + 10));
	//buser1->setScale(1);
	buser2->setPosition(Vec2(WIDTH_DESIGN / 8 + W_MONEY_BG / 2 - 15, HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 7 - (W_AVATAR + H_MONEY_BG + W_MONEY_BG / 2) / 2 + 15));
	//buser2->setScale(1);
	buser3->setPosition(Vec2(WIDTH_DESIGN / 2 - WIDTH_DESIGN / 4 - W_MONEY_BG / 2 + 25, HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 3 - W_AVATAR - H_MONEY_BG / 2 + 10));
	//buser3->setScale(1);
	buser4->setPosition(Vec2(WIDTH_DESIGN / 2 + WIDTH_DESIGN / 4 - W_MONEY_BG / 2 - 65, HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 3 - W_AVATAR - H_MONEY_BG / 2 + 10));
	//buser4->setScale(1);
	buser5->setPosition(Vec2(WIDTH_DESIGN / 9 * 8 - W_MONEY_BG - W_AVATAR / 2 - 35, HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 7 - (W_AVATAR + H_MONEY_BG + W_MONEY_BG / 2) / 2 + 15));
	//buser5->setScale(1);
	buser6->setPosition(Vec2(WIDTH_DESIGN / 9 * 8 - W_MONEY_BG * 5 / 4 - 35, HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 7 + H_MONEY_BG / 2 + 10));
	//buser6->setScale(1);

	bme->setTag(kuser0);
	spec->setTag(100);
	buser1->setTag(kuser1);
	buser2->setTag(kuser2);
	buser3->setTag(kuser3);
	buser4->setTag(kuser4);
	buser5->setTag(kuser5);
	buser6->setTag(kuser6);

	bme->setVisible(false);
	spec->setVisible(false);
	buser1->setVisible(false);
	buser2->setVisible(false);
	buser3->setVisible(false);
	buser4->setVisible(false);
	buser5->setVisible(false);
	buser6->setVisible(false);

	this->addChild(bme);
	this->addChild(spec);
	this->addChild(buser1);
	this->addChild(buser2);
	this->addChild(buser3);
	this->addChild(buser4);
	this->addChild(buser5);
	this->addChild(buser6);

	this->LayerResult = Layer::create();
	this->LayerResult->setAnchorPoint(Point(0, 0));
	this->LayerResult->setPosition(Point(0, 0));
	this->addChild(this->LayerResult);

	return true;
}

FrameBet* BetGame3Cay::GetFrameBetByPos(int pos)
{
	if (pos == -1)
		pos = 100;
	if (this->getChildByTag(pos) == NULL)
		return NULL;
	return (FrameBet*)this->getChildByTag(pos);
}

void BetGame3Cay::VisibleAllFrameBet()
{
	this->GetFrameBetByPos(kuser0)->setVisible(false);
	this->GetFrameBetByPos(kuser1)->setVisible(false);
	this->GetFrameBetByPos(kuser2)->setVisible(false);
	this->GetFrameBetByPos(kuser3)->setVisible(false);
	this->GetFrameBetByPos(kuser4)->setVisible(false);
	this->GetFrameBetByPos(kuser5)->setVisible(false);
	this->GetFrameBetByPos(kuser6)->setVisible(false);
}

void BetGame3Cay::SetResult4AllUser(const int& pos, string& result, string& score){
	float x = -1, y = -1;
	//Size convert
	Size _size = ChanUtils::getSizePos();
	switch (pos)
	{
	case kuser0:
		x = (WIDTH_DESIGN / 2);
		y = 140 + _size.height;
		break;
	case kuser1:
		x = WIDTH_DESIGN / 6;
		y = 220 + _size.height;
		break;
	case kuser2:
		x = WIDTH_DESIGN / 9;
		y = (HEIGHT_DESIGN / 2) - 30 + _size.height;
		break;
	case kuser3:
		x = WIDTH_DESIGN / 6;
		y = (HEIGHT_DESIGN - 280) + _size.height;
		break;
	case kuser4:
		x = WIDTH_DESIGN / 6 * 5;
		y = (HEIGHT_DESIGN - 280) + _size.height;
		break;
	case kuser5:
		x = WIDTH_DESIGN / 9 * 8;
		y = (HEIGHT_DESIGN / 2) - 30 + _size.height;
		break;
	case kuser6:
		x = WIDTH_DESIGN / 6 * 5;
		y = 220 + _size.height;
		break;
	}

	auto BG = Sprite::create("moc3-avatar-result.png");

	string txt1 = "";
	string txt2 = "";
	if (1 == atoi(result.c_str()))
	{
		vector<string> arr = mUtils::splitString(score, '|');
		if (arr.size() < 3) return;
		txt1 = arr[0];
		txt2 = arr[1] + " " + GetTricks(arr[2]);
		if (1 == atoi(arr[1].c_str())){
			txt1 = arr[0];
			txt2 = "Át " + GetTricks(arr[2]);
		}
	}
	auto KetQua1 = Label::createWithSystemFont(txt1, "", 45);
	KetQua1->setAnchorPoint(Vec2(0.5, 0.5));
	KetQua1->setPosition(Vec2(80, 100));
	KetQua1->setColor(Color3B(225, 101, 81));

	auto KetQua2 = Label::createWithSystemFont(txt2, "", 35);
	KetQua2->setAnchorPoint(Vec2(0.5, 0.5));
	KetQua2->setPosition(Vec2(72, 60));
	KetQua2->setColor(Color3B(225, 101, 81));

	BG->setPosition(Vec2(x, y));
	this->addChild(BG);
	BG->setTag(pos + 100);

	BG->addChild(KetQua1);
	BG->addChild(KetQua2);
}

Layer* BetGame3Cay::GetLayerResult(){
	return this->LayerResult;
}

string BetGame3Cay::GetTricks(string& trick)
{
	int itr = atoi(trick.c_str());
	switch (itr){
	case 0:
		return "Bích";
	case 1:
		return "Tép";
	case 2:
		return "Dô";
	case 3:
		return "Cơ";
	default:
		return "";
	}
	return "";
}

void BetGame3Cay::resetAllBets(int money)
{
	string _money = mUtils::convertMoneyEx(money);
	this->GetFrameBetByPos(kuser0)->SetValueBet(_money);
	this->GetFrameBetByPos(kuser1)->SetValueBet(_money);
	this->GetFrameBetByPos(kuser2)->SetValueBet(_money);
	this->GetFrameBetByPos(kuser3)->SetValueBet(_money);
	this->GetFrameBetByPos(kuser4)->SetValueBet(_money);
	this->GetFrameBetByPos(kuser5)->SetValueBet(_money);
	this->GetFrameBetByPos(kuser6)->SetValueBet(_money);
	for (int i = 0; i < 7; i++) {
		this->removeChildByTag(i + 100);
	}
}

Bien3Cay::Bien3Cay()
{

}

Bien3Cay::~Bien3Cay()
{

}

bool Bien3Cay::init()
{
	return true;
}

bool Bien3Cay::init(double _money)
{
	if (!Layer::init())
		return false;

	auto _visibleSize = Director::getInstance()->getVisibleSize();

	bme = LayerBien::create(_money, this->lstUser);
	//LayerBien *spec = LayerBien::create(_money);
	buser1 = LayerBien::create(_money, this->lstUser);
	buser2 = LayerBien::create(_money, this->lstUser);
	buser3 = LayerBien::create(_money, this->lstUser);
	buser4 = LayerBien::create(_money, this->lstUser);
	buser5 = LayerBien::create(_money, this->lstUser);
	buser6 = LayerBien::create(_money, this->lstUser);

	bme->setPosition(_visibleSize.width / 2 - _visibleSize.width / 7 + W_MONEY_BG / 2, _visibleSize.height / 2 - _visibleSize.height / 4 - H_MONEY_BG - 25.0);
	//spec->setPosition(0, 0);
	buser1->setPosition(Vec2(_visibleSize.width / 8 + W_MONEY_BG / 2 + H_MONEY_BG / 2 - 25.0, _visibleSize.height / 2 - _visibleSize.height / 7 - W_MONEY_BG + 35.0));
	buser2->setPosition(Vec2(_visibleSize.width / 8 + W_MONEY_BG / 2 + H_MONEY_BG / 2 - 25.0, _visibleSize.height / 2 + _visibleSize.height / 7 - H_MONEY_BG));
	buser3->setPosition(Vec2(_visibleSize.width / 2 - _visibleSize.width / 8 - W_MONEY_BG + H_MONEY_BG - 5.0, _visibleSize.height / 2 + _visibleSize.height / 3 - H_MONEY_BG - 25.0));
	buser4->setPosition(Vec2(_visibleSize.width / 2 + _visibleSize.width / 8 + 15.0, _visibleSize.height / 2 + _visibleSize.height / 3 - H_MONEY_BG - 25.0));
	buser5->setPosition(Vec2(_visibleSize.width / 9 * 8 - H_MONEY_BG / 2 - W_MONEY_BG + 25.0, _visibleSize.height / 2 + _visibleSize.height / 7 - H_MONEY_BG));
	buser6->setPosition(Vec2(_visibleSize.width / 9 * 8 - H_MONEY_BG / 2 - W_MONEY_BG + 25.0, _visibleSize.height / 2 - _visibleSize.height / 7 - W_MONEY_BG + 35.0));

	this->displayBien3cay(false);
	//setTag
	bme->setTag(kuser0);
	//spec->setTag(100);
	buser1->setTag(kuser1);
	buser2->setTag(kuser2);
	buser3->setTag(kuser3);
	buser4->setTag(kuser4);
	buser5->setTag(kuser5);
	buser6->setTag(kuser6);

	this->addChild(bme);
	//this->addChild(spec);
	this->addChild(buser1);
	this->addChild(buser2);
	this->addChild(buser3);
	this->addChild(buser4);
	this->addChild(buser5);
	this->addChild(buser6);

	return true;
}

Bien3Cay* Bien3Cay::create()
{
	Bien3Cay* pRet = new Bien3Cay();
	if (pRet && pRet->init()){
		pRet->autorelease();
		return pRet;
	}
	else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

Bien3Cay* Bien3Cay::create(double _money)
{
	Bien3Cay* pRet = new Bien3Cay();
	if (pRet && pRet->init(_money)){
		pRet->autorelease();
		return pRet;
	}
	else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

LayerBien* Bien3Cay::GetLayerBienByPos(int pos)
{
	if (this->getChildByTag(pos) == NULL)
		return NULL;
	return (LayerBien*)this->getChildByTag(pos);
}

void Bien3Cay::displayBien3cay(bool _display)
{
	bme->setVisible(false);
	/*spec->setVisible(false);*/
	buser1->setVisible(_display);
	buser2->setVisible(_display);
	buser3->setVisible(_display);
	buser4->setVisible(_display);
	buser5->setVisible(_display);
	buser6->setVisible(_display);
}

void Bien3Cay::resetAllBien()
{
	bme->resetBien();
	/*spec->setVisible(false);*/
	buser1->resetBien();
	buser2->resetBien();
	buser3->resetBien();
	buser4->resetBien();
	buser5->resetBien();
	buser6->resetBien();
}

void Bien3Cay::setListUser(std::string _lstUser)
{
	this->lstUser = _lstUser;
	this->bme->setArrList(_lstUser);
	this->buser1->setArrList(_lstUser);
	this->buser2->setArrList(_lstUser);
	this->buser3->setArrList(_lstUser);
	this->buser4->setArrList(_lstUser);
	this->buser5->setArrList(_lstUser);
	this->buser6->setArrList(_lstUser);
}
