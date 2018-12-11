//
//  LayerPlayGameSlotMachine.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 8/31/16.
//
//

#ifndef LayerPlayGameSlotMachine_hpp
#define LayerPlayGameSlotMachine_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ThreadQueueMsg.h"
#include "GameServer.h"
#include "CircleAvatar.h"
#include "RoundLayerSlot.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPlayGameSlotMachine : public cocos2d::Layer,public PlayerCallBack
{
public:
    LayerPlayGameSlotMachine();
    ~LayerPlayGameSlotMachine();
    virtual bool init();
    void onHieuUngCallBack(Ref* pSender);
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
    void onBtnLineBack(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnLineNext(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnBet(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnBetMax(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnMulti(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnChooseLine(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnRecharge(Ref *sender, ui::Widget::TouchEventType pType);
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
    void stopRound1(string tag);
    void stopRound2(string tag);
    void stopRound3(string tag);
    void stopRound4(string tag);
    void stopRound5(string tag);
    void showResult();
    string getStrImageRS(int rsc);
    void playAnimationMachine();
    void getMinMoney();
    void createAnimPlay();
    void resetGame();
    void playAnimStartGame();
    void startGame();
    void updateTimerStart(float dt);
    void updateMoneyWin(float dt);

    void showResultPrevMatch(string str);
    void loadChooseLine(string str);
    
    void drawLine(int line);
    void drawChooseLine(string str);
    void showWinLine();

    
    void event_EXT_EVENT_GAME_START();
    void event_EXT_EVENT_GAME_RESULT();
    void event_PK_GET_QUY_THUONG();
    void event_EXT_EVENT_GAME_STATE();
    void event_EXT_EVENT_GAME_END();
    void event_EXT_EVENT_GAME_AUTO_RESP();
    void event_EXT_EVENT_GAME_ROTATE_RES();
    
    void event_EXT_FIELD_GAME_INFO();
    void event_EXT_EVENT_GAME_AUTO_RES();
    void event_EXT_EVENT_SLOT_GAME_START();
    
    void updateEvent(float dt);
    void updatemotion(float dt);

    void OnExtensionResponseProcess();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void onBtnSilver(Ref *sender);
    void onBtnGold(Ref *sender);
    void onBtnChuyenDongTien(Ref* sender);
    void onBtnBetUp(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnBetDown(Ref *sender, ui::Widget::TouchEventType pType);
    void drawLineNew(int line, vector<int> item);
    void resetLine(int currLine);
    void pushItemEat1(int item,int row);
    void pushItemEat2(int item,int row);
    void pushItemEat3(int item,int row);
    void pushItemEat4(int item,int row);
    void pushItemEat5(int item,int row);
    void animSlotMachineWinLine();
    void onBtnRank(Ref *sender, ui::Widget::TouchEventType pType);
    void stopNoHuAnimmation();
    CREATE_FUNC(LayerPlayGameSlotMachine);
private:
    EventListenerTouchOneByOne* listener;
    ThreadQueueMsg* mQueueMsg;

    boost::shared_ptr<ISFSObject> param;
    RoundLayerSlot* round1;
    RoundLayerSlot* round2;
    RoundLayerSlot* round3;
    RoundLayerSlot* round4;
    RoundLayerSlot* round5;
    ImageView* imgAnimMachine;
    ImageView* imgAnimPress;
    Sprite* sprRob;
    Animation* anim;
    Sprite* sprDown;
    Text*   txtMyMoney;
    long  currMyMoney = 10000;
    Button* btn100;
    Button* btn1k;
    Button* btn10k;
    long currBetMoney = 10000;
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
    
    //New Slot
    ImageView* bgChooseLine;
    vector<Button*> lstChooseLine;
    Text* txtLineCount;
    string lsLines= "";
    vector<ImageView*> lstpointXLines;
    
    int countLine = 9;
    
    vector<ImageView*> lstlineLeft;
    vector<ImageView*> lstlineRight;
    
    Button* btnLineBack;
    Button* btnLineNext;

    vector<string> itemResult;
    vector<DrawNode*> lstDrawLine;
    string strLineWin = "";
    
    Text* txtTienThang;
    Text* txtTongDat;
    
    vector<int> lstBet;
    Button* btnBet;
    int currPosBet;
    int winvalue = 0;
    int lastMoney = 0;
    int winvalue2 = 0;

    Button* btnMax;
    Button* btnMulti;
    Text* txtMucCuoc;
    string PATH_SLOT = "design/ResSlot/";
    MotionStreak* _streak;
    Sprite* _sprite ;
    ImageView* imgMoneyTable;
    ImageView* imgMoneyType;
    bool isResult = false;
    bool isJoin = false;
    
    Text* txtMoneyHuThuong;
    Button* btnBetUp;
    Button* btnBetDown;
    int GameLine = 17;
    ImageView* imgCuoc;
    vector<Color3B> colors;
    ImageView* bgWin;
    Text* txtMoneyWinBig;
    vector<vector<int>> currRows;
    vector<int> currLineDraws ;
    vector<vector<int>> currLines;
    vector<vector<int>> currItems;
    int currShow = 0;
    ImageView* bgNoHu;
    vector<ImageView*> lstItemNoHu;
    Text* txtMoneyNoHu;
    double moneyNoHu;
    string lineNoHu;
    vector<DrawNode*> lstDraw;

};

#endif /* LayerPlayGameSlotMachine_hpp */
