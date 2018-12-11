

#include "LayerConnecting.h"
#include "../mUtils.h"
#include "../Common.h"

LayerConnecting::LayerConnecting() : loading(NULL), count(0)
{
    arrText.clear();
}

LayerConnecting::~LayerConnecting(){
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerConnecting::updateString),this);
}

bool LayerConnecting::init(){
    if(!Layer::init())
        return false;

    this->arrText.push_back(dataManager.GetSysString(571)+" .");
    this->arrText.push_back(dataManager.GetSysString(571)+" ..");
    this->arrText.push_back(dataManager.GetSysString(571)+" ...");

    Size size = Director::getInstance()->getVisibleSize();
    Size sizeAdd = mUtils::getSizePos();

    LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 200));
    color->setAnchorPoint(Vec2(0, 0));
    color->setPosition(Vec2(-sizeAdd.width, -sizeAdd.height));
    color->setScaleX(size.width / WIDTH_DESIGN);
    color->setScaleY(size.height / HEIGHT_DESIGN);
    color->setOpacity(175);
    this->addChild(color);

    this->loading = Label::createWithTTF(dataManager.GetSysString(571)+" ...","",30);
    this->loading->setAnchorPoint(Vec2::ZERO);
    this->loading->setColor(Color3B::RED);
    //this->loading->setScale(1.65);

    this->loading->setPosition(Vec2(WIDTH_DESIGN / 2 - 140, HEIGHT_DESIGN / 2 + 50));
    this->addChild(this->loading);

    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerConnecting::updateString),this,1, false);

    return true;
}

void LayerConnecting::onEnter() {
    Layer::onEnter();
    this->runAction(mUtils::getActionOpenPopup());
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerConnecting::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void LayerConnecting::onExit() {
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
}

bool LayerConnecting::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    return true;
}

void LayerConnecting::updateString(float dt){
    this->loading->setString(this->arrText[this->count]);
    if(this->count < this->arrText.size() - 1) {
        this->count ++;
    } else {
        this->count = 0;
    }
}
