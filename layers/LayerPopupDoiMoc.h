//
//  LayerPopupDoiMoc.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerPopupDoiMoc_hpp
#define LayerPopupDoiMoc_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "LayerWebView.h"
#include "LayerDoiWing.h"
#include "LayerNapHao.h"
#include "LayerIAP2.h"
#include "LayerDoiThe.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupDoiMoc: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerPopupDoiMoc();
    ~LayerPopupDoiMoc();
    virtual bool init();
    void hideLayer();
    void onBtnClose(Ref* pSender);
    void onBtnMaThe(Ref* pSender);
    void onBtnDaiLy(Ref* pSender);
    void onBtnNhanThuong(Ref* pSender);
    void onBtnNapHao(Ref* pSender);
    void onBtnChinhSach(Ref* pSender);
    void onBtnIAP(Ref* pSender);
    void onBtnDoiThe(Ref* pSender);

    void showLayer();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    CREATE_FUNC(LayerPopupDoiMoc);
private:
    LayerDoiWing* muathe;
    Button* btnMuaMaThe;
    Button* btnDaiLy;
    Button* btnNhanThuong;
    Button* btnNapHao;
    Button* btnIAP;
    Button* btnDoiThe;

    LayerNapHao* naphao;
    LayerIAP2* iap2;
    Text* txtNhan;
    LayerWebView* webview;
    LayerDoiThe* doithe;
};

#endif /* LayerPopupDoiMoc_hpp */
