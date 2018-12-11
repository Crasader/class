//
//  _Chat_inGame_.cpp
//  iCasinov2
//
//  Created by Thanhhv on 5/27/14.
//
//

#include "_Chat_inGame_.h"
#include "mUtils.h"

bool LayerChatToastInGame::init() {
    if (!Layer::init()) return false;
    
    this->setAnchorPoint(Point(0, 0));
    
    return true;
}

void LayerChatToastInGame::showChatByPos(int pos, string mes) {
    Chat *newMes = Chat::create(mes, pos);
    newMes->setStatusByServer(false);
	Size sizeAd = mUtils::getSizePos();
    Point point;
    
    switch (pos) {
        case kUserLeft:
            point.setPoint(10 - sizeAd.width , 175 + 137);
            break;
            
        case kUserRight:
            point.setPoint(WIDTH_DESIGN - newMes->getSize().width - 10 + sizeAd.width, 175 + 137);
            break;
            
        case kUserTop:
            point.setPoint((WIDTH_DESIGN - newMes->getSize().width) / 2, 343 + sizeAd.height);
            break;
            
        case kUserBot: case kUserMe:
            point.setPoint((WIDTH_DESIGN - newMes->getSize().width) / 2, 70 - sizeAd.height);
            break;
        
        default:
            point.setPoint((WIDTH_DESIGN - newMes->getSize().width) / 2, (HEIGHT_DESIGN - newMes->getSize().height) / 2);
            newMes->setStatusByServer(true);
            break;
    }
    
    newMes->setPosition(point);
    this->addChild(newMes);
}