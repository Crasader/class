//
//  ThreadFunction.cpp
//  iCasino_v4-mobile
//
//  Created by Sherlock Tuan on 7/17/18.
//

#include "ThreadFunction.h"
#include <unistd.h>
void ThreadFunction::runEvent(SEL_CallFunc mCallBackListener,Ref *mCallBack){
    this->mCallBack = mCallBack;
    this->mCallBackListener = mCallBackListener;
    std::thread tA(&ThreadFunction::callFunction, this );
    tA.detach();
}
void ThreadFunction::callFunction(){
    if (mCallBack && mCallBackListener){
        while (true)
        {
            mutex.lock();
            if (this->isRunning)
            {
                usleep(this->delayTime);
                if (this->isRunning){
                    (mCallBack->*mCallBackListener)();//(param);
                    mutex.unlock();
                }else{
                    mutex.unlock();
                    break;
                }
            }
            else {
                mutex.unlock();
                break;
                
            }
        }
    }
}
void ThreadFunction::runEvent(SEL_CallFunc mCallBackListener,Ref *mCallBack, int delay){
    
}
void ThreadFunction::runEventOnece(SEL_CallFunc mCallBackListener,Ref *mCallBack){
    
}
void ThreadFunction::stopEvent(){
    this->isRunning = false;
}
