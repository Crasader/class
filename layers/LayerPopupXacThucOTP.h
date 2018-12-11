//
//  LayerPopupXacThucOTP.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 10/28/16.
//
//

#ifndef LayerPopupXacThucOTP_hpp
#define LayerPopupXacThucOTP_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
class LayerPopupXacThucOTP : public Layer,
public	PlayerCallBack
{
public:
    LayerPopupXacThucOTP();
    ~LayerPopupXacThucOTP();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void onButtonHuy(Ref* pSender);
    void onButtonDongy(Ref* pSender);
    void setMessage(string msg);
    void setContentMess(string msg);
    void showAButton();
    void onBtnGetOTP(Ref* pSender);
    
    Button* getBtnHuy();
    Button* getBtnOk();
    Text* getLblTitle();
    
    void setCallBackFromButton(Ref* target, SEL_CallFuncND listener);
    
    virtual bool init();
    CREATE_FUNC(LayerPopupXacThucOTP);
protected:
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
    cocos2d::EventListener* _touchListener;
    Text* lblMessage;
    Text* lblTitle;
    TextField* txtOTP;
    Button *btn_huy, *btn_dongy;
    SEL_CallFuncND mCallBackListener;
    Ref *mCallBack;
    int tagcallback;
    void sendRequestCountMess(float dt);
};


#endif /* LayerPopupXacThucOTP_hpp */
