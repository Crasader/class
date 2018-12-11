//
//  LayerOffer.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/27/16.
//
//

#ifndef LayerOffer_hpp
#define LayerOffer_hpp

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
class LayerItemOffer : public Layer{
public:
    virtual bool init();
    void setDatas(int index, int trangthai, int sogiay,string name, int type);
    void CountDownTimer(float dt);
    void showTimer();
    void onBtnNhanThuong(Ref* pSender);
    CREATE_FUNC(LayerItemOffer);
private:
    Text* txtName;
    Text* txtTimer;
    Button* btnGet;
    int timer;
    int currType;

    
};
class LayerOffer: public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack{
public:
    LayerOffer();
    ~LayerOffer();
    virtual bool init();
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    void hideLayer();
    void showLayer();

    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerOffer);
private:
  
    TableView* tableSpecial;
    struct Offer{
        int id;
        string name;
        int type;
        int trangthai;
        int thoigian;
    };
    vector<Offer> lstOffer;
    Text* txtNoGift;
    
};

#endif /* LayerOffer_hpp */
