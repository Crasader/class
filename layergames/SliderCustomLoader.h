//
//  SliderCustomLoader.h
//  demo
//
//  Created by AlexDao on 5/24/14.
//
//

#ifndef demo_SliderCustomLoader_h
#define demo_SliderCustomLoader_h

#include "cocos2d.h"
#include "cocosbuilder/CocosBuilder.h"

using namespace cocos2d;
using namespace cocosbuilder;

class SliderCustomLoader : public cocosbuilder::LayerLoader{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SliderCustomLoader, loader);
protected:
	virtual ControlSlider * createNode(cocos2d::Node * pParent, cocosbuilder::CCBReader * pCCBReader) {
		ControlSlider* slider = ControlSlider::create(Sprite::createWithSpriteFrameName("assest/frame_scroll.png")
			, Sprite::createWithSpriteFrameName("assest/light_scroll.png")
			, Sprite::createWithSpriteFrameName("assest/button_scroll.png") );
        return slider;
    }
};

#endif
