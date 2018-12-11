#include "SceneLoading.h"
#include "../SceneManager.h"
#include "../layergames/NativeHelper.h"

USING_NS_CC;

SceneLoading::SceneLoading()
{

}

SceneLoading::~SceneLoading()
{
}

bool SceneLoading::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();



	auto logoSprite = Sprite::create("background.jpg");
	logoSprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	//logoSprite->setScaleX(visibleSize.width / 1920);
	//logoSprite->setScaleY(visibleSize.height / 1079);
	this->addChild(logoSprite);

    this->runAction(Sequence::create(DelayTime::create(DISPLAY_TIME_LOGO_SCENE),CallFunc::create(CC_CALLBACK_0(SceneLoading::goToScenemanager, this)),NULL));
	//loading();

	//label Loading
	//auto labelLoading = cocos2d::Label::createWithTTF("Loading...", kNormalFont, FONT_SIZE_LOADING_LABEL);
	//labelLoading->setPosition(Point(logoSprite->getPositionX(), visibleSize.height / 5 - POSITIONY_LOADING_LABEL));
	//this->addChild(labelLoading);

	//Director::getInstance()->getScheduler()->scheduleUpdate();
	return true;
}

void SceneLoading::goToScenemanager()
{
    SceneManager* _sceneManager = SceneManager::create();
    _sceneManager->retain();
    _sceneManager->gotoLogin();
}

void SceneLoading::loading(){

//	Size visibleSize = Director::getInstance()->getVisibleSize();
//
//	//loading animation
//	SpriteBatchNode* spritebatch = SpriteBatchNode::create("loading.png");
//
//	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
//	cache->addSpriteFramesWithFile("loading.plist");
//
//	auto Sprite1 = Sprite::createWithSpriteFrameName("loading1.png");
//	Sprite1->setPosition(Point(visibleSize.width / 2, visibleSize.height / 5));
//	spritebatch->addChild(Sprite1);
//	this->addChild(spritebatch);
//
//	Vector<SpriteFrame*> animFrames(15);
//
//	char str[100] = { 0 };
//	for (int i = 2; i < 8; i++)
//	{
//		sprintf(str, "loading%d.png", i);
//		SpriteFrame* frame = cache->getSpriteFrameByName(str);
//		animFrames.pushBack(frame);
//	}
//
//	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
//	Sprite1->runAction(RepeatForever::create(Animate::create(animation)));

}

void SceneLoading::update(float dt){
	loading();
}
