#ifndef LayerPopupInfo_h__
#define LayerPopupInfo_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameServer.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../layergames/CircleAvatar.h"
#include "../cotuong/gameUtils.h"
#include "LayerPopupCuaHang.h"
#include "LayerPopupDoiMoc.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

class  LayerPopupInfo :
	public Layer,
	public PlayerCallBack
{
public:
	LayerPopupInfo();
	~LayerPopupInfo();

	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;

	void onBtnClose(Ref* pSender);
	void setFriendID(string friendID);
	void setAvatarImage(Sprite* sprite);
	void setAvatarUrl(string url);
	void reloadAllDatas();
	void setIsBossRoom(bool isBoss);
	void onButtonInviteFriend(Ref* pSender);
	void onButtonUnfriend(Ref* pSender);
	void onButtonKick(Ref* pSender);
	void setIsPlaying(bool isPlaying);
	bool getIsPlaying();
	void hideBtnKick();
	void setMoney(double money);//chi danh cho game quay thuong
	void setUserIP(string ip);

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	CREATE_FUNC(LayerPopupInfo)
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void processButtonNapMocTouched(Ref *pObject, Widget::TouchEventType pType);
	void processButtonNapHaoTOuched(Ref *pObject, Widget::TouchEventType pType);
protected:
	Text* labelName;
	Text* labelGender;
	Text* labelMoney;
	Text* labelLevel;
	Text* labelIP;
    Text* labelSilver;
	Sprite* spriteNodeAvatar;
	string mFriendID;
	bool isPlaying;
	Button* btnAddFriend;
	Button* btnKick;
	Button* btnUnfriend;
	EventListenerTouchOneByOne* listener;
	LoadingBar* progressLevel;
	ImageView* imgVip;
	int gameID;
	CircleAvatar* circleAvatar;

	string formatIP(string _ip);
private:
	Button *btnNapMoc;
	Button *btnNapHao;
};

class PopUpInfoInChan : public LayerPopupInfo{
public:
	PopUpInfoInChan();
	~PopUpInfoInChan();
	CREATE_FUNC(PopUpInfoInChan)

		void setIsBossRoom(bool isBoss);
	void setTypeVip(const int& vip);

protected:
	bool init();
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

private:
	bool _meIsBoss;
	int _vip;

};

#endif // LayerPopupInfo_h__
