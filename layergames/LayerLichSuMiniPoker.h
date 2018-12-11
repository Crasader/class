//
//  LayerLichSuMiniPoker.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/1/16.
//
//

#ifndef LayerLichSuMiniPoker_hpp
#define LayerLichSuMiniPoker_hpp

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
class LayerItemLichSuMiniPoker : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemLichSuMiniPoker);
    void setDatas(string phien,string time,string bet, string result,string win);
    void showResult(string str);
private:
    
    Text* txtPhien;
    Text* txtBet;
    Text* txtGet;
    Text* txtResult;
    Text* txtTime;
    ImageView* imgBg;
    ImageView* img1;
    ImageView* img2;
    ImageView* img3;
    ImageView* img4;
    ImageView* img5;
};

class LayerLichSuMiniPoker : public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack
{
public:
    LayerLichSuMiniPoker();
    ~LayerLichSuMiniPoker();
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
    CREATE_FUNC(LayerLichSuMiniPoker);
private:
    struct item{
        string phien;
        string time;
        string bet;
        string result;
        string win;
    };
    TableView* tblListHis;
    vector<item> lstHis;
    
};

#endif /* LayerLichSuMiniPoker_hpp */
