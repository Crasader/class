#ifndef ItemMenuGame_h__
#define ItemMenuGame_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../layergames/ThreadQueueMsg.h"
#include "../GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
class ItemMenuGame : public Layer,public PlayerCallBack
{
public:
	ItemMenuGame();
	~ItemMenuGame();

	virtual void onEnter() override;
	virtual void onExit() override;
    void updateEvent(float dt);
    void OnExtensionResponseProcess();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void event_EXT_EVENT_START();
    void event_EXT_EVENT_END();
    void event_EXT_EVENT_GAME_STATE_NTF();
    
    void UpdateTimeStartGame();
    void updateTimer(float dt);
    
    void StopMyTimer();
    void animationTLMN();
    void animationSLot();
    void animationTX();
    void animationCatte();

    void CallBackResult(Ref *pSend);
    void CallBackFromAnim();
    void showResultAfter();
    void showResultGame(string money, string result);
    void showResult();
    void playAnimationCatte1();
    void playAnimationCatte2();
    void playAnimationBaDoi1();
    void playAnimationBaDoi2();
    void playAnimationBaDoi3();
    void playAnimationMB1();
    void playAnimationMB2();
    void playAnimationMB3();
    void playAnimationTCC();
    void showXD();
    void showTXBig();
    string GetUrlXiNgau(int number);
    void showPrevMatchHis(string str);
    void animationBaDoi();
    void setGameID(int gameID, bool state);
    void onBtnGame(Ref* pSender, ui::Widget::TouchEventType pType);
    void onBtnGameOpen(Ref* pSender);
    void onBtnSticker(Ref* pSender);

    void gotoGameMini(int tag);
    void animTXBeforeLogin();
    void setShouldTouch(bool isTouch);
    void animationMB();
    void updateHuTX(float dt);
    void updateHuSlot(float dt);
    void hideButtonPlayTCC();
    void scheduleBubble(float dt);
    void reloadLang();
    void isLogon();
    void isLogout();
    
    
    ///
    void showNewGame(int gameID, vector<int> listNewGame);

    
    virtual bool init();
    CREATE_FUNC(ItemMenuGame);

private:

    vector<ImageView*> lstCard;
    vector<ImageView*> lstCardCatte;
    vector<ImageView*> lstCardBaDoi;
    vector<ImageView*> lstCardMB;
    Vector<SpriteFrame*> animFrames;
    Layout* pnlTLMN;
    Layout* pnlSlot;
    Layout* pnlTX;
    Layout* pnlTXBig;
    Layout* pnlCatte;
    Layout* pnlBaDoi;
    Layout* pnlMB;
    Layout* pnlXD;
    Layout* pnlTCC;
    Layout* pnlNewGame;
    
    ImageView* imgAnim;
    ThreadQueueMsg* mQueueMsg;
    int PlayTurnTime = 0;
    int TimerCountDown = 0;
    int timerToHide = 3;
    boost::shared_ptr<ISFSObject> param;
    
    ImageView* imgKQ;
    Text* txtKQ;

    ImageView* imgdice1;
    ImageView* imgdice2;
    ImageView* imgdice3;
    ImageView* imgBat;
    ImageView* imgTimer;
    ImageView* imgGameName;
    Text* huTX;
    Text* huSlot1;
    Text* huSlot2;
    Text* huSlot3;


    Text* txtTimer;
    string rg = "";
    int kGameVQMM = 132;
    int kGameMNPK = 130;
    int kGameXDMN = 134;
    int kGameCT = 129;
    int kGameTX = 128;
    
    Layout* pnlDisable;
    Layout* pnlActive;
    bool shouldTouch = true;
    Sprite* sprite;
    Sprite* sprCua;
    Sprite* sprCa;
    Sprite* sprTom;
    Sprite* sprBubble;
    bool isNoOpen = false;
    bool _isLogon = false;
    bool _isLoginFromLogout = false;
    
public:
    int gameID = 0;
    Layout* pnlEvent;
    Sprite* sprStick;
    Text* txtEvent;
    string urlSticker;

};
#endif // ItemMenuGame_h__
