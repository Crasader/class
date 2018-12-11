//
//  LayerPopupDaiLy.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerPopupDaiLy_hpp
#define LayerPopupDaiLy_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "LayerMuaThe.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerItemDaiLy : public Layer{
public:
    virtual bool init();
    void onBtnBanMoc(Ref* pSender);
    void setDatas(string stt, string name, string khuvuc, string ma);
    CREATE_FUNC(LayerItemDaiLy);
private:
    Button* btnBanMoc;
    Text* txtSTT;
    Text* txtTenDaily;
    Text* txtMaDaiLy;
    Text* txtKhuVuc;

};
class LayerPopupDaiLy: public cocos2d::Layer, public PlayerCallBack, public TableViewDataSource,
public TableViewDelegate{
public:
    LayerPopupDaiLy();
    ~LayerPopupDaiLy();
    virtual bool init();
    void hideLayer();
    void onBtnClose(Ref* pSender);
    void onBtnMB(Ref* pSender);
    void onBtnMT(Ref* pSender);
    void onBtnMN(Ref* pSender);

    
    void showLayer();
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    int getCell();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    CREATE_FUNC(LayerPopupDaiLy);
private:
    TableView* tblDaiLy;
    
    struct item{
        string stt;
        string ten;
        string ma;
        string khuvuc;
    };
    vector<item> lstDaiLy;
    string strdaily;
    Button* btnMB;
    Button* btnMT;
    Button* btnMN;
};

#endif /* LayerPopupDaiLy_hpp */
