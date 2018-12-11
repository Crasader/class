//
//  LayerShowChuChay.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/27/16.
//
//

#ifndef LayerShowChuChay_hpp
#define LayerShowChuChay_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerShowChuChay: public cocos2d::Layer{
    public:
    LayerShowChuChay();
    ~LayerShowChuChay();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void setContent(string str);
    void actionFinished();
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerShowChuChay);
    private:
    TextBMFont* txtChuChay;
    Label* txtChuChay2;
    vector<string> lstChuChay;
    void chayChu(float dt);
    bool isShow = false;
    int currentshow = 0;
};

#endif /* LayerShowChuChay_hpp */
