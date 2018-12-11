//
//  LayerLogin.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerLogin.h"
#include "Requests/LoginRequest.h"
#include "Requests/ExtensionRequest.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../data_manager.h"
#include "boost/algorithm/string.hpp"
#include "../layergames/PhomMessDef.h"
#include "../layergames/_Chat_.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "Requests/LogoutRequest.h"
#include "Util/PasswordUtil.h"
#include "layergames/ChanUtils.h"
#include "../scenes/SceneMain.h"
#include "../scenes/SceneLogin.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "PluginFacebook/PluginFacebook.h"
#include "mFaceBook.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h>
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <string>

#pragma comment(lib, "wbemuuid.lib")
#endif

USING_NS_CC;

#define ZORDER_POPUP 2000
#define ZORDER_LOADING 20001

LayerLogin* LayerLogin::instance = NULL;
std::string gPlusUid = "";
std::string gPlusName = "";
std::string gPlusEmail = "";
std::string gPlusAvatar = "";
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
extern "C" {
	//org/cocos2dx/cpp/AppActivity
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_loginGPlus(JNIEnv*  env, jobject thiz, jstring uid, jstring name, jstring email, jstring urlAvatar) {
		gPlusUid = JniHelper::jstring2string(uid);
		gPlusName = JniHelper::jstring2string(name);
		gPlusEmail = JniHelper::jstring2string(email);
		gPlusAvatar = JniHelper::jstring2string(urlAvatar);
		log("in cocos2d-x %s : %s : %s : %s", gPlusUid.c_str(), gPlusName.c_str(), gPlusEmail.c_str(), gPlusAvatar.c_str());
		//        if (LayerLogin::instance != NULL)
		//        {
		//            LayerLogin::instance->socialInfo.uid = gPlusUid;
		//            LayerLogin::instance->socialInfo.name = gPlusName;
		//            LayerLogin::instance->socialInfo.email = gPlusEmail;
		//            LayerLogin::instance->socialInfo.urlAvatar = gPlusAvatar;
		//            if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
		//                return;
		//            }
		//            LayerLogin::getInstance()->currLoginType = LayerLogin::gmailType;
		//            if (!GameServer::getSingleton().getSmartFox()->IsConnected())
		//            {
		//                SceneManager::getSingleton().connect2Server(0);
		//            }
		//            else
		//            {
		//                //GameServer::getSingleton().getSmartFox()->Disconnect();
		//                SceneManager::getSingleton().connect2Server(0);
		//            }
		//        }
	}
}
#endif
#include "layergames/PhomMessDef.h"
#include "Requests/SetUserVariablesRequest.h"
#include "Entities/Variables/SFSUserVariable.h"

LayerLogin* LayerLogin::getInstance(){
	if (instance == NULL)
	{
		instance = LayerLogin::create();
		return instance;
	}
	else{
		return instance;
	}
}


LayerLogin::LayerLogin()
{
	GameServer::getSingleton().addListeners(this);

}


LayerLogin::~LayerLogin()
{
	GameServer::getSingleton().removeListeners(this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLogin::scheduleUpdateContactInfo),this);
    this->unschedule(schedule_selector(LayerLogin::gotoLogin));

}

bool LayerLogin::init()
{
	if (!Layer::init())
	{
		return false;
	}

//    if (SceneManager::getSingleton().getIsFlagStore()){
//        SceneLogin* layer = SceneLogin::create();
//        this->addChild(layer);
//        return true;
//    }
    
    
    SceneMain::getSingleton().currentLayer = SceneMain::getSingleton().typelayerLogin;
	instance = this;
	isShouldAuto = true;
	isShow = true;
	this->setKeyboardEnabled(true);
    isLogin = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


    string path = "LayerLoginCam2.csb";
    if (SceneManager::getSingleton().getIsFlagStore()){
        path = "LayerLoginStore.csb";
    }
	auto rootNode = CSLoader::getInstance()->createNode(path);
    
   

	auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("pnlBG"));
    Panel_2->setSwallowTouches(false);

	Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    pnlBG = Panel_2;
    pnlBG->setOpacity(0);

	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	Size sizeAdd;
//	if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
		sizeAdd = ChanUtils::getSizePos();

//    }
//    else{
//        sizeAdd = Size(0, 0);
//    }
	Size sizeAdd2 = Size(0,0);
	rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));

	ui::Helper::doLayout(rootNode);


	this->addChild(rootNode);

	UserDefault::getInstance()->setBoolForKey("isAutoLogin", false);

	auto pnlParent = static_cast<Layout*>(rootNode->getChildByName("pnlParent"));
    pnlParent->setSwallowTouches(false);
    this->pnlBgButton = static_cast<Layout*>(pnlParent->getChildByName("pnlGuide"));
    this->pnlBgButton->setSwallowTouches(false);
    this->pnlBgButton->setVisible(false);
    this->isShowTouch = true;
    this->isShouldShowGuide = true;
    this->bgButton = static_cast<ImageView*>(this->pnlBgButton->getChildByName("Image_3"));
    this->txtNhanGuide = static_cast<Text*>(this->pnlBgButton->getChildByName("txtNhan"));
    this->txtNhanGuide->setString(dataManager.GetSysString(988));
    this->txtGoldBonusFB2 = static_cast<Text*>(this->pnlBgButton->getChildByName("Text_2_0"));
    if (SceneManager::getSingleton().isNagaWin){
        txtGoldBonusFB2->setVisible(false);
    }
    //
    
	this->nodeButton = static_cast<Node*>(pnlParent->getChildByName("nodeButton"));
    this->nodeDangNhap = static_cast<Node*>(pnlParent->getChildByName("nodeDangNhap"));
    if (SceneManager::getSingleton().isNagaNew){
        this->nodeDangNhap = static_cast<Layout*>(pnlParent->getChildByName("pnlDangNhap"));
    }
    this->btnVN = static_cast<Button*>(this->nodeButton->getChildByName("btnVN"));
    if (btnVN){
        btnVN->addClickEventListener(CC_CALLBACK_1(LayerLogin::onChooseLang, this));
        this->btnVN->setTag(0);
        if (SceneManager::getSingleton().getIsFlagStore()){
            
            btnVN->setVisible(false);
        }
    }
    this->btnCAM = static_cast<Button*>(this->nodeButton->getChildByName("btnCAM"));
    if (SceneManager::getSingleton().isNagaNew){
        this->btnCAM = static_cast<Button*>(this->nodeDangNhap->getChildByName("btnCAM"));
        
    }
    if (btnCAM){
        btnCAM->addClickEventListener(CC_CALLBACK_1(LayerLogin::onChooseLang, this));
        this->btnCAM->setTag(2);
        //anhnt naga new
        if (SceneManager::getSingleton().getIsFlagStore()){

            btnCAM->setVisible(false);
        }
    }
    this->btnCN = static_cast<Button*>(this->nodeButton->getChildByName("btnCN"));
    if (btnCN){
        btnCN->addClickEventListener(CC_CALLBACK_1(LayerLogin::onChooseLang, this));
        this->btnCN->setTag(4);

    }
    
    auto Subcribe = static_cast<Button*>(rootNode->getChildByName("Subcribe"));
    if (Subcribe){
        Subcribe->addClickEventListener(CC_CALLBACK_1(LayerLogin::onSubcribe, this));
        Subcribe->setVisible(false);
    }
    
    this->btnENG = static_cast<Button*>(this->nodeButton->getChildByName("btnENG"));
     if (SceneManager::getSingleton().isNagaNew){
         this->btnENG = static_cast<Button*>(this->nodeDangNhap->getChildByName("btnENG"));

     }
    if (btnENG){
        btnENG->addClickEventListener(CC_CALLBACK_1(LayerLogin::onChooseLang, this));
        //anhnt only eng
        btnENG->setVisible(false);
        this->btnENG->setTag(1);

    }
    this->nodeButton->setPosition(Vec2(this->nodeButton->getPositionX(),this->nodeButton->getPositionY()+sizeAdd.height));
//    auto animOrgin = dynamic_cast<Sprite*>(this->nodeButton->getChildByName("animOrgin"));
//    
//    if (animOrgin){
//        Vector<SpriteFrame*> animFrames;
//        
//        string str = "";
//        for (int i = 0; i <= 25; i++) {
//            str = StringUtils::format("anims/anim_header_%d.png",i);
//            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 1811, 155));
//            animFrames.pushBack(spriteFrame);
//        }
//        
//        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.16f);
//        Animate* animN = Animate::create(animation);
//        animOrgin->runAction(RepeatForever::create(animN));
//    }
	
    this->nodeDangNhap->setPosition(Vec2(this->nodeDangNhap->getPositionX(),this->nodeDangNhap->getPositionY()+sizeAdd.height));
    Button* btnHotline = dynamic_cast<Button*>(this->nodeButton->getChildByName("btnHotline"));
    if (btnHotline){
        btnHotline->addClickEventListener(CC_CALLBACK_1(LayerLogin::onButtonHotLineClicked, this));
    }
    Button* btnChoiNgayNew = static_cast<Button*>(this->nodeDangNhap->getChildByName("btnChoiNgay"));
    if (btnChoiNgayNew){
        btnChoiNgayNew->addClickEventListener(CC_CALLBACK_1(LayerLogin::onButtonChoiNgay, this));
    }
    
    Button* btnDangKyNew = static_cast<Button*>(this->nodeDangNhap->getChildByName("btnDangKy"));
    if (btnDangKyNew){
        btnDangKyNew->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnDangKy, this));
    }
    
    txtDangKyNew = static_cast<Text*>(this->nodeDangNhap->getChildByName("txtDangKy"));
    if (txtDangKyNew){
        txtDangKyNew->setString(dataManager.GetSysString(958));
    }
    
    btnDangNhapFBNew = static_cast<Button*>(this->nodeDangNhap->getChildByName("btnFacebook"));
    if (btnDangNhapFBNew){
        btnDangNhapFBNew->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnDangNhapFB, this));
        btnDangNhapFBNew->setVisible(false);
        //btnDangNhapFBNew->setVisible(false);
    }
    
    this->txBonusFaecbookNew = static_cast<Text*>(this->nodeDangNhap->getChildByName("txBonusFaecbook"));
    
    imgHotLine = dynamic_cast<ImageView*>(this->nodeButton->getChildByName("imgHotLine"));

    this->lblHotLine = dynamic_cast<Text*>(imgHotLine->getChildByName("txtHotLine"));
    //lblHotLine->setPositionY(childNode->getPositionY() + sizeAdd.height);
    //lblHotLine->setString("0984.234.567");
    imgHotLine->setVisible(false);
    
	this->nodeDangKy = static_cast<Node*>(pnlParent->getChildByName("nodeDangKy"));
    this->nodeDangKy->setPosition(Vec2(this->nodeDangKy->getPositionX(),this->nodeDangKy->getPositionY()+sizeAdd.height));

	this->nodeCapNhatTen = static_cast<Node*>(pnlParent->getChildByName("nodeCapNhatTen"));
	this->nodeCapNhatTaiKhoan = static_cast<Node*>(pnlParent->getChildByName("nodeCapNhatTaiKhoan"));
	btnDangNhap = static_cast<Button*>(this->nodeButton->getChildByName("btnDangNhap"));
	if (btnDangNhap){
		btnDangNhap->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnDangNhap, this));
	}
	btnDangKy = static_cast<Button*>(this->nodeButton->getChildByName("btnDangKy"));
	if (btnDangKy){
		btnDangKy->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnDangKy, this));

	}
    btnDangkyGuide = static_cast<Button*>(this->pnlBgButton->getChildByName("btnDangKy"));
    if (btnDangkyGuide){
        btnDangkyGuide->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnDangKy, this));
        
    }
	btnDangNhapFB = static_cast<Button*>(this->nodeButton->getChildByName("btnDangNhapFB"));
	if (btnDangNhapFB){
		btnDangNhapFB->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnDangNhapFB, this));
        btnDangNhapFB->setVisible(false);
	}
    txtGoldBonusFB = static_cast<Text*>(this->btnDangNhapFB->getChildByName("Text_2"));
    
    
    
    btnFacebookGuide = static_cast<Button*>(this->pnlBgButton->getChildByName("btnLoginFacecbook"));
    if (btnFacebookGuide){
        btnFacebookGuide->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnDangNhapFB, this));
    }
//    labelFacebook = static_cast<Text*>(this->nodeButton->getChildByName("Text_1"));
//    labelFacebook->setVisible(false);

	auto btnBackDangNhap = static_cast<Button*>(this->nodeDangNhap->getChildByName("btnBack"));
	if (btnBackDangNhap){
		btnBackDangNhap->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnBackDangNhap, this));
	}

	auto btnBackDangKy = static_cast<Button*>(this->nodeDangKy->getChildByName("btnBack"));
	if (btnBackDangKy){
		btnBackDangKy->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnBackDangKy, this));
	}
	btnQuenMatKhau = static_cast<Button*>(this->nodeButton->getChildByName("btnQuenMatKhau"));
	if (btnQuenMatKhau){
		btnQuenMatKhau->addClickEventListener(CC_CALLBACK_1(LayerLogin::onButtonForgetPassword, this));
	}

    btnQuenMatKhau1 = static_cast<Button*>(this->nodeDangNhap->getChildByName("btnQuenMatKhau"));
    if (btnQuenMatKhau1){
        btnQuenMatKhau1->addClickEventListener(CC_CALLBACK_1(LayerLogin::onButtonForgetPassword, this));

    }

	btnChoiNgay = static_cast<Button*>(this->nodeButton->getChildByName("btnChoiNgay"));
	if (btnChoiNgay){
		btnChoiNgay->addClickEventListener(CC_CALLBACK_1(LayerLogin::onButtonChoiNgay, this));

	}
    
    btnChoiNgayGuide = static_cast<Button*>(this->pnlBgButton->getChildByName("btnChoiNgay"));
    if (btnChoiNgayGuide){
        btnChoiNgayGuide->addClickEventListener(CC_CALLBACK_1(LayerLogin::onButtonChoiNgay, this));
        
    }
	this->txtUserName = dynamic_cast<TextField*>(this->nodeDangNhap->getChildByName("tfTaiKhoan"));
    this->txtUserName->setMaxLength(30);
	this->txtPassword = dynamic_cast<TextField*>(this->nodeDangNhap->getChildByName("tfMatKhau"));


    this->txtPassword->setMaxLength(30);

	btnDangNhapVaoGame = static_cast<Button*>(this->nodeDangNhap->getChildByName("btnDangNhap"));
	if (btnDangNhapVaoGame){
		btnDangNhapVaoGame->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnDangNhapVaoGame, this));

	}

	btnDangKyVaoGame = static_cast<Button*>(this->nodeDangKy->getChildByName("btnDangKy"));
	if (btnDangKyVaoGame){
		btnDangKyVaoGame->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnDangKyVaoGame, this));

	}

	txtRegUserName = dynamic_cast<TextField*>(this->nodeDangKy->getChildByName("tfTaiKhoan"));
    this->txtRegUserName->setMaxLength(30);
    
	txtRegPassword = dynamic_cast<TextField*>(this->nodeDangKy->getChildByName("tfMatKhau"));
    this->txtRegPassword->setMaxLength(30);



	txtReRegUserName = dynamic_cast<TextField*>(this->nodeDangKy->getChildByName("tfNhapLaiMatKhau"));
    this->txtReRegUserName->setMaxLength(30);


	//cap nhat ten nhan vat
	this->tfTenNhanVat = static_cast<TextField*>(this->nodeCapNhatTen->getChildByName("tfTenNhanVat"));

	this->txtUserNameMe = static_cast<Text*>(this->nodeCapNhatTen->getChildByName("txtUserName"));
	this->txtError = static_cast<Text*>(this->nodeCapNhatTen->getChildByName("txtError"));
	this->bgInput = static_cast<ImageView*>(this->nodeCapNhatTen->getChildByName("bgInput"));


	this->tfTenTaiKhoan = static_cast<TextField*>(this->nodeCapNhatTaiKhoan->getChildByName("tfTenTaiKhoan"));

	this->txtErrorTaiKhoan = static_cast<Text*>(this->nodeCapNhatTaiKhoan->getChildByName("txtError"));

	btnCapNhatTaiKhoan = static_cast<Button*>(this->nodeCapNhatTaiKhoan->getChildByName("btnCapNhat"));
	if (btnCapNhatTaiKhoan){
		btnCapNhatTaiKhoan->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnCapNhatTaiKhoan, this));

	}
   
	btnCapNhat = static_cast<Button*>(this->nodeCapNhatTen->getChildByName("btnCapNhat"));
	if (btnCapNhat){
		btnCapNhat->addClickEventListener(CC_CALLBACK_1(LayerLogin::onBtnCapNhatTen, this));

	}
    
	//
	//    SceneManager::getSingleton().setIsShowTX(false);
	//    SceneManager::getSingleton().setIsShowPK(false);
	//    SceneManager::getSingleton().setIsShowTCC(false);
	//
    readInfo();
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLogin::showMaintain),this,1, false);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLogin::showUpdateInfo),this,1, false);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLogin::updateFB),this,1, false);

    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLogin::gotoLogin), this, 1, 1, 1, false);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLogin::scheduleUpdateEditbox), this, 0.5, 1, 0.5, false);
	startTimeOutChat();
	partnerID = "1";

	SceneManager::getSingleton().setIsShowTX(false);
	SceneManager::getSingleton().setIsShowTXDay(false);
    //anhnt only for test english
    //this->onChooseLang(this->btnENG);
    this->onChooseLang(this->btnCAM);
    if (SceneManager::getSingleton().getIsFlagStore()){
        this->onChooseLang(this->btnCAM);

    }
	if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
		Chat *toast = Chat::create(dataManager.GetSysString(459), -1);
		this->addChild(toast);
		return true;
	}
	if (!GameServer::getSingleton().getSmartFox()->IsConnected())
	{
		SceneManager::getSingleton().connect2Server(0);
	}


	SceneManager::getSingleton().setIsShowTXDay(false);
    
    UserDefault *def = UserDefault::getInstance();
    //anhnt only for test english
    SceneManager::getSingleton().currLang= def->getIntegerForKey("lang",2);
    SceneManager::getSingleton().currLang= 2;

    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    NativeHelper::reloadLang(SceneManager::getSingleton().currLang);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::reloadLang(SceneManager::getSingleton().currLang);
#endif
    
    if (SceneManager::getSingleton().currLang == 0){
        this->btnVN->setEnabled(false);
        this->btnVN->setBright(false);
        
        this->btnENG->setEnabled(true);
        this->btnENG->setBright(true);
        
        this->btnCAM->setEnabled(true);
        this->btnCAM->setBright(true);
    }
    if (SceneManager::getSingleton().currLang == 1){
        this->btnVN->setEnabled(true);
        this->btnVN->setBright(true);
        
        this->btnENG->setEnabled(false);
        this->btnENG->setBright(false);
        
        this->btnCAM->setEnabled(true);
        this->btnCAM->setBright(true);
    }
    if (SceneManager::getSingleton().currLang == 2){
        this->btnVN->setEnabled(true);
        this->btnVN->setBright(true);
        
        this->btnENG->setEnabled(true);
        this->btnENG->setBright(true);
        
        this->btnCAM->setEnabled(false);
        this->btnCAM->setBright(false);
    }
    
    if (SceneManager::getSingleton().currLang == 4){
        this->btnVN->setEnabled(true);
        this->btnVN->setBright(true);
        
        this->btnENG->setEnabled(true);
        this->btnENG->setBright(true);
        
        this->btnCAM->setEnabled(true);
        this->btnCAM->setBright(true);
        if (this->btnCN){
            this->btnCN->setEnabled(false);
            this->btnCN->setBright(false);
        }

    }
    this->reloadLang();
   
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLogin::scheduleUpdateContactInfo),this,1, false);


    this->btnVN->setVisible(false);
	//SceneManager::getSingleton().gotoLogin();
    //anhnt only eng
    this->onChooseLang(this->btnCAM);
    if (SceneManager::getSingleton().getIsFlagStore()){
        this->onChooseLang(this->btnCAM);

    }
    //anhnt china
    this->btnCAM->setVisible(false);
    this->onChooseLang(this->btnCAM);
	//this->txtChuChay->setScale(5);

	return true;
}
void LayerLogin::onSubcribe(Ref* pSender){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    NativeHelper::subcribeTopic("nagatest");
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::subcribeTopic("nagatest");
#endif
}
void LayerLogin::onButtonHotLineClicked(Ref* pSender){
  std::string strHotLine = dataManager.getAppConfig().callcenter + "";
  auto nums = mUtils::splitString(strHotLine, '.');
  strHotLine.erase(std::remove(strHotLine.begin(), strHotLine.end(), '.'), strHotLine.end());
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    NativeHelper::callPhone(strHotLine.c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::tryToCall(strHotLine);
#endif
}

void LayerLogin::scheduleUpdateContactInfo(float dt)
{
    if (dataManager.getAppConfig().callcenter.length() == 0)
        return;
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLogin::scheduleUpdateContactInfo),this);
    std::string strHotLine = dataManager.getAppConfig().callcenter + "";
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        lblHotLine->setString(strHotLine);
    }
    else{
        this->lblHotLine->setString(strHotLine);
    }
}
void LayerLogin::onChooseLang(Ref* pSender){
    auto btn = (Button*)pSender;
    int tag = btn->getTag();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    NativeHelper::reloadLang(tag);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::reloadLang(tag);
#endif
    if (tag == 0){
        this->btnVN->setEnabled(false);
        this->btnVN->setBright(false);

        this->btnENG->setEnabled(true);
        this->btnENG->setBright(true);
        
        this->btnCAM->setEnabled(true);
        this->btnCAM->setBright(true);
        
        if (this->btnCN){
            this->btnCN->setEnabled(true);
            this->btnCN->setBright(true);
        }
    }
    if (tag == 1){
//        SceneManager::getSingleton().showToast("No Support");
//        return;
        this->btnVN->setEnabled(true);
        this->btnVN->setBright(true);

        this->btnENG->setEnabled(false);
        this->btnENG->setBright(false);

        this->btnCAM->setEnabled(true);
        this->btnCAM->setBright(true);
        if (this->btnCN){
        this->btnCN->setEnabled(true);
        this->btnCN->setBright(true);
        }
    }
    if (tag == 2){
        this->btnVN->setEnabled(true);
        this->btnVN->setBright(true);
        
        this->btnENG->setEnabled(true);
        this->btnENG->setBright(true);
        
        this->btnCAM->setEnabled(false);
        this->btnCAM->setBright(false);
        
        if (this->btnCN){
            this->btnCN->setEnabled(true);
            this->btnCN->setBright(true);
        }
    }
    
    if (tag == 4){
        this->btnVN->setEnabled(true);
        this->btnVN->setBright(true);
        
        this->btnENG->setEnabled(true);
        this->btnENG->setBright(true);
        
        this->btnCAM->setEnabled(true);
        this->btnCAM->setBright(true);
        
        if (this->btnCN){
            this->btnCN->setEnabled(false);
            this->btnCN->setBright(false);
        }
    }
    
    UserDefault *def = UserDefault::getInstance();
    def->setIntegerForKey("lang", tag);
    SceneManager::getSingleton().currLang = tag;
    
    
    
    this->reloadLang();
}
void LayerLogin::reloadLang(){
    
    //anhnt sua download resource
//SceneManager::getSingleton().currLang = 2;
//    vector<string> searchPaths;
//    string langfolder = "/VN";
//    switch (SceneManager::getSingleton().currLang){
//        case 0:
//            langfolder = "/VN";
//            break;
//        case 1:
//            langfolder = "/ENG";
//            break;
//        case 2:
//            langfolder = "/CAM";
//            break;
//        case 4:
//            langfolder = "/CN";
//            break;
//    }
//    searchPaths.push_back("avatars"+langfolder);
//    searchPaths.push_back("cards");
//    searchPaths.push_back("ccbi");
//    searchPaths.push_back("design");
//    searchPaths.push_back("sounds"+langfolder);
//    searchPaths.push_back("sounds"+langfolder+"/game");
//    searchPaths.push_back("sounds"+langfolder+"/game_phom");
//    searchPaths.push_back("sounds"+langfolder+"/game_tienlen");
//    searchPaths.push_back("design/background");
//    searchPaths.push_back("design/guifiles");
//    searchPaths.push_back("design/frame"+langfolder);
//    searchPaths.push_back("design/LayerTaiXiuWeb");
//    searchPaths.push_back("design/backgrounds");
//    searchPaths.push_back("design/avatarGame");
//    searchPaths.push_back("design/fonts");
//    searchPaths.push_back("fonts");
//    searchPaths.push_back("ResChan"+langfolder);
//    searchPaths.push_back("ResXiTo"+langfolder);
//    searchPaths.push_back("backgrounds");
//    searchPaths.push_back("buttons"+langfolder);
//    searchPaths.push_back("cards");
//    searchPaths.push_back("icon_result");
//    searchPaths.push_back("numbers");
//    searchPaths.push_back("ResQuaythuong");
//    searchPaths.push_back("gameicons");
//    searchPaths.push_back("emotions");
//    searchPaths.push_back("ResXocDia");
//    searchPaths.push_back("design/LayerTaiXiuMini");
//    searchPaths.push_back("design/LayerMiniPoker");
//    searchPaths.push_back("design/LayerTomCuaCa");
//    searchPaths.push_back("design/LayerLieng"+langfolder);
//    searchPaths.push_back("design/LayerGameCaoThap");
//    searchPaths.push_back("design/LayerDoDenMini");
//    searchPaths.push_back("gamecaro");
//    searchPaths.push_back("MauBinh"+langfolder);
//    searchPaths.push_back("CoTuong");
//    searchPaths.push_back("CoTuong/res");
//    searchPaths.push_back("CoTuong/chess_ui");
//    searchPaths.push_back("design/LayerCoTuong/sanhgame");
//    searchPaths.push_back("design/LayerCoTuong");
//    searchPaths.push_back("localedata");
//    searchPaths.push_back("design/LayerMenuMiniGame");
//    searchPaths.push_back("res_extend"+langfolder);
//    searchPaths.push_back("design/ResBautom");
//    searchPaths.push_back("Music");
//
//    FileUtils::getInstance()->setSearchPaths(searchPaths);
//    
    SceneManager::getSingleton().loadString();
   // log("text: %s",dataManager.GetSysString(876).c_str());
    btnQuenMatKhau->setTitleText(dataManager.GetSysString(876)+ "?");
    btnQuenMatKhau1->setTitleText(dataManager.GetSysString(876)+ "?");
    
    btnDangkyGuide->setTitleText(dataManager.GetSysString(958));
    btnChoiNgayGuide->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"choi-ngay-guide.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"choi-ngay-guide.png");
    btnDangNhap->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"moc3-btn-dangnhap.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"moc3-btn-dangnhap.png");
    btnDangKy->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"moc3-btn-dangky.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"moc3-btn-dangky.png");
    btnChoiNgay->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"moc-btn-choingay.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"moc-btn-choingay.png");
    this->txtUserName->setPlaceHolder(dataManager.GetSysString(877));
    this->txtPassword->setPlaceHolder(dataManager.GetSysString(878));
    btnDangNhapVaoGame->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"moc3-btn-dangnhap-1.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"moc3-btn-dangnhap-1.png");
    btnDangKyVaoGame->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"moc3-btn-dangky.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"moc3-btn-dangky.png");
    this->tfTenTaiKhoan->setPlaceHolder(dataManager.GetSysString(877));
    this->tfTenNhanVat->setPlaceHolder(dataManager.GetSysString(822));
    this->txtReRegUserName->setPlaceHolder(dataManager.GetSysString(879));
    this->txtRegPassword->setPlaceHolder(dataManager.GetSysString(878));
    this->txtRegUserName->setPlaceHolder(dataManager.GetSysString(877));
    
    auto lblTipTen = static_cast<Text*> (this->nodeCapNhatTen->getChildByName("Text_21"));
    lblTipTen->setString(dataManager.GetSysString(788)+":");
    auto lblTipTe2 = static_cast<Text*> (this->nodeCapNhatTen->getChildByName("Text_21_1"));
    lblTipTe2->setString(dataManager.GetSysString(882));
    auto lblTitleTen = static_cast<Text*> (nodeCapNhatTen->getChildByName("Text_1"));
    lblTitleTen->setString(dataManager.GetSysString(880));
    auto lblTitleTK = static_cast<Text*> (nodeCapNhatTaiKhoan->getChildByName("Text_1"));
    lblTitleTK->setString(dataManager.GetSysString(880));
    auto lblTipTK = static_cast<Text*> (nodeCapNhatTaiKhoan->getChildByName("Text_21"));
    lblTipTK->setString(dataManager.GetSysString(881));
    auto lblTipTK2 = static_cast<Text*> (nodeCapNhatTaiKhoan->getChildByName("Text_21_1"));
    lblTipTK2->setString(dataManager.GetSysString(882));
    this->txtNhanGuide->setString(dataManager.GetSysString(988));

    btnCapNhatTaiKhoan->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"btn-capnhat.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"btn-capnhat.png");
    btnCapNhat->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"btn-capnhat.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"btn-capnhat.png");

    SceneMain* _parent = (SceneMain*)this->getParent();
    if (_parent != NULL){
        _parent->reloadLang();
    }
    if (txtDangKyNew){
        txtDangKyNew->setString(dataManager.GetSysString(958));
    }
    SceneManager::getSingleton().getNotificationFromUrl();
    SceneManager::getSingleton().getStatusEventFromUrl();
}
void LayerLogin::updateFB(float dt){
    //only for test dataManager.getAppConfig().isVN
    this->btnVN->setVisible(dataManager.getAppConfig().isVN);
    bool isShowFB = false;
    auto lstVersion = mUtils::splitString(dataManager.getAppConfig().hidefb, ';');
    vector<string> vsIOS;
    vector<string> vsAndroid;
    string is = "ios";
    string as = "and";
    for (int i = 0; i < lstVersion.size(); i++) {
        string vs = lstVersion[i];
        if (vs.find(is) != std::string::npos) {
            vsIOS.push_back(vs);
        }
        if (vs.find(as) != std::string::npos) {
            vsAndroid.push_back(vs);
        }
    }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for (int i = 0; i < vsIOS.size(); i++) {
        if (strcmp(vsIOS[i].c_str(), dataManager.getAppConfig().version_ios.c_str()) == 0){
            isShowFB = true;
        }
    }
    
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    for (int i = 0; i < vsAndroid.size(); i++) {
        if (strcmp(vsAndroid[i].c_str(), dataManager.getAppConfig().version_android.c_str()) == 0){
            isShowFB = true;
        }
    }
#endif
    
    if (isShowFB){
        btnDangNhapFB->setVisible(true);
        imgHotLine->setVisible(true);
        if (btnDangNhapFBNew){
            btnDangNhapFBNew->setVisible(true);
        }
        //labelFacebook->setVisible(true);
    }
    
    if (txtGoldBonusFB) {
        txtGoldBonusFB->setString(StringUtils::format("+%d", SceneManager::getSingleton().getFacebookBonus()));
        if (SceneManager::getSingleton().isNagaWin){
            txtGoldBonusFB->setVisible(false);
        }
    }
    if (txtGoldBonusFB2) {
        txtGoldBonusFB2->setString(StringUtils::format("+%d", SceneManager::getSingleton().getFacebookBonus()));
        if (SceneManager::getSingleton().isNagaWin){
            txtGoldBonusFB2->setVisible(false);
        }
    }
    if (this->txBonusFaecbookNew){
        txBonusFaecbookNew->setString(StringUtils::format("+%d", SceneManager::getSingleton().getFacebookBonus()));
        if (SceneManager::getSingleton().isNagaWin){
            txBonusFaecbookNew->setVisible(false);
        }
    }
    
}
void LayerLogin::onBtnCapNhatTaiKhoan(Ref* pSender){
	std::string _str = tfTenTaiKhoan->getString();
	if (_str.length() <= 0){
		txtErrorTaiKhoan->setVisible(true);
        txtErrorTaiKhoan->setString(dataManager.GetSysString(882));
	}
	else if (_str.length() < 6){
		txtErrorTaiKhoan->setString("* "+dataManager.GetSysString(883));
		txtErrorTaiKhoan->setVisible(true);
	}
	else if (mUtils::findSpecialCharNotSpace(string(tfTenTaiKhoan->getString())) == true){
		txtErrorTaiKhoan->setString(dataManager.GetSysString(186));
		txtErrorTaiKhoan->setVisible(true);
	}
	else{
		/*this->removeFromParentAndCleanup(true);*/

		int  osid = 2; //ios
		std::string devID = "";
		/*bool isConnectting = false;*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		osid = 1;
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
			devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
		else
			devID = NativeHelper::getDeviceId();
		/*isConnectting = NativeHelper::hasConnection();*/
		//devID = NativeHelper::getDeviceID();

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		osid = 2;
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
			devID = dataManager.getAppConfig().tientomocp2 + IOS::getDeviceID();
		else
			devID = IOS::getDeviceID();
#endif
		getPartnerInfo();
		string LT = UserDefault::getInstance()->getStringForKey("LT", "facebook");

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		string fbID = UserDefault::getInstance()->getStringForKey("facebook_id");
		string gID = UserDefault::getInstance()->getStringForKey("google_id");
		if (LT.compare("facebook") == 0)
			params->PutUtfString("fgid", fbID);// FG_ID = "fgid";
		else
			params->PutUtfString("fgid", gID);// FG_ID = "fgid";

		int utype = LT.compare("facebook") == 0 ? 1 : 2;
		params->PutInt("utype", utype);//UPDATE_TYPE = "utype"; 1 là facebook - 2 google
		params->PutUtfString("aI", tfTenTaiKhoan->getString());//

		params->PutInt("osid", osid);// Operatorsystems
		params->PutUtfString("devid", devID);// device id
		params->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
		params->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
		SceneManager::getSingleton().showLoading();
		boost::shared_ptr<IRequest> request(new ExtensionRequest("ruaarf", params));//REQUEST_UPDATE_ACC_AFTER_RF = "ruaarf";
		GameServer::getSingleton().Send(request);
	}
}
void LayerLogin::onButtonChoiNgay(Ref* pSender){




	showMaintain(0);
	showUpdateInfo(0);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	IOS::tryIsInternetConnection();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	NativeHelper::hasConnection();
#endif
	if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
		Chat *toast = Chat::create(dataManager.GetSysString(459), -1);
		this->addChild(toast);
		return;
	}
	currLoginType = quickType;
	if (!GameServer::getSingleton().getSmartFox()->IsConnected())
	{
		SceneManager::getSingleton().connect2Server(0);
	}
	else
	{
		doLogin(currLoginType);
	}

}
void LayerLogin::onButtonForgetPassword(Ref* pSender){
    Scene* _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
	auto _layer = LayerPopupForgetPassword::create();
	if (_layer != NULL){
		_currScene->addChild(_layer,1000000,9087);
	}
}

void LayerLogin::onBtnCapNhatTen(Ref* pSender){
	this->txtError->setVisible(false);
	string strS = string(this->tfTenNhanVat->getString());

	if (mUtils::trimString(strS).length() < 6){
		Chat *toast = Chat::create(dataManager.GetSysString(460), -1);
		this->addChild(toast);
		return;
	}
	if (mUtils::findSpecialCharNotSpace(strS) == true){
		Chat *toast = Chat::create(dataManager.GetSysString(461), -1);
		this->addChild(toast);
		return;
	}

	if (mUtils::isNumber(strS.c_str())){
		Chat *toast = Chat::create(dataManager.GetSysString(462), -1);
		this->addChild(toast);
		return;
	}
	if (strS == ""){
		Chat *toast = Chat::create(dataManager.GetSysString(463), -1);
		this->addChild(toast);
		return;
	}

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString(ACC_DISPLAY_NAME, strS);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_UPDATE_DISPLAY_NAME_REQ, params));
	GameServer::getSingleton().Send(request);

	SceneManager::getSingleton().showLoading();
}
void LayerLogin::timeoutChatMaxtime(float dt){
	SceneManager::getSingleton().timerchat++;
	log("timer chat couting!");
	if (SceneManager::getSingleton().timerchat == dataManager.getAppConfig().chattimeout*SceneManager::getSingleton().sec) {
		SceneManager::getSingleton().timerchat = 0;
		SceneManager::getSingleton().chatcounttime = 0;
		SceneManager::getSingleton().arrCurrentChat.clear();
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLogin::timeoutChatMaxtime),this);
	}
}
void LayerLogin::startTimeOutChat(){
	if (SceneManager::getSingleton().timerchat > 0){
		Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLogin::timeoutChatMaxtime),this,1, false);
	}
}
void LayerLogin::onBtnDangKyVaoGame(Ref* pSender){
	string strS = string(txtRegUserName->getString());

	if (mUtils::trimString(strS).length() < 6){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(464));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}
	if (mUtils::findSpecialCharNotSpace(string(txtRegUserName->getString())) == true){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(465));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}
	strS = string(txtRegPassword->getString());

	for (int i = 0; i < strS.size(); ++i)
	{
		if (strS.at(i) == ' '){
            LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
            popupThongBao->setMessage(dataManager.GetSysString(141));
            popupThongBao->setContentMess(dataManager.GetSysString(468));
            popupThongBao->showAButton();
            this->addChild(popupThongBao);
			return;
		}
	}

	if (mUtils::trimString(strS).length() < 6){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(466));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}

	if (strcmp(txtRegUserName->getString().c_str(), txtRegPassword->getString().c_str()) == 0){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(469));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}

	if (txtReRegUserName->getString() == ""){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(470));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}

	if (strcmp(txtRegPassword->getString().c_str(), txtReRegUserName->getString().c_str()) != 0){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(467));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}

	if (mUtils::isNumber(txtRegPassword->getString().c_str())){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(471));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}
	if (!mUtils::checkAlpha(txtRegPassword->getString().c_str())){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(471));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}

	if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(459));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}
	currLoginType = registType;
	if (!GameServer::getSingleton().getSmartFox()->IsConnected())
	{
		SceneManager::getSingleton().connect2Server(0);
		return;
	}
	//regist
	//Send request



	boost::shared_ptr<ISFSObject> params(new SFSObject());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	params->PutUtfString("lav", dataManager.getAppConfig().version_android);// LOGIN_APP_VERSION = "lav";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	params->PutUtfString("lav", dataManager.getAppConfig().version_ios);// LOGIN_APP_VERSION = "lav";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//parameters->PutUtfString("lav", "ios1452832982");// Only for test
	params->PutUtfString("lav", dataManager.getAppConfig().version_win);
#endif
	int  osid = 2; //ios
	std::string devID = "1111111111sssaa";
	/*bool isConnectting = false;*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	osid = 1;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
		devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
	else
		devID = NativeHelper::getDeviceId();
	/*isConnectting = NativeHelper::hasConnection();*/
	//devID = NativeHelper::getDeviceID();

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	devID = IOS::getDeviceID();
#endif
	getPartnerInfo();

	string _username = txtRegUserName->getString();
	boost::algorithm::to_lower(_username);
	params->PutUtfString("aI", _username);
	params->PutUtfString("aN", _username);
	boost::shared_ptr<string> _pass = PasswordUtil::MD5Password(txtRegPassword->getString());
	_pass = PasswordUtil::MD5Password(*_pass);
    if (SceneManager::getSingleton().isNagaCard){
        _pass = PasswordUtil::MD5Password(txtRegPassword->getString()+_username);
        _pass = PasswordUtil::MD5Password(*_pass);
    }
	params->PutUtfString("aP", *_pass);
	params->PutUtfString("aM", "");
	params->PutInt("aT", 1);
	params->PutInt("aS", 1);
	params->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
	params->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
	params->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
    params->PutUtfString("test", "5");

	params->PutInt("osid", osid);// Operatorsystems
	params->PutUtfString("devid", devID);// decvice id
    params->PutInt(LANG_CODE, SceneManager::getSingleton().currLang);

	boost::shared_ptr<IRequest> request(new ExtensionRequest("rr", params));
	GameServer::getSingleton().Send(request);
	SceneManager::getSingleton().showLoading();
}
void LayerLogin::getPartnerInfo()
{
	string sPartInfo = "";
	partnerID = "1";
	srcID = "0";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sPartInfo = NativeHelper::getPartnerID();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	sPartInfo = IOS::tryGetPartnerID();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	sPartInfo = "1-1";
#endif
	CCLOG("************************** %s", sPartInfo.c_str());
	//split partnerID and srcID
	//if (sPartInfo == "")
    sPartInfo = "1-0";
    if (SceneManager::getSingleton().isNagaCard){
        sPartInfo = "2-0";
    }
    if (SceneManager::getSingleton().isNagaWin){
        sPartInfo = "3-0";
    }
    if (SceneManager::getSingleton().isMegaWin){
        sPartInfo = "5-0";
    }
    if (SceneManager::getSingleton().isNagaNew){
        sPartInfo = "4-0";
    }
	vector<string> lstPartInfo = mUtils::splitString(sPartInfo, '-');
	if (lstPartInfo.size() < 2){
		return;
	}
	partnerID = lstPartInfo.at(0);
	srcID = lstPartInfo.at(1);
}
void LayerLogin::OnSmartFoxConnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSuccess = (*ptrEventParams)["success"];
	boost::shared_ptr<bool> ptrValue = (boost::static_pointer_cast<bool>)(ptrEventParamValueSuccess);

	if (*ptrValue != true)//not success
	{
		Chat *toast = Chat::create(dataManager.GetSysString(472), -1);
		this->addChild(toast);
		log("LayerLogin: OnSmartFoxConnection -- !GameServer::getSingleton().getSmartFox()->IsConnected()");
		return;
	}
	log("LayerLogin: OnSmartFoxConnection");
	//    if (!isShouldAuto)
	//        return;
	if (currLoginType == registType){
		doLogin(currLoginType);
		return;
	}
	if (currLoginType == facebookType || currLoginType == gmailType || currLoginType == quickType){
		doLogin(currLoginType);
		return;
	}

	if (txtUserName->getString().length() < 3 || txtPassword->getString().length() < 3)
		return;
	if (currLoginType == loginType)
		doLogin(currLoginType);

}

void LayerLogin::OnSmartFoxLogin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    //anhnt only for test
    //return;
    SceneManager::getSingleton().isShouldAuto = true;
    SceneManager::getSingleton().isShowEvent = false;
    SceneManager::getSingleton().setIsChuyenTab(false);
    SceneManager::getSingleton().hideLoading();

    UserDefault *def = UserDefault::getInstance();

    string lastgameid = def->getStringForKey("lastgameid", "");
    auto lstgameid = mUtils::splitString(lastgameid, '_');
    int gid = 0;
    string name = "";
    if (lstgameid.size()>=2){
        gid = atoi(lstgameid[0].c_str());
        name = lstgameid[1];
    }
    if (gid == 128 && strcmp(name.c_str(), SceneManager::getSingleton().getMyName().c_str())== 0){
        def->setBoolForKey("showinstantTX", true);
    }else{
        def->setBoolForKey("showinstantTX", false);
    }
    def->setStringForKey("lastgameid", "");


    int a = rand() % 8 + 1;
    int avatar = 0;
    avatar = def->getIntegerForKey("avatar", a);
    def->setIntegerForKey("avatar", avatar);
    
    
    
    
    this->pnlBgButton->setVisible(false);
    isLogin = true;
	SceneManager::getSingleton().hideLoading();
	if (currLoginType == registType)
		return;
	log("LayerLogin::OnSmartFoxLogin() - Login OK");
    dataManager.getAppConfig().isShowMatKhau =  false;
    dataManager.getAppConfig().isShowMatKhauDone = false;

    boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
    boost::shared_ptr<int> _a(new int(SceneManager::getSingleton().currLang));
    boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable(LANG_CODE, _a, VARIABLETYPE_INT));
    collectionUserVariable->push_back(variable);    //
    boost::shared_ptr<IRequest> request23(new SetUserVariablesRequest(collectionUserVariable));
    GameServer::getSingleton().Send(request23);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt(LANG_CODE, SceneManager::getSingleton().currLang);
    boost::shared_ptr<IRequest> request1(new ExtensionRequest(EXTENSION_REQUEST_LIST_EVENT_REQUEST, params));//sent
    GameServer::getSingleton().Send(request1);
    
    
	//save info
	saveInfo();

	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueDatas = (*ptrEventParams)["data"];
	boost::shared_ptr<ISFSObject> datas = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueDatas)));
	if (datas == NULL)
		return;

    
	SceneManager::getSingleton().isLogin = true;
	boost::shared_ptr<long> mt = datas->GetInt("mt");
	if (mt != NULL){
		log("mt = %ld", *mt);
		if (1 == *mt){

			return;

			Scene* _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;

			LayerPopupThongBao* _layer = LayerPopupThongBao::create();
			_layer->setMessage(dataManager.GetSysString(141));
			_layer->showAButton();
			_layer->setCallBackFromButton(this, callfuncO_selector(LayerLogin::onButtonDongy), 3);
			_layer->setContentMess(dataManager.GetSysString(612));
			_currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_NOTIFICATION, 1942);
			return;
		}
	}
	else{
		log("null");
	}

    
	SceneManager::getSingleton().getNotificationFromUrl();
	SceneManager::getSingleton().getStatusEventFromUrl();

	if (datas == NULL){
	}
	//Check các cờ ẩn hiện
	dataManager.getAppConfig().isHideBilling = false;
	dataManager.getAppConfig().isHideExchange = false;
	dataManager.getAppConfig().isHideTransfer = false;
	if (datas->GetUtfString("ihb")){
		string _ihb = *datas->GetUtfString("ihb");
		//_ihb = "billing:0-exchange:1-transfer:0";
		//-billing:0-exchange:0-transfer:1-sms:0-card:0
		if (_ihb != ""){
			//split
			vector<string> lstVisibleInfos = mUtils::splitString(_ihb, '-');
			for (int i = 0; i < lstVisibleInfos.size(); i++){
				vector<string> lstChildInfos = mUtils::splitString(lstVisibleInfos.at(i), ':');
				if (lstChildInfos.size() != 2)
					continue;
				if (lstChildInfos.at(0).compare("billing") == 0){
					dataManager.getAppConfig().isHideBilling = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("delay_reqot") == 0){
					dataManager.getAppConfig().delay_reqot = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("delay_rqoffer") == 0){
					dataManager.getAppConfig().delay_rqoffer = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("delay_reqdt") == 0){
					dataManager.getAppConfig().delay_reqdt = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("exchange") == 0){
					dataManager.getAppConfig().isHideExchange = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("transfer") == 0){
					dataManager.getAppConfig().isHideTransfer = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("game") == 0){
					dataManager.getAppConfig().sListHiddenGame = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("vip") == 0){
					dataManager.getAppConfig().sListVip = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("quaythuong") == 0){
					dataManager.getAppConfig().sListQuayThuong = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("sms") == 0){
					dataManager.getAppConfig().isHideSMS = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("card") == 0){
					dataManager.getAppConfig().isHideTheCao = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("minjoin") == 0)
				{
					dataManager.getAppConfig().minjoin = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("transvip") == 0){
					dataManager.getAppConfig().strMinTranfer = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("invitetime") == 0){
					dataManager.getAppConfig().invitetime = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("typetable") == 0)
				{
					SceneManager::getSingleton().setTypePickRoom(atoi(lstChildInfos.at(1).c_str()));
				}
				else if (lstChildInfos.at(0).compare("chatlevel") == 0)
				{
					dataManager.getAppConfig().chatlevel = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("chattimeout") == 0)
				{
					dataManager.getAppConfig().chattimeout = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("chattimemax") == 0)
				{
					dataManager.getAppConfig().chattimemax = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("bonusimg") == 0){
//                    dataManager.getAppConfig().urlBonusIMG = "http://" + lstChildInfos.at(1);
//                    SceneManager::getSingleton().getImageDownloadProcess()->downLoadImage(NULL, dataManager.getAppConfig().urlBonusIMG, 1);
				}
				else if (lstChildInfos.at(0).compare("mail_sup") == 0)
				{
					dataManager.getAppConfig().mailSup = lstChildInfos.at(1);
				}
				//config cho quy thuong tom cua ca
				else if (lstChildInfos.at(0).compare("notifiQT") == 0)
				{
					dataManager.getAppConfig().notifiQT = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("timeToQT") == 0)
				{
					dataManager.getAppConfig().timeToQT = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("timetoNextWeek") == 0)
				{
					dataManager.getAppConfig().timetoNextWeek = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("firstBetTCC") == 0)
				{
					dataManager.getAppConfig().firstBetTCC = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("isShowLucky") == 0)
				{
					dataManager.getAppConfig().isShowLucky = lstChildInfos.at(1).compare("0") == 0;
				}
				else if (lstChildInfos.at(0).compare("sdtnaptien") == 0)
				{
					dataManager.getAppConfig().isHideXacThucSDT = lstChildInfos.at(1).compare("1") == 0;;
				}
				else if (lstChildInfos.at(0).compare("fbbonus") == 0)
				{
					dataManager.getAppConfig().isfb = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("tccLimitBet") == 0)
				{
					dataManager.getAppConfig().tccLimitCount = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("maxxito") == 0)
				{
					dataManager.getAppConfig().maxxito = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("maxlieng") == 0)
				{
					dataManager.getAppConfig().maxlieng = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("pheketsat") == 0)
				{
					dataManager.getAppConfig().pheketsat = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("minungtien") == 0)
				{
					string min = lstChildInfos.at(1);
					vector<string> lst = mUtils::splitString(min, '_');
					dataManager.getAppConfig().minungtien.clear();
					for (int i = 0; i < lst.size(); i++){
						dataManager.getAppConfig().minungtien.push_back(atoi(lst[i].c_str()));
					}
				}
				else if (lstChildInfos.at(0).compare("gamemini") == 0)
				{
					dataManager.getAppConfig().gamemini = lstChildInfos.at(1);;
				}

				else if (lstChildInfos.at(0).compare("otp") == 0)
				{
					dataManager.getAppConfig().otp = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("ketsat") == 0)
				{
					dataManager.getAppConfig().ketsat = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("ungtien") == 0)
				{
					dataManager.getAppConfig().ungtien = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("ruongvip") == 0)
				{
					dataManager.getAppConfig().ruongvip = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("phichuyentien") == 0)
				{
					dataManager.getAppConfig().phichuyentien = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("minVipDoiMoc") == 0)
				{
					dataManager.getAppConfig().minVipDoiMoc = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("minVipBanMoc") == 0)
				{
					dataManager.getAppConfig().minVipBanMoc = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("isNhapPass") == 0)
				{
					dataManager.getAppConfig().isNhapPass = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("mintaoban") == 0)
				{
					dataManager.getAppConfig().mintaoban = atoi(lstChildInfos.at(1).c_str());
				}
                else if (lstChildInfos.at(0).compare("isSound") == 0)
                {
                    dataManager.getAppConfig().isSound = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("isShareCancel") == 0)
                {
                    dataManager.getAppConfig().isShareCancel = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("notifyZen") == 0)
                {
                    dataManager.getAppConfig().notifyZen = atoi(lstChildInfos.at(1).c_str());
                }
                else if (lstChildInfos.at(0).compare("notifyGold") == 0)
                {
                    dataManager.getAppConfig().notifyGold = atoi(lstChildInfos.at(1).c_str());
                }
                else if (lstChildInfos.at(0).compare("timeShowTX") == 0)
                {
                    dataManager.getAppConfig().timeShowTX = atoi(lstChildInfos.at(1).c_str());
                }
                else if (lstChildInfos.at(0).compare("isBetWhenOpenDish") == 0)
                {
                    dataManager.getAppConfig().isBetWhenOpenDish = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("isPopupHuongDan") == 0)
                {
                    dataManager.getAppConfig().isPopupHuongDan = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("numHuongDanTX") == 0)
                {
                    dataManager.getAppConfig().numHuongDanTX = atoi(lstChildInfos.at(1).c_str());
                }
                else if (lstChildInfos.at(0).compare("cellcard") == 0)
                {
                    dataManager.getAppConfig().cellcard = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("isrecommend") == 0)
                {
                    dataManager.getAppConfig().isrecommend = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("isChip") == 0)
                {
                    dataManager.getAppConfig().isChip = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("isDoiThe") == 0)
                {
                    dataManager.getAppConfig().isDoiThe = lstChildInfos.at(1).compare("1") == 0;
                }
                
			}
		}
	}
    //UserDefault *def = UserDefault::getInstance();
    def->setBoolForKey("sound", dataManager.getAppConfig().isSound);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    if (datas->GetUtfString("billingcheckstatus")){
//        string billingcheckstatus = *datas->GetUtfString("billingcheckstatus");
//        auto lstData = mUtils::splitString(billingcheckstatus, ',');
//        if (lstData.size()>=2) {
//            if (!dataManager.getAppConfig().isHideBilling)
//                dataManager.getAppConfig().isHideBilling = lstData.at(0).compare("1") == 0;
//            dataManager.getAppConfig().is24h = lstData.at(1).compare("1") == 0;
//        }
//    }
#endif
	if (datas->GetUtfString("data")){
		string data = *datas->GetUtfString("data");
		auto lstData = mUtils::splitString(data, '|');
		if (atoi(lstData[0].c_str()) == 0)
			dataManager.getAppConfig().isLoginFisrt = true;
		else
			dataManager.getAppConfig().isLoginFisrt = false;
		if (atoi(lstData[1].c_str()) == 0){
			dataManager.getAppConfig().isLoginFisrtDay = true;
			UserDefault::getInstance()->setBoolForKey("isShowEventDay", false);
			UserDefault::getInstance()->flush();
           //SceneManager::getSingleton().isShowEvent = false;
		}
        else{
			dataManager.getAppConfig().isLoginFisrtDay = false;
            //SceneManager::getSingleton().isShowEvent = true;
        }
		if (atoi(lstData[2].c_str()) == 0)
			dataManager.getAppConfig().isreceivedRegisterFace = false;
		else
			dataManager.getAppConfig().isreceivedRegisterFace = true;
		if (atoi(lstData[3].c_str()) == 0)
			dataManager.getAppConfig().isreceivedLinkFace = false;
		else
			dataManager.getAppConfig().isreceivedLinkFace = true;
	}
    boost::shared_ptr<string> exchangerate = datas->GetUtfString("exchangerate");
    //dataManager.getAppConfig().exchangerate = "5_61240;10_124124;15_124124;20_1241050;50_245235/5_61440;10_121124;15_12125;20_125125125;25_12512512;30_12412412";
    if (exchangerate){
        dataManager.getAppConfig().exchangerate = *exchangerate;
    }
    boost::shared_ptr<string> bankaccount = datas->GetUtfString("bankaccount");
    //dataManager.getAppConfig().bankaccount = "1:testaccountwing|2:testaccountdcb";
    if (bankaccount){
        dataManager.getAppConfig().bankaccount = *bankaccount;
    }
    
	if (datas->GetUtfString("LT")){
		string type = *datas->GetUtfString("LT");
		dataManager.getAppConfig().loginType = type;
	}
	if (datas->GetInt("hasotp")){
		int type = *datas->GetInt("hasotp");
		if (type == 0){
			dataManager.getAppConfig().isactiveOTP = false;
		}
		else if (type == 1){
			dataManager.getAppConfig().isactiveOTP = true;
		}
	}

	if (datas->GetInt("nord")){
		int num = *datas->GetInt("nord");
		dataManager.getAppConfig().NUM_REGISTER_DEVICE = num;
	}
	//Get list pin ( game )
	boost::shared_ptr<string> lipg = datas->GetUtfString("lipg");//LIST_PIN_GAMES = lipg
	SceneManager::getSingleton().setListPin("");
	if (lipg != NULL){
		SceneManager::getSingleton().setListPin(*lipg);
	}
	//Get Url promotion
	boost::shared_ptr<string> km = datas->GetUtfString("km");//PROMOTION_URL = "km";
	SceneManager::getSingleton().setUrlPromotion("");
	if (km != NULL){
		SceneManager::getSingleton().setUrlPromotion(*km);
		SceneManager::getSingleton().getPromotionFromUrl();
	}
	//	LayerMain::getSingleton().reloadChargeConfig();
	///get gameCONFIG - MINHDV

	boost::shared_ptr<string> gameconfig = datas->GetUtfString("gmcfg");
	if (gameconfig)
	{
		SceneManager::getSingleton().setGameConfig(gameconfig->c_str());
	}
	//get ip and set to user variable
	boost::shared_ptr<string> myIP = datas->GetUtfString("aIp");
	if (myIP)
	{
		boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
		boost::shared_ptr<string> _a(new string(myIP->c_str()));
		//boost::shared_ptr<string> _a(new string("192.168.1.1"));
		boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("aIp", _a, VARIABLETYPE_STRING));
		collectionUserVariable->push_back(variable);	//
		boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
		GameServer::getSingleton().Send(request);
	}
	//set user chicken variable
	bool uck = def->getBoolForKey("UCK", true);
	setUckVariable(uck);
    //anhnt only for test
    //return;
	boost::shared_ptr<string> ast = datas->GetUtfString("aSt");
	if (ast == NULL){
		boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
		if (myself != NULL){

			if (myself->GetVariable("aN")){
				string _strName = *myself->GetVariable("aN")->GetStringValue();
				UserDefault *def = UserDefault::getInstance();
				string _lt = def->getStringForKey("LT", "");
				if (_lt.compare("normal") != 0 && _strName.compare("") == 0){
					//Neu ko phhai la login binh thuong, se kiem tra aN, neu aN = "" thi bat buoc phai update
					this->nodeCapNhatTaiKhoan->setVisible(true);
					this->nodeButton->setVisible(false);
					this->nodeDangNhap->setVisible(false);
					this->nodeDangKy->setVisible(false);
					this->nodeCapNhatTen->setVisible(false);

                    pnlBG->setOpacity(210);
					return;
				}
			}

			if (myself->GetVariable("adn") != NULL)
			{
			

                //bool isLink = UserDefault::getInstance()->getBoolForKey("isReceivedFacebook");
                //if (!isLink){//|| std::strcmp(status.c_str(),"0")==0) {
                if (strcmp("facebook", dataManager.getAppConfig().loginType.c_str()) == 0){
                    
                    boost::shared_ptr<ISFSObject> params(new SFSObject());
                    boost::shared_ptr<IRequest> request(new ExtensionRequest("rfb", params));
                    GameServer::getSingleton().Send(request);
                    
                }
                //}

				string _strName = *myself->GetVariable("adn")->GetStringValue();
				//26/01/2016 HoangDD changes
				if (_strName.compare("") == 0){
					//Neu ko phhai la login binh thuong, se kiem tra aN, neu aN = "" thi bat buoc phai update
					if (myself->GetVariable("aN") != NULL)
						this->txtUserNameMe->setString(*myself->GetVariable("aN")->GetStringValue());
					this->nodeButton->setVisible(false);
					this->nodeDangNhap->setVisible(false);
					this->nodeDangKy->setVisible(false);
					this->nodeCapNhatTen->setVisible(true);

                    pnlBG->setOpacity(210);
					if (myself->GetVariable("aN") != NULL)
						this->txtUserNameMe->setString(*myself->GetVariable("aN")->GetStringValue());
					return;
				}

			}
		}
		//SceneManager::getSingleton().gotoMain();
        SceneMain* parent = (SceneMain*)this->getParent();
        if (parent){
            this->setVisible(false);
            parent->logInDone();
            this->removeFromParent();
        }
        return;
	}
	//ast = bokinhvan113,GameZone1,P0_112_0,2,06/09/2017 13:09:31,112,0,1234
	log("ast = %s", ast->c_str());
	lstDatas = mUtils::splitString(*ast, ',');

    
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself != NULL){

		if (myself->GetVariable("adn") != NULL)
		{

			string _strName = *myself->GetVariable("adn")->GetStringValue();
			//26/01/2016 HoangDD changes
			if (_strName.compare("") == 0){
				//Neu ko phhai la login binh thuong, se kiem tra aN, neu aN = "" thi bat buoc phai update
				this->nodeButton->setVisible(false);
				this->nodeDangNhap->setVisible(false);
				this->nodeDangKy->setVisible(false);
				this->nodeCapNhatTen->setVisible(true);

				return;
			}

		}
	}


	if (lstDatas.size() != 0)
	{
		int mRoomID;
		int mGameID;
		//mRoomID = atoi(lstDatas.at(2).c_str());
		mGameID = atoi(lstDatas.at(5).substr(0, 3).c_str());
		int roomType = atoi(lstDatas.at(6).c_str());
		string passWord = "";
		if (lstDatas.size() == 8)
			passWord = lstDatas.at(7);
		if (mGameID != kGameTaiXiu && mGameID != kGameQuayThuong
			&& mGameID != kGameXocDia && mGameID != atoi(SERVICE_CHAT_ROOM.c_str())
            && mGameID != kGameBauTom && !SceneManager::getSingleton().getIsFlagStore())
		{
            this->setVisible(false);
            SceneMain* parent = (SceneMain*)this->getParent();
            if (parent){
                this->setVisible(false);
                parent->logInDone();
            }
			RejoinInfoStruct joinGameInfo;
			joinGameInfo.gameID = lstDatas.at(5);
			joinGameInfo.roomID = -1;
			joinGameInfo.roomName = lstDatas.at(2).c_str();
			joinGameInfo.pass = passWord;
			joinGameInfo.isNeedRejoin = true;
			SceneManager::getSingleton().setRejoinInfo(joinGameInfo);
			SceneManager::getSingleton().setCurrRoomType(roomType);
			SceneManager::getSingleton().setGameID(atoi(joinGameInfo.gameID.substr(0, 3).c_str()));
			SceneManager::getSingleton().gotoPickGame(atoi(joinGameInfo.gameID.substr(0, 3).c_str()));
            //this->removeFromParent();
//            SceneMain* parent = (SceneMain*)this->getParent();
//            if (parent){
//                parent->logInDone();
//            }
		}
        else if (mGameID == kGameBauTom || mGameID == kGameXocDia || mGameID == kGameTaiXiu|| mGameID == kGameSlot )
        {
            SceneMain* parent = (SceneMain*)this->getParent();
            if (parent){
                this->setVisible(false);
                parent->logInDone();
            }
            SceneManager::getSingleton().hideLoading();
            SceneManager::getSingleton().setGameID(mGameID);
            SceneManager::getSingleton().setCurrRoomType(roomType);
            SceneManager::getSingleton().gotoPickGame(mGameID);
           // this->removeFromParent();

//            SceneMain* parent = (SceneMain*)this->getParent();
//            if (parent){
//                parent->logInDone();
//            }
            
        }
		else
		{
			//SceneManager::getSingleton().gotoMain();
            SceneMain* parent = (SceneMain*)this->getParent();
            if (parent){
                this->setVisible(false);
                parent->logInDone();
                this->removeFromParent();
            }
		}
		// Kiểm tra xem room có tồn tại hay không, nếu tồn tại có nghĩa là
		// Mất kết nối lúc chơi game, bây giờ join lại. //ROOM_ID = "ri";
		// 		boost::shared_ptr<MatchExpression> expr(new MatchExpression("ri", NumberMatch::EQUALS, boost::make_shared<int>(mGameID)
		//             , MatchExpression::MatchExpressionValueType::MatchExpression_ValueOther));
		// 		// Send request
		// 		boost::shared_ptr<IRequest> request(new FindRoomsRequest(expr));
		// 		GameServer::getSingleton().Send(request);
		// 		if( GameServer::getSingleton().getSmartFox()->GetRoomById(mRoomID) == NULL)
		// 			return;
		//
		// 		log("Ban dang choi game %s phong so %d", mUtils::getGameNameByID(mGameID), mRoomID);
		//		LayerMain::getSingleton().autoJoinGameWithID(mGameID, mRoomID);
	}
	else
	{
        SceneMain* parent = (SceneMain*)this->getParent();
        if (parent){
            this->setVisible(false);
            parent->logInDone();
            this->removeFromParent();
        }
	}
}
void LayerLogin::OnSmartFoxLogout(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	SceneManager::getSingleton().hideLoading();
    isLogin = false;
	gotoLogin(0);
}
void LayerLogin::onButtonDongy(Ref* pSender){


	auto node = (Node*)pSender;
	int tag = node->getTag();

	if (tag == 3) {
		//Director::getInstance()->end();
		boost::shared_ptr<IRequest> request(new LogoutRequest());
		GameServer::getSingleton().Send(request);
		//
		SceneManager::getSingleton().isLogout = true;
		SceneManager::getSingleton().rcForLogin = false;

		Scene* _currScene = Director::getInstance()->getRunningScene();
		if (!_currScene) return;
		_currScene->removeChildByTag(1942);
	}
	else if (tag == 4) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		NativeHelper::openStore(dataManager.getAppConfig().url_android_store.c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (dataManager.getAppConfig().url_ios_nostore != ""){
            IOS::openUrl(dataManager.getAppConfig().url_ios_nostore);
        }else{
		IOS::gotoStore(dataManager.getAppConfig().url_ios_store);
        }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
		return;
#endif

		Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}
	else{
		//Director::getInstance()->end();
		//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
		return;
#endif

		Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}

}

void LayerLogin::OnSmartFoxLoginError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	SceneManager::getSingleton().hideLoading();
	if (currLoginType == registType)
		return;
	log("LayerLogin::OnSmartFoxLoginError() - Login error");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueErrorMessage = (*ptrEventParams)["errorMessage"];
	boost::shared_ptr<string> ptrErrorMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueErrorMessage);
	boost::shared_ptr<string> message(new string("OnSmartFoxLoginError Failure: " + *ptrErrorMessage + " !"));
	log("Error string: %s", message->c_str());
	//
	boost::shared_ptr<void> ptrEventParamValueErrorCode = (*ptrEventParams)["errorCode"];
	boost::shared_ptr<short> ptrErrorCode = ((boost::static_pointer_cast<short>))(ptrEventParamValueErrorCode);
	//
	log("OnSmartFoxRoomJoinError - errorCode: %d", *ptrErrorCode);

	//Chat *toast = Chat::create(mUtils::getErrorSmartfoxExplain(*ptrErrorCode), -1);
    LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
    popupThongBao->setMessage(dataManager.GetSysString(141));
    popupThongBao->setContentMess(mUtils::getErrorSmartfoxExplain(*ptrErrorCode));
    popupThongBao->showAButton();
    this->addChild(popupThongBao);
}
void LayerLogin::showMaintain(float dt){
    //only for test, nho dong
    //return;
	if (strcmp(dataManager.getAppConfig().maintenancestr.c_str(), "") != 0){
		isShouldAuto = false;
        SceneManager::getSingleton().isShouldAuto = false;

		if (isShow){
			//Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLogin::showUpdateInfo));

			//            Scene* _currScene = Director::getInstance()->getRunningScene();
			//            if (!_currScene) return;
            this->setVisible(true);

			LayerPopupThongBao* _layer = LayerPopupThongBao::create();
			_layer->setMessage(dataManager.GetSysString(141));
			_layer->showAButton();
			_layer->setCallBackFromButton(this, callfuncO_selector(LayerLogin::onButtonDongy), 1);
			string mess = dataManager.getAppConfig().maintenancestr + dataManager.GetSysString(597);
			_layer->setContentMess(mess);
			this->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_MAINTAIN, 1942);
			isShow = false;

		}
		return;
	}

}
void LayerLogin::showUpdateInfo(float dt){
	//update for dev win32
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	{
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLogin::showUpdateInfo),this);
		return;
	}


	auto lstVersion = mUtils::splitString(dataManager.getAppConfig().versionforceupdate, ';');
	vector<string> vsIOS;
	vector<string> vsAndroid;
	string is = "ios";
	string as = "and";
	for (int i = 0; i < lstVersion.size(); i++) {
		string vs = lstVersion[i];
		if (vs.find(is) != std::string::npos) {
			vsIOS.push_back(vs);
		}
		if (vs.find(as) != std::string::npos) {
			vsAndroid.push_back(vs);
		}
	}
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	for (int i = 0; i < vsIOS.size(); i++) {
		if (strcmp(vsIOS[i].c_str(), dataManager.getAppConfig().version_ios.c_str()) == 0){
			dataManager.getAppConfig().force_update = "-1";
		}
	}

#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	for (int i = 0; i < vsAndroid.size(); i++) {
		if (strcmp(vsAndroid[i].c_str(), dataManager.getAppConfig().version_android.c_str()) == 0){
			dataManager.getAppConfig().force_update = "-1";
		}
	}
#endif
	if (strcmp(dataManager.getAppConfig().force_update.c_str(), "1") == 0){
		isShouldAuto = false;
        SceneManager::getSingleton().isShouldAuto = false;
		if (isShow){
			Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLogin::showUpdateInfo),this);

			//            Scene* _currScene = Director::getInstance()->getRunningScene();
			//            if (!_currScene) return;

            this->setVisible(true);
			LayerPopupThongBao* _layer = LayerPopupThongBao::create();
			_layer->setMessage(dataManager.GetSysString(141));
			_layer->showAButton();
			_layer->setCallBackFromButton(this, callfuncO_selector(LayerLogin::onButtonDongy), 4);
			string mess = dataManager.GetSysString(598);
			_layer->setContentMess(mess);
			this->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_MAINTAIN, 1943);
			isShow = false;

		}
		return;
	}
	else  if (strcmp(dataManager.getAppConfig().force_update.c_str(), "0") == 0){
		isShouldAuto = false;
        SceneManager::getSingleton().isShouldAuto = false;

		if (isShow){
			Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLogin::showUpdateInfo),this);
            this->setVisible(true);

			Scene* _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;

			LayerPopupThongBao* _layer = LayerPopupThongBao::create();
			_layer->setMessage(dataManager.GetSysString(141));
			// _layer->showAButton();
			_layer->setCallBackFromButton(this, callfuncO_selector(LayerLogin::onButtonDongy), 4);
			string mess = dataManager.GetSysString(599);
			_layer->setContentMess(mess);
			this->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_MAINTAIN, 1943);
			isShow = false;

		}
		return;
	}
}
void LayerLogin::timeoutLoadingCallback()
{
	int tag = SceneManager::getSingleton().getLoadingTag();
	if (tag == -1)
		return;

	switch (tag){
	case 123://login
		if (GameServer::getSingleton().getSmartFox()->IsConnected())
			break;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
		//		if (sdkbox::PluginFacebook::isLoggedIn())
		//		{
		//			sdkbox::PluginFacebook::logout();
		//			//_loginItem->setString("Login");
		//			log("logout");
		//		}
#endif
		Chat *toast = Chat::create(dataManager.GetSysString(473), -1);
		this->addChild(toast);
		break;
	}

	SceneManager::getSingleton().setLoadingTag(-1);
}


void LayerLogin::doLogin(const int& type /*= 0*/)
{
    bool isConnect = true;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    isConnect = IOS::tryIsInternetConnection();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    isConnect = NativeHelper::hasConnection();
    
   
#endif
    if (!isConnect){
        SceneManager::getSingleton().showPopupFailed(3);
        return;
    }
    
	showMaintain(0);
	showUpdateInfo(0);
	SceneManager::getSingleton().setCallBackLoading(this, callfunc_selector(LayerLogin::timeoutLoadingCallback));
	SceneManager::getSingleton().showLoading();
	boost::shared_ptr<ISFSObject> parameters(new SFSObject());
	int  osid = 2; //ios
	std::string devID = "";
	/*bool isConnectting = true;*/
	getPartnerInfo();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	osid = 1;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
		devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
	else
		devID = NativeHelper::getDeviceId();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	osid = 2;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
		devID = dataManager.getAppConfig().tientomocp2 + IOS::getDeviceID();
	else
		devID = IOS::getDeviceID();
    


#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	devID = "123ddzzz-aa";// Only for test
	//update 29/11/2016 for check device id
	//devID = getDeviceID();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	parameters->PutUtfString("lav", dataManager.getAppConfig().version_android);// LOGIN_APP_VERSION = "lav";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	parameters->PutUtfString("lav", dataManager.getAppConfig().version_ios);// LOGIN_APP_VERSION = "lav";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//parameters->PutUtfString("lav", "ios1452832982");// Only for test
	parameters->PutUtfString("lav", dataManager.getAppConfig().version_win);
#endif
    if (SceneManager::getSingleton().isMegaWin){
        devID += "_5";
    }
    if (SceneManager::getSingleton().isNagaNew){
        devID += "_4";
    }
	if (type == loginType)
	{
		if (txtUserName == NULL || txtPassword == NULL)
			return;
		UserDefault::getInstance()->setIntegerForKey("logintype", 1);
		UserDefault::getInstance()->setStringForKey("LT", "normal");
		UserDefault::getInstance()->setStringForKey("google_id", "");
		UserDefault::getInstance()->setStringForKey("facebook_id", "");
		UserDefault::getInstance()->flush();

		parameters->PutUtfString("LT", "normal");
		parameters->PutUtfString("LSK", "");
		parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
		parameters->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
		parameters->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
		parameters->PutInt("osid", osid);// Operatorsystems
		parameters->PutUtfString("devid", devID);// decvice id
		parameters->PutUtfString("aIp", "");//ip
		parameters->PutInt("srcref", 0);
        parameters->PutInt(LANG_CODE, SceneManager::getSingleton().currLang);
        
		boost::shared_ptr<string> _pass = PasswordUtil::MD5Password(txtPassword->getString());
		_pass = PasswordUtil::MD5Password(*_pass);
     
		string _username = txtUserName->getString();
		boost::algorithm::to_lower(_username);
        if (SceneManager::getSingleton().isNagaCard){
            _pass = PasswordUtil::MD5Password(txtPassword->getString()+_username);
            _pass = PasswordUtil::MD5Password(*_pass);
        }
		boost::shared_ptr<IRequest> request(new LoginRequest(_username, *_pass, "GameZone1", parameters));
		GameServer::getSingleton().SendLogin(request);
	}
	else if (type == facebookType)
	{
		log("Login With Type = 1");

		if (strcmp(socialInfo.uid.c_str(), "") == 0)
			return;
		/*socialInfo.uid = scene.getUID();
		 socialInfo.name = scene.getName();
		 socialInfo.email = scene.getEmail();*/
		UserDefault::getInstance()->setIntegerForKey("logintype", 2);
		UserDefault::getInstance()->setStringForKey("LT", "facebook");
		UserDefault::getInstance()->setStringForKey("google_id", "");
		UserDefault::getInstance()->setStringForKey("facebook_id", socialInfo.uid);
		UserDefault::getInstance()->flush();

		string email_ = socialInfo.email.compare("") == 0 ? "" : socialInfo.email;
         if (SceneManager::getSingleton().isNagaCard){
             email_ = "nagacard_"+email_;
         }
		parameters->PutUtfString("LT", "facebook");
		parameters->PutUtfString("aI", socialInfo.uid);
		parameters->PutUtfString("aN", "");
		parameters->PutUtfString("imc", "mobile");
		parameters->PutUtfString("aM", email_);
		parameters->PutUtfString("remail", email_);
		parameters->PutUtfString("LSK", "");
		parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
		parameters->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
		parameters->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
		parameters->PutInt("osid", osid);// Operatorsystems
		parameters->PutUtfString("devid", devID);// decvice id
		parameters->PutUtfString("aIp", "");//ip
		parameters->PutInt("srcref", 0);
        parameters->PutInt(LANG_CODE, SceneManager::getSingleton().currLang);

		CCLOG("UID: %s -- Name: %s", socialInfo.uid.c_str(), socialInfo.name.c_str());
		boost::shared_ptr<IRequest> req_login(new LoginRequest(socialInfo.uid, "", "GameZone1", parameters));
		GameServer::getSingleton().SendLogin(req_login);
	}
	else if (type == gmailType)
	{
		log("Login With Type = 1");

		/*socialInfo.uid = scene.getUID();
		 socialInfo.name = scene.getName();
		 socialInfo.email = scene.getEmail();*/
		if (strcmp(socialInfo.uid.c_str(), "") == 0)
			return;

		UserDefault::getInstance()->setIntegerForKey("logintype", 3);
		UserDefault::getInstance()->setStringForKey("LT", "google");
		UserDefault::getInstance()->setStringForKey("google_id", socialInfo.uid);
		UserDefault::getInstance()->setStringForKey("facebook_id", "");
		UserDefault::getInstance()->flush();

		string email_ = socialInfo.email.compare("") == 0 ? "" : socialInfo.email;
		parameters->PutUtfString("LT", "google");
		parameters->PutUtfString("aI", socialInfo.uid);
		parameters->PutUtfString("aN", "");
		parameters->PutUtfString("imc", "mobile");
		parameters->PutUtfString("aM", email_);
		parameters->PutUtfString("remail", email_);
		parameters->PutUtfString("LSK", "");
		parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
		parameters->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
		parameters->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
		parameters->PutInt("osid", osid);// Operatorsystems
		parameters->PutUtfString("devid", devID);// decvice id
		parameters->PutUtfString("aIp", "");//ip
		parameters->PutInt("srcref", 0);
        parameters->PutInt(LANG_CODE, SceneManager::getSingleton().currLang);

		CCLOG("UID: %s -- Name: %s", socialInfo.uid.c_str(), socialInfo.name.c_str());
		boost::shared_ptr<IRequest> req_login(new LoginRequest(socialInfo.uid, "", "GameZone1", parameters));
		GameServer::getSingleton().SendLogin(req_login);
	}
	else if (type == quickType)
	{
		log("Login With Type = quickType");

		UserDefault::getInstance()->setIntegerForKey("logintype", 4);
		UserDefault::getInstance()->setStringForKey("LT", "sp");
		UserDefault::getInstance()->setStringForKey("google_id", "");
		UserDefault::getInstance()->setStringForKey("facebook_id", "");
		UserDefault::getInstance()->flush();

		string email_ = "";
		parameters->PutUtfString("LT", "sp");
		parameters->PutUtfString("aI", "");
		parameters->PutUtfString("aN", "");
		parameters->PutUtfString("imc", "mobile");
		parameters->PutUtfString("aM", "");
		parameters->PutUtfString("LSK", "");
		parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
		parameters->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
		parameters->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
		parameters->PutInt("osid", osid);// Operatorsystems
		parameters->PutUtfString("aIp", "");//ip
		parameters->PutInt("srcref", 0);
        parameters->PutInt(LANG_CODE, SceneManager::getSingleton().currLang);

        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (SceneManager::getSingleton().isNagaCard){
            boost::shared_ptr<string> _aidevice = PasswordUtil::MD5Password(mUtils::replaceAll(devID, "-", ""));
           // log("device id: %s",_aidevice->c_str());
            parameters->PutUtfString("devid", *_aidevice);// decvice id

            boost::shared_ptr<IRequest> req_login(new LoginRequest(*_aidevice, "", "GameZone1", parameters));
            GameServer::getSingleton().SendLogin(req_login);
        
        }else{
            parameters->PutUtfString("devid", devID);// decvice id

            string _ai = mUtils::replaceAll(devID, "-", "");

            boost::shared_ptr<IRequest> req_login(new LoginRequest(_ai, "", "GameZone1", parameters));
            GameServer::getSingleton().SendLogin(req_login);
        }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
       // log("android nagacard nhay xuong day lam deo gi?");
        parameters->PutUtfString("devid", devID);// decvice id

        string _ai = mUtils::replaceAll(devID, "-", "");
        
        boost::shared_ptr<IRequest> req_login(new LoginRequest(_ai, "", "GameZone1", parameters));
        GameServer::getSingleton().SendLogin(req_login);
#endif
 
	}
	else if (type == registType){
		boost::shared_ptr<IRequest> request(new LoginRequest("", "123456", "RegisterZone"));
		GameServer::getSingleton().SendLogin(request);
	}
}

void LayerLogin::readInfo()
{
	UserDefault *def = UserDefault::getInstance();

	string username = def->getStringForKey("username", "");
	string password = def->getStringForKey("password", "");

	log("username = %s", username.c_str());
	log("password = %s", password.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	if (this->txtUserName == NULL)// log("txtUserName === NULL");
	this->txtUserName->setString(username);
	this->txtPassword->setString(password);
#else
	this->txtUserName->setString(username);
	this->txtPassword->setString(password);
#endif

}

void LayerLogin::saveInfo()
{
	if (txtUserName == NULL || txtPassword == NULL)
		return;
	UserDefault *def = UserDefault::getInstance();
	string _oldFacebookID = def->getStringForKey("facebook_id", "");
	string _oldGmailID = def->getStringForKey("google_id", "");
	string _oldLT = def->getStringForKey("LT", "");

	string _oldUsername = def->getStringForKey("username", "");
	string _oldUserpass = def->getStringForKey("password", "");

	boost::shared_ptr<string> _pass = PasswordUtil::MD5Password(_oldUserpass);
	_oldUserpass = *PasswordUtil::MD5Password(*_pass);

	if (_oldLT.compare("normal") != 0){
		_oldUsername = "";
		_oldUserpass = "";
	}
	else{
		_oldFacebookID = "";
		_oldGmailID = "";
	}

	string _oldUserRechargeHis = def->getStringForKey("litr", "");
	string _oldAmf = def->getStringForKey("amf", "");
	string _oldAm = def->getStringForKey("am", "");
	string _oldaN = def->getStringForKey("aN", "");
	string _oldaM = def->getStringForKey("aM", "");
	string _oldaMo = def->getStringForKey("aMo", "");
	string _oldLvl = def->getStringForKey("lvl", "");
	string _oldExp = def->getStringForKey("exp", "");
	string _oldAccVipType = def->getStringForKey("accVipType", "");
	string _oldDeviceID = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	_oldDeviceID = NativeHelper::getDeviceId();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	_oldDeviceID = IOS::getDeviceID();
#endif
	//insert 01/08/2016
	if ((_oldLT.compare("normal") == 0 && _oldUsername.compare(txtUserName->getString()) == 0)
		|| (_oldLT.compare("facebook") == 0 && socialInfo.uid.compare(_oldFacebookID) == 0)
		|| (_oldLT.compare("google") == 0 && socialInfo.uid.compare(_oldGmailID) == 0)
		){
		//nothing change
	}
	else if (_oldUsername.length() > 0){
		string _lstUserInfo = def->getStringForKey("lstUserInfo", "");
		//save to list
		rapidjson::Document d;
		d.Parse<0>(_lstUserInfo.c_str());
		bool isExisted = false;
		if (d.IsArray()){
			rapidjson::SizeType size = d.Size();
			for (rapidjson::SizeType i = 0; i < size; ++i){
				if ((_oldLT.compare("normal") == 0 && _oldUsername.compare(d[i]["username"].GetString()) == 0)
					|| (_oldLT.compare("facebook") == 0 && socialInfo.uid.compare(d[i]["facebook"].GetString()) == 0)
					|| (_oldLT.compare("google") == 0 && socialInfo.uid.compare(d[i]["google"].GetString()) == 0)
					){
					isExisted = true;
					d[i].RemoveMember("password");
					d[i].AddMember("password", rapidjson::Value(_oldUserpass.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("LT");
					d[i].AddMember("LT", rapidjson::Value(_oldLT.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("facebook");
					d[i].AddMember("facebook", rapidjson::Value(_oldFacebookID.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("google");
					d[i].AddMember("google", rapidjson::Value(_oldGmailID.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("amf");
					d[i].AddMember("amf", rapidjson::Value(_oldAmf.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("am");
					d[i].AddMember("am", rapidjson::Value(_oldAm.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("aN");
					d[i].AddMember("aN", rapidjson::Value(_oldaN.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("aM");
					d[i].AddMember("aM", rapidjson::Value(_oldaM.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("aMo");
					d[i].AddMember("aMo", rapidjson::Value(_oldaMo.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("lvl");
					d[i].AddMember("lvl", rapidjson::Value(_oldLvl.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("exp");
					d[i].AddMember("exp", rapidjson::Value(_oldExp.c_str(), d.GetAllocator()), d.GetAllocator());
					d[i].RemoveMember("accVipType");
					d[i].AddMember("accVipType", rapidjson::Value(_oldAccVipType.c_str(), d.GetAllocator()), d.GetAllocator());
					if (_oldUserRechargeHis.length() > 0){
						d[i].RemoveMember("litr");
						d[i].AddMember("litr", rapidjson::Value(_oldUserRechargeHis.c_str(), d.GetAllocator()), d.GetAllocator());
					}
					d[i].RemoveMember("deviceID");
					d[i].AddMember("deviceID", rapidjson::Value(_oldDeviceID.c_str(), d.GetAllocator()), d.GetAllocator());
					break;
				}
			}
		}
		else{
			d.SetArray();
		}
		if (!isExisted){ //Not exist in list user history
			//insert to his logs
			rapidjson::Value _user(rapidjson::kObjectType);
			{
				_user.AddMember("username", rapidjson::Value(_oldUsername.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("LT", rapidjson::Value(_oldLT.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("google", rapidjson::Value(_oldGmailID.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("facebook", rapidjson::Value(_oldFacebookID.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("password", rapidjson::Value(_oldUserpass.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("litr", rapidjson::Value(_oldUserRechargeHis.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("amf", rapidjson::Value(_oldAmf.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("am", rapidjson::Value(_oldAm.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("aN", rapidjson::Value(_oldaN.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("lvl", rapidjson::Value(_oldLvl.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("accVipType", rapidjson::Value(_oldAccVipType.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("deviceID", rapidjson::Value(_oldDeviceID.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("exp", rapidjson::Value(_oldExp.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("aM", rapidjson::Value(_oldaM.c_str(), d.GetAllocator()), d.GetAllocator());
				_user.AddMember("aMo", rapidjson::Value(_oldaMo.c_str(), d.GetAllocator()), d.GetAllocator());
			}
			d.PushBack(_user, d.GetAllocator());
		}
		rapidjson::StringBuffer strbuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		d.Accept(writer);
		def->setStringForKey("lstUserInfo", strbuf.GetString());
	}
	//
	def->setStringForKey("username", txtUserName->getString());
	def->setStringForKey("password", txtPassword->getString());
	def->setStringForKey("litr", "");
	def->setStringForKey("amf", "");
	def->setStringForKey("am", "");
	def->setStringForKey("aN", "");
	def->setStringForKey("aM", "");
	def->setStringForKey("aMo", "");
	def->setStringForKey("lvl", "");
	def->setStringForKey("exp", "");
	def->setStringForKey("accVipType", "");
	def->flush();
}

void LayerLogin::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    Scene* _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
	if (strcmp("rg", cmd->c_str()) == 0){
		int rc = *param->GetInt("rc");
		if (rc == 0){//OK
			//#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
			//            //LoginLinux* layerLogin = SceneManager::getSingleton().getLayerLogin();
			//#else
			//            gotoLogin(0);
			//#endif
            SceneManager::getSingleton().isShouldAuto = false;
            SceneManager::getSingleton().showLoading();
            boost::shared_ptr<IRequest> request(new LogoutRequest());
            GameServer::getSingleton().Send(request);

			this->txtUserName->setString(this->txtRegUserName->getString());
			this->txtPassword->setString(this->txtRegPassword->getString());
			//
			Chat *toast = Chat::create(dataManager.GetSysString(474), -1);
			this->addChild(toast);

			boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
			if (myself != NULL){

				if (myself->GetVariable("adn") != NULL)
				{

					string _strName = *myself->GetVariable("adn")->GetStringValue();
					//26/01/2016 HoangDD changes
					if (_strName.compare("") == 0){
						//Neu ko phhai la login binh thuong, se kiem tra aN, neu aN = "" thi bat buoc phai update
						this->nodeButton->setVisible(false);
						this->nodeDangNhap->setVisible(false);
						this->nodeDangKy->setVisible(false);
						this->nodeCapNhatTen->setVisible(true);
                        pnlBG->setOpacity(190);


						return;
					}

				}
			}

			this->onBtnDangNhapVaoGame(NULL);
		}
		else{//Not OK
            LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
            popupThongBao->setMessage(dataManager.GetSysString(141));
            popupThongBao->setContentMess(*param->GetUtfString("rd"));
            popupThongBao->showAButton();
            _currScene->addChild(popupThongBao);
//            Chat *toast = Chat::create(*param->GetUtfString("rd"), -1);
//            this->addChild(toast);
		}
		SceneManager::getSingleton().hideLoading();
	}
	else if (strcmp(EXT_EVENT_UPDATE_DISPLAY_NAME_RESP, cmd->c_str()) == 0){
		SceneManager::getSingleton().hideLoading();
		int rc = *param->GetInt("rc");
		if (rc == 0){//OK
			SceneMain* _parent = (SceneMain*)this->getParent();
			if (_parent != NULL){
				_parent->reloadData();
			}

			if (lstDatas.size() != 0)
			{



				int mRoomID;
				int mGameID;
				//mRoomID = atoi(lstDatas.at(2).c_str());
				mGameID = atoi(lstDatas.at(5).substr(0, 3).c_str());
				if (mGameID != kGameTaiXiu && mGameID != kGameQuayThuong && mGameID != kGameXocDia && mGameID != atoi(SERVICE_CHAT_ROOM.c_str()))
				{
					RejoinInfoStruct joinGameInfo;
					joinGameInfo.gameID = lstDatas.at(5);
					joinGameInfo.roomID = -1;
					joinGameInfo.roomName = lstDatas.at(2).c_str();
					joinGameInfo.pass = "";
					joinGameInfo.isNeedRejoin = true;
					SceneManager::getSingleton().setRejoinInfo(joinGameInfo);

					SceneManager::getSingleton().setGameID(atoi(joinGameInfo.gameID.substr(0, 3).c_str()));
					SceneManager::getSingleton().gotoPickGame(atoi(joinGameInfo.gameID.substr(0, 3).c_str()));
				}
				else
				{
					//SceneManager::getSingleton().gotoMain();
                    SceneMain* parent = (SceneMain*)this->getParent();
                    if (parent){
                        this->setVisible(false);
                        parent->logInDone();
                        this->removeFromParent();
                    }
				}
				// Kiểm tra xem room có tồn tại hay không, nếu tồn tại có nghĩa là
				// Mất kết nối lúc chơi game, bây giờ join lại. //ROOM_ID = "ri";
				// 		boost::shared_ptr<MatchExpression> expr(new MatchExpression("ri", NumberMatch::EQUALS, boost::make_shared<int>(mGameID)
				//             , MatchExpression::MatchExpressionValueType::MatchExpression_ValueOther));
				// 		// Send request
				// 		boost::shared_ptr<IRequest> request(new FindRoomsRequest(expr));
				// 		GameServer::getSingleton().Send(request);
				// 		if( GameServer::getSingleton().getSmartFox()->GetRoomById(mRoomID) == NULL)
				// 			return;
				//
				// 		log("Ban dang choi game %s phong so %d", mUtils::getGameNameByID(mGameID), mRoomID);
				//		LayerMain::getSingleton().autoJoinGameWithID(mGameID, mRoomID);
			}
			else
			{
				log("Ban khong choi game nao");
				//SceneManager::getSingleton().gotoMain();
                SceneMain* parent = (SceneMain*)this->getParent();
                if (parent){
                    this->setVisible(false);
                    parent->logInDone();
                    this->removeFromParent();
                }
			}

		}
		else{//Not OK
            LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
            popupThongBao->setMessage(dataManager.GetSysString(141));
            popupThongBao->setContentMess(*param->GetUtfString("rd"));
            popupThongBao->showAButton();
            _currScene->addChild(popupThongBao);
		}
	}
	else if (strcmp("resuaarf", cmd->c_str()) == 0){//OK
		log("param: %d", *param->GetInt("rc"));
		log("param: %s", (*param->GetUtfString("rd")).c_str());

		if (param->GetInt("rc") == NULL || param->GetInt("rd") == NULL) return;
		SceneManager::getSingleton().hideLoading();
		if (*param->GetInt("rc") == 0){

			log("Change Name OK");
			Chat *toast = Chat::create(dataManager.GetSysString(475), -1);
			this->addChild(toast);
			LayerPopupThongBao* layerpoup = LayerPopupThongBao::create();
			layerpoup->setMessage(dataManager.GetSysString(141));
			layerpoup->setContentMess(dataManager.GetSysString(440));
			layerpoup->getBtnHuy()->setVisible(false);
			layerpoup->getBtnOk()->setPositionX(layerpoup->getLblTitle()->getPositionX());
			layerpoup->setCallBackFromButton(this, callfuncO_selector(LayerLogin::onPopupCallBack), 1);
           // layerpoup->setPopupClose();
			_currScene->addChild(layerpoup);
			this->currentPop = 2;
            
//            bool isLink = UserDefault::getInstance()->getBoolForKey("isReceivedFacebook");
//            if (!isLink){//|| std::strcmp(status.c_str(),"0")==0) {
//                if (strcmp("facebook", dataManager.getAppConfig().loginType.c_str()) == 0){
//
//                    boost::shared_ptr<ISFSObject> params(new SFSObject());
//                    boost::shared_ptr<IRequest> request(new ExtensionRequest("rfb", params));
//                    GameServer::getSingleton().Send(request);
//
//                }
 //           }

		}
		//
		else{
			auto rd = *param->GetUtfString("rd");
			//			string mess;
			//			switch (rc){
			//			case 139:
			//				mess = "Tên đã tồn tại!";
			//				break;
			//			}
            
            LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
            popupThongBao->setMessage(dataManager.GetSysString(141));
            popupThongBao->setContentMess(rd);
            popupThongBao->showAButton();
            _currScene->addChild(popupThongBao);
		}

	}
}
void LayerLogin::gotoLogin(float dt)
{
    isShouldAuto = SceneManager::getSingleton().isShouldAuto;
    this->unschedule(schedule_selector(LayerLogin::gotoLogin));
    if (!isShouldAuto)
        return;
    int lastLoginType = UserDefault::getInstance()->getIntegerForKey("logintype");
    bool isAuto = UserDefault::getInstance()->getBoolForKey("isAutoLogin");
    switch (lastLoginType){
    case 2:
        if ((!SceneManager::getSingleton().isLogout&&!isAuto) || SceneManager::getSingleton().rcForLogin){
            onBtnDangNhapFB(NULL);
            readInfo();
            return;
        }
    case 3:
        if ((!SceneManager::getSingleton().isLogout&&!isAuto) || SceneManager::getSingleton().rcForLogin){
            readInfo();
            return;
        }
    case 4:
        if ((!SceneManager::getSingleton().isLogout&&!isAuto) || SceneManager::getSingleton().rcForLogin){
            onButtonChoiNgay(NULL);
            readInfo();
            return;
        }
    case 1:
        if (!SceneManager::getSingleton().isLogout&&!isAuto){
            onBtnDangNhapVaoGame(NULL);
            readInfo();
            return;
        }
    }
    this->unschedule(schedule_selector(LayerLogin::gotoLogin));
    readInfo();
}
void LayerLogin::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		if (this->getChildByTag(POPUP_THONGBAO) == NULL){
			showPopupThongbao();
		}
	}
}

void LayerLogin::showPopupThongbao(){
	this->currentPop = 1;
    this->setVisible(true);
    LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
    popupThongBao->setMessage(dataManager.GetSysString(141));
    popupThongBao->setContentMess(dataManager.GetSysString(534));
	popupThongBao->setCallBackFromButton(this, callfuncO_selector(LayerLogin::onPopupCallBack), 3);
	this->addChild(popupThongBao,1000000);
}

void LayerLogin::onPopupCallBack(Ref *pSender)
{
	if (this->currentPop == 1){
		Director::getInstance()->end();
	}
	else if (this->currentPop == 2){
		SceneManager::getSingleton().showLoading();
		boost::shared_ptr<IRequest> request(new LogoutRequest());
		GameServer::getSingleton().Send(request);
		//
		SceneManager::getSingleton().rcForLogin = true;
		SceneManager::getSingleton().gotoLogin();
		UserDefault::getInstance()->setBoolForKey("isShouldShowWelcome", true);
	}
}

void LayerLogin::scheduleUpdateEditbox(float dt)
{
	readInfo();
}

void LayerLogin::onBtnDangNhapVaoGame(Ref* pSender){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	IOS::tryIsInternetConnection();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	NativeHelper::hasConnection();
#endif
	if (txtUserName == NULL || txtPassword == NULL)
		return;
	std::string strS = string(txtUserName->getString());
	if (mUtils::trimString(strS).length() < 6)
	{
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(464));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}

	if (mUtils::findSpecialCharNotSpace(string(txtUserName->getString()))){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(465));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}

	strS = string(txtPassword->getString());
	if (mUtils::trimString(strS).length() < 6)
	{
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(466));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}
	if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
        LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
        popupThongBao->setMessage(dataManager.GetSysString(141));
        popupThongBao->setContentMess(dataManager.GetSysString(459));
        popupThongBao->showAButton();
        this->addChild(popupThongBao);
		return;
	}
	currLoginType = loginType;
	if (!GameServer::getSingleton().getSmartFox()->IsConnected())
	{
		SceneManager::getSingleton().connect2Server(0);
	}
	else
	{
		//GameServer::getSingleton().getSmartFox()->Disconnect();
		SceneManager::getSingleton().connect2Server(0);
	}

}
void LayerLogin::onBtnBackDangNhap(Ref* pSender){
	this->nodeButton->setVisible(true);
	this->nodeDangNhap->setVisible(false);
	this->nodeDangKy->setVisible(false);

//	SceneManager::getSingleton().showLoading();
//	boost::shared_ptr<IRequest> request(new LogoutRequest());
//	GameServer::getSingleton().Send(request);

}
void LayerLogin::onBtnBackDangKy(Ref* pSender){
    if (SceneManager::getSingleton().isNagaNew){
        this->nodeDangNhap->setVisible(true);
        this->nodeDangKy->setVisible(false);
        pnlBG->setOpacity(0);
        
        //currLoginType = registType;
        this->isShouldShowGuide = true;
        SceneManager::getSingleton().showLoading();
        boost::shared_ptr<IRequest> request(new LogoutRequest());
        GameServer::getSingleton().Send(request);
        
        return;
    }
	this->nodeButton->setVisible(true);
	this->nodeDangNhap->setVisible(false);
	this->nodeDangKy->setVisible(false);
    pnlBG->setOpacity(0);

	//currLoginType = registType;
    this->isShouldShowGuide = true;
	SceneManager::getSingleton().showLoading();
	boost::shared_ptr<IRequest> request(new LogoutRequest());
	GameServer::getSingleton().Send(request);
	//
}
void LayerLogin::onBtnDangNhap(Ref* pSender){
	this->nodeButton->setVisible(false);
	this->nodeDangNhap->setVisible(true);
	this->nodeDangKy->setVisible(false);

}
void LayerLogin::onBtnDangKy(Ref* pSender){
	this->nodeButton->setVisible(false);
	this->nodeDangNhap->setVisible(false);
    this->pnlBgButton->setVisible(false);

	this->nodeDangKy->setVisible(true);
    this->nodeDangKy->setScale(0);
    this->nodeDangKy->runAction(ScaleTo::create(0.2, 1));
    pnlBG->setOpacity(190);

    this->isShouldShowGuide = false;
	currLoginType = registType;

	//
	if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
		return;
	}
	if (!GameServer::getSingleton().getSmartFox()->IsConnected()){
		SceneManager::getSingleton().connect2Server(0);
		return;
	}
	doLogin(registType);
}
void LayerLogin::onBtnDangNhapFB(Ref* pSender){
	showMaintain(0);
	showUpdateInfo(0);
	log("onButtonLoginFacebook press");
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	mFaceBook *face = new mFaceBook();
	face->onLogin();
#endif
}
void LayerLogin::hideLayer(){
	this->setVisible(false);
}
void LayerLogin::showLayer(){
	this->setVisible(true);

}
bool LayerLogin::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    showMaintain(0);
    showUpdateInfo(0);
	this->setVisible(true);
    
//    if (SceneManager::getSingleton().isNagaNew){
//        Size visibleSize = Director::getInstance()->getVisibleSize();
//        auto rect = Rect(0,0,visibleSize.width,visibleSize.height-160);
//        auto locationInNode = pTouch->getLocation();
//        if (!this->nodeDangNhap->isVisible()){
//            if (rect.containsPoint(locationInNode)){
//                this->isShowTouch = true;
//
//            }
//            else{
//                this->isShowTouch = false;
//            }
//        }else{
//            auto rectBg = this->nodeDangNhap->getBoundingBox();
//            if (rectBg.containsPoint(locationInNode)){
//                this->isShowTouch = false;
//            }
//            else{
//                this->isShowTouch = true;
//
//            }
//            return true;
//
//            if (rect.containsPoint(locationInNode)){
//                this->isShowTouch = true;
//            }else{
//                this->isShowTouch = false;
//
//            }
//        }
//    }else{
        Size visibleSize = Director::getInstance()->getVisibleSize();
        auto rect = Rect(0,0,visibleSize.width,visibleSize.height-160);
        auto locationInNode = pTouch->getLocation();
        if (!this->pnlBgButton->isVisible()){
            if (rect.containsPoint(locationInNode)){
                this->isShowTouch = true;
                
            }
            else{
                this->isShowTouch = false;
            }
        }else{
            auto rectBg = this->bgButton->getBoundingBox();
            if (rectBg.containsPoint(locationInNode)){
                this->isShowTouch = false;
            }
            else{
                this->isShowTouch = true;
                
            }
            return true;
            
            if (rect.containsPoint(locationInNode)){
                this->isShowTouch = true;
            }else{
                this->isShowTouch = false;
                
            }
        }
    //}
    
   
    return true;

}
void LayerLogin::onTouchEnded(Touch *touch, Event *unused_event){
    if (!this->isShowTouch || this->isLogin) return;
    if (SceneManager::getSingleton().isNagaNew){
        if (!this->nodeDangNhap->isVisible()){
            this->nodeDangNhap->setVisible(true);
            //        this->pnlBgButton->setScale(0);
            //        this->pnlBgButton->runAction(ScaleTo::create(0.2, 1));
        }else{
            this->nodeDangNhap->setVisible(false);
        }
    }else{
        if (!this->pnlBgButton->isVisible()){
            this->pnlBgButton->setVisible(true);
            //        this->pnlBgButton->setScale(0);
            //        this->pnlBgButton->runAction(ScaleTo::create(0.2, 1));
        }else{
            this->pnlBgButton->setVisible(false);
        }
    }
    
}
void LayerLogin::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerLogin::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerLogin::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
	//
	//    GameServer::getSingleton().addListeners(this);
}

void LayerLogin::onExit()
{
	Layer::onExit();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	//GameServer::getSingleton().removeListeners(this);
	//    Layer::onExit();
}

