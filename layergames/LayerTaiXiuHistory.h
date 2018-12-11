
#ifndef _iCasino_TaiXiu_History_h_
#define _iCasino_TaiXiu_History_h_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;


class LayerTaiXiuHistory : public Layer
	, public cocos2d::extension::TableViewDataSource
	, public cocos2d::extension::TableViewDelegate
{
public:
	LayerTaiXiuHistory();
	LayerTaiXiuHistory(const std::vector<std::string>& P);
	~LayerTaiXiuHistory();
    static LayerTaiXiuHistory* create(const std::vector<std::string>& P);

protected:
	virtual bool init();
    CREATE_FUNC(LayerTaiXiuHistory)

    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);

	//TABLE VIEW
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

private:
	TableView*		tblHistory;
    std::vector<std::string> ListHistory;

    string GetUrlXiNgau(string& number);
    void SetListHistory(const std::vector<std::string>& P);
};

#endif

