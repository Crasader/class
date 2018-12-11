//
//  LayerPopupChinhSach.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerPopupChinhSach_hpp
#define LayerPopupChinhSach_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "LayerWebView.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupChinhSach: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerPopupChinhSach();
    ~LayerPopupChinhSach();
    virtual bool init();
    void hideLayer();
    void onBtnClose(Ref* pSender);
    void onBtnTyLeNap(Ref* pSender);
    void onBtnTyLeRut(Ref* pSender);
    void onBtnQuyenLoiVip(Ref* pSender);
    void onBtnTyLePhe(Ref* pSender);
    void onBtnTyLeCellCard(Ref* pSender);

    void showLayer();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(LayerPopupChinhSach);
private:
    Button* btnTyLeNap;
    Button* btnTyLeRut;
    Button* btnTyLePhe;
    Button* btnQuyenLoiVip;
    Button* btnTyLeCellCard;
    LayerWebView* webview;
};

#endif /* LayerPopupChinhSach_hpp */
