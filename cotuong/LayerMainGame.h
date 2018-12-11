#ifndef LayerMainGame_h__
#define LayerMainGame_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "LayerUserPlayInfo.h"
#include "Chess.h"
#include "LayerChat.h"
#include "../GameServer.h"
#include "../layergames/ThreadQueueMsg.h"
#include "LogicInChess.h"
#include "LayerPopupReady.h"
#include "CommonChess.h"
#include "LayerPopupAlert.h"
#include "LayerPopupKingCheck.h"
#include "LayerBottomInGame.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

struct EatPos{ //struct dinh nghia truoc cac vi tri cac quan co an duoc animation toi
	int status;
	int id;
	Vec2 pos;
};

class LayerMainGame : public cocos2d::Layer,
	public PlayerCallBack,
	public cocos2d::extension::TableViewDataSource,
	public cocos2d::extension::TableViewDelegate
{
public:
	virtual bool init() override;
	LayerMainGame();
	~LayerMainGame();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerMainGame);

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent){};

	virtual void onEnter();
	virtual void onExit();
	void updateTimer(float dt);//update thoi gian thi dau cua tung user
protected:
	void OnExtensionResponseProcess();
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxUserEnterRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
private:
	enum TAG_CALLBACK{
		TAG_QUIT,
		TAG_READY_SETUP,//người có đồng ý setup hay không
		TAG_SETUP_DONE,//người setup 

		TAG_LEAVE_GAME,
		TAG_PEACE_REQUEST,//yeu cau cau hoa
		TAG_PEACE_RESPONSE,//phan hoi cau hoa
		TAG_LOOSE
	};
	Size sizeScreen;
	float startLeft;
	float startTop;
	float scaleApp;
	string myName;//ten cua minh

	Layout* pnlMain;
	//User panel
	vector<LayerUserPlayInfo*> listUserInfoLayer;
	Text*lblBetValue, *lblTime1, *lblTime2;
	Text* lblRoomName;
	Sprite* spTable;
	//Other ui
	Button* btnChat;
	Layout* pnlChat;
	TableView* tblChat;
	LayerChat* layerChat;
	std::vector<ChatInfoStruct> listStore;
	//chess 
	vector<Chess*> arrChess;
	Sprite *prevPoint;//ghi lai nuoc di truoc do
	//Sprite *justPoint;//nuoc di hien tai

	/*logic game*/
	ChessLogic *logicChess;
	int foldPos[90];
	int indexCurrent;//index quan co hien tai
	int indexTarget;//index quan co dc chon

	int indexCurrentHis;//luu lai de rollback nuoc di
	int indexTargetHis;//luu lai de rollback nuoc di

	//layer timer
	LayerPopupReady* layerTimer;

	int convertIDByRow(int id);
	int convertIDByCol(int id);
	Chess* getChessByID(int id);
	void moveChess(int fromID, int toID, bool isCheckChieuTuong, int fromChessType = -1);
	void showHistoryPoint(int fromID, int toID, bool isCheck); // ham bieu dien lai nuoc da di
	void showCurrentPoint(int currIndex);//show iicon quan co duoc chon
	void hideCurrentPoint();

	//other variables
	int currGameID;
	bool isStartedGame;
	bool isSpectator;
	bool isYourTurn;
	string nameCurrentTurn;
	string listUserPlay;
	string player1; // người ở trên, màu đen
	string player2; // người ở dưới, màu đỏ - là mình

	int timeRestBlack;//tong thoi gian  cua quan den
	int timeRestRed;//tong thoi gian cua quan do

	int timeForTurnBlack = 90;//thoi gian cua moi turn black
	int timeForTurnRed = 90;//thoi gian cua moi turn red

	//init method
	void initPanelUser();
	void initTableChess();
	void initUI();
	void initVariables();
	void initPointInTable();//khoi tao cac point ve cac nuoc di co the tren ban co

	void refreshChess();
	void resetGame();
	bool checkSpectator();


	void drawCanMove(vector<int> arrPos);//ve cac nuoc di co the cho quan co
	void drawChess(); // vẽ lại các quân cờ trên bàn dựa vào id_pos của chúng
	//caculte and convert index to point in table chess
	Vec2 getPositionFromIndex(int id);
	Vec2 getPositionFromIndexServer(int id);
	int getIndexFromPosition(Vec2 pos);
	float getDistant2Point(Point p1, Point p2);
	void setPointVisible(int id, bool isShow);//ve cac diem tren ban co - check nuoc di co the

	Chess* getChessByName_Side(int name, int side);
	Chess* getChessByName_SideForCheck(int name, int side);
	Chess* getChessByRawName_Side(int name, int side);
	Chess* getChessByInitName_Side(int name, int side);
	Chess* getAteChessByRawName_Side(int name, int side); // ham lay cac quan da an, chi danh cho co up

	//button callback
	void onButtonChat(Ref* pSender);
	void onButtonReady(Ref* pSender);

	//smartfox event handle
	void event_EXT_EVENT_LIST_USER_UPDATE();
	void event_EXT_EVENT_NOT_ENOUGH_MONEY();
	void event_EXT_EVENT_START();             // = “s”();
	void event_EXT_EVENT_NEXT_TURN();         // = "nt"();
	void event_EXT_EVENT_END();               // = "e"
	void event_EXT_EVENT_READY_NTF();         // = "rntf"();
	void event_EXT_EVENT_GAME_RESULT();       // = "grs"();
	void event_EXT_EVENT_ERROR_READY_NTF();   // = "e_rntf"();
	void event_EXT_EVENT_MOVE_NTF();          // = "mntf"();
	void event_EXT_EVENT_UNDO_MOVE_REQ();
	void event_EXT_EVENT_UNDO_MOVE_NTF();
	void event_EXT_EVENT_CHESS_TABLE_NTF();
	void event_EXT_EVENT_UNREADY_NTF();
	void event_EXT_EVENT_USER_INFO();
	void event_EXT_EVENT_AUTOKICK();
	void event_EXT_EVENT_MOVE_ERROR();
	void event_EXT_EVENT_START_READY_TIMER_NOTIF();
	void event_EXT_EVENT_WARNING_NOTIF();
	//thread queue
	ThreadQueueMsg*		mThreadQueueMsg;
	boost::shared_ptr<ISFSObject> param;
	void updateEvent(float dt);

	//config tao ban
	int countChapTien = 0;//ham luu lai o client, nếu có chấp tiên thì sẽ check index quân cờ > 45 thì không cho phép
	int numUser = 0;//so nguoi trong phong
	string uidConfig;//user thay doi config
	string paramConfig;//param sau khi config
	void requestChangeSetting();
	void event_EXT_EVENT_CHANGE_SETTING();
	void event_EXT_EVENT_CHANGE_SETTING_NOTIF();
	void event_EXT_EVENT_PRE_CHANGE_SETTING_NOTIF();
	void loadConfigRoom(string roomConfig);//ham thuc thi thay doi theo config ban
	void callBackFromAlert(LayerPopupAlert* sender, int tag);

	void loadChessTableByType();
	LayerPopupKingCheck* popupKingCheck;
	void playSound(int _type);

	void testReconnect();
	//updated 21/5
	//Text* lblViews;
	LayerPopupVisitor* popupVisitor;
	LayerBottomInGame* layerBottom;

	//bottom in game
	Button* btnBack;
	Button* btnSound;
	Button* btnPeace;
	Button* btnSetting;
	Button* btnLose;
	Button* btnSpec;
	bool isShowSetting;
	CC_SYNTHESIZE(bool, isFirst, FirstConfig);//ham de set config lan dau tien khi co user vao ban
	CC_SYNTHESIZE(string, params, ParamConfig);

	void initBottomInGame();
	void onButtonBack(Ref* pSender);
	void onButtonSetting(Ref* pSender);
	void onButtonPeace(Ref* pSender);
	void onButtonLose(Ref* pSender);
	void onButtonSound(Ref* pSender);
	void onButtonCharge(Ref* pSender);
	void onButtonSpector(Ref* pSender);

	void event_EXT_EVENT_GAME_LEAVE_RES(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_FOR_PEACE_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_PRE_CHANGE_SETTING_RESPONSE(boost::shared_ptr<ISFSObject> param);
	void createLayerNotification(string title, string content_mess, int tag);
	void showSpectatorMode(bool isSpec);
	//update 16/6 - for switch sound
	Sprite* bt_sound2_5;
	Sprite* bt_sound2_6;
	Sprite* bt_text_am_8;
	Sprite* tat_am_4;

	Sprite* timeSprite1;
	Sprite* timeSprite2;
	Sprite* timeSpriteTurn1;
	Sprite* timeSpriteTurn2;
	Sprite* Sprite_13;

	void showTurn(int type);//0 la nguoi choi 1, 1 la nguoi choi thu 2, 3 la reset game
	//update 10/7
	int betValue = 0;
	vector<EatPos> arrEatPosPlayer1;
	vector<EatPos> arrEatPosPlayer2;
	vector<EatPos> createArrPos(Vec2 startPoint);
	void moveEatChess(Chess* chess, float duration);///di chuyen cac quan da an vao goc
	void showEatChess(string listChess);
	//update 17/7
	int countGame = 0;//bien luu lai neu = 1, se khong gui ban tin pre change setting 
	string enemyName = "";
	void callBackFromAlertClose(LayerPopupAlert* sender, int tag);
	//update 21/7
	string bossName = "";
	//update 25/7
	void checkShowSetting();
	int totalTurnTime = 90;
	void initPanelPos();
};

#endif // LayerMainGame_h__



