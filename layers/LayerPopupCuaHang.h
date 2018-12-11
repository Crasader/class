//
//  LayerPopupCuaHang.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerPopupCuaHang_hpp
#define LayerPopupCuaHang_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "LayerNapZen.h"
#include "LayerNapHao.h"
#include "LayerNapSMS.h"
#include "LayerChuyenMoc.h"
#include "GameServer.h"
#include "LayerIAP2.h"
#include "LayerNapCellCard.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupCuaHang: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerPopupCuaHang();
    ~LayerPopupCuaHang();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onBtnClose(Ref* pSender);
    void onBtnNapThe(Ref* pSender);
    void onBtnNapHao(Ref* pSender);
    void onBtnNapCellCard(Ref* pSender);

    void onBtnSMS(Ref* pSender);
    void onBtnChuyenMoc(Ref* pSender);
    void onBtnDoiMoc(Ref* pSender);
    void onBtnChinhSach(Ref* pSender);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void showNapTien(float dt);
    
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    
    CREATE_FUNC(LayerPopupCuaHang);
private:
    Button* btnNapThe;
    Button* btnIAP;
    Button* btnSMS;
    Button* btnChuyenMoc;
    Button* btnDoiMoc;
    Button* btnNapCellCard;
    Text* txtNhan;
    LayerNapZen* napmoc;
    LayerNapSMS* napsms;
    LayerIAP2* iap;
    LayerChuyenMoc* chuyenmoc;
    LayerNapCellCard* napcellcard;
    ImageView* stickWing;
    ImageView* stickCellCard;
    Text* txtKhuyenMaiWing;
    Text* txtKhuyenMaiCellCard;

};

#endif /* LayerPopupCuaHang_hpp */
