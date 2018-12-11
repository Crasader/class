#ifndef LayerUserPlayInfo_h__
#define LayerUserPlayInfo_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "../layergames/CircleAvatar.h"
#include "../GameServer.h"
#include "CommonChess.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;
class LayerUserPlayInfo : public cocos2d::Layer,
	public PlayerCallBack
{
public:
	virtual bool init() override;
	LayerUserPlayInfo();
	~LayerUserPlayInfo();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerUserPlayInfo);

	void setInfoUser(string _name, string _money, bool isBoss, int vipLv, int khungAva, string urlAvatar);
	void updateInfo(string _money, bool isBoss);
	void setInfoUser(string _name);
	void setBossRoom(bool isBoss);

	void resetInfo();
	void startTimer(const float& duration_ = DEFAULT_TIME_1TURN);
	void stopTimer();
	string getMyName();

	void showNumber(string number);
	void reversePos();
	void showAlertAnim(bool isShow);
	//update 25/7
	void resumeTimer(int totalTime, int currTime);
protected:
	virtual void OnSmartFoxBuddyAdd(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxBuddyError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	// 	ImageView* imgAvatar, *imgKhungAvatar;
	// 	Sprite* spVip;
	Sprite* spBoss;
	Text* lblUserName, *lblUserMoney;
	CircleAvatar* circleAvatar;
	Button* btnGold;
	string myName;
	Sprite* spTime;
	Sprite* spTimeTurn;

	void onButtonAvatar();
	void onButtonGold(Ref* pSender);
};

class LayerPopupUserInfo : public Layer,
	public PlayerCallBack
{
public:
	virtual bool init();
	LayerPopupUserInfo();
	~LayerPopupUserInfo();
	CREATE_FUNC(LayerPopupUserInfo);

	void loadData(string _userName);
protected:
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	string userName;

	Button *btnFriend;
	Text* lblLevel;
	Text* lblUserName;
	Text* lblUserMoney;
	Text* lblElo;
	Text* lblTiLe;
	CircleAvatar* circleAvatar;

	void onButtonFriend(Ref* pSender);
	void onButtonClose(Ref* pSender);
	void requestHistory();
};

#endif // LayerUserPlayInfo_h__



