//
//  LayerHuThuong.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/25/16.
//
//

#ifndef LayerHuThuong_hpp
#define LayerHuThuong_hpp

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
class LayerHuThuong : public cocos2d::Layer,
public PlayerCallBack
{
public:
    LayerHuThuong();
    ~LayerHuThuong();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    void setMoney(string money);
    void CallFuncRemove();
    
    CREATE_FUNC(LayerHuThuong);
private:
    ImageView* imghu;
    ImageView* imgbg;
    Label* labelResult;
    Node* rootNode;
    Node* clipper;
    bool isGetBonus;
    int moneyBonus;
    bool isNoHu;
};

#endif /* LayerHuThuong_hpp */
