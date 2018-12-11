//
//  LayerMuaThe.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerMuaThe.h"
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
LayerMuaThe::LayerMuaThe()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerMuaThe::~LayerMuaThe()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerMuaThe::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerMuaThe.csb");
    
    
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    auto lblMGQD = static_cast<Text*> (rootNode->getChildByName("Text_2"));
    lblMGQD->setString(dataManager.GetSysString(815));
    auto lblGBQD= static_cast<Text*> (rootNode->getChildByName("Text_2_0"));
    lblGBQD->setString(dataManager.GetSysString(839));
    this->pnlContent = dynamic_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    auto lblNM = static_cast<Text*> (this->pnlContent->getChildByName("Text_1"));
    lblNM->setString(dataManager.GetSysString(835));
    auto lblMG= static_cast<Text*> (this->pnlContent->getChildByName("Text_1_0"));
    lblMG->setString(dataManager.GetSysString(836));
    auto lblGB = static_cast<Text*> (this->pnlContent->getChildByName("Text_1_0_0"));
    lblGB->setString(dataManager.GetSysString(837));
    this->btnNhaMang = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnNhaMang"));
    if (this->btnNhaMang != NULL){
        this->btnNhaMang->addClickEventListener(CC_CALLBACK_1(LayerMuaThe::onBtnNhaMang, this));
        //this->btnNhaMang->setPressedActionEnabled(true);
    }
    this->btnViettel = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnViettel"));
    if (this->btnViettel != NULL){
        this->btnViettel->addClickEventListener(CC_CALLBACK_1(LayerMuaThe::onBtnViettel, this));
        //this->btnViettel->setPressedActionEnabled(true);
        this->btnViettel->setTitleText(dataManager.GetSysString(812));

    }
    this->btnVina = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnVina"));
    if (this->btnVina != NULL){
        this->btnVina->addClickEventListener(CC_CALLBACK_1(LayerMuaThe::onBtnVina, this));
        //this->btnVina->setPressedActionEnabled(true);
        this->btnVina->setTitleText(dataManager.GetSysString(813));

    }
    this->btnMobi = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnMobi"));
    if (this->btnMobi != NULL){
        this->btnMobi->addClickEventListener(CC_CALLBACK_1(LayerMuaThe::onBtnMobi, this));
        //this->btnMobi->setPressedActionEnabled(true);
        this->btnMobi->setTitleText(dataManager.GetSysString(814));

    }
    this->btnMuaThe = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnMuaThe"));
    if (this->btnMuaThe != NULL){
        this->btnMuaThe->addClickEventListener(CC_CALLBACK_1(LayerMuaThe::onBtnMuaThe, this));
        this->btnMuaThe->setPressedActionEnabled(true);
        this->btnMuaThe->setTitleText(dataManager.GetSysString(838));

    }
    this->btnMenhGia = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnMenhGia"));
    if (this->btnMenhGia != NULL){
        this->btnMenhGia->addClickEventListener(CC_CALLBACK_1(LayerMuaThe::onBtnMenhGia, this));
        //this->btnMobi->setPressedActionEnabled(true);
    }
    
    
    
    //this->txtChuChay->setScale(5);
    
    Size sizeAdd;
  //  if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    
    auto pnlTyGia = dynamic_cast<Layout*>(rootNode->getChildByName("pnlTyGia"));
    pnlTyGia->setVisible(false);
    
    
    this->scrollTyGia = ui::ScrollView::create();
    this->scrollTyGia->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollTyGia->setClippingEnabled(true);
    this->scrollTyGia->setContentSize(pnlTyGia->getContentSize());
    this->scrollTyGia->setAnchorPoint(pnlTyGia->getAnchorPoint());
    this->scrollTyGia->setPosition(pnlTyGia->getPosition());
    this->scrollTyGia->setScrollBarEnabled(false);
    this->scrollTyGia->setBounceEnabled(false);
    this->scrollTyGia->setTag(129323);
    this->scrollTyGia->setInnerContainerSize(Size(734, pnlTyGia->getContentSize().height));
    this->addChild(this->scrollTyGia);
    
    
    this->pnlMenhGia = dynamic_cast<Layout*>(pnlContent->getChildByName("pnlMenhGia"));
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
    this->pnlContent->addChild(this->scrollView);

    
    
    
    
    this->txtGiaBan = dynamic_cast<Text*>(pnlContent->getChildByName("txtGiaBan"));

    this->lstThe.clear();
    this->the.clear();
    the.push_back(50000);
    the.push_back(100000);
    the.push_back(200000);
    the.push_back(500000);

    string strDataSms = "3-50000;3-100000;3-200000;3-500000;2-50000;2-100000;2-200000;2-500000;1-50000;1-100000;1-200000;1-500000";
    this->lstThe = mUtils::splitString(strDataSms,';');
    

    
    return true;
}
void LayerMuaThe::loadTyGia(){
    this->scrollTyGia->removeAllChildren();
    this->scrollTyGia->setInnerContainerSize(Size(734,this->the.size()*79));
    for (int i =0;i<this->the.size();i++){
        ItemTyGiaMuaThe* item = ItemTyGiaMuaThe::create();
        item->setAnchorPoint(Vec2(0,1));
        item->setDatas(i,this->the[i],this->the[i]*this->mTax);
        this->scrollTyGia->addChild(item);
         item->setPosition(Vec2(0,470 - (i+1) * 79));
    }
}
void LayerMuaThe::loadThe(int type){
    
    this->scrollView->removeAllChildren();
    this->lstMenhGia.clear();
    for (int i = 0 ; i < this->lstThe.size(); i++){
        auto item = mUtils::splitString(this->lstThe[i],'-');
        if (item.size() < 2)
            continue;
        if (atoi(item[0].c_str()) == type){
            // vector<Button*> lstMenhGia;
            auto button = ui::Button::create();
            button->loadTextures("design/guifiles/moc3-item-doimoc.png", "design/guifiles/moc3-item-doimoc.png", "design/guifiles/moc3-item-doimoc.png");
            button->setAnchorPoint(Point(0, 1));
            button->setContentSize(Size(517, 80));
            button->setTitleText(item[1]);
            button->setTitleFontSize(40);
            //button->setTitleFontName("__Roboto-Regular.ttf");
            button->addClickEventListener(CC_CALLBACK_1(LayerMuaThe::onMenhGia, this));
            //button->setTitleColor(ccc3(255, 255, 255));
            button->setTag(this->lstMenhGia.size()+1);
            /*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
            button->setPosition(Vec2(0,this->pnlMenhGia->getContentSize().height - this->lstMenhGia.size() * 80));
            this->scrollView->addChild(button);
            this->scrollView->setVisible(false);
            this->lstMenhGia.push_back(button);
        }
    }
    if (this->lstMenhGia.size() > 0){
        this->onMenhGia(this->lstMenhGia[0]);
        this->btnMenhGia->setEnabled(true);
        this->btnMenhGia->setBright(true);
        this->btnMuaThe->setEnabled(true);
        this->btnMuaThe->setBright(true);
    }else{
        this->btnMenhGia->setEnabled(false);
        this->btnMenhGia->setBright(false);
        this->btnMenhGia->setTitleText(dataManager.GetSysString(562));
        this->btnMuaThe->setEnabled(false);
        this->btnMuaThe->setBright(false);
    }
}

void LayerMuaThe::onBtnMuaThe(Ref* pSender){
    
    string strNhaMang = "";
    switch (this->typeNhamang){
        case 1:
            strNhaMang = "Vina";
            break;
        case 2:
            strNhaMang = "Mobi";
            break;
        case 3:
            strNhaMang = "Viettel";
            break;
    }

    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
//    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
//    if (myself != NULL){
//        if (myself->GetVariable("amf") != NULL)
//        {
//            if (myself->GetVariable("lvl") != NULL){
//                
//                auto level = myself->GetVariable("lvl")->GetIntValue();
//                if (level != NULL){
//                    if (*level < SceneManager::getSingleton().getMinMoney(4)){
//                        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
//                            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
//                            layerPopup->showAButton();
//                            layerPopup->setMessage(dataManager.GetSysString(141));
//                            layerPopup->setContentMess(StringUtils::format("Level %d trở lên mới được đổi thưởng!", SceneManager::getSingleton().getMinMoney(4)));
//                            layerPopup->setTag(24);
//                            _currScene->addChild(layerPopup);
//                            return;
//                        }
//                    }
//                }
//            }
//            if (myself->GetVariable("amf") != NULL){
//                auto myMoney = myself->GetVariable("amf")->GetDoubleValue();
//                if (myMoney != NULL){
//                    if (*myMoney < atoi(this->lstMenhGia[this->currMenhgia]->getTitleText().c_str())*this->mTax){
//                        LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
//                        layerPopup->showAButton();
//                        layerPopup->setMessage(dataManager.GetSysString(141));
//                        layerPopup->setContentMess("Bạn không đủ tiền để mua thẻ cào này!");
//                        layerPopup->setTag(24);
//                        _currScene->addChild(layerPopup);
//                        return;
//                    }
//                    else if ((*myMoney - atoi(this->lstMenhGia[this->currMenhgia]->getTitleText().c_str())*this->mTax) < SceneManager::getSingleton().getMinMoney(1) && SceneManager::getSingleton().getCapVip() < 6){
//                        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
//                            
//                            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
//                            layerPopup->showAButton();
//                            layerPopup->setMessage(dataManager.GetSysString(141));
//                            layerPopup->setContentMess(StringUtils::format("Số tiền của bạn sau khi đổi thẻ này phải lớn hơn %s %s!", mUtils::format_money((long)SceneManager::getSingleton().getMinMoney(1))->c_str(), SceneManager::getSingleton().getCoin(0).c_str()));
//                            layerPopup->setTag(24);
//                            _currScene->addChild(layerPopup);
//                            return;
//                        }
//                    }
//                    
//                }
//            }
//        }
//    }
//    
    
    LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
    layerPopup->setCallBackFromButton(this, callfuncO_selector(LayerMuaThe::popUpCallBack), 5);
    layerPopup->setMessage(dataManager.GetSysString(141));
    layerPopup->setContentMess(StringUtils::format("%s %s %s %s %s %s %s",dataManager.GetSysString(653).c_str(), mUtils::format_money(atoi(this->lstMenhGia[this->currMenhgia]->getTitleText().c_str())*this->mTax)->c_str(), SceneManager::getSingleton().getCoin(0).c_str(),dataManager.GetSysString(654).c_str(), strNhaMang.c_str(), mUtils::format_money(atoi(this->lstMenhGia[this->currMenhgia]->getTitleText().c_str()))->c_str(),dataManager.GetSysString(144).c_str()));
    layerPopup->setTag(24);
    _currScene->addChild(layerPopup,2000,9088);
    
 }
void LayerMuaThe::popUpCallBack(Ref* pSender){
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
    auto node = (Node*)pSender;
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("aN", myself->Name());
    params->PutUtfString("issuer", StringUtils::format("%d", this->typeNhamang).c_str());
    params->PutInt("vv", atoi(this->lstMenhGia[this->currMenhgia]->getTitleText().c_str()));
    params->PutInt("vnc", 1);
    params->PutUtfString("voucherDesc", "");
    params->PutInt("osid", osid);
    params->PutUtfString("devid", devID);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rgb", params));
    GameServer::getSingleton().Send(request);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    NativeHelper::trackDoiThuong(StringUtils::format("%d", atoi(this->lstMenhGia[this->currMenhgia]->getTitleText().c_str())).c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::trackDoiThuong(StringUtils::format("%d",atoi(this->lstMenhGia[this->currMenhgia]->getTitleText().c_str())));
#endif
    SceneManager::getSingleton().showLoading();

}
void LayerMuaThe::onMenhGia(Ref* pSender){
    auto sender = (Button*)pSender;
    int tag = sender->getTag();
    
    this->isShowSelectMenhGia = false;
    this->scrollView->setVisible(false);
    this->currMenhgia = tag-1;
    
    this->btnMenhGia->setTitleText(this->lstMenhGia[this->currMenhgia]->getTitleText());
    this->txtGiaBan->setString(mUtils::convertMoneyEx(atoi(this->lstMenhGia[this->currMenhgia]->getTitleText().c_str())*this->mTax));
    
}
void LayerMuaThe::onBtnMenhGia(Ref* pSender){
    
    if (this->isShowSelectMenhGia) {
        this->isShowSelectMenhGia = false;
        this->scrollView->setVisible(false);


    }else{
        this->isShowSelectMenhGia = true;
        this->scrollView->setVisible(true);

    }
    
    this->isShowSelectNhaMang = false;
    this->btnViettel->setVisible(false);
    this->btnVina->setVisible(false);
    this->btnMobi->setVisible(false);
        
}
void LayerMuaThe::hideLayer(){
    this->setVisible(false);
   /// this->webView->setVisible(false);
}
void LayerMuaThe::showLayer(){
    this->setVisible(true);
//    this->webView->setVisible(true);
//    this->webView->reload();
//    this->onBtnBack(NULL);
    SceneManager::getSingleton().showLoading();
    //
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    params->PutInt("cfk", 15);//CONFIG_KEY = "cfk"; 15: lấy tỉ giá qui đổi ngược
    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("reqgcf", params)); //REQUEST_GET_CONFIG = "reqgcf";
    GameServer::getSingleton().Send(request);
}
void LayerMuaThe::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    if (strcmp("respcf", cmd->c_str()) == 0){ //RESP_GET_CONFIG = "respcf";
        this->mTax = *param->GetDouble("cfv");//CONFIG_VALUE = "cfv";
        //recalculator
        SceneManager::getSingleton().hideLoading();
        this->onBtnViettel(this->btnViettel);
        this->loadTyGia();

    }
    else if (strcmp("tgbresp", cmd->c_str()) == 0){
         SceneManager::getSingleton().hideLoading();
        int rc = *param->GetInt("rc");
        auto rd = *param->GetUtfString("rd");
        if (rc == 0) {
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(dataManager.GetSysString(525));
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,2000,9089);
        }
        else {
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(rd + "");
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,2000,9090);
            
        }
    }
}
void LayerMuaThe::onBtnNhaMang(Ref* pSender){
    this->isShowSelectMenhGia = false;
    this->scrollView->setVisible(false);

    
    if (this->isShowSelectNhaMang){
        this->isShowSelectNhaMang = false;
        this->btnViettel->setVisible(false);
        this->btnVina->setVisible(false);
        this->btnMobi->setVisible(false);
        
    }else{
        this->btnViettel->setVisible(true);
        this->btnMobi->setVisible(true);
        this->isShowSelectNhaMang = true;
        this->btnVina->setVisible(true);
        
    }
}
void LayerMuaThe::onBtnViettel(Ref* pSender){
    auto sender = (Button*)pSender;
    this->typeNhamang = VIETTEL;
    this->btnNhaMang->setTitleText(sender->getTitleText());
    this->scrollView->setVisible(false);

    this->isShowSelectNhaMang = false;
    this->btnViettel->setVisible(false);
    this->btnVina->setVisible(false);
    this->btnMobi->setVisible(false);
    this->loadThe(this->typeNhamang);
    
}
void LayerMuaThe::onBtnVina(Ref* pSender){
    auto sender = (Button*)pSender;
    this->typeNhamang = VINA;
    this->btnNhaMang->setTitleText(sender->getTitleText());
    
    this->isShowSelectNhaMang = false;
    this->btnViettel->setVisible(false);
    this->btnVina->setVisible(false);
    this->btnMobi->setVisible(false);
    this->loadThe(this->typeNhamang);

}
void LayerMuaThe::onBtnMobi(Ref* pSender){
    auto sender = (Button*)pSender;
    this->typeNhamang = MOBI;
    this->btnNhaMang->setTitleText(sender->getTitleText());
    
    this->isShowSelectNhaMang = false;
    this->btnViettel->setVisible(false);
    this->btnVina->setVisible(false);
    this->btnMobi->setVisible(false);
    this->loadThe(this->typeNhamang);

}
//bool LayerMuaThe::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
//{
//    return true;
//}

void LayerMuaThe::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerMuaThe::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerMuaThe::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool ItemTyGiaMuaThe::init()
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
void ItemTyGiaMuaThe::setDatas(int idx,int menhgia, int giamua){
    if (idx%2 ==1 ){
        this->imgBg->loadTexture("moc3-table-tbody-2.png");
    }
    this->txtMenhGia->setString(mUtils::convertMoneyEx(menhgia));
    this->txtGiaBan->setString(mUtils::convertMoneyEx(giamua));
}
