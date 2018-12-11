//
//  LayerLoading.h
//  TestCpp
//
//  Created by leafsoar on 7/29/13.
//
//

#ifndef TestCpp_LayerLoading_h
#define TestCpp_LayerLoading_h

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LayerLoading: public Layer{
public:
	LayerLoading();
	~LayerLoading();
	ProgressTimer* timer;

	virtual bool init();
	CREATE_FUNC(LayerLoading);

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void setProgress(float value);
	void start(float duration);
	void stop();
private:
	LabelBMFont *mTextLoading;
	void stopLoading();
	void onScheduleLoading(float dt);
	Sprite* lightProgress;
	EventListenerTouchOneByOne* _listener;

	int startX = 10;
	void createLoading(int gamever);
};

#endif