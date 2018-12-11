//
//  LayerNapSMS.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerNapSMS_hpp
#define LayerNapSMS_hpp

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
class ItemTyGiaSMS : public Layer{
public:
    virtual bool init();
    void setDatas(int idx,int menhgia, int giamua);
    CREATE_FUNC(ItemTyGiaSMS);
private:
    Text* txtMenhGia;
    Text* txtGiaBan;
    ImageView* imgBg;
};
class LayerNapSMS: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerNapSMS();
    ~LayerNapSMS();
    virtual bool init();
    
    void onBtnNhaMang(Ref* pSender);
    void onBtnViettel(Ref* pSender);
    void onBtnVina(Ref* pSender);
    void onBtnMobi(Ref* pSender);
    void onBtnMenhGia(Ref* pSender);
    void onMenhGia(Ref* pSender);
    void onBtnBack(Ref* pSender);
    void onBtnNapSMS(Ref* pSender);
    void onBtnSend(Ref* pSender);
    void hideLayer();
    void showLayer();
    void loadSMS(int type);
    void onBtnNap(Ref* pSender);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void loadTyGia();
    
    
    //bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerNapSMS);
private:
    
    Button* btnNhaMang;
    Button* btnViettel;
    Button* btnVina;
    Button* btnMobi;
    Button* btnMenhGia;
    Button* btnNap;
    Layout* pnlContent;
    Layout* pnlNap;
    Text* txtTyGia;
    Text* txtCuPhap;

    Layout* pnlMenhGia;
    
    ui::ScrollView* scrollView;
    ui::ScrollView* scrollTyGia;
    
    bool isShowSelectNhaMang = false;
    bool isShowSelectMenhGia = false;
    int typeNhamang = 0;
    int currMenhgia = 0;

    vector<string> lstSMS;
    vector<Button*> lstMenhGia;
    vector<string> lstcuphap;
    vector<string> lsttiennhanduoc;
    vector<string> lstdauso;
};

#endif /* LayerNapSMS_hpp */
