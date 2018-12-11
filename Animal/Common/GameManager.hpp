//
//  GameManager.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/21/18.
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Common.h"
using namespace std;
class GameManager
{
private:
    GameManager();
    ~GameManager();
    
public:
    static GameManager * getInstance();
//    static int getTotalKnifeInConfig();
//    static void resetScore();
//    static std::string getKinfeUsing();
//    static std::string getKnifeByIndex(int idx);
//    static std::string getNameKnifeInPlist(int idx);
//    static bool checkKnifeUnlocked(std::string knifeName);
//    static void unlockKnife(std::string knifeName);
    
    static int getItemByNameInt(std::string nameItem);
    static string getItemByName(std::string nameItem);
    
    static int getItemInPlist(std::string nameItem);
    
    
    static bool setPlusItem(std::string nameItem, int amount);
    static bool setMinusItem(std::string nameItem);
    
    static bool setHighScore(std::string nameScore,int totalScore);
    static int getHighScore(GAME_ID idGame);
    
    static void setMoney(int amount);
    static int getMoney();
    static void plusScore(int score);
    static int getTotalArchie();
    static void plusCountSquare();
    static string getMoneyConverted();
    
    void setStatusSetting(string nameSetting, bool fromMainGame);
    
    static string getStatusSetting(string nameSetting);
    
    static bool getSound();
    static bool getVibration();
    static bool buyItem(int price);
    
    static int boughtItem(std::string nameItem);
    
    //    static
    CC_SYNTHESIZE(int, typeGame, TypeGame);
    CC_SYNTHESIZE(int, scoreValue, ScoreValue);
    
    CC_SYNTHESIZE(int, countSquare, countSquare);
    
    
//    CC_SYNTHESIZE(int, totalKnife, TotalKnife);
//    CC_SYNTHESIZE(int, currentLevel, CurrentLevel);
//    CC_SYNTHESIZE(int, totalScore, TotalScore);
};

#endif /* GameManager_hpp */
