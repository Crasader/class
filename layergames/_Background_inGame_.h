

#ifndef _Background_inGame__h__
#define _Background_inGame__h__
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
using namespace std;
using namespace cocos2d;

class BackgroundInGame : public cocos2d::Layer
{
private:
	cocos2d::Label *labelGame;
	Label *labelIdRoom;
	Label *labelCuoc;
	Sprite* backgroundGame;
	Label* lblTotalBet;//tong tien to, danh cho game lieng
public:
	BackgroundInGame();
	~BackgroundInGame();
	virtual bool init();
	CREATE_FUNC(BackgroundInGame);
	void setNameTable(string _nameGame, int idRoom, double moneyBet);
	void setTotalBet(double _value);
	void hideTotalBet(bool isHide);
};

#endif // _Background_inGame__h__



