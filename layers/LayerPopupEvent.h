//
//  LayerPopupEvent.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/13/16.
//
//

#ifndef LayerPopupEvent_hpp
#define LayerPopupEvent_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupEvent :
public cocos2d::Layer
{
public:
    LayerPopupEvent();
    ~LayerPopupEvent();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onBtnClose(Ref* pSender);
    void onButtonClicked(Ref* pSender);
    void popEvent(float dt);
    void loadEvent();
    void loadFirst();
    void reloadData(std::string url);
    virtual void onEnter() override;
    virtual void onExit() override;
    void onButtonAutoLogin(Ref* pSender);
    void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
    
    CREATE_FUNC(LayerPopupEvent);
private:
    vector<ui::Button*> lstButtons;
    vector<std::string> lstLinks;
    Sprite* sprHeader;
    Layout* pnlWebView;
	Layout* pnlHeaderTabs;
    ImageView* imgAutoLogin;
    bool isDestroy;
    
};

#endif /* LayerPopupEvent_hpp */
