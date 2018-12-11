#ifndef __LayerPopupInviteGame__
#define __LayerPopupInviteGame__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../SceneManager.h"
#include <string>
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d::ui;

class LayerPopupInviteGame : public cocos2d::Layer
{
public:
	enum{
		tagPickGame = 1,
		tagSceneMain = 2
	};
	LayerPopupInviteGame();
	~LayerPopupInviteGame();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void onButtonAccept(Ref* pSender);
	void onButtonDeny(Ref* pSender);
	void onButtonDenyAll(Ref* pSender);

	void back2PickGame(bool denyAll);

	void setDatas(string userID, long userMoney, string roomName, int gameID, string pass, long betValue, string players, int roomID);
	void showFakeInfo(int gameID);// hien thi thong tin moi choi dang fake
	void autoClosePopup(float dt);

	virtual bool init();
	CREATE_FUNC(LayerPopupInviteGame);

	CC_SYNTHESIZE(int, nCurrScreen, CurrScreen);
	
	void setClickCallBack(const std::function<void()> &listener);
private:
	cocos2d::EventListener* _touchListener;

	Text* lblUserName1;
	Text* lblUserName2;
	Text* lblGameName;
    ImageView* imgCoin;
// 	Text* lblRoomID;
// 	Text* lblBetValue;
// 	Text* lblPlayers;
	Text* lblGold;
    ImageView* avtVip;
	int gameID;

	int mRoomID4Invite;
	string mPassRoom4Invite;
	string mCurrGroupList;

	std::function<void()> listener;
};

#endif //__iCasino_v2__LayerPopupInviteGame__

