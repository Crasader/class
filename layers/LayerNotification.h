#ifndef __iCasino_v2__LayerNotification__
#define __iCasino_v2__LayerNotification__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../layergames/NotificationCallBack.h"
#include "extensions/cocos-ext.h"
#include "../layers/LayerNotificationTop.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
class LayerNotification : public cocos2d::Layer{
public:
	LayerNotification();
	~LayerNotification();

	
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual bool init();
	CREATE_FUNC(LayerNotification);

	void reloadData(std::string url);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onHttpRequestCompleted(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
	int const TAG_CHILD = 111;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
private:
    cocos2d::EventListener* _touchListener;
	LayerNotificationTop* layerNotificationTop;
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
};
#endif //__iCasino_v2__LayerNotification__
