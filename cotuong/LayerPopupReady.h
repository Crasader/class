#ifndef LayerPopupReady_h__
#define LayerPopupReady_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;
class LayerPopupReady : public cocos2d::Layer,
	public PlayerCallBack
{
public:
	virtual bool init() override;
	LayerPopupReady();
	~LayerPopupReady();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerPopupReady);

	void startTimer();
	void stopTimer();
	void showUIWithCheckBoss(string _listUser);//ham show giao dien voi 2 case : chu phong va nguoi choi
	void hideTimer();
	void showTimer();
	CC_SYNTHESIZE(int, time_count, TimeCount);
	virtual void onEnter();
	virtual void onExit();
protected:
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	
private:
	Button* btnReady, *btnLeave;
	Text *lblTimer;
	Text* lblContent;
	void updateTimer(float dt);
	string myName;
	bool isIamReady;//bien de check minh da ready hay chua
	bool isBossRoom;

	int time_buff = 0;
	int type_show = 0;//0 la danh cho chu phong, 1 la danh cho nguoi vao choi
	void onButtonReady(Ref* pSender);
	void onButtonLeave(Ref* pSender);
	void event_EXT_EVENT_READY_NTF(boost::shared_ptr<ISFSObject> param);
	void showButtonByType();
};


class LayerPopupVisitor : public cocos2d::Layer,
	public PlayerCallBack
{
public:
	virtual bool init() override;
	LayerPopupVisitor();
	~LayerPopupVisitor();
	CREATE_FUNC(LayerPopupVisitor);
protected:
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	void onButtonPlay(Ref* pSender);
	void onButtonSpec(Ref* pSender);
};

#endif // LayerPopupReady_h__



