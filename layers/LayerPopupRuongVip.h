//
//  LayerPopupRuongVip.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/10/16.
//
//

#ifndef LayerPopupRuongVip_hpp
#define LayerPopupRuongVip_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "Requests/ExtensionRequest.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupRuongVip : public cocos2d::Layer,public PlayerCallBack
{
public:
    LayerPopupRuongVip();
    ~LayerPopupRuongVip();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnTabThongTin(Ref* pSender);
    void onBtnTabLichSu(Ref* pSender);
    void onBtnOpenChest(Ref* pSender);
    void setValueOfChest(string value,string num);
    void showResult(float dt);
    void showWebview();
    void getPartnerInfo();
    bool isOpenning;
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
    CREATE_FUNC(LayerPopupRuongVip);
private:
    Text* txtNumKey;
    Text* txtValue;
    Button* btnTabThongTin;
    Button* btnTabLichSu;
    Button* btnOpenChest;
    Sprite* sprNoKey;
    Button* btnClose;
    ImageView* imgChest;
    Size visibleSize;
    Point orgPosOfChest;
    Sprite* imgEffectChest;
    int numOfKey;
    long valueOfChest;

    int moneyBonus;
    
    string partnerID;
    string srcID;
    int osid;
    string devID;
    vector<string> lstValue;
    
};

#endif /* LayerPopupRuongVip_hpp */
