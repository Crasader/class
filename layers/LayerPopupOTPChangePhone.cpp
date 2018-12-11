//
//  LayerPopupOTPChangePhoneChangePhone.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 10/27/16.
//
//

#include "LayerPopupOTPChangePhone.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../data_manager.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/_Chat_.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerPopupOTPChangePhone::LayerPopupOTPChangePhone()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupOTPChangePhone::~LayerPopupOTPChangePhone()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupOTPChangePhone::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupOTPChangePhone.csb");
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
    pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    
    
    auto bg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    auto btn_close = dynamic_cast<Button*>(bg->getChildByName("btnClose"));
    if (btn_close != NULL){
        btn_close->addClickEventListener(CC_CALLBACK_1(LayerPopupOTPChangePhone::onButtonClose, this));
        btn_close->setPressedActionEnabled(true);
    }
    
    
    txtNhapSDT = static_cast<TextField*>(bg->getChildByName("txtNhapSDT"));
    txtOTP = static_cast<TextField*>(bg->getChildByName("txtOTP"));
    
    
    btnGetOTP = static_cast<Button*>(bg->getChildByName("btnLayOTP"));
    if (btnGetOTP){
        btnGetOTP->addClickEventListener(CC_CALLBACK_1(LayerPopupOTPChangePhone::onBtnGetOTP, this));
        btnGetOTP->setPressedActionEnabled(true);
    }

    
    btnXacThuc = static_cast<Button*>(bg->getChildByName("btnXacThuc"));
    if (btnXacThuc){
        btnXacThuc->addClickEventListener(CC_CALLBACK_1(LayerPopupOTPChangePhone::onBtnXacThucOTP, this));
        btnXacThuc->setPressedActionEnabled(true);
    }
    
    btnDoiSDT = static_cast<Button*>(bg->getChildByName("btnChangePhone"));
    if (btnDoiSDT){
        btnDoiSDT->addClickEventListener(CC_CALLBACK_1(LayerPopupOTPChangePhone::onBtnDoiSDT, this));
        btnDoiSDT->setPressedActionEnabled(true);
    }
    
    imgbgsdt = static_cast<ImageView*>(bg->getChildByName("imgbgsdt"));
    
    //this->setMoney("2000");
    
    return true;
}
void LayerPopupOTPChangePhone::onBtnGetOTP(Ref* pSender){
    SceneManager::getSingleton().showLoading();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("gotpcmd", params));
    GameServer::getSingleton().Send(request);
    txtOTP->setString("");
}

void LayerPopupOTPChangePhone::onBtnDoiSDT(Ref* pSender)
{
    string sdt = this->txtNhapSDT->getString();
    string otp = this->txtOTP->getString();
    if (strcmp("", sdt.c_str()) == 0){
        Chat* toast = Chat::create("Bạn chưa nhập số điện thoại",-1);
        this->addChild(toast);
        return;
    }
    if (strcmp("", otp.c_str()) == 0){
        Chat* toast = Chat::create("Bạn chưa nhập mã OTP",-1);
        this->addChild(toast);
        return;
    }
    
    SceneManager::getSingleton().showLoading();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("aMo", sdt);
    params->PutUtfString("otpcode", otp);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("uotpcmd", params));
    GameServer::getSingleton().Send(request);
    txtOTP->setString("");
}
void LayerPopupOTPChangePhone::onButtonClose(Ref* pSender)
{
    this->removeFromParentAndCleanup(true);
}
void LayerPopupOTPChangePhone::onBtnXacThucOTP(Ref* pSender){
    string otp = this->txtOTP->getString();
    if (strcmp("", otp.c_str()) == 0){
        Chat* toast = Chat::create("Bạn chưa nhập mã OTP",-1);
        this->addChild(toast);
        return;
    }
    
    
    
    SceneManager::getSingleton().showLoading();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("otpcode", otp);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("votpcmd", params));
    GameServer::getSingleton().Send(request);
    txtOTP->setString("");

    
}
void LayerPopupOTPChangePhone::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("votpresp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            Chat* toast = Chat::create("Xác thực thành công, vui lòng đổi số điện thoại!",-1);
            this->addChild(toast);
            imgbgsdt->setVisible(true);
            btnXacThuc->setVisible(false);
            btnDoiSDT->setVisible(true);
            this->txtNhapSDT->setVisible(true);
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }
    else if (strcmp("otpresp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            Chat* toast = Chat::create("Vui lòng nhận OTP trong hộp thư của điện thoại!",-1);
            this->addChild(toast);
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }
    if (strcmp("votpresp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            Chat* toast = Chat::create("Xác thực thành công, vui lòng đổi số điện thoại!",-1);
            this->addChild(toast);
            imgbgsdt->setVisible(true);
            btnXacThuc->setVisible(false);
            btnDoiSDT->setVisible(true);
            this->txtNhapSDT->setVisible(true);
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
            Chat* toast = Chat::create("Đổi số điện thoại đăng ký OTP thành công!",-1);
            this->addChild(toast);
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }
}

bool LayerPopupOTPChangePhone::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerPopupOTPChangePhone::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    
}
void LayerPopupOTPChangePhone::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupOTPChangePhone::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerPopupOTPChangePhone::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupOTPChangePhone::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
