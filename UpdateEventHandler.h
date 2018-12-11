#ifndef _UpdateEventHandler_h_
#define _UpdateEventHandler_h_
#include "GameServer.h"
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class UpdateEventHandler : public Layer{
public:
	void initScheduleServer(){
		Director::getInstance()->getScheduler()->schedule(schedule_selector(UpdateEventHandler::updateEvent),this,1/60, false);
		Director::getInstance()->getScheduler()->schedule(schedule_selector(UpdateEventHandler::updateEvent4CheckConnection),this,1, false);
	}
	void unInitScheduleServer(){
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(UpdateEventHandler::updateEvent),this);
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(UpdateEventHandler::updateEvent4CheckConnection),this);
	}
private:
	void updateEvent(float dt){

	}
	void updateEvent4CheckConnection(float dt){

	}
};
#endif //_UpdateEventHandler_h_
