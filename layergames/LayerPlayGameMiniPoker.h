//
//  LayerPlayGameMiniPoker.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 8/31/16.
//
//

#ifndef LayerPlayGameMiniPoker_hpp
#define LayerPlayGameMiniPoker_hpp

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
class LayerPlayGameMiniPoker : public cocos2d::Layer,public PlayerCallBack
{
public:
    LayerPlayGameMiniPoker();
    ~LayerPlayGameMiniPoker();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

    void onBtnClose(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnSuKien(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnHuongDan(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnLichSu(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnXepHang(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnPlay(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnAutoPlay(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnBet100(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnBet1K(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnBet10K(Ref *sender, ui::Widget::TouchEventType pType);
    
//    //Cheat
//    void onBtnNhanQT(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnKetQua(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnSetQT(Ref *sender, ui::Widget::TouchEventType pType);


    void getHuThuong(float dt);
    void startRound();
    void startRound1();
    void startRound2();
    void startRound3();
    void startRound4();
    void startRound5();
    void stopRound(string str);
    void stopRound1(int tag);
    void stopRound2(int tag);
    void stopRound3(int tag);
    void stopRound4(int tag);
    void stopRound5(int tag);
    void showResult();
    string getStrImageRS(int rsc);
    void playAnimationMachine();
    void getMinMoney();
    void createAnimPlay();
    void resetGame();
    void playAnimStartGame();
    void startGame();
    void updateTimerStart(float dt);
    void showResultPrevMatch(string str);
    
    void event_EXT_EVENT_GAME_START();
    void event_EXT_EVENT_GAME_RESULT();
    void event_PK_GET_QUY_THUONG();
    void event_EXT_EVENT_GAME_STATE();
    void event_EXT_EVENT_GAME_END();
    void event_EXT_EVENT_GAME_AUTO_RESP();
    
    void updateEvent(float dt);
    void tangHuPK(float dt);
    void OnExtensionResponseProcess();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void onBtnSilver(Ref *sender);
    void onBtnGold(Ref *sender);
    CREATE_FUNC(LayerPlayGameMiniPoker);
private:
    EventListenerTouchOneByOne* listener;
    ThreadQueueMsg* mQueueMsg;

    boost::shared_ptr<ISFSObject> param;
    RoundLayer* round1;
    RoundLayer* round2;
    RoundLayer* round3;
    RoundLayer* round4;
    RoundLayer* round5;
    ImageView* imgAnimMachine;
    ImageView* imgAnimPress;
    Sprite* sprRob;
    Animation* anim;
    Sprite* sprDown;
    Text*   txtMyMoney;
    long  currMyMoney = 0;
    Button* btn100;
    Button* btn1k;
    Button* btn10k;
    long currBetMoney = 100;
    Text* txtTongHu;
    Text* txtPhien;
    Button* btnAutoPlay;
    bool autoPlay = false;
    ImageView* imgAutoPlay;
    bool    CurrentGameState = false;
    string  strResult;
    long winMoney = 0;
    vector<int> arrTagCard;
    Button* btnPlay;
    ImageView* imgResult;
    Layout* pnlBg;
    int currCodeRS = 0;
    int MinBetValue = 0;
    bool Animating = false;
    int TAG_RESULT_MONEY = 10;
    int TAG_RESULT_IMG = 11;
    long currHuThuong = 0;
    //Text* txtCheatKetQua;
    //strCheat:"",
    long chenhlechHuThuong = 0;
    bool isNoHu = false;
    int numberAuto = 0;
    int timerStart = 0;
    
    ImageView* imgBonusQT;
    
    ImageView* imgLucky1;
    ImageView* imgLucky2;
    ImageView* imgLucky3;
    
    
    
//    //Cheat
//    TextField* txtNhanQT;
    TextField* txtKetqua;
    TextField* txtQT;
    
    Size sizeAdd;
    bool ishas7 = false;
    string strlst7;
    
    Button* btnGold;
    Button* btnSilver;
    int currTypeMoney = 0;
    unsigned int effect;
    string PATH_PK = "design/LayerMiniPoker/";
    int fakeHuPK = 0;
    int huPK = 0;
};

#endif /* LayerPlayGameMiniPoker_hpp */
