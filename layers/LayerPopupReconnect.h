#ifndef LayerPopupReconnect_h__
#define LayerPopupReconnect_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;

class LayerPopupReconnect : public Layer,
	public	PlayerCallBack
{
public:
	LayerPopupReconnect();
	~LayerPopupReconnect();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	virtual bool init();
	CREATE_FUNC(LayerPopupReconnect);

	void showReconnect();
protected:
	void OnSmartFoxConnectionResume(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxHandShake(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	float timeAutoReconn = 6;
	float timeAutoReconnBuff = 1;
	int countReconn = 0;
	int maxCountReconn = 2;
	int timePrepare;//thoi gian thu reconnect cho lan thu 2 - mac dinh 8s
	Vec2 startPos;
	float startPosX;
	string firstStr;

	cocos2d::EventListener* _touchListener;
	void scheduleTimeReconnect(float dt);
	Text* lblHeader, *lblContent1, *lblContent2, *lblContent3, *lblTime, *lblCount;
	ui::Button* btnClose, *btnOK, *btnCancel;
	LoadingBar* loadingBar;
	Sprite* lightLoading, *bgLoading;

	void onButtonClose(Ref* pSender);
	void onButtonCancel(Ref* pSender);
	void onButtonOK(Ref* pSender);
	void hideAllLabel(bool isHide);

	void showConnecting();
	void showCannotConnect();

	void scheduleCheckConnectting(float dt);
};


#endif // LayerPopupReconnect_h__

