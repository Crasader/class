//
//  LayerLauncherMiniGame.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/1/16.
//
//

#ifndef LayerLauncherMiniGame_hpp
#define LayerLauncherMiniGame_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "LayerPlayGameTXMini.h"
#include "ThreadQueueMsg.h"
#include "../GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerLauncherMiniGame : public cocos2d::Layer,public PlayerCallBack
{
public:
    LayerLauncherMiniGame();
    ~LayerLauncherMiniGame();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    void onTimerToHideButton(float dt);
    void startWaittoHide();
    void stopWaitWhenTouch();
    
    void updateEvent(float dt);
    void OnExtensionResponseProcess();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    void event_EXT_EVENT_START();
    void event_EXT_EVENT_END();
    void event_EXT_EVENT_GAME_STATE_NTF();
    
    void UpdateTimeStartGame();
    void updateTimer(float dt);
    void StopMyTimer();
    void onButtonMiniGame(Ref* pSender);

    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnOpen(Ref* pSender);
    CREATE_FUNC(LayerLauncherMiniGame);
public:
    ImageView* imgBGTimer;
    ImageView* menu;
private:
    Size visibleSize;
    EventListenerTouchOneByOne *listener;
    bool isTouchMove = false;
    bool isShow = true;
    bool isHaveHiden = false;
    Text*  txtTimer;
    ThreadQueueMsg* mQueueMsg;
    int PlayTurnTime = 0;
    int TimerCountDown = 0;
    int timerToHide = 3;
    boost::shared_ptr<ISFSObject> param;
    ImageView* imgketqua;
    Layout* pnlBg;
    int sizeMove = 645;
    Button* btnPoker;
    Button* btnSapNgua;
    Button* btnCaoThap;
    Button* btnVongQuay;

};


#endif /* LayerLauncherMiniGame_hpp */
