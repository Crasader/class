//
//  ArchievementManager.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/23/18.
//

#include "ArchievementManager.hpp"
#include "Common.h"
#include "GameManager.hpp"
#include "GameManager.hpp"
using namespace std;
using namespace cocos2d;
static ArchievementManager * archievementManager = nullptr;

ArchievementManager::ArchievementManager(){
    listArchie = FileUtils::getInstance()->getValueVectorFromFile("achievement.plist");
    
}

ArchievementManager * ArchievementManager::getInstance(){
    if (!archievementManager) {
        archievementManager = new (std::nothrow)ArchievementManager();
    }
    return archievementManager;
}
//ValueMap ArchievementManager::getLevelData(){
////    ValueMap levelData = getInstance()->getListTheme().at(getInstance()->getIdTheme()).asValueMap();
////    return levelData;
//}
//
int ArchievementManager::getTrophie(){
    //get config trophie
    
    int trophies = 0;
    for (auto i = 0; i < getInstance()->getListArchie().size(); i++) {
        ValueMap valueMap = getInstance()->getListArchie().at(i).asValueMap();
        string statusUnlock = valueMap.at("StatusUnlock").asString();
        bool statusUnlockArch = UserDefault::getInstance()->getBoolForKey(statusUnlock.c_str());
        if (statusUnlockArch) {
            trophies += 1;
        }
    }
    return trophies;
}

ValueVector ArchievementManager::getListUnlockArchie(){
    ValueVector newList;
    for (auto i = 0; i < getInstance()->getListArchie().size(); i++) {
        ValueMap valueMap = getInstance()->getListArchie().at(i).asValueMap();
        string statusUnlock = valueMap.at("StatusUnlock").asString();
//        auto square = valueMap.at("square").asInt();
        bool statusUnlockArch = UserDefault::getInstance()->getBoolForKey(statusUnlock.c_str());
        //Nếu thành tích này chưa unlock thì sẽ vào kiểm tra thông tin.
        if (!statusUnlockArch) {
            auto name = valueMap.at("Name").asString();
            auto mode = valueMap.at("Mode").asInt();
            auto square = valueMap.at("square").asInt();
            int bouns = valueMap.at("bouns").asInt();
            /// Check xem nếu có mode thì lấy theo type game
            if (mode == GameManager::getInstance()->getTypeGame()) {
                //set trạng thái unlock của archievement đó.
                UserDefault::getInstance()->setBoolForKey(name.c_str(), true);
                UserDefault::getInstance()->setBoolForKey(statusUnlock.c_str(), true);
                
                //Cộng tiền
                GameManager::getInstance()->setMoney(bouns);
                
                //đẩy nhiệm vụ này vào list vừa mới unlock.
                newList.push_back(Value(valueMap));
                
            }else{
                //Kiểm tra phần plist có square k?
                if (square != 0) {
                    int countSquare = UserDefault::getInstance()->getIntegerForKey(SQUARE.c_str());
                    if (countSquare >= square) {
                        UserDefault::getInstance()->setBoolForKey(name.c_str(), true);
                        UserDefault::getInstance()->setBoolForKey(statusUnlock.c_str(), true);
                        //Cộng tiền
                        GameManager::getInstance()->setMoney(bouns);
                        newList.push_back(Value(valueMap));
                        
                    }
                }
            }
        }
    }
    return newList;
}
