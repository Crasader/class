//
//  LayerMenuGame.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/16/18.
//

#include "LayerMenuGame.hpp"

#include "AppMacros.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "GameManager.hpp"
#include "Common.h"
#include "../Scene/MainScene.hpp"
#include "../Layer/LayerMainGame.hpp"
#include "../Scene/GameScene.hpp"
USING_NS_CC;
bool LayerMenuGame::init()
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
    Node *rootNode;
    if(GameManager::getInstance()->getTypeGame() == GAME_FREE){
        rootNode = CSLoader::getInstance()->createNode("LayerMenuEndless.csb");
    }else{
        rootNode = CSLoader::getInstance()->createNode("LayerMenuGame.csb");
    }
        
    rootNode->setContentSize(Size(visibleSize.width, rootNode->getContentSize().height));
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    this->setContentSize(glview->getFrameSize());
    
    auto pnlMain = (Layout *)rootNode->getChildByName("pnlMain");
    if (pnlMain) {
        auto btnUnpause = (Button *)pnlMain->getChildByName("btnUnpause");
        btnUnpause->addClickEventListener(CC_CALLBACK_1(LayerMenuGame::unPause, this));
        auto btnRestart = (Button *)pnlMain->getChildByName("btnRestart");
        if (btnRestart) {
            btnRestart->addClickEventListener(CC_CALLBACK_1(LayerMenuGame::reStart, this));
        }
        auto btnExit = (Button *)pnlMain->getChildByName("btnExit");
        btnExit->addClickEventListener(CC_CALLBACK_1(LayerMenuGame::backMain, this));
    }
    
    return true;
}

LayerMenuGame::LayerMenuGame()
{
    
}

LayerMenuGame::~LayerMenuGame()
{
    
}

void LayerMenuGame::backMain(Ref *pSender)
{
    auto scene = MainScene::createScene();
    Director::getInstance()->replaceScene(scene);
}
void LayerMenuGame::reStart(Ref *pSender){
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void LayerMenuGame::unPause(Ref *pSender)
{
    if (mCallBack && mCallBackListener){
        //mCallBack
        Node *param = Node::create();
        (mCallBack->*mCallBackListener)(param);
        this->removeFromParent();
    }
}

void LayerMenuGame::setCallBackUnPause(Ref * target, SEL_CallFuncO listener){
    mCallBack = target;
    mCallBackListener = listener;
}


