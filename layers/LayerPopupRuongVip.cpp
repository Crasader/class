//
//  LayerPopupRuongVip.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/10/16.
//
//

#include "LayerPopupRuongVip.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "LayerKetQuaRuongVip.h"
#include "../mUtils.h"
#include "../layergames/_Chat_.h"
#include "../scenes/SceneMain.h"
#include "LayerMainBottom.h"
#include "LayerMainTop.h"
#include "../layergames/PhomMessDef.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
USING_NS_CC;
using namespace cocos2d::ui;

LayerPopupRuongVip::LayerPopupRuongVip()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupRuongVip::~LayerPopupRuongVip()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupRuongVip::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupRuongVip.csb");
    auto bg = static_cast<Layout*>(rootNode->getChildByName("Panel_7"));
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    bg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupRuongVip::onBtnClose, this));
    }

    txtNumKey = static_cast<Text*>(pnlBg->getChildByName("lblKey"));
    
    txtValue = static_cast<Text*>(pnlBg->getChildByName("lblMoney"));
    btnTabThongTin = dynamic_cast<Button*>(pnlBg->getChildByName("btnThongTin"));
    if (btnTabThongTin) {
        btnTabThongTin->addClickEventListener(CC_CALLBACK_1(LayerPopupRuongVip::onBtnTabThongTin, this));
        btnTabThongTin->setEnabled(false);
    }
    btnTabLichSu = dynamic_cast<Button*>(pnlBg->getChildByName("btnLichSu"));
    if (btnTabLichSu) {
        btnTabLichSu->setTitleText("Top rương");
        btnTabLichSu->addClickEventListener(CC_CALLBACK_1(LayerPopupRuongVip::onBtnTabLichSu, this));
    }
    btnOpenChest = dynamic_cast<Button*>(pnlBg->getChildByName("btnMoRuong"));
    if (btnOpenChest) {
        btnOpenChest->addClickEventListener(CC_CALLBACK_1(LayerPopupRuongVip::onBtnOpenChest, this));
    }
    sprNoKey = static_cast<Sprite*>(pnlBg->getChildByName("imgNoKey"));
    sprNoKey->setVisible(false);
    
    imgChest = static_cast<ImageView*>(pnlBg->getChildByName("Image_5"));
    
    orgPosOfChest = imgChest->getPosition();
    
    auto bgWebview = static_cast<Layout*>(pnlBg->getChildByName("Panel_6"));
    bgWebview->setVisible(true);

    
    imgEffectChest = Sprite::create("chest.png");
    imgEffectChest->setAnchorPoint(Point(0.5,0.5));
    imgEffectChest->setPosition(Point(visibleSize/2));
    imgEffectChest->setScale(1.5);
    this->addChild(imgEffectChest);
    imgEffectChest->setVisible(false);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView = cocos2d::experimental::ui::WebView::create();
    webView->setPosition(Point(0, 0));
    webView->setAnchorPoint(bgWebview->getAnchorPoint());
    webView->setContentSize(bgWebview->getContentSize());
    webView->loadURL(GT_RUONGVIP);
    webView->setScalesPageToFit(true);
    webView->setLocalZOrder(99);
    bgWebview->addChild(webView);
    isOpenning = false;
    webView->setVisible(false);
    
    webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerPopupRuongVip::onWebViewShouldStartLoading, this));
    webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerPopupRuongVip::onWebViewDidFinishLoading, this));
    webView->setOnDidFailLoading(CC_CALLBACK_2(LayerPopupRuongVip::onWebViewDidFailLoading, this));
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(GT_RUONGVIP);
#endif

    setValueOfChest("0", "0");
    
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rdc", params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
    onBtnTabThongTin(NULL);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::logScreenToGA("Đang mở: Rương Vip");
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string screen = "Rương Vip";
    NativeHelper::trackScreen(screen.c_str());
#endif
    return true;
}
void LayerPopupRuongVip::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerPopupRuongVip::onBtnTabThongTin(Ref* pSender){
    btnTabLichSu->setEnabled(true);
    btnTabThongTin->setEnabled(false);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView->loadURL(GT_RUONGVIP);
    webView->setScalesPageToFit(true);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(GT_RUONGVIP);
#endif
}
void LayerPopupRuongVip::onBtnTabLichSu(Ref* pSender){
    btnTabLichSu->setEnabled(false);
    btnTabThongTin->setEnabled(true);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView->loadURL(TOP_MORUONG);
    webView->setScalesPageToFit(true);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(TOP_MORUONG);
#endif
}
void LayerPopupRuongVip::onBtnOpenChest(Ref* pSender){
    
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
    getPartnerInfo();
    
        if (numOfKey == 0) {
        sprNoKey->setVisible(true);
        sprNoKey->setOpacity(255);
        sprNoKey->runAction(FadeOut::create(2.5));
    }
    else{
        this->btnClose->setEnabled(false);
        this->btnOpenChest->setEnabled(false);
        isOpenning = true;
        imgChest->setVisible(false);
        imgEffectChest->setTexture("chest.png");
        imgEffectChest->setVisible(true);
        imgEffectChest->setPosition(Point(visibleSize/2));
        imgEffectChest->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.1,Point(visibleSize.width/2,visibleSize.height/2+20)),MoveTo::create(0.1,Point(visibleSize.width/2+20,visibleSize.height/2+20)),MoveTo::create(0.1,Point(visibleSize.width/2-20,visibleSize.height/2+20)),MoveTo::create(0.1,Point(visibleSize.width/2-20,visibleSize.height/2-20)), NULL)));
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        webView->setVisible(false);
#endif
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt("osid", osid);
        params->PutUtfString("devid", devID);
        params->PutInt("srcref", atoi(srcID.c_str()));
        boost::shared_ptr<IRequest> request(new ExtensionRequest("roc", params));
        GameServer::getSingleton().Send(request);
        
        dataManager.getAppConfig().isOpenChest = true;
    }
}
void LayerPopupRuongVip::getPartnerInfo()
{
    string sPartInfo = "";
    partnerID = "1";
    srcID = "0";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sPartInfo = NativeHelper::getPartnerID();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sPartInfo = IOS::tryGetPartnerID();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    sPartInfo = "1-1";
#endif
    CCLOG("************************** %s", sPartInfo.c_str());
    //split partnerID and srcID
    if (sPartInfo == "")
        sPartInfo = "1-0";
    vector<string> lstPartInfo = mUtils::splitString(sPartInfo, '-');
    if (lstPartInfo.size() < 2){
        return;
    }
    partnerID = lstPartInfo.at(0);
    srcID = lstPartInfo.at(1);
}
void LayerPopupRuongVip::setValueOfChest(string value,string num){
    txtValue->setString(mUtils::convertMoneyEx(atoll(value.c_str())));
    txtNumKey->setString(mUtils::convertMoneyEx(atoll(num.c_str())));
    numOfKey = atoi(num.c_str());
    valueOfChest = atoi(value.c_str());
    if(numOfKey>0){
        sprNoKey->setVisible(false);
    }
}
void LayerPopupRuongVip::showResult(float dt){
    dataManager.getAppConfig().isOpenChest = false;
    imgEffectChest->setPosition(Point(visibleSize/2));
    imgEffectChest->setVisible(false);
    imgEffectChest->stopAllActions();
    imgChest->setVisible(true);
    
    SceneMain* parent = (SceneMain*)this->getParent();
    if (parent){
        LayerMainBottom* btm = (LayerMainBottom*)(parent->getChildByTag(parent->TAG_BOTTOM));
        if (btm)
            btm->loadAllDatas();
        LayerMainTop* top = (LayerMainTop*)(parent->getChildByTag(parent->TAG_TOP));
        if (top)
            top->setValueMenuRuongVip(lstValue[0],lstValue[1]);
    }
    LayerKetQuaRuongVip* layer = LayerKetQuaRuongVip::create();
    layer->setMoney(StringUtils::format("%d",moneyBonus));
    this->addChild(layer);
    this->btnClose->setEnabled(true);
    this->btnOpenChest->setEnabled(true);
    if (lstValue.size()<=0) return;
    setValueOfChest(lstValue[0],lstValue[1]);
}
void LayerPopupRuongVip::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    this->removeFromParentAndCleanup(true);
}
void LayerPopupRuongVip::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("rsldc", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        if (param->GetUtfString("detailcoffer") == NULL) return;
        string dc = *param->GetUtfString("detailcoffer");
        auto lstdc = mUtils::splitString(dc, '|');
        if (lstdc.size()<=0) return;
        setValueOfChest(lstdc[0],lstdc[1]);
    }
    else if (strcmp("rsoc", cmd->c_str()) == 0){
       // log("%ld",*param->GetInt("rc"));
        int rc = *param->GetInt("rc");
        if (rc == 0){
            if (param->GetUtfString("detailcoffer") == NULL || param->GetInt("coinBonus") == NULL) return;
            string dc = *param->GetUtfString("detailcoffer");
            moneyBonus = *param->GetInt("coinBonus");
            lstValue = mUtils::splitString(dc, '|');
            //this->scheduleOnce(schedule_selector(LayerPopupRuongVip::showResult), 2);
                Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupRuongVip::showResult), this, 2, 1, 2, false);
        }
        else{
            if (param->GetUtfString("detailcoffer") == NULL || param->GetInt("coinBonus") == NULL)// log("loi me no roi");
            return;
            //log("%s",*param->GetUtfString("detailcoffer")->c_str());
            //log("%d",*param->GetInt("coinBonus"));
            isOpenning = false;
            imgEffectChest->setTexture("light-bg.png");
            imgEffectChest->setVisible(false);
            imgChest->setVisible(true);
            this->btnClose->setEnabled(true);
            this->btnOpenChest->setEnabled(true);
            Chat* chat = Chat::create("Mở rương thất bại, vui lòng thử lại!",-1);
            this->addChild(chat);
        }
    }

    
}

void LayerPopupRuongVip::showWebview(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView->reload();
#endif
}


bool LayerPopupRuongVip::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerPopupRuongVip::onEnter()
{
    Layer::onEnter();
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(LayerPopupRuongVip::onTouchBegan, this);
        dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupRuongVip::onExit()
{
    Layer::onExit();
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerPopupRuongVip::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
   // SceneManager::getSingleton().showLoading();
    webView->setVisible(false);
    return true;
    
}

void LayerPopupRuongVip::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
   // SceneManager::getSingleton().hideLoading();
    if(!isOpenning)
        webView->setVisible(true);
}

void LayerPopupRuongVip::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    webView->setVisible(false);
    //SceneManager::getSingleton().hideLoading();
}
#endif



