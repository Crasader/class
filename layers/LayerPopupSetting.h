#ifndef __iCasino_v2__LayerPopupSetting__
#define __iCasino_v2__LayerPopupSetting__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LayerPopupSetting : public cocos2d::Layer
{
public:
	LayerPopupSetting();
	~LayerPopupSetting();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void onButtonConfig(Ref* pSender);
	void onButtonSoNguoiChoi(Ref* pSender);
	void onButtonGuide(Ref* pSender);

	virtual bool init();
	CREATE_FUNC(LayerPopupSetting);
private:
	cocos2d::EventListener* _touchListener;
};
#endif //__iCasino_v2__LayerPopupSetting__

