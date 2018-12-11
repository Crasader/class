#ifndef __POPSTAR_STAR_H__
#define __POPSTAR_STAR_H__

#include "cocos2d.h"
using namespace cocos2d;
class HelloWorld;
class Star : public cocos2d::Sprite
{
public:
    Star(int index);
    ~Star();
    static Star* create(int index);
    
    bool isHighLight();
    void setHighLight(bool hl);
    int  getIndex();
    void setPos(const cocos2d::Vec2& pos);
    cocos2d::Vec2 getDestPos();
    void setDestPos(const cocos2d::Vec2& pos);
   // void draw(Renderer* renderer, const cocos2d::kmMat4& transform, bool transformUpdated);

    void onUpdate(float delta);
    
    virtual bool init();
    
private:
    std::string getPath();
    
private:
    int  index;
    bool highlight; // 是否高亮显示
    cocos2d::Vec2 destPos;
};

#endif // __POPSTAR_STAR_H__
