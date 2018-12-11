#ifndef LayerStep_h__
#define LayerStep_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;
class LayerStep : public cocos2d::Layer{
public:
	enum TYPE_STEP{
		TIME = 0,
		CHAP_TIEN
	};
	virtual bool init() override;
	LayerStep();
	~LayerStep();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerStep);
    
	void loadData();
	void setData(vector<int> _data);
	void setTypeStep(TYPE_STEP _type);
	int getValueConfig();
	//update 11/7
	void setSelectedIndex(int idx);
	void loadIndexFromValue(int value);
private :
	Text* lblTime;
	string subText;
	vector<int> dataStep;
	int jumpUnit;//neu la dang time se nhan 60
	int currIdx = 0;

	void onButtonSub(Ref* pSender);
	void onButtonAdd(Ref* pSender);
	void loadLabel();
	//update 25/7
	int typeStep = 0;//type step
};

#endif 



