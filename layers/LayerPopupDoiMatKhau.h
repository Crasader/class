//
//  LayerPopupDoiMatKhau.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/19/17.
//
//

#ifndef LayerPopupDoiMatKhau_hpp
#define LayerPopupDoiMatKhau_hpp

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
class LayerPopupDoiMatKhau: public cocos2d::Layer,public PlayerCallBack{
public:
    LayerPopupDoiMatKhau();
    ~LayerPopupDoiMatKhau();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void onBtnCapNhat(Ref* pSender);
    void onBtnOTP(Ref* pSender);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    CREATE_FUNC(LayerPopupDoiMatKhau);
private:
    TextField* txtOldPass;
    TextField* txtmatkhauMoi;
    TextField* txtnhaplaiMatKhauMoi;
    string _oldPass;
    Text* lblError;

};


#endif /* LayerPopupDoiMatKhau_hpp */
