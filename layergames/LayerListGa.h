//
//  LayerListGa.h
//  iCasinov2
//
//  Created by AlexDao on 6/3/14.
//
//

#ifndef iCasinov2_LayerListGa_h
#define iCasinov2_LayerListGa_h

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

class LayerListGa
: public cocos2d::Layer
{
private:
	Label *lblLevel;
	ListView* listView;
	Text* defaultLabel;
	Size sizeParent;
	
public:
    LayerListGa(Size _parentSize);
	virtual ~LayerListGa();
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
    
	static LayerListGa* create(Size _parentSize);
	virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void setListUser(vector<string> _listGa);
	void resetAll();
	void setLevel(int _lv);
};
#endif