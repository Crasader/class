//
//  LayerSetting.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/22/18.
//

#include "LayerSetting.hpp"
#include "AppMacros.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "GameManager.hpp"
#include "Common.h"
#include "ThemeManager.hpp"
#include "cocostudio/CocoStudio.h"
#include "../Scene/MainScene.hpp"

using namespace cocos2d;
using namespace cocostudio::timeline;
USING_NS_CC;
bool LayerSetting::init()
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
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerSetting.csb");
    rootNode->setContentSize(Size(visibleSize.width, rootNode->getContentSize().height));
    ui::Helper::doLayout(rootNode);
    
    ActionTimeline* action = CSLoader::createTimeline("LayerSetting.csb");
    action->gotoFrameAndPlay(0, false);
    this->addChild(rootNode);
    this->setContentSize(glview->getFrameSize());
    
    auto pnlMain = (Layout *)rootNode->getChildByName("pnlMain");
    auto pnlMainMain = (Layout *)pnlMain->getChildByName("pnlMainMain");
    if (pnlMainMain) {
        auto pnlVibra = (Layout *)pnlMainMain->getChildByName("pnlVibra");
        if (pnlVibra) {
            pnlVibra->addClickEventListener(CC_CALLBACK_1(LayerSetting::tapPnlVibra, this));
            lblVibra = (Text *)pnlVibra->getChildByName("lblVibration");
            lblVibra->setColor(ThemeManager::getInstance()->getFontColor());
            lblVibra->setString(GameManager::getInstance()->getStatusSetting(VIBRATION.c_str()));
            auto Text_10 = (Text *)pnlVibra->getChildByName("Text_10");
            Text_10->setColor(ThemeManager::getInstance()->getFontColor());
        }
        
        auto Panel_1_0_0 = (Layout *)pnlMainMain->getChildByName("Panel_1_0_0");
        Panel_1_0_0->setColor(ThemeManager::getInstance()->getColorLine());
        auto Panel_1_0_0_0 = (Layout *)pnlMainMain->getChildByName("Panel_1_0_0_0");
        Panel_1_0_0_0->setColor(ThemeManager::getInstance()->getColorLine());
        
        
        auto pnlSound = (Layout *)pnlMainMain->getChildByName("pnlSound");
        if (pnlSound) {
            pnlSound->addClickEventListener(CC_CALLBACK_1(LayerSetting::tapPnlSound, this));
            lblSound = (Text *)pnlSound->getChildByName("lblSound");
            lblSound->setColor(ThemeManager::getInstance()->getFontColor());
            lblSound->setString(GameManager::getInstance()->getStatusSetting(SOUND.c_str()));
            auto Text_10 = (Text *)pnlSound->getChildByName("Text_10");
            Text_10->setColor(ThemeManager::getInstance()->getFontColor());
            
        }
    }
    auto pnlTop = (Layout *)rootNode->getChildByName("pnlTop");
    auto pnlMainTop = (Layout *)pnlTop->getChildByName("pnlMainTop");
    
    if (pnlMainTop) {
        btnBack = (Button *)pnlMainTop->getChildByName("btnBack");
        btnBack->addClickEventListener(CC_CALLBACK_1(LayerSetting::backMain, this));
        btnBack->loadTextures(ThemeManager::getInstance()->getTextTureBtnBack(),ThemeManager::getInstance()->getTextTureBtnBack());
        auto lblTop = (Text *)pnlMainTop->getChildByName("lblMoney");
        lblTop->setColor(ThemeManager::getInstance()->getFontColor());
        
        auto Panel_1 = (Layout *)pnlMainTop->getChildByName("Panel_1");
        Panel_1->setColor(ThemeManager::getInstance()->getColorLine());
        
    }
    
    pnlMainTop->runAction(action);
    pnlMainMain->runAction(action);
    
    return true;
}
LayerSetting::LayerSetting()
{
    
}

LayerSetting::~LayerSetting()
{
    
}

void LayerSetting::tapPnlSound(Ref *pSender)
{
    GameManager::getInstance()->setStatusSetting(SOUND.c_str(),this->fromMainGame);
    CCLOG("tapPnlSound");
    lblSound->setString(GameManager::getInstance()->getStatusSetting(SOUND.c_str()));
}

void LayerSetting::tapPnlVibra(Ref *pSender)
{
    
    GameManager::getInstance()->setStatusSetting(VIBRATION.c_str(),this->fromMainGame);
    CCLOG("tapPnlVibra");
    lblVibra->setString(GameManager::getInstance()->getStatusSetting(VIBRATION.c_str()));
}
void LayerSetting::backMain(Ref *pSender)
{
    auto scene = MainScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void LayerSetting::setLayerFromMainGame(){
    btnBack->addClickEventListener(CC_CALLBACK_1(LayerSetting::backToMainGame, this));
}

void LayerSetting::backToMainGame(Ref *pSender)
{
    if (mCallBack && mCallBackListener){
        //mCallBack
        Node *param = Node::create();
        (mCallBack->*mCallBackListener)(param);
    }
}

void LayerSetting::setCallBack(Ref * target, SEL_CallFuncO listener){
    mCallBack = target;
    mCallBackListener = listener;
}
