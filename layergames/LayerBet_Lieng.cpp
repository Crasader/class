//
//  LayerBet_Lieng.cpp
//  demo
//
//  Created by AlexDao on 5/24/14.
//
//

#include "LayerBet_Lieng.h"
#include "Requests/ExtensionRequest.h"
#include "GameServer.h"
#include "mUtils.h"
#include "ClientMsgDefs.h"
#include "../SceneManager.h"

using namespace cocos2d;

LayerBet_Lieng::LayerBet_Lieng()
:lblMoney(NULL),
sliderMoney(NULL),
spriteMoney(NULL),
lblTitle1(NULL),
lblTitle2(NULL),
lblTitle3(NULL),
mMinBet(0),
mCurrentBet(0)
{

}

LayerBet_Lieng::~LayerBet_Lieng()
{

}

cocos2d::SEL_MenuHandler LayerBet_Lieng::onResolveCCBCCMenuItemSelector(Ref *pTarget, const char *pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnCreate", LayerBet_Lieng::OnButtonCreate);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnClose", LayerBet_Lieng::OnButtonClose);
	return NULL;
}

void LayerBet_Lieng::OnButtonCreate(Ref* pSender)
{
	long long max = 0;
	int minBet = 0;
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL)
		return;
    if (SceneManager::getSingleton().getCurrRoomType() == 0){

        if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") != NULL) {
            max = (long long)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue();
        }
    }else{
        if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amfs") != NULL) {
            max = (long long)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amfs")->GetDoubleValue();
        }
    }
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") != NULL) {
		minBet = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue()->c_str());
	}
	int gameBet = sliderMoney->getValue() == sliderMoney->getMaximumValue() ? max : mUtils::getMoneyFromSliderValue(floor(sliderMoney->getValue()));
	gameBet = gameBet < minBet ? minBet : gameBet;
	//check under minBet
	gameBet = gameBet < mMinBet ? mMinBet : gameBet;

	//+ currentBet
	gameBet = gameBet + mCurrentBet;
	//check gameBet with AMF
	gameBet = gameBet > max ? max : gameBet;

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt(EXT_FIELD_GAME_BET_VALUE, gameBet);
	params->PutInt(EXT_FIELD_BET_TYPE, 1);
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
	GameServer::getSingleton().Send(request);
	this->removeFromParentAndCleanup(true);
}

void LayerBet_Lieng::OnButtonClose(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

void LayerBet_Lieng::SliderValueChanged(Ref* sender, cocos2d::extension::Control::EventType controlEvent)
{
	ControlSlider* pSlider = (ControlSlider*)sender;
	float percent = pSlider->getValue() - pSlider->getMinimumValue();
	float max = pSlider->getMaximumValue() - pSlider->getMinimumValue();
	float delta = max;
	float posX = pSlider->getPositionX();
	float sliderWidth = pSlider->getContentSize().width;

	spriteMoney->setPosition(Point(posX + percent * (sliderWidth / delta), spriteMoney->getPositionY()));

	long long maxMoney = 0;
    
    if (SceneManager::getSingleton().getCurrRoomType() == 0){
    
        if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") != NULL) {
            maxMoney = (long long)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue();
        }
    }else{
        if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amfs") != NULL) {
            maxMoney = (long long)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amfs")->GetDoubleValue();
        }
    }

	long long money = pSlider->getValue() == pSlider->getMaximumValue() ? maxMoney : mUtils::getMoneyFromSliderValue(floor(pSlider->getValue()));
	money = money < mMinBet ? mMinBet : money;

	lblMoney->setString(StringUtils::format("%s xu", mUtils::convertMoneyEx(money).c_str()).c_str());
	log("--%f", pSlider->getValue());
}

bool LayerBet_Lieng::onAssignCCBMemberVariable(Ref *pTarget, const char *pMemberVariableName, cocos2d::Node *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblMoney", Label *, lblMoney);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sliderMoney", ControlSlider *, sliderMoney);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sMoney", Sprite *, spriteMoney);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblTitle1", Label *, lblTitle1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblTitle2", Label *, lblTitle2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblTitle3", Label *, lblTitle3);
	return true;
}

void LayerBet_Lieng::onNodeLoaded(Node * pNode, NodeLoader * pNodeLoader)
{
	//this->sliderMoney->setTouchPriority(-128);
}

void LayerBet_Lieng::SetInfoBet(const int& pMinBet, const int& pCurrentBet){
	this->sliderMoney->addTargetWithActionForControlEvents(this, cccontrol_selector(LayerBet_Lieng::SliderValueChanged), Control::EventType::VALUE_CHANGED);

	float max = 0;
    if (SceneManager::getSingleton().getCurrRoomType() == 0){
        if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") != NULL) {
		max = *GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue();
        }
    }else{
        if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amfs") != NULL) {
            max = *GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amfs")->GetDoubleValue();
        }
    }

	this->lblTitle1->setString(("Mức tố hiện tại: " + mUtils::convertMoneyEx(pMinBet)).c_str());
	this->lblTitle2->setString(("Lượt tố trước của bạn: " + mUtils::convertMoneyEx(pCurrentBet)).c_str());
	this->lblTitle3->setString((dataManager.GetSysString(510) + mUtils::convertMoneyEx(pMinBet - pCurrentBet)).c_str());

	this->mMinBet = pMinBet - pCurrentBet;
	this->mCurrentBet = pCurrentBet;

	this->sliderMoney->setMinimumValue(mUtils::getMaximumValue4Slider(this->mMinBet));
	this->sliderMoney->setValue(mUtils::getMaximumValue4Slider(this->mMinBet));
	this->sliderMoney->setMaximumValue(mUtils::getMaximumValue4Slider(floor(max)) + 1);
	//this->sliderMoney->setTouchPriority(-128);
}

// void LayerBet_Lieng::registerWithTouchDispatcher(void)
// {
// 	Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
// }

bool LayerBet_Lieng::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void LayerBet_Lieng::onEnter()
{
	this->runAction(mUtils::getActionOpenPopup());
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerBet_Lieng::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerBet_Lieng::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}
