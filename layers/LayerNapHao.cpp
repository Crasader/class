//
//  LayerNapHao.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/24/17.
//
//

#include "LayerNapHao.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupThongBao.h"
#include "../data_manager.h"
#include "../layergames/_Chat_.h"
LayerNapHao::LayerNapHao()
{
    GameServer::getSingleton().addListeners(this);
}


LayerNapHao::~LayerNapHao()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerNapHao::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerNapHao.csb");
//    rootNode->setAnchorPoint(Point(0.5, 0.5));
//    rootNode->setPosition(Point(visibleSize / 2));
//    Size sizeAdd;
//    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
//        sizeAdd = ChanUtils::getSizePos();
//        
//    }else{
//        sizeAdd = Size(0,0);
//    }
//    Size sizeAdd2;
//    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
//        sizeAdd2 = ChanUtils::getSizePos();
//        
//    }else{
//        sizeAdd2 = Size(0,0);
//    }
//    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    auto lblSDM = static_cast<Text*> (pnlBg->getChildByName("Text_3"));
    lblSDM->setString(dataManager.GetSysString(840));
    auto lblSHND = static_cast<Text*> (pnlBg->getChildByName("Text_3_0"));
    lblSHND->setString(dataManager.GetSysString(841));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerNapHao::onBtnClose, this));
    }
    
    this->txtGold = static_cast<Text*>(pnlBg->getChildByName("txtGold"));
    this->txtSilver= static_cast<Text*>(pnlBg->getChildByName("txtSilver"));
    this->txtSilver->setString("---");
    this->tfGold= static_cast<TextField*>(pnlBg->getChildByName("tfGold"));
    this->tfGold->setPlaceHolder(dataManager.GetSysString(842));
    Button* btnDoiTien = dynamic_cast<Button*>(pnlBg->getChildByName("btnDoi"));
    if(btnDoiTien){
        btnDoiTien->addClickEventListener(CC_CALLBACK_1(LayerNapHao::onBtnDoiTien, this));
        btnDoiTien->setTitleText(dataManager.GetSysString(754));

    }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    this->tfGold->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->tfGold->setMaxLength(9);
#endif

    
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return true;
    if (myself->GetVariable("amf") == NULL)
        return true;
    auto amf = myself->GetVariable("amf")->GetDoubleValue();
    if (amf == NULL) return true;
    this->txtGold->setString(mUtils::convertMoneyEx(*amf).c_str());
    this->myGold = *amf;
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerNapHao::onScheduleUpdateCost),this,1, false);
        return true;
}
void LayerNapHao::hideLayer(){
    this->tfGold->setEnableEditBox(false);
    this->setVisible(false);
}
void LayerNapHao::showLayer(){
    this->tfGold->setEnableEditBox(true);
    this->setVisible(true);

}
void LayerNapHao::onBtnDoiTien(Ref* pSender){

    
    if (mUtils::findContainChar(this->tfGold->getString().c_str())) {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(137));
        return;
    }

    if (this->tfGold->getString() == "") {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(138));
        return;
    }
    
    if (this->myGold < atoi(this->tfGold->getString().c_str())) {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(1136));
        return;
    }
    
    
    int tientra = atoi(this->tfGold->getString().c_str());
    
    if (tientra < 0){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(139));
        return;
    }
    
    if (tientra == 0){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(140));
        return;
    }
    
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    
    auto layerpoup = LayerPopupThongBao::create();
    layerpoup->setMessage(dataManager.GetSysString(141));
    layerpoup->setContentMess(StringUtils::format("%s %s %s %s %s %s ?",dataManager.GetSysString(142).c_str(),this->tfGold->getString().c_str(),SceneManager::getSingleton().getCoin(0).c_str(),dataManager.GetSysString(143).c_str(),this->txtSilver->getString().c_str(),SceneManager::getSingleton().getCoin(1).c_str(),dataManager.GetSysString(144).c_str()));
    layerpoup->setCallBackFromButton(this, callfuncO_selector(LayerNapHao::onPopupCallBack), 1);
    layerpoup->setTag(24);
    _currScene->addChild(layerpoup, 10000,9076);
}

void LayerNapHao::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerNapHao::onScheduleUpdateCost(float dt)
{
    if (this->tfGold != NULL && this->tfGold->getString().length() == 0 || mUtils::findContainChar(this->tfGold->getString().c_str())){
        this->txtSilver->setString("--");
        return;
    }
    long currMoney = atol(this->tfGold->getString().c_str());
    this->txtSilver->setString(*mUtils::format_money(currMoney*mTax));
    //lblSumMoney->setString(*mUtils::format_money(currMoney + currMoney*mTax / 100));
}
void LayerNapHao::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_EXCHANGE_GOLD2SILVER_RESP, cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("rc") == NULL) return;
        int rc = *param->GetInt("rc");
        
        if (rc == 0){
            if (param->GetInt(REAL_RECEIVED_SILVER) == NULL) return;
            int rv = *param->GetInt(REAL_RECEIVED_SILVER);
            
            
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(dataManager.GetSysString(524));
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,1000000,9077);
            this->tfGold->setString("");
            
        }
        else{
            if (param->GetUtfString("rd") == NULL) return;
            string rd = *param->GetUtfString("rd");
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(rd);
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,1000000,9078);
        }
    }
    else if (strcmp("respcf", cmd->c_str()) == 0){ //RESP_GET_CONFIG = "respcf";
        this->mTax = *param->GetDouble("cfv");//CONFIG_VALUE = "cfv";
        //
        SceneManager::getSingleton().hideLoading();
    }
    
}
void LayerNapHao::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    
    if (ptrNotifiedUser->IsItMe() == true){
        boost::shared_ptr<double> amf;
        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
        if (myself == NULL)
            return;
        if (myself->GetVariable("amf") == NULL)
            return;
        amf = myself->GetVariable("amf")->GetDoubleValue();
        if (amf == NULL) return;
        this->txtGold->setString(mUtils::convertMoneyEx(*amf).c_str());
        this->myGold = *amf;
    }
}

bool LayerNapHao::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerNapHao::onEnter()
{
    Layer::onEnter();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = CC_CALLBACK_2(LayerNapHao::onTouchBegan, this);
//    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerNapHao::onExit()
{
    Layer::onExit();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
void LayerNapHao::onPopupCallBack(Ref *pSender)
{
    auto _currScene = Director::getInstance()->getRunningScene();
    if (_currScene)
        _currScene->removeChildByTag(24);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt(EXCHANGE_GOLD_VALUE,atoi(this->tfGold->getString().c_str()));
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_EXCHANGE_GOLD2SILVER_REQ,params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}
