//
//  _Background_inGame_.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/16/14.
//
//

#ifndef __iCasino_v2___counter___
#define __iCasino_v2___counter___


#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
class LayerCount : public Layer
{
private:
	ImageView*	Ready;
	ImageView*	TurnUp;

	Label* pLabelTurnUp;
	Label* pLabelReady;

	int pCountReady;
	int pCountTurnUp;

	// Selector Count Down Timer
	void CountDownTurnUp(float dt);
	void CountDownReady(float dt);

public:
	LayerCount();
	~LayerCount();
	bool init();
	CREATE_FUNC(LayerCount);

	// Đếm ngược thời gian ready
	void StartCountDownReady();
	void StopCountDownReady();
	void setTitleReady(string title);

	// Đếm ngược thời gian lật bài
	void StartCountDownTurnUp();
	void StopCountDownTurnUp();

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
};

#endif
