#ifndef LayerPopupMailDetails_h__
#define LayerPopupMailDetails_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../GameServer.h"
#include "../Common.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
class LayerPopupMailDetails : public Layer, public PlayerCallBack
{
public:
	LayerPopupMailDetails();
	~LayerPopupMailDetails();


	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	virtual bool init();
	CREATE_FUNC(LayerPopupMailDetails);

	void onButtonClose(Ref* pSender);
// 	void onButtonReply(Ref* pSender);
 	void onButtonDelete(Ref* pSender);
	void setDatas(int id, std::string sender, std::string date, std::string content);

	CC_SYNTHESIZE(int, mIDMail, IDMail);

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	cocos2d::EventListener* _touchListener;
	Text* lblMailContent;
	Text* lblDatetime;
// 	ui::Button* btnReply;
 	ui::Button* btnDelMail;

	void onLabelMailTouch(Ref* pSender, ui::Widget::TouchEventType type);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
};

#endif // LayerPopupMailDetails_h__
