#ifndef LayerPopupKingCheck_h__
#define LayerPopupKingCheck_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "CommonChess.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerPopupKingCheck : public cocos2d::LayerColor
{
public:
	virtual bool init() override;
	LayerPopupKingCheck();
	~LayerPopupKingCheck();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerPopupKingCheck);
	void showKingCheck(string nameCurrTurn);
	void reset();
	CC_SYNTHESIZE(int, coutCheck, CountCheck);//chi hien 1 lan check cua doi phuong
	CC_SYNTHESIZE(int, coutMeCheck, CoutMeCheck);//chi hien 1 lan check cua minh
	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
private:
	Size visibleSize;
	Sprite* spChieu;
	void callbackAnim(Node *sender);
	string nameCurrentTurn;
	EventListenerTouchOneByOne* _listener;
};

#endif // LayerPopupKingCheck_h__



