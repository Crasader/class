
#include "_Number_Chat_inGame7u.h"
#include "_Chat_.h"
#include "mUtils.h"
#include <string.h>
#include "ChanUtils.h"

Number_Chat_inGame7u::Number_Chat_inGame7u()
    :IsRunning(false) {
    arrPos.clear();
    arrNumberString.clear();
    arrNumberDouble.clear();
}

Number_Chat_inGame7u::~Number_Chat_inGame7u() {
    arrPos.clear();
    arrNumberString.clear();
    arrNumberDouble.clear();
}

bool Number_Chat_inGame7u::init() {
    if (!Layer::init())
        return false;

    this->setAnchorPoint(Point(0, 0));
    return true;
}

void Number_Chat_inGame7u::ShowNumberByPos(const int& pos, string numberString) {
    long num = atol(numberString.c_str());
	int len = numberString.length();
	numScale = 0.6;
	if (len > 7)
		numScale=0.5;
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

    if (!IsRunning) {
        this->IsRunning = true;
        //this->scheduleOnce(schedule_selector(Number_Chat_inGame7u::runAction), 1);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(Number_Chat_inGame7u::runAction), this, 1, 1, 1, false);
    }
}

void Number_Chat_inGame7u::ShowNumberByPos(const int& pos, const double& numberDouble) {
    arrPos.push_back(pos);
    arrNumberDouble.push_back(numberDouble);

    if (!this->IsRunning) {
        this->IsRunning = true;
        //this->scheduleOnce(schedule_selector(Number_Chat_inGame7u::runAction2), 0.5);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(Number_Chat_inGame7u::runAction2), this, 0.5, 1, 0.5, false);
    }
}

void Number_Chat_inGame7u::runAction(float dt) {
    int demMe = 0;
    int demUser1 = 0;
    int demUser2 = 0;
    int demUser3 = 0;
    int demUser4 = 0;
    int demUser5 = 0;
    int demUser6 = 0;
    for (int i = 0; i < this->arrPos.size(); ++i) {
        int pos = this->arrPos.at(i);
        string numberString = this->arrNumberString.at(i);

        Number *number = Number::create(numberString);
        auto numScale = 0.6;
        number->setScale(numScale);
        Point point;
		//Size convert
		Size _size = ChanUtils::getSizePos();
        switch (pos) {
            case kuser0:
                point.setPoint((WIDTH_DESIGN >> 1) - number->getSize().width * numScale / 2, 170+demMe* number->getSize().height+_size.height);
                demMe++;
                break;
            case kuser1:
                point.setPoint(WIDTH_DESIGN/6 - number->getSize().width * numScale / 2, 240+demUser1* number->getSize().height+_size.height);
                demUser1++;
                break;
            case kuser2:
                point.setPoint(WIDTH_DESIGN/9 - number->getSize().width * numScale / 2, 540+demUser2* number->getSize().height+_size.height);
                demUser2++;
                break;
            case kuser3:
                point.setPoint(WIDTH_DESIGN/6 - number->getSize().width * numScale / 2, (HEIGHT_DESIGN - 250)+demUser3* number->getSize().height+_size.height);
                demUser3++;
                break;
            case kuser4:
                point.setPoint(WIDTH_DESIGN/6*5 - number->getSize().width * numScale / 2, (HEIGHT_DESIGN - 250)+demUser4* number->getSize().height+_size.height);
                demUser4++;
                break;
            case kuser5:
                point.setPoint(WIDTH_DESIGN/9*8 - number->getSize().width * numScale / 2, 540+demUser5* number->getSize().height+_size.height);
                demUser5++;
                break;
            case kuser6:
                point.setPoint(WIDTH_DESIGN/6*5 - number->getSize().width * numScale / 2, 240+demUser6* number->getSize().height+_size.height);
                demUser6++;
                break;
        }
        number->setPositionStart(point);
        this->addChild(number);
    }

    this->arrPos.clear();
    this->arrNumberString.clear();
    this->IsRunning = false;
}

void Number_Chat_inGame7u::runAction2(float dt) {

    for (int i = 0; i < arrPos.size(); ++i) {
        int pos = arrPos.at(i);
        double numberDouble = this->arrNumberDouble.at(i);
       // log("--- %u", numberDouble);
        Number *pNumber = Number::create(numberDouble);
		pNumber->setScale(numScale);

        Point point;
		//Size convert
		Size _size = ChanUtils::getSizePos();
		switch (pos) {
		case kuser0:
			point = Vec2((WIDTH_DESIGN >> 1) - pNumber->getSize().width * numScale / 2, 50+_size.height);
			break;
		case kuser1:
			point = Vec2(80 - pNumber->getSize().width * numScale / 2, 110+_size.height);
			break;
		case kuser2:
			point = Vec2(80 - pNumber->getSize().width * numScale / 2, 280+_size.height);
			break;
		case kuser3:
			point = Vec2(250 - pNumber->getSize().width * numScale / 2, 360+_size.height);
			break;
		case kuser4:
			point = Vec2(490 - pNumber->getSize().width * numScale / 2, 360+_size.height);
			break;
		case kuser5:
			point = Vec2(670 - pNumber->getSize().width * numScale / 2, 280+_size.height);
			break;
		case kuser6:
			point = Vec2(670 - pNumber->getSize().width * numScale / 2, 110+_size.height);
			break;
		}
        pNumber->setPositionStart(point);
        this->addChild(pNumber);
    }

    this->arrPos.clear();
    this->arrNumberDouble.clear();
    this->IsRunning = false;
}

void Number_Chat_inGame7u::callbackShowNumber(float dt) {
    this->IsRunning = false;
}

void Number_Chat_inGame7u::ShowChatByPos(const int& pos, const char* strMsg) {
    string  sMess = string(strMsg);
    this->ShowChatByPos(pos, sMess);
}

void Number_Chat_inGame7u::ShowChatByPos(const int& pos, string& strMsg) {
    Chat *Message = Chat::create(strMsg, kuser0);
    Message->setStatusByServer(false);
	int W_AVATAR = 143;
	Point point;
	//Size convert
	Size _size = ChanUtils::getSizePos();
    switch (pos) {
	case kuser0:
		point = Vec2((WIDTH_DESIGN / 2) - (Message->getSize().width) / 2, 120+_size.height);
		break;
	case kuser1:
		point = Vec2(W_AVATAR - (Message->getSize().width) / 2, HEIGHT_DESIGN / 2+_size.height);
		break;
	case kuser2:
		point = Vec2((Message->getSize().width) / 2, 400+_size.height);
		break;
	case kuser3:
		point = Vec2(W_AVATAR * 2 - +Message->getSize().width / 2, HEIGHT_DESIGN / 2 + W_AVATAR * 2+_size.height);
		break;
	case kuser4:
		point = Vec2(400 + Message->getSize().width / 2, 420+_size.height);
		break;
	case kuser5:
		point = Vec2(WIDTH_DESIGN - Message->getSize().width, 400+_size.height);
		break;
	case kuser6:
		point = Vec2(WIDTH_DESIGN - Message->getSize().width, 230+_size.height);
		break;

	default:
		point = Vec2((WIDTH_DESIGN - Message->getSize().width) / 2, (HEIGHT_DESIGN - Message->getSize().height) / 2+_size.height);

		break;
    }
    Message->setPosition(point);
    this->addChild(Message);
}

void Number_Chat_inGame7u::ShowNumberByPosTCC(const int& pos, string numberString)
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

	if (!IsRunning) {
		this->IsRunning = true;
		//this->scheduleOnce(schedule_selector(Number_Chat_inGame7u::runAction), 1);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(Number_Chat_inGame7u::runAction), this, 1, 1, 1, false);
	}
}

void Number_Chat_inGame7u::runActionTCC(float dt)
{
	for (int i = 0; i < arrPos.size(); ++i) {
		int pos = arrPos.at(i);
		double numberDouble = this->arrNumberDouble.at(i);
		log("--- %u", numberDouble);
		Number *pNumber = Number::create(numberDouble);
		pNumber->setScale(numScale);

		Point point;
		//Size convert
		Size _size = ChanUtils::getSizePos();
		switch (pos) {
		case kuser0:
			point = Vec2(WIDTH_DESIGN / 4 - pNumber->getSize().width * numScale / 2, HEIGHT_DESIGN / 2 + 143);
			break;
		case kuser1:
			point = Vec2(80 - pNumber->getSize().width * numScale / 2, 300);
			break;
		case kuser2:
			point = Vec2(WIDTH_DESIGN / 4 - pNumber->getSize().width * numScale / 2, HEIGHT_DESIGN / 4 + 143);
			break;
		case kuser3:
			point = Vec2(WIDTH_DESIGN - 100 - pNumber->getSize().width * numScale / 2, 420);
			break;
		case kuser4:
			point = Vec2(WIDTH_DESIGN - 130 - pNumber->getSize().width * numScale / 2, 300);
			break;
		case kuser5:
			point = Vec2(WIDTH_DESIGN - 100 - pNumber->getSize().width * numScale / 2, 180);
			break;
		}
		pNumber->setPositionStart(point);
		this->addChild(pNumber);
	}

	this->arrPos.clear();
	this->arrNumberDouble.clear();
	this->IsRunning = false;
}

void Number_Chat_inGame7u::ShowChatByPosTCC(const int& pos, string& strMsg)
{
	Chat *Message = Chat::create(strMsg, kuser0);
	Message->setStatusByServer(false);
	Point point;
	//Size convert
	Size _size = ChanUtils::getSizePos();
	switch (pos) {
	case kuser0:
		point = Vec2(50, 430);
		break;
	case kuser1:
		point = Vec2(100, 310);
		break;
	case kuser2:
		point = Vec2(50, 190);
		break;
	case kuser3:
		point = Vec2(WIDTH_DESIGN - 100, 430);
		break;
	case kuser4:
		point = Vec2(WIDTH_DESIGN - 130, 310);
		break;
	case kuser5:
		point = Vec2(WIDTH_DESIGN - 100, 190);
		break;
	}
	Message->setPosition(point);
	this->addChild(Message);
}
