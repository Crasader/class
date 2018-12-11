//
//  LayerGameOver.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/17/18.
//

#ifndef LayerGameOver_hpp
#define LayerGameOver_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerGameOver : public cocos2d::Layer
{
    
public:
    virtual bool init() override;
    LayerGameOver();
    ~LayerGameOver();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LayerGameOver);
    
    void setCallBackPlayAgain(Ref * target, SEL_CallFuncO listener);
    
protected:
    
private:
    SEL_CallFuncO mCallBackListener;
    Ref *mCallBack;
    void backMain(Ref *pSender);
    Text* lblMoney, *lblItemTime, *lblItemShrinker, *lblItemExpander;
    void playAgain(Ref *pSender);
    
    
};

#endif /* LayerGameOver_hpp */
