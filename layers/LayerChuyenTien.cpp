#include "LayerChuyenTien.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "mUtils.h"
#include "../SceneManager.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupXacThucOTP.h"
#include "../layergames/ClientMsgDefs.h"
LayerChuyenTien::LayerChuyenTien()
{

}

LayerChuyenTien::~LayerChuyenTien()
{
	GameServer::getSingleton().removeListeners(this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerChuyenTien::onScheduleUpdateCost),this);
}

bool LayerChuyenTien::init()
{
	if (!Layer::init())
	{
		return false;
	}
	mTax = 5.0;
	GameServer::getSingleton().addListeners(this);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
		// add background
		auto background = Sprite::create("background-in-game.jpg");
		background->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
		background->setAnchorPoint(Point(0, 0));
		background->setPosition(Point(0, 0));
		this->addChild(background);

		auto sprBg = Sprite::create("background-tranf.png");
		sprBg->setPosition(visibleSize / 2);
		sprBg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
		this->addChild(sprBg);

	}
	else{
		auto sprBg = Sprite::create("background-sanh.jpg");
		sprBg->setPosition(visibleSize / 2);
		sprBg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
		this->addChild(sprBg);

	}
	auto rootChuyenTien = CSLoader::getInstance()->createNode("LayerChuyenTien.csb");
	ui::Helper::doLayout(rootChuyenTien);
	this->addChild(rootChuyenTien);

	/*-------------------------init variable ----------------*/
	//ChuyenTien Top
	auto nodeChuyenTienTop = dynamic_cast<Node*>(rootChuyenTien->getChildByName("nodeChuyenTienTop"));
	nodeChuyenTienTop->setAnchorPoint(Point(0.5, 0.5));
	nodeChuyenTienTop->setPosition(Point(visibleSize.width / 2, visibleSize.height - nodeChuyenTienTop->getContentSize().height / 2));

	auto layerOnlyBack = dynamic_cast<Node*>(nodeChuyenTienTop->getChildByName("layerNodeOnlyBack"));
	auto btnBack = dynamic_cast<Button*>(layerOnlyBack->getChildByName("btnBack"));
	if (btnBack != NULL){
		btnBack->addClickEventListener(CC_CALLBACK_1(LayerChuyenTien::onBtnClose, this));
		btnBack->setPressedActionEnabled(true);
	}

	btnChuyenTien = dynamic_cast<Button*>(nodeChuyenTienTop->getChildByName("btnChuyenTien"));
	if (btnChuyenTien != NULL){
		btnChuyenTien->addClickEventListener(CC_CALLBACK_1(LayerChuyenTien::onBtnChuyenTienClicked, this));
	}
    btnDoiTien = dynamic_cast<Button*>(nodeChuyenTienTop->getChildByName("btnDoiTien"));
    if (btnDoiTien != NULL){
        btnDoiTien->addClickEventListener(CC_CALLBACK_1(LayerChuyenTien::onBtnDoiTien, this));
    }
    
    doitien = LayerNapHao::create();
    this->addChild(doitien);
    doitien->setVisible(false);

	btnLichSu = dynamic_cast<Button*>(nodeChuyenTienTop->getChildByName("btnLichSu"));
	if (btnLichSu != NULL){
		btnLichSu->addClickEventListener(CC_CALLBACK_1(LayerChuyenTien::onBtnLichSuClicked, this));
	}
	// 	//ChuyenTien Content
	nodeContentChuyenTien = dynamic_cast<Node*>(rootChuyenTien->getChildByName("nodeContent"));
	nodeContentChuyenTien->setAnchorPoint(Point(0.5, 1));
	nodeContentChuyenTien->setPositionY(visibleSize.height - nodeChuyenTienTop->getContentSize().height);
    
    lblMoney = dynamic_cast<Text*>(nodeChuyenTienTop->getChildByName("lblGold"));
    lblSilver = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("lblSilver"));



	auto nodeText = dynamic_cast<Node*>(nodeContentChuyenTien->getChildByName("nodeText"));
	lblPhi = dynamic_cast<Text*>(nodeText->getChildByName("lblPhi"));
	txtMinTrade = dynamic_cast<Text*>(nodeText->getChildByName("txtMin"));
	if (txtMinTrade)
		txtMinTrade->setString(StringUtils::format("%s %s", mUtils::format_money((long)SceneManager::getSingleton().getMinMoney(3))->c_str(),SceneManager::getSingleton().getCoin(0).c_str()));
	Text* txtMinNeed = dynamic_cast<Text*>(nodeText->getChildByName("txtMinNeed"));
	if (txtMinNeed)
		txtMinNeed->setString(StringUtils::format("%s %s", mUtils::format_money((long)SceneManager::getSingleton().getMinMoneyTranfer())->c_str(),SceneManager::getSingleton().getCoin(0).c_str()));

	Text* txtMinVip = dynamic_cast<Text*>(nodeText->getChildByName("txtMinVip"));
	if (txtMinVip)
		txtMinVip->setString(StringUtils::format("- VIP %d trở lên mới được chuyển khoản.", SceneManager::getSingleton().getMinVipTranfer()));
	lblMoneyPhi = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("lblTienPhi"));
	lblSumMoney = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("lblTongTien"));

	///username chuyen vs so tien - MINHDV
	txtAcc = dynamic_cast<TextField*>(nodeContentChuyenTien->getChildByName("txtNick"));
	txtValue = dynamic_cast<TextField*>(nodeContentChuyenTien->getChildByName("txtGiaTri"));
    txtContent = dynamic_cast<TextField*>(nodeContentChuyenTien->getChildByName("txtContent"));
	txtAcc->setMaxLengthEnabled(true);
	txtValue->setMaxLengthEnabled(true);
	txtAcc->setMaxLength(25);
    txtContent->setMaxLengthEnabled(true);
    txtContent->setMaxLength(200);
	txtValue->setMaxLength(9);


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	txtValue->setInputMode(ui::EditBox::InputMode::NUMERIC);
#endif

	lblPhi->setString(StringUtils::format("%0.0lf ", mTax) + "%");

	btnReceiver = dynamic_cast<Button*>(nodeContentChuyenTien->getChildByName("btnChuyenKhoan"));
	if (btnReceiver != NULL){
		btnReceiver->addClickEventListener(CC_CALLBACK_1(LayerChuyenTien::onBtnReceiverClicked, this));
		btnReceiver->setPressedActionEnabled(true);
	}

	bg = static_cast<Layout*>(rootChuyenTien->getChildByName("Panel_1"));

	bg->setContentSize(Size(WIDTH_DESIGN, visibleSize.height - nodeChuyenTienTop->getContentSize().height - 50));

	//validate
	lblErrorNick = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("lblErrorNick"));
	lblErrorNick->setVisible(false);
	lblErrorValue = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("lblErrorValue"));
	lblErrorValue->setVisible(false);
    lblErrorContent = dynamic_cast<Text*>(nodeContentChuyenTien->getChildByName("lblErrorContent"));
    lblErrorContent->setVisible(false);

	//setTabDefault
	btnChuyenTien->setEnabled(false);
	btnChuyenTien->setBright(false);
	lblMoneyPhi->setString("");
	lblSumMoney->setString("");

	//setValue to Money
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself != NULL){
		if (myself->GetVariable("amf") != NULL){
			boost::shared_ptr<double> amf = myself->GetVariable("amf")->GetDoubleValue();
			if (amf != NULL){
				lblMoney->setString(mUtils::convertMoneyEx(*amf).c_str());
			}
		}
        if (myself->GetVariable("amfs") != NULL){
            boost::shared_ptr<double> amfs = myself->GetVariable("amfs")->GetDoubleValue();
            if (amfs != NULL){
                lblSilver->setString(mUtils::convertMoneyEx(*amfs).c_str());
            }
        }
	}
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerChuyenTien::onScheduleUpdateCost),this,1, false);
	//
	loadAllDatas();
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
		/*this->btnChuyenTien->setTitleColor(Color3B::BLACK);
		this->btnLichSu->setTitleColor(Color3B::WHITE);*/
	}
    
    auto lstphi = mUtils::splitString(dataManager.getAppConfig().phichuyentien,'_');
    if (lstphi.size()>=2){
        Text* txtPhiDaiLy = static_cast<Text*>(nodeContentChuyenTien->getChildByName("txtPhiDaiLy"));
        if (txtPhiDaiLy)
            txtPhiDaiLy->setString(StringUtils::format("%s",lstphi[1].c_str())+ " %");
        Text* txtPhiNguoiChoi = static_cast<Text*>(nodeContentChuyenTien->getChildByName("txtPhiNguoiChoi"));
        if (txtPhiNguoiChoi)
            txtPhiNguoiChoi->setString(StringUtils::format("%s",lstphi[0].c_str())+ " %");
    }

	return true;
}

void LayerChuyenTien::createWebview(){
	//webview
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL || myself->Name() == NULL)
		return;
	/*init linkURL*/
	string urlLinkHistories=""; 
	if(SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		urlLinkHistories= LS_CHUYEN_TIEN_P2;
	}else{
		urlLinkHistories= LS_CHUYEN_TIEN;
	}
	urlLinkHistories += *myself->Name();
	urlLinkHistories += "&checksum=";
	urlLinkHistories += mUtils::getMD5KeyHis(*myself->Name());
	if (bg == NULL)
		return;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	auto webView = cocos2d::experimental::ui::WebView::create();
	webView->setAnchorPoint(bg->getAnchorPoint());
	webView->setPosition(bg->getPosition());
	webView->setContentSize(bg->getContentSize());
	webView->loadURL(urlLinkHistories);
	webView->setTag(24);
	webView->setScalesPageToFit(true);
	webView->setVisible(false);
	this->addChild(webView);

	webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerChuyenTien::onWebViewShouldStartLoading, this));
	webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerChuyenTien::onWebViewDidFinishLoading, this));
	webView->setOnDidFailLoading(CC_CALLBACK_2(LayerChuyenTien::onWebViewDidFailLoading, this));
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(urlLinkHistories);
#endif
}

void LayerChuyenTien::onBtnClose(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}
void LayerChuyenTien::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerChuyenTien::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->removeChildByTag(24);
}

void LayerChuyenTien::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(listener);
	Layer::onExit();
}

bool LayerChuyenTien::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
	return true;
}
void LayerChuyenTien::onBtnDoiTien(Ref* pSender){
    //SceneManager::getSingleton().showLoading();
    nodeContentChuyenTien->setVisible(false);
    btnChuyenTien->setEnabled(true);
    btnChuyenTien->setBright(true);
    btnLichSu->setEnabled(true);
    btnLichSu->setBright(true);
    btnDoiTien->setEnabled(false);
    btnDoiTien->setBright(false);
    doitien->setVisible(true);
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        /*this->btnChuyenTien->setTitleColor(Color3B::BLACK);
         this->btnLichSu->setTitleColor(Color3B::WHITE);*/
    }
    this->removeChildByTag(24);
    currTab = 2;
    
}
void LayerChuyenTien::onBtnChuyenTienClicked(Ref* pSender){
	SceneManager::getSingleton().showLoading();
	nodeContentChuyenTien->setVisible(true);
	btnChuyenTien->setEnabled(false);
	btnChuyenTien->setBright(false);
	btnLichSu->setEnabled(true);
	btnLichSu->setBright(true);
    btnDoiTien->setEnabled(true);
    btnDoiTien->setBright(true);
    doitien->setVisible(false);
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
		/*this->btnChuyenTien->setTitleColor(Color3B::BLACK);
		this->btnLichSu->setTitleColor(Color3B::WHITE);*/
	}
	this->removeChildByTag(24);
	currTab = 1;

}

void LayerChuyenTien::onBtnLichSuClicked(Ref* pSender){
	SceneManager::getSingleton().showLoading();
	btnChuyenTien->setEnabled(true);
	btnChuyenTien->setBright(true);
	btnLichSu->setEnabled(false);
	btnLichSu->setBright(false);
    btnDoiTien->setEnabled(true);
    btnDoiTien->setBright(true);
    doitien->setVisible(false);
	nodeContentChuyenTien->setVisible(false);
	createWebview();
	currTab = 3;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
		/*this->btnLichSu->setTitleColor(Color3B::BLACK);
		this->btnChuyenTien->setTitleColor(Color3B::WHITE);*/
	}
}

void LayerChuyenTien::onBtnReceiverClicked(Ref* pSender){
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL)
		return;
	if (myself->GetVariable("amf") == NULL)
		return;

	CCLOG("onBUttonReceiverClicked");
	string inputName = txtAcc->getString();
	std::transform(inputName.begin(), inputName.end(), inputName.begin(), ::tolower);

	//validate
	if (txtAcc->getString() == ""){
		lblErrorNick->setString("* "+dataManager.GetSysString(427));
		lblErrorNick->setVisible(true);
		return;
	}
	else{
		lblErrorNick->setVisible(false);
	}
	if (txtValue->getString() == ""){
		lblErrorValue->setString("* "+dataManager.GetSysString(428));
		lblErrorValue->setVisible(true);
		return;
	}
	else if (mUtils::findContainChar(txtValue->getString().c_str())){
		lblErrorValue->setString("* "+dataManager.GetSysString(429));
		lblErrorValue->setVisible(true);
		return;
	}
	else if (atoi(txtValue->getString().c_str()) > *myself->GetVariable("amf")->GetDoubleValue()){
		lblErrorValue->setString("* "+dataManager.GetSysString(430));
		lblErrorValue->setVisible(true);
		return;
	}
	else if (atoi(txtValue->getString().c_str()) < SceneManager::getSingleton().getMinMoney(3)){
		lblErrorValue->setString(StringUtils::format("%s %s!",dataManager.GetSysString(431).c_str(), mUtils::format_money((long)SceneManager::getSingleton().getMinMoney(3))->c_str()));
		lblErrorValue->setVisible(true);
		return;
	}
	else if (atoi(txtValue->getString().c_str()) > 1000000000){
		lblErrorValue->setString("* "+dataManager.GetSysString(432));
		lblErrorValue->setVisible(true);
		return;
	}


	else if (strcmp(inputName.c_str(), SceneManager::getSingleton().getMyName().c_str()) == 0)
	{
		lblErrorNick->setString("* "+dataManager.GetSysString(433));
		lblErrorNick->setVisible(true);
		return;
	}

	else if ((*myself->GetVariable("amf")->GetDoubleValue() - (atol(txtValue->getString().c_str()) + atol(lblMoneyPhi->getString().c_str()))) < SceneManager::getSingleton().getMinMoneyTranfer()){
		lblErrorValue->setString(StringUtils::format("* %s %s ",dataManager.GetSysString(434).c_str(), mUtils::format_money((long)SceneManager::getSingleton().getMinMoneyTranfer())->c_str()));
		lblErrorValue->setVisible(true);
		return;
	}
	else{
		lblErrorValue->setVisible(false);
	}

	if (myself->GetVariable("accVipType") == NULL || myself->GetVariable("amf") == NULL) return;

	int accVipType = *myself->GetVariable("accVipType")->GetIntValue();
	if (accVipType < SceneManager::getSingleton().getMinVipTranfer()) {
        Chat *toast = Chat::create(StringUtils::format("%s %d %s",dataManager.GetSysString(425).c_str(), SceneManager::getSingleton().getMinVipTranfer(),dataManager.GetSysString(23).c_str()), -1);
		this->addChild(toast);
		return;
	}
    
    if (txtContent->getString() == ""){
        lblErrorContent->setString("* "+ dataManager.GetSysString(426));
        lblErrorContent->setVisible(true);
        return;
    }else{
        lblErrorContent->setVisible(false);
    }
	//
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    int am = atoi(txtValue->getString().c_str());
    
    if (am < 0)
        return;
    params->PutUtfString("aI", txtAcc->getString());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_TYPE_AN_REQ, params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();

}

void LayerChuyenTien::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

	CCLOG("REceiver call function");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	CCLOG("onExtentions");
	layerThongBao = LayerPopupThongBao::create();
	layerThongBao->showAButton();
	if (strcmp("tfbalance", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
		int rc = *param->GetInt("rc");
		auto rd = *param->GetUtfString("rd");
		if (rc == 0) {
			layerThongBao->setContentMess("Chuyển tiền thành công!");
			this->addChild(layerThongBao);
            this->txtAcc->setString("");
            this->txtContent->setString("");
            this->txtValue->setString("");
        }
		else {
			layerThongBao->setMessage("THẤT BẠI");
			layerThongBao->setContentMess(rd + "");
			this->addChild(layerThongBao);
		}
	}
	else if (strcmp("respcf", cmd->c_str()) == 0){ //RESP_GET_CONFIG = "respcf";
		mTax = *param->GetDouble("cfv");//CONFIG_VALUE = "cfv";
		//
		lblPhi->setString(StringUtils::format("%0.0lf ", mTax) + "%");
		//recalculator
		onScheduleUpdateCost(1.0);
		//
		SceneManager::getSingleton().hideLoading();
	}
	else if (strcmp("notiRMFF", cmd->c_str()) == 0){
		if (currTab == 3)
			onBtnChuyenTienClicked(NULL);

	}
    else if (strcmp(EXT_EVENT_GET_TYPE_AN_RES, cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        int type = *param->GetInt("type");
        int fee = *param->GetInt("fee");
        float phichuyen = 0;
        if (fee != NULL)
            phichuyen = fee * (atof(txtValue->getString().c_str())) /100;
        if (type == 0){
            layerThongBao = LayerPopupThongBao::create();
            layerThongBao->showAButton();
            layerThongBao->setContentMess(dataManager.GetSysString(634));
            layerThongBao->setTag(24);
            this->addChild(layerThongBao);
        }else if (type == 1){
            layerThongBao = LayerPopupThongBao::create();
            layerThongBao->getBtnOk()->addClickEventListener(CC_CALLBACK_1(LayerChuyenTien::onBtnOKConfirm, this));
            layerThongBao->setContentMess(StringUtils::format("%s \n %s (%s = %s) \n %s %s ?",dataManager.GetSysString(517).c_str(),
                                                              mUtils::convertMoneyEx(atof(txtValue->getString().c_str())).c_str(),dataManager.GetSysString(518).c_str(), mUtils::convertMoneyEx(phichuyen).c_str(),dataManager.GetSysString(519).c_str(), txtAcc->getString().c_str()));
            layerThongBao->setTag(24);
            layerThongBao->getBtnOk()->setEnabled(true);
            this->addChild(layerThongBao);
        }else if (type == 2){
            layerThongBao = LayerPopupThongBao::create();
            layerThongBao->getBtnOk()->addClickEventListener(CC_CALLBACK_1(LayerChuyenTien::onBtnOKConfirm, this));
            layerThongBao->setContentMess(StringUtils::format("%s \n %s (%s = %s) \n %s %s ?",dataManager.GetSysString(517).c_str(),
                                                              mUtils::convertMoneyEx(atof(txtValue->getString().c_str())).c_str(),dataManager.GetSysString(518).c_str(), mUtils::convertMoneyEx(phichuyen).c_str(),dataManager.GetSysString(519).c_str(), txtAcc->getString().c_str()));
            layerThongBao->setTag(24);
            this->addChild(layerThongBao);
        }

        
        
    }
}

void LayerChuyenTien::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);

	if (ptrNotifiedUser->IsItMe() == true){
		boost::shared_ptr<double> amf;
		boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
		if (myself == NULL)
			return;
		if (myself->GetVariable("amf") == NULL)
			return;
		amf = myself->GetVariable("amf")->GetDoubleValue();
		lblMoney->setString(mUtils::convertMoneyEx(*amf).c_str());
        
        if (myself->GetVariable("amfs") != NULL){
            boost::shared_ptr<double> amfs = myself->GetVariable("amfs")->GetDoubleValue();
            if (amfs != NULL){
                lblSilver->setString(mUtils::convertMoneyEx(*amfs).c_str());
            }
        }
	}
}

void LayerChuyenTien::onBtnOKConfirm(Ref* pSender){
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL)
		return;
	if (myself->GetVariable("aN") == NULL)
		return;
    
    
    if (dataManager.getAppConfig().isactiveOTP){
        LayerPopupXacThucOTP* popup = LayerPopupXacThucOTP::create();
        popup->setCallBackFromButton(this, callfuncND_selector(LayerChuyenTien::onXacThucOTP));
        popup->setTag(29);
        this->addChild(popup);

    }
    else{
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        
        int am = atoi(txtValue->getString().c_str());
        
        if (am < 0)
            return;
        params->PutUtfString("aN", myself->GetVariable("aN")->GetStringValue());
        params->PutUtfString("aI", myself->Name());
        params->PutUtfString("aR", txtAcc->getString());
        params->PutUtfString("aRN", txtAcc->getString());
        //string content = txtContent->getString();
        params->PutUtfString("ct", txtContent->getString());
        params->PutInt("am", am);
        params->PutInt("ppf", 1);
        
        boost::shared_ptr<IRequest> request(new ExtensionRequest("tfbalance", params));
        GameServer::getSingleton().Send(request);
        
        SceneManager::getSingleton().showLoading();

    }
    this->removeChildByTag(24);

}
void LayerChuyenTien::onXacThucOTP(Node* sender, void* data)
{
    string *value = (string*)data;
    string otp = *value;
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    if (myself->GetVariable("aN") == NULL)
        return;
    int am = atoi(txtValue->getString().c_str());
    
    if (am < 0)
        return;
    params->PutUtfString("aN", myself->GetVariable("aN")->GetStringValue());
    params->PutUtfString("aI", myself->Name());
    params->PutUtfString("aR", txtAcc->getString());
    params->PutUtfString("aRN", txtAcc->getString());
    params->PutUtfString("ct", txtContent->getString());
    params->PutInt("am", am);
    params->PutInt("ppf", 1);
    params->PutUtfString("otpcode", otp);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("tfbalance", params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();

    this->removeChildByTag(29);
}
void LayerChuyenTien::loadAllDatas()
{
}

void LayerChuyenTien::onScheduleUpdateCost(float dt)
{
	if (txtValue != NULL && txtValue->getString().length() == 0 || mUtils::findContainChar(txtValue->getString().c_str())
		|| atoi(txtValue->getString().c_str()) < 10000 || atoi(txtValue->getString().c_str()) > 1000000000){
		lblMoneyPhi->setString("--");
		lblSumMoney->setString("--");
		return;
	}
	long currMoney = atol(txtValue->getString().c_str());
	lblMoneyPhi->setString(*mUtils::format_money(currMoney*mTax / 100));
	lblSumMoney->setString(*mUtils::format_money(currMoney + currMoney*mTax / 100));
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerChuyenTien::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().showLoading();
	sender->setVisible(false);
	return true;
}

void LayerChuyenTien::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
	sender->setVisible(true);
}

void LayerChuyenTien::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
}

#endif
void LayerChuyenTien::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    this->removeFromParentAndCleanup(true);
}
