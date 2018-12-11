#include "CCProgressLayer.h"
#include "cotuong/CommonChess.h"
#include "../Common.h"
#include "../SceneManager.h"
#define kProgressLayerBackgroundShowOpacity 175
CCProgressLayer::CCProgressLayer()
{
	touchListener = NULL;
	spriteLoading = NULL;
	m_callbackTimer = NULL;
	m_callbackTimerListener = NULL;
}

CCProgressLayer::~CCProgressLayer()
{
	if (touchListener != NULL)
		_eventDispatcher->removeEventListener(touchListener);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(CCProgressLayer::hideLoading),this);
}

bool CCProgressLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
	{
		return false;
	}
	this->setTag(LAYER_POPUP_ID::POPUP_LOADING);
	this->setLocalZOrder(ZORDER_LIST::ZORDER_LAYER_LOADING);
	this->setOpacity(kProgressLayerBackgroundShowOpacity);

	Size visibleSize = Director::getInstance()->getVisibleSize();
    spriteLoading = Sprite::create("loading.png");
    spriteLoading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    spriteLoading->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 ));
    this->addChild(spriteLoading);
    
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
	Size winSize = Director::getInstance()->getWinSize();
	this->lblStatus = Label::createWithTTF("Loading", "__Roboto-Bold.ttf", 40, Size::ZERO, TextHAlignment::CENTER);
	this->lblStatus->setPosition(Point(winSize.width / 2, winSize.height / 2 - spriteLoading->getContentSize().height));
	this->addChild(this->lblStatus);
    this->lblStatus->setVisible(false);
	this->setVisible(false);
	return true;
}
void CCProgressLayer::timShowLoading(float dt){
    this->timeLoading++;
    if (this->timeLoading >= 0.5 && this->shouldHide){
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(CCProgressLayer::timShowLoading), this);
        this->timeLoading = 0;
        if (touchListener == NULL)
            return;
        spriteLoading->stopAllActions();
        touchListener->setSwallowTouches(false);
        //this->stopAllActions();
        //this->runAction(CallFunc::create(listener));//callback
        this->setVisible(false);
        if (m_callbackTimer && m_callbackTimerListener){
            (m_callbackTimer->*m_callbackTimerListener)();
        }
        this->shouldHide = false;
    }
}
void CCProgressLayer::showLoading(const float& timeOut)
{
    this->timeLoading = 0;
    this->shouldHide = false;
	if (touchListener == NULL)
		return;
    this->currTimeOut = timeOut;
	touchListener->setSwallowTouches(true);
    if (this->isVisible() == true) return;

    
    if (SceneManager::getSingleton().isNagaWin || SceneManager::getSingleton().isNagaNew){
        spriteLoading->setTexture("loading.png");
        spriteLoading->runAction(RepeatForever::create(RotateBy::create(0.2,50)));
        
    }else{
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 12; i++) {
            str = StringUtils::format("moc3-loading-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 1200, 1200));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.05f);
        //anim->retain();
        
        spriteLoading->runAction(RepeatForever::create(Animate::create(anim)));

    }
    
   
    //this->scheduleOnce(schedule_selector(CCProgressLayer::hideLoading), timeOut);
        Director::getInstance()->getScheduler()->schedule(schedule_selector(CCProgressLayer::hideLoading), this, timeOut, 1, timeOut, false);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(CCProgressLayer::timShowLoading), this, 1, false);
    this->setVisible(true);

}

void CCProgressLayer::hideLoading(float dt)
{
    this->shouldHide = true;

//    if (this->timeLoading < 0.5) return;

    this->timeLoading = 0;
	if (touchListener == NULL)
		return;
	spriteLoading->stopAllActions();
	touchListener->setSwallowTouches(false);
	//this->stopAllActions();
	//this->runAction(CallFunc::create(listener));//callback
	this->setVisible(false);
//    if (m_callbackTimer && m_callbackTimerListener){
//        (m_callbackTimer->*m_callbackTimerListener)();
//    }
}

void CCProgressLayer::onEnter()
{
	Layer::onEnter();
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(CCProgressLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void CCProgressLayer::onExit()
{
	Layer::onExit();
}

bool CCProgressLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void CCProgressLayer::onLoadingCallBack(const std::function<void()> &listener)
{
	this->listener = listener;
}

void CCProgressLayer::setStatusString(string sStatus)
{
	this->lblStatus->setString(sStatus);
}

void CCProgressLayer::setCallBack(Ref* target, SEL_CallFunc listener)
{
	m_callbackTimer = target;
	m_callbackTimerListener = listener;
}

