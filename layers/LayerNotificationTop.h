#ifndef __iCasino_v2__LayerNotificationTop__
#define __iCasino_v2__LayerNotificationTop__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include "network/HttpClient.h"
#include "../Common.h"
#include "GameServer.h"
USING_NS_CC_EXT;

using namespace cocos2d;
using namespace network;
using namespace cocos2d::ui;
using namespace std;

class LayerNotificationTop : public cocos2d::Layer, public PlayerCallBack
{
public:
	LayerNotificationTop();
	~LayerNotificationTop();

	void onButtonClose(Ref* pSender);
	void onButtonClicked(Ref* pSender);

	virtual bool init();
	CREATE_FUNC(LayerNotificationTop);

	void loadNotification();
    void loadFirst();
    void getBtnNoti(float dt);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	virtual void update();
	void onExit() override;
private:
	Button* btnNotification;
	Button* btnBonus;

	vector<ui::Button*> lstButtons;
	vector<std::string> lstLinks;
    bool isGet;
};
#endif //__iCasino_v2__LayerNotificationTop__
