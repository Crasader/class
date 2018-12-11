#ifndef LayerFriendTop_h__
#define LayerFriendTop_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class LayerFriendTop : public Layer
{
public:
	LayerFriendTop();
	~LayerFriendTop();

	void onButtonThongtin(Ref* pSender);
	void onButtonMoiBanBe(Ref* pSender);
	void onButtonBack(Ref* pSender);

	bool init();
	CREATE_FUNC(LayerFriendTop);
private:
	cocos2d::EventListener* _touchListener;
	Button* btnThongtin;
	Button* btnInvite;
};

#endif // LayerFriendTop_h__
