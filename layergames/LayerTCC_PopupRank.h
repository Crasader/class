#ifndef LayerTCC_PopupRank_h__
#define LayerTCC_PopupRank_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

struct ItemTCCRank{
	int number;
	string name;
	int bet;
	ItemTCCRank(int _number, string _name, int _bet) :
		number(_number), name(_name), bet(_bet){};
};

class LayerTCC_ItemRank :public Layer{
public:
	CREATE_FUNC(LayerTCC_ItemRank);
	virtual bool init();
	void setDatas(ItemTCCRank item);
	void setBackGround(int _type);
private:
	Text* lblNumber;
	Text*lblName;
	Text*lblBet;
	Layout* Panel_1;
};

class LayerTCC_PopupRank : public Layer,
	public TableViewDataSource,
	public TableViewDelegate,
	public PlayerCallBack
{
public:
	LayerTCC_PopupRank();
	~LayerTCC_PopupRank();
	CREATE_FUNC(LayerTCC_PopupRank);
	virtual bool init();

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override;
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override;
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	vector<ItemTCCRank> lstRank;
	TableView *tblRank;
	void event_EXT_EVENT_TCC_RANK_RESP(boost::shared_ptr<ISFSObject> param);
	void onBtnClose(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif // LayerTCC_PopupRank_h__
