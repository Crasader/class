//
//  LayerSettings.cpp
//  demo
//
//  Created by AlexDao on 5/24/14.
//
//

#include "LayerSettings.h"
#include "mUtils.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace cocos2d::extension;


LayerSettings::LayerSettings()
{
    lblMoney=NULL;
    sliderMoney=NULL;
    txtPassword=NULL;
    spriteMoney=NULL;

	btnSound = NULL;
	btnVibrate = NULL;
}

LayerSettings::~LayerSettings()
{
	CC_SAFE_RELEASE(lblMoney);
	CC_SAFE_RELEASE(sliderMoney);
	CC_SAFE_RELEASE(txtPassword);
	CC_SAFE_RELEASE(spriteMoney);

	CC_SAFE_RELEASE(btnSound);
	CC_SAFE_RELEASE(btnVibrate);
	log("LayerSettings Destructor");
}

// CCBSelectorResolver interface
SEL_MenuHandler LayerSettings::onResolveCCBCCMenuItemSelector(Ref *pTarget, const char *pSelectorName)
{
   // log("onResolveCCBCCMenuItemSelector");
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnSound", LayerSettings::onButtonSound);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnVibrate", LayerSettings::onButtonVibrate);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnClose", LayerSettings::onButtonClose);
    return NULL;
}

void LayerSettings::onButtonSound(Ref* pSender)
{
   // log("onButtonSound");
    MenuItemImage* btn = (MenuItemImage*)pSender;
   // log("onButtonSound x:%f", btn->getPositionX());
	bool bVal = true;
    if( btn->getPositionX()==211 ){
        //disabling, switch to enable
        btn->setPosition(Point(270, btn->getPositionY()));
		bVal = false;
    }else{
        //else
        btn->setPosition(Point(211, btn->getPositionY()));
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
	//
	UserDefault *def=UserDefault::getInstance();
	def->setBoolForKey("sound", bVal);
}

void LayerSettings::onButtonVibrate(Ref* pSender)
{
	log("onButtonVibrate");
	bool bVal = true;
    MenuItemImage* btn = (MenuItemImage*)pSender;
   // log("onButtonSound x:%f", btn->getPositionX());
    if( btn->getPositionX()==211 ){
        //disabling, switch to enable
		bVal = false;
        btn->setPosition(Point(270, btn->getPositionY()));
    }else{
        //else
        btn->setPosition(Point(211, btn->getPositionY()));
	}
	//
	UserDefault *def=UserDefault::getInstance();
	def->setBoolForKey("vibrate", bVal);
}

void LayerSettings::onButtonClose(Ref* pSender)
{
   // log("onButtonClose");
    this->removeFromParent();
}

// CCBMemberVariableAssigner interface
bool LayerSettings::onAssignCCBMemberVariable(Ref *pTarget, const char *pMemberVariableName, cocos2d::Node *pNode)
{
   //// log("Imhere onAssignCCBMemberVariable: %s", pMemberVariableName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnVibrate", MenuItem*, btnVibrate);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnSound", MenuItem*, btnSound);
    return true;
}

void LayerSettings::onNodeLoaded( Node * pNode,  NodeLoader * pNodeLoader)
{
	//
	getInfo();
	//add key touch handler
	_keyListener = EventListenerKeyboard::create();
	_keyListener->onKeyReleased = CC_CALLBACK_2(LayerSettings::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyListener, this);
    return;
}

// void LayerSettings::registerWithTouchDispatcher( void )
// {
// 	Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
// }

bool LayerSettings::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerSettings::onEnter()
{
	Layer::onEnter();
	//
	this->runAction(mUtils::getActionOpenPopup());
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerSettings::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerSettings::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	dispatcher->removeEventListener(_keyListener);
	Layer::onExit();
}

void LayerSettings::saveInfo()
{
// 	CCUserDefault *def=CCUserDefault::sharedUserDefault();
// 	def->setStringForKey("sound", txtUsername->getText());
// 	def->setStringForKey("vibrate", txtPassword->getText());

//	def->flush();
}

void LayerSettings::getInfo()
{
	UserDefault *def=UserDefault::getInstance();
	bool isSoundOn = def->getBoolForKey("sound", true);
	bool isVibrateOn = def->getBoolForKey("vibrate", true);

	btnSound->setPositionX( !isSoundOn?270:211);
	btnVibrate->setPositionX( !isVibrateOn?270:211);
}

void LayerSettings::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
			this->removeFromParentAndCleanup(true);
	}
}
