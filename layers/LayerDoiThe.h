//
//  LayerDoiThe.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerDoiThe_hpp
#define LayerDoiThe_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define MOBI 2
#define VINA 1
#define VIETTEL 3
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerDoiThe: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerDoiThe();
    ~LayerDoiThe();
    virtual bool init();
    

    void onBtnDoiThe(Ref* pSender);
    void onMenhGia(Ref* pSender);
    void onBtnNhaMang(Ref* pSender);
    void hideLayer();
    void showLayer();
    void loadTyGia();
    void popUpCallBack(Ref* pSender);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    
    //bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerDoiThe);
private:
    
   
    ui::ScrollView* scrollTyGia;
    Layout* pnlMenhGia;
    bool isShowSelect = false;
    int typeNhamang = 0;
    Text* txtMoneyNeed;
    TextField* txtWingAccount;
    Text* txtErrorWingAccount;
    
    Button* btnSmart;
    Button* btnMetPhone;
    Button* btnCellCard;

    struct mathe{
        int menhgia;
        int money;
    };
    vector<mathe> the;
    vector<Button*> lstMenhGia;
    int currMenhgia = 0;
    int currMoney = 0;
    string listcard = "";
    bool cellcard;
    bool smart;
    bool metphone;
    Layout* _layerContent;

};
#endif /* LayerDoiThe_hpp */
