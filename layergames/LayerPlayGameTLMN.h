//
//  LayerPlayGameTLMN.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//

#ifndef __iCasino_v2__LayerPlayGameTLMN__
#define __iCasino_v2__LayerPlayGameTLMN__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

#include "AllData.h"
#include "GameServer.h"
//
#include "_Background_inGame_.h"
#include "_Button_inGame_.h"
#include "_Avatar_inGame_.h"
#include "_Number_inGame_.h"
#include "_Card_inGame_.h"
#include "_Chat_inGame_.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/SpectatorToPlayerRequest.h"
#include "Requests/PlayerToSpectatorRequest.h"
#include "LayerCount.h"
#include "LayerGa.h"
#include "ThreadQueueMsg.h"
#include "LogicTienLenMN.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerPlayGameTLMN : public Layer, public PlayerCallBack
{
private:
	///MinhDV
	LogicTienLenMN *logicTienLenMN;/// class de kiem tra logic khi danh ra

	DoTogether *doTogether;
    boost::shared_ptr<ISFSObject> param;
    
    string myName;
    string currentPlaying;
	//string listUser;
	string myListUser;
	bool isNewTurn;
	bool isSpector;
	bool isStartedGame;
	bool isRegistSittingDown;
	bool isRegistStandUp;
	bool isNuoiGa;
	int gameID;
	string previousCards;//luu lai chuoi carid nguoi danh cuoi
    
    vector<string> arrName;
    vector<int> arrMoney;
	vector<double> arrMoneyDouble;
    vector<int> arrIDTurn;
    
	LayerGa				*layerGa;
    LayerAvatarInGame   *layerAvatars;
    LayerCardInGame     *layerCards;
    LayerButtonInGame   *layerButtons;
    LayerNumberInGame   *layerNumbers;

//    LayerChatInGame     *layerChats;

	BackgroundInGame	*layerBackGround;
	LayerCount			*layerTimer;
	Label *nameGame;
	ThreadQueueMsg*		mThreadQueueMsg;
	Size sizeAd;
	void showButtonMyTurn(bool isAllow);
	void showButtonOtherTurn();
public:
    LayerPlayGameTLMN();
    ~LayerPlayGameTLMN();
	CREATE_FUNC(LayerPlayGameTLMN);
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
	void playSound(string soundPath);
    
	vector<string> split(string &S, const char &str);
    int    convertResponseToInt(string inString);
    string convertResponseToString(int inInt);
    Button* createButtonWithTitle_Position(const char *title, Point pPoint);
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
	
	// event server
	void OnExtensionResponseProcess();
    virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    virtual void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

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
    void event_EXT_EVENT_AMF_TEST_NOTIF();       // = "cblltf";
    void event_EXT_EVENT_VICTORY_NOTIF();        // = "vicntf";
    void event_EXT_EVENT_NEAD_PLAY_NOTIF();      // = "ndpntf";
    void event_EXT_EVENT_PLAY_CARD_NOTIF();      // = "plcntf";
    void event_EXT_EVENT_GAME_CHANGE_NOTIF();    // = "gchntf";
    void event_EXT_EVENT_PASS_CARD_NOTIF();      // = "pscntf";
    void event_EXT_EVENT_PLAY_CARD_RES();        // = "plcrs";
    void event_EXT_EVENT_HIT_CARD_ERROR();   //pscrs
	void event_EXT_EVENT_CHICKEN_UPDATE_NTF();		//chkntf
};

#endif /* defined(__iCasino_v2__LayerPlayGameTLMN__) */
