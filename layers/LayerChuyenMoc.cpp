//
//  LayerChuyenMoc.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerChuyenMoc.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "../layergames/_Chat_.h"
LayerChuyenMoc::LayerChuyenMoc()
{
    GameServer::getSingleton().addListeners(this);

}


LayerChuyenMoc::~LayerChuyenMoc()
{
    GameServer::getSingleton().removeListeners(this);

}

bool LayerChuyenMoc::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto rootNode = CSLoader::getInstance()->createNode("LayerChuyenMoc.csb");
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto nodeContentChuyenTien = dynamic_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    auto lblPhiTip = static_cast<Text*> (nodeContentChuyenTien->getChildByName("Text_1"));
    lblPhiTip->setString(dataManager.GetSysString(827));
    
    auto Text_3 = static_cast<Text*> (nodeContentChuyenTien->getChildByName("Text_3"));
    Text_3->setString(dataManager.GetSysString(923));
    
    auto Text_3_1 = static_cast<Text*> (nodeContentChuyenTien->getChildByName("Text_3_1"));
    Text_3_1->setString(dataManager.GetSysString(924));
    auto Text_3_2 = static_cast<Text*> (nodeContentChuyenTien->getChildByName("Text_3_2"));
    Text_3_2->setString(dataManager.GetSysString(925));
    auto Text_3_3 = static_cast<Text*> (nodeContentChuyenTien->getChildByName("Text_3_3"));
    Text_3_3->setString(dataManager.GetSysString(926));
    auto Text_3_3_0 = static_cast<Text*> (nodeContentChuyenTien->getChildByName("Text_3_3_0"));
    Text_3_3_0->setString(dataManager.GetSysString(927));
    auto Text_3_0 = static_cast<Text*> (nodeContentChuyenTien->getChildByName("Text_3_0"));
    Text_3_0->setString(dataManager.GetSysString(928));
    
    auto Text_3_0_1 = static_cast<Text*> (nodeContentChuyenTien->getChildByName("Text_3_0_1"));
    Text_3_0_1->setString(dataManager.GetSysString(929));
    auto Text_3_0_0 = static_cast<Text*> (nodeContentChuyenTien->getChildByName("Text_3_0_0"));
    Text_3_0_0->setString(dataManager.GetSysString(930));
    
    tfNhapLaiTaiKhoan = dynamic_cast<TextField*>(nodeContentChuyenTien->getChildByName("tfNhapLaiTaiKhoan"));
    txtAcc = dynamic_cast<TextField*>(nodeContentChuyenTien->getChildByName("tfTaiKhoan"));


    txtValue = dynamic_cast<TextField*>(nodeContentChuyenTien->getChildByName("tfGiaTri"));
   
    txtContent = dynamic_cast<TextField*>(nodeContentChuyenTien->getChildByName("tfLydo"));
    txtContent->setPlaceHolder(dataManager.GetSysString(825));
    txtValue->setPlaceHolder(dataManager.GetSysString(824));
    txtAcc->setPlaceHolder(dataManager.GetSysString(822));
    tfNhapLaiTaiKhoan->setPlaceHolder(dataManager.GetSysString(823));
    txtAcc->setMaxLengthEnabled(true);
    txtValue->setMaxLengthEnabled(true);
    txtAcc->setMaxLength(20);
    txtContent->setMaxLengthEnabled(true);
    txtContent->setMaxLength(200);
    txtValue->setMaxLength(9);
    lblMoneyPhi = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("lblTienPhi"));
    lblPhi = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("lblPhi"));
    txtMinTrade = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("txtMin"));
    if (txtMinTrade)
        txtMinTrade->setString(StringUtils::format("%s %s", mUtils::format_money((long)SceneManager::getSingleton().getMinMoney(3))->c_str(),SceneManager::getSingleton().getCoin(0).c_str()));
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    txtValue->setInputMode(ui::EditBox::InputMode::NUMERIC);
#endif

    btnReceiver = dynamic_cast<Button*>(nodeContentChuyenTien->getChildByName("btnChuyen"));
    if (btnReceiver != NULL){
        btnReceiver->addClickEventListener(CC_CALLBACK_1(LayerChuyenMoc::onBtnReceiverClicked, this));
        btnReceiver->setPressedActionEnabled(true);
        btnReceiver->setTitleText(dataManager.GetSysString(826));

    }
    
    //validate
    lblErrorNick = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("txtErrorTK"));
    lblErrorNick->setVisible(false);
    lblErrorValue = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("txtErrorGiaTri"));
    lblErrorValue->setVisible(false);
    lblErrorContent = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("txtErrorLyDo"));
    lblErrorContent->setVisible(false);

    txtErrorNhapTK = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("txtErrorNhapTK"));
    txtErrorNhapTK->setVisible(false);
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerChuyenMoc::onScheduleUpdateCost),this,1, false);

    //this->txtChuChay->setScale(5);
    return true;
}
void LayerChuyenMoc::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    
    CCLOG("REceiver call function");
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    CCLOG("onExtentions");
    LayerPopupThongBao* layerThongBao = LayerPopupThongBao::create();
    layerThongBao->showAButton();
    layerThongBao->setMessage(dataManager.GetSysString(141));
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene) return;
    if (strcmp("tfbalance", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        int rc = *param->GetInt("rc");
        auto rd = *param->GetUtfString("rd");
        if (rc == 0) {
            layerThongBao->setContentMess(dataManager.GetSysString(563));
            scene->addChild(layerThongBao,2000);
            this->txtAcc->setString("");
            this->txtContent->setString("");
            this->txtValue->setString("");
            this->tfNhapLaiTaiKhoan->setString("");
        }
        else {
            layerThongBao->setMessage(dataManager.GetSysString(685));
            layerThongBao->setContentMess(rd + "");
            scene->addChild(layerThongBao,2000);
        }
    }	else if (strcmp("respcf", cmd->c_str()) == 0){ //RESP_GET_CONFIG = "respcf";
        mTax = *param->GetDouble("cfv");//CONFIG_VALUE = "cfv";
        //
        lblPhi->setString(StringUtils::format("%0.0lf ", mTax) + "%");
        //recalculator
        onScheduleUpdateCost(1.0);
        //
        SceneManager::getSingleton().hideLoading();
    }
}
void LayerChuyenMoc::onScheduleUpdateCost(float dt)
{
    if (txtValue != NULL && txtValue->getString().length() == 0 || mUtils::findContainChar(txtValue->getString().c_str())
        || atoi(txtValue->getString().c_str()) < 10000 || atoi(txtValue->getString().c_str()) > 1000000000){
        lblMoneyPhi->setString("--");
        return;
    }
    long currMoney = atol(txtValue->getString().c_str());
    lblMoneyPhi->setString(*mUtils::format_money(currMoney*mTax / 100));
}
void LayerChuyenMoc::onPopupCallBack(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    _currScene->removeChildByTag(24);
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    int am = atoi(txtValue->getString().c_str());
    
    if (am < 0)
        return;
    //params->PutUtfString("aN", myself->GetVariable("aN")->GetStringValue());
    params->PutUtfString("aI", myself->Name());
    //params->PutUtfString("aR", txtAcc->getString());
    params->PutUtfString("adnR", txtAcc->getString());
    //string content = txtContent->getString();
    params->PutUtfString("ct", txtContent->getString());
    params->PutInt("am", am);
    params->PutInt("ppf", 1);
    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("tfbalance", params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}
void LayerChuyenMoc::onBtnReceiverClicked(Ref* psender){
    
    
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    if (myself->GetVariable("amf") == NULL)
        return;
    
    CCLOG("onBUttonReceiverClicked");
    string inputName = txtAcc->getString();
    std::transform(inputName.begin(), inputName.end(), inputName.begin(), ::tolower);
    
    //validate
    if (txtAcc->getString() == ""){
        lblErrorNick->setString("* "+dataManager.GetSysString(427));
        lblErrorNick->setVisible(true);
        return;
    }
    else{
        lblErrorNick->setVisible(false);
    }
    if (tfNhapLaiTaiKhoan->getString() == ""){
        txtErrorNhapTK->setString("* "+dataManager.GetSysString(719));
        txtErrorNhapTK->setVisible(true);
        return;
    }
    else{
        txtErrorNhapTK->setVisible(false);
    }
    if (tfNhapLaiTaiKhoan->getString() != txtAcc->getString()){
        txtErrorNhapTK->setString("* "+dataManager.GetSysString(720));
        txtErrorNhapTK->setVisible(true);
        return;
    }
    else{
        txtErrorNhapTK->setVisible(false);
    }
    if (txtValue->getString() == ""){
        lblErrorValue->setString("* "+dataManager.GetSysString(721));
        lblErrorValue->setVisible(true);
        return;
    }
    else if (mUtils::findContainChar(txtValue->getString().c_str())){
        lblErrorValue->setString(dataManager.GetSysString(429));
        lblErrorValue->setVisible(true);
        return;
    }
    else if (atoi(txtValue->getString().c_str()) > *myself->GetVariable("amf")->GetDoubleValue()){
        lblErrorValue->setString(dataManager.GetSysString(652));
        lblErrorValue->setVisible(true);
        return;
    }
    else if (atoi(txtValue->getString().c_str()) < SceneManager::getSingleton().getMinMoney(3)){
        lblErrorValue->setString(StringUtils::format("%s %s!",dataManager.GetSysString(431).c_str(), mUtils::format_money((long)SceneManager::getSingleton().getMinMoney(3))->c_str()));
        lblErrorValue->setVisible(true);
        return;
    }
    else if (atoi(txtValue->getString().c_str()) > 1000000000){
        lblErrorValue->setString(dataManager.GetSysString(432));
        lblErrorValue->setVisible(true);
        return;
    }
    
    
    else if (strcmp(inputName.c_str(), ChanUtils::GetNamePlayer(SceneManager::getSingleton().getMyName()).c_str()) == 0)
    {
        lblErrorNick->setString("* "+dataManager.GetSysString(722));
        lblErrorNick->setVisible(true);
        return;
    }
    
    else if ((*myself->GetVariable("amf")->GetDoubleValue() - (atol(txtValue->getString().c_str()) + atol(lblMoneyPhi->getString().c_str()))) < SceneManager::getSingleton().getMinMoneyTranfer()){
        lblErrorValue->setString(StringUtils::format("* %s %s ",dataManager.GetSysString(723).c_str(), mUtils::format_money((long)SceneManager::getSingleton().getMinMoneyTranfer())->c_str()));
        lblErrorValue->setVisible(true);
        return;
    }
    else{
        lblErrorValue->setVisible(false);
    }
    
    if (myself->GetVariable("accVipType") == NULL || myself->GetVariable("amf") == NULL) return;
    
    int accVipType = *myself->GetVariable("accVipType")->GetIntValue();
    if (accVipType < SceneManager::getSingleton().getMinVipTranfer()) {
        Chat *toast = Chat::create(StringUtils::format("%s %d %s",dataManager.GetSysString(425).c_str(), SceneManager::getSingleton().getMinVipTranfer(),dataManager.GetSysString(23).c_str()), -1);
        this->addChild(toast);
        return;
    }
    
    if (txtContent->getString() == ""){
        lblErrorContent->setString("* "+dataManager.GetSysString(724));
        lblErrorContent->setVisible(true);
        return;
    }else{
        lblErrorContent->setVisible(false);
    }

    
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    
    
    LayerPopupThongBao* layerpoup = LayerPopupThongBao::create();
    layerpoup->setMessage(dataManager.GetSysString(141));
    layerpoup->setContentMess(StringUtils::format("%s \n %s (%s = %s) \n %s %s ?",dataManager.GetSysString(517).c_str(),
                                                  mUtils::convertMoneyEx(atof(txtValue->getString().c_str())).c_str(),dataManager.GetSysString(518).c_str(), lblMoneyPhi->getString().c_str(),dataManager.GetSysString(520).c_str(), txtAcc->getString().c_str()));
    layerpoup->setCallBackFromButton(this, callfuncO_selector(LayerChuyenMoc::onPopupCallBack), 1);
    layerpoup->setTag(24);
    _currScene->addChild(layerpoup, 2000,9080);

    

    
   

}
void LayerChuyenMoc::hideLayer(){
    this->setVisible(false);
}
void LayerChuyenMoc::showLayer(){
    this->setVisible(true);
    SceneManager::getSingleton().showLoading();
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    params->PutInt("cfk", 1);//CONFIG_KEY = "cfk"; 1: thuế chuyển tiền giữa các accounts
    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("reqgcf", params)); //REQUEST_GET_CONFIG = "reqgcf";
    GameServer::getSingleton().Send(request);

}
bool LayerChuyenMoc::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerChuyenMoc::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerChuyenMoc::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerChuyenMoc::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

