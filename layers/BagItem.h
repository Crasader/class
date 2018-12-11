#ifndef BagItem_h__
#define BagItem_h__

#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;
using namespace std;
class BagItem: public Layer
{
public:
	Label* labelNumber;
	virtual bool init();
	void setNumber(string msg);
    void runActionNoti();
	CREATE_FUNC(BagItem);
private:
    Sprite* bg;
};

#endif // BagItem_h__
