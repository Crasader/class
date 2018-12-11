#ifndef __iCasino_v2__SceneManager__
#define __iCasino_v2__SceneManager__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocosbuilder/CocosBuilder.h"
#include "SimpleAudioEngine.h"

#include "layergames/LayerChatWindow.h"
#include "layers/LayerNotification.h"
#include "layers/LayerPopupInvite.h"
#include "layergames/CCProgressLayer.h"

#include "FileDownloader.h"
#include "ImageDownloader.h"
#include "Common.h"

#include "PromotionHandler.h"

#include "GameServer.h"
#include "SingLeton.h"
#include "AllData.h"

#include "Entities/Invitation/SFSInvitation.h"
#include "Entities/Invitation/Invitation.h"

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif
#endif

#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//#include "App42API.h"
#endif

using namespace Sfs2X;
using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocosbuilder;
using namespace CocosDenshion;

enum eSceneTags
{
	tag_SceneLogin = 1000,
	tag_SceneMain,
	tag_SceneGaming,
	tag_SceneNotification,
	tag_SceneLoading
};

enum eLayerZOrders
{
	zorder_SceneLogin = 0,
	zorder_SceneMain,
	zorder_SceneGaming,
	zorder_SceneLoading,
	zorder_SceneNotification = 1000
};

enum eCheckConnStep
{
	tagGetLevel1Info = 0,
	tagConnLevel1,
	tagUploadData1,
	tagGetLevel2Info,
	tagConnLevel2,
	tagLoginedWithLevel1,
	tagLoginedWithLevel2,
	tagUploadData2,
	tagConnLevelDefault
};

enum typeShowHideLayer
{
	None,
	fromLeft,
	fromRight,
	toLeft,
	toRight
};

struct FriendInfo{
	string uid;
	string name;
	string url_picture;
    bool isInvite;
};

struct NotificationInfo{
	string linkShow;
	bool isClick;
	string textShow;
	string linkPop;
	string textPop;
	bool isShow;
};
///MINHDV - struct info gameconfig
struct strGameCF{
	int gameid;
	string list_cuoc;
	int default_hide;
	int ratioBet;
	strGameCF(int _id, string _list_cuoc, int _default_hide, int _ratiobet) : gameid(_id), list_cuoc(_list_cuoc), default_hide(_default_hide), ratioBet(_ratiobet){};
};
//MINHDV - type pickroom - 0: dạng bàn chơi, 1: dạng mức cược
enum TableType{
	PlayTable,
	BetTable
};

class SceneManager :
	public Layer,
	public SingLeton<SceneManager>,
	public PlayerCallBack
{
private:
	//	FileDownloader* gFileDownloaderProcess;
	vector<string> gListIpAddress;
	//FileDownloader* gFileDownloaderProcess;
	ImageDownloader* gImgDownloadProcess;

	CCProgressLayer* mLayerLoading;
	eCheckConnStep mCurrStepCheckConn;
	string ipv6 = "";
	int mCurrIpIndex;
	//MINHDV
	string strGameConfig;
public:
	vector<NotificationInfo> gListNotification;
	CC_SYNTHESIZE(PromotionHandler, mPromotionHandler, PromotionHandlers);
	CC_SYNTHESIZE(string, sListPin, ListPin);
	CC_SYNTHESIZE(string, sUrlPromotion, UrlPromotion);
	CC_SYNTHESIZE(string, sUrlNotifcation, UrlNotifcation);
	CC_SYNTHESIZE(int, nGameID, GameID);
	CC_SYNTHESIZE(RejoinInfoStruct, mRejoinInfo, RejoinInfo);
	CC_SYNTHESIZE(string, mIP, MIP);
	CC_SYNTHESIZE(vector<PromotionInfo>, lstPromotionInfo, LstPromotionInfo);
	CC_SYNTHESIZE(bool, isAuto, AutoCreateRoom);//bien de kiem tra de tu dong gui loi moi
	CC_SYNTHESIZE(bool, isKick, KickRoom);//bien de kiem tra xem co phai do bi kich khoi phong khong?
	CC_SYNTHESIZE(bool, isKickMoney, KickNem);//bien de kiem tra xem co phai bi kich do het tien hay khong
	CC_SYNTHESIZE(double, betvl, BetValue);//bien de lay gia tri cuoc cua phong join cuoi cung gan nhat
	CC_SYNTHESIZE(string, listbet, ListBet);//bien de luu lai mang muc cuoc dai gia, trieu phu... chi lay 1 lan duy nhat
	CC_SYNTHESIZE(int, typePR, TypePickRoom);//bien de switch kieu pick room dang ban choi hay muc cuoc
	CC_SYNTHESIZE(int, gameVer, GameVersion);//bien de switch cac phien ban game bai
	CC_SYNTHESIZE(vector<string>, _inviteUserIdxs, InviteUserIdxs);
	CC_SYNTHESIZE(vector<FriendInfo>, _inviteableUsers, InviteableUsers);
    CC_SYNTHESIZE(bool, isFlagStore, IsFlagStore);//bien de kiem tra xem co bat co an ko

	void pushInviteableUsers(FriendInfo info);
	void earseInviteUserIdxs(int idx);
    void earseInviteableUsers(int idx);
    void pushInviteUserIdxs(string uid);
    void setInviteInviteableUsers(int idx, bool isInv);
    void clearInviteableUsers();
    void clearInviteUserIdx();
    string getCoin(int type);
	//MINHDV - getconfig from sever
	void setGameConfig(string _config);
	string getGameConfig();
	CC_SYNTHESIZE(vector<strGameCF>, arrConfig, ArrConfig);
	vector<strGameCF> getConfigFromSever(string _stringcfg);

	//////////////////////////////////////////////////////////////////////////
	ImageDownloader* getImageDownloadProcess(){
		return gImgDownloadProcess;
	}

	virtual bool init();

	CREATE_FUNC(SceneManager);

	SceneManager();
	~SceneManager();

	static SceneManager& getSingleton(void);
	static SceneManager* getSingletonPtr(void);

	cocosbuilder::NodeLoaderLibrary* getNodeLoaderLibrary()
	{
		return new NodeLoaderLibrary();
	}

	LayerChatWindow* getLayerChatWindow(){
		return NULL;
	}
	void showLayerChatWindow(){

	}
	void hideLayerChatWindow(){}
	LayerNotification* getLayerNotification(){
		return NULL;
	}
	void hideNotification(){

	}
	bool showNotification(){
		return true;
	}

	void nextStepConn();

	void showChatWithCurrentScene(string msg, int _id);

	void getPromotionFromUrl();
	void getNotificationFromUrl();
	void getStatusEventFromUrl();
    
    void getInfoShareFromUrl();

	void showLoading(const float& timeOut = 10);

	void hideLoading();
	//void setTimeOutLoading(float dt);
	void setCallBackLoading(Ref* target, SEL_CallFunc listener);
	void setLoadingTag(int tag);
	void setLoadingString(string msg);
	int getLoadingTag();

	virtual void onEnter() override;
	virtual void onExit() override;

	void gotoLogin();
	void gotoMain();
	void gotoPickGame(int gameID);
	void gotoChonBanChoi(int gameID);
	void gotoGame(int typeGame);



	string getMyName();

	void loadString();

	void reloadChargeConfig();

	void connect2Server(float dt);
	void disconnectFromServerDelay(float dt);

	void disconnectFromServer();

	void onFileDownLoaded(Node* pSender, void* pResponse);
	void uploadData2Server(string url);
	void checkIpLevel2(string url);

	void updateEvent(float dt);
	void updateEvent4CheckConnection(float dt);
	void timeoutLoadingCallback();
    
    int getCapVip();

	//
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxConnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxDisconnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSocketError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxConnectionRetry(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxConnectionRetryFail(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxConnectionResume(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomFindResult(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxBuddyListInit(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxBuddyOnlineStateUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxLogin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxLoginError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxInvitation(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	//
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void SetKieuBanChan(const int& kieuban);
	const int& GetKieuBanChan() const;
	bool isLogout = false;
	bool rcForLogin = false;
    bool isLogin = false;
	void onScheduleGetConfig(float dt);
    void onScheduleGetHuTHuong(float dt);
	int getLoginBonusNotFB();

	int getLoginBonus();
	int getFacebookBonus();

	int getMinVipTranfer();
	int getMinMoneyTranfer();
	int getMinMoney(int id);

	//MINHDV - get notification on scene pickroom
	vector<NotificationInfo> gListNotificationPickRoom;
	void getNotificationPickRoomFromUrl();
	void getVersionFromUrl();
	void getbtnRechargeFromUrl();
	void getUrlAvatar();
	void getButtonNotificationFromUrl();
	string versiontohide;
	string getCoinUnit();
    
    double getMoneyByType(int type);
    double getSoTienNap();
    
    void nhanthuongLevel();

	void sendEventEndGame();

	struct event{
		string name;
		string link;
	};
	vector<event> lstEvent;
	vector<event> lstbtnNoti;
	vector<event> lstTabRecharge;
	vector<string> arrLink;//limit 8 links
	struct chatspam{
		int count;
		string txt;
	};

	vector<chatspam> arrCurrentChat;

	int chatcounttime = 0;
	int timerchat = 0;
	int sec = 60;
	int winMoney = 0;
    
    int currentGameEnd = 0;

	//fake scene
	void filterIP();
private:
	void changeBuddyGameState(string gameState);
	int pKieuBanChan;
	//fake scene
public:
	void initFirst();
	CC_SYNTHESIZE(bool, isBotZone, IsBotZone);//update for login to bot zone ba cay
    //only for co tuong, co up
	CC_SYNTHESIZE(bool, filterChat, FilterChat);///on off filter chat trong game
	CC_SYNTHESIZE(int, betzone, BetZone);//current zone in list bet value - received from server config
	CC_SYNTHESIZE(int, betFilter, BetFilter);//danh dau dang o tab room nao
	CC_SYNTHESIZE(int, currRoomType, CurrRoomType);//danh dau dang o ban loai gold hay silver

    CC_SYNTHESIZE(bool, isChuyenTab, IsChuyenTab);
    CC_SYNTHESIZE(bool, isShowTX, IsShowTX);
    CC_SYNTHESIZE(bool, isShowPK, IsShowPK);
    CC_SYNTHESIZE(bool, isShowXD, IsShowXD);
    CC_SYNTHESIZE(bool, isShowVQ, IsShowVQ);
    CC_SYNTHESIZE(bool, isShowVQBonus, IsShowVQBonus);

    CC_SYNTHESIZE(bool, isShowCT, IsShowCT);
    CC_SYNTHESIZE(bool, currScene, CurrScene);
    CC_SYNTHESIZE(bool, isShowTXDay, IsShowTXDay);
    

    
	void showPopupMoneyCharge();
    void showPopupVongQuay();
	void showToast(string _msg);
    void showPopupNapTien(string _msg);
    void showPopupNapTienGold(string _msg);

	void showToast(string _msg, float _duration);
	void showPopupNotify(string messageText);
	void clearListBet();
    int currLang = 1; // 1 là Việt NAM, 2 là Campuchia, 3 là English
    string getFolderLang();
    string getFolderLangWeb();
    void runEvent(int dieukien);
    bool isEndGame;
    bool showCuoiVan;
    bool isShouldAuto;
    bool isShowEvent;
    bool isNotify;
    int moneyNotify = 0;
    int huPoker = 0;
    int huTaiXiu = 0;
    int huCaoThap = 0;
    int currHuTaiXiu = 0;
    int currHuPoker = 0;
    int currHuCaoThap = 0;
    int currHuSlot1 = 0;
    int currHuSlot2 = 0;
    int currHuSlot3 = 0;

    bool isInGame = false;
    void onScheduleFakeHuThuongTaiXiu(float dt);
    void onScheduleFakeHuThuongPoker(float dt);
    void onScheduleFakeHuThuongCaoThap(float dt);
    
    int winTX = 0;
    int winDoDen = 0;
    int winCT = 0;
    int winPK = 0;
    int winVQMM = 0;
    int totalTX = 0;
    int currRoomTypeTX = 0;
    int currRoomTypePK = 0;
    int currRoomTypeCT = 0;
    int currRoomTypeDD = 0;

    void sendEventNotify(string msg);
    void setLastGameID(int gameID);
    int getExchangeRate(int type,int menhgia);
    string getBankAccountByType(int type);
    void flagStore(string listFlagStore);
    int numPopup = 0;
    
    bool isNagaCard = true;
    bool isNagaWin = false;
    bool isNagaNew = false;
    bool isMegaWin = false;
    string tokenFireBase = "";
    string screenNotify = "";
    
    
    void getConfigStore();
    
    void showPopupFailed(int tag);
    void layerPopupThongBaoCallBack2(Ref* pSender);
    
    bool isGetConfig = false;
};

#endif /* defined(__iCasino_v2__SceneManager__) */
