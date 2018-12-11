#ifndef __iCasino_v2__ScenePickRoom__
#define __iCasino_v2__ScenePickRoom__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "GameServer.h"
#include "../layers/BagItem.h"
#include "../UINumberPicker.h"
#include "layergames/CircleAvatar.h"
#include "../layers/LayerMailInbox.h"
#include "layers/LayerMainBottom.h"
#include "../SceneManager.h"
#include "../layers/LayerPopupCuaHang.h"
#include "../layers/LayerBangXepHang.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerPlayRoom : public Layer{

public:
	LayerPlayRoom(void);
	~LayerPlayRoom(void);

	CC_SYNTHESIZE(int, nRoomID, RoomID);
	CC_SYNTHESIZE(int, nGameID, GameID);
	CC_SYNTHESIZE(int, nBetValue, BetValue);

	virtual bool init(int gameID);
	static LayerPlayRoom* create(int gameID);
	//for type play table
	void setDataRoom(int betvl, int usercount);
	//for type bet table
	void setDataBet(int _money, int _type, int _state);
private:
	Text* txtBetValue;
	Node* rootNode;
	int userCount;
	int typePr;

	//for type bet table
	Button* btnCoin;
	Sprite* coin;
	Label* labelmoney;
    ImageView* imgCoin;
	int moneybet;
    
    Button* imgTable;

	int getUserCountByGame(int gameID);
	void onButtonRoom(Ref* pSender, ui::Widget::TouchEventType type);
	double getRatioMinBetByGame(int gameID, double amf);
	double getMinBetByGame(int gameID, double amf);
};

class ScenePickRoom : public cocos2d::Layer
	, public PlayerCallBack
	, public TableViewDataSource, public TableViewDelegate
{
public:
	ScenePickRoom();
	~ScenePickRoom();

	CC_SYNTHESIZE(string, sCurrGroupList, CurrGroupList);
	CC_SYNTHESIZE(int, nGameID, GameID);
    
    
    int roomType = 0;
    void onButtonTypeRoom(Ref* pSender);
    Button* btnMoneyGold;
    Button* btnMoneySilver;
    
	static cocos2d::Scene* createScene(int gameID);

	void subscribeGame(string type);
	void unSubscribeGame();

	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
    void onButtonTaoBan(Ref* pSender);
    void onButtonTimBan(Ref* pSender);
	//void onButtonCreate(Ref* pSender, ui::Widget::TouchEventType type);
	//void onButtonPlayNow(Ref* pSender, ui::Widget::TouchEventType type);

	void continueInvitePlay();
	void setDenyAllInvite();

	void gotoChargeMoney();

	// 	void createRoom(int gameID, int betValue, string pass);
	// 
	// 	void playRoomCallBack(int resultCode, int roomID);

	void onScheduleReloadPlayRoom(float dt);
	void onScheduleRejoinRoom(float dt);
	void onScheduleUpdateRoomList(float dt);

	void initFilterMoney(string listBet);
    void onButtonRoomScene(Ref* pSender);
	virtual bool init()override;
	CREATE_FUNC(ScenePickRoom);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
	void showToast(string mess);
	LayerMainBottom *layerScenePickRoomBottom;
    void createScrollView();
    void createScrollViewNew();
	virtual void onEnter()override;
	virtual void onExit()override;
protected:
	virtual void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	virtual void OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	virtual void OnSmartFoxRoomCreationError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	virtual void OnSmartFoxRoomAdd(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	virtual void OnSmartFoxRoomDestroy(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	virtual void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	virtual void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	virtual void OnSmartFoxInvitation(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;

	void autoInvitePlay(float dt);
	vector<boost::shared_ptr<Room> > getListRooms();
	//bien de set hien thi ds ban theo moi hay cu
	void setGListNotification(vector<NotificationInfo> notifi);
private:
	CC_SYNTHESIZE(bool, isDenyAllInvite, IsDenyAllInvite)
	enum tagNotification{
		tagComfirmInvite = 100,
		tagAutoInvite,
	};
	enum tag_filter
	{
		tag_binh_dan,
		tag_trieu_phu,
		tag_dai_gia,
		tag_all
	};
    ui::ScrollView* scrollView;
	ui::Layout* mScrollViewTableList;
	Text* m_lblGameName;
	Button* btnDaiGia;
	Button* btnBinhDan;
	Button* btnTrieuPhu;
	Button* btnAll;
	//
	bool isMoneySortAsc;
	bool isDestroy;

	int mRoomID4Invite;

	int mCurrentRoomIndex;

	int mCurrentRoomIDJoin;

	string passRoom4Invite;

	vector<int> listCuoc;

	bool isRejoinRoom;

	bool isWebView = true;

	double userMoney;// bien luu lai so tien ban dau de kiem tra khi nhay vao ham uservariable update neu thay doi se cap nhat danh sach ban
	//bool sortPro(RoomInfo& lhs, RoomInfo& rhs) const;
	//vector<RoomInfo> lstTables;
	TableView* tblPlayRooms;
	/************************************************************************/
	/*              MINH DV - 11/4 - new List Room                                                        */
	/************************************************************************/
	struct RoomInfo{
		int betvl;//bet value
		int usercount;//so nguoi
		int type;//dang ban
		bool state;//trang thai cua ban
		RoomInfo(int _betvl, int _usercount) :
			betvl(_betvl), usercount(_usercount){};
		RoomInfo(int _betvl, int _type, bool _state) :
			betvl(_betvl), type(_type), state(_state){};
	};
	//list room get from sever
	vector<RoomInfo> listRoom;//list room khac chi de gan vao thang room chinh
	vector<RoomInfo> listRoomMain;
	vector<string> lstFilterMoney;
	int currFilterMoneyIndex;//danh dau dang o muc cuoc nao nhu 100, 200
	int currFilterBet;//danh dau dang o muc dai gia hay binh dan
	vector<string> arrBet;//mang luu lai tat ca cac muc cuoc
	//UINumberPicker *picker;


	Label* lblThongBao;
	int currIndexNotification = 0;
	Layout* pnlThongBao;
	bool isAutoReload = false;

	void onScheduleThongBao(float dt);
	void createLabelThongBao(string _str);
	void onButtonThongBao(Ref* pSender, ui::Widget::TouchEventType type);

	void onButtonFilterRoom(string betvl);
	void onButtonFilterPicker();//ham lay muc cuoc va hien thi len picker
	void onButtonFilter(Ref* pSender, ui::Widget::TouchEventType type);
	void pickerChangeHandler(cocos2d::Node *pnode);
	string getGameNameByID(int id);
	///create cell for 2 version
	TableViewCell* createCellOldVersion(TableView *table, ssize_t idx);
	TableViewCell* createCellNewVersion(TableView *table, ssize_t idx);

	//Mail Top
	// 	Button* btnMail;
	// 	BagItem* bagItem;
	int tagChild;
	int const TAG_CHILD = 111;
	void gotoMail(Ref* pSender);
	void gotoPickRoom();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;
	Text *lblName, *lblLevel, *lblMoney;
	ImageView* avtVip, *imgAvatar;
	LoadingBar *prgLevelProgress;
	CircleAvatar* circleAvatar;

	///method
	void onButtonCharge(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonSearchRoom(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonUserInfo(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonPlayNow(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonRank(Ref* pSender, ui::Widget::TouchEventType type);
	void scheduleUpdateInfo(float dt);
	void loadDataInfo();
    
    void timeoutChatMaxtime(float dt);
    void startTimeOutChat();
    void onBtnSticker(Ref* pSender);

    
	// 	int getMaxMoneyCreateRoomByBet(int gameID, double currBet);
	// 	int getFilterTypeByUserMoney(double currBet, int gameID);
	void createListData();
	bool getStateByMoney(int total_show, int index, int default_hide);
	boost::shared_ptr<Room> roomChat;
	double getMinBetByGame(int gameID, double amf);
    void hideWebView();
    void showWebView();
	//MINHDV -changes 26/4 - get new notification 
	void getNotificationFromUrl();
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	vector<NotificationInfo> gListNotification;
	string linkPopUp;
	//MINHDV - changes 18/7 - switch table type
	int typePR;
	//for gamebai 2.0
    string urlSticker;


	void sendListBetRequest();

	Sprite* Image_24;
    Sprite* sprStick;
	Text* lblMoney1;
	Text* lblMoney2;
	Text* lblMoney3;
	Text* lblMoney4;
	Text* lblMoneyAll;
	Sprite* spriteArrow;
    LayerBangXepHang* bxh;
    Text* txtXH;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
	void onButtonFilterMoney(Ref* pSender);
};

class LayerPopupJoinRoomPass : public Layer{
private:
	Button* btnCancel;
	Button* btnJoinRoom;
	TextField* txtPassRoom;
	int roomID;
	SEL_CallFunc mCallBackListener;
	Ref *mCallBack;
	void onButtonCancelClicked(Ref* pSender, Widget::TouchEventType type);
	void onButtonJoinRoomClicked(Ref* pSender, Widget::TouchEventType type);
	virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
public:
	LayerPopupJoinRoomPass();
	~LayerPopupJoinRoomPass();
	virtual bool init();
	CREATE_FUNC(LayerPopupJoinRoomPass);
	void onPassWordRoomCallBack(Ref *target, SEL_CallFunc listener);
	virtual void onEnter();
	virtual void onExit();
	void setRoomID(int roomID);
};


#endif //__iCasino_v2__ScenePickRoom__
