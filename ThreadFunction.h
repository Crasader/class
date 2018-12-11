//
//  ThreadFunction.hpp
//  iCasino_v4-mobile
//
//  Created by Sherlock Tuan on 7/17/18.
//

#ifndef ThreadFunction_hpp
#define ThreadFunction_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;

class ThreadFunction{
public:
    std::mutex mutex;
    
    SEL_CallFunc mCallBackListener;
    Ref *mCallBack;
    void runEvent(SEL_CallFunc mCallBackListener,Ref *mCallBack);
    void runEvent(SEL_CallFunc mCallBackListener,Ref *mCallBack, int delay);
    void runEventOnece(SEL_CallFunc mCallBackListener,Ref *mCallBack);
    bool isRunning = true;
    void stopEvent();
    void callFunction();
    int delayTime = 20000;
};

#endif /* ThreadFunction_hpp */
