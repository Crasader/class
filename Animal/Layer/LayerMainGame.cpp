//
//  LayerMainGame.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/22/18.
//

#include "../Layer/LayerMainGame.hpp"
#include "AppMacros.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "Common.h"
#include "Dot.hpp"
#include "GameManager.hpp"
#include "../Scene/MainScene.hpp"
#include "LayerMenuGame.hpp"
#include "ThemeManager.hpp"
#include "../Scene/GameScene.hpp"
#include "cocostudio/CocoStudio.h"
#include "LayerSetting.hpp"
#include "LayerAlert.hpp"
#include "SimpleAudioEngine.h"
#include "SoundManager.hpp"
#include "LayerPopupBuyItem.hpp"


using namespace cocos2d;
using namespace cocostudio::timeline;
using namespace CocosDenshion;
using namespace std;


USING_NS_CC;
bool LayerMainGame::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    
    auto director = Director::getInstance();
    this->glview = director->getOpenGLView();
    
    //    CCLOG(std::string(visibleSize));
    CCLOG("%s", std::to_string(glview->getFrameSize().width).c_str());
    CCLOG("%s", std::to_string(glview->getFrameSize().height).c_str());
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    this->isInSingleState = false;
    this->isInSameState = false;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerMainGame.csb");
    rootNode->setContentSize(Size(visibleSize.width, rootNode->getContentSize().height));
    ui::Helper::doLayout(rootNode);

//    ActionTimeline* action = CSLoader::createTimeline("LayerMainGame.csb");
//    action->gotoFrameAndPlay(0, false);
    
    this->addChild(rootNode,1);
    this->setContentSize(glview->getFrameSize());
    this->pnlMain = (Layout *)rootNode->getChildByName("pnlMain");
    if (pnlMain) {
        this->pnlMainGame = (Layout *)pnlMain->getChildByName("pnlGame");
        auto Panel_1 = (Layout *)pnlMain->getChildByName("Panel_1");
        
        tfRow = (TextField *)Panel_1->getChildByName("TextField_4");
        tfCol = (TextField *)Panel_1->getChildByName("TextField_4_0");
        tfName = (TextField *)Panel_1->getChildByName("TextField_4_0_0");
        
        Button* btn = (Button *)Panel_1->getChildByName("Button_2");
        btn->addClickEventListener(CC_CALLBACK_1(LayerMainGame::setAni, this));
    }
    
    pnlBottom = (Layout *)rootNode->getChildByName("pnlBottom");
    auto pnlBottomMain = (Layout *)pnlBottom->getChildByName("Panel_9");
    if (pnlBottomMain) {
        this->lblItemTime = (Text*) pnlBottomMain->getChildByName("lblItemTime");
        this->lblItemTime->setColor(ThemeManager::getInstance()->getFontColor());
        this->lblItemExpander = (Text*) pnlBottomMain->getChildByName("lblItemExpander");
        this->lblItemExpander->setColor(ThemeManager::getInstance()->getFontColor());
        this->lblItemShrinker = (Text*) pnlBottomMain->getChildByName("lblItemShrinker");
        this->lblItemShrinker->setColor(ThemeManager::getInstance()->getFontColor());
        
        btnTime = (Button *)pnlBottomMain->getChildByName("btnItemTime");
        btnShrinker = (Button *)pnlBottomMain->getChildByName("btnShrinker");
        btnExpander = (Button *)pnlBottomMain->getChildByName("btnExpander");
        
        btnTime->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onTapedBtnTime, this));
        btnShrinker->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onTapedBtnShrinker, this));
        btnExpander->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onTapedBtnExpander, this));
     
        if(GameManager::getInstance()->getTypeGame() == GAME_FREE){
            btnTime->setVisible(false);
            lblItemTime->setVisible(false);
//            lblItemExpander->setPosition(Vec2(810, 420));
//            lblItemShrinker->setPosition(Vec2(270, 420));
//            btnShrinker->setPosition(Vec2(270,270));
//            btnExpander->setPosition(Vec2(810,270));
        }
    }
    
    
    pnlTop = (Layout *)rootNode->getChildByName("pnlTop");
    auto pnlTopMain = (Layout *)pnlTop->getChildByName("Panel_8");
    
    if (pnlTopMain) {
        this->lblScore =(Text *)pnlTopMain->getChildByName("lblScore");
        this->lblModeGame = (Text*) pnlTopMain->getChildByName("lblMode");
        
        auto text_4 = (Text*) pnlTopMain->getChildByName("Text_4");
        text_4->setColor(ThemeManager::getInstance()->getFontColor());
        
        this->lblScore->setColor(ThemeManager::getInstance()->getFontColor());
        this->lblModeGame->setColor(ThemeManager::getInstance()->getFontColor());
        
        btnBack = (Button *) pnlTopMain->getChildByName("btnSaveBack");
        btnBack->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onButtonStop, this));
        
        
        
    }
    
//    pnlMainGame->runAction(action);
//    pnlTopMain->runAction(action);
//    pnlBottomMain->runAction(action);
    
    
    
    
    this->schedule(schedule_selector(LayerMainGame::startGame), 0.7f);
    
    list = EventListenerTouchOneByOne::create();
    list->setSwallowTouches(false);
    list->onTouchBegan = CC_CALLBACK_2(LayerMainGame::onTouchBegan, this);
    list->onTouchMoved = CC_CALLBACK_2(LayerMainGame::onTouchMoved, this);
    list->onTouchEnded = CC_CALLBACK_2(LayerMainGame::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(list, this->pnlMainGame);
    this->GAP = 150;
    
    
    
    auto played = UserDefault::getInstance()->getBoolForKey(std::to_string(typeGame).c_str());
    if (!played) {
        UserDefault::getInstance()->setBoolForKey(std::to_string(typeGame).c_str(), true);
    }
    
    btnExpanderClicked = false;
    btnShinkerClicked = false;
    
    
    
//    list = EventListenerTouchOneByOne::create();
//    list->onTouchBegan = CC_CALLBACK_2(LayerMainGame::onTouchBegan,this);
//    list->onTouchMoved =CC_CALLBACK_2(LayerMainGame::onTouchMoved,this);
//    list->onTouchEnded = CC_CALLBACK_2(LayerMainGame::onTouchEnded, this);
//    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(list,this);
//    
//    list->setEnabled(true);
    
    //Create
    createDotArray();
    createGameNode();

    
    //Fake Dot selected
    dotSelected = Dot::create(0, 10, 10, 1, 0, 0, false);
    //Fake Dot meeting name = 6;
    dotMeeting = Dot::create(0, 10, 10, 5, 0, 0, false);
    dotMeeting->setNameDot(6);
    
    
    audio = SimpleAudioEngine::getInstance();
    return true;
}

void LayerMainGame::animation(int dotName){
    layerAnimation = CSLoader::getInstance()->createNode("LayerAnimation.csb");
    this->pnlMainGame->addChild(layerAnimation);
    layerAnimation->setPosition(Vec2(0, 0));

    auto pnlMain = (Layout *)layerAnimation->getChildByName("pnlMain");
    
    auto star = (Sprite *)pnlMain->getChildByName("star");
    auto star1 = (Sprite *)pnlMain->getChildByName("star1");
    auto star2 = (Sprite *)pnlMain->getChildByName("star2");
    auto star3 = (Sprite *)pnlMain->getChildByName("star3");
    auto star4 = (Sprite *)pnlMain->getChildByName("star4");
    
    auto spriteAni = (Sprite *)pnlMain->getChildByName("spriteAni");
    
    spriteAni->setTexture(ThemeManager::getInstance()->getNameAnimal(dotName));
    auto light = (Sprite *)pnlMain->getChildByName("light");
    
    
    
    
    ActionTimeline* action = CSLoader::createTimeline("LayerAnimation.csb");
    action->gotoFrameAndPlay(0, false);
    
    star->runAction(action);
    star2->runAction(action);
    star1->runAction(action);
    star3->runAction(action);
    star4->runAction(action);

    light->runAction(action);
    spriteAni->runAction(action);
    if(GameManager::getInstance()->getSound()){
        audio->playEffect("starting.flac", false, 1.0f, 1.0f, 1.0f);
    }
   this->schedule(schedule_selector(LayerMainGame::timerForHidenAnimation), 0.3f);
}

LayerMainGame::LayerMainGame()
{
    
}

LayerMainGame::~LayerMainGame()
{
    int gameType = GameManager::getInstance()->getTypeGame();
    if (gameType == GAME_TIME) {
        GameManager::getInstance()->setHighScore(HIGH_SCORE_TIMED, GameManager::getInstance()->getScoreValue());
    }else if (gameType == GAME_MOVES){
        GameManager::getInstance()->setHighScore(HIGH_SCORE_MOVES, GameManager::getInstance()->getScoreValue());
    }else if(gameType == GAME_FREE){
        GameManager::getInstance()->setHighScore(HIGH_SCORE_FREE, GameManager::getInstance()->getScoreValue());
    }
    GameManager::getInstance()->setMoney(GameManager::getInstance()->getScoreValue());
}

void LayerMainGame::createGameNode(){
    
    
    for (int x = 0; x < 6; x++) {
        std::vector<Dot *> dotCol;
        for(int y = 0; y < 6; y++) {
            
            auto dot = Dot::create(x+y, x,y, random(1, 5), x, y, false);
//            auto dot = Dot::create(x+y, x,y, 5, x, y, false);
            auto posx = (GAP * x)/2 + (GAP * (x + 1))/2;
            auto posy = (GAP * y)/2 + (GAP * (y + 1))/2;
            
            
            dotCol.push_back(dot);
            this->pnlMainGame->addChild(dot, 2);
            dot->cocos2d::Node::setPosition(posx, posy);
            dot->setX(posx);
            dot->setY(posy);
        }
        this->listDot.push_back(dotCol);
    }
    
    if(!this->checkCanEat()){
        animationFadeOutAnimal();
        this->schedule(schedule_selector(LayerMainGame::reBuildAnimal), 1.0f);
    }
}

void LayerMainGame::updateLblItem(){
    this->lblItemShrinker->setString(GameManager::getInstance()->getItemByName(ITEM_SHRINKER));
    this->lblItemExpander->setString(GameManager::getInstance()->getItemByName(ITEM_EXPANDER));
    this->lblItemTime->setString(GameManager::getInstance()->getItemByName(ITEM_TIME));
}


bool LayerMainGame::onTouchBegan(Touch *pTouch, Event *pEvent){
    
    Vec2 pos = pTouch->getLocation();
    auto widthScreen = glview->getFrameSize().width;
    auto heightScreen = glview->getFrameSize().height;
    
//    auto target = static_cast<Layout*>(pEvent->getCurrentTarget());
//    Vec2 pos = target->convertToNodeSpace(pTouch->getLocation());
    
    if ( widthScreen > 1125) {
        CCLOG("widthscreen %s", std::to_string(widthScreen).c_str());
        CCLOG("heightScreen %s", std::to_string(heightScreen).c_str());

        auto sideWidth = ((widthScreen - ((heightScreen/1920) * 1080) )/2) ;
        
        
        //ipad pro 2
        if(widthScreen >= 1668){
        pos.set(pos.x + POSITION_PNL_GAME.x  - sideWidth , pos.y);
            
        }else{
           pos.set(pos.x  - sideWidth , pos.y);
        }
    }
    
    
    this->connectedDot.clear();
    this->position1 = Vec2();
    this->position2 = Vec2();
    this->isInSameState = false;
    this->isInSingleState = false;
    this->arrayDrawConnector.clear();
    
    startCol = int(std::abs(pos.x - POSITION_PNL_GAME.x)/GAP);
    startRow = int(std::abs(pos.y - POSITION_PNL_GAME.y)/GAP);
    
    if(startCol > 5){
        return false;
    }
    if(startRow > 5){
        return false;
    }
    
    
    
    firstDot = listDot[startCol][startRow];
    //Check btn item
    
    
    
    auto dotPosX = firstDot->getX();
    auto dotPosY = firstDot->getY();
//    auto startValue = arrayNameDot[startCol][startRow];
//    CCLOG("startCol %s", std::to_string(startCol).c_str());
//    CCLOG("startRow %s", std::to_string(startRow).c_str());
    
    
    if (!isInSingleState && !isInSameState) {
        
    }
    
    //Ngoài khoảng game play
    if(startCol >5 || startRow > 5){
        return false;
    }
    
    //Thỏa mãn điều kiện touch vào khu vực dot
    if (pos.x >= POSITION_PNL_GAME.x && pos.x <= pnlMainGame->getContentSize().width + POSITION_PNL_GAME.x &&  pos.y >= POSITION_PNL_GAME.y && pos.y <= pnlMainGame->getContentSize().height + POSITION_PNL_GAME.y && listDot[startCol][startRow]->getNameDot() != 0) {
        if (btnExpanderClicked) {
            if(GameManager::getInstance()->setMinusItem(ITEM_EXPANDER)){
                isInSameState = true;
                connectedDot.push_back(firstDot);
                endAction();
                btnExpanderClicked = false;
//                btnExpander->setEnabled(false);
                this->updateLblItem();
                return false;
            }else{
                showPopUpBuyItem("Expander", NOTI_EXPANDER, "5 for 500", "res/NewFolder/Asset 16.png", ITEM_EXPANDER);
                return false;
            }
        }else{
            if (btnShinkerClicked) {
                if(GameManager::getInstance()->setMinusItem(ITEM_SHRINKER)){
                    isInSingleState = true;
                    connectedDot.push_back(firstDot);
                    endAction();
                    btnShinkerClicked = false;
                    this->updateLblItem();
                    return false;
                }else{
                    showPopUpBuyItem("Shrinker", NOTI_SHRINKER, "10 for 100", "res/NewFolder/Asset 17.png", ITEM_SHRINKER);
                    return false;
                }
            }
            if (!firstDot->getClick()) {
                
                if (dotSelected != firstDot && dotSelected != NULL) {
                    dotSelected->setClick(false);
                }
                firstDot->setClick(true);
                dotSelected = firstDot;
                this->schedule(schedule_selector(LayerMainGame::timerForDoubleClickDot), 0.2f);
            }else{
                
                // Kiểm tra dot này hợp lệ double click rồi thì dùng item Shrinker
                if(GameManager::getInstance()->setMinusItem(ITEM_SHRINKER)){
                    isInSingleState = true;
                    connectedDot.push_back(firstDot);
                    endAction();
                    this->updateLblItem();
                    return false;
                }else{
                    showPopUpBuyItem("Shrinker", NOTI_SHRINKER, "10 for 100", "res/NewFolder/Asset 17.png", ITEM_SHRINKER);
                    return false;
                }
            }
        }
        
        
            CCLOG("posx %s", std::to_string(pos.x - POSITION_PNL_GAME.x).c_str());
            CCLOG("posy %s", std::to_string(pos.y - POSITION_PNL_GAME.y).c_str());
        
        if (pos.x - POSITION_PNL_GAME.x <= dotPosX + RADIUS_DOT && pos.x - POSITION_PNL_GAME.x >= dotPosX - RADIUS_DOT && pos.y - POSITION_PNL_GAME.y <= dotPosY + RADIUS_DOT && pos.y - POSITION_PNL_GAME.y >= dotPosY - RADIUS_DOT ) {
            
            this->position1 = this->listDot[startCol][startRow]->getPosition();
            connectedDot.push_back((Dot *)listDot[startCol][startRow]);
            if(GameManager::getInstance()->getSound()){
                audio->playEffect("click.flac", false, 1.0f, 1.0f, 1.0f);
            }
            
            runAnimationFadeOutDot(firstDot);
            CCLOG("%s", std::to_string(this->position1.x).c_str());
            CCLOG("%s", std::to_string(this->position1.y).c_str());
            
            return true;
        }
    }
    return false;
}

void LayerMainGame::runAnimationFadeOutDot(Dot *dot){
    auto sprite = Sprite::create(ThemeManager::getNameAnimal(dot->getNameDot()));
    this->pnlMainGame->addChild(sprite);
    sprite->setPosition(dot->getPosition());
    
    auto fadeOut = FadeOut::create(0.2f);
    auto scaleTo = ScaleTo::create(0.2f, 2);
    auto spaw = Spawn::createWithTwoActions(fadeOut, scaleTo);
    sprite->runAction(Sequence::create(spaw, CallFuncN::create(CC_CALLBACK_1(LayerMainGame::removeSpriteAnimation, this, sprite)), NULL));
    
}

void LayerMainGame::showRecomentUsingItem(){
    for (auto col = 0; col < listDot.size(); col++) {
        std::vector<Sprite *> spriteCol;
        for (auto row = 0; row < listDot[col].size(); row++) {
            Dot * dot = listDot[col][row];
            auto sprite = Sprite::create("res/NewFolder/Ellipse 1.png");
            this->pnlMainGame->addChild(sprite);
            sprite->setPosition(dot->getPosition());
            sprite->setOpacity(180);
            auto scaleTo = ScaleTo::create(0.2f, 1.3);
            sprite->runAction(scaleTo);
            spriteCol.push_back(sprite);
        }
        listSpriteItem.push_back(spriteCol);
    }
}

void LayerMainGame::removeDotFromList(Node* pSender, Dot *dot){
    pnlMainGame->removeChild(dot);
//    CCLOG("removed");
}

void LayerMainGame::removeSpriteAnimation(Node* pSender, Sprite *dot){
    pnlMainGame->removeChild(dot);
//    CCLOG("removed");
}

void LayerMainGame::pushDot(Dot* dot){
    auto a = std::find(connectedDot.begin(), connectedDot.end(), dot);
    auto index = std::distance(connectedDot.begin(), a);
    if (index == connectedDot.size()){
        connectedDot.push_back(dot);
    }
}

void LayerMainGame::onTouchMoved(Touch *pTouch, Event *pEvent){

    auto pos = pTouch->getLocation();

    auto widthScreen = glview->getFrameSize().width;
    auto heightScreen = glview->getFrameSize().height;
    if ( widthScreen > 1125) {
        auto sideWidth = ((widthScreen - ((heightScreen/1920) * 1080) )/2) ;
        
        
        //ipad pro 2
        if(widthScreen == 2048){
            pos.set(pos.x + POSITION_PNL_GAME.x  - sideWidth , pos.y);
        }else{
          pos.set(pos.x - sideWidth , pos.y);
        }
    }
    
//    auto target = static_cast<Layout*>(pEvent->getCurrentTarget());
//    Vec2 pos = target->convertToNodeSpace(pTouch->getLocation());
    
    this->position2 = Vec2(pos.x - POSITION_PNL_GAME.x, pos.y - POSITION_PNL_GAME.y);
    curCol = int(std::abs(pos.x - POSITION_PNL_GAME.x)/GAP);
    curRow = int(std::abs(pos.y - POSITION_PNL_GAME.y)/GAP);
    
    auto startDot = listDot[startCol][startRow];
    this->drawnNode(startDot->getNameDot());
    
    if(curCol > 5){
        return;
    }
    if (curRow > 5) {
        return;
    }

//    CCLOG("this->position2.x %s", std::to_string(this->position2.x).c_str());
//    CCLOG("this->position2.y %s", std::to_string(this->position2.y).c_str());
    
    
    
    if (!isInSingleState && curRow < 6 && curCol < 6 && ((curRow == startRow && std::abs(curCol - startCol) == 1)) || (curCol == startCol && std::abs(curRow - startRow) == 1)) {
        
        auto dotPosX = listDot[curCol][curRow]->getX();
        auto dotPosY = listDot[curCol][curRow]->getY();
        
        if (pos.x - POSITION_PNL_GAME.x <= dotPosX + RADIUS_DOT && pos.x - POSITION_PNL_GAME.x >= dotPosX - RADIUS_DOT && pos.y - POSITION_PNL_GAME.y <= dotPosY + RADIUS_DOT && pos.y - POSITION_PNL_GAME.y >= dotPosY - RADIUS_DOT ) {
        
            CCLOG("Hợp lệ");

            auto curDot = listDot[curCol][curRow];
            auto startDot = listDot[startCol][startRow];

            //2 dot giống nhau
            if (curDot->getNameDot() && startDot->getNameDot() == curDot->getNameDot()) {
                CCLOG("Giống nhau");
                
                
                auto curDotInConnected = std::find(connectedDot.begin(), connectedDot.end(), curDot);
                auto startDotInConnected = std::find(connectedDot.begin(), connectedDot.end(), startDot);
                auto index = std::distance(connectedDot.begin(), curDotInConnected);
                auto indexStart = std::distance(connectedDot.begin(), startDotInConnected);

                
                //Check dot cos trong connectedDot chua? index = size => chưa có.
                if (index == connectedDot.size()){
                    
                    connectedDot.push_back((Dot *)listDot[curCol][curRow]);
                    runAnimationFadeOutDot(curDot);
                    if(GameManager::getInstance()->getSound()){
                        audio->playEffect("click.flac", false, 1.0f, 1.0f, 1.0f);
                    }
                    
                    this->position2 = this->listDot[curCol][curRow]->getPosition();
                    
                    if(startDot == dotMeeting){
                        curDot->setLastSquare(true);
                    }
                    
                    drawNodeConnector(curDot);
                    this->position1 = position2;
                    startCol = curCol;
                    startRow = curRow;
                }else{
                    
                    // Check số lần nối có đủ thành hình vuông k?
                    if(connectedDot.size() - index > 3){
                        CCLOG("ZÔ đây");
                        
                        ///kiểm tra điểm nối hiện tại có trong list connect
                        if (this->isInSameState && curDot != dotMeeting) {
                            return;
                        }
                        
                        if(curDot == dotMeeting){
                            if(!startDot->getLastSquare()){
                                return;
                            }
                            /// Nối ngược lại
                            this->pnlMainGame->removeChild((Node *)arrayDrawConnector[indexStart]);
                            arrayDrawConnector.erase(arrayDrawConnector.begin() + (indexStart));
                            //remove dot from list dot connected
                            connectedDot.erase(connectedDot.begin() + (indexStart));
                            auto dot = listDot[curCol][curRow];
                            this->position1 = dot->getPosition();
                            startCol = curCol;
                            startRow = curRow;
                            this->drawnNode(dot->getNameDot());
                            startDot->setLastSquare(false);
                            return;
                        }else{
                            this->isInSameState = true;
                            dotMeeting = curDot;
                        }
                        
                        auto dot = connectedDot[index];
                        this->position2 = dot->getPosition();
                        drawNodeConnector(dot);
                        this->position1 = this->position2;
                        
                        startCol = dot->getCol();
                        startRow = dot->getRow();

                    }else{
//                        //Nếu trong trường hợp này sẽ là từ hình vuông nối ngược lại
//                        if (isInSameState) {
//
//                            else
                        if (isInSameState) {
                            if (connectedDot.size() - index == 1) {
                                this->isInSameState = false;
                                dotMeeting = Dot::create(0, 10, 10, 5, 0, 0, false);
                                this->pnlMainGame->removeChild((Node *)arrayDrawConnector[index]);
                                arrayDrawConnector.erase(arrayDrawConnector.begin() + index);
                            }else if (connectedDot.size() - index == 2) {
                                this->pnlMainGame->removeChild((Node *)arrayDrawConnector[index + 1]);
                                arrayDrawConnector.erase(arrayDrawConnector.begin() + index + 1);
                                //remove dot from list dot connected
                                connectedDot.erase(connectedDot.begin() + (index + 1));
                            }else{
                                return;
                            }
                            
                            
                        }else{

                        
                        // Khi người dùng nối ngược lại dot sẽ remove dot trước đó khỏi listDotRemoved
                            if (connectedDot.size() - index == 2) {
                                this->pnlMainGame->removeChild((Node *)arrayDrawConnector[index]);
                                arrayDrawConnector.erase(arrayDrawConnector.begin() + index);
                                connectedDot.erase(connectedDot.begin() + (index + 1));
                            }
                        }
                        
                        //                        this->clearDrawnNode();
                        auto dot = listDot[curCol][curRow];
                        this->position1 = dot->getPosition();
                        startCol = curCol;
                        startRow = curRow;
                        this->drawnNode(dot->getNameDot());
                        
                    }

                }

            }
        }
        
    }
    
}
void LayerMainGame::onTouchEnded(Touch *pTouch, Event *pEvent){
//    CCLog("onTouchesEnded");
    dotMeeting = Dot::create(0, 10, 10, 5, 0, 0, false);
    dotMeeting->setNameDot(6);
    auto pos = pTouch->getLocation();
    if (drawNode) {
        this->pnlMainGame->removeChild(drawNode);
    }
    
    if (connectedDot.size() > 1){
        this->endAction();
    }
    
    
}

void LayerMainGame::endAction(){
    createListDotRemove();
    removeSpriteUsingItem();
    if (connectedDot.size() >= 1 || isInSingleState || isInSameState) {
        
        if (isInSingleState) {
            isInSingleState = false;
            removeDot(connectedDot[0]);
            updateLblItem();
        }else if (isInSameState) {
            if(GameManager::getInstance()->getVibration()){
                cocos2d::Device::vibrate(0.2);
            }
            int nameDotConnected = connectedDot[0]->getNameDot();
            animation(nameDotConnected);
            for (auto rol = 0; rol < listDot.size(); rol++) {
                for (auto row = 0; row < listDot[rol].size(); row++) {
                    auto dot = listDot[rol][row];
                    if (dot->getNameDot() == nameDotConnected) {
                        removeDot(dot);
                    }
                }
            }
            //nối hình vuông thì sẽ tính count square
            if(!btnExpanderClicked){
                GameManager::getInstance()->plusCountSquare();
            }
        }else{
            
            for (auto i = 0; i < connectedDot.size(); i++) {
                removeDot(connectedDot[i]);
                
            }
        }
        connectedDot.clear();
        
        for (auto col = 0 ; col < listDot.size(); col ++) {
            
            int countDotRemoved = 0;
            
            for (auto row = 0 ; row < listDot[col].size(); row++) {
                Dot *curDot = listDot[col][row];
                if (curDot->getNameDot() == 0) {
                    countDotRemoved += 1;
//                    curDot->setVisible(false);
                    
                    auto scale = ScaleTo::create(0.2f, 0);
                    curDot->runAction(Sequence::create(scale, CallFuncN::create(CC_CALLBACK_1(LayerMainGame::removeDotFromList, this, curDot)),CallFuncN::create(CC_CALLBACK_1(LayerMainGame::moveDotToNewPosition, this, col,row ,countDotRemoved)), NULL));
                }
                
                // neesu có dot bị ăn thì move Dot Có sẵn
                if (countDotRemoved > 0) {
                    
                    Dot *dot = listDot[col][row];
                    if (dot){
                        int yNew = -(countDotRemoved * this->GAP);
                        
                        if (dot->getNameDot() != 0 && dot->getNameDot() < 6 ) {
                            
                            auto rowDot = dot->getRow();
                            auto move = MoveBy::create(TIME_RUN, Vec2(0, yNew));
                            //                            auto move_ease_out = EaseBackInOut::create(move);
                            //                            EaseQuadraticActionIn
                            auto move_ease_out = EaseBounceOut::create(move);
                            dot->runAction(move_ease_out);
                            dot->setRow(rowDot - countDotRemoved);
                            auto dotY = dot->getY();
                            dot->setY(dotY + yNew);
                            this->listDot[col][row - countDotRemoved] = dot;
                        }else{
                            CCLOG("Name Dot: %d" + dot->getNameDot());
                        }
                        
                    }
                }
            }
            
            //Thêm mới dot
            for (auto i = 0; i < countDotRemoved; i++) {
                int yNew = -(countDotRemoved * this->GAP);
                
                
                //newRow là số row thêm mới của dot này
                
                int newRow = 6 - (countDotRemoved/countDotRemoved) - i;
                
                int newX = (col * GAP) + (GAP/2);
                int newY = (newRow * GAP) + (GAP/2);
                int newYTop = newY + (GAP * countDotRemoved);
                Dot *newDot = Dot::create(0, col, newRow, random(1, 5), newX , newY, false);
//                Dot *newDot = Dot::create(0, col, newRow, 5, newX , newY, false);
                
                //set vị trí Dot ở top
                
//                auto x = newDot->getX();
                this->pnlMainGame->addChild(newDot, 2);
                newDot->cocos2d::Node::setPosition(newDot->getX(), newYTop);
                
                auto move = MoveBy::create(TIME_RUN, Vec2(0, yNew));
                auto move_ease_out = EaseBounceOut::create(move);
                
                //Move Dot xuống đúng vị trí
                newDot->runAction(move_ease_out);

                //thêm vào list dot
                this->listDot[col][newRow] = newDot;
                
            }
        }
        //Xóa drawn connector
        for (auto index = 0; index < arrayDrawConnector.size(); index++) {
            this->pnlMainGame->removeChild((Node *)arrayDrawConnector[index]);
            //                    (arrayDrawConnector[index].getNode);
        }
        
        
        this->getScore();
        this->updateScore();
        if (GameManager::getInstance()->getTypeGame() == GAME_MOVES) {
            this->countDownMove();
        }
        
        this->arrayDrawConnector.clear();
        Vector<Node*> childList = pnlMainGame->getChildren();
        for (auto i = 0; i < childList.size(); i++) {
//            Node *child = childList[i];
            DrawNode * childDraw = dynamic_cast<DrawNode *>(childList.at(i));
            if (childDraw) {
                childDraw->removeFromParent();
            }
        }
        listDotRemoved.clear();
        if(!this->checkCanEat()){
            animationFadeOutAnimal();
            this->schedule(schedule_selector(LayerMainGame::reBuildAnimal), 1.0f);
        }
    }
}

void LayerMainGame::moveDotToNewPosition(Node* pSender,int col, int row, int countDotRemoved){
//    if (countDotRemoved > 0) {
//
//        Dot *dot = listDot[col][row];
//        if (dot){
//            int yNew = -(countDotRemoved * this->GAP);
//
//            if (dot->getNameDot() != 0 && dot->getNameDot() < 6 ) {
//
//                auto rowDot = dot->getRow();
//                auto move = MoveBy::create(TIME_RUN, Vec2(0, yNew));
//                //                            auto move_ease_out = EaseBackInOut::create(move);
//                //                            EaseQuadraticActionIn
//                auto move_ease_out = EaseBounceOut::create(move);
//                dot->runAction(move_ease_out);
//                dot->setRow(rowDot - countDotRemoved);
//                auto dotY = dot->getY();
//                dot->setY(dotY + yNew);
//                this->listDot[col][row - countDotRemoved] = dot;
//            }else{
//                CCLOG("Name Dot: %d" + dot->getNameDot());
//            }
//
//        }
//    }
}

void LayerMainGame::getScore(){
    auto score = 0;
    for (auto col = 0; col < listDotRemoved.size(); col++) {
        score += listDotRemoved[col].size();
    }
    
    GameManager::getInstance()->plusScore(score);
    
    int scoreNew = GameManager::getInstance()->getScoreValue();
    CCLOG("SCORE %s", std::to_string(scoreNew).c_str());
    
//    this->listDotRemoved.clear();
}

void LayerMainGame::removeDot(Dot * dot){
//    dot->removeFromParent();
    
    auto row = dot->getRow();
    auto col = dot->getCol();
    dot->setNameDot(0);
    this->listDotRemoved[col].push_back(dot);
    
}

void LayerMainGame::drawnNode(int colorDot){

    
    if (drawNode != nullptr) {
        this->pnlMainGame->removeChild(drawNode);
    }
    this->drawNode = DrawNode::create();
    
    drawNode->drawSegment(this->position1, this->position2, 20.0f, getColorDraw(colorDot));
    this->pnlMainGame->addChild(drawNode, 1);
}


void LayerMainGame::drawNodeConnector(Dot *dot){
    
    this->runAnimationFadeOutDot(dot);
    auto drawNodeConnector =  DrawNode::create();
    drawNodeConnector->drawSegment(this->position1, this->position2, 20.0f, getColorDraw(dot->getNameDot()));
    arrayDrawConnector.push_back(drawNodeConnector);
    this->pnlMainGame->addChild(drawNodeConnector, 1);
}

void LayerMainGame::onEnter(){
    Layer::onEnter();
    
    
}

void LayerMainGame::createListDotRemove(){
    listDotRemoved.clear();
    for (auto i = 0; i < 6; i++) {
        std::vector<Dot *> arr;
        listDotRemoved.push_back(arr);
    }
    
}
void LayerMainGame::createDotArray(){
    
}

Color4F LayerMainGame::getColorDraw(int name){
    switch (name) {
        case 5:
            return Color4F(Color3B(237, 97, 110));
            
        case 2:
            return Color4F(Color3B(234, 194, 10));
            
        case 4:
            return Color4F(Color3B(110, 168, 232));
            
        case 1:
            return Color4F(Color3B(40, 205, 137));
            
        case 3:
            return Color4F(Color3B(154, 98, 223));
            
        default:
            break;
    }
}

void LayerMainGame::gameOver(){
    if (drawNode != nullptr) {
        this->pnlMainGame->removeChild(drawNode);
    }
    for (auto i = 0; i < arrayDrawConnector.size(); i++) {
        this->pnlMainGame->removeChild(arrayDrawConnector[i]);
    }
    
    
    
    pnlMainGame->setBackGroundImageOpacity(0);
    
    animationFadeOutAnimal();
    
    
    auto actionRunTop = MoveBy::create(0.1f, Vec2(0, -100));
    auto actionRunTop2 = MoveBy::create(0.2f, Vec2(0, 600));
    pnlTop->runAction(Sequence::create(actionRunTop, actionRunTop2, NULL));
    auto actionRunBot = MoveBy::create(0.1f, Vec2(0, 100));
    auto actionRunBot2 = MoveBy::create(0.2f, Vec2(0, -600));
    pnlBottom->runAction(Sequence::create(actionRunBot, actionRunBot2, NULL));
    
    
    
    auto action = ScaleTo::create(0.2f, 0);
    auto actionZoom = ScaleTo::create(0.2f, 1.5);
    
    if (layerMenuGameVisible) {
        auto sequence = Sequence::create(actionZoom, action, NULL);
        layerMenuGame->runAction(sequence);
    }
    
    this->list->setEnabled(false);
    this->schedule(schedule_selector(LayerMainGame::goToSceneGameOver), 0.5f);
    
}

void LayerMainGame::reBuildAnimal(float dt){
    
    if (typeGame == GAME_TIME) {
        this->unschedule(schedule_selector(LayerMainGame::countDownTimer));
    }
    this->listDot.clear();
    createGameNode();
    
    if (typeGame == GAME_TIME){
        schedule(schedule_selector(LayerMainGame::countDownTimer), 1.0f);
    }
    
    this->unschedule(schedule_selector(LayerMainGame::reBuildAnimal));
}


void LayerMainGame::animationFadeOutAnimal(){
    for (auto col = 0; col < listDot.size(); col++) {
        for (auto row = 0; row < listDot[col].size(); row++) {
            Dot * dot = dynamic_cast<Dot *>(listDot[col][row]);
            if(dot){
                auto action = ScaleTo::create(0.2f, 0);
                auto actionZoom = ScaleTo::create(0.2f, 1.5);
                dot->runAction(Sequence::create(actionZoom, action, NULL));
            }
        }
    }
}


void LayerMainGame::goToSceneGameOver(float dt){
    if (mCallBack && mCallBackListener){
        //mCallBack
        Node *param = Node::create();
        (mCallBack->*mCallBackListener)(param);
//        this->removeFromParent();
    }
    
    
}

void LayerMainGame::updateScore(){
    this->lblScore->setString(std::to_string(GameManager::getInstance()->getScoreValue()));
}

void LayerMainGame::countDownTimer(float dt){
//    auto mode = ;
    timerCountDown -= 1;
    
    if (timerCountDown < 0) {
        CCLOG("StopGame");
        list->setEnabled(false);
        this->unschedule(schedule_selector(LayerMainGame::countDownTimer));
        this->gameOver();
        return;
    }
    updateTimer();
    
}

void LayerMainGame::countDownMove(){
    this->move -= 1;
    updateMove();
    if (move <= 0) {
        CCLOG("StopGame");
        list->setEnabled(false);
        this->gameOver();
        return;
    }
    
   
}
void LayerMainGame::updateTimer(){
    std::string mode = "Time: " + std::to_string(timerCountDown);
    this->lblModeGame->setString(mode);
}

void LayerMainGame::updateMove(){
    std::string mode = "Move: " + std::to_string(move);
    this->lblModeGame->setString(mode);
}


void LayerMainGame::onButtonStop(Ref *pSender){
    
    showMenuGame();
    
    pnlMainGame->setVisible(false);
    list->setEnabled(false);
    btnBack->setEnabled(false);
    
    if (typeGame == GAME_TIME) {
        this->unschedule(schedule_selector(LayerMainGame::countDownTimer));
    }
    
    
}

void LayerMainGame::onTapedBtnTime(Ref *pSender){
   int totalItemTime = GameManager::getInstance()->getItemByNameInt(ITEM_TIME);
    if (totalItemTime > 0) {
        switch (this->typeGame) {
            case GAME_FREE:
                break;
            case GAME_TIME:
                this->timerCountDown += POINT_FOR_PLUS_TIME;
                this->updateTimer();
                GameManager::getInstance()->setMinusItem(ITEM_TIME);
                this->updateLblItem();
                btnTime->setEnabled(false);
                break;
            case GAME_MOVES:
                this->move += POINT_FOR_PLUS_MOVE;
                this->updateMove();
                GameManager::getInstance()->setMinusItem(ITEM_TIME);
                this->updateLblItem();
                btnTime->setEnabled(false);
                break;
            default:
                break;
        }
    }else{
        showPopUpBuyItem("Time", NOTI_TIME, "5 for 500", "res/NewFolder/Asset 18.png", ITEM_TIME);
    }
        
}

void LayerMainGame::onTapedBtnShrinker(Ref *pSender){
    if (btnShinkerClicked) {
        btnShinkerClicked = false;
        removeSpriteUsingItem();
    }else{
        if(GameManager::getInstance()->getItemByNameInt(ITEM_SHRINKER) > 0){
            btnShinkerClicked = true;
            btnExpanderClicked = false;
            showRecomentUsingItem();
        }else{
            showPopUpBuyItem("Shrinker", NOTI_SHRINKER, "10 for 100", "res/NewFolder/Asset 17.png", ITEM_SHRINKER);
        }
    }
}
void LayerMainGame::onTapedBtnExpander(Ref *pSender){
//    btnExpander->setPressedActionEnabled(false);
    if (btnExpanderClicked) {
        btnExpanderClicked = false;
        removeSpriteUsingItem();
    }else{
        if(GameManager::getInstance()->getItemByNameInt(ITEM_EXPANDER) > 0){
            
            btnExpanderClicked = true;
            btnShinkerClicked = false;
            showRecomentUsingItem();
        }else{
            showPopUpBuyItem("Expander", NOTI_EXPANDER, "5 for 500", "res/NewFolder/Asset 16.png", ITEM_EXPANDER);
            
        }
    }
}

void LayerMainGame::showPopUpBuyItem(string title, string des, string money, string imgLink,string typeItem){
    LayerPopupBuyItem *layer = LayerPopupBuyItem::create();
    layer->setContent(title, des, money, imgLink, typeItem);
    layer->setCallBack(this, callfuncO_selector(LayerMainGame::callBackFromBuyItem));
    layer->setCallBackCancel(this,callfuncO_selector(LayerMainGame::callBackCancel));
    this->addChild(layer, 10);
    if (typeGame == GAME_TIME) {
        this->unschedule(schedule_selector(LayerMainGame::countDownTimer));
    }
}
void LayerMainGame::timerForDoubleClickDot(float dt){

    dotSelected->setClick(false);
    this->unschedule(schedule_selector(LayerMainGame::timerForDoubleClickDot));
}


void LayerMainGame::timerForHidenAnimation(float dt){
    this->pnlMainGame->removeChild(layerAnimation);
    this->unschedule(schedule_selector(LayerMainGame::timerForHidenAnimation));
}

void LayerMainGame::showMenuGame(){
    layerMenuGameVisible = true;
    
//    if(GameManager::getInstance()->getTypeGame() == GAME_FREE){
//        layerMenuGame = CSLoader::getInstance()->createNode("LayerMenuEndless.csb");
//    }else{
        layerMenuGame = CSLoader::getInstance()->createNode("LayerMenuGame.csb");
//    }
    
    this->addChild(layerMenuGame);
    layerMenuGame->setPosition(Vec2(0, 0));
    
    auto pnlMain = (Layout *)layerMenuGame->getChildByName("pnlMain");
    if (pnlMain) {
        auto btnUnpause = (Button *)pnlMain->getChildByName("btnUnpause");
        btnUnpause->addClickEventListener(CC_CALLBACK_1(LayerMainGame::unPause, this));
        auto btnRestart = (Button *)pnlMain->getChildByName("btnRestart");
        if(btnRestart){
            btnRestart->addClickEventListener(CC_CALLBACK_1(LayerMainGame::reStart, this));
        }
        auto btnExit = (Button *)pnlMain->getChildByName("btnExit");
        btnExit->addClickEventListener(CC_CALLBACK_1(LayerMainGame::backMain, this));
//        auto btnSetting = (Button *)pnlMain->getChildByName("btnSetting");
//        btnSetting->addClickEventListener(CC_CALLBACK_1(LayerMainGame::btnSetting, this));
    }
//    btnTime->setTouchEnabled(false);
//    btnShrinker->setTouchEnabled(false);
//    btnExpander->setTouchEnabled(false);
}

void LayerMainGame::startGame(float dt){
    GameManager::getInstance()->setScoreValue(0);
    this->typeGame = GameManager::getInstance()->getTypeGame();
    if (typeGame == GAME_MOVES) {
        this->move = 120;
        this->updateMove();
    }else if (typeGame == GAME_TIME){
        this->timerCountDown = 120;
        this->updateTimer();
        this->schedule(schedule_selector(LayerMainGame::countDownTimer), 1.0f);
    }else if (typeGame == GAME_FREE){
        this->btnTime->setEnabled(false);
    }
    this->unschedule(schedule_selector(LayerMainGame::startGame));
    this->updateScore();
    this->updateLblItem();
}

void LayerMainGame::btnSetting(Ref * pSender){
    layerSetting = LayerSetting::create();
    layerSetting->setLayerFromMainGame();
    layerSetting->setCallBack(this, callfuncO_selector(LayerMainGame::callBackFromSetting));
    layerSetting->fromMainGame = true;
    this->pnlTop->setVisible(false);
    this->pnlBottom->setVisible(false);
    this->layerMenuGame->setVisible(false);
    this->addChild(layerSetting, 5);
}

void LayerMainGame::callBackFromBuyItem(Ref * pSender){
    this->updateLblItem();
}
void LayerMainGame::callBackCancel(Ref * pSender){
    if(GameManager::getInstance()->getTypeGame() == GAME_TIME){
        schedule(schedule_selector(LayerMainGame::countDownTimer), 1.0f);
    }
}

void LayerMainGame::callBackFromSetting(Ref * pSender){
    if(GameManager::getInstance()->getSound()){
        if(!audio->isBackgroundMusicPlaying()){
            audio->playBackgroundMusic("Caketown 1.mp3", true);
        }
    }else{
        if(audio->isBackgroundMusicPlaying()){
            audio->stopAllEffects();
            audio->stopBackgroundMusic();
        }
    }
    this->pnlTop->setVisible(true);
    this->layerMenuGame->setVisible(true);
    this->pnlBottom->setVisible(true);
    this->removeChild(layerSetting);
}
void LayerMainGame::unPause(Ref * pSender){
    if (typeGame == GAME_TIME){
        schedule(schedule_selector(LayerMainGame::countDownTimer), 1.0f);
    }
    list->setEnabled(true);
    pnlMainGame->setVisible(true);
    btnBack->setEnabled(true);
    this->removeChild(layerMenuGame);
    layerMenuGameVisible = false;
    btnTime->setTouchEnabled(true);
    btnShrinker->setTouchEnabled(true);
    btnExpander->setTouchEnabled(true);
}
void LayerMainGame::reStart(Ref * pSender){
    
    if (mCallBack && mCallBackListener2) {
        Node *param = Node::create();
        (mCallBack->*mCallBackListener2)(param);
        this->removeFromParent();
    }
    layerMenuGameVisible = false;
    
//    auto scene = GameScene::createScene();
//    Director::getInstance()->replaceScene(scene);
//    this->release();
    
    
//    LayerMainGame();
}
void LayerMainGame::backMain(Ref * pSender){
    gameOver();
    
//    auto scene = MainScene::createScene();
//    Director::getInstance()->replaceScene(scene);
}
void LayerMainGame::setCallBackRestart(Ref * target, SEL_CallFuncO listener){
    mCallBack = target;
    mCallBackListener2 = listener;
}

void LayerMainGame::setCallBack(Ref * target, SEL_CallFuncO listener){
    mCallBack = target;
    mCallBackListener = listener;
}

bool LayerMainGame::checkCanEat(){
    for (int col = 0; col < listDot.size(); col++) {
        int nameDot1;
        int nameDotTop;
        int nameDotRight;
        
        for (int row = 0; row < listDot[col].size(); row++) {
            Dot *dot = listDot[col][row];
             nameDot1 = dot->getNameDot();
            
            /// Check dot trên vs dưới. Thằng top thì k check với thằng trên.
            if (row < (listDot[col].size() - 1) ) {
                Dot *dotRow = listDot[col][row + 1];
                nameDotTop = dotRow->getNameDot();
                if (nameDot1 == nameDotTop) {
                    return true;
                }
            }
            
            //check dot trái vs phải. thằng col cuối k check với thằng bên trái.
            if (col < (listDot.size() - 1) ) {
                Dot *dotCol = listDot[col + 1][row];
                nameDotRight = dotCol->getNameDot();
                if (nameDot1 == nameDotRight) {
                    return true;
                }
            }
            
        }
        
        
    }
    return false;
}


void LayerMainGame::dismisAlert(Ref *pSender){
    this->list->setEnabled(true);
}

void LayerMainGame::removeSpriteUsingItem(){
    for (auto col = 0; col < listSpriteItem.size() ; col++) {
        for (auto row = 0 ; row < listSpriteItem[col].size(); row++) {
            this->pnlMainGame->removeChild(listSpriteItem[col][row]);
        }
    }
    listSpriteItem.clear();
    
}



void LayerMainGame::setAni(Ref *pSender){
    auto row = std::atoi(tfRow->getStringValue().c_str());
    auto name = std::atoi(tfName->getStringValue().c_str());
    auto col = std::atoi(tfCol->getStringValue().c_str());
    
    
    if (row >5 || row < 0||col >5 || col < 0) {
        return;
    }
    
    if (name >5 || name < 1) {
        return;
    }
    auto dot = listDot[col][row];
    dot->setNameDot(name);
    dot->reload();
    
}
