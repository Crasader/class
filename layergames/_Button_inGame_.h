
#ifndef __iCasino_v2___Button_inGame___
#define __iCasino_v2___Button_inGame___

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Requests/ExtensionRequest.h"
#include "../LayerChatInGame.h"
#include "../Common.h"
#include "GameServer.h"
#include "../layers/BagItem.h"
#include "../layers/LayerSettingInGameMain.h"
#include "../layers/LayerMenuNhiemVuInGame.h"
#include "layers/LayerPopupMenuInGame.h"
#include "../layers/LayerPopupThongBao.h"
#include "../layers/LayerPopupInvite.h"
#include "BangCuocMauBinh.h"

USING_NS_CC;
using namespace cocos2d::ui;

#define TAG_LB_BTN_BACK 101
#define TAG_LB_BTN_SETTING 102
#define TAG_LB_BTN_CHAT 103
#define TAG_LB_BTN_COIN 104
#define TAG_LB_BTN_MISSION 198
#define TAG_POPUP_MENU 24609

class LayerButtonInGame : public Layer,
	public PlayerCallBack
{
private:
	int gameID;
	bool isGuess;
	bool isLeave;
	Size sizeAdd;
	SEL_CallFuncO mCallBackListener;
	Ref *mCallBack;
	string quitMess;
	bool isChauRia;//bien luu lai de set cho popupMenu
	LayerPopupMenuInGame *popupButtonIngame;

	void initBetCuoc();
	void layerPopupThongBaoCallBack(Ref *pSender);
	bool isStartedGame();
	void createLayerNotification(string title, string content_mess, int tag);
	void event_EXT_EVENT_GAME_LEAVE_RES(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_REGISTER_QUEUE_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_SRVNTF_PLAYER_LIST(boost::shared_ptr<ISFSObject> param);
    void event_EXT_EVENT_USER_LEAVE_NOTIF(boost::shared_ptr<ISFSObject> param);
	void onButtonHelp(Ref* pSender, Widget::TouchEventType pType);

	string _filename;
	Button* btnCapture;
	Button *btnInvite;
	Button *btnListCai;
	void onCaptured(cocos2d::Ref*);
	void afterCaptured(bool succeed, const std::string& outputFile);

public:
	LayerButtonInGame();
	~LayerButtonInGame();
	virtual bool init();
	CREATE_FUNC(LayerButtonInGame);
	Button* createButtonWith_Style_Position(int style, Point pPoint);
	void addWidgetEx(Button* node);

	// add event listener
	void processDisplayButtonInvite(bool _display);
	void processButtonInviteTouched(Ref *pObject, Widget::TouchEventType pType);
	void processButtonExitGameTouched(Ref *pObject, Widget::TouchEventType pType);
	
	void eventTouchBtnBack(Ref *pObject, Widget::TouchEventType pType);
	void eventTouchBtnSetting(Ref *pObject, Widget::TouchEventType pType);
	void eventTouchBtnChat(Ref *pObject, Widget::TouchEventType pType);
	void eventTouchBtnCoin(Ref *pObject, Widget::TouchEventType pType);
	void eventTouchBtnSetting_Chan(Ref *pObject, Widget::TouchEventType pType);
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void eventTouchBtnMenu(Ref *pObject, Widget::TouchEventType pType);
	void BackToChooseDesk();

	Button* getButtonListCai();

	//Callback
	void callbackFromChatWindow(Node*, void*);

	Button *getButtonByTag(int tag);
	void hideButtonInChanGame();
	void SetIsGuess(const bool& guess = false);
	void setCallBackFromButton(Ref* target, SEL_CallFuncO listener);

	LayerMocChatInGame* layerChatInGame;
	BagItem* bagItem;
	LayerSettingInGameMain* layerSettingInGameMain;
	//LayerMenuNhiemVuInGame* mButtonNhiemVu;

	Sprite* signalNetwork;

	LayerMocChatInGame* getLayerChat() const;
	void showToast(string mess);
	void setMessageNotification(string mess);
	void setBaoSam(bool _isbao);
    void loadCuoc(int cuoc1, int cuoc2, int cuoc3);
    void resetBangCuoc(int cuoc1, int cuoc2, int cuoc3);
    void hideBangCuoc();
    BangCuocMauBinh* bangcuocingame;
	//
	void SetPosLayerSetting(const Vec2& pos);
	void ResetStatus();
	Label* lblPing;
};

#endif /* defined(__iCasino_v2___Button_inGame___) */
