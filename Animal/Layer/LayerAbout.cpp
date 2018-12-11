//
//  LayerAbout.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/23/18.
//

#include "LayerAbout.hpp"
#include "AppMacros.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "GameManager.hpp"
#include "Common.h"
#include "ThemeManager.hpp"
#include "mUtils.hpp"
#include "cocostudio/CocoStudio.h"
#include "../Scene/MainScene.hpp"


using namespace cocos2d;
using namespace cocostudio::timeline;

USING_NS_CC;
bool LayerAbout::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    //    CCLOG(std::string(visibleSize));
    CCLOG("%s", std::to_string(glview->getFrameSize().width).c_str());
    CCLOG("%s", std::to_string(glview->getFrameSize().height).c_str());
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerAbout.csb");
    rootNode->setContentSize(Size(visibleSize.width, rootNode->getContentSize().height));
    ui::Helper::doLayout(rootNode);
    
    ActionTimeline* action = CSLoader::createTimeline("LayerAbout.csb");
    action->gotoFrameAndPlay(0, false);
    
    this->addChild(rootNode);
    this->setContentSize(glview->getFrameSize());
    
    
    auto pnlTop = (Layout *)rootNode->getChildByName("pnlTop");
    auto pnlmainTop = (Layout *)pnlTop->getChildByName("pnlmainTop");
    if (pnlmainTop) {
        auto lblMoney = (Text*) pnlmainTop->getChildByName("lblMoney");
        lblMoney->setColor(ThemeManager::getInstance()->getFontColor());
        auto btnBack = (Button*) pnlmainTop->getChildByName("btnBack");
        btnBack->addClickEventListener(CC_CALLBACK_1(LayerAbout::backMain, this));
        btnBack->loadTextures(ThemeManager::getInstance()->getTextTureBtnBack(),ThemeManager::getInstance()->getTextTureBtnBack());
        ///Set color line
        auto Panel_1 = pnlmainTop->getChildByName("Panel_1");
        Panel_1->setColor(ThemeManager::getInstance()->getColorLine());
    }
    
    auto pnlMain = (Layout *)rootNode->getChildByName("pnlMain");
    auto pnlMainMain = (Layout *)pnlMain->getChildByName("pnlMainMain");
    if (pnlMainMain) {
        auto ver = (Text*) pnlMainMain->getChildByName("Text_9");
        auto verDouble = (Text*) pnlMainMain->getChildByName("lblVersion");
        ver->setColor(ThemeManager::getInstance()->getFontColor());
        verDouble->setColor(ThemeManager::getInstance()->getFontColor());
        
//        float a = [[UIDevice currentDevice].systemVersion floatValue];
//        verDouble->setString(std::to_string(a));
        
        
        ///Set color line
        auto panel = pnlMainMain->getChildByName("Panel_7_0");
        panel->setColor(ThemeManager::getInstance()->getColorLine());
        auto Panel_2 = pnlMainMain->getChildByName("Panel_7");
        Panel_2->setColor(ThemeManager::getInstance()->getColorLine());
        
        
    }
    
    
    
    pnlMainMain->runAction(action);
    pnlmainTop->runAction(action);
    
    return true;
}

LayerAbout::LayerAbout()
{
    
}

LayerAbout::~LayerAbout()
{
    
}


void LayerAbout::backMain(Ref *pSender){
    auto scene = MainScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

