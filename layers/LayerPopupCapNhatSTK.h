//
//  LayerPopupCapNhatSTK.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerPopupCapNhatSTK_hpp
#define LayerPopupCapNhatSTK_hpp

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
class LayerPopupCapNhatSTK: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerPopupCapNhatSTK();
    ~LayerPopupCapNhatSTK();
    virtual bool init();
    void hideLayer();
    void onBtnClose(Ref* pSender);
    void onBtnDoi(Ref* pSender);
    void onBtnChonNganHang(Ref* pSender);
    void onBtnXacNhan(Ref* pSender);
    void onBtnBack(Ref* pSender);
    void onNganHang(Ref* pSender);
    bool validateName(string src);
    bool isWhitespace(std::string s);
    void showLayer();
    bool utf8_check_is_valid(const string& string);
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    CREATE_FUNC(LayerPopupCapNhatSTK);
private:

    Button* btnChonNganHang;
    TextField* txtTK;
    Node* nodeSTK;
    Node* nodeOTP;
    TextField* txtChiNhanh;
    TextField* txtOTP;
    TextField* txtChuTK;
    struct bank{
        int id;
        string name;
        string fullname;
    };
    vector<bank> lstbank;
    bool isShowNganHang = false;

    Layout* pnlNganHang;
    
    ui::ScrollView* scrollView;
    
    Text* txtErrorTK;
    Text* txtErrorChiNhanh;
    Text* txtErrorChuTK;
    Text* txtErrorTenNganHang;
    int currentClick = -1;
};

#endif /* LayerPopupCapNhatSTK_hpp */
