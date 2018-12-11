//
//  LayerDanhSachLamCaiTXMini.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/18/16.
//
//

#ifndef LayerDanhSachLamCaiTXMini_hpp
#define LayerDanhSachLamCaiTXMini_hpp
#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "LayerPlayGameTXMini.h"
#include "../GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerItemLamCaiTXMini : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemLamCaiTXMini);
    void setDatas(int stt,string name,string status);
private:
    ImageView* imgbg;
    Text* txtName;
    Text* txtStatus;
    Text* txtRank;
};

class LayerDanhSachLamCaiTXMini : public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack
{
public:
    LayerDanhSachLamCaiTXMini();
    ~LayerDanhSachLamCaiTXMini();
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
    string getResult(string str);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    CREATE_FUNC(LayerDanhSachLamCaiTXMini);
private:
    struct item{
        int stt;
        string name;
        string status;
    };
    TableView* tblListRank;
    vector<item> lstRank;
    
};

#endif /* LayerDanhSachLamCaiTXMini_hpp */

