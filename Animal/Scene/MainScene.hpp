//
//  MainScene.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/22/18.
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"
#include "stdio.h"

#include "network/HttpRequest.h"
using namespace cocos2d::network;

class MainScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
private:
    void onHttpRequestCompleted(HttpClient* sender, HttpResponse* response);
};



#endif /* MainScene_hpp */
