
#ifndef __Chan___Toast___
#define __Chan___Toast___

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include <iostream>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LayerToast : public Layer{
public:
	LayerToast(const string& msg);
	~LayerToast();
	static LayerToast* create(const string &pMes);

	virtual	bool init();
	void showToast(float duration);
private:
	std::string _message;
};

#endif
