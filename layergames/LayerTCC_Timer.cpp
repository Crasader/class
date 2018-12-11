#include "LayerTCC_Timer.h"
#include "PhomMessDef.h"
#include "LayerTCC_PopupMain.h"

LayerTCC_Timer::LayerTCC_Timer()
{
	timeBet = timeBetBuff;
	timePrepare = timePrepareBuf;
}

LayerTCC_Timer::~LayerTCC_Timer()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_Timer::scheduleTimeBet),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_Timer::scheduleTimePrepare),this);
}

bool LayerTCC_Timer::init()
{
	if (!Layer::init())
		return false;
	Vec2 posLeft = Vec2(287, 200);
	Vec2 posRight = Vec2(450, 450);
	this->createLayerBetCount(posLeft);
	this->createLayerPrepareCount(posRight);
	return true;
}

void LayerTCC_Timer::createLayerBetCount(Vec2 pos)
{
	this->imgBet = Sprite::create("bg-datcuoc1.png");
	this->imgBet->setAnchorPoint(Vec2::ZERO);
	this->imgBet->setPosition(pos);
	this->addChild(this->imgBet);

	Size imgSize = imgBet->getContentSize();

	// 	this->lblBetTimeCount = Label::createWithBMFont("sfuStaff-font.fnt", StringUtils::format("%d", this->timeBet));
	// 	this->lblBetTimeCount->setAnchorPoint(Vec2(0.5, 0));
	// 	this->lblBetTimeCount->setPosition(imgSize.width / 2, 10);
	// 	this->lblBetTimeCount->setScale(0.3);
	// 	this->imgBet->addChild(this->lblBetTimeCount);
	// 	this->imgBet->setVisible(false);
	this->lblBetTimeCount = Label::createWithTTF(StringUtils::format("%d", this->timeBet), kBoldFont, FONT_SIZE_TITLE - 7);
	this->lblBetTimeCount->setAnchorPoint(Vec2(0.5, 0.5));
	this->lblBetTimeCount->setColor(Color3B(162, 11, 0));
	this->lblBetTimeCount->setPosition(32, 39);
	this->imgBet->addChild(this->lblBetTimeCount);
	this->imgBet->setVisible(false);
}

void LayerTCC_Timer::createLayerPrepareCount(Vec2 pos)
{
	this->imgPrepare = Sprite::create("prepare.png");
	this->imgPrepare->setAnchorPoint(Vec2::ZERO);
	this->imgPrepare->setPosition(pos);
	this->addChild(this->imgPrepare);

	Size imgSize = Size(408, 142);

	this->lblPrepareTimeCount = Label::createWithBMFont("font_number-export.fnt", StringUtils::format("%d", this->timePrepare));
	this->lblPrepareTimeCount->setAnchorPoint(Vec2(0.5, 0.5));
	this->lblPrepareTimeCount->setScale(0.8);
	this->lblPrepareTimeCount->setPosition(imgSize.width + 50, imgSize.height / 3);
	this->imgPrepare->addChild(this->lblPrepareTimeCount);
	this->imgPrepare->setVisible(false);
}

void LayerTCC_Timer::startTimeBet()
{
	this->stopAllActions();
	this->stopTimePrepare();
	this->timeBetBuff = this->timeBet;
	this->lblBetTimeCount->setString(StringUtils::format("%d", this->timeBet));
	this->imgBet->setVisible(true);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerTCC_Timer::scheduleTimeBet),this,1, false);
}

void LayerTCC_Timer::stopTimeBet()
{
	//LayerTCC_PopupMain* parent = dynamic_cast<LayerTCC_PopupMain*>(this->getParent());
	// 	if (parent)
	// 		parent->CallBackFromTimer(0);
	//this->runAction(CallFunc::create(CC_CALLBACK_0(listener, target, 0)));
	//	std::bind(&listener, this, 0);
	this->runAction(CallFunc::create(listener));

	this->imgBet->setVisible(false);
	this->lblBetTimeCount->setString(StringUtils::format("%d", this->timeBet));
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_Timer::scheduleTimeBet),this);
}

void LayerTCC_Timer::startTimePrepare()
{
	stopTimeBet();
	this->timePrepareBuf = this->timePrepare;
	this->lblPrepareTimeCount->setString(StringUtils::format("%d", this->timePrepare));
	this->imgPrepare->setVisible(true);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerTCC_Timer::scheduleTimePrepare),this,1, false);
}

void LayerTCC_Timer::stopTimePrepare()
{
	// 	LayerTCC_PopupMain* parent = dynamic_cast<LayerTCC_PopupMain*>(this->getParent());
	// 	if (parent)
	// 		parent->CallBackFromTimer(1);
	//	std::bind(&listener, this, 1);
	//this->runAction(CallFunc::create(CC_CALLBACK_0(listener,target, 1)));
	this->imgPrepare->setVisible(false);
	this->lblPrepareTimeCount->setString(StringUtils::format("%d", this->timePrepare));
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_Timer::scheduleTimePrepare),this);
}

void LayerTCC_Timer::scheduleTimeBet(float dt)
{
	this->timeBetBuff--;
	if (this->timeBetBuff < 0){
		this->stopTimeBet();
		return;
	}
	this->lblBetTimeCount->setString(StringUtils::format("%d", this->timeBetBuff));
}

void LayerTCC_Timer::scheduleTimePrepare(float dt)
{
	this->timePrepareBuf--;
	if (this->timePrepareBuf < 1){
		this->stopTimePrepare();
		return;
	}
	this->lblPrepareTimeCount->setString(StringUtils::format("%d", this->timePrepareBuf));
}

void LayerTCC_Timer::setCallBackFromTimer(const std::function<void()> &listener)
{
	this->listener = listener;
}

void LayerTCC_Timer::onEnter()
{
	Layer::onEnter();
}

void LayerTCC_Timer::onExit()
{
	Layer::onExit();
}

