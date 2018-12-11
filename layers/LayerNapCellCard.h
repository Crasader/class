//
//  LayerNapCellCard.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerNapCellCard_hpp
#define LayerNapCellCard_hpp

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
class LayerNapCellCard: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerNapCellCard();
    ~LayerNapCellCard();
    virtual bool init();
    

    void onBtnNapThe(Ref* pSender);
    void onMenhGia(Ref* pSender);
    void onBtnChiTiet(Ref* pSender);
    void hideLayer();
    void showLayer();
    void loadTyGia();
    void updateValueTextField(float dt);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    
    //bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerNapCellCard);
private:
    
   
    ui::ScrollView* scrollTyGia;
    ui::ScrollView* scrollView;
    Layout* pnlMenhGia;
    bool isShowSelect = false;
    int typeNhamang = 0;
    double mTax = 0;
    
    TextField* txtPhoneNumber;
    TextField* txtMoney;

    Text* txtErrorPhoneNumber;
    Text* txtErrorMoney;
    Text* txtTienNhan;
    Text* txtKhuyenMai;

    vector<int> the;
    vector<int> money;
    vector<int> khuyenmai;

    vector<Button*> lstMenhGia;
    int currMenhgia = 0;
    int amount = 0;
    int moneyNhap = 0;

};
#endif /* LayerNapCellCard_hpp */
