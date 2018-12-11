//
//  LayerPopupBuyItem.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 5/9/18.
//

#ifndef LayerPopupBuyItem_hpp
#define LayerPopupBuyItem_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerPopupBuyItem : public cocos2d::Layer
{
    
public:
    virtual bool init() override;
    LayerPopupBuyItem();
    ~LayerPopupBuyItem();
    void setContent(std::string title, std::string des,std::string money, std::string imgLink, std::string typeItem);
    // implement the "static create()" method manually
    CREATE_FUNC(LayerPopupBuyItem);
    void setCallBack(Ref *target, SEL_CallFuncO listener);
    void setCallBackCancel(Ref *target, SEL_CallFuncO listener);
    void setNameItem(std::string name);
    
protected:
    
private:
    SEL_CallFuncO mCallBackListener;
    SEL_CallFuncO mCallBackListener2;
    Ref *mCallBack;
    void tapBuy(Ref *pSender);
    void tapCancel(Ref *pSender);
    Text *lblTitle, *lblDes, *lblMoney;
    Sprite *img;
    std::string typeItem;
    void showNotifi();
    Node  *pnlMain;
    void removeLayer(Ref *pSender);
};
#endif /* LayerPopupBuyItem_hpp */
