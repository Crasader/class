//
//  SoundManager.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/26/18.
//

#ifndef SoundManager_hpp
#define SoundManager_hpp

#include <stdio.h>
#include "SimpleAudioEngine.h"

using namespace std;
using namespace CocosDenshion;
class SoundManager{
    
public:
    
    static void playEffectConnectDot(){
        SimpleAudioEngine* audio = SimpleAudioEngine::getInstance();
        audio->playEffect("starting.flac", false, 1.0f, 1.0f, 1.0f);
        
    };
};
#endif /* SoundManager_hpp */
