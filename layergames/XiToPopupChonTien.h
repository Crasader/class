//
//  XiToPopupChonTien.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/27/16.
//
//

#ifndef XiToPopupChonTien_hpp
#define XiToPopupChonTien_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "LayerPlayGameTXMini.h"
#include "../GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class XiToPopupChonTien : public cocos2d::Layer,public PlayerCallBack
{
public:
    XiToPopupChonTien();
    ~XiToPopupChonTien();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    void onBtnDongY(Ref* pSender);
    void btnHuyBo(Ref* pSender);
    void onBtnMangVaoKhiHetBac(Ref* pSender);
    void onBtnBoSungBac(Ref* pSender);
    void sliderEvent(Ref *pSender, Slider::EventType type);
    float formatValue(float value);
    void setMinMaxValue(float muccuoc, float muctoithieuvaoban);
    void setJoinType(int type);
    
    void onBtnSubPercent(Ref* pSender);
    void onBtnAddPercent(Ref* pSender);
    void displayMoney(int percent);
    
    string getTime(string str);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    void onBtnClose(Ref* pSender);
    CREATE_FUNC(XiToPopupChonTien);
private:
    Text* txtBacMangVao;
    Text* txtMinMoney;
    Text* txtMaxMoney;
    
    ImageView* imgAutoBoSungKhiHetBac;
    ImageView* imgAutoBoSungSauMoiVan;
    
    
    int strBacMangVao = 0;
    Slider* slider;
    float maxValue = 0;
    float minValue = 0;
    float currBacMangVao = 0;
    float bacMangVao = 0;
    
    float myMoney;
    int currentJoinType = 0; // 0 = choi bt, 1 = choi ngay, 2 = dang trong ban
    int betValue= 0;
    int isAutoBoSungBacSauMoiVan = 1;
    int isAutoBoSungBacKhiHet = 1;
};


#endif /* XiToPopupChonTien_hpp */
