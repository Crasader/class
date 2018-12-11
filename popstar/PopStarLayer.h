#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class PopStar;
using namespace cocos2d;
enum GUI_EVENT_TYPE
{
    EVENT_UPDATE_SCORE,					// 刷新分数
    EVENT_UPDATE_TARGET_SCORE,			// 刷新目标分数
    EVENT_UPDATE_LEVEL,					// 刷新关卡
    EVENT_UPDATE_REDUCE_SCORE,			// 刷新消除分数
    EVENT_UPDATE_TOTAL_HISTORY_SCORE,	// 刷新历史最高分
    EVENT_UPDATE_LEVEL_HISTORY_SCORE,	// 刷新本关最高分
};

class PopStarLayer : public cocos2d::Layer
{
public:
    ~PopStarLayer();
    EventListener* _touchListener;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnter();
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onClickMenu(Ref* obj);
    void update(float delta);
    
    void onGuiEvent(GUI_EVENT_TYPE event, int nValue, unsigned int uValue = 0);
    
    // implement the "static node()" method manually
    CREATE_FUNC(PopStarLayer);
    
    
public:
    Label* historyTotalScore;	// 历史最高分
    Label* historyLevelScore;	// 本关最高分
    Label* gameLevel;			// 关卡
    Label* targetScore;		// 目标
    Label* curScore;			// 分数
    Label* reduceScore;
    
private:
    PopStar*	popStar;
};

#endif // __HELLOWORLD_SCENE_H__
