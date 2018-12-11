#ifndef LayerFriend_h__
#define LayerFriend_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../layers/LayerFriendTop.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LayerFriend : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(LayerFriend);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
	LayerFriendTop* top;
};

#endif // LayerFriend_h__
