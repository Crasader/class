//
//  LayerPlayGamePhom.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//

#ifndef __iCasino_v2__LayerPlayGamePhom__
#define __iCasino_v2__LayerPlayGamePhom__

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
#include "LayerTimer.h"
#include "LayerGa.h"

#include "Requests/ExtensionRequest.h"
#include "Requests/SpectatorToPlayerRequest.h"
#include "Requests/PlayerToSpectatorRequest.h"
#include "ThreadQueueMsg.h"
#include "NotificationCallBack.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace boost;

class LayerPlayGamePhom : public Layer, public PlayerCallBack, public NotificationCallBack
{
private:

	enum ACTION_LAST {
		DANH_BAI = 1,
		BOC_BAI,
		AN_BAI,
		XEP_BAI,
		HA_PHOM,
		SAN_SANG,
		START_GAME,
		END_GAME
	};
	enum TYPE_BUTTON{
		AFTER_GIVE = 0, //sau khi boc
		BEFORE_GIVE,//truoc khi boc
		FIRST_HAND_OFF,//ha dau tien khong co gui
		LAST_HAND_OFF,// ha cuoi cung cung nhu ha bt
	};

	int actionLast;
	string myName;
	string currentPlaying;
	string listID_HaPhom;
	string dstphsString; // danh sach cac bo phom co the gui
	int cardid_push;
	bool isSpector;
	bool isStartedGame;
	bool isRegistSittingDown;
	bool isRegistStandUp;
	bool myReady;
	bool isEndGame;
	int readyTimeOut, discardTimeout, turnTimeout, endGameTimeout;
	bool isNuoiGa;
	bool isResume;
	int vipRoom;

	// mảng lưu các tên và thông tin tương ứng để hiển thị
	vector<string> arrName;
	vector<int> arrMoney;
	vector<double> arrMoneyDouble;
	vector<Button*> arrButtonCanPush; // luu tru va remove Button khi khong can thiet
	boost::shared_ptr<string> lstUser;

	boost::shared_ptr<ISFSObject> param;
	LayerAvatarInGame52La   *layerAvatars;
	LayerCardInGame     *layerCards;
	LayerButtonInGame   *layerButtons;
	LayerNumberInGame   *layerNumbers;
	Sprite *arrowNoc;

	//	LayerChatInGame     *layerChats;

	LayerTimer *layerTimer;
	LayerGa *layerGa;
	bool isFirstReady;
	void setFuncallBack(Ref *pSender);
	ThreadQueueMsg*		mThreadQueueMsg;
	void initParam();
	void callBackFromTimer();
	Size sizeAdd;
	bool isSpectator();
	int lastHitCard;
	bool isU9;// bien kiem tra xem co u 9 cay hay ko
	bool isHaPhom;//bien kiem tra viec animation ha phom duoi client
	string listCardHandOff;//list card co the ha sever gui
	Label *labelNoc;
	int countRequestJoin;
	///ham show cac button 
	void showButtonOtherTurn(); //luot choi cua nguoc khac
	void showButtonMyTurn(int type);//luot choi cua minh
	void showListUserChicken();
	void showBocNoc(bool isBoc);
	void showButtonReady(float dt);///hien button sang sang cho chu phong sau n giay - nhan config tu sever
	Size winSize;
public:
	 LayerPlayGamePhom();
	~LayerPlayGamePhom();
	bool init();
	CREATE_FUNC(LayerPlayGamePhom);
	void updateEvent(float dt);


	string find_ChuPhong(string listUser);

	void createBackgrounds();
	void createAvatars();
	void createButtons();
	void createNumbers();
	void createCards();
	void createChats();
	void createLayerGa();

	void initGame();
	void resetGame();
	void playSound(int type);
	void createLayerTimer();

// 	int    convertResponseToInt(string inString);
// 	string convertResponseToString(int inInt);
	void createButton_PushMulti_By_CardId(int cardid);
	void createButtonWith_Tag_Title_Position(int tag, string imgPath, Point pPoint, string title);
	void hideAllButton();
	Button* getButtonByTag(int tag);

	// actions
	void actionReady(Ref *pSender, Widget::TouchEventType pType);
	void actionSortCards(Ref *pSender, Widget::TouchEventType pType);
	void actionHitCards(Ref *pSender, Widget::TouchEventType pType);
	void actionGiveCards(Ref *pSender, Widget::TouchEventType pType);
	void actionEatCards(Ref *pSender, Widget::TouchEventType pType);
	void actionHaPhom(Ref *pSender, Widget::TouchEventType pType);
	void callbackHaPhom(float dt);
	void callbackHaPhom_stepByStep(float dt);
	void actionPush(Ref *pSender, Widget::TouchEventType pType);
	void actionUUU(Ref *pSender, Widget::TouchEventType pType);
	void actionPushMulti(Ref *pSender, Widget::TouchEventType pType);
	void actionSitting(Ref *pSender, Widget::TouchEventType pType);
	void actionStandUp(Ref *pSender, Widget::TouchEventType pType);
	void actionUnReady(Ref *pSender, Widget::TouchEventType pType);
	void actionConfigGa();
	void actionLeaveGame(Ref* pSender);// thuc hien khi dang choi ma out ra

	// event server
	void OnExtensionResponseProcess();
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	void sendRequestJoinGameInterval(float dt);
	void sendRequestJoinGame();
	void sendRequestLeaveGame();
	vector<string> split(string &S, const char &str);
	void resetCard(float dt);

	void event_EXT_SRVNTF_PUBLIC_MESSAGE();
	void event_EXT_SRVNTF_PRIVATE_MESSAGE();
	void event_EXT_SRVNTF_GAME_MASTER_INFO();
	void event_EXT_SRVNTF_READY();
	void event_EXT_EVENT_READY_RES();
	void event_EXT_SRVNTF_PLAYER_LIST();//ban tin cap nhat danh sach nguoi choi
	void event_EXT_SRVNTF_RECOMMENDED_CARDSETS();//ban tin lay danh sach cac quan co the ha phom
	void event_EXT_EVENT_START();// ban tin bat dau game
	void event_EXT_EVENT_END();//ban tin ket thuc game
	void event_EXT_EVENT_GAME_RESULT();//ban tin ket qua
	void event_EXT_EVENT_LISTCARD_NTF();//ban tin danh sach quan bai bao gom chia bai, ha phom, xep bai
	void event_EXT_EVENT_COIN_NTF();//ban tin tinh tien
	void event_EXT_SRVNTF_CURRENT_PLAYING_USER();//ban tin cho biet dang den luot nguoi nao
	void event_EXT_SRVNTF_END_PLAYING_GAME();//ban tin cho biet chuan bi ket thuc game
	void event_EXT_SRVNTF_GAME_INFO();//cac thong tin ngoai ve game nhu ga
	void event_EXT_SRVNTF_USER_READY();//ban tin san sang
	void event_EXT_SRVNTF_USER_UNREADY();
	void event_EXT_SRVNTF_CARD_ORIGINATION();//ban tin gui cho toan bo nguoi trong ban nhu danh bai, an bai, chuyen bai
	void event_EXT_EVENT_RES_DRAW_CARD();//ban tin tra ve ket qua boc bai - thanh cong hay khong
	void event_EXT_EVENT_RES_TAKE_CARD();//ban tin tra ve ket qua an bai
	void event_EXT_EVENT_RES_DISCARD();//ban tin tra ve ket qua danh bai
	void event_EXT_EVENT_RES_HANDOFF_PHOM();//ban tin tra ve ket qua ha phom
	void event_EXT_EVENT_RES_END_PLAYING();
	void event_EXT_EVENT_RES_SET_BET();
	void event_EXT_EVENT_RES_U();
	void event_EXT_EVENT_RES_PUSH_CARD();
	void event_EXT_SRVNTF_CAN_TAKE();
	void event_EXT_SRVNTF_PLAYER_REPLAY();
	void event_EXT_SRVNTF_U_9();
	void event_EXT_SRVNTF_U();
	void event_EXT_SRVNTF_PLAYER_KICKOUT();
	void event_EXT_EVENT_RES_LEAVE_GAME();
	void event_EXT_EVENT_CHICKEN_UPDATE_NTF();
	void event_EXT_EVENT_NTF_TIMER();
	void event_EXT_EVENT_RES_UPDATE_GAME_CONFIG();
	void event_EXT_SRVNTF_NOC_COUNT();
	void event_EXT_EVENT_REGISTER_QUEUE_NTF();
	bool isNguoiChauRia(string _list);
	void event_EXT_SRVNTF_RECOMMENDED_PUSH_CARDS();
	void event_EXT_EVENT_RES_JOIN_GAME();
};

#endif /* defined(__iCasino_v2__LayerPlayGamePhom__) */
