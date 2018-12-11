
#include "LayerCount.h"
#include "mUtils.h"

LayerCount::LayerCount()
:Ready(NULL),
TurnUp(NULL),
pLabelTurnUp(NULL),
pLabelReady(NULL),
pCountReady(0),
pCountTurnUp(0)
{

}

LayerCount::~LayerCount()
{

}

bool LayerCount::init() {
	if (!Layer::init())
		return false;
	Size sizeAd = mUtils::getSizePos();
	this->setAnchorPoint(Point(0, 0));

	this->Ready = ui::ImageView::create();
	this->Ready->setPosition(Point(750 + sizeAd.width, 430 + sizeAd.height));
	this->Ready->setScale(0.7);
	this->Ready->setVisible(true);
	this->addChild(this->Ready);

	this->TurnUp = ui::ImageView::create();
	this->TurnUp->setPosition(Point(750 + sizeAd.width, 430 + sizeAd.height));
	this->TurnUp->setScale(0.7);
	this->TurnUp->setVisible(true);
	this->addChild(this->TurnUp);

	this->pLabelTurnUp = Label::createWithSystemFont(dataManager.GetSysString(910),"", 25);
	this->pLabelTurnUp->setPosition(Point(680 + sizeAd.width, 432 + sizeAd.height));
	this->pLabelTurnUp->setColor(Color3B::YELLOW);
	this->pLabelTurnUp->setVisible(false);
	this->pLabelTurnUp->setScale(0.7);
	this->addChild(this->pLabelTurnUp);

	this->pLabelReady = Label::createWithSystemFont(dataManager.GetSysString(912),"", 25);
	this->pLabelReady->setPosition(Point(680 + sizeAd.width, 432 + sizeAd.height));
	this->pLabelReady->setColor(Color3B::YELLOW);
	this->pLabelReady->setVisible(false);
	this->pLabelReady->setScale(0.7);
	this->addChild(this->pLabelReady);

	return true;
}

void LayerCount::StartCountDownReady()
{
	this->pCountReady = 15;
	this->Ready->loadTexture(cocos2d::StringUtils::format("%d.png", pCountReady).c_str());
	this->Ready->setVisible(true);
	this->pLabelReady->setVisible(true);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerCount::CountDownReady),this,1, false);
}

void LayerCount::StopCountDownReady()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCount::CountDownReady),this);
	this->Ready->setVisible(false);
	this->pLabelReady->setVisible(false);
	this->pCountReady = 15;
}

void LayerCount::CountDownReady(float dt)
{
	pCountReady--;
	if (pCountReady < 0) {
		this->StopCountDownReady();
	}
	this->Ready->loadTexture(cocos2d::StringUtils::format("%d.png", pCountReady).c_str());
}

void LayerCount::StartCountDownTurnUp()
{
	this->pCountTurnUp = 15;
	this->TurnUp->loadTexture(cocos2d::StringUtils::format("%d.png", pCountTurnUp).c_str());
	this->TurnUp->setVisible(true);
	this->pLabelTurnUp->setVisible(true);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerCount::CountDownTurnUp),this,1, false);
}

void LayerCount::StopCountDownTurnUp()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCount::CountDownTurnUp),this);
	this->pCountTurnUp = 15;
	this->TurnUp->setVisible(false);
	this->pLabelTurnUp->setVisible(false);
}

void LayerCount::CountDownTurnUp(float dt)
{
	this->pCountTurnUp--;
	if (pCountTurnUp < 0) {
		this->StopCountDownTurnUp();
	}
	this->TurnUp->loadTexture(cocos2d::StringUtils::format("%d.png", pCountTurnUp).c_str());
}


bool LayerCount::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return false;
}

void LayerCount::setTitleReady(string title)
{
	this->pLabelReady->setString(title);
}
