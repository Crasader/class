//
//  LayerAchievementUnlock.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/23/18.
//

#ifndef LayerAchievementUnlock_hpp
#define LayerAchievementUnlock_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerAchievementUnlock : public cocos2d::Layer, public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate
{
    
public:
    virtual bool init() override;
    LayerAchievementUnlock();
    ~LayerAchievementUnlock();
    void setListArchie(ValueVector listAch);
    // implement the "static create()" method manually
    CREATE_FUNC(LayerAchievementUnlock);
    
    
protected:
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
private:
    Node *rootNode;
    ValueVector listArchie;
    TableView* tableView;
    void nextBtn(Ref *pSender);
    Text* lblMoney, *lblBestTime, *lblBestMove, *lblItemExpander;
};
#endif /* LayerAchievementUnlockUnlock_hpp */
