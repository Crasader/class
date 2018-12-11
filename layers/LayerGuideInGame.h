#ifndef LayerGuideInGame_h__
#define LayerGuideInGame_h__

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
class LayerGuideInGame : public Layer, public PlayerCallBack
{
public:
	LayerGuideInGame();
	~LayerGuideInGame();


	virtual bool init();
	void btnClose(Ref* pSender);
    void setTitle(string str);

    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	int gameID;
    void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	//LayerGuideInGame* create();
	//void create() override;
	CREATE_FUNC(LayerGuideInGame);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	cocos2d::experimental::ui::WebView *webView;
	bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
    void setLink(string url);
    Text* lblTitle;
};

#endif // LayerGuideInGame_h__
