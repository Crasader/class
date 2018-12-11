//
//  LayerWinSlot.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerWinSlot_hpp
#define LayerWinSlot_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerWinSlot: public cocos2d::Layer{
public:
    LayerWinSlot();
    ~LayerWinSlot();
    virtual bool init();
    void startAnim();
    void actionDone();
    void setCallBack(Ref* target,SEL_CallFuncO listener);
    void setRoomType(int type);
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(LayerWinSlot);
private:
    SEL_CallFuncO mCallBackListener;
    Ref *mCallBack;
    vector<ImageView*> coin;
    vector<Point> pos;

    ImageView* imgDich;
    
};

#endif /* LayerWinSlot_hpp */
