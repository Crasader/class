//
//  LayerPopupTaoBan.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/18/16.
//
//

#ifndef LayerPopupTaoBan_hpp
#define LayerPopupTaoBan_hpp
#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;

class LayerPopupTaoBan : public cocos2d::Layer,public PlayerCallBack
{
public:
    LayerPopupTaoBan();
    ~LayerPopupTaoBan();
    virtual bool init();
    
       bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnMucCuoc(Ref* pSender);
    void onBtnTaoBan(Ref* pSender);
    void onMucCuoc(Ref* pSender);
    void loadMucCuoc();
    double getMinBetByGame(int gameID, double amf);

    CREATE_FUNC(LayerPopupTaoBan);
private:
    TextField* tfMatKhau;
    int betValue = 0;
    Layout* pnlMenhGia;
    Button* btnMucCuoc;
    ui::ScrollView* scrollView;
    bool isShowSelectMenhGia = false;
    vector<string> listbet;
    Layer* container;
    int minCuoc = 2000;
    int maxCuoc = 10000;
    int config = 0;
};

#endif /* LayerPopupTaoBan_hpp */

