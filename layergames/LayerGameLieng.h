//
//  BaCayChuong.h
//  iCasinov2
//
//  Created by DauA on 5/23/14.
//
//

#ifndef LayerGameLieng_h__
#define LayerGameLieng_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameServer.h"
#include "LayerAvatar_BaCay.h"
#include "_CardBaCay_.h"
#include "_Button_inGame_.h"
#include "ThreadQueueMsg.h"
#include "LayerTimer.h"
#include "_Background_inGame_.h"
#include "LayerDatCuocBaCay.h"

#define TAG_BET_LIENG_LAYER 234
#define TAG_NAN_BACAY_LAYER 123

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace Sfs2X;

class LayerGameLieng : public Layer, public PlayerCallBack{
private:
	LayerBaCayAvatar*	LayerAvatars;
	LayerButtonInGame*	LayerButton;
	LayerCardLieng*			LayerCards;
	LayerTimer*			layerTimer;
	BackgroundInGame* layerBackGround;

	Size sizeAd;

	string ListCards;
	string ListUsers;
	string MyName;
	string StringResult;
	int		SizeListUser;
	int		mCurrGameID;

	bool FlagChiaBai;
	bool FlagDatCuoc;
	bool IsTurnUpAll;
	bool IsSpector;
	int vipRoom;

	ThreadQueueMsg*		mThreadQueueMsg;
	//4 Lieng
	string	CurrentPlayerBet;
	string	CurrentBetal;

	int		mBetDefault;
	LayerDatCuocBaCay* betLayerSlider;

	int		MinBetValue;
	int		Current_My_Bet;
	bool	mReal;
	bool isStartedGame;
	boost::shared_ptr<ISFSObject> param;

	void hideAllButton();
	void disableForNextTurn();
	void setEnabledButton(int tag, bool enable);
	void callBackLayerDatCuoc();
	bool IsPlaying();
public:
	LayerGameLieng();
	~LayerGameLieng();
	virtual bool init();
	CREATE_FUNC(LayerGameLieng);
	void updateEvent(float dt);

	void CreateBackgrounds();
	void CreateTimer();
	void CreateCards();
	void CreateAvatars();
	void CreateButtons4Lieng();

	void PlaySound(const string& soundPath);
	void SpecMode();
	string FindChuong(string& pListUsers);
	string find_ChuPhong(string listUser);
	vector<string> split(string &S, const char &str);
	//4 Lieng
	void nextTurnHandle(const string& uid, string& betal);
	std::string FindBossRoom(string listUser);

	Button* CreateButtonWithTitle_Pos(const char *pName, const Vec2& pPoint);
	Button* GetButtonByTag(const int& pTag);

	//Các sự kiện Button trong game
	void OnButtonClick(Ref *sender, ui::Widget::TouchEventType type);

	void JoinGame();
	void ActionReady();
	void UnReady();
	void onButtonBet();
	void TurnUpAll();
	void ViewCards();
	void SqueezCards();
	void resetGame();
	void FoldCards();
	void Follow();
	void Complete();
	///End 4 lieng
	void processButtonToTouched(Ref *sender, ui::Widget::TouchEventType type);
	void CallFuncTurnUpAll(Node *pSend);
	void CallFuncEndGive(Node *pSend);
	void onScheduleEndBet(float dt);

	//Server
	void OnExtensionResponseProcess();
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	//
	void event_EXT_EVENT_LIST_USER_UPDATE_4_LIENG(boost::shared_ptr<ISFSObject> param);
	void event_EXT_KICK_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_READY_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_REGISTER_QUEUE_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_ERROR_READY_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_START_READY_TIMER_NOTIF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_START(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_LISTCARD_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_GAME_BET_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_NEXT_TURN(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_GAME_RESULT_4_LIENG(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_CHANGE_BALANCE_NOTIF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_END(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_EXPIRE_CHIP_NTF(boost::shared_ptr<ISFSObject> param);
	void event_event_EXT_EVENT_SUB_BALANCE_FIRST_NOTIF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_GAME_CHANGE_NOTIF(boost::shared_ptr<ISFSObject> param);
};
#endif // LayerGameLieng_h__
