#ifndef LAYERTAIXIUPOPUPWINNERS_H
#define LAYERTAIXIUPOPUPWINNERS_H

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


class LayerTaiXiuPopupWinners : public Layer
        , public cocos2d::extension::TableViewDataSource
        , public cocos2d::extension::TableViewDelegate
{
public:
    LayerTaiXiuPopupWinners();
    LayerTaiXiuPopupWinners(const std::vector<std::string>& P);
    ~LayerTaiXiuPopupWinners();
     static LayerTaiXiuPopupWinners* createInstace(const vector<string> & P);

protected:
    virtual bool init();
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
    TableView* tblTopWinner;
    Text* lblPopupWinner;
    std::vector< std::vector<std::string> > WinnerArray;
    int CurrentHistory;
    int SizeListTops;

    void SetListWinners(const vector<string> &P);

    void OnBtnNextClick(Ref *sender,  ui::Widget::TouchEventType pType);
    void OnBtnPrevClick(Ref *sender,  ui::Widget::TouchEventType pType);

    std::string GetLinkAvatar(const std::string& uid);

};

#endif // LAYERTAIXIUPOPUPWINNERS_H
