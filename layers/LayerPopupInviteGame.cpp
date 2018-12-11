#include "LayerPopupInviteGame.h"
#include "cocostudio/CocoStudio.h"

#include "Requests/JoinRoomRequest.h"

#include "../GameServer.h"
#include "../scenes/ScenePickRoom.h"
#include "../scenes/SceneMain.h"

#include "../mUtils.h"
#include "layergames/PhomMessDef.h"
#include "../cotuong/ServerMessageDef.h"
#include "cotuong/gameUtils.h"


USING_NS_CC;

LayerPopupInviteGame::LayerPopupInviteGame()
{
	//this->listener = NULL;
}


LayerPopupInviteGame::~LayerPopupInviteGame()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupInviteGame::autoClosePopup),this);
}
bool LayerPopupInviteGame::init()
{
	if (!Layer::init())
	{
		return false;
	}

	setCurrScreen(-1);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	string csbPath = "LayerPopupMoiChoi.csb";
	int currGameID = SceneManager::getSingleton().getGameID();
	if (currGameID == kGameCoUp || currGameID == kGameCoTuong)
		csbPath = "LayerPopupMoiChoiChess.csb";
	auto rootNode = CSLoader::getInstance()->createNode(csbPath);
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		pnlBg->setAnchorPoint(Point::ZERO);
		pnlBg->setPosition(Point::ZERO);
	}
	else{
		rootNode->setAnchorPoint(Point(0.5, 0.5));
		rootNode->setPosition(Point(visibleSize / 2));
	}

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);
    auto lblTitle = static_cast<Text*> (rootNode->getChildByName("title_moichoi"));
    lblTitle->setString(dataManager.GetSysString(934));
    auto label_moichoi_text = static_cast<Text*> (rootNode->getChildByName("label_moichoi_text"));
    try {
    label_moichoi_text->setString(dataManager.GetSysString(935));
    }
    catch (...){
       // log("Unknow");
    }
	//add event listenner
	auto btnReject = dynamic_cast<Button*>(rootNode->getChildByName("btnReject"));
	if (btnReject != NULL){
		btnReject->addClickEventListener(CC_CALLBACK_1(LayerPopupInviteGame::onButtonDeny, this));
        btnReject->setTitleText(dataManager.GetSysString(936));
	}
	auto btnRejectAll = dynamic_cast<Button*>(rootNode->getChildByName("btnRejectAll"));
	if (btnRejectAll != NULL){
		btnRejectAll->addClickEventListener(CC_CALLBACK_1(LayerPopupInviteGame::onButtonDenyAll, this));
        btnRejectAll->setTitleText(dataManager.GetSysString(937));
	}
	auto btnAccept = dynamic_cast<Button*>(rootNode->getChildByName("btnAccept"));
	if (btnAccept != NULL){
		btnAccept->addClickEventListener(CC_CALLBACK_1(LayerPopupInviteGame::onButtonAccept, this));
        btnAccept->setTitleText(dataManager.GetSysString(889));
	}
	lblUserName1 = dynamic_cast<Text*>(rootNode->getChildByName("lblUserName1"));
	lblUserName2 = dynamic_cast<Text*>(rootNode->getChildByName("lblUserName2"));
	lblGameName = dynamic_cast<Text*>(rootNode->getChildByName("lblGameName"));
    //imgCoin = dynamic_cast<ImageView*>(rootNode->getChildByName("imgCoin"));
	// 	lblRoomID = dynamic_cast<Text*>(rootNode->getChildByName("lblRoomID"));
	// 	lblBetValue = dynamic_cast<Text*>(rootNode->getChildByName("lblBetValue"));
	// 	lblPlayers = dynamic_cast<Text*>(rootNode->getChildByName("lblPlayers"));
    int roomType = SceneManager::getSingleton().getCurrRoomType();
//<<<<<<< HEAD
//	if (currGameID != kGameCoTuong && currGameID != kGameCoUp)
//	{
//		if (roomType == 0){
//			imgCoin->loadTexture("profile-coin.png");
//		}
//		else{
//			imgCoin->loadTexture("button_xu.png");
//
//		}
//	}
//=======
//	if (imgCoin != NULL)
//	{
//		if (currGameID != kGameCoTuong && currGameID != kGameCoUp)
//		{
//			if (roomType == 0){
//				imgCoin->loadTexture("profile-coin.png");
//			}
//			else{
//				imgCoin->loadTexture("button_xu.png");
//
//			}
//		}
//	}
//	
//>>>>>>> 641da2c1901b292cf2662b27306bd332f97f9f11
    
	lblGold = dynamic_cast<Text*>(rootNode->getChildByName("lblGold"));
	if (lblGold)
	{
        if (SceneManager::getSingleton().getCurrRoomType() == 0){
            boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
            if (mySelf != NULL) {
                boost::shared_ptr<UserVariable> amf_ptr = mySelf->GetVariable("amf");
                if (amf_ptr != NULL)
                {
                    double amf = *amf_ptr->GetDoubleValue();
                    lblGold->setString(formatMoneySymbol(amf));
                }
            }
            
        }else{
            boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
            if (mySelf != NULL) {
                boost::shared_ptr<UserVariable> amf_ptr = mySelf->GetVariable("amfs");
                if (amf_ptr != NULL)
                {
                    double amf = *amf_ptr->GetDoubleValue();
                    lblGold->setString(formatMoneySymbol(amf));
                }
            }

        }
		
	}
	Text* lblSilver = (Text*)rootNode->getChildByName("lblSilver");
	if (lblSilver)
	{
		double silverUser = GameUtils::getAmfSilverUser(GameUtils::getMyName());
		lblSilver->setString(mUtils::convertMoneyEx(silverUser));

		double goldUser = GameUtils::getAmfGoldUser(GameUtils::getMyName());
		lblGold->setString(mUtils::convertMoneyEx(goldUser));
	}

	avtVip = static_cast<ImageView*>(rootNode->getChildByName("imgAvatarVip"));
	avtVip->setVisible(false);
	//this->scheduleOnce(schedule_selector(LayerPopupInviteGame::autoClosePopup), 10);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupInviteGame::autoClosePopup), this, 10, 1, 10, false);

	return true;
}

void LayerPopupInviteGame::onButtonAccept(Ref* pSender)
{
	// 	if (getCurrScreen() == tagSceneMain){
	// 		SceneMain* _scene1 = static_cast<SceneMain*>(this->getParent());
	// 		//for SceneMain
	// 		_scene1->onCallbackInvitePopup(true);
	// 		return;
	// 	}
	// 	boost::shared_ptr<IRequest> request(new JoinRoomRequest(mRoomID4Invite, mPassRoom4Invite));
	// 	GameServer::getSingleton().Send(request);
    
    int roomType = SceneManager::getSingleton().getCurrRoomType();


	if (listener == NULL)
	{
		int roomType = SceneManager::getSingleton().getCurrRoomType();
		SceneManager::getSingleton().showLoading();
		boost::shared_ptr<ISFSObject> param(new SFSObject());
		int _gameID = SceneManager::getSingleton().getGameID();
		param->PutInt(AMOUNT_BET, -1);
		param->PutUtfString(GAME_ID, StringUtils::format("%d", _gameID));
		param->PutInt(EXT_FIELD_ROOM_TYPE, roomType);
		boost::shared_ptr<IRequest> request(new ExtensionRequest(REQUEST_AUTO_JOIN_ROOM, param));
		GameServer::getSingleton().Send(request);
		back2PickGame(false);
	}
	else{
		this->runAction(CallFunc::create(this->listener));
	}
    SceneMain::getSingleton().prepareToPickGame();
}

void LayerPopupInviteGame::onButtonDeny(Ref* pSender)
{
	if (getCurrScreen() == tagSceneMain){
		SceneMain* _scene1 = static_cast<SceneMain*>(this->getParent());
		//for SceneMain
		_scene1->onCallbackInvitePopup(false);
		return;
	}
	back2PickGame(false);
}

void LayerPopupInviteGame::onButtonDenyAll(Ref* pSender)
{
	if (getCurrScreen() == tagSceneMain){
		SceneMain* _scene1 = static_cast<SceneMain*>(this->getParent());
		//for SceneMain
		_scene1->onCallbackInvitePopup(false);
		return;
	}
    UserDefault *def = UserDefault::getInstance();
    def->setBoolForKey("denyAllInvite", false);
    def->flush();
	back2PickGame(true);

}

bool LayerPopupInviteGame::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupInviteGame::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupInviteGame::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerPopupInviteGame::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerPopupInviteGame::setDatas(string userID, long userMoney, string roomName, int gameID, string pass, long betValue, string players, int roomID)
{
	mRoomID4Invite = roomID;
	mPassRoom4Invite = pass;
	string myName = SceneManager::getSingleton().getMyName();
	std::string _myName = "";
	(myName.length() > 20) ? _myName = myName.substr(0, 20) + "..." : _myName = myName;
	std::string _userID = "";
	(userID.length() > 15) ? _userID = userID.substr(0, 15) + "..." : _userID = userID;

	lblUserName1->setString(_userID);
	lblUserName2->setString(_myName);
	//lblBetValue->setString(*mUtils::format_money(betValue));
	lblGameName->setString(*mUtils::getGameNameByID(gameID));
    
    int roomType = SceneManager::getSingleton().getCurrRoomType();
    
    /*if (roomType == 0){
        imgCoin->loadTexture("moc3-icon-moc.png");
    }else{
        imgCoin->loadTexture("moc3-icon-hao.png");
        
    }*/
    
	//lblGold->setString(*mUtils::format_money(userMoney));
	///lblPlayers->setString(players);
	//lblRoomID->setString(roomName);
}

void LayerPopupInviteGame::back2PickGame(bool denyAll)
{
	ScenePickRoom* _curScene = dynamic_cast<ScenePickRoom*> (this->getParent());
	if (_curScene){
		if (denyAll)
			_curScene->setDenyAllInvite();
		else
			_curScene->continueInvitePlay();
	}
	this->removeFromParentAndCleanup(true);
}

void LayerPopupInviteGame::autoClosePopup(float dt)
{
	back2PickGame(false);
}

void LayerPopupInviteGame::showFakeInfo(int gameID)
{
	string myName = SceneManager::getSingleton().getMyName();
	std::string _myName = formatUserName(myName, 15);
	lblUserName1->setString(dataManager.GetSysString(15));
	lblUserName2->setString(_myName);
	lblGameName->setString(*mUtils::getGameNameByID(gameID));
}

void LayerPopupInviteGame::setClickCallBack(const std::function<void()> &listener)
{
	this->listener = listener;
}
