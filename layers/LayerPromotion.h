#ifndef __iCasino_v2__LayerPromotion__
#define __iCasino_v2__LayerPromotion__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/UIScrollView.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../PromotionHandler.h"

#include "LayerPromotionItemProgress.h"
#include "LayerPromotionItemNormal.h"
#include "network/HttpClient.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;

class LayerPromotion : public cocos2d::Layer
	, public PlayerCallBack
{
public:
	LayerPromotion();
	~LayerPromotion();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void onButtonClose(Ref* pSender);

	void loadAllDatas();
	void onHttpRequestCompleted(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
	Layout* createItemNormal(string body, int tagID);
	Layout* createItemProgress(string body, string sProgress, int tagID);

	virtual bool init();
	CREATE_FUNC(LayerPromotion);
private:
	cocos2d::EventListener* _touchListener;
	ListView* mListViewPromoItems;
protected:
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
};
#endif //__iCasino_v2__LayerPromotion__

