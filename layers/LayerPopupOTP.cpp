//
//  LayerPopupOTP.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 10/27/16.
//
//

#include "LayerPopupOTP.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../data_manager.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/_Chat_.h"
#include "LayerPopupOTPChangePhone.h"
#include "LayerPopupThongBao.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerPopupOTP::LayerPopupOTP()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupOTP::~LayerPopupOTP()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupOTP::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupOTP.csb");
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto bg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    auto lblTitle = static_cast<Text*> (bg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(791));
    auto btn_close = dynamic_cast<Button*>(bg->getChildByName("btnClose"));
    if (btn_close != NULL){
        btn_close->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onButtonClose, this));
        btn_close->setPressedActionEnabled(true);
    }

    
    //
    layerDienThoai = static_cast<Node*>(bg->getChildByName("layerDienThoai"));
    txtNhapSDT = static_cast<TextField*>(layerDienThoai->getChildByName("txtNhapSDT"));
    txtNhapSDT->setPlaceHolder(dataManager.GetSysString(792));
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    txtNhapSDT->setInputMode(ui::EditBox::InputMode::NUMERIC);
#endif
    txtOTP = static_cast<TextField*>(layerDienThoai->getChildByName("txtOTP"));
    txtOTP->setPlaceHolder(dataManager.GetSysString(793));

    btnKichHoatOTP = static_cast<Button*>(layerDienThoai->getChildByName("btnKichHoat"));
    if (btnKichHoatOTP){
        btnKichHoatOTP->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onBtnKichHoatOTP, this));
        btnKichHoatOTP->setPressedActionEnabled(true);
        btnKichHoatOTP->setTitleText(dataManager.GetSysString(795));
    }

    btnGetOTP = static_cast<Button*>(layerDienThoai->getChildByName("btnLayOTP"));
    if (btnGetOTP){
        btnGetOTP->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onBtnGetOTP, this));
        btnGetOTP->setPressedActionEnabled(true);
        btnGetOTP->setTitleText(dataManager.GetSysString(794));

    }
    
    btnDoiSDT = static_cast<Button*>(layerDienThoai->getChildByName("btnChangePhone"));
    if (btnDoiSDT){
        btnDoiSDT->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onBtnDoiSDT, this));
        btnDoiSDT->setPressedActionEnabled(true);
        btnDoiSDT->setTitleText(dataManager.GetSysString(796));

    }
    
    auto btnGuideKichHoatOTP = static_cast<Button*>(layerDienThoai->getChildByName("btnGuide"));
    if (btnGuideKichHoatOTP){
        btnGuideKichHoatOTP->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onBtnGuideOTP, this));
        btnGuideKichHoatOTP->setPressedActionEnabled(true);
    }

    
    btnHuyOTP = static_cast<Button*>(layerDienThoai->getChildByName("btnHuyOTP"));
    if (btnHuyOTP){
        btnHuyOTP->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onBtnHuyOTP, this));
        btnHuyOTP->setPressedActionEnabled(true);
        btnHuyOTP->setTitleText(dataManager.GetSysString(797));

    }

    imgbgsdt = static_cast<ImageView*>(layerDienThoai->getChildByName("imgbgsdt"));
    
    mainTitle = static_cast<Text*>(layerDienThoai->getChildByName("txtMainTitle"));
    mainTitle->setString(dataManager.GetSysString(942));

    mainTitle->setString("");
    if (dataManager.getAppConfig().isactiveOTP){
        mainTitle->setString(dataManager.GetSysString(449));
        imgbgsdt->setVisible(false);
        btnHuyOTP->setVisible(true);
        btnDoiSDT->setVisible(true);
        btnKichHoatOTP->setVisible(false);
        txtNhapSDT->setEnableEditBox(false);
        //txtNhapSDT->setVisible(false);

    }
    
    
    //
    layerChangePhone = static_cast<Node*>(bg->getChildByName("layerChangePhone"));
    changePhoneTitle = static_cast<Text*>(layerChangePhone->getChildByName("txtTitleMain"));
    
    txtNhapSDTChangePhone = static_cast<TextField*>(layerChangePhone->getChildByName("txtNhapSDT"));
    txtNhapSDTChangePhone->setPlaceHolder(dataManager.GetSysString(798));
    txtOTPChangePhone = static_cast<TextField*>(layerChangePhone->getChildByName("txtOTP"));
    txtOTPChangePhone->setPlaceHolder(dataManager.GetSysString(793));
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    txtNhapSDTChangePhone->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->txtNhapSDTChangePhone->setMaxLength(13);
    this->txtNhapSDT->setMaxLength(13);
#endif
    auto btnGuideDoiSDT = static_cast<Button*>(layerChangePhone->getChildByName("btnGuide"));
    if (btnGuideDoiSDT){
        btnGuideDoiSDT->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onBtnGuideOTP, this));
        btnGuideDoiSDT->setPressedActionEnabled(true);
    }
    
    btnGetOTPChangePhone = static_cast<Button*>(layerChangePhone->getChildByName("btnLayOTP"));
    if (btnGetOTPChangePhone){
        btnGetOTPChangePhone->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onBtnGetOTPChangePhone, this));
        btnGetOTPChangePhone->setPressedActionEnabled(true);
        btnGetOTPChangePhone->setTitleText(dataManager.GetSysString(794));
    }
    
    
    btnXacThucChangePhone = static_cast<Button*>(layerChangePhone->getChildByName("btnXacThuc"));
    if (btnXacThucChangePhone){
        btnXacThucChangePhone->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onBtnXacThucOTPChangePhone, this));
        btnXacThucChangePhone->setPressedActionEnabled(true);
        btnXacThucChangePhone->setTitleText(dataManager.GetSysString(799));
    }
    
    btnDoiSDTChangePhone = static_cast<Button*>(layerChangePhone->getChildByName("btnChangePhone"));
    if (btnDoiSDTChangePhone){
        btnDoiSDTChangePhone->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onBtnDoiSDTChangePhone, this));
        btnDoiSDTChangePhone->setPressedActionEnabled(true);
        btnDoiSDTChangePhone->setTitleText(dataManager.GetSysString(796));

    }
    Button* btnBackChangePhone = static_cast<Button*>(layerChangePhone->getChildByName("btnBackChangePhone"));
    if (btnBackChangePhone){
        btnBackChangePhone->addClickEventListener(CC_CALLBACK_1(LayerPopupOTP::onBtnBackChangePhone, this));
        btnBackChangePhone->setPressedActionEnabled(true);
    }
    imgbgsdtChangePhone = static_cast<ImageView*>(layerChangePhone->getChildByName("imgbgsdt"));
    //this->setMoney("2000");
    //
    this->shouldGetOTPNewPhone = false;
    return true;
}
void LayerPopupOTP::onBtnGuideOTP(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    /*getUser*/
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL || myself->Name() == NULL)
        return;
    string strlink = "https://gamebai.net/webview/gotp";
        LayerPopupWebView* _layer = LayerPopupWebView::create();
        _layer->setWebView(strlink);
        _currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_WEBVIEW,9059);

}
void LayerPopupOTP::onBtnHuyOTP(Ref* pSender)
{
    string otp = this->txtOTP->getString();
    if (strcmp("", otp.c_str()) == 0){
        Chat* toast = Chat::create(dataManager.GetSysString(442),-1);
        this->addChild(toast);
        return;
    }
    
    SceneManager::getSingleton().showLoading();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("otpcode", otp);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("dotpcmd", params));
    GameServer::getSingleton().Send(request);
    txtOTP->setString("");
}
void LayerPopupOTP::onButtonClose(Ref* pSender)
{
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("safeboxacmd", params));
    GameServer::getSingleton().Send(request);
    
    this->removeFromParentAndCleanup(true);
}
void LayerPopupOTP::onBtnKichHoatOTP(Ref* pSender){
    string sdt = this->txtNhapSDT->getString();
    string otp = this->txtOTP->getString();
    if (strcmp("", sdt.c_str()) == 0){
        Chat* toast = Chat::create(dataManager.GetSysString(443),-1);
        this->addChild(toast);
        return;
    }
    if (mUtils::findContainChar(sdt.c_str())){
        Chat* toast = Chat::create(dataManager.GetSysString(444),-1);
        this->addChild(toast);
        return;
    }
    if (sdt.length() < 10){
        Chat* toast = Chat::create(dataManager.GetSysString(445),-1);
        this->addChild(toast);
        return;
    }
    if (strcmp("", otp.c_str()) == 0){
        Chat* toast = Chat::create(dataManager.GetSysString(442),-1);
        this->addChild(toast);
        return;
    }
    
    SceneManager::getSingleton().showLoading();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("aMo", sdt);
    params->PutUtfString("otpcode", otp);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("aotpcmd", params));
    GameServer::getSingleton().Send(request);
    txtOTP->setString("");
}
void LayerPopupOTP::onBtnGetOTP(Ref* pSender){
    
    string sdt = this->txtNhapSDT->getString();
    if (!dataManager.getAppConfig().isactiveOTP){
    if (strcmp("", sdt.c_str()) == 0){
        Chat* toast = Chat::create(dataManager.GetSysString(443),-1);
        this->addChild(toast);
        return;
    }
    if (mUtils::findContainChar(sdt.c_str())){
        Chat* toast = Chat::create(dataManager.GetSysString(444),-1);
        this->addChild(toast);
        return;
    }
    if (sdt.length() < 10){
        Chat* toast = Chat::create(dataManager.GetSysString(445),-1);
        this->addChild(toast);
        return;
    }
    }
    
    SceneManager::getSingleton().showLoading();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    if (!dataManager.getAppConfig().isactiveOTP)
        params->PutUtfString("aMo", sdt);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("gotpcmd", params));
    GameServer::getSingleton().Send(request);
    txtOTP->setString("");
}
void LayerPopupOTP::onBtnDoiSDT(Ref* pSender){
    layerChangePhone->setVisible(true);
    layerDienThoai->setVisible(false);
}
void LayerPopupOTP::onBtnBackChangePhone(Ref* pSender){
    layerChangePhone->setVisible(false);
    layerDienThoai->setVisible(true);
}
//
void LayerPopupOTP::onBtnGetOTPChangePhone(Ref* pSender){
    
    if (this->shouldGetOTPNewPhone)
    {
        string sdt = this->txtNhapSDTChangePhone->getString();
        if (strcmp("", sdt.c_str()) == 0){
            Chat* toast = Chat::create(dataManager.GetSysString(443),-1);
            this->addChild(toast);
            return;
        }
        if (mUtils::findContainChar(sdt.c_str())){
            Chat* toast = Chat::create(dataManager.GetSysString(444),-1);
            this->addChild(toast);
            return;
        }
        if (sdt.length() < 10){
            Chat* toast = Chat::create(dataManager.GetSysString(445),-1);
            this->addChild(toast);
            return;
        }
        SceneManager::getSingleton().showLoading();
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("aMo", sdt);
        boost::shared_ptr<IRequest> request(new ExtensionRequest("gotpcmd", params));
        GameServer::getSingleton().Send(request);
        txtOTPChangePhone->setString("");
    }
    else{
        SceneManager::getSingleton().showLoading();
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        boost::shared_ptr<IRequest> request(new ExtensionRequest("gotpcmd", params));
        GameServer::getSingleton().Send(request);
        txtOTPChangePhone->setString("");
    }
}

void LayerPopupOTP::onBtnDoiSDTChangePhone(Ref* pSender)
{
    string sdt = this->txtNhapSDTChangePhone->getString();
    string otp = this->txtOTPChangePhone->getString();
    if (strcmp("", sdt.c_str()) == 0){
        Chat* toast = Chat::create(dataManager.GetSysString(443),-1);
        this->addChild(toast);
        return;
    }
    if (strcmp("", otp.c_str()) == 0){
        Chat* toast = Chat::create(dataManager.GetSysString(442),-1);
        this->addChild(toast);
        return;
    }
    
    if (strcmp(strOTPSDTCu.c_str(), otp.c_str()) == 0){
        Chat* toast = Chat::create(dataManager.GetSysString(446),-1);
        this->addChild(toast);
        return;
    }
    if (mUtils::findContainChar(sdt.c_str())){
        Chat* toast = Chat::create(dataManager.GetSysString(444),-1);
        this->addChild(toast);
        return;
    }
    if (sdt.length() < 10){
        Chat* toast = Chat::create(dataManager.GetSysString(445),-1);
        this->addChild(toast);
        return;
    }

    SceneManager::getSingleton().showLoading();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("aMo", sdt);
    params->PutUtfString("otpcode", otp);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("uotpcmd", params));
    GameServer::getSingleton().Send(request);
    txtOTPChangePhone->setString("");
}

void LayerPopupOTP::onBtnXacThucOTPChangePhone(Ref* pSender){
    string otp = this->txtOTPChangePhone->getString();
    if (strcmp("", otp.c_str()) == 0){
        Chat* toast = Chat::create(dataManager.GetSysString(442),-1);
        this->addChild(toast);
        return;
    }
    
    strOTPSDTCu = otp;
    
    SceneManager::getSingleton().showLoading();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("otpcode", otp);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("votpcmd", params));
    GameServer::getSingleton().Send(request);
    txtOTPChangePhone->setString("");
    
    
}

void LayerPopupOTP::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("otpresp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();

        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            Chat* toast = Chat::create(dataManager.GetSysString(447),-1);
            this->addChild(toast);
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }
    else if (strcmp("aotpresp", cmd->c_str()) == 0){
        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            Chat* toast = Chat::create(dataManager.GetSysString(448),-1);
            this->addChild(toast);
            mainTitle->setString(dataManager.GetSysString(449));
            imgbgsdt->setVisible(false);
            btnHuyOTP->setVisible(true);
            btnDoiSDT->setVisible(true);
            btnKichHoatOTP->setVisible(false);
            txtNhapSDT->setEnableEditBox(false);
            txtNhapSDT->setVisible(false);
            dataManager.getAppConfig().isactiveOTP = true;
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }
    else if (strcmp("dotpresp", cmd->c_str()) == 0){
        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            Chat* toast = Chat::create(dataManager.GetSysString(450),-1);
            this->addChild(toast);
            mainTitle->setString(dataManager.GetSysString(451));
            imgbgsdt->setVisible(true);
            btnHuyOTP->setVisible(false);
            btnDoiSDT->setVisible(false);
            btnKichHoatOTP->setVisible(true);
            txtNhapSDT->setEnableEditBox(true);
            txtNhapSDT->setString("");
            txtNhapSDT->setVisible(true);

            //txtNhapSDT->setVisible(true);
            dataManager.getAppConfig().isactiveOTP= false;
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }
    else if (strcmp("votpresp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            Chat* toast = Chat::create(dataManager.GetSysString(452),-1);
            this->addChild(toast);
            changePhoneTitle->setString(dataManager.GetSysString(453));
            imgbgsdtChangePhone->setVisible(true);
            btnXacThucChangePhone->setVisible(false);
            btnDoiSDTChangePhone->setVisible(true);
            //this->txtNhapSDTChangePhone->setVisible(true);
            this->txtNhapSDTChangePhone->setEnableEditBox(true);
            this->shouldGetOTPNewPhone = true;
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }
    
    else if (strcmp("uotpresp", cmd->c_str()) == 0){
        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            Chat* toast = Chat::create(dataManager.GetSysString(454),-1);
            this->addChild(toast);
            changePhoneTitle->setString(dataManager.GetSysString(455));
            imgbgsdtChangePhone->setVisible(false);
            btnXacThucChangePhone->setVisible(true);
            btnDoiSDTChangePhone->setVisible(false);
            //this->txtNhapSDT->setVisible(false);
            this->txtNhapSDT->setEnableEditBox(false);
            this->txtNhapSDT->setVisible(false);

            this->shouldGetOTPNewPhone = false;
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }
   }

bool LayerPopupOTP::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerPopupOTP::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    
}
void LayerPopupOTP::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupOTP::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerPopupOTP::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupOTP::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
