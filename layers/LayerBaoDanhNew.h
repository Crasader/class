//
//  LayerBaoDanhNew.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerBaoDanhNew_hpp
#define LayerBaoDanhNew_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerBaoDanhNew: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerBaoDanhNew();
    ~LayerBaoDanhNew();
    virtual bool init();
    void onBtnNhanThuong(Ref* pSender);
    void onBtnClose(Ref* pSender);

    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    CREATE_FUNC(LayerBaoDanhNew);
private:
    
    vector<Text*> lstTextNgay;
    vector<Text*> lstTextMoney;
    vector<Text*> lstTextCoin;
    vector<Sprite*> lstSpriteDone;

    vector<Button*> lstButton;
    Layout* bgBaoDanh;
};

#endif /* LayerBaoDanhNew_hpp */
