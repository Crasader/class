//
//  LayerPopupDauTruong.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/28/16.
//
//

#ifndef LayerPopupDauTruong_hpp
#define LayerPopupDauTruong_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupDauTruong: public cocos2d::Layer{
public:
    LayerPopupDauTruong();
    ~LayerPopupDauTruong();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnBaoTri(Ref* pSender);
    
    CREATE_FUNC(LayerPopupDauTruong);
private:
};

#endif /* LayerPopupDauTruong_hpp */
