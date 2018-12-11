#ifndef CoinXocDia_h__
#define CoinXocDia_h__
#include "cocos-ext.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;
class CoinXocDia : public Button{
public:
	CoinXocDia(string type, string normal, string disable);
	~CoinXocDia();
	static CoinXocDia* create(string type, string normal, string disable);
	virtual bool init();
	void SetSelected(bool state);
	string getType();
	int getCoinVal();
	string getSrc();
	Vec2 getPositionCoin();
private:
	string pType;
	string pImageNormal;
	string pImageDisable;
};

class LayerAnimationXocDia : public Layer{
public:
	LayerAnimationXocDia(Vec2 pos, int type);
	~LayerAnimationXocDia();
	virtual bool init();
	static LayerAnimationXocDia* create(Vec2 pos, int type);
	void setTarget(Ref* _target);
	void setCallBackListener(const std::function<void()> &listener);
private:
	Vec2 posFrame;
	Ref* fnTarget;
    int curType;

	void CallFuncRemove();
	std::function<void()> listener;
};

class LayerHistoryXocDia :public Layer{
public:
	LayerHistoryXocDia();
	~LayerHistoryXocDia();
	virtual bool init();
	CREATE_FUNC(LayerHistoryXocDia);
	void setListHistory(string lst);
	Button* getButtonClose();
private:
	vector<Sprite*> arrResult;
	Label* lblNumChan;
	Label* lblNumLe;
	Layout *pnlHistory;
	Button *btnClose;
};
#endif // CoinXocDia_h__
