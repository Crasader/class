//
//  StructHistory.h
//  demo
//
//  Created by AlexDao on 5/25/14.
//
//

#ifndef demo_StructHistory_h
#define demo_StructHistory_h

#include "cocos2d.h"

using namespace cocos2d;

struct StructHistory{
    int ID;
    std::string* name;
    int numOfLevel;
    int numOfWin;
    int numOfLose;
};

#endif
