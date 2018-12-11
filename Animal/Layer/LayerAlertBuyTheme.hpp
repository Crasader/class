//
//  LayerAlertBuyTheme.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/24/18.
//

#ifndef LayerAlertBuyTheme_hpp
#define LayerAlertBuyTheme_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerAlertBuyTheme : public cocos2d::Layer
{
    
public:
    virtual bool init() override;
    LayerAlertBuyTheme();
    ~LayerAlertBuyTheme();
    void setNoti(std::string noti);
    // implement the "static create()" method manually
    CREATE_FUNC(LayerAlertBuyTheme);
    void setCallBack(Ref *target, SEL_CallFuncO listener);
    void setCallBackCancel(Ref *target, SEL_CallFuncO listener);
    void setInfoTheme(int idTheme);
protected:
    
private:
    SEL_CallFuncO mCallBackListener;
    SEL_CallFuncO mCallBackListener2;
    Ref *mCallBack;
    void tapUnlock(Ref *pSender);
    void tapCancel(Ref *pSender);
    Text *lblNoti;
};
#endif /* LayerAlertBuyTheme_hpp */
