//
//  LayerPopupBanMoc.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerPopupBanMoc_hpp
#define LayerPopupBanMoc_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "LayerMuaThe.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupBanMoc: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerPopupBanMoc();
    ~LayerPopupBanMoc();
    virtual bool init();
    void hideLayer();
    void onBtnClose(Ref* pSender);
    void onBtnTiepTuc(Ref* pSender);
    void onBtnCapNhatSTK(Ref* pSender);
    void onBtnChonSTK(Ref* pSender);
    void onBtnMenhGia(Ref* pSender);
    void onBtnTaiKhoan(Ref* pSender);
    void onBtnXacNhan(Ref* pSender);
    void onBtnBack(Ref* pSender);
    void onMenhGia(Ref* pSender);
    void showLayer();
    void setDaiLy(string daily);
    void onPopupCallBack(Ref *pSender);

    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    CREATE_FUNC(LayerPopupBanMoc);
private:
    Text* txtDaiLy;
    Button* btnChonSTK;
    Button* btnMenhGia;
    Button* btnTK;
    TextField* txtLyDo;
    Node* nodeDaiLy;
    Node* nodeOTP;
    TextField* txtOTP;
    
    struct bank{
        int id;
        string ma;
        string name;
        string chutk;
        string sotk;
        string chinhanh;
    };
    
    bank info;
    
    Layout* pnlMenhGia;
    
    ui::ScrollView* scrollView;
    
    Text* txtErrorSoTK;
    Text* txtErrorMenhGia;
    Text* txtErrorLyDoChuyenKhoan;

    bool isChonSTK = false;
    bool isChonMenhGia = false;
    
    bool isShowTaiKhoan = false;
    bool isShowMenhGia = false;
    bool hasBank = false;
    vector<int> menhgia;
    int menhgiachon = 0;
    float tax = 0;
    float fee = 0;
    Text* txtSoMocNhanDuoc;

};

#endif /* LayerPopupBanMoc_hpp */
