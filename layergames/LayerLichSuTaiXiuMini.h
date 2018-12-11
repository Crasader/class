//
//  LayerLichSuTaiXiuMini.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/13/16.
//
//

#ifndef LayerLichSuTaiXiuMini_hpp
#define LayerLichSuTaiXiuMini_hpp

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
class LayerItemLichSuTXMini : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemLichSuTXMini);
    struct item{
        string phien;
        string time;
        string cuadat;
        string dat;
        string tralai;
        string nhan;
        string ketqua;
    };
    void setDatas(int stt,string phien,string time,string cuadat,string ketqua,string dat, string tralai,string nhan);
    item it;
private:
    ImageView* imgBg;
    Text* txtPhien;
    Text* txtThoiGian;
    Text* txtCuaDat;
    Text* txtKetQua;
    Text* txtTienCuoc;
    Text* txtTienTraLai;
    Text* txtTienNhan;
  };

class LayerLichSuTaiXiuMini : public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack
{
public:
    LayerLichSuTaiXiuMini();
    ~LayerLichSuTaiXiuMini();
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
    CREATE_FUNC(LayerLichSuTaiXiuMini);
private:
    struct item{
        int stt;
        string phien;
        string time;
        string cuadat;
        string dat;
        string tralai;
        string nhan;
        string ketqua;
    };
    TableView* tblListHis;
    vector<item> lstHistory;
    
};

#endif /* LayerLichSuTaiXiuMini_hpp */
