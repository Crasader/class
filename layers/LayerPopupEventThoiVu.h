//
//  LayerPopupEventThoiVu.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerPopupEventThoiVu_hpp
#define LayerPopupEventThoiVu_hpp

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
class LayerPopupEventThoiVu: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerPopupEventThoiVu();
    ~LayerPopupEventThoiVu();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onBtnClose(Ref* pSender);
    void onBtnAction(Ref* pSender);
    void shareSucess(Node* sender, void* data);
    void setCurrentEvent(int eventNum);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerPopupEventThoiVu);
private:
    
    Button* btnAction;
    Text* txtName;
    Text* txtContent;
    int eventNum = 0;
    
};

#endif /* LayerPopupEventThoiVu_hpp */
