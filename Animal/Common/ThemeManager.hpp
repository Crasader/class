//
//  ThemeManager.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/9/18.
//

#ifndef ThemeManager_hpp
#define ThemeManager_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;


class ThemeManager
{
private:
    ThemeManager();
    ~ThemeManager();
    
public:
    static string getValueByKey(string key);
    static ThemeManager * getInstance();
    static int getMoney();
    static bool setIdTheme();
    static Sprite *getBackground();
    static std::string getNameAnimal(int idAnimal);
    static ValueMap getInfoThemeByIndex(int index);
    static bool getStatusThemeUnlock(int index);
    static string getNameThemeUnlock(int index);
    static int getPriceThemeUnlock(int index);
    static bool buyThemeByIdx(int index);
    
    CC_SYNTHESIZE(int, idTheme, IdTheme);
    CC_SYNTHESIZE(ValueVector, listTheme, ListTheme);
    CC_SYNTHESIZE(Color3B, fontColor, FontColor);
    CC_SYNTHESIZE(Color3B, backgroundColor, BackgroundColor);
    CC_SYNTHESIZE(Color3B, colorLine, ColorLine);
    CC_SYNTHESIZE(Color3B, colorTime, ColorTime);
    CC_SYNTHESIZE(Color3B, colorMove, ColorMove);
    CC_SYNTHESIZE(Color3B, colorAchie, ColorAchie);
    CC_SYNTHESIZE(Color3B, colorAchieUnlock, ColorAchieUnlock);
    CC_SYNTHESIZE(string, textTureBtnBack, TextTureBtnBack);
//    CC_SYNTHESIZE(ValueMap, levelData, LevelData);
    static void getConfig();
    static ValueMap getLevelData();
    Color3B getConfigColorByName(string nameColor);
    vector<string> splitColor(string color);
};
#endif /* ThemeManager_hpp */
