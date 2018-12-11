#ifndef __LayerChatWindow_SCENE_H__
#define __LayerChatWindow_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocosbuilder/CocosBuilder.h"
#include "cocostudio/CocoStudio.h"
#include <vector>
#include <string>


USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocosbuilder;
using namespace cocostudio;

class LayerChatWindow : public cocos2d::Layer 
{
private:
	int _count;
	TextField* txtChat;

	Ref* m_callback;
	SEL_CallFuncND m_callbackListener;

	vector<string> lstStringMessage;
	vector<Armature*> lstEmo;

	Layer* layerButton;
	cocos2d::EventListener* _touchListener;
public:
	~LayerChatWindow();
	//virtual void registerWithTouchDispatcher(void);
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void showChatWindow();

	/*
	 * only 8 message
	 */
	void setListChatMessage(vector<string> lst);

	void setCallbackFunc(Ref* target, SEL_CallFuncND callfun);

	void pauseAllAnimations();

	void resumeAllAnimations();

	virtual bool init(); 
	void dataLoaded(float percent);

	static cocos2d::Scene* scene();

	// a selector callback
	void onButtonClose(Ref* pSender);
	void onButtonSend(Ref* pSender);
	void onButtonChats(Ref* pSender);

	void OnEmoClick(Ref* pSender);

	void textFieldEvent(Ref *pSender, TextField::EventType type);

	//LoadingBar *loadingBar;

	CREATE_FUNC(LayerChatWindow);
};

class LayerChatWindowLoader : public cocosbuilder::LayerLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LayerChatWindowLoader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LayerChatWindow);
};

#endif
