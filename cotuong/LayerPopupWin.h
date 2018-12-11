#ifndef LayerPopupWin_h__
#define LayerPopupWin_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "../GameServer.h"
#include "CommonChess.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerPopupWin : public cocos2d::Layer,
	public PlayerCallBack
{
public:
	virtual bool init() override;
	LayerPopupWin();
	~LayerPopupWin();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerPopupWin);
protected:
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	void onButtonBattle(Ref* pSender);
	void onButtonCancel(Ref* pSender);
};

class LayerPopupPeace : public cocos2d::Layer
{
public:
	virtual bool init() override;
	LayerPopupPeace();
	~LayerPopupPeace();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerPopupPeace);
private:
	void onButtonBattle(Ref* pSender);
	void onButtonCancel(Ref* pSender);
};

#endif // LayerPopupWin_h__



