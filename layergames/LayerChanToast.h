
#ifndef __Chan___Toast___
#define __Chan___Toast___

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocostudio;

class ChanToast : public Layer
{
public:
	~ChanToast();
	ChanToast(string& message);
	static ChanToast* create(string& message);
	bool init();

	const string& GetStringToast() const;
	//mem_fun
	bool CompareToast(const string& newStrToast);
private:
	std::string StringToast;
};

class ObjToast : public Layer{
private:
     std::vector<string> arrayToast;
public:
    ObjToast(std::vector<string>& array);
    ~ObjToast();
	static Ref* createToast(std::vector<string>& array);
    bool init();
    int GetSize();
};


class LayerChanToast : public Layer{
public:
	LayerChanToast();
	~LayerChanToast();
	bool init();
    CREATE_FUNC(LayerChanToast)

    void ShowToast(std::string stringToast, const float& duration = 3, const int& type = 1);
   // void ShowToast(string stringToast, const float& duration = 3, const int& type = 1);
	void DissmissToast(Ref* sender);
	void RemoveAllToast();
private:
	std::vector<ChanToast*> arrToasts;

    float StartY;
    float Distance;
};

#endif
