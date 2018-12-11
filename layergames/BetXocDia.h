#ifndef BetXocDia_h__
#define BetXocDia_h__
#include "cocos-ext.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;
class BetXocDiaChanLe : public Button{
public:
	BetXocDiaChanLe(string _type);
	~BetXocDiaChanLe();
	virtual bool init();
	static BetXocDiaChanLe* create(string _type);
private:
	string type;
};
class BetXocDiaDiem :public Button{
public:
	BetXocDiaDiem(string _type);
	~BetXocDiaDiem();
	virtual bool init();
	static BetXocDiaDiem* create(string _type);
private:
	string type;
};

#endif // BetXocDia_h__
