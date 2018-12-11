#ifndef LayerPopupBattle_h__
#define LayerPopupBattle_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerPopupBattle : public cocos2d::Layer
{
public:
	virtual bool init() override;
	LayerPopupBattle();
	~LayerPopupBattle();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerPopupBattle);
private:
};

#endif // LayerPopupBattle_h__



