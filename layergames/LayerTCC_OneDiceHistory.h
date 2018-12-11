#ifndef LayerTCC_OneDiceHistory_h__
#define LayerTCC_OneDiceHistory_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class LayerItemTCC_OneDicesHis :public Layer{
public:
	LayerItemTCC_OneDicesHis(int _id);
	~LayerItemTCC_OneDicesHis();
	static LayerItemTCC_OneDicesHis* create(int _id);
	virtual bool init();
	void setDataDice(string _result);
private:
	ImageView* imgDice;
	Text* lblCount;
	int _id;
};

class LayerTCC_OneDiceHistory :public Layer{
public:
	CREATE_FUNC(LayerTCC_OneDiceHistory);
	virtual bool init();
	void setDataHis(string _data);
private:
	void createDices();
};


#endif // LayerTCC_OneDiceHistory_h__
