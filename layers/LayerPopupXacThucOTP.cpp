//
//  LayerPopupXacThucOTP.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 10/28/16.
//
//

#include "LayerPopupXacThucOTP.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Requests/ExtensionRequest.h"
#include "../SceneManager.h"
#include "../scenes/SceneLogin.h"
#include "../layergames/_Chat_.h"
using namespace cocos2d::ui;

LayerPopupXacThucOTP::LayerPopupXacThucOTP()
{
    GameServer::getSingleton().addListeners(this);
}

LayerPopupXacThucOTP::~LayerPopupXacThucOTP()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupXacThucOTP::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    this->setKeyboardEnabled(false);
    this->setTag(POPUP_THONGBAO);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupXacThucOTP.csb");
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlBg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    this->setContentSize(pnlBg->getContentSize());
    
    btn_huy = dynamic_cast<Button*>(rootNode->getChildByName("btnCancel"));
    if (btn_huy != NULL)
    {
        btn_huy->addClickEventListener(CC_CALLBACK_1(LayerPopupXacThucOTP::onButtonHuy, this));
    }
    btn_dongy = dynamic_cast<Button*>(rootNode->getChildByName("btnAccept"));
    if (btn_dongy != NULL)
    {
        btn_dongy->addClickEventListener(CC_CALLBACK_1(LayerPopupXacThucOTP::onButtonDongy, this));
    }
    
    txtOTP = static_cast<TextField*>(rootNode->getChildByName("txtOTP"));
    
    Button* btnGetOTP = static_cast<Button*>(rootNode->getChildByName("btnLayOTP"));
    if (btnGetOTP){
        btnGetOTP->addClickEventListener(CC_CALLBACK_1(LayerPopupXacThucOTP::onBtnGetOTP, this));
        btnGetOTP->setPressedActionEnabled(true);
    }
    
    lblMessage = static_cast<Text*>(rootNode->getChildByName("lblMessage"));
    lblTitle = static_cast<Text*>(rootNode->getChildByName("lblTitle"));
    
    mCallBack = NULL;
    mCallBackListener = NULL;
    
    return true;
}
void LayerPopupXacThucOTP::onBtnGetOTP(Ref* pSender){
    SceneManager::getSingleton().showLoading();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("gotpcmd", params));
    GameServer::getSingleton().Send(request);
    txtOTP->setString("");
}

Button* LayerPopupXacThucOTP::getBtnHuy(){
    return btn_huy;
}

Button* LayerPopupXacThucOTP::getBtnOk(){
    return btn_dongy;
}

Text* LayerPopupXacThucOTP::getLblTitle(){
    return lblTitle;
}

bool LayerPopupXacThucOTP::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupXacThucOTP::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupXacThucOTP::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void LayerPopupXacThucOTP::onExit()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
}

void LayerPopupXacThucOTP::setMessage(string msg)
{
    this->lblTitle->setString(msg);
}

void LayerPopupXacThucOTP::onButtonHuy(Ref* pSender)
{
    //
    if (SceneLogin::isInstanceExisted())
        SceneLogin::getInstance()->setShouldAuto(true);
    //
    this->removeFromParentAndCleanup(true);
}

void LayerPopupXacThucOTP::onButtonDongy(Ref* pSender)
{
    //sample
    /*
     paramCallBack->setTag(0);
     (mCallBack->*mCallBackListener)(paramCallBack);
     */
    string otp = this->txtOTP->getString();
    if (strcmp("", otp.c_str()) == 0){
       // Chat* toast = Chat::create(dataManager.GetSysString(145),-1);
       // this->addChild(toast);
        return;
    }
    std::string* value = new std::string(otp);
    if (mCallBack && mCallBackListener){
        (mCallBack->*mCallBackListener)(NULL,(void*)value);
    }
    else{
        this->removeFromParentAndCleanup(true);
    }
    
}

void LayerPopupXacThucOTP::setCallBackFromButton(Ref* target, SEL_CallFuncND listener)
{
    mCallBack = target;
    mCallBackListener = listener;
}

void LayerPopupXacThucOTP::setContentMess(string msg)
{
    this->lblMessage->setString(msg);
}
void LayerPopupXacThucOTP::showAButton(){
    this->btn_huy->setVisible(false);
    this->btn_dongy->setPositionX(this->getContentSize().width / 2);
}

void LayerPopupXacThucOTP::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (cmd->compare("notiRMFF") == 0)
    {
        //this->scheduleOnce(schedule_selector(LayerPopupXacThucOTP::sendRequestCountMess), 2);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupXacThucOTP::sendRequestCountMess), this, 2, 1, 2, false);
    }
    else if (strcmp("otpresp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            //Chat* toast = Chat::create(dataManager.GetSysString(447),-1);
           // this->addChild(toast);
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }
}

void LayerPopupXacThucOTP::sendRequestCountMess(float dt)
{
    //delay 2s to get new mail
   // log("sendRequestCountMess");
    boost::shared_ptr<IRequest> request(new ExtensionRequest("cmi", NULL));//count msg inbox
    GameServer::getSingleton().Send(request);
}
