//
//  LayerGameOver.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/17/18.
//

#include "LayerGameOver.hpp"

#include "AppMacros.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "GameManager.hpp"
#include "Common.h"
#include "ThemeManager.hpp"
#include "../Scene/MainScene.hpp"
#include "../Layer/LayerMainGame.hpp"
#include "../Scene/GameScene.hpp"
#include "ArchievementManager.hpp"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocostudio::timeline;
USING_NS_CC;
bool LayerGameOver::init()
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
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerGameOver.csb");
    rootNode->setContentSize(Size(visibleSize.width, rootNode->getContentSize().height));
    ui::Helper::doLayout(rootNode);
    
    ActionTimeline* action = CSLoader::createTimeline("LayerGameOver.csb");
    action->gotoFrameAndPlay(0, false);
    this->addChild(rootNode);
    this->setContentSize(glview->getFrameSize());
    
    auto pnlMain = (Layout *)rootNode->getChildByName("pnlMain");
    auto pnlMainMain = (Layout *)pnlMain->getChildByName("pnlMainMain");
    if (pnlMainMain) {
        
        auto btnPlayAgain = (Button *)pnlMainMain->getChildByName("Button_2");
        btnPlayAgain->addClickEventListener(CC_CALLBACK_1(LayerGameOver::playAgain, this));
        
//        if(GameManager::getInstance()->getTypeGame() == GAME_FREE){
//            btnPlayAgain->setVisible(false);
//        }
        
        auto lblBestScore = (Text *)pnlMainMain->getChildByName("lblBestScore");
        lblBestScore->setString(std::to_string(GameManager::getInstance()->getHighScore((GAME_ID)GameManager::getInstance()->getTypeGame())));
        lblBestScore->setColor(ThemeManager::getInstance()->getColorTime());
        
        auto lblTimes = (Text *)pnlMainMain->getChildByName("lblTimes");
        lblTimes->setString(GameManager::getInstance()->getItemByName(ITEM_TIME));
        lblTimes->setColor(ThemeManager::getInstance()->getFontColor());
        auto lblShrin = (Text *)pnlMainMain->getChildByName("lblShrin");
        lblShrin->setString(GameManager::getInstance()->getItemByName(ITEM_SHRINKER));
        lblShrin->setColor(ThemeManager::getInstance()->getFontColor());
        auto lblExpand = (Text *)pnlMainMain->getChildByName("lblExpand");
        lblExpand->setString(GameManager::getInstance()->getItemByName(ITEM_EXPANDER));
        lblExpand->setColor(ThemeManager::getInstance()->getFontColor());
        auto lblMoney = (Text *)pnlMainMain->getChildByName("lblMoney");
        lblMoney->setString(GameManager::getInstance()->getMoneyConverted());
        lblMoney->setColor(ThemeManager::getInstance()->getFontColor());
        
        
        
        ///Set color line
//        auto Panel_4 = pnlMainMain->getChildByName("Panel_4");
//        Panel_4->setColor(ThemeManager::getInstance()->getColorLine());
//        auto Panel_7 = pnlMainMain->getChildByName("Panel_7");
//        Panel_7->setColor(ThemeManager::getInstance()->getColorLine());
//        auto Panel_6 = pnlMainMain->getChildByName("Panel_6");
//        Panel_6->setColor(ThemeManager::getInstance()->getColorLine());
//        auto Panel_5 = pnlMainMain->getChildByName("Panel_5");
//        Panel_5->setColor(ThemeManager::getInstance()->getColorLine());
        
        
        
        auto lblTitleScore = (Text *)pnlMainMain->getChildByName("lblTitleScore");
        lblTitleScore->setColor(ThemeManager::getInstance()->getFontColor());
        
        auto lblScore = (Text *)pnlMainMain->getChildByName("lblScore");
        lblScore->setColor(ThemeManager::getInstance()->getFontColor());
        lblScore->setString(StringUtils::format("%d",GameManager::getInstance()->getScoreValue()));
        
        
        auto Text_12_0 = (Text *)pnlMainMain->getChildByName("Text_12_0");
        Text_12_0->setColor(ThemeManager::getInstance()->getFontColor());
        auto Text_4 = (Text *)pnlMainMain->getChildByName("Text_4");
        Text_4->setColor(ThemeManager::getInstance()->getFontColor());
//        auto Text_12 = (Text *)pnlMainMain->getChildByName("Text_12");
//        Text_12->setColor(ThemeManager::getInstance()->getFontColor());
        auto Text_4_0 = (Text *)pnlMainMain->getChildByName("Text_4_0");
        Text_4_0->setColor(ThemeManager::getInstance()->getFontColor());
        
//        auto lblTrophies = (Text *)pnlMainMain->getChildByName("lblTrophies");
//        lblTrophies->setString(GameManager::getInstance()->getMoneyConverted());
//        lblTrophies->setColor(ThemeManager::getInstance()->getColorMove());
        
//        int trophies = ArchievementManager::getInstance()->getListArchie().size();
//        int trophiesUnlock = ArchievementManager::getInstance()->getTrophie();
        
//        lblTrophies->setString(std::to_string(trophiesUnlock) + "/" + std::to_string(trophies));
        
    }
    auto pnlTop = (Layout *)rootNode->getChildByName("pnlTop");
    auto pnlMainTop = (Layout *)pnlTop->getChildByName("Panel_2");
    
    if (pnlMainTop) {
        auto btnBack = (Button *)pnlMainTop->getChildByName("btnBack");
        btnBack->addClickEventListener(CC_CALLBACK_1(LayerGameOver::backMain, this));
        btnBack->loadTextures(ThemeManager::getInstance()->getTextTureBtnBack(),ThemeManager::getInstance()->getTextTureBtnBack());
        auto lblTop = (Text *)pnlMainTop->getChildByName("lblMoney");
        lblTop->setColor(ThemeManager::getInstance()->getFontColor());
        
        ///Set color line
        auto Panel_1 = pnlMainTop->getChildByName("Panel_1");
        Panel_1->setColor(ThemeManager::getInstance()->getColorLine());
    }
    
//    pnlMainMain->runAction(action);
//    pnlMainTop->runAction(action);
    
    //Add label Score
//    auto lblScore = Label::createWithTTF(to_string(GameManager::getInstance()->getScoreValue()), FONT_JUNGBN, 200);
//    lblScore->setColor(ThemeManager::getInstance()->getColorMove());
//    pnlMainMain->addChild(lblScore);
//    lblScore->setPosition(Vec2(540, 1425));
    
    
    return true;
}

LayerGameOver::LayerGameOver()
{
    
}

LayerGameOver::~LayerGameOver()
{
    
}
void LayerGameOver::playAgain(Ref *pSender)
{
    if (mCallBack && mCallBackListener) {
        Node *param = Node::create();
        (mCallBack->*mCallBackListener)(param);
        this->removeFromParent();
    }
}

void LayerGameOver::backMain(Ref *pSender)
{
    auto scene = MainScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void LayerGameOver::setCallBackPlayAgain(Ref * target, SEL_CallFuncO listener)
{
    mCallBack = target;
    mCallBackListener = listener;
}

