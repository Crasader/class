#ifndef __iCasino_v2__SceneMain__
#define __iCasino_v2__SceneMain__

#include "cocos2d.h"
#include "../Common.h"
#include "cocos-ext.h"
#include "network/HttpClient.h"

#include "../GameServer.h"

#include "../layers/LayerChatRoom.h"
#include "../layers/LayerFriend.h"
#include "../layers/LayerRank.h"
#include "../layers/LayerMailInbox.h"
#include "../layers/LayerPopupThongBao.h"
#include "../layers/LayerMainTop.h"
#include "../layers/LayerMainBottom.h"
#include "../layers/LayerMainContent.h"
#include "../layers/LayerMainContent2.h"

#include "../layers/LayerNotification.h"
#include "../SingLeton.h"
#include "ScenePickRoom.h"
#include "SceneGame.h"
#include "Requests/JoinRoomRequest.h"
#include "../layers/LayerChatInScenePickRoom.h"
USING_NS_CC;
USING_NS_CC_EXT;

class SceneMain : public cocos2d::Layer
, public PlayerCallBack, public SingLeton<SceneMain>

{
public:
	SceneMain();
	~SceneMain();

	static cocos2d::Scene* createScene();

	virtual bool init()override;
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)override;
    virtual void onEnter() override;
    virtual void onExit() override;
    static SceneMain& getSingleton(void);
    static SceneMain* getSingletonPtr(void);

	CREATE_FUNC(SceneMain);

	void gotoPickGame();
	void gotoChargeMoney();
	void gotoRank();
	void gotoNotifcation();
	void gotoFriend();
	void gotoMailInbox();
	void gotoSettingMain();
	void onButtonBackPress();
	void gotoChatRoom();
	void gotoPopupSendMessage();
    void gotoRuongVip();
	void reloadChargeConfig();
    void gotoEvent();
    void goToDiemDanh(float dt);
    void gotoReconnect(float dt);

	void onCallbackInvitePopup(bool isAccept);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)override;
	void onPopupCallBack(Ref *pSender);
	void showPopupThongbao();
    void showEvent();
    void gotoEventAfter(float dt);
    void reloadData();
    
    void timeoutChatMaxtime(float dt);
    void scheduleTXMINI(float dt);
    void scheduleshowTXMINI(float dt);
    void gotoGuideGame(float dt);
    void startTimeOutChat();
    void reloadLang();
    void logInDone();
    void logOut();
    
    void goToMain();
    void goToPickRoom(int gameID);
    void gotoGameByID(int gameID);
    
    void prepareToPickGame();
    void setChatting();
    void updateChating(float dt);
    void removeAllTag();
    void gotoDiemDanh();
    
	//MinhDV changes 5/4/2016
	void showToast(string mess);
    int const TAG_BOTTOM = 190;
    int const TAG_TOP = 191;
    bool isNoEvent = false;

protected:
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	void OnSmartFoxInvitation(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	void OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

private:
	LayerMailInbox* _layerMailInbox;
	LayerRank* _layerRank;
	LayerNotification* _layerNotification;
	LayerFriend* _layerFriend;
	LayerChatRoom* _layerChat;

	bool isPopupShow;
	int tagChild;
	int const TAG_CHILD = 111;
	bool isRejoinRoom;
    bool isShowMatKhau;
	int mCurrentRoomIDJoin;
public :
    LayerMainTop* mLayerMainTop;
    LayerMainBottom* mLayerMainBottom;
    LayerMainContentNew2* mLayerMainContent;
    ScenePickRoom* layerPickRoom;
    SceneGame* layerGame;
    
    int currentLayer;
    int typelayerLobby = 1234;
    int typelayerPickRoom = 1235;
    int typelayerGame = 1236;
    int typelayerLogin = 1237;

    int tagMainContent = 234;
    int tagMainTop = 235;
    int tagMainBottom = 236;
    int tagMainPickRoom = 237;
    int tagGame = 238;
    int tagLogin = 239;
    bool isMain = false;
    int currTypeMoneyGo = 0;
    bool chatting = false;
    string tokenFireBase = "";
    LayerChatInScenePickRoom* chat;
    
};
#endif // __iCasino_v2__SceneMain__

