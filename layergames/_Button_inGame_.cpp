#include "Requests/LeaveRoomRequest.h"
#include "Requests/UnsubscribeRoomGroupRequest.h"
#include "Requests/SetRoomVariablesRequest.h"
#include "_Button_inGame_.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "Requests/PublicMessageRequest.h"
#include "ChanUtils.h"
#include "../layers/LayerPopupRecharge.h"
#include "_Chat_.h"
#include "GameServer.h"
#include "PhomMessDef.h"
#include "layers/LayerGuideInGame.h"
#include "../layers/LayerHuThuong.h"
#include "LayerGameMauBinh.h"
#include "LayerGameXocDia.h"
#include "LayerAvatarPoker.h"
#include "Requests/SubscribeRoomGroupRequest.h"

#include "../scenes/SceneMain.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "NativeHelper.h"
#endif
using namespace Sfs2X;

bool LayerButtonInGame::init()
{
	if (!Layer::init())
		return false;

	isChauRia = false;
	quitMess = dataManager.GetSysString(236);
	Size visibleSize = Director::getInstance()->getVisibleSize();

	this->setAnchorPoint(Point(0, 0));
	this->setPosition(Point(0, 0));

	int widButton2 = 125;
	int heiButton = 125;
	int dis = 30;
	int w_coin = 82;
	float ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
	gameID = SceneManager::getSingleton().getGameID();
    sizeAdd = ChanUtils::getSizePos();

//    if (gameID == kGameChan || gameID == kGameXiTo)
//        sizeAdd = Vec2(0,0);
	Size winSize = Director::getInstance()->getVisibleSize();
	Vec2 posSignal = Vec2(winSize.width - widButton2 + 20, winSize.height - heiButton / 1.5 - 150);
	//init node
	Button *btnExitGame = createButtonWith_Style_Position(1, Vec2(dis, DESIGN_RESOLUTION_HEIGHT - heiButton - dis / 2));
	Button* btnMenu = createButtonWith_Style_Position(5, Vec2(dis, dis));
	Button *btnChat = this->createButtonWith_Style_Position(3, Vec2(dis + btnMenu->getContentSize().width + 20, dis));
    if (dataManager.getAppConfig().is24h)
    {
        btnChat->setVisible(false);
    }
    
   
    this->btnListCai = createButtonWith_Style_Position(6, Vec2(visibleSize.width - 140,
                                                               visibleSize.height - 240));
	this->btnInvite = createButtonWith_Style_Position(7, Vec2(btnChat->getPositionX() + btnChat->getContentSize().width + 15, btnChat->getPositionY()));
	Button* btnHelp = Button::create();
	bagItem = BagItem::create();
	signalNetwork = Sprite::create("signal-good.png");
	this->layerChatInGame = LayerMocChatInGame::create();
	this->layerChatInGame->setIsPickRoom(false);
	this->popupButtonIngame = LayerPopupMenuInGame::create();
	layerSettingInGameMain = LayerSettingInGameMain::create();
	Size sizeChicken = Size(102, 102);
	btnCapture = Button::create("moc3-btnCamera.png", "moc3-btnCamera.png", "moc3-btnCamera.png");
	lblPing = Label::createWithTTF("100 ms", kBoldFont, 25);
	//set tag button
	btnMenu->setTag(TAG_LB_BTN_BACK);
	this->popupButtonIngame->setTag(TAG_POPUP_MENU);
	btnChat->setTag(TAG_LB_BTN_CHAT);

	//add event
	btnMenu->addTouchEventListener(CC_CALLBACK_2(LayerButtonInGame::eventTouchBtnMenu, this));
	btnChat->addTouchEventListener(CC_CALLBACK_2(LayerButtonInGame::eventTouchBtnChat, this));
	btnExitGame->addTouchEventListener(CC_CALLBACK_2(LayerButtonInGame::processButtonExitGameTouched, this));
	//btnListCai->addTouchEventListener(CC_CALLBACK_2(LayerButtonInGame::processButtonDanhSachCaiTouched, this));
	this->btnInvite->addTouchEventListener(CC_CALLBACK_2(LayerButtonInGame::processButtonInviteTouched, this));

	//add button
	this->addChild(this->popupButtonIngame);
	this->addChild(layerSettingInGameMain, 1000);
	this->addChild(this->layerChatInGame, 100000);
	this->addChild(bagItem);
	//this->addChild(signalNetwork);
	this->addChild(this->btnInvite);
	this->addChild(btnExitGame);
	this->addChild(btnMenu);
	this->addChild(btnChat);
	//add action for button
	this->layerChatInGame->hide();

	this->popupButtonIngame->setPosition(Point(30, 150));

	this->popupButtonIngame->setVisible(false);

	layerSettingInGameMain->setAnchorPoint(Vec2(0, 1));
	layerSettingInGameMain->setPosition(Vec2(1490, btnMenu->getPositionY() - layerSettingInGameMain->getContentSize().height));
	layerSettingInGameMain->hide();

	bagItem->setPosition(btnChat->getContentSize().width, btnChat->getContentSize().height);
	bagItem->setVisible(false);
	bagItem->setTag(982);

	signalNetwork->cocos2d::Node::setAnchorPoint(Vec2(0, 0));
	signalNetwork->setPosition(Vec2(visibleSize.width - signalNetwork->getContentSize().width * 2 - 15, visibleSize.height - 100));
	lblPing->setAnchorPoint(Vec2(0.5, 0.5));
	signalNetwork->addChild(lblPing);
	//set visible false
	lblPing->setVisible(false);
	lblPing->setPosition(Vec2(signalNetwork->getContentSize().width / 2, 0));
	lblPing->setColor(Color3B(0, 255, 222));

	btnCapture->setSwallowTouches(false);
	btnCapture->setPressedActionEnabled(true);
	btnCapture->addClickEventListener(CC_CALLBACK_1(LayerButtonInGame::onCaptured, this));
	this->addChild(btnCapture);

	signalNetwork->setTag(120);

	btnHelp->loadTextureNormal("xocdia-hd.png");
	btnHelp->setPressedActionEnabled(true);
	btnHelp->setScale9Enabled(false);
	btnHelp->setAnchorPoint(Vec2::ZERO);
	btnHelp->setPosition(Vec2(btnChat->getPositionX()+10, HEIGHT_DESIGN - heiButton - dis / 2-10));
	btnHelp->addTouchEventListener(CC_CALLBACK_2(LayerButtonInGame::onButtonHelp, this));
	/*moc3-begin remove */
	/*Button *btnCoin = createButtonWith_Style_Position(4, Vec2(dis + widButton2 + 10, dis + 10));
	this->addChild(btnCoin);
	btnCoin->addTouchEventListener(CC_CALLBACK_2(LayerButtonInGame::eventTouchBtnCoin, this));
	btnCoin->setTag(TAG_LB_BTN_COIN);
	btnCoin->setScale(0.8);*/
	//mButtonNhiemVu = LayerMenuNhiemVuInGame::create();
	//mButtonNhiemVu->setAnchorPoint(Vec2(0, 1));
	//mButtonNhiemVu->setTag(TAG_LB_BTN_MISSION);
	////     if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	////         mButtonNhiemVu->setPosition(Vec2(dis * 2.9, btnMenu->getPosition().y - 100));
	////     else
	//mButtonNhiemVu->setPosition(Vec2(sizeAdd.width, btnMenu->getPosition().y - 20));
	//this->addChild(mButtonNhiemVu);

	//if (gameID == kGameTaiXiu || gameID == kGameQuayThuong)
	//{
	//	mButtonNhiemVu->setVisible(false);
	//}
	//if (gameID == kGameXiTo)
	//{
	//	mButtonNhiemVu->setPosition(Vec2(sizeAdd.width + 5, btnMenu->getPosition().y - 80 - sizeAdd.height));
	//}
	//   if (gameID == kGameXocDia){
	//       mButtonNhiemVu->setAnchorPoint(Vec2(0,0));
	//       mButtonNhiemVu->setPosition(Vec2(350, HEIGHT_DESIGN - dis));

	//   }
	/*moc3-end change*/

	//#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	// 	btnChat->setVisible(false);
	// 	btnCoin->setVisible(false);
	//#endif

	// 	//Save game ID
	// 	boost::shared_ptr<Room> lastRooms = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	// 	if (lastRooms != NULL)
	// 	{
	// 		gameID = atoi(lastRooms->GroupId()->substr(0, 3).c_str());
	// 	}

	/*if (this->gameID != kGameChan || this->gameID != kGameXiTo){
		this->layerChatInGame->setPosition(-20, dis - 10);
		}*/
	if (gameID == kGameTaiXiu){
		btnExitGame->setVisible(false);
		this->btnInvite->setVisible(false);
		btnChat->setVisible(false);
	}

	if (gameID == kGameLieng){
		posSignal = Vec2(winSize.width - widButton2 + 20, winSize.height - heiButton / 1.5 - 50);
	}
	if (gameID == kGameChan)
	{
		btnMenu->setVisible(false);
		//this->layerChatInGame->setPosition(Vec2(-this->sizeAdd.width, -this->sizeAdd.height));

		btnCapture->loadTextures("button_camera_chan.png", "button_camera_chan.png", "");
		btnCapture->setAnchorPoint(Vec2(1, 1));
		btnCapture->setPosition(Vec2(WIDTH_DESIGN - 20 + sizeAdd.width, HEIGHT_DESIGN - 20 - 120 + sizeAdd.height));
		//signalNetwork->setTexture("chan_network_high.png");
		lblPing->setColor(Color3B(255, 171, 16));
		lblPing->setPosition(Vec2(signalNetwork->getContentSize().width / 2, -15));

		this->btnInvite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->btnInvite->setPosition(Point(btnExitGame->getPositionX() + btnExitGame->getContentSize().width / 2, btnExitGame->getPositionY() - btnInvite->getContentSize().height / 2 - 15.0));
	}
	else if (gameID == kGameQuayThuong)
	{
		btnChat->setVisible(false);
		posSignal = Vec2(winSize.width - widButton2 + 20, winSize.height - heiButton / 1.5 - 50);

		signalNetwork->setTexture("qt-signal3.png");
		lblPing->setColor(Color3B(255, 181, 19));
		lblPing->setPosition(Vec2(signalNetwork->getContentSize().width / 2, 40));
		signalNetwork->setPosition(Vec2(WIDTH_DESIGN - 300, HEIGHT_DESIGN - 150 + sizeAdd.height));
		btnCapture->setAnchorPoint(Vec2(0, 0));
		btnCapture->loadTextures("button_camera_qt.png", "button_camera_qt.png");
		btnCapture->setPosition(Vec2(WIDTH_DESIGN - 170, HEIGHT_DESIGN - 150 + sizeAdd.height));
	}
	else if (gameID == kGameXocDia)
	{
		this->addChild(btnHelp);
		this->addChild(this->btnListCai);
		btnHelp->setPosition(Point(btnMenu->getPositionX(), btnExitGame->getPositionY() - btnHelp->getContentSize().height - 18.0));
        if (SceneManager::getSingleton().isNagaCard){
            btnHelp->setPosition(Point(btnMenu->getPositionX()+20, btnExitGame->getPositionY() - btnHelp->getContentSize().height - 18.0));

        }
        this->btnListCai->setVisible(false);
		//this->btnListCai->setPosition(Point(btnMenu->getPosition()));
		btnMenu->setPosition(Point(btnExitGame->getPosition()));
		btnExitGame->setVisible(false);
		this->btnInvite->setVisible(false);
		btnChat->setVisible(false);

		btnCapture->setAnchorPoint(Vec2(0.5, 0.5));
		btnCapture->setPosition(Vec2(visibleSize.width - btnCapture->getContentSize().width / 2 - 20,
			visibleSize.height - btnCapture->getContentSize().height / 2 - 20));

		/*Label *lblNameRoom = Label::createWithTTF("Xóc đĩa \nTiền", "__Roboto-Regular.ttf", 35,
			Size(350, 200),
			TextHAlignment::LEFT, TextVAlignment::CENTER);
		lblNameRoom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		lblNameRoom->setPosition(Point(btnExitGame->getPositionX() + btnExitGame->getContentSize().height + 25.0,
			btnExitGame->getPositionY() + btnExitGame->getContentSize().height / 2));

		int roomType = SceneManager::getSingleton().getCurrRoomType();
		Sprite* coinImg = Sprite::create("moc3-icon-moc.png");
		if (roomType == 0){
			coinImg->setTexture("moc3-icon-moc.png");
		}
		else{
			coinImg->setTexture("moc3-icon-hao.png");
		}
		coinImg->setPosition(Point(lblNameRoom->getPositionX() + lblNameRoom->getContentSize().height / 2,
			btnExitGame->getPositionY() + coinImg->getContentSize().height / 2 + 10.0));
		this->addChild(lblNameRoom);
		this->addChild(coinImg);*/
	}
	else if (gameID == kGameXiTo)
	{
		//this->layerChatInGame->setPosition(Vec2(-this->sizeAdd.width, -this->sizeAdd.height));

		btnCapture->setAnchorPoint(Vec2(0.5, 0.5));
		btnCapture->setPosition(Vec2(visibleSize.width - sizeChicken.width * 2 - 50,
			visibleSize.height - sizeChicken.height + 20 - sizeAdd.height));
		btnCapture->setVisible(false);
	}
	else {
		btnCapture->setAnchorPoint(Vec2(1, 0.5));
		btnCapture->setPosition(Vec2(visibleSize.width - 30,
			visibleSize.height - sizeChicken.height + 27));
		btnCapture->setVisible(false);
	}
    
    if (SceneManager::getSingleton().getGameID()== kGamePoker){
        btnChat->setPosition(Vec2(dis, dis+260));
        btnMenu->setPosition(Vec2(dis, dis+130));
        this->btnInvite->setPosition(Vec2(dis, dis+390));
        popupButtonIngame->setPositionX(150);
    }
    if (this->gameID == kGameMauBinh){
        this->bangcuocingame = BangCuocMauBinh::create();
        this->addChild(this->bangcuocingame);
        this->bangcuocingame->hideBangCuoc();
        this->bangcuocingame->reloadValueCuoc(-1, 22, 23);
        this->bangcuocingame->showBoBai();
        
        //         btnReport->setPositionY(btnReport->getPositionY() + heiButton * 2);
        //         btnCapture->setPositionY(btnCapture->getPositionY() + heiButton * 2);
    }
	/*if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		LayerHuThuong* huthuong = LayerHuThuong::create();
		this->addChild(huthuong);
		}*/
	this->initBetCuoc();

	if (SceneManager::getSingleton().getIsBotZone()
		&& CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	{
		btnCapture->setVisible(false);
		//btnCoin->setVisible(false);
		btnMenu->setVisible(false);
		btnChat->setVisible(false);
		//mButtonNhiemVu->setVisible(false);
	}
	this->processDisplayButtonInvite(false);
    if (SceneManager::getSingleton().isNagaWin){
        btnCapture->setPosition(Vec2(visibleSize.width - 30,
                                     visibleSize.height - sizeChicken.height + 47));
        if (this->gameID == kGameXocDia){
            btnCapture->setPosition(Vec2(visibleSize.width - 70,
                                         visibleSize.height - sizeChicken.height + 47));
        }
    }
	return true;
}

void LayerButtonInGame::loadCuoc(int cuoc1, int cuoc2, int cuoc3){
    
    this->bangcuocingame->reloadValueCuoc(cuoc1, cuoc2, cuoc3);
    // this->bangcuocingame.showBoBai();
    //}
}
void LayerButtonInGame::resetBangCuoc(int cuoc1, int cuoc2, int cuoc3){
    this->bangcuocingame->reloadValueCuoc(-1, -1, -1);
}
void LayerButtonInGame::hideBangCuoc(){
    if (this->bangcuocingame){
        this->bangcuocingame->hideBangCuoc();
        //this->bangcuocingame = null;
    }
}
void LayerButtonInGame::addWidgetEx(Button* node)
{
	this->addChild(node);
}

Button* LayerButtonInGame::createButtonWith_Style_Position(int style, Point pPoint)
{
	Button* button = Button::create();
	button->setScale9Enabled(false);
	button->setAnchorPoint(Vec2(0, 0));
	switch (style) {
	case 1:
		button->loadTextures("buttons/CAM/moc3-btn-thoat.png", "", "");
		button->setPosition(Vec2(pPoint.x + sizeAdd.width, pPoint.y + sizeAdd.height * 2));

		break;
	case 2:
		button->loadTextures("button_setting.png", "", "");
		button->setPosition(Vec2(pPoint.x - sizeAdd.width, pPoint.y + sizeAdd.height * 2));
		break;
	case 3:
		button->loadTextures("moc3-chat-icon.png", "", "");
		button->setPosition(Vec2(pPoint.x + sizeAdd.width, pPoint.y));
		break;
	case 4:
		/*button->loadTextures("button_coin.png", "", "");
		button->setPosition(Vec2(pPoint.x + sizeAdd.width, pPoint.y));*/
		break;
	case 5:
		if (gameID == kGameQuayThuong){
			button->loadTextures("moc3-btn-thoat.png", "", "");
			button->setPositionY(HEIGHT_DESIGN - 150 + sizeAdd.height);
		}
		else if (gameID == kGameXocDia || gameID == kGameTaiXiu || gameID == kGameChan)
		{
			button->loadTextures("buttons/CAM/moc3-btn-thoat.png", "", "");
		}
		else
		{
			button->loadTextures("moc3-ingame-menu.png", "", "");
		}
		button->setPosition(Vec2(pPoint.x + sizeAdd.width, pPoint.y));
		break;
	case 6:
		button->loadTextures("moc3-xoc-dia-btn-cai.png", "", "");
		button->setPosition(Vec2(pPoint.x, pPoint.y));
		break;
	case 7:
		button->loadTextures("moc3-btn-invite.png", "", "");
		button->setPosition(Vec2(pPoint.x + sizeAdd.width, pPoint.y));
		break;
	default:
		break;
	}

	return button;
}

void LayerButtonInGame::eventTouchBtnBack(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED)
	{
		if (gameID == kGameSam || gameID == kGameTienLenMienBac ||
			gameID == kGameTienLenMienNam || gameID == kGameTLMNDemLa ||
			gameID == kGameSamSolo || gameID == kGameDemLaSolo ||
			gameID == kGameBaCay || gameID == kGameBaCayChuong || gameID == kGameXiTo || gameID == kGameLieng)
		{
			boost::shared_ptr<ISFSObject> parameter(new SFSObject());
			boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom == NULL){
				SceneManager::getSingleton().gotoPickGame(gameID);
				return;
			}
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_LEAVE_REQ, parameter, lastRoom));
			GameServer::getSingleton().Send(request);
		}
		//        else if (gameID == kGameMauBinh){
		//            
		//        }
		else
		{
			initBetCuoc();
			//neu khong phai la khach ma dang choi
			if (this->gameID != kGameTaiXiu && !this->isGuess && this->isStartedGame())
			{
				this->createLayerNotification(dataManager.GetSysString(235), quitMess, 0);
				return;
			}

			boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (NULL == lastRoom)
			{
				SceneManager::getSingleton().gotoPickGame(this->gameID);
			}
			else
			{
				boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
				GameServer::getSingleton().Send(request);
                SceneMain::getSingleton().prepareToPickGame();

			}
		}
	}
}

void LayerButtonInGame::BackToChooseDesk()
{
	if (gameID == 0)
	{
		if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() != NULL){
			if (gameID < 0)
			{
				SceneManager::getSingleton().gotoMain();
				return;
			}
		}
	}

	log("gameID = %d", gameID);
	//HoangDD
	// 	if (gameID.compare("109") == 0 && SceneManager::getSingletonPtr() != NULL)
	// 		SceneManager::getSingleton().setBackgroundScreen(false);
	//
	// 	if (SceneManager::getSingletonPtr() != NULL)
	// 		SceneManager::getSingleton().gotoMain(gameID);
	// 	if (LayerMain::getSingletonPtr() != NULL)
	// 	{
	// 		string gameId_ = LayerMain::getSingleton().getGameId();
	// 		SceneManager::getSingleton().gotoPickGame(atol(gameId_.c_str()));
	// 	}
}

void LayerButtonInGame::processButtonExitGameTouched(Ref *pObject, Widget::TouchEventType pType){
	if (pType == Widget::TouchEventType::ENDED)
	{
		int gameID = SceneManager::getSingleton().getGameID();
		if ((gameID == kGamePhom || gameID == kGameChan) && isChauRia)//bay luon neu la chau ria
		{
			boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
			GameServer::getSingleton().Send(request);
			return;
		}
		int _status = 1;
		std::string _mess;
		if (isLeave){
			_status = 0;
			_mess = dataManager.GetSysString(136);
		}
		else{
			_status = 1;
			_mess = dataManager.GetSysString(135);
		}
		isLeave = !isLeave;

		if (gameID == kGameSam || gameID == kGameDemLaSolo
			|| gameID == kGameSamSolo || gameID == kGameTLMNDemLa || gameID == kGamePhom || gameID == kGameCatte || gameID == kGame3Doi )
		{
			//LayerAvatarInGame52La *grandmother = static_cast<LayerAvatarInGame52La*>(this->getParent()->getParent()->getChildByTag(tag_layer_avatar_52));
			LayerAvatarInGame52La *grandmother = static_cast<LayerAvatarInGame52La*>(this->getParent()->getChildByTag(tag_layer_avatar_52));
			if (grandmother)
			{
				log("setFLAG");
				if (this->isStartedGame() == true)
				{
					grandmother->getUserByPos(kUserMe)->setFlag(isLeave);
					SceneManager::getSingleton().showToast(_mess);
				}
			}
		}
        else if (gameID == kGamePoker)
            {
                //LayerAvatarInGame52La *grandmother = static_cast<LayerAvatarInGame52La*>(this->getParent()->getParent()->getChildByTag(tag_layer_avatar_52));
                LayerAvatarPoker *grandmother = static_cast<LayerAvatarPoker*>(this->getParent()->getChildByTag(90));
                if (grandmother)
                {
                   // log("setFLAG");
                    if (this->isStartedGame() == true)
                    {
                        grandmother->GetUserByPos(kUserMe)->setFlag(isLeave);
                        SceneManager::getSingleton().showToast(_mess);
                    }
                    
                    
                }
            }
        else if ( gameID == kGameMauBinh)
        {
            //LayerAvatarInGame52La *grandmother = static_cast<LayerAvatarInGame52La*>(this->getParent()->getParent()->getChildByTag(tag_layer_avatar_52));
            LayerAvatarInGame52La *grandmother = static_cast<LayerAvatarInGame52La*>(this->getParent()->getChildByTag(100));
            if (grandmother)
            {
               // log("setFLAG");
                if (this->isStartedGame() == true)
                {
                    grandmother->getUserByPos(kUserMe)->setFlag(isLeave);
                    SceneManager::getSingleton().showToast(_mess);
                }
                
                
            }
        }
		else if (gameID == kGameBaCay || gameID == kGameBaCayChuong || gameID == kGameLieng)
		{
			LayerAvatarInGame52La *grandmother = static_cast<LayerAvatarInGame52La*>(this->getParent()->getChildByTag(tag_layer_avatar_52));
			if (grandmother)
			{
				if (this->isStartedGame() == true)
				{
					grandmother->getUserByPos(kuser0)->setFlag(isLeave);
					SceneManager::getSingleton().showToast(_mess);
				}
			}
		}

		boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		parameter->PutInt(EXT_FIELD_LEAVE_STATUS, _status);
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom != NULL)
		{
            if (0 == lastRoom->GroupId()->compare(boost::to_string(118)))
            {
                boost::shared_ptr<IRequest> request2(new UnsubscribeRoomGroupRequest(boost::to_string(118)));
                GameServer::getSingleton().Send(request2);
                boost::shared_ptr<IRequest> request(new LeaveRoomRequest(lastRoom));
                GameServer::getSingleton().Send(request);
                int currGameID = SceneManager::getSingleton().getGameID();
                SceneManager::getSingleton().setAutoCreateRoom(false);
                SceneManager::getSingleton().setRejoinInfo(RejoinInfoStruct());
                if (SceneManager::getSingleton().getGameID() == kGameTaiXiu || SceneManager::getSingleton().getGameID() == kGameQuayThuong) {
                    SceneManager::getSingleton().gotoMain();
                }
                //update 3/8
                else if (currGameID == kGameXocDia || currGameID == kGameBauTom || currGameID == kGameSlot){
                    if (SceneManager::getSingleton().getCurrRoomType() == -1)
                        SceneManager::getSingleton().gotoMain();
                    else{
                        SceneManager::getSingleton().showLoading();
                        boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(StringUtils::format("%d", SceneManager::getSingleton().getGameID())));
                        GameServer::getSingleton().Send(request);
                    }
                    
                }
                else {
                    //MINHDV -changes - showpopup if betvalue last room larger user money
                    int gameID = SceneManager::getSingleton().getGameID();
                    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
                    if (mySelf)
                    {
                        boost::shared_ptr<UserVariable> amfptr;
                        if (SceneManager::getSingleton().getCurrRoomType() == 0){
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
                                vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
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
                                double money_bet = SceneManager::getSingleton().getBetValue();
                                if (betUser < money_bet)
                                {
                                    ///set flag to show popup
                                    SceneManager::getSingleton().setKickNem(true);
                                }
                            }
                        }
                    }
                    SceneManager::getSingleton().gotoPickGame(gameID);
                }
            }
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_LEAVE_GAME_REQ, parameter, lastRoom));
			GameServer::getSingleton().Send(request);
        }else{
            int currGameID = SceneManager::getSingleton().getGameID();
            SceneManager::getSingleton().setAutoCreateRoom(false);
            SceneManager::getSingleton().setRejoinInfo(RejoinInfoStruct());
            if (SceneManager::getSingleton().getGameID() == kGameTaiXiu || SceneManager::getSingleton().getGameID() == kGameQuayThuong) {
                SceneManager::getSingleton().gotoMain();
            }
            //update 3/8
            else if (currGameID == kGameXocDia || currGameID == kGameBauTom || currGameID == kGameSlot){
                if (SceneManager::getSingleton().getCurrRoomType() == -1)
                    SceneManager::getSingleton().gotoMain();
                else{
                    SceneManager::getSingleton().showLoading();
                    boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(StringUtils::format("%d", SceneManager::getSingleton().getGameID())));
                    GameServer::getSingleton().Send(request);
                }
                
            }
            else {
                //MINHDV -changes - showpopup if betvalue last room larger user money
                int gameID = SceneManager::getSingleton().getGameID();
                boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
                if (mySelf)
                {
                    boost::shared_ptr<UserVariable> amfptr;
                    if (SceneManager::getSingleton().getCurrRoomType() == 0){
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
                            vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
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
                            double money_bet = SceneManager::getSingleton().getBetValue();
                            if (betUser < money_bet)
                            {
                                ///set flag to show popup
                                SceneManager::getSingleton().setKickNem(true);
                            }
                        }
                    }
                }
                SceneManager::getSingleton().gotoPickGame(gameID);
            }
        }
	}
}

void LayerButtonInGame::eventTouchBtnSetting(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED)
	{
		log("Click Setting");
		layerSettingInGameMain->show(false);
	}
}

void LayerButtonInGame::eventTouchBtnChat(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED)
	{
		this->layerChatInGame->setVisible(true);
		////layerChatInGame->resetBagNum();
		//if (layerChatInGame->isVisible()) {
		//	layerChatInGame->hide();
		//	bagItem->setVisible(false);
		//}
		//else {
		//	layerChatInGame->show();
		//	//layerChatInGame->resetBagNum();
		//	bagItem->setVisible(false);

		//}
	}
}

LayerMocChatInGame* LayerButtonInGame::getLayerChat() const{
	return this->layerChatInGame;
}

void LayerButtonInGame::eventTouchBtnCoin(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED)
	{
		//SceneManager::getSingleton().disconnectFromServer();
		if (this->getChildByTag(2468)) return;
		LayerPopupRecharge* _child = LayerPopupRecharge::create();
		_child->setTag(2468);
		if (this->gameID == kGameXiTo){
			_child->setPosition(Vec2(-this->sizeAdd.width, -this->sizeAdd.height));
		}
		this->addChild(_child);
	}
}

void LayerButtonInGame::callbackFromChatWindow(Node*, void* data)
{
	if (strlen((char*)data) == 0 || GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL)
		return;
	boost::shared_ptr<ISFSObject> parameters(new SFSObject());
	boost::shared_ptr<IRequest> request(new PublicMessageRequest((char*)data, parameters, GameServer::getSingleton().getSmartFox()->LastJoinedRoom()));
	GameServer::getSingleton().Send(request);
}

Button *LayerButtonInGame::getButtonByTag(int tag)
{
	if (this->getChildByTag(tag) != NULL)
	{
		return (Button*)this->getChildByTag(tag);
	}
	return NULL;
}

void LayerButtonInGame::hideButtonInChanGame()
{
	//getButtonByTag(TAG_LB_BTN_SETTING)->setVisible(false);
	/*getButtonByTag(TAG_LB_BTN_CHAT)->setVisible(false);
	getButtonByTag(TAG_LB_BTN_COIN)->setPositionX(30);*/
}

void LayerButtonInGame::SetIsGuess(const bool& guess /*= false*/)
{
	isGuess = guess;
}

void LayerButtonInGame::eventTouchBtnSetting_Chan(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED)
	{
		log("Setting in Chan Game");
	}
}

LayerButtonInGame::LayerButtonInGame()
:gameID(102),
isGuess(false)
{
	mCallBackListener = NULL;
	mCallBack = NULL;
	btnCapture = NULL;
	isLeave = false;
	log("LayerButtonInGame Constructor");
	GameServer::getSingleton().addListeners(this);
}

LayerButtonInGame::~LayerButtonInGame()
{
	GameServer::getSingleton().removeListeners(this);
	log("LayerButtonInGame Destructor");
}

bool LayerButtonInGame::isStartedGame()
{
	// 		// Kiểm tra xem có đang chơi hay không, nếu đang chơi thì không được back ra
	boost::shared_ptr<Room> lastRooms = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRooms == NULL) return false;
	boost::shared_ptr<RoomVariable> rv = lastRooms->GetVariable("params");
	if (rv == NULL) return false;
	vector<string> lstParams = mUtils::splitString(*rv->GetStringValue(), '@');
	// 	for (int i = 0; i < lstParams.size(); ++i){
	// 		log("params[%d] = %s", i, lstParams[i].c_str());
	// 	}
	// 	if (lstParams.size() < 3)
	// 		return true;
	// 	int currPlayers = atoi(lstParams.at(2).c_str());
	bool isPlaying = (lstParams.at(1).compare("1") == 0);
	return isPlaying;
}

void LayerButtonInGame::createLayerNotification(string title, string content_mess, int tag)
{
	LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
	layerPopup->setCallBackFromButton(this, callfuncO_selector(LayerButtonInGame::layerPopupThongBaoCallBack), tag);
	layerPopup->setMessage(title);
	layerPopup->setContentMess(content_mess);
	layerPopup->setTag(24);
	if (this->gameID == kGameChan || this->gameID == kGameCoTuong || this->gameID == kGameCoUp){
		layerPopup->setPosition(Vec2(-mUtils::getSizePos().width, -mUtils::getSizePos().height));
	}
	this->addChild(layerPopup);
}

void LayerButtonInGame::layerPopupThongBaoCallBack(Ref *pSender)
{
	int	tag = ((Node*)pSender)->getTag();
	switch (tag)
	{
	case 0:
	{
			  // 			  boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			  // 			  if (lastRoom == NULL){
			  // 				  SceneManager::getSingleton().gotoPickGame(gameID);
			  // 				  return;
			  // 			  }

			  switch (gameID)
			  {
			  case kGameChan:
			  case kGamePhom:
			  {
								SceneManager::getSingleton().showLoading();
								boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
								GameServer::getSingleton().Send(request);
			  }
				  break;
			  case kGameMauBinh:
			  {
								   SceneManager::getSingleton().showLoading();
								   // public static final String EXT_FIELD_CONFIRM = "cf";
								   boost::shared_ptr<ISFSObject> parameter(new SFSObject());
								   boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
								   if (lastRoom == NULL){
									   // SceneManager::getSingleton().gotoPickGame(gameID);
									   return;
								   }
								   parameter->PutInt("cf", 1);
								   boost::shared_ptr<IRequest> request(new ExtensionRequest("lvgrq", parameter, lastRoom));
								   GameServer::getSingleton().Send(request);
			  }
				  break;
			  default:
				  break;
			  }
	}
		break;
	case 1:
	{
			  SceneManager::getSingleton().showLoading();
			  // public static final String EXT_FIELD_CONFIRM = "cf";
			  boost::shared_ptr<ISFSObject> parameter(new SFSObject());
			  boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			  if (lastRoom == NULL){
				  // SceneManager::getSingleton().gotoPickGame(gameID);
				  return;
			  }
			  parameter->PutInt("cf", 1);
			  boost::shared_ptr<IRequest> request(new ExtensionRequest("lvgrq", parameter, lastRoom));
			  GameServer::getSingleton().Send(request);
	}
		break;
	default:
		break;
	}
	this->removeChildByTag(24);
}

void LayerButtonInGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	log("============== CMD = %s", cmd->c_str());
	if (0 == cmd->compare("lvgrs"))
	{
		event_EXT_EVENT_GAME_LEAVE_RES(param);
	}
	else if (0 == cmd->compare(EXT_SRVNTF_PLAYER_LIST))
	{
		event_EXT_SRVNTF_PLAYER_LIST(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_REGISTER_QUEUE_NTF))
	{
		event_EXT_EVENT_REGISTER_QUEUE_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_START) ||
		0 == cmd->compare(EXT_EVENT_START_GAME_NOTIF) ||
		0 == cmd->compare(EXT_EVENT_GAME_CHANGE_NOTIF))
	{
		if (btnCapture != NULL)
		{
			if (SceneManager::getSingleton().getIsBotZone()
				&& CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
				btnCapture->setVisible(false);
			else
				btnCapture->setVisible(true);
		}
	}
	else if (0 == cmd->compare(EXT_EVENT_END) ||
		0 == cmd->compare(EXT_EVENT_END_GAME_NOTIF))
	{
		if (gameID != kGameChan && btnCapture != NULL)
		{
			btnCapture->setVisible(false);
		}
	}
    else if (0 == cmd->compare(EXT_EVENT_USER_LEAVE_NOTIF))
    {
        event_EXT_EVENT_USER_LEAVE_NOTIF(param);
    }
}
void LayerButtonInGame::event_EXT_EVENT_USER_LEAVE_NOTIF(boost::shared_ptr<ISFSObject> param){
    boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
    return;
    if (uid != NULL){
        if (strcmp(SceneManager::getSingleton().getMyName().c_str(), uid->c_str()) == 0){
            int currGameID = SceneManager::getSingleton().getGameID();
                SceneManager::getSingleton().setAutoCreateRoom(false);
                SceneManager::getSingleton().setRejoinInfo(RejoinInfoStruct());
                if (SceneManager::getSingleton().getGameID() == kGameTaiXiu || SceneManager::getSingleton().getGameID() == kGameQuayThuong) {
                    SceneManager::getSingleton().gotoMain();
                }
                //update 3/8
                else if (currGameID == kGameXocDia || currGameID == kGameBauTom || currGameID == kGameSlot){
                    if (SceneManager::getSingleton().getCurrRoomType() == -1)
                        SceneManager::getSingleton().gotoMain();
                    else{
                        SceneManager::getSingleton().showLoading();
                        boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(StringUtils::format("%d", SceneManager::getSingleton().getGameID())));
                        GameServer::getSingleton().Send(request);
                    }
                    
                }
                else {
                    //MINHDV -changes - showpopup if betvalue last room larger user money
                    int gameID = SceneManager::getSingleton().getGameID();
                    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
                    if (mySelf)
                    {
                        boost::shared_ptr<UserVariable> amfptr;
                        if (SceneManager::getSingleton().getCurrRoomType() == 0){
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
                                vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
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
                                double money_bet = SceneManager::getSingleton().getBetValue();
                                if (betUser < money_bet)
                                {
                                    ///set flag to show popup
                                    SceneManager::getSingleton().setKickNem(true);
                                }
                            }
                        }
                    }
                    SceneManager::getSingleton().gotoPickGame(gameID);
                }
        }
    }
}
void LayerButtonInGame::event_EXT_EVENT_REGISTER_QUEUE_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> lu = param->GetUtfString("lu");

	if (lu != NULL)
	{
		string listUser = lu->c_str();
		string myName = SceneManager::getSingleton().getMyName();
		if (listUser.find(myName) != std::string::npos) //neu co trong danh sach hang doi thi chau ria = true
		{
			isChauRia = true;
		}
		else{
			isChauRia = false;
		}
		//LayerPopupMenuInGame* layerPopup = dynamic_cast<LayerPopupMenuInGame*>(this->getChildByTag(TAG_POPUP_MENU));
		if (this->popupButtonIngame)
			this->popupButtonIngame->setChauRia(isChauRia);
	}

}
void LayerButtonInGame::event_EXT_SRVNTF_PLAYER_LIST(boost::shared_ptr<ISFSObject> param) {
	boost::shared_ptr<string> lu = param->GetUtfString("lu");

	if (lu != NULL)
	{
		string listUser = lu->c_str();
		string myName = SceneManager::getSingleton().getMyName();
		if (listUser.find(myName) != std::string::npos) //neu co trong danh sach thang choi thi chau ria = false
		{
			isChauRia = false;
		}
		else{
			isChauRia = true;
		}
		LayerPopupMenuInGame* layerPopup = dynamic_cast<LayerPopupMenuInGame*>(this->getChildByTag(TAG_POPUP_MENU));
		if (layerPopup)
			layerPopup->setChauRia(isChauRia);
	}
}
void LayerButtonInGame::event_EXT_EVENT_GAME_LEAVE_RES(boost::shared_ptr<ISFSObject> param)
{
	if (param == NULL) return;
	boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT);
	if (rscode != NULL)
	{
		if (*rscode != 0) return;
		isLeave = !isLeave;
		LayerButtonInGame *parent = static_cast<LayerButtonInGame*> (this->getParent());
		if (!parent) return;
		int gameID = SceneManager::getSingleton().getGameID();

		if (gameID == kGameSam || gameID == kGameDemLaSolo
			|| gameID == kGameSamSolo || gameID == kGameTLMNDemLa || gameID == kGamePhom)
		{
			LayerAvatarInGame52La *grandmother = static_cast<LayerAvatarInGame52La*>(this->getParent()->getParent()->getChildByTag(tag_layer_avatar_52));
			if (grandmother)
			{
				log("setFLAG");
				grandmother->getUserByPos(kUserMe)->setFlag(isLeave);
			}
		}
		else if (gameID == kGameBaCay || gameID == kGameBaCayChuong || gameID == kGameLieng)
		{
			LayerAvatarInGame52La *grandmother = static_cast<LayerAvatarInGame52La*>(this->getParent()->getParent()->getChildByTag(tag_layer_avatar_52));
			if (grandmother)
			{
				grandmother->getUserByPos(kuser0)->setFlag(isLeave);
			}
		}
	}

	//if (rscode == NULL) return;
	//int int_res = (int)*rscode;
	//switch (int_res)
	//{
	//case 10:
	//{
	//		   if (gameID == kGameLieng)
	//		   {
	//			   SceneManager::getSingleton().showLoading();
	//			   // public static final String EXT_FIELD_CONFIRM = "cf";
	//			   boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	//			   boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	//			   if (lastRoom == NULL){
	//				   // SceneManager::getSingleton().gotoPickGame(gameID);
	//				   return;
	//			   }
	//			   parameter->PutInt("cf", 1);
	//			   boost::shared_ptr<IRequest> request(new ExtensionRequest("lvgrq", parameter, lastRoom));
	//			   GameServer::getSingleton().Send(request);
	//		   }
	//		   else{
	//			   createLayerNotification(dataManager.GetSysString(235), quitMess, 1);
	//		   }
	//}
	//	break;
	//case 5:
	//	SceneManager::getSingleton().gotoPickGame(gameID);
	//	break;
	//}
}

void LayerButtonInGame::showToast(string mess)
{
	Chat* message = Chat::create(mess, -1);
	message->setTag(246);
	this->addChild(message);
}

void LayerButtonInGame::initBetCuoc()
{
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom != NULL)
	{
		boost::shared_ptr<RoomVariable> paramString = lastRoom->GetVariable("params");
		if (paramString != NULL)
		{
			vector<string> arrInfo = mUtils::splitString(*paramString->GetStringValue(), '@');
			double money = atol(arrInfo.at(0).c_str());
			if (gameID != kGameChan && gameID != kGameTaiXiu && gameID != kGameXiTo)
			{
				int betratio = 30;
				if (gameID == kGameBaCay || gameID == kGameBaCayChuong)
					betratio = 3;
				double betPhat = betratio * money;
				if (gameID == kGameMauBinh){
					int user_count = 0;
					if (arrInfo.size() > 2){
						user_count = atoi(arrInfo.at(2).c_str()) - 1;
						betPhat = money * 6 * user_count;
					}
				}
				quitMess = StringUtils::format("%s %s %s \n %s",dataManager.GetSysString(54).c_str(), mUtils::convertMoneyEx(betPhat).c_str(),dataManager.GetSysString(55).c_str(),dataManager.GetSysString(56).c_str());
			}
			else if (gameID == kGameXiTo)
			{
				quitMess = dataManager.GetSysString(57);
			}
			else if (gameID == kGameChan)
			{
				int user_count = 0;
				if (arrInfo.size() > 2){
					user_count = atoi(arrInfo.at(2).c_str());
					int betratio = 17 * (user_count - 1);
					string header = dataManager.GetSysString(58)+" \n";
					double betPhat = betratio * money;
					quitMess = StringUtils::format("%s %s %s \n %s", header.c_str(),mUtils::convertMoneyEx(betPhat).c_str(),dataManager.GetSysString(55).c_str(),dataManager.GetSysString(56).c_str());
				}
			}
		}
	}
}

void LayerButtonInGame::eventTouchBtnMenu(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED)
	{
		if (gameID != kGameTaiXiu && gameID != kGameQuayThuong && gameID != kGameXocDia)
		{
			LayerPopupMenuInGame* layerPopup = dynamic_cast<LayerPopupMenuInGame*>(this->getChildByTag(TAG_POPUP_MENU));

			//LayerPopupMenuInGame *layerPopup = LayerPopupMenuInGame::create();
			/*if (layerPopup == NULL)
			{
			layerPopup->setPosition(Point(30, 150));
			*/
			layerPopup->setChauRia(isChauRia);
			/*layerPopup->setTag(TAG_POPUP_MENU);
			this->addChild(layerPopup);
			}*/
			layerPopup->showMenu();
		}
		else
		{
            SceneManager::getSingleton().setIsChuyenTab(false);

			boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (NULL == lastRoom)
			{
                SceneManager::getSingleton().gotoMain();
                SceneMain::getSingleton().prepareToPickGame();

			}
			else
			{
				if (gameID == kGameXocDia || gameID == kGameTaiXiu){
					SceneManager::getSingleton().setCurrRoomType(-1);
				}

				boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
				GameServer::getSingleton().Send(request);
                SceneMain::getSingleton().prepareToPickGame();

			}
		}
	}
}

void LayerButtonInGame::setMessageNotification(string mess)
{
	quitMess = mess;
}

void LayerButtonInGame::setBaoSam(bool _isbao)
{
	LayerPopupMenuInGame* layerPopup = (LayerPopupMenuInGame*) this->getChildByTag(TAG_POPUP_MENU);
	if (layerPopup)
	{
		layerPopup->setBaoSam(_isbao);
	}
}

void LayerButtonInGame::SetPosLayerSetting(const Vec2& pos){
	this->layerSettingInGameMain->setAnchorPoint(Vec2(0, 1));
	this->layerSettingInGameMain->setPosition(pos);
}

void LayerButtonInGame::ResetStatus(){
	isLeave = false;
	/*LayerPopupMenuInGame* layerPopup = (LayerPopupMenuInGame*) this->getChildByTag(TAG_POPUP_MENU);
	if (layerPopup)
	{
	layerPopup->ResetStatus();
	}*/
}

void LayerButtonInGame::onButtonHelp(Ref* pSender, Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED)
	{
        Scene* currScene = Director::getInstance()->getRunningScene();
        if (currScene != NULL)
        {
            LayerGuideInGame* _layer = LayerGuideInGame::create();
            currScene->addChild(_layer, 10000,9007);
            _layer->setTag(1504);
        }
	
	}
}

void LayerButtonInGame::onCaptured(cocos2d::Ref*)
{
//    if (gameID == kGame3Doi){
//        boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
//        boost::shared_ptr<ISFSObject> parameter(new SFSObject());
//        boost::shared_ptr<IRequest> request(new ExtensionRequest("dr3d", parameter, lastRoom));
//        GameServer::getSingleton().Send(request);
//        return;
//    }
	btnCapture->setEnabled(false);
	if (!_filename.empty())
		Director::getInstance()->getTextureCache()->removeTextureForKey(_filename);
	long current_time = utils::getTimeInMilliseconds();
	_filename = StringUtils::format("bmscreenshot_%d.png", current_time);
	utils::captureScreen(CC_CALLBACK_2(LayerButtonInGame::afterCaptured, this), _filename);
}

void LayerButtonInGame::afterCaptured(bool succeed, const std::string& outputFile)
{
	if (succeed)
	{
		btnCapture->setEnabled(true);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		_filename = outputFile;
		IOS::saveScreenShot(outputFile);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		NativeHelper::saveScreenShot(_filename.c_str());
#endif
		this->showToast(dataManager.GetSysString(1));

	}
	else
	{
		btnCapture->setEnabled(true);
		log("Capture screen failed.");
		this->showToast(dataManager.GetSysString(2));
	}
}

void LayerButtonInGame::processButtonInviteTouched(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		LayerPopupInvite *popupInvite = LayerPopupInvite::create();
		this->addChild(popupInvite);
	}
}

void LayerButtonInGame::processDisplayButtonInvite(bool _display)
{
    if (SceneManager::getSingleton().getCapVip()< dataManager.getAppConfig().mintaoban)
        _display = false;
	this->btnInvite->setVisible(_display);
}

Button* LayerButtonInGame::getButtonListCai()
{
	return this->btnListCai;
}
