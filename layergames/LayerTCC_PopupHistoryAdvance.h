#ifndef LayerTCC_PopupHistoryAdvance_h__
#define LayerTCC_PopupHistoryAdvance_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

struct ItemTCCHis_Ad{
	string bettype;
	int betvl;
	int win;
	ItemTCCHis_Ad(string _bettype, int _bet, int _win) :
		bettype(_bettype), betvl(_bet), win(_win){};
};

class LayerTCC_ItemHistoryAdvance :public Layer{
public:
	CREATE_FUNC(LayerTCC_ItemHistoryAdvance);
	virtual bool init();
	void setDatas(ItemTCCHis_Ad item);
	void setBackGround(int _type);
private:
	Text* lblPos;
	Text* lblBet;
	Text* lblWin;
	Layout* Panel_1;
};

class LayerTCC_PopupHistoryAdvance : public Layer,
	public TableViewDataSource,
	public TableViewDelegate
{
public:
	LayerTCC_PopupHistoryAdvance();
	~LayerTCC_PopupHistoryAdvance();
	CREATE_FUNC(LayerTCC_PopupHistoryAdvance);
	virtual bool init();

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override;
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override;
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
	void setDataHis(string resStr, string listInfo);
private:
	vector<ItemTCCHis_Ad> lstHis;
	vector<ImageView*> lstImg;
	TableView *tblHis;
	int matchID = -1;
	void onBtnClose(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
};


#endif // LayerTCC_PopupHistoryAdvance_h__
