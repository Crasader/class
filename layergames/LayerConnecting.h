#ifndef LAYERCONNECTING_H
#define LAYERCONNECTING_H

#include "cocos2d.h"

using namespace cocos2d;

class LayerConnecting : public Layer
{
public:
    LayerConnecting();
    ~LayerConnecting();
    virtual bool init();
    CREATE_FUNC(LayerConnecting)

    virtual void onEnter() override;
    virtual void onExit() override;

    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

private:
    Label* loading;
    int count;
    std::vector<std::string> arrText;

    void updateString(float dt);
};

#endif // LAYERCONNECTING_H
