//
//  LayerPopupBanMoc.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerPopupBanMoc.h"
#include "LayerPopupCapNhatSTK.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "LayerPopupDaiLy.h"
#include "LayerPopupOTP.h"
#include "LayerPopupThongBao.h"
#include "../layergames/_Chat_.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
LayerPopupBanMoc::LayerPopupBanMoc()
{
    GameServer::getSingleton().addListeners(this);

}


LayerPopupBanMoc::~LayerPopupBanMoc()
{
    GameServer::getSingleton().removeListeners(this);

}

bool LayerPopupBanMoc::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupBanMoc.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
  //  if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupBanMoc::onBtnClose, this));
    }

    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    this->nodeDaiLy = static_cast<Node*>(pnlBg->getChildByName("nodeDaiLy"));
    this->txtDaiLy= static_cast<Text*>(this->nodeDaiLy->getChildByName("txtDaiLy"));
     this->btnChonSTK = dynamic_cast<Button*>(this->nodeDaiLy->getChildByName("btnChonSTK"));
    if( this->btnChonSTK){
         this->btnChonSTK->addClickEventListener(CC_CALLBACK_1(LayerPopupBanMoc::onBtnChonSTK, this));
    }
    this->btnMenhGia = dynamic_cast<Button*>(this->nodeDaiLy->getChildByName("btnMenhGia"));
    if( this->btnMenhGia){
         this->btnMenhGia->addClickEventListener(CC_CALLBACK_1(LayerPopupBanMoc::onBtnMenhGia, this));
    }
    this->txtLyDo  = static_cast<TextField*>(this->nodeDaiLy->getChildByName("txtLyDo"));

    Button* btnTiepTuc = dynamic_cast<Button*>(this->nodeDaiLy->getChildByName("btnTiepTuc"));
    if(btnTiepTuc){
        btnTiepTuc->addClickEventListener(CC_CALLBACK_1(LayerPopupBanMoc::onBtnTiepTuc, this));
    }
    
    Button* btnCapNhatSTK = dynamic_cast<Button*>(this->nodeDaiLy->getChildByName("btnCapNhatSTK"));
    if(btnCapNhatSTK){
        btnCapNhatSTK->addClickEventListener(CC_CALLBACK_1(LayerPopupBanMoc::onBtnCapNhatSTK, this));
    }
    this->btnTK = dynamic_cast<Button*>(this->nodeDaiLy->getChildByName("btnTK"));
    if(this->btnTK){
        this->btnTK->addClickEventListener(CC_CALLBACK_1(LayerPopupBanMoc::onBtnTaiKhoan, this));
    }
    
    
    this->nodeOTP = static_cast<Node*>(pnlBg->getChildByName("nodeOTP"));
    this->txtOTP =  static_cast<TextField*>(this->nodeOTP->getChildByName("txtOTP"));
    
    Button* btnBack = dynamic_cast<Button*>(this->nodeOTP->getChildByName("btnBack"));
    if(btnBack){
        btnBack->addClickEventListener(CC_CALLBACK_1(LayerPopupBanMoc::onBtnBack, this));
    }
    Button* btnXacNhan = dynamic_cast<Button*>(this->nodeOTP->getChildByName("btnXacNhan"));
    if(btnXacNhan){
        btnXacNhan->addClickEventListener(CC_CALLBACK_1(LayerPopupBanMoc::onBtnXacNhan, this));
    }
    
    this->nodeOTP->setVisible(false);
    this->nodeDaiLy->setVisible(true);
    
    
    this->txtErrorSoTK = dynamic_cast<Text*>(this->nodeDaiLy->getChildByName("txtErrorSoTK"));
    this->txtErrorMenhGia = dynamic_cast<Text*>(this->nodeDaiLy->getChildByName("txtErrorMenhGia"));
    this->txtErrorLyDoChuyenKhoan = dynamic_cast<Text*>(this->nodeDaiLy->getChildByName("txtErrorLyDoChuyenKhoan"));
    
    this->txtErrorSoTK->setVisible(false);
    this->txtErrorMenhGia->setVisible(false);
    this->txtErrorLyDoChuyenKhoan->setVisible(false);
    
    this->txtSoMocNhanDuoc = dynamic_cast<Text*>(this->nodeDaiLy->getChildByName("txtSoMocNhanDuoc"));
    
    
    this->pnlMenhGia = dynamic_cast<Layout*>(this->nodeDaiLy->getChildByName("pnlMenhGia"));
    this->pnlMenhGia->setVisible(false);
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
    this->nodeDaiLy->addChild(this->scrollView);

    info.sotk  = "";
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_ACC_BANK_INFOR_REQ, params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
    

       //this->txtChuChay->setScale(5);
    return true;
}
void LayerPopupBanMoc::onBtnBack(Ref* pSender){
    this->nodeOTP->setVisible(false);
    this->nodeDaiLy->setVisible(true);
}
void LayerPopupBanMoc::onBtnXacNhan(Ref* pSender){
    
    string otp = this->txtOTP->getString();

    if (otp == ""){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(145));
        return;
    }
    
    int osid = 1;
    string devID = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    osid = 1;
    devID = NativeHelper::getDeviceId();
    /*isConnectting = NativeHelper::hasConnection();*/
    //devID = NativeHelper::getDeviceID();
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    osid = 2;
    devID = IOS::getDeviceID();
#endif
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt(BANK_ID, info.id);
    params->PutUtfString(BANK_NUMBER, info.sotk);
    params->PutUtfString(BANK_NAME, info.chutk);
    params->PutUtfString(BANK_BRANCH, info.chinhanh);
    params->PutInt(AMOUNT_BET, this->menhgiachon);
    params->PutUtfString(RECHARGE_DESC, this->txtLyDo->getString());
    params->PutInt("osid", osid);
    params->PutUtfString("otpcode", otp);
    params->PutUtfString("devid", devID);
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GOLD2_BANK_CREATE_REQ,params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();

}
void LayerPopupBanMoc::onPopupCallBack(Ref *pSender){
    

//    if (this->menhgiachon == 0 ){
//        this->txtErrorMenhGia->setVisible(true);
//        this->txtErrorMenhGia->setString("Bạn chưa chọn số mộc cần bán!");
//    }
//    else{
//        this->txtErrorMenhGia->setVisible(false);
//
//    }
//    if (info.sotk == "" ){
//        this->txtErrorSoTK->setVisible(true);
//        this->txtErrorSoTK->setString("Bạn chưa chọn số tài khoản!");
//    }
//    else{
//        this->txtErrorSoTK->setVisible(false);
//        
//    }
//    if (this->txtLyDo->getString() == "" ){
//        this->txtErrorLyDoChuyenKhoan->setVisible(true);
//        this->txtErrorLyDoChuyenKhoan->setString("Bạn chưa nhập lý do chuyển khoản!");
//    }
//    else{
//        this->txtErrorLyDoChuyenKhoan->setVisible(false);
//        
//    }
    
    
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    _currScene->removeChildByTag(24);
    
    if (dataManager.getAppConfig().isactiveOTP){
        SceneManager::getSingleton().showLoading();
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        boost::shared_ptr<IRequest> request(new ExtensionRequest("gotpcmd", params));
        GameServer::getSingleton().Send(request);


    }else{
        int osid = 1;
        string devID = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        osid = 1;
        devID = NativeHelper::getDeviceId();
        /*isConnectting = NativeHelper::hasConnection();*/
        //devID = NativeHelper::getDeviceID();
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        osid = 2;
        devID = IOS::getDeviceID();
#endif

        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt(BANK_ID, info.id);
        params->PutUtfString(BANK_NUMBER, info.sotk);
        params->PutUtfString(BANK_NAME, info.chutk);
        params->PutUtfString(BANK_BRANCH, info.chinhanh);
        params->PutInt(AMOUNT_BET, this->menhgiachon);
        params->PutUtfString(RECHARGE_DESC, this->txtLyDo->getString());
        params->PutInt("osid", osid);
        params->PutUtfString("devid", devID);
        boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GOLD2_BANK_CREATE_REQ,params));
        GameServer::getSingleton().Send(request);
        
        SceneManager::getSingleton().showLoading();
    }
}
void LayerPopupBanMoc::onBtnTaiKhoan(Ref* pSender){
    this->isChonSTK = true;
    this->btnChonSTK->setTitleText(this->btnTK->getTitleText());
    this->isShowTaiKhoan = false;
    this->btnTK->setVisible(false);
}

void LayerPopupBanMoc::onBtnChonSTK(Ref* pSender){
    if ( info.sotk == "") return;
    this->isShowMenhGia = false;
    this->scrollView->setVisible(false);
    if (this->isShowTaiKhoan){
        this->isShowTaiKhoan = false;
        this->btnTK->setVisible(false);
    }else{
        this->isShowTaiKhoan = true;
        this->btnTK->setVisible(true);
    }
}
void LayerPopupBanMoc::onBtnMenhGia(Ref* pSender){
    this->isShowTaiKhoan = false;
    this->btnTK->setVisible(false);
    if (this->isShowMenhGia){
        this->isShowMenhGia = false;
        this->scrollView->setVisible(false);
    }else{
        this->isShowMenhGia = true;
        this->scrollView->setVisible(true);
    }

}
void LayerPopupBanMoc::onBtnTiepTuc(Ref* pSender){
    this->isShowMenhGia = false;
    this->scrollView->setVisible(false);
    this->isShowTaiKhoan = false;
    this->btnTK->setVisible(false);
    string lydo = this->txtLyDo->getString();
    if (lydo == "")
    {
        this->txtErrorLyDoChuyenKhoan->setVisible(true);
        this->txtErrorLyDoChuyenKhoan->setString(dataManager.GetSysString(616));
        return;
        
    }else{
        this->txtErrorLyDoChuyenKhoan->setVisible(false);
    }
    if (!this->isChonSTK){
        this->txtErrorSoTK->setVisible(true);
        this->txtErrorSoTK->setString(dataManager.GetSysString(526));
        return;
    }else{
        this->txtErrorSoTK->setVisible(false);
    }
    if (!this->isChonMenhGia){
        this->txtErrorMenhGia->setVisible(true);
        this->txtErrorMenhGia->setString(dataManager.GetSysString(527));
        return;
    }else{
        this->txtErrorMenhGia->setVisible(false);
    }
    
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    
    auto layerpoup = LayerPopupThongBao::create();
    layerpoup->setMessage(dataManager.GetSysString(141));
    layerpoup->setContentMess(dataManager.GetSysString(528));
    layerpoup->setCallBackFromButton(this, callfuncO_selector(LayerPopupBanMoc::onPopupCallBack), 1);
    layerpoup->setTag(24);
    _currScene->addChild(layerpoup, 10000,9091);
 
}
void LayerPopupBanMoc::onBtnCapNhatSTK(Ref* pSender){
    //if (dataManager.getAppConfig().isactiveOTP){
    LayerPopupCapNhatSTK* popup = LayerPopupCapNhatSTK::create();
    this->addChild(popup);
  //  }else{
//        LayerPopupOTP* otp = LayerPopupOTP::create();
//        this->addChild(otp);
//    }
}
void LayerPopupBanMoc::setDaiLy(string daily){
    this->txtDaiLy->setString(daily);
}
void LayerPopupBanMoc::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp(EXT_EVENT_GET_ACC_BANK_INFOR_RESP, cmd->c_str()) == 0){//OK
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetUtfString("rd") == NULL) return;
        string bankstr = *param->GetUtfString("rd");
        //1|2;TCB;Ngân hàng TMCP Kỹ Thương Việt Nam;Nguyen Tuan Anh;124124124124124124;Ha Noi|1;2;5;10;20;50
        auto lstbank = mUtils::splitString(bankstr,'|');
        if (lstbank.size() < 5) return;
        if (atoi(lstbank[0].c_str()) == 0){
            this->hasBank = false;
        }else{
            this->hasBank = true;
            this->tax = atof(lstbank[3].c_str());
            this->fee = atof(lstbank[4].c_str());
            auto infobank = mUtils::splitString(lstbank[1],';');
            if (infobank.size()<6)
                return;
            
            info.id = atoi(infobank[0].c_str());
            info.ma = infobank[1];
            info.name = infobank[2];
            info.chutk = infobank[3];
            info.sotk = infobank[4];
            info.chinhanh = infobank[5];
            this->btnTK->setTitleText(info.sotk);
            
            auto menhgias = mUtils::splitString(lstbank[2],';');

            this->menhgia.clear();
            for (int i = 0; i < menhgias.size(); i++){
                auto button = ui::Button::create();
                button->loadTextures("design/guifiles/moc3-btnnhamang.png", "design/guifiles/moc3-btnnhamang.png", "design/guifiles/moc3-btnnhamang.png");
                button->setAnchorPoint(Point(0, 1));
                button->setContentSize(Size(692, 80));
                button->setTitleText(mUtils::convertMoneyEx(atoi(menhgias[i].c_str())*1000000*this->tax+atoi(menhgias[i].c_str())*1000000*this->fee/100));
                button->setTitleFontSize(40);
                //button->setTitleFontName("__Roboto-Bold.ttf");
                button->addClickEventListener(CC_CALLBACK_1(LayerPopupBanMoc::onMenhGia, this));
                //button->setTitleColor(ccc3(255, 255, 255));
                button->setTag(i);
                /*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
                button->setPosition(Vec2(0,this->pnlMenhGia->getContentSize().height - this->menhgia.size() * 80));
                this->scrollView->addChild(button);
                this->scrollView->setVisible(false);
                this->menhgia.push_back(atoi(menhgias[i].c_str())*1000000);
            }

            
        }
       // log("bank : %s",bankstr.c_str());
    }else if (strcmp(EXT_EVENT_GOLD2_BANK_CREATE_RESP, cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        if (param->GetUtfString("rd") == NULL || param->GetInt("rc") == NULL ) return;
        int rc = *param->GetInt("rc");
        if (rc == 0){
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(dataManager.GetSysString(646));
            this->addChild(layerPopup);
            this->txtLyDo->setString("");
            this->nodeOTP->setVisible(false);
            this->nodeDaiLy->setVisible(true);
            this->txtOTP->setString("");
        }
        else{
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(*param->GetUtfString("rd"));
            this->addChild(layerPopup);
            
        }

    }else if (strcmp("otpresp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            Chat* toast = Chat::create(dataManager.GetSysString(447),-1);
            this->addChild(toast);
            this->nodeDaiLy->setVisible(false);
            this->nodeOTP->setVisible(true);
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }

}
void LayerPopupBanMoc::onMenhGia(Ref* pSender){
    auto node = (Node*)pSender;
    int tag = node->getTag();
    this->isChonMenhGia = true;
    this->menhgiachon = this->menhgia[tag];
    this->isShowMenhGia = false;
    this->scrollView->setVisible(false);
    this->btnMenhGia->setTitleText(mUtils::convertMoneyEx(this->menhgiachon*this->tax+this->menhgiachon*this->fee/100));
    this->txtSoMocNhanDuoc->setString(mUtils::convertMoneyEx(this->menhgiachon));
}
void LayerPopupBanMoc::hideLayer(){
    this->setVisible(false);
}
void LayerPopupBanMoc::showLayer(){
    this->setVisible(true);
}
bool LayerPopupBanMoc::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerPopupBanMoc::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerPopupBanMoc::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupBanMoc::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupBanMoc::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

