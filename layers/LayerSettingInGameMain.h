#ifndef __iCasino_v4__LayerSettingInGameMain_h__
#define __iCasino_v4__LayerSettingInGameMain_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;

class LayerSettingInGameMain : public cocos2d::Layer
{
public:
	LayerSettingInGameMain();
	~LayerSettingInGameMain();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	virtual bool init();
	CREATE_FUNC(LayerSettingInGameMain);

	
	void onBtnCauhinh(Ref* pSender, Widget::TouchEventType type);
	void onBtnHuongDan(Ref* pSender, Widget::TouchEventType type);
	void onBtnClose(Ref* pSender, Widget::TouchEventType type);
	void show(bool isDisableCuoc);
	void hide();

private:
	cocos2d::EventListener* _touchListener;
	int gameID;
	ui::Button* btnCauhinh;
	ui::Button* btnHuongdan;
};
#endif //__iCasino_v2__LayerSendMessage__

