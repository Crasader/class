//
//  _Number_inGame_.cpp
//  iCasino_v2
//
//  Created by Thanhhv on 5/16/14.
//
//

#include "_Number_inTomCuaCa.h"

NumberInTomCuaCa::~NumberInTomCuaCa() {
	log("~~~~LayerNumberInGame");
	arrPos.clear();
	arrNumberString.clear();
}
bool NumberInTomCuaCa::init() {
    if (!Layer::init()) return false;
    
    this->setAnchorPoint(Point(0, 0));
	this->isRunning = false;
    
    return true;
}
void NumberInTomCuaCa::showNumberByPos(int pos, string numberString) {
	arrPos.push_back(pos);
	arrNumberString.push_back(numberString);

	if (!isRunning) {
		isRunning = true;
		//this->scheduleOnce(schedule_selector(NumberInTomCuaCa::runAction), 1);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(NumberInTomCuaCa::runAction), this, 1, 1, 1, false);
	}
}
void NumberInTomCuaCa::showNumberByPos(int pos, double numberDouble) {
	arrPos.push_back(pos);
	arrNumberDouble.push_back(numberDouble);

	if (!isRunning) {
		isRunning = true;
		//this->scheduleOnce(schedule_selector(NumberInTomCuaCa::runAction2), 1);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(NumberInTomCuaCa::runAction2), this, 1, 1, 1, false);
	}
}

void NumberInTomCuaCa::runAction(float dt) {
	int demMe = 0;
	int demLeft = 0;
	int demRight = 0;
	int demTop = 0;
	int demBot=0;

	for (int i = 0; i < arrPos.size(); i++)
	{
		int pos = arrPos.at(i);
		string numberString = arrNumberString.at(i);

		Number *number = Number::create(numberString);
        number->setScale(0.7);
		Point point;

		switch (pos) {
		case kuser0:
			point.setPoint(50, 350);
                
			demLeft++;
			break;
            case kuser1:
                point.setPoint(50, 220);
                demLeft++;
                break;
            case kuser2:
                point.setPoint(20, 80);
                demLeft++;
                break;
            case kuser3:
                point.setPoint(630, 360);
                demLeft++;
                break;
            case kuser4:
                point.setPoint(630, 210);
                demLeft++;
                break;
            case kuser5:
                point.setPoint(630, 90);
                demLeft++;
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
void NumberInTomCuaCa::runAction2(float dt) {
	int demMe = 0;
	int demLeft = 0;
	int demRight = 0;
	int demTop = 0;

	for (int i = 0; i < arrPos.size(); i++)
	{
		int pos = arrPos.at(i);
		double numberDouble = arrNumberDouble.at(i);

		Number *number = Number::create(numberDouble);
        number->setScale(0.7);
		Point point;

		switch (pos) {
            case kuser0:
                point.setPoint(650, 250 + demLeft * number->getSize().height);
                demLeft++;
                break;
            case kuser1:
                point.setPoint(650, 250 + demLeft * number->getSize().height);
                demLeft++;
                break;
            case kuser2:
                point.setPoint(650, 250 + demLeft * number->getSize().height);
                demLeft++;
                break;
            case kuser3:
                point.setPoint(650, 250 + demLeft * number->getSize().height);
                demLeft++;
                break;
            case kuser4:
                point.setPoint(650, 250 + demLeft * number->getSize().height);
                demLeft++;
                break;
            case kuser5:
                point.setPoint(650, 250 + demLeft * number->getSize().height);
                demLeft++;
                break;
		}
		number->setPositionStart(point);
		this->addChild(number);
	}

	arrPos.clear();
	arrNumberDouble.clear();

	this->isRunning = false;
}
void NumberInTomCuaCa::callbackShowNumber(float dt) {
	isRunning = false;
}
