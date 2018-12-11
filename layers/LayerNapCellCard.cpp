//
//  LayerNapCellCard.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerNapCellCard.h"
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
LayerNapCellCard::LayerNapCellCard()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerNapCellCard::~LayerNapCellCard()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerNapCellCard::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerNapZenCellCard.csb");
    
    
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    auto pnlContent = dynamic_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    auto lblMG = static_cast<Text*> (rootNode->getChildByName("Text_2"));
    lblMG->setString(dataManager.GetSysString(815));
    auto lblQD = static_cast<Text*> (rootNode->getChildByName("Text_2_0"));
    lblQD->setString(dataManager.GetSysString(816));

    auto btnNapThe = dynamic_cast<Button*>(pnlContent->getChildByName("btnNapThe"));
    if (btnNapThe != NULL){
        btnNapThe->addClickEventListener(CC_CALLBACK_1(LayerNapCellCard::onBtnNapThe, this));
        //this->btnMobi->setPressedActionEnabled(true);
        btnNapThe->setTitleText(dataManager.GetSysString(790));
    }
    
  
    auto btnChiTiet = dynamic_cast<Button*>(pnlContent->getChildByName("btnChiTiet"));
    if (btnChiTiet != NULL){
        btnChiTiet->addClickEventListener(CC_CALLBACK_1(LayerNapCellCard::onBtnChiTiet, this));
        //this->btnMobi->setPressedActionEnabled(true);
        btnChiTiet->setTitleText(dataManager.GetSysString(977));
    }
    
    
    
    
    
    this->txtPhoneNumber = dynamic_cast<TextField*>(pnlContent->getChildByName("txtPhoneNumber"));
    this->txtPhoneNumber->setPlaceHolder(dataManager.GetSysString(945));
    this->txtPhoneNumber->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->txtPhoneNumber->setMaxLength(30);
   
    this->txtMoney = dynamic_cast<TextField*>(pnlContent->getChildByName("txtMoney"));
    this->txtMoney->setPlaceHolder(dataManager.GetSysString(1028));
    this->txtMoney->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->txtMoney->setMaxLength(4);
    
    //    this->txtUserName = dynamic_cast<TextField*>(pnlContent->getChildByName("txtUserName"));
    //    this->txtUserName->setPlaceHolder(dataManager.GetSysString(946));
  
    this->txtErrorPhoneNumber = dynamic_cast<Text*>(pnlContent->getChildByName("txtErrorPhoneNumber"));
    this->txtErrorPhoneNumber->setVisible(false);
    //    this->txtErrorUserName = dynamic_cast<Text*>(pnlContent->getChildByName("txtErrorUserName"));
    //    this->txtErrorUserName->setVisible(false);
    this->txtErrorMoney = dynamic_cast<Text*>(pnlContent->getChildByName("txtErrorMoney"));
    this->txtErrorMoney->setVisible(false);
    auto txtLabel2 = dynamic_cast<Text*>(pnlContent->getChildByName("txtLabel2"));
    txtLabel2->setString(dataManager.GetSysString(945));
    this->txtTienNhan = dynamic_cast<Text*>(pnlContent->getChildByName("txtTienNhan"));
    this->txtTienNhan->setVisible(false);
    auto txtLabel1 = dynamic_cast<Text*>(pnlContent->getChildByName("txtLabel2_0_0"));
    txtLabel1->setString(dataManager.GetSysString(1027));
    if (SceneManager::getSingleton().currLang == 1){
        txtLabel1->setFontSize(33);
    }
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
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_FORTUMO_GET_LIST_EXCHANGE_REQ,params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
    
    
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
//        button->addClickEventListener(CC_CALLBACK_1(LayerNapCellCard::onMenhGia, this));
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
//    this->loadTyGia();

    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerNapCellCard::updateValueTextField),this,1, false);
    
    return true;
}
void LayerNapCellCard::onBtnChiTiet(Ref* pSender){
    LayerGuideInGame* _layer = LayerGuideInGame::create();
    string url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+"ty_le_cellcard.html";
    _layer->setLink(url);
    _layer->setTitle(dataManager.GetSysString(972));
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene)
        return;
    scene->addChild(_layer, 2000);
}
void LayerNapCellCard::onMenhGia(Ref* pSender){
    this->moneyNhap = 0;
    if (this->money.size()<=0) return;
    for (int i = 0 ; i < this->lstMenhGia.size();i++){
        this->lstMenhGia[i]->setEnabled(true);
        this->lstMenhGia[i]->setBright(true);

    }
    if (this->txtMoney->getString().length()>0){
        this->txtMoney->setString("");
    }
    auto sender = (Button*)pSender;
    sender->setEnabled(false);
    sender->setBright(false);
    int tag = sender->getTag();
    this->scrollView->setVisible(false);
    this->currMenhgia = tag-1;
    this->txtTienNhan->setVisible(true);
    int tiennhan = this->money[this->currMenhgia];
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
    int tienkhuyenmai = this->khuyenmai[this->currMenhgia] +khuyenmailandau;
    this->txtTienNhan->setString(mUtils::convertMoneyEx(tiennhan));
    this->txtKhuyenMai->setString(mUtils::convertMoneyEx(tienkhuyenmai));
    this->txtKhuyenMai->setVisible(true);
}
void LayerNapCellCard::hideLayer(){
    this->setVisible(false);
    //    TextField* txtWingAccount;
    //    TextField* txtPhoneNumber;
    //    TextField* txtUserName;
    //    TextField* txtOTP;
    this->txtPhoneNumber->setEnableEditBox(false);
    //this->txtUserName->setEnableEditBox(false);
    this->txtMoney->setEnableEditBox(false);
    
}
void LayerNapCellCard::showLayer(){
    this->setVisible(true);
    this->txtMoney->setEnableEditBox(true);
    this->txtPhoneNumber->setEnableEditBox(true);
    //this->txtUserName->setEnableEditBox(true);
}
void LayerNapCellCard::loadTyGia(){
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
        button->addClickEventListener(CC_CALLBACK_1(LayerNapCellCard::onMenhGia, this));
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
void LayerNapCellCard::updateValueTextField(float dt){
    string txt = this->txtMoney->getString();
    if (txt.length()>0){
        for (int i = 0 ; i < this->lstMenhGia.size();i++){
            this->lstMenhGia[i]->setEnabled(true);
            this->lstMenhGia[i]->setBright(true);
            
        }
        
        if (mUtils::findContainChar(this->txtMoney->getString().c_str())) {
            this->txtErrorMoney->setVisible(true);
            this->txtErrorMoney->setString("* "+dataManager.GetSysString(137));
            return;
        }
        
        else{
            this->txtErrorMoney->setVisible(false);
        }
        
        
        
        
        int tientra = atoi(this->txtMoney->getString().c_str());
        if (tientra <= 0){
            this->txtErrorMoney->setVisible(true);
            this->txtErrorMoney->setString("* "+dataManager.GetSysString(139));
            return;
        }
        
        else{
            this->txtErrorMoney->setVisible(false);
        }
        if (tientra == this->moneyNhap) return;

        
        this->currMenhgia = 0;
        
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt(EXT_FIELD_FORTUMO_VALUE, tientra);
        this->moneyNhap = tientra;
        boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_FORTUMO_EXCHANGE_REQ,params));
        GameServer::getSingleton().Send(request);
       // SceneManager::getSingleton().showLoading();
        
    }
    else{
        if (this->currMenhgia == 0){
            if (this->lstMenhGia.size() <= 0 ) return;
            this->onMenhGia(this->lstMenhGia[0]);
        }
    }
}
void LayerNapCellCard::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene) return;
   // log("OnExtensionResponse %s", cmd->c_str());

    if (strcmp(EXT_EVENT_FORTUMO_GET_LIST_EXCHANGE_RESP, cmd->c_str()) == 0){//OK
        SceneManager::getSingleton().hideLoading();

        std::string str = " ";
        boost::shared_ptr<string> ftexl = param->GetUtfString(EXT_FIELD_FORTUMO_EXCHANGE_LIST);
        if (ftexl)
        {
            auto lst = mUtils::splitString(*ftexl,';');
            for (int i = 0 ; i < lst.size() ; i++){
                auto it = mUtils::splitString(lst[i],',');
                if (it.size()<3)
                    continue;
                this->the.push_back(atoi(it[0].c_str()));
                this->money.push_back(atoi(it[1].c_str()));
                this->khuyenmai.push_back(atoi(it[2].c_str()));
                this->loadTyGia();
            }
        }
    }
    if (strcmp(EXT_EVENT_FORTUMO_EXCHANGE_RESP, cmd->c_str()) == 0){//OK
       // SceneManager::getSingleton().hideLoading();
        
        std::string str = " ";
        boost::shared_ptr<string> ftexr = param->GetUtfString(EXT_FIELD_FORTUMO_EXCHANGE_RESULT);
        if (ftexr)
        {
            auto it = mUtils::splitString(*ftexr,',');
            if (it.size()<3)
                return;
            double tiennhan = atof(it[0].c_str());
            double tienkhuyenmai = atof(it[1].c_str());
            this->txtTienNhan->setString(mUtils::convertMoneyEx(tiennhan));
            this->txtKhuyenMai->setString(mUtils::convertMoneyEx(tienkhuyenmai));
        }
    }
    if (strcmp(EXT_EVENT_FORTUMO_GET_CODE_RESP, cmd->c_str()) == 0){//OK
        boost::shared_ptr<string> ftcode = param->GetUtfString(EXT_FIELD_FORTUMO_CODE);
        boost::shared_ptr<string> fturl = param->GetUtfString(EXT_FIELD_FORTUMO_URL);
        SceneManager::getSingleton().hideLoading();

        if (ftcode && fturl){
            //https://sv.nagaclub.net/payment/cellcard?amount=1&msisdn=077410410&username=dungdt&transid=84b96728-d659-429c-ae06-af1e88f54772
            
            int amount = this->the[this->currMenhgia];
            if (this->txtMoney->getString().length()>0){
                amount = atoi(this->txtMoney->getString().c_str());
            }
            string url = StringUtils::format("%s?amount=%d&msisdn=%s&username=%s&transid=%s",fturl->c_str(),amount,this->txtPhoneNumber->getString().c_str(),SceneManager::getSingleton().getMyName().c_str(),ftcode->c_str());
            
            LayerGuideInGame* layer = LayerGuideInGame::create();
            layer->setLink(url);
            layer->setTitle(dataManager.GetSysString(1030));
            Scene *scene = Director::getInstance()->getRunningScene();
            if (!scene)
                return;
            scene->addChild(layer, 2000);
            layer->setTag(1504);
        }

    }
    if (strcmp(EXT_EVENT_FORTUMO_CHARGE_SUCCESS_NTF, cmd->c_str()) == 0){//OK
        boost::shared_ptr<string> ftmsg = param->GetUtfString(EXT_FIELD_FORTUMO_MESSAGE);
        
        if (ftmsg){
            Scene *scene = Director::getInstance()->getRunningScene();
            if (!scene)
                return;
            scene->removeChildByTag(1504);
            
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(*ftmsg);
            scene->addChild(layerPopup,2000);
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

void LayerNapCellCard::onBtnNapThe(Ref* pSender){
    

    
        if (strlen(this->txtPhoneNumber->getString().c_str()) == 0){
            this->txtErrorPhoneNumber->setVisible(true);
            this->txtErrorPhoneNumber->setString("* "+dataManager.GetSysString(949));
            return;
        }
        else{
            this->txtErrorPhoneNumber->setVisible(false);
        }
    
    //    if (strlen(this->txtUserName->getString().c_str()) == 0){
    //        this->txtErrorUserName->setVisible(true);
    //        this->txtErrorUserName->setString("* "+dataManager.GetSysString(949));
    //        return;
    //    }
    //    else{
    //        this->txtErrorUserName->setVisible(false);
    //    }
    

    //
        if (mUtils::findContainChar(txtPhoneNumber->getString().c_str())){
            CCLOG("loi nhap so");
            this->txtErrorPhoneNumber->setVisible(true);
            this->txtErrorPhoneNumber->setString("* "+dataManager.GetSysString(650));
            return;
        }
        else{
            this->txtErrorPhoneNumber->setVisible(false);
    
        }
    
    if (strlen(this->txtMoney->getString().c_str()) > 0){
        if (mUtils::findContainChar(this->txtMoney->getString().c_str())) {
            this->txtErrorMoney->setVisible(true);
            this->txtErrorMoney->setString("* "+dataManager.GetSysString(137));
            return;
        }
       
        else{
            this->txtErrorMoney->setVisible(false);
        }
       
        

        
        int tientra = atoi(this->txtMoney->getString().c_str());
        
        if (tientra <= 0){
            this->txtErrorMoney->setVisible(true);
            this->txtErrorMoney->setString("* "+dataManager.GetSysString(139));
            return;
        }
        
        else{
            this->txtErrorMoney->setVisible(false);
        }
        
    }
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_FORTUMO_GET_CODE_REQ,params));
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


//bool LayerNapCellCard::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
//{
//    return true;
//}

void LayerNapCellCard::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerNapCellCard::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerNapCellCard::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

