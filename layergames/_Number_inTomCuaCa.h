//
//  _Number_inGame_.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/16/14.
//
//

#ifndef __iCasino_v2___Number_inTomCuaCa___
#define __iCasino_v2___Number_inTomCuaCa___

#include <iostream>

#include "cocos2d.h"
#include "AllData.h"
#include "_Number_.h"

using namespace std;
using namespace cocos2d;

class NumberInTomCuaCa : public Layer
{
private:
    vector<int> arrPos;
    vector<string> arrNumberString;
	bool isRunning;
	vector<double> arrNumberDouble;
    
public:
	~NumberInTomCuaCa();
    bool init();
    CREATE_FUNC(NumberInTomCuaCa);
    
    void showNumberByPos(int pos, string numberString);
	void showNumberByPos(int pos, double numberDouble);
	void runAction(float dt);
    void callbackShowNumber(float dt);
	void runAction2(float dt);
};

#endif /* defined(__iCasino_v2___Number_inGame___) */
