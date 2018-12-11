//
//  LayerSettings.h
//  demo
//
//  Created by AlexDao on 5/24/14.
//
//

#ifndef demo_LayerSettings_h
#define demo_LayerSettings_h

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

class LayerSettings
: public cocos2d::Layer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
{
private:
    cocos2d::Label *lblMoney;
    ControlSlider* sliderMoney;
    ui::EditBox* txtPassword;
    Sprite* spriteMoney;

	MenuItem* btnVibrate;
	MenuItem* btnSound;
	cocos2d::EventListener* _touchListener;
	cocos2d::EventListenerKeyboard* _keyListener;
public:
    LayerSettings();
	virtual ~LayerSettings();
//	virtual void registerWithTouchDispatcher(void);
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void saveInfo();
	void getInfo();
    
    CREATE_FUNC(LayerSettings);
    
	// CCBSelectorResolver interfaces
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref * pTarget, const char* pSelectorName);
	virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
	virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
	virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(Ref * pTarget, const char* pSelectorName){
		return NULL;
	};
    // selector callbacks
    void onButtonSound(Ref* pSender);
    void onButtonVibrate(Ref* pSender);
    void onButtonClose(Ref* pSender);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
};

class LayerSettingsLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LayerSettingsLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LayerSettings);
};

#endif
