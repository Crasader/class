//
//  LayerPopupOTP.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 10/27/16.
//
//

#ifndef LayerPopupOTP_hpp
#define LayerPopupOTP_hpp

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
class LayerPopupOTP : public cocos2d::Layer,
public PlayerCallBack
{
public:
    LayerPopupOTP();
    ~LayerPopupOTP();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void onBtnKichHoatOTP(Ref* pSender);
    void onBtnGetOTP(Ref* pSender);
    void onBtnDoiSDT(Ref* pSender);
    void onButtonClose(Ref* pSender);
    void onBtnHuyOTP(Ref* pSender);
    void onBtnGuideOTP(Ref* pSender);
    
    //
    void onBtnXacThucOTPChangePhone(Ref* pSender);
    void onBtnDoiSDTChangePhone(Ref* pSender);
    void onBtnGetOTPChangePhone(Ref* pSender);
    void onBtnBackChangePhone(Ref* pSender);
    
    //

    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    CREATE_FUNC(LayerPopupOTP);
private:
    Node* layerDienThoai;
    Node* layerChangePhone;
    TextField* txtNhapSDT;
    TextField* txtOTP;
    ImageView* imgbgsdt;
    Text* mainTitle;
    
    Text* changePhoneTitle;

    Button* btnGetOTP;
    Button* btnKichHoatOTP;
    Button* btnDoiSDT;
    Button* btnHuyOTP;
    
    //
    TextField* txtNhapSDTChangePhone;
    TextField* txtOTPChangePhone;
    Button* btnDoiSDTChangePhone;
    Button* btnXacThucChangePhone;
    Button* btnGetOTPChangePhone;
    ImageView* imgbgsdtChangePhone;
    string strOTPSDTCu;
    bool shouldGetOTPNewPhone;
    
   
};

#endif /* LayerPopupOTP_hpp */
