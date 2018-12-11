#ifndef __iCasino_v2__LayerPromotionItemNormal__
#define __iCasino_v2__LayerPromotionItemNormal__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "ui/CocosGUI.h"

#include "../GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class LayerPromotionItemNormal : public cocos2d::Layer
	, public PlayerCallBack
{
public:
	LayerPromotionItemNormal();
	~LayerPromotionItemNormal();

	void onButtonPress(Ref* _ref);

	void setIncreGoldType(int nID, int nType, string kmname, string kmdesc, int kmGold);
	void setReceiveGoldType(int nID, int nType, string kmname, string kmdesc);
	void setProgressType(int nID, int nType, string kmname, string kmdesc, int kmProgressNumber, string kmProgressText);
    void setIsUsed(bool isUsed);
    
	virtual bool init();
	CREATE_FUNC(LayerPromotionItemNormal);
private:
// 	Text* lblPromoBody;
// 	LoadingBar* ldbProgress;
// 	Text* lblProgress;
// 	Text* lblGold;

	Text* lblDetailPro;
	Button* btnClainReward;

	enum{
		tagReceiveMoney = 1,
		tagIncreGold = 2,
		tagProgress = 3
	};
	int typePromotion;
// 	Layout* pnlProgress;
// 	Layout* pnlBox;
// 	Layout* pnlGold;

	int nID;
	int nGameID;

	bool isMeSendMsg;
protected:
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
};
#endif //__iCasino_v2__LayerPromotionItemNormal__

