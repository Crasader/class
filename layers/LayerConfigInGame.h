#ifndef LayerConfigInGame_h__
#define LayerConfigInGame_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../SceneManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;

class LayerConfigInGame : public cocos2d::Layer
{
private:
	void selectedEvent(Ref *pSender, CheckBox::EventType type);
public:
	LayerConfigInGame();
	~LayerConfigInGame();

	virtual void onEnter() override;
	virtual void onExit() override;
	void onBtnFaceBook(Ref* pSender);
	void onBtnClose(Ref* pSender);
	virtual bool init();
	CREATE_FUNC(LayerConfigInGame);
	int gameId;

private:
	EventListenerTouchOneByOne* listener;
};
#endif // LayerConfigInGame_h__
