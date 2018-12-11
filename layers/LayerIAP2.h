//
//  LayerIAP2.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerIAP2_hpp
#define LayerIAP2_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "mIAP.h"
#endif
USING_NS_CC;
USING_NS_CC_EXT;
#define MOBI 2
#define VINA 1
#define VIETTEL 3
using namespace std;
using namespace ui;
using namespace cocos2d;
class ItemIAP : public Layer{
public:
    virtual bool init();
    void setDatas(string price,string money,int tag,int type, mIAP* _iap);
    void onBtnBuy(Ref* pSender);
    CREATE_FUNC(ItemIAP);
private:
    Text* txtPrice;
    Text* txtMoney;
    Text* txtType;
    int tag = 0;
    int type = 0;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    mIAP* iap;
#endif
};
class LayerIAP2: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerIAP2();
    ~LayerIAP2();
    virtual bool init();

    void loadTyGia(int _type);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    void showLayer();
    void hideLayer();
    //bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerIAP2);
private:
    struct iap{
        string price;
        string gold;
        int tag;
        int type;
        iap(string _price,string _gold,int _tag):price(_price),gold(_gold),tag(_tag){};
    };
    vector<iap> lstIAP;
    ui::ScrollView* scrollTyGia;
    ui::ScrollView* scrollView;
    Layout* pnlMenhGia;
    int type = 0;
    
};
#endif /* LayerIAP2_hpp */
