//
//  LayerListGa7User.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 2/24/16.
//
//

#ifndef LayerListGa7User_hpp
#define LayerListGa7User_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocosbuilder/CocosBuilder.h"

#include <iostream>
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

class LayerListGa7User
: public cocos2d::Layer
{
private:
    Label *lblLevel;
    ListView* listView;
    Text* defaultLabel;
    Size sizeParent;
    Sprite* spriteBackground;
    Label *headerLabel;
public:
    LayerListGa7User(Size _parentSize);
    virtual ~LayerListGa7User();
    virtual bool init();
    virtual void onEnter() override;
    virtual void onExit() override;
    
    static LayerListGa7User* create(Size _parentSize);
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void setListUser(vector<string> _listGa);
    void resetAll();
    void setLevel(int _lv);
};


#endif /* LayerListGa7User_hpp */
