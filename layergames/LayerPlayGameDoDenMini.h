//
//  LayerPlayGameDoDenMini.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 6/22/16.
//
//

#ifndef LayerPlayGameDoDenMini_hpp
#define LayerPlayGameDoDenMini_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ThreadQueueMsg.h"
#include "GameServer.h"
#include "CircleAvatar.h"
#include "LayerChatMiniGame.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerAnimationDoDenMini : public cocos2d::Layer
{
public:
    LayerAnimationDoDenMini();
    LayerAnimationDoDenMini(Vec2& pos);
    static LayerAnimationDoDenMini* createInstance(Vec2& pos);
    void SetCallBackResult(Ref* target, SEL_CallFuncO callfun);
    void CallFuncRemove();
protected:
    bool init();
private:
    Ref* m_callback2;
    SEL_CallFuncO m_callbackListener2;
    Vec2 posFrame;
};
class LayerPlayGameDoDenMini : public cocos2d::Layer, public PlayerCallBack
{
public:
    LayerPlayGameDoDenMini();
    ~LayerPlayGameDoDenMini();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onBtnBetTai(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnBetXiu(Ref *sender, ui::Widget::TouchEventType pType);
    void onLichSuPhien(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnResetTienCuoc(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnChonTienCuoc(Ref *sender, ui::Widget::TouchEventType pType);
    void onButtonClose(Ref *sender, ui::Widget::TouchEventType pType);
    void onButtonLichSu(Ref *sender, ui::Widget::TouchEventType pType);
    void onButtonXepHang(Ref *sender, ui::Widget::TouchEventType pType);
    void onButtonSukien(Ref *sender, ui::Widget::TouchEventType pType);
    void onButtonHuongDan(Ref *sender, ui::Widget::TouchEventType pType);
    void onButtonThongKe(Ref* pSender);
    void createAvatar();
    void ResetGame(bool state);
    void sendRequestInfoGame();
    
    void updateEvent(float dt);
    void OnExtensionResponseProcess();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    //cheat
    void onBtnSet(Ref *sender);
    
    void event_EXT_EVENT_START();
    void event_EXT_EVENT_END();
    void event_EXT_EVENT_GAME_RESULT();
    void event_EXT_EVENT_GAME_BET_ERROR_NOTIFY();
    void event_EXT_EVENT_GAME_INFO_NTF();
    void event_EXT_EVENT_GAME_HIS_NTF();
    void event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF();
    void event_EXT_EVENT_GAME_STATE_NTF();
    void event_EXT_EVENT_GAME_BET_NTF();
    void event_EXT_EVENT_SHOW_CANCUA();
    //moc3-new

    
    void showHistory();
    void UpdateTimeStartGame();
    void updateTimer(float dt);
    void StopMyTimer();
    void CallBackResult(Ref *pSend);
    void CallBackFromAnim();
    void showResultAfter();
    void showResult();
    string GetUrlXiNgau(int number);
    void showResultGame(string money, string result);
    void UpdateTimeEndGame();
    void updateToNextTurn(float dt);
    void showTotalPlayer(string str);
    void ReloadGameWhenReturn(string gbt);
    void showPlayer(string str);
    void showTongTienBet(string str);
    void RollBackBetValue(string betType, string betValue);
    void updateMoneyTraThuong(float dt);
    void getMinMoney();
    void showPrevMatchHis(string str);
    void showAnimationAfterCanCua();
    void onBtnSilver(Ref *sender);
    void onBtnGold(Ref *sender);
    void GetQuy(float dt);
    void onBtnDatCuoc(Ref* pSender);
    void onBtnCuocTatCa(Ref* pSender);
    void onBtnHuyCuoc(Ref* pSender);
    void onBtnChonTien(Ref* pSender);
    void chucnangNan();
    void timeNanBat(float dt);
    void onBtnNan(Ref* pSender);
    void showKetQua();
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(LayerPlayGameDoDenMini);
private:
    EventListenerTouchOneByOne* listener;
    ThreadQueueMsg* mQueueMsg;
    ImageView* imgTai;
    ImageView* imgXiu;
    Text* txtTongBacTai;
    Text* txtTongBacXiu;
    Text* txtSoNguoiTai;
    Text* txtSoNguoiXiu;
    Button* btnDatTai;
    Button* btnDatXiu;
    Text* txtDaDatTai;
    Text* txtDaDatXiu;
    vector<ImageView*> lstimgHis;
    bool CurrentGameState = false;
    ImageView* imgBat;
    Label* txtTimerP2;
    Label* lblMoneyTraThuong;
    Text* txtTimer;
    vector<string> listHistory;
    boost::shared_ptr<ISFSObject> param;
    vector<int> arrayValueBtnCuoc;
    int tongSoTienDinhCuoc = 0;
    int PlayTurnTime = 60;
    int BetweentwoPlayTime = 20;
    int TimerCountDown = 60;
    int TimerCountNextTime = 20;
    int MoneyTraThuong = 9999999;
    //_pTimer: null,
    
    string rg = "";
    string rt10 = "";
    vector<string> listTopWinner;
    TextField* txtMoneyBetPlayer;
    ImageView* resultDice1;
    ImageView* resultDice2;
    ImageView* resultDice3;
    int currentValueTai = 0;
    int currentValueXiu = 0;
    int rangeValueTai;
    int rangeValueXi = 0;
    Text* txtPhienChoi;
    int currBetTai = 0;
    int currBetXiu = 0;
    int currTongBetTai = 0;
    int currTongBetXiu = 0;
    bool HoverFlag = true;
    ImageView* imgHisNow;
    
    bool firstTimeOpen = false;
    
    vector<ImageView*> lstHisDetails;
    vector<Text*> lstTxtDetails;
    //hoverlistener:null,
    string currResult = "";
    int currSonguoiTai = 0;
    int currSonguoiXiu = 0;
    bool isAddMeBetTai = false;
    bool isAddMeBetXiu = false;
    
    ImageView* imgTongDiem;
    Text* txtTongDiem;
    ImageView* imgNextPlay;
    Text* txtMyMoney;
    ImageView* imgAvatar;
    ImageView* imgCanCua;
    int money;
    Label* lblEndTimer;
    bool isShowMoney = false;
    CircleAvatar* circleAvatar;
    double currMyMoney = 0;
    string urlAvatar;
    bool Animating = false;
    double amfUser;
    
    //int TAG_ANIM_END_GAME = 503;
    int TAG_RESULT_MONEY = 504;
    string wincell = "";
    int MinBetValue = 0;
    bool isCanCua = 0; // 0 la tai, 1 la xiu
    Text* txtTongHu;
    Size sizeAdd;
    boost::shared_ptr<string> ghs;
    //Cheat
    TextField* txtFieldCheat;
    
    //New GB2.0
    Button* btnGold;
    Button* btnSilver;
    Layout* pnlBg1;
    int currTypeMoney = 0;
    
    LayerChatMiniGame* layerChat;
    void onButtonChat(Ref* pSender);
    //moc3-new
    Layout* pnlDatCuoc;
    bool isBetTai = false;
    bool isBetXiu = false;
    Button* btnMoBat;
    bool nguoimobat = false;
    Button* btnChatClose;
    Button* btnChatOpen;

    bool IsNanBat = false;
    Button* btnNan;
    bool isTouchBat = false;
    ImageView* imgTimeNanBat;
    Text* txtTimeNanBat;
    ImageView* imgBatNan;
    Layout* pnlKetQua;
    ImageView* imgBgDo;
    ImageView* imgBgDen;

    string PATH_DD = "design/LayerDoDenMini/";
    string msg = "";
};

#endif /* LayerPlayGameDoDenMini_hpp */
