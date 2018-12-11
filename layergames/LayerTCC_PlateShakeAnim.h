#ifndef LayerTCC_PlateShakeAnim_h__
#define LayerTCC_PlateShakeAnim_h__

#include "cocos-ext.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class LayerTCC_PlateShakeAnim : public Layer{
public:
	LayerTCC_PlateShakeAnim(Vec2 pos, int type);
	~LayerTCC_PlateShakeAnim();
	virtual bool init();
	static LayerTCC_PlateShakeAnim* create(Vec2 pos, int type);
	void setTarget(Ref* _target);
	void setCallBackListener(const std::function<void()> &listener);
private:
    
	Vec2 posFrame;
	Ref* fnTarget;
	int curType;

	void CallFuncRemove();
	std::function<void()> listener;
};

#endif // LayerTCC_PlateShakeAnim_h__
