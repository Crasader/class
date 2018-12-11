//
//  LayerDoiThe.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerDoiThe.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "Requests/ExtensionRequest.h"
#include "LayerPopupThongBao.h"
#include "LayerGuideInGame.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
LayerDoiThe::LayerDoiThe()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerDoiThe::~LayerDoiThe()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerDoiThe::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerDoiTien.csb");
    
    
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    auto pnlContent = dynamic_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    this->_layerContent = pnlContent;
    auto txtNhan = static_cast<Text*> (pnlContent->getChildByName("txtNhan"));
    txtNhan->setString(dataManager.GetSysString(1041));
    auto txtNhanTien = static_cast<Text*> (pnlContent->getChildByName("txtNhanTien"));
    txtNhanTien->setString(dataManager.GetSysString(999));

    auto txtLabel1 = static_cast<Text*> (pnlContent->getChildByName("txtLabel1"));
    txtLabel1->setString(dataManager.GetSysString(784));
    
    this->txtWingAccount = static_cast<TextField*> (pnlContent->getChildByName("txtWingAccount"));
    this->txtWingAccount->setPlaceHolder(dataManager.GetSysString(784));
    this->txtWingAccount->setInputMode(ui::EditBox::InputMode::NUMERIC);

    this->txtErrorWingAccount = static_cast<Text*> (pnlContent->getChildByName("txtErrorWingAccount"));
    this->txtErrorWingAccount->setVisible(false);
    
    auto btnDoiThe = dynamic_cast<Button*>(pnlContent->getChildByName("btnDoiThe"));
    if (btnDoiThe != NULL){
        btnDoiThe->addClickEventListener(CC_CALLBACK_1(LayerDoiThe::onBtnDoiThe, this));
        btnDoiThe->setTitleText(dataManager.GetSysString(1040));
    }
 
    
    this->btnSmart= dynamic_cast<Button*>(pnlContent->getChildByName("btnSmart"));
    if (this->btnSmart != NULL){
        this->btnSmart->addClickEventListener(CC_CALLBACK_1(LayerDoiThe::onBtnNhaMang, this));
        this->btnSmart->setTag(SMART);
    }
    this->btnMetPhone= dynamic_cast<Button*>(pnlContent->getChildByName("btnMetPhone"));
    if (this->btnMetPhone != NULL){
        this->btnMetPhone->addClickEventListener(CC_CALLBACK_1(LayerDoiThe::onBtnNhaMang, this));
        this->btnMetPhone->setTag(MET_FONE);
    }
    this->btnCellCard= dynamic_cast<Button*>(pnlContent->getChildByName("btnCellCard"));
    if (this->btnCellCard != NULL){
        this->btnCellCard->addClickEventListener(CC_CALLBACK_1(LayerDoiThe::onBtnNhaMang, this));
        this->btnCellCard->setTag(CELL_CARD);
    }
    
    this->txtMoneyNeed = dynamic_cast<Text*>(pnlContent->getChildByName("txtZenCanCo"));
    this->txtMoneyNeed->setString("");
    
    
    
    auto pnlTyGia = dynamic_cast<Layout*>(rootNode->getChildByName("pnlTyGia"));
    pnlTyGia->setVisible(false);
    
    this->pnlMenhGia = dynamic_cast<Layout*>(pnlContent->getChildByName("pnlMenhGia"));
    this->pnlMenhGia->setVisible(false);

    this->scrollTyGia = ui::ScrollView::create();
    this->scrollTyGia->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollTyGia->setClippingEnabled(true);
    this->scrollTyGia->setContentSize(this->pnlMenhGia->getContentSize());
    this->scrollTyGia->setAnchorPoint(this->pnlMenhGia->getAnchorPoint());
    this->scrollTyGia->setPosition(this->pnlMenhGia->getPosition());
    this->scrollTyGia->setScrollBarEnabled(false);
    this->scrollTyGia->setBounceEnabled(false);
    this->scrollTyGia->setTag(129323);
    this->scrollTyGia->setInnerContainerSize(Size(734, pnlTyGia->getContentSize().height));
    this->addChild(this->scrollTyGia);
    
    
    auto txtNhanLuuY = static_cast<Text*> (pnlContent->getChildByName("txtNhanLuuY"));
    txtNhanLuuY->setString(dataManager.GetSysString(1039));
    txtNhanLuuY->setVisible(false);
    auto txtLuuy1 = static_cast<Text*> (pnlContent->getChildByName("txtLuuy1"));
    txtLuuy1->setString(dataManager.GetSysString(1042));
    txtLuuy1->setVisible(false);

    auto txtLuuy2 = static_cast<Text*> (pnlContent->getChildByName("txtLuuy2"));
    txtLuuy2->setString(dataManager.GetSysString(1043));
    txtLuuy2->setVisible(false);

    auto txtLuuy3 = static_cast<Text*> (pnlContent->getChildByName("txtLuuy3"));
    txtLuuy3->setString(dataManager.GetSysString(1044));
    txtLuuy3->setVisible(false);

    
    
    return true;
}

void LayerDoiThe::onBtnNhaMang(Ref* pSender){
    Button* btn = (Button*)pSender;
    if (btn->getTag() == CELL_CARD){
        this->btnSmart->setEnabled(true);
        this->btnSmart->setBright(true);

        this->btnMetPhone->setEnabled(true);
        this->btnMetPhone->setBright(true);
        
        this->btnCellCard->setEnabled(false);
        this->btnCellCard->setBright(false);
    }else if (btn->getTag() == MET_FONE){
        this->btnSmart->setEnabled(true);
        this->btnSmart->setBright(true);
        
        this->btnMetPhone->setEnabled(false);
        this->btnMetPhone->setBright(false);
        
        this->btnCellCard->setEnabled(true);
        this->btnCellCard->setBright(true);
    }else if (btn->getTag() == SMART){
        this->btnSmart->setEnabled(false);
        this->btnSmart->setBright(false);
        
        this->btnMetPhone->setEnabled(true);
        this->btnMetPhone->setBright(true);
        
        this->btnCellCard->setEnabled(true);
        this->btnCellCard->setBright(true);
    }
    this->typeNhamang = btn->getTag();
    this->loadTyGia();
}
void LayerDoiThe::onMenhGia(Ref* pSender){
    Button* btn = (Button*)pSender;
    for (int i = 0 ; i < this->lstMenhGia.size();i++){
        this->lstMenhGia[i]->setEnabled(true);
        this->lstMenhGia[i]->setBright(true);
        
    }
    btn->setEnabled(false);
    btn->setBright(false);
    this->txtMoneyNeed->setString(mUtils::convertMoneyEx(this->the[btn->getTag()].money));
    this->currMenhgia = this->the[btn->getTag()].menhgia;
    this->currMoney = this->the[btn->getTag()].money;
}
void LayerDoiThe::hideLayer(){
    this->setVisible(false);
    
}
void LayerDoiThe::showLayer(){
    this->setVisible(true);
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_INFO_CARD_REQ,params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
}
void LayerDoiThe::loadTyGia(){
    this->the.clear();
    auto lst = mUtils::splitString(this->listcard, '/');
    for (int i = 0; i < lst.size();i++){
        auto nhamang = mUtils::splitString(lst[i], '|');
        if (nhamang.size()<2) return;
        if (atoi(nhamang[0].c_str()) == this->typeNhamang){
            auto menhgia = mUtils::splitString(nhamang[1], ';');
            for (int j = 0; j < menhgia.size();j++){
                auto it =  mUtils::splitString(menhgia[j], '-');
                if (it.size()<2) continue;
                mathe mt;
                mt.menhgia = atoi(it[0].c_str());
                mt.money = atoi(it[1].c_str());
                this->the.push_back(mt);
            }
        }
    }
    this->scrollTyGia->setInnerContainerSize(Size(734,230));
    this->scrollTyGia->removeAllChildren();
    this->lstMenhGia.clear();
    int col = 3;
    for (int i =0;i<this->the.size();i++){
//        ItemTyGiaNapZen* item = ItemTyGiaNapZen::create();
//        item->setAnchorPoint(Vec2(0,1));
//        item->setDatas(i,this->the[i],this->the[i]*this->mTax);
//        this->scrollTyGia->addChild(item);
//        item->setPosition(Vec2(0,470 - (i+1) * 79));
        auto button = ui::Button::create();
        button->loadTextures("design/guifiles/muc-gia-nor-min.png", "design/guifiles/muc-gia-act-min.png", "design/guifiles/muc-gia-act-min.png");
        button->setAnchorPoint(Point(0, 1));
        button->setContentSize(Size(259, 146));
        button->setTitleText(StringUtils::format("%d",this->the[i].menhgia) + " $" );
        button->setTitleFontSize(50);
        //button->setTitleFontName("__Roboto-Regular.ttf");
        button->addClickEventListener(CC_CALLBACK_1(LayerDoiThe::onMenhGia, this));
        //button->setTitleColor(ccc3(255, 255, 255));
        button->setTag(i);
        /*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
        button->setPosition(Vec2(0 + (i%col)*229,230 - 106 * (i/col)-5));
        this->scrollTyGia->addChild(button);
        //this->scrollTyGia->setVisible(false);
        this->lstMenhGia.push_back(button);
    }
    if (this->lstMenhGia.size()>0)
        this->onMenhGia(this->lstMenhGia[0]);
}
void LayerDoiThe::popUpCallBack(Ref* pSender){
    
   
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    _currScene->removeChildByTag(24);
    int osid = 1;
    string devID = "";
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
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("aN", myself->Name());
    params->PutUtfString("issuer", this->txtWingAccount->getString());
    params->PutInt("vv", this->currMenhgia);
    params->PutInt("vnc", 1);
    params->PutUtfString("voucherDesc", "");
    params->PutInt("osid", osid);
    params->PutInt("vctype", 1);

    
    params->PutUtfString("devid", devID);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rgb", params));
    GameServer::getSingleton().Send(request);
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    //NativeHelper::trackDoiThuong(StringUtils::format("%d", this->currMenhgia));
//#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::trackDoiThuong(StringUtils::format("%d",this->currMenhgia));
#endif
    SceneManager::getSingleton().showLoading();
    
}
void LayerDoiThe::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene) return;
   // log("OnExtensionResponse %s", cmd->c_str());
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    if (strcmp(EXT_EVENT_GET_INFO_CARD_RESP, cmd->c_str()) == 0){//OK
        SceneManager::getSingleton().hideLoading();
        this->smart = true;
        this->metphone = true;
        this->cellcard = true;
        std::string str = " ";
        boost::shared_ptr<string> ei = param->GetUtfString(EXT_INFO);
        if (ei){
            this->listcard = *ei;
            auto lst = mUtils::splitString(this->listcard, '/');
            for (int i = 0; i < lst.size();i++){
                auto nhamang = mUtils::splitString(lst[i], '|');
                if (nhamang.size()<2){
                    switch (atoi(nhamang[0].c_str())){
                        case CELL_CARD:{
                            this->btnCellCard->setVisible(false);
                            this->cellcard= false;
                            auto button = ui::Button::create();
                            button->loadTextures("cellcard3.png", "cellcard3.png", "cellcard3.png");
                            button->setAnchorPoint(this->btnCellCard->getAnchorPoint());
                            button->setContentSize(this->btnCellCard->getContentSize());
                            button->setTitleText(dataManager.GetSysString(1045));
                            button->setPosition(this->btnCellCard->getPosition());
                            this->_layerContent->addChild(button);
                            break;
                        }
                        case MET_FONE:{
                            this->btnMetPhone->setVisible(false);
                            this->metphone = false;
                            auto button = ui::Button::create();
                            button->loadTextures("metphone3.png", "metphone3.png", "metphone3.png");
                            button->setAnchorPoint(this->btnMetPhone->getAnchorPoint());
                            button->setContentSize(this->btnMetPhone->getContentSize());
                            button->setTitleText(dataManager.GetSysString(1045));
                            button->setPosition(this->btnMetPhone->getPosition());
                            this->_layerContent->addChild(button);
                            break;
                        }
                        case SMART:{
                            this->btnSmart->setVisible(false);
                            this->smart = false;
                            auto button = ui::Button::create();
                            button->loadTextures("smart3.png", "smart3.png", "smart3.png");
                            button->setAnchorPoint(this->btnSmart->getAnchorPoint());
                            button->setContentSize(this->btnSmart->getContentSize());
                            button->setTitleText(dataManager.GetSysString(1045));
                            button->setPosition(this->btnSmart->getPosition());
                            this->_layerContent->addChild(button);
                            break;
                        }
                            
                    }
                }
                else{
                    if (nhamang[1] == ""){
                        switch (atoi(nhamang[0].c_str())){
                            case CELL_CARD:{
                                this->btnCellCard->setVisible(false);
                                this->cellcard= false;
                                auto button = ui::Button::create();
                                button->loadTextures("cellcard3.png", "cellcard3.png", "cellcard3.png");
                                button->setAnchorPoint(this->btnCellCard->getAnchorPoint());
                                button->setContentSize(this->btnCellCard->getContentSize());
                                button->setTitleText(dataManager.GetSysString(1045));
                                button->setPosition(this->btnCellCard->getPosition());
                                this->_layerContent->addChild(button);
                                break;
                            }
                            case MET_FONE:{
                                this->btnMetPhone->setVisible(false);
                                this->metphone = false;
                                auto button = ui::Button::create();
                                button->loadTextures("metphone3.png", "metphone3.png", "metphone3.png");
                                button->setAnchorPoint(this->btnMetPhone->getAnchorPoint());
                                button->setContentSize(this->btnMetPhone->getContentSize());
                                button->setTitleText(dataManager.GetSysString(1045));
                                button->setPosition(this->btnMetPhone->getPosition());
                                this->_layerContent->addChild(button);
                                break;
                            }
                            case SMART:{
                                this->btnSmart->setVisible(false);
                                this->smart = false;
                                auto button = ui::Button::create();
                                button->loadTextures("smart3.png", "smart3.png", "smart3.png");
                                button->setAnchorPoint(this->btnSmart->getAnchorPoint());
                                button->setContentSize(this->btnSmart->getContentSize());
                                button->setTitleText(dataManager.GetSysString(1045));
                                button->setPosition(this->btnSmart->getPosition());
                                this->_layerContent->addChild(button);
                                break;
                            }
                                
                        }
                    }
                }
            }
        }
        if (this->smart){
            this->onBtnNhaMang(this->btnSmart);
        }
        else if (this->metphone){
            this->onBtnNhaMang(this->btnMetPhone);

        }
        else if(this->cellcard){
            this->onBtnNhaMang(this->btnCellCard);
        }
    }
    else if (strcmp("tgbresp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        int rc = *param->GetInt("rc");
        auto rd = *param->GetUtfString("rd");
        if (rc == 0) {
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(dataManager.GetSysString(1047));
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,3000,9020);
        }
        else {
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(rd + "");
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,3000,9021);
            
        }
    }
 
}

void LayerDoiThe::onBtnDoiThe(Ref* pSender){
//    string strNhaMang = "";
//    switch (this->typeNhamang ){
//        case CELL_CARD:
//            strNhaMang = "CellCard";
//            break;
//        case SMART:
//            strNhaMang = "Smart";
//            break;
//        case MET_FONE:
//            strNhaMang = "MetFone";
//            break;
//    }
    if (mUtils::findContainChar(this->txtWingAccount->getString().c_str())){
        CCLOG("loi nhap so");
        this->txtErrorWingAccount->setVisible(true);
        this->txtErrorWingAccount->setString("* "+dataManager.GetSysString(650));
        return;
    }
    else{
        this->txtErrorWingAccount->setVisible(false);
        
    }
    if (strlen(this->txtWingAccount->getString().c_str()) == 0){
        this->txtErrorWingAccount->setVisible(true);
        this->txtErrorWingAccount->setString("* "+dataManager.GetSysString(949));
        return;
    }
    else{
        this->txtErrorWingAccount->setVisible(false);
    }
    if (mUtils::findSpecialCharSpace(this->txtWingAccount->getString())){
        this->txtErrorWingAccount->setVisible(true);
        this->txtErrorWingAccount->setString(dataManager.GetSysString(186));
        return;
    }
    else{
        this->txtErrorWingAccount->setVisible(false);
        
    }
    if (this->txtWingAccount->getString().length() > 50){
        this->txtErrorWingAccount->setVisible(true);
        this->txtErrorWingAccount->setString("* "+dataManager.GetSysString(957));
        return;
    }
    else{
        this->txtErrorWingAccount->setVisible(false);
    }
    
    
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;

    LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
    layerPopup->setCallBackFromButton(this, callfuncO_selector(LayerDoiThe::popUpCallBack), 5);
    layerPopup->setMessage(dataManager.GetSysString(141));
    //layerPopup->setContentMess(StringUtils::format("%s %s %s %s %s %s $ %s",dataManager.GetSysString(653).c_str(), mUtils::format_money(this->currMoney)->c_str(), SceneManager::getSingleton().getCoin(0).c_str(),dataManager.GetSysString(654).c_str(), strNhaMang.c_str(), mUtils::format_money(this->currMenhgia)->c_str(),dataManager.GetSysString(144).c_str()));
    layerPopup->setContentMess(StringUtils::format("%s %s %s %s %s $ %s",dataManager.GetSysString(653).c_str(), mUtils::format_money(this->currMoney)->c_str(), SceneManager::getSingleton().getCoin(0).c_str(),dataManager.GetSysString(654).c_str(), mUtils::format_money(this->currMenhgia)->c_str(),dataManager.GetSysString(144).c_str()));

    layerPopup->setTag(24);
    _currScene->addChild(layerPopup,2000,9022);
}

void LayerDoiThe::onEnter()
{
    Layer::onEnter();
  
}

void LayerDoiThe::onExit()
{
    Layer::onExit();
  
}

