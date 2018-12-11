//
//  LayerMenuRuongVip.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/10/16.
//
//

#ifndef LayerMenuRuongVip_hpp
#define LayerMenuRuongVip_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "Requests/ExtensionRequest.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerMenuRuongVip :
public cocos2d::Layer,
public PlayerCallBack
{
public:
    LayerMenuRuongVip();
    ~LayerMenuRuongVip();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void gotoRuongVip(Ref* pSender);
    virtual void onEnter() override;
    virtual void onExit() override;
    void setValueChest(string value,string num);
    void getInfoChest(float dt);
    
    
    
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    CREATE_FUNC(LayerMenuRuongVip);
private:
    Text* txtValue;
    Text* txtNum;
};


#endif /* LayerMenuRuongVip_hpp */
