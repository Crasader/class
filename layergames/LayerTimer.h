//
//  _Background_inGame_.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/16/14.
//
//

#ifndef __iCasino_v2___Timer___
#define __iCasino_v2___Timer___


#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
class LayerTimer : public Layer
{
private:
	const int w_number = 175;
	const int w_timer = 713;
	Size sizeAd;
	Size winSize;
	bool isRunning;
	int gameVersion = 1;
	LabelBMFont* labelCount;
public:
// 	ImageView* timer;
// 	ImageView* timer2;
	ImageView *title;
	int count;
	int tg;
    bool init();
    CREATE_FUNC(LayerTimer);
	LayerTimer();
	~LayerTimer();

	void setCount(int _count);
	void startTimer();
	void stopTimer();
	void changeNumber(float dt);
	 virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
};

#endif
