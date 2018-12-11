//
//  LayerKetQuaMauBinh.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/25/17.
//
//

#ifndef LayerKetQuaMauBinh_hpp
#define LayerKetQuaMauBinh_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "LayerPlayGameTXMini.h"
#include "../GameServer.h"
#include "LayerTimer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerItemKetQuaMauBinh : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemKetQuaMauBinh);
    void setDatas(string name,string chi,string tien);
private:
    Text* txtName;
    Text* txtChi;
    Text* txtTien;

};

class LayerKetQuaMauBinh : public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate
{
public:
    LayerKetQuaMauBinh();
    ~LayerKetQuaMauBinh();
    virtual bool init();
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnBatDau(Ref* pSender);
    void startReady(int _time);
    void setKetQua(string str);
    CREATE_FUNC(LayerKetQuaMauBinh);
private:
    TableView* tblKetQua;
    struct ketqua{
        string name;
        string tien;
        string chi;
    };
    vector<ketqua> lstkq;
    Button* btnStart;
    LayerTimer* layerTimer;

};


#endif /* LayerKetQuaMauBinh_hpp */
