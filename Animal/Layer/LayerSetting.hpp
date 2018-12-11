//
//  LayerSetting.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/22/18.
//

#ifndef LayerSetting_hpp
#define LayerSetting_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerSetting : public cocos2d::Layer
{
    
public:
    virtual bool init() override;
    LayerSetting();
    ~LayerSetting();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LayerSetting);
    void setLayerFromMainGame();
    void setCallBack(Ref * target, SEL_CallFuncO listener);
    bool fromMainGame = false;
protected:
    
private:
    SEL_CallFuncO mCallBackListener;
    Ref *mCallBack;
    void backMain(Ref *pSender);
    void tapPnlVibra(Ref *pSender);
    void tapPnlSound(Ref *pSender);
    Text *lblVibra, *lblSound;
    Button *btnBack;
    void backToMainGame(Ref *pSender);
    
    
};
#endif /* LayerSetting_hpp */
