#ifndef LayerPopupLose_h__
#define LayerPopupLose_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "../GameServer.h"
#include "CommonChess.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerPopupLose : public cocos2d::Layer,
	public PlayerCallBack
{
public:
	virtual bool init() override;
	LayerPopupLose();
	~LayerPopupLose();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerPopupLose);

protected:
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	void onButtonBattle(Ref* pSender);
	void onButtonCancel(Ref* pSender);
};

#endif // LayerPopupLose_h__



