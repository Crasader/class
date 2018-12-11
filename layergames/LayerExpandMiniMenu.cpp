//
//  LayerExpandMiniMenu.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/1/16.
//
//

#include "LayerExpandMiniMenu.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LayerPlayGameTXMini.h"
#include "../SceneManager.h"
#include "ChanUtils.h"
#include "TaiXiuMessageDefs.h"
#include "PhomMessDef.h"
#include "LayerPlayGameMiniPoker.h"
#include "LayerMenuMiniGame.h"
#include "LayerTCC_PopupMain.h"
#include "LayerCaoThapPopupGame.h"
#include "../mUtils.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerExpandMiniMenu::LayerExpandMiniMenu()
{

}


LayerExpandMiniMenu::~LayerExpandMiniMenu()
{

}

bool LayerExpandMiniMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}

    visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerExpandMenuMiniGame.csb");
	auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	pnlB->setSwallowTouches(false);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
//	if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
		sizeAdd = ChanUtils::getSizePos();

//    }
//    else{
//        sizeAdd = Size(0, 0);
//    }
	Size sizeAdd2 = Size(0,0);
	rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
	pnlBg->setSwallowTouches(false);
	this->imgBg = static_cast<ImageView*>(pnlBg->getChildByName("imgBg"));

	auto btnMenu = static_cast<Button*>(this->imgBg->getChildByName("btnMenu"));
	if (btnMenu) {
		btnMenu->addClickEventListener(CC_CALLBACK_1(LayerExpandMiniMenu::onBtnMenu, this));
	}

	this->btnTX = static_cast<Button*>(this->imgBg->getChildByName("btnTX"));
	if (this->btnTX) {
        this->btnTX->setEnabled(!SceneManager::getSingleton().getIsShowTX());
		this->btnTX->addClickEventListener(CC_CALLBACK_1(LayerExpandMiniMenu::onBtnTX, this));
	}
	this->btnPK = static_cast<Button*>(this->imgBg->getChildByName("btnPK"));
	if (this->btnPK) {
        this->btnPK->setEnabled(!SceneManager::getSingleton().getIsShowPK());

		this->btnPK->addClickEventListener(CC_CALLBACK_1(LayerExpandMiniMenu::onBtnPK, this));
	}

	this->btnTCC = static_cast<Button*>(this->imgBg->getChildByName("btnTCC"));
	if (this->btnTCC) {
        //this->btnTCC->setEnabled(!SceneManager::getSingleton().getIsShowTCC());

		this->btnTCC->addClickEventListener(CC_CALLBACK_1(LayerExpandMiniMenu::onBtnTCC, this));
	}


	//if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
	auto lstmini = mUtils::splitString(dataManager.getAppConfig().gamemini, '_');
	for (int i = 0; i < lstmini.size(); i++){
		if (strcmp(lstmini[i].c_str(), "128") == 0){
			btnTX->setVisible(false);
		}
		if (strcmp(lstmini[i].c_str(), "130") == 0){
			btnPK->setVisible(false);
		}
		if (strcmp(lstmini[i].c_str(), "131") == 0){
			btnTCC->setVisible(false);
		}
	}
	//}

	this->imgBg->setScale(0.1);
	this->imgBg->runAction(ScaleTo::create(0.3, 1));
	this->imgBg->runAction(Sequence::create(RotateTo::create(0.1, 180), RotateTo::create(0.2, 360), NULL));

	this->imgBGTimer = static_cast<ImageView*>(this->imgBg->getChildByName("imgBGTimer"));
	this->txtTimer = static_cast<Text*>(this->imgBGTimer->getChildByName("txtTimer"));
	this->txtTimer->setString("");
	this->imgBGTimer->setVisible(false);

	this->mQueueMsg = new ThreadQueueMsg();

	this->mQueueMsg->setFunCallBack(this, callfunc_selector(LayerExpandMiniMenu::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerExpandMiniMenu::updateEvent),this,1/60, false);

	GameServer::getSingleton().addListeners(this);


	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt("tm", 0);
	boost::shared_ptr<IRequest> req(new ExtensionRequest("txj", params));
	GameServer::getSingleton().Send(req);



	return true;
}
void LayerExpandMiniMenu::onBtnTX(Ref* pSender) {
	this->btnTCC->setEnabled(false);
	this->btnPK->setEnabled(false);

//	Button* btnSender = ((Button*)pSender);
//	LoaderNode* loaderNode = LoaderNode::create();
//	btnSender->addChild(loaderNode);
//	loaderNode->setTagCallBack(1);
//	loaderNode->setCallBack(CC_CALLBACK_2(LayerExpandMiniMenu::callBackFromLoader, this));
//	loaderNode->startCheckUpdate();
    
	 	LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
    
	 	LayerExpandMiniMenu* parent = (LayerExpandMiniMenu*)this->getParent();
	 	if (parent){
	 		parent->addChild(layerPlayerGame);
            layerPlayerGame->shouldShowHuongDan();
            //parent->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd.height));
            SceneManager::getSingleton().setIsShowTX(true);
	 		this->closeMenu();
	 	}
}
void LayerExpandMiniMenu::onBtnPK(Ref* pSender){
	this->btnTCC->setEnabled(false);
	this->btnTX->setEnabled(false);

//	Button* btnSender = ((Button*)pSender);
//	LoaderNode* loaderNode = LoaderNode::create();
//	btnSender->addChild(loaderNode);
//	loaderNode->setTagCallBack(0);
//	loaderNode->setCallBack(CC_CALLBACK_2(LayerExpandMiniMenu::callBackFromLoader, this));
//	loaderNode->startCheckUpdate();
	 	LayerPlayGameMiniPoker* layerPlayerGame = LayerPlayGameMiniPoker::create();
	 
	 	LayerExpandMiniMenu* parent = (LayerExpandMiniMenu*)this->getParent();
	 	if (parent){
	 		parent->addChild(layerPlayerGame);
            SceneManager::getSingleton().setIsShowPK(true);

	 		this->closeMenu();
	 	}
}
void LayerExpandMiniMenu::onBtnTCC(Ref* pSender) {
	this->btnTX->setEnabled(false);
	this->btnPK->setEnabled(false);

//	Button* btnSender = ((Button*)pSender);
//	LoaderNode* loaderNode = LoaderNode::create();
//	btnSender->addChild(loaderNode);
//	loaderNode->setTagCallBack(2);
//	loaderNode->setCallBack(CC_CALLBACK_2(LayerExpandMiniMenu::callBackFromLoader, this));
//	loaderNode->startCheckUpdate();

	 	LayerTCC_PopupMain* layerTCC = LayerTCC_PopupMain::create();
	 	LayerExpandMiniMenu* parent = (LayerExpandMiniMenu*)this->getParent();
	 	if (parent){
	 		Size sizeAdd;
	 		sizeAdd = ChanUtils::getSizePos();
	 		// 		if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
	 		// 			//layerTCC->setPositionY(layerTCC->getPositionY() - sizeAdd.height);
	 		// 		}
	 		// 		else{
	 		// 			layerTCC->setPositionY(layerTCC->getPositionY() + sizeAdd.height);
	 		// 
	 		// 		}
           // SceneManager::getSingleton().setIsShowTCC(true);

	 		parent->addChild(layerTCC);
	 		this->closeMenu();
	 	}
}
void LayerExpandMiniMenu::onBtnMenu(Ref* pSender) {
	this->imgBg->runAction(ScaleTo::create(0.3, 0.1));
	this->imgBg->runAction(Sequence::create(RotateTo::create(0.2, -180), RotateTo::create(0.1, -360), CallFunc::create(CC_CALLBACK_0(LayerExpandMiniMenu::closeMenu, this)), NULL));
}
void LayerExpandMiniMenu::closeMenu() {
	LayerMenuMiniGame* parent = (LayerMenuMiniGame*)this->getParent();
	if (parent)
		parent->menu->setVisible(true);
	this->removeFromParent();
}
bool LayerExpandMiniMenu::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

	return true;
}
void LayerExpandMiniMenu::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){

}
void LayerExpandMiniMenu::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){

	Size sizeAdd;

	sizeAdd = ChanUtils::getSizePos();

	auto target = pEvent->getCurrentTarget();

	auto locationInNode = pTouch->getLocation();
	//var s = target.getContentSize();
	auto rect = this->imgBg->getBoundingBox();

	//Check the click area
	if (rect.containsPoint(locationInNode)) {
		//return true;
	}
	else {
		this->imgBg->runAction(ScaleTo::create(0.3, 0.1));
		this->imgBg->runAction(Sequence::create(RotateTo::create(0.2, -180), RotateTo::create(0.1, -360), CallFunc::create(CC_CALLBACK_0(LayerExpandMiniMenu::closeMenu, this)), NULL));
	}

}
void LayerExpandMiniMenu::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	pObjs->PutUtfString("_cmd_", *cmd);
	this->mQueueMsg->addQueue(pObjs);
}

void LayerExpandMiniMenu::OnExtensionResponseProcess(){
	this->param = this->mQueueMsg->frontQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");

	if (0 == cmd->compare(EXT_EVENT_START_TXMINI)) {
		this->event_EXT_EVENT_START();
	}
	else if (0 == cmd->compare(EXT_EVENT_END_TXMINI)) {
		this->event_EXT_EVENT_END();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF_TXMINI)) {
		this->event_EXT_EVENT_GAME_STATE_NTF();
	}
	this->mQueueMsg->deQueue();
}

void LayerExpandMiniMenu::event_EXT_EVENT_START(){
	long playerTime = *this->param->GetInt("pt");
	this->txtTimer->setVisible("");
	this->PlayTurnTime = playerTime / 1000;
	this->UpdateTimeStartGame();

}
void LayerExpandMiniMenu::UpdateTimeStartGame(){
	this->imgBGTimer->setVisible(true);
	this->imgBGTimer->stopAllActions();
	this->imgBGTimer->setOpacity(255);
	this->txtTimer->setVisible(true);
	this->StopMyTimer();
	this->TimerCountDown = this->PlayTurnTime;

	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerExpandMiniMenu::updateTimer),this,1, false);

}
void LayerExpandMiniMenu::updateTimer(float dt) {
	this->TimerCountDown -= 1;
	if (this->TimerCountDown < 0) {
		this->StopMyTimer();
		return;
	}
	if (this->TimerCountDown <= 5){
		this->imgBGTimer->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3), FadeIn::create(0.3), NULL)));
	}
	string str = StringUtils::format("%d", this->TimerCountDown);
	if (this->txtTimer != NULL) {
		this->txtTimer->setString(str);
		this->imgBGTimer->setVisible(true);
	}

	if (this->TimerCountDown == 0) {
		// this.imgBGTimer.setVisible(false);
		this->StopMyTimer();
	}
}
void LayerExpandMiniMenu::StopMyTimer() {
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerExpandMiniMenu::updateTimer),this);

	//this.txtTimer.setVisible(false);
	this->TimerCountDown = this->PlayTurnTime;
}
void LayerExpandMiniMenu::event_EXT_EVENT_END(){
	this->StopMyTimer();
}
void LayerExpandMiniMenu::event_EXT_EVENT_GAME_STATE_NTF(){
	boost::shared_ptr<long> gtime = this->param->GetInt(EXT_FIELD_GAME_TIME);
	boost::shared_ptr<long> gstate = this->param->GetInt(EXT_FIELD_GAME_PLAY_STATE);

	if (gstate != NULL) {
		if (0 == *gstate) {
			this->imgBGTimer->setVisible(false);

		}
		else {
			// this->CurrentGameState = true;

			this->StopMyTimer();
			//Update timer
			this->TimerCountDown = *gtime;
			this->imgBGTimer->setVisible(true);
			this->imgBGTimer->stopAllActions();
			this->imgBGTimer->setOpacity(255);
			this->txtTimer->setVisible(true);

			// this.schedule(this.UpdateHayDatcuoc, 0.5)
			Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerExpandMiniMenu::updateTimer),this,1, false);
		}
	}
}
void LayerExpandMiniMenu::updateEvent(float dt){
	if (this->mQueueMsg->isQueueEmpty())
		return;

	this->mQueueMsg->run();
}

void LayerExpandMiniMenu::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerExpandMiniMenu::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(LayerExpandMiniMenu::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(LayerExpandMiniMenu::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
	//
	//    GameServer::getSingleton().addListeners(this);
}

void LayerExpandMiniMenu::onExit()
{
	Layer::onExit();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(listener);
	GameServer::getSingleton().removeListeners(this);
}

void LayerExpandMiniMenu::callBackFromLoader(LoaderNode* sender, int tagButton)
{
	this->btnPK->setEnabled(true);
	this->btnTX->setEnabled(true);
	this->btnTCC->setEnabled(true);
	switch (tagButton)
	{
	case 0:
	{
			  LayerPlayGameMiniPoker* layerPlayerGame = LayerPlayGameMiniPoker::create();

			  LayerExpandMiniMenu* parent = (LayerExpandMiniMenu*)this->getParent();
			  if (parent){
				  parent->addChild(layerPlayerGame);
				  this->closeMenu();
			  }
	}
		break;
	case 1:
	{
			  LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
			  LayerExpandMiniMenu* parent = (LayerExpandMiniMenu*)this->getParent();
			  if (parent){
				  parent->addChild(layerPlayerGame);
				  this->closeMenu();
			  }
	}
		break;

	case 2:
	{
			  LayerTCC_PopupMain* layerTCC = LayerTCC_PopupMain::create();
			  LayerExpandMiniMenu* parent = (LayerExpandMiniMenu*)this->getParent();
			  if (parent){
				  parent->addChild(layerTCC);
				  this->closeMenu();
			  }
	}
		break;
	default:
		break;
	}
}


const char* sceneManifests[] = { "minipoker.manifest", "taixiumini.manifest", "tomcuaca.manifest" };
const char* storagePaths[] = { "minipoker/", "taixiumini/", "tomcuaca/" };

LoaderNode::LoaderNode()
{

}

LoaderNode::~LoaderNode()
{

}

bool LoaderNode::init()
{
	if (!Layer::init())
		return false;

	return true;
}

void LoaderNode::onEnter()
{
	Layer::onEnter();
	auto _listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = CC_CALLBACK_2(LoaderNode::onTouchBegan, this);
	_listener->onTouchEnded = CC_CALLBACK_2(LoaderNode::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void LoaderNode::onExit()
{
	Layer::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool LoaderNode::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto target = unused_event->getCurrentTarget();
	auto s = target->getContentSize();
	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	auto rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
		return true;
	else
		return false;
}

void LoaderNode::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void LoaderNode::setCallBack(const loaderCallback& callback)
{
	this->_callback = callback;
}

void LoaderNode::startCheckUpdate()
{
	// 	this->createLoadUI();
	// 	return;
	int idx = this->tagCb;
	string manifestPath = sceneManifests[idx];
	string storagePath = FileUtils::getInstance()->getWritablePath() + storagePaths[idx];
	AssetsManagerEx* _am = AssetsManagerEx::create(manifestPath, storagePath);
	_am->retain();
	if (!_am->getLocalManifest()->isLoaded())
	{
		SceneManager::getSingleton().showToast("Fail to update assets, step skipped.");
		return;
	}
	else{
		int testIndex = 0;
		this->createLoadUI();
		cocos2d::extension::EventListenerAssetsManagerEx *amListener = cocos2d::extension::EventListenerAssetsManagerEx::create(_am, [=](EventAssetsManagerEx* event){
			static int failCount = 0;
			switch (event->getEventCode())
			{
			case EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:
			{
																			 SceneManager::getSingleton().showToast("No local manifest file found, skip assets update.");
																			 //this->onLoadEnd();
			}
				break;
			case EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION:
			{
																		string assetID = event->getAssetId();
																		float percent = event->getPercent();
																		string str = "";
																		if (assetID == AssetsManagerEx::VERSION_ID)
																		{
																			//str = StringUtils::format("Version file: %.2f", percent) + "%";
																		}
																		else if (assetID == AssetsManagerEx::MANIFEST_ID)
																		{
																			//str = StringUtils::format("Manifest file: %.2f", percent) + "%";
																		}
																		else
																		{
																			str = StringUtils::format("%.2f", percent) + "%";
																			//CCLOG("%.2f Percent", percent);
																		}
																		lbTimer->setString(str);
																		bgTimer->setPercentage(percent);
			}
				break;
			case EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:
			case EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:
			{
																		  SceneManager::getSingleton().showToast("Fail to download manifest file, update skipped.");
																		  //this->onLoadEnd();
			}
				break;
			case EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:
			case EventAssetsManagerEx::EventCode::UPDATE_FINISHED:
			{
																	 CCLOG("Update finished. %s", event->getMessage().c_str());
																	 lbTimer->setString("");
																	 bgTimer->setVisible(false);
																	 this->onLoadEnd();
			}
				break;
			case EventAssetsManagerEx::EventCode::UPDATE_FAILED:
			{
																   SceneManager::getSingleton().showToast(StringUtils::format("Update failed. %s", event->getMessage().c_str()));

																   failCount++;
																   if (failCount < 5)
																   {
																	   _am->downloadFailedAssets();
																   }
																   else
																   {
																	   SceneManager::getSingleton().showToast("Reach maximum fail count, exit update process");
																	   failCount = 0;
																	   //this->onLoadEnd();
																   }
			}
				break;
			case EventAssetsManagerEx::EventCode::ERROR_UPDATING:
			{
																	SceneManager::getSingleton().showToast(StringUtils::format("Asset %s : %s", event->getAssetId().c_str(), event->getMessage().c_str()));
			}
				break;
			case EventAssetsManagerEx::EventCode::ERROR_DECOMPRESS:
			{
																	  SceneManager::getSingleton().showToast(StringUtils::format("%s", event->getMessage().c_str()));
			}
				break;

			default:
				break;
			}

		});

		Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(amListener, 1);
		_am->update();
	}
}

void LoaderNode::onLoadEnd()
{
	if (_callback != nullptr)
	{
		this->runAction(Sequence::create(CallFunc::create([=]{
			_callback(this, this->tagCb);
		}), RemoveSelf::create(), NULL));
	}
	else{
		this->removeFromParentAndCleanup(true);
	}
}

void LoaderNode::createLoadUI()
{
	Size parentSize = Size(179, 177);
	Size bgSize = Size(613, 613);
	Size loadingSize = Size(151, 151);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(Vec2(parentSize.width / 2, parentSize.height / 2));

	Sprite* bg = Sprite::create("bg-minigame.png");
	bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bg->setScale(parentSize.width / bgSize.width, parentSize.height / bgSize.height);
	bg->setPosition(Vec2::ZERO);
	this->addChild(bg);

	this->bgTimer = ProgressTimer::create(Sprite::create("frame_time.png"));
	this->bgTimer->setScale(parentSize.width / loadingSize.width, parentSize.height / loadingSize.height);
	this->bgTimer->setType(ProgressTimer::Type::RADIAL);
	this->bgTimer->setPercentage(0);
	this->bgTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->bgTimer->setPosition(Vec2::ZERO);
	this->addChild(this->bgTimer);
	this->setContentSize(parentSize);

//    this->lbTimer = Label::createWithTTF("", BOLD_FONT, 40);
//    this->lbTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    this->lbTimer->setPosition(Vec2::ZERO);
//    this->addChild(lbTimer);
}
