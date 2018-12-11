//
//  LayerMenuGame.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/16/18.
//

#ifndef LayerMenuGame_hpp
#define LayerMenuGame_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerMenuGame : public cocos2d::Layer
{
    
public:
    virtual bool init() override;
    LayerMenuGame();
    ~LayerMenuGame();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LayerMenuGame);
    
    void setCallBackUnPause(Ref * target, SEL_CallFuncO listener);
    
protected:
    
private:
    SEL_CallFuncO mCallBackListener;
    Ref *mCallBack;
    void backMain(Ref *pSender);
    Text* lblMoney, *lblItemTime, *lblItemShrinker, *lblItemExpander;
    void unPause(Ref *pSender);
    void reStart(Ref *pSender);
    
};

#endif /* LayerMenuGame_hpp */
