//
//  LayerChuyenMoc.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerChuyenMoc_hpp
#define LayerChuyenMoc_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Requests/ExtensionRequest.h"
#include "LayerPopupThongBao.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerChuyenMoc: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerChuyenMoc();
    ~LayerChuyenMoc();
    virtual bool init();
    void hideLayer();
    void showLayer();
    void onBtnReceiverClicked(Ref* psender);
    void onPopupCallBack(Ref* pSender);
    void onScheduleUpdateCost(float dt);
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    CREATE_FUNC(LayerChuyenMoc);
private:
    Text *lblMoney, *lblMoneyPhi, *lblPhi, *lblSumMoney, *txtMinTrade;
    TextField *txtAcc, *txtValue,*txtContent,*tfNhapLaiTaiKhoan;
    Button *btnReceiver;
    Node *nodeContentChuyenTien;
    Text *lblErrorNick, *lblErrorValue,*lblErrorContent,*txtErrorNhapTK;

    Text* lblSilver;
    double mTax = 0;
};

#endif /* LayerChuyenMoc_hpp */
