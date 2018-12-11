//
//  LayerPopupDoiDiem.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/6/16.
//
//

#ifndef LayerPopupDoiDiem_hpp
#define LayerPopupDoiDiem_hpp

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
class LayerItemDoiDiem : public Layer{
public:
    virtual bool init();
    void onBtnDoiDiem(Ref* pSender);
    void popUpCallBack(Ref* pSender);
	void setDatas(int diem, string _desc, int id, int type, int value);
    CREATE_FUNC(LayerItemDoiDiem);
private:
    Button* btnDoiDiem;
    Text* txtDescription;
    Text* txtDiem;
	ImageView* imgItem;
	ImageView* imgCoin;
	TextBMFont* lbmPercent;

	int idItem;
};

class LayerPopupDoiDiem : public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack
{
public:
    LayerPopupDoiDiem();
    ~LayerPopupDoiDiem();
    virtual bool init();
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    int getCell();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    
    
    
    CREATE_FUNC(LayerPopupDoiDiem);
private:
    
    TableView* tblListReward;
    
    struct itemrw{
        int id;
        int diem;
        string desc;
		int type;
		int value;
    };
    
    vector<itemrw> lstrw;
    Text* txtDiemHienTai;
    
};


#endif /* LayerPopupDoiDiem_hpp */
