#ifndef LayerPopupRankGame_h__
#define LayerPopupRankGame_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "layergames/CircleAvatar.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;

class LayerPopupRankGame : public cocos2d::Layer,
	public PlayerCallBack,
	public TableViewDataSource, public TableViewDelegate
{
private:
	TextField* txtNoiDung;
	TableView* tblChat;
	Label* lblRoomName;
	Button* btnPlayNow;
	Button* btnSendChat;
	Layout* panelTable;
	TableView* tblRankGame;
	vector<infoUser> lstTops;

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);
	TableViewCell* createCell(TableView *table, int idx);
protected:
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
public:
	LayerPopupRankGame();
	~LayerPopupRankGame();

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual bool init();
	CREATE_FUNC(LayerPopupRankGame);
};

class LayerItemPopupRank : public Layer {
public:
	LayerItemPopupRank();
	~LayerItemPopupRank();
	virtual bool init();
	CREATE_FUNC(LayerItemPopupRank);
	void setDatas(const int& index, const infoUser& data);
private:
	CircleAvatar* circleAvatarItem;
	Text* lblTop;
	Text* txtName;
	Text* txtMoney;
	Text* txtLevel;
	Sprite* spriteAvatar;
	ImageView* avtVip;
};

#endif // LayerPopupRankGame_h__





