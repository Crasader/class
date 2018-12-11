#ifndef __iCasino_v2__LayerRankTop__
#define __iCasino_v2__LayerRankTop__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class LayerRankTop : public cocos2d::Layer
{
public:
	LayerRankTop();
	~LayerRankTop();

	void onButtonBack(Ref* pSender, Widget::TouchEventType type);
	void onButtonTopCaoThu(Ref* pSender, Widget::TouchEventType type);
	void onButtonTopDaiGia(Ref* pSender, Widget::TouchEventType type);

	virtual bool init();
	CREATE_FUNC(LayerRankTop);
public:
    bool isWebView;
private:
	Button* btnTopCaoThu;
	Button* btnTopDaiGia;
	Button* btnBack;
};
#endif //__iCasino_v2__LayerRechargeTop__

