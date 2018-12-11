//
//  MainScene.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/22/18.
//

#include "../Scene/MainScene.hpp"
#include "GameManager.hpp"
#include "LayerMain.hpp"
#include "Common.h"
#include "ThemeManager.hpp"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;
using namespace std;
using namespace cocos2d::network;

Scene* MainScene::createScene()
{
    return MainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    UserDefault::getInstance()->setIntegerForKey(ID_THEME.c_str(),0);
    
    
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    
    audio->preloadBackgroundMusic("Caketown 1.mp3");
    audio->preloadEffect("game-music_0.mp3");
    audio->preloadEffect("starting.flac");
    audio->preloadEffect("click.flac");
    
    
//    CCLOG(std::to_string(visibleSize.width).c_str());
//    CCLOG(std::to_string(visibleSize.height).c_str());
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    

    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    

//    GameManager::getInstance()->setHighScore(HIGH_SCORE_MOVES, 50);
//    GameManager::getInstance()->setHighScore(HIGH_SCORE_TIMED, 50);
//    GameManager::getInstance()->setPlusItem(ITEM_TIME, 1);
//    GameManager::getInstance()->setMinusItem(ITEM_SHRINKER);
//    GameManager::getInstance()->setMoney(10000);
//    CCLOG("%s", std::to_string(GameManager::getInstance()->getHighScore(HIGH_SCORE_MOVES)).c_str());
//    CCLOG("%s", std::to_string(GameManager::getInstance()->getHighScore(HIGH_SCORE_TIMED)).c_str());
//
//
//    CCLOG("%s", std::to_string(GameManager::getInstance()->getItemByName(ITEM_TIME)).c_str());
//    CCLOG("%s", std::to_string(GameManager::getInstance()->getItemByName(ITEM_SHRINKER)).c_str());
//    CCLOG("%s", std::to_string(GameManager::getInstance()->getItemByName(ITEM_EXPANDER)).c_str());
//    CCLOG("%s", std::to_string(GameManager::getInstance()->getMoney()).c_str());
//
    
    
    auto layerMain = LayerMain::create();
    this->addChild(layerMain);
    
//    auto layerTop = LayerTopMain::create();
//    this->addChild(layerTop);
    
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
//        label->setColor(ThemeManager::getInstance()->getFontColor());
//        this->addChild(label, 1);
//    }
    
    
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
    
    
    HttpRequest* request = new (std :: nothrow) HttpRequest();
    request->setUrl("http://14.177.235.101/api/stories1");
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2 (MainScene::onHttpRequestCompleted, this));
    
    HttpClient::getInstance()->sendImmediate(request);
    
    request->release();
 
    return true;
}

void MainScene::onHttpRequestCompleted(HttpClient* sender, HttpResponse* response)
{
    if (!response)
    {
        return;
    }
    
    
    auto error = response->getErrorBuffer();
    if(error == "The Internet connection appears to be offline."){
        return; 
    }
    // Dump the data
    std::vector<char>* buffer = response->getResponseData();
    
    
    for (unsigned int i = 0; i <buffer-> size (); i ++)
    {
        log ("% c", (* buffer) [i]);
    }
}

void MainScene::menuCloseCallback(Ref* pSender)
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
