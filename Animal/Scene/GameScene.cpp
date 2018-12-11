//
//  GameScene.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/22/18.
//

#include "../Scene/GameScene.hpp"
#include "../Layer/LayerMainGame.hpp"
#include "ThemeManager.hpp"
#include "Common.h"
#include "LayerGameOver.hpp"
#include "LayerAchievementUnlock.hpp"
#include "ArchievementManager.hpp"
#include "GameManager.hpp"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;

Scene* GameScene::createScene()
{
    return GameScene::create();
}


// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    
    
    //    layerMain->setPosition(0,layerbottom->getContentSize().height);
    
//    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
//    if (label == nullptr)
//    {
//        problemLoading("'fonts/Marker Felt.ttf'");
//    }
//    else
//    {
//        // position the label on the center of the screen
//        label->setPosition(Vec2(origin.x + visibleSize.width/2,
//                                origin.y + visibleSize.height - label->getContentSize().height));
//        
//        // add the label as a child to this layer
//        this->addChild(label, 1);
//    }

    
    createLayerGame();
    
    
    auto bgBackground = ThemeManager::getInstance()->getBackground();
    if (bgBackground == nullptr) {
        bgBackground = Sprite::create("backgroundWhite.png");
        auto bgBackground3b = ThemeManager::getInstance()->getBackgroundColor();
        bgBackground->setColor(bgBackground3b);
    }
    bgBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    bgBackground->setScaleX(visibleSize.width / bgBackground->getContentSize().width);
    this->addChild(bgBackground,-1);
    
    //sound
    if(GameManager::getInstance()->getSound()){
        if(!audio->isBackgroundMusicPlaying()){
            audio->playBackgroundMusic("Caketown 1.mp3", true);
        }
        
    }
    
    return true;
}
void GameScene::createLayerGame(){
    layerGame = LayerMainGame::create();
    layerGame->setTag(LAYER_ID::MAIN_GAME_LAYER);
    
    this->addChild(layerGame);
    layerGame->setCallBackRestart(this, callfuncO_selector(GameScene::onReStart));
    layerGame->setCallBack(this, callfuncO_selector(GameScene::gotoGameOver));
    audio = CocosDenshion::SimpleAudioEngine::getInstance();
    
}

void GameScene::onReStart(Ref *pSender){
    this->createLayerGame();
    if(GameManager::getInstance()->getSound()){
        audio->resumeBackgroundMusic();
    }
    
}
void GameScene::gotoGameOver(Ref *pSender){
    
    this->removeChildByTag(LAYER_ID::MAIN_GAME_LAYER);
    audio->pauseBackgroundMusic();
    
    LayerGameOver* layerGameOver = LayerGameOver::create();
    layerGameOver->setTag(LAYER_ID::GAMEOVER_LAYER);
    layerGameOver->setCallBackPlayAgain(this, callfuncO_selector(GameScene::onReStart));
    this->addChild(layerGameOver);

    
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
}

