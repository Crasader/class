//
//  LayerAlert.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/22/18.
//

#ifndef LayerAlert_hpp
#define LayerAlert_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerAlert : public cocos2d::Layer
{
    
public:
    virtual bool init() override;
    LayerAlert();
    ~LayerAlert();
    void setNoti(std::string noti);
    void btnOk();
    void btnOkBuyItem();
    // implement the "static create()" method manually
    CREATE_FUNC(LayerAlert);
    void setCallBack(Ref *target, SEL_CallFuncO listener);
protected:
    
private:
    SEL_CallFuncO mCallBackListener;
    Ref *mCallBack;
    void backMain(Ref *pSender);
    Text *lblNoti;
    void tapOk(Ref *pSender);
    Button *btnBack;
    void tapOkBuy(Ref *pSender);
};
#endif /* LayerAlert_hpp */
