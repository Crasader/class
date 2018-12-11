//
//  LayerXepHangMiniPoker.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/1/16.
//
//

#ifndef LayerXepHangMiniPoker_hpp
#define LayerXepHangMiniPoker_hpp

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
class LayerItemRankMiniPoker : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemRankMiniPoker);
    void setDatas(int stt,string name,string time,string bet, string win,string ketqua);
private:
    Text* txtName;
    Text* txtTime;
    Text* txtBet;
    Text* txtWin;
    Text* txtResult;
    ImageView* imgBg;
    ImageView* imgLine;
};

class LayerXepHangMiniPoker : public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack
{
public:
    LayerXepHangMiniPoker();
    ~LayerXepHangMiniPoker();
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
    string getResult(int rc);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    CREATE_FUNC(LayerXepHangMiniPoker);
private:
    struct item{
        string time;
        string name;
        string bet;
        string win;
        string ketqua;
    };
    TableView* tblListRank;
    vector<item> lstRank;
    
};
#endif /* LayerXepHangMiniPoker_hpp */
