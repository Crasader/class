//
//  LayerPopupBaoMatCaNhan.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/19/17.
//
//

#ifndef LayerPopupBaoMatCaNhan_hpp
#define LayerPopupBaoMatCaNhan_hpp

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
class LayerPopupBaoMatCaNhan: public cocos2d::Layer,public PlayerCallBack{
public:
    LayerPopupBaoMatCaNhan();
    ~LayerPopupBaoMatCaNhan();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnCapNhat(Ref* pSender);
    void onBtnOTP(Ref* pSender);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    CREATE_FUNC(LayerPopupBaoMatCaNhan);
private:
    TextField* txtCMTND;
    TextField* txtEmail;
    TextField* txtSDT;
    
    Text* txtErrorCMT;
    Text* txtErrorSDT;
    Text* txtErrorEmail;

};


#endif /* LayerPopupBaoMatCaNhan_hpp */
