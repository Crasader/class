//
//  LayerLichSuPhienTXMini.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/13/16.
//
//

#ifndef LayerLichSuPhienTXMini_hpp
#define LayerLichSuPhienTXMini_hpp

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
class LayerItemLichSuPhienTXMiniTai : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemLichSuPhienTXMiniTai);
    void setDatas(int stt,string time,string name,string tai,string xiu,string hoan);
private:
    ImageView* imgBG;
    Text* txtTime;
    Text* txtTen;
    Text* txtTai;
    Text* txtHoan;
};

class LayerItemLichSuPhienTXMiniXiu : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemLichSuPhienTXMiniXiu);
    void setDatas(int stt,string time,string name,string tai,string xiu,string hoan);
private:
    ImageView* imgBG;
    Text* txtTime;
    Text* txtTen;
    Text* txtXiu;
    Text* txtHoan;
};


class LayerLichSuPhienTXMini : public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack
{
public:
    LayerLichSuPhienTXMini();
    ~LayerLichSuPhienTXMini();
    virtual bool init();
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnPrev(Ref* pSender);
    void onBtnNext(Ref* pSender);
    string getDay(string str);
    void loadHis();
    void showDice(string str);
    string GetUrlXiNgau(int number);
    int getTotalPoint(string str);
    string getTime(string str);
    void setCurrentItem(int tag);
    void setLstHistory(vector<string> list);
    void reloadCoin(int type);

    CREATE_FUNC(LayerLichSuPhienTXMini);
private:
    struct item{
        int stt;
        string time;
        string name;
        string tai;
        string xiu;
        string hoan;
    };
    TableView* tblListHisTai;
    TableView* tblListHisXiu;
    vector<string> lstHistory;
    Button *btnPre;
    Button *btnNext;
    Text* txtPhien;
    Text* txtNgay;
    ImageView* imgResult;
    Text* txtTotal;
    ImageView* imgDice1;
    ImageView* imgDice2;
    ImageView* imgDice3;
    Text* txtMoney;
    Text* txtName;
    vector<item> lstHistoryMatchTai;
    vector<item> lstHistoryMatchXiu;
    int currItem = 0;
    Text* txtTongTai;
    Text* txtTongXiu;
    Text* txtTongHoanTai;
    struct match{
        string mid;
        string time;
    };

    vector<match> lstMatch;
    string  phienid;
    int currHisID;
    ImageView* imgCoin;
    Text* txtHoanXiu;
};


#endif /* LayerLichSuPhienTXMini_hpp */
