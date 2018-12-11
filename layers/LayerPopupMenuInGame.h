#ifndef LayerPopupMenuInGame_h__
#define LayerPopupMenuInGame_h__
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Requests/ExtensionRequest.h"
#include "../LayerChatInGame.h"
#include "../Common.h"
#include "GameServer.h"
#include "../layers/BagItem.h"
#include "../layers/LayerSettingInGameMain.h"
#include "../layers/LayerPopupThongBao.h"
USING_NS_CC;
using namespace cocos2d::ui;

#define TAG_LB_BTN_BACK 101
#define TAG_LB_BTN_SETTING 102
#define TAG_LB_BTN_CHAT 103
#define TAG_LB_BTN_COIN 104
#define TAG_POPUP_MENU 24609

class LayerPopupMenuInGame : public Layer,
	public PlayerCallBack
{
private:
	bool isLeave;
	Button* btnRoiBan;
	bool isBaoSam;
	LayerPopupWebView *popupWebview;
	bool isChauRia;//danh cho chau ria chan va phom - dang ki roi ban cho bay luon
	void onButtonRoiBan(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonRoiBanNhanh(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonHuongDan(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonCaiDat(Ref* pSender, ui::Widget::TouchEventType type);

	EventListenerTouchOneByOne* listener;
public:
	LayerPopupMenuInGame();
	~LayerPopupMenuInGame();
	virtual bool init();
	CREATE_FUNC(LayerPopupMenuInGame);
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void showMenu();
	void hideMenu();
	void event_EXT_EVENT_LEAVE_GAME_RES(boost::shared_ptr<ISFSObject> ptrEvent);
	void setBaoSam(bool _isbao);
	bool getBaoSam();
	bool getLeave();
	bool getChauRia();
	virtual void onEnter() override;
	virtual void onExit() override;
    void ResetStatus();
	void setChauRia(bool _isChauRia);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
};

#endif // LayerPopupMenuInGame_h__


