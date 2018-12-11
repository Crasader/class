//
//  ThemeManager.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/9/18.
//

#include "ThemeManager.hpp"
#include "Common.h"
#include "GameManager.hpp"

USING_NS_CC;

using namespace std;
using namespace cocos2d;
static ThemeManager * themeManager = nullptr;

ThemeManager::ThemeManager(){
    
    int id = UserDefault::getInstance()->getIntegerForKey(ID_THEME.c_str());
    idTheme = id;
    listTheme = FileUtils::getInstance()->getValueVectorFromFile("Themes.plist");
    fontColor = Color3B();
    backgroundColor = Color3B();
    colorLine = Color3B();
    colorTime = Color3B();
    colorMove = Color3B();
    colorAchie = Color3B();
    colorAchieUnlock = Color3B();
    textTureBtnBack = "";
}

ThemeManager * ThemeManager::getInstance(){
    if (!themeManager) { 
        themeManager = new (std::nothrow)ThemeManager();
    }
    return themeManager;
}
ValueMap ThemeManager::getLevelData(){
   ValueMap levelData = getInstance()->getListTheme().at(getInstance()->getIdTheme()).asValueMap();
    return levelData;
}

Sprite *ThemeManager::getBackground(){
    string nameBackground = getInstance()->getLevelData().at(BACKGROUND.c_str()).asString();
    Sprite* background = Sprite::create(nameBackground);
    return background;
}

string ThemeManager::getNameAnimal(int idAnimal){
    ValueVector nameBackground = getInstance()->getLevelData().at(LIST_ANIMAL.c_str()).asValueVector();
    string nameAnimal = nameBackground.at(idAnimal-1).asString();
    return nameAnimal;
}
ValueMap ThemeManager::getInfoThemeByIndex(int index){
    return getInstance()->getListTheme().at(index).asValueMap();
}
string ThemeManager::getNameThemeUnlock(int index){
    ValueMap valueMap = getInstance()->getInfoThemeByIndex(index);
    string nameTheme = valueMap.at("Name").asString();
    return nameTheme;
}
int ThemeManager::getPriceThemeUnlock(int index){
    ValueMap valueMap = getInstance()->getInfoThemeByIndex(index);
    int price = valueMap.at("price").asInt();
    return price;
}


bool ThemeManager::buyThemeByIdx(int index){
    int price = getInstance()->getPriceThemeUnlock(index);
    bool result = GameManager::getInstance()->buyItem(price);
//    if (result) {
//        UserDefault::getInstance()->setBoolForKey(ID_THEME.c_str(), index);
//    }
    return result;
}

bool ThemeManager::getStatusThemeUnlock(int index){
    ValueMap valueMap = getInstance()->getListTheme().at(index).asValueMap();
    string nameDot = valueMap.at("Name").asString();
    bool status = UserDefault::getInstance()->getBoolForKey(nameDot.c_str());
    if (status) {
        return true;
    }
    return false;
}
Color3B ThemeManager::getConfigColorByName(string nameColor){
    string colorFont = getInstance()->getLevelData().at(nameColor.c_str()).asString();
    if(colorFont == ""){
        return Color3B(0, 0, 0);
    }
    vector<string> arr = splitColor(colorFont);
    int r = atoi(arr[0].c_str());
    int g = atoi(arr[1].c_str());
    int b = atoi(arr[2].c_str());
    
    Color3B colorFont3B = Color3B(r, g, b);
    
    return colorFont3B;
    
}


void ThemeManager::getConfig(){
    getInstance()->setFontColor(getInstance()->getConfigColorByName(COLOR_FONT));
    getInstance()->setBackgroundColor(getInstance()->getConfigColorByName(BACKGROUND_COLOR));
    getInstance()->setColorLine(getInstance()->getConfigColorByName(COLOR_PANEL));
    getInstance()->setColorTime(getInstance()->getConfigColorByName(COLOR_TIME));
    getInstance()->setColorMove(getInstance()->getConfigColorByName(COLOR_MOVE));
    getInstance()->setColorAchie(getInstance()->getConfigColorByName(COLOR_ARCHIE));
    getInstance()->setColorAchieUnlock(getInstance()->getConfigColorByName(COLOR_ARCHIE_UNLOCK));
    getInstance()->setTextTureBtnBack(getInstance()->getValueByKey("buttonBack"));
}

string ThemeManager::getValueByKey(string key){
    ValueMap data = getInstance()->getLevelData();
    return data.at(key).asString();
}
vector<string> ThemeManager::splitColor(string color) {
    
    string buf = " "; // Have a buffer string
    stringstream ss(color); // Insert the string into a stream
    
    vector<string> tokens; // Create vector to hold our words
    
    while (ss >> buf)
        tokens.push_back(buf);
    return tokens;
}

