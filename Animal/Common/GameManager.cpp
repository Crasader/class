//
//  GameManager.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/21/18.
//

#include "GameManager.hpp"
#include "Common.h"
#include "mUtils.hpp"
#include "audio/include/SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;
static GameManager * gameManager = nullptr;

GameManager::GameManager(){
    int count = UserDefault::getInstance()->getIntegerForKey(SQUARE.c_str());
    countSquare = count;
    typeGame = 0;
//    scoreValue = 0;
//    totalKnife = 0;
//    currentLevel = 0;
//    totalScore = 0;
}
GameManager * GameManager::getInstance(){
    if (!gameManager) {
        gameManager = new (std::nothrow)GameManager();
    }
    return gameManager;
}
//void GameManager::resetScore(){
////    GameManager::getInstance()->setTotalScore(0);
////    GameManager::getInstance()->setScoreValue(0);
////    GameManager::getInstance()->setCurrentLevel(0);
////    GameManager::getInstance()->setTotalKnife(0);
////    LevelManager::getInstance()->setValueMap(LevelManager::getInstance()->getLevel().at(GameManager::getInstance()->getCurrentLevel()).asValueMap());
//    
//}
//int GameManager::getTotalKnifeInConfig(){
//    auto level = FileUtils::getInstance()->getValueVectorFromFile("ListKnife.plist");
//    return level.size();
//}

void GameManager::setMoney(int amount){
    int money = UserDefault::getInstance()->getIntegerForKey(MONEY.c_str());
    int newMoney = money + amount;
    UserDefault::getInstance()->setIntegerForKey(MONEY.c_str(), newMoney);
}

int GameManager::getMoney(){
    int money = UserDefault::getInstance()->getIntegerForKey(MONEY.c_str());
    return money;
}



bool GameManager::setHighScore(std::string nameScore, int totalScore){
    int highScoreCurrent = UserDefault::getInstance()->getIntegerForKey(nameScore.c_str());
    if (totalScore > highScoreCurrent) {
        UserDefault::getInstance()->setIntegerForKey(nameScore.c_str(), totalScore);
        return true;
//        SonarCocosHelper::GameCenter::submitScore(totalScore, "TopHighScore");
    }
    return false;
}

int GameManager::getHighScore(GAME_ID idGame){
    string nameScore = "";
    
    switch (idGame) {
        case GAME_FREE:
            nameScore = HIGH_SCORE_FREE;
            break;
        case GAME_TIME:
            nameScore = HIGH_SCORE_TIMED;
            break;
        case GAME_MOVES:
            nameScore = HIGH_SCORE_MOVES;
            break;
        default:
            break;
    }
    int highScoreCurrent = UserDefault::getInstance()->getIntegerForKey(nameScore.c_str());
    return highScoreCurrent;
}

void GameManager::plusCountSquare(){
    getInstance()->setcountSquare(getInstance()->getcountSquare()+1);
    UserDefault::getInstance()->setIntegerForKey(SQUARE.c_str(), getInstance()->getcountSquare());
}

void GameManager::plusScore(int score){
    int newScore = GameManager::getInstance()->getScoreValue() + score;
    GameManager::getInstance()->setScoreValue(newScore);
}

int GameManager::getItemByNameInt(std::string nameItem) {
    string a = UserDefault::getInstance()->getStringForKey(nameItem.c_str());
    
    if (a == "") {
        auto configItem = FileUtils::getInstance()->getValueMapFromFile(FILE_NAME_ITEM.c_str());
        auto countItem = configItem.at(nameItem.c_str()).asInt();
        UserDefault::getInstance()->setStringForKey(nameItem.c_str(), std::to_string(countItem));
        return countItem;
    }
    return std::atoi(a.c_str());
    
}

int GameManager::getItemInPlist(std::string nameItem) {
    auto configItem = FileUtils::getInstance()->getValueMapFromFile(FILE_NAME_ITEM.c_str());
    auto countItem = configItem.at(nameItem.c_str()).asInt();
    return countItem;
}

string GameManager::getItemByName(std::string nameItem) {
    return mUtils::convertMoneyEx(std::stol(std::to_string(getInstance()->getItemByNameInt(nameItem)).c_str()));
}


bool GameManager::setPlusItem(std::string nameItem, int amount){
    auto currentItemCount = getInstance()->getItemByNameInt(nameItem);
    auto newTotalItem = currentItemCount + amount;
    UserDefault::getInstance()->setStringForKey(nameItem.c_str(), std::to_string(newTotalItem));
    return true;
}

bool GameManager::setMinusItem(std::string nameItem){
    auto currentItemCount = getInstance()->getItemByNameInt(nameItem);
    if (currentItemCount > 0) {
        int curItem = currentItemCount - 1;
        
        UserDefault::getInstance()->setStringForKey(nameItem.c_str(), std::to_string(curItem));
        return true;
    }
    return false;
    
}

int GameManager::getTotalArchie(){
    auto level = FileUtils::getInstance()->getValueVectorFromFile("archievement.plist");
    return level.size();
}



string GameManager::getMoneyConverted() {
    return mUtils::convertMoneyEx(stol(to_string(getMoney())));
}
bool GameManager::getVibration()
{
    auto status = UserDefault::getInstance()->getBoolForKey(VIBRATION.c_str());
    if (status) {
        return true;
    }
    return false;
}
bool GameManager::getSound()
{
    auto status = UserDefault::getInstance()->getBoolForKey(SOUND.c_str());
    if (status) {
        return true;
    }
    return false;
}


bool GameManager::buyItem(int price){
    
    auto curMoney = getInstance()->getMoney();
    if (curMoney >= price) {
        getInstance()->setMoney(-price);
        return true;
    }
    
    return false;
}

void GameManager::setStatusSetting(string nameSetting, bool fromMainGame){
    auto status = UserDefault::getInstance()->getBoolForKey(nameSetting.c_str());
    if (status) {
        UserDefault::getInstance()->setBoolForKey(nameSetting.c_str(), false);
        if (nameSetting == SOUND) {
                SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
                SimpleAudioEngine::getInstance()->pauseAllEffects();
        }
    }else{
        UserDefault::getInstance()->setBoolForKey(nameSetting.c_str(), true);
        if (nameSetting == SOUND) {
            if(fromMainGame){
                SimpleAudioEngine::getInstance()->rewindBackgroundMusic();
            }
        }
        
    }
}

string GameManager::getStatusSetting(string nameSetting)
{
    bool status = UserDefault::getInstance()->getBoolForKey(nameSetting.c_str());
    if (status) {
        return "ON";
    }
    return "OFF";
}

int GameManager::boughtItem(string nameItem){
    auto curMoney = getInstance()->getMoney();
    int moneyForItem;
    int amountItem;
    if (nameItem == ITEM_TIME) {
        moneyForItem = GameManager::getInstance()->getItemInPlist(MoneyTime);
        amountItem = GameManager::getInstance()->getItemInPlist(PlusForTime);
    }else if (nameItem == ITEM_EXPANDER){
        moneyForItem = GameManager::getInstance()->getItemInPlist(MoneyExpand);
        amountItem = GameManager::getInstance()->getItemInPlist(PlusForExpand);
    }else{
        moneyForItem = GameManager::getInstance()->getItemInPlist(MoneyShrin);
        amountItem = GameManager::getInstance()->getItemInPlist(PlusForShin);
    }
    
    if (curMoney >= moneyForItem) {
        //Trừ tiền.
        getInstance()->setMoney(-moneyForItem);
        getInstance()->setPlusItem(nameItem, amountItem);
        return amountItem;
    }
    return 0;
}

//std::string GameManager::getKinfeUsing(){
//    auto a = UserDefault::getInstance()->getIntegerForKey("indexKnife");
//    if (a == NULL) {
//        a = 0;
//        UserDefault::getInstance()->setIntegerForKey("indexKnife", 0);
//    }
//
//    auto configKnife = FileUtils::getInstance()->getValueVectorFromFile("ListKnife.plist");
//
//    ValueMap levelData = configKnife.at(a).asValueMap();
//    auto name = levelData.at("Name").asString();
//    return "res/knifes/"+name;
//
//}

//std::string GameManager::getKnifeByIndex(int idx){
//    auto configKnife = FileUtils::getInstance()->getValueVectorFromFile("ListKnife.plist");
//
//    ValueMap levelData = configKnife.at(idx).asValueMap();
//    auto name = levelData.at("Name").asString();
//
//    return "res/knifes/"+name;
//
//}
//
//std::string GameManager::getNameKnifeInPlist(int idx){
//    auto configKnife = FileUtils::getInstance()->getValueVectorFromFile("ListKnife.plist");
//
//    ValueMap levelData = configKnife.at(idx).asValueMap();
//    auto name = levelData.at("Name").asString();
//    return name;
//}
//void GameManager::unlockKnife(std::string knifeName){
//    UserDefault::getInstance()->setBoolForKey(knifeName.c_str(), true);
//}
//
//bool GameManager::checkKnifeUnlocked(std::string knifeName){
//    auto unlock = UserDefault::getInstance()->getBoolForKey(knifeName.c_str());
//    //    auto a = UserDefault::getInstance()->getIntegerForKey(knifeName.c_str(), 0);
//    return unlock;
//
//}
