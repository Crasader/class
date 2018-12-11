#ifndef LayerTCC_DicesHistory_h__
#define LayerTCC_DicesHistory_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class LayerItemTCC_DicesHis : public Layer{
public:
	LayerItemTCC_DicesHis();
	~LayerItemTCC_DicesHis();
	CREATE_FUNC(LayerItemTCC_DicesHis);
	virtual bool init();

	void setDataDice(string _result);
	void loadFirst();
private:
	Node* root;
};

class LayerTCC_DicesHistory : public Layer
{
public:
	LayerTCC_DicesHistory();
	~LayerTCC_DicesHistory();
	CREATE_FUNC(LayerTCC_DicesHistory);
	virtual bool init();

	void setDataHis(string _data);
	void showFirst();
private:
	void createDices();
	vector<LayerItemTCC_DicesHis*> arrDices;
	cocos2d::ui::ScrollView* scrollView;
};



#endif // LayerTCC_DicesHistory_h__
