//
//  _Chat_inGame_.h
//  iCasinov2
//
//  Created by Thanhhv on 5/27/14.
//
//

#ifndef __iCasinov2___Chat_inGame___
#define __iCasinov2___Chat_inGame___

#include <iostream>
#include "cocos2d.h"
#include "AllData.h"
#include "_Chat_.h"

using namespace std;
using namespace cocos2d;

class LayerChatToastInGame : public Layer
{
public:
    bool init();
	CREATE_FUNC(LayerChatToastInGame);
    
    void showChatByPos(int posUser, string mes);
};

#endif /* defined(__iCasinov2___Chat_inGame___) */
