#pragma once
#ifndef LayerGa_h__
#define LayerGa_h__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "LayerListGa.h"
#include "LayerListGa7User.h"
#include "../SceneManager.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;

class LayerGa: public Layer
{
private:
	//SpriteBatchNode *spriteBatchNode;
	Label *labelTienGa;
	Label *labelLv;
	ImageView *spriteGa;
	void clickSpriteGa(Ref *pSender, Widget::TouchEventType type);
	vector<string> listGa;
	SEL_CallFunc mCallBackListener;
	Ref *mCallBack;
	double _tienGa;
    LayerListGa7User* layerListga7u;
    LayerListGa* layerListga;
	Sprite* toastNotifi;
public:
	enum CONFIG
	{
		OFF,
		ON
	};
	LayerGa(void);
	~LayerGa(void);
	virtual bool init();
	CREATE_FUNC(LayerGa);
	void runActionAnGa(Point position, float duration);
	void actionCallBack(Node *pSender);
	void resetAll();
	void setChickenMoney(double money);
	double getChickenMoney();
	void setListUser(string listUser,char c1, char c2);
	void setListDirect(vector<string> listUSer);
	void setChickenLv(int _lv);
	void callBackFromActionGa(Ref *target, SEL_CallFunc selector);
	void setConfigChicken(int isOn);
	bool getConfigChicken();
	CC_SYNTHESIZE(bool, isNuoi, NuoiGa);
	CC_SYNTHESIZE(int, lv, LevelChicken);
	vector<string> getListGa();
	void showListChicken();
	void showToast(bool isshow);
};

#endif // LayerGa_h__
