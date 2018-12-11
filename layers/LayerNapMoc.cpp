//
//  LayerNapMoc.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerNapMoc.h"
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
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
LayerNapMoc::LayerNapMoc()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerNapMoc::~LayerNapMoc()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerNapMoc::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerNapMoc.csb");
    
    
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    auto pnlContent = dynamic_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    auto lblMG = static_cast<Text*> (rootNode->getChildByName("Text_2"));
    lblMG->setString(dataManager.GetSysString(815));
    auto lblQD = static_cast<Text*> (rootNode->getChildByName("Text_2_0"));
    lblQD->setString(dataManager.GetSysString(816));
    this->btnNhaMang = dynamic_cast<Button*>(pnlContent->getChildByName("btnNhaMang"));
    if (this->btnNhaMang != NULL){
        this->btnNhaMang->addClickEventListener(CC_CALLBACK_1(LayerNapMoc::onBtnNhaMang, this));
        //this->btnNhaMang->setPressedActionEnabled(true);
    }
    this->btnViettel = dynamic_cast<Button*>(pnlContent->getChildByName("btnViettel"));
    if (this->btnViettel != NULL){
        this->btnViettel->addClickEventListener(CC_CALLBACK_1(LayerNapMoc::onBtnViettel, this));
        this->btnViettel->setTitleText(dataManager.GetSysString(812));

        //this->btnViettel->setPressedActionEnabled(true);
    }
    this->btnVina = dynamic_cast<Button*>(pnlContent->getChildByName("btnVina"));
    if (this->btnVina != NULL){
        this->btnVina->addClickEventListener(CC_CALLBACK_1(LayerNapMoc::onBtnVina, this));
        this->btnVina->setTitleText(dataManager.GetSysString(813));

        //this->btnVina->setPressedActionEnabled(true);
    }
    this->btnMobi = dynamic_cast<Button*>(pnlContent->getChildByName("btnMobi"));
    if (this->btnMobi != NULL){
        this->btnMobi->addClickEventListener(CC_CALLBACK_1(LayerNapMoc::onBtnMobi, this));
        this->btnMobi->setTitleText(dataManager.GetSysString(814));

        //this->btnMobi->setPressedActionEnabled(true);
    }
    auto btnNapThe = dynamic_cast<Button*>(pnlContent->getChildByName("btnNapThe"));
    if (btnNapThe != NULL){
        btnNapThe->addClickEventListener(CC_CALLBACK_1(LayerNapMoc::onBtnNapThe, this));
        //this->btnMobi->setPressedActionEnabled(true);
        btnNapThe->setTitleText(dataManager.GetSysString(811));
    }
    
    this->txtMaThe = dynamic_cast<TextField*>(pnlContent->getChildByName("txtMaThe"));
    this->txtMaThe->setPlaceHolder(dataManager.GetSysString(809));
    this->txtSeri = dynamic_cast<TextField*>(pnlContent->getChildByName("txtSeri"));
    this->txtSeri->setPlaceHolder(dataManager.GetSysString(810));

    this->txtErrorMaThe = dynamic_cast<Text*>(pnlContent->getChildByName("txtErrorMaThe"));
    this->txtErrorSeri = dynamic_cast<Text*>(pnlContent->getChildByName("txtErrorSeri"));
    
    this->onBtnViettel(this->btnViettel);
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

    this->the.push_back(10000);
    this->the.push_back(20000);
    this->the.push_back(30000);
    this->the.push_back(50000);
    this->the.push_back(100000);
    this->the.push_back(200000);
    this->the.push_back(500000);

    this->scrollTyGia = ui::ScrollView::create();
    this->scrollTyGia->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollTyGia->setClippingEnabled(true);
    this->scrollTyGia->setContentSize(pnlTyGia->getContentSize());
    this->scrollTyGia->setAnchorPoint(pnlTyGia->getAnchorPoint());
    this->scrollTyGia->setPosition(pnlTyGia->getPosition());
    this->scrollTyGia->setScrollBarEnabled(true);
    this->scrollTyGia->setBounceEnabled(false);
    this->scrollTyGia->setTag(129323);
    this->scrollTyGia->setInnerContainerSize(Size(734, pnlTyGia->getContentSize().height));
    this->addChild(this->scrollTyGia);
    
    return true;
}
void LayerNapMoc::hideLayer(){
    this->setVisible(false);
    this->txtSeri->setEnableEditBox(false);
    this->txtMaThe->setEnableEditBox(false);
}
void LayerNapMoc::showLayer(){
    this->setVisible(true);
    this->txtSeri->setEnableEditBox(true);
    this->txtMaThe->setEnableEditBox(true);
}
void LayerNapMoc::loadTyGia(){
    this->scrollTyGia->setInnerContainerSize(Size(734,this->the.size()*79));
    this->scrollTyGia->removeAllChildren();
    for (int i =0;i<this->the.size();i++){
        ItemTyGiaNapThe* item = ItemTyGiaNapThe::create();
        item->setAnchorPoint(Vec2(0,1));
        item->setDatas(i,this->the[i],this->the[i]*this->mTax);
        this->scrollTyGia->addChild(item);
        item->setPosition(Vec2(0,470 - (i) * 79));
    }
}
void LayerNapMoc::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp("respcf", cmd->c_str()) == 0){ //RESP_GET_CONFIG = "respcf";
        this->mTax = *param->GetDouble("cfv");//CONFIG_VALUE = "cfv";
//        //recalculator
//        SceneManager::getSingleton().hideLoading();
//        this->onBtnViettel(this->btnViettel);
        this->loadTyGia();
        
    }
    if (strcmp("rg", cmd->c_str()) == 0){//OK
        std::string str = " ";
        boost::shared_ptr<long> rscodeptr = param->GetInt("rc");
        if (rscodeptr)
        {
            int rscode = *rscodeptr;
            if (rscode == 0)
            {
                //str = dataManager.GetSysString(28);
                boost::shared_ptr<string> res_decrip_ptr = param->GetUtfString("rd");
                if (res_decrip_ptr)
                {
                    str += *res_decrip_ptr;
                }
            }
            else if (rscode == 1){
                int am = 0;
                boost::shared_ptr<double> amfptr = param->GetDouble("am");
                if (amfptr != NULL)
                {
                    am = *amfptr;
                }
                //if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                NativeHelper::trackNapTien(StringUtils::format("%d",am).c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                IOS::trackingNapThe(StringUtils::format("%d",am));
                
#endif
                //}
               // log("Nap the OK");
                str = dataManager.GetSysString(636);
                
                //                if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
                //                    auto currScene = Director::getInstance()->getRunningScene();
                //                    if (currScene != NULL)
                //                    {
                ////                        if (dataManager.getAppConfig().isShowLucky)
                ////                        {
                ////                            auto _popupLucky = LayerPopupLuckyNumber::create(CODE_REASON_NAPTHE, txtMaThe->getString(), this->txtSeri->getString());
                ////                            currScene->addChild(_popupLucky, ZORDER_LIST::ZORDER_POPUP_MAINTAIN, TAG_LAYER_NAP_TIEN);
                ////                        }
                ////                        else{
                ////                            dataManager.getAppConfig().isLoginFisrtDay = false;
                ////                        }
                //                    }
                //                }
                this->txtMaThe->setString("");
                this->txtSeri->setString("");
                //this->btnNapThe->setEnabled(true);
                
            }
            else{
               // log("Nap the NOT OK %s", "i dont no");
                str = dataManager.GetSysString(637);
                //this->btnNapThe->setEnabled(true);
            }
            this->txtErrorSeri->setString(str);
            this->txtErrorSeri->setVisible(true);
            SceneManager::getSingleton().hideLoading();

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

void LayerNapMoc::onBtnNapThe(Ref* pSender){
    this->txtErrorMaThe->setString("");
    this->txtErrorSeri->setString("");
    
    
    //
    std::string issues = StringUtils::format("%d",this->typeNhamang);
    
    //    if (strcmp(issues.c_str(), "") == 0)
    //    {
    //        CCLOG("Chua chon nha Mang");
    //        lblPromotion->setVisible(true);
    //        lblPromotion->setString("Vui lòng chọn nhà mạng để tiếp tục, xin cám ơn!");
    //        return;
    //    }
    
    if (mUtils::findContainChar(txtMaThe->getString().c_str())){
        CCLOG("loi nhap so");
        this->txtErrorMaThe->setVisible(true);
        this->txtErrorMaThe->setString("* "+dataManager.GetSysString(716));
        return;
    }
    else if (strlen(txtMaThe->getString().c_str()) < 12){
        CCLOG("loi nhap ma the < 12");
        this->txtErrorMaThe->setVisible(true);
        this->txtErrorMaThe->setString("* "+dataManager.GetSysString(717));
        return;
    }
    else{
        CCLOG("loi khac");
        this->txtErrorMaThe->setVisible(false);
    }
    
    if (strlen(this->txtSeri->getString().c_str()) == 0){
        this->txtErrorSeri->setVisible(true);
        this->txtErrorSeri->setString("* "+dataManager.GetSysString(718));
        return;
    }
    else{
        this->txtErrorSeri->setVisible(false);
    }
    //send Request
    //this->btnNapThe->setEnabled(false);
    //send request
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("issuer", issues.c_str());
    params->PutUtfString("cardcode", this->txtMaThe->getString());
    params->PutUtfString("cardserial", this->txtSeri->getString());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rv", params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();

    
}
void LayerNapMoc::onBtnNhaMang(Ref* pSender){
    
    if (this->isShowSelect){
        this->isShowSelect = false;
        this->btnViettel->setVisible(false);
        this->btnVina->setVisible(false);
        this->btnMobi->setVisible(false);
        this->txtMaThe->setEnableEditBox(true);
        this->txtSeri->setEnableEditBox(true);
    }else{
        this->btnViettel->setVisible(true);
        this->btnMobi->setVisible(true);
        this->isShowSelect = true;
        this->btnVina->setVisible(true);
        this->txtSeri->setEnableEditBox(false);
        this->txtMaThe->setEnableEditBox(false);
    }
}
void LayerNapMoc::onBtnViettel(Ref* pSender){
    auto sender = (Button*)pSender;
    this->typeNhamang = sender->getTag();
    this->btnNhaMang->setTitleText(sender->getTitleText());
    
    this->isShowSelect = false;
    this->btnViettel->setVisible(false);
    this->btnVina->setVisible(false);
    this->btnMobi->setVisible(false);
    this->txtMaThe->setEnableEditBox(true);
    this->txtSeri->setEnableEditBox(true);
}
void LayerNapMoc::onBtnVina(Ref* pSender){
    auto sender = (Button*)pSender;
    this->typeNhamang = sender->getTag();
    this->btnNhaMang->setTitleText(sender->getTitleText());
    
    this->isShowSelect = false;
    this->btnViettel->setVisible(false);
    this->btnVina->setVisible(false);
    this->btnMobi->setVisible(false);
    this->txtMaThe->setEnableEditBox(true);
    this->txtSeri->setEnableEditBox(true);
}
void LayerNapMoc::onBtnMobi(Ref* pSender){
    auto sender = (Button*)pSender;
    this->typeNhamang = sender->getTag();
    this->btnNhaMang->setTitleText(sender->getTitleText());
    
    this->isShowSelect = false;
    this->btnViettel->setVisible(false);
    this->btnVina->setVisible(false);
    this->btnMobi->setVisible(false);
    this->txtMaThe->setEnableEditBox(true);
    this->txtSeri->setEnableEditBox(true);
}
//bool LayerNapMoc::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
//{
//    return true;
//}

void LayerNapMoc::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerNapMoc::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerNapMoc::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool ItemTyGiaNapThe::init()
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
    
    return true;
}
void ItemTyGiaNapThe::setDatas(int idx,int menhgia, int giamua){
    if (idx%2 ==1 ){
        this->imgBg->loadTexture("moc3-table-tbody-2.png");
    }
    this->txtMenhGia->setString(mUtils::convertMoneyEx(menhgia));
    this->txtGiaBan->setString(mUtils::convertMoneyEx(giamua));
}

