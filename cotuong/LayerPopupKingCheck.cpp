#include "LayerPopupKingCheck.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../GameServer.h"
#include "Requests/ExtensionRequest.h"
#include "ServerMessageDef.h"
#include "Requests/LeaveRoomRequest.h"
#include "gameUtils.h"
#include "MyAnimation.h"


USING_NS_CC;
#define  TAG_SPIRTE_HERO 3737

// on "init" you need to initialize your instance
bool LayerPopupKingCheck::init()
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) return false;
	this->setOpacity(180);

	visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	spChieu = Sprite::create("chieu_tuong.png");
	spChieu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	spChieu->setPosition(visibleSize.width / 2, visibleSize.height);
	spChieu->setTag(10);
	spChieu->setLocalZOrder(10);
	this->addChild(spChieu);

	return true;
}

LayerPopupKingCheck::LayerPopupKingCheck()
{
	this->setCountCheck(0);
	this->setCoutMeCheck(0);
}

LayerPopupKingCheck::~LayerPopupKingCheck()
{
}

void LayerPopupKingCheck::showKingCheck(string _nameCurrTurn)
{
	this->nameCurrentTurn = _nameCurrTurn;
	string _myName = GameUtils::getMyName();
	if (this->nameCurrentTurn.compare(_myName) != 0)
	{
		if (this->getCountCheck() > 0)
			return;
	}
	else {
		if (this->getCoutMeCheck() > 0)
			return;
	}


	if (0 == this->nameCurrentTurn.compare(_myName))
		this->setCoutMeCheck(1);
	else
		this->setCountCheck(1);


	this->setVisible(true);
	spChieu->setVisible(true);
	//spTuong->setVisible(true);
	//spchieu move from left to middle
	float defaultDuration = 0.2f;
	float delayDuration = 2.0f;

	EaseIn *ease1 = EaseIn::create(MoveTo::create(defaultDuration, Vec2(visibleSize.width / 2,
		visibleSize.height / 2 - spChieu->getContentSize().height / 2)), 0.2f);
	Sequence* moveAct1 = Sequence::create(Show::create(), ease1, DelayTime::create(delayDuration),
		CallFuncN::create(CC_CALLBACK_1(LayerPopupKingCheck::callbackAnim, this)), NULL);
	spChieu->runAction(moveAct1);
}

void LayerPopupKingCheck::callbackAnim(Node *sender)
{
	int tagNode = sender->getTag();
	float posY = 0;
	if (tagNode == 11)
		posY = visibleSize.height;
	Sequence* moveAct = Sequence::create(EaseIn::create(MoveTo::create(0.3, Vec2(visibleSize.width / 2, posY)), 0.3f), NULL);
	// 	if (tagNode == 11)
	// 	{
	this->setVisible(false);
	// 	}
	// 	else{
	// 		spChieu->runAction(moveAct);
	// 	}
}

bool LayerPopupKingCheck::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto target = static_cast<Sprite*>(unused_event->getCurrentTarget());

	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	if (rect.containsPoint(locationInNode))
	{
		return true;
	}
	return false;
}

void LayerPopupKingCheck::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (this->isVisible())
	{
		this->setVisible(false);
		this->nameCurrentTurn = "";
		this->removeChildByTag(TAG_SPIRTE_HERO);
		spChieu->stopAllActions();
		spChieu->setPosition(visibleSize.width / 2, visibleSize.height);
	}
}

void LayerPopupKingCheck::onEnter()
{
	Layer::onEnter();
	_listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(false);
	_listener->onTouchBegan = CC_CALLBACK_2(LayerPopupKingCheck::onTouchBegan, this);
	_listener->onTouchEnded = CC_CALLBACK_2(LayerPopupKingCheck::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void LayerPopupKingCheck::onExit()
{
	Layer::onExit();
	log("~On Exit");
	_eventDispatcher->removeEventListenersForTarget(this);
}

void LayerPopupKingCheck::reset()
{
	this->setVisible(false);
	this->nameCurrentTurn = "";
	this->setCountCheck(0);
	this->setCoutMeCheck(0);
	this->removeChildByTag(TAG_SPIRTE_HERO);
	spChieu->stopAllActions();
	spChieu->setPosition(visibleSize.width / 2, visibleSize.height);
}


