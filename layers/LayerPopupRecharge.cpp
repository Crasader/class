#include "LayerPopupRecharge.h"

#include "LayerPopupThongBao.h"
#include "../layergames/_Chat_.h"
#include "../data_manager.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/ChanUtils.h"
#include "../layergames/PhomMessDef.h"
#include "Requests/ExtensionRequest.h"
#include "layergames/_Chat_.h"
#include <vector>
#include "json/rapidjson.h"
#include "json/document.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
#define TIME_DISABLE 5
LayerPopupRecharge::LayerPopupRecharge()
{
}

LayerPopupRecharge::~LayerPopupRecharge()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupRecharge::init()
{
    if (!Layer::init())
    {
        return false;
    }
	this->gameID = SceneManager::getSingleton().getGameID();
	string pathCSB = "LayerPopupRechargeInGame.csb";
	if (gameID == kGameCoTuong || gameID == kGameCoUp || gameID == kGameChan)
		pathCSB = "LayerPopupRechargeChess.csb";
   
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto root = CSLoader::getInstance()->createNode(pathCSB);
    auto pnlBg = static_cast<Layout*>(root->getChildByName("Panel_3"));
    pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    if (gameID == kGameChan){
        root->setAnchorPoint(Point::ZERO);
        root->setPosition(Point::ZERO);
    }
    else{
        root->setAnchorPoint(Point(0.5, 0.5));
        root->setPosition(Point(visibleSize / 2));
    }
    pnlBg->setOpacity(200);
    pnlBg->setBackGroundColor(Color3B::BLACK);
    
    ui::Helper::doLayout(root);
    addChild(root);
    
    // 	lblPhoneHotLine = dynamic_cast<Text*>(root->getChildByName("lblPhoneNumber"));
    // 	strHotLine = dataManager.getAppConfig().callcenter + "";
    // 	lblPhoneHotLine->setString(strHotLine);
    // 	btnHotLine = dynamic_cast<Button*>(root->getChildByName("btnHotLine"));
    // 	if (btnHotLine != NULL){
    // 		btnHotLine->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnHotLine, this));
    // 		btnHotLine->setVisible(!dataManager.getAppConfig().isHideBilling);
    // 	}
    Size sizeAdd = ChanUtils::getSizePos();
    auto btnClose = dynamic_cast<Button*>(root->getChildByName("btnClose"));
    if (btnClose != NULL) {
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnClose, this));
        btnClose->setPressedActionEnabled(true);
    }
    ///MINHDV CHANGES - 14/4/2016 - add tab IAP
    auto pnlWebView = dynamic_cast<Layout*>(root->getChildByName("pnlWebView"));
    pnlWebView->setVisible(false);
    
    btnIAP = (Button*)(root->getChildByName("btnIAP"));
    if (btnIAP != NULL){
        btnIAP->setTitleText("IAP");
        layerIAP = LayerPopupPurcharse::create();
        layerIAP->setAnchorPoint(Vec2::ZERO);
        if (gameID != kGameChan)
            layerIAP->setPosition(Vec2(0, -50));
        else
            layerIAP->setPosition(Vec2(pnlWebView->getPosition().x, pnlWebView->getPosition().y));
        
        this->addChild(layerIAP);
        
        btnIAP->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnIAP, this));
        btnIAP->setPressedActionEnabled(true);
    }
    
    btnIAP->setTitleText(StringUtils::format("Mua %s", SceneManager::getSingleton().getCoin(0).c_str()));
    
    btnTheCao = dynamic_cast<Button*>(root->getChildByName("btnTheCao"));
    if (btnTheCao != NULL){
        btnTheCao->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnTheCao, this));
        btnTheCao->setPressedActionEnabled(true);
    }
    btnSMS = (Button*)root->getChildByName("btnSMS");
    if (btnSMS)
    {
        layerSMS = LayerPopupSMS::create();
        layerSMS->setAnchorPoint(Vec2::ZERO);
        if (gameID != kGameChan)
            layerSMS->setPosition(Vec2(pnlWebView->getPosition().x, pnlWebView->getPosition().y + sizeAdd.height));
        else
            layerSMS->setPosition(Vec2(pnlWebView->getPosition().x, pnlWebView->getPosition().y));
        this->addChild(layerSMS);
        
        btnSMS->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnSMS, this));
        btnSMS->setPressedActionEnabled(true);
    }
    
    btnTyGia = dynamic_cast<Button*>(root->getChildByName("btnTyGia"));
    if (btnTyGia){
        btnTyGia->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnTyGia, this));
        btnTyGia->setPressedActionEnabled(true);
    }
    btnVIP = dynamic_cast<Button*>(root->getChildByName("btnVip"));
    if (btnVIP){
        btnVIP->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnVIP, this));
        btnVIP->setPressedActionEnabled(true);
    }
    btnHistory = dynamic_cast<Button*>(root->getChildByName("btnHistories"));
    if (btnHistory != NULL){
        btnHistory->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnHistory, this));
        btnHistory->setPressedActionEnabled(true);
    }
    
    nodeTheCao = dynamic_cast<Node*>(root->getChildByName("NodeTheCao"));
    lblPromotion = dynamic_cast<Text*>(nodeTheCao->getChildByName("lblPromotion"));
    lblPromotion->setVisible(false);
    lblErrorMaThe = static_cast<Text*>(nodeTheCao->getChildByName("lblErrorMaThe"));
    lblErrorMaThe->setVisible(false);
    lblErrorSeri = static_cast<Text*>(nodeTheCao->getChildByName("lblErrorSoSeri"));
    lblErrorSeri->setVisible(false);
    btnMobi = dynamic_cast<Button*>(nodeTheCao->getChildByName("btnMobi"));
    btnMobi->setEnabled(true);
    if (btnMobi){
        auto img_mobi = static_cast<ImageView*>(nodeTheCao->getChildByName("img_mobi"));
        if (img_mobi)
            img_mobi->setVisible(false);
        btnMobi->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onButtonMobi, this));
        btnMobi->setPressedActionEnabled(true);
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
            btnMobi->setTitleText("Mobiphone");
        btnMobi->setTitleColor(Color3B::BLACK);
    }
    btnViettel = dynamic_cast<Button*>(nodeTheCao->getChildByName("btnViettel"));
    if (btnViettel){
        auto img_viett = static_cast<ImageView*>(nodeTheCao->getChildByName("img_viett"));
        if (img_viett)
            img_viett->setVisible(false);
        btnViettel->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onButtonViettel, this));
        btnViettel->setPressedActionEnabled(true);
         if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        btnViettel->setTitleText("Viettel");
         
        btnViettel->setTitleColor(Color3B::BLACK);
         }
        
    }
    btnVina = dynamic_cast<Button*>(nodeTheCao->getChildByName("btnVina"));
    if (btnVina){
        auto img_vina = static_cast<ImageView*>(nodeTheCao->getChildByName("img_mobi"));
        if (img_vina)
            img_vina->setVisible(false);
        btnVina->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onButtonVina, this));
        btnVina->setPressedActionEnabled(true);
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
            btnVina->setTitleText("Vinaphone");
        btnVina->setTitleColor(Color3B::BLACK);
        
    }
    btnOK = dynamic_cast<Button*>(nodeTheCao->getChildByName("btnOK"));
    if (btnOK){
        btnOK->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onButtonOK, this));
        btnOK->setPressedActionEnabled(true);
    }
    
    btnCSVIP = dynamic_cast<Button*>(nodeTheCao->getChildByName("btnCSVip"));
    if (btnCSVIP){
        btnCSVIP->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onButtonVip, this));
        btnCSVIP->setPressedActionEnabled(true);
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
            btnCSVIP->setVisible(false);
        }
    }
    
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
        auto btnGiftCode = ui::Button::create();
        btnGiftCode->loadTextures("designgopro/guifiles/btn-gift-code.png", "designgopro/guifiles/btn-gift-code.png", "designgopro/guifiles/btn-gift-code.png");
        btnGiftCode->setAnchorPoint(Point(0.5, 0.5));
        btnGiftCode->setPosition(Point(960, 70));
        this->addChild(btnGiftCode);
        btnGiftCode->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onButtonGiftCode, this));
    }
    
    // 	btnTheCao->setBright(false);
    // 	btnTheCao->setEnabled(false);
    txtSoThe = dynamic_cast<TextField*>(nodeTheCao->getChildByName("txtSeri"));
    txtMaThe = dynamic_cast<TextField*>(nodeTheCao->getChildByName("txtMaThe"));
    
    // 	txtMaThe->setMaxLengthEnabled(true);
    // 	txtSoThe->setMaxLengthEnabled(true);
    txtSoThe->setMaxLength(15);
    txtMaThe->setMaxLength(20);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    txtMaThe->setInputMode(ui::EditBox::InputMode::NUMERIC);
#endif
    
    GameServer::getSingleton().addListeners(this);
    //
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto posX = pnlWebView->getPositionX();
    auto posY = pnlWebView->getPositionY() + distance;
    sizeAdd = Size(0, 0);
    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameCoTuong || SceneManager::getSingleton().getGameID() == kGameCoUp)
        sizeAdd = ChanUtils::getSizePos();
    
    webView = cocos2d::experimental::ui::WebView::create();
    webView->setAnchorPoint(Vec2(0, 0));
    webView->setPosition(Vec2(posX, posY - sizeAdd.height));
    webView->setContentSize(Size(pnlWebView->getContentSize().width, pnlWebView->getContentSize().height));
    //webView->loadURL(TYGIA_LINK);
    //webView->setContentSize(Size(lsvHistories->getContentSize().width * ratio, lsvHistories->getContentSize().height * ratio));
    webView->setScalesPageToFit(true);
    this->addChild(webView);
    webView->setVisible(false);
    
    webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerPopupRecharge::onWebViewShouldStartLoading, this));
    webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerPopupRecharge::onWebViewDidFinishLoading, this));
    webView->setOnDidFailLoading(CC_CALLBACK_2(LayerPopupRecharge::onWebViewDidFailLoading, this));
#endif
    
    // 	//check flag an hien IAP - the cao - sms
    //dataManager.getAppConfig().isHideBilling = true;
    // 	dataManager.getAppConfig().isHideTheCao = true;
    //	if (dataManager.getAppConfig().isHideBilling)
    //	{
    //
    //		this->onBtnIAP(btnIAP);
    //	}
    //	if (dataManager.getAppConfig().isHideSMS)
    //	{
    //		btnSMS->setVisible(false);
    //		this->onBtnTheCao(btnTheCao);
    //	}
    //	if (dataManager.getAppConfig().isHideTheCao)
    //	{
    //		btnTheCao->setVisible(false);
    //		this->onBtnIAP(btnIAP);
    //	}
    //	else if (!dataManager.getAppConfig().isHideBilling){
    //		this->onBtnTheCao(btnTheCao);
    //	}
    
    btnIAP->setVisible(false);
    layerIAP->setVisible(false);
    
    btnTheCao->setVisible(false);
    btnVIP->setVisible(false);
    nodeTheCao->setVisible(false);
    btnTyGia->setVisible(false);
    btnHistory->setVisible(false);
    
    btnSMS->setVisible(false);
    layerSMS->setVisible(false);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("pmchkrq", params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
    
    if (dataManager.getAppConfig().isHideBilling){
        btnIAP->setVisible(true);
        this->onBtnIAP(NULL);
        
    }
    
    // 	btnSMS = dynamic_cast<Button*>(root->getChildByName("btnSMS"));
    // 	if (btnSMS){
    // 		btnSMS->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnSMS, this));
    // 		btnSMS->setPressedActionEnabled(true);
    // 	}
    // 	btnTheCao = dynamic_cast<Button*>(root->getChildByName("btnTheCao"));
    // 	if (btnTheCao){
    // 		btnTheCao->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnTheCao, this));
    // 		btnTheCao->setPressedActionEnabled(true);
    // 	}
    // 	btnGiftCode = dynamic_cast<Button*>(root->getChildByName("btnGiftCode"));
    // 	if (btnGiftCode){
    // 		btnGiftCode->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnGiftCode, this));
    // 		btnGiftCode->setPressedActionEnabled(true);
    // 	}
    // 	btnHistory = dynamic_cast<Button*>(root->getChildByName("btnHistories"));
    // 	if (btnHistory){
    // 		btnHistory->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnHistory, this));
    // 		btnHistory->setPressedActionEnabled(true);
    // 	}
    //
    // 	lsvHistories = dynamic_cast<ListView*>(root->getChildByName("lsvHistories"));
    // 	lsvHistories->setVisible(false);
    // 	//lsvHistories->setBackGroundColor(Color3B::RED);
    //
    // 	btnTyGia = dynamic_cast<Button*>(root->getChildByName("btntyGia"));
    // 	if (btnTyGia){
    // 		btnTyGia->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnTyGia, this));
    // 		btnTyGia->setPressedActionEnabled(true);
    // 	}
    //
    // 	nodeSMS = dynamic_cast<Node*>(root->getChildByName("NodeSMS"));
    // 	btn10K = dynamic_cast<Button*>(nodeSMS->getChildByName("btn10k"));
    // 	if (btn10K)
    // 	{
    // 		btn10K->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnNap10k, this));
    // 		btn10K->setPressedActionEnabled(true);
    // 	}
    // 	btn15K = dynamic_cast<Button*>(nodeSMS->getChildByName("btn15k"));
    // 	if (btn15K)
    // 	{
    // 		btn15K->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnNap15k, this));
    // 		btn15K->setPressedActionEnabled(true);
    // 	}
    // 	btn3K = dynamic_cast<Button*>(nodeSMS->getChildByName("btn3k"));
    // 	if (btn3K)
    // 	{
    // 		btn3K->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnNap3k, this));
    // 		btn3K->setPressedActionEnabled(true);
    // 	}
    // 	auto btnNapnhanh = dynamic_cast<Button*>(nodeSMS->getChildByName("btnFastCharge"));
    // 	if (btnNapnhanh)
    // 	{
    // 		btnNapnhanh->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnNapTien, this));
    // 		btnNapnhanh->setPressedActionEnabled(true);
    // 	}
    // 	lblMoneyReceived = dynamic_cast<Text*>(nodeSMS->getChildByName("lblMoneyReceived"));
    // 	lblPromotionSMS = static_cast<Text*>(nodeSMS->getChildByName("lblPromotion"));
    // 	lblCuphap = dynamic_cast<Text*>(nodeSMS->getChildByName("lblCuphap"));
    // 	lblDauSo = dynamic_cast<Text*>(nodeSMS->getChildByName("lblDauSo"));
    // 	lblPromotionSMS->setVisible(false);
    //    	//////////////////////////////////////////////////////////////////////////
    //
    // 	nodeTheCao = dynamic_cast<Node*>(root->getChildByName("NodeTheCao"));
    // 	lblPromotion = dynamic_cast<Text*>(nodeTheCao->getChildByName("lblPromotion"));
    // 	lblPromotion->setVisible(false);
    // 	lblErrorMaThe = static_cast<Text*>(nodeTheCao->getChildByName("lblErrorMaThe"));
    // 	lblErrorMaThe->setVisible(false);
    // 	lblErrorSeri = static_cast<Text*>(nodeTheCao->getChildByName("lblErrorSeri"));
    // 	lblErrorSeri->setVisible(false);
    // 	btnMobi = dynamic_cast<Button*>(nodeTheCao->getChildByName("btnMobi"));
    //     btnMobi->setEnabled(true);
    // 	if (btnMobi){
    // 		btnMobi->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onButtonMobi, this));
    // 		btnMobi->setPressedActionEnabled(true);
    // 	}
    // 	btnViettel = dynamic_cast<Button*>(nodeTheCao->getChildByName("btnViettel"));
    // 	if (btnViettel){
    // 		btnViettel->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onButtonViettel, this));
    // 		btnViettel->setPressedActionEnabled(true);
    // 	}
    // 	btnVina = dynamic_cast<Button*>(nodeTheCao->getChildByName("btnVina"));
    // 	if (btnVina){
    // 		btnVina->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onButtonVina, this));
    // 		btnVina->setPressedActionEnabled(true);
    // 	}
    // 	btnOK = dynamic_cast<Button*>(nodeTheCao->getChildByName("btnOK"));
    // 	if (btnOK){
    // 		btnOK->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onButtonOK, this));
    // 		btnOK->setPressedActionEnabled(true);
    // 	}
    // 	txtSoThe = dynamic_cast<TextField*>(nodeTheCao->getChildByName("txtSeri"));
    // 	txtMaThe = dynamic_cast<TextField*>(nodeTheCao->getChildByName("txtMaThe"));
    // #if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // 	txtMaThe->setInputMode(ui::EditBox::InputMode::NUMERIC);
    // #endif
    // // 	lblPromotion = dynamic_cast<Text*>(nodeTheCao->getChildByName("lblPromotion"));
    // //     lblPromotion->setVisible(false);
    //     lblFastCharge = dynamic_cast<Text*>(nodeTheCao->getChildByName("lblFastCharge"));
    //
    // 	//////////////////////////////////////////////////////////////////////////
    //
    // 	nodeGiftCode = dynamic_cast<Node*>(root->getChildByName("NodeGiftCode"));
    // 	lblReportGiftCode = static_cast<Text*>(nodeGiftCode->getChildByName("lblReport"));
    // 	lblReportGiftCode->setVisible(false);
    // 	txtGiftCode = dynamic_cast<TextField*>(nodeGiftCode->getChildByName("txtGifCode"));
    //
    // 	auto btn_nhapgiftcode = dynamic_cast<Button*>(nodeGiftCode->getChildByName("btnAccept"));
    // 	if (btn_nhapgiftcode){
    // 		btn_nhapgiftcode->addClickEventListener(CC_CALLBACK_1(LayerPopupRecharge::onBtnNhapGiftCode, this));
    // 		btn_nhapgiftcode->setPressedActionEnabled(true);
    // 	}
    //
    // 	GameServer::getSingleton().addListeners(this);
    // 	//
    // #if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // 	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    // 	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    // 	auto posX = lsvHistories->getPositionX();
    // 	auto posY = lsvHistories->getPositionY() + distance;
    //
    //     auto sizeAdd = Size(0,0);
    //     if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameCoTuong || SceneManager::getSingleton().getGameID() == kGameCoUp)
    //             sizeAdd = ChanUtils::getSizePos();
    //
    // 	webView = cocos2d::experimental::ui::WebView::create();
    // 	webView->setAnchorPoint(Vec2(0, 1));
    // 	webView->setPosition(Vec2(posX, posY-sizeAdd.height));
    // 	webView->setContentSize(Size(lsvHistories->getContentSize().width, lsvHistories->getContentSize().height));
    // 	webView->loadURL(TYGIA_LINK);
    // 	//webView->setContentSize(Size(lsvHistories->getContentSize().width * ratio, lsvHistories->getContentSize().height * ratio));
    // 	webView->setScalesPageToFit(true);
    // 	this->addChild(webView);
    //
    // 	webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerPopupRecharge::onWebViewShouldStartLoading, this));
    // 	webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerPopupRecharge::onWebViewDidFinishLoading, this));
    // 	webView->setOnDidFailLoading(CC_CALLBACK_2(LayerPopupRecharge::onWebViewDidFailLoading, this));
    // #endif
    // 	loadAllDatas();
    // 	onBtnSMS(btnSMS);
    // 	onBtnNap10k(btn10K);
    //
    //
    //
    //
    return true;
}
bool LayerPopupRecharge::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupRecharge::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupRecharge::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerPopupRecharge::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void LayerPopupRecharge::onExit()
{
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
}

void LayerPopupRecharge::onBtnHotLine(Ref* pSender){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    NativeHelper::callPhone(strHotLine.c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::tryToCall(strHotLine);
#endif
}

void LayerPopupRecharge::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerPopupRecharge::onBtnIAP(Ref* pSender)
{
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        currTab = 0;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
            /*btnIAP->setTitleColor(ccc3(255, 255, 255));
             btnTheCao->setTitleColor(ccc3(0, 0, 0));
             btnVIP->setTitleColor(ccc3(0, 0, 0));
             btnSMS->setTitleColor(ccc3(0, 0, 0));
             btnHistory->setTitleColor(ccc3(0, 0, 0));
             btnTyGia->setTitleColor(ccc3(0, 0, 0));*/
        }
        
        btnIAP->setEnabled(false);
        btnTheCao->setEnabled(true);
        btnSMS->setEnabled(true);
        btnHistory->setEnabled(true);
        btnTyGia->setEnabled(true);
        btnVIP->setEnabled(true);
        
        
        layerSMS->setVisible(false);
        layerIAP->setVisible(true);
        nodeTheCao->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
        webView->setVisible(false);
#endif
        
    }
    else{
        if (!dataManager.getAppConfig().isHideBilling){
            if (isShowLocalGlobal){
                currTab = 0;
                btnIAP->setEnabled(false);
                btnTheCao->setEnabled(true);
                btnSMS->setEnabled(true);
                btnHistory->setEnabled(true);
                btnTyGia->setEnabled(true);
                btnVIP->setEnabled(true);
                
                
                layerSMS->setVisible(false);
                layerIAP->setVisible(false);
                nodeTheCao->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
                webView->setVisible(true);
                webView->loadURL(LINK_DS_DAILY);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                Application::getInstance()->openURL(LINK_DS_DAILY);
#endif
            }
            else{
                currTab = 0;
                btnIAP->setEnabled(false);
                btnTheCao->setEnabled(true);
                btnSMS->setEnabled(true);
                btnHistory->setEnabled(true);
                btnTyGia->setEnabled(true);
                btnVIP->setEnabled(true);
                
                
                layerSMS->setVisible(false);
                layerIAP->setVisible(true);
                nodeTheCao->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
                webView->setVisible(false);
#endif
                
            }
            
        }
        else{
            currTab = 0;
            btnIAP->setEnabled(false);
            btnTheCao->setEnabled(true);
            btnSMS->setEnabled(true);
            btnHistory->setEnabled(true);
            btnTyGia->setEnabled(true);
            btnVIP->setEnabled(true);
            
            
            layerSMS->setVisible(false);
            layerIAP->setVisible(true);
            nodeTheCao->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
            webView->setVisible(false);
#endif
            
        }
        
    }
}
void LayerPopupRecharge::onBtnTheCao(Ref* pSender){
    currTab = 1;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        /*btnTheCao->setTitleColor(ccc3(255, 255, 255));
         btnVIP->setTitleColor(ccc3(0, 0, 0));
         btnIAP->setTitleColor(ccc3(0, 0, 0));
         btnSMS->setTitleColor(ccc3(0, 0, 0));
         btnHistory->setTitleColor(ccc3(0, 0, 0));
         btnTyGia->setTitleColor(ccc3(0, 0, 0));*/
    }
    btnIAP->setEnabled(true);
    btnTheCao->setEnabled(false);
    btnSMS->setEnabled(true);
    btnHistory->setEnabled(true);
    btnTyGia->setEnabled(true);
    btnVIP->setEnabled(true);
    
    
    layerSMS->setVisible(false);
    layerIAP->setVisible(false);
    nodeTheCao->setVisible(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    webView->setVisible(false);
#endif
}
void LayerPopupRecharge::onBtnSMS(Ref* pSender)
{
    currTab = 2;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        /*btnSMS->setTitleColor(ccc3(255, 255, 255));
         btnTheCao->setTitleColor(ccc3(0, 0, 0));
         btnIAP->setTitleColor(ccc3(0, 0, 0));
         btnVIP->setTitleColor(ccc3(0, 0, 0));
         btnHistory->setTitleColor(ccc3(0, 0, 0));
         btnTyGia->setTitleColor(ccc3(0, 0, 0));*/
    }
    btnIAP->setEnabled(true);
    btnTheCao->setEnabled(true);
    btnSMS->setEnabled(false);
    btnHistory->setEnabled(true);
    btnTyGia->setEnabled(true);
    btnVIP->setEnabled(true);
    
    
    layerSMS->setVisible(true);
    layerIAP->setVisible(false);
    nodeTheCao->setVisible(false);
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        // 		auto popupSms = LayerPopupSMS::create();
        // 		popupSms->onButtonMobi(NULL);
        layerSMS->onButtonMobi(NULL);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    webView->setVisible(false);
#endif
}
void LayerPopupRecharge::onBtnHistory(Ref* pSender){
    SceneManager::getSingleton().showLoading();
    currTab = 3;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        /*btnHistory->setTitleColor(ccc3(255, 255, 255));
         btnTheCao->setTitleColor(ccc3(0, 0, 0));
         btnIAP->setTitleColor(ccc3(0, 0, 0));
         btnSMS->setTitleColor(ccc3(0, 0, 0));
         btnVIP->setTitleColor(ccc3(0, 0, 0));
         btnTyGia->setTitleColor(ccc3(0, 0, 0));*/
    }
    btnIAP->setEnabled(true);
    btnTheCao->setEnabled(true);
    btnSMS->setEnabled(true);
    btnHistory->setEnabled(false);
    btnTyGia->setEnabled(true);
    btnVIP->setEnabled(true);
    
    
    layerSMS->setVisible(false);
    layerIAP->setVisible(false);
    nodeTheCao->setVisible(false);
    /*getUser*/
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
    if (myself == NULL || myself->Name() == NULL)
        return;
    /*init linkURL*/
    string urlLinkHistories = "";
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        urlLinkHistories = HISTORY_P2_LINK;
    }
    else{
        urlLinkHistories = HISTORY_LINK;
    }
    urlLinkHistories += *myself->Name();
    urlLinkHistories += "&checksum=";
    urlLinkHistories += mUtils::getMD5KeyHis(*myself->Name());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    webView->loadURL(urlLinkHistories);
    webView->setVisible(false);
    //webView->setVisible(false);mUlti
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    Application::getInstance()->openURL(urlLinkHistories);
#endif
}
void LayerPopupRecharge::onBtnTyGia(Ref* pSender)
{
    SceneManager::getSingleton().showLoading();
    currTab = 4;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        /*btnTyGia->setTitleColor(ccc3(255, 255, 255));
         btnTheCao->setTitleColor(ccc3(0, 0, 0));
         btnIAP->setTitleColor(ccc3(0, 0, 0));
         btnSMS->setTitleColor(ccc3(0, 0, 0));
         btnHistory->setTitleColor(ccc3(0, 0, 0));
         btnVIP->setTitleColor(ccc3(0, 0, 0));*/
    }
    
    btnIAP->setEnabled(true);
    btnTheCao->setEnabled(true);
    btnSMS->setEnabled(true);
    btnHistory->setEnabled(true);
    btnTyGia->setEnabled(false);
    btnVIP->setEnabled(true);
    
    layerSMS->setVisible(false);
    layerIAP->setVisible(false);
    nodeTheCao->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    /*getUser*/
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL || myself->Name() == NULL)
        return;
    string url="";
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        url= TYGIA_P2_LINK;
        string myName = SceneManager::getSingleton().getMyName();
        url += myName + "&checksum=" + mUtils::getMD5KeyHis(myName);
        webView->loadURL(url);
    }
    else{
        webView->loadURL(TYGIA_LINK);
    }
    
    webView->setVisible(false);
    
    //webView->setVisible(false);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL || myself->Name() == NULL)
        return;
    string url = "";
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        url = TYGIA_P2_LINK;
        string myName = SceneManager::getSingleton().getMyName();
        url += myName + "&checksum=" + mUtils::getMD5KeyHis(myName);
        Application::getInstance()->openURL(url);
    }
    else{
        Application::getInstance()->openURL(TYGIA_LINK);
    }
#endif
}
void LayerPopupRecharge::onButtonGiftCode(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    /*getUser*/
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL || myself->Name() == NULL)
        return;
    string strlink = LINK_NHAP_GIFTCODE;
    string username = *myself->Name();
    strlink += username + "&checksum=" + mUtils::getMD5KeyHis(username);
    LayerPopupWebView* _layer = LayerPopupWebView::create();
    _layer->setWebView(strlink);
    _currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_WEBVIEW,9050);
}
void LayerPopupRecharge::onBtnVIP(Ref* pSender)
{
    SceneManager::getSingleton().showLoading();
    currTab = 5;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        /*btnVIP->setTitleColor(ccc3(255, 255, 255));
         btnTheCao->setTitleColor(ccc3(0, 0, 0));
         btnIAP->setTitleColor(ccc3(0, 0, 0));
         btnSMS->setTitleColor(ccc3(0, 0, 0));
         btnHistory->setTitleColor(ccc3(0, 0, 0));
         btnTyGia->setTitleColor(ccc3(0, 0, 0));*/
    }
    btnIAP->setEnabled(true);
    btnTheCao->setEnabled(true);
    btnSMS->setEnabled(true);
    btnHistory->setEnabled(true);
    btnTyGia->setEnabled(true);
    btnVIP->setEnabled(false);
    
    layerSMS->setVisible(false);
    layerIAP->setVisible(false);
    nodeTheCao->setVisible(false);
    /*getUser*/
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL || myself->Name() == NULL)
        return;
    boost::shared_ptr<UserVariable> accVipType = myself->GetVariable("accVipType");
    if (accVipType == NULL)
        return;
    /*init linkURL*/
    int __vip = *accVipType->GetIntValue();
    double money = 0;
    if (myself->GetVariable("am") != NULL){
        money = *myself->GetVariable("am")->GetDoubleValue();
    }
    
    string moneystr = StringUtils::format("%d", int(money));
    string csvipurl = "";
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        csvipurl = CS_P2_VIP;
        csvipurl += *myself->Name() + "&checksum=" + mUtils::getMD5KeyHis(*myself->Name());
        csvipurl += "v=" + StringUtils::format("%d", __vip);
    }
    else{
        csvipurl = CS_VIP;
        csvipurl += "u=" + *myself->Name();
        csvipurl += "&c=";
        csvipurl += mUtils::getMD5KeyHis(*myself->Name());
        csvipurl += "&v=" + StringUtils::format("%d", __vip);
        csvipurl += "&t=" + moneystr;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    webView->loadURL(csvipurl);
    //webView->setVisible(false);
    webView->setVisible(false);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    Application::getInstance()->openURL(csvipurl);
#endif
}

void LayerPopupRecharge::onButtonMobi(Ref* pSender)
{
    btnMobi->setEnabled(false);
    btnViettel->setEnabled(true);
    btnVina->setEnabled(true);
    lblPromotion->setVisible(false);
    
}

void LayerPopupRecharge::onButtonVina(Ref* pSender)
{
    btnMobi->setEnabled(true);
    btnViettel->setEnabled(true);
    btnVina->setEnabled(false);
    lblPromotion->setVisible(false);
}

void LayerPopupRecharge::onButtonViettel(Ref* pSender)
{
    btnMobi->setEnabled(true);
    btnViettel->setEnabled(false);
    btnVina->setEnabled(true);
    lblPromotion->setVisible(false);
}
void LayerPopupRecharge::onButtonVip(Ref* pSender){
    this->onBtnVIP(NULL);
}
void LayerPopupRecharge::onButtonOK(Ref* pSender){
   // log("onButtonOKClick");
    lblPromotion->setString("");
    lblErrorMaThe->setString("");
    //
    std::string issues = "";
    if (!btnMobi->isEnabled())
        issues = string("2");
    else if (!btnVina->isEnabled())
        issues = string("1");
    else if (!btnViettel->isEnabled())
        issues = string("3");
    
    if (strcmp(issues.c_str(), "") == 0)
    {
        CCLOG("Chua chon nha Mang");
        lblPromotion->setVisible(true);
        lblPromotion->setString("Vui lòng chọn nhà mạng để tiếp tục, xin cám ơn!");
        return;
    }
    
    if (mUtils::findContainChar(txtMaThe->getString().c_str())){
        CCLOG("loi nhap so");
        lblErrorMaThe->setVisible(true);
        lblErrorMaThe->setString("* Mã thẻ chỉ được nhập số!");
        return;
    }
    else if (strlen(txtMaThe->getString().c_str()) < 12){
        CCLOG("loi nhap ma the < 12");
        lblErrorMaThe->setVisible(true);
        lblErrorMaThe->setString("* Mã thẻ phải từ 12 ký tự trở lên!");
        return;
    }
    else{
        CCLOG("loi khac");
        lblErrorMaThe->setVisible(false);
    }
    
    if (strlen(txtSoThe->getString().c_str()) == 0){
        lblErrorSeri->setVisible(true);
        lblErrorSeri->setString("* Bạn chưa nhập số seri!");
        return;
    }
    else{
        lblErrorSeri->setVisible(false);
    }
    //send Request
    this->btnOK->setEnabled(false);
    //send request
    Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(LayerPopupRecharge::scheduleButtonOK),this);
    //this->scheduleOnce(CC_SCHEDULE_SELECTOR(LayerPopupRecharge::scheduleButtonOK), TIME_DISABLE);
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupRecharge::scheduleButtonOK), this, TIME_DISABLE, 1, TIME_DISABLE, false);
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("issuer", issues.c_str());
    params->PutUtfString("cardcode", txtMaThe->getString());
    params->PutUtfString("cardserial", txtSoThe->getString());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rv", params));
    GameServer::getSingleton().Send(request);
}
void LayerPopupRecharge::scheduleButtonOK(float dt){
    this->btnOK->setEnabled(true);
    this->lblPromotion->setVisible(false);
}
void LayerPopupRecharge::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    this->removeFromParentAndCleanup(true);
}
void LayerPopupRecharge::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
   // log("OnExtensionResponse %s", cmd->c_str());
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
                    str += "\n" + *res_decrip_ptr;
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
                str = dataManager.GetSysString(27);
                
                if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
                    auto currScene = Director::getInstance()->getRunningScene();
                    if (currScene != NULL)
                    {
                        if (dataManager.getAppConfig().isShowLucky)
                        {
                            auto _popupLucky = LayerPopupLuckyNumber::create(CODE_REASON_NAPTHE, txtMaThe->getString(), this->txtSoThe->getString());
                            currScene->addChild(_popupLucky, ZORDER_LIST::ZORDER_POPUP_MAINTAIN, 9051);
                        }
                        else{
                            dataManager.getAppConfig().isLoginFisrtDay = false;
                        }
                    }
                }
                this->txtMaThe->setString("");
                this->txtSoThe->setString("");
                this->btnOK->setEnabled(true);
                
            }
            else{
               // log("Nap the NOT OK %s", "i dont no");
                str = dataManager.GetSysString(28);
                this->btnOK->setEnabled(true);
            }
            lblPromotion->setString(str);
            lblPromotion->setVisible(true);
            SceneManager::getSingleton().hideLoading();
        }
    }
    else if (strcmp("notiRMFF", cmd->c_str()) == 0){
        if (currTab == 2 || currTab == 3 || currTab == 0)
            onBtnClose(NULL);
        
    }
    else if (strcmp("pmchkrs", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        string cf = *param->GetUtfString("pmstt");
        auto cfs = mUtils::splitString(cf, '|');
        if (cfs.size() > 1){
            bool isShowInapp = false;
            bool isShowLocalPayment = false;
            if (strcmp("0", cfs[0].c_str()) == 0)
                isShowLocalPayment = false;
            else
                isShowLocalPayment = true;
            if (strcmp("0", cfs[1].c_str()) == 0)
                isShowInapp = false;
            else
                isShowInapp = true;
            this->showLocalPayment(isShowInapp, isShowLocalPayment);
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
void LayerPopupRecharge::showLocalPayment(bool isShowInApp, bool isShowLocal){
    if (dataManager.getAppConfig().isHideBilling)
    {
        btnIAP->setVisible(true);
        layerIAP->setVisible(true);
        
        btnTheCao->setVisible(false);
        nodeTheCao->setVisible(false);
        btnTyGia->setVisible(false);
        btnHistory->setVisible(false);
        btnVIP->setVisible(false);
        
        btnSMS->setVisible(false);
        layerSMS->setVisible(false);
        this->onBtnIAP(btnIAP);
        return;
    }
    if (dataManager.getAppConfig().isHideSMS)
    {
        btnSMS->setVisible(false);
        this->onBtnTheCao(btnTheCao);
    }
    if (dataManager.getAppConfig().isHideTheCao)
    {
        btnTheCao->setVisible(false);
        btnVIP->setVisible(false);
        
        this->onBtnIAP(btnIAP);
    }
    else if (!dataManager.getAppConfig().isHideBilling){
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
            btnIAP->setVisible(isShowInApp);
            layerIAP->setVisible(isShowInApp);
            
            btnTheCao->setVisible(isShowLocal);
            btnVIP->setVisible(isShowLocal);
            nodeTheCao->setVisible(isShowLocal);
            btnTyGia->setVisible(isShowLocal);
            btnHistory->setVisible(isShowLocal);
            
            if (!dataManager.getAppConfig().isHideSMS)
            {
                btnSMS->setVisible(isShowLocal);
                layerSMS->setVisible(isShowLocal);
            }
            if (isShowLocal)
                this->onBtnTheCao(btnTheCao);
            else
                this->onBtnIAP(btnIAP);
        }
        else{
            btnIAP->setVisible(isShowInApp);
            if (isShowLocal){
                btnIAP->setTitleText(dataManager.GetSysString(594));
                layerIAP->setVisible(false);
            }
            
            layerIAP->setVisible(isShowLocal);
            isShowLocalGlobal = isShowLocal;
            
            btnTheCao->setVisible(isShowLocal);
            btnVIP->setVisible(isShowLocal);
            nodeTheCao->setVisible(isShowLocal);
            btnTyGia->setVisible(isShowLocal);
            btnHistory->setVisible(isShowLocal);
            
            if (!dataManager.getAppConfig().isHideSMS)
            {
                btnSMS->setVisible(isShowLocal);
                layerSMS->setVisible(isShowLocal);
            }
            if (isShowLocal)
                this->onBtnTheCao(btnTheCao);
            else
                this->onBtnIAP(btnIAP);
            
        }
    }
    
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerPopupRecharge::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().showLoading();
    sender->setVisible(false);
    return true;
    
}

void LayerPopupRecharge::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
    if (currTab == 3 || currTab == 4 || currTab == 5)
        webView->setVisible(true);
    if (isShowLocalGlobal && currTab == 0){
        webView->setVisible(true);
    }
}

void LayerPopupRecharge::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
}
#endif

void LayerPopupRecharge::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    // 	SceneManager::getSingleton().hideLoading();
    // 	if (!response)
    // 	{
    // 		return;
    // 	}
    // 	long statusCode = response->getResponseCode();
    // 	char statusString[64] = {};
    // 	if (!response->isSucceed())
    // 	{
    // 		return;
    // 	}
    // 	std::vector<char> *buffer = response->getResponseData();
    // 	log("LayerPopupRecharge::onHttpRequestCompleted - datas receive from http: %s", buffer[0].data());
    // 	// dump data
    // 	string _s = string(buffer[0].data());
    // 	int pos = _s.find_first_of("!");
    // 	string _res = _s.substr(0, pos);
    //
    // 	lblReportGiftCode->setVisible(true);
    // 	lblReportGiftCode->setString(_res);
}

void LayerPopupRecharge::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto locationInNode = this->convertToNodeSpace(pTouch->getLocation());
    auto s = this->getContentSize();
    auto rect = Rect(0, 0, s.width, s.height);
    if (!rect.containsPoint(locationInNode))
    {
        this->removeFromParentAndCleanup(true);
    }
}

bool LayerPopupPurcharse::init()
{
    if (!Layer::init())
        return false;
    auto root = CSLoader::getInstance()->createNode("LayerPopupPurcharse.csb");
    int gameID = SceneManager::getSingleton().getGameID();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (gameID == kGameChan || gameID == kGameCoTuong || gameID == kGameCoUp){
        root->setAnchorPoint(Point::ZERO);
        root->setPosition(Point::ZERO);
    }
    else{
        root->setAnchorPoint(Point(0.5, 0.5));
        root->setPosition(Point(visibleSize / 2));
    }
    ui::Helper::doLayout(root);
    addChild(root);
    // 	Sprite* Sprite_1 = (Sprite*)root->getChildByName("Sprite_1");
    // 	if (Sprite_1)
    // 		Sprite_1->setVisible(false);
    // 	Label* Text_1 = (Label*)root->getChildByName("Text_1");
    // 	if (Text_1)
    // 		Text_1->setVisible(false);
    // 	Text* lblPhoneHotLine = dynamic_cast<Text*>(root->getChildByName("lblPhoneNumber"));
    // 	if (lblPhoneHotLine)
    // 		lblPhoneHotLine->setVisible(false);
    Layout* panel_bronze = (Layout*)root->getChildByName("panel_bronze");
    int money_rate = 2;
    if (panel_bronze)
    {
        Button* btnBronze = static_cast<Button*>(panel_bronze->getChildByName("btnBronze"));
        if (btnBronze)
        {
            btnBronze->setTitleText("0.99$");
            btnBronze->addTouchEventListener(CC_CALLBACK_2(LayerPopupPurcharse::onButtonBronze, this));
        }
        Text *txtBronze = static_cast<Text*>(panel_bronze->getChildByName("labelBronze"));
        if (txtBronze)
            txtBronze->setString(mUtils::convertMoneyEx(money_rate * 1000));
    }
    
    Layout* panel_silver = (Layout*)root->getChildByName("panel_silver");
    if (panel_silver)
    {
        Button* btnSilver = static_cast<Button*>(panel_silver->getChildByName("btnSilver"));
        if (btnSilver)
        {
            btnSilver->setTitleText("9.99$");
            btnSilver->addTouchEventListener(CC_CALLBACK_2(LayerPopupPurcharse::onButtonSilver, this));
        }
        Text *txtSilver = static_cast<Text*>(panel_silver->getChildByName("labelSilver"));
        if (txtSilver)
            txtSilver->setString(mUtils::convertMoneyEx(money_rate * 1000 * 10));
    }
    
    Layout* panel_gold = (Layout*)root->getChildByName("panel_gold");
    if (panel_gold)
    {
        Button* btnGold = static_cast<Button*>(panel_gold->getChildByName("btnGold"));
        if (btnGold)
        {
            btnGold->setTitleText("99.99$");
            btnGold->addTouchEventListener(CC_CALLBACK_2(LayerPopupPurcharse::onButtonGold, this));
        }
        Text *txtGold = static_cast<Text*>(panel_gold->getChildByName("labelGold"));
        if (txtGold)
            txtGold->setString(mUtils::convertMoneyEx(money_rate * 1000 * 100));
    }
    // 	Button* btnClose = static_cast<Button*>(root->getChildByName("btnClose"));
    // 	if (btnClose)
    // 		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPopupPurcharse::onButtonClose, this));
    // 	Button* btnMuabac = static_cast<Button*>(root->getChildByName("btnMuabac"));
    // 	if (btnMuabac)
    // 		btnMuabac->setTitleText("Mua Bạc");
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    iap = new mIAP();
    iap->loadIAP();
#endif
    return true;
}

LayerPopupPurcharse::LayerPopupPurcharse()
{
    
}

LayerPopupPurcharse::~LayerPopupPurcharse()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    iap->removeListener();
    delete iap;
#endif
}

void LayerPopupPurcharse::onButtonBronze(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
      // iap->purchaseItem(0);
#endif
    }
}

void LayerPopupPurcharse::onButtonSilver(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS  || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
       // iap->purchaseItem(1);
#endif
    }
}

void LayerPopupPurcharse::onButtonGold(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
      //  iap->purchaseItem(2);
#endif
    }
}

void LayerPopupPurcharse::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        Layer *parent = static_cast<Layer*>(this->getParent());
        if (parent)
            parent->removeFromParentAndCleanup(true);
    }
}

LayerPopupSMS::LayerPopupSMS()
{
    GameServer::getSingleton().addListeners(this);
    /*if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
     
     }*/
}

LayerPopupSMS::~LayerPopupSMS()
{
    GameServer::getSingleton().removeListeners(this);
}

void LayerPopupSMS::onEnter(){
    Layer::onEnter();
}

void LayerPopupSMS::onExit(){
    
    Layer::onExit();
}

bool LayerPopupSMS::init()
{
    if (!Layer::init())
        return false;
	int currGameID = SceneManager::getSingleton().getGameID();
	string pathCSB = "LayerPopupSMS.csb";
	cellSize = Size(1000, 160);
	if (currGameID == kGameCoTuong || currGameID == kGameCoUp || currGameID == kGameChan)
	{
		pathCSB = "LayerPopupSMSChess.csb";
		cellSize = Size(1000, 200);
	}
	auto root = CSLoader::getInstance()->createNode(pathCSB);
    ui::Helper::doLayout(root);
    addChild(root);
    this->setContentSize(root->getContentSize());
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
    btnMobi = dynamic_cast<Button*>(root->getChildByName("btnMobi"));
    btnMobi->setEnabled(true);
    if (btnMobi){
        btnMobi->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onButtonMobi, this));
        btnMobi->setPressedActionEnabled(true);
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
            btnMobi->setTitleText("Mobiphone");
        btnMobi->setTitleColor(Color3B::BLACK);
    }
    btnViettel = dynamic_cast<Button*>(root->getChildByName("btnViettel"));
    if (btnViettel){
        btnViettel->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onButtonViettel, this));
        btnViettel->setPressedActionEnabled(true);
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
            btnViettel->setTitleText("Viettel");
        btnViettel->setTitleColor(Color3B::BLACK);
    }
    btnVina = dynamic_cast<Button*>(root->getChildByName("btnVina"));
    if (btnVina){
        btnVina->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onButtonVina, this));
        btnVina->setPressedActionEnabled(true);
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
            btnVina->setTitleText("Vinaphone");
        btnVina->setTitleColor(Color3B::BLACK);
    }
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        
        /*btn10K = dynamic_cast<Button*>(root->getChildByName("btn10k"));
         btn15K = dynamic_cast<Button*>(root->getChildByName("btn15k"));
         btn3K = dynamic_cast<Button*>(root->getChildByName("btn3k"));*/
        
        this->pnlListTheSms = static_cast<Layout*>(root->getChildByName("pnlListSMS"));
        this->pnlListTheSms->setVisible(false);
        this->pnlSelect = static_cast<Layout*>(root->getChildByName("pnl_select"));
        //this->pnlSelect->setVisible(true);
        this->pnlCuphap = static_cast<Layout*>(root->getChildByName("pnl_cuphap"));
        
        //this->pnlCuphap->setVisible(true);
        
        //pnlSelect Object
        this->lblUserSelect = static_cast<Text*>(pnlSelect->getChildByName("lblUserName"));
        if (myself)
            this->lblUserSelect->setString(*myself->Name());
        this->btnUserNap = static_cast<Button*>(pnlSelect->getChildByName("btnSelectUserNap"));
        this->lblError = static_cast<Text*>(pnlSelect->getChildByName("lblError"));
        if (this->btnUserNap != NULL){
            btnUserNap->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onButtonNap1Selected, this));
            btnUserNap->setPressedActionEnabled(true);
        }
        this->btnNapForAnother = static_cast<Button*>(pnlSelect->getChildByName("btnSelectReceiverNap"));
        if (this->btnNapForAnother != NULL){
            btnNapForAnother->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onButtonNap2Selected, this));
            btnNapForAnother->setPressedActionEnabled(true);
        }
        this->txtTennguoinhan = static_cast<TextField*>(pnlSelect->getChildByName("txtNguoiNhan"));
        if (this->txtTennguoinhan != NULL){
            this->txtTennguoinhan->setTextColor(Color4B::BLACK);
        }
        this->btnBackSelect = static_cast<Button*>(pnlSelect->getChildByName("btnBackSelect"));
        if (this->btnBackSelect != NULL){
            this->btnBackSelect->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onButtonBackSelectClicked, this));
            this->btnBackSelect->setPressedActionEnabled(true);
        }
        
        //pnlCuphap Object
        this->lblQuyDoi = static_cast<Text*>(pnlCuphap->getChildByName("lblQuyDoi"));
        this->lblNguoiNhan = static_cast<Text*>(pnlCuphap->getChildByName("lblNguoiNhan"));
        this->lblCuphapNap = static_cast<Text*>(pnlCuphap->getChildByName("lblCuPhap"));
        this->lblDauSonap = static_cast<Text*>(pnlCuphap->getChildByName("lblDauSo"));
        this->btnNap = static_cast<Button*>(pnlCuphap->getChildByName("btnNap"));
        if (this->btnNap != NULL){
            this->btnNap->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onBtnNapTien, this));
            this->btnNap->setPressedActionEnabled(true);
        }
        this->btnBackCuPhap = static_cast<Button*>(pnlCuphap->getChildByName("btnBack"));
        if (this->btnBackCuPhap != NULL){
            this->btnBackCuPhap->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onButtonBackCuphapClicked, this));
            this->btnBackCuPhap->setPressedActionEnabled(true);
        }
    }
    else{
        cocos2d::network::HttpRequest *request = new cocos2d::network::HttpRequest();
        request->setUrl("http://125.212.225.173/noteSMS");
        request->setResponseCallback(CC_CALLBACK_2(LayerPopupSMS::onHttpRequestCompleted, this));
        request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
        
        this->lblNote = static_cast<Text*>(root->getChildByName("lblNote"));
        this->lblNoList = static_cast<Text*>(root->getChildByName("Text_19"));
        this->pnlListTheSms = static_cast<Layout*>(root->getChildByName("Panel_1"));
        this->pnlListTheSms->setVisible(false);
        /*auto napnhanhtxt = static_cast<Text*>(root->getChildByName("Text_20"));
         napnhanhtxt->setVisible(true);
         btn10K->setVisible(true);
         btn15K->setVisible(true);
         btn3K->setVisible(true);*/
    }
    
    this->tblTheSms = TableView::create(this, this->pnlListTheSms->getContentSize());
    this->tblTheSms->setAnchorPoint(this->pnlListTheSms->getAnchorPoint());
    this->tblTheSms->setContentSize(this->pnlListTheSms->getContentSize());
    this->tblTheSms->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblTheSms->setPosition(this->pnlListTheSms->getPosition());
    this->tblTheSms->setDelegate(this);
    this->tblTheSms->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->addChild(this->tblTheSms);
    this->getAllDataSms();
    this->onButtonMobi(NULL);
    
    /*if (btn10K)
     {
     txt10k = (Text*)btn10K->getChildByName("Text_8");
     btn10K->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onBtnNap10k, this));
     btn10K->setPressedActionEnabled(true);
     }
     
     if (btn15K)
     {
     txt15k = (Text*)btn15K->getChildByName("Text_8");
     btn15K->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onBtnNap15k, this));
     btn15K->setPressedActionEnabled(true);
     }
     
     if (btn3K)
     {
     txt3k = (Text*)btn3K->getChildByName("Text_8");
     btn3K->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onBtnNap3k, this));
     btn3K->setPressedActionEnabled(true);
     }
     
     Layout *panel = (Layout *)root->getChildByName("Panel_1");
     if (panel)
     {
     auto btnNapnhanh = dynamic_cast<Button*>(panel->getChildByName("btnFastCharge"));
     if (btnNapnhanh)
     {
     btnNapnhanh->addClickEventListener(CC_CALLBACK_1(LayerPopupSMS::onBtnNapTien, this));
     btnNapnhanh->setPressedActionEnabled(true);
     }
     lblMoneyReceived = dynamic_cast<Text*>(panel->getChildByName("lblMoneyReceived"));
     lblPromotionSMS = static_cast<Text*>(panel->getChildByName("lblPromotionSMS"));
     lblCuphap = dynamic_cast<Text*>(panel->getChildByName("lblCuphap"));
     lblDauSo = dynamic_cast<Text*>(panel->getChildByName("lblDauSo"));
     lblFastCharge = (Text*)panel->getChildByName("lblfastcharge");
     Button* btnTyGia = (Button*)panel->getChildByName("btnTyGia");
     if (btnTyGia)
     btnTyGia->addTouchEventListener(CC_CALLBACK_1(LayerPopupSMS::onBtnTyGia, this));
     }*/
    
    /*this->loadAllDatas();
     onBtnNap10k(NULL);*/
    return true;
}

void LayerPopupSMS::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
    std::vector<char> *buffer = response->getResponseData();
    char *concatenated = (char*)malloc(buffer->size() + 1);
    std::string str2(buffer->begin(), buffer->end());
    strcpy(concatenated, str2.c_str());
    
    //	Json *json = Json_create(concatenated);
    //	const char *note = Json_getString(json, "note", "Giới hạn nạp SMS");
    //
    //	this->lblNote->setString(note);
    //	//CCLOG("note %s", this->lblNote->getString());
    //	Json_dispose(json);
}

void LayerPopupSMS::onMenhGiaClick(Ref* pSender){
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        this->txtTennguoinhan->setEnableEditBox(true);
        Node* _node = (Node*)pSender;
        int tag = _node->getTag();
        this->current_tag = tag;
        //*mUtils::format_money(sms.money_exchange)
        this->lblQuyDoi->setString(StringUtils::format("%d = %d", lst.at(tag).menhgia, lst.at(tag).tiennhanduoc));
        this->lblCuphapNap->setString(lst.at(tag).cuphap);
        this->lblDauSonap->setString(StringUtils::format("%d", lst.at(tag).dauso));
        //
        this->tblTheSms->setVisible(false);
        this->pnlSelect->setVisible(true);
        this->pnlCuphap->setVisible(false);
    }
    else{
        
    }
}

void LayerPopupSMS::onButtonBackCuphapClicked(Ref* pSender){
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        this->txtTennguoinhan->setEnableEditBox(true);
        
        this->lblNguoiNhan->setString(this->lblUserSelect->getString());
        this->tblTheSms->setVisible(false);
        this->pnlCuphap->setVisible(false);
        this->pnlSelect->setVisible(true);
    }
}

void LayerPopupSMS::onButtonNap1Selected(Ref* pSender){
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        this->txtTennguoinhan->setEnableEditBox(false);
        
        this->lblNguoiNhan->setString(this->lblUserSelect->getString());
        this->lblCuphapNap->setString(StringUtils::format("%s %s", lst.at(current_tag).cuphap.c_str(), this->lblNguoiNhan->getString().c_str()));
        this->tblTheSms->setVisible(false);
        this->pnlCuphap->setVisible(true);
        this->pnlSelect->setVisible(false);
    }
}

void LayerPopupSMS::onButtonNap2Selected(Ref* pSender){
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        this->txtTennguoinhan->setEnableEditBox(false);
        
        if (this->txtTennguoinhan->getString().size() < 6){
            // 			Chat* toast = Chat::create("Bạn cần nhập đúng tên người nhận", -1);
            // 			this->getParent()->addChild(toast, ZORDER_LIST::ZORDER_TOAST);
            this->lblError->setVisible(true);
            this->lblError->setString("Bạn cần nhập đúng tên người nhận");
        }
        else{
            this->lblError->setVisible(false);
            this->lblNguoiNhan->setString(this->txtTennguoinhan->getString());
            this->lblCuphapNap->setString(StringUtils::format("%s %s", lst.at(current_tag).cuphap.c_str(), this->lblNguoiNhan->getString().c_str()));
            this->tblTheSms->setVisible(false);
            this->pnlCuphap->setVisible(true);
            this->pnlSelect->setVisible(false);
        }
    }
}

void LayerPopupSMS::onButtonBackSelectClicked(Ref* pSender){
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        this->txtTennguoinhan->setEnableEditBox(false);
        this->tblTheSms->setVisible(true);
        this->pnlSelect->setVisible(false);
        this->pnlCuphap->setVisible(false);
    }
}
void LayerPopupSMS::getAllDataSms(){
    //if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    this->lstTheSMS.clear();
    string strDataSms = (dataManager.getAppConfig().strListSMS);
    CCLOG("strDatas %s", strDataSms.c_str());
    tachChuoi(strDataSms);
    for (int i = 0; i < smsString.size(); ++i)
    {
        //CCLOG("tachchuoi: %s", smsString.at(i).c_str());
        char *smsChar = new char[smsString.at(i).length() + 1];
        char *token;
        TheSMS theSms;
        int j = 0;
        strcpy(smsChar, smsString.at(i).c_str());
        token = strtok(smsChar, "-");
        while (token != NULL)
        {
            string params = string(token);
            if (j == 0){
                theSms.loaithe = atoi(params.c_str());
            }
            else if (j == 1){
                theSms.dauso = atoi(params.c_str());
            }
            else if (j == 2){
                theSms.menhgia = atoi(params.c_str());
            }
            else if (j == 3)
            {
                theSms.cuphap = params;
            }
            else if (j == 4){
                theSms.tiennhanduoc = atoi(params.c_str());
            }
            token = strtok(NULL, "-");
            j++;
        }
        lstTheSMS.push_back(theSms);
    }
    
    for (int k = 0; k < lstTheSMS.size(); ++k){
        CCLOG("LoaiThe: %d - Dauso: %d - Menhgia: %d - Cuphap: %s - Tiennhanduoc: %d",
              lstTheSMS.at(k).loaithe, lstTheSMS.at(k).dauso, lstTheSMS.at(k).menhgia, lstTheSMS.at(k).cuphap.c_str(), lstTheSMS.at(k).tiennhanduoc);
    }
    //}
}

vector<string> LayerPopupSMS::tachChuoi(string str){
    vector<string> strVector(5);
    //if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    int idx = str.find(";");
    if (idx == -1){
        //smsString.push_back(str);
        return smsString;
    }
    else{
        string sms = str.substr(0, idx);
        smsString.push_back(sms);
        string shortSms = str.substr(idx + 1, str.length());
        tachChuoi(shortSms);
    }
    return smsString;
    //}
    return strVector;
}

void LayerPopupSMS::onButtonMobi(Ref* pSender)
{
    btnMobi->setEnabled(false);
    btnViettel->setEnabled(true);
    btnVina->setEnabled(true);
    this->tblTheSms->reloadData();
    this->tblTheSms->setVisible(true);
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        //lblPromotion->setVisible(false);
        this->pnlCuphap->setVisible(false);
        this->pnlSelect->setVisible(false);
    }
}

void LayerPopupSMS::onButtonVina(Ref* pSender)
{
    btnMobi->setEnabled(true);
    btnViettel->setEnabled(true);
    btnVina->setEnabled(false);
    this->tblTheSms->reloadData();
    this->tblTheSms->setVisible(true);
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        //lblPromotion->setVisible(false);
        this->pnlCuphap->setVisible(false);
        this->pnlSelect->setVisible(false);
    }
}

void LayerPopupSMS::onButtonViettel(Ref* pSender)
{
    btnMobi->setEnabled(true);
    btnViettel->setEnabled(false);
    btnVina->setEnabled(true);
    this->tblTheSms->reloadData();
    this->tblTheSms->setVisible(true);
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        //lblPromotion->setVisible(false);
        this->pnlCuphap->setVisible(false);
        this->pnlSelect->setVisible(false);
    }
}

void LayerPopupSMS::onBtnNap10k(Ref* pSender)
{
    /*Color3B _colorPress = Color3B(175, 8, 0);
     Color3B _colorNormal = Color3B(255, 236, 192);
     btn10K->setEnabled(false);
     btn3K->setEnabled(true);
     btn15K->setEnabled(true);
     btn10K->setTitleColor(_colorPress);
     txt10k->setColor(_colorPress);
     btn3K->setTitleColor(_colorNormal);
     txt3k->setColor(_colorNormal);
     btn15K->setTitleColor(_colorNormal);
     txt15k->setColor(_colorNormal);
     setInfo(tagIndexListSMS_10k, btn10K->getTitleText());*/
}

void LayerPopupSMS::onBtnNap15k(Ref* pSender)
{
    //Color3B _colorPress = Color3B(175, 8, 0);
    //Color3B _colorNormal = Color3B(255, 236, 192);
    //btn10K->setEnabled(true);
    //btn3K->setEnabled(true);
    //btn15K->setEnabled(false);
    //btn10K->setTitleColor(_colorNormal);
    //txt10k->setColor(_colorNormal);
    //btn3K->setTitleColor(_colorNormal);
    //txt3k->setColor(_colorNormal);
    //btn15K->setTitleColor(_colorPress);
    //txt15k->setColor(_colorPress);
    ////
    //setInfo(tagIndexListSMS_15k, btn15K->getTitleText());
}

void LayerPopupSMS::onBtnNap3k(Ref* pSender)
{
    /*Color3B _colorPress = Color3B(175, 8, 0);
     Color3B _colorNormal = Color3B(255, 236, 192);
     btn10K->setEnabled(true);
     btn3K->setEnabled(false);
     btn15K->setEnabled(true);
     btn10K->setTitleColor(_colorNormal);
     txt10k->setColor(_colorNormal);
     btn3K->setTitleColor(_colorPress);
     txt3k->setColor(_colorPress);
     btn15K->setTitleColor(_colorNormal);
     txt15k->setColor(_colorNormal);
     setInfo(tagIndexListSMS_3k, btn3K->getTitleText());*/
}

void LayerPopupSMS::onBtnNapTien(Ref* pSender)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
        /*StructSMS sms = lstSMS.at(currTabSMS);
         const char* dauSo = sms.number.c_str();
         const char* cuPhap = sms.content.c_str();*/
        const char* dauSo = this->lblDauSo->getString().c_str();
        const char* cuPhap = this->lblCuphap->getString().c_str();
        NativeHelper::callSendSMS(dauSo, cuPhap);
    }
    else{
        const char* dauSo = this->lblDauSonap->getString().c_str();
        const char* cuPhap = this->lblCuphapNap->getString().c_str();
        NativeHelper::callSendSMS(dauSo, cuPhap);
    }
#endif
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
        StructSMS sms = lstSMS.at(currTabSMS);
        IOS::trySendSMS(sms.number, sms.content);
    }
    else{
        const char* dauSo = this->lblDauSonap->getString().c_str();
        const char* cuPhap = this->lblCuphapNap->getString().c_str();
        IOS::trySendSMS(string(dauSo), string(cuPhap));
    }
    
#endif
}

void LayerPopupSMS::onBtnTyGia(Ref* pSender)
{
    LayerPopupRecharge* parent = dynamic_cast<LayerPopupRecharge*>(this->getParent());
    if (parent)
    {
        parent->onBtnTyGia(NULL);
    }
}

void LayerPopupSMS::loadAllDatas()
{
    lstSMS.clear();
    UserDefault *def = UserDefault::getInstance();
    string liSMSCMD = def->getStringForKey("liSMSCMD", "");
    //btn3K->setVisible(false);
    //btn10K->setVisible(false);
    //btn15K->setVisible(false);
    if (liSMSCMD.compare("") == 0){
        return;
    }
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    vector<string> lstSmss = mUtils::splitString(liSMSCMD, '#');
    int size = lstSmss.size();
    if (size > 3)
        size = 3;
    for (int i = 0; i < size; i++){
        if (lstSmss.at(i) == "")
            continue;
        vector<string> lstChilds = mUtils::splitString(lstSmss.at(i), '$');
        if (lstChilds.size() < 4)
            continue;
        StructSMS sms;
        
        try {
            string myName = myself->Name()->c_str();
            sms.number = lstChilds.at(1);
            sms.content = lstChilds.at(4) + " " + string(ChanUtils::GetNamePlayer(myName));
            sms.money = atoi(lstChilds.at(2).c_str());
            sms.money_exchange = atoi(lstChilds.at(3).c_str());
            lstSMS.push_back(sms);
            
        }
        catch (std::exception ex) {
            //
        }
        catch (...) {
           // log("GameServer::connectToServer() - unknown exception");
        }
        //
        switch (i)
        {
            case 0:
                /*btn10K->setTitleText(StringUtils::format("%s", mUtils::format_money(sms.money)->c_str()));
                 btn10K->setVisible(true);*/
                break;
            case 1:
                /*btn15K->setTitleText(StringUtils::format("%s", mUtils::format_money(sms.money)->c_str()));
                 btn15K->setVisible(true);*/
                break;
            case 2:
                /*btn3K->setTitleText(StringUtils::format("%s", mUtils::format_money(sms.money)->c_str()));
                 btn3K->setVisible(true);*/
                break;
            default:
                break;
        }
    }
}

void LayerPopupSMS::setInfo(int tag, string title)
{
    if (lstSMS.size() < tag || lstSMS.size() == 0)
    {
        // 		SceneManager::getSingleton().showChatWithCurrentScene("Đường truyền của bạn đang có vấn đề, đang kết nối lại...", -1);
        // 		this->scheduleOnce(schedule_selector(LayerPopupSMS::gotoReconnect), 1);
        
    }
    else{
        currTabSMS = tag;
        StructSMS sms = lstSMS.at(currTabSMS);
        lblMoneyReceived->setString(*mUtils::format_money(sms.money_exchange));
        lblCuphap->setString(sms.content);
        lblDauSo->setString(sms.number);
        lblFastCharge->setString(title);
        
    }
    
}
void LayerPopupSMS::gotoReconnect(float dt){
    SceneManager::getSingleton().disconnectFromServer();
}

void LayerPopupSMS::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
    /*auto itemSms = LayerItemSMS::create();
     itemSms->onBtnMenhGiaClicked(NULL, ui::Widget::TouchEventType::ENDED);*/
}

Size LayerPopupSMS::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    CCLOG("tableCellSizeForIndex");
	return cellSize;
    //}
}

TableViewCell* LayerPopupSMS::tableCellAtIndex(TableView *table, ssize_t idx)
{
    CCLOG("tableCellAtIndex");
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    auto cell = table->dequeueCell();
    if (!cell) {
        cell = TableViewCell::create();
        
    }
    else {
        cell->removeAllChildren();
    }
    if (idx < 0)
        return cell;
    int cols;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        cols = 3;
    }
    else{
        cols = 1;
    }
    auto _child_ = LayerItemSMS::create();
    auto sizeTable = this->pnlListTheSms->getContentSize();
    auto width = _child_->getContentSize().width;
    
    //comment
    lst.clear();
    if (!btnMobi->isEnabled()){
        for (int i = 0; i < lstTheSMS.size(); ++i){
            TheSMS theSms = lstTheSMS.at(i);
            if (theSms.loaithe == TAG_MOBI){
                TheSMS lstThe;
                lstThe.loaithe = theSms.loaithe;
                lstThe.dauso = theSms.dauso;
                lstThe.menhgia = theSms.menhgia;
                lstThe.cuphap = theSms.cuphap;
                lstThe.tiennhanduoc = theSms.tiennhanduoc;
                
                lst.push_back(lstThe);
            }
        }
        CCLOG("Size: %d", lst.size());
    }
    else if (!btnViettel->isEnabled()){
        for (int i = 0; i < lstTheSMS.size(); ++i){
            TheSMS theSms = lstTheSMS.at(i);
            if (theSms.loaithe == TAG_VIETTEL){
                TheSMS lstThe;
                lstThe.loaithe = theSms.loaithe;
                lstThe.dauso = theSms.dauso;
                lstThe.menhgia = theSms.menhgia;
                lstThe.cuphap = theSms.cuphap;
                lstThe.tiennhanduoc = theSms.tiennhanduoc;
                
                lst.push_back(lstThe);
            }
        }
    }
    else if (!btnVina->isEnabled()){
        for (int i = 0; i < lstTheSMS.size(); ++i){
            TheSMS theSms = lstTheSMS.at(i);
            if (theSms.loaithe == TAG_VINA){
                TheSMS lstThe;
                lstThe.loaithe = theSms.loaithe;
                lstThe.dauso = theSms.dauso;
                lstThe.menhgia = theSms.menhgia;
                lstThe.cuphap = theSms.cuphap;
                lstThe.tiennhanduoc = theSms.tiennhanduoc;
                
                lst.push_back(lstThe);
            }
        }
    }
    
    //auto startX = sizeTable.width - (_child_->getContentSize().width*cols + 20 * (cols - 1) + _child_->getContentSize().width / 2);
    float startX;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        startX = sizeTable.width - (_child_->getContentSize().width*cols + 75 * (cols - 1));
    }
    else{
        startX = sizeTable.width - (_child_->getContentSize().width*cols + 50 * (cols - 1));
    }
    
    for (int i = 0; i < cols; i++) {
        int index = idx *cols + i;
        if (index < lst.size())
        {
            auto _child = LayerItemSMS::create();
            CCLOG("X: %3.0f - Y: %3.0f ", _child->getAnchorPoint().x, _child->getAnchorPoint().y);
            //  _child.onPlayRoomCallBack(this.playRoomCallBack, this);
            _child->setAnchorPoint(Vec2(0,0));
            _child->onMenhGiaCallBack(this, callfuncO_selector(LayerPopupSMS::onMenhGiaClick));
            _child->setPosition(startX + (i * width) + 65 * i, 0);
            //  _child.setDatas(this.listTheCaos[idx].type,this.listTheCaos[idx].menhgia,this.listTheCaos[idx].cost);
            cell->addChild(_child, 0);
            if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
                _child->setDatas(lst[index].loaithe, lst[index].menhgia, lst[index].tiennhanduoc);
            }
            else{
                if (myself)
                    _child->setDataItem(lst[index].menhgia, lst[index].tiennhanduoc, (lst[index].cuphap + " " + *myself->Name()), lst[index].dauso);
            }
            _child->setVisible(true);
            _child->setTag(index);
        }
    }
    return cell;
}

ssize_t LayerPopupSMS::numberOfCellsInTableView(TableView *table)
{
    CCLOG("numberOfCellsInTableView");
    int rows = 0;
    //if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    lst.clear();
    if (!btnMobi->isEnabled()){
        for (int i = 0; i < lstTheSMS.size(); ++i){
            TheSMS theSms = lstTheSMS.at(i);
            if (theSms.loaithe == TAG_MOBI){
                TheSMS lstThe;
                lstThe.loaithe = theSms.loaithe;
                lstThe.dauso = theSms.dauso;
                lstThe.menhgia = theSms.menhgia;
                lstThe.cuphap = theSms.cuphap;
                lstThe.tiennhanduoc = theSms.tiennhanduoc;
                
                lst.push_back(lstThe);
            }
        }
    }
    else if (!btnViettel->isEnabled()){
        for (int i = 0; i < lstTheSMS.size(); ++i){
            TheSMS theSms = lstTheSMS.at(i);
            if (theSms.loaithe == TAG_VIETTEL){
                TheSMS lstThe;
                lstThe.loaithe = theSms.loaithe;
                lstThe.dauso = theSms.dauso;
                lstThe.menhgia = theSms.menhgia;
                lstThe.cuphap = theSms.cuphap;
                lstThe.tiennhanduoc = theSms.tiennhanduoc;
                
                lst.push_back(lstThe);
            }
        }
    }
    else if (!btnVina->isEnabled()){
        for (int i = 0; i < lstTheSMS.size(); ++i){
            TheSMS theSms = lstTheSMS.at(i);
            if (theSms.loaithe == TAG_VINA){
                TheSMS lstThe;
                lstThe.loaithe = theSms.loaithe;
                lstThe.dauso = theSms.dauso;
                lstThe.menhgia = theSms.menhgia;
                lstThe.cuphap = theSms.cuphap;
                lstThe.tiennhanduoc = theSms.tiennhanduoc;
                
                lst.push_back(lstThe);
            }
        }
    }
    
    if (this->lst.size() == 0){
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
            this->lblNoList->setVisible(true);
        }
        else{
            rows = 0;
        }
        return 0;
    }
    else{
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
            this->lblNoList->setVisible(false);
        }
    }
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        if (this->lst.size() < 3)
            return 1;
        rows = this->lst.size() / 3;
        if (this->lst.size() % 3 != 0)
            rows += 1;
    }
    else{
        rows = this->lst.size();
    }
    return rows;
}

bool LayerItemSMS::init(){
    if (!Layer::init())
    {
        return false;
    }
	int gameId = SceneManager::getSingleton().getGameID();
	string pathCSB = "ItemSMS.csb";
	if (gameId == kGameCoTuong || gameId == kGameCoUp || gameId == kGameChan)
		pathCSB = "ItemSMSChess.csb";
    auto root = CSLoader::getInstance()->createNode(pathCSB);
    ui::Helper::doLayout(root);
    this->addChild(root);
    this->setContentSize(root->getContentSize());
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        this->lblMenhgia = static_cast<ui::Text*>(root->getChildByName("lblGiaTriThe"));
        this->lblTiennhanduoc = static_cast<ui::Text*>(root->getChildByName("lblReciverValue"));
        this->btnMenhGia = static_cast<ui::Button*>(root->getChildByName("btn_menhgia"));
        
        if (btnMenhGia != NULL){
            btnMenhGia->setSwallowTouches(true);
            btnMenhGia->addTouchEventListener(CC_CALLBACK_2(LayerItemSMS::onBtnMenhGiaClicked, this));
        }
    }
    else{
        this->lblCuPhap = static_cast<ui::Text*>(root->getChildByName("lblCuphap"));
        this->lblQuyDoi = static_cast<ui::Text*>(root->getChildByName("lblQuyDoi"));
        this->lblDauSo = static_cast<ui::Text*>(root->getChildByName("lblDauSo"));
        this->btnNap = static_cast<ui::Button*>(root->getChildByName("btnNap"));
        
        if (btnNap != NULL){
            btnNap->setSwallowTouches(true);
            btnNap->addTouchEventListener(CC_CALLBACK_2(LayerItemSMS::onBtnNapClicked, this));
        }
    }
    return true;
}

void LayerItemSMS::setDatas(int loaithe, int menhgia, int tiennhanduoc){
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        if (this->lblMenhgia && this->lblTiennhanduoc){
            this->lblMenhgia->setString(mUtils::format_money(menhgia)->c_str());
            this->lblTiennhanduoc->setString(mUtils::format_money(tiennhanduoc)->c_str());
        }
    }
}

void LayerItemSMS::setDataItem(int menhgia, int tiennhanduoc, string cuphap, int dauso){
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
        this->lblQuyDoi->setString(StringUtils::format("Nạp: %s được %s %s", mUtils::format_money(menhgia)->c_str(), mUtils::format_money(tiennhanduoc)->c_str(),SceneManager::getSingleton().getCoin(0).c_str()));
        this->lblCuPhap->setString(cuphap);
        this->lblDauSo->setString(StringUtils::format("%d", dauso));
    }
}

void LayerItemSMS::onMenhGiaCallBack(Ref* target, SEL_CallFuncO listener){
    mCallBack = target;
    mCallBackListener = listener;
}

void LayerItemSMS::onBtnNapClicked(Ref* pSender, ui::Widget::TouchEventType type){
    if (type == ui::Widget::TouchEventType::ENDED){
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
            const char* dauSo = this->lblDauSo->getString().c_str();
            const char* cuPhap = this->lblCuPhap->getString().c_str();
            NativeHelper::callSendSMS(dauSo, cuPhap);
        }
#endif
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
            //StructSMS sms = lstSMS.at(currTabSMS);
            const char* dauSo = this->lblDauSo->getString().c_str();
            const char* cuPhap = this->lblCuPhap->getString().c_str();
            IOS::trySendSMS(string(dauSo), string(cuPhap));
        }
#endif
    }
}

void LayerItemSMS::onBtnMenhGiaClicked(Ref* pSender, ui::Widget::TouchEventType type){
    if (type == ui::Widget::TouchEventType::ENDED){
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
            if (mCallBack && mCallBackListener){
                //mCallBack
                // 				Node* node = Node::create();
                // 				node->setTag(this->getTag());
                (mCallBack->*mCallBackListener)(this);
            }
        }
    }
    
}
