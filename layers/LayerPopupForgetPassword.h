#ifndef __LayerPopupForgetPassword_h__
#define __LayerPopupForgetPassword_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/NativeHelper.h"
#include "../scenes/SceneLogin.h"
#include "../Common.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
class LayerPopupForgetPassword : public Layer
{
public:
	LayerPopupForgetPassword();
	~LayerPopupForgetPassword();


	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void onButtonGetPassword(Ref* pSender);
	void onButtonClose(Ref* pSender);

	virtual bool init();
	CREATE_FUNC(LayerPopupForgetPassword);

	void scheduleUpdateContactInfo(float dt);
private:
	cocos2d::EventListener* _touchListener;
	Text* lblContact;
	
};

#endif // __LayerPopupForgetPassword_h__
