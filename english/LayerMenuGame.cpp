//
//  LayerMenuGame.cpp
//  EnglishGame-mobile
//
//  Created by Sherlock Tuan on 11/30/18.
//

#include "LayerMenuGame.hpp"
#include "LayerMainGame2.hpp"
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#include "PluginAdMob/PluginAdMob.h"
#endif


Scene* LayerMenuGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LayerMenuGame::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    //    CCSVParse *pCsv2 = new CCSVParse();
    //    pCsv2->openFile("englishgame/enemy.csv");
    //
    // return the scene
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    sdkbox::PluginAdMob::show("top_banner");
#endif
    
    return scene;
}

// on "init" you need to initialize your instance
bool LayerMenuGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    string path = "LayerMainMenu.csb";
    auto rootNode = CSLoader::getInstance()->createNode(path);
    //    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    //    pnlBg->setPosition(Point(visibleSize / 2));
    Sprite* Sprite_1 = static_cast<Sprite*>(rootNode->getChildByName("Sprite_1"));
    Sprite_1->setScaleY(visibleSize.height / 1080);
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    
    Button* btnPlay = static_cast<Button*>(rootNode->getChildByName("btnPlay"));
    btnPlay->addClickEventListener(CC_CALLBACK_1(LayerMenuGame::onBtnPlay, this));
    
    return true;
}
void LayerMenuGame::onBtnPlay(Ref* pSender){
    auto scene = LayerMainGame2::createScene();
    Director::getInstance()->replaceScene(scene);
}
