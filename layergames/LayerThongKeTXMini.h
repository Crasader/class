//
//  LayerThongKeTXMini.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/19/17.
//
//

#ifndef LayerThongKeTXMini_hpp
#define LayerThongKeTXMini_hpp

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
class LayerThongKeTXMini: public cocos2d::Layer,public PlayerCallBack{
public:
    LayerThongKeTXMini();
    ~LayerThongKeTXMini();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnBack(Ref* pSender);
    void onBtnNext(Ref* pSender);
    void drawBieuDoXucXac();
    void drawBieuDoDiem();
    void loadThongKe(string lhs);
    void drawThongKe2();
    void drawThongKe1();

    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    CREATE_FUNC(LayerThongKeTXMini);
private:
//    TextField* txtOldPass;
//    TextField* txtmatkhauMoi;
//    TextField* txtnhaplaiMatKhauMoi;
//    string _oldPass;
//    Text* lblError;
    Node* nodeBieuDo;
    Node* nodeThongKe;
    ImageView* point1Org;
    ImageView* diemOrg;

    Text* txtPhienGanNhat;
    vector<int> lstXucXac1;
    vector<int> lstXucXac2;
    vector<int> lstXucXac3;
    vector<int> lstTongDiem;

    ImageView* thongke1Org;
    ImageView* thongke2Org;
    
    int width = 47;
    int height = 49;
    Text* txtTai1 ;
     Text* txtXiu1 ;
     Text* txtTai2 ;
     Text* txtXiu2 ;
    
    Button* btnNext;
    Button* btnBack;
};


#endif /* LayerThongKeTXMini_hpp */
