//
//  LayerAbout.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/23/18.
//

#ifndef LayerAbout_hpp
#define LayerAbout_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerAbout : public cocos2d::Layer
{
    
public:
    virtual bool init() override;
    LayerAbout();
    ~LayerAbout();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LayerAbout);
    
    
protected:
    
private:
    void backMain(Ref *pSender);
};
#endif /* LayerAbout_hpp */
