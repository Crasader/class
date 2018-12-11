#ifndef __iCasino_v2__LayerRechargeTop__
#define __iCasino_v2__LayerRechargeTop__

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

class LayerRechargeTop : public cocos2d::Layer, public PlayerCallBack
{
public:
    LayerRechargeTop();
    ~LayerRechargeTop();
    
    void onButtonClose(Ref* pSender);
    void onButtonClicked(Ref* pSender);
    
    virtual bool init();
    CREATE_FUNC(LayerRechargeTop);
    
    void loadTab();
    void loadFirst();
    void getBtnNoti(float dt);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void showLocalPayment(bool isShowInApp, bool isShowLocal);
    void onExit() override;
private:
    Button* btnNotification;
    Button* btnBonus;
    
    vector<ui::Button*> lstButtons;
    vector<std::string> lstLinks;
    bool isGet;
};
#endif //__iCasino_v2__LayerNotificationTop__
