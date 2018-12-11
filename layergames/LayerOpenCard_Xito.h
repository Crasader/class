
#ifndef demo_LayerOpenCard_Xito_h
#define demo_LayerOpenCard_Xito_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include <string>
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::ui;

class LayerOpenCard_Xito :public cocos2d::Layer
{
private:
    string listDeal;
    float p_Timer;
	cocos2d::EventListener* _touchListener;

    void onChooseCard(Ref *sender, ui::Widget::TouchEventType pType);
public:

    LayerOpenCard_Xito(const string& lst, const float& timer);
    LayerOpenCard_Xito();
    virtual ~LayerOpenCard_Xito();

    static LayerOpenCard_Xito* create(const string& lst, const float& timer);
    bool init();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
    virtual void onExit() override;

	void SendOpenCards(const int& cardIdx);
};

#endif
