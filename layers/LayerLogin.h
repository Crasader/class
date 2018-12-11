//
//  LayerLogin.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerLogin_hpp
#define LayerLogin_hpp

#include "layers/LayerPopupThongBao.h"
#include "cocos2d.h"
#include "../Common.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layers/LayerPopupForgetPassword.h"
#include "../layers/LayerPopupThongBao.h"
#include "../layergames/NativeHelper.h"
#include "network/HttpClient.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;



class LayerLogin: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerLogin();
    ~LayerLogin();
    
    int currLoginType;
    static LayerLogin* instance;
    struct SocialNetworkAcc{
        std::string LT;
        std::string uid;
        std::string name;
        std::string email;
        std::string urlAvatar;
        std::string urlWallPicture;
    
    public:
        SocialNetworkAcc() :
            LT(""),
            uid(""),
            name(""),
            email(""),
            urlAvatar(""),
            urlWallPicture("")
        {
        };
        void clear(){
            LT = "";
            uid = "";
            name = "";
            email = "";
            urlAvatar = "";
            urlWallPicture = "";
        }
    };
    enum {
        loginType,
        registType,
        facebookType,
        gmailType,
        quickType
    };
    SocialNetworkAcc socialInfo;

    static LayerLogin* getInstance();
    
    virtual bool init();
    void hideLayer();
    void showLayer();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(Touch *touch, Event *unused_event);

    virtual void onEnter() override;
    virtual void onExit() override;
    
    void onBtnDangNhap(Ref* pSender);
    void onBtnDangKy(Ref* pSender);
    void onBtnDangNhapFB(Ref* pSender);
    void onBtnBackDangNhap(Ref* pSender);
    void onBtnBackDangKy(Ref* pSender);
    void onBtnDangNhapVaoGame(Ref* pSender);
    void onButtonDongy(Ref* pSender);
    void onChooseLang(Ref* pSender);

    void onButtonForgetPassword(Ref* pSender);
    void doLogin(const int& type /*= 0*/);
    void readInfo();
    void saveInfo();
    void scheduleUpdateEditbox(float dt);
    void onPopupCallBack(Ref *pSender);
    void showPopupThongbao();
    void onBtnDangKyVaoGame(Ref* pSender);
    void onBtnCapNhatTaiKhoan(Ref* pSender);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
    void getPartnerInfo();
    void timeoutLoadingCallback();
    void showUpdateInfo(float dt);
    void showMaintain(float dt);
    void gotoLogin(float dt);
    void startTimeOutChat();
    void onBtnCapNhatTen(Ref* pSender);
    void timeoutChatMaxtime(float dt);
    void onButtonChoiNgay(Ref* pSender);
    void updateFB(float dt);
    void reloadLang();
    void scheduleUpdateContactInfo(float dt);
    void onButtonHotLineClicked(Ref* pSender);
    void onSubcribe(Ref* pSender);
    CREATE_FUNC(LayerLogin);
protected:
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxConnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxLogin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxLogout(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxLoginError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
    Node* nodeButton;
    Node* nodeDangNhap;
    Node* nodeDangKy;
    Node* nodeCapNhatTen;
    Node* nodeCapNhatTaiKhoan;
    
    Layout* pnlBG;
    
    TextField* txtUserName ;//= dynamic_cast<TextField*>(this->nodeDangNhap->getChildByName("tfTaiKhoan"));
    TextField* txtPassword;
    
    TextField* txtRegUserName;
    TextField* txtRegPassword;
    TextField* txtReRegUserName;
    
    TextField* tfTenNhanVat;
    Text* txtUserNameMe;
    Text* txtError;
    
    TextField* tfTenTaiKhoan;
    Text* txtErrorTaiKhoan;
    
    ImageView* bgInput;
    
    bool isShow;
    bool isShouldAuto;
    static bool instanceFlag;
    int const tagChildNode = 113;
    
    string partnerID;
    string srcID;
    
    vector<string> lstDatas;
    int currentPop = 0;
    Text* labelFacebook;
    Button* btnDangNhapFB;
    Button* btnVN;
    Button* btnCAM;
    Button* btnENG;
    Button* btnCN;
    Button* btnQuenMatKhau;
    Button* btnQuenMatKhau1;
    Button* btnDangNhap;
    Button* btnDangKy;
    Button* btnChoiNgay;
    Button* btnDangNhapVaoGame;
    Button* btnDangKyVaoGame;
    Button* btnCapNhatTaiKhoan;
    Button* btnCapNhat;
    Button* btnChoiNgayGuide;
    Button* btnFacebookGuide;
    Button* btnDangkyGuide;
    
    Layout* pnlBgButton;
    ImageView* bgButton;
    bool isShouldShowGuide;
    bool isShowTouch;
    bool isLogin;
    Text* lblHotLine;
    ImageView* imgHotLine;
    Text* txtNhanGuide;
    Text* txtGoldBonusFB;
    Text* txtGoldBonusFB2;
    Text* txBonusFaecbookNew;
    Button* btnDangNhapFBNew;
    Text* txtDangKyNew;
    
    Layout* pnlLogin;
    Layout* pnlRegist;
};

#endif /* LayerLogin_hpp */
