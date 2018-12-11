#include "SceneLogin.h"

#include "Requests/LoginRequest.h"
#include "Requests/ExtensionRequest.h"
#include "../SceneManager.h"
#include "boost/algorithm/string.hpp"
#include "../layergames/_Chat_.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "Requests/LogoutRequest.h"
#include "Util/PasswordUtil.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "PluginFacebook/PluginFacebook.h"
#include "mFaceBook.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h>
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <string>

#pragma comment(lib, "wbemuuid.lib")
#endif

USING_NS_CC;

#define ZORDER_POPUP 2000
#define ZORDER_LOADING 20001

SceneLogin* SceneLogin::instance = NULL;
//std::string gPlusUid = "";
//std::string gPlusName = "";
//std::string gPlusEmail = "";
//std::string gPlusAvatar = "";
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "../layergames/NativeHelper.h"
//extern "C" {
//	//org/cocos2dx/cpp/AppActivity
//	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_loginGPlus(JNIEnv*  env, jobject thiz, jstring uid, jstring name, jstring email, jstring urlAvatar) {
//		gPlusUid = JniHelper::jstring2string(uid);
//		gPlusName = JniHelper::jstring2string(name);
//		gPlusEmail = JniHelper::jstring2string(email);
//		gPlusAvatar = JniHelper::jstring2string(urlAvatar);
//		//log("in cocos2d-x %s : %s : %s : %s", gPlusUid.c_str(), gPlusName.c_str(), gPlusEmail.c_str(), gPlusAvatar.c_str());
//		if (SceneLogin::instance != NULL)
//		{
//			SceneLogin::instance->socialInfo.uid = gPlusUid;
//			SceneLogin::instance->socialInfo.name = gPlusName;
//			SceneLogin::instance->socialInfo.email = gPlusEmail;
//			SceneLogin::instance->socialInfo.urlAvatar = gPlusAvatar;
//			if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
//				return;
//			}
//			SceneLogin::getInstance()->currLoginType = SceneLogin::gmailType;
//			if (!GameServer::getSingleton().getSmartFox()->IsConnected())
//			{
//				SceneManager::getSingleton().connect2Server(0);
//			}
//			else
//			{
//				//GameServer::getSingleton().getSmartFox()->Disconnect();
//				SceneManager::getSingleton().connect2Server(0);
//			}
//		}
//	}
//}
//#endif
#include "layergames/PhomMessDef.h"
#include "Requests/SetUserVariablesRequest.h"
#include "Entities/Variables/SFSUserVariable.h"


bool SceneLogin::instanceFlag = false;
//SceneManager scene = new SceneManager();

SceneLogin* SceneLogin::getInstance(){
	if (instance == NULL)
	{
		instance = SceneLogin::create();
		return instance;
	}
	else{
		return instance;
	}
}

bool SceneLogin::isInstanceExisted()
{
	if (instance == NULL)
		return false;
	return true;
}

void SceneLogin::DestroyInstance(){

}

SceneLogin::SceneLogin() : mCurrentNode(NULL)
{
	instanceFlag = true;
	btnLoginFace = NULL;
}

SceneLogin::~SceneLogin()
{
	//log("SceneLogin::~SceneLogin()");
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::scheduleUpdateContactInfo),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::hideToUp),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::setBonusFacebook),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::showMaintain),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::showUpdateInfo),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::gotoLogin),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::timeoutChatMaxtime),this);
	// 	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneManager::updateEvent),this,1/60, false);
	// 	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneManager::updateEvent4CheckConnection));
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::scheduleUpdateEditbox),this);
	GameServer::getSingleton().removeListeners(this);
}

cocos2d::Scene* SceneLogin::createScene()
{
	auto scene = Scene::create();

	//auto layer = SceneLogin::getInstance();
	auto layer = SceneLogin::create();
	scene->addChild(layer, 0, tag_SceneLogin);

	return scene;
}

bool SceneLogin::init()
{
	if (!Layer::init())
		return false;
	// 	return true;

	instance = this;
	isShouldAuto = true;
	isShow = true;
	this->setKeyboardEnabled(true);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	NativeHelper::sendPushNotification();
	//NativeHelper::callVibrate(5);
#endif

	Size visibleSize = Director::getInstance()->getVisibleSize();
// 	Sprite* background = Sprite::create("background.jpg");
// 	background->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
// 	background->setAnchorPoint(Point(0, 0));
// 	background->setPosition(Point(0, 0));
// 	this->addChild(background);

	this->createLayerLoading();

	GameServer::getSingleton().addListeners(this);

	auto childNode = CSLoader::getInstance()->createNode("LayerLogin.csb");
	childNode->setContentSize(visibleSize);
	ui::Helper::doLayout(childNode);

	childNode->setTag(tagChildNode);

	this->addChild(childNode);

	Size winSize = Director::getInstance()->getVisibleSize();

	pnlLogin = dynamic_cast<Layout*>(childNode->getChildByName("pnlLogin"));
	pnlRegist = dynamic_cast<Layout*>(childNode->getChildByName("pnlRegist"));
	ImageView* Image_6 = (ImageView*)childNode->getChildByName("Image_6");
	if (Image_6)
	{
		Image_6->setContentSize(winSize);
	}

	std::string strHotLine = dataManager.getAppConfig().callcenter + "";

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		//imgGirl->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
		imgFade = dynamic_cast<ImageView*>(pnlLogin->getChildByName("img_fade"));
		imgFadeRegister = dynamic_cast<ImageView*>(pnlRegist->getChildByName("img_fade_register"));
		pnlFade = dynamic_cast<Layout*>(childNode->getChildByName("pnl_fade"));
		pnlFade->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);

		auto btnXem = dynamic_cast<Button*>(childNode->getChildByName("btn_xem"));
		if (btnXem != NULL){
			btnXem->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonOtherClicked, this));
			btnXem->setSwallowTouches(true);
			btnXem->setPressedActionEnabled(true);
		}

		auto btnGame1 = dynamic_cast<Button*>(childNode->getChildByName("btnGame1"));
		if (btnGame1 != NULL){
			btnGame1->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonOtherClicked, this));
			btnGame1->setPressedActionEnabled(true);
		}

		auto btnGame2 = dynamic_cast<Button*>(childNode->getChildByName("btnGame2"));
		if (btnGame2 != NULL){
			btnGame2->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonOtherClicked, this));
			btnGame2->setPressedActionEnabled(true);
		}

		auto btnGame3 = dynamic_cast<Button*>(childNode->getChildByName("btnGame3"));
		if (btnGame3 != NULL){
			btnGame3->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonOtherClicked, this));
			btnGame3->setPressedActionEnabled(true);
		}

        Size sizeAdd;
        sizeAdd = ChanUtils::getSizePos();
		lblHotLine = dynamic_cast<Text*>(childNode->getChildByName("txtHotline"));
		//lblHotLine->setPositionY(childNode->getPositionY() + sizeAdd.height);
        lblHotLine->setString(strHotLine);
        //lblHotLine->setString("0984.234.567");
        lblHotLine->setVisible(!dataManager.getAppConfig().isHideBilling);
		btnHotLine = dynamic_cast<Button*>(childNode->getChildByName("btnHotLine"));
        //btnHotLine->setPositionY(btnHotLine->getPositionY()+sizeAdd.height);
        
        btnLoginGPlus = dynamic_cast<Button*>(childNode->getChildByName("btnLoginGmail"));
        btnLoginFace = dynamic_cast<Button*>(childNode->getChildByName("btnLoginFace"));
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		//auto posX = pnlListDT->getPositionX();
		//auto posY = pnlListDT->getPositionY();// + distance;

		//auto sizeAdd = Size(0, 0);
		//webView = cocos2d::experimental::ui::WebView::create();
		//webView->setAnchorPoint(Vec2(0, 0));
		//webView->setPosition(Vec2(posX - 10, posY - sizeAdd.height + 10));
		//webView->setContentSize(Size(pnlListDT->getContentSize().width, pnlListDT->getContentSize().height));
		//webView->setScalesPageToFit(true);
		//this->addChild(webView);
		//webView->setVisible(false);

		//webView->setOnShouldStartLoading(CC_CALLBACK_2(SceneLogin::onWebViewShouldStartLoading, this));
		//webView->setOnDidFinishLoading(CC_CALLBACK_2(SceneLogin::onWebViewDidFinishLoading, this));
		//webView->setOnDidFailLoading(CC_CALLBACK_2(SceneLogin::onWebViewDidFailLoading, this));
#endif
	}
	else{
		btnHotLine = dynamic_cast<Button*>(childNode->getChildByName("btnHotLine"));
		btnLoginGPlus = dynamic_cast<Button*>(pnlLogin->getChildByName("btnLoginGmail"));
		btnLoginFace = dynamic_cast<Button*>(pnlLogin->getChildByName("btnLoginFace"));
		btnHotLine->setTitleText("            "+strHotLine);
		btnHotLine->setPositionY(winSize.height - btnHotLine->getContentSize().height / 2);
	}

	if (btnHotLine != NULL){
		btnHotLine->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonHotLineClicked, this));
		btnHotLine->setPressedActionEnabled(true);
	}
	Sprite* sprite_loading_logo = dynamic_cast<Sprite*>(childNode->getChildByName("sprite_loading_logo"));
	if (sprite_loading_logo != NULL){
		sprite_loading_logo->setVisible(true);
	}

	pnlLoading = dynamic_cast<Layout*>(childNode->getChildByName("pnlLoading"));
	if (pnlLoading != NULL){
		pnlLoading->setVisible(false);
	}

	if (pnlRegist != NULL){
		pnlRegist->setVisible(false);

		auto btnSignIn = dynamic_cast<Button*>(pnlRegist->getChildByName("btnSignIn"));
		if (btnSignIn != NULL){
			btnSignIn->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonCreateAcc, this));
			btnSignIn->setPressedActionEnabled(true);
		}
		auto btnBackToLogin = dynamic_cast<Button*>(pnlRegist->getChildByName("btnLogin"));
		if (btnBackToLogin != NULL){
			btnBackToLogin->addClickEventListener(CC_CALLBACK_1(SceneLogin::backToLogin, this));
			btnBackToLogin->setPressedActionEnabled(true);
		}

		txtRegUserName = dynamic_cast<TextField*>(pnlRegist->getChildByName("txtUsername"));
		txtRegPassword = dynamic_cast<TextField*>(pnlRegist->getChildByName("txtPassword"));
		txtReRegUserName = dynamic_cast<TextField*>(pnlRegist->getChildByName("txtRePassword"));
	}

	if (pnlLogin != NULL){

		pnlLogin->setVisible(true);

		Button* btnLogin = dynamic_cast<Button*>(pnlLogin->getChildByName("btnLogin"));
		if (btnLogin != NULL){
			btnLogin->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonLogin, this));
			btnLogin->setPressedActionEnabled(true);
		}


		if (btnLoginFace != NULL){
			btnLoginFace->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonLoginFacebook, this));
			btnLoginFace->setPressedActionEnabled(true);
			auto ActionFade = RepeatForever::create(Sequence::create(FadeTo::create(0.7, 50), FadeTo::create(0.7, 254), 0, nullptr));
			btnLoginFace->runAction(ActionFade);

			txtMoney = static_cast<Text*>(btnLoginFace->getChildByName("txtMoneyGet"));
			int moneyBonus = SceneManager::getSingleton().getLoginBonus() + SceneManager::getSingleton().getFacebookBonus();
			txtMoney->setString(StringUtils::format("+%d", moneyBonus));
			txtMoney->setVisible(true);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			if (strcmp(dataManager.getAppConfig().maintenancestr.c_str(), "") == 0){
				btnLoginFace->setVisible(false);
			}
#endif
		}


		if (btnLoginGPlus != NULL){
			btnLoginGPlus->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonLoginGPlus, this));
			btnLoginGPlus->setPressedActionEnabled(true);
		}

		labelDNB = static_cast<Label*>(pnlLogin->getChildByName("Text_4"));

		Button* btnSignIn = dynamic_cast<Button*>(pnlLogin->getChildByName("btnSignIn"));
		if (btnSignIn != NULL){
			btnSignIn->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonGotoCreateAcc, this));
			btnSignIn->setPressedActionEnabled(true);
		}

		Button* btnForgetPassword = static_cast<ui::Button*>(pnlLogin->getChildByName("btnForgetPassword"));
		if (btnForgetPassword != NULL){
			btnForgetPassword->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonForgetPassword, this));
			btnForgetPassword->setPressedActionEnabled(true);
		}

		auto btnForgetPass = static_cast<ui::Button*>(pnlLogin->getChildByName("btnForgetPass"));
		if (btnForgetPass != NULL){
			btnForgetPass->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonForgetPassword, this));
			btnForgetPass->setPressedActionEnabled(true);
		}

		Button* btnAutoLogin = static_cast<ui::Button*>(pnlLogin->getChildByName("btnAutoLogin"));
		if (btnAutoLogin != NULL){
			btnAutoLogin->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonAutoLogin, this));
		}

		Button* btnPLayNotSignIn = static_cast<ui::Button*>(pnlLogin->getChildByName("btnPLayNotSignIn"));
		if (btnPLayNotSignIn != NULL){
			btnPLayNotSignIn->addClickEventListener(CC_CALLBACK_1(SceneLogin::onButtonQuickPlay, this));
			btnPLayNotSignIn->setPressedActionEnabled(true);
		}

		imgAutoLogin = static_cast<ui::ImageView*>(pnlLogin->getChildByName("imgCheckBox"));
		logo = static_cast<Sprite*>(pnlLogin->getChildByName("Sprite_2"));

		bool isAuto = UserDefault::getInstance()->getBoolForKey("isAutoLogin");
		if (!isAuto)
			imgAutoLogin->loadTexture("checkbox-active.png");
		else
			imgAutoLogin->loadTexture("checkbox.png");

		txtUserName = dynamic_cast<TextField*>(pnlLogin->getChildByName("txtUsername"));
		txtPassword = dynamic_cast<TextField*>(pnlLogin->getChildByName("txtPassword"));

		iconPhone = static_cast<Sprite*>(childNode->getChildByName("sprPhoneIcon"));
	}
	currLoginType = UserDefault::getInstance()->getIntegerForKey("logintype");

	Label* versionLabel = Label::createWithTTF("version 1.1", kNormalFont, 40);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	versionLabel->setString("version 2.1.0");
#endif
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		versionLabel->setString("version 2.0.0");
	}
	versionLabel->setColor(Color3B::ORANGE);
	versionLabel->setAnchorPoint(Vec2(0, 0));
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		versionLabel->setPosition(Vec2(30, 0));
	}
	else{
		versionLabel->setPosition(Vec2(30, 30));
	}
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		//		Sprite* logoIcon = Sprite::create("logo.png");
		//		logoIcon->setAnchorPoint(Vec2(0, 0));
		//		logoIcon->setPosition(50, 100);
		//		this->addChild(logoIcon);
#endif
	}
	this->addChild(versionLabel);
    
    
//    SceneManager::getSingleton().setIsShowTX(false);
//    SceneManager::getSingleton().setIsShowPK(false);
//    SceneManager::getSingleton().setIsShowTCC(false);
    

	Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneLogin::showMaintain),this,1, false);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneLogin::showUpdateInfo),this,1, false);

	//this->scheduleOnce(schedule_selector(SceneLogin::gotoLogin), 4);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneLogin::gotoLogin), this, 4, 1, 4, false);
	// 	Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::updateEvent),this,1/60, false);
	// 	Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::updateEvent4CheckConnection), 1);

	//this->scheduleOnce(schedule_selector(SceneLogin::scheduleUpdateEditbox),1);
        Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneLogin::scheduleUpdateEditbox), this, 1, 1, 1, false);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneLogin::scheduleUpdateContactInfo),this,1, false);

	Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneLogin::hideToUp),this,1, false);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneLogin::setBonusFacebook),this,1, false);
	startTimeOutChat();
	partnerID = "1";
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	IOS::logScreenToGA("Đăng nhập");
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	NativeHelper::trackScreen("Login");
#endif
	return true;
}
void SceneLogin::timeoutChatMaxtime(float dt){
	SceneManager::getSingleton().timerchat++;
	//log("timer chat couting!");
	if (SceneManager::getSingleton().timerchat == dataManager.getAppConfig().chattimeout*SceneManager::getSingleton().sec) {
		SceneManager::getSingleton().timerchat = 0;
		SceneManager::getSingleton().chatcounttime = 0;
		SceneManager::getSingleton().arrCurrentChat.clear();
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::timeoutChatMaxtime),this);
	}
}
void SceneLogin::startTimeOutChat(){
	if (SceneManager::getSingleton().timerchat > 0){
		Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneLogin::timeoutChatMaxtime),this,1, false);
	}
}
void SceneLogin::getVersionIOS(float dt){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//SceneManager::getSingleton().getVersionFromUrl();
#endif
}
void SceneLogin::showMaintain(float dt){
	if (strcmp(dataManager.getAppConfig().maintenancestr.c_str(), "") != 0){
		isShouldAuto = false;
		if (isShow){
			//Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::showUpdateInfo));

			Scene* _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;

			LayerPopupThongBao* _layer = LayerPopupThongBao::create();
			_layer->setMessage(dataManager.GetSysString(141));
			_layer->showAButton();
			_layer->setCallBackFromButton(this, callfuncO_selector(SceneLogin::onButtonDongy), 1);
			string mess = dataManager.getAppConfig().maintenancestr + "\nClick \"Đồng ý\" để thoát app!";
			_layer->setContentMess(mess);
			this->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_MAINTAIN, 1942);
			isShow = false;

		}
		return;
	}
}
void SceneLogin::showUpdateInfo(float dt){
    auto lstVersion = mUtils::splitString(dataManager.getAppConfig().versionforceupdate, ';');
    vector<string> vsIOS;
    vector<string> vsAndroid;
    string is = "ios";
    string as = "and";
    for (int i = 0; i < lstVersion.size(); i++) {
        string vs = lstVersion[i];
        if (vs.find(is) != std::string::npos) {
            vsIOS.push_back(vs);
        }
        if (vs.find(as) != std::string::npos) {
            vsAndroid.push_back(vs);
        }
    }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for (int i = 0; i < vsIOS.size(); i++) {
        if (strcmp(vsIOS[i].c_str(), dataManager.getAppConfig().version_ios.c_str()) == 0){
            dataManager.getAppConfig().force_update = "-1";
        }
    }
    
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    for (int i = 0; i < vsAndroid.size(); i++) {
        if (strcmp(vsAndroid[i].c_str(), dataManager.getAppConfig().version_android.c_str()) == 0){
            dataManager.getAppConfig().force_update = "-1";
        }
    }
#endif
    if (strcmp(dataManager.getAppConfig().force_update.c_str(), "1") == 0){
		isShouldAuto = false;
		if (isShow){
			Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::showUpdateInfo),this);

			Scene* _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;

			LayerPopupThongBao* _layer = LayerPopupThongBao::create();
			_layer->setMessage(dataManager.GetSysString(141));
			_layer->showAButton();
			_layer->setCallBackFromButton(this, callfuncO_selector(SceneLogin::onButtonDongy), 4);
			string mess = "Hiện tại đã có phiên bản mới. Bạn hãy update để có thể trải nghiệm các tính năng mới.\nClick \"Đồng ý\" để đến store!";
			_layer->setContentMess(mess);
			this->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_MAINTAIN, 1943);
			isShow = false;

		}
		return;
	}
	else  if (strcmp(dataManager.getAppConfig().force_update.c_str(), "0") == 0){
		isShouldAuto = false;
		if (isShow){
			Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::showUpdateInfo),this);

			Scene* _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;

			LayerPopupThongBao* _layer = LayerPopupThongBao::create();
			_layer->setMessage(dataManager.GetSysString(141));
			// _layer->showAButton();
			_layer->setCallBackFromButton(this, callfuncO_selector(SceneLogin::onButtonDongy), 4);
			string mess = "Hiện tại đã có phiên bản mới. Bạn hãy update để có thể trải nghiệm các tính năng mới.\nClick \"Đồng ý\" để đến store!";
			_layer->setContentMess(mess);
			this->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_MAINTAIN, 1943);
			isShow = false;

		}
		return;
	}
}
static inline void printAllChildrenTypes(Node* node, int level)
{
	std::string spaces(level, ' ');
	//log("%s %02d : %s - name: %s", spaces.c_str(), level, typeid(*node).name(), node->getName().c_str());

	++level;
	int index = 0;

	while (index < node->getChildren().size()){
		Node* obj = node->getChildren().at(index);
		printAllChildrenTypes(static_cast<Node*>(obj), level);
		index++;
	}
}

void SceneLogin::gotoLogin(float dt)
{
	if (!isShouldAuto)
		return;
	int lastLoginType = UserDefault::getInstance()->getIntegerForKey("logintype");
	bool isAuto = UserDefault::getInstance()->getBoolForKey("isAutoLogin");
	switch (lastLoginType){
	case 2:
		if ((!SceneManager::getSingleton().isLogout&&!isAuto) || SceneManager::getSingleton().rcForLogin){
			onButtonLoginFacebook(NULL);
			pnlLogin->setVisible(true);
			pnlRegist->setVisible(false);
			readInfo();
			return;
		}
	case 3:
		if ((!SceneManager::getSingleton().isLogout&&!isAuto) || SceneManager::getSingleton().rcForLogin){
			onButtonLoginGPlus(NULL);
			pnlLogin->setVisible(true);
			pnlRegist->setVisible(false);
			readInfo();
			return;
		}
	case 4:
		if ((!SceneManager::getSingleton().isLogout&&!isAuto) || SceneManager::getSingleton().rcForLogin){
			onButtonQuickPlay(NULL);
			pnlLogin->setVisible(true);
			pnlRegist->setVisible(false);
			readInfo();
			return;
		}
	case 1:
		if (!SceneManager::getSingleton().isLogout&&!isAuto){
			//SceneManager::getSingleton().showLoading();
			onButtonLogin(NULL);
			pnlLogin->setVisible(true);
			pnlRegist->setVisible(false);
			readInfo();
			return;
		}
	}

	pnlLogin->setVisible(true);
	pnlRegist->setVisible(false);
	readInfo();
}

void SceneLogin::onButtonHotLineClicked(Ref* pSender){
	std::string strHotLine = dataManager.getAppConfig().callcenter + "";
	auto nums = mUtils::splitString(strHotLine, '.');
	strHotLine.erase(std::remove(strHotLine.begin(), strHotLine.end(), '.'), strHotLine.end());
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	NativeHelper::callPhone(strHotLine.c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	IOS::tryToCall(strHotLine);
#endif
}

void SceneLogin::onButtonForgetPassword(Ref* pSender){
	auto _layer = LayerPopupForgetPassword::create();
	if (_layer != NULL){
		this->addChild(_layer);
	}
}

void SceneLogin::gotoRegister()
{
	pnlLogin->setVisible(false);
	pnlRegist->setVisible(true);
	currLoginType = registType;
	//
	if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
		return;
	}
	if (!GameServer::getSingleton().getSmartFox()->IsConnected()){
		SceneManager::getSingleton().connect2Server(0);
		return;
	}
	doLogin(registType);
}
void SceneLogin::onEnter(){
    Layer::onEnter();
    this->txtPassword->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
	this->txtRegPassword->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
	this->txtReRegUserName->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
}
void SceneLogin::onExit(){
    Layer::onExit();
}
void SceneLogin::onButtonLogin(Ref* pSender)
{
	/*showMaintain(0);
	showUpdateInfo(0);*/
	// 	boost::shared_ptr<ISFSObject> parameters(new SFSObject());
	// 	int  osid = 2; //ios
	// 	std::string devID = "3e02ca7728d74aa8";
	// 	/*bool isConnectting = true;*/
	// 	UserDefault::getInstance()->setIntegerForKey("logintype", 1);
	// 	UserDefault::getInstance()->setStringForKey("LT", "sp");
	// 	UserDefault::getInstance()->setStringForKey("google_id", "");
	// 	UserDefault::getInstance()->setStringForKey("facebook_id", "");
	//
	// 	string email_ = "";
	// 	parameters->PutUtfString("LT", "sp");
	// 	parameters->PutUtfString("aI", "");
	// 	parameters->PutUtfString("aN", "");
	// 	parameters->PutUtfString("imc", "mobile");
	// 	parameters->PutUtfString("aM", "");
	// 	parameters->PutUtfString("LSK", "");
	// 	parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
	// 	parameters->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
	// 	parameters->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
	// 	parameters->PutInt("osid", 1);// Operatorsystems
	// 	parameters->PutUtfString("devid", devID);// decvice id
	// 	parameters->PutUtfString("aIp", "");//ip
	// 	parameters->PutInt("srcref", 0);
	// 	string _ai = mUtils::replaceAll(devID, "-", "");
	//
	// 	boost::shared_ptr<IRequest> req_login(new LoginRequest(_ai, "", "GameZone1", parameters));
	// 	GameServer::getSingleton().SendLogin(req_login);
	//
	// 	return;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	IOS::tryIsInternetConnection();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	NativeHelper::hasConnection();
#endif
	if (txtUserName == NULL || txtPassword == NULL)
		return;
	std::string strS = string(txtUserName->getString());
	if (mUtils::trimString(strS).length() < 6)
	{
		Chat *toast = Chat::create(dataManager.GetSysString(39), -1);
		this->addChild(toast);
		return;
	}

	if (mUtils::findSpecialCharNotSpace(string(txtUserName->getString()))){
		Chat *toast = Chat::create(dataManager.GetSysString(40), -1);
		this->addChild(toast);
		return;
	}

	strS = string(txtPassword->getString());
	if (mUtils::trimString(strS).length() < 6)
	{
		Chat *toast = Chat::create(dataManager.GetSysString(41), -1);
		this->addChild(toast);
		return;
	}
	if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
		Chat *toast = Chat::create(dataManager.GetSysString(211), -1);
		this->addChild(toast);
		return;
	}
	currLoginType = loginType;
	if (!GameServer::getSingleton().getSmartFox()->IsConnected())
	{
		SceneManager::getSingleton().connect2Server(0);
	}
	else
	{
		//GameServer::getSingleton().getSmartFox()->Disconnect();
		SceneManager::getSingleton().connect2Server(0);
	}
}

void SceneLogin::onButtonOtherClicked(Ref* pSender){
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		auto delay = DelayTime::create(5.0);

		/*auto callBackShowFade = CallFunc::create(CC_CALLBACK_0(SceneLogin::showFade, this));
		auto callBackShowFade = CallFunc::create(CC_CALLBACK_0(SceneLogin::hideFade, this));*/
		auto callBackShowFade = CallFunc::create([this](){
			this->showFade();
		});

		auto callBackHideFade = CallFunc::create([this](){
			this->hideFade();
		});

		auto seq = Sequence::create(callBackShowFade, delay, callBackHideFade, nullptr);
		this->runAction(seq);
	}
}

void SceneLogin::showFade(){
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		pnlFade->setVisible(true);
		imgFade->setVisible(true);
		imgFadeRegister->setVisible(true);
	}
}

void SceneLogin::hideFade(){
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		pnlFade->setVisible(false);
		imgFade->setVisible(false);
		imgFadeRegister->setVisible(false);
	}
}

void SceneLogin::onButtonAutoLogin(Ref* pSender)
{
    UserDefault::getInstance()->setBoolForKey("isAutoLogin", false);


}

void SceneLogin::onButtonLoginFacebook(Ref* pSender)
{
	showMaintain(0);
	showUpdateInfo(0);
	//log("onButtonLoginFacebook press");
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	mFaceBook *face = new mFaceBook();
    face->onLogin();
#endif
}

void SceneLogin::onButtonLoginGPlus(Ref* pSender)
{
	showMaintain(0);
	showUpdateInfo(0);
	//log("onButtonLoginGPlus press");
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	NativeHelper::googlePlusLogin();
#endif
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
	IOS::tryLoginGmail();
#endif
}

void SceneLogin::onButtonGotoCreateAcc(Ref* pSender)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		pnlFade->setVisible(false);
		imgFade->setVisible(false);
		imgFadeRegister->setVisible(false);
	}
	showMaintain(0);
	showUpdateInfo(0);
	gotoRegister();
}

void SceneLogin::onButtonCreateAcc(Ref* pSender)
{

	string strS = string(txtRegUserName->getString());

	if (mUtils::trimString(strS).length() < 6){
		Chat *toast = Chat::create(dataManager.GetSysString(39), -1);
		this->addChild(toast);
		return;
	}
	if (mUtils::findSpecialCharNotSpace(string(txtRegUserName->getString())) == true){
		Chat *toast = Chat::create(dataManager.GetSysString(40), -1);
		this->addChild(toast);
		return;
	}
	strS = string(txtRegPassword->getString());

	for (int i = 0; i < strS.size(); ++i)
	{
		if (strS.at(i) == ' '){
			Chat *toast = Chat::create("Mật khẩu không được chứa ký tự đặc biệt", -1);
			this->addChild(toast);
			return;
		}
	}

	if (mUtils::trimString(strS).length() < 6){
		Chat *toast = Chat::create(dataManager.GetSysString(41), -1);
		this->addChild(toast);
		return;
	}

	if (strcmp(txtRegUserName->getString().c_str(), txtRegPassword->getString().c_str()) == 0){
		Chat *toast = Chat::create("Tài khoản và mật khẩu không được giống nhau!", -1);
		this->addChild(toast);
		return;
	}

	if (strcmp(txtRegPassword->getString().c_str(), txtReRegUserName->getString().c_str()) != 0){
		Chat *toast = Chat::create(dataManager.GetSysString(42), -1);
		this->addChild(toast);
		return;
	}

	if (mUtils::isNumber(txtRegPassword->getString().c_str())){
		Chat *toast = Chat::create("Mật khẩu phải bao gồm cả số và chữ!", -1);
		this->addChild(toast);
		return;
	}
	if (!mUtils::checkAlpha(txtRegPassword->getString().c_str())){
		Chat *toast = Chat::create("Mật khẩu phải bao gồm cả số và chữ!", -1);
		this->addChild(toast);
		return;
	}

	if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
		Chat *toast = Chat::create(dataManager.GetSysString(211), -1);
		this->addChild(toast);
		return;
	}
	currLoginType = registType;
	if (!GameServer::getSingleton().getSmartFox()->IsConnected())
	{
		SceneManager::getSingleton().connect2Server(0);
		return;
	}
	//regist
	//Send request
    
    
    
	boost::shared_ptr<ISFSObject> params(new SFSObject());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    params->PutUtfString("lav", dataManager.getAppConfig().version_android);// LOGIN_APP_VERSION = "lav";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    params->PutUtfString("lav", dataManager.getAppConfig().version_ios);// LOGIN_APP_VERSION = "lav";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    //parameters->PutUtfString("lav", "ios1452832982");// Only for test
    params->PutUtfString("lav", dataManager.getAppConfig().version_win);
#endif
	int  osid = 2; //ios
	std::string devID = "1111111111sssaa";
	/*bool isConnectting = false;*/
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

	string _username = txtRegUserName->getString();
	boost::algorithm::to_lower(_username);
	params->PutUtfString("aI", _username);
	params->PutUtfString("aN", _username);
	boost::shared_ptr<string> _pass = PasswordUtil::MD5Password(txtRegPassword->getString());
	_pass = PasswordUtil::MD5Password(*_pass);
	params->PutUtfString("aP", *_pass);
	params->PutUtfString("aM", "");
	params->PutInt("aT", 1);
	params->PutInt("aS", 1);
	params->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
	params->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
	params->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";

	params->PutInt("osid", osid);// Operatorsystems
	params->PutUtfString("devid", devID);// decvice id

	boost::shared_ptr<IRequest> request(new ExtensionRequest("rr", params));
	GameServer::getSingleton().Send(request);
	SceneManager::getSingleton().showLoading();
}

void SceneLogin::OnSmartFoxConnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSuccess = (*ptrEventParams)["success"];
	boost::shared_ptr<bool> ptrValue = (boost::static_pointer_cast<bool>)(ptrEventParamValueSuccess);

	if (*ptrValue != true)//not success
	{
		Chat *toast = Chat::create(dataManager.GetSysString(472), -1);
		this->addChild(toast);
		//log("LayerLogin: OnSmartFoxConnection -- !GameServer::getSingleton().getSmartFox()->IsConnected()");
		return;
	}
	//log("LayerLogin: OnSmartFoxConnection");
	if (!isShouldAuto)
		return;
	if (currLoginType == registType){
		doLogin(currLoginType);
		return;
	}
	if (currLoginType == facebookType || currLoginType == gmailType || currLoginType == quickType){
		doLogin(currLoginType);
		return;
	}

	if (txtUserName->getString().length() < 3 || txtPassword->getString().length() < 3)
		return;
	if (currLoginType == loginType)
		doLogin(currLoginType);

}

void SceneLogin::OnSmartFoxLogin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	SceneManager::getSingleton().hideLoading();
	if (currLoginType == registType)
		return;
	//log("LayerLogin::OnSmartFoxLogin() - Login OK");



	//save info
	saveInfo();

	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueDatas = (*ptrEventParams)["data"];
	boost::shared_ptr<ISFSObject> datas = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueDatas)));
	if (datas == NULL)
		return;
	//MinhDV check ip to show caro games - only for p1
	// 	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	// 	{
	// 		//public static final String IS_IN_VN = "invn";
	// 		boost::shared_ptr<long> invn_ptr = datas->GetInt("invn");
	// 		if (invn_ptr != NULL)
	// 		{
	// 			int invn = *invn_ptr;
	// 			if (invn == 0)
	// 			{
	// 				//show game caro
	// 				Scene* gameCaro = TicTacToeScene::scene();
	// 				Director::getInstance()->replaceScene(gameCaro);
	// 				return;
	// 			}
	// 		}
	// 	}

	boost::shared_ptr<long> mt = datas->GetInt("mt");
	if (mt != NULL){
		//log("mt = %ld", *mt);
		if (1 == *mt){

			return;

			Scene* _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;

			LayerPopupThongBao* _layer = LayerPopupThongBao::create();
			_layer->setMessage(dataManager.GetSysString(141));
			_layer->showAButton();
			_layer->setCallBackFromButton(this, callfuncO_selector(SceneLogin::onButtonDongy), 3);
			_layer->setContentMess("Hệ thống đang bảo trì \nVui lòng quay lại sau!");
			_currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_NOTIFICATION, 1942);
			return;
		}
	}
	else{
		//log("null");
	}


	//Read param, check already play game, if true, gotoGame
	/*
	 * Check aSt: . If not playing game, value = "null"
	 * account_id,zone_id,room_id,status,updated_time,game_id
	 */
	SceneManager::getSingleton().reloadChargeConfig();
	SceneManager::getSingleton().getNotificationFromUrl();
	SceneManager::getSingleton().getbtnRechargeFromUrl();
	SceneManager::getSingleton().getStatusEventFromUrl();

	if (datas == NULL){
	}
	//Check các cờ ẩn hiện
	dataManager.getAppConfig().isHideBilling = false;
	dataManager.getAppConfig().isHideExchange = false;
	dataManager.getAppConfig().isHideTransfer = false;
	if (datas->GetUtfString("ihb")){
		string _ihb = *datas->GetUtfString("ihb");
		//_ihb = "billing:0-exchange:1-transfer:0";
		//-billing:0-exchange:0-transfer:1-sms:0-card:0
		if (_ihb != ""){
			//split
			vector<string> lstVisibleInfos = mUtils::splitString(_ihb, '-');
			for (int i = 0; i < lstVisibleInfos.size(); i++){
				vector<string> lstChildInfos = mUtils::splitString(lstVisibleInfos.at(i), ':');
				if (lstChildInfos.size() != 2)
					continue;
				if (lstChildInfos.at(0).compare("billing") == 0){
					dataManager.getAppConfig().isHideBilling = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("delay_reqot") == 0){
					dataManager.getAppConfig().delay_reqot = atoi(lstChildInfos.at(1).c_str());
				}
                else if (lstChildInfos.at(0).compare("delay_rqoffer") == 0){
                    dataManager.getAppConfig().delay_rqoffer = atoi(lstChildInfos.at(1).c_str());
                }
				else if (lstChildInfos.at(0).compare("delay_reqdt") == 0){
					dataManager.getAppConfig().delay_reqdt = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("exchange") == 0){
					dataManager.getAppConfig().isHideExchange = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("transfer") == 0){
					dataManager.getAppConfig().isHideTransfer = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("game") == 0){
					dataManager.getAppConfig().sListHiddenGame = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("vip") == 0){
					dataManager.getAppConfig().sListVip = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("quaythuong") == 0){
					dataManager.getAppConfig().sListQuayThuong = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("sms") == 0){
					dataManager.getAppConfig().isHideSMS = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("card") == 0){
					dataManager.getAppConfig().isHideTheCao = lstChildInfos.at(1).compare("1") == 0;
				}
				else if (lstChildInfos.at(0).compare("minjoin") == 0)
				{
					dataManager.getAppConfig().minjoin = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("transvip") == 0){
					dataManager.getAppConfig().strMinTranfer = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("invitetime") == 0){
					dataManager.getAppConfig().invitetime = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("typetable") == 0)
				{
					SceneManager::getSingleton().setTypePickRoom(atoi(lstChildInfos.at(1).c_str()));
				}
				else if (lstChildInfos.at(0).compare("chatlevel") == 0)
				{
					dataManager.getAppConfig().chatlevel = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("chattimeout") == 0)
				{
					dataManager.getAppConfig().chattimeout = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("chattimemax") == 0)
				{
					dataManager.getAppConfig().chattimemax = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("bonusimg") == 0){
//                    dataManager.getAppConfig().urlBonusIMG = "http://" + lstChildInfos.at(1);
//                    SceneManager::getSingleton().getImageDownloadProcess()->downLoadImage(NULL, dataManager.getAppConfig().urlBonusIMG, 1);
				}
				else if (lstChildInfos.at(0).compare("mail_sup") == 0)
				{
					dataManager.getAppConfig().mailSup = lstChildInfos.at(1);
				}
				//config cho quy thuong tom cua ca
				else if (lstChildInfos.at(0).compare("notifiQT") == 0)
				{
					dataManager.getAppConfig().notifiQT = lstChildInfos.at(1);
				}
				else if (lstChildInfos.at(0).compare("timeToQT") == 0)
				{
					dataManager.getAppConfig().timeToQT = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("timetoNextWeek") == 0)
				{
					dataManager.getAppConfig().timetoNextWeek = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("firstBetTCC") == 0)
				{
					dataManager.getAppConfig().firstBetTCC = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("isShowLucky") == 0)
				{
					dataManager.getAppConfig().isShowLucky = lstChildInfos.at(1).compare("0") == 0;
				}
				else if (lstChildInfos.at(0).compare("sdtnaptien") == 0)
				{
					dataManager.getAppConfig().isHideXacThucSDT = lstChildInfos.at(1).compare("1") == 0;;
				}
                else if (lstChildInfos.at(0).compare("fbbonus") == 0)
                {
                    dataManager.getAppConfig().isfb = lstChildInfos.at(1);
                }
				else if (lstChildInfos.at(0).compare("tccLimitBet") == 0)
				{
					dataManager.getAppConfig().tccLimitCount = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("maxxito") == 0)
				{
					dataManager.getAppConfig().maxxito = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("maxlieng") == 0)
				{
					dataManager.getAppConfig().maxlieng = atoi(lstChildInfos.at(1).c_str());
				}
				else if (lstChildInfos.at(0).compare("pheketsat") == 0)
				{
					dataManager.getAppConfig().pheketsat = atoi(lstChildInfos.at(1).c_str());
				}
                else if (lstChildInfos.at(0).compare("minungtien") == 0)
                {
                    string min = lstChildInfos.at(1);
                    vector<string> lst = mUtils::splitString(min, '_');
                    dataManager.getAppConfig().minungtien.clear();
                    for (int i = 0; i < lst.size();i++){
                        dataManager.getAppConfig().minungtien.push_back(atoi(lst[i].c_str()));
                    }
                }
               // else if (lstChildInfos.at(0).compare("gamemini") == 0)
                //{
                   // dataManager.getAppConfig().gamemini = lstChildInfos.at(1);;
                //}

                else if (lstChildInfos.at(0).compare("otp") == 0)
                {
                    dataManager.getAppConfig().otp = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("ketsat") == 0)
                {
                    dataManager.getAppConfig().ketsat = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("ungtien") == 0)
                {
                    dataManager.getAppConfig().ungtien = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("ruongvip") == 0)
                {
                    dataManager.getAppConfig().ruongvip = lstChildInfos.at(1).compare("1") == 0;
                }
                else if (lstChildInfos.at(0).compare("phichuyentien") == 0)
                {
                    dataManager.getAppConfig().phichuyentien = lstChildInfos.at(1);
                }

                
			}
		}
	}
	if (datas->GetUtfString("data")){
		string data = *datas->GetUtfString("data");
		auto lstData = mUtils::splitString(data, '|');
		if (atoi(lstData[0].c_str()) == 0)
			dataManager.getAppConfig().isLoginFisrt = true;
		else
			dataManager.getAppConfig().isLoginFisrt = false;
		if (atoi(lstData[1].c_str()) == 0){
			dataManager.getAppConfig().isLoginFisrtDay = true;
			UserDefault::getInstance()->setBoolForKey("isShowEventDay", false);
			UserDefault::getInstance()->flush();
		}
		else
			dataManager.getAppConfig().isLoginFisrtDay = false;
		if (atoi(lstData[2].c_str()) == 0)
			dataManager.getAppConfig().isreceivedRegisterFace = false;
		else
			dataManager.getAppConfig().isreceivedRegisterFace = true;
		if (atoi(lstData[3].c_str()) == 0)
			dataManager.getAppConfig().isreceivedLinkFace = false;
		else
			dataManager.getAppConfig().isreceivedLinkFace = true;
	}
	if (datas->GetUtfString("LT")){
		string type = *datas->GetUtfString("LT");
		dataManager.getAppConfig().loginType = type;
	}
	if (datas->GetInt("hasotp")){
		int type = *datas->GetInt("hasotp");
		if (type == 0){
			dataManager.getAppConfig().isactiveOTP = false;
		}
		else if (type == 1){
			dataManager.getAppConfig().isactiveOTP = true;
		}
	}
	if (datas->GetInt("nord")){
		int num = *datas->GetInt("nord");
		dataManager.getAppConfig().NUM_REGISTER_DEVICE = num;
	}
	//Get list pin ( game )
	boost::shared_ptr<string> lipg = datas->GetUtfString("lipg");//LIST_PIN_GAMES = lipg
	SceneManager::getSingleton().setListPin("");
	if (lipg != NULL){
		SceneManager::getSingleton().setListPin(*lipg);
	}
	//Get Url promotion
	boost::shared_ptr<string> km = datas->GetUtfString("km");//PROMOTION_URL = "km";
	SceneManager::getSingleton().setUrlPromotion("");
	if (km != NULL){
		SceneManager::getSingleton().setUrlPromotion(*km);
		SceneManager::getSingleton().getPromotionFromUrl();
	}
	//	LayerMain::getSingleton().reloadChargeConfig();
	///get gameCONFIG - MINHDV

	boost::shared_ptr<string> gameconfig = datas->GetUtfString("gmcfg");
	if (gameconfig)
	{
		SceneManager::getSingleton().setGameConfig(gameconfig->c_str());
	}
	//get ip and set to user variable
	boost::shared_ptr<string> myIP = datas->GetUtfString("aIp");
	if (myIP)
	{
		boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
		boost::shared_ptr<string> _a(new string(myIP->c_str()));
		//boost::shared_ptr<string> _a(new string("192.168.1.1"));
		boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("aIp", _a, VARIABLETYPE_STRING));
		collectionUserVariable->push_back(variable);	//
		boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
		GameServer::getSingleton().Send(request);
	}
	//set user chicken variable
	UserDefault *def = UserDefault::getInstance();
	bool uck = def->getBoolForKey("UCK", true);
	setUckVariable(uck);

	boost::shared_ptr<string> ast = datas->GetUtfString("aSt");
	if (ast == NULL){
		SceneManager::getSingleton().gotoMain();
		return;
	}

	//log("ast = %s", ast->c_str());
	vector<string> lstDatas = mUtils::splitString(*ast, ',');
	//ast = bokinhvan115,GameZone1,TCCThuong,1,05/09/2017 17:09:30,133

	if (lstDatas.size() != 0)
	{
		int mGameID;
		int roomType = atoi(lstDatas.at(3).c_str());
		//mRoomID = atoi(lstDatas.at(2).c_str());
		mGameID = atoi(lstDatas.at(5).substr(0, 3).c_str());
		if (mGameID != kGameTaiXiu && mGameID != kGameQuayThuong && mGameID != kGameXocDia 
			&& mGameID != atoi(SERVICE_CHAT_ROOM.c_str()) && mGameID != kGameBauTom)
		{
			RejoinInfoStruct joinGameInfo;
			joinGameInfo.gameID = lstDatas.at(5);
			joinGameInfo.roomID = -1;
			joinGameInfo.roomName = lstDatas.at(2).c_str();
			joinGameInfo.pass = "";
			joinGameInfo.isNeedRejoin = true;
			SceneManager::getSingleton().setRejoinInfo(joinGameInfo);

			SceneManager::getSingleton().setGameID(atoi(joinGameInfo.gameID.substr(0, 3).c_str()));
			SceneManager::getSingleton().gotoPickGame(atoi(joinGameInfo.gameID.substr(0, 3).c_str()));
		}
		else if (mGameID == kGameBauTom || mGameID == kGameXocDia || mGameID == kGameTaiXiu)
		{
			SceneManager::getSingleton().setGameID(mGameID);
			SceneManager::getSingleton().setCurrRoomType(roomType);
			SceneManager::getSingleton().gotoPickGame(mGameID);
		}
		else
		{
			SceneManager::getSingleton().gotoMain();
		}
		// Kiểm tra xem room có tồn tại hay không, nếu tồn tại có nghĩa là
		// Mất kết nối lúc chơi game, bây giờ join lại. //ROOM_ID = "ri";
		// 		boost::shared_ptr<MatchExpression> expr(new MatchExpression("ri", NumberMatch::EQUALS, boost::make_shared<int>(mGameID)
		//             , MatchExpression::MatchExpressionValueType::MatchExpression_ValueOther));
		// 		// Send request
		// 		boost::shared_ptr<IRequest> request(new FindRoomsRequest(expr));
		// 		GameServer::getSingleton().Send(request);
		// 		if( GameServer::getSingleton().getSmartFox()->GetRoomById(mRoomID) == NULL)
		// 			return;
		//
		// 		//log("Ban dang choi game %s phong so %d", mUtils::getGameNameByID(mGameID), mRoomID);
		//		LayerMain::getSingleton().autoJoinGameWithID(mGameID, mRoomID);
	}
	else
	{
		//log("Ban khong choi game nao");
		SceneManager::getSingleton().gotoMain();
	}
}
void SceneLogin::OnSmartFoxLogout(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	SceneManager::getSingleton().hideLoading();

}
void SceneLogin::onButtonDongy(Ref* pSender){


	auto node = (Node*)pSender;
	int tag = node->getTag();

	if (tag == 3) {
		//Director::getInstance()->end();
		boost::shared_ptr<IRequest> request(new LogoutRequest());
		GameServer::getSingleton().Send(request);
		//
		SceneManager::getSingleton().isLogout = true;
		SceneManager::getSingleton().rcForLogin = false;

		Scene* _currScene = Director::getInstance()->getRunningScene();
		if (!_currScene) return;
		_currScene->removeChildByTag(1942);
	}
	else if (tag == 4) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		NativeHelper::openStore(dataManager.getAppConfig().url_android_store.c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (dataManager.getAppConfig().url_ios_nostore != ""){
            IOS::openUrl(dataManager.getAppConfig().url_ios_nostore);
        }else{
            IOS::gotoStore(dataManager.getAppConfig().url_ios_store);
        }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
		return;
#endif

		Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}
	else{
		//Director::getInstance()->end();
		//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
		return;
#endif

		Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}

}

void SceneLogin::OnSmartFoxLoginError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	SceneManager::getSingleton().hideLoading();
	if (currLoginType == registType)
		return;
	//log("LayerLogin::OnSmartFoxLoginError() - Login error");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueErrorMessage = (*ptrEventParams)["errorMessage"];
	boost::shared_ptr<string> ptrErrorMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueErrorMessage);
	boost::shared_ptr<string> message(new string("OnSmartFoxLoginError Failure: " + *ptrErrorMessage + " !"));
	//log("Error string: %s", message->c_str());
	//
	boost::shared_ptr<void> ptrEventParamValueErrorCode = (*ptrEventParams)["errorCode"];
	boost::shared_ptr<short> ptrErrorCode = ((boost::static_pointer_cast<short>))(ptrEventParamValueErrorCode);
	//
	//log("OnSmartFoxRoomJoinError - errorCode: %d", *ptrErrorCode);

	Chat *toast = Chat::create(mUtils::getErrorSmartfoxExplain(*ptrErrorCode), -1);
	this->addChild(toast);
}

void SceneLogin::doLogin(const int& type /*= 0*/)
{
	showMaintain(0);
	showUpdateInfo(0);
	SceneManager::getSingleton().setCallBackLoading(this, callfunc_selector(SceneLogin::timeoutLoadingCallback));
	SceneManager::getSingleton().showLoading();
	boost::shared_ptr<ISFSObject> parameters(new SFSObject());
	int  osid = 2; //ios
	std::string devID = "";
	/*bool isConnectting = true;*/
	getPartnerInfo();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	osid = 1;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
    else
        devID = NativeHelper::getDeviceId();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	osid = 2;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        devID = dataManager.getAppConfig().tientomocp2 + IOS::getDeviceID();
    else
        devID = IOS::getDeviceID();
	
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//devID = "123ddzzz-aa";// Only for test
	//update 29/11/2016 for check device id
	devID = getDeviceID();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	parameters->PutUtfString("lav", dataManager.getAppConfig().version_android);// LOGIN_APP_VERSION = "lav";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	parameters->PutUtfString("lav", dataManager.getAppConfig().version_ios);// LOGIN_APP_VERSION = "lav";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//parameters->PutUtfString("lav", "ios1452832982");// Only for test
	parameters->PutUtfString("lav", dataManager.getAppConfig().version_win);
#endif

	if (type == loginType)
	{
		if (txtUserName == NULL || txtPassword == NULL)
			return;
		UserDefault::getInstance()->setIntegerForKey("logintype", 1);
		UserDefault::getInstance()->setStringForKey("LT", "normal");
		UserDefault::getInstance()->setStringForKey("google_id", "");
		UserDefault::getInstance()->setStringForKey("facebook_id", "");
		UserDefault::getInstance()->flush();

		parameters->PutUtfString("LT", "normal");
		parameters->PutUtfString("LSK", "");
		parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
		parameters->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
		parameters->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
		parameters->PutInt("osid", osid);// Operatorsystems
		parameters->PutUtfString("devid", devID);// decvice id
		parameters->PutUtfString("aIp", "");//ip
		parameters->PutInt("srcref", 0);
		boost::shared_ptr<string> _pass = PasswordUtil::MD5Password(txtPassword->getString());
		_pass = PasswordUtil::MD5Password(*_pass);
		string _username = txtUserName->getString();
		boost::algorithm::to_lower(_username);
		boost::shared_ptr<IRequest> request(new LoginRequest(_username, *_pass, "GameZone1", parameters));
		GameServer::getSingleton().SendLogin(request);
	}
	else if (type == facebookType)
	{
		//log("Login With Type = 1");

//		if (strcmp(socialInfo.uid.c_str(), "") == 0)
//			return;
//		/*socialInfo.uid = scene.getUID();
//		 socialInfo.name = scene.getName();
//		 socialInfo.email = scene.getEmail();*/
//		UserDefault::getInstance()->setIntegerForKey("logintype", 2);
//		UserDefault::getInstance()->setStringForKey("LT", "facebook");
//		UserDefault::getInstance()->setStringForKey("google_id", "");
//		UserDefault::getInstance()->setStringForKey("facebook_id", socialInfo.uid);
//		UserDefault::getInstance()->flush();
//
//		string email_ = socialInfo.email.compare("") == 0 ? "" : socialInfo.email;
//		parameters->PutUtfString("LT", "facebook");
//		parameters->PutUtfString("aI", socialInfo.uid);
//		parameters->PutUtfString("aN", "");
//		parameters->PutUtfString("imc", "mobile");
//		parameters->PutUtfString("aM", email_);
//		parameters->PutUtfString("remail", email_);
//		parameters->PutUtfString("LSK", "");
//		parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
//		parameters->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
//		parameters->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
//		parameters->PutInt("osid", osid);// Operatorsystems
//		parameters->PutUtfString("devid", devID);// decvice id
//		parameters->PutUtfString("aIp", "");//ip
//		parameters->PutInt("srcref", 0);
//		CCLOG("UID: %s -- Name: %s", socialInfo.uid.c_str(), socialInfo.name.c_str());
//		boost::shared_ptr<IRequest> req_login(new LoginRequest(socialInfo.uid, "", "GameZone1", parameters));
//		GameServer::getSingleton().SendLogin(req_login);
	}
	else if (type == gmailType)
	{
//		//log("Login With Type = 1");
//
//		/*socialInfo.uid = scene.getUID();
//		 socialInfo.name = scene.getName();
//		 socialInfo.email = scene.getEmail();*/
//		if (strcmp(socialInfo.uid.c_str(), "") == 0)
//			return;
//
//		UserDefault::getInstance()->setIntegerForKey("logintype", 3);
//		UserDefault::getInstance()->setStringForKey("LT", "google");
//		UserDefault::getInstance()->setStringForKey("google_id", socialInfo.uid);
//		UserDefault::getInstance()->setStringForKey("facebook_id", "");
//		UserDefault::getInstance()->flush();
//
//		string email_ = socialInfo.email.compare("") == 0 ? "" : socialInfo.email;
//		parameters->PutUtfString("LT", "google");
//		parameters->PutUtfString("aI", socialInfo.uid);
//		parameters->PutUtfString("aN", "");
//		parameters->PutUtfString("imc", "mobile");
//		parameters->PutUtfString("aM", email_);
//		parameters->PutUtfString("remail", email_);
//		parameters->PutUtfString("LSK", "");
//		parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
//		parameters->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
//		parameters->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
//		parameters->PutInt("osid", osid);// Operatorsystems
//		parameters->PutUtfString("devid", devID);// decvice id
//		parameters->PutUtfString("aIp", "");//ip
//		parameters->PutInt("srcref", 0);
//		CCLOG("UID: %s -- Name: %s", socialInfo.uid.c_str(), socialInfo.name.c_str());
//		boost::shared_ptr<IRequest> req_login(new LoginRequest(socialInfo.uid, "", "GameZone1", parameters));
//		GameServer::getSingleton().SendLogin(req_login);
	}
	else if (type == quickType)
	{
		//log("Login With Type = quickType");

		UserDefault::getInstance()->setIntegerForKey("logintype", 4);
		UserDefault::getInstance()->setStringForKey("LT", "sp");
		UserDefault::getInstance()->setStringForKey("google_id", "");
		UserDefault::getInstance()->setStringForKey("facebook_id", "");
		UserDefault::getInstance()->flush();

		string email_ = "";
		parameters->PutUtfString("LT", "sp");
		parameters->PutUtfString("aI", "");
		parameters->PutUtfString("aN", "");
		parameters->PutUtfString("imc", "mobile");
		parameters->PutUtfString("aM", "");
		parameters->PutUtfString("LSK", "");
		parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
		parameters->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
		parameters->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
		parameters->PutInt("osid", osid);// Operatorsystems
		parameters->PutUtfString("devid", devID);// decvice id
		parameters->PutUtfString("aIp", "");//ip
		parameters->PutInt("srcref", 0);
		string _ai = mUtils::replaceAll(devID, "-", "");

		boost::shared_ptr<IRequest> req_login(new LoginRequest(_ai, "", "GameZone1", parameters));
		GameServer::getSingleton().SendLogin(req_login);
	}
	else if (type == registType){
		boost::shared_ptr<IRequest> request(new LoginRequest("", "123456", "RegisterZone"));
		GameServer::getSingleton().SendLogin(request);
	}
}

void SceneLogin::readInfo()
{
	UserDefault *def = UserDefault::getInstance();

	string username = def->getStringForKey("username", "");
	string password = def->getStringForKey("password", "");

	//log("username = %s", username.c_str());
	//log("password = %s", password.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	if (this->txtUserName == NULL) //log("txtUserName === NULL");
	this->txtUserName->setString(username);
	this->txtPassword->setString(password);
#else
	this->txtUserName->setString(username);
	this->txtPassword->setString(password);
#endif

}

void SceneLogin::saveInfo()
{
//	if (txtUserName == NULL || txtPassword == NULL)
//		return;
//	UserDefault *def = UserDefault::getInstance();
//	string _oldFacebookID = def->getStringForKey("facebook_id", "");
//	string _oldGmailID = def->getStringForKey("google_id", "");
//	string _oldLT = def->getStringForKey("LT", "");
//
//	string _oldUsername = def->getStringForKey("username", "");
//	string _oldUserpass = def->getStringForKey("password", "");
//
//	boost::shared_ptr<string> _pass = PasswordUtil::MD5Password(_oldUserpass);
//	_oldUserpass = *PasswordUtil::MD5Password(*_pass);
//
//	if (_oldLT.compare("normal") != 0){
//		_oldUsername = "";
//		_oldUserpass = "";
//	}
//	else{
//		_oldFacebookID = "";
//		_oldGmailID = "";
//	}
//
//	string _oldUserRechargeHis = def->getStringForKey("litr", "");
//	string _oldAmf = def->getStringForKey("amf", "");
//	string _oldAm = def->getStringForKey("am", "");
//	string _oldaN = def->getStringForKey("aN", "");
//	string _oldaM = def->getStringForKey("aM", "");
//	string _oldaMo = def->getStringForKey("aMo", "");
//	string _oldLvl = def->getStringForKey("lvl", "");
//	string _oldExp = def->getStringForKey("exp", "");
//	string _oldAccVipType = def->getStringForKey("accVipType", "");
//	string _oldDeviceID = "";
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	_oldDeviceID = NativeHelper::getDeviceId();
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	_oldDeviceID = IOS::getDeviceID();
//#endif
	//insert 01/08/2016
//	if ((_oldLT.compare("normal") == 0 && _oldUsername.compare(txtUserName->getString()) == 0)
//		|| (_oldLT.compare("facebook") == 0 && socialInfo.uid.compare(_oldFacebookID) == 0)
//		|| (_oldLT.compare("google") == 0 && socialInfo.uid.compare(_oldGmailID) == 0)
//		){
//		//nothing change
//	}
//	else if (_oldUsername.length() > 0){
//		string _lstUserInfo = def->getStringForKey("lstUserInfo", "");
//		//save to list
//		rapidjson::Document d;
//		d.Parse<0>(_lstUserInfo.c_str());
//		bool isExisted = false;
//		if (d.IsArray()){
//			rapidjson::SizeType size = d.Size();
//			for (rapidjson::SizeType i = 0; i < size; ++i){
//				if ((_oldLT.compare("normal") == 0 && _oldUsername.compare(d[i]["username"].GetString()) == 0)
//					|| (_oldLT.compare("facebook") == 0 && socialInfo.uid.compare(d[i]["facebook"].GetString()) == 0)
//					|| (_oldLT.compare("google") == 0 && socialInfo.uid.compare(d[i]["google"].GetString()) == 0)
//					){
//					isExisted = true;
//					d[i].RemoveMember("password");
//					d[i].AddMember("password", rapidjson::Value(_oldUserpass.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("LT");
//					d[i].AddMember("LT", rapidjson::Value(_oldLT.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("facebook");
//					d[i].AddMember("facebook", rapidjson::Value(_oldFacebookID.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("google");
//					d[i].AddMember("google", rapidjson::Value(_oldGmailID.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("amf");
//					d[i].AddMember("amf", rapidjson::Value(_oldAmf.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("am");
//					d[i].AddMember("am", rapidjson::Value(_oldAm.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("aN");
//					d[i].AddMember("aN", rapidjson::Value(_oldaN.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("aM");
//					d[i].AddMember("aM", rapidjson::Value(_oldaM.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("aMo");
//					d[i].AddMember("aMo", rapidjson::Value(_oldaMo.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("lvl");
//					d[i].AddMember("lvl", rapidjson::Value(_oldLvl.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("exp");
//					d[i].AddMember("exp", rapidjson::Value(_oldExp.c_str(), d.GetAllocator()), d.GetAllocator());
//					d[i].RemoveMember("accVipType");
//					d[i].AddMember("accVipType", rapidjson::Value(_oldAccVipType.c_str(), d.GetAllocator()), d.GetAllocator());
//					if (_oldUserRechargeHis.length() > 0){
//						d[i].RemoveMember("litr");
//						d[i].AddMember("litr", rapidjson::Value(_oldUserRechargeHis.c_str(), d.GetAllocator()), d.GetAllocator());
//					}
//					d[i].RemoveMember("deviceID");
//					d[i].AddMember("deviceID", rapidjson::Value(_oldDeviceID.c_str(), d.GetAllocator()), d.GetAllocator());
//					break;
//				}
//			}
//		}
//		else{
//			d.SetArray();
//		}
//		if (!isExisted){ //Not exist in list user history
//			//insert to his logs
//			rapidjson::Value _user(rapidjson::kObjectType);
//			{
//				_user.AddMember("username", rapidjson::Value(_oldUsername.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("LT", rapidjson::Value(_oldLT.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("google", rapidjson::Value(_oldGmailID.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("facebook", rapidjson::Value(_oldFacebookID.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("password", rapidjson::Value(_oldUserpass.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("litr", rapidjson::Value(_oldUserRechargeHis.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("amf", rapidjson::Value(_oldAmf.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("am", rapidjson::Value(_oldAm.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("aN", rapidjson::Value(_oldaN.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("lvl", rapidjson::Value(_oldLvl.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("accVipType", rapidjson::Value(_oldAccVipType.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("deviceID", rapidjson::Value(_oldDeviceID.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("exp", rapidjson::Value(_oldExp.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("aM", rapidjson::Value(_oldaM.c_str(), d.GetAllocator()), d.GetAllocator());
//				_user.AddMember("aMo", rapidjson::Value(_oldaMo.c_str(), d.GetAllocator()), d.GetAllocator());
//			}
//			d.PushBack(_user, d.GetAllocator());
//		}
//		rapidjson::StringBuffer strbuf;
//		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
//		d.Accept(writer);
//		def->setStringForKey("lstUserInfo", strbuf.GetString());
//	}
//	//
//	def->setStringForKey("username", txtUserName->getString());
//	def->setStringForKey("password", txtPassword->getString());
//	def->setStringForKey("litr", "");
//	def->setStringForKey("amf", "");
//	def->setStringForKey("am", "");
//	def->setStringForKey("aN", "");
//	def->setStringForKey("aM", "");
//	def->setStringForKey("aMo", "");
//	def->setStringForKey("lvl", "");
//	def->setStringForKey("exp", "");
//	def->setStringForKey("accVipType", "");
//	def->flush();
}

void SceneLogin::timeoutLoadingCallback()
{
	int tag = SceneManager::getSingleton().getLoadingTag();
	if (tag == -1)
		return;

	switch (tag){
	case 123://login
		if (GameServer::getSingleton().getSmartFox()->IsConnected())
			break;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
		//		if (sdkbox::PluginFacebook::isLoggedIn())
		//		{
		//			sdkbox::PluginFacebook::logout();
		//			//_loginItem->setString("Login");
		//			//log("logout");
		//		}
#endif
		Chat *toast = Chat::create("Hiện đang mất kết nối vui lòng kiểm tra lại đường truyền của bạn", -1);
		this->addChild(toast);
		break;
	}

	SceneManager::getSingleton().setLoadingTag(-1);
}

void SceneLogin::backToLogin(Ref* pSender)
{
	SceneManager::getSingleton().showLoading();
	boost::shared_ptr<IRequest> request(new LogoutRequest());
	GameServer::getSingleton().Send(request);
	//
	gotoLogin(0);
}

void SceneLogin::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (strcmp("rg", cmd->c_str()) == 0){
		int rc = *param->GetInt("rc");
		if (rc == 0){//OK
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
			//LoginLinux* layerLogin = SceneManager::getSingleton().getLayerLogin();
#else
			gotoLogin(0);
#endif

			txtUserName->setString(txtRegUserName->getString());
			txtPassword->setString(txtRegPassword->getString());
			//
			Chat *toast = Chat::create(dataManager.GetSysString(9), -1);
			this->addChild(toast);
			onButtonLogin(NULL);
		}
		else{//Not OK
			Chat *toast = Chat::create(*param->GetUtfString("rd"), -1);
			this->addChild(toast);
		}
		SceneManager::getSingleton().hideLoading();
	}
}

void SceneLogin::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		if (this->getChildByTag(POPUP_THONGBAO) == NULL){
			showPopupThongbao();
		}
	}
}

void SceneLogin::showPopupThongbao(){
	auto layerpoup = LayerPopupThongBao::create();
	layerpoup->setCallBackFromButton(this, callfuncO_selector(SceneLogin::onPopupCallBack), 1);
	this->addChild(layerpoup);
}

void SceneLogin::onPopupCallBack(Ref *pSender)
{
	Director::getInstance()->end();
}

void SceneLogin::scheduleUpdateEditbox(float dt)
{
	readInfo();
}

void SceneLogin::scheduleUpdateContactInfo(float dt)
{
	if (dataManager.getAppConfig().callcenter.length() == 0)
		return;
	layerloading->stop();
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneLogin::scheduleUpdateContactInfo),this);
	std::string strHotLine = dataManager.getAppConfig().callcenter + "";
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		lblHotLine->setString(strHotLine);
	}
	else{
		btnHotLine->setTitleText("            "+strHotLine);
	}
}
void SceneLogin::hideToUp(float dt){
	if (strcmp(SceneManager::getSingleton().versiontohide.c_str(), "i114") == 0){
		//log("GetVersion IOS - datas receive from http: hidden button");
		if (btnLoginFace)
			btnLoginFace->setVisible(false);
		if (btnLoginGPlus)
			btnLoginGPlus->setVisible(false);
		if (labelDNB)
			labelDNB->setVisible(false);
		if (logo)
			logo->setVisible(false);
		if (btnHotLine)
			btnHotLine->setVisible(false);
		if (iconPhone)
			iconPhone->setVisible(false);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			if (lblHotLine)
				lblHotLine->setVisible(false);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			//webView->setVisible(false);
#endif
		}
		return;
	}
	else {
		if (btnLoginFace)
			btnLoginFace->setVisible(true);
		if (btnLoginGPlus)
			btnLoginGPlus->setVisible(true);
		if (labelDNB)
			labelDNB->setVisible(true);
		if (logo)
			logo->setVisible(true);
		if (btnHotLine)
			btnHotLine->setVisible(true);
		if (iconPhone)
			iconPhone->setVisible(true);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			if (lblHotLine)
				lblHotLine->setVisible(true);
		}

		return;
	}

}

void SceneLogin::getPartnerInfo()
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

void SceneLogin::onButtonQuickPlay(Ref* pSender)
{

	showMaintain(0);
	showUpdateInfo(0);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	IOS::tryIsInternetConnection();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	NativeHelper::hasConnection();
#endif
	if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
		Chat *toast = Chat::create(dataManager.GetSysString(211), -1);
		this->addChild(toast);
		return;
	}
	currLoginType = quickType;
	if (!GameServer::getSingleton().getSmartFox()->IsConnected())
	{
		SceneManager::getSingleton().connect2Server(0);
	}
	else
	{
		doLogin(currLoginType);
	}
}
void SceneLogin::setBonusFacebook(float dt){
	int moneyBonus = SceneManager::getSingleton().getLoginBonus() + SceneManager::getSingleton().getFacebookBonus();
	txtMoney->setString(StringUtils::format("+%d", moneyBonus));

}

void SceneLogin::createLayerLoading()
{
	layerloading = LayerLoading::create();
	layerloading->setPosition(0, 0);
	layerloading->setAnchorPoint(Vec2::ZERO);
	this->addChild(layerloading, ZORDER_LIST::ZORDER_TOAST);
	layerloading->start(5);
}

void SceneLogin::setShouldAuto(bool isShould)
{
	isShouldAuto = isShould;
}

void SceneLogin::onLoadingFinish()
{
	//log("Visible layer loading");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		//webView->setVisible(true);
	}
#endif
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool SceneLogin::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		SceneManager::getSingleton().showLoading();
		sender->setVisible(false);
	}
	return true;

}

void SceneLogin::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		SceneManager::getSingleton().hideLoading();
		//if (currTab == 3 || currTab == 4 || currTab == 5)
		//webView->setVisible(true);
	}
}

void SceneLogin::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		SceneManager::getSingleton().hideLoading();
	}
}
#endif

std::string SceneLogin::getDeviceID()
{
	std::string device_id = "123ddzzz-aa";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	HRESULT hres;

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		cout << "Failed to initialize COM library. Error code = 0x"
			<< hex << hres << endl;
		return device_id;                  // Program has failed.
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);


	if (FAILED(hres))
	{
		cout << "Failed to initialize security. Error code = 0x"
			<< hex << hres << endl;
		CoUninitialize();
		return device_id;                    // Program has failed.
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		cout << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<< hex << hres << endl;
		CoUninitialize();
		return device_id;                 // Program has failed.
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
		);

	if (FAILED(hres))
	{
		cout << "Could not connect. Error code = 0x"
			<< hex << hres << endl;
		pLoc->Release();
		CoUninitialize();
		return device_id;                // Program has failed.
	}

	cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;


	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
		cout << "Could not set proxy blanket. Error code = 0x"
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return device_id;               // Program has failed.
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_BIOS"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		cout << "Query for operating system name failed."
			<< " Error code = 0x"
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return device_id;               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		// Get the value of the Name property
		hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
		wcout << " OS Name : " << vtProp.bstrVal << endl;
		device_id = (const char*)_bstr_t(vtProp.bstrVal);
		VariantClear(&vtProp);

		pclsObj->Release();
	}

	// Cleanup
	// ========
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();
#endif
	return device_id;
}
