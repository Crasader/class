#ifndef __LayerPopupChonDongTien_h__
#define __LayerPopupChonDongTien_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupChonDongTien: public cocos2d::Layer{
public:
	LayerPopupChonDongTien();
	~LayerPopupChonDongTien();
    virtual bool init();
	virtual bool init(int _gameId);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonMocTouched(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonHaoTouched(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    
	static LayerPopupChonDongTien* create();
	static LayerPopupChonDongTien* create(int _gameId);
private:
	int gameId;
};

#endif /* LayerPopupChonDongTien_h */
