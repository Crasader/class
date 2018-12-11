
#ifndef __iCasino_v2__AvatarLayer__
#define __iCasino_v2__AvatarLayer__

#include <iostream>

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"

#include "_Chat_.h"
#include "ImageDownloader.h"
#include "../layers/LayerPopupCuaHang.h"

#include "CircleAvatar.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;

#define LOCAL_ZORDER_CIR_AVATAR 100
#define LOCAL_ZORDER_SP_SAME_IP 101
#define LOCAL_ZORDER_SP_EMOTION 102
#define LOCAL_ZORDER_LB_BI_BAO 103

#define tag_STRING_CHAT 2201
#define tag_EMOTION 2202

class Avatar : public Layer
	, public PlayerCallBack
{
public:
    Avatar(bool isMe);
	~Avatar();

	static Avatar* create(bool isMe);

    Size getSizeThis();

	void hideLayerInvite();
	void showLayerInvite();
    void setVisibleLayerInvite(bool isShow);

    void setIcon(string url);
    void setFlag(bool isShow);
    void setName(string name);
	void setMoney(const long long& money);
    void setMoney(string money);
	void setMoney(int money);
	void setMoney(double money);
    void setReady(bool isReady);

    void setAI(string ai, bool forguess = false);
    const std::string& getAI() const;

    const std::string& getIp() const;

    virtual void setMeIsBoss( bool isBoss );

	void setPosition(float x, float y);
	void setPosition(Vec2 vPos);

	void setLbMoneyBet(string text);
	Label* getLbMoneyBet();

	ImageView* getFlag();
	void setBlinkAvatar();

    void startTimer(const float& duration_ = 15.0);
    void stopTimer();

	void showFlagIsMe();
	void hideFlagIsMe();

    void showSameIP(bool state);
    virtual void showChat(string& mess);

	bool isStartedGame();

    void callbackTimer(Node *pSender);
    string convertMoney(int money);
	string convertMoneyFromDouble(double money);
	string convertMoneyFromDouble_Detail(double money);

	void setCanTouch(bool _cantouch);

    //!Level
	void SetLevel(const int& level);
	void SetLevel(const std::string& level);
	int& GetLevel();
	Label* GetLabelLevel();
	void StartAnimExp(const std::string& exp);
	void StartAnimExp(const int& exp);

	virtual void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void setCallBackTimer(Ref *target, SEL_CallFunc listener);

	void runAnimationWarning(float dt);

    virtual void ResetAll();
    void SetTaoLaBot(bool boss);

    void setPlayerRegist();

    bool forGuess;

    //
    void DisplayEmotion(Chat* emo);

    void showIconLeaveGame(bool state = false);
	void processButtonRechardTouched(Ref *pObject, Widget::TouchEventType pType);
	void loadImgIconRechardMe();
protected:
    virtual bool init();
    void OnAvatarTouches(Ref *pObject, Widget::TouchEventType pType);

protected:
	int const DELAY_TIME = 100;
    enum
	{
        tagIcon = 0,
        tagInvite
    };

    float _scaleTextName;
    bool isMe;
	bool meIsBoss;
	bool isCanTouch;

    bool taolabot;

    string aI;
	string urlAvatar;
    Size sizeThis;
    Size sizeIcon;

    ProgressTimer *timer;
    Layer   *layerWidget;
    Layer   *layerInvite;
    ImageView *bg;
    ImageView *icon, *flag;
    Label *name, *money;
    Label *lblWaiting;
	Sprite *flagIsMe;
    Button* icc;
	Sprite* iconMoney;
    Sprite* iconLeaveGame;

	//! Level
	int n_Level;
	Label* p_Level;
	//current EXP
	int mCurrentExp;

	///callback timer
	SEL_CallFunc mCallBackListener;
	Ref *mCallBack;

    CircleAvatar* circleAvatar;

    //La nguoi choi
    bool isPlayerMe;

    int _vipTypePlayer;

    // Ip
    std::string sIp;

    Sprite* spSameIP;
	void onAvatarCallback();
};


class AvatarChanNormal : public Avatar{
public:
    AvatarChanNormal();
    ~AvatarChanNormal();
    CREATE_FUNC(AvatarChanNormal)

    void ShowBiBao();
    void HideBiBao();
    void ResetAll();

protected:
    Sprite* _bibao;
    bool init();
};

class AvatarChanIsMe : public AvatarChanNormal{
public:
    AvatarChanIsMe();
    ~AvatarChanIsMe();
    CREATE_FUNC(AvatarChanIsMe)
    void setName(string name);

    void setMoney(string pMoney);
    void setMoney(const long long& money);
    void setMoney(int money);
    void setMoney(double money);
protected:
    bool init();
};

#endif /* defined(__iCasino_v2__AvatarLayer__) */
