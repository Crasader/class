//
//  XiTo.h
//  iCasinov2
//
//  Created by DauA on 5/28/14.
//
//

#ifndef __iCasinov2__XiTo__
#define __iCasinov2__XiTo__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameServer.h"
#include "AllData.h"
#include "LayerAvatarXiTo.h"
#include "_Button_inGame_.h"
#include "_Card_.h"
#include "FrameBet.h"
#include "ThreadQueueMsg.h"
#include "PlayerXiTo.h"
#include "ButtonXiTo.h"


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace Sfs2X;

struct Give{
    int pos;
    std::string img;
    std::string allc;
    Give(const int& pos, const std::string& img, const std::string& allc):pos(pos), img(img), allc(allc){}
};

class XiTo : public Layer, public PlayerCallBack
{
private:
    std::string MyAI;

    PlayerXiTo* PlayerMe;
    PlayerXiTo* PlayerLeftBottom;
    PlayerXiTo* PlayerLeftTop;
    PlayerXiTo* PlayerRightTop;
    PlayerXiTo* PlayerRightBottom;

    cocos2d::Layer*		LayerFrameBets;
    cocos2d::Layer*		layerLabelVictype;

    // Thong tin bet o giua ban.
    cocos2d::Sprite*	spFrameBetTotal;
    cocos2d::Label*     lblBetTotal;

    //
    LayerAvatarXiTo*		LayerAvatars;
    LayerButtonInGame*		LayerButtons;

    //! Đếm số giây nếu người chơi không bấm button sẵn sàng sau khi end game
    //! thì tự động xóa bàn chơi cũ.
    int m_TimeDelayToEndGame;

    //! Biến này tính số lần đã chia deal card (2 lá bài đầu tiên).
    int m_CountGive2CardFirst;

    //! Biến này đếm việc chia thêm cho 1 mỗi người 1 lá bài.
    //! Mỗi khi 1 lượt chia thêm kết thúc thì biến này reset về 0.
    int m_CountGiveCardNext;

    std::map<std::string, int> mapPlayers;
    //! Danh sách người chơi trong phòng
    std::string s_ListPlayers;

    //! chuỗi 2 lá bài đầu tiên của mình.
    std::string s_TwoCardsFirst;

    //! Timer đếm sau 15s;
    unsigned short CountDownToReady;

    //!Duration animation
    float m_Duration;

    //
    float f_RoomBet;
    float f_TotalCurrentRoomBet;
    float f_CurrentTurnBet;

    Label* TimerToReady;

    boost::shared_ptr<ISFSObject> param;
    ThreadQueueMsg*		mThreadQueueMsg;

    bool IsSpect;
	int vipRoom;

    std::vector<Give> posgive;
    std::vector<PlayerXiTo*> listBet;

private:
    vector<ButtonXiTo*> pArrayButton1;
    vector<ButtonXiTo*> pArrayButton2;

    Sprite* bgButton;

    float _pTimerReady;
    float _pTurnTime;
    float _pTimeTurnOne;

    float StepTimeout;

    void GetRoomVar();

    Button* btnReady;
    ButtonXiTo* btnToDisplay;
    Button* btnDis;

    bool isRegistLeave;

public:
    ~XiTo();
    CREATE_FUNC(XiTo)

private:
    XiTo();
    bool	init();
    void updateEvent(float dt);

    void	CreateAvatar();
    void	CreateBackground();
    void	CreateButtons();
    void	CreateFrameBets();

    void	event_EXT_EVENT_USER_JOIN_NOTIF();
    void	event_EXT_EVENT_USER_LEAVE_NOTIF();
    void	event_EXT_EVENT_USER_READY_NOTIF();
    void	event_EXT_EVENT_SUB_BALANCE_FIRST_NOTIF();
    void	event_EXT_EVENT_START_GAME_NOTIF();
    void	event_EXT_EVENT_DEAL_CARD_NOTIF();
    void	event_EXT_EVENT_USER_KICKED_NOTIF();
    void	event_EXT_EVENT_START_READY_TIMER_NOTIF();
    void	event_EXT_EVENT_USER_EXPIRE_MONEY_NOTIF();
    void	event_EXT_EVENT_END_GAME_NOTIF();
    void	event_EXT_EVENT_GAME_CHANGE_NOTIF();
    void	event_EXT_EVENT_CHANGE_BALANCE_NOTIF();
    void	event_EXT_EVENT_VICTORY_NOTIF();
    void	event_EXT_EVENT_OPEN_ALL_CARD_NOTIF();
    void	event_EXT_EVENT_USER_HAVE_SET_NOTIF();
    void	event_EXT_EVENT_RAISE_NOTIF();
    void	event_EXT_EVENT_OPEN_CARD_NOTIF();
    void	event_EXT_EVENT_READY_RES();
    void	event_EXT_EVENT_NEAD_RAISE_NOTIF();
    void    event_EXT_EVENT_LIST_USER_UPDATE();
    void    event_EXT_EVENT_REGISTER_QUEUE_NTF();
    void    event_EXT_EVENT_LEAVE_GAME_RES();

    void	SetMapPlayer(string& listplayers);
    int		GetPosPlayer(const std::string& uid);

    void	EventListPlayers(string& listUser);
    void	SetInfoAllPlayers(boost::shared_ptr<XiToInfoPlayer> info);

    void	EventReJoinGame(string& roomInfo);

    //Chia 2 lá bài đầu tiên
    void	Chia2LaBaiDauTien_1();
    void    Chia2LaBaiDauTien_2(PlayerXiTo* player, const int& totalchia);

    //Hiện Popup để chọn lá bài tẩy
    void	SendOpenCard();

    //Chia thêm cho mỗi người chơi 1 lá bài
    void	ChiaThem1LaBai_1();
    void    ChiaThem1LaBai_2(PlayerXiTo* player, const string& allc, const string& url, const int& totalchia);

    void	HideAllFrameBet();

    //
    void	SetButtonBet(const string& uid, string& lsBet);
    void	SendBetNTF(const int& typeBet);
    void	ActionEndGame();
    void	DelayToEndGame(float dt);

    void	VisibleButtonPlay();

    // Button Click
    void	OnBtnReadyClick(Ref *sender, ui::Widget::TouchEventType  pType);
    void    OnButtonBetClick(Ref *sender, ui::Widget::TouchEventType pType);
    void    OnButtonDisplay(Ref *sender, ui::Widget::TouchEventType pType);

    // Tạo các Button
    ButtonXiTo* createButtonType1(const int& tag, const string& name);
    int		GetTagButton(const int& iTag);

    void    HideButtonReady();

    //
    void	TimerToReadyFunc(float dt);
    void	ResetTimeoutReady();

    //Server Respond
    void	OnExtensionResponseProcess();
    void	OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void	OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void	OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void	OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void	OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void    OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    PlayerXiTo* GetPlayer(const int& pos);
    double  CalcMoneyBet(const int& type);

    void    SetTotalMoney(const double& money);
};

#endif /* defined(__iCasinov2__XiTo__) */
