//
//  LayerKetQuaRuongVip.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/10/16.
//
//

#ifndef LayerKetQuaRuongVip_hpp
#define LayerKetQuaRuongVip_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerKetQuaRuongVip : public cocos2d::Layer
{
public:
    LayerKetQuaRuongVip();
    ~LayerKetQuaRuongVip();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    void setMoney(string money);
    void showFacebookUser();
    
    CREATE_FUNC(LayerKetQuaRuongVip);
    
    
    
private:
    ImageView* img1;
    ImageView* img2;
    ImageView* img3;
    ImageView* img4;
    ImageView* img5;
    ImageView* img6;
    ImageView* img7;
    ImageView* img8;
    ImageView* img9;
    ImageView* img10;
    Button* btnClose;
    
    
    
    Sprite* imgTrungLon;
    Sprite* iconBac;
    Sprite* sprThongBao;
    
    vector<ImageView*> imgMoney;
    
};
#endif /* LayerKetQuaRuongVip_hpp */
