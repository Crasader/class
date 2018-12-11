//
//  LayerDoiWing.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerDoiWing_hpp
#define LayerDoiWing_hpp

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
class ItemTyGiaDoiWing : public Layer{
public:
    virtual bool init();
    void setDatas(int idx,int menhgia, int giamua);
    void showNapTien(float dt);

    CREATE_FUNC(ItemTyGiaDoiWing);
private:
    Text* txtMenhGia;
    Text* txtGiaBan;
    ImageView* imgBg;
    Text* txtKhuyenMai;
    ImageView* imgZen;
};
class LayerDoiWing: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerDoiWing();
    ~LayerDoiWing();
    virtual bool init();
    
    void onBtnMenhGia(Ref* pSender);
    void onMenhGia(Ref* pSender);
    void onBtnDoiWing(Ref* pSender);
    void onBtnChiTiet(Ref* pSender);

    void hideLayer();
    void showLayer();
    void loadTyGia();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    
    //bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerDoiWing);
private:
    
    Button* btnMenhGia;

    ui::ScrollView* scrollTyGia;
    ui::ScrollView* scrollView;
    Layout* pnlMenhGia;
    bool isShowSelect = false;
    int typeNhamang = 0;
    double mTax = 0;
    
    TextField* txtWingAccount;

    Text* txtErrorWingAccount,*txtGuide1;
    vector<int> the;
    vector<Button*> lstMenhGia;
    int currMenhgia = 0;
    int amount = 0;
    
};
#endif /* LayerDoiWing_hpp */
