//
//  LayerWebView.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/24/17.
//
//

#ifndef LayerWebView_hpp
#define LayerWebView_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../GameServer.h"
#include "Requests/ExtensionRequest.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerWebView :public Layer,public PlayerCallBack{
public:
    LayerWebView();
    ~LayerWebView();
    virtual bool init();
    CREATE_FUNC(LayerWebView);
    void hideLayer();
    void showLayer();
    void setWebView(string urlWeb);//set link de hien thi webview
    void setWebFile(string pathFile);
    bool isShouldShow = false;
private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
    void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
};
#endif /* LayerWebView_hpp */
