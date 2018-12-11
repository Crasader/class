#ifndef __iCasino_v2__LayerChickenPopup__
#define __iCasino_v2__LayerChickenPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/UIScrollView.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;

class LayerChickenPopup : public cocos2d::Layer
{
public:
	LayerChickenPopup();
	~LayerChickenPopup();

	void loadAllDatas();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	virtual bool init();
	CREATE_FUNC(LayerChickenPopup);
private:
	cocos2d::EventListener* _touchListener;
	ListView* mListViewChicken;
};
#endif //__iCasino_v2__LayerChickenPopup__

