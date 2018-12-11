//
//  LayerPopupUngTien.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/8/16.
//
//

#ifndef LayerPopupUngTien_hpp
#define LayerPopupUngTien_hpp

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

class LayerItemUngTien : public Layer{
public:
    virtual bool init();
    void setDatas(int vip, int money, int isActive);
    CREATE_FUNC(LayerItemUngTien);
private:
    Text* txtCapVip;
    Text* txtMoney;
    ImageView* imgBg;
    
};

class LayerPopupUngTien: public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack
{
public:
    LayerPopupUngTien();
    ~LayerPopupUngTien();
    virtual bool init();
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    int getCell();
    
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnTabUngTien(Ref* pSender);
    void onBtnTabLichSu(Ref* pSender);
    void onBtnHuongDan(Ref* pSender);
    void onBtnUngTien(Ref* pSender);
    void popUpCallBack(Ref* pSender);
    void onBtnTraNo(Ref* pSender);
    
    CREATE_FUNC(LayerPopupUngTien);
private:
    
    Layout* pnlUngTien;
    Layout* pnlLichSu;
    
    Text* txtCapVip;
    Text* txtPhe;
    Text* txtSoTienNo;
    
    Text* txtMyMoney;
    Text* txtSoLanUng;
    
    Button* btnTabUngTien;
    Button* btnTabLichSu;
    Button* btnUngTien;
    Button* btnHuongDan;
    
    TextField* txtMoney;
    
    int currTab = 1;
    
    TableView* tblListUngTien;
    
    struct itemu{
        int vip;
        int money;
        int isActive;
    };
    
    vector<itemu> lstu;
    
    int currVip = 0;
    
    int numAdvace = 0;
    
    int myMoney = 0;
    
    int currSotienno = 0;
    
};


#endif /* LayerPopupUngTien_hpp */
