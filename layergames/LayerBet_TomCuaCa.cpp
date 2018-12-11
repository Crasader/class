//
//  LayerBet_TomCuaCa.cpp
//  demo
//
//  Created by Tuan on 6/25/14.
//
//

#include "LayerBet_TomCuaCa.h"
#include "GameServer.h"
#include "Requests/ExtensionRequest.h"
#include "mUtils.h"

using namespace Sfs2X;
using namespace cocos2d;

LayerBet_TomCuaCa::LayerBet_TomCuaCa()
{
	lblMoney=NULL;
	sliderMoney=NULL;
	spriteMoney=NULL;
	btnBetParent = NULL;
}

LayerBet_TomCuaCa::~LayerBet_TomCuaCa()
{
}

SEL_MenuHandler LayerBet_TomCuaCa::onResolveCCBCCMenuItemSelector(cocos2d::Ref *pTarget, const char *pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnCreate", LayerBet_TomCuaCa::onButtonCreate);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnClose", LayerBet_TomCuaCa::onButtonClose);
	return NULL;
}

void LayerBet_TomCuaCa::onButtonCreate(Ref* pSender)
{
	log("mTip: %s", lblMoney->getString().c_str());
	//
	long long max = 0;
	int minBet = 0;
	if (GameServer::getSingleton().getSmartFox()->MySelf() == NULL || GameServer::getSingleton().getSmartFox()->LastJoinedRoom()==NULL)
		return;
	if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") == NULL)
		return;

	if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue() != NULL) {
		max = (long long) *GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue();
	}
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") != NULL) {
		minBet =atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue()->c_str());
	}
	int gameBet = sliderMoney->getValue()==sliderMoney->getMaximumValue()?max:mUtils::getMoneyFromSliderValue( floor( sliderMoney->getValue() ) ) ;
	gameBet = gameBet<minBet?minBet:gameBet;
	//EXT_EVENT_GAME_BET_REQ = "gbr";
	boost::shared_ptr<ISFSObject> params (new SFSObject());
	params->PutInt("aid",_tag);
	params->PutInt("gbv", gameBet);
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request (new ExtensionRequest("gbr",params,lastRoom));
	GameServer::getSingleton().Send(request);
	log("-- aid: %d",_tag);
	this->removeFromParentAndCleanup(true);
}

void LayerBet_TomCuaCa::onButtonClose(Ref* pSender)
{
	log("onButtonClose");
	this->removeFromParentAndCleanup(true);
}

void LayerBet_TomCuaCa::valueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent){
	ControlSlider* pSlider = (ControlSlider*)sender;
	float percent =pSlider->getValue()-pSlider->getMinimumValue();
	float max = pSlider->getMaximumValue() - pSlider->getMinimumValue();
	float delta = max;
	float posX =pSlider->getPositionX();
	float sliderWidth =pSlider->getContentSize().width;
	spriteMoney->setPosition(Point(posX+percent*(sliderWidth/delta), spriteMoney->getPositionY()));
	//
	long long maxMoney = 0;
	int minBet = 0;
	if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") == NULL
		|| GameServer::getSingleton().getSmartFox()->LastJoinedRoom()==NULL)
		return;
	if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") != NULL) {
		maxMoney = (long long) *GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue();
	}
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") != NULL) {
		minBet =atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue()->c_str());
	}
	long long money = pSlider->getValue()==pSlider->getMaximumValue()?maxMoney:mUtils::getMoneyFromSliderValue( floor( pSlider->getValue() ) ) ;
	money = money<minBet?minBet:money;
	//
	lblMoney->setString( StringUtils::format("%s xu", 
		mUtils::convertMoneyEx( money ).c_str()
		) );
	log("--%f",pSlider->getValue());
}

// CCBMemberVariableAssigner interface
bool LayerBet_TomCuaCa::onAssignCCBMemberVariable(Ref *pTarget, const char *pMemberVariableName, Node *pNode)
{
	//log("Imhere onAssignCCBMemberVariable: %s", pMemberVariableName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblMoney", Label *, lblMoney);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sliderMoney", ControlSlider *, sliderMoney);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sMoney", Sprite *, spriteMoney);
	//    sliderMoney->addTargetWithActionForControlEvents(this, cccontrol_selector(LayerBet_BaCayChuong::valueChanged), CCControlEventValueChanged);
	return true;
}

void LayerBet_TomCuaCa::onNodeLoaded( Node * pNode,  NodeLoader * pNodeLoader)
{
	log("Imhere onNodeLoaded");
	long long max = 0;
	if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") == NULL)
		return;
	if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue() != NULL) {
		max = (long long) *GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue();
	}
	//sliderMoney
	this->sliderMoney->addTargetWithActionForControlEvents(this, cccontrol_selector(LayerBet_TomCuaCa::valueChanged), Control::EventType::VALUE_CHANGED);
	sliderMoney->setMinimumValue(1);
	sliderMoney->setMaximumValue(mUtils::getMaximumValue4Slider(max)+1);
	sliderMoney->setValue(1);

	//sliderMoney->setTouchPriority(-128);

}

// void LayerBet_TomCuaCa::registerWithTouchDispatcher( void )
// {
// 	Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
// }

bool LayerBet_TomCuaCa::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void LayerBet_TomCuaCa::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerBet_TomCuaCa::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerBet_TomCuaCa::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerBet_TomCuaCa::setBtnBetFromParent(Button *btn)
{
	btnBetParent = btn;
}

void LayerBet_TomCuaCa::setAID(int _mtag)
{
	_tag = _mtag;
}

void LayerBet_TomCuaCa::setMoney(double _money)
{
	if(_money == 0) return;
 	sliderMoney->setValue(mUtils::getMaximumValue4Slider(_money)-1);
 	lblMoney->setString(StringUtils::format("%s xu", mUtils::convertMoneyEx( _money).c_str()));
	
}
