//
//  _Background_inGame_.cpp
//  iCasino_v2
//
//  Created by Thanhhv on 5/16/14.
//
//

#include "LayerTimer.h"
#include "AllData.h"
#include "ChanUtils.h"
#include "SceneManager.h"
#include "PhomMessDef.h"

bool LayerTimer::init() {
	if (!Layer::init()) return false;
	sizeAd = ChanUtils::getSizePos();
	winSize = Director::getInstance()->getVisibleSize();
	count = 15;
	tg = 15;
	isRunning = false;
	gameVersion = SceneManager::getSingleton().getGameVersion();

	this->setAnchorPoint(Point(0.5, 0.5));
	this->setPosition(Vec2::ZERO);

	title = ImageView::create();
	title->loadTexture("text-start.png");
	title->setAnchorPoint(Vec2(0.5, 0.5));
	title->setPosition(Vec2(WIDTH_DESIGN / 2, winSize.height / 2));
	title->setVisible(false);
	this->addChild(title);

	string font_name = "font_number-export.fnt";
	Vec2 pos = Vec2(title->getContentSize().width / 2 + 200, title->getContentSize().height / 2);
	int _scale = 0.8;
	Vec2 anchorPoint = Vec2::ANCHOR_MIDDLE;

	labelCount = LabelBMFont::create("0", font_name);
	//labelCount->setScale(_scale);
	labelCount->setAnchorPoint(anchorPoint);
	labelCount->setPosition(pos);
	title->addChild(labelCount);

	return true;
}

void LayerTimer::startTimer()
{
	// 	if (count == tg)
	// 		count == tg;
	//timer->setPosition(Vec2(WIDTH_DESIGN / 2 + w_number, winSize.height / 2));
	title->setVisible(true);
	isRunning = true;

	// 	if (gameVersion == GameVersion::p2)
	// 	{
	if (labelCount != NULL)
	{
		labelCount->setVisible(true);
		labelCount->setString(StringUtils::format("%d", count));
	}
	// 	}
	// 	else{
	// 		string str_num = "number-";
	// 		if (count <= 10){
	// 			timer->setPosition(Vec2(WIDTH_DESIGN / 2 + w_number + 50, winSize.height / 2));
	// 			timer->loadTexture(cocos2d::StringUtils::format("%s%d.png", str_num.c_str(), count));
	// 			timer->setVisible(true);
	// 			timer2->setVisible(false);
	// 		}
	// 		else {
	// 			timer->setPosition(Vec2(WIDTH_DESIGN / 2 + w_number, winSize.height / 2));
	// 			timer->loadTexture(cocos2d::StringUtils::format("%s%d.png", str_num.c_str(), int(count / 10)));
	// 			timer2->loadTexture(cocos2d::StringUtils::format("%s%d.png", str_num.c_str(), (count % 10)));
	// 			timer->setVisible(true);
	// 			timer2->setVisible(true);
	// 		}
	// 	}
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerTimer::changeNumber),this,1, false);
}
void LayerTimer::stopTimer()
{
	count = tg;
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTimer::changeNumber),this);
	// 	timer->setVisible(false);
	// 	timer2->setVisible(false);
	title->setVisible(false);
	this->stopAllActions();
	isRunning = false;
	if (labelCount != NULL)
		labelCount->setVisible(false);
}

void LayerTimer::changeNumber(float dt)
{
	count--;
	if (count < 0){
		stopTimer();
	}
	// 	if (gameVersion == GameVersion::p2)
	// 	{
	if (labelCount != NULL)
	{
		labelCount->setString(StringUtils::format("%d", count));
	}
	// 	}
	// 	else{
	// 		string str_num = "number-";
	// 		if (count <= 10 && count > 0){
	// 			timer->setPosition(Vec2(WIDTH_DESIGN / 2 + w_number + 50, winSize.height / 2));
	// 			timer->loadTexture(cocos2d::StringUtils::format("%s%d.png", str_num.c_str(), count));
	// 			timer2->setVisible(false);
	// 		}
	// 		else if (count > 10){
	// 			//timer->setPosition(Vec2(WIDTH_DESIGN / 2 + w_number, winSize.height / 2));
	// 			timer->loadTexture(cocos2d::StringUtils::format("%s%d.png", str_num.c_str(), int(count / 10)));
	// 			timer2->loadTexture(cocos2d::StringUtils::format("%s%d.png", str_num.c_str(), (count % 10)));
	// 		}
	// 	}
}



bool LayerTimer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return false;
}

void LayerTimer::setCount(int _count)
{
	if (isRunning)
		return;
	count = _count;
	tg = _count;
// 	if (count <= 10){
// 		timer->setPosition(Vec2(WIDTH_DESIGN / 2 + w_number + 50, winSize.height / 2));
// 	}
// 	else if (count > 10){
// 		timer->setPosition(Vec2(WIDTH_DESIGN / 2 + w_number, winSize.height / 2));
// 	}
}

LayerTimer::LayerTimer()
{
	labelCount = NULL;
}

LayerTimer::~LayerTimer()
{

}
