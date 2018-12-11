//
//  GameScene.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/22/18.
//
#pragma once
#ifndef GameScene_hpp
#define GameScene_hpp

#include <stdio.h>
#include "Common.h"
#include "../Layer/LayerMainGame.hpp"
#include "SimpleAudioEngine.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    GameScene();
    ~GameScene();
    virtual bool init();
    
    int typeGame;
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onReStart(Ref *pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    
    
    
private:
    CocosDenshion::SimpleAudioEngine* audio;
    LayerMainGame* layerGame;
    void createLayerGame();
    void gotoGameOver(Ref *pSender);
    
    
};

#endif /* GameScene_hpp */
