#ifndef LayerTCC_PopupMain_h__
#define LayerTCC_PopupMain_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LayerTCC_DicesHistory.h"
#include "LayerTCC_OneDiceHistory.h"
#include "LayerTCC_BetDices.h"
#include "LayerTCC_Timer.h"
#include "ThreadQueueMsg.h"
#include "LayerChatMiniGame.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerTCC_PopupMain : public cocos2d::Layer,
	public	PlayerCallBack
{

public:
	LayerTCC_PopupMain();
	~LayerTCC_PopupMain();

	CREATE_FUNC(LayerTCC_PopupMain);

	virtual bool init();
	void CallBackFromTimer();
	void CallBackFromAnim();
	virtual void onEnter();
	virtual void onExit();
    int currTypeMoney = 0;
	void startScheduleTimeMoQuy();
	CC_SYNTHESIZE(int, timeMoQuy, TimeMoQuy);
protected:
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	// event server
	void OnExtensionResponseProcess();
	void updateEvent(float dt);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	void event_EXT_EVENT_TCC_GAME_STATE_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EVENT_TCC_GAME_INFO_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EVENT_TCC_END(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_TCC_START(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_TCC_GAME_HIS_RESP(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_TCC_GAME_BET_ERROR_NOTIFY(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_TCC_GAME_BET_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_TCC_GET_QUY_THUONG_RESP(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_TCC_GAME_RESULT(boost::shared_ptr<ISFSObject> param);
private:
	//function
	void createSmartFoxEvent();
	void joinGameRequest();
	void leaveGameRequest();
	void infoGameRequest();
	void gameStateRequest();
	void scheduleRequestQuyThuong(float dt);

	void handleGameState(int state, int gametime, int endtime, int playtime);
	void handleCurrentBet(string gbt);


	void createListBetValues(string glb);
	void createLayerDiceHis();
	void createLayerBetDice();
	void createListBetCoin();
	void createBetRateList();
	void createListResDices();
	void createPlate();
	void createUI();
	void createLayerTimer();

	void onButtonBetRate(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonBetCoin(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonRank(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonHis(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonTut(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonMoQuy(Ref* pSender, ui::Widget::TouchEventType type);

	void reloadBetCoins(int tag);
	void showPressStateButton(Button* pSender, bool isPress);
	void startAnimShake();
	void showResult(string _result);
	void actionOpenBowlDone(string _result);
	void delayToShowPrepare();

	void resetGame();
	void startGame();
	void prepareGame();
	void showResultGame(int money);
	
	void scheduleTimeMoQuy(float dt);
	void getResFromMatchID(string strHis, string mid);
    
    void onBtnSilver(Ref *sender);
    void onBtnGold(Ref *sender);

	///variables
	boost::shared_ptr<ISFSObject> param;
	Layout *pnlMain;
	LayerTCC_DicesHistory *layerDiceHis;
	LayerTCC_OneDiceHistory *layer1DiceHis;
	LayerBetDice*layerBetDices;
	Text*lblMatchId;
	Text*lblChestMoney;
	Layout* pnlDiceHis;
	Layout* pnl1DiceHis;
	Layout* pnlPlate;
	ImageView*	imgBowl;
	LayerTCC_Timer* layerTimer;

	int startBowlX = 0;
	Sprite* line_25;

	vector<Button*> listBetRates;
	vector<Button*> listBetCoins;
	vector<string> listBetRateValues;
	vector<string> listBetCoinValues;
	vector<ImageView*> listResDices;//mang image xuc xac hien thi ket qua
	vector<ImageView*>listResImgPlates;//mang xuc xac hien thi tren dia
	int currBetRate = 0;//danh dau dang o phong nao - 100 -1000 -10000

	ThreadQueueMsg *mThreadQueueMsg;
	bool isGameStarted = false;//co danh dau trang thay game chua bat dau hay dang dien ra
	string myName;
	//config variable
	int timePrepare = 1;
	int timeRequestQuyThuong = 5;//send request 5s 1 lan

	//biến lưu thời gian va phan tram an cua tung o cho layer mở quỹ thưởng
	//int timeMoQuy = 0;
	string listPercent;
	string listQuyThuong;
	string listHisTop;//danh cho xep hang diem tich luy
	string listHisUser;
	string currMatchID;
    
    Button* btnGold;
    Button* btnSilver;
    
	LayerChatMiniGame* layerChat;
	void onButtonChat(Ref* pSender);
    EventListenerTouchOneByOne* listener;
};


#endif // LayerTCC_PopupMain_h__
