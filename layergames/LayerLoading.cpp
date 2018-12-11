//
//  LayerLoading.cpp
//  TestCpp
//
//  Created by leafsoar on 7/29/13.
// henh
//

#include "LayerLoading.h"
#include "AllData.h"
#include "data_manager.h"
#include "SceneManager.h"
#include "PhomMessDef.h"
#include "../scenes/SceneLogin.h"
#include "ChanUtils.h"

LayerLoading::LayerLoading()
{
	timer = nullptr;
	lightProgress = nullptr;
	mTextLoading = nullptr;
}

LayerLoading::~LayerLoading(){
	getScheduler()->setTimeScale(1);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLoading::onScheduleLoading),this);
	_eventDispatcher->removeEventListener(_listener);
	log("~LayerLoading");
	timer = nullptr;
	lightProgress = nullptr;
	mTextLoading = nullptr;
}

bool LayerLoading::init(){
	if (!Layer::init())
		return false;
	this->createLoading(SceneManager::getSingleton().getGameVersion());

	return true;
}


bool LayerLoading::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
	return true;
}


void LayerLoading::onEnter(){
	Layer::onEnter();
	_listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);
	_listener->onTouchBegan = CC_CALLBACK_2(LayerLoading::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void LayerLoading::onExit(){
	Layer::onExit();
	_listener->setSwallowTouches(false);
}

void LayerLoading::setProgress(float value)
{
	timer->setPercentage(value);
}

void LayerLoading::start(float duration)
{
	ProgressTo* to1 = ProgressTo::create(duration, 100);
	timer->runAction(to1);
	// 	float posY = lightProgress->getPositionY();
	// 	lightProgress->runAction(MoveTo::create(duration, Vec2(timer->getContentSize().width - startX, posY)));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLoading::onScheduleLoading),this,1, false);
}

void LayerLoading::stop()
{
	if (timer == nullptr || lightProgress == nullptr)
		return;
	Scheduler *scheduler = getScheduler();
	if (scheduler != nullptr)
		scheduler->setTimeScale(3);
}

void LayerLoading::stopLoading()
{
	if (timer == nullptr)
		return;
	getScheduler()->setTimeScale(1);
	timer->stopAllActions();
	this->setVisible(false);
	SceneLogin* sceneLogin = dynamic_cast<SceneLogin*>(this->getParent());
	if (sceneLogin != NULL)
	{
		sceneLogin->onLoadingFinish();
	}
	_listener->setSwallowTouches(false);
}

void LayerLoading::onScheduleLoading(float dt)
{
	// 	if (timer->getPercentage() < 100)
	// 	{
	// 		if (this->getScheduler()->getTimeScale() != 1)
	// 		{
	// 			lightProgress->setPositionX(timer->getPercentage() * 5 + 15);
	// 		}
	// 		else{
	// 			lightProgress->setPositionX(timer->getPercentage() * 5);
	// 		}
	// 	}
	// 	else
	if (mTextLoading != nullptr)
	{
		int percentage = (int)timer->getPercentage();
		mTextLoading->setString(StringUtils::format("%d %%", percentage));
	}
	if (timer->getPercentage() >= 100)
	{
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLoading::onScheduleLoading),this);
		this->runAction(Sequence::create(DelayTime::create(0.1),
			CallFunc::create(CC_CALLBACK_0(LayerLoading::stopLoading, this)), NULL));
	}
}

void LayerLoading::createLoading(int gamever)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 	if (gamever == GameVersion::p1)
	// 	{
	Sprite *mBg = Sprite::create("bg_loading.jpg");
	mBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	mBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	mBg->setScaleY(visibleSize.height / mBg->getContentSize().height);
	this->addChild(mBg);

	// 		Sprite* logoIcon = Sprite::create("promotion-logo.png");
	// 		logoIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	// 		logoIcon->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	// 		logoIcon->setScale(2);
	// 		this->addChild(logoIcon);


//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	mBg->setTexture("background_loading.jpg");
//#endif
	Size sizeAdd = ChanUtils::getSizePos();

	Sprite* timebg = Sprite::create("moc3-loading-1.png");
	timebg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	timebg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + sizeAdd.height));
	this->addChild(timebg);
    Vector<SpriteFrame*> animFrames;
    string str = "";
    for (int i = 1; i <= 12; i++) {
        str = StringUtils::format("moc3-loading-%d.png",i);
        auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 1200, 1200));
        animFrames.pushBack(spriteFrame);
    }
    auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
    anim->retain();
    
    timebg->runAction(RepeatForever::create(Animate::create(anim)));

//	Sprite *loadedSprite = Sprite::create("loading-loaded.png");
//
//	timer = ProgressTimer::create(loadedSprite);
//	timer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
//	timer->setType(ProgressTimer::Type::BAR);
//	timer->setMidpoint(Vec2(0, 0));
//	timer->setBarChangeRate(Vec2(1, 0));
//	timer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 150 - sizeAdd.height));
//	this->addChild(timer);

	// 		lightProgress = Sprite::create("loading-arrow-light.png");
	// 		lightProgress->setAnchorPoint(Vec2(0.5, 0));
	// 		lightProgress->setPosition(Vec2(lightProgress->getContentSize().width / 2 - startX, -3));
	// 		timer->addChild(lightProgress);

	// 		LabelBMFont *mText = LabelBMFont::create(dataManager.GetSysString(263), "robotofont.fnt");
	// 		mText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	// 		mText->setScale(2);
	// 		mText->setPosition(timebg->getPositionX(), timebg->getPositionY() + timebg->getContentSize().height * 3);
	// 		this->addChild(mText);
	// 	}
	// 	else{
	// 		Sprite *mBg = Sprite::create("background.jpg");
	// 		mBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	// 		mBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	// 		mBg->setScaleY(visibleSize.height / mBg->getContentSize().height);
	// 		this->addChild(mBg);
	// 
	// 		// gaidep-right
	// 		Sprite* playGirlRight = Sprite::create("gaidep.png");
	// 		playGirlRight->setAnchorPoint(Point(0.5, 0));
	// 		//playGirlRight->setScale(visibleSize.height / playGirlRight->getContentSize().height);
	// 		playGirlRight->setPosition(visibleSize.width - (playGirlRight->getContentSize().width / 2 - 100), 0);
	// 		this->addChild(playGirlRight);
	// 
	// 		//gaidep-left
	// 		Sprite* playGrilLeft = Sprite::create("gaidep-left.png");
	// 		playGrilLeft->setAnchorPoint(Point(0.5, 0));
	// 		//playGrilLeft->setScale(visibleSize.height / playGrilLeft->getContentSize().height);
	// 		playGrilLeft->setPosition(playGrilLeft->getContentSize().width / 2 - 100, 0);
	// 		this->addChild(playGrilLeft);
	// 
	// 		int distanceAdd = 300;
	// 		startX = 35;
	// 		Sprite* timebg = Sprite::create("loading-bg.png");
	// 		timebg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	// 		timebg->setPosition(visibleSize / 2);
	// 		this->addChild(timebg);
	// 
	// 		Sprite *loadedSprite = Sprite::create("loading-loaded.png");
	// 
	// 		timer = ProgressTimer::create(loadedSprite);
	// 		timer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	// 		timer->setType(ProgressTimer::Type::BAR);
	// 		timer->setMidpoint(Vec2(0, 0));
	// 		timer->setBarChangeRate(Vec2(1, 0));
	// 		timer->setPosition(visibleSize / 2);
	// 		this->addChild(timer);
	// 
	// 		lightProgress = Sprite::create("loading-arrow-light.png");
	// 		lightProgress->setAnchorPoint(Vec2(0.5, 0.5));
	// 		lightProgress->setPosition(Vec2(lightProgress->getContentSize().width / 2 - startX, lightProgress->getContentSize().height / 4));
	// 		timer->addChild(lightProgress);
	// 
	// 		ParticleFlower* _emitter = ParticleFlower::create();
	// 		_emitter->retain();
	// 		_emitter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	// 		_emitter->setPosition(Vec2(lightProgress->getContentSize().width / 2,
	// 			lightProgress->getContentSize().height / 2));
	// 		lightProgress->addChild(_emitter, 10);
	// 		_emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("loading-arrow-light.png"));
	// 
	// 		mTextLoading = LabelBMFont::create("0%", "robotofont.fnt");
	// 		mTextLoading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	// 		mTextLoading->setScale(1.5);
	// 		mTextLoading->setPosition(timebg->getPositionX(), timebg->getPositionY() - timebg->getContentSize().height * 3);
	// 		this->addChild(mTextLoading);
	// 	}

}
