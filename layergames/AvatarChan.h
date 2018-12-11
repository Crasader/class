#ifndef __iCasino_v2__AvatarChan__
#define __iCasino_v2__AvatarChan__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ImageDownloader.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocos2d;
using namespace std;

class AvatarChan : public Layer
{
public:

	~AvatarChan();
	AvatarChan(const bool& IsSmall);
	static AvatarChan* create(const bool& isSmall);
	bool init();

	Size GetSizeThis();

	void OnAvatarTouches(Ref *pObject, TouchEventType pType);

	void HideLayerInvite();
	void ShowLayerInvite();
	void VisibleLayerInvite(const bool& isShow);

	void setIcon(string url);
	void setName(string name);
	void setMoney(const long long& money);
	void setMoney(string money);
	void setMoney(int money);
	void setMoney(double money);
	void setReady(bool isReady);
	void setAI(string ai);
	void setMeIsBoss( bool isBoss ); 

	void setLblNTFChan(const char* text);
	Label* getLblNTFChan();

	void setLbMoneyBet(string text);
	Label* getLbMoneyBet();

	void setBlinkAvatar();

	void startTimer(const float& duration_ = 15.0);
	void stopTimer();

	void showFlagIsMe();
	void hideFlagIsMe();

	bool isStartedGame();

	void callbackTimer(Node *pSender);
	string convertMoney(int money);
	string convertMoneyFromDouble(double money);
	string convertMoneyFromDouble_Detail(double money);

	void setCanTouch(bool _cantouch);

private:

	ImageDownloader* Downloader;

	bool IsMe;
	bool IsBoss;
	bool IsCanTouch;

	string aI;
	string nameString;
	string urlAvatar;
	Size sizeThis;
	Size sizeIcon;
	Node* nodeIcon;

	ProgressTimer *Timer;
	Layer   *LayerWidget;
	Layer   *layerInvite;
	ImageView * BG;
	Label *DisplayName, *DisplayMoney;
	Sprite *flagIsMe;


	//for Chắn
	Label * lblNTF;
	//for tomcuaca
	Label *moneBet;
};

#endif