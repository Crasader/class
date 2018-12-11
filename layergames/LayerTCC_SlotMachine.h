#ifndef LayerTCC_SlotMachine_h__
#define LayerTCC_SlotMachine_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class LayerTCC_SLotMachine :public Layer{
public:
	LayerTCC_SLotMachine();
	~LayerTCC_SLotMachine();
	CREATE_FUNC(LayerTCC_SLotMachine);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void setCallBack(const std::function<void()> &listener);
	void setCallBack(Ref* target, SEL_CallFuncND selector);

	void setShowSize(Size _size);
	void setSlotSize(Size _size);
	void insertSlot(vector<string> listRes, int repeatSlot);
	void setRewardItem(int idx);
	void setFirstSlot(int idx);
	void startTurn(float time);
	void setResPath(string path);
private:
	ui::ScrollView* scrollView;
	Size slotSize;
	string resPath;
	Layout* innerContainer;
	int numItem;
	int rewardItem;
	float rewardPosY;
	std::function<void()> listener;
	Ref* mTarget;
	cocos2d::SEL_CallFuncND mFunctionSelector;

	void caculatePosition();
	void onRunSlotDone();
};

#endif // LayerTCC_SlotMachine_h__
