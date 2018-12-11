//
//  LayerMain.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/21/18.
//

#ifndef LayerMain_hpp
#define LayerMain_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace CocosDenshion;
using namespace cocostudio::timeline;

class LayerMain : public cocos2d::Layer
{
    
public:
    virtual bool init() override;
    LayerMain();
    ~LayerMain();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LayerMain);

protected:
    
private:

    Layout *pnlMain;
    Button *btnMove, *btnTime, *btnFree;
    void onButtonMove(Ref *pSender);
    void onButtonTime(Ref *pSender);
    void onButtonShowAds(Ref *pSender);
    void onButtonFree(Ref *pSender);
    void changeScene();
    void goToGame(float dt);
    void disableButton();
    SimpleAudioEngine* audio;
    int idEffect;
    ActionTimeline* action;
    Node *layerLoad;
    void checkEndless();
    Node *rootNode;
    
    bool fromLoadingLayer = false;
    void dismisLoading(float dt);
    void showVideoReward();
};

#endif /* LayerMain_hpp */
