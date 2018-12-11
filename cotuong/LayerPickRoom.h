#ifndef LayerRoom_h__
#define LayerRoom_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "../GameServer.h"
#include "layergames/CircleAvatar.h"
#include "SceneManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerPickRoom :public cocos2d::Layer,
	public cocos2d::extension::TableViewDataSource,
	public cocos2d::extension::TableViewDelegate,
	public PlayerCallBack
{
public:
	virtual bool init() override;
	LayerPickRoom();
	~LayerPickRoom();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerPickRoom);
	virtual void onEnter();
	virtual void onExit();

	void onButtonPlayNow(Ref* pSender);
protected:
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
	virtual void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	//smartfox event
	void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void onScheduleRejoinRoom(float dt);
	void autoInvitePlay(float dt);
private:
	Text* lblGameName;
	TableView* tableView;
	Button* btnPlayNow;
	Button* btnCreateRoom;
	Layout * pnlList;
	Button* btnTab1;
	Button *btnTab2;
	Button *btnTab3;
	Button* btnMoneySilver;
	Button* btnMoneyGold;
	Text* lblGold;
	Text* lblSilver;
	CheckBox* cb1;
	CheckBox* cb2;
	CheckBox* cb3;
	Text* lblMoney1;
	Text* lblMoney2;
	Text* lblMoney3;
	Text* lblMoney4;
	Sprite* bt_cuoc2_5;
	CircleAvatar* circleAvatar;
	Text* lblName;
	Text* lblMoneySilver;
	Text* lblMoneyGold;
	Text* lblLevel;
	LoadingBar* prgProgress;

	bool isFullRoomFilter = false;
	bool isEmptyRoomFilter = false;
	bool isFriendFilter = false;
	int filterBetValue = -1;
	//variables for rejoin room
	bool isRejoinRoom;
	int mCurrentRoomIDJoin;


	void onButtonCreateRoom(Ref *pSender);
	void onButtonBack(Ref* pSender);
	void onCheckbox(Ref* pSender, CheckBox::EventType type);
	void onLabelBetValue(Ref* pSender);
	void onButtonTypeRoom(Ref* pSender);
	void onButtonCharge(Ref *pSender);
	void onButtonAvatar();
	void onButtonFindRoom(Ref *pSender);

	void loadUserInfo();
	void loadLabelByZone(int zoneIdx);
	void sendListBetRequest();
	void sendListRoomRequest();
	void registerInviteRequest();

	//filter room
	vector<int> listBetFilter;
	void onButtonFilterRoom(Ref* pSender);
	void parseListBet(string listBet);
	double getMinBetByGame(int gameID, double amf);

	//updated 16/5
	int currTab = 0;//xac dinh dang o binh dan, trieu phu hay dai gia
	int roomType = 0;//xac dinh danh gold hay silver - silver = 1, gold = 0
	struct RoomInfo{
		int room_max_user = 0;
		int room_id = 0;
		int room_money = 0;
		bool isPasswordProtected = false;
		int room_numbers = 0;
		bool allow_view = true;
		int spectator_count = 0;
	};
	int currGameID = 0;
	CC_SYNTHESIZE(string, sCurrGroupList, CurrGroupList);
	vector<RoomInfo> lstTables;

	void subscribeGame(string type);
	void unSubscribeGame();
	void loadAllData();
	vector<boost::shared_ptr<Room> > getListRooms();
	void onScheduleUpdateRoomList(float dt);
	void onPopupInviteCallBack();

	struct less_than_key
	{
		inline bool operator() (const RoomInfo& struct1, const RoomInfo& struct2)
		{
			if (struct1.room_money == struct2.room_money)
				return struct1.room_id > struct2.room_id;
			else
				return struct1.room_money < struct2.room_money;
		}
	};
	struct sort_size_table
	{
		inline bool operator() (const RoomInfo& struct1, const RoomInfo& struct2)
		{
			return struct1.room_numbers < struct2.room_numbers;
		}
	};
	//update 11/7
	struct structChat{
		string Name;
		string Content;
		bool isadm;
		structChat(string _name, string _content, bool _isadm) :
			Name(_name), Content(_content), isadm(_isadm){};
	};

	TextField* txtChat;
	TableView* tblChat;
	vector<structChat> lstChat;

	float getHeightForCell(string _strMsg);
	bool isImageExist(string url);
	void joinRoomChat();
	void leaveRoomChat();
	void onButtonChat(Ref* pSender);
	void parseChat(string json);
	void doChat(string txt);
	bool checkSpamChat(string txt);
	void timeoutChatMaxtime(float dt);
	void startTimeOutChat();
	void getHisChatReq();
	void onButtonChuyenTien(Ref* pSender);
	void onButtonUngTien(Ref* pSender);

	TableViewCell* createCellForChat(TableView* table, ssize_t idx);
	TableViewCell* createCellForRoomList(TableView* table, ssize_t idx);

protected:
	void OnSmartFoxRoomCreationError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomAdd(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomDestroy(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	//update 11/7
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	boost::shared_ptr<Room> roomChat;
	//update 19/8
private:
	Layout* pnlNotify;
	Label* lblThongBao;
	int currIndexNotification = 0;
	vector<NotificationInfo> gListNotification;
	string linkPopUp;
	Sprite* spriteBg;
	bool isFirstJoin = true;

	void createLabelThongBao(string _str);
	void onScheduleThongBao(float dt);
	void getNotificationFromUrl();
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	//update 16/8
	void callBackFromPopupHetTien();
};

#endif 



