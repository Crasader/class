//
//  LayerThongKeDoDenMini.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/19/17.
//
//

#ifndef LayerThongKeDoDenMini_hpp
#define LayerThongKeDoDenMini_hpp

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
class LayerThongKeDoDenMini: public cocos2d::Layer,public PlayerCallBack{
public:
    LayerThongKeDoDenMini();
    ~LayerThongKeDoDenMini();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    
    void drawBieuDoXucXac();
    void drawBieuDoDiem();
    void loadThongKe(string lhs);
    void drawThongKe2();
    void drawThongKe1();

    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    CREATE_FUNC(LayerThongKeDoDenMini);
private:
//    TextField* txtOldPass;
//    TextField* txtmatkhauMoi;
//    TextField* txtnhaplaiMatKhauMoi;
//    string _oldPass;
//    Text* lblError;
    Node* nodeThongKe;

    Text* txtPhienGanNhat;
    vector<int> lstXucXac1;
    vector<int> lstXucXac2;
    vector<int> lstXucXac3;
    vector<int> lstTongDiem;

    ImageView* imgBieuDo1;
    ImageView* imgBieuDo2;
    
    int width = 47;
    int height = 49;

};


#endif /* LayerThongKeDoDenMini_hpp */
