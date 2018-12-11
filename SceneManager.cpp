#include "SceneManager.h"

#include "Entities/Variables/SFSBuddyVariable.h"
#include "Entities/Variables/UserVariable.h"
#include "Entities/Variables/SFSUserVariable.h"

#include "Requests/SetUserVariablesRequest.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/Buddylist/SetBuddyVariablesRequest.h"
#include "Requests/Buddylist/GoOnlineRequest.h"
#include "Requests/SubscribeRoomGroupRequest.h"
#include "Requests/UnsubscribeRoomGroupRequest.h"
#include "Requests/JoinRoomRequest.h"
#include "Requests/LeaveRoomRequest.h"

#include "mUtils.h"

#include "scenes/SceneLogin.h"
#include "scenes/SceneMain.h"
#include "scenes/ScenePickRoom.h"
#include "scenes/SceneGame.h"

#include "layergames/_Chat_.h"
#include "layers/LayerPopupThongBao.h"
#include "layers/LayerPopupChangeName.h"
#include "layers/LayerShowChuChay.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#include "layergames/PhomMessDef.h"
#include "layergames/ClientMsgDefs.h"
#include "layers/LayerPopupReconnect.h"
#include "layercaro/FakeScene.h"
#include "cotuong/LayerToast.h"
#include "cotuong/LayerPickRoom.h"
#include "layers/LayerPopupRecharge.h"
#include "cotuong/LayerPopupAlert.h"
#include "layergames/LayerMenuMiniGame.h"
#include "layergames/ChanUtils.h"
#include "cotuong/ServerMessageDef.h"
#include "cotuong/gameUtils.h"
#include "layers/LayerPopupCuaHang.h"
#include "layers/LayerPopupEventThoiVu.h"
#include "layers/LayerPopupNapTienAlert.h"
#include "layergames/LayerPlayVQMM.h"
#include "layers/LayerPopupDoiMoc.h"
#include "layers/LayerLoadingStore.h"
#define URL_NOTIFICATION_URL "https://gamebai.net/notifyServices"
#define IP_DEFAULT "http://config-01.dynu.net:7788/api?cmd=getconfig"
#define IP_DEFAULT1 "http://config-02.dynu.net:7788/api?cmd=getconfig"

//#define IP_P2_DEFAULT "http://pub1.publicvm.com:7788/api?cmd=getconfig"
//#define IP_P2_DEFAULT1 "http://pub2.publicvm.com:7788/api?cmd=getconfig"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
	//org/cocos2dx/cpp/AppActivity
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_InstallReferrerReceiver_setCampaign(JNIEnv*  env, jobject thiz, jstring compaign) {
		std::string sCompaign = JniHelper::jstring2string(compaign);
		//log("Receive compaign in cocos2d-x %s", sCompaign.c_str());
	}
}
#endif

template<> SceneManager* SingLeton<SceneManager>::mSingleton = 0;

SceneManager* SceneManager::getSingletonPtr(void)
{
	return mSingleton;
}

SceneManager& SceneManager::getSingleton(void)
{
	assert(mSingleton);
	return (*mSingleton);
}
void SceneManager::loadString()
{
	std::string strings = STRING_PATH+getFolderLang()+"strings.conf";
	if (!dataManager.LoadStrings(strings.c_str()))
	{
		//log("Can't load: %s", strings.c_str());
		return;
	}
}
SceneManager::SceneManager(){

}
SceneManager::~SceneManager(){

}
bool SceneManager::init()
{
	if (!Layer::init())
		return false;


	//        vector<string> searchPaths;
	//        searchPaths.push_back("avatars");
	//        CCFileUtils::getInstance()->setSearchPaths(searchPaths);
    
    //Nếu tất cả là false => thì sẽ là naga.
    //anhnt change version
    this->isNagaCard = false;
    this->isNagaWin = false;
    this->isNagaNew = false;
    this->isMegaWin = false;
    this->huPoker = 672384;
    this->huTaiXiu = 767123;
    //anhnt download store set true, else set false
    this->setIsFlagStore(false);
    UserDefault *def = UserDefault::getInstance();
    def->setBoolForKey("sound", false);
//    switch(CCApplication::sharedApplication()->getCurrentLanguage())
//    {
//        case kLanguageChinese:
//            currLang = def->getIntegerForKey("lang", 4);
//            break;
//        case kLanguageEnglish:
//            currLang = def->getIntegerForKey("lang", 1);
//            break;
//        default:
//            currLang = def->getIntegerForKey("lang", 2);
//            break;
//    };
//    //anht only for test english
    //currLang =1;
    //def->setIntegerForKey("lang", 1);
    //anhnt china
    currLang = 2;
    
    vector<string> searchPaths;
    isShouldAuto = true;
    dataManager.getAppConfig().list_game = "112_true;128_true;137_true;139_true;140_true;114_true;250_true;138_dis";//

    if (this->isNagaWin){
        dataManager.getAppConfig().list_game = "112_true;128_true;137_true;139_true;140_true;114_true;250_true;133_true";//

    }
    //lang_code = 0: vie, 1: en, 2: cam
    string langfolder = "/VN";
    switch (currLang){
        case 0:
            langfolder = "/VN";
            break;
        case 1:
            langfolder = "/ENG";
            break;
        case 2:
            langfolder = "/CAM";
            break;
        case 4:
            langfolder = "/CN";
            break;
    }
    searchPaths.push_back("avatars"+langfolder);
    searchPaths.push_back("cards");
    searchPaths.push_back("ccbi");
    searchPaths.push_back("design");
    searchPaths.push_back("sounds");
    searchPaths.push_back("sounds/game");
    searchPaths.push_back("sounds/game_phom");
    searchPaths.push_back("sounds/game_tienlen");
    searchPaths.push_back("design/background");
    searchPaths.push_back("design/guifiles");
    searchPaths.push_back("design/frame"+langfolder);
    searchPaths.push_back("design/LayerTaiXiuWeb");
    searchPaths.push_back("design/backgrounds");
    searchPaths.push_back("design/avatarGame");
    searchPaths.push_back("design/fonts");
    searchPaths.push_back("fonts");
    searchPaths.push_back("ResChan"+langfolder);
    searchPaths.push_back("ResXiTo"+langfolder);
    searchPaths.push_back("backgrounds");
    searchPaths.push_back("buttons"+langfolder);
    searchPaths.push_back("cards");
    searchPaths.push_back("icon_result");
    searchPaths.push_back("numbers");
    searchPaths.push_back("ResQuaythuong");
    searchPaths.push_back("gameicons");
    searchPaths.push_back("emotions");
    searchPaths.push_back("ResXocDia");
    searchPaths.push_back("design/LayerTaiXiuMini");
    searchPaths.push_back("design/LayerMiniPoker");
    searchPaths.push_back("design/LayerTomCuaCa");
    searchPaths.push_back("design/LayerLieng"+langfolder);
    searchPaths.push_back("design/LayerGameCaoThap");
    searchPaths.push_back("design/LayerDoDenMini");
    searchPaths.push_back("gamecaro");
    searchPaths.push_back("MauBinh");
    searchPaths.push_back("CoTuong");
    searchPaths.push_back("CoTuong/res");
    searchPaths.push_back("CoTuong/chess_ui");
    searchPaths.push_back("design/LayerCoTuong/sanhgame");
    searchPaths.push_back("design/LayerCoTuong");
    searchPaths.push_back("localedata");
    searchPaths.push_back("design/LayerMenuMiniGame");
    searchPaths.push_back("res_extend"+langfolder);
    searchPaths.push_back("design/ResBautom");
    searchPaths.push_back("Music");
    
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    
//    auto audio = SimpleAudioEngine::getInstance();
//    audio->preloadBackgroundMusic("Music/bm-theme_v3.mp3");
    
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gamechan.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("card_small.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gold_coin.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("silver_coin.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("card_big.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animxocdia.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("chan_new.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("chess.plist");
	mCurrIpIndex = 0;
	loadString();
	setKickRoom(false);
	setKickNem(false);
	setBetValue(0);
	setListBet("");

	this->isLogin = false;

	setTypePickRoom(TableType::PlayTable);
	//set game version by add value config at appdelegate.cpp
	int _gamever = Configuration::getInstance()->getValue("gameversion").asInt();
	setGameVersion(_gamever);// game bai p2

    
//    if (getGameVersion() == GameVersion::p2){
//#undef IP_DEFAULT
//#undef IP_DEFAULT1
//#define IP_DEFAULT IP_P2_DEFAULT
//#define IP_DEFAULT1 IP_P2_DEFAULT1
//    }

	//
	GameServer* gameServer = new GameServer();
	gameServer->initServer();

	GameServer::getSingleton().addListeners(this);
	//check for filter ip
	//	if (_gamever == GameVersion::p1)
	//	{
	//#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//		this->initFirst();
	//#endif
	//	}
	//	else{
   // return true;

	this->initFirst();
	//	}


	return true;
}

void SceneManager::initFirst()
{
	//Download file IP
	gImgDownloadProcess = new ImageDownloader();

	mCurrStepCheckConn = tagGetLevel1Info;
	//MINHDV -init default config bet
	strGameConfig = "100-1,100,500,2000,5000,10000,20000,50000,100000,200000,500000,1000000-0-20|101-1,100,500,2000,5000,10000,20000,50000,100000,200000,500000,1000000-0-20|102-1,100,500,2000,5000,10000,20000,50000,100000,200000,500000,1000000-0-20|103-1,100,500,2000,5000,10000,20000,50000,100000,200000,500000,1000000-0-20|109-1,100,500,2000,5000,10000,20000,50000,100000,200000,500000,1000000-0-36|111-1,100,500,2000,5000,10000,20000,50000,100000,200000,500000,1000000-0-30|112-1,100,500,2000,5000,10000,20000,50000,100000,200000,500000,1000000-0-30|116-1,100,500,2000,5000,10000,20000,50000,100000,200000,500000,1000000-0-30|117-1,100,500,2000,5000,10000,20000,50000,100000,200000,500000,1000000-0-30";
	arrConfig = getConfigFromSever(strGameConfig);

    //return;
	//MINHDV - fix schedule
	Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::updateEvent), this, 0, false);
	scheduleUpdateWithPriority(1);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::onScheduleGetConfig), this, 1, false);
	//Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::onScheduleGetConfig), 1);

   
	/************************************************************************/
	/* load config                                                          */
	/************************************************************************/
    UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
    UserDefault::getInstance()->setIntegerForKey("numEvent", 0);
	bool isAuto = UserDefault::getInstance()->getBoolForKey("isAutoLogin");
	if (!isAuto){
		HttpRequest* request = new (std::nothrow) HttpRequest();
		request->setUrl(dataManager.getAppConfig().url_list_ip.c_str());
		request->setRequestType(HttpRequest::Type::GET);
		request->setTag("iplevel1");
		request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
		HttpClient::getInstance()->send(request);
		request->release();
		this->setLoadingString(dataManager.GetSysString(211));
		//this->setTimeOutLoading(20.f);
		this->showLoading(5);

	}
    int random = rand() %331230 + 111133;
    this->currHuTaiXiu = 2000000;
    this->currHuPoker= 2000000;
    this->currHuCaoThap = 2000000;

    this->huTaiXiu = 423471+random;
    int random2 = rand() %354230 + 124533;
    this->huPoker = 421471+random2;
    int random3 = rand() %252230 + 124533;
    this->huCaoThap = 321471+random3;
    this->onScheduleGetHuTHuong(0);
    
    
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(SceneManager::onScheduleGetHuTHuong,this,1));
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(SceneManager::onScheduleFakeHuThuongTaiXiu,this,1));
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(SceneManager::onScheduleFakeHuThuongPoker,this,1));
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(SceneManager::onScheduleFakeHuThuongCaoThap,this,1));

    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::onScheduleGetHuTHuong), this, 60, false);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::onScheduleFakeHuThuongTaiXiu), this, 1/2, false);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::onScheduleFakeHuThuongPoker), this, 1/2, false);

    Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::onScheduleFakeHuThuongCaoThap), this, 1/2, false);

    // return;

	//getVersionFromUrl();
	getUrlAvatar();
	//getButtonNotificationFromUrl();
	//getInfoShareFromUrl();
	setIsShowTXDay(false);

	//update 4-1-2017
	this->setIsBotZone(false);
    //this->setIsFlagStore(true);
    //anhnt download resource
    if (this->getIsFlagStore()){
        auto layerLoadingStore = LayerLoadingStore::createScene();
        if (layerLoadingStore == NULL)
            return;
        Director::getInstance()->replaceScene(TransitionFade::create(0.0, layerLoadingStore));
    }
    else{
        auto scene = SceneMain::createScene();
        if (scene == NULL)
            return;
        Director::getInstance()->replaceScene(TransitionFade::create(0.0, scene));
    }
}
void SceneManager::flagStore(string listFlagStore){
    //anhnt download resource
    if (!this->getIsFlagStore())
    return;
    //only for test dataManager.getAppConfig().isVN
    auto lstVersion = mUtils::splitString(listFlagStore, ';');
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
            this->setIsFlagStore(false);
        }
    }
    
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    for (int i = 0; i < vsAndroid.size(); i++) {
        if (strcmp(vsAndroid[i].c_str(), dataManager.getAppConfig().version_android.c_str()) == 0){
            this->setIsFlagStore(false);
        }
    }
#endif
   // if (LayerLoadingStore::getSingleton())
    LayerLoadingStore::getSingleton().startDownloadResouces();
    
}
void SceneManager::onFileDownLoaded(Node* pSender, void* pResponse)
{
	this->hideLoading();
	string* str = (string*)pResponse;

	if (str->size() == 0 || str->compare("error") == 0)
	{
		showChatWithCurrentScene(dataManager.GetSysString(49), -1);
		return;
	}
	// 	strData = mUtils::replaceStrChar(strData, "\r", 0);
	// 	strData = mUtils::replaceStrChar(strData, " ", 0);
	vector<string> lstIp = mUtils::splitString(*str, '\n');
	//
	gListIpAddress.push_back(lstIp.at(0));
	//
	//this->scheduleOnce(schedule_selector(SceneManager::connect2Server), 2);
	//MINHDV - fix schedule
	Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::connect2Server), this, 2, false);

}
void SceneManager::getConfigStore(){
    
}
void SceneManager::gotoLogin(){

    
    this->isLogin = false;

  
    
	//add request to check ip only for android
    SceneManager::getSingleton().hideLoading();

    auto _currScene = Director::getInstance()->getRunningScene();
    if (_currScene){
    _currScene->removeChildByTag(128);
    _currScene->removeChildByTag(130);
    _currScene->removeChildByTag(133);
    _currScene->removeChildByTag(134);
    _currScene->removeChildByTag(135);
    }
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneManager::updateEvent4CheckConnection), this);
    SceneMain::getSingleton().logOut();
}
void SceneManager::gotoMain(){
    if (SceneManager::getSingleton().getIsFlagStore()) return;
	// 	CCProgressLayer* _LayerLoading = CCProgressLayer::create();
	// 	_LayerLoading->setTag(-1);
	// 	_LayerLoading->hide();

//    auto scene = SceneMain::createScene();
//    if (scene == NULL)
//        return;
//    //MINHDV - fix schedule - run only for p1
//    Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::updateEvent4CheckConnection), this, 1, false);
//
//    Director::getInstance()->replaceScene(TransitionFade::create(0.7, scene));
    //only for china
    //SceneMain::getSingleton().gotoGameByID(kGamePoker);
    SceneMain::getSingleton().goToMain();

	//scene->addChild(_LayerLoading, 1100, tag_SceneLoading);
}
void SceneManager::gotoPickGame(int gameID){
    if (gameID == 0 || gameID == -1 || gameID == 118 || gameID == 200) return;
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL)/// cho disconnect luon
	{
		this->showChatWithCurrentScene(dataManager.GetSysString(602), -1);
		//this->scheduleOnce(schedule_selector(SceneManager::disconnectFromServerDelay), 1);
		//MINHDV - fix schedule
		Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::disconnectFromServerDelay), this, 1.0, false);
		return;
	}
	auto amf = myself->GetVariable("amf");
	auto amfs = myself->GetVariable("amfs");

	//log("SceneManager CurrentRoomType= %d", this->getCurrRoomType());
	boost::shared_ptr<UserVariable> amfptr;
	if (this->getCurrRoomType() == 0){
		amfptr = amf;
	}
	else{
		amfptr = amfs;

	}
	if (amfptr == NULL)
		return;
	boost::shared_ptr<double> amfvalue = amfptr->GetDoubleValue();
	if (amfvalue == NULL)
		return;
	auto curMoney = *amfvalue;
	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return;
	if (gameID == kGameTaiXiu){
//        if (curMoney < this->getMinMoney(kGameTaiXiu)){
//            this->hideLoading();
//            Chat* toast = Chat::create(StringUtils::format("%s s%",dataManager.GetSysString(476).c_str(), mUtils::format_money((long)this->getMinMoney(kGameTaiXiu))->c_str()), -1);
//            _currScene->addChild(toast, ZORDER_LIST::ZORDER_TOAST);
//            return;
//        }

	}
	if (gameID == kGameXocDia){
//        if ((*amf->GetDoubleValue()) < this->getMinMoney(kGameXocDia) && (*amfs->GetDoubleValue()) < this->getMinMoney(1114)){
//            Chat* toast = Chat::create(StringUtils::format("%s %s",dataManager.GetSysString(476).c_str(), mUtils::format_money((long)this->getMinMoney(kGameXocDia))->c_str()),-1);
//            _currScene->addChild(toast, 100000);
//            return;
//        }
	}


	setGameID(gameID);
	//update 2/8
	if (gameID == kGameTaiXiu || gameID == kGameQuayThuong || gameID == kGameXocDia || gameID == kGameBauTom || gameID == kGameSlot){
		//this->showLoading();
		boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(StringUtils::format("%d", gameID)));
		GameServer::getSingleton().Send(request);
        SceneMain::getSingleton().prepareToPickGame();

		return;
	}
	//
	// 	CCProgressLayer* _LayerLoading = CCProgressLayer::create();
	// 	_LayerLoading->setTag(-1);
	// 	_LayerLoading->hide();
	//MINHDV - get notification on scene pickroom
	getNotificationPickRoomFromUrl();
	if (gameID == kGameCoTuong || gameID == kGameCoUp)
	{
        
        SceneMain::getSingleton().goToPickRoom(gameID);

		//Director::getInstance()->replaceScene(TransitionFade::create(0, scene));

//        Size sizeAdd = ChanUtils::getSizePos();
//        LayerMenuMiniGame* minigame = LayerMenuMiniGame::create();
//        minigame->setPosition(Vec2(sizeAdd.width, sizeAdd.height));
//        layerPickRoom->addChild(minigame, ZORDER_LIST::ZORDER_TAIXIU_MINI);
	}
	else{
//        auto scene = ScenePickRoom::create();
//        if (scene == NULL)
//            return;
//        SceneMain::getSingleton().addChild(scene);
        //SceneMain::getSingleton().prepareToPickGame();
        SceneMain::getSingleton().goToPickRoom(gameID);
        //Director::getInstance()->replaceScene(TransitionFade::create(0, scene));

	}

}
void SceneManager::gotoGame(int typeGame){
//    //
//    //     CCProgressLayer* _LayerLoading = CCProgressLayer::create();
//    //     _LayerLoading->setTag(-1);
//    //     _LayerLoading->hide();
//
//    auto scene = SceneGame::createScene(typeGame);
//    if (scene == NULL)
//        return;
//    Director::getInstance()->replaceScene(TransitionFade::create(0, scene));
//
//    //scene->addChild(_LayerLoading, 1100, tag_SceneLoading);
    SceneMain::getSingleton().gotoGameByID(typeGame);
}

//SceneManager::SceneManager() : pKieuBanChan(1)
//{
//
//}

void SceneManager::onEnter()
{
	Layer::onEnter();
}

void SceneManager::onExit()
{
	Layer::onExit();
}

void SceneManager::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL)
		return;

	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	if (strcmp("hbc", ptrNotifiedCmd->c_str()) == 0)
	{
	}
	else if (strcmp("nem_ntf", ptrNotifiedCmd->c_str()) == 0)
	{
		showChatWithCurrentScene(dataManager.GetSysString(52), -1);
	}
	else if (strcmp("kkntf", ptrNotifiedCmd->c_str()) == 0)
	{
		////EXT_EVENT_USER_KICKED_NOTIF    = "kkntf";

		if (strcmp(myself->Name()->c_str(), param->GetUtfString("uid")->c_str()) == 0)
		{
			showChatWithCurrentScene(dataManager.GetSysString(53), -1);
		}
		else
		{
			showChatWithCurrentScene(StringUtils::format("%s %s",dataManager.GetSysString(54).c_str(), param->GetUtfString("uid")->c_str()), -1);
		}
	}
	else if (strcmp("kbsa", ptrNotifiedCmd->c_str()) == 0)//KICK_BY_SAME_ACC
	{
		showChatWithCurrentScene(dataManager.GetSysString(533), -1);
		GameServer::getSingleton().getSmartFox()->Disconnect();
		// 		LayerMain::getSingleton().gotoServices();
		// 		releaseCurrentLayerGame();
		gotoLogin();
	}
	else if (strcmp("liSMSResp", ptrNotifiedCmd->c_str()) == 0){// LIST_CARD_INFOR = "liCARDInfor";
		if (param->GetUtfString("liSMSCMD") != NULL){
			UserDefault *def = UserDefault::getInstance();
			string currDate = mUtils::getCurrentDateTime("%d/%m/%Y");
			def->setStringForKey("dateConfig", currDate);

			string lstSmss = *param->GetUtfString("liSMSCMD");//String LIST_SMS_CMD = "liSMSCMD";
			def->setStringForKey("liSMSCMD", lstSmss);

			def->flush();
		}
	}
	else if (strcmp("liCARDResp", ptrNotifiedCmd->c_str()) == 0){// LIST_CARD_INFOR = "liCARDInfor";
		if (param->GetUtfString("liCARDCMD") != NULL){
			UserDefault *def = UserDefault::getInstance();
			string currDate = mUtils::getCurrentDateTime("%d/%m/%Y");
			def->setStringForKey("dateConfig", currDate);

			string lstCards = *param->GetUtfString("liCARDCMD");//String LIST_CARD_CMD = "liCARDCMD";
			def->setStringForKey("liCARDCMD", lstCards);

			def->flush();
		}
	}
	else if (ptrNotifiedCmd->compare("notiRMFF") == 0)
	{
		auto _currScene = Director::getInstance()->getRunningScene();
		if (!_currScene) return;
		_currScene->removeChildByTag(TAG_LAYER_NAP_TIEN);
		LayerPopupThongBao* _layer = LayerPopupThongBao::create();
		_layer->setMessage(dataManager.GetSysString(141));
		_layer->setContentMess(*param->GetUtfString("notiMSG"));
		_layer->getBtnOk()->setVisible(false);
		_layer->getBtnHuy()->setTitleText("OK");
		_layer->getBtnHuy()->setPositionX(_layer->getLblTitle()->getPositionX());
		_currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_NOTIFICATION,9115);
	}
	else if (ptrNotifiedCmd->compare(EXT_EVENT_END_GAME_NOTIF) == 0 && this->getGameID() == kGameMauBinh){
        this->showCuoiVan = true;

        if (this->isEndGame){
            this->runEvent(1);
        }else{
            this->runEvent(2);
        }
		this->currentGameEnd++;
		if (this->currentGameEnd >= dataManager.getAppConfig().delay_rqoffer){
			this->sendEventEndGame();
			this->currentGameEnd = 0;
		}
	}
    else if (ptrNotifiedCmd->compare(EXT_EVENT_VICTORY_NOTIF) == 0){
        this->showCuoiVan = true;
        
        if (this->isEndGame){
            this->runEvent(1);
        }else{
            this->runEvent(2);
        }
    }
	else if (ptrNotifiedCmd->compare(EXT_EVENT_END) == 0){
        this->showCuoiVan = true;
        if (this->isEndGame){
            this->runEvent(1);
        }else{
            this->runEvent(2);
        }
		this->currentGameEnd++;
		if (this->currentGameEnd >= dataManager.getAppConfig().delay_rqoffer){
			this->sendEventEndGame();
			this->currentGameEnd = 0;
		}
	}
    if (0 == ptrNotifiedCmd->compare(EXT_EVENT_VQ_ROTATE_RESP)) {
        boost::shared_ptr<string> grs = param->GetUtfString(EXT_FIELD_GAME_VQ_RESULT);
        if (grs){
            int nohu = 0;
            auto lst = mUtils::splitString(*grs, ';');
            

//            if (atoi(lst[2].c_str())>0)
//                this->winVQMM = atoi(lst[2].c_str());
            
            if (lst.size()<6) return;
            string result = lst[1]+ ";"+lst[2]+ ";"+lst[3]+ ";"+lst[4];
            auto lst2 = mUtils::splitString(result, ';');
            if (lst2.size() < 4) return;
            if (atoi(lst2[1].c_str())>0)
                this->winVQMM = atoi(lst[1].c_str());
            if (atoi(lst[1].c_str()) == 0 && atoi(lst[2].c_str()) == 0 &&atoi(lst[3].c_str()) == 0 &&atoi(lst[4].c_str()) == 0){
                //this->typePrize =MAT_LUOT_VQMM ;
               // return;
            }
            for (int k = 6; k < lst.size() ; k++){
                int resultBet= 0;
                auto lstphanthuong = mUtils::splitString(lst[k], ',');
                /*#define MAT_LUOT_VQMM 5
                 #define NO_HU_VQMM 1
                 #define HAU_TO_EXP 2
                 #define HAU_TO_GOLD 0
                 #define HAU_TO_SILVER 1*/
                if (atoi(lstphanthuong[2].c_str()) > 0){
                    string str = lstphanthuong[2] + "0";
                    if (atoi(str.c_str()) == NO_HU_VQMM){
                        nohu++;
                    }
                }
                if (atoi(lstphanthuong[3].c_str()) > 0){
                    string str = lstphanthuong[3] + "1";
                    if (atoi(str.c_str()) == NO_HU_VQMM){
                        nohu++;
                    }
                }
                if (atoi(lstphanthuong[4].c_str()) > 0){
                    string str = lstphanthuong[4] + "2";
                    if (atoi(str.c_str()) == NO_HU_VQMM){
                        nohu++;
                    }
                }
            }
        
                        if (nohu>0){
                            if (this->winVQMM > dataManager.getAppConfig().notifyZen){
                                string typewin = "";
                                string typewin1 = "";
                                if (nohu == 1){
                                    typewin = dataManager.GetSysString(985);
                                    typewin1 = dataManager.GetSysString(9851);

                                }
                                if (nohu == 2){
                                    typewin = dataManager.GetSysString(986);
                                    typewin1 = dataManager.GetSysString(9861);

                                }
                                if (nohu == 3){
                                    typewin = dataManager.GetSysString(987);
                                    typewin1 = dataManager.GetSysString(9871);

                                }
                            string msg = "";
                            msg = StringUtils::format("%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(1235).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),typewin.c_str(),mUtils::convertMoneyEx(this->winVQMM).c_str(),this->getCoin(0).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(12351).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),typewin1.c_str(),mUtils::convertMoneyEx(this->winVQMM).c_str(),this->getCoin(0).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(12352).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),typewin.c_str(),mUtils::convertMoneyEx(this->winVQMM).c_str(),this->getCoin(0).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(12353).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),typewin.c_str(),mUtils::convertMoneyEx(this->winVQMM).c_str(),this->getCoin(0).c_str());
                            this->sendEventNotify(msg);
                            }
                        }
            
        }
        
    }
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_CAOTHAP_GAME_RESULT)) {
      
        if (param->GetInt("gwv")){
            this->winCT = *param->GetInt("gwv");
                if (this->currRoomTypeCT == 0){
                    if (this->winCT > dataManager.getAppConfig().notifyZen){
                
                        string msg = "";
                        msg = StringUtils::format("%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(1238).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(984).c_str(),mUtils::convertMoneyEx(this->winCT).c_str(),this->getCoin(this->currRoomTypeCT).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(12381).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9841).c_str(),mUtils::convertMoneyEx(this->winCT).c_str(),this->getCoin(this->currRoomTypeCT).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(12382).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9842).c_str(),mUtils::convertMoneyEx(this->winCT).c_str(),this->getCoin(this->currRoomTypeCT).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(12383).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9843).c_str(),mUtils::convertMoneyEx(this->winCT).c_str(),this->getCoin(this->currRoomTypeCT).c_str());
                        this->sendEventNotify(msg);
                        
                    }
                }else{
                    if (this->winCT > dataManager.getAppConfig().notifyGold){
               
                        string msg = "";
                                 msg = StringUtils::format("%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(1238).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(984).c_str(),mUtils::convertMoneyEx(this->winCT).c_str(),this->getCoin(this->currRoomTypeCT).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(12381).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9841).c_str(),mUtils::convertMoneyEx(this->winCT).c_str(),this->getCoin(this->currRoomTypeCT).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(12382).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9842).c_str(),mUtils::convertMoneyEx(this->winCT).c_str(),this->getCoin(this->currRoomTypeCT).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(12383).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9843).c_str(),mUtils::convertMoneyEx(this->winCT).c_str(),this->getCoin(this->currRoomTypeCT).c_str());
                        this->sendEventNotify(msg);
                    }
                }
        }
        
		this->currentGameEnd++;
		if (this->currentGameEnd >= dataManager.getAppConfig().delay_rqoffer){
			this->sendEventEndGame();
			this->currentGameEnd = 0;
		}
	}
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_GAME_RESULT_PKMINI)) {
        
        boost::shared_ptr<long> winValue = param->GetInt("gwv");
        boost::shared_ptr<string> rc = param->GetUtfString("gbt");

        if (winValue!= NULL)
            this->winPK = *winValue;
        if (rc != NULL){//        case POKER_THUNG_PHA_SANH_LON:
            if (atof((*rc).c_str()) == POKER_THUNG_PHA_SANH_LON || POKER_THUNG_PHA_SANH_BE == atof((*rc).c_str()) || POKER_TU_QUY == atof((*rc).c_str())){
                if (this->currRoomTypePK == 0){
                    if (this->winPK > dataManager.getAppConfig().notifyZen){
                        string winCell = "";
                
                        string msg = "";
                        msg = StringUtils::format("%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s//%s|%s|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(1237).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(984).c_str(),mUtils::convertMoneyEx(this->winPK).c_str(),this->getCoin(this->currRoomTypePK).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(12371).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9841).c_str(),mUtils::convertMoneyEx(this->winPK).c_str(),this->getCoin(this->currRoomTypePK).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(12372).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9842).c_str(),mUtils::convertMoneyEx(this->winPK).c_str(),this->getCoin(this->currRoomTypePK).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(12373).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9843).c_str(),mUtils::convertMoneyEx(this->winPK).c_str(),this->getCoin(this->currRoomTypePK).c_str());
                        this->sendEventNotify(msg);
                        
                    }
                }else{
                    if (this->winPK > dataManager.getAppConfig().notifyGold){
                        string winCell = "";
                   
                        string msg = "";
                                msg = StringUtils::format("%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(1237).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(984).c_str(),mUtils::convertMoneyEx(this->winPK).c_str(),this->getCoin(this->currRoomTypePK).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(12371).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9841).c_str(),mUtils::convertMoneyEx(this->winPK).c_str(),this->getCoin(this->currRoomTypePK).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(12372).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9842).c_str(),mUtils::convertMoneyEx(this->winPK).c_str(),this->getCoin(this->currRoomTypePK).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(12373).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9843).c_str(),mUtils::convertMoneyEx(this->winPK).c_str(),this->getCoin(this->currRoomTypePK).c_str());
                        this->sendEventNotify(msg);
                    }
                }
            }
        }
  
        
        
		this->currentGameEnd++;
		if (this->currentGameEnd >= dataManager.getAppConfig().delay_rqoffer){
			this->sendEventEndGame();
			this->currentGameEnd = 0;
		}

	}
    
    else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_END_XDMINI)) {
        if (param->GetUtfString(EXT_FIELD_RESULT_GAME)){
            string rg = *param->GetUtfString(EXT_FIELD_RESULT_GAME);
            vector<string> arrRg = mUtils::splitString(rg, '-');
            if (arrRg.size() < 3)
                return;
            this->totalTX = atoi(arrRg.at(0).c_str()) + atoi(arrRg.at(1).c_str()) + atoi(arrRg.at(2).c_str());
        }
    }
    else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_GAME_RESULT_XDMINI)) {
        boost::shared_ptr<long> gwv = param->GetInt(EXT_FIELD_GAME_WIN_VALUE);
        if (gwv){
            this->winDoDen = *gwv;
        }

        
    }
    
    else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_END_TXMINI)) {
        if (param->GetUtfString(EXT_FIELD_RESULT_GAME)){
            string rg = *param->GetUtfString(EXT_FIELD_RESULT_GAME);
            vector<string> arrRg = mUtils::splitString(rg, '-');
            if (arrRg.size() < 3)
                return;
            this->totalTX = atoi(arrRg.at(0).c_str()) + atoi(arrRg.at(1).c_str()) + atoi(arrRg.at(2).c_str());
        }
    }
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_GAME_RESULT_TXMINI)) {
        boost::shared_ptr<long> gwv = param->GetInt(EXT_FIELD_GAME_WIN_VALUE);
        if (gwv){
            this->winTX = *gwv;
        }


	}
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_RECEIVE_AWARD_SILVER_DAILY_RESP)) {
		if (param->GetInt(AWARD_SILVER) == NULL || param->GetInt("rc") == NULL || param->GetInt(AWARD_GOLD) == NULL || param->GetUtfString("rd") == NULL) return;
		int rc = *param->GetInt("rc");
		if (rc == 0){
			int silver = *param->GetInt(AWARD_SILVER);
			int gold = *param->GetInt(AWARD_GOLD);

			string content = dataManager.GetSysString(530)+ " ";

			if (silver > 0)
				content += StringUtils::format("%s %s ", mUtils::convertMoneyEx(silver).c_str(), this->getCoin(1).c_str());
			if (gold > 0)
				content += StringUtils::format("%s %s ", mUtils::convertMoneyEx(gold).c_str(), this->getCoin(0).c_str());

			content += "!";
			auto _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;

			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setMessage(dataManager.GetSysString(141));
			layerPopup->setContentMess(content);
			layerPopup->setTag(24);
			layerPopup->showAButton();
			_currScene->addChild(layerPopup, 10000000,9116);
		}
	}
	else if (0 == ptrNotifiedCmd->compare("eensn")){
		boost::shared_ptr<string> content = param->GetUtfString("content");
		if (content == NULL) return;

		auto lstnotice = mUtils::splitString(*content, ';');
		for (int i = 0; i < lstnotice.size(); i++){
			auto lstitem = mUtils::splitString(lstnotice[i], '|');
			if (lstitem.size() < 2) return;
			if (atoi(lstitem[0].c_str()) == 1){
				boost::shared_ptr<ISFSObject> params(new SFSObject());
				boost::shared_ptr<IRequest> request(new ExtensionRequest("eeglsorq", params));
				GameServer::getSingleton().Send(request);

				auto _currScene = Director::getInstance()->getRunningScene();
				if (!_currScene) return;
				_currScene->removeChildByTag(TAG_LAYER_NAP_TIEN);
				LayerPopupThongBao* _layer = LayerPopupThongBao::create();
				_layer->setMessage(dataManager.GetSysString(141));
				_layer->setContentMess(lstitem[1]);
				_layer->showAButton();
				_currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_NOTIFICATION,9117);
			}
			else{
				auto _currScene = Director::getInstance()->getRunningScene();
				if (!_currScene) return;

				LayerShowChuChay* chuchay = (LayerShowChuChay*)_currScene->getChildByTag(19909);
				if (chuchay){
					chuchay->setContent(lstitem[1]);
				}
				else{
					LayerShowChuChay* top = LayerShowChuChay::create();
					top->setTag(19909);
					_currScene->addChild(top, ZORDER_LIST::ZORDER_TAIXIU_MINI,9118);
					top->setContent(lstitem[1]);
				}
			}
		}
	}
    else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_GAME_RESULT) && this->getGameID() == kGameXocDia)
    {
        boost::shared_ptr<long> gbv = param->GetInt(EXT_FIELD_GAME_BET_VALUE);
        if (gbv != NULL) {
            int m = (*gbv);
            if (m > 0){
                this->isEndGame = true;
                if (this->getCurrRoomType()==0){
                    if (m > dataManager.getAppConfig().notifyZen){
                        this->isNotify = true;
                        this->moneyNotify = m;
                    }
                }else{
                    if (m > dataManager.getAppConfig().notifyGold){
                        this->isNotify = true;
                        this->moneyNotify = m;
                    }
                }
                this->runEvent(1);
            }
        }
    }
//    else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_GAME_RESULT) && this->getGameID() == kGameBauTom)
//    {
//        boost::shared_ptr<long> gbv = param->GetInt(EXT_FIELD_GAME_BET_VALUE);
//        if (gbv != NULL) {
//            int m = (*gbv);
//            if (m > 0){
//                this->isEndGame = true;
//                if (this->getCurrRoomType()==0){
//                    if (m > dataManager.getAppConfig().notifyZen){
//                        this->isNotify = true;
//                        this->moneyNotify = m;
//                    }
//                }else{
//                    if (m > dataManager.getAppConfig().notifyGold){
//                        this->isNotify = true;
//                        this->moneyNotify = m;
//                    }
//                }
//                this->runEvent(1);
//            }
//        }
//    }
    
    else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_CHANGE_BALANCE_NOTIFY))
    {
        boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
        boost::shared_ptr<double> money_ptr = param->GetDouble(EXT_FIELD_USER_AMF_GOLD);
        long money = 0;
        if (name != NULL && money_ptr != NULL) {
            money = (long)(*money_ptr.get());
            if (this->getMyName() == *name){
                if (money > 0){
                this->isEndGame = true;
                    if (this->getCurrRoomType()==0){
                        if (money > dataManager.getAppConfig().notifyZen){
                            this->isNotify = true;
                            this->moneyNotify = money;
                        }
                    }else{
                        if (money > dataManager.getAppConfig().notifyGold){
                            this->isNotify = true;
                            this->moneyNotify = money;
                        }
                    }
                }

            else
                this->isEndGame = false;
        }
        }
    }
    else if (0 == ptrNotifiedCmd->compare(EXTENSION_REQUEST_LIST_EVENT_RESPONSE))
    {
        boost::shared_ptr<string> ler = param->GetUtfString(LIST_EVENT_RUNNING);
        if (ler) {
            dataManager.getAppConfig().events = *ler;
        }
    }
    else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_START_GAME_NOTIF))
    {
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return ;
        _currScene->removeChildByTag(12390);
    }

    if (strcmp(EXTENSION_REQUEST_REWARD_EVENT_REPONSE, ptrNotifiedCmd->c_str()) == 0){//OK
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene) return;
        boost::shared_ptr<string> rd = param->GetUtfString("rd");
        
        if (rd == NULL) return;
        LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
        layerPopup->showAButton();
        layerPopup->setMessage(dataManager.GetSysString(141));
        layerPopup->setContentMess(*rd);
        scene->addChild(layerPopup,2000);
        this->hideLoading();
    }
    
}
void SceneManager::sendEventNotify(string msg){
    if (msg.length()<=0 || msg == "") return;
    this->winTX = 0;
    this->winDoDen = 0;
    this->winCT = 0;
    this->winPK = 0;
    this->winVQMM = 0;
    this->moneyNotify = 0;
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString(EXT_FIELD_MESSAGE_STRING, msg);
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXTENSION_REQUEST_REWARD_EVENT_REQUEST, params));
    GameServer::getSingleton().Send(request);
}
void SceneManager::runEvent(int dieukien){
    if (this->isNotify){
        this->isNotify = false;
        string msg = "";
        
        if (this->moneyNotify>0){
        
        msg = StringUtils::format("%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s/%s|%s|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),mUtils::getGameNameByIDViet(getGameID())->c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(983).c_str(),mUtils::convertMoneyEx(this->moneyNotify).c_str(),this->getCoin(this->getCurrRoomType()).c_str(),dataManager.GetSysString(9811).c_str(),mUtils::getGameNameByIDCam(getGameID())->c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9831).c_str(),mUtils::convertMoneyEx(this->moneyNotify).c_str(),this->getCoin(this->getCurrRoomType()).c_str(),dataManager.GetSysString(9812).c_str(),mUtils::getGameNameByIDEng(getGameID())->c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9832).c_str(),mUtils::convertMoneyEx(this->moneyNotify).c_str(),this->getCoin(this->getCurrRoomType()).c_str(),dataManager.GetSysString(9813).c_str(),mUtils::getGameNameByIDCN(getGameID())->c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(this->getMyName()).c_str(),dataManager.GetSysString(9833).c_str(),mUtils::convertMoneyEx(this->moneyNotify).c_str(),this->getCoin(this->getCurrRoomType()).c_str());
        this->sendEventNotify(msg);
        }
    }
    this->isEndGame = false;
    if (!mUtils::isHaveEvent() || !this->showCuoiVan) return;
    this->showCuoiVan = false;
    int countEvent = mUtils::eventNum();
    for (int i = 0 ; i < countEvent ; i++){
        if (mUtils::isEventThangGame(i) && dieukien == 1){
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return ;
            _currScene->removeChildByTag(119);

            LayerPopupEventThoiVu* top = LayerPopupEventThoiVu::create();
            top->setCurrentEvent(i);
            top->setTag(119);
            _currScene->addChild(top);
        }
        if (mUtils::isEventThuagGame(i) && dieukien == 2){
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return ;
            _currScene->removeChildByTag(119);

            LayerPopupEventThoiVu* top = LayerPopupEventThoiVu::create();
            top->setCurrentEvent(i);
            top->setTag(119);
            _currScene->addChild(top);
        }
    }
}
void SceneManager::sendEventEndGame(){
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt("gid", 50);
	params->PutInt("gwv", 50);
	params->PutInt("get", 50);
	params->PutInt("gbv", 50);
	params->PutUtfString("roomName", "trigger");
	params->PutUtfString("accParId", "trigger");
	boost::shared_ptr<IRequest> request(new ExtensionRequest("eeggorq", params));
	GameServer::getSingleton().Send(request);
}
void SceneManager::OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	auto _currScene = Director::getInstance()->getRunningScene();
	if (_currScene) {
		if (SceneMain::getSingleton().currentLayer == SceneMain::getSingleton().typelayerLogin){
			return;
		}
	}
	//	//phan xu ly retry gio chi handle o popup reconnect ( only for bai moc-p2)
	//	if (this->getGameVersion() == GameVersion::p1)
	//	{
	//		////log("OnSmartFoxConnectionLost");
	//		boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
	//		boost::shared_ptr<void> ptrEventParamValueReason = (*ptrEventParams)["reason"];
	//		//string reason = *((boost::static_pointer_cast<string>)(ptrEventParamValueReason));
	//		boost::shared_ptr<string> message = ((boost::static_pointer_cast<string>)(ptrEventParamValueReason));
	//		////log(string("Connection was lost, Reason: " + *((boost::static_pointer_cast<string>)(ptrEventParamValueReason))).c_str());
	//		if (message->compare(*ClientDisconnectionReason::MANUAL) != 0)
	//		{
	//			if (message->compare(*ClientDisconnectionReason::IDLE) == 0)
	//			{
	//				//log("A disconnection occurred due to inactivity");
	//				timeoutLoadingCallback();
	//			}
	//			else if (message->compare(*ClientDisconnectionReason::KICK) == 0)
	//			{
	//				//log("You have been kicked by the moderator");
	//				showChatWithCurrentScene("Bạn đã bị kích bởi một nơi khác", -1);
	//				GameServer::getSingleton().getSmartFox()->Disconnect();
	//				// 			releaseCurrentLayerGame();
	//				// 			LayerMain::getSingleton().gotoServices();
	//				gotoLogin();
	//			}
	//			else if (message->compare(*ClientDisconnectionReason::BAN) == 0)
	//			{
	//				//log("You have been banned by the moderator");
	//				showChatWithCurrentScene(dataManager.GetSysString(82), -1);
	//				GameServer::getSingleton().getSmartFox()->Disconnect();
	//				//timeoutLoadingCallback();
	//			}
	//			else
	//			{
	//				//log("A disconnection occurred due to unknown reason; please check the server log");
	//				GameServer::getSingleton().getSmartFox()->Disconnect();
	//				gotoLogin();
	//			}
	//		}
	//		else
	//		{
	//			// Manual disconnection is usually ignored
	//		}
	//
	//	}
	//	else if (this->getGameVersion() == GameVersion::p2)
	//	{
	if (_currScene->getChildByTag(120) != NULL)
		return;
    _currScene->removeChildByTag(1504);
	LayerPopupReconnect* layerReconn = LayerPopupReconnect::create();
	_currScene->addChild(layerReconn, ZORDER_LIST::ZORDER_POPUP_RECONNECT, 9120);
	layerReconn->showReconnect();
    layerReconn->setTag(1504);
	//	}
}

void SceneManager::OnSmartFoxDisconnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//log("OnSmartFoxDisconnection");
	// 	this->hideLoading();
	// 	disconnectFromServer();
}

void SceneManager::OnSmartFoxSocketError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//log("OnSmartFoxSocketError");
	this->hideLoading();
	disconnectFromServer();
}

void SceneManager::OnSmartFoxConnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSuccess = (*ptrEventParams)["success"];
	boost::shared_ptr<bool> ptrValue = (boost::static_pointer_cast<bool>)(ptrEventParamValueSuccess);
	if (*ptrValue != true)//not success
	{
		nextStepConn();
	}
	else{ //connect success
		if (gListIpAddress.size() == 0)
			return;
		string sip = gListIpAddress.at(0);
		if (mCurrIpIndex > 0)
			sip = gListIpAddress.at(mCurrIpIndex - 1);
		gListIpAddress.clear();
		gListIpAddress.push_back(sip);
		mCurrIpIndex = 0;
	}
	hideLoading();
}

void SceneManager::OnSmartFoxRoomFindResult(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	// Rooms found
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRooms = (*ptrEventParams)["rooms"];
	boost::shared_ptr < vector<boost::shared_ptr<Room> > > ptrNotifiedRooms
		= ((boost::static_pointer_cast <vector< boost::shared_ptr<Room> >>)(ptrEventParamValueRooms));
	for (int i = 0; i < ptrNotifiedRooms->size(); i++)
	{
		boost::shared_ptr<Room> ptrRoom = ptrNotifiedRooms->at(i);
		if (ptrRoom == NULL)
			continue;
		string gameID = *ptrRoom->GroupId();
		int roomID = ptrRoom->Id();
		////log("Ban dang choi game %s phong so %d", mUtils::getGameNameByID(mGameID), mRoomID);
		//		LayerMain::getSingleton().autoJoinGameWithID(atoi(gameID.substr(0, 3).c_str()), roomID);

		break;
	}
}

void SceneManager::OnSmartFoxConnectionRetry(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//phan xu ly lost connect va retry gio chi handle o popup reconnect ( only for bai moc-p2)
	//	if (this->getGameVersion() == GameVersion::p1)
	//	{
	//log("SceneManager::OnSmartFoxConnectionRetry");
    
    bool isHasInternet = true;
#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    isHasInternet = NativeHelper::hasConnection();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    isHasInternet = IOS::tryIsInternetConnection();
#endif
    
    if (!isHasInternet)
    {
        this->hideLoading();
        //showChatWithCurrentScene(dataManager.GetSysString(623), -1);
       // showPopupFailed(2);
        /*        auto _currScene = Director::getInstance()->getRunningScene();
         if (!_currScene) return;
         SceneLogin* _LayerLoading = (SceneLogin*)_currScene->getChildByTag(tag_SceneLogin);
         if (_LayerLoading == NULL){
         gotoLogin();
         }*/
        return;
    }
    
	auto _currScene = Director::getInstance()->getRunningScene();
	if (_currScene) {
		if (_currScene->getChildByTag(tag_SceneLogin) != NULL){
			return;
		}
	}
	//show loading
	this->setCallBackLoading(this, callfunc_selector(SceneManager::timeoutLoadingCallback));
	setLoadingString(dataManager.GetSysString(212));
	this->showLoading(5);
	// showChatWithCurrentScene(dataManager.GetSysString(212), -1);
	//	}
}

void SceneManager::OnSmartFoxConnectionResume(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//log("SceneManager::OnSmartFoxConnectionResume");
	//	hideLoading();
}

void SceneManager::OnSmartFoxConnectionRetryFail(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//phan xu ly lost connect va retry gio chi handle o popup reconnect ( only for bai moc-p2)
	//log("SceneManager::OnSmartFoxConnectionRetryFail");
	//	if (this->getGameVersion() == GameVersion::p1)
	//	{
	disconnectFromServer();
	//	}
}

void SceneManager::disconnectFromServer()
{
	GameServer::getSingleton().getSmartFox()->Disconnect();
	//
	gotoLogin();
	showChatWithCurrentScene(dataManager.GetSysString(56), -1);
}

void SceneManager::timeoutLoadingCallback()
{
	//GameServer::getSingleton().getSmartFox()->Disconnect();
	//LayerMain::getSingleton().gotoServices();
	//	releaseCurrentLayerGame();
	gotoLogin();
	//	if (layerLogin != NULL)
	//{

		//layerLogin->onButtonLogin(NULL);
		connect2Server(0);
		//layerLogin->scheduleOnce(schedule_selector(LayerLogin::retryLogin), 1);

		// 		layerLogin->onButtonLogin(NULL);
		// 		connect2Server(0);
		//layerLogin->scheduleOnce(schedule_selector(LayerLogin::retryLogin), 1);

	//}
}

std::string SceneManager::getMyName()
{
	if (GameServer::getSingleton().getSmartFox()->MySelf() == NULL)
		return "";
	return (GameServer::getSingleton().getSmartFox()->MySelf()->Name())->c_str();
}

void SceneManager::gotoChonBanChoi(int gameID)
{

}

void SceneManager::showLoading(const float& timeOut /*= 10*/)
{
//    if (this->getIsFlagStore()){
//        return;
//    }
	Scene* currScene = Director::getInstance()->getRunningScene();
	if (currScene != NULL)
	{
		CCProgressLayer* layerLoading = (CCProgressLayer*)(currScene->getChildByTag(LAYER_POPUP_ID::POPUP_LOADING));
		if (layerLoading == NULL)
		{
			layerLoading = CCProgressLayer::create();
            layerLoading->setTag(LAYER_POPUP_ID::POPUP_LOADING);
			currScene->addChild(layerLoading);
		}
		layerLoading->showLoading(timeOut);
	}
}
// 
// void SceneManager::showLoading(float dt)
// {
// 	auto _currScene = Director::getInstance()->getRunningScene();
// 	if (!_currScene) return;
// 	CCProgressLayer* _LayerLoading = (CCProgressLayer*)_currScene->getChildByTag(tag_SceneLoading);
// 	if (_LayerLoading){
// 		_LayerLoading->setTimeOut(dt);
// 		_LayerLoading->show();
// 		_LayerLoading->setLocalZOrder(ZORDER_LIST::ZORDER_LAYER_LOADING);
// 	}
// 	else
// 	{
// 		CCProgressLayer* _LayerLoading = CCProgressLayer::create();
// 		_LayerLoading->show();
// 		_LayerLoading->setTimeOut(dt);
// 		_currScene->addChild(_LayerLoading, ZORDER_LIST::ZORDER_LAYER_LOADING, tag_SceneLoading);
// 	}
// }

void SceneManager::hideLoading()
{
	Scene* currScene = Director::getInstance()->getRunningScene();
	if (currScene != NULL)
	{
		CCProgressLayer* layerLoading = (CCProgressLayer*)currScene->getChildByTag(LAYER_POPUP_ID::POPUP_LOADING);
		if (layerLoading != NULL)
		{
			layerLoading->hideLoading(1);
		}
	}
}

void SceneManager::setCallBackLoading(Ref* target, SEL_CallFunc listener)
{
	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return;
	Scene* currScene = Director::getInstance()->getRunningScene();
	if (currScene != NULL)
	{
		CCProgressLayer* layerLoading = static_cast<CCProgressLayer*>(currScene->getChildByTag(LAYER_POPUP_ID::POPUP_LOADING));
		if (layerLoading != NULL)
		{
			layerLoading->setCallBack(target, listener);
		}
	}
}

void SceneManager::setLoadingTag(int tag)
{
	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return;
	CCProgressLayer* _LayerLoading = (CCProgressLayer*)_currScene->getChildByTag(tag_SceneLoading);
	if (_LayerLoading)
		_LayerLoading->setTag(tag);
	else
	{
		CCProgressLayer* _LayerLoading = CCProgressLayer::create();
		_currScene->addChild(_LayerLoading, ZORDER_LIST::ZORDER_LAYER_LOADING, tag_SceneLoading);
		_LayerLoading->setTag(tag);
	}
}

int SceneManager::getLoadingTag(){
	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return -1;
	CCProgressLayer* _LayerLoading = (CCProgressLayer*)_currScene->getChildByTag(tag_SceneLoading);
	if (_LayerLoading)
		return _LayerLoading->getTag();
	return -1;
}

void SceneManager::setLoadingString(string msg)
{
	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return;
	CCProgressLayer* _LayerLoading = (CCProgressLayer*)_currScene->getChildByTag(tag_SceneLoading);
	if (_LayerLoading)
		return _LayerLoading->setStatusString(msg);
}

void SceneManager::OnSmartFoxBuddyListInit(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	if (GameServer::getSingleton().getSmartFox()->BuddyManager()->MyOnlineState() != true){
		boost::shared_ptr<IRequest> request(new Buddylist::GoOnlineRequest(true));
		GameServer::getSingleton().Send(request);
	}
	else
		changeBuddyGameState("");
}
void SceneManager::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	// Room joined
    SceneManager::getSingleton().hideLoading();
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
	boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
	if (ptrNotifiedRoom == NULL)
		return;
	int roomID = ptrNotifiedRoom->Id();
    if (ptrNotifiedRoom->GroupId()->compare(SERVICE_CHAT_ROOM) == 0 || ptrNotifiedRoom->GroupId()->compare("200") == 0){
        if (this->isInGame){
            boost::shared_ptr<IRequest> request2(new UnsubscribeRoomGroupRequest(boost::to_string(118)));
            GameServer::getSingleton().Send(request2);
            boost::shared_ptr<IRequest> request(new LeaveRoomRequest(ptrNotifiedRoom));
            GameServer::getSingleton().Send(request);
        }
		return;
    }
	int gameID = atoi(ptrNotifiedRoom->GroupId()->c_str());
	//log("Join Room In Chon Ban Choi ID = %d ------------------------------", gameID);    //
	boost::shared_ptr<RoomVariable> typePtr = ptrNotifiedRoom->GetVariable(EXT_FIELD_ROOM_TYPE);
	if (typePtr)
	{
		int typeRoom = *typePtr->GetIntValue();
		this->setCurrRoomType(typeRoom);
	}
	string strGameID = StringUtils::format("%d", gameID);
	if (strGameID.length() > 3)
		strGameID = strGameID.substr(0, 3);
	int gameIDParse = atoi(strGameID.c_str());

	if (GameServer::getSingleton().getSmartFox()->BuddyManager()->Inited()){
		string _str = StringUtils::format("%d,%d", getGameID(), roomID);
		changeBuddyGameState(_str);
	}

	//update 2/8
	if (this->nGameID == kGameTaiXiu || this->nGameID == kGameQuayThuong || this->nGameID == kGameXocDia || this->nGameID == kGameBauTom || this->nGameID == kGameSlot)
	{
		//only 4 taixiu and quay thuong
		if (getIsChuyenTab() == false)
			gotoGame(getGameID());
		//update deny all invite
		boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
		boost::shared_ptr<int> a(new int(0));
		boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("pai", a, VARIABLETYPE_INT));
		collectionUserVariable->push_back(variable);	//
		boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
		GameServer::getSingleton().Send(request);
		//unscribe
		boost::shared_ptr<IRequest> request1(new UnsubscribeRoomGroupRequest(StringUtils::format("%d", getGameID())));
	}
	else
	{
       // if(this->nGameID != kGameXiTo && this->nGameID != kGamePoker){
            this->gotoGame(gameIDParse);
     //   }
	}
}

void SceneManager::OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	//do nothing
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
    boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);

    if (ptrNotifiedRoom == NULL) return;

    if (ptrNotifiedRoom->GroupId()->compare(SERVICE_CHAT_ROOM) == 0
        || ptrNotifiedRoom->Id() == 118 || ptrNotifiedRoom->GroupId()->compare("200") == 0 || ptrNotifiedRoom->Id() == 200)
        return;
    //if it is me
   // if (ptrNotifiedUser->IsItMe() && GameServer::getSingleton().getSmartFox()->BuddyManager()->Inited()){
    //    changeBuddyGameState("");
    //}
    //only for taixiu
//    int currGameID = getGameID();
//   // if (ptrNotifiedUser->IsItMe()){
//        setAutoCreateRoom(false);
//        this->setRejoinInfo(RejoinInfoStruct());
//        if (getGameID() == kGameTaiXiu || getGameID() == kGameQuayThuong) {
//
//            gotoMain();
//        }
//        //update 3/8
//        else if (currGameID == kGameXocDia || currGameID == kGameBauTom || currGameID == kGameSlot){
//            if (getCurrRoomType() == -1)
//                gotoMain();
//            else{
//                this->showLoading();
//                boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(StringUtils::format("%d", getGameID())));
//                GameServer::getSingleton().Send(request);
//            }
//
//        }
//        else {
//            //MINHDV -changes - showpopup if betvalue last room larger user money
            int gameID = getGameID();
//            boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
//            if (mySelf)
//            {
//                boost::shared_ptr<UserVariable> amfptr;
//                if (this->getCurrRoomType() == 0){
//                    amfptr = mySelf->GetVariable("amf");
//                }
//                else{
//                    amfptr = mySelf->GetVariable("amfs");
//
//                }
//                if (amfptr)
//                {
//                    boost::shared_ptr<double> amf = amfptr->GetDoubleValue();
//                    if (amf)
//                    {
//                        double betUser = 0;
//                        vector<strGameCF> arrBet = this->getArrConfig();
//                        int bet_ratio = 1;
//                        for (int i = 0; i < arrBet.size(); ++i)
//                        {
//                            if (arrBet.at(i).gameid == gameID)
//                            {
//                                bet_ratio = arrBet.at(i).ratioBet;
//                                break;
//                            }
//                        }
//                        betUser = *amf / bet_ratio;
//                        double money_bet = this->getBetValue();
//                        if (betUser < money_bet)
//                        {
//                            ///set flag to show popup
//                            setKickNem(true);
//                        }
//                    }
//                }
//            }
//            gotoPickGame(gameID);
//        }
   /// }
 //   gotoMain();
}
void SceneManager::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	// Get the user parameter of the event
    SceneManager::getSingleton().hideLoading();

	boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	// Get the room parameter of the event
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
	boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
	//
	if (ptrNotifiedUser == NULL || ptrNotifiedRoom == NULL) return;

	if (ptrNotifiedRoom->GroupId()->compare(SERVICE_CHAT_ROOM) == 0
        || ptrNotifiedRoom->Id() == 118 || ptrNotifiedRoom->GroupId()->compare("200") == 0
        || ptrNotifiedRoom->Id() == 200)
		return;
	//if it is me
	if (ptrNotifiedUser->IsItMe() && GameServer::getSingleton().getSmartFox()->BuddyManager()->Inited()){
		changeBuddyGameState("");
	}
	//only for taixiu
	int currGameID = getGameID();
	if (ptrNotifiedUser->IsItMe()){
		setAutoCreateRoom(false);
		this->setRejoinInfo(RejoinInfoStruct());
		if ( getGameID() == kGameQuayThuong || currGameID == 0 || currGameID == -1) {
            SceneMain::getSingleton().prepareToPickGame();

			gotoMain();
		}
		//update 3/8
		else if (currGameID == kGameXocDia || currGameID == kGameBauTom || currGameID == kGameSlot|| currGameID == kGameTaiXiu){
            if (this->getIsChuyenTab()) return;
            if (getCurrRoomType() == -1){
				gotoMain();
            }
//            else{
//                this->showLoading();
//                boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(StringUtils::format("%d", getGameID())));
//                GameServer::getSingleton().Send(request);
//            }

		}
		else {
			//MINHDV -changes - showpopup if betvalue last room larger user money
			int gameID = getGameID();
			boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
			if (mySelf)
			{
				boost::shared_ptr<UserVariable> amfptr;
				if (this->getCurrRoomType() == 0){
					amfptr = mySelf->GetVariable("amf");
				}
				else{
					amfptr = mySelf->GetVariable("amfs");

				}
				if (amfptr)
				{
					boost::shared_ptr<double> amf = amfptr->GetDoubleValue();
					if (amf)
					{
						double betUser = 0;
						vector<strGameCF> arrBet = this->getArrConfig();
						int bet_ratio = 1;
						for (int i = 0; i < arrBet.size(); ++i)
						{
							if (arrBet.at(i).gameid == gameID)
							{
								bet_ratio = arrBet.at(i).ratioBet;
								break;
							}
						}
						betUser = *amf / bet_ratio;
						double money_bet = this->getBetValue();
						if (betUser < money_bet)
						{
							///set flag to show popup
							setKickNem(true);
						}
					}
				}
			}
			gotoPickGame(gameID);            
		}
	}
}

void SceneManager::changeBuddyGameState(string gameState)
{
	boost::shared_ptr<list<boost::shared_ptr<BuddyVariable> > > myVars(new list<boost::shared_ptr<BuddyVariable> >());
	boost::shared_ptr<string> _gameState(new string(gameState));
	boost::shared_ptr<SFSBuddyVariable> currentTrack(new SFSBuddyVariable("gs", _gameState, VARIABLETYPE_STRING));
	myVars->push_back(currentTrack);
	boost::shared_ptr<IRequest> request(new Buddylist::SetBuddyVariablesRequest(myVars));
	GameServer::getSingleton().Send(request);
}

void SceneManager::OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	/*
	 int gameID = getGameID();
	 if (gameID == kGameTaiXiu || gameID == kGameQuayThuong)
	 {
	 //log("OnSmartFoxSubscribeRoomGroup Game Tai Xiu");
	 boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(gameID));
	 if (list_room->size() > 0)
	 {
	 boost::shared_ptr<Room> room = list_room->at(0);
	 if (room != NULL)
	 {
	 //
	 //log("Room Tai Xiu != NULL");
	 boost::shared_ptr<IRequest> request(new JoinRoomRequest(room->Id(), ""));
	 GameServer::getSingleton().Send(request);
	 //unsubscribe
	 boost::shared_ptr<IRequest> request1(new UnsubscribeRoomGroupRequest(StringUtils::format("%d", gameID)));
	 GameServer::getSingleton().Send(request1);
	 }
	 }
	 else{
	 boost::shared_ptr<IRequest> request(new JoinRoomRequest(gameID, ""));
	 GameServer::getSingleton().Send(request);
	 }
	 }*/

	int gameID = getGameID();
	if (gameID == kGameQuayThuong)
	{
		//log("OnSmartFoxSubscribeRoomGroup Game ID = %d", gameID);
		boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(gameID));
		//log("list room : %d", list_room->size());
		if (list_room->size() > 0)
		{
			boost::shared_ptr<Room> room = list_room->at(0);
			if (room != NULL)
			{
				//
				boost::shared_ptr<long> lastRoomID(new long(-1));
				boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
				if (lastRoom)
					lastRoomID = boost::shared_ptr<long>(new long(lastRoom->Id()));
				//log("Room Tai Xiu != NULL");
				boost::shared_ptr<IRequest> request(new JoinRoomRequest(room, "", lastRoomID, false));
				GameServer::getSingleton().Send(request);
			}
		}
	}
	//update 2/8
	if (gameID == kGameXocDia || gameID == kGameTaiXiu || gameID == kGameBauTom || gameID == kGameSlot){
		//log("OnSmartFoxSubscribeRoomGroup Game ID = %d", gameID);
		boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(gameID));
		//log("list room : %d", list_room->size());
		for (int i = 0; i < list_room->size(); i++)
		{
			boost::shared_ptr<Room> room = list_room->at(i);
			if (room != NULL)
			{
				if (room->GetVariable("roomType") != NULL){
					int roomType = *room->GetVariable("roomType")->GetIntValue();
					//log("roomType: %d - currRoom: %d", roomType, getCurrRoomType());
					/*if (getCurrRoomType() == 0){
						if (this->getMoneyByType(0) < this->getMinMoney(kGameXocDia))
						setCurrRoomType(1);
						}

						if (getCurrRoomType() == 1){
						if (this->getMoneyByType(1) < this->getMinMoney(kGameXocDia))
						setCurrRoomType(0);
						}*/

					if (roomType == getCurrRoomType()){
						boost::shared_ptr<long> lastRoomID(new long(-1));
						boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
						if (lastRoom)
							lastRoomID = boost::shared_ptr<long>(new long(lastRoom->Id()));
						//log("Room Tai Xiu != NULL");
						boost::shared_ptr<IRequest> request(new JoinRoomRequest(room, "", lastRoomID, false));
						GameServer::getSingleton().Send(request);
					}
				}
			}
		}
	}
}

void SceneManager::OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	int gameID = getGameID();
	if (gameID == kGameTaiXiu || gameID == kGameQuayThuong || gameID == kGameXocDia || gameID == kGameBauTom|| gameID == kGameSlot)
	{
		boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
		boost::shared_ptr<void> ptrEventParamValueErrorCode = (*ptrEventParams)["errorCode"];
		boost::shared_ptr<short> ptrErrorCode = ((boost::static_pointer_cast<short>))(ptrEventParamValueErrorCode);
		//
		//log("OnSmartFoxSubscribeRoomGroupError: %s", mUtils::getErrorSmartfoxExplain(*ptrErrorCode).c_str());
		if (*ptrErrorCode == 24){
			if (gameID == kGameXocDia || gameID == kGameTaiXiu || gameID == kGameBauTom || gameID == kGameSlot){
				//log("OnSmartFoxSubscribeRoomGroup Game ID = %d", gameID);
				boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(gameID));
				//log("list room : %d", list_room->size());
				for (int i = 0; i < list_room->size(); i++)
				{
					boost::shared_ptr<Room> room = list_room->at(i);
					if (room != NULL)
					{
						if (room->GetVariable("roomType") != NULL){
							int roomType = *room->GetVariable("roomType")->GetIntValue();
							//log("roomType: %d - currRoom: %d", roomType, getCurrRoomType());
							//

							/* if (getCurrRoomType() == 0){
								 if (this->getMoneyByType(0) < this->getMinMoney(kGameXocDia))
								 setCurrRoomType(1);
								 }

								 if (getCurrRoomType() == 1){
								 if (this->getMoneyByType(1) < this->getMinMoney(kGameXocDia))
								 setCurrRoomType(0);
								 }*/
							if (roomType == getCurrRoomType()){
								boost::shared_ptr<long> lastRoomID(new long(-1));
								boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
								if (lastRoom)
									lastRoomID = boost::shared_ptr<long>(new long(lastRoom->Id()));
								//log("Room Tai Xiu != NULL");
								boost::shared_ptr<IRequest> request(new JoinRoomRequest(room, "", lastRoomID, false));
								GameServer::getSingleton().Send(request);
							}
						}
					}
				}
			}
			else{
				boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(gameID));
				if (list_room->size() > 0)
				{
					boost::shared_ptr<Room> room = list_room->at(0);
					if (room != NULL)
					{
						//
						//log("Room Tai Xiu != NULL");
						boost::shared_ptr<IRequest> request(new JoinRoomRequest(room->Id(), ""));
						GameServer::getSingleton().Send(request);
						//unsubscribe
						boost::shared_ptr<IRequest> request1(new UnsubscribeRoomGroupRequest(StringUtils::format("%d", gameID)));
						GameServer::getSingleton().Send(request1);
					}
				}
				else{
					boost::shared_ptr<IRequest> request(new JoinRoomRequest(gameID, ""));
					GameServer::getSingleton().Send(request);
				}
			}

		}
		//
		//showChatWithCurrentScene(dataManager.GetSysString(203), -1);
	}
}

static std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

static inline std::string base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

static string decryptBytes(string sVal)
{
    //return sVal;
	sVal = base64_decode(sVal);
	char key[6] = { 'D', 'T', 'S', 'K', 'E', 'Y' };
	char *_newVal = new char[sVal.length() + 1];
    //_newVal->au
	for (int i = 0; i < sVal.length(); i++){
		_newVal[i] = sVal[i] ^ key[i % 6];
	}
	_newVal[sVal.length()] = 0;
    string data = string(_newVal);
    delete _newVal;
	return data;
}

void SceneManager::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	////log("SceneManager::onHttpRequestCompleted");
	if (!response)
	{
		return;
	}
	long statusCode = response->getResponseCode();
	char statusString[64] = {};
	if (!response->isSucceed())
	{
		//this->hideLoading();
		//for level1 only
		if (strcmp(response->getHttpRequest()->getTag(), "uploadData") == 0 && mCurrStepCheckConn == tagUploadData1){
			if (gListIpAddress.size() == 0){
				checkIpLevel2(IP_DEFAULT);
				//
				mCurrStepCheckConn = tagGetLevel2Info;
			}
			else
				//this->scheduleOnce(schedule_selector(SceneManager::connect2Server), 2);
			{
				//MINHDV - fix schedule
				Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::connect2Server), this, 2, false);
			}
			return;
		}
		if (strcmp(response->getHttpRequest()->getTag(), "promotion") == 0){
			this->getPromotionHandlers().setUserID("");
		}
		if (strcmp(response->getHttpRequest()->getTag(), "huthuong") == 0){
			//log("loi hu thuong");
		}
		else if (strcmp(response->getHttpRequest()->getTag(), "notification") == 0){
			gListNotification.clear();
		}
		else if (strcmp(response->getHttpRequest()->getTag(), "event") == 0){
			UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
			UserDefault::getInstance()->setIntegerForKey("numEvent", 0);
		}
		else if (strcmp(response->getHttpRequest()->getTag(), "iplevel1") == 0){
			if (dataManager.getAppConfig().url_list_ip.compare(response->getHttpRequest()->getUrl()) == 0){
                this->showPopupFailed(3);
			}
			else{
				checkIpLevel2(IP_DEFAULT);
				//
				mCurrStepCheckConn = tagGetLevel2Info;
			}
		}
		else if (strcmp(response->getHttpRequest()->getTag(), "iplevel2") == 0
			|| (mCurrStepCheckConn == tagUploadData2 && strcmp(response->getHttpRequest()->getTag(), "uploadData") == 0)){
			if (strcmp(IP_DEFAULT, response->getHttpRequest()->getUrl()) == 0 && strcmp(response->getHttpRequest()->getTag(), "iplevel2") == 0){
				checkIpLevel2(IP_DEFAULT1);
				return;
			}
			// 			Chat *toast = Chat::create(dataManager.GetSysString(7), -1);
			// 			auto _currScene = Director::getInstance()->getRunningScene();
			// 			if (_currScene){
			// 				_currScene->addChild(toast);
			// 			}
			gListIpAddress.clear();
			//
			gListIpAddress = mUtils::splitString(dataManager.getAppConfig().ip_default, ';');
			mCurrIpIndex = 0;
			//
			mCurrStepCheckConn = tagConnLevelDefault;
			//this->scheduleOnce(schedule_selector(SceneManager::connect2Server), 2);
			//MINHDV - fix schedule
			Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::connect2Server), this, 2, false);
		}
		else if (strcmp(response->getHttpRequest()->getTag(), "filter_ip") == 0)
		{
			this->initFirst();
			this->gotoLogin();
		}
        else if (strcmp(response->getHttpRequest()->getTag(), "firebase") == 0)
        {
            log("firebase errror");
        }
        
		return;
	}
	//MINHDV - get notification on scene pickroom
	// 	else if (strcmp(response->getHttpRequest()->getTag(), "notificationPR") == 0)
	// 	{
	// 		gListNotificationPickRoom.clear();
	// 	}
	std::vector<char> *buffer = response->getResponseData();
	if (buffer->size() == 0)
		return;
	////log("datas receive from [%s]: %s", response->getHttpRequest()->getTag(), buffer[0].data());
	if (strcmp(response->getHttpRequest()->getTag(), "promotion") == 0){
		// dump data

		PromotionHandler _handler;
		vector<PromotionInfo> var;
		this->setLstPromotionInfo(var);
		_handler.handleDatas(buffer[0].data());
		//
		boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
		if (myself == NULL)
			return;
		_handler.setUserID(*myself->Name());
		//
		if (_handler.getListPromotionInfo().size() == 0)
			return;
		//
		this->setPromotionHandlers(_handler);
	}
	else if (strcmp(response->getHttpRequest()->getTag(), "huthuong") == 0){
		std::vector<char>* buffer = response->getResponseData();
		string datas = buffer[0].data();
		//  datas = decryptBytes(datas);
		unsigned vt1 = datas.find_first_of('{');
		unsigned vt = datas.find_last_of('}');
		if (vt != -1 && vt1 != -1)
			datas = datas.substr(vt1, vt - vt1 + 1);
		datas = mUtils::replaceAll(datas, "\r", "");
		datas = mUtils::replaceAll(datas, "\n", "");
		dataManager.getAppConfig().huthuong = datas;
        auto lst =  mUtils::splitString(dataManager.getAppConfig().huthuong,';');
        
        for (int i = 0; i <lst.size();i++){
            if (atoi(mUtils::splitString(lst[i],':')[0].c_str()) == 128){
                this->currHuTaiXiu = atoi(mUtils::splitString(lst[i],':')[1].c_str());
                
            }
            if (atoi(mUtils::splitString(lst[i],':')[0].c_str()) == 132){
                this->currHuCaoThap= atoi(mUtils::splitString(lst[i],':')[1].c_str());
                
            }
            if (atoi(mUtils::splitString(lst[i],':')[0].c_str()) == 1303){
                this->currHuPoker = atoi(mUtils::splitString(lst[i],':')[1].c_str());
                
            }
        }
        
	}
    else if (strcmp(response->getHttpRequest()->getTag(), "firebase") == 0){
        std::vector<char>* buffer = response->getResponseData();
        string datas = buffer[0].data();
        //  datas = decryptBytes(datas);
        unsigned vt1 = datas.find_first_of('{');
        unsigned vt = datas.find_last_of('}');
        if (vt != -1 && vt1 != -1)
            datas = datas.substr(vt1, vt - vt1 + 1);
        datas = mUtils::replaceAll(datas, "\r", "");
        datas = mUtils::replaceAll(datas, "\n", "");
        //        datas = mUtils::replaceAll(datas, "\"", "");
        
        //
        rapidjson::Document readdoc;
    }
	else if (strcmp(response->getHttpRequest()->getTag(), "notification") == 0){
		std::vector<char>* buffer = response->getResponseData();
		string datas = buffer[0].data();
		//  datas = decryptBytes(datas);
		unsigned vt1 = datas.find_first_of('{');
		unsigned vt = datas.find_last_of('}');
		if (vt != -1 && vt1 != -1)
			datas = datas.substr(vt1, vt - vt1 + 1);
		datas = mUtils::replaceAll(datas, "\r", "");
		datas = mUtils::replaceAll(datas, "\n", "");
//        datas = mUtils::replaceAll(datas, "\"", "");

		//
		rapidjson::Document readdoc;
		bool bRet = false;
		ssize_t size = 0;
		std::string load_str = datas;
		readdoc.Parse<0>(load_str.c_str());
		////log(datas.c_str());
		if (readdoc.HasParseError())
		{
			//heckIpLevel2(IP_DEFAULT);
			return;
		}
		if (readdoc.IsObject())
		{
			string textShow = "";
			string linkShow = "";
			string textPop = "";
			string linkPop = "";
			bool isShow = false;
			gListNotification.clear();
			if (readdoc.HasMember("scroll"))
			{
				const rapidjson::Value& b = readdoc["scroll"];
				if (b.IsArray()){
					// rapidjson uses SizeType instead of size_t.
					for (rapidjson::SizeType i = 0; i < b.Size(); i++)
					{
						const rapidjson::Value& c = b[i];

						if (c.HasMember("text"))
						{
							textShow = c["text"].GetString();
						}
						if (c.HasMember("link"))
						{
							linkShow = c["link"].GetString();
						}

					}
				}
			}
			if (readdoc.HasMember("popup"))
			{

				const rapidjson::Value& b = readdoc["popup"];
				if (b.IsArray()){
					if (b.Size() == 0)
						isShow = false;
					else
						isShow = true;
					for (rapidjson::SizeType i = 0; i < b.Size(); i++)
					{
						const rapidjson::Value& c = b[i];
						if (c.HasMember("text"))
						{
							textPop = c["text"].GetString();
						}
						if (c.HasMember("link"))
						{
							linkPop = c["link"].GetString();
						}

					}
				}
				NotificationInfo notifiInfo;
				notifiInfo.textShow = textShow;
				notifiInfo.linkShow = linkShow;
				notifiInfo.textPop = textPop;
				notifiInfo.linkPop = linkPop;
				notifiInfo.isShow = isShow;
				gListNotification.push_back(notifiInfo);
			}

		}
	}
	else if (strcmp(response->getHttpRequest()->getTag(), "event") == 0){
		std::vector<char>* buffer = response->getResponseData();
		string datas = buffer[0].data();
		//  datas = decryptBytes(datas);
		unsigned vt1 = datas.find_first_of('{');
		unsigned vt = datas.find_last_of('}');
		if (vt != -1 && vt1 != -1)
			datas = datas.substr(vt1, vt - vt1 + 1);
		datas = mUtils::replaceAll(datas, "\r", "");
		datas = mUtils::replaceAll(datas, "\n", "");
		//
		rapidjson::Document readdoc;
		bool bRet = false;
		ssize_t size = 0;
		std::string load_str = datas;
		readdoc.Parse<0>(load_str.c_str());
		////log(datas.c_str());
		if (readdoc.HasParseError())
		{
			//heckIpLevel2(IP_DEFAULT);
			return;
		}
		if (readdoc.IsObject())
		{
			if (readdoc.HasMember("status"))
			{
				int status = readdoc["status"].GetInt();
				if (status > 0){
					UserDefault::getInstance()->setBoolForKey("isShowEvent", true);
					UserDefault::getInstance()->setIntegerForKey("numEvent", status);
				}
				else{
					UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
					UserDefault::getInstance()->setIntegerForKey("numEvent", 0);
				}
			}
			if (readdoc.HasMember("cats"))
			{
				const rapidjson::Value& d = readdoc["cats"];

				if (d.IsArray()){
					//log("here children");
					rapidjson::SizeType size = d.Size();
					this->lstEvent.clear();
					for (rapidjson::SizeType i = 0; i < size; ++i){
						event e;
						e.name = d[i]["name"].GetString();
						e.link = d[i]["link"].GetString();
						lstEvent.push_back(e);
					}
				}
			}

		}
	}
	else if (strcmp(response->getHttpRequest()->getTag(), "shareinfo") == 0){
		std::vector<char>* buffer = response->getResponseData();
		string datas = buffer[0].data();
		//  datas = decryptBytes(datas);
		unsigned vt1 = datas.find_first_of('{');
		unsigned vt = datas.find_last_of('}');
		if (vt != -1 && vt1 != -1)
			datas = datas.substr(vt1, vt - vt1 + 1);
		datas = mUtils::replaceAll(datas, "\r", "");
		datas = mUtils::replaceAll(datas, "\n", "");
		//
		rapidjson::Document readdoc;
		bool bRet = false;
		ssize_t size = 0;
		std::string load_str = datas;
		readdoc.Parse<0>(load_str.c_str());
		////log(datas.c_str());
		if (readdoc.HasParseError())
		{
			//heckIpLevel2(IP_DEFAULT);
			return;
		}
		if (readdoc.IsObject())
		{
			const char* key = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			key = dataManager.getAppConfig().version_android.c_str();
#endif
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
			key = dataManager.getAppConfig().version_ios.c_str();
#endif
			if (readdoc.HasMember(key))
			{
				const rapidjson::Value& d = readdoc[key];

				if (d.IsObject()) {
					if (d.HasMember("link"))
					{
						dataManager.getAppConfig().linkshare = d["link"].GetString();
					}
					if (d.HasMember("title"))
					{
						dataManager.getAppConfig().titleshare = d["title"].GetString();
					}
					if (d.HasMember("text"))
					{
						dataManager.getAppConfig().textshare = d["text"].GetString();

					}
					if (d.HasMember("image"))
					{
						dataManager.getAppConfig().imageshare = d["image"].GetString();

					}
				}
			}
		}
	}
	else if (strcmp(response->getHttpRequest()->getTag(), "iplevel1") == 0){
		//this->hideLoading();
        if (this->isGetConfig) return;
        this->isGetConfig = true;
        this->hideLoading();
		string datas = buffer[0].data();
		datas = decryptBytes(datas);
		unsigned vt1 = datas.find_first_of('{');
		unsigned vt = datas.find_last_of('}');
		if (vt != -1 && vt1 != -1)
			datas = datas.substr(vt1, vt - vt1 + 1);
		datas = mUtils::replaceAll(datas, "\r", "");
		datas = mUtils::replaceAll(datas, "\n", "");
		//
		string prefixkey = "";
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		prefixkey = dataManager.getAppConfig().version_android + "_prefix";
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		prefixkey = dataManager.getAppConfig().version_ios + "_prefix";
#endif

		rapidjson::Document readdoc;
		bool bRet = false;
		ssize_t size = 0;
		std::string load_str = datas;
		readdoc.Parse<0>(load_str.c_str());
		////log(datas.c_str());
		if (readdoc.HasParseError())
		{
			checkIpLevel2(IP_DEFAULT);
			return;
		}
		if (readdoc.IsObject())
		{
            if (readdoc.HasMember("isVN"))
            {
                dataManager.getAppConfig().isVN = string(readdoc["isVN"].GetString()).compare("1") == 0;
            }
            if (readdoc.HasMember("iap_rate"))
            {
                dataManager.getAppConfig().iap_rate = atoi(string(readdoc["iap_rate"].GetString()).c_str());
            }
            
			if (readdoc.HasMember("url_notification"))
			{
				dataManager.getAppConfig().url_notification = readdoc["url_notification"].GetString();
			}
            if (readdoc.HasMember("url_notification_cam"))
            {
                dataManager.getAppConfig().url_notification_cam = readdoc["url_notification_cam"].GetString();
            }
            if (readdoc.HasMember("url_notification_cn"))
            {
                dataManager.getAppConfig().url_notification_cn = readdoc["url_notification_cn"].GetString();
            }
            if (readdoc.HasMember("url_notification_eng"))
            {
                dataManager.getAppConfig().url_notification_eng = readdoc["url_notification_eng"].GetString();
            }
            if (readdoc.HasMember("fanpage"))
            {
                dataManager.getAppConfig().fanpage = readdoc["fanpage"].GetString();
            }
			if (readdoc.HasMember("callcenter"))
			{
				dataManager.getAppConfig().callcenter = readdoc["callcenter"].GetString();
			}
			if (readdoc.HasMember("callcenter2"))
			{
				dataManager.getAppConfig().callcenter2 = readdoc["callcenter2"].GetString();
			}
			dataManager.getAppConfig().url_upload = "";
			if (readdoc.HasMember("url_upload"))
			{
				dataManager.getAppConfig().url_upload = readdoc["url_upload"].GetString();
			}
            if (readdoc.HasMember("gamemini"))
            {
                dataManager.getAppConfig().gamemini = readdoc["gamemini"].GetString();
            }
			if (readdoc.HasMember("url_avatar_upload"))
			{
				dataManager.getAppConfig().url_avatar_upload = readdoc["url_avatar_upload"].GetString();
			}
			if (readdoc.HasMember("url_giftcode"))
			{
				dataManager.getAppConfig().url_giftcode = readdoc["url_giftcode"].GetString();
			}
			if (readdoc.HasMember("fb_gift"))
			{
				dataManager.getAppConfig().moneyFB = readdoc["fb_gift"].GetString();
			}
			if (readdoc.HasMember(prefixkey.c_str()))
			{
				dataManager.getAppConfig().tientomocp2 = readdoc[prefixkey.c_str()].GetString();
			}
			if (readdoc.HasMember("url_event"))
			{
				dataManager.getAppConfig().url_event = readdoc["url_event"].GetString();
			}
            if (readdoc.HasMember("url_event_cam"))
            {
                dataManager.getAppConfig().url_event_cam = readdoc["url_event_cam"].GetString();
            }
            if (readdoc.HasMember("url_event_eng"))
            {
                dataManager.getAppConfig().url_event_eng = readdoc["url_event_eng"].GetString();
            }
            if (readdoc.HasMember("url_event_cn"))
            {
                dataManager.getAppConfig().url_event_cn = readdoc["url_event_cn"].GetString();
            }
			if (readdoc.HasMember("maintenance"))
			{
				dataManager.getAppConfig().maintenancestr = readdoc["maintenance"].GetString();
			}
			if (readdoc.HasMember("url_android_store"))
			{
				dataManager.getAppConfig().url_android_store = readdoc["url_android_store"].GetString();
			}
			if (readdoc.HasMember("url_ios_store"))
			{
				dataManager.getAppConfig().url_ios_store = readdoc["url_ios_store"].GetString();
			}
            if (readdoc.HasMember("url_ios_nostore"))
            {
                dataManager.getAppConfig().url_ios_nostore = readdoc["url_ios_nostore"].GetString();
            }
			if (readdoc.HasMember("force_update"))
			{
				dataManager.getAppConfig().force_update = readdoc["force_update"].GetString();
			}
			if (readdoc.HasMember("url_charge"))
			{
				dataManager.getAppConfig().url_charge = readdoc["url_charge"].GetString();
			}
			if (readdoc.HasMember("lst_sms"))
			{
				dataManager.getAppConfig().strListSMS = readdoc["lst_sms"].GetString();
			}
			if (readdoc.HasMember("new_version"))
			{
				string newVersion = readdoc["new_version"].GetString();
				dataManager.getAppConfig().versionforceupdate = newVersion;
				//				auto lstVersion = mUtils::splitString(newVersion, ';');
				//				if (lstVersion.size() > 1){
				//					dataManager.getAppConfig().versionIOS = lstVersion[0];
				//					dataManager.getAppConfig().versionAndroid = lstVersion[1];
				//				}
			}
            if (readdoc.HasMember("list_game3"))
            {
                #if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
                    string _list_game = readdoc["list_game3"].GetString();
//                string _list_game = readdoc["list_game_and"].GetString();
                    dataManager.getAppConfig().list_game = _list_game;
                #endif
                
            }
            
            if (readdoc.HasMember("list_game_and"))
            {
                #if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
                    string _list_game = readdoc["list_game_and"].GetString();
                    dataManager.getAppConfig().list_game = _list_game;
                #endif
            }
            
            if (this->isNagaWin){
                if (readdoc.HasMember("list_game_bvnew"))
                {
                    string _list_game = readdoc["list_game_bvnew"].GetString();
                    dataManager.getAppConfig().list_game = _list_game;
                    
                }
            }
            if (readdoc.HasMember("lst_game_new")){
                string _list_new_game = readdoc["lst_game_new"].GetString();
                dataManager.getAppConfig().list_new_game = _list_new_game;
            }
            
			if (readdoc.HasMember("hidefb"))
			{
				string hidefb_ = readdoc["hidefb"].GetString();
				dataManager.getAppConfig().hidefb = hidefb_;
				//				auto lstVersion = mUtils::splitString(newVersion, ';');
				//				if (lstVersion.size() > 1){
				//					dataManager.getAppConfig().versionIOS = lstVersion[0];
				//					dataManager.getAppConfig().versionAndroid = lstVersion[1];
				//				}
			}
			if (readdoc.HasMember("facebook_appid"))
			{
				string fb_id = readdoc["facebook_appid"].GetString();
				//log("FB ID = %s", fb_id.c_str());
				dataManager.getAppConfig().facebookappid = fb_id;
				//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				//				Button* btn = SceneLogin::getInstance()->btnLoginFace;
				//				if (btn != NULL)
				//					btn->setVisible(true);
				//#endif
			}
			if (readdoc.HasMember("linksdt"))
			{
				string link = readdoc["linksdt"].GetString();
				dataManager.getAppConfig().urllinksdt = link;
			}
            if (readdoc.HasMember("flagstore"))
            {
                string flagstore = readdoc["flagstore"].GetString();
                this->flagStore(flagstore);
            }
            if (readdoc.HasMember("isDownLoadResource"))
            {
                string vls = readdoc["isDownLoadResource"].GetString();
                dataManager.getAppConfig().isDownLoadResource = vls.compare("1") == 0;;
            }
			gListIpAddress.clear();
			if (readdoc.HasMember("ip_list"))
			{
				string sip = readdoc["ip_list"].GetString();
				vector<string> lstIp = mUtils::splitString(sip, ';');
				//
				mCurrIpIndex = 0;
				for (int l = 0; l < lstIp.size(); l++)
					gListIpAddress.push_back(lstIp.at(l));
			}
			if (readdoc.HasMember("ip_list_v6"))
			{
				ipv6 = readdoc["ip_list_v6"].GetString();
			}

			//HoangDD add new 29/4, for ip_version
			string ip_ver = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			ip_ver = dataManager.getAppConfig().version_android;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			ip_ver = dataManager.getAppConfig().version_ios;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			ip_ver = "ios1452832982";
			//ip_ver = dataManager.getAppConfig().version_win;
#endif
			if (ip_ver != "" && readdoc.HasMember(ip_ver.c_str()))
			{
				gListIpAddress.clear();
				string sip = readdoc[ip_ver.c_str()].GetString();
				vector<string> lstIp = mUtils::splitString(sip, ';');
				//
				mCurrIpIndex = 0;
				for (int l = 0; l < lstIp.size(); l++)
					gListIpAddress.push_back(lstIp.at(l));
			}
			//End HoangDD add new 29/4, for ip_version
			//
			mCurrStepCheckConn = tagConnLevel1;
			if (dataManager.getAppConfig().url_upload != ""){
				uploadData2Server(dataManager.getAppConfig().url_upload);
				//
				mCurrStepCheckConn = tagUploadData1;
				//this->setTimeOutLoading(20.f);
				this->showLoading(20.f);
			}
			else{
				//this->scheduleOnce(schedule_selector(SceneManager::connect2Server), 2);
			}

			//check fakescene instance - only for p1 bv
			//			if (this->getGameVersion() == GameVersion::p1
			//				&& this->getIsBotZone())
			//			{
			//				FakeScene* _fakeScene = FakeScene::getSingletonPtr();
			//				if (_fakeScene != NULL)
			//				{
			//					if (!_fakeScene->getIsShow())
			//					{
			//						this->connect2Server(0);
			//						_fakeScene->setIsShow(true);
			//						return;
			//					}
			//				}
			//			}
            this->getStatusEventFromUrl();
            this->getNotificationFromUrl();
		}
	}
	else if (strcmp(response->getHttpRequest()->getTag(), "iplevel2") == 0){
		this->hideLoading();
		string datas = buffer[0].data();
		datas = decryptBytes(datas);
		unsigned vt1 = datas.find_first_of('{');
		unsigned vt = datas.find_last_of('}');
		if (vt != -1 && vt1 != -1)
			datas = datas.substr(vt1, vt - vt1 + 1);
		datas = mUtils::replaceAll(datas, "\r", "");
		datas = mUtils::replaceAll(datas, "\n", "");
		rapidjson::Document readdoc;
		bool bRet = false;
		ssize_t size = 0;
		std::string load_str = datas;
		readdoc.Parse<0>(load_str.c_str());
		if (readdoc.HasParseError())
		{
			//parse error, load default
			gListIpAddress.clear();
			//
			gListIpAddress = mUtils::splitString(dataManager.getAppConfig().ip_default, ';');
			mCurrIpIndex = 0;
			//
			mCurrStepCheckConn = tagConnLevelDefault;
			//this->scheduleOnce(schedule_selector(SceneManager::connect2Server), 2);
			//MINHDV - fix schedule
			Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::connect2Server), this, 2, false);
			return;
		}
		if (readdoc.IsObject())
		{
            if (readdoc.HasMember("iap_rate"))
            {
                dataManager.getAppConfig().iap_rate = atoi(string(readdoc["iap_rate"].GetString()).c_str());
            }
            if (readdoc.HasMember("url_notification"))
            {
                dataManager.getAppConfig().url_notification = readdoc["url_notification"].GetString();
            }
            if (readdoc.HasMember("url_notification_cam"))
            {
                dataManager.getAppConfig().url_notification_cam = readdoc["url_notification_cam"].GetString();
            }
            if (readdoc.HasMember("callcenter"))
            {
                dataManager.getAppConfig().callcenter = readdoc["callcenter"].GetString();
            }
            if (readdoc.HasMember("callcenter2"))
            {
                dataManager.getAppConfig().callcenter2 = readdoc["callcenter2"].GetString();
            }
            dataManager.getAppConfig().url_upload = "";
            if (readdoc.HasMember("url_upload"))
            {
                dataManager.getAppConfig().url_upload = readdoc["url_upload"].GetString();
            }
            if (readdoc.HasMember("url_avatar_upload"))
            {
                dataManager.getAppConfig().url_avatar_upload = readdoc["url_avatar_upload"].GetString();
            }
            if (readdoc.HasMember("url_giftcode"))
            {
                dataManager.getAppConfig().url_giftcode = readdoc["url_giftcode"].GetString();
            }
            if (readdoc.HasMember("fb_gift"))
            {
                dataManager.getAppConfig().moneyFB = readdoc["fb_gift"].GetString();
            }

            if (readdoc.HasMember("url_event"))
            {
                dataManager.getAppConfig().url_event = readdoc["url_event"].GetString();
            }
            if (readdoc.HasMember("url_event_cam"))
            {
                dataManager.getAppConfig().url_event_cam = readdoc["url_event_cam"].GetString();
            }
            if (readdoc.HasMember("maintenance"))
            {
                dataManager.getAppConfig().maintenancestr = readdoc["maintenance"].GetString();
            }
            if (readdoc.HasMember("url_android_store"))
            {
                dataManager.getAppConfig().url_android_store = readdoc["url_android_store"].GetString();
            }
            if (readdoc.HasMember("url_ios_store"))
            {
                dataManager.getAppConfig().url_ios_store = readdoc["url_ios_store"].GetString();
            }
            if (readdoc.HasMember("url_ios_nostore"))
            {
                dataManager.getAppConfig().url_ios_nostore = readdoc["url_ios_nostore"].GetString();
            }
            if (readdoc.HasMember("force_update"))
            {
                dataManager.getAppConfig().force_update = readdoc["force_update"].GetString();
            }
            if (readdoc.HasMember("url_charge"))
            {
                dataManager.getAppConfig().url_charge = readdoc["url_charge"].GetString();
            }
            if (readdoc.HasMember("lst_sms"))
            {
                dataManager.getAppConfig().strListSMS = readdoc["lst_sms"].GetString();
            }
            if (readdoc.HasMember("new_version"))
            {
                string newVersion = readdoc["new_version"].GetString();
                dataManager.getAppConfig().versionforceupdate = newVersion;
                //                auto lstVersion = mUtils::splitString(newVersion, ';');
                //                if (lstVersion.size() > 1){
                //                    dataManager.getAppConfig().versionIOS = lstVersion[0];
                //                    dataManager.getAppConfig().versionAndroid = lstVersion[1];
                //                }
            }
            if (readdoc.HasMember("hidefb"))
            {
                string hidefb_ = readdoc["hidefb"].GetString();
                dataManager.getAppConfig().hidefb = hidefb_;
                //                auto lstVersion = mUtils::splitString(newVersion, ';');
                //                if (lstVersion.size() > 1){
                //                    dataManager.getAppConfig().versionIOS = lstVersion[0];
                //                    dataManager.getAppConfig().versionAndroid = lstVersion[1];
                //                }
            }
            if (readdoc.HasMember("facebook_appid"))
            {
                string fb_id = readdoc["facebook_appid"].GetString();
                //log("FB ID = %s", fb_id.c_str());
                dataManager.getAppConfig().facebookappid = fb_id;
                //#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                //                Button* btn = SceneLogin::getInstance()->btnLoginFace;
                //                if (btn != NULL)
                //                    btn->setVisible(true);
                //#endif
            }
            if (readdoc.HasMember("linksdt"))
            {
                string link = readdoc["linksdt"].GetString();
                dataManager.getAppConfig().urllinksdt = link;
            }
            
            gListIpAddress.clear();
            if (readdoc.HasMember("ip_list"))
            {
                string sip = readdoc["ip_list"].GetString();
                vector<string> lstIp = mUtils::splitString(sip, ';');
                //
                mCurrIpIndex = 0;
                for (int l = 0; l < lstIp.size(); l++)
                    gListIpAddress.push_back(lstIp.at(l));
            }
            if (readdoc.HasMember("ip_list_v6"))
            {
                ipv6 = readdoc["ip_list_v6"].GetString();
            }
            
            //HoangDD add new 29/4, for ip_version
            string ip_ver = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            ip_ver = dataManager.getAppConfig().version_android;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            ip_ver = dataManager.getAppConfig().version_ios;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            ip_ver = "ios1452832982";
            //ip_ver = dataManager.getAppConfig().version_win;
#endif
            if (ip_ver != "" && readdoc.HasMember(ip_ver.c_str()))
            {
                gListIpAddress.clear();
                string sip = readdoc[ip_ver.c_str()].GetString();
                vector<string> lstIp = mUtils::splitString(sip, ';');
                //
                mCurrIpIndex = 0;
                for (int l = 0; l < lstIp.size(); l++)
                    gListIpAddress.push_back(lstIp.at(l));
            }
            //End HoangDD add new 29/4, for ip_version
            //
            mCurrStepCheckConn = tagConnLevel1;
            if (dataManager.getAppConfig().url_upload != ""){
                uploadData2Server(dataManager.getAppConfig().url_upload);
                //
                mCurrStepCheckConn = tagUploadData1;
                //this->setTimeOutLoading(20.f);
                this->showLoading(20.f);
            }
            else{
                //this->scheduleOnce(schedule_selector(SceneManager::connect2Server), 2);
            }
            
            //check fakescene instance - only for p1 bv
            //            if (this->getGameVersion() == GameVersion::p1
            //                && this->getIsBotZone())
            //            {
            //                FakeScene* _fakeScene = FakeScene::getSingletonPtr();
            //                if (_fakeScene != NULL)
            //                {
            //                    if (!_fakeScene->getIsShow())
            //                    {
            //                        this->connect2Server(0);
            //                        _fakeScene->setIsShow(true);
            //                        return;
            //                    }
            //                }
            //            }
            this->getStatusEventFromUrl();
            this->getNotificationFromUrl();
		}
	}
	else if (strcmp(response->getHttpRequest()->getTag(), "uploadData") == 0){
		string datas = "OK";
		if (buffer->size() > 0)
			datas = buffer[0].data();
		size_t i = datas.find("NOK");
		//for level1 only
		if (mCurrStepCheckConn == tagUploadData1){
			mCurrStepCheckConn = tagConnLevel1;
			connect2Server(0);
			return;
		}
		//
		if (i != string::npos){
			showChatWithCurrentScene(dataManager.GetSysString(49), -1);
			gListIpAddress.clear();
			mCurrStepCheckConn = tagConnLevel1;
			this->hideLoading();
			return;
		}
		mCurrStepCheckConn = tagConnLevel2;
		connect2Server(0);
	}
	//MINHDV - get notification on scene pickroom
	else if (strcmp(response->getHttpRequest()->getTag(), "notificationPR") == 0){
		long statusCode = response->getResponseCode();
		char statusString[64] = {};

		std::vector<char>* buffer = response->getResponseData();
		//log("datas receive from [%s]: %s", response->getHttpRequest()->getTag(), buffer[0].data());
		string datas = buffer[0].data();
		//  datas = decryptBytes(datas);
		unsigned vt1 = datas.find_first_of('{');
		unsigned vt = datas.find_last_of('}');
		if (vt != -1 && vt1 != -1)
			datas = datas.substr(vt1, vt - vt1 + 1);
		datas = mUtils::replaceAll(datas, "\r", "");
		datas = mUtils::replaceAll(datas, "\n", "");
		//
		rapidjson::Document readdoc;
		bool bRet = false;
		ssize_t size = 0;
		std::string load_str = datas;
		readdoc.Parse<0>(load_str.c_str());
		////log(datas.c_str());
		if (readdoc.HasParseError())
		{
			return;
		}
		if (readdoc.IsObject())
		{
			string textShow = "";
			string linkShow = "";
			string textPop = "";
			string linkPop = "";
			bool isShow = false;
			gListNotificationPickRoom.clear();
			if (readdoc.HasMember("text"))
			{
				textShow = readdoc["text"].GetString();
			}
			if (readdoc.HasMember("link"))
			{
				linkShow = readdoc["link"].GetString();
			}
			NotificationInfo notifiInfo;
			notifiInfo.textShow = textShow;
			notifiInfo.linkShow = linkShow;
			notifiInfo.isShow = isShow;
			gListNotificationPickRoom.push_back(notifiInfo);

			// 			ScenePickRoom *curScene = dynamic_cast<ScenePickRoom*>(Director::getInstance()->getRunningScene());
			// 			if (curScene)
			// 			{
			// 				curScene->setGListNotification(gListNotificationPickRoom);
			// 			}
		}

	}
	else if (strcmp(response->getHttpRequest()->getTag(), "versionios") == 0){

		long statusCode = response->getResponseCode();
		char statusString[64] = {};
		std::vector<char> *buffer = response->getResponseData();
		//log("LayerPopupRecharge::onHttpRequestCompleted - datas receive from http: %s", buffer[0].data());
		// dump data
		if (buffer->size() <= 0)
			return;
		string _s = string(buffer[0].data());
		int pos = _s.find_first_of("!");
		string _res = _s.substr(0, pos);
		this->versiontohide = _res;
	}
	else if (strcmp(response->getHttpRequest()->getTag(), "urlAvatar") == 0){

		long statusCode = response->getResponseCode();
		//log("statusCode = %ld", statusCode);

		if (response->isSucceed()){
			std::vector<char> *buffer = response->getResponseData();
			string datas = buffer[0].data();

			unsigned vt = datas.find_last_of(']');
			if (vt != -1)
				datas = datas.substr(0, vt + 1);

			rapidjson::Document readdoc;
			std::string load_str = datas;
			readdoc.Parse<0>(load_str.c_str());

			if (readdoc.HasParseError())
				return;

			if (readdoc.IsArray())
			{
				int a = 8;
				if (readdoc.Size() < 8){
					a = readdoc.Size();
				}
				this->arrLink.clear();
				for (int i = 0; i < a; i++) {
					rapidjson::Value& arraydoc = readdoc[i];

					if (arraydoc.HasMember("url"))
					{
						//log("url = %s", arraydoc["url"].GetString());
						string _surl = string(arraydoc["url"].GetString());
						this->arrLink.push_back(_surl);
						//this->arrLink.push_back("http://img.zing.vn/volam2/images/danhsachbaiviet/dactrung/kethon/damcuoi2.JPG");
					}
				}
			}
			else
			{
				this->arrLink.push_back("https://gamebai.net/avatar/tonngokhong_169.png");
				this->arrLink.push_back("https://gamebai.net/avatar/anhnt567_19.jpg");
				this->arrLink.push_back("https://gamebai.net/avatar/khanhgb89_193.png");
				this->arrLink.push_back("https://gamebai.net/avatar/policelove_107.png");
				this->arrLink.push_back("https://gamebai.net/avatar/atua75_10.png");
				this->arrLink.push_back("https://gamebai.net/avatar/akayvn_150.jpg");
				this->arrLink.push_back("https://gamebai.net/avatar/khanhgb89_185.png");
				this->arrLink.push_back("https://gamebai.net/avatar/khanhgb89_183.png");
			}
		}
	}
	else if (strcmp(response->getHttpRequest()->getTag(), "GetButtonNoti") == 0){
		std::vector<char>* buffer = response->getResponseData();
		std::string res;
		res.insert(res.begin(), buffer->begin(), buffer->end());
		rapidjson::Document d;
		d.Parse<0>(res.c_str());
		//log("i am here");
		if (d.IsArray()){
			//log("here children");
			rapidjson::SizeType size = d.Size();
			lstbtnNoti.clear();
			/*auto orgX = (1920 - (285 * size) - 20 * (size - 1)) / 2;*/
			for (rapidjson::SizeType i = 0; i < size; ++i){
				event e;
				e.name = d[i]["name"].GetString();
				e.link = d[i]["link"].GetString();
				lstbtnNoti.push_back(e);
			}
		}
	}
	else if (strcmp(response->getHttpRequest()->getTag(), "url_charge") == 0){
		std::vector<char>* buffer = response->getResponseData();
		std::string res;
		res.insert(res.begin(), buffer->begin(), buffer->end());
		rapidjson::Document d;
		d.Parse<0>(res.c_str());
		//log("i am here");
		if (d.IsArray()){
			//log("here children");
			rapidjson::SizeType size = d.Size();
			lstTabRecharge.clear();
			event e;
			e.name = "IAP";
			e.link = "";
			lstTabRecharge.push_back(e);
			/*auto orgX = (1920 - (285 * size) - 20 * (size - 1)) / 2;*/
			for (rapidjson::SizeType i = 0; i < size; ++i){
				event e;
				e.name = d[i]["name"].GetString();
				e.link = d[i]["link"].GetString();
				lstTabRecharge.push_back(e);
			}
		}
	}
	else if (strcmp(response->getHttpRequest()->getTag(), "filter_ip") == 0)
	{
		//		std::vector<char>* buffer = response->getResponseData();
		//		std::string res;
		//		res.insert(res.begin(), buffer->begin(), buffer->end());
		//		if (res.find("true") != std::string::npos) //contain string '-'
		//		{
		//			this->initFirst();
		//			this->gotoLogin();
		//			// 			this->setIsBotZone(true);
		//			// 			Scene* fakeScene = FakeScene::scene();
		//			// 			Director::getInstance()->replaceScene(fakeScene);
		//		}
		//		else{
		//			this->setIsBotZone(true);
		//			Scene* fakeScene = FakeScene::scene();
		//			Director::getInstance()->replaceScene(fakeScene);
		//		}
	}
}

void SceneManager::getPromotionFromUrl()
{
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(this->getUrlPromotion().c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("promotion");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
}

void SceneManager::OnSmartFoxBuddyOnlineStateUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	changeBuddyGameState("");
}

void SceneManager::reloadChargeConfig()
{
	//check and receive card & sms config
	UserDefault *def = UserDefault::getInstance();
	string dateConf = def->getStringForKey("dateConfig", "");
	if (dateConf.compare("") != 0){
		string currDate = mUtils::getCurrentDateTime("%d/%m/%Y");
		if (dateConf.compare(currDate) == 0)
			return;
	}
	//Send get list
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	//Send request
	boost::shared_ptr<IRequest> request(new ExtensionRequest("liCARDInfor", NULL));//LIST_CARD_INFOR = "liCARDInfor";
	GameServer::getSingleton().Send(request);
	boost::shared_ptr<IRequest> request1(new ExtensionRequest("liSMSInfor", NULL));//LIST_SMS_INFOR = "liSMSInfor";
	GameServer::getSingleton().Send(request1);
}

void SceneManager::SetKieuBanChan(const int& kieuban){
	this->pKieuBanChan = kieuban;
}

const int& SceneManager::GetKieuBanChan() const{
	return this->pKieuBanChan;
}

void SceneManager::getNotificationFromUrl()
{
    gListNotification.clear();
	if (dataManager.getAppConfig().url_notification == "")
		return;
    
	string link = "";
    if (this->currLang == 0){
        link = dataManager.getAppConfig().url_notification;
    }else if(this->currLang == 2){
        link = dataManager.getAppConfig().url_notification_cam;
    }
    else if(this->currLang == 1){
        link = dataManager.getAppConfig().url_notification_eng;
    }
    else if(this->currLang == 4){
        link = dataManager.getAppConfig().url_notification_cn;
    }
    
	//    if (strcmp(dataManager.getAppConfig().loginType.c_str(), "facebook") == 0)
	//        link = dataManager.getAppConfig().url_notification + "?new" + "&fb=1";
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(link.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("notification");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
}
void SceneManager::onScheduleGetHuTHuong(float dt){
	// string link = dataManager.getAppConfig().url_notification + "?new";
	//    if (strcmp(dataManager.getAppConfig().loginType.c_str(), "facebook") == 0)
	//        link = dataManager.getAppConfig().url_notification + "?new" + "&fb=1";
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl("https://sv.nagaclub.net/omap/huthuongservice");
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("huthuong");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
}

void SceneManager::getStatusEventFromUrl(){
	//if (dataManager.getAppConfig().url_event == "")
	//	return;
    UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
    UserDefault::getInstance()->setIntegerForKey("numEvent", 0);
	string link = "";
    if (this->currLang == 0){
        link = dataManager.getAppConfig().url_event;
    }else if (this->currLang == 2){
        link = dataManager.getAppConfig().url_event_cam;

    }
    else if (this->currLang == 1){
        link = dataManager.getAppConfig().url_event_eng;
        
    }
    else if(this->currLang == 4){
        link = dataManager.getAppConfig().url_event_cn;
    }
    this->lstEvent.clear();
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(link.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("event");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
}
void SceneManager::getInfoShareFromUrl(){
	string link = "https://baimoc.com/config_share";
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(link.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("shareinfo");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
}
void SceneManager::getbtnRechargeFromUrl(){
	if (dataManager.getAppConfig().url_charge == "")
		return;
	string link = dataManager.getAppConfig().url_charge + "&v=ios114";
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(link.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("url_charge");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();

}
void SceneManager::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	// Get the message parameter of the event
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> ptrNotifiedMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);

	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return;

	LayerPopupThongBao* _layer = LayerPopupThongBao::create();
	_layer->showAButton();
	_layer->setMessage(dataManager.GetSysString(141));
	_layer->setContentMess(*ptrNotifiedMessage);
	_currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_ADMIN,9121);
}

void SceneManager::uploadData2Server(string url)
{
	UserDefault *def = UserDefault::getInstance();
	string sDatas = def->getStringForKey("lstUserInfo", "");

	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setTag("uploadData");
	// write the post data

	string postData = "data=" + sDatas;
	//log("data: %s", postData.c_str());
	request->setRequestData(postData.c_str(), postData.length());
	//
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
}

void SceneManager::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	if (ptrEventParams == NULL)
		return;
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	if (ptrEventParamValueUser == NULL) return;
	boost::shared_ptr<User> m_Player = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	if (m_Player == NULL)
		return;
	if (m_Player->IsItMe()){
		double amf = GameUtils::getAmfGoldUser();
		double am = GameUtils::getAmUser();
		string _strName = GameUtils::getMyName();
		int _level = GameUtils::getLevelUser();
		int _exp = GameUtils::getExpUser();
		int accVipType = GameUtils::getTypeVipUser();
		try {
			/*moc3-change aN=>adn*/
			UserDefault *def = UserDefault::getInstance();
			def->setStringForKey("amf", StringUtils::format("%lf", amf));
			def->setStringForKey("am", StringUtils::format("%lf", am));
			def->setStringForKey("adn", _strName);
			def->setStringForKey("lvl", StringUtils::format("%d", _level));
			def->setStringForKey("exp", StringUtils::format("%d", _exp));
			def->setStringForKey("accVipType", StringUtils::format("%d", accVipType));
			def->flush();

		}
		catch (...) {
			//log("");
		}
	}
}

void SceneManager::OnSmartFoxLogin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	if (mCurrStepCheckConn == tagConnLevel1)
		mCurrStepCheckConn = tagLoginedWithLevel1;
	else if (mCurrStepCheckConn == tagConnLevel2)
		mCurrStepCheckConn = tagLoginedWithLevel2;
}

void SceneManager::OnSmartFoxLoginError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	if (mCurrStepCheckConn == tagConnLevel1)
		mCurrStepCheckConn = tagLoginedWithLevel1;
	else if (mCurrStepCheckConn == tagConnLevel2)
		mCurrStepCheckConn = tagLoginedWithLevel2;
}

void SceneManager::checkIpLevel2(string url)
{
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("iplevel2");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
	this->setLoadingString(dataManager.GetSysString(211));
	//this->setTimeOutLoading(20.f);
	this->showLoading(5);
}

void SceneManager::nextStepConn()
{
	if (mCurrIpIndex < gListIpAddress.size()){//retry to other ips
		connect2Server(0);
		hideLoading();
		return;
	}
	//
	if (mCurrStepCheckConn == tagConnLevel1){
		gListIpAddress.clear();
		//retry connect 2 level2
		checkIpLevel2(IP_DEFAULT);
		//
		mCurrStepCheckConn = tagGetLevel2Info;
	}
	else{
		showChatWithCurrentScene(dataManager.GetSysString(49), -1);
	}
}

void SceneManager::showChatWithCurrentScene(string msg, int _id)
{
	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return;
	Chat *toast = Chat::create(msg, _id);
	_currScene->addChild(toast, ZORDER_LIST::ZORDER_TOAST,9122);
}

vector<strGameCF> SceneManager::getConfigFromSever(string _stringcfg)
{
	vector<strGameCF> arr_config;
	vector<string> arr = mUtils::splitString(_stringcfg, '|');
	for (int i = 0; i < arr.size(); i++)
	{
		vector<string> arr_info = mUtils::splitString(arr.at(i), '-');
		int _id = atoi(arr_info.at(0).c_str());
		string _list = arr_info.at(1);
		int _n_hide = atoi(arr_info.at(2).c_str());
		int _ratio = atoi(arr_info.at(3).c_str());
		strGameCF gcf = strGameCF(_id, _list, _n_hide, _ratio);
		arr_config.push_back(gcf);
	}
	return arr_config;
}

void SceneManager::setGameConfig(string _config)
{
	strGameConfig = _config;
	arrConfig = getConfigFromSever(strGameConfig);
}

std::string SceneManager::getGameConfig()
{
	return strGameConfig;
}

int SceneManager::getLoginBonus(){

	auto lstBonus = mUtils::splitString(dataManager.getAppConfig().moneyFB, '-');
	if (lstBonus.size() < 1)
		return 300;
	return std::atoi(lstBonus[1].c_str());
}

int SceneManager::getLoginBonusNotFB(){

	auto lstBonus = mUtils::splitString(dataManager.getAppConfig().moneyFB, '-');
	if (lstBonus.size() == 0)
		return 300;
	return std::atoi(lstBonus[0].c_str());

}
int SceneManager::getFacebookBonus(){
	auto lstBonus = mUtils::splitString(dataManager.getAppConfig().moneyFB, '-');
	if (lstBonus.size() <= 2)
		return 700;
	return std::atoi(lstBonus[2].c_str());
}
int SceneManager::getMinVipTranfer(){
	auto lstBonus = mUtils::splitString(dataManager.getAppConfig().strMinTranfer, '_');
	if (lstBonus.size() == 0)
		return 3;
	return std::atoi(lstBonus[0].c_str());
}
int SceneManager::getMinMoneyTranfer(){
	auto lstBonus = mUtils::splitString(dataManager.getAppConfig().strMinTranfer, '_');
	if (lstBonus.size() < 1)
		return 3;
	return std::atoi(lstBonus[1].c_str());
}
int SceneManager::getMinMoney(int id){
	//1: so tien toi thieu the cao
	//2: so tien toi thieu vat pham
	//3: so tien toi thieu chuyen tien
	//4: level toi thieu khi doi rhuong
	auto lstBonus = mUtils::splitString(dataManager.getAppConfig().minjoin, ';');
	for (int i = 0; i<lstBonus.size(); i++){
		auto lstmin = mUtils::splitString(lstBonus[i], '_');
		if (lstmin.size()>1){
			if (atoi(lstmin[0].c_str()) == id)
				return atoi(lstmin[1].c_str());
		}
		else{
			return 10000;
		}
	}
	return 10000;
}

void SceneManager::getNotificationPickRoomFromUrl()
{
	if (dataManager.getAppConfig().url_notification == "")
		return;
	int gameid = getGameID();
	string url_notif = dataManager.getAppConfig().url_notification;
	string urlNotifcation = StringUtils::format("%s?gameid=%d", url_notif.c_str(), gameid);
	if (url_notif.find('new') != std::string::npos) //contain string '-'
	{
		//		if (this->getGameVersion() == GameVersion::p1){
		//			urlNotifcation = StringUtils::format("%sgameid=%d", NOTIFICATION_SERVICES, gameid);
		//		}
		//		else{
		urlNotifcation = StringUtils::format("%sgameid=%d", NOTIFICATION_P2_SERVICES, gameid);
		//		}
	}

	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(urlNotifcation.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("notificationPR");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
}
string SceneManager::getCoinUnit(){

	if (getCurrRoomType() == 0){
		return "Zen";
	}
	else{
		return "Gold";
	}
}
void SceneManager::getVersionFromUrl(){
	//	this->versiontohide = "1.16";
	//	HttpRequest* request = new (std::nothrow) HttpRequest();
	//	if (this->getGameVersion() == GameVersion::p2)
	//	{
	//#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//		request->setUrl("https://baimoc.com/aversion?ver=a10");
	//#endif
	//#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//		request->setUrl("https://baimoc.com/iosversion?ver=ios10");
	//#endif
	//	}
	//	else{
	//#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//		request->setUrl("https://gamebai.net/version?ver=a102");
	//#endif
	//#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//		request->setUrl("https://gamebai.net/version?ver=ios112");
	//#endif
	//	}
	//	request->setRequestType(HttpRequest::Type::GET);
	//	request->setTag("versionios");
	//	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	//	HttpClient::getInstance()->send(request);
	//	request->release();
}
void SceneManager::getUrlAvatar() {
	HttpRequest* request = new (std::nothrow) HttpRequest();
	string _url = "";
	//if (this->getGameVersion() == GameVersion::p2)
		_url = "https://nagaclub.net/GetAllAvatarLink";
	request->setUrl(_url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("urlAvatar");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();

}
void SceneManager::getButtonNotificationFromUrl(){
	// headers.push_back("Content-Type: application/json; charset=utf-8");
	HttpRequest* request = new (std::nothrow) HttpRequest();
	std::string url = "";
	if (this->getGameVersion() == GameVersion::p2){
		url = NOTIFICATION_P2_LINK;
	}
	else{
		url = NOTIFICATION_LINK;
	}
	request->setUrl(url.c_str());

	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	request->setTag("GetButtonNoti");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	this->showLoading();

}

void SceneManager::onScheduleGetConfig(float dt)
{
	//Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneManager::onScheduleGetConfig));
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneManager::onScheduleGetConfig), this);
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(dataManager.getAppConfig().url_list_ip.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("iplevel1");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
	this->setLoadingString(dataManager.GetSysString(211));
	//this->setTimeOutLoading(20.f);
	//this->showLoading(5);
}
void SceneManager::showPopupFailed(int tag){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    SceneManager::getSingleton().hideLoading();
    _currScene->removeChildByTag(24);
    LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
    layerPopup->setCallBackFromButton(this, callfuncO_selector(SceneManager::layerPopupThongBaoCallBack2), tag);
    layerPopup->setMessage(dataManager.GetSysString(141));
    layerPopup->setContentMess(dataManager.GetSysString(623));
    layerPopup->setPopupClose();
    layerPopup->setTag(24);
    _currScene->addChild(layerPopup,5000);
    SceneManager::getSingleton().hideLoading();
}
void SceneManager::layerPopupThongBaoCallBack2(Ref* pSender){
    int tag = ((Node*)pSender)->getTag();
    if (tag == 2){
        this->connect2Server(1);
    }else if (tag == 3){
        this->onScheduleGetConfig(1);
    }
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    _currScene->removeChildByTag(24);
}
void SceneManager::connect2Server(float dt)
{
	RejoinInfoStruct rejoinInfo;
	this->setRejoinInfo(rejoinInfo);
	//
	//GameServer::getSingleton().connectToServer("103.24.244.167", "9933");
	// 	this->setTimeOutLoading(10.0f);
	// 	this->setLoadingTag(123); //123 = do login
	// 	this->showLoading();
	bool isHasInternet = true;
#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	isHasInternet = NativeHelper::hasConnection();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	isHasInternet = IOS::tryIsInternetConnection();
#endif

	if (!isHasInternet)
	{
		this->hideLoading();
        this->showPopupFailed(2);
        this->unschedule(schedule_selector(SceneManager::connect2Server));
		return;
	}
	if (gListIpAddress.size() == 0)
	{
		//gFileDownloaderProcess->downLoadFile(this, callfuncND_selector(SceneManager::onFileDownLoaded), dataManager.getAppConfig().url_list_ip, "ip.txt");
		HttpRequest* request = new (std::nothrow) HttpRequest();
		request->setUrl(dataManager.getAppConfig().url_list_ip.c_str());
		request->setRequestType(HttpRequest::Type::GET);
		request->setTag("iplevel1");
		request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
		HttpClient::getInstance()->send(request);
		request->release();
		this->setLoadingString(dataManager.GetSysString(211));
		//this->setTimeOutLoading(20.f);
		this->showLoading(5);
		return;
	}
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneManager::connect2Server), this);
//    if (mCurrIpIndex >= gListIpAddress.size()) //get last ip
//    {
//        string sip = gListIpAddress.back();
//        auto lstip = mUtils::splitString(sip, '@');
//        if (lstip.size() == 1) {
//            GameServer::getSingleton().connectToServer(lstip[0].c_str(), "9933");
//            this->setMIP(lstip[0]);
//        }
//        else if (lstip.size() == 2){
//            GameServer::getSingleton().connectToServer(lstip[0].c_str(), lstip[1].c_str());
//            this->setMIP(lstip[0]);
//        }
//        this->setMIP(lstip[0]);
//        return;
//    }

	//    bool isV4 = true;
	//#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//    string iptype = IOS::tryToGetIPAddress();
	//    isV4 = (iptype == "v4") ? true : false;
	//#endif
	//MINHDV - fix schedule
    string sip = gListIpAddress.at(0);
   // mCurrIpIndex++;
   // string sip = "";
	//    if(!isV4){
	//        sip = "bv6.baivip.net";
	//        if(ipv6.compare("") != 0)
	//            sip = ipv6;
	//    }
	//connect
	//sip = "bv6.baivip.net";
	//sip = "188.166.250.168";
	//sip = "128.199.124.113";
	//sip = "125.212.225.173";
	//sip = "128.199.124.113";
	//sip = "115.146.127.177@9944";
	//sip = "124.158.6.196";
	//sip = "210.245.8.31";
	//sip = "210.245.8.39";
	//sip = "210.245.8.39";
	//sip = "210.245.8.82@9944";
	//sip = "sv.baimoc.com";
    //anhnt only for test ip
    //sip = "sv.nagaclub.net";
    sip = "203.162.166.162@9922";
    //sip = "13.251.217.47@9922";
    //sip = "2406:da18:400:4a00:25ac:a05e:e9ba:ee6f";
    //sip = "bv.baivip.net";
	//	if (getGameVersion() == GameVersion::p2){
	auto lstip = mUtils::splitString(sip, '@');
	if (lstip.size() == 1) {
		GameServer::getSingleton().connectToServer(lstip[0].c_str(), "9933");
		this->setMIP(lstip[0]);
	}
	else if (lstip.size() == 2){
		GameServer::getSingleton().connectToServer(lstip[0].c_str(), lstip[1].c_str());
		this->setMIP(lstip[0]);
	}
	//	}
	//	else{
	//		GameServer::getSingleton().connectToServer(sip.c_str(), "9933");
	//		this->setMIP(sip);
	//	}
	// GameServer::getSingleton().connectToServer("2400:6180:0:d0::9d8:d001", "9933");
	//only for test
	//GameServer::getSingleton().connectToServer(sip.c_str(), "9944");

}
void SceneManager::disconnectFromServerDelay(float dt){

	this->disconnectFromServer();
	//MINHDV - fix schedule
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneManager::disconnectFromServerDelay), this);
}
void SceneManager::updateEvent(float dt)
{
	GameServer::getSingleton().updateEvent();
}

void SceneManager::updateEvent4CheckConnection(float dt)
{
	GameServer::getSingleton().updateEvent4CheckConnection();
}
// void SceneManager::setTimeOutLoading(float dt)
// {
// 	auto _currScene = Director::getInstance()->getRunningScene();
// 	if (!_currScene) return;
// 	CCProgressLayer* _LayerLoading = (CCProgressLayer*)_currScene->getChildByTag(tag_SceneLoading);
// 	if (_LayerLoading)
// 		_LayerLoading->setTimeOut(dt);
// }

void SceneManager::filterIP()
{
	Scene* scene = Scene::create();
	Director::getInstance()->runWithScene(scene);
	LayerLoading* loading = LayerLoading::create();

	scene->addChild(loading,2000);
	loading->start(20);

	//create request ip
	string link = StringUtils::format("http://location.gamebai.net:8889/api?cmd=getcountry&version=%s", dataManager.getAppConfig().version_win.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	link = StringUtils::format("http://location.gamebai.net:8889/api?cmd=getcountry&version=%s", dataManager.getAppConfig().version_android.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	link = StringUtils::format("http://location.gamebai.net:8889/api?cmd=getcountry&version=%s", dataManager.getAppConfig().version_ios.c_str());
#endif
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(link.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("filter_ip");
	request->setResponseCallback(CC_CALLBACK_2(SceneManager::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
}

void SceneManager::pushInviteableUsers(FriendInfo info)
{
	_inviteableUsers.push_back(info);
}

void SceneManager::pushInviteUserIdxs(string uid)
{
	_inviteUserIdxs.push_back(uid);
}
void SceneManager::clearInviteUserIdx()
{
	_inviteUserIdxs.clear();
}
void SceneManager::earseInviteUserIdxs(int idx)
{
	_inviteUserIdxs.erase(this->_inviteUserIdxs.begin() + idx);
}
void SceneManager::setInviteInviteableUsers(int idx, bool isInv){
	_inviteableUsers[idx].isInvite = isInv;
}
void SceneManager::clearInviteableUsers(){
	_inviteableUsers.clear();
}
void SceneManager::earseInviteableUsers(int idx){
	_inviteableUsers.erase(_inviteableUsers.begin() + idx);
}
void SceneManager::showToast(string _msg)
{
	LayerToast* _toast = LayerToast::create(_msg);
	Scene* _currScene = Director::getInstance()->getRunningScene();
	if (_currScene != NULL)
	{
		_toast->showToast(3.0f);
		_currScene->addChild(_toast, ZORDER_LIST::ZORDER_TOAST, 9123);
	}
}

void SceneManager::showToast(string _msg, float _duration)
{
	LayerToast* _toast = LayerToast::create(_msg);
	Scene* _currScene = Director::getInstance()->getRunningScene();
	if (_currScene != NULL)
	{
		_toast->showToast(_duration);
		_currScene->addChild(_toast, ZORDER_LIST::ZORDER_TOAST,9124);
	}
}

void SceneManager::showPopupMoneyCharge()
{
	Scene* currScene = Director::getInstance()->getRunningScene();
	if (currScene != NULL)
	{
		if (this->getCurrRoomType() == 0){
			LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
			popup->onBtnNapThe(NULL);
			currScene->addChild(popup, 2000, 9125);
		}
		else{
			LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
			popup->onBtnNapHao(NULL);
			currScene->addChild(popup, 2000, 9126);
		}

	}
}

void SceneManager::showPopupNotify(string messageText)
{
	Scene* currScene = Director::getInstance()->getRunningScene();
	if (currScene != NULL)
	{
		LayerPopupAlert* layerPopup = dynamic_cast<LayerPopupAlert*>(currScene->getChildByTag(127));
		if (layerPopup == NULL)
		{
			layerPopup = LayerPopupAlert::create();
			currScene->addChild(layerPopup, ZORDER_LIST::ZORDER_POPUP_INGAME, 127);
		}
		layerPopup->showOnlyButtonAccept();
		layerPopup->setContentMessage(dataManager.GetSysString(141), messageText);
	}
}

void SceneManager::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	boost::shared_ptr<Room> room = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	//
	boost::shared_ptr<RoomVariable> typePtr = room->GetVariable(EXT_FIELD_ROOM_TYPE);
	if (typePtr)
	{
		int typeRoom = *typePtr->GetIntValue();
		this->setCurrRoomType(typeRoom);
	}
}

void SceneManager::clearListBet()
{
	this->listbet.clear();
}
string SceneManager::getCoin(int type){
	if (type == 0) {
		return "Zen";
	}
	else{
		return "Gold";
	}
}
double SceneManager::getMoneyByType(int type){
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL)/// cho disconnect luon
	{
		this->showChatWithCurrentScene(dataManager.GetSysString(602), -1);
		//this->scheduleOnce(schedule_selector(SceneManager::disconnectFromServerDelay), 1);
		//MINHDV - fix schedule
		Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneManager::disconnectFromServerDelay), this, 1.0, false);
		return 0;
	}
	auto amf = myself->GetVariable("amf");
	auto amfs = myself->GetVariable("amfs");

	boost::shared_ptr<UserVariable> amfptr;
	if (type == 0){
		amfptr = amf;
	}
	else{
		amfptr = amfs;

	}
	if (amfptr == NULL)
		return 0;
	boost::shared_ptr<double> amfvalue = amfptr->GetDoubleValue();
	if (amfvalue == NULL)
		return 0;
	return *amfvalue;
}
int SceneManager::getCapVip(){
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf == NULL) return 0;
	if (mySelf->GetVariable("accVipType") == NULL) return 0;
	auto am = mySelf->GetVariable("accVipType");
	if (am == NULL) return 0;
	return *am->GetIntValue();
}
double SceneManager::getSoTienNap(){
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf == NULL) return 0;
	if (mySelf->GetVariable("am") == NULL) return 0;
	auto am = mySelf->GetVariable("am");
	if (am == NULL) return 0;
	return *am->GetDoubleValue();
}
void SceneManager::nhanthuongLevel(){
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_RECEIVE_AWARD_SILVER_DAILY_REQ, params));
	GameServer::getSingleton().Send(request);
}

void SceneManager::OnSmartFoxInvitation(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueInvitation = (*ptrEvetnParams)["invitation"];
	boost::shared_ptr<Invitation::Invitation> invitation = ((boost::static_pointer_cast<Invitation::Invitation>(ptrEventParamValueInvitation)));
	// 	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = invitation->Params();
	boost::shared_ptr<User> MySelf = GameServer::getSingleton().getSmartFox()->MySelf();

	//std::string _displayName = *param->GetUtfString("adn");
	//Get PAI
	string pass = "";
	boost::shared_ptr<Room> lastjoinRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastjoinRoom != NULL)
	{
		if (lastjoinRoom->GroupId()->compare(SERVICE_CHAT_ROOM) != 0)
			return;
	}
	if (param->GetUtfString("pass") != NULL)
		pass = *param->GetUtfString("pass");
	//
	//	//log("Invitee: %s Inviter: %s, gameID: %s, minbet: %s, pass: %s, roomType: %d",
	//		invitation->Invitee()->Name()->c_str(),
	//		invitation->Inviter()->Name()->c_str(),
	//		param->GetUtfString("gid")->c_str(),
	//		param->GetUtfString("mb")->c_str(),
	//		pass.c_str(),
	//		param->GetInt("roomType"));
	double _amf = 0;
	boost::shared_ptr<UserVariable> amfPtr = invitation->Inviter()->GetVariable("amf");
	std::string minbet = *param->GetUtfString("mb");
	int _roomType = *param->GetInt("roomType");
	if (getCurrRoomType() == 0){
		amfPtr = invitation->Inviter()->GetVariable("amf");
	}
	else {
		amfPtr = invitation->Inviter()->GetVariable("amfs");
	}
	if (amfPtr)
	{
		_amf = *amfPtr->GetDoubleValue();
	}

	if (MySelf->GetVariable("pai")->GetIntValue() != 0)
	{

		////log("***** DEBUG 1");
		int mRoomID4Invite = atoi(param->GetUtfString("roomid")->c_str());
		int _gameId = atoi(param->GetUtfString("gid")->c_str());
        string roomName = "";
        
        if (param->GetUtfString("roomName")){
            auto name = param->GetUtfString("roomName");
           // //log("room :%s",name->c_str());
            roomName = name->c_str();
        }
		////log("***** DEBUG 2");
		int _gameID = atoi(param->GetUtfString("gid")->substr(0, 3).c_str());
		LayerPopupShowInvite* popupInvite = LayerPopupShowInvite::create();
        popupInvite->setTag(249);
		boost::shared_ptr<UserVariable> userPtr = invitation->Inviter()->GetVariable(ACC_DISPLAY_NAME);
		string userInvite = invitation->Inviter()->Name()->c_str();
		if (userPtr != NULL)
			userInvite = userPtr->GetStringValue()->c_str();
		popupInvite->processSetDatasToDisplay(userInvite, pass, atoi(minbet.c_str()), mRoomID4Invite, _gameId, _roomType,roomName);
		//popupInvite->processSetDatasToDisplay(_displayName, pass, atoi(minbet.c_str()), mRoomID4Invite, _gameId, _roomType);
		//update 10/7

		bool isInvite = UserDefault::getInstance()->getBoolForKey("denyAllInvite", true);
		if (isInvite)
		{
			auto currScene = Director::getInstance()->getRunningScene();
			if (currScene)
			{
                if (this->getChildByTag(128)) return;
				//SceneMain* instance = SceneMain::getSingletonPtr();
				/*if (instance != NULL)
					instance->removeChildByTag(LAYER_POPUP_ID::POPUP_AUTO_INVITE);*/
				auto layerInvite = currScene->getChildByTag(128);
				if (layerInvite == NULL){
					currScene->addChild(popupInvite, 2000, 9128);
				}
			}
		}
	}
	//log("***** DEBUG 4");
}
string SceneManager::getFolderLang(){
    switch(currLang){
        case 0:
            return "VN/";
        case 2:
            return "CAM/";
        case 1:
            return "ENG/";
        case 4:
            return "CN/";
    }
 }
string SceneManager::getFolderLangWeb(){
    switch(currLang){
        case 0:
            return "ingame_vi/";
        case 2:
            return "ingame_km/";
        case 1:
            return "ingame_en/";
        case 4:
            return "ingame_cn/";
    }
}
void SceneManager::showPopupNapTien(string _msg){
    Scene* currScene = Director::getInstance()->getRunningScene();
    if (currScene != NULL)
    {
        LayerPopupNapTienAlert* layerPopup = LayerPopupNapTienAlert::create();
        currScene->addChild(layerPopup, 2000, 9129);
        layerPopup->setText(_msg);
        
    }
}
void SceneManager::showPopupNapTienGold(string _msg){
    Scene* currScene = Director::getInstance()->getRunningScene();
    if (currScene != NULL)
    {
        LayerPopupNapTienAlert* layerPopup = LayerPopupNapTienAlert::create();
        currScene->addChild(layerPopup, 2000+10,9130);
        layerPopup->setText(_msg);
        layerPopup->setType(1);
    }
}
void SceneManager::showPopupVongQuay(){
    if (this->isNagaWin) {
        SceneMain::getSingleton().gotoDiemDanh();
        return;
    }
    this->setIsShowVQ(false);
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    if (mySelf)
    {
        boost::shared_ptr<UserVariable> rotatebonus = mySelf->GetVariable("rotatebonus");
        if (rotatebonus)
        {
            int rotatebonus_value = *rotatebonus->GetIntValue();
            if (rotatebonus_value <= 0){
                this->setIsShowVQBonus(true);
                boost::shared_ptr<ISFSObject> params(new SFSObject());
                params->PutUtfString("aI", this->getMyName());
                boost::shared_ptr<IRequest> request(new ExtensionRequest("rgaci", params));
                GameServer::getSingleton().Send(request);
                return;
            }
        }
    }
    Scene* currScene = Director::getInstance()->getRunningScene();
    if (currScene != NULL)
    {
        this->setIsShowVQBonus(false);
        this->setIsShowVQ(true);
        LayerPlayVQMM* pop = LayerPlayVQMM::create();
        currScene->addChild(pop, 2000, 9131);
    }
}
void SceneManager::onScheduleFakeHuThuongTaiXiu(float dt){
    static int count = 0;
    static int count2 = 0;
    int random3 = rand() %30 + 3;
    static int step = 37+random3;
    this->huTaiXiu+=step;
    count+=37;
    int random2 = rand() %340 + 1533;
    int random4 = rand() %4+1;
    if (count>443+random2*random4){
        count2+=34;
        step= 0;
       //step = 37+random3;
        if (count2>244+random2*random4){
            step = 37;
            count2 = 0 ;
            count = 0;

        }

    }
    if (this->huTaiXiu > this->currHuTaiXiu){
        int random = rand() %331230 + 111133;
        this->huTaiXiu = this->currHuTaiXiu/3+random;

    }
}
void SceneManager::onScheduleFakeHuThuongPoker(float dt){
    static int count = 0;
    static int count2 = 0;
    int random3 = rand() %30 + 3;
    static int step = 37+random3;
    this->huPoker+=step;
    count+=37;
    int random2 = rand() %230 + 1533;
    int random4 = rand() %4+1;
    if (count>843+random2*random4){
        count2+=34;
        step= 0;
        //step = 37+random3;

        if (count2>244+random2*random4){
            step = 37;
            count2 = 0 ;
            count = 0;
            
        }
        
    }
    if (this->huPoker > this->currHuPoker){
        int random = rand() %354230 + 124533;
        this->huPoker = this->currHuPoker/3+random;
    }
}
void SceneManager::onScheduleFakeHuThuongCaoThap(float dt){
    static int count = 0;
    static int count2 = 0;
    int random3 = rand() %30 + 3;
    static int step = 37+random3;
    this->huCaoThap+=step;
    count+=37;
    int random2 = rand() %230 + 1533;
    int random4 = rand() %4+1;
    if (count>343+random2*random4){
        count2+=34;
        step= 0;
        //step = 37+random3;
        
        if (count2>1144+random2*random4){
            step = 37;
            count2 = 0 ;
            count = 0;
            
        }
        
    }
    if (this->huCaoThap > this->currHuCaoThap){
        int random = rand() %254230 + 124533;
        this->huCaoThap = this->currHuCaoThap/3+random;
    }
}
void SceneManager::setLastGameID(int gameID){
    UserDefault *def = UserDefault::getInstance();
    def->setStringForKey("lastgameid", StringUtils::format("%d_%s",gameID,this->getMyName().c_str()));
    //def->setIntegerForKey("lastgameid", gameID);
}
int SceneManager::getExchangeRate(int type,int menhgia){
    auto lst = mUtils::splitString(dataManager.getAppConfig().exchangerate,'/');
    if (lst.size()<2) return 0;
    auto lstMenhGia = mUtils::splitString(lst[type],';');
    for (int i = 0 ; i < lstMenhGia.size();i++){
        auto it = mUtils::splitString(lstMenhGia[i],'_');
        if (it.size()<2) return 0;
        if (atoi(it[0].c_str())==menhgia)
            return atoi(it[1].c_str());
    }
    return 0;
}

string SceneManager::getBankAccountByType(int type){
    auto lst = mUtils::splitString(dataManager.getAppConfig().bankaccount,'|');
    for (int i = 0 ; i < lst.size();i++){
        auto it = mUtils::splitString(lst[i], ':');
        if (it.size()<2)
            return "";
        if (atoi(it[0].c_str()) == type){
            return it[1];
        }
    }
    return "";
}
