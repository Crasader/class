//
//  LayerPopupSuKien.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/25/17.
//
//

#ifndef LayerPopupSuKien_hpp
#define LayerPopupSuKien_hpp
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
class LayerItemSuKien : public Layer{
public:
    virtual bool init();
    void setDatas(string id, string name, string type,string url);
    void onBtnNhanThuong(Ref* pSender);
    void onBtnHuongDan(Ref* pSender);
    CREATE_FUNC(LayerItemSuKien);
private:
    Text* txtName;
    Button* btnGet;
    Button* huongDan;
    string urlHuongDan = "";
};
class LayerPopupSuKien: public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack{
public:
    LayerPopupSuKien();
    ~LayerPopupSuKien();
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
    void loadData(string data);
    
    CREATE_FUNC(LayerPopupSuKien);
private:
    TableView* tableSpecial;
    struct SuKien{
        string id;
        string name;
        string type;
        string url;
    };
    vector<SuKien> sukien;
    ImageView* Image_1;
};

#endif /* LayerPopupSuKien_hpp */
