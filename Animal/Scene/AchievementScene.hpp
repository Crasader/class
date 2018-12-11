//
//  ArchievementScene.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/3/18.
//

#ifndef ArchievementScene_hpp
#define ArchievementScene_hpp

#include <stdio.h>

class AchievementScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(AchievementScene);
private:
    
};
#endif /* ArchievementScene_hpp */
