//
//  LayerLichSuVQMM.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/18/16.
//
//

#ifndef LayerLichSuVQMM_hpp
#define LayerLichSuVQMM_hpp
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
class LayerItemLichSuVQMM : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemLichSuVQMM);
    void setDatas(int stt,string name,string money);
private:
    ImageView* imgbg;
    ImageView* imgLichSu;
    ImageView* imgAvatar;
    Text* txtName;
    Text* txtMoney;
    Text* txtRank;
    ImageView* imgLine;
};

class LayerLichSuVQMM : public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack
{
public:
    LayerLichSuVQMM();
    ~LayerLichSuVQMM();
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
    void onBtnMoc(Ref* pSender);
    
    void onBtnHao(Ref* pSender);
    
    CREATE_FUNC(LayerLichSuVQMM);
private:
    struct item{
        int stt;
        string name;
        string money;
    };
    Button* btnMoc;
    Button* btnHao;
    TableView* tblListLichSu;
    vector<item> lstLichSuMoc;
    vector<item> lstLichSuHao;
    
    int currentTab = 1;
    
};

#endif /* LayerLichSuVQMM_hpp */

