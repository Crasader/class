#ifndef LayerPopupSearchRoom_h__
#define LayerPopupSearchRoom_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
class LayerPopupSearchRoom : public Layer
{
public:
	LayerPopupSearchRoom();
	~LayerPopupSearchRoom();
	virtual bool init();
	CREATE_FUNC(LayerPopupSearchRoom);

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

private:
	TextField* txtRoomNumber;
	void onButtonHuy(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonDongy(Ref* pSender, ui::Widget::TouchEventType type);


// private:
// 	cocos2d::EventListener* _touchListener;
// 	Text* lblMessage;
// 	Text* lblTitle;
// 	SEL_CallFuncO mCallBackListener;
// 	Ref *mCallBack;
// 	int tagcallback;
};

#endif // LayerPopupSearchRoom_h__
