
#include "LayerAnimationTaiXiu.h"
#include <string>
#include <cstdio>

LayerAnimationTaiXiu::LayerAnimationTaiXiu(){

}

LayerAnimationTaiXiu::LayerAnimationTaiXiu(Vec2& pos)
{
    this->m_callback2 = NULL;
    this->m_callbackListener2 = NULL;
    this->posFrame.x = pos.x;
    this->posFrame.y = pos.y;
}

LayerAnimationTaiXiu *LayerAnimationTaiXiu::createInstance(Vec2 &pos)
{
    LayerAnimationTaiXiu* layer = new LayerAnimationTaiXiu(pos);
    if(layer && layer->init()){
        layer->autorelease();
        return layer;
    }else{
        delete layer;
        layer = 0;
        return NULL;
    }
}

bool LayerAnimationTaiXiu::init(){
    if (!Layer::init()) {
        return false;
    }

    SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("txresultanimate.plist");

    Vector<SpriteFrame*> animFrames(15);
    char str[100] = {0};
    for(int i = 1; i <= 3; ++i)
    {
        sprintf(str, "xucxac%d.png",i);
        auto frame = frameCache->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f, 10);
    AnimationCache::getInstance()->addAnimation(animation, "dance");

    AnimationCache* animCache = AnimationCache::getInstance();
    Animation* normal = animCache->getAnimation("dance");
    normal->setRestoreOriginalFrame(true);

    Animate* animN = Animate::create(normal);

    auto _sprite = Sprite::create();
    auto frame = frameCache->getSpriteFrameByName("xucxac1.png");
    _sprite->setSpriteFrame(frame);
    _sprite->setAnchorPoint(Vec2(0.5, 1));
    _sprite->setPosition(Vec2(this->posFrame.x, this->posFrame.y));
    _sprite->setScale(0.9);
    this->addChild(_sprite);
    CallFunc* call = CallFunc::create(CC_CALLBACK_0(LayerAnimationTaiXiu::CallFuncRemove, this));

    _sprite->runAction(Sequence::create(animN, call, NULL));

    return true;
}

void LayerAnimationTaiXiu::CallFuncRemove(){
    if (this->m_callback2 && this->m_callbackListener2) {
        (this->m_callback2->*this->m_callbackListener2)(this);
    }
    this->stopAllActions();
    this->removeFromParent();
}

void LayerAnimationTaiXiu::SetCallBackResult(Ref* target, SEL_CallFuncO callfun){
    this->m_callback2 = target;
    this->m_callbackListener2 = callfun;
}

