#ifndef __iCasino_v2__LayerIAP__
#define __iCasino_v2__LayerIAP__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "LayerPopupLuckyNumber.h"
#include "../Common.h"
#include "../layergames/NativeHelper.h"
#include "../data_manager.h"
#include "network/HttpClient.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "mIAP.h"
#endif

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocos2d::network;

class LayerIAP : public Layer,public PlayerCallBack
{
public:
	virtual bool init();
	LayerIAP();
	~LayerIAP();
    void showLayer();
    void hideLayer();
	CREATE_FUNC(LayerIAP);
    
private:
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void onButtonBronze(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonSilver(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonGold(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	mIAP* iap;
#endif
};
#endif //__iCasino_v2__LayerIAP__

