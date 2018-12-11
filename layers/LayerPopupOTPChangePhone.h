//
//  LayerPopupOTPChangePhone.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 10/27/16.
//
//

#ifndef LayerPopupOTPChangePhone_hpp
#define LayerPopupOTPChangePhone_hpp

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
class LayerPopupOTPChangePhone : public cocos2d::Layer,
public PlayerCallBack
{
public:
    LayerPopupOTPChangePhone();
    ~LayerPopupOTPChangePhone();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void onBtnXacThucOTP(Ref* pSender);
    void onBtnDoiSDT(Ref* pSender);
    void onButtonClose(Ref* pSender);
    void onBtnGetOTP(Ref* pSender);
   
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    CREATE_FUNC(LayerPopupOTPChangePhone);
private:
    TextField* txtNhapSDT;
    TextField* txtOTP;
    Button* btnDoiSDT;
    Button* btnXacThuc;
    Button* btnGetOTP;
    ImageView* imgbgsdt;
    
};

#endif /* LayerPopupOTPChangePhone_hpp */
