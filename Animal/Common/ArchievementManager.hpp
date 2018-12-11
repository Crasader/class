//
//  ArchievementManager.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/23/18.
//

#ifndef ArchievementManager_hpp
#define ArchievementManager_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;


class ArchievementManager
{
private:
    ArchievementManager();
    ~ArchievementManager();
    
public:
    static ArchievementManager * getInstance();
    static bool checkNewUnlockArchie();
    static ValueVector getListUnlockArchie();
    static ValueMap getInfoThemeByIndex(int index);
    static int getTrophie();
    
    CC_SYNTHESIZE(ValueVector, listArchie, ListArchie);
    
    //    CC_SYNTHESIZE(ValueMap, levelData, LevelData);
    
    static ValueMap getLevelData();
    Color3B getConfigColorByName(string nameColor);
    vector<string> splitColor(string color);
};
#endif /* ArchievementManager_hpp */
