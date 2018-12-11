#ifndef __iCasino_v2__LayerPopupInviteGame__
#define __iCasino_v2__LayerPopupInviteGame__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include <string>
#include "../SceneManager.h"
//#include "../layergames/ChanUtils.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d::ui;

class LayerPopupInvite : public cocos2d::Layer, public	PlayerCallBack
{
public:
	enum{
		tagPickGame = 1,
		tagSceneMain = 2
	};
	LayerPopupInvite();
	~LayerPopupInvite();

	//bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	//void onButtonAccept(Ref* pSender);
	//void onButtonDeny(Ref* pSender);
	//void onButtonDenyAll(Ref* pSender);

	//void back2PickGame(bool denyAll);

	//void setDatas(string userID, long userMoney, string roomName, int gameID, string pass, long betValue, string players, int roomID);
	//void showFakeInfo(int gameID);// hien thi thong tin moi choi dang fake
	//void autoClosePopup(float dt);

	virtual bool init();
	CREATE_FUNC(LayerPopupInvite);
	CC_SYNTHESIZE(int, nCurrScreen, CurrScreen);
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void processButtonOkTouched(Ref* pSender, ui::Widget::TouchEventType type);
	void processButtonExitTouched(Ref* pSender, ui::Widget::TouchEventType type);
	//void setClickCallBack(const std::function<void()> &listener);
private:
	//cocos2d::EventListener* _touchListener;

	//Text* lblUserName1;
	//Text* lblUserName2;
	//Text* lblGameName;
	//   ImageView* imgCoin;
	// 	Text* lblRoomID;
	// 	Text* lblBetValue;
	// 	Text* lblPlayers;
	/*Text* lblGold;
	ImageView* avtVip;
	int gameID;
	int mRoomID4Invite;
	string mPassRoom4Invite;
	string mCurrGroupList;
	std::function<void()> listener;*/
	Text* lblError;
	TextField* tfUserInvite;
	Button* btnOk;
	Button* btnExit;
};

class LayerPopupShowInvite :public Layer, public PlayerCallBack{
public:
	LayerPopupShowInvite();
	~LayerPopupShowInvite();
	virtual bool init();
	CREATE_FUNC(LayerPopupShowInvite);

	std::string getGameNameById(int _gameId);
	double getMinBetByGame(int _gameId, double _amf);
	void processSetDatasToDisplay(std::string _user, std::string _passRoom, double _betRoom, int _typeRoom, int _gameId, int _roomType,string roomName);
	void processButtonExitTouched(Ref* pSender, ui::Widget::TouchEventType type);
	void processButtonOkTouched(Ref* pSender, ui::Widget::TouchEventType type);
private:
	Text *lblUserInvite, *lblGameName, *lblBetRoomInvite;
	ImageView *imgCoin;
	Button *btnOk, *btnExit;
	int roomID = -1;
	double minBet;
	int roomType;
    int gameID = 0;
    string roomName = "";
	std::string passRoom;
};
#endif //__iCasino_v2__LayerPopupInviteGame__

