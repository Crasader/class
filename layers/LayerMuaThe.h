//
//  LayerMuaThe.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerMuaThe_hpp
#define LayerMuaThe_hpp

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
class ItemTyGiaMuaThe : public Layer{
public:
    virtual bool init();
    void setDatas(int idx,int menhgia, int giamua);
    CREATE_FUNC(ItemTyGiaMuaThe);
private:
    Text* txtMenhGia;
    Text* txtGiaBan;
    ImageView* imgBg;
};
class LayerMuaThe: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerMuaThe();
    ~LayerMuaThe();
    virtual bool init();
    
    void onBtnNhaMang(Ref* pSender);
    void onBtnViettel(Ref* pSender);
    void onBtnVina(Ref* pSender);
    void onBtnMobi(Ref* pSender);
    void onBtnMenhGia(Ref* pSender);
    void onMenhGia(Ref* pSender);
    void hideLayer();
    void showLayer();
    void loadThe(int type);
    void onBtnMuaThe(Ref* pSender);
    void popUpCallBack(Ref* pSender);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void loadTyGia();
    
    //bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerMuaThe);
private:
    
    Button* btnNhaMang;
    Button* btnViettel;
    Button* btnVina;
    Button* btnMobi;
    Button* btnMenhGia;
    Button* btnMuaThe;
    Layout* pnlContent;
    Layout* pnlMenhGia;
    
    ui::ScrollView* scrollView;
    ui::ScrollView* scrollTyGia;
    
    bool isShowSelectNhaMang = false;
    bool isShowSelectMenhGia = false;
    int typeNhamang = 0;
    int currMenhgia = 0;
    vector<string> lstThe;
    vector<Button*> lstMenhGia;
    vector<string> lstGiaBan;
    
    Text* txtGiaBan;
    double mTax = 0;
    int typenhamang = 0;
    vector<int> the;
    
};

#endif /* LayerMuaThe_hpp */
