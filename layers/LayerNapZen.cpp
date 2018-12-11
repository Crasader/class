//
//  LayerNapZen.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerNapZen.h"
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
LayerNapZen::LayerNapZen()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerNapZen::~LayerNapZen()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerNapZen::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerNapZen.csb");
    
    
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    auto pnlContent = dynamic_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    auto lblMG = static_cast<Text*> (rootNode->getChildByName("Text_2"));
    lblMG->setString(dataManager.GetSysString(815));
    auto lblQD = static_cast<Text*> (rootNode->getChildByName("Text_2_0"));
    lblQD->setString(dataManager.GetSysString(816));
    this->btnMenhGia = dynamic_cast<Button*>(pnlContent->getChildByName("btnMenhGia"));
    if (this->btnMenhGia != NULL){
        this->btnMenhGia->addClickEventListener(CC_CALLBACK_1(LayerNapZen::onBtnMenhGia, this));
        //this->btnNhaMang->setPressedActionEnabled(true);
    }
    auto btnNapThe = dynamic_cast<Button*>(pnlContent->getChildByName("btnNapThe"));
    if (btnNapThe != NULL){
        btnNapThe->addClickEventListener(CC_CALLBACK_1(LayerNapZen::onBtnNapThe, this));
        //this->btnMobi->setPressedActionEnabled(true);
        btnNapThe->setTitleText(dataManager.GetSysString(790));
    }
    
    auto btnGetOTP = dynamic_cast<Button*>(pnlContent->getChildByName("btnGetOTP"));
    if (btnGetOTP != NULL){
        btnGetOTP->addClickEventListener(CC_CALLBACK_1(LayerNapZen::onBtnGetOTP, this));
        //this->btnMobi->setPressedActionEnabled(true);
        btnGetOTP->setTitleText(dataManager.GetSysString(948));
    }
    
    
    
    this->txtWingAccount = dynamic_cast<TextField*>(pnlContent->getChildByName("txtWingAccount"));
    this->txtWingAccount->setPlaceHolder(dataManager.GetSysString(944));
    this->txtWingAccount->setMaxLength(50);
   
    if (SceneManager::getSingleton().getBankAccountByType(1).length()>0 || SceneManager::getSingleton().getBankAccountByType(1)!=""){
        this->txtWingAccount->setString(SceneManager::getSingleton().getBankAccountByType(1));
    }
    
    
    this->txtPhoneNumber = dynamic_cast<TextField*>(pnlContent->getChildByName("txtPhoneNumber"));
    this->txtPhoneNumber->setPlaceHolder(dataManager.GetSysString(945));
    this->txtPhoneNumber->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->txtPhoneNumber->setMaxLength(30);
   
    //    this->txtUserName = dynamic_cast<TextField*>(pnlContent->getChildByName("txtUserName"));
    //    this->txtUserName->setPlaceHolder(dataManager.GetSysString(946));
    this->txtOTP = dynamic_cast<TextField*>(pnlContent->getChildByName("txtOTP"));
    this->txtOTP->setPlaceHolder(dataManager.GetSysString(947));
    auto txtLabel3 = dynamic_cast<Text*>(pnlContent->getChildByName("txtLabel3"));
    txtLabel3->setString(dataManager.GetSysString(947));
    this->txtErrorWingAccount = dynamic_cast<Text*>(pnlContent->getChildByName("txtErrorWingAccount"));
    this->txtErrorWingAccount->setVisible(false);
    this->txtErrorPhoneNumber = dynamic_cast<Text*>(pnlContent->getChildByName("txtErrorPhoneNumber"));
    this->txtErrorPhoneNumber->setVisible(false);
    //    this->txtErrorUserName = dynamic_cast<Text*>(pnlContent->getChildByName("txtErrorUserName"));
    //    this->txtErrorUserName->setVisible(false);
    this->txtErrorOTP = dynamic_cast<Text*>(pnlContent->getChildByName("txtErrorOTP"));
    this->txtErrorOTP->setVisible(false);
    
    this->txtTienNhan = dynamic_cast<Text*>(pnlContent->getChildByName("txtTienNhan"));
    this->txtTienNhan->setVisible(false);
    auto txtLabel1 = dynamic_cast<Text*>(pnlContent->getChildByName("txtLabel1"));
    txtLabel1->setString(dataManager.GetSysString(944));
    auto txtLabel2 = dynamic_cast<Text*>(pnlContent->getChildByName("txtLabel2"));
    txtLabel2->setString(dataManager.GetSysString(945));
    auto txtNhan = dynamic_cast<Text*>(pnlContent->getChildByName("txtNhan"));
    txtNhan->setString(dataManager.GetSysString(1024));
    
    auto txtNhan_0 = dynamic_cast<Text*>(pnlContent->getChildByName("txtNhan_0"));
    txtNhan_0->setString(dataManager.GetSysString(1025));
    this->txtKhuyenMai = dynamic_cast<Text*>(pnlContent->getChildByName("txtTienNhan_0"));
    this->txtKhuyenMai->setVisible(false);
    
    //this->txtChuChay->setScale(5);
    
    Size sizeAdd;
    //  if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
    sizeAdd = ChanUtils::getSizePos();
    
    //    }else{
    //        sizeAdd = Size(0,0);
    //    }
    //
    
    
    auto pnlTyGia = dynamic_cast<Layout*>(rootNode->getChildByName("pnlTyGia"));
    pnlTyGia->setVisible(false);
    
    auto lst = mUtils::splitString(dataManager.getAppConfig().exchangerate,'/');
    if (lst.size()>=2){
        auto lstMenhGia = mUtils::splitString(lst[1],';');
        for (int i = 0 ; i < lstMenhGia.size();i++){
            auto it = mUtils::splitString(lstMenhGia[i],'_');
            if (it.size()>=2){
                this->the.push_back(atoi(it[0].c_str()));
            }
        }
    }
    
    
    auto btnChiTiet = dynamic_cast<Button*>(pnlContent->getChildByName("btnChiTiet"));
    if (btnChiTiet != NULL){
        btnChiTiet->addClickEventListener(CC_CALLBACK_1(LayerNapZen::onBtnChiTiet, this));
        //this->btnMobi->setPressedActionEnabled(true);
        btnChiTiet->setTitleText(dataManager.GetSysString(977));
    }
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
    
    this->scrollView = ui::ScrollView::create();
    this->scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollView->setClippingEnabled(true);
    this->scrollView->setContentSize(this->pnlMenhGia->getContentSize());
    this->scrollView->setAnchorPoint(this->pnlMenhGia->getAnchorPoint());
    this->scrollView->setPosition(this->pnlMenhGia->getPosition());
    this->scrollView->setScrollBarEnabled(false);
    this->scrollView->setBounceEnabled(false);
    this->scrollView->setTag(12932);
    this->scrollView->setInnerContainerSize(Size(692, this->pnlMenhGia->getContentSize().height));
    pnlContent->addChild(this->scrollView);
    
//    this->scrollView->removeAllChildren();
//    this->lstMenhGia.clear();
//    for (int i = 0 ; i < this->the.size(); i++){
//        auto button = ui::Button::create();
//        button->loadTextures("design/guifiles/moc3-btnnhamang.png", "design/guifiles/moc3-btnnhamang.png", "design/guifiles/moc3-btnnhamang.png");
//        button->setAnchorPoint(Point(0, 1));
//        button->setContentSize(Size(692, 80));
//        button->setTitleText(StringUtils::format("%d",this->the[i]) + " $" );
//        button->setTitleFontSize(50);
//        //button->setTitleFontName("__Roboto-Regular.ttf");
//        button->addClickEventListener(CC_CALLBACK_1(LayerNapZen::onMenhGia, this));
//        //button->setTitleColor(ccc3(255, 255, 255));
//        button->setTag(this->lstMenhGia.size()+1);
//        /*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
//        button->setPosition(Vec2(0,(this->the.size())*80 - this->lstMenhGia.size() * 80));
//        this->scrollView->addChild(button);
//        this->scrollView->setVisible(false);
//        this->lstMenhGia.push_back(button);
//    }
//    this->scrollView->setInnerContainerSize(Size(734,this->lstMenhGia.size()*80));
//
    this->loadTyGia();

    if (this->lstMenhGia.size()<=0) return true;
    this->onMenhGia(this->lstMenhGia[0]);
    
    
    return true;
}
void LayerNapZen::onBtnChiTiet(Ref* pSender){
    LayerGuideInGame* _layer = LayerGuideInGame::create();
    string url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+"ty_le_nap.html";
    _layer->setLink(url);
    _layer->setTitle(dataManager.GetSysString(972));
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene)
        return;
    scene->addChild(_layer, 2000);
}
void LayerNapZen::onMenhGia(Ref* pSender){
    for (int i = 0 ; i < this->lstMenhGia.size();i++){
        this->lstMenhGia[i]->setEnabled(true);
        this->lstMenhGia[i]->setBright(true);

    }
    auto sender = (Button*)pSender;
    sender->setEnabled(false);
    sender->setBright(false);
    int tag = sender->getTag();
    this->scrollView->setVisible(false);
    this->currMenhgia = tag-1;
    this->btnMenhGia->setTitleText(this->lstMenhGia[this->currMenhgia]->getTitleText());
    this->txtTienNhan->setVisible(true);
    int tiennhan = (SceneManager::getSingleton().getExchangeRate(1,this->the[0])/this->the[0])*this->the[this->currMenhgia];
    
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    if (myself->GetVariable("am") == NULL) return;
    auto amf = myself->GetVariable("am")->GetDoubleValue();
    int khuyenmailandau = 0;
    if (amf){
        int am = *amf;
        if (am <=0 && atoi(dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR.c_str()) > 0){
            khuyenmailandau = tiennhan*atoi(dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR.c_str())/100;
        }
    }
    
    int tienkhuyenmai = SceneManager::getSingleton().getExchangeRate(1,this->the[this->currMenhgia])- tiennhan+khuyenmailandau;
    this->txtTienNhan->setString(mUtils::convertMoneyEx(tiennhan));
    this->txtKhuyenMai->setString(mUtils::convertMoneyEx(tienkhuyenmai));
    this->txtKhuyenMai->setVisible(true);
}
void LayerNapZen::hideLayer(){
    this->setVisible(false);
    //    TextField* txtWingAccount;
    //    TextField* txtPhoneNumber;
    //    TextField* txtUserName;
    //    TextField* txtOTP;
    this->txtWingAccount->setEnableEditBox(false);
    this->txtPhoneNumber->setEnableEditBox(false);
    //this->txtUserName->setEnableEditBox(false);
    this->txtOTP->setEnableEditBox(false);
    
}
void LayerNapZen::showLayer(){
    this->setVisible(true);
    this->txtWingAccount->setEnableEditBox(true);
    this->txtPhoneNumber->setEnableEditBox(true);
    //this->txtUserName->setEnableEditBox(true);
    this->txtOTP->setEnableEditBox(true);
}
void LayerNapZen::loadTyGia(){
    this->scrollTyGia->setInnerContainerSize(Size(734,270));
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
        button->setTitleText(StringUtils::format("%d",this->the[i]) + " $" );
        button->setTitleFontSize(50);
        //button->setTitleFontName("__Roboto-Regular.ttf");
        button->addClickEventListener(CC_CALLBACK_1(LayerNapZen::onMenhGia, this));
        //button->setTitleColor(ccc3(255, 255, 255));
        button->setTag(this->lstMenhGia.size()+1);
        /*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
        button->setPosition(Vec2(0 + (i%col)*259,310 - 146 * (i/col)-5));
        this->scrollTyGia->addChild(button);
        //this->scrollTyGia->setVisible(false);
        this->lstMenhGia.push_back(button);
    }
    this->onMenhGia(this->lstMenhGia[0]);
}
void LayerNapZen::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene) return;
   // log("OnExtensionResponse %s", cmd->c_str());

    if (strcmp(EXT_EVENT_WING_GEN_SECURITY_CODE_RESP, cmd->c_str()) == 0){//OK
        std::string str = " ";
        boost::shared_ptr<long> rscodeptr = param->GetInt("rc");
        boost::shared_ptr<string> rd = param->GetUtfString("rd");
        
        if (rscodeptr)
        {
            if (*rscodeptr == 0){
                LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
                layerPopup->showAButton();
                layerPopup->setMessage(dataManager.GetSysString(141));
                layerPopup->setContentMess(dataManager.GetSysString(950));
                scene->addChild(layerPopup,2000);
                SceneManager::getSingleton().hideLoading();
            }else{
                if (rd == NULL) return;
                LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
                layerPopup->showAButton();
                layerPopup->setMessage(dataManager.GetSysString(141));
                layerPopup->setContentMess(*rd);
                scene->addChild(layerPopup,2000);
                SceneManager::getSingleton().hideLoading();
            }
            
        }
    }
    if (strcmp(EXT_EVENT_WING_RECHARGE_RESP, cmd->c_str()) == 0){//OK
        std::string str = " ";
        boost::shared_ptr<long> rscodeptr = param->GetInt("rc");
        boost::shared_ptr<string> rd = param->GetUtfString("rd");
        if (rscodeptr)
        {
            if (*rscodeptr == 0){
                LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
                layerPopup->showAButton();
                layerPopup->setMessage(dataManager.GetSysString(141));
                layerPopup->setContentMess(dataManager.GetSysString(951));
                scene->addChild(layerPopup,2000);
                SceneManager::getSingleton().hideLoading();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                IOS::eventPurchase(this->amount);
#endif
            }else{
                if (rd == NULL) return;
                LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
                layerPopup->showAButton();
                layerPopup->setMessage(dataManager.GetSysString(141));
                layerPopup->setContentMess(*rd);
                scene->addChild(layerPopup,2000);
                SceneManager::getSingleton().hideLoading();
            }
        }
    }
    //    if (strcmp("rsiapp", cmd->c_str()) == 0){//OK
    //        int rc = *param->GetInt("rc");
    //        if (rc == 0){
    //            string valuePur = "";
    //            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
    //            layerPopup->showAButton();
    //            layerPopup->setMessage("Chúc mừng");
    //            layerPopup->setContentMess("Nạp tiền thành công!");
    //            this->addChild(layerPopup);
    //            SceneManager::getSingleton().hideLoading();
    //        }
    //        else{
    //            string valuePur = "";
    //            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
    //            layerPopup->showAButton();
    //            layerPopup->setMessage("Chúc mừng");
    //            layerPopup->setContentMess("Nạp tiền thất bại. Vui lòng thử lại !");
    //            this->addChild(layerPopup);
    //            SceneManager::getSingleton().hideLoading();
    //        }
    //    }
}

void LayerNapZen::onBtnNapThe(Ref* pSender){
    
    if (mUtils::findSpecialCharSpace(this->txtWingAccount->getString())){
        this->txtErrorWingAccount->setVisible(true);
        this->txtErrorWingAccount->setString(dataManager.GetSysString(186));
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
    
    //    if (strlen(this->txtPhoneNumber->getString().c_str()) == 0){
    //        this->txtErrorPhoneNumber->setVisible(true);
    //        this->txtErrorPhoneNumber->setString("* "+dataManager.GetSysString(949));
    //        return;
    //    }
    //    else{
    //        this->txtErrorPhoneNumber->setVisible(false);
    //    }
    
    //    if (strlen(this->txtUserName->getString().c_str()) == 0){
    //        this->txtErrorUserName->setVisible(true);
    //        this->txtErrorUserName->setString("* "+dataManager.GetSysString(949));
    //        return;
    //    }
    //    else{
    //        this->txtErrorUserName->setVisible(false);
    //    }
    
    if (strlen(this->txtOTP->getString().c_str()) == 0){
        this->txtErrorOTP->setVisible(true);
        this->txtErrorOTP->setString("* "+dataManager.GetSysString(949));
        return;
    }
    else{
        this->txtErrorOTP->setVisible(false);
    }
    //
    //    if (mUtils::findContainChar(txtPhoneNumber->getString().c_str())){
    //        CCLOG("loi nhap so");
    //        this->txtErrorPhoneNumber->setVisible(true);
    //        this->txtErrorPhoneNumber->setString("* "+dataManager.GetSysString(716));
    //        return;
    //    }
    //    else{
    //        this->txtErrorPhoneNumber->setVisible(false);
    //
    //    }
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
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    params->PutUtfString("lav", dataManager.getAppConfig().version_android);// LOGIN_APP_VERSION = "lav";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    params->PutUtfString("lav", dataManager.getAppConfig().version_ios);// LOGIN_APP_VERSION = "lav";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    //parameters->PutUtfString("lav", "ios1452832982");// Only for test
    params->PutUtfString("lav", dataManager.getAppConfig().version_win);
#endif
    params->PutInt("osid", osid);
    params->PutUtfString("devid", devID);
    params->PutUtfString(EXT_FIELD_WING_ACCOUNT, this->txtWingAccount->getString());
    params->PutUtfString(EXT_FIELD_WING_SECURITY_CODE, this->txtOTP->getString());
    this->amount = this->the[this->currMenhgia];
    params->PutInt(EXT_FIELD_WING_AMOUNT,this->the[this->currMenhgia]);
    // params->PutUtfString(EXT_FIELD_WING_SECURITY_CODE, this->txtUserName->getString());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_WING_RECHARGE_REQ, params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
    
    
    //    else if (strlen(txtMaThe->getString().c_str()) < 12){
    //        CCLOG("loi nhap ma the < 12");
    //        this->txtErrorMaThe->setVisible(true);
    //        this->txtErrorMaThe->setString("* "+dataManager.GetSysString(717));
    //        return;
    //    }
    //    else{
    //        CCLOG("loi khac");
    //        this->txtErrorMaThe->setVisible(false);
    //    }
    
    
    //send Request
    //this->btnNapThe->setEnabled(false);
    //send request
    
    //    boost::shared_ptr<ISFSObject> params(new SFSObject());
    //    params->PutUtfString("issuer", issues.c_str());
    //    params->PutUtfString("cardcode", this->txtMaThe->getString());
    //    params->PutUtfString("cardserial", this->txtSeri->getString());
    //    boost::shared_ptr<IRequest> request(new ExtensionRequest("rv", params));
    //    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
    
    
}
void LayerNapZen::onBtnMenhGia(Ref* pSender){
    
    if (this->scrollView->isVisible() ){
        this->scrollView->setVisible(false);
        
        
    }else{
        this->scrollView->setVisible(true);
        
    }
}
void LayerNapZen::onBtnGetOTP(Ref* pSender){
    if (strlen(this->txtWingAccount->getString().c_str()) == 0){
        this->txtErrorWingAccount->setVisible(true);
        this->txtErrorWingAccount->setString("* "+dataManager.GetSysString(949));
        return;
    }
    else{
        this->txtErrorWingAccount->setVisible(false);
    }
    
    
    
    if (strlen(this->txtPhoneNumber->getString().c_str()) == 0){
        this->txtErrorPhoneNumber->setVisible(true);
        this->txtErrorPhoneNumber->setString("* "+dataManager.GetSysString(949));
        return;
    }
    else{
        this->txtErrorPhoneNumber->setVisible(false);
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
    
    //    if (strlen(this->txtUserName->getString().c_str()) == 0){
    //        this->txtErrorUserName->setVisible(true);
    //        this->txtErrorUserName->setString("* "+dataManager.GetSysString(949));
    //        return;
    //    }
    //    else{
    //        this->txtErrorUserName->setVisible(false);
    //    }
    
    //    if (strlen(this->txtOTP->getString().c_str()) == 0){
    //        this->txtErrorOTP->setVisible(true);
    //        this->txtErrorOTP->setString("* "+dataManager.GetSysString(949));
    //        return;
    //    }
    //    else{
    //        this->txtErrorOTP->setVisible(false);
    //    }
    
    if (mUtils::findContainChar(txtPhoneNumber->getString().c_str())){
        CCLOG("loi nhap so");
        this->txtErrorPhoneNumber->setVisible(true);
        this->txtErrorPhoneNumber->setString("* "+dataManager.GetSysString(650));
        return;
    }
    else{
        this->txtErrorPhoneNumber->setVisible(false);
        
    }
    //    else if (strlen(txtMaThe->getString().c_str()) < 12){
    //        CCLOG("loi nhap ma the < 12");
    //        this->txtErrorMaThe->setVisible(true);
    //        this->txtErrorMaThe->setString("* "+dataManager.GetSysString(717));
    //        return;
    //    }
    //    else{
    //        CCLOG("loi khac");
    //        this->txtErrorMaThe->setVisible(false);
    //    }
    
    
    //send Request
    //this->btnNapThe->setEnabled(false);
    //send request
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString(EXT_FIELD_WING_ACCOUNT, this->txtWingAccount->getString());
    params->PutUtfString(EXT_FIELD_WING_PHONE, this->txtPhoneNumber->getString());
    this->amount = this->the[this->currMenhgia];
    params->PutInt(EXT_FIELD_WING_AMOUNT,this->the[this->currMenhgia]);
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_WING_GEN_SECURITY_CODE_REQ, params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
    
}

//bool LayerNapZen::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
//{
//    return true;
//}

void LayerNapZen::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerNapZen::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerNapZen::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool ItemTyGiaNapZen::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("ItemTyGiaMuaThe.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    this->imgBg = static_cast<ImageView*>(root->getChildByName("imgBg"));
    this->txtMenhGia = static_cast<Text*>(this->imgBg->getChildByName("txtMenhGia"));
    this->txtGiaBan = static_cast<Text*>(this->imgBg->getChildByName("txtGiaBan"));
    this->txtKhuyenMai = static_cast<Text*>(this->imgBg->getChildByName("txtKhuyenMai"));
    this->imgZen = static_cast<ImageView*>(this->imgBg->getChildByName("imgZen"));
    this->imgZen->setVisible(false);
    
    this->txtKhuyenMai->setVisible(false);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ItemTyGiaNapZen::showNapTien),this,1, false);
    this->showNapTien(0);
    return true;
}

void ItemTyGiaNapZen::showNapTien(float dt){
    if (!SceneManager::getSingleton().isLogin)
        return;
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    auto amf = myself->GetVariable("am")->GetDoubleValue();
    if (amf){
        int am = *amf;
        if (am <=0){
            this->txtKhuyenMai->setVisible(true);
            this->imgZen->setVisible(false);
            
        }else{
            this->txtKhuyenMai->setVisible(false);
            this->imgZen->setVisible(true);
            
        }
    }
}
void ItemTyGiaNapZen::setDatas(int idx,int menhgia, int giamua){
    if (idx%2 ==1 ){
        this->imgBg->loadTexture("moc3-table-tbody-2.png");
    }
    this->txtMenhGia->setString(mUtils::convertMoneyEx(menhgia));
    this->txtGiaBan->setString(mUtils::convertMoneyEx(giamua));
}

