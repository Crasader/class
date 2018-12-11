#ifndef _GameManager_H_
#define _GameManager_H_

#include "cocos2d.h"

class GameManager
{
private:
    GameManager();
    ~GameManager();
    
public:
    static GameManager* getInstance();
    const char* GetDataGame(int code);
    std::string _dataStrings[51];
    int countQuest = 50;
};

#endif
