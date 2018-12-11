#ifndef __iCasino_v2__LayerSettingMain__
#define __iCasino_v2__LayerSettingMain__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "ui/CocosGUI.h"
#include "Requests/LogoutRequest.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class LayerSettingMain : public cocos2d::Layer
{
public:
	LayerSettingMain();
	~LayerSettingMain();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void selectedEvent(Ref *pSender, CheckBox::EventType type);
	void onButtonCloseClicked(Ref* pSender);
	void onBtnLogout(Ref* pSender);
	virtual bool init();
	CREATE_FUNC(LayerSettingMain);
private:
	int gameID;
	cocos2d::EventListener* _touchListener;
	CheckBox *_buttonToggleMusic;
	CheckBox *_buttonToggleRung;
	cocos2d::ui::Button *_btnClose;
};
#endif //__iCasino_v2__LayerPopupSetting__

