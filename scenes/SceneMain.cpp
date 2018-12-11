#include "SceneMain.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/PhomMessDef.h"

#include "Entities/Invitation/Invitation.h"
//4 test
#include "ScenePickRoom.h"
#include "../layers/LayerPopupInviteGame.h"
#include "../layers/LayerPopupSetting.h"
#include "../layers/LayerChickenPopup.h"
#include "../layers/LayerPromotion.h"
#include "../layers/LayerCreateRoom.h"
#include "../layers/LayerRecharge.h"
#include "../layers/LayerPickGame.h"
#include "../layers/LayerFriendTop.h"
#include "../cotuong/LayerPickRoom.h"

#include "../mUtils.h"
#include "../data_manager.h"
#include "../layers/LayerPopupHomThu.h"
#include "Requests/SubscribeRoomGroupRequest.h"
#include "Requests/UnsubscribeRoomGroupRequest.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/Buddylist/InitBuddyListRequest.h"
#include "Requests/Buddylist/GoOnlineRequest.h"

#include "../layers/LayerPopupChangeName.h"
#include "../SceneManager.h"
#include "Requests/LogoutRequest.h"
#include "../layers/LayerPopupRecharge.h"
#include "../layers/LayerWelcomeNewbieNormal.h"
#include "../layers/LayerPopupRuongVip.h"
#include "../layers/LayerPopupMission.h"
#include "../layers/LayerPopupEvent.h"
#include "../layergames/LayerGameTaiXiuMini.h"
#include "../layergames/LayerMenuMiniGame.h"
#include "../layergames/ChanUtils.h"
#include "../layers/LayerShowChuChay.h"
#include "../layers/LayerHuThuong.h"
#include "../layergames/MauBinhLayerXepBai.h"
#include "../layers/LayerLogin.h"
#include "../layers/LayerBaoDanhNew.h"
#include "../layergames/LayerPlayGameTXMini.h"
#include "../layers/LayerPopupInviteGame.h"
#include "../layers/LayerPopupDoiMatKhauChoiNgay.h"
#include "../layergames/LayerPlayVQMM.h"
#include "../layers/ItemMenuGame.h"
#include "../layers/LayerGuide.h"
#include "../layers/LayerPopupDoiMoc.h"
#include "../layers/LayerChatInScenePickRoom.h"
#include "../layers/LayerMainBottom2.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
USING_NS_CC;
using namespace cocos2d::ui;

#define zorder_layer_nap_tien 500000
#define ZORDER_POPUP 10000

template<> SceneMain* SingLeton<SceneMain>::mSingleton = 0;

SceneMain* SceneMain::getSingletonPtr(void)
{
    return mSingleton;
}

SceneMain& SceneMain::getSingleton(void)
{
    assert(mSingleton);
    return (*mSingleton);
}
void SceneMain::setChatting(){
    this->unschedule(schedule_selector(SceneMain::updateChating));
    this->chatting = true;
    this->scheduleOnce(schedule_selector(SceneMain::updateChating),0.5);
}
void SceneMain::updateChating(float dt){
    this->chatting = false;
}
void SceneMain::goToMain(){

    //this->gotoDiemDanh();
    this->currTypeMoneyGo = 0;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    this->removeAllTag();
    this->isMain = true;
    this->removeChildByTag(12356);
    this->removeChildByTag(12357);

    this->removeChildByTag(this->tagLogin);
    this->schedule(schedule_selector(SceneMain::scheduleTXMINI),1);
    this->scheduleOnce(schedule_selector(SceneMain::gotoGuideGame), 0.1);
    if (this->currentLayer == this->typelayerGame){
        
        if (SceneManager::getSingleton().isNagaWin && this->getChildByTag(9156) == NULL){
            this->chat = LayerChatInScenePickRoom::create();
            this->addChild(chat);
            chat->setTag(9156);
            chat->setLocalZOrder(1000);
            chat->setPosition(50,160);
        }
        this->removeChildByTag(this->tagGame);

        mLayerMainTop = LayerMainTop::create();
        Size _s = mLayerMainTop->getContentSize();
        
        mLayerMainTop->setPosition(Point(0, visibleSize.height - mLayerMainTop->getContentSize().height));
        mLayerMainTop->setLocalZOrder(1000);
        mLayerMainTop->currentParent = 0;
        ui::Helper::doLayout(mLayerMainTop);
        this->mLayerMainTop->setTag(this->tagMainTop);
        this->addChild(mLayerMainTop);
        
        mLayerMainBottom = LayerMainBottom::create();
        mLayerMainBottom->setPosition(Point(0, 0));
        mLayerMainBottom->setLocalZOrder(1000);
        mLayerMainBottom->currScene = 1;
        mLayerMainBottom->hideTXMenu();
        this->mLayerMainBottom->setTag(this->tagMainBottom);

        ui::Helper::doLayout(mLayerMainBottom);
        this->addChild(mLayerMainBottom);
        
        if (!this->getChildByTag(this->tagMainContent)){
            this->mLayerMainContent = LayerMainContentNew2::create();
            this->mLayerMainContent->setTag(this->tagMainContent);
            this->mLayerMainContent->setAnchorPoint(Point(0.5, 0.5));
            this->mLayerMainContent->setPosition(Point(mLayerMainTop->getPositionX(), visibleSize.height / 2 - mLayerMainContent->getContentSize().height / 2));
            ui::Helper::doLayout(mLayerMainContent);
            this->addChild(this->mLayerMainContent, 2);
        }
        
        this->mLayerMainTop->currentParent = 0;
        
        if (SceneManager::getSingleton().isNagaWin){
            this->mLayerMainTop->imgLogo->setVisible(true);
            mLayerMainBottom->showTXMenu();
        }
        else if (SceneManager::getSingleton().isNagaNew){
            this->mLayerMainTop->btnBack->setVisible(false);
        }
        else{
            this->mLayerMainTop->btnThoat->loadTextures("moc3-ingame-menu.png", "moc3-ingame-menu.png", "moc3-ingame-menu.png");
        }
        SceneManager::getSingleton().setGameID(0);
    }else{

        if (!this->getChildByTag(this->tagMainContent)){
            this->mLayerMainContent = LayerMainContentNew2::create();
            this->mLayerMainContent->setTag(this->tagMainContent);
            this->mLayerMainContent->setAnchorPoint(Point(0.5, 0.5));
            this->mLayerMainContent->setPosition(Point(mLayerMainTop->getPositionX(), visibleSize.height / 2 - mLayerMainContent->getContentSize().height / 2));
            ui::Helper::doLayout(mLayerMainContent);
            this->addChild(this->mLayerMainContent, 2);
        }
        if (!SceneManager::getSingleton().isNagaNew)
            mLayerMainBottom->hideTXMenu();
        this->mLayerMainTop->currentParent = 0;
        if (SceneManager::getSingleton().isNagaWin){
            this->mLayerMainTop->imgLogo->setVisible(true);
        }
        else if (SceneManager::getSingleton().isNagaNew){
            this->mLayerMainTop->btnBack->setVisible(false);
        }
        else{
            this->mLayerMainTop->btnThoat->loadTextures("moc3-ingame-menu.png", "moc3-ingame-menu.png", "moc3-ingame-menu.png");
        }
        this->removeChildByTag(this->tagMainPickRoom);
    }
    if (SceneManager::getSingleton().isNagaWin){
        this->chat->setScene(1);
        this->chat->loadAllDatas();
        
        mLayerMainBottom->showTXMenu();

    }
    if (SceneManager::getSingleton().isNagaNew && this->currentLayer == this->typelayerPickRoom){
        this->removeChildByTag(this->tagMainBottom);
        mLayerMainBottom = LayerMainBottom::create();
        mLayerMainBottom->setPosition(Point(0, 0));
        mLayerMainBottom->setLocalZOrder(1000);
        mLayerMainBottom->currScene = 1;
        mLayerMainBottom->hideTXMenu();
        this->mLayerMainBottom->setTag(this->tagMainBottom);
        
        ui::Helper::doLayout(mLayerMainBottom);
        this->addChild(mLayerMainBottom);
    }
    this->currentLayer = this->typelayerLobby;
    if (atoi(dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR.c_str()) > 0){
        this->chat->sprKhuyenMai->setVisible(true);
        this->chat->txtKhuyenMai->setString(StringUtils::format("+%s%",dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR.c_str())+"%");
    }
}
void SceneMain::goToPickRoom(int gameID){
    
    
    if (gameID == kGameCoTuong || gameID == kGameCoUp)
    {
        this->removeChildByTag(this->tagGame);

        LayerPickRoom * layerPickRoom = LayerPickRoom::create();
        this->addChild(layerPickRoom);
        layerPickRoom->setTag(this->tagMainPickRoom);
        return;
    }
    
   
    Size visibleSize = Director::getInstance()->getVisibleSize();
    this->isMain = false;
    this->removeChildByTag(12356);
    this->removeChildByTag(12357);

    this->removeChildByTag(this->tagLogin);
    this->removeAllTag();

    if (this->currentLayer == this->typelayerGame){
        if (SceneManager::getSingleton().isNagaWin && this->getChildByTag(9156) == NULL){
            this->chat = LayerChatInScenePickRoom::create();
            this->addChild(chat);
            chat->setTag(9156);
            chat->setLocalZOrder(1000);
            chat->setPosition(50,160);
        }
        
        
        
        this->removeChildByTag(this->tagGame);
        mLayerMainTop = LayerMainTop::create();
        Size _s = mLayerMainTop->getContentSize();
        
        mLayerMainTop->setPosition(Point(0, visibleSize.height - mLayerMainTop->getContentSize().height));
        mLayerMainTop->setLocalZOrder(1000);
        mLayerMainTop->currentParent = 0;
        ui::Helper::doLayout(mLayerMainTop);
        this->mLayerMainTop->setTag(this->tagMainTop);

        this->addChild(mLayerMainTop);
        
        mLayerMainBottom = LayerMainBottom::create();
        mLayerMainBottom->setPosition(Point(0, 0));
        mLayerMainBottom->setLocalZOrder(1000);
        mLayerMainBottom->currScene = 1;
        mLayerMainBottom->showTXMenu();
        this->mLayerMainBottom->setTag(this->tagMainBottom);

        ui::Helper::doLayout(mLayerMainBottom);
        this->addChild(mLayerMainBottom);

        this->layerPickRoom = ScenePickRoom::create();
        this->layerPickRoom->setTag(this->tagMainPickRoom);
        this->addChild(this->layerPickRoom);
        this->mLayerMainTop->currentParent = 1;
//        this->layerPickRoom->setLocalZOrder(1000);
        if (SceneManager::getSingleton().isNagaWin){
            this->chat->setScene(2);
        }
        if (SceneManager::getSingleton().isNagaWin){
            this->mLayerMainTop->imgLogo->setVisible(false);
        }
        else if (SceneManager::getSingleton().isNagaNew){
            this->mLayerMainTop->btnBack->setVisible(true);
        }
        else
        this->mLayerMainTop->btnThoat->loadTextures("moc3-btn-thoat.png", "moc3-btn-thoat.png", "moc3-btn-thoat.png");
    }
    else{
        
        
       
        
        this->mLayerMainTop->imgPopMenu->setVisible(false);
        SceneMain::getSingleton().currentLayer =0;
        this->removeChildByTag(this->tagMainContent);
        this->layerPickRoom = ScenePickRoom::create();
        this->layerPickRoom->setTag(this->tagMainPickRoom);
        this->layerPickRoom->setGameID(gameID);
//        this->layerPickRoom->setLocalZOrder(1000);

        this->addChild(this->layerPickRoom);
        mLayerMainBottom->showTXMenu();
        this->mLayerMainTop->currentParent = 1;
        if (SceneManager::getSingleton().isNagaWin){
            this->chat->setScene(2);
        }
        if (SceneManager::getSingleton().isNagaWin){
            this->mLayerMainTop->imgLogo->setVisible(false);
            
        }
        else if (SceneManager::getSingleton().isNagaNew){
            this->mLayerMainTop->btnBack->setVisible(true);
        }
        else
         this->mLayerMainTop->btnThoat->loadTextures("moc3-btn-thoat.png", "moc3-btn-thoat.png", "moc3-btn-thoat.png");
    }
  
    if (SceneManager::getSingleton().isNagaNew){
        this->removeChildByTag(this->tagMainBottom);
        LayerMainBottom2* bottom = LayerMainBottom2::create();
        bottom->setPosition(Point(0, 0));
        bottom->setLocalZOrder(1000);
        bottom->currScene = 1;
        bottom->showTXMenu();
        bottom->setTag(this->tagMainBottom);
        this->addChild(bottom);
    }
    this->currentLayer = this->typelayerPickRoom;
    if (SceneManager::getSingleton().isNagaWin)
        this->chat->sprKhuyenMai->setVisible(false);
    
}
void SceneMain::removeAllTag(){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (_currScene){
        for (int i = 9001; i <= 9156; i++){
            _currScene->removeChildByTag(i);
        }
    }
}
void SceneMain::gotoGameByID(int gameID){
    this->removeChildByTag(9156);
    this->removeAllTag();
    this->currTypeMoneyGo = 0;
    this->isMain = false;
    this->unschedule(schedule_selector(SceneMain::gotoEventAfter));
    this->unschedule(schedule_selector(SceneMain::scheduleTXMINI));
    this->unschedule(schedule_selector(::SceneMain::scheduleshowTXMINI));
    this->unschedule(schedule_selector(SceneMain::gotoGuideGame));
    this->removeChildByTag(this->tagMainContent);
    this->removeChildByTag(this->tagMainTop);
    this->removeChildByTag(this->tagMainBottom);
    this->removeChildByTag(this->tagMainPickRoom);
    this->removeChildByTag(this->tagLogin);
    this->removeChildByTag(12356);
    this->removeChildByTag(12357);
    this->layerGame = SceneGame::createScene(gameID);
    this->addChild(this->layerGame,100000);
    this->layerGame->setTag(this->tagGame);
    this->currentLayer = this->typelayerGame;
    

}
void SceneMain::prepareToPickGame(){
    
  this->removeAllTag();
    this->isMain = false;
    this->unschedule(schedule_selector(SceneMain::scheduleTXMINI));
    this->unschedule(schedule_selector(::SceneMain::scheduleshowTXMINI));
    
    this->removeChildByTag(this->tagMainContent);
    this->removeChildByTag(this->tagMainTop);
    this->removeChildByTag(this->tagMainBottom);
    this->removeChildByTag(this->tagMainPickRoom);
    this->removeChildByTag(this->tagLogin);
    this->removeChildByTag(this->tagGame);
    this->removeChildByTag(9156);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Sprite* bgloading = Sprite::create("bg_loading.jpg");
    this->addChild(bgloading);
    bgloading->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    bgloading->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);

    bgloading->setTag(12357);
    
    Sprite* loading = Sprite::create("logo-loading-2.png");
    this->addChild(loading);
    loading->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    loading->setTag(12356);
    
}
SceneMain::SceneMain()
{
	isRejoinRoom = false;
	mCurrentRoomIDJoin = 0;
}


SceneMain::~SceneMain()
{
	//log("SceneMain::~SceneMain()");
	GameServer::getSingleton().removeListeners(this);
	this->unschedule(schedule_selector(SceneMain::gotoEventAfter));
    this->unschedule(schedule_selector(SceneMain::scheduleTXMINI));
    this->unschedule(schedule_selector(::SceneMain::scheduleshowTXMINI));
    this->unschedule(schedule_selector(SceneMain::gotoGuideGame));

	//
	//HttpClient::getInstance()->;
}

void SceneMain::gotoPickGame()
{
	if (this->getChildByTag(TAG_CHILD) != NULL){
		this->removeChildByTag(TAG_CHILD, true);
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
    this->isMain = false;

    this->unschedule(schedule_selector(SceneMain::gotoEventAfter));
    this->unschedule(schedule_selector(SceneMain::scheduleTXMINI));
    this->unschedule(schedule_selector(::SceneMain::scheduleshowTXMINI));
    this->unschedule(schedule_selector(SceneMain::gotoGuideGame));
    
     mLayerMainContent = LayerMainContentNew2::create();
    mLayerMainContent->setAnchorPoint(Point(0.5, 0.5));
    mLayerMainContent->setPosition(Point(mLayerMainTop->getPositionX(), visibleSize.height / 2 - mLayerMainContent->getContentSize().height / 2));
    mLayerMainContent->setTag(this->tagMainContent);
    ui::Helper::doLayout(mLayerMainContent);
    this->addChild(mLayerMainContent, 2);


	mLayerMainTop->setVisible(true);
	if (!mLayerMainBottom->isVisible())
	{
		mLayerMainBottom->setVisible(true);
	}
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	IOS::logScreenToGA("Sảnh chính");
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	NativeHelper::trackScreen("Sảnh");
#endif
}
void SceneMain::reloadLang(){
    LayerMainContentNew2* chongame = (LayerMainContentNew2*)this->getChildByTag(this->tagMainContent);
    if (chongame){
        chongame->reloadLang();
    }
    
    LayerMainBottom* bottom = (LayerMainBottom*)this->getChildByTag(this->tagMainBottom);
    if (bottom){
        bottom->reloadLang();
    }
}
void SceneMain::gotoMailInbox(){
    auto currScene = Director::getInstance()->getRunningScene();

    LayerPopupHomThu* popup = LayerPopupHomThu::create();
    currScene->addChild(popup,2000,9132);
}

void SceneMain::gotoChargeMoney()
{
	//    this->tagChild = TOP_RECHARGE;
	//    if (this->getChildByTag(TAG_CHILD) != NULL){
	//        this->removeChildByTag(TAG_CHILD, true);
	//    }
	//    Size visibleSize = Director::getInstance()->getVisibleSize();
	//    
	//    LayerRecharge* _layerRecharge = LayerRecharge::create();
	//    _layerRecharge->setContentSize(visibleSize);
	//    _layerRecharge->setAnchorPoint(Vec2(0, 0));
	//    _layerRecharge->currScene = 1;
	//
	//    //_layer->setPosition(Point(mLayerMainTop->getPositionX(), visibleSize.height / 2 - mLayerMainContent->getContentSize().height / 2));
	//    _layerRecharge->setTag(TAG_CHILD);
	//    this->addChild(_layerRecharge);
	//    mLayerMainTop->setVisible(false);
	//    mLayerMainBottom->setVisible(false);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	LayerPopupRecharge* _child = LayerPopupRecharge::create();
	_child->setLocalZOrder(zorder_layer_nap_tien);
	Size s = s = Size(0, 0);
	if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameCoTuong || SceneManager::getSingleton().getGameID() == kGameCoUp){
		s = mUtils::getSizePos();
		_child->setPosition(Vec2(s.width, s.height));
	}
	this->addChild(_child, zorder_layer_nap_tien);
}

void SceneMain::gotoRank()
{


	this->tagChild = TOP_RANK;
	if (this->getChildByTag(TAG_CHILD) != NULL){
		this->removeChildByTag(TAG_CHILD, true);
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	_layerRank = LayerRank::create();
	//_layer->setContentSize(Size(1920, 1080));
	_layerRank->setAnchorPoint(Vec2(0, 0));
	//_layer->setPosition(Point(mLayerMainTop->getPositionX(), visibleSize.height / 2 - mLayerMainContent->getContentSize().height / 2));
	_layerRank->setTag(TAG_CHILD);
	this->addChild(_layerRank);
	mLayerMainTop->setVisible(false);
}

void SceneMain::gotoEvent(){
    
    if (this->currentLayer == this->typelayerPickRoom) return;

    
//    if (dataManager.getAppConfig().isLoginFisrtDay){
//        int osid = 1;
//        string devID = "";
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//         osid = 1;
//        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
//            devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
//        else
//            devID = NativeHelper::getDeviceId();
//        /*isConnectting = NativeHelper::hasConnection();*/
//        //devID = NativeHelper::getDeviceID();
//        
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        osid = 2;
//        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
//            devID = dataManager.getAppConfig().tientomocp2 + IOS::getDeviceID();
//        else
//            devID = IOS::getDeviceID();
//#endif
//        boost::shared_ptr<ISFSObject> params(new SFSObject());
//        params->PutInt("osid", osid);
//        params->PutUtfString("devid", devID);
//        params->PutInt("srcref", 1);
//        boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_ATTEND_REQ, params));
//        GameServer::getSingleton().Send(request);
//    }
    int size = SceneManager::getSingleton().lstEvent.size();
    
    if (size<=0){
        this->scheduleOnce(schedule_selector(SceneMain::goToDiemDanh),1);
        //    this->schedule(schedule_selector(SceneMain::goToDiemDanh), this, 1, 1, 1, false);
        this->unschedule(schedule_selector(SceneMain::gotoEventAfter));
//        SceneManager::getSingleton().isShowEvent = true;
//        dataManager.getAppConfig().isLoginFisrtDay = false;
        this->isNoEvent = true;
        return;
    }
    if (dataManager.getAppConfig().isHideBilling || !SceneManager::getSingleton().isLogin || SceneManager::getSingleton().isShowEvent || !dataManager.getAppConfig().isLoginFisrtDay)
    {
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("aI", SceneManager::getSingleton().getMyName());
        boost::shared_ptr<IRequest> request(new ExtensionRequest("rgaci", params));
        GameServer::getSingleton().Send(request);
        SceneManager::getSingleton().isShowEvent = true;
        this->isNoEvent = true;
        this->scheduleOnce(schedule_selector(SceneMain::goToDiemDanh),1);
        //this->schedule(schedule_selector(SceneMain::goToDiemDanh), this, 1, 1, 1, false);
        this->unschedule(schedule_selector(SceneMain::gotoEventAfter));
        return;
    }
  
	LayerPopupEvent* event = LayerPopupEvent::create();
    SceneManager::getSingleton().isShowEvent = true;
    this->isNoEvent = false;
	this->addChild(event, 100000);
	this->unschedule(schedule_selector(SceneMain::gotoEventAfter));
}

void SceneMain::gotoRuongVip(){
	LayerPopupRuongVip* ruong = LayerPopupRuongVip::create();
	this->addChild(ruong, 1000);
}

void SceneMain::gotoNotifcation()
{
	this->tagChild = TOP_NOTIFICATION;
	if (this->getChildByTag(TAG_CHILD) != NULL){
		this->removeChildByTag(TAG_CHILD, true);
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
    auto currScene = Director::getInstance()->getRunningScene();
	_layerNotification = LayerNotification::create();
	_layerNotification->setContentSize(visibleSize);
	_layerNotification->setAnchorPoint(Vec2(0, 0));
	//_layer->setPosition(Point(mLayerMainTop->getPositionX(), visibleSize.height / 2 - mLayerMainContent->getContentSize().height / 2));
	_layerNotification->setTag(TAG_CHILD);
	currScene->addChild(_layerNotification,2000,9133);

	mLayerMainTop->setVisible(false);
}

cocos2d::Scene* SceneMain::createScene()
{
	auto scene = Scene::create();

	auto layer = SceneMain::create();
	layer->setTag(2468);//cheat for get scene main

	scene->addChild(layer);

	return scene;
}

bool SceneMain::init()
{
	if (!Layer::init())
	{
		return false;
	}
    
    if (SceneManager::getSingleton().getIsFlagStore()) return true;
    
	this->setKeyboardEnabled(true);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->setAnchorPoint(Vec2(0.5, 0.5));

	// add background
	auto background = Sprite::create("bg_sanh_cho_game.jpg");
	background->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	background->setAnchorPoint(Point(0, 0));
	background->setPosition(Point(0, 0));
	this->addChild(background);
    
    
//    auto light = Sprite::create("anims/light-bg-main.png");
//    light->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
//    light->setAnchorPoint(Point(0.5, 0.5));
//    light->setPosition(Vec2(visibleSize.width/2-300,visibleSize.height/2));
//    this->addChild(light);
//    light->runAction(RepeatForever::create(RotateBy::create(0.9, 30)));
//    light->setTag(12190);
	SceneManager::getSingleton().setCurrScene(0);


	//test maubinh
	//    MauBinhLayerXepBai* layer = MauBinhLayerXepBai::create();
	//    this->addChild(layer);
	//    return true;

//    mLayerMainTop = LayerMainTop::create();
//    Size _s = mLayerMainTop->getContentSize();
//
//    mLayerMainTop->setPosition(Point(0, visibleSize.height - mLayerMainTop->getContentSize().height));
//    mLayerMainTop->setLocalZOrder(1000);
//    mLayerMainTop->currentParent = 0;
//    ui::Helper::doLayout(mLayerMainTop);
//    mLayerMainTop->setTag(this->tagMainTop);
//    this->addChild(mLayerMainTop);
//
    mLayerMainBottom = LayerMainBottom::create();
    mLayerMainBottom->setPosition(Point(0, 0));
    mLayerMainBottom->setLocalZOrder(1000);
    mLayerMainBottom->setTag(this->tagMainBottom);
    mLayerMainBottom->currScene = 1;
    mLayerMainBottom->hideTXMenu();
    ui::Helper::doLayout(mLayerMainBottom);
    this->addChild(mLayerMainBottom);

	SceneManager::getSingleton().setGameID(0);

//	gotoPickGame();
	startTimeOutChat();
    
    if (SceneManager::getSingleton().isNagaWin && this->getChildByTag(9156) == NULL){
        this->chat = LayerChatInScenePickRoom::create();
        this->addChild(chat);
        chat->setTag(9156);
        chat->setLocalZOrder(1000);
        chat->setPosition(50,160);
    }
    if (SceneManager::getSingleton().isNagaWin){
        mLayerMainBottom->showTXMenu();
    }

	Size sizeAdd = Size(0,0);
	//    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
	//        LayerMenuMiniGame* minigame = LayerMenuMiniGame::create();
	//        minigame->setPosition(Vec2(sizeAdd.width, sizeAdd.height));
	//        this->addChild(minigame, ZORDER_LIST::ZORDER_TAIXIU_MINI);
	//        
	//        
	//    }else{
	//        LayerMenuMiniGame* minigame = LayerMenuMiniGame::create();
	//        minigame->setPosition(Vec2(sizeAdd.width, sizeAdd.height));
	//        this->addChild(minigame, ZORDER_LIST::ZORDER_TAIXIU_MINI);
	//    }
	//


	// 	this->schedule(schedule_selector(SceneManager::updateEvent),this,1/60, false);
	// 	this->schedule(schedule_selector(SceneManager::updateEvent4CheckConnection), 1);

    //if (mUtils::isSoundOn()){
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    //}
    
   
    if (SceneManager::getSingleton().isNagaNew){
        mLayerMainTop = LayerMainTop::create();
        Size _s = mLayerMainTop->getContentSize();
        
        mLayerMainTop->setPosition(Point(0, visibleSize.height - mLayerMainTop->getContentSize().height));
        mLayerMainTop->setLocalZOrder(1000);
        mLayerMainTop->currentParent = 0;
        ui::Helper::doLayout(mLayerMainTop);
        this->mLayerMainTop->setTag(this->tagMainTop);
        this->addChild(mLayerMainTop);
    }
    
	if (!SceneManager::getSingleton().isLogin){
//        LayerLogin* login = LayerLogin::create();
//        //mLayerMainTop->setVisible(false);
//        login->setTag(this->tagLogin);
//        this->addChild(login, 10000);
		return true;
	}
    
    if (mUtils::isSoundOn()){
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bm-theme_v3.mp3",true);
    }

    LayerHuThuong* huthuong = LayerHuThuong::create();
    huthuong->setTag(19802);
    this->addChild(huthuong,100000);
    


    
   	boost::shared_ptr<IRequest> request(new ExtensionRequest("cmi", NULL));//count msg inbox
	GameServer::getSingleton().Send(request);

	//    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
	//
	//        boost::shared_ptr<IRequest> request1(new ExtensionRequest("glsrd", NULL));//get sms recharged
	//        GameServer::getSingleton().Send(request1);
	//    }


	//reloadChargeConfig();


	//Get json promotion
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL){
		SceneManager::getSingleton().showChatWithCurrentScene(dataManager.GetSysString(602), -1);
		//SceneManager::getSingleton().showLoading();
		this->scheduleOnce(schedule_selector(SceneMain::gotoReconnect), 2);
    //this->schedule(schedule_selector(SceneMain::gotoReconnect), this, 2, 1, 2, false);
		return true;
	}
	bool isGetPromotion = true;
	// 	if (myself != NULL && SceneManager::getSingleton().getPromotionHandlers().getUserID().compare(*myself->Name()) != 0)
	// 		isGetPromotion = false;
	if (!isGetPromotion && SceneManager::getSingleton().getUrlPromotion().compare("") != 0){
		isGetPromotion = true;
		if (myself != NULL)
			SceneManager::getSingleton().getPromotionHandlers().setUserID(*myself->Name());
		SceneManager::getSingleton().getPromotionFromUrl();
	}


	if (!GameServer::getSingleton().getSmartFox()->BuddyManager()->Inited()){
		boost::shared_ptr<IRequest> request(new Buddylist::InitBuddyListRequest);
		GameServer::getSingleton().Send(request);
	}




	if (myself->GetVariable("aN") == NULL)
		return true;

	//	string _strName = *myself->GetVariable("aN")->GetStringValue();
	//	//26/01/2016 HoangDD changes
	//	UserDefault *def = UserDefault::getInstance();
	//	string _lt = def->getStringForKey("LT", "");
	//	if (_lt.compare("normal") != 0 && _strName.compare("") == 0){
	//		//Neu ko phhai la login binh thuong, se kiem tra aN, neu aN = "" thi bat buoc phai update
	//		LayerPopupChangeName* _layer = LayerPopupChangeName::create();
	//		this->addChild(_layer, ZORDER_POPUP);
	//
	//		return true;
	//	}

	//dataManager.getAppConfig().isLoginFisrt = true;
    int rotatebonus = 0;
    if (myself->GetVariable("rotatebonus") != NULL){
        if (myself->GetVariable("rotatebonus")->GetIntValue())
        rotatebonus = *myself->GetVariable("rotatebonus")->GetIntValue();
    }
    

 
    
	if (strcmp("facebook", dataManager.getAppConfig().loginType.c_str()) == 0){
		if (dataManager.getAppConfig().isreceivedRegisterFace)
			UserDefault::getInstance()->setBoolForKey("isReceivedFacebook", true);
		else
			UserDefault::getInstance()->setBoolForKey("isReceivedFacebook", false);
		UserDefault::getInstance()->flush();
	}
	else{
		if (dataManager.getAppConfig().isreceivedLinkFace)
			UserDefault::getInstance()->setBoolForKey("isLinkFacebook", true);
		else
			UserDefault::getInstance()->setBoolForKey("isLinkFacebook", false);
		UserDefault::getInstance()->flush();
	}

   // if (dataManager.getAppConfig().isLoginFisrt){

   // }
    if (dataManager.getAppConfig().isHideBilling)
        return true;
    this->schedule(schedule_selector(SceneMain::scheduleTXMINI),1);
    this->scheduleOnce(schedule_selector(SceneMain::gotoGuideGame), 0.1);
    //this->schedule(schedule_selector(SceneMain::gotoGuideGame), this, 0.1, 1, 0.1, false);
	return true;
}
void SceneMain::logOut(){
    this->removeChildByTag(this->tagLogin);
    this->removeChildByTag(this->tagGame);
    this->removeChildByTag(this->tagMainPickRoom);
    this->removeChildByTag(this->tagMainTop);
    if (SceneManager::getSingleton().isNagaWin){
        this->chat->isJoin = false;
    }
    if (SceneManager::getSingleton().getIsFlagStore()){
        SceneManager::getSingleton().showLoading();
        UserDefault *def = UserDefault::getInstance();
        def->setBoolForKey("UCK", true);
        def->flush();
        boost::shared_ptr<IRequest> request(new LogoutRequest());
        GameServer::getSingleton().Send(request);
        //
        SceneManager::getSingleton().isLogout = true;
        SceneManager::getSingleton().rcForLogin = false;
        LayerLogin* login = LayerLogin::create();
        login->setTag(this->tagLogin);
        this->addChild(login, 100000);
        return;
    }
    this->removeChildByTag(12356);
    this->removeChildByTag(12357);

    this->removeChildByTag(1942);

    auto _currScene = Director::getInstance()->getRunningScene();
    if (_currScene){
    _currScene->removeChildByTag(1504);
    }

    if (_currScene){
        for (int i = 9001; i <= 9153; i++){
            _currScene->removeChildByTag(i);
        }
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    this->removeChildByTag(this->tagLogin);
    this->removeChildByTag(this->tagGame);
    this->removeChildByTag(this->tagMainPickRoom);
    this->removeChildByTag(this->tagMainTop);

    if (this->currentLayer == this->typelayerLobby){

        LayerLogin* login = LayerLogin::create();
        login->setTag(this->tagLogin);
        this->removeChildByTag(this->tagMainTop);
        this->addChild(login, 10000);
        mLayerMainBottom->logOut();
        mLayerMainBottom->hideTXMenu();
        mLayerMainContent->isLogout();
    }
    else if (this->currentLayer == this->typelayerPickRoom){
        this->removeChildByTag(this->tagMainContent);

        LayerLogin* login = LayerLogin::create();
        login->setTag(this->tagLogin);
        this->addChild(login, 10000);
        if (!SceneManager::getSingleton().isNagaNew){
        mLayerMainBottom->logOut();
        mLayerMainBottom->hideTXMenu();
    }
        else{
            this->removeChildByTag(this->tagMainBottom);
            mLayerMainBottom = LayerMainBottom::create();
            mLayerMainBottom->setPosition(Point(0, 0));
            mLayerMainBottom->setLocalZOrder(1000);
            mLayerMainBottom->currScene = 1;
            mLayerMainBottom->hideTXMenu();
            this->mLayerMainBottom->setTag(this->tagMainBottom);
            
            ui::Helper::doLayout(mLayerMainBottom);
            this->addChild(mLayerMainBottom);
        }

        this->mLayerMainContent = LayerMainContentNew2::create();
        this->mLayerMainContent->setTag(this->tagMainContent);
        this->mLayerMainContent->setAnchorPoint(Point(0.5, 0.5));
        this->mLayerMainContent->setPosition(Point(0, visibleSize.height / 2 - mLayerMainContent->getContentSize().height / 2));
        ui::Helper::doLayout(mLayerMainContent);
        this->addChild(this->mLayerMainContent, 2);
    }else{
        this->removeChildByTag(this->tagMainBottom);
        this->removeChildByTag(this->tagMainContent);

        LayerLogin* login = LayerLogin::create();
        login->setTag(this->tagLogin);
        this->removeChildByTag(this->tagMainTop);
        this->addChild(login, 10000);
        
        this->mLayerMainContent = LayerMainContentNew2::create();
        this->mLayerMainContent->setTag(this->tagMainContent);
        this->mLayerMainContent->setAnchorPoint(Point(0.5, 0.5));
        this->mLayerMainContent->setPosition(Point(0, visibleSize.height / 2 - mLayerMainContent->getContentSize().height / 2));
        ui::Helper::doLayout(mLayerMainContent);
        this->addChild(this->mLayerMainContent, 2);
        
        mLayerMainBottom = LayerMainBottom::create();
        mLayerMainBottom->setPosition(Point(0, 0));
        mLayerMainBottom->setLocalZOrder(1000);
        mLayerMainBottom->currScene = 1;
        mLayerMainBottom->hideTXMenu();
        this->mLayerMainBottom->setTag(this->tagMainBottom);
        
        ui::Helper::doLayout(mLayerMainBottom);
        this->addChild(mLayerMainBottom);
        mLayerMainBottom->logOut();

    }
    if (SceneManager::getSingleton().isNagaNew){
        mLayerMainTop = LayerMainTop::create();
        Size _s = mLayerMainTop->getContentSize();
        
        mLayerMainTop->setPosition(Point(0, visibleSize.height - mLayerMainTop->getContentSize().height));
        mLayerMainTop->setLocalZOrder(1000);
        mLayerMainTop->currentParent = 0;
        ui::Helper::doLayout(mLayerMainTop);
        this->mLayerMainTop->setTag(this->tagMainTop);
        this->addChild(mLayerMainTop);
    }
}

void SceneMain::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
   if (strcmp(response->getHttpRequest()->getTag(), "firebase") == 0)
        {
            log("firebase errror");
        }
        
        return;
    }
 if (strcmp(response->getHttpRequest()->getTag(), "firebase") == 0){
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
}
void SceneMain::logInDone(){
    
    if (SceneManager::getSingleton().getIsFlagStore()){
        SceneManager::getSingleton().setGameID(kGameCoTuong);
        SceneManager::getSingleton().gotoPickGame(kGameCoTuong);
        return;
    }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::askPushNotificationSetting();
#endif
//    //only for china
//    this->prepareToPickGame();
//    this->currentLayer = this->typelayerGame;
//    SceneManager::getSingleton().setGameID(kGamePoker);
//    this->goToPickRoom(kGamePoker);
    
    this->removeChildByTag(this->tagMainTop);

    
    string urlToken = "http://sv.nagaclub.net:8887/push/user/token?accountId="+SceneManager::getSingleton().getMyName()+"&firebaseToken="+SceneManager::getSingleton().tokenFireBase+"&firebaseProjectId="+"nagacard-dc50a";
    
    //http://sv.nagaclub.net:8887/push/user/token?accountId=gianglt2009&firebaseToken=<firebaseToken>&firebaseProjectId=<firebaseProjectId>
    
    HttpRequest* requesthttp = new (std::nothrow) HttpRequest();
    requesthttp->setUrl(urlToken.c_str());
    requesthttp->setRequestType(HttpRequest::Type::GET);
    requesthttp->setTag("firebase");
    requesthttp->setResponseCallback(CC_CALLBACK_2(SceneMain::onHttpRequestCompleted, this));
    HttpClient::getInstance()->send(requesthttp);
    requesthttp->release();
    
    //return;

    if (SceneManager::getSingleton().isNagaWin){
        this->chat->setScene(1);
        this->chat->loadAllDatas();
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();

    this->reloadData();
    if (mUtils::isSoundOn()){
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bm-theme_v3.mp3",true);
    }
    
    mLayerMainTop = LayerMainTop::create();
    Size _s = mLayerMainTop->getContentSize();
    mLayerMainTop->setPosition(Point(0, visibleSize.height - mLayerMainTop->getContentSize().height));
    mLayerMainTop->setLocalZOrder(1000);
    mLayerMainTop->currentParent = 0;
    ui::Helper::doLayout(mLayerMainTop);
    this->mLayerMainTop->setTag(this->tagMainTop);
    this->addChild(mLayerMainTop);
    this->mLayerMainTop->loadDatas();
    
    mLayerMainBottom->hideTXMenu();
    this->isMain = true;
    if (SceneManager::getSingleton().isNagaWin){
        mLayerMainBottom->showTXMenu();
    }
    
    LayerHuThuong* huthuong = LayerHuThuong::create();
    huthuong->setTag(19802);
    this->addChild(huthuong,100000);
    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("cmi", NULL));//count msg inbox
    GameServer::getSingleton().Send(request);

    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL){
        SceneManager::getSingleton().showChatWithCurrentScene(dataManager.GetSysString(602), -1);
        this->scheduleOnce(schedule_selector(SceneMain::gotoReconnect),2);
        return;
    }
    bool isGetPromotion = true;
    if (!isGetPromotion && SceneManager::getSingleton().getUrlPromotion().compare("") != 0){
        isGetPromotion = true;
        if (myself != NULL)
            SceneManager::getSingleton().getPromotionHandlers().setUserID(*myself->Name());
        SceneManager::getSingleton().getPromotionFromUrl();
    }
    
    
    if (!GameServer::getSingleton().getSmartFox()->BuddyManager()->Inited()){
        boost::shared_ptr<IRequest> request(new Buddylist::InitBuddyListRequest);
        GameServer::getSingleton().Send(request);
    }
    mLayerMainContent->reloadListGame();
    
    
    //this->gotoDiemDanh();

    
    if (myself->GetVariable("aN") == NULL)
        return;

    int rotatebonus = 0;
    if (myself->GetVariable("rotatebonus") != NULL){
        if (myself->GetVariable("rotatebonus")->GetIntValue())
            rotatebonus = *myself->GetVariable("rotatebonus")->GetIntValue();
    }
    
    if (strcmp("facebook", dataManager.getAppConfig().loginType.c_str()) == 0){
        if (dataManager.getAppConfig().isreceivedRegisterFace)
            UserDefault::getInstance()->setBoolForKey("isReceivedFacebook", true);
        else
            UserDefault::getInstance()->setBoolForKey("isReceivedFacebook", false);
        UserDefault::getInstance()->flush();
    }
    else{
        if (dataManager.getAppConfig().isreceivedLinkFace)
            UserDefault::getInstance()->setBoolForKey("isLinkFacebook", true);
        else
            UserDefault::getInstance()->setBoolForKey("isLinkFacebook", false);
        UserDefault::getInstance()->flush();
    }
    //SceneManager::getSingleton().screenNotify = "cashout";
    if (dataManager.getAppConfig().isHideBilling)
        return;
    if (SceneManager::getSingleton().screenNotify == ""){
        this->schedule(schedule_selector(SceneMain::scheduleTXMINI),1);
        this->scheduleOnce(schedule_selector(SceneMain::gotoGuideGame),0.1);
    }else{
//       gameId-roomType => tương ứng đi đến sảnh của game đó
//        store => chuyến hướng sang appstore, ggplay
//        cashin => tab nạp tiền
//        cashout => tab rút tiền
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return;
        if (SceneManager::getSingleton().screenNotify == "cashin"){
            LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
            popup->onBtnNapThe(NULL);
            if (dataManager.getAppConfig().isHideBilling){
                popup->onBtnNapHao(NULL);
            }
            _currScene->addChild(popup,2000+20,9101);
        }
        else if (SceneManager::getSingleton().screenNotify == "cashout"){
            LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
            popup->onBtnMaThe(NULL);
            _currScene->addChild(popup,2000+20,9100);
            this->removeFromParent();
        }else{
            auto lst = mUtils::splitString(SceneManager::getSingleton().screenNotify, '-');
            if (lst.size()>=2){
                SceneManager::getSingleton().setGameID(atoi(lst[0].c_str()));
                this->goToPickRoom(atoi(lst[0].c_str()));
                if (lst[1].c_str() == "zen"){
                    this->layerPickRoom->roomType = 1;
                    this->layerPickRoom->onButtonTypeRoom(this->layerPickRoom->btnMoneyGold);
                }else{
                    this->layerPickRoom->roomType = 0;
                    this->layerPickRoom->onButtonTypeRoom(this->layerPickRoom->btnMoneySilver);
                }
            }
        }
        
        SceneManager::getSingleton().screenNotify = "";
    }
}
void SceneMain::scheduleTXMINI(float dt){
    //anhnt only for test
    //return;
    if (this->currentLayer == this->typelayerPickRoom) return;
    
    bool isShouldShowTX = false;
    if (SceneManager::getSingleton().isShowEvent && !dataManager.getAppConfig().isLoginFisrtDay){
        isShouldShowTX = true;
    }
    if (this->isNoEvent){
        isShouldShowTX = true;

    }
    if (SceneManager::getSingleton().isLogin && isShouldShowTX && SceneManager::getSingleton().getIsShowVQBonus() && !SceneManager::getSingleton().getIsShowTX()&& !SceneManager::getSingleton().getIsShowXD()&& !SceneManager::getSingleton().getIsShowPK() && !SceneManager::getSingleton().getIsShowCT() && !SceneManager::getSingleton().getIsShowVQ() && dataManager.getAppConfig().isShowMatKhauDone && SceneManager::getSingleton().numPopup<=0){
        UserDefault *def = UserDefault::getInstance();
        bool shouldShow = def->getBoolForKey("showinstantTX", false);
        if (shouldShow){
            if (SceneManager::getSingleton().getIsShowTX()){
                def->setBoolForKey("showinstantTX", false);
                //this->unschedule(schedule_selector(SceneMain::scheduleTXMINI));
                return;
            }
            auto currScene = Director::getInstance()->getRunningScene();
            def->setBoolForKey("showinstantTX", false);
            LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
            currScene->addChild(layerPlayerGame, 1000,9134);
            SceneManager::getSingleton().setIsShowTX(true);
            //this->unschedule(schedule_selector(SceneMain::scheduleTXMINI));
            return;
        }
        //this->unschedule(schedule_selector(SceneMain::scheduleTXMINI));
        //    this->schedule(schedule_selector(SceneMain::scheduleTXMINI), this, dataManager.getAppConfig().timeShowTX, 1, dataManager.getAppConfig().timeShowTX, false);
        this->scheduleOnce(schedule_selector(SceneMain::scheduleshowTXMINI),dataManager.getAppConfig().timeShowTX);

    }else{
        this->unschedule(schedule_selector(SceneMain::scheduleshowTXMINI));
    }
}
void SceneMain::scheduleshowTXMINI(float dt){
    //anhnt only for test
    //return;
    
    if (this->currentLayer == this->typelayerPickRoom) return;

    
    auto currScene = Director::getInstance()->getRunningScene();
    bool isShouldShowTX = false;
    if (SceneManager::getSingleton().isShowEvent && !dataManager.getAppConfig().isLoginFisrtDay){
        isShouldShowTX = true;
    }
    if (this->isNoEvent){
        isShouldShowTX = true;
        
    }
    if (SceneManager::getSingleton().getIsShowTX() || !isShouldShowTX || !SceneManager::getSingleton().getIsShowVQBonus() || SceneManager::getSingleton().getIsShowTX() || SceneManager::getSingleton().getIsShowXD() ||SceneManager::getSingleton().getIsShowPK() || SceneManager::getSingleton().getIsShowCT() ||SceneManager::getSingleton().getIsShowVQ() || !dataManager.getAppConfig().isShowMatKhauDone || SceneManager::getSingleton().numPopup>0){
        this->unschedule(schedule_selector(SceneMain::scheduleshowTXMINI));
        return;
    }
    LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
    currScene->addChild(layerPlayerGame, 1000,9135);
    SceneManager::getSingleton().setIsShowTX(true);
    this->unschedule(schedule_selector(SceneMain::scheduleshowTXMINI));
}
void SceneMain::reloadData(){
    if (SceneManager::getSingleton().getIsFlagStore()){
        return;
    }
	mLayerMainBottom->loadAllDatas();
    mLayerMainContent->isLogon();
//    mLayerMainTop->setVisible(true);
}
void SceneMain::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->onTouchBegan = CC_CALLBACK_2(SceneMain::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;

	GameServer::getSingleton().addListeners(this);
}
bool SceneMain::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
	if (!SceneManager::getSingleton().isLogin){
		LayerLogin* login = LayerLogin::create();
		this->addChild(login, 10000);
		return true;
	}
	return true;
}
void SceneMain::onExit()
{
  
	Layer::onExit();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}
void SceneMain::gotoReconnect(float dt){
	SceneManager::getSingleton().hideLoading();
	SceneManager::getSingleton().disconnectFromServer();
}

void SceneMain::onButtonBackPress()
{
	if (this->getChildByTag(POPUP_THONGBAO) == NULL){
		showPopupThongbao();
	}
}

void SceneMain::gotoFriend()
{
	this->tagChild = TOP_FRIEND;
	if (this->getChildByTag(TAG_CHILD) != NULL){
		this->removeChildByTag(TAG_CHILD, true);
	}

	_layerFriend = LayerFriend::create();
	this->addChild(_layerFriend);
	_layerFriend->setTag(TAG_CHILD);

	mLayerMainTop->setVisible(false);
	mLayerMainBottom->setVisible(false);
}

void SceneMain::reloadChargeConfig()
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

void SceneMain::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	//log("============== CMD = %s", cmd->c_str());
	if (cmd->compare("notiRMFF") == 0)
	{
        //log("notiRMFF");
	}
    if (0 == cmd->compare(AUTO_JOIN_TBL_RESP))
    {
        boost::shared_ptr<long> resptr = param->GetInt(RESULT_CODE);
        if (resptr != NULL)
        {
            int rescode = *resptr;
            // log("RESCODE = %d", rescode);
            SceneManager::getSingleton().hideLoading();
            switch (rescode)
            {
                case 0:
                    //MINHDV - leave room chat
                {
                    // log("join thanh cong");
                }
                    break;
                case 1:
                    this->currentLayer = this->typelayerGame;
                    this->goToPickRoom(SceneManager::getSingleton().getGameID());
                    this->showToast(dataManager.GetSysString(404));
                    break;
                case 2:
                    this->currentLayer = this->typelayerGame;
                    this->goToPickRoom(SceneManager::getSingleton().getGameID());

                    if (this->currTypeMoneyGo == 1){
                        auto _currScene = Director::getInstance()->getRunningScene();
                        if (!_currScene) return;
                        LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
                        popup->onBtnNapHao(NULL);
                        _currScene->addChild(popup,2000,9143);
                    }
                    else{
                        SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(965));
                    }
                    
                    
                    break;
                default:
                    break;
            }
        }
    }
	else if (cmd->compare("cmi") == 0)
	{
		if (param->GetInt("slmin") == NULL)
			return;
		int count = *param->GetInt("slmin");
	}
	else if (cmd->compare("glsrdr") == 0)
	{
		if (param->GetUtfString("lsms") == NULL) return;
		string strlstsms = *param->GetUtfString("lsms");
		vector<string> lstsms = mUtils::splitString(strlstsms, '\n');
		int money = 0;
		for (int i = 0; i < lstsms.size(); i++) {
			vector<string> dtsms = mUtils::splitString(lstsms[i], '|');
			if (dtsms.size() < 2) continue;
			if (strcmp(dtsms[1].c_str(), "") == 0 || strcmp(dtsms[0].c_str(), "") == 0)
			{
				continue;
			}
			money += atoi(dtsms[1].c_str());
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutInt("isms", atoi(dtsms[0].c_str()));
			boost::shared_ptr<IRequest> request1(new ExtensionRequest("afss", params));//sent
			GameServer::getSingleton().Send(request1);
		}
		if (money > 0){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			NativeHelper::trackNapTien(StringUtils::format("%d",money).c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			IOS::trackingNapThe(StringUtils::format("%d",money));

#endif
		}

	}
	else if (strcmp("liSMSResp", cmd->c_str()) == 0){// LIST_CARD_INFOR = "liCARDInfor";
		if (param->GetUtfString("liSMSCMD") != NULL){
			UserDefault *def = UserDefault::getInstance();
			string currDate = mUtils::getCurrentDateTime("%d/%m/%Y");
			def->setStringForKey("dateConfig", currDate);

			string lstSmss = *param->GetUtfString("liSMSCMD");//String LIST_SMS_CMD = "liSMSCMD";
			def->setStringForKey("liSMSCMD", lstSmss);

			def->flush();
		}
	}
	else if (strcmp("liCARDResp", cmd->c_str()) == 0){// LIST_CARD_INFOR = "liCARDInfor";
		if (param->GetUtfString("liCARDCMD") != NULL){
			UserDefault *def = UserDefault::getInstance();
			string currDate = mUtils::getCurrentDateTime("%d/%m/%Y");
			def->setStringForKey("dateConfig", currDate);

			string lstCards = *param->GetUtfString("liCARDCMD");//String LIST_CARD_CMD = "liCARDCMD";
			def->setStringForKey("liCARDCMD", lstCards);

			def->flush();
		}
	}
	else if (0 == cmd->compare("gaic"))
	{
		//MINHDV changes - check phonenumber and money, if it's not exist show popup add info
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf == NULL)
			return;
        if (param->GetUtfString("aP") != NULL){
            //Insert datas to textfield
            string _oldPass = *param->GetUtfString("aP");
            if (this->isMain && _oldPass == "" && !dataManager.getAppConfig().isLoginFisrt && dataManager.getAppConfig().loginType == "sp" && !dataManager.getAppConfig().isShowMatKhau && SceneManager::getSingleton().numPopup<=0){
                dataManager.getAppConfig().isShowMatKhau = true;
                LayerPopupDoiMatKhauChoiNgay* popup = LayerPopupDoiMatKhauChoiNgay::create();
                this->addChild(popup,100000);
                
            }
            else{
                dataManager.getAppConfig().isShowMatKhauDone = true;
            }
//            if (dataManager.getAppConfig().isLoginFisrt){
//                dataManager.getAppConfig().isLoginFisrt = false;
//            }
        }
		boost::shared_ptr<UserVariable> amfPtr;
		if (SceneManager::getSingleton().getCurrRoomType() == 0){

			amfPtr = mySelf->GetVariable("amf");
		}
		else{
			amfPtr = mySelf->GetVariable("amfs");
		}
		if (amfPtr == NULL)
			return;
		boost::shared_ptr<double> amf = amfPtr->GetDoubleValue();
		if (amf == NULL)
			return;
		if (*amf >= 100000)
		{
			UserDefault *userdefault = UserDefault::getInstance();
			bool isCheckMobile = userdefault->getBoolForKey("isCheckMobile", true);
			boost::shared_ptr<string> phonePtr = param->GetUtfString("aMo");
			if (phonePtr)
			{
				if (0 == phonePtr->compare("") && isCheckMobile == true)//login lan dau tien trong ngay
				{
					//					userdefault->setBoolForKey("isCheckMobile", false);
					//					LayerPopupChangeMobile* changeName = LayerPopupChangeMobile::create();
					//					changeName->setFlagPopup(LayerPopupChangeMobile::FLAG_CHANGE_PHONE);
					//					this->addChild(changeName, ZORDER_POPUP);
				}
			}
			else{
				if (isCheckMobile == true)
				{
					//					userdefault->setBoolForKey("isCheckMobile", false);
					//					LayerPopupChangeMobile* changeName = LayerPopupChangeMobile::create();
					//					changeName->setFlagPopup(LayerPopupChangeMobile::FLAG_CHANGE_PHONE);
					//					this->addChild(changeName, ZORDER_POPUP);
				}
			}
		}
	}
//	else  if (strcmp("rsfb", cmd->c_str()) == 0){
//		if (dataManager.getAppConfig().isMissionFacebookClick)
//			return;
//
//		if (param->GetUtfString("coinBonus") == NULL || param->GetInt("rc") == NULL) return;
//		SceneManager::getSingleton().hideLoading();
//		int rc = *param->GetInt("rc");
//		if (rc == 0){
//			UserDefault::getInstance()->setBoolForKey("isReceivedFacebook", true);
//			UserDefault::getInstance()->flush();
//			dataManager.getAppConfig().isreceivedRegisterFace = true;
//			string bonus = *param->GetUtfString("coinBonus");
//			auto lstbonus = mUtils::splitString(bonus, '|');
//			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
//			layerPopup->showAButton();
//			layerPopup->setCallBackFromButton(this, callfuncO_selector(SceneMain::onPopupCallBack), 3);
//			layerPopup->setMessage("Chúc mừng");
//
//
//			if (atoi(lstbonus[0].c_str()) > 0){
//				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s %s quà liên kết facebook!", lstbonus[0].c_str(), SceneManager::getSingleton().getCoinUnit().c_str()));
//				this->addChild(layerPopup, 100000, 1098);
//			}
//			else if (atoi(lstbonus[1].c_str()) > 0){
//				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s Exp quà liên kết facebook!", lstbonus[1].c_str()));
//				this->addChild(layerPopup, 100000, 1098);
//			}
//			else if (atoi(lstbonus[2].c_str()) > 0){
//				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s lần mở rương vip quà liên kết facebook!", lstbonus[2].c_str()));
//				this->addChild(layerPopup, 100000, 1098);
//			}
//			else if (atoi(lstbonus[3].c_str()) > 0){
//				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s lượt quay vòng quay may mắn quà liên kết facebook!", lstbonus[3].c_str()));
//				this->addChild(layerPopup, 100000, 1098);
//			}
//		}
//		else{
//			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
//			layerPopup->showAButton();
//			layerPopup->setCallBackFromButton(this, callfuncO_selector(SceneMain::onPopupCallBack), 3);
//			layerPopup->setMessage(dataManager.GetSysString(141));
//			layerPopup->setContentMess("Nhận thưởng thất bại! Vui lòng thử lại sau!");
//			this->addChild(layerPopup, 100000, 1098);
//
//		}
//	}
	else if (strcmp("rsilf", cmd->c_str()) == 0){
        auto scene = Director::getInstance()->getRunningScene();
        if (!scene) return;
		if ( param->GetInt("rc") == NULL) return;
		int rc = *param->GetInt("rc");
		if (rc == 0){
			//string bonus = *param->GetUtfString("coinBonus");
			//auto lstbonus = mUtils::splitString(bonus, '|');
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setCallBackFromButton(this, callfuncO_selector(SceneMain::onPopupCallBack), 3);
			layerPopup->showAButton();
			layerPopup->setMessage(dataManager.GetSysString(141));
			scene->addChild(layerPopup, 100000, 1098);

			//if (atoi(lstbonus[0].c_str()) > 0){
			layerPopup->setContentMess(dataManager.GetSysString(619));
			//			}
			//			else if (atoi(lstbonus[1].c_str()) > 0){
			//				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s Exp quà liên kết facebook!", lstbonus[1].c_str()));
			//			}
			//			else if (atoi(lstbonus[2].c_str()) > 0){
			//				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s lần mở rương vip quà liên kết facebook!", lstbonus[2].c_str()));
			//			}
			//			else if (atoi(lstbonus[3].c_str()) > 0){
			//				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s lượt quay vòng quay may mắn quà liên kết facebook!", lstbonus[3].c_str()));
			//			}
			//btn_lkfb->setVisible(false);
			UserDefault *def = UserDefault::getInstance();
			def->setBoolForKey("isLinkFacebook", true);
			dataManager.getAppConfig().isreceivedLinkFace = true;
		}
		else{
			string desc = "";
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setCallBackFromButton(this, callfuncO_selector(SceneMain::onPopupCallBack), 3);
			layerPopup->showAButton();
			layerPopup->setMessage(dataManager.GetSysString(141));

			scene->addChild(layerPopup, 100000, 1098);

			switch (rc) {
                case 1:
                    desc = dataManager.GetSysString(686);
                    layerPopup->setContentMess(desc);
                    break;
                case 2:
                    desc = dataManager.GetSysString(687);
                    layerPopup->setContentMess(desc);
                    break;
                case 3:
                    desc = dataManager.GetSysString(688);
                    layerPopup->setContentMess(desc);
                    break;
                case 4:
                    desc = dataManager.GetSysString(689);
                    layerPopup->setContentMess(desc);
                    break;
			}
		}



	}
    if (strcmp(EXT_EVENT_STICKER_GAME_NTF, cmd->c_str()) == 0){
        if (SceneManager::getSingleton().isNagaWin){
            auto stcpro = param->GetInt(FISRT_CHARGE_PROMOTION);
            if (stcpro){
                dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR = StringUtils::format("%d",*stcpro);
//                if (atoi(dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR.c_str()) > 0){
//                    this->chat->sprKhuyenMai->setVisible(true);
//                    this->chat->txtKhuyenMai->setString(StringUtils::format("+%s%",dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR.c_str())+"%");
//                }
            }
            auto stkgame = param->GetUtfString(STICKER_GAME);
            if (stkgame){
                dataManager.getAppConfig().STICKER_GAME_STR = *stkgame;
            }
            this->mLayerMainContent->setStickGame();
        }
        
    }
    
    

}

void SceneMain::gotoChatRoom()
{
	this->tagChild = BOTTOM_CHAT;
	if (this->getChildByTag(TAG_CHILD) != NULL){
		this->removeChildByTag(TAG_CHILD, true);
	}

	_layerChat = LayerChatRoom::create();
	this->addChild(_layerChat);
	_layerChat->setTag(TAG_CHILD);

	mLayerMainTop->setVisible(false);
	mLayerMainBottom->setVisible(false);
}

void SceneMain::gotoPopupSendMessage(){

}

void SceneMain::onCallbackInvitePopup(bool isAccept)
{
	if (!isAccept){
		SceneManager::getSingleton().setRejoinInfo(RejoinInfoStruct());
		return;
	}
	RejoinInfoStruct _rejoin = SceneManager::getSingleton().getRejoinInfo();
	SceneManager::getSingleton().setGameID(atoi(_rejoin.gameID.substr(0, 3).c_str()));
	SceneManager::getSingleton().gotoPickGame(atoi(_rejoin.gameID.substr(0, 3).c_str()));
}

void SceneMain::OnSmartFoxInvitation(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    return;
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueInvitation = (*ptrEvetnParams)["invitation"];
	boost::shared_ptr<Invitation::Invitation> invitation = ((boost::static_pointer_cast<Invitation::Invitation>(ptrEventParamValueInvitation)));

	// 	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = invitation->Params();
	//
	boost::shared_ptr<User> MySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	//Get PAI
	string passRoom4Invite = "";
	if (param->GetUtfString("pass") != NULL)
		passRoom4Invite = *param->GetUtfString("pass");

	if (this->getChildByTag(1234) != NULL)
	{
		//already add invite
		return;
	}

	//log("Invitee: %s Inviter: %s, gameID: %s, minbet: %s, pass: %s",
//        invitation->Invitee()->Name()->c_str(),
//        invitation->Inviter()->Name()->c_str(),
//        param->GetUtfString("gid")->c_str(),
//        param->GetUtfString("mb")->c_str(),
//        passRoom4Invite.c_str());

	string sgid = *param->GetUtfString("gid");

// 	RejoinInfoStruct joinGameInfo;
// 	joinGameInfo.gameID = sgid;
// 	joinGameInfo.roomID = atoi(param->GetUtfString("roomid")->c_str());
// 	joinGameInfo.pass = passRoom4Invite;
// 	joinGameInfo.isNeedRejoin = true;
// 	SceneManager::getSingleton().setRejoinInfo(joinGameInfo);

	if (!this->getChildByTag(1234)){
		int roomID = atoi(param->GetUtfString("roomid")->c_str());
		boost::shared_ptr<Room> ro = GameServer::getSingleton().getSmartFox()->GetRoomById(roomID);
		if (ro == NULL) return;
		string sRoomIdDisplay = mUtils::getRoomIDByName(ro->Name()->c_str());
		if (sRoomIdDisplay == "")
			sRoomIdDisplay = StringUtils::format("%d", roomID);
		LayerPopupInviteGame* _layer = LayerPopupInviteGame::create();
		_layer->setTag(1234);
		_layer->setCurrScreen(LayerPopupInviteGame::tagSceneMain);
		_layer->setLocalZOrder(1000);
		boost::shared_ptr<UserVariable> userPtr = invitation->Inviter()->GetVariable(ACC_DISPLAY_NAME);
		string userInvite = invitation->Inviter()->Name()->c_str();
		if (userPtr != NULL)
			userInvite = userPtr->GetStringValue()->c_str();
        auto _currScene = Director::getInstance()->getRunningScene();

		_layer->setDatas(userInvite, 10000, sRoomIdDisplay
			, atoi(sgid.substr(0, 3).c_str())
			, passRoom4Invite
			, atoi(param->GetUtfString("mb")->c_str())
			, "1/4", roomID);
		_currScene->addChild(_layer, 3000,9136);
	}
}

void SceneMain::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		switch (this->tagChild)
		{
		case TOP_FRIEND:
			break;
		case TOP_MAIL:
			break;
		case TOP_NOTIFICATION:
			break;
		case TOP_RANK:
			break;
		case BOTTOM_CHAT:
			break;
		default:
			if (this->getChildByTag(POPUP_THONGBAO) == NULL)
				showPopupThongbao();
			break;
		}
	}
}

void SceneMain::showPopupThongbao(){
    if (!SceneManager::getSingleton().isLogin) return;
	auto layerpoup = LayerPopupThongBao::create();
	layerpoup->setMessage(dataManager.GetSysString(567));
	layerpoup->setContentMess(dataManager.GetSysString(534));
	layerpoup->setCallBackFromButton(this, callfuncO_selector(SceneMain::onPopupCallBack), 1);
	this->addChild(layerpoup, 5000);
}



void SceneMain::showEvent(){
    if (!this->isMain) return;

    SceneManager::getSingleton().getStatusEventFromUrl();

	if (dataManager.getAppConfig().isHideBilling || !SceneManager::getSingleton().isLogin)
		return;
	this->scheduleOnce(schedule_selector(SceneMain::gotoEventAfter),2);
}
void SceneMain::gotoGuideGame(float dt){
    //anhnt only for test
 //   LayerGuide* guide = LayerGuide::create();
//    this->addChild(guide,1000000);
//    dataManager.getAppConfig().isLoginFisrt = false;
//    this->unschedule(schedule_selector(SceneMain::gotoGuideGame));
//    
//    return;
    if (dataManager.getAppConfig().isHideBilling){
        this->unschedule(schedule_selector(SceneMain::gotoGuideGame));

        return ;
    }
    boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(StringUtils::format("%d", kGameSlot)));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().hideLoading();
    if (dataManager.getAppConfig().isLoginFisrt){
        LayerGuide* guide = LayerGuide::create();
        this->addChild(guide,1000000);
        dataManager.getAppConfig().isLoginFisrt = false;
        this->unschedule(schedule_selector(SceneMain::gotoGuideGame));
    }else{
        this->showEvent();
        this->unschedule(schedule_selector(SceneMain::gotoGuideGame));
    }
}
void SceneMain::gotoEventAfter(float dt){
    if (this->currentLayer == this->typelayerPickRoom) return;
    //anhnt only fortest, test commend it
   // this->gotoDiemDanh();

    gotoEvent();
}

void SceneMain::onPopupCallBack(Ref *pSender)
{
	auto node = (Node*)pSender;
	int tag = node->getTag();

	if (tag == 3) {
		if (dataManager.getAppConfig().isLoginFisrtDay ) {
			//if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
			this->goToDiemDanh(1);
			this->removeChildByTag(1098);
			//}
			//            if (strcmp("facebook", dataManager.getAppConfig().loginType.c_str()) == 0 || strcmp("google", dataManager.getAppConfig().loginType.c_str()) == 0){
			//                UserDefault::getInstance()->setBoolForKey("isShouldShowWelcome", false);
			//            }
		}
		else if (!dataManager.getAppConfig().isLoginFisrtDay){
			this->removeChildByTag(1098);
		}
	}
	else{
		//Director::getInstance()->end();
		boost::shared_ptr<IRequest> request(new LogoutRequest());
		GameServer::getSingleton().Send(request);
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

void SceneMain::showToast(string mess)
{
	Chat *toast = Chat::create(mess, -1);
	this->addChild(toast, ZORDER_POPUP);
}
void SceneMain::goToDiemDanh(float dt){
    if (!SceneManager::getSingleton().isLogin) return;
    if (!this->isMain) return;
    SceneManager::getSingleton().showPopupVongQuay();
    this->unschedule(schedule_selector(SceneMain::goToDiemDanh));
}
void SceneMain::timeoutChatMaxtime(float dt){
	SceneManager::getSingleton().timerchat++;
	//log("timer chat couting!");
	if (SceneManager::getSingleton().timerchat == dataManager.getAppConfig().chattimeout*SceneManager::getSingleton().sec) {
		SceneManager::getSingleton().timerchat = 0;
		SceneManager::getSingleton().chatcounttime = 0;
		SceneManager::getSingleton().arrCurrentChat.clear();
		this->unschedule(schedule_selector(SceneMain::timeoutChatMaxtime));
	}
}
void SceneMain::startTimeOutChat(){
//    if (SceneManager::getSingleton().timerchat > 0){
//        this->scheduleOnce(schedule_selector(SceneMain::timeoutChatMaxtime),this,1, false);
//    }
}

//void SceneMain::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
//{
//	boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
//	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
//	boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
//	if (ptrNotifiedRoom == NULL)
//		return;
//	int roomID = ptrNotifiedRoom->Id();
//	if (ptrNotifiedRoom->GroupId()->compare(SERVICE_CHAT_ROOM) == 0)
//		return;
//	int gameID = atoi(ptrNotifiedRoom->GroupId()->c_str());
//	//log("Join Room In Chon Ban Choi ID = %d ------------------------------", gameID);    //
//	boost::shared_ptr<RoomVariable> typePtr = ptrNotifiedRoom->GetVariable("roomType");
//	if (typePtr)
//	{
//		int typeRoom = *typePtr->GetIntValue();
//		SceneManager::getSingleton().setCurrRoomType(typeRoom);
//	}
//	string strGameID = StringUtils::format("%d", gameID);
//	if (strGameID.length() > 3)
//		strGameID = strGameID.substr(0, 3);
//	int gameIDParse = atoi(strGameID.c_str());
//	//log("ID = %d", gameIDParse);
//	//SceneManager::getSingleton().gotoGame(gameIDParse);
//
//	//string gameID = StringUtils::format("%d", getGameID());
//	/*if (gameID.length() > 3)
//		gameID = gameID.substr(0, 3);*/
//	//isRejoinRoom = false;
//	////SceneManager::getSingleton().hideLoading();
//	//RejoinInfoStruct sRejoinInfoOld = SceneManager::getSingleton().getRejoinInfo();
//	//mCurrentRoomIDJoin = -1;
//	//RejoinInfoStruct rejoinInfo;
//	//SceneManager::getSingleton().setRejoinInfo(rejoinInfo);
//	////Update, khong chap nhan loi moi nua
//	//boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
//	//boost::shared_ptr<int> _a(new int(0));
//	//boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("pai", _a, VARIABLETYPE_INT));
//	//collectionUserVariable->push_back(variable);	//
//	//boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
//	//GameServer::getSingleton().Send(request);
//
//	//if (sRejoinInfoOld.roomName.length() > 0){
//	//	SceneManager::getSingleton().gotoGame(gameIDParse);
//	//	return;
//	//}
//	////if (getGameID() != kGameXiTo){
//	//SceneManager::getSingleton().gotoGame(gameIDParse);
//}

void SceneMain::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	///loi nay hay bi nen bi unschedule luon
    this->unschedule(CC_SCHEDULE_SELECTOR(ScenePickRoom::onScheduleUpdateRoomList));
//    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
//    boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
//    boost::shared_ptr<Room> roomJoin = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
//    //     if (!roomJoin->IsPasswordProtected())
//    //         SceneManager::getSingleton().setAutoCreateRoom(true);
//    int roomID = roomJoin->Id();
//    if (roomJoin->GroupId()->compare(SERVICE_CHAT_ROOM) == 0 || roomJoin->GroupId()->compare("200") == 0)
//        return;
//    int gameID = atoi(roomJoin->GroupId()->c_str());
//    SceneManager::getSingleton().setGameID(gameID);
//    //log("Join Room In Chon Ban Choi ID = %d ------------------------------", gameID);
//    string _gameID = StringUtils::format("%d", gameID);
//    if (_gameID.length() > 3)
//        _gameID = _gameID.substr(0, 3);
//    isRejoinRoom = false;
//    //SceneManager::getSingleton().hideLoading();
//    RejoinInfoStruct sRejoinInfoOld = SceneManager::getSingleton().getRejoinInfo();
//    mCurrentRoomIDJoin = -1;
//    RejoinInfoStruct rejoinInfo;
//    SceneManager::getSingleton().setRejoinInfo(rejoinInfo);
//    //Update, khong chap nhan loi moi nua
//    boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
//    boost::shared_ptr<int> _a(new int(0));
//    boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("pai", _a, VARIABLETYPE_INT));
//    collectionUserVariable->push_back(variable);    //
//    boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
//    GameServer::getSingleton().Send(request);
//
//    if (sRejoinInfoOld.roomName.length() > 0){
//        SceneManager::getSingleton().gotoGame(atoi(_gameID.c_str()));
//        return;
//    }
//    //if (getGameID() != kGameXiTo){
//    SceneManager::getSingleton().gotoGame(atoi(_gameID.c_str()));
}

void SceneMain::OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	//log("Join Room Error");
	//SceneManager::getSingleton().hideLoading();
	RejoinInfoStruct rejoinInfo;
	SceneManager::getSingleton().setRejoinInfo(rejoinInfo);
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueErrorMessage = (*ptrEventParams)["errorMessage"];
	boost::shared_ptr<string> ptrErrorMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueErrorMessage);
	//
	boost::shared_ptr<void> ptrEventParamValueErrorCode = (*ptrEventParams)["errorCode"];
	boost::shared_ptr<short> ptrErrorCode = ((boost::static_pointer_cast<short>))(ptrEventParamValueErrorCode);
	//
	//log("OnSmartFoxRoomJoinError - errorCode: %d", *ptrErrorCode);
	boost::shared_ptr<string> message(new string("Join Room Failure: " + *ptrErrorMessage));
	//Error code = 20 is Room is fulls
    boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
    boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
    if (ptrNotifiedRoom == NULL) {
        if (*ptrErrorCode == 21){
            this->currentLayer = this->typelayerGame;

            if (SceneManager::getSingleton().getGameID() == kGameXocDia || SceneManager::getSingleton().getGameID() == kGameBauTom || SceneManager::getSingleton().getGameID() == kGameSlot){
                this->gotoPickGame();
                return;
            }
            this->goToPickRoom(SceneManager::getSingleton().getGameID());
            SceneManager::getSingleton().showToast(dataManager.GetSysString(181));
        }
        return;
    }
    
    if (ptrNotifiedRoom->GroupId()->compare(SERVICE_CHAT_ROOM) == 0
        || ptrNotifiedRoom->Id() == 118 || ptrNotifiedRoom->GroupId()->compare("200") == 0 || ptrNotifiedRoom->Id() == 200)
        return;

	string mess = mUtils::getErrorSmartfoxExplain(*ptrErrorCode);

	//log("Join Room Failure: %s", mess.c_str());
	//Neu ma rejoin room, thi se ko thong bao gi ca
   // if (!isRejoinRoom)
   // {
    if (*ptrErrorCode == 19) //tam an err code 19
    {
        this->showToast(mess.c_str());
        return;
    }
  //  }

	isRejoinRoom = false;
	//
	switch (*ptrErrorCode){
	case 20:
		//Room is full
		if (mCurrentRoomIDJoin != -1){
			int a = -1;
			if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()){
				a = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->Id();
			}
			//if (a == -1) return;
			boost::shared_ptr<long int> id2Left(new long int(a));
			//
			boost::shared_ptr<IRequest> request(new JoinRoomRequest(mCurrentRoomIDJoin, "", id2Left, true));
			GameServer::getSingleton().Send(request);
			mCurrentRoomIDJoin = -1;
		}
		break;
	case 96:
		SceneManager::getSingleton().showLoading();
		break;
	default:
		mCurrentRoomIDJoin = -1;
		break;
	}
//    this->currentLayer = this->typelayerGame;
//
//    if (SceneManager::getSingleton().getGameID() == kGameXocDia || SceneManager::getSingleton().getGameID() == kGameBauTom || SceneManager::getSingleton().getGameID() == kGameSlot){
//        this->gotoPickGame();
//        return;
//    }
//    this->goToPickRoom(SceneManager::getSingleton().getGameID());
}
void SceneMain::gotoDiemDanh(){
    //anhnt test
    if (!SceneManager::getSingleton().isNagaWin) return;
    if (!dataManager.getAppConfig().isLoginFisrtDay) return;
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    LayerBaoDanhNew* baodanh = LayerBaoDanhNew::create();
    _currScene->addChild(baodanh, 20000);
    dataManager.getAppConfig().isLoginFisrtDay = false;
}
