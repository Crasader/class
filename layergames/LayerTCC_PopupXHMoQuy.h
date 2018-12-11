#ifndef LayerTCC_PopupXHMoQuy_h__
#define LayerTCC_PopupXHMoQuy_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class LayerTCC_PopupXHMoQuy :public Layer,
	public PlayerCallBack
{
public:
	LayerTCC_PopupXHMoQuy();
	~LayerTCC_PopupXHMoQuy();
	CREATE_FUNC(LayerTCC_PopupXHMoQuy);
	virtual bool init();

	void setListRank(string listUser, string listMe);
protected:
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	Layout* pnlMain;
	void createButton();
	void createUI();
	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonRule(Ref* pSender, ui::Widget::TouchEventType type);
	void onLabelUserTouch(Ref* pSender, ui::Widget::TouchEventType type);

	vector<Text*> arrLblUser;
	vector<Text*> arrLblPoint;
	vector<Text*> arrLblPointMe;
	Text* llblThongBao;

	Text* getLabelByBetType(string bettype, vector<Text*> arrLabel);

	void event_EXT_EVENT_TCC_GAME_RESULT(boost::shared_ptr<ISFSObject> param);
};
#endif // LayerTCC_PopupXHMoQuy_h__
