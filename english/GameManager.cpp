#include "GameManager.hpp"

USING_NS_CC;

static GameManager* gameManager = nullptr;

GameManager::GameManager()
{
    countQuest = 50;
}

GameManager* GameManager::getInstance()
{
    if (!gameManager) {
        gameManager = new (std::nothrow)GameManager();
    }
    
    return gameManager;
}
const char* GameManager::GetDataGame(int code)
{
    if(code < 0 || code >= 2048)
        return "error";
    return _dataStrings[code].c_str();
}

