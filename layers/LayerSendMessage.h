#ifndef __iCasino_v2__LayerSendMessage__
#define __iCasino_v2__LayerSendMessage__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/UIScrollView.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "../Common.h"
#include "../layergames/_Chat_.h"
#include "Requests/ExtensionRequest.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;

class LayerSendMessage : public cocos2d::Layer, public PlayerCallBack
{
public:
	LayerSendMessage();
	~LayerSendMessage();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void setSender(string sender);

	virtual bool init();
	CREATE_FUNC(LayerSendMessage);

	void sendReceiver(std::string name);
	void onBtnClose(Ref* pSender);
	void onBtnSend(Ref* pSender);

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

private:
	cocos2d::EventListener* _touchListener;
	cocos2d::ui::Button* _btnClose;
	TextField *txtSendTo, *txtContent;
	Text* lblPopup;
};
#endif //__iCasino_v2__LayerSendMessage__

