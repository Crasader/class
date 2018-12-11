#ifndef LAYERGAMETAIXIU_H
#define LAYERGAMETAIXIU_H

#include "GameServer.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LayerChanToast.h"
#include "TaiXiuBet.h"
#include "TaiXiuCoin.h"

#include "ThreadQueueMsg.h"
#include "PlayerTaiXiuInfo.h"
#include "QueueTaiXiu.h"
#include "../layers/LayerPopupCuaHang.h"
#include "../layers/LayerPopupDoiMoc.h"

using namespace cocos2d::ui;
USING_NS_CC;

class LayerGameTaiXiu :
        public cocos2d::Layer,
        public PlayerCallBack
{
public:
    LayerGameTaiXiu();
    ~LayerGameTaiXiu();
    virtual bool init();

	CREATE_FUNC(LayerGameTaiXiu);

    void OnButtonChipClick(Ref *sender,  ui::Widget::TouchEventType pType);
    void OnButtonBetClick(Ref *sender,  ui::Widget::TouchEventType pType);
    void OnButtonHistoryClick(Ref *sender,  ui::Widget::TouchEventType pType);
    void OnButtonTopWinClick(Ref *sender,  ui::Widget::TouchEventType pType);
    void OnBtnHuongDanChan(Ref *sender,  ui::Widget::TouchEventType pType);

    //Listen Server Event
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserCountChange(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxPingPong(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    //Execute Server Event
    void event_EXT_EVENT_START();
    void event_EXT_EVENT_END();
    void event_EXT_EVENT_GAME_RESULT();
    void event_EXT_FIELD_GAME_BET_EXCHANGE();
    void event_EXT_EVENT_GAME_INFO_NTF();
    void event_EXT_EVENT_GAME_HIS_NTF();
    void event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF();
    void event_EXT_EVENT_GAME_STATE_NTF();
    void event_EXT_EVENT_GAME_BET_NTF();
    void event_EXT_EVENT_GAME_BET_ERROR_NOTIFY();

    void eventTouchBtnCoin(Ref *pObject, Widget::TouchEventType pType);
	void processButtonSetTouched(Ref *pObject, Widget::TouchEventType pType);
    void CreateContainerButton(Button* btn, const char* name, const int& group, const int& tag, const char* type, const char* url);
    void CreateContainerCoin(Button* btn, const char* name, const int& tag, const char* type);

    TaiXiuBet* GetContainerBetByButtonTag(const int& tag);
    TaiXiuBet* GetContainerBetByBetType(const string& bettype);

    TaiXiuCoin* GetTaiXiuCoin(const string& type);

    int GetMoneyBet(const std::string& gbv);

    void CallBackResult(Ref *pSend);

private:

    Size SizeAdd;

    float press;
    void updatepress(float delta);

    std::vector<TaiXiuBet*>  arrBet;
    std::vector<TaiXiuCoin*> arrCoin;

    std::string MyAI;
    boost::shared_ptr<ISFSObject> param;

    //Layer Toast
    LayerChanToast* Toast;

    //3 con xuc xac ket qua
    Sprite* resultDice1;
    Sprite* resultDice2;
    Sprite* resultDice3;

    //So nguoi dang choi.
    Text* txtNumOfPlayers;

    Text* lblBetMoney;
    Text* lblWinMoney;
    Text* lblMoney;
	ImageView *iconMoney;
	TextField *tfResult;

    Text* txtTimer;

    //
    Node* rootNode;

    bool Animating;

    Sprite* signalNetwork;
private:
    int         TimerCountDown;
    int         TimerCountNextTime;

    std::string CurrentBetValue;
    bool        CurrentGameState;

    int BetweentwoPlayTime;
    int PlayTurnTime;

    int CountSizeDatCuoc;
    std::vector<std::string> arrTextHayDatCuoc;
    Label* HayDatCuoc;

    std::vector<std::string> listHistory;
    std::vector<std::string> listTopWinner;

    Label* lblTimer;
    Label* lblCountNextTime;

    int CountZoder;

    ThreadQueueMsg* mQueueMsg;

    Vec2 posFrameAnim;
    PlayerTaiXiuInfo* playerInfo;
    QueueTaiXiu* mQueueTaiXiu;

    std::string rg;
    std::string rt10;

private:
    void ResetGame();
    string GetUrlXiNgau(const int& number);

    void UpdateHayDatcuoc(float dt);

    void updateTimer(float dt);
    void updateToNextTurn(float dt);

    void ReloadGameWhenReturn(string& gbt);

    void SetRoomConfig();
    string SelectCoinValue(const double& amf, const int& ext = 0);
    void SelectCoin(const double& amf);

    void HideAllSuggestion();

    int CurrentTag;
    int OldPressTag;
    float longpresstime;

    void UpdateTimeStartGame();
    void UpdateTimeEndGame();

    void updateEvent(float dt);
    void OnExtensionResponseProcess();

    void DisplayInfoMoney(const long& tienconlai, const long& dadatcuoc);

    void BetGameByClient(const int& tag);
    void AddCoinCallFunc2(Ref *sender, TaiXiuBet* data);
    void AddCoinCallFunc(Ref *sender, Struct4Call1 data);

    void Update5sStep(float dt);
	void processAnimationWin(Vec2 _dest, Vec2 _pos);

    struct BtnCoin{
        Button* btn;
        int tag;
    };
    std::vector<string> ArrListValueBet;
    std::vector<string> ArrListValueBetDefault;
                         
    std::vector<BtnCoin> ArrayListCoinFromStudio;

    void initPlayerInfo();
    void initListCoin();

    void CallBackFromAnim();

};

#endif // LAYERGAMETAIXIU_H
