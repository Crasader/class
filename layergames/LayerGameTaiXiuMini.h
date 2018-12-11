//
//  LayerGameTaiXiuMini.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 6/22/16.
//
//

#ifndef LayerGameTaiXiuMini_hpp
#define LayerGameTaiXiuMini_hpp

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
class LayerGameTaiXiuMini : public cocos2d::Layer,public PlayerCallBack
{
public:
    LayerGameTaiXiuMini();
    ~LayerGameTaiXiuMini();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
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
    void onBtnOpen(Ref* pSender);
    CREATE_FUNC(LayerGameTaiXiuMini);
public:
    ImageView* imgBGTimer;
private:
    ImageView* menu;
    Size visibleSize;
    LayerPlayGameTXMini* layerPlayerGame;
    EventListenerTouchOneByOne *listener;
    bool isTouchMove = false;
    
    Text*  txtTimer;
    ThreadQueueMsg* mQueueMsg;
    int PlayTurnTime = 0;
    int TimerCountDown = 0;
    boost::shared_ptr<ISFSObject> param;
};


#endif /* LayerGameTaiXiuMini_hpp */
