
#ifndef __iCasino_v2___Number_Chat_inGame7u__
#define __iCasino_v2___Number_Chat_inGame7u__

#include <iostream>

#include "cocos2d.h"
#include "AllData.h"
#include "_Number_.h"

using namespace std;
using namespace cocos2d;

class Number_Chat_inGame7u : public Layer {
private:
    vector<int>		arrPos;
    vector<string>	arrNumberString;
    vector<double>	arrNumberDouble;
    bool IsRunning;
	int numScale;
public:
    Number_Chat_inGame7u();
    ~Number_Chat_inGame7u();
    bool init();
    CREATE_FUNC(Number_Chat_inGame7u);

    void ShowNumberByPos(const int& pos, string numberString);
	void ShowNumberByPos(const int& pos, const double& numberDouble);
	void ShowNumberByPosTCC(const int& pos, string numberString);

    void runAction(float dt);
	void runAction2(float dt);
	void runActionTCC(float dt);
    void callbackShowNumber(float dt);
    void ShowChatByPos(const int& pos, string& strMsg);
	void ShowChatByPos(const int& pos, const char* strMsg);
	void ShowChatByPosTCC(const int& pos, string& strMsg);
};

#endif
