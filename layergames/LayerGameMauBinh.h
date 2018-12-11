//
//  LayerGameMauBinh.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/19/17.
//
//

#ifndef LayerGameMauBinh_hpp
#define LayerGameMauBinh_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

#include "AllData.h"
#include "GameServer.h"
//
#include "_Background_inGame_.h"
#include "_Button_inGame_.h"
#include "AvatarInGame52La.h"
#include "_Number_inGame_.h"
#include "LayerBobaiMauBinh.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/SpectatorToPlayerRequest.h"
#include "Requests/PlayerToSpectatorRequest.h"
#include "LayerGa.h"
#include "ThreadQueueMsg.h"
#include "NotificationCallBack.h"
#include "LayerTimer.h"
using namespace cocos2d;
using namespace cocos2d::ui;

class LayerGameMauBinh : public Layer, public PlayerCallBack, public NotificationCallBack
{
private:
    boost::shared_ptr<ISFSObject> param;
    
    string myName;
    string currentPlaying;
    string myListUser;
    string queueList;//danh sach hang doi
    bool isNewTurn;
    bool isSpector;
    bool isStartedGame;
    bool isRegistSittingDown;
    bool isRegistStandUp;
    int gameID;
    //bool isNuoiGa;
    Sprite *bgLoading;
    //ProgressTimer *timerSam;
    Label* labelTimerXepBai;
    int timeXepbai;//
    bool isBaoSam;
    
    string leaveUser;//bien lưu thằng out game, để khi cập nhật listuser có thằng này thì không cập nhật
    
    vector<string> arrName;
    vector<int> arrMoney;
    vector<double> arrMoneyDouble;
    void event_EXT_EVENT_USER_JOIN_NOTIF();
    void event_EXT_EVENT_START_READY_TIMER_NOTIF();
    void event_EXT_EVENT_START_GAME_NOTIF();
    void event_EXT_EVENT_DEAL_CARD_NOTIF();
    void event_EXT_EVENT_FINISH_CARD_LINK_NOTIFY();
    void event_EXT_EVENT_END_GAME_NOTIF();
    void event_EXT_EVENT_GAME_RESULT();
    void event_EXT_EVENT_LIST_USER_UPDATE();
    void event_EXT_EVENT_REGISTER_QUEUE_NTF();
    void event_EXT_EVENT_CHANGE_BALANCE_NOTIFY();
    void event_EXT_EVENT_VICTORY_NOTIF();
    void event_EXT_EVENT_USER_LEAVE_NOTIF();
    void event_EXT_EVENT_GAME_CHANGE_NOTIF();
    
    LayerAvatarInGame52La   *layerAvatars;
    LayerBoBaiMauBinh     *layerCards;
    LayerButtonInGame   *layerButtons;
    LayerNumberInGame   *layerNumbers;
    
    BackgroundInGame	*layerBackGround;
    LayerTimer			*layerTimer;
    LayerGa				*layerGa;
    ThreadQueueMsg*		mThreadQueueMsg;
    Size sizeAd;
    void hideAllButton();
    void showListUserChicken();
public:
    LayerGameMauBinh();
    ~LayerGameMauBinh();
    CREATE_FUNC(LayerGameMauBinh);
    void updateEvent(float dt);
    void updateTimeXepBai(float dt);
    //void resetCard(float dt);
    
    virtual bool init();
    void createBackgrounds();
    void createAvatars();
    void createButtons();
    void createNumbers();
    void createCards();
    void createChats();
    void createTimer();
    void createLayerGa();
    
    void initGame();
    void resetGame();
    void playSound(int type);
    
    vector<string> split(string &S, const char &str);
    // 	int    convertResponseToInt(string inString);
    // 	string convertResponseToString(int inInt);
    Button* createButtonWithTitle_Position(string imgPath, Point pPoint, string title);
    Button* getButtonByTag(int tag);
    bool isSpectator();
    string find_ChuPhong(string listUser);
    void isWinner();
    
    // actions
    void actionReady(Ref *pSender, Widget::TouchEventType pType);
    void actionUnReady(Ref *pSender, Widget::TouchEventType pType);
    void actionSortCards(Ref *pSender, Widget::TouchEventType pType);
    void actionHitCards(Ref *pSender, Widget::TouchEventType pType);
    void actionNextTurn(Ref *pSender, Widget::TouchEventType pType);
    void actionSitting(Ref *pSender, Widget::TouchEventType pType);
    void actionStandUp(Ref *pSender, Widget::TouchEventType pType);
    void actionConfigGa();
    void actionLeaveGame(Ref* pSender);
        // event server
    void OnExtensionResponseProcess();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    bool isNguoiChauRia(string _list);
    void reloadCuoc(int linkChi1,int linkChi2,int linkChi3);
    void hideCuoc();
private:
    //recommend card
    void cardInGameCallBack(Ref *pSender);
    void showArrowInButton(Button* btn, bool isShow);
    int  timerReady = 0;
    int HOA_IMG_TAG = 12193;
    bool isHoa = false;
    int vipRoom = 0;
    int time_so_chi = 4;
    int numDone;
};


#endif /* LayerGameMauBinh_hpp */
