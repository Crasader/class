//
//  QuayThuongHN.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//
#ifndef QuayThuongHNHN_h__
#define QuayThuongHNHN_h__
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

#include "AllData.h"
#include "GameServer.h"
#include "_Button_inGame_.h"
#include "ThreadQueueMsg.h"
#include "CircleAvatar.h"
using namespace cocos2d;
using namespace cocos2d::ui;


class LayerBaoDanh : public Layer{
private:
	vector<ImageView*> arrImgBaoDanh;
	ImageView* createBaoDanh(Vec2 pos, int tag);
	void onButtonTut(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
public:
	CREATE_FUNC(LayerBaoDanh);
	virtual bool init();
	void setListBaoDanh(string _list);
	void setListBaoDanh(int _num);
};


class QuayThuongHN : public Layer, public PlayerCallBack
{
private:
	struct SliceInfo{
		int id;
		string text;
		SliceInfo(int _id, string _text) :
			id(_id), text(_text){};
	};

	Size visibleSize;
	Sprite* pSprite;
	float curPosition = 0;
	int luotquay = 0;
	vector<SliceInfo> listBet;
	Button* btnArrow;
	int prize = -1;//danh dau giai thuong

	//MINHDV -changes 22/7 for game p2
	int gameVersion = 1;
	Label* labelResult;
	LayerBaoDanh* layerBaoDanh;
	bool isLoginFirstDay = false;//bien nay de kiem tra cho thang vip 0 dang nhap lan dau tien trong ngay neu chua du 7 ngay se hien
	//thong bao la phai nang cap len vip 1

	void createListBet();
	void actionRotateDone();
	void createBackground();
	void createButtons();
	void showToast(string mess);
	void startRotating(int resultBet);
	void createSmartFoxEvent();
	void requestListAttend();
	void requestNumspin();

	void onButtonArrow(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonBack(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonCoin(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	void layerPopupThongBaoCallBack(Ref *pSender);
	void createLayerNotification(string title, string content_mess, int tag);
	bool onTouchBegan(Touch *touch, Event *unused_event);
public:
	QuayThuongHN();
	~QuayThuongHN();
	CREATE_FUNC(QuayThuongHN);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	static Scene* createScene();
	void setLuotquay(int _luotquay);
	void sendRequestAttend(int osid, string devid, string srcid);
protected:
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
};


#endif // QuayThuongHNHN_h__






