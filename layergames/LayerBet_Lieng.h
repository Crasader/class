//
//  LayerBet_Lieng.h
//  demo
//
//  Created by AlexDao on 5/24/14.
//
//

#ifndef demo_LayerBet_Lieng_h
#define demo_LayerBet_Lieng_h

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

class LayerBet_Lieng
: public cocos2d::Layer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
{
private:
	Label*			lblMoney;
	ControlSlider*	sliderMoney;
	Sprite*			spriteMoney;

	Label*			lblTitle1;
	Label*			lblTitle2;
	Label*			lblTitle3;

	long mMinBet;
	long mCurrentBet;
	cocos2d::EventListener* _touchListener;

public:
	LayerBet_Lieng();
	virtual ~LayerBet_Lieng();

	//virtual void registerWithTouchDispatcher(void);
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	CREATE_FUNC(LayerBet_Lieng);
    
	// CCBSelectorResolver interfaces
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref * pTarget, const char* pSelectorName);
	virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
	virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
	virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(Ref * pTarget, const char* pSelectorName){
		return NULL;
	};

	void SetInfoBet(const int& pMinBet, const int& pCurrentBet);

protected:
	void OnButtonCreate(Ref* pSender);
	void OnButtonClose(Ref* pSender);

	void SliderValueChanged(Ref* sender, cocos2d::extension::Control::EventType controlEvent);
};

class LayerBet_LiengLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LayerBet_LiengLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LayerBet_Lieng);
};

#endif
