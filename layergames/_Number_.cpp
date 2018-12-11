//
//  Number.cpp
//  iCasinoCard
//
//  Created by Thanhhv on 5/9/14.
//
//

#include "_Number_.h"
#define TIME_REMOVE 2
Number* Number::create(const string& pNumberString){
	Number* number = new Number(pNumberString);
	if (number && number->init()){
		number->autorelease();
		return number;
	}
	else
	{
		delete number;
		number = NULL;
		return NULL;
	}
}

Number* Number::create(const double& pNumberDouble){
	Number* number = new Number(pNumberDouble);
	if (number && number->init()){
		number->autorelease();
		return number;
	}
	else
	{
		delete number;
		number = NULL;
		return NULL;
	}
}

Number::Number(const string& pNumberString)
:numberString(pNumberString)
{
}

Number::Number(const double& pNumberDouble) {
	// dựa vào số double mà tạo ra chuỗi kí tự tương ứng (vd: 1.34e+006 -> "1340000")
	this->numberString = mUtils::convertMoneyFromDoubleToString(pNumberDouble);
}

Number::~Number(){
	log("Number Destructor");
}

bool Number::init(){
	if (!Layer::init())
		return false;

	this->setAnchorPoint(Point(0, 0));
	this->setNumbersWithString(this->numberString);

	return true;
}

void Number::setNumbersWithString(string pNumberString) {
	removeAllChildrenWithCleanup(true);
	this->numberString = pNumberString;

	// trim
	pNumberString.erase(0, pNumberString.find_first_not_of(' '));
	pNumberString.erase(pNumberString.find_last_not_of(' ') + 1);

	int widNum = 57;
	int heiNum = 45;

	int length = pNumberString.length();
	//HoangDD, cal size of ','
	int c = 0;
	for (int i = 1; i < length; i++)
	if (pNumberString.at(i) == ',')
	{
		c++;
	}
	setSize(widNum * (length - c) + c * 13, heiNum);

	// kiểm tra xem dãy số có hợp lệ hay không?!
	// dãy số hợp lệ là dãy số:
	//	- các phần từ 1 trở đi đều là 1 c/số (từ 0->9)
	if (length == 0) return;
	else {
		for (int i = 1; i < length; i++)
		{
			if ((pNumberString[i] < '0' || pNumberString[i] > '9') && pNumberString[i] != ',') return;
		}
	}

	int widthNum = 0;

	for (int i = 0; i < length; i++) {
		Sprite *num = getNumberWithChar(pNumberString[i]);
		if (num == NULL) continue;
		num->setAnchorPoint(Point(0, 0));
		widthNum += (pNumberString[i] == ',' ? 25 : widNum);
		num->setPosition((pNumberString[i] == ',' ? widthNum + 18 : widthNum), 0);
		this->addChild(num);
	}
}

void Number::startRunAction() {
	this->runAction(Sequence::create(MoveTo::create(0.5, Point(this->pointStart.x, this->pointStart.y + 45)),
		DelayTime::create(TIME_REMOVE),
		RemoveSelf::create(),
		NULL));
}

void Number::startRunAction(float toY){
	this->runAction(Sequence::create(//DelayTime::create(0.7), 
		MoveTo::create(0.5, Point(this->pointStart.x, this->pointStart.y + toY)),
		DelayTime::create(TIME_REMOVE),
		RemoveSelf::create(),
		NULL));
}

Sprite* Number::getNumberWithChar(char a) {

	Sprite *numSprite = NULL;
	string url = "";
	bool isRight = true;

	switch (a) {
	case '+':
		url = "cong.png";
		style = "";
		break;

	case '-':
		url = "tru.png";
		style = "lose/";
		break;

	case ',':
		url = "dot.png";
		break;

	default:
		if (a >= '0' && a <= '9'){
			ostringstream oss;
			oss << a;
			url += oss.str();
			url += ".png";
		}
		else {
			isRight = false;
		}
		break;
	}

	url = style + url;

	if (isRight)
	{
		numSprite = Sprite::create(url.c_str());
		numSprite->setAnchorPoint(Point(0, 0));
	}

	return numSprite;
}

// --------------
// setter, getter

void Number::setSize(int wid, int hei) {
	this->sizeThis.setSize(wid, hei);
}

Size Number::getSize() {
	return this->sizeThis;
}

void Number::setPositionStart(Point pPointStart){
	this->pointStart = pPointStart;

	this->setPosition(pPointStart);
	startRunAction();
}

void Number::setPositionStart(Point pPointStart, float toY)
{
	this->pointStart = pPointStart;

	this->setPosition(pPointStart);
	startRunAction(toY);
}

Point Number::getPositionStart() {
	return this->pointStart;
}
