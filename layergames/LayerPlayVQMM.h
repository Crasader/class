//
//  LayerPlayVQMM.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 8/31/16.
//
//

#ifndef LayerPlayVQMM_hpp
#define LayerPlayVQMM_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ThreadQueueMsg.h"
#include "GameServer.h"
#include "CircleAvatar.h"
#include "RoundLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPlayVQMM : public cocos2d::Layer,public PlayerCallBack
{
public:
    LayerPlayVQMM();
    ~LayerPlayVQMM();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

    void onBtnClose(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnSuKien(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnHuongDan(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnLichSu(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnXepHang(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnPlay(Ref *sender, ui::Widget::TouchEventType pType);

    
//    //Cheat
//    void onBtnNhanQT(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnKetQua(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnSetQT(Ref *sender, ui::Widget::TouchEventType pType);

    void resetGame();
    
    void onBtnMuaLuotQuay(Ref* pSender);


    void showResult();
    
    void startGame();
    
    void event_EXT_EVENT_GAME_RESULT();
    void event_EXT_EVENT_GAME_END();
    void event_EXT_EVENT_GAME_STATE_NTF();
    void event_EXT_EVENT_GAME_BET_NTF();
    void event_EXT_EVENT_GAME_BET_ERROR_NOTIFY_VQMINI();
    void actionRotateDone();
    void createListBet();
    
    void updateEvent(float dt);
    void OnExtensionResponseProcess();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void getMinMoney();

    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerPlayVQMM);
private:
    EventListenerTouchOneByOne* listener;
    ThreadQueueMsg* mQueueMsg;

    boost::shared_ptr<ISFSObject> param;
    Size sizeAdd;
    
    TextField* txtKetqua;
    
    Layout* pnlBg;
    Button* btnPlay;
    double currMyMoney = 0;
    double MinBetValue = 0;
    
    ImageView* imgVong1;
    ImageView* imgVong2;
    ImageView* imgVong3;
    
    Button* btnVong1;
    Button* btnVong2;
    Button* btnVong3;

    vector<int> listBet1;
    vector<int> listBet2;
    vector<int> listBet3;
    
    Text* imgPrice;
   
    bool isMuaLuotQuay = false;
    bool isJoinGame = false;
    int gia = 1000;
    int currentVongQuay = 1;
    string rs = "";
    int prize = -1;
    int typePrize = -1;
    int value = 0;
    bool isDangQuay = false;
    string result = "";
    bool isNoHu = false;
    unsigned int effect;
    string PATH_VQ = "design/VQMMMini/";
};

#endif /* LayerPlayVQMM_hpp */
