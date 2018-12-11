//
//  QuayThuong.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//
#ifndef QuayThuong_h__
#define QuayThuong_h__
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "AllData.h"
#include "GameServer.h"
#include "_Button_inGame_.h"
#include "ThreadQueueMsg.h"
#include "CircleAvatar.h"
using namespace cocos2d;
using namespace cocos2d::ui;

class QuayThuong : public Layer, public PlayerCallBack
{
private:
	enum 
	{
		TAG_1_TIME,
		TAG_5_TIME,
		TAG_10_TIME
	};
	struct qtStruct{
		int money;
		int numRotate;
		qtStruct(int _money, int _num) :
			money(_money), numRotate(_num){};
	};

	Size visibleSize;
	Sprite* pSprite;
	float curPosition = 0;
	int luotquay = 0;
	vector<int> listBet;
	LayerButtonInGame *layerButtons;
	Button* btnArrow;
	Label* labelCountRotate;//label hien thi so luot quay
	int prize = -1;//danh dau giai thuong
	bool isStaredGame = false;
	CircleAvatar *circleAvatar;
	Label* labelUserName;
	Label* labelUserMoney;
	Label* labelLv;

	string urlAvatar52La;
	string aI;
	double amfUser;
	double firstAmf;
	int minBetBuy = 0;//gia tri min cua muc tien mua luot quay
	vector<qtStruct> arrInfo;//luu lai tag cua cac button de lay gia tri tien mua luot quay
	//MINHDV -changes 22/7 for game p2
	int gameVersion = 1;

	void actionRotateDone();
	void createAvatar();
	void createBackground();
	void createButtons();
	void createButtonBuy(string count, string money, Vec2 pos);
	void createListBet();
	void showToast(string mess);
	void startRotating(int resultBet);

	void onButtonArrow(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonBuyRotate(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAvatarTouches(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	void layerPopupThongBaoCallBack(Ref *pSender);
	void createLayerNotification(string title, string content_mess, int tag);
	void setStateArrButtonBuy(bool _isDis);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onAvatarCallBack();
public:
	QuayThuong();
	~QuayThuong();
	CREATE_FUNC(QuayThuong);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
protected:
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
};

class KetQuaQuayThuong : public Layer
{
public:
	KetQuaQuayThuong();
	~KetQuaQuayThuong();
	CREATE_FUNC(KetQuaQuayThuong);
	virtual bool init();
	void setMoney(int money);
	void setResult(string result);
private:
	Sprite* bg;
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	virtual void onEnter();
	virtual void onExit();
	void createParticle();
};

#endif // QuayThuong_h__




