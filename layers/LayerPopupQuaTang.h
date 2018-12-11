//
//  LayerPopupQuaTang.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerPopupQuaTang_hpp
#define LayerPopupQuaTang_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "LayerOffer.h"
#include "LayerGiftCode.h"
//#include "LayerWebView.h"
#include "LayerCongDong.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupQuaTang: public cocos2d::Layer{
public:
    LayerPopupQuaTang();
    ~LayerPopupQuaTang();
    virtual bool init();
    void hideLayer();
    void onBtnClose(Ref* pSender);
    void onBtnSuKien(Ref* pSender);
    void onBtnQuaTang(Ref* pSender);
    void onBtnGiftCode(Ref* pSender);
    void onBtnCongDong(Ref* pSender);

    void showLayer();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerPopupQuaTang);
private:
    LayerOffer* offer;
    LayerGiftCode* giftcode;
    //LayerWebView* sukien;
    Button* btnSuKien;
    Button* btnGiftcode;
    Button* btnQuaTang;
    Button* btnCongDong;
    LayerCongDong* congdong;
};

#endif /* LayerPopupQuaTang_hpp */
