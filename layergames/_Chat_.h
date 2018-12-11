
#ifndef __testcpp2605___Chat___
#define __testcpp2605___Chat___

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "AllData.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocostudio;

class Chat : public Layer {
private:
    string mes;
    int posUser;
    Size sizeThis;
	ui::Scale9Sprite *blocks;
    Sprite* emo;
    bool emoChat;
    bool isChanGame;
	string chatFrame;
public:
    Chat(const string& pMes, const int& posUser, const bool& isChan);
	///MINHDV - for GameP2;
	Chat(const string& pMes, const string& chatFrame);
	Chat();
	~Chat();

    virtual bool init();
    static Chat* create(const string& pMes, const int& posUser, const bool& isChan = false);
	///MINHDV - for GameP2;
	static Chat* create(const string& pMes, const string& chatFrame);

    void setStatusByServer(bool isServer);
    
    Size getSize();
    void formatMes();
    void releaseSafe();
	void setPosThis(Vec2 point);
};

#endif /* defined(__testcpp2605___Chat___) */
