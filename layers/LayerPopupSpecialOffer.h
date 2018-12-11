//
//  LayerPopupSpecialOffer.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/27/16.
//
//

#ifndef LayerPopupSpecialOffer_hpp
#define LayerPopupSpecialOffer_hpp

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
class LayerItemOffer : public Layer{
public:
    virtual bool init();
    void setDatas(int index, int trangthai, int sogiay,string name, int type);
    void CountDownTimer(float dt);
    void showTimer();
    void onBtnNhanThuong(Ref* pSender);
    CREATE_FUNC(LayerItemOffer);
private:
    Text* txtName;
    Text* txtTimer;
    Button* btnGet;
    int timer;
    int currType;

    
};
class LayerPopupSpecialOffer: public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack{
public:
    LayerPopupSpecialOffer();
    ~LayerPopupSpecialOffer();
    virtual bool init();
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    
    CREATE_FUNC(LayerPopupSpecialOffer);
private:
    Text* txtThongBao;
    TableView* tableSpecial;
    struct Offer{
        int id;
        string name;
        int type;
        int trangthai;
        int thoigian;
    };
    vector<Offer> lstOffer;
    
};

#endif /* LayerPopupSpecialOffer_hpp */
