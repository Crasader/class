#ifndef LayerPopupHotLine_h__
#define LayerPopupHotLine_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
class LayerPopupHotLine : public Layer,
	public	PlayerCallBack
{
public:
	LayerPopupHotLine();
	~LayerPopupHotLine();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void onBtnHotLine(Ref* pSender);
    void onBtnClose(Ref* pSender);
    void onBtnFB(Ref* pSender);
    virtual bool init();
	CREATE_FUNC(LayerPopupHotLine);

private:

	Button *btnHotLine1, *btnHotLine2,*btnFP;

};
#endif // LayerPopupHotLine_h__
