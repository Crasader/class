//
//  Sam.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//

#ifndef __iCasino_v2__Sam__
#define __iCasino_v2__Sam__

#include <iostream>
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
#include "_Card_inGame_.h"
#include "_Chat_inGame_.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/SpectatorToPlayerRequest.h"
#include "Requests/PlayerToSpectatorRequest.h"
#include "LayerGa.h"
#include "ThreadQueueMsg.h"
#include "LogicTienLenMN.h"
#include "NotificationCallBack.h"
#include "LayerTimer.h"
using namespace cocos2d;
using namespace cocos2d::ui;

class Sam : public Layer, public PlayerCallBack, public NotificationCallBack
{
private:
	//Minh DV
	LogicTienLenMN *logicTienLenMN;//class kiem tra locgic sam o client
	int demotest;
	DoTogether *doTogether;
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
	bool flagRollBack;//co nay de kiem tra trang thai roll back co 2 loai la tu dong danh = true va bo luot = false
	string previousCards;//luu lai chuoi cardid nguoi danh cuoi
	string myListCards;//luu lai chuoi cardid minh danh ra
	Sprite *bgLoading;
	ProgressTimer *timerSam;
	Label* labelTimerSam;
	int timeBaoSam;//thoi gian bao sam
	bool isBaoSam;
	int vipRoom;

	string leaveUser;//bien lưu thằng out game, để khi cập nhật listuser có thằng này thì không cập nhật

	vector<string> arrName;
	vector<int> arrMoney;
	vector<double> arrMoneyDouble;
	vector<int> arrIDTurn;

	LayerAvatarInGame52La   *layerAvatars;
	LayerCardInGame     *layerCards;
	LayerButtonInGame   *layerButtons;
	LayerNumberInGame   *layerNumbers;

	BackgroundInGame	*layerBackGround;
	LayerTimer			*layerTimer;
	LayerGa				*layerGa;
	ThreadQueueMsg*		mThreadQueueMsg;
	Size sizeAd;
	void showButtonMyTurn(bool isAllow);
	void showButtonOtherTurn();
	void hideAllButton();
	void showListUserChicken();
	void createLoadingBaoSam();
	void showLoadingSam();
	void hideLoadingSam();
	void updateTimerSam(float dt);
public:
	Sam();
	~Sam();
	CREATE_FUNC(Sam);
	void updateEvent(float dt);
	void resetCard(float dt);

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
	void actionBaoSam(Ref *pSender, Widget::TouchEventType pType);
	void actionHuySam(Ref *pSender, Widget::TouchEventType pType);
	void actionConfigGa();
	void actionLeaveGame(Ref* pSender);

	void showBaoSam();
	void hideBaoSam();
	// event server
	void OnExtensionResponseProcess();
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	void event_EXT_EVENT_USER_JOIN_NOTIF();      // jrntf
	void event_EXT_EVENT_LISTCARD_NTF();         //"lcntf";
	void event_EXT_EVENT_ERROR_READY_NTF();      // = "e_rntf";
	void event_EXT_EVENT_USER_READY_NOTIF();     // = "rdyntf";
	void event_EXT_EVENT_UNREADY_NTF();          // = "urntf";
	void event_EXT_EVENT_READY_RES();            // = "rdres";
	void event_EXT_EVENT_LIST_USER_UPDATE();     // = "luu";
	void event_EXT_EVENT_START_GAME_NOTIF();     // = "strntf";
	void event_EXT_EVENT_DEAL_CARD_NOTIF();      // = "dlcntf";
	void event_EXT_EVENT_USER_LEAVE_NOTIF();     // = "lrntf";
	void event_EXT_EVENT_END_GAME_NOTIF();       // = "endntf";
	void event_EXT_EVENT_CHANGE_BALANCE_NOTIFY();       // = "cblltf";
	void event_EXT_EVENT_VICTORY_NOTIF();        // = "vicntf";
	void event_EXT_EVENT_NEAD_PLAY_NOTIF();      // = "ndpntf";
	void event_EXT_EVENT_PLAY_CARD_NOTIF();      // = "plcntf";
	void event_EXT_EVENT_GAME_CHANGE_NOTIF();    // = "gchntf";
	void event_EXT_EVENT_PASS_CARD_NOTIF();      // = "pscntf";
	void event_EXT_EVENT_PLAY_CARD_RES();        // = "plcrs";
	void event_EXT_EVENT_HIT_CARD_ERROR();   //pscrs
	void event_EXT_EVENT_SAM_NTF();
	void event_EXT_EVENT_UNSAM_NOTIF(); // huy sam usmntf
	void event_EXT_EVENT_UNSAM_RES();
	void event_EXT_EVENT_SAM_RES();
	void event_EXT_EVENT_BAO_NTF();
	void event_EXT_EVENT_CHICKEN_UPDATE_NTF();		//chkntf
	void event_EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY();
	void event_EXT_EVENT_GAME_CHIKKEN_CONFIG_RES();
	void event_EXT_EVENT_START_READY_TIMER_NOTIF();
	void event_EXT_EVENT_GAME_LEAVE_NOTI();//roi game khi dang choi
	void event_EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF();//het tien
	void event_EXT_EVENT_REGISTER_QUEUE_NTF();
	void event_EXT_EVENT_AUTO_KICK();
	bool isNguoiChauRia(string _list);
private:
	//recommend card
	void cardInGameCallBack(Ref *pSender);
	void showArrowInButton(Button* btn, bool isShow);
};

#endif /* defined(__iCasino_v2__Sam__) */
