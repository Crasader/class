#ifndef __iCasino_v4__LayerPickGame__
#define __iCasino_v4__LayerPickGame__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LayerPickGame : public cocos2d::Layer
{
public:
	LayerPickGame();
	~LayerPickGame();

	virtual bool init();
	CREATE_FUNC(LayerPickGame);
};
#endif //__iCasino_v4__LayerPickGame__

