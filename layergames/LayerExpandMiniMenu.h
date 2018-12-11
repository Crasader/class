//
//  LayerExpandMiniMenu.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/1/16.
//
//

#ifndef LayerExpandMiniMenu_hpp
#define LayerExpandMiniMenu_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ThreadQueueMsg.h"
#include "../GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class LoaderNode : public Layer{
public:
	LoaderNode();
	~LoaderNode();
	CREATE_FUNC(LoaderNode);
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	typedef std::function<void(LoaderNode*, int)> loaderCallback;
	void setCallBack(const loaderCallback& callback);
	CC_SYNTHESIZE(int, tagCb, TagCallBack);

	void startCheckUpdate();
private:
	loaderCallback _callback;
	ProgressTimer* bgTimer;
	Label* lbTimer;
	void onLoadEnd();
	void createLoadUI();
};

class LayerExpandMiniMenu : public Layer,public PlayerCallBack
{
public:
    LayerExpandMiniMenu();
    ~LayerExpandMiniMenu();
    
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    void onBtnMenu(Ref* pSender);
    void onBtnPK(Ref* pSender);
    void onBtnTX(Ref* pSender);
    void onBtnTCC(Ref* pSender);
    void closeMenu();
    
    void updateEvent(float dt);
    void OnExtensionResponseProcess();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    void event_EXT_EVENT_START();
    void event_EXT_EVENT_END();
    void event_EXT_EVENT_GAME_STATE_NTF();
    
    void UpdateTimeStartGame();
    void updateTimer(float dt);
    void StopMyTimer();

    
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(LayerExpandMiniMenu);
private:
    EventListenerTouchOneByOne *listener;
    ImageView* imgBg;
    
    ImageView* imgBGTimer;
    
    Text*  txtTimer;
    ThreadQueueMsg* mQueueMsg;
    int PlayTurnTime = 0;
    int TimerCountDown = 0;
    boost::shared_ptr<ISFSObject> param;

	void callBackFromLoader(LoaderNode* sender, int tagButton);
	Button* btnTX;
	Button* btnPK;
	Button* btnTCC;
    Size sizeAdd;
    Size visibleSize;

};

#endif /* LayerExpandMiniMenu_hpp */
