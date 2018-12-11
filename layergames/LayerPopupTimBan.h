//
//  LayerPopupTimBan.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/18/16.
//
//

#ifndef LayerPopupTimBan_hpp
#define LayerPopupTimBan_hpp
#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;

class LayerPopupTimBan : public cocos2d::Layer,public PlayerCallBack
{
public:
    LayerPopupTimBan();
    ~LayerPopupTimBan();
    virtual bool init();
    
       bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnTimBan(Ref* pSender);

    CREATE_FUNC(LayerPopupTimBan);
private:
    TextField* tfBanChoi;
    TextField* tfMatKhau;
    ImageView* imgMatKhau;
};

#endif /* LayerPopupTimBan_hpp */

