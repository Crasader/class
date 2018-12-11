//
//  LayerGiftCode.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/24/17.
//
//

#ifndef LayerGiftCode_hpp
#define LayerGiftCode_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../GameServer.h"
#include "Requests/ExtensionRequest.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerGiftCode: public cocos2d::Layer,public PlayerCallBack{
public:
    LayerGiftCode();
    ~LayerGiftCode();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnXacNhan(Ref* pSender);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void hideLayer();
    void showLayer();
    CREATE_FUNC(LayerGiftCode);
private:
    TextField* tfGifCode;
};

#endif /* LayerGiftCode_hpp */
