//
//  LayerMainGame.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/22/18.
//
#pragma once
#ifndef LayerMainGameGame_hpp
#define LayerMainGameGame_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "Dot.hpp"
#include "LayerMenuGame.hpp"
#include "LayerSetting.hpp"
#include "SimpleAudioEngine.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;
using namespace CocosDenshion;


static int RADIUS_DOT = 70;
static int COMMON_X = 79;
static int COMMON_Y = 79;
static double TIME_RUN = 0.3;

static Vec2 POSITION_PNL_GAME = Vec2(80, 80);

class LayerMainGame : public cocos2d::Layer
{
    
public:
    
    virtual bool init() override;
    LayerMainGame();
    ~LayerMainGame();
    
    
//    virtual void onEnter();
//    virtual void onExit();
    int typeGame;
    // implement the "static create()" method manually
    CREATE_FUNC(LayerMainGame);
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent)override;
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent)override;
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent)override;
    virtual void onEnter() override;
//    virtual void onExit() override;
    
    EventListenerTouchOneByOne* list;
    void setCallBack(Ref * target, SEL_CallFuncO listener);
    void setCallBackRestart(Ref * target, SEL_CallFuncO listener);
protected:
    
private:
    
    
    std::vector<DrawNode *> arrayDrawConnector;
    std::vector<Dot *> connectedDot;
    
    std::vector<std::vector<Dot*>> listDot;
    std::vector<std::vector<Dot*>> listDotRemoved;
    
    Dot *dotMeeting;
    
    
    
    Layout *pnlMain, *pnlMainGame, *pnlBottom, *pnlTop;
    LayerMenuGame *pnlMenuGame;
    
    Color4F getColorDraw(int name);
    
    void removeDot(Dot * dot);
    void drawNodeConnector(Dot *dot);
    void createGameNode();
    void createDotArray();
    void getScore();
    void gameOver();
    void createListDotRemove();
    void pushDot(Dot* dot);
    Vec2 position1;
    Vec2 position2;
    DrawNode *drawNode;
    void drawnNode(int colorDot);
    bool isInSingleState, isInSameState;
    int startCol, startRow, curCol, curRow, isClosedDot, closeIndex, GAP;
    void countDownTimer(float dt);
    void countDownMove();
    Text* lblScore, *lblModeGame, *lblItemTime, *lblItemShrinker, *lblItemExpander;
    void updateScore();
    GLView *glview;
    int timerCountDown, score, move;
    
    
    Button *btnTime, *btnShrinker, *btnExpander, *btnBack;
    void updateLblItem();
    void updateTimer();
    void updateMove();
    void onButtonStop(Ref *pSender);
    void onTapedBtnTime(Ref *pSender);
    void onTapedBtnShrinker(Ref *pSender);
    void onTapedBtnExpander(Ref *pSender);
    void endAction();
    bool btnExpanderClicked, btnShinkerClicked;
    Dot *firstDot, *dotSelected;
    void timerForDoubleClickDot(float dt);
    
    void startGame(float dt);
    void unPauseCallBack(Ref * pSender);
    
    SEL_CallFuncO mCallBackListener;
    SEL_CallFuncO mCallBackListener2;
    Ref *mCallBack;
    
    void countDownToGameOver(float dt);
    int timerCountDownGameOver;
    void animation(int dotName);
    Node *layerAnimation, *layerMenuGame;
    bool layerMenuGameVisible = false;
    
    
    void timerForHidenAnimation(float dt);
    
    void showMenuGame();
    void unPause(Ref * pSender);
    void reStart(Ref * pSender);
    void backMain(Ref * pSender);
    void btnSetting(Ref * pSender);
    
    void goToSceneGameOver(float dt);
    void removeSpriteAnimation(Node* pSender, Sprite *dot);
    
    void removeDotFromList(Node* pSender, Dot *dot);
    void moveDotToNewPosition(Node* pSender,int col, int row, int countDotRemoved);
    
    void runAnimationFadeOutDot(Dot *dot);
    
    void callBackFromSetting(Ref * pSender);
    LayerSetting *layerSetting;
    
    bool checkCanEat();
    
    void animationFadeOutAnimal();
    
    void reBuildAnimal(float dt);
    
    SimpleAudioEngine* audio;
    int idEffect;
    
    void dismisAlert(Ref *pSender);
    
    
    ///Tool
    
    TextField *tfRow,*tfCol, *tfName;
    void setAni(Ref *pSender);
    
    void showRecomentUsingItem();
    
    
    //List sprite using item;
    std::vector<std::vector<Sprite *>> listSpriteItem;
    
    
    void removeSpriteUsingItem();
    
    //show popup buy item
    void showPopUpBuyItem(string title, string des, string money, string imgLink,string typeItem);
    void callBackFromBuyItem(Ref * pSender);
    void callBackCancel(Ref * pSender);
};

#endif /* LayerMainGame_hpp */

