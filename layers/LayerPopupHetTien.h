//
//  LayerPopupHetTien.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/8/16.
//
//

#ifndef LayerPopupHetTien_hpp
#define LayerPopupHetTien_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "LayerPopupCuaHang.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupHetTien: public cocos2d::Layer{
public:
    LayerPopupHetTien();
    ~LayerPopupHetTien();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnUngTien(Ref* pSender);
    void onBtnNapTien(Ref* pSender);
    void onBtnThoat(Ref* pSender);
    void onPopupCallBack(Ref *pSender);
   
    CREATE_FUNC(LayerPopupHetTien);
private:
    Button* btnUngTien;

};

#endif /* LayerPopupHetTien_hpp */
