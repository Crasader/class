//
//  LayerWelcomeNewbieNormal.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/7/16.
//
//

#ifndef LayerWelcomeNewbieNormal_hpp
#define LayerWelcomeNewbieNormal_hpp

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
class LayerWelcomeNewbieNormal : public cocos2d::Layer,
public PlayerCallBack
{
public:
    LayerWelcomeNewbieNormal();
    ~LayerWelcomeNewbieNormal();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnLKFBClick(Ref* pSender);
    void onBtnMissionFBClick(Ref* pSender);
    void onBtnClose(Ref* pSender);
    void setMoney(string money);
    void showFacebookUser();
    void gotoDiemDanh();
    void doLienKetFB(Node* sender, void* data);
    
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    
    CREATE_FUNC(LayerWelcomeNewbieNormal);
private:
    ImageView* img1;
    ImageView* img2;
    ImageView* img3;
    ImageView* img4;
    ImageView* img5;
    Button* btnLKFB;
    Button* btnMisisonFB;
    Button* btnClose;
    Text* txtGuide;
    
    ImageView* imageLK;
    Sprite* CoinImg;
	TextBMFont* lbmMoney;
    
    //vector<ImageView*> imgMoney;
    
};

#endif /* LayerWelcomeNewbieNormal_hpp */
