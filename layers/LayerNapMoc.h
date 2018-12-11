//
//  LayerNapMoc.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerNapMoc_hpp
#define LayerNapMoc_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define MOBI 2
#define VINA 1
#define VIETTEL 3
using namespace std;
using namespace ui;
using namespace cocos2d;
class ItemTyGiaNapThe : public Layer{
public:
    virtual bool init();
    void setDatas(int idx,int menhgia, int giamua);
    CREATE_FUNC(ItemTyGiaNapThe);
private:
    Text* txtMenhGia;
    Text* txtGiaBan;
    ImageView* imgBg;
};
class LayerNapMoc: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerNapMoc();
    ~LayerNapMoc();
    virtual bool init();
    
    void onBtnNhaMang(Ref* pSender);
    void onBtnViettel(Ref* pSender);
    void onBtnVina(Ref* pSender);
    void onBtnMobi(Ref* pSender);
    
    void onBtnNapThe(Ref* pSender);
    void hideLayer();
    void showLayer();
    void loadTyGia();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    
    //bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerNapMoc);
private:
    
    Button* btnNhaMang;
    Button* btnViettel;
    Button* btnVina;
    Button* btnMobi;
    ui::ScrollView* scrollTyGia;
    bool isShowSelect = false;
    int typeNhamang = 0;
    double mTax = 0;
    
    TextField* txtMaThe;
    TextField* txtSeri;
    
    Text* txtErrorMaThe;
    Text* txtErrorSeri;
    vector<int> the;
    
};
#endif /* LayerNapMoc_hpp */
