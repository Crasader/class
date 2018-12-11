//
//  _Number_inGame_.cpp
//  iCasino_v2
//
//  Created by Thanhhv on 5/16/14.
//
//

#include "_Number_inGame_.h"
#include "PhomMessDef.h"
#include "_Chat_.h"
LayerNumberInGame::~LayerNumberInGame() {
	log("~~~~LayerNumberInGame");
	arrPos.clear();
	arrNumberString.clear();
}
bool LayerNumberInGame::init() {
	if (!Layer::init()) return false;
	numScale = 0.6;
	this->setAnchorPoint(Point(0, 0));
	this->isRunning = false;
	sizeAd = mUtils::getSizePos();
	return true;
}

void LayerNumberInGame::showNumberByPos(int pos, string numberString) {
	if (pos == -1) return;
	long num = atol(numberString.c_str());
	int len = numberString.size();
	if (len > 7)
		numScale = 0.5;
	else if (len > 9)
		numScale = 0.4;
	bool isA = num < 0;
	num = num < 0 ? -1 * num : num;
	numberString = mUtils::convertMoneyEx(num);
	if (isA)
		numberString = "-" + numberString;
	else
		numberString = "+" + numberString;
	this->removeChildByTag(pos);
	arrPos.push_back(pos);
	arrNumberString.push_back(numberString);

	if (!isRunning) {
		isRunning = true;
		//this->scheduleOnce(schedule_selector(LayerNumberInGame::runAction), 0.5);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerNumberInGame::runAction), this, 0.5, 1, 0.5, false);
	}
}

void LayerNumberInGame::showNumberByPos(int pos, double numberDouble) {
	arrPos.push_back(pos);
	arrNumberDouble.push_back(numberDouble);

	if (!isRunning) {
		isRunning = true;
		//this->scheduleOnce(schedule_selector(LayerNumberInGame::runAction), 0.5);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerNumberInGame::runAction), this, TIME_RESET_GAME, 1, TIME_RESET_GAME, false);
	}
}

void LayerNumberInGame::runAction(float dt) {
	int demMe = 0;
	int demLeft = 0;
	int demRight = 0;
	int demTop = 0;
	Size winSize = Director::getInstance()->getVisibleSize();
	for (int i = 0; i < arrPos.size(); i++)
	{
		int pos = arrPos.at(i);
		string numberString = arrNumberString.at(i);

		Number *number = Number::create(numberString);
		number->setTag(pos);
		number->setScale(numScale);
		Point point;

		switch (pos) {
		case kUserMe:
			point.setPoint(W_AVATAR + sizeAd.width - number->getSize().width*numScale / 2 - AVATAR_L_SPACE,
				demMe * number->getSize().height + 200 + sizeAd.height + SIZE_ADD);
			demMe++;
			break;
		case kUserBot:
			point.setPoint(winSize.width / 2 - number->getSize().width*numScale,
				demMe * number->getSize().height + 200 + sizeAd.height + SIZE_ADD);
			demMe++;
			break;

		case kUserTop:
			point.setPoint(winSize.width / 2 - number->getSize().width*numScale,
				HEIGHT_DESIGN - W_AVATAR*1.5 + demTop * number->getSize().height + sizeAd.height);
			demTop++;
			break;

		case kUserLeft:
			point.setPoint(W_AVATAR + sizeAd.width - number->getSize().width*numScale / 2 - AVATAR_L_SPACE,
				winSize.height / 2 + demLeft * number->getSize().height + SIZE_ADD);
			demLeft++;
			break;

		case kUserRight:
			point.setPoint(winSize.width - W_AVATAR - number->getSize().width*numScale / 2 - AVATAR_L_SPACE ,
				winSize.height / 2 + demRight * number->getSize().height + SIZE_ADD);
			demRight++;
			break;
		}
		number->setPositionStart(point);
		this->addChild(number);
	}

	arrPos.clear();
	arrNumberString.clear();

	this->isRunning = false;
	//this->scheduleOnce(schedule_selector(LayerNumberInGame::callbackShowNumber), 3);
}

void LayerNumberInGame::callbackShowNumber(float dt) {
	isRunning = false;
}

void LayerNumberInGame::showChatByPos(int pos, string strMsg)
{
	Size winSize = Director::getInstance()->getVisibleSize();
	Chat* message = Chat::create(strMsg, pos);
	message->setTag(246);
	Vec2 point;
	switch (pos)
	{
	case kUserMe:
		point = Vec2(winSize.width / 2,
			HEIGHT_DESIGN / 2.5 + sizeAd.height);
		break;
	case kUserBot:
		point = Vec2(WIDTH_DESIGN / 2 - message->getSize().width / 2 + sizeAd.width,
			HEIGHT_DESIGN / 4 + sizeAd.height + SIZE_ADD);
		break;
	case kUserLeft:
		point = Vec2(AVATAR_L_SPACE + W_AVATAR + sizeAd.width - message->getSize().width / 2,
			winSize.height / 2 + W_AVATAR / 2 + SIZE_ADD);
		break;
	case kUserRight:
		point = Vec2(WIDTH_DESIGN - AVATAR_L_SPACE - W_AVATAR - sizeAd.width - message->getSize().width / 2,
			winSize.height / 2 + W_AVATAR / 2 + SIZE_ADD);
		break;
	case kUserTop:
		point = Vec2(WIDTH_DESIGN / 2 - message->getSize().width / 2 + sizeAd.width,
			HEIGHT_DESIGN - W_AVATAR + sizeAd.height);
		break;
	default:
		point = Vec2(WIDTH_DESIGN / 2 - message->getSize().width / 2 + sizeAd.width,
			HEIGHT_DESIGN / 4 + sizeAd.height);
		break;
	}
	message->setPosThis(point);
	this->addChild(message);
}

void LayerNumberInGame::showNumberByPosForChess(int pos, double numberDouble)
{
	arrPos.push_back(pos);
	arrNumberDouble.push_back(numberDouble);

	if (!isRunning) {
		isRunning = true;
		//this->scheduleOnce(schedule_selector(LayerNumberInGame::runAction2), 0.5);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerNumberInGame::runAction2), this, 0.5, 1, 0.5, false);
	}
}

void LayerNumberInGame::showNumberByPosForChess(int pos, string numberString)
{
	long num = atol(numberString.c_str());
	int len = numberString.length();
	numScale = 0.6;
	if (len > 7)
		numScale = 0.5;
	else if (len > 9)
		numScale = 0.4;

	bool isA = num < 0;
	num = num < 0 ? -1 * num : num;
	numberString = mUtils::convertMoneyEx(num);
	if (isA)
		numberString = "-" + numberString;
	else
		numberString = "+" + numberString;

	this->arrPos.push_back(pos);
	this->arrNumberString.push_back(numberString);

	if (!isRunning) {
		isRunning = true;
		//this->scheduleOnce(schedule_selector(LayerNumberInGame::runAction2), 0.5);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerNumberInGame::runAction2), this, 0.5, 1, 0.5, false);
	}
}

void LayerNumberInGame::showChatByPosForChess(int pos, string strMsg)
{
	Chat* message = Chat::create(strMsg, pos);
	Size winSize = Director::getInstance()->getVisibleSize();
	Vec2 point;
	switch (pos)
	{
	case kuser0://thang o tren
		point = Vec2(WIDTH_DESIGN / 10 + sizeAd.width,
			HEIGHT_DESIGN / 2 + W_AVATAR * 2 + sizeAd.height);
		break;
	case kuser1://thang o duoi
		point = Vec2(WIDTH_DESIGN / 10 + sizeAd.width,
			HEIGHT_DESIGN / 2 - W_AVATAR + sizeAd.height);
		break;
	}
	message->setPosThis(point);
	this->addChild(message);
}
void LayerNumberInGame::runAction2(float dt) {
	Size sizeAd = mUtils::getSizePos();
	for (int i = 0; i < arrPos.size(); i++)
	{
		int pos = arrPos.at(i);
		string numberString = arrNumberString.at(i);

		Number *pNumber = Number::create(numberString);
		pNumber->setScale(numScale);
		Point point;

		switch (pos) {
		case kuser0://thang o tren
			point = Vec2(WIDTH_DESIGN / 4 - pNumber->getSize().width + sizeAd.width,
				HEIGHT_DESIGN / 2 + W_AVATAR * 2 + sizeAd.height);
			break;
		case kuser1://thang o duoi
			point = Vec2(WIDTH_DESIGN / 4 - pNumber->getSize().width + sizeAd.width,
				HEIGHT_DESIGN / 2 - W_AVATAR + sizeAd.height);
			break;
		}
		pNumber->setPositionStart(point);
		this->addChild(pNumber);
	}
	arrPos.clear();
	arrNumberString.clear();

	this->isRunning = false;
}
