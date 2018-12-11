//
//  LayerChanGame.h
//  iCasinov2
//
//  Created by DauA on 6/4/14.
//
//

#ifndef __iCasinov2__LayerChanGame__
#define __iCasinov2__LayerChanGame__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameServer.h"
#include "FrameBet.h"
#include "_Avatar_inGame_.h"
#include "_Button_inGame_.h"
#include "_Layer_CardChan_.h"
#include "LayerChanToast.h"
#include "ThreadQueueMsg.h"
#include "ChanUtils.h"
#include "ButtonChan.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace Sfs2X;

struct CauHinhChanRoom {
    int timeout;
    bool isNuoiGa;
    bool choiUXuong;
    bool uDuoi4Diem;
    bool commentChiu;
    bool commentU;

    CauHinhChanRoom(){
        timeout = 30;
        isNuoiGa = true;
        choiUXuong = false;
        uDuoi4Diem = false;
        commentChiu = false;
        commentU = false;
    }

    void reset(){
        timeout = 30;
        isNuoiGa = true;
        choiUXuong = false;
        uDuoi4Diem = false;
        commentChiu = false;
        commentU = false;
    }
};

class LayerChanGame :
	public Layer,
	public PlayerCallBack
{
private:
    boost::shared_ptr<ISFSObject> param;

    string  MyAI;
    string	ListPlayers;
	string	CurrentPlayer;

	bool	FlagSitDown;
	bool	HasGiveCard;
	bool    HasNotifyU;
    bool	flagChiu;
	bool	FlagAnBao;
	bool	IsGamePlaying;
	string	StrResonAnBao;

	//flag
	bool IsNuoiGa;
	bool IsUXuong;

	//Guess
	bool IsSpect;

	float Step_Timeout;

    Size SizeAdd;

	// Biến này cho biết đã bốc cái hay chưa.
    int CountBocCai;
	// Biến này đếm xem có bao nhiêu người chơi. Không tính khách vào xem.
	int CountPlayers;
	// Biến này cho biết số lá bài còn lại trong nọc.
    int CountNocs;

	LayerAvatarInGame	*LayerAvatars;
	LayerButtonInGame	*layerButtons;
	_Layer_CardChan_	*LayerCardsChan;
	LayerChanToast		*Toast;

	Label*		 LblDetail;
    Label*       LblRoomName;
    Label*       LblCuocBan;
    Label*       LblSuggestU;

	ProgressTimer* TimerMe;
	ImageView*	 _ga;

	// Hiển thị số điểm gà.
	Label*	 LblCoinGa;
    ThreadQueueMsg*		mThreadQueueMsg;

    //
    ButtonChan* BtnDrawCard;
    ButtonChan* BtnTakeCard;
    ButtonChan* BtnBaoU;
    ButtonChan* BtnChiuCard;
    ButtonChan* BtnDisCard;
    ButtonChan* BtnDuoiCard;

    Button* BtnReady;
    Button* BtnSitDown;

    PlayingState State;

    ImageView* _bg;

    CauHinhChanRoom CauHinh;

    bool showAvatarMe;

    bool isChiuPending;
    int _countPendingChiu;
	int vipRoom;

    bool isRegistLeave;
public:
	LayerChanGame();
	~LayerChanGame();
    CREATE_FUNC(LayerChanGame)

protected:
    bool init();
    void updateEvent(float dt);
	std::map<std::string, int> mapPlayers;

	void CreateAvatars();
	void CreateCards();
	void CreateButtons();
	void SendRequestJoinGame(float dt);
	void SendRequestJoinGame();

	void event_EXT_EVENT_END();
	void event_EXT_EVENT_START();
	void event_EXT_EVENT_READY_NTF();
	void event_EXT_SRVNTF_BOC_CAI();
	void event_EXT_SRVNTF_GA_NUOI_SCORE();
	void event_EXT_SRVNTF_COIN_CHANGE();
	void event_EXT_SRVNTF_GAME_INFO();
	void event_EXT_EVENT_RES_DISCARD();
	void event_EXT_SRVNTF_ONE_EXPECTING_U();
	void event_EXT_EVENT_RES_U();
	void event_EXT_SRVNTF_TRENTAY_DETAIL();
	void event_EXT_SRVNTF_NOC_DETAIL();
	void event_EXT_EVENT_GAME_RESULT();
	void event_EXT_EVENT_RES_DUOI_CARD();
	void event_EXT_EVENT_RES_TAKE_CARD();
	void event_EXT_EVENT_RES_DRAW_CARD();
	void event_EXT_SRVNTF_CURRENT_PLAYING_USER();
	void event_EXT_SRVNTF_ONHAND_DETAILS();
	void event_EXT_SRVNTF_NOC_COUNT();
	void event_EXT_EVENT_RES_BOC_CAI();
	void event_EXT_EVENT_LISTCARD_NTF();
	void event_EXT_SRVNTF_USER_READY();
	void event_EXT_SRVNTF_PLAYER_LIST();
	void event_EXT_SRVNTF_ANBAO();
	void event_EXT_SRVNTF_ONE_EXPECTING_CHIU();
	void event_EXT_SRVNTF_BEGIN_ANNOUCING();
	void event_EXT_SRVNTF_PLAYER_KICKOUT();
	void event_EXT_SRVNTF_U_DE();
	void event_EXT_SRVNTF_U_SUCCESS();
	void event_EXT_EVENT_RES_CHIU_CARD();
	void event_EXT_EVENT_RES_JOIN_GAME();
	void event_EXT_EVENT_READY_RES();
	void event_EXT_EVENT_RES_UPDATE_GAME_CONFIG();
    void event_EXT_EVENT_REGISTER_QUEUE_NTF();
    void event_EXT_EVENT_LEAVE_GAME_RES();
    void event_EXT_EVENT_GAME_LEAVE_RES();

	void DelayListCardFirst(Ref *sender, std::string& data);
	void SetEndGame();

    void EventListPlayers(string& list);

    void SetInfoAvatar(const PlayerChanInfo& info);
	void SetCurrentPlayer(float dt);
	void DisplayAnBaoError(const long& rscode, string& uid, string &note, const bool& enter = true);

	void ActionXuongU();
	void Delay10s(Ref *sender, std::string& data);

	//Kết quả game
	void DisplayWinnerResultCuoc(Ref *sender, std::string& data);
	void DisplayLayerKetQua(string& resuilt);
	void DisPlayOnlyBtnU();

	//Đánh bài
	void EventTakeCards(const string& f_user, const string& t_user, const int& cardnu, const int& cardsu, const int& crdorg, const int& count);
	void EventTakeCards_Showing(CardOrg& card);
	void EventTakeCards_Playing(CardOrg& card);

	void callbackTest(Ref* sender, std::string name);
	void call2Test(Ref *pSender, ui::Widget::TouchEventType pType);

	void OnBtnReadyClick(Ref *sender,  ui::Widget::TouchEventType  pType);
	void OnBtnTakeClick(Ref *sender,  ui::Widget::TouchEventType  type);
	void OnBtnBocClick(Ref *sender,  ui::Widget::TouchEventType  type);
	void OnBtnEateClick(Ref *sender,  ui::Widget::TouchEventType  type);
	void OnBtnDuoiClick(Ref *sender,  ui::Widget::TouchEventType  type);
	void OnBtnUClick(Ref *sender,  ui::Widget::TouchEventType  type);
	void OnBtnChiuClick(Ref *sender,  ui::Widget::TouchEventType  type);
	void OnBtnSetting_Chan(Ref *pObject,  ui::Widget::TouchEventType  pType);
	void OnBtnSitDownClick(Ref* pObject,  ui::Widget::TouchEventType  pType);

	void OnExtensionResponseProcess();
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomDestroy(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	void StartTimerMe(const float& duration = 30);
	void StopTimer_Me();

	Button* CreateButtonWithTitle_Pos(const char *pName, const Vec2& pPoint);

    string  NguonGocLaBai(const int& orig);
	string	FindBossRoom();
	string	GetValueSubReson(const long& sres);

	int GetPosPlayer(const string& uid);
    PlayerChanInfo GetPlayerInfo(std::string args);

	void CallBackFunc_ListCards(Ref *pSend);
	void CallBackFunc_BocBaiIfGuess(Ref* pSend);
	void CallBackFunc_LatBai(Ref *pSend);
	void CallBackFunc_ResetGame(Ref *pSend);

    //
    void CallBackAfterDisCard(Ref *pSend);

	void VisibleButtonReady(const bool& isDisplay = true);
	void HideAllButton();
    void PlaySounds(const string& url);


private:
    CauHinhChanRoom SetRoomVar(boost::shared_ptr<Room> lastRoom);
    void DisPlayAvatarMe(bool show);

    void SetCauHinhRoom(boost::shared_ptr<Room> lastRoom);

    bool checkConfigChanged(CauHinhChanRoom config);
    void OnBtnHuongDanChan(Ref* pObject,  ui::Widget::TouchEventType  pType);

    void CheckAndRemovePlayBefore();

    string ConvertMoneyEx(int money);

    void startChiuPending();
    void executeChiuPending(float dt);
    void stopChiuPending();
};

#endif /* defined(__iCasinov2__LayerChanGame__) */
