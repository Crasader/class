//
//  Number.h
//  iCasinoCard
//
//  Created by Thanhhv on 5/9/14.
//

#ifndef __iCasinoCard__Number__
#define __iCasinoCard__Number__

#include "cocos2d.h"
#include "mUtils.h"

USING_NS_CC;
using namespace std;

class Number : public Layer
{
private:
    string numberString;
    string style;
    
    Size sizeThis;
    Point pointStart;

public:

	static Number* create(const string& pNumberString);
	static Number* create(const double& pNumberDouble);

    Number(const string& pNumberString);
	Number(const double& pNumberDouble);
    ~Number();
    
    virtual bool init();
    
    void startRunAction();
    void startRunAction(float toY);

    Sprite* getNumberWithChar(char a);
    void setNumbersWithString(string pNumberString);
    
    // setter, getter
    void setSize(int wid, int hei);
    Size getSize();
    void setPositionStart(Point pPointStart);
	void setPositionStart(Point pPointStart, float toY);

    Point getPositionStart();
};

#endif /* defined(__iCasinoCard__Number__) */
