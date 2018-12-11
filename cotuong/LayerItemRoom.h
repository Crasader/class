#ifndef LayerItemRoom_h__
#define LayerItemRoom_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;
class LayerItemRoom : public cocos2d::Layer
{
public:
	virtual bool init() override;
	LayerItemRoom();
	~LayerItemRoom();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerItemRoom);
	void setDataCell(int betvl, int userCount);
	void setDatas(int _idtable, int betvl, int userCount, bool _allowView, int spec_count);
	//	void setDataCell(int idx, string str_header, string str_date);
private:
	Layout* pnlMain;
	Text *lblMoney;
	Sprite* button_xu_5;
	Sprite* table1_1;
	ImageView *imgUserLeft, *imgUserRight;
// 	ImageView* imgView;
	Text* lblId;

	int betValue = 0;
	int userCount = 0;
	int idTable = 0;
	bool allowView = false;
	void onButtonJoinRoom(Ref* pSender);
	double getMinBetByGame(int gameID, double amf);
};

#endif 



