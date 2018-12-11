#ifndef LayerTCC_PopupMoQuy_h__
#define LayerTCC_PopupMoQuy_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "LayerTCC_SlotMachine.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class LayerTCC_PopupMoQuy :public cocos2d::Layer,
	public PlayerCallBack
{
public:
	LayerTCC_PopupMoQuy();
	~LayerTCC_PopupMoQuy();
	CREATE_FUNC(LayerTCC_PopupMoQuy);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void setTimeMoQuy(int _time);
	void setListPercent(string _list);
	void startMoQuy();
	void setHisQuyThuong(string _his);
	void setListHistTichLuy(string _listHisTop, string _listHisUser);
	void setCurrentType(int type);
protected:
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

private:
	struct TimeMQ{
		int dayMQ;
		int hourMQ;
		int minuteMQ;
		int secondMQ;
		TimeMQ(int _day, int _hour, int _min, int _se) :
			dayMQ(_day), hourMQ(_hour), minuteMQ(_min), secondMQ(_se){};
	};
	struct ResMQ{
		string username;
		string bettype;
		string money;
		ResMQ(string _username, string _bettype, string _money) :
			username(_username), bettype(_bettype), money(_money){};
	};

	Layout* pnlMain;
	Sprite* imgHeader;

	Layout* pnlTimer;
	Text* lblGio;
	Text* lblPhut;
	Text* lblNgay;
	Layout* pnlQuayThuong;
	Sprite* spriteQuayThuong;
	Sprite* Sprite_17;//sprite hien thi o duoi thoi gian chay
	Text* lblTime;
	TextField* txtCheat;

	int timeMoQuy;
	int timeToNextWeek;

	vector<Text*> arrLabelUser;
	vector<TextBMFont*> arrLabelPercent;
	vector<LayerTCC_SLotMachine*> arrLayerSlot;
	vector<ResMQ> arrResMQ;
	//new version
	vector<Text*> arrLabelMoney;
    vector<Sprite*> lstCoin;
    
	void createUI();
	void createSlotMachines();
	void createButton();

	void scheduleUpdateTime(float dt);
	void slotMachineCallBack(Node* sender, void* data);
	void startQuayThuong();
	void startSlotWithIdx(int idx, string bettype, int time);

	void showAllSlot(bool isShow);
	void showResultGame(int tag);
	void gameStateRequest();

	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonRank(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonRule(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonCheat(Ref* pSender, ui::Widget::TouchEventType type);
	void onLabelUserTouch(Ref* pSender, ui::Widget::TouchEventType type);
	TimeMQ getDateFromInt(int _time);

	void event_EXT_EVENT_TCC_OPEN_QUY_THUONG_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_TCC_GAME_STATE_NTF(boost::shared_ptr<ISFSObject> param);
	string listHisTop;//danh cho xep hang diem tich luy
	string listHisUser;
};

#endif // LayerTCC_PopupMoQuy_h__
