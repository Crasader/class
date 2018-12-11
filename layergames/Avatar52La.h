//
//  Avatar52LaLayer.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//

#ifndef Avatar52La52La_h__
#define Avatar52La52La_h__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"

#include "_Chat_.h"
#include "ImageDownloader.h"
#include "CircleAvatar.h"
#include "../layers/LayerPopupCuaHang.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d::ui;

class Avatar52La : public Layer
	, public PlayerCallBack
{
public:
	Avatar52La(bool isMe, int typeAva);
	~Avatar52La();

	static Avatar52La* create(bool isMe, int typeAva);
	bool init();

	Size getSizeThis();

	void OnAvatarTouches(Ref *pObject, Widget::TouchEventType pType);
	void onAvatarCallback();

	void displayButtonRechard(bool _display);
	void processButtonRechardTouched(Ref *pObject, Widget::TouchEventType pType);

	void hideLayerInvite();
	void showLayerInvite();
	void setVisibleLayerInvite(bool isShow);

	void setIcon(string url);
	void setFlag(bool isShow);
	void setName(string name);
	string getUserName();
	void setMoney(const long long& money);
	void setMoney(string money);
	void setMoney(int money);
	void setMoney(double money);
	void setReady(bool isReady);
	void setAI(string ai);
	string getAI();
	void setMeIsBoss(bool isBoss);
	void setVisbleLabelGuess(bool isShow);

	void setPosition(float x, float y);
	void setPosition(Vec2 vPos);

	ImageView* getFlag();
	void setBlinkAvatar52La();

	void startTimer(const float& duration_ = 15.0);
	void stopTimer();

	void showFlagIsMe();
	void hideFlagIsMe();

	bool isStartedGame();
	bool isChauRia();

	void callbackTimer(Node *pSender);
	string convertMoney(int money);
	string convertMoneyFromDouble(double money);
	string convertMoneyFromDouble_Detail(double money);

	void setCanTouch(bool _cantouch);

	virtual void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void setCallBackTimer(Ref *target, SEL_CallFunc listener);

	void runAnimationWarning(float dt);
	void setResult(int rescode, int type, bool isOngame, double money);
	void removeResult();
	void hideTypeVip();
	void showChat(string mess);
	void showNumber(string numberString);
	void showNumber(double number);
	void runActionShowNumber(float dt);

	//MINHDV -changes - getIP to compare
	CC_SYNTHESIZE(string, _ip, UserIP);
	void showSameIP(bool isShow);//show icon nhung thang trung ip

	//!Level
	void SetLevel(const int& level);
	void SetLevel(const std::string& level);
	int& GetLevel();
	Label* GetLabelLevel();
	void StartAnimExp(const std::string& exp);
	void StartAnimExp(const int& exp);
	//hien thi ket qua cho 3cay
	void SetResult4AllUser(string& resuilt, string& score);
	void resetAll();
	void removeResultBaCay();
	//update for new version - lieng
	void setResultLieng(string pointcard, string card_type, string maxtype, string maxsubtance);
	void createFrameBetLieng();
	void hideFrameBet(bool isHide);
	void setBetValue(double betvl);
	void hideFrameResult();
	void runActionBet(double betvl);
	void runActionTotalBet(Vec2 dest);
	void showAnimationWin(bool isShow);
    void showNumberSoChi(string numberString);
    void runActionShowNumberSoChiMB(float dt);
private:
	Size sizeAd;
	int const DELAY_TIME = 100;
	Vec2 mMyRawPoint;
	enum
	{
		tagIcon = 0,
		tagInvite
	};

	bool isMe;
	bool meIsBoss;
	bool isCanTouch;
	bool isGuess;

	string aI;
	string nameString;
	string urlAvatar52La;
	Size sizeThis;
	Size sizeIcon;
	Node* nodeIcon;
	CircleAvatar* circleAvatar;

	ProgressTimer *timer;
	Layer   *layerWidget;
	Layer   *layerInvite;
	ImageView *bg;
	ImageView *icon;
	ImageView *flag;
	Label *name;
	Label  *money;
	Button *btnRechard;
	Sprite *moneyMask;
	Label *labelGuess;
	Sprite *flagIsMe;
	//Sprite* icc;
	float mCurrentDuration;
	float mCurrentTimer;
	int typeAva;
	//Type vip
	void setTypeVip(int _type);
	bool isImageExist(string url);

	vector<string> arrNumberString;
	vector<double> arrNumberDouble;
	bool isRunning;
	float numScale;
	void showLostConnect(bool isLost);
	string GetTricks(string& trick);
	//! Level
	int n_Level;
	//Label* p_Level;
	//current EXP
	int mCurrentExp;
	//update for new version - lieng
	Label* lblBet;
	ImageView *frameResult;
	string formatResLieng(string pointcard, string maxtype, string maxsubtance);
	vector<Sprite*> arrCoins;
	Vec2 destPos;//diem ket thuc arrcoins move
	Vec2 startPos;//diem bat dau arrcoins move
};

#endif // Avatar52La52La_h__



