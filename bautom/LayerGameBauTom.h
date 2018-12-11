#ifndef LayerGameBauTom_h__
#define LayerGameBauTom_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "../GameServer.h"
#include "../layergames/ThreadQueueMsg.h"
#include "layergames/CircleAvatar.h"
#include "LayerChatInGame.h"
#include "../layers/LayerBangXepHang.h"
#include "../layers/LayerChatGame.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;
class LayerGameBauTom : public cocos2d::Layer,
	public cocos2d::extension::TableViewDataSource,
	public cocos2d::extension::TableViewDelegate,
	public PlayerCallBack
{
public:
	virtual bool init() override;
	LayerGameBauTom();
	~LayerGameBauTom();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerGameBauTom);

	virtual void onEnter();
	virtual void onExit();
protected:
	void OnExtensionResponseProcess();
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxUserCountChange(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
private:
	Layout* pnlMain;
	//Lich su van choi
	Layout* pnlHis;
	Button* btnCloseHis;
	Layout* pnlTable;
	vector<string> listHis;
	TableView* tableViewHis;
	Vec2 firstPosHis;
	bool isHide = true;
	void hidePanelHis(bool isHide);
	//nha cai
	Button* btnCuopCai;
	Text* lblBossMoney;
	Node* nodeAvatarCai;
	CircleAvatar* avatarCai;
	//danh sach nguoi choi
	bool isLoadedAvatar;
	Node* nodeAvatarPlayer;
	Text* lblUserName;
	Text* lblUserMoney;
	CircleAvatar* avatarMe;
	string myName;
	double amfUserClient = 0;//so tien nguoi choi luu duoi client
	string lastBetType = "";//luu lai o cuoc chon cuoi cung
	int lastTag = 0;
	struct DiceInfo{
		string bettype;
		double userbet;
		double totalbet;
		DiceInfo(string _bettype, double _userbet, double _totalbet) :
			bettype(_bettype), userbet(_userbet), totalbet(_totalbet){};
	};
	vector<DiceInfo> listBetInfoClient;//so tien dat luu o duoi client
	Text* lblBetEat;
	Text* lblBet;
	int totalBetEat = 0;
	bool canBeBoss;
	//button chon muc cuoc
    
    Button* btnBet500;
    Sprite* spActive500;
    
	Button* btnBet1k;
	Sprite* spActive1k;

	Button* btnBet5k;
	Sprite* spActive5k;

	Button* btnBet10k;
	Sprite* spActive10k;

	Button* btnBet50k;
	Sprite* spActive50k;

	Button* btnBet100k;
	Sprite* spActive100k;

	Button* btnBet500k;
	Sprite* spActive500k;

	Button* btnBet1000k;
	Sprite* spActive1000k;
	//button bau tom
	Button* btnTom;
	Button* btnCua;
	Button* btnCa;
	Button* btnGa;
	Button* btnHo;
	Button* btnBau;
	//cac sprite active,x2, x3 cua cac button bet
	Sprite* spActiveTom;
	Sprite* spX2Tom;
	Sprite* spX3Tom;

	Sprite* spActiveCua;
	Sprite* spX2Cua;
	Sprite* spX3Cua;

	Sprite* spActiveCa;
	Sprite* spX2Ca;
	Sprite* spX3Ca;

	Sprite* spActiveGa;
	Sprite* spX2Ga;
	Sprite* spX3Ga;

	Sprite* spActiveBau;
	Sprite* spX2Bau;
	Sprite* spX3Bau;

	Sprite* spActiveHo;
	Sprite* spX2Ho;
	Sprite* spX3Ho;

	//cac label hien thi tien dat cuoc
	Text* lblTom;
	Text* lblTotalTom;

	Text* lblCua;
	Text* lblTotalCua;

	Text* lblCa;
	Text* lblTotalCa;

	Text* lblGa;
	Text* lblTotalGa;

	Text* lblBau;
	Text* lblTotalBau;

	Text* lblHo;
	Text* lblTotalHo;

	//cac thong tin khac
	Text* lblRoom;
	Text* lblRoomMoney;

	//Button betx2, x3
	Button* btnReBet;
	Button* btnX2;
	Button* btnX3;
    
    LayerChatGame* chat;
    
    Text* txtTimerHuThuong;
    Text* txtHuThuong;



	void initButton();
	void initButtonBet();
	void initPanelHis();
	void initPanelUser();
	void initPanelChat();
	void initTable();
	void initAllVariables();
    void updateTimeMoQuy(float dt);
    int timeNoHu = 0;
	//tao list avatar
	Label* lblUserCount;
	vector<CircleAvatar*> listAvatar;
	void initAllListAvatar();

	//method button
	void onButtonQuit(Ref* pSender);
	void onButtonHis(Ref* pSender);
	void onButtonChat(Ref* pSender);
	void onButtonCamera(Ref* pSender);
	void onButtonCuopCai(Ref* pSender);
	void onButtonMenu(Ref* pSender);
	void onButtonCloseHis(Ref* pSender);
	void onButtonBet(Ref* pSender);
    void onButtonBauTom(Ref *sender, ui::Widget::TouchEventType pType);
	void onButtonReBet(Ref* pSender);
	void onButtonX2(Ref* pSender);
	void onButtonX3(Ref* pSender);
	void onButtonCoin(Ref* pSender);
    void onBtnXepHang(Ref* pSender);
    void onBtnHuongDan(Ref *sender, ui::Widget::TouchEventType pType);
    void onBtnHuThuong(Ref* pSender);
    void animationBet(int money,string bettype,int showtype);
    void animationBetWhenReturn(int money,string bettype,int showtype);
    void callBackAnimation2();
    void showAnimationThuTien();
    void showAnimationTraTien();
    bool checkImgWin(string tag);
    int getTypeByBet(string tag);
    void showAnimationTraTienUser();
    void rollbackBeInt(int money,string bettype);
    void showMoneyWin();
    void animationBetOther(int money,string bettype,int showtype);
    Sprite* currRolBackSprite;
	LayerMocChatInGame* layerChatInGame;
	string _filename;//ten anh chup man hinh
	Button* btnCamera;
	void afterCaptured(bool succeed, const std::string& outputFile);
	//sfs variables
	ThreadQueueMsg*		mThreadQueueMsg;
	boost::shared_ptr<ISFSObject> param;
    void updateEvent(float dt);
	//animation bat
	ImageView* mDiaNoc;
	vector<ImageView*> arrResult;
	vector<Vec2> posVi;//vi tri random
    vector<string> lstImg;
    vector<Sprite*> listImgMoney;

    vector<Sprite*> listImgTom;
    vector<Sprite*> listImgCua;
    vector<Sprite*> listImgCa;
    vector<Sprite*> listImgHo;
    vector<Sprite*> listImgBau;
    vector<Sprite*> listImgGa;
    vector<Sprite*> listImgMove;
    vector<Sprite*> listImgWin;
    vector<Sprite*> listCheckRemove;
    vector<Sprite*> listImgMeTom;
    vector<Sprite*> listImgMeCua;
    vector<Sprite*> listImgMeCa;
    vector<Sprite*> listImgMeHo;
    vector<Sprite*> listImgMeBau;
    vector<Sprite*> listImgMeGa;
    vector<Sprite*> listImgMeWin;
    
    string listwin = "";



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
	void event_EXT_EVENT_OFFER_BOSS_NTF();
	void event_EXT_EVENT_REGISTER_BOSS_RES();
	void event_EXT_EVENT_NEXT_BOSS_NTF();
	void event_EXT_EVENT_LIST_WIN_NTF();

	//////////////////////////////////////////////////////////////////////////

	vector<int> listBetvalue;
	int currBetvalue = 0;
	void initBetString();
	//update from xoc dia
	int TimerCountNextTime;
	int BetweentwoPlayTime;
	int TimerCountDown = 0;
	bool CurrentGameState;
	Label* txtTimer;
	Sprite* bgTimer;
	int PlayTurnTime;
	string listUserPlay;//danh sach nguoi choi
	//update timer
	void UpdateTimeStartGame();
	void updateTimer(float dt);
	void UpdateTimeEndGame();
	void updateToNextTurn(float dt);
	//animation xoc dia
	string resultGame = "";
	void showAnimation(float dt);
	void callBackAnimation();
	//update info every body
	void updateListUser();
	void updateInfoMe();
    void updatepress(float delta);
    
    float press;
    float longpresstime;
    int OldPressTag;

    void HideAllSuggestion();
	//update trang thai cac button bet
	void setStateButtonBet(int _tag);
	void setMoneyBetBautom(int totalBet, int meBet, string tagBetType);//hien thi so tien bet o tung o tom cuaca
	void updateBetInfoClient(string bettype, int total, int meBet);
	void rollbackBetInfoClient(string bettype, int betvalue);
    void SelectCoinValue(long money);
	//show ket qua
	int countGa = 0;
	int countHuou = 0;
	int countCa = 0;
	int countBau = 0;
	int countTom = 0;
	int countCua = 0;
	void setResultGame(string betType);
	//////////////////////////////////////////////////////////////////////////
	void ReloadGameWhenReturn(string gameBetType);
	void resetGame();
	void resetButtonBautom();
	//validate
	bool validateBet(int betValue);
	void SetRoomConfig();
	string getResImageByBetType(string bettype);
	void showChatByName(string nameChat, string message);
	void onPopupCallBack(Ref* pSender);
	void avatarCaiCallBack();
	void avatarMeCallBack();
	void avatarUserCallBack(CircleAvatar* sender, string callback);
    
	string bossName;
	string bossNext;
	void onButtonDsCai(Ref* pSender);
    void onBtnChuyenDongTien(Ref* sender);

    void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	ProgressTimer* timerHo;
	ProgressTimer* timerCa;
	ProgressTimer* timerBau;
	ProgressTimer* timerCua;
	ProgressTimer* timerGa;
	ProgressTimer* timerTom;
	ProgressTimer* createTimerFromSprite(Sprite *inputSprite);
    void onBtnCheat(Ref* btnSet);
	void updateInfoCai(string _bossName);
	bool meIsBoss = false;
    bool isJoin = false;
    int currTypeMoney = 0;
    Button* btnChuyenDongTien;
    ImageView* imgMoneyTom;
    ImageView* imgMoneyCua;
    ImageView* imgMoneyCa;
    ImageView* imgMoneyGa;
    ImageView* imgMoneyBau;
    ImageView* imgMoneyHo;
    
    TextField* txtCheat;

    bool isReset = false;

};

class LayerItemHis : public Layer{
public:
	LayerItemHis();
	~LayerItemHis();
	CREATE_FUNC(LayerItemHis);
	virtual bool init();

    void setDataDice(int idx,string _result);
private:
	ImageView* img1;
	ImageView* img2;
	ImageView* img3;
    ImageView* imghis;
    Text* txtSTT;

	string getResImageByBetType(string bettype);
};
class LayerItemListCai : public Layer{
public:
	LayerItemListCai();
	~LayerItemListCai();
	CREATE_FUNC(LayerItemListCai);
	virtual bool init();

	void setData(string _username, int idx);
private:
	Text* lblTitle;
	Text* lblName;
	Text* lblStatus;
};
class LayerPopupListCai :public Layer,
	public cocos2d::extension::TableViewDataSource,
	public cocos2d::extension::TableViewDelegate,
	public PlayerCallBack
{
public:
	LayerPopupListCai();
	~LayerPopupListCai();
	CREATE_FUNC(LayerPopupListCai);
	virtual bool init();
	void disableButtonRegister();
protected:
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
private:
	bool isCanBeBoss;
	Button* btnRegister;
	void onButtonRegisterBoss(Ref* pSender);
	void onButtonClose(Ref* pSender);
	TableView* tableViewHis;
	vector<string> listCai;
};
#endif // LayerGameBauTom_h__



