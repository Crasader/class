#ifndef __iCasino_v2__LayerRank__
#define __iCasino_v2__LayerRank__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LayerRankContent.h"
#include "LayerRankTop.h"
USING_NS_CC;
USING_NS_CC_EXT;
class LayerRank : public cocos2d::Layer
{
public:
	LayerRank();
	~LayerRank();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	virtual bool init();
	void showTopCaoThu();
	void showTopDaiGia();

	CREATE_FUNC(LayerRank);
private:
	cocos2d::EventListener* _touchListener;
	int const TAG_CHILD = 111;
    
    bool isWebView;

	LayerRankContent* layerRankContent;
	LayerRankTop* layerRankTop;
	void onExitLayer();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
};
#endif //__iCasino_v2__LayerRecharge__