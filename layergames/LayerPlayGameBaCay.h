//
//  BaCayChuong.h
//  iCasinov2
//
//  Created by DauA on 5/23/14.
//
//

#ifndef __iCasinov2__BaCay__
#define __iCasinov2__BaCay__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameServer.h"
#include "LayerAvatar_BaCay.h"
#include "_CardBaCay_.h"
#include "_LayerBet_.h"
#include "_Number_Chat_inGame7u.h"
#include "_Button_inGame_.h"
#include "LayerCount.h"
#include "ThreadQueueMsg.h"
#include "LayerTimer.h"
#include "LayerGa.h"
#include "LayerDatCuocBaCay.h"
#include "../layers/LayerPopupThongBao.h"
#include "_Card_inGame_.h"
#include "../cotuong/gameUtils.h"

#define TAG_BET_LIENG_LAYER 234
#define TAG_NAN_BACAY_LAYER 123

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace Sfs2X;

#define TAG_BTN_NHAN_BIEN_ALL 92
#define TAG_BTN_BIEN_ALL_X1 93
#define TAG_BTN_BIEN_ALL_X2 94
#define TAG_BTN_DAT_CUA 95

class LayerPlayGameBaCay : public Layer, public PlayerCallBack{
private:
	LayerBaCayAvatar*	LayerAvatars;
	Number_Chat_inGame7u* LayerChat;
	LayerButtonInGame*	LayerButton;
	CardBaCay*			LayerCards;
	BetGame3Cay*		LayerBet;
	LayerTimer*			mLayerCount;
	Label*			nameGame;
	LayerGa* layerGa;

	Size sizeAd;

	string ListCards;
	string ListUsers;
	string MyAI;
	string StringResult;
	string myName;
	int		SizeListUser;
	int		mCurrGameID;

	bool FlagChiaBai;
	bool FlagDatCuoc;
	bool IsTurnUpAll;
	bool IsSpector;

	ThreadQueueMsg*		mThreadQueueMsg;
	boost::shared_ptr<ISFSObject> param;

	//4 Lieng
	string	CurrentPlayerBet;
	string	CurrentBetal;

	int		mBetDefault;
	double money_bet;
	int		MinBetValue;
	int		Current_My_Bet;
	bool	mReal;
	bool isStartedGame;
	// bool isNuoiGa;

	Sprite *bgLoading;
	ProgressTimer *timerBet;
	Label* labelTimerBet;
	int timerBet3Cay;//th

	//giao dien chuc nang danh bien
	Bien3Cay* bien3Cay;
	std::string _lcFindChuPhong;
	std::string _uidChuPhong;
	int statusNhanBienAll;
	Sprite *checkedBg;
	Label *labelBienAllMoneyx2;
	Label *labelBienAllMoney;
	Button *btnNhanBienAll1;
	Button *btnBienTat1;
	Button *btnBienTatx2_1;
	Button *btnDat;
	LayerDatCuocBaCay *layerBetBaCay;
	std::string uidChuong = "";
	double valueBet;
	int vipRoom;

	void hideUIDanhBien();
	void displayUIBet(bool _display);
	void displayButtonDanhBien(bool _display);

	void createLoadingBet();
	void showLoadingBet();
	void hideLoadingBet();
	void updateTimerBet(float dt);
	string queueList = "";
public:
	LayerPlayGameBaCay();
	~LayerPlayGameBaCay();
	virtual bool init();
	CREATE_FUNC(LayerPlayGameBaCay);
	void updateEvent(float dt);

	void CreateBackgrounds();
	void CreateAvatars();
	void CreateButtons();
	void createNumbers();
	void CreateCards();
	void CreateLayerBet();
	void createLayerBien();
	void createLayerChat();
	void CreateTimer();
	void createLayerGa();
	void actionConfigGa();
	void PlaySound(const string& soundPath);
	void event_EXT_EVENT_AMF_TEST_NOTIF();
	//
	void SpecMode();
	double getMoneyBet();
	string FindChuong(string& pListUsers);
	string find_ChuPhong(string listUser);
	vector<string> split(string &S, const char &str);
	//4 Lieng
	void UserRejoinGame(string& listplayers, int isdeal);
	void BetGame(const string& uid, string& betal);

	void MoveButtonToRight();
	void ResetButtonLocation();
	///End 4 lieng
	std::string FindBossRoom(string listUser);

	Button* CreateButtonWithTitle_Pos(string imgPath, const Vec2& pPoint);
	Button* GetButtonByTag(const int& pTag);

	//Các sự kiện Button trong game
	void OnButtonClick(Ref *sender, ui::Widget::TouchEventType type);

	void JoinGame();
	void ActionReady();
	void UnReady();
	void ActionBet();
	void TurnUpAll();
	void ViewCards();
	void SqueezCards();
	void ResetGame();
	void CallFuncTurnUpAll(Node *pSend);
	void CallFuncEndGive(Node *pSend);
	void onScheduleEndBet(float dt);

	void processBtnDatCuaTouched(Ref *sender, ui::Widget::TouchEventType type);
	void processBtnNhanBienAllTouched();
	void processBtnBienTatTouched();
	void processBtnBienTatx2Touched();
	std::string getURL_byID_3CayChuong(int pID);
	void showBienUser();

	//Server
	void OnExtensionResponseProcess();
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSpectatorToPlayerError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSpectatorToPlayer(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxPlayerToSpectator(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	//
	void event_EXT_EVENT_ERROR_READY_NTF();
	void event_EXT_EVENT_UNREADY_NTF();
	void event_EXT_EVENT_START_GAME_NOTIF();
	void event_EXT_EVENT_READY_NTF();
	void event_EXT_EVENT_LIST_USER_UPDATE_3CAYCHUONG();
	void event_EXT_EVENT_LIST_USER_UPDATE_3CAYNORMAL();
	void event_EXT_EVENT_LISTCARD_NTF();
	void event_EXT_EVENT_GAME_RESULT();
	void event_EXT_EVENT_GAME_BET_NTF();
	void event_EXT_EVENT_START();
	void event_EXT_EVENT_END();
	void event_EXT_EVENT_REGISTER_QUEUE_NTF();
	void event_EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY();
	//event danh bien
	void event_EXT_EVENT_DEAL_CARD_BOSS_NOTIF(std::string _lc);
	//Event 4 lieng
	void event_EXT_EVENT_NEXT_TURN();
	void event_EXT_EVENT_GAME_RESULT_4_LIENG();
	void event_EXT_EVENT_LIST_USER_UPDATE_4_LIENG();
	void event_EXT_EVENT_WAIT_BET_TIMER_NTF();
	void event_EXT_EVENT_USER_LEAVE_NOTIF();
	void event_EXT_EVENT_NEW_BOSS_NOTIF();
	void event_EXT_EVENT_ENROLL_ALL_INFO_NOTIF(std::string _lstBien);
	void event_EXT_EVENT_USER_JOIN_NOTIF();
	void event_EXT_EVENT_ENROLL_BET_NTF(std::string _uid, double _bet);
};
#endif 
