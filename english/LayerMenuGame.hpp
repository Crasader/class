//
//  LayerMenuGame.hpp
//  EnglishGame-mobile
//
//  Created by Sherlock Tuan on 11/30/18.
//

#ifndef LayerMenuGame_hpp
#define LayerMenuGame_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

class LayerMenuGame : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void onBtnPlay(Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(LayerMenuGame);

};

#endif /* LayerMenuGame_hpp */
