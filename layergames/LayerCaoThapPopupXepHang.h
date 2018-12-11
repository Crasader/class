//
//  LayerCaoThapPopupXepHang.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 11/14/16.
//
//

#ifndef LayerCaoThapPopupXepHang_hpp
#define LayerCaoThapPopupXepHang_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerCaoThapItemXepHang : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(LayerCaoThapItemXepHang);
    void setDatas(int stt,string name, string thang);
private:
    Layout* bg;
    Text* txtRank;
    ImageView* imgRank;
    ImageView* imgBg;
    Text* txtName;
    Text* txtGetMoney;
    ImageView* imgLine;
};

class LayerCaoThapPopupXepHang : public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack
{
public:
    LayerCaoThapPopupXepHang();
    ~LayerCaoThapPopupXepHang();
    virtual bool init();
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    string getTime(string str);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    CREATE_FUNC(LayerCaoThapPopupXepHang);
private:
    struct item{
        int stt;
        string name;
        string thang;
    };
    TableView* tblListRank;
    vector<item> lstRank;
    
};

#endif
