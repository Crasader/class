//
//  LayerMain.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/21/18.
//

#include "LayerMain.hpp"
#include "../Scene/GameScene.hpp"
#include "AppMacros.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "cocostudio/CocoStudio.h"
#include "GameManager.hpp"
#include "Common.h"
#include "ThemeManager.hpp"
#include "SimpleAudioEngine.h"
#include "LayerAlert.hpp"

USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio::timeline;
using namespace CocosDenshion;
bool LayerMain::init()
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
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerMain.csb");
    rootNode->setContentSize(Size(visibleSize.width, rootNode->getContentSize().height));
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    this->setContentSize(glview->getFrameSize());
    
    
    pnlMain = (Layout *)rootNode->getChildByName("pnlMain");
    if (pnlMain) {
//        this->btnMove = (Button *)pnlMain->getChildByName("btnMove");
//        this->btnFree = (Button *)pnlMain->getChildByName("btnFree");
        this->btnTime = (Button *)pnlMain->getChildByName("btnTime");
        
        this->btnTime->addClickEventListener(CC_CALLBACK_1(LayerMain::onButtonFree, this));
//        this->btnFree->addClickEventListener(CC_CALLBACK_1(LayerMain::onButtonFree, this));
//        this->btnMove->addClickEventListener(CC_CALLBACK_1(LayerMain::onButtonMove, this));
        
        auto Image_1 = (ImageView *)pnlMain->getChildByName("Image_1");
        auto Image_2 = (ImageView *)pnlMain->getChildByName("Image_2");
        
        ActionTimeline* action = CSLoader::createTimeline("LayerMain.csb");
        action->gotoFrameAndPlay(0, false);
//        action->setDuration(105);
        
        //    auto fadeIn = FadeIn::create(0.3f);
        //    auto spaw = Spawn::create((FiniteTimeAction *)action,fadeIn, NULL);
        
        int idTheme = ThemeManager::getInstance()->getIdTheme();
//        if (idTheme >= 10) {
//            Image_1->setVisible(false);
//            Image_2->setVisible(false);
//        }
        
        rootNode->runAction(action);
        
//        btnTime->runAction(action);
//        btnFree->runAction(action);
//        btnMove->runAction(action);
        
    }
    
//    GameManager::getInstance()->setMoney(GameManager::getInstance()->getItemInPlist("Money"));
    audio = SimpleAudioEngine::getInstance();
    audio->stopBackgroundMusic();
    if(GameManager::getInstance()->getSound()){
        audio->playEffect("game-music_0.mp3", true, 1.0f, 1.0f, 1.0f);
    }

    
    checkEndless();
    
    
    bool firstOpen = UserDefault::getInstance()->getBoolForKey(FIRST_OPEN.c_str());
    //false => mở lần đầu
    if (!firstOpen) {
        
//        layerLoad = CSLoader::getInstance()->createNode("LayerLoading.csb");
//        this->addChild(layerLoad, 9999);
    }

    return true;
}
void LayerMain::checkEndless(){
    
//    Check lượt chơi endless
//    if(!UserDefault::getInstance()->getBoolForKey("Endless")){
//        btnFree->loadTextures("res/Asset 207.png","res/Asset 206.png");
//        btnFree->addClickEventListener(CC_CALLBACK_1(LayerMain::onButtonShowAds, this));
//    }else{
//        btnFree->loadTextures("res/NewFolder/resize/Asset 64-min.png","res/NewFolder/resize/Asset 63-min.png");
//        btnFree->addClickEventListener(CC_CALLBACK_1(LayerMain::onButtonFree, this));
//    }
}
LayerMain::LayerMain()
{
    
}

LayerMain::~LayerMain()
{
    audio->stopAllEffects();
}

void LayerMain::changeScene(){
    auto action = ScaleTo::create(0.1f, 1.5);
    auto action2 = ScaleTo::create(0.25f, 0);
    pnlMain->runAction(Sequence::create(action,action2, NULL));
}

void LayerMain::disableButton(){
//    btnFree->setEnabled(false);
//    btnMove->setEnabled(false);
    btnTime->setEnabled(false);
    this->schedule(schedule_selector(LayerMain::goToGame), 0.5f);
}
void LayerMain::goToGame(float dt){
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(scene);
    this->unschedule(schedule_selector(LayerMain::goToGame));
}





void LayerMain::onButtonFree(Ref *pSender){
    changeScene();
    GameManager::getInstance()->setTypeGame(GAME_FREE);
    disableButton();
    
    UserDefault::getInstance()->setBoolForKey("Endless", false);
    
    
    
}
void LayerMain::onButtonMove(Ref *pSender){
    changeScene();
    GameManager::getInstance()->setTypeGame(GAME_MOVES);
    disableButton();
}
void LayerMain::onButtonTime(Ref *pSender){
    changeScene();
    GameManager::getInstance()->setTypeGame(GAME_TIME);
    disableButton();
}

