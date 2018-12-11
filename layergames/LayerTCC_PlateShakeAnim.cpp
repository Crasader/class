#include "LayerTCC_PlateShakeAnim.h"
#include "LayerTCC_PopupMain.h"

LayerTCC_PlateShakeAnim::LayerTCC_PlateShakeAnim(Vec2 pos, int type)
{
	posFrame = pos;
	curType = type;
	fnTarget = NULL;
}

LayerTCC_PlateShakeAnim::~LayerTCC_PlateShakeAnim()
{
	this->stopAllActions();
}

bool LayerTCC_PlateShakeAnim::init()
{
	if (!Layer::init())
		return false;

	cocos2d::Vector<SpriteFrame*> animFrames;
	SpriteFrame *frame;
	Animation *animFrame;
	string str = "";
	for (int i = 3; i >= 1; i--){
		str = StringUtils::format("dia-%d.png", i);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	animFrame = Animation::createWithSpriteFrames(animFrames, 1);
	Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.1, 6);

	auto animCache = AnimationCache::getInstance();
	animCache->addAnimation(animation, "dance");
	Animation* normal = animCache->getAnimation("dance");
	normal->setRestoreOriginalFrame(true);

	Animate* animN = Animate::create(normal);
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("dia-3.png");

	Sprite *_sprite = Sprite::createWithSpriteFrame(frame);
	_sprite->setAnchorPoint(Vec2::ZERO);
	_sprite->setPosition(posFrame);
	_sprite->setScale(1.3);
	this->addChild(_sprite);

	CallFunc* call = CallFunc::create(CC_CALLBACK_0(LayerTCC_PlateShakeAnim::CallFuncRemove, this));
	Action* move = MoveTo::create(0.7, Vec2(posFrame.x - 150, posFrame.y));
	Action* scale = ScaleTo::create(0.2, 1.2);
	_sprite->runAction(Sequence::create(animN, call, nullptr));

	return true;
}

LayerTCC_PlateShakeAnim* LayerTCC_PlateShakeAnim::create(Vec2 pos, int type)
{
	LayerTCC_PlateShakeAnim *layerAnim = new LayerTCC_PlateShakeAnim(pos, type);
	if (layerAnim && layerAnim->init()){
		layerAnim->autorelease();
		return layerAnim;
	}
	else{
		delete layerAnim;
		layerAnim = NULL;
		return NULL;
	}
}

void LayerTCC_PlateShakeAnim::setTarget(Ref* _target)
{
	fnTarget = _target;
}

void LayerTCC_PlateShakeAnim::CallFuncRemove()
{
// 	LayerTCC_PopupMain* parent = dynamic_cast<LayerTCC_PopupMain*> (this->getParent());
// 	if (parent)
// 	{
// 		parent->CallBackFromAnim();
// 	}
	this->setVisible(false);
	this->runAction(Sequence::create(CallFunc::create(listener), DelayTime::create(0.2), RemoveSelf::create(), NULL));
	
	//this->removeFromParent();
}

void LayerTCC_PlateShakeAnim::setCallBackListener(const std::function<void()> &listener)
{
	this->listener = listener;
}
