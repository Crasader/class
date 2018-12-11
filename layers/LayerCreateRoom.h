#ifndef __iCasino_v2__LayerCreateRoom__
#define __iCasino_v2__LayerCreateRoom__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class LayerCreateRoom : public cocos2d::Layer, public TableViewDataSource, public TableViewDelegate
	, public PlayerCallBack
{
public:
	LayerCreateRoom();
	~LayerCreateRoom();

	CC_SYNTHESIZE(int, nGameID, GameID);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void onButtonClose(Ref* pSender);
	void onButtonCreate(Ref* pSender);
	void onButtonChonCuoc(Ref* pSender);

	virtual bool init();
	CREATE_FUNC(LayerCreateRoom);
	//
	void OnSmartFoxRoomCreationError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	cocos2d::EventListener* _touchListener;
	TextField* txtPassword;

	Layout* mLayerContainTable;
	ImageView* spriteChonCuoc;
	TableView* tableListCuoc;
	Button* btnCancel;
	Button* btnCreate;
	Text* txtBetValue;
	Text* lblMinMoney;

	vector<int> listCuoc;
	int currID;
	void createDataCuoc();
	void loadMinMoney4CreateRoom();
	Size mOldPosEditbox;
	int getMaxMoneyCreateRoomByBet(int gameID, double currBet);
};
#endif //__iCasino_v2__LayerCreateRoom__

