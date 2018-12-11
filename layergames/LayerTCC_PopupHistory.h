#ifndef LayerTCC_PopupHistory_h__
#define LayerTCC_PopupHistory_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

struct ItemTCCHis{
	int matchid;
	string time;
	string betroom;
	int bet;
	int receive;
	string res;
	ItemTCCHis(int _mid, string _time, string _betroom, int _bet, int _receive, string _res) :
		matchid(_mid), time(_time), betroom(_betroom), bet(_bet), receive(_receive), res(_res){};
};
struct ItemTCCHisAdvance{
	int matchid;
	string info;
	string res;
	ItemTCCHisAdvance(){
		matchid = 0;
		info = "";
		res = "";
	};
	ItemTCCHisAdvance(int _mid, string _info, string _res) :
		matchid(_mid), info(_info), res(_res){};
};

class LayerTCC_ItemHistory :public Layer{
public:
	CREATE_FUNC(LayerTCC_ItemHistory);
	virtual bool init();
	void setDatas(ItemTCCHis item, ItemTCCHisAdvance listInfo);
	void setBackGround(int _type);
private:
	Layout* Panel_1;
	Text* lblMatchId;
	Text* lblTime;
	Text* lblBetRoom;
	Text* lblBet;
	Text* lblReceived;
	Button* btnAdvance;
	int matchID;
	vector<ImageView*> imgDices;
	ItemTCCHisAdvance lstInfo;

	void onButtonAdvance(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	string getTimeHistory(string str);
};

class LayerTCC_PopupHistory : public Layer,
	public TableViewDataSource,
	public TableViewDelegate,
	public PlayerCallBack
{
public:
	LayerTCC_PopupHistory();
	~LayerTCC_PopupHistory();
	CREATE_FUNC(LayerTCC_PopupHistory);
	virtual bool init();

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override;
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override;
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	vector<ItemTCCHis> lstHis;
	vector<ItemTCCHisAdvance> lstDetail;
	TableView *tblHis;
	void event_EXT_EVENT_TCC_USER_HIS_RESP(boost::shared_ptr<ISFSObject> param);
	void onBtnClose(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	ItemTCCHisAdvance getInfoByMatchID(int matchid);
};

#endif // LayerTCC_PopupHistory_h__
