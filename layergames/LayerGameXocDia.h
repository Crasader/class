#ifndef LayerGameXocDia_h__
#define LayerGameXocDia_h__

#include "cocos2d.h"
#include "ThreadQueueMsg.h"
#include "TaiXiuBet.h"
#include "TaiXiuCoin.h"
#include "LayerChanToast.h"
#include "_Button_inGame_.h"
#include "PlayerTaiXiuInfo.h"
#include "AvatarXiTo.h"
#include "QueueTaiXiu.h"
#include "CoinXocDia.h"
#include "BetXocDia.h"
#include "../layers/LayerPopupThongBao.h"
#include "../cotuong/gameUtils.h"
#include "../layers/LayerPopupInfo.h"
#include "Avatar52La.h"
#include "../layers/LayerChatGame.h"

USING_NS_CC;
using namespace std;
class LayerGameXocDia :public Layer,
	public PlayerCallBack
{
private:
	struct UrlCollection
	{
		string normal;
		string active;
		string press;
		string an;
		UrlCollection(string _normal, string _active, string _press, string _an) :
			normal(_normal), active(_active), press(_press), an(_an){};
	};
	struct BtnCoin{
		CoinXocDia* btn;
		int tag;
		BtnCoin(CoinXocDia* _btn, int _tag) :
			btn(_btn), tag(_tag){};
	};
	std::vector<string> ArrListValueBet;
	std::vector<string> ArrListValueBetDefault;
	std::vector<BtnCoin> ArrayListCoinOrg;
	std::vector<ImageView*> arrResult;
	std::vector<std::string> listHistory;
	std::vector<std::string> listTopWinner;
	PlayerTaiXiuInfo* playerInfo;

	bool Animating;
	ThreadQueueMsg *mQueueMsg;
	QueueTaiXiu* mQueueTaiXiu;
	vector<TaiXiuBet*> arrBet;
	vector<TaiXiuCoin*> arrCoin;
	string MyAI;
	boost::shared_ptr<ISFSObject> param;
	LayerChanToast* Toast;
	Size sizeAdd;

	Label* txtNumOfPlayers;
	Text* lblBetMoney;
	Text* lblWinMoney;
	Label* txtMoney;
	Label* txtTimer;
	Sprite* bgTimer;
	ImageView* mDiaNoc;
	//AvatarXiTo* mAvatar;
	Avatar52La* mAvatar;
	LayerHistoryXocDia* layerHistory;

	vector<std::string> lstPlayer;
	string listUserPlay;//danh sach nguoi choi
	string myName;
	Label* lblUserCount;

	Node* rootNode;
	int PlayTurnTime;
	int BetweentwoPlayTime;
	int TimerCountDown = 0;
	int TimerCountNextTime;
	int CountZoder;
	string CurrentBetValue;
	bool CurrentGameState;

	Button* btnCuopCai;
	Label *bossName, *lblBoss, *lblMoneyBoss;
	CircleAvatar* avatarBoss;
	vector<std::string> lstBoss;
	std::string dsBoss;

	Label* lblTimer;
	Label* lblCountNextTime;
	LayerButtonInGame* layerButton;
	//	LayerPopupActiveCai* popupActiveCai;
	// co xd lam cai hay khong, neu lam cai thi khong co action bet
	int status;// trang thai lam cai hay khong: 0-No; 1-Yes

	Button* btnGold;
	Button* btnSilver;
    Button* btnDongTien;
	string rg;
	string rt10;
	int roomType = 0;
	vector<Vec2> posVi;
    
    LayerChatGame* chat;
    
    Text* txtPhienGanNhat;
    vector<ImageView*> lstImgHis;


	//function
	void DisplayInfoMoney(const long& tienconlai, const long& dadatcuoc);
	void sendRequestInfoGame();
	void SelectCoin(long _money);
	string SelectCoinValue(long money, int ext);
	void myTimer();
	void CreateContainerButton(Button* btn, const char* name, const int& group, const int& tag, const char* type, UrlCollection url);
	void CreateContainerCoin(Button* btn, const char* name, const int& tag, const char* type);
	TaiXiuCoin* CreateTaiXiuCoin(Button* item, const std::string& name, const std::string& type, const int& tag);
	TaiXiuBet* GetContainerBetByButtonTag(const int& tag);
	TaiXiuBet* GetContainerBetByBetType(const string& bettype);
	TaiXiuCoin* GetTaiXiuCoin(const string& type);
	int GetMoneyBet(const std::string& gbv);
	void ResetGame(bool state);
	void UpdateTimeStartGame();
	void UpdateTimeEndGame();
	void updateTimer(float dt);
	void updateToNextTurn(float dt);
	void Update5sStep(float dt);
	void ReloadGameWhenReturn(string& gbt);

	void processButtonDanhSachCaiTouched(Ref *pObject, Widget::TouchEventType pType);
	void processButtonExitTouched(Ref *sender, ui::Widget::TouchEventType pType);
	void processButtonHistoryTouched(Ref *sender, ui::Widget::TouchEventType pType);
	void processButtonCupCaiTouched(Ref *sender, ui::Widget::TouchEventType pType);
	void processButtonHuyCaiTouched(Ref *sender, ui::Widget::TouchEventType pType);

	void OnButtonChipClick(Ref *sender, ui::Widget::TouchEventType pType);
	void OnButtonBetClick(Ref *sender, ui::Widget::TouchEventType pType);
	void OnButtonHistoryClick(Ref *sender, ui::Widget::TouchEventType pType);
	void OnButtonTopWinClick(Ref *sender, ui::Widget::TouchEventType pType);
	void BetGameByClient(int tag);

	void processAnimationWin(Vec2 _dest, Vec2 _pos);

	void initPlayerInfo();
	void initListCoin();

	void updateEvent(float dt);
	void OnExtensionResponseProcess();
	void SetRoomConfig();
	void AddCoinCallFunc(Ref *sender, Struct4Call1 data);
	void AddCoinCallFunc2(Ref *sender, TaiXiuBet* data);

	void updateListUser();
	void showAnimation(float dt);

	float press;
	int CurrentTag;
	int OldPressTag;
	float longpresstime;
	void updatepress(float delta);
	void HideAllSuggestion();

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

	void event_EXT_EVENT_REGISTER_BOSS_RES();
	void event_EXT_EVENT_UNREGISTER_BOSS_RES();
	void event_EXT_EVENT_NEXT_BOSS_NTF();
	void event_EXT_EVENT_DEL_BOSS_NTF();
	void event_EXT_EVENT_OFFER_BOSS_NTF();
	void event_EXT_EVENT_LIST_WIN_NTF();
	void event_EXT_EVENT_GET_LIST_BOSS_RES(boost::shared_ptr<ISFSObject> param);

	void GetUrlXiNgau(int number);
    void onBtnChuyenDongTien(Ref* sender);

	void onButtonSilver(Ref* pSender);
	void onButtonGold(Ref* pSender);
	//update 9/8
	string bossNext;
	string bossUserName;
	bool canBeBoss = false;
	bool isLoaded = false;
    
	void updateInfoCai(string _bossName);
	vector<CircleAvatar*> listAvatar;
	void initAllListAvatar();
	void avatarUserCallBack(CircleAvatar* sender, string callback);
	void scheduleLoadAvatar(float dt);
    void processButtonDatX2(Ref* pSender);
    void onBtnXepHang(Ref* pSender);
    Vec2 getPointCoin(string type,TaiXiuBet *bet);
    
protected:
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserCountChange(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
public:
	LayerGameXocDia();
	~LayerGameXocDia();
	void onEnter();
	void onExit();
	virtual bool init();
	CREATE_FUNC(LayerGameXocDia);
	void CallBackFromAnim();
	void processAvatarClicked(Ref *pObject, Widget::TouchEventType pType);
	bool ChuyenTab = false;
    string PATH_SLOT = "design/ResSlot/";
bool isJoin = false;
};

#endif // LayerGameXocDia_h__
