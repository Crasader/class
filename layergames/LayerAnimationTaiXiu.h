#ifndef _____LAYERANIMATIONTAIXIU_H_____
#define _____LAYERANIMATIONTAIXIU_H_____

#include "cocos2d.h"

using namespace cocos2d;

class LayerAnimationTaiXiu : public cocos2d::Layer
{
public:
    LayerAnimationTaiXiu();
    LayerAnimationTaiXiu(Vec2& pos);
    static LayerAnimationTaiXiu* createInstance(Vec2& pos);
    void SetCallBackResult(Ref* target, SEL_CallFuncO callfun);
    void CallFuncRemove();
protected:
    bool init();
private:
    Ref* m_callback2;
    SEL_CallFuncO m_callbackListener2;
    Vec2 posFrame;
};

#endif // _____LAYERANIMATIONTAIXIU_H_____
