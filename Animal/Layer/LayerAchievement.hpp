//
//  LayerAchievement.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/3/18.
//

#ifndef LayerAchievement_hpp
#define LayerAchievement_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerAchievement : public cocos2d::Layer, public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate
{
    
public:
    virtual bool init() override;
    LayerAchievement();
    ~LayerAchievement();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LayerAchievement);
    
    
protected:
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
private:
    ValueVector listArchie;
    TableView* tableView;
    void backMain(Ref *pSender);
    Text* lblMoney, *lblBestTime, *lblBestMove, *lblItemExpander;
};
#endif /* LayerAchievement_hpp */

