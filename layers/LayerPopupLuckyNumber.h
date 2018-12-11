#ifndef LayerPopupLuckyNumber_h__
#define LayerPopupLuckyNumber_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../SceneManager.h"
#include "cocostudio/CocoStudio.h"
#include "../Common.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class LayerPopupLuckyNumber : public Layer, public PlayerCallBack
{
public:
	LayerPopupLuckyNumber();
	LayerPopupLuckyNumber(int reasonCode, string cardCode, string seriNumber);
	//LayerPopupLuckyNumber(int reasonCode);
	~LayerPopupLuckyNumber();


	virtual bool init();
	void btnClose(Ref* pSender, ui::Widget::TouchEventType type);
	int gameID;
	int codeReason;
	string cardCode;
	string seriNumber;

	//LayerGuideInGame* create();
	//void create() override;
	//CREATE_FUNC(LayerPopupLuckyNumber);
	static LayerPopupLuckyNumber* create(int rescode, string _card, string _seri);
    void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	cocos2d::experimental::ui::WebView *webView;
	bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
    void setLink(string url);
};

#endif // LayerPopupLuckyNumber_h__
