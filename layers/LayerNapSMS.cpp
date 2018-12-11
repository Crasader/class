//
//  LayerNapSMS.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerNapSMS.h"
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
LayerNapSMS::LayerNapSMS()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerNapSMS::~LayerNapSMS()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerNapSMS::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerNapSMS.csb");
    
    
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    
    this->pnlContent = dynamic_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    auto lblMG = static_cast<Text*> (rootNode->getChildByName("Text_2"));
    lblMG->setString(dataManager.GetSysString(815));
    auto lblQD = static_cast<Text*> (rootNode->getChildByName("Text_2_0"));
    lblQD->setString(dataManager.GetSysString(816));
  
    
    this->btnNhaMang = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnNhaMang"));
    if (this->btnNhaMang != NULL){
        this->btnNhaMang->addClickEventListener(CC_CALLBACK_1(LayerNapSMS::onBtnNhaMang, this));
        //this->btnNhaMang->setPressedActionEnabled(true);
    }
    this->btnViettel = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnViettel"));
    if (this->btnViettel != NULL){
        this->btnViettel->addClickEventListener(CC_CALLBACK_1(LayerNapSMS::onBtnViettel, this));
        //this->btnViettel->setPressedActionEnabled(true);
        this->btnViettel->setTitleText(dataManager.GetSysString(818));

    }
    this->btnVina = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnVina"));
    if (this->btnVina != NULL){
        this->btnVina->addClickEventListener(CC_CALLBACK_1(LayerNapSMS::onBtnVina, this));
        //this->btnVina->setPressedActionEnabled(true);
        this->btnVina->setTitleText(dataManager.GetSysString(819));

    }
    this->btnMobi = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnMobi"));
    if (this->btnMobi != NULL){
        this->btnMobi->addClickEventListener(CC_CALLBACK_1(LayerNapSMS::onBtnMobi, this));
        //this->btnMobi->setPressedActionEnabled(true);
        this->btnMobi->setTitleText(dataManager.GetSysString(820));

    }
    this->btnNap = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnNap"));
    if (this->btnNap != NULL){
        this->btnNap->addClickEventListener(CC_CALLBACK_1(LayerNapSMS::onBtnNap, this));
        //this->btnMobi->setPressedActionEnabled(true);
        this->btnNap->setTitleText(dataManager.GetSysString(817));

    }
    this->btnMenhGia = dynamic_cast<Button*>(this->pnlContent->getChildByName("btnMenhGia"));
    if (this->btnMenhGia != NULL){
        this->btnMenhGia->addClickEventListener(CC_CALLBACK_1(LayerNapSMS::onBtnMenhGia, this));
        //this->btnMobi->setPressedActionEnabled(true);
    }
    
    
    
    //this->txtChuChay->setScale(5);
    
    Size sizeAdd;
 //   if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
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
    this->scrollTyGia->setScrollBarEnabled(true);
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
    this->scrollView->setScrollBarEnabled(true);
    this->scrollView->setBounceEnabled(false);
    this->scrollView->setTag(12932);
    this->scrollView->setInnerContainerSize(Size(692, this->pnlMenhGia->getContentSize().height));
    this->pnlContent->addChild(this->scrollView);

    this->pnlNap = dynamic_cast<Layout*>(rootNode->getChildByName("pnlNap"));
    this->pnlNap->setVisible(false);
    auto lblNap= static_cast<Text*> (this->pnlNap->getChildByName("Text_1"));
    lblNap->setString(dataManager.GetSysString(817));
    auto lblCuPhap= static_cast<Text*> (this->pnlNap->getChildByName("Text_1_0"));
    lblCuPhap->setString(dataManager.GetSysString(821));
    this->txtTyGia = dynamic_cast<Text*>(this->pnlNap->getChildByName("txtTyGia"));
    this->txtCuPhap = dynamic_cast<Text*>(this->pnlNap->getChildByName("txtCuPhap"));
    auto btnBack = dynamic_cast<Button*>(this->pnlNap->getChildByName("btnBack"));
    if (btnBack){
        btnBack->addClickEventListener(CC_CALLBACK_1(LayerNapSMS::onBtnBack, this));
    }
    auto btnSend = dynamic_cast<Button*>(this->pnlNap->getChildByName("btnSend"));
    if (btnSend){
        btnSend->addClickEventListener(CC_CALLBACK_1(LayerNapSMS::onBtnSend, this));
        btnSend->setTitleText(dataManager.GetSysString(817));

    }
    
    string strDataSms = (dataManager.getAppConfig().strListSMS);
    //"3-9029-10000-MW 10000 CU NAP-4500;3-9029-20000-MW 20000 CU NAP-9000;3-9029-50000-MW 50000 CU NAP-22500;2-9029-10000-MW CU NAP10-4500;2-9029-20000-MW CU NAP20-9000;2-9029-50000-MW CU NAP50-22500;"
    this->lstSMS = mUtils::splitString(strDataSms,';');
    this->loadTyGia();
    this->onBtnViettel(this->btnViettel);
    
    return true;
}
void LayerNapSMS::loadTyGia(){
    int k = 0;
    for (int i = 0 ; i < this->lstSMS.size(); i++){
        auto item = mUtils::splitString(this->lstSMS[i],'-');
        if (item.size() < 5)
            continue;
        
        if (atoi(item[0].c_str()) == 3){
            k++;
            ItemTyGiaSMS* it = ItemTyGiaSMS::create();
            it->setAnchorPoint(Vec2(0,1));
            it->setDatas(i,atoi(item[2].c_str()),atoi(item[4].c_str()));
            this->scrollTyGia->addChild(it);
            it->setPosition(Vec2(0,470 - (k-1) * 79));
        }
    }
    

}
void LayerNapSMS::loadSMS(int type){
    
    this->scrollView->removeAllChildren();
    this->lstMenhGia.clear();
    this->lstcuphap.clear();
    this->lsttiennhanduoc.clear();
    this->lstdauso.clear();

    for (int i = 0 ; i < this->lstSMS.size(); i++){
        auto item = mUtils::splitString(this->lstSMS[i],'-');
        if (item.size() < 5)
            continue;
        if (atoi(item[0].c_str()) == type){
            // vector<Button*> lstMenhGia;
            this->lstcuphap.push_back(item[3]+ " " + SceneManager::getSingleton().getMyName());
            this->lsttiennhanduoc.push_back(item[4]);
            this->lstdauso.push_back(item[1]);
            auto button = ui::Button::create();
            button->loadTextures("design/guifiles/moc3-btnnhamang.png", "design/guifiles/moc3-btnnhamang.png", "design/guifiles/moc3-btnnhamang.png");
            button->setAnchorPoint(Point(0, 1));
            button->setContentSize(Size(692, 80));
            button->setTitleText(item[2]);
            button->setTitleFontSize(40);
            //button->setTitleFontName("__Roboto-Bold.ttf");
            button->addClickEventListener(CC_CALLBACK_1(LayerNapSMS::onMenhGia, this));
            //button->setTitleColor(ccc3(255, 255, 255));
            button->setTag(this->lstMenhGia.size()+1);
            /*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
            button->setPosition(Vec2(0,this->pnlMenhGia->getContentSize().height - (this->lstMenhGia.size()-1) * 80));
            this->scrollView->addChild(button);
            this->scrollView->setVisible(false);
            this->lstMenhGia.push_back(button);
        }
    }
    this->scrollView->setInnerContainerSize(Size(734,this->lstMenhGia.size()*79));
    if (this->lstMenhGia.size() > 0){
        this->onMenhGia(this->lstMenhGia[0]);
        this->btnMenhGia->setEnabled(true);
        this->btnMenhGia->setBright(true);
        this->btnNap->setEnabled(true);
        this->btnNap->setBright(true);
    }else{
        this->btnMenhGia->setEnabled(false);
        this->btnMenhGia->setBright(false);
        this->btnMenhGia->setTitleText(dataManager.GetSysString(562));
        this->btnNap->setEnabled(false);
        this->btnNap->setBright(false);
    }
}
void LayerNapSMS::onBtnSend(Ref* pSender){
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        const char* dauSo = this->lstdauso[this->currMenhgia].c_str();
        const char* cuPhap = this->lstcuphap[this->currMenhgia].c_str();
        NativeHelper::callSendSMS(dauSo, cuPhap);
#endif
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        const char* dauSo = this->lstdauso[this->currMenhgia].c_str();
        const char* cuPhap = this->lstcuphap[this->currMenhgia].c_str();
        IOS::trySendSMS(string(dauSo), string(cuPhap));

#endif
}
void LayerNapSMS::onBtnBack(Ref* pSender){
    this->pnlContent->setVisible(true);
    this->pnlNap->setVisible(false);
}
void LayerNapSMS::onBtnNap(Ref* pSender){
    this->pnlContent->setVisible(false);
    this->pnlNap->setVisible(true);
    this->txtTyGia->setString(this->lstMenhGia[this->currMenhgia]->getTitleText() + " = "+this->lsttiennhanduoc[this->currMenhgia]);
    this->txtCuPhap->setString(this->lstcuphap[this->currMenhgia]);
}
void LayerNapSMS::onMenhGia(Ref* pSender){
    auto sender = (Button*)pSender;
    int tag = sender->getTag();
    
    this->isShowSelectMenhGia = false;
    this->scrollView->setVisible(false);
    this->currMenhgia = tag-1;
    
    this->btnMenhGia->setTitleText(this->lstMenhGia[this->currMenhgia]->getTitleText());

    
}
void LayerNapSMS::onBtnMenhGia(Ref* pSender){
    
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
void LayerNapSMS::hideLayer(){
    this->setVisible(false);

}
void LayerNapSMS::showLayer(){
    this->setVisible(true);

    this->onBtnBack(NULL);
}
void LayerNapSMS::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
}

void LayerNapSMS::onBtnNapSMS(Ref* pSender){
    
    
}
void LayerNapSMS::onBtnNhaMang(Ref* pSender){
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
void LayerNapSMS::onBtnViettel(Ref* pSender){
    auto sender = (Button*)pSender;
    this->typeNhamang = VIETTEL;
    this->btnNhaMang->setTitleText(sender->getTitleText());
    this->scrollView->setVisible(false);

    this->isShowSelectNhaMang = false;
    this->btnViettel->setVisible(false);
    this->btnVina->setVisible(false);
    this->btnMobi->setVisible(false);
    this->loadSMS(this->typeNhamang);
    
}
void LayerNapSMS::onBtnVina(Ref* pSender){
    auto sender = (Button*)pSender;
    this->typeNhamang = VINA;
    this->btnNhaMang->setTitleText(sender->getTitleText());
    
    this->isShowSelectNhaMang = false;
    this->btnViettel->setVisible(false);
    this->btnVina->setVisible(false);
    this->btnMobi->setVisible(false);
    this->loadSMS(this->typeNhamang);

}
void LayerNapSMS::onBtnMobi(Ref* pSender){
    auto sender = (Button*)pSender;
    this->typeNhamang = MOBI;
    this->btnNhaMang->setTitleText(sender->getTitleText());
    
    this->isShowSelectNhaMang = false;
    this->btnViettel->setVisible(false);
    this->btnVina->setVisible(false);
    this->btnMobi->setVisible(false);
    this->loadSMS(this->typeNhamang);

}
//bool LayerNapSMS::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
//{
//    return true;
//}

void LayerNapSMS::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerNapSMS::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerNapSMS::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool ItemTyGiaSMS::init()
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
void ItemTyGiaSMS::setDatas(int idx,int menhgia, int giamua){
    if (idx%2 ==1 ){
        this->imgBg->loadTexture("moc3-table-tbody-2.png");
    }
    this->txtMenhGia->setString(mUtils::convertMoneyEx(menhgia));
    this->txtGiaBan->setString(mUtils::convertMoneyEx(giamua));
}

