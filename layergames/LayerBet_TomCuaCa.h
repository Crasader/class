//
//  LayerBet_TomCuaCa.h
//  demo
//
//  Created by Tuan on 6/25/14.
//
//

#ifndef demo_LayerBet_TomCuaCa_h
#define demo_LayerBet_TomCuaCa_h

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocosbuilder/CocosBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocosbuilder;

class LayerBet_TomCuaCa
	: public cocos2d::CCLayer
	, public cocosbuilder::CCBSelectorResolver
	, public cocosbuilder::CCBMemberVariableAssigner
	, public cocosbuilder::NodeLoaderListener
{
private:
	Label *lblMoney;
	ControlSlider* sliderMoney;
	Sprite* spriteMoney;
	Button* btnBetParent;
public:
	int _tag;
	LayerBet_TomCuaCa();
	virtual ~LayerBet_TomCuaCa();
	//virtual void registerWithTouchDispatcher(void);
	bool onTouchBegan(Touch *pTouch, Event *pEvent);

	virtual void onEnter() override;
	virtual void onExit() override;

	CREATE_FUNC(LayerBet_TomCuaCa);

	// CCBSelectorResolver interfaces
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref * pTarget, const char* pSelectorName);
	virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
	virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
	virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(Ref * pTarget, const char* pSelectorName){
		return NULL;
	};
	// selector callbacks
	void onButtonCreate(Ref* pSender);
	void onButtonClose(Ref* pSender);

	void valueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
	void setAID(int _mtag);
	void setMoney(double _money);

	void setBtnBetFromParent(Button *btn);
};

class LayerBet_TomCuaCaLoader : public cocosbuilder::LayerLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LayerBet_TomCuaCaLoader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LayerBet_TomCuaCa);
};

#endif
