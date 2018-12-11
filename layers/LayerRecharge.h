#ifndef __iCasino_v2__LayerRecharge_
#define __iCasino_v2__LayerRecharge_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../layergames/NotificationCallBack.h"
#include "extensions/cocos-ext.h"
#include "../layers/LayerRechargeTop.h"
#include "ui/CocosGUI.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "mIAP.h"
#endif
USING_NS_CC;
USING_NS_CC_EXT;
class LayerPurcharse : public Layer
{
public:
    virtual bool init();
    LayerPurcharse();
    ~LayerPurcharse();
    CREATE_FUNC(LayerPurcharse);
private:
    void onButtonBronze(Ref* pSender, ui::Widget::TouchEventType type);
    void onButtonSilver(Ref* pSender, ui::Widget::TouchEventType type);
    void onButtonGold(Ref* pSender, ui::Widget::TouchEventType type);
    void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    mIAP* iap;
#endif
};

class LayerRecharge : public cocos2d::Layer{
public:
    LayerRecharge();
    ~LayerRecharge();
    
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init();
    CREATE_FUNC(LayerRecharge);
    
    void reloadData(std::string url);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onHttpRequestCompleted(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
    void onBtnShowInApp();
    void onBtnHideInApp();
    void showWebView();
    void hideWebView();

    int const TAG_CHILD = 111;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
    int currScene = 0;
private:
    cocos2d::EventListener* _touchListener;
    LayerRechargeTop* rechargeTop;
    LayerPurcharse* layerIAP;
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
};
#endif //__iCasino_v2__LayerNotification__
