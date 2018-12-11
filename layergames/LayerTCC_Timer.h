#ifndef LayerTCC_Timer_h__
#define LayerTCC_Timer_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class LayerTCC_Timer :public cocos2d::Layer
{
public:
	LayerTCC_Timer();
	~LayerTCC_Timer();
	CREATE_FUNC(LayerTCC_Timer);
	virtual bool init();

	CC_SYNTHESIZE(int, timeBet, TimeBet);
	CC_SYNTHESIZE(int, timePrepare, TimePrepare);

	void startTimeBet();
	void startTimePrepare();
	void stopTimeBet();
	void stopTimePrepare();
	void setCallBackFromTimer(const std::function<void()> &listener);

	virtual void onEnter();
	virtual void onExit();
private:
	Sprite* imgBet;
	Label* lblBetTimeCount;
	int timeBetBuff = 30;

	Sprite* imgPrepare;
	Label* lblPrepareTimeCount;
	int timePrepareBuf = 10;

	void createLayerBetCount(Vec2 pos);
	void createLayerPrepareCount(Vec2 pos);

	void scheduleTimeBet(float dt);
	void scheduleTimePrepare(float dt);
	std::function<void()> listener;
};


#endif // LayerTCC_Timer_h__
