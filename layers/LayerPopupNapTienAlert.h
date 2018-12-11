#ifndef LayerPopupNapTienAlert_h__
#define LayerPopupNapTienAlert_h__

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
class LayerPopupNapTienAlert : public Layer,
	public	PlayerCallBack
{
public:
	LayerPopupNapTienAlert();
	~LayerPopupNapTienAlert();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void onBtnNapTien(Ref* pSender);
    void onBtnClose(Ref* pSender);
    void setText(string msg);
    void setType(int _type);
    virtual bool init();
	CREATE_FUNC(LayerPopupNapTienAlert);

private:

	Button *btnNapTien;
    Text* txtMoTa;
    int type = 0;

};
#endif // LayerPopupNapTienAlert_h__
