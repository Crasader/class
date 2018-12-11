//
//  LayerDoiWing.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerDoiWing.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../layergames/_Chat_.h"
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
LayerDoiWing::LayerDoiWing()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerDoiWing::~LayerDoiWing()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerDoiWing::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerDoiWing.csb");
    
    
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    auto pnlContent = dynamic_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    auto lblMG = static_cast<Text*> (rootNode->getChildByName("Text_2"));
    lblMG->setString(dataManager.GetSysString(815));
    auto lblQD = static_cast<Text*> (rootNode->getChildByName("Text_2_0"));
    lblQD->setString(dataManager.GetSysString(596));
    this->btnMenhGia = dynamic_cast<Button*>(pnlContent->getChildByName("btnMenhGia"));
    if (this->btnMenhGia != NULL){
        this->btnMenhGia->addClickEventListener(CC_CALLBACK_1(LayerDoiWing::onBtnMenhGia, this));
        //this->btnNhaMang->setPressedActionEnabled(true);
    }
    auto btnDoiWing = dynamic_cast<Button*>(pnlContent->getChildByName("btnDoiWing"));
    if (btnDoiWing != NULL){
        btnDoiWing->addClickEventListener(CC_CALLBACK_1(LayerDoiWing::onBtnDoiWing, this));
        //this->btnMobi->setPressedActionEnabled(true);
        btnDoiWing->setTitleText(dataManager.GetSysString(790));
    }
    
    auto btnChiTiet = dynamic_cast<Button*>(pnlContent->getChildByName("btnChiTiet"));
    if (btnChiTiet != NULL){
        btnChiTiet->addClickEventListener(CC_CALLBACK_1(LayerDoiWing::onBtnChiTiet, this));
        //this->btnMobi->setPressedActionEnabled(true);
        btnChiTiet->setTitleText(dataManager.GetSysString(977));
    }

    this->txtWingAccount = dynamic_cast<TextField*>(pnlContent->getChildByName("txtWingAccount"));
    this->txtWingAccount->setPlaceHolder(dataManager.GetSysString(944));
    this->txtWingAccount->setMaxLength(50);
    if (SceneManager::getSingleton().getBankAccountByType(1).length()>0 || SceneManager::getSingleton().getBankAccountByType(1)!=""){
        this->txtWingAccount->setString(SceneManager::getSingleton().getBankAccountByType(1));
    }
    this->txtErrorWingAccount = dynamic_cast<Text*>(pnlContent->getChildByName("txtErrorWingAccount"));
    this->txtErrorWingAccount->setVisible(false);
    auto txtLabel1 = dynamic_cast<Text*>(pnlContent->getChildByName("txtLabel1"));
    txtLabel1->setString(dataManager.GetSysString(944));
    
    auto nhan = dynamic_cast<Text*>(pnlContent->getChildByName("txtGuide1"));
    nhan->setString(dataManager.GetSysString(999));
    
    auto txtGuide1_1 = dynamic_cast<Text*>(pnlContent->getChildByName("txtGuide1_1"));
    txtGuide1_1->setString(dataManager.GetSysString(1026));
    this->txtGuide1 = dynamic_cast<Text*>(pnlContent->getChildByName("txtTienNhan"));
    this->txtGuide1->setVisible(false);
//    auto txtGuide2 = dynamic_cast<Text*>(pnlContent->getChildByName("txtGuide1_0"));
//    txtGuide2->setString(dataManager.GetSysString(968));
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
        auto lstMenhGia = mUtils::splitString(lst[0],';');
        for (int i = 0 ; i < lstMenhGia.size();i++){
            auto it = mUtils::splitString(lstMenhGia[i],'_');
            if (it.size()>=2){
                this->the.push_back(atoi(it[0].c_str()));
            }
        }
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
    this->scrollTyGia->setInnerContainerSize(Size(this->pnlMenhGia->getContentSize().width, this->pnlMenhGia->getContentSize().height));
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
    
    this->scrollTyGia->setInnerContainerSize(Size(787,430));
    this->scrollTyGia->removeAllChildren();
    this->lstMenhGia.clear();
    int col = 3;
    for (int i = 0 ; i < this->the.size(); i++){
        auto button = ui::Button::create();
        button->loadTextures("design/guifiles/muc-gia-nor-min.png", "design/guifiles/muc-gia-act-min.png", "design/guifiles/muc-gia-act-min.png");
        button->setAnchorPoint(Point(0, 1));
        button->setContentSize(Size(259, 146));
        button->setTitleText(StringUtils::format("%d",this->the[i]) + " $" );
        button->setTitleFontSize(50);
        //button->setTitleFontName("__Roboto-Regular.ttf");
        button->addClickEventListener(CC_CALLBACK_1(LayerDoiWing::onMenhGia, this));
        //button->setTitleColor(ccc3(255, 255, 255));
        button->setTag(this->lstMenhGia.size()+1);
        /*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
        button->setPosition(Vec2(0 + (i%col)*259,430 - 146 * (i/col)-5));
        this->scrollTyGia->addChild(button);
        //this->scrollTyGia->setVisible(false);
        this->lstMenhGia.push_back(button);
    }
    if (this->lstMenhGia.size()<=0) return true;

    this->onMenhGia(this->lstMenhGia[0]);

   
    return true;
}
void LayerDoiWing::onMenhGia(Ref* pSender){
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
    this->txtGuide1->setString(StringUtils::format("%s",mUtils::convertMoneyEx(SceneManager::getSingleton().getExchangeRate(0,this->the[this->currMenhgia])).c_str()));
    this->txtGuide1->setVisible(true);
}
void LayerDoiWing::hideLayer(){
    this->setVisible(false);
    //    TextField* txtWingAccount;
    //    TextField* txtPhoneNumber;
    //    TextField* txtUserName;
    //    TextField* txtOTP;
    this->txtWingAccount->setEnableEditBox(false);
  
}
void LayerDoiWing::showLayer(){
    this->setVisible(true);
    this->txtWingAccount->setEnableEditBox(true);
    
    SceneManager::getSingleton().showLoading();
    //
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    params->PutInt("cfk", 15);//CONFIG_KEY = "cfk"; 15: lấy tỉ giá qui đổi ngược
    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("reqgcf", params)); //REQUEST_GET_CONFIG = "reqgcf";
    GameServer::getSingleton().Send(request);
}
void LayerDoiWing::loadTyGia(){
    this->scrollTyGia->setInnerContainerSize(Size(787,430));
    this->scrollTyGia->removeAllChildren();
    this->lstMenhGia.clear();
    for (int i =0;i<this->the.size();i++){
//        ItemTyGiaDoiWing* item = ItemTyGiaDoiWing::create();
//        item->setAnchorPoint(Vec2(0,1));
//        item->setDatas(i,this->the[i],this->the[i]*this->mTax);
//        this->scrollTyGia->addChild(item);
//        item->setPosition(Vec2(0,470 - (i) * 79));

    }
    this->onMenhGia(this->lstMenhGia[0]);
}
void LayerDoiWing::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene) return;
   // log("OnExtensionResponse %s", cmd->c_str());
//   // if (strcmp("respcf", cmd->c_str()) == 0){ //RESP_GET_CONFIG = "respcf";
//        this->mTax = *param->GetDouble("cfv");//CONFIG_VALUE = "cfv";
//        //recalculator
//        SceneManager::getSingleton().hideLoading();
//        //this->onBtnViettel(this->btnViettel);
//        this->loadTyGia();
//        this->txtGuide1->setString(StringUtils::format("-%s 1$ = %f Zen",dataManager.GetSysString(967).c_str(),1*this->mTax));
//
//
//    }
    if (strcmp(EXT_EVENT_WING_CASHOUT_RESP, cmd->c_str()) == 0){//OK
        std::string str = " ";
        boost::shared_ptr<long> rscodeptr = param->GetInt("rc");
        boost::shared_ptr<string> rd = param->GetUtfString("rd");
        
        if (rscodeptr)
        {
            if (*rscodeptr == 0){
                LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
                layerPopup->showAButton();
                layerPopup->setMessage(dataManager.GetSysString(141));
                layerPopup->setContentMess(dataManager.GetSysString(646));
                scene->addChild(layerPopup,2000);
                SceneManager::getSingleton().hideLoading();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                IOS::eventCashout(this->amount);
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
void LayerDoiWing::onBtnChiTiet(Ref* pSender){
    LayerGuideInGame* _layer = LayerGuideInGame::create();
    string url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+"ty_le_rut.html";
    _layer->setLink(url);
    _layer->setTitle(dataManager.GetSysString(973));

    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene)
        return;
    scene->addChild(_layer, 2000);
}
void LayerDoiWing::onBtnDoiWing(Ref* pSender){

    if (SceneManager::getSingleton().getSoTienNap() < dataManager.getAppConfig().minVipDoiMoc){
        auto toast = Chat::create(StringUtils::format("%s %d $",dataManager.GetSysString(457).c_str(),dataManager.getAppConfig().minVipDoiMoc),-1);
        auto curentScene = Director::getInstance()->getRunningScene();
        curentScene->addChild(toast,10000);
        return;
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
    this->amount = this->the[this->currMenhgia];
    params->PutInt(EXT_FIELD_WING_AMOUNT,this->the[this->currMenhgia]);
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_WING_CASHOUT_REQ, params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}
void LayerDoiWing::onBtnMenhGia(Ref* pSender){
    
    if (this->scrollView->isVisible() ){
        this->scrollView->setVisible(false);
        
        
    }else{
        this->scrollView->setVisible(true);
        
    }
}


//bool LayerDoiWing::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
//{
//    return true;
//}

void LayerDoiWing::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerDoiWing::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerDoiWing::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool ItemTyGiaDoiWing::init()
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
    this->imgZen->setVisible(true);
    
    this->txtKhuyenMai->setVisible(false);
    //Director::getInstance()->getScheduler()->schedule(schedule_selector(ItemTyGiaDoiWing::showNapTien), 1);
    //this->showNapTien(0);
    return true;
}
void ItemTyGiaDoiWing::showNapTien(float dt){
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
void ItemTyGiaDoiWing::setDatas(int idx,int menhgia, int giamua){
    if (idx%2 ==1 ){
        this->imgBg->loadTexture("moc3-table-tbody-2.png");
    }
    this->txtMenhGia->setString(mUtils::convertMoneyEx(menhgia));
    this->txtGiaBan->setString(mUtils::convertMoneyEx(giamua));
}

