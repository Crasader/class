#ifndef __iCasino_v2__LayerPromotionItemProgress__
#define __iCasino_v2__LayerPromotionItemProgress__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class LayerPromotionItemProgress : public cocos2d::Layer
{
public:
	LayerPromotionItemProgress();
	~LayerPromotionItemProgress();
	CC_SYNTHESIZE(int, nGameID, GameID);

	void onButtionPress(Ref* _ref);

	void setDatas(string sBody, string sProgress, int proID, int gameID);

	virtual bool init();
	CREATE_FUNC(LayerPromotionItemProgress);
private:
	Text* lblPromoBody;
	Text* lblProgressInfo;
	Button* btnClainReward;
};
#endif //__iCasino_v2__LayerPromotionItemProgress__

