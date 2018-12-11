//
//  LayerBet_BaCayChuong.cpp
//  demo
//
//  Created by AlexDao on 5/24/14.
//
//

#include "LayerBet_BaCayChuong.h"
#include "GameServer.h"
#include "Requests/ExtensionRequest.h"
#include "mUtils.h"
#include "AllData.h"
#include "ClientMsgDefs.h"

using namespace Sfs2X;
using namespace cocos2d;

LayerBet_BaCayChuong::LayerBet_BaCayChuong()
:lblMoney(NULL),
sliderMoney(NULL),
spriteMoney(NULL),
TypeGame(kGameBaCayChuong)
{
}

LayerBet_BaCayChuong::~LayerBet_BaCayChuong()
{
}

SEL_MenuHandler LayerBet_BaCayChuong::onResolveCCBCCMenuItemSelector(Ref *pTarget, const char *pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnCreate", LayerBet_BaCayChuong::onButtonCreate);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnClose", LayerBet_BaCayChuong::onButtonClose);
	return NULL;
}

void LayerBet_BaCayChuong::onButtonCreate(Ref* pSender)
{
	long long max = 0;
	int minBet = 0;
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL)
		return;
	if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") != NULL) {
		max = (long long)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue();
	}
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") != NULL) {
		minBet = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue()->c_str());
	}
	int gameBet = sliderMoney->getValue() == sliderMoney->getMaximumValue() ? max : mUtils::getMoneyFromSliderValue(floor(sliderMoney->getValue()));
	gameBet = gameBet < minBet ? minBet : gameBet;

	if (TypeGame == kGameBaCayChuong) {
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutInt(EXT_FIELD_GAME_BET_VALUE, gameBet);
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
		GameServer::getSingleton().Send(request);
	}
	else if (TypeGame == kGameCoTuong) {
		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		parameter->PutDouble("amf", gameBet);
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		boost::shared_ptr<IRequest> request(new ExtensionRequest("umr", parameter, lastRoom));
		GameServer::getSingleton().Send(request);
	}
	else if (TypeGame == kGameTomCuaCa){
		log("-- aid: %d", _tag);
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutInt("aid", _tag);
		params->PutInt("gbv", gameBet);
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		boost::shared_ptr<IRequest> request(new ExtensionRequest("gbr", params, lastRoom));
		GameServer::getSingleton().Send(request);
	}
	this->removeFromParentAndCleanup(true);
}

void LayerBet_BaCayChuong::onButtonClose(Ref* pSender)
{
	log("onButtonClose");
	this->removeFromParentAndCleanup(true);
}

void LayerBet_BaCayChuong::valueChanged(Ref *sender, cocos2d::extension::Control::EventType controlEvent){
	ControlSlider* pSlider = (ControlSlider*)sender;
	float percent = pSlider->getValue() - pSlider->getMinimumValue();
	float max = pSlider->getMaximumValue() - pSlider->getMinimumValue();
	float delta = max;
	float posX = pSlider->getPositionX();
	float sliderWidth = pSlider->getContentSize().width;
	spriteMoney->setPosition(Vec2(posX + percent*(sliderWidth / delta), spriteMoney->getPositionY()));
	//
	long long maxMoney = 0;
	int minBet = 0;
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL)
		return;
	if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") != NULL) {
		maxMoney = (long long)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue();
	}
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") != NULL) {
		minBet = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue()->c_str());
	}
	long long money = pSlider->getValue() == pSlider->getMaximumValue() ? maxMoney : mUtils::getMoneyFromSliderValue(floor(pSlider->getValue()));
	money = money < minBet ? minBet : money;
	//
	this->lblMoney->setString(StringUtils::format("%s xu",
		mUtils::convertMoneyEx(money).c_str()
		).c_str());
	log("--%f", pSlider->getValue());
}

void LayerBet_BaCayChuong::setStyleGame(const int& styleGame) {
	this->TypeGame = styleGame;
}

bool LayerBet_BaCayChuong::onAssignCCBMemberVariable(Ref *pTarget, const char *pMemberVariableName, cocos2d::Node *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblMoney", Label *, lblMoney);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sliderMoney", ControlSlider *, sliderMoney);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sMoney", Sprite *, spriteMoney);
	return true;
}

void LayerBet_BaCayChuong::onNodeLoaded(Node * pNode, NodeLoader * pNodeLoader)
{
	double max = 0;
	int minBet = 0;
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL)
		return;

	if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") != NULL) {
		max = (int)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue();
	}

	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") != NULL) {
		minBet = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue()->c_str());
	}

	this->sliderMoney->addTargetWithActionForControlEvents(this, cccontrol_selector(LayerBet_BaCayChuong::valueChanged), Control::EventType::VALUE_CHANGED);
	this->sliderMoney->setMinimumValue(1);
	this->sliderMoney->setMaximumValue(mUtils::getMaximumValue4Slider(floor(max)) + 1);
	this->sliderMoney->setValue(1);

	//this->sliderMoney->setTouchPriority(-128);

}

// void LayerBet_BaCayChuong::registerWithTouchDispatcher(void)
// {
// 	Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
// }

bool LayerBet_BaCayChuong::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void LayerBet_BaCayChuong::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerBet_BaCayChuong::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerBet_BaCayChuong::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerBet_BaCayChuong::setAID(int _mtag)
{
	_tag = _mtag;
}

void LayerBet_BaCayChuong::setMoney(double _money)
{
	if (_money == 0) return;
	sliderMoney->setValue(mUtils::getMaximumValue4Slider(_money) - 1);
	lblMoney->setString(StringUtils::format("%s xu", mUtils::convertMoneyEx(_money).c_str()));

}
