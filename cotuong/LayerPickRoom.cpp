#include "LayerPickRoom.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "LayerItemRoom.h"
#include "../SceneManager.h"
#include "ServerMessageDef.h"
#include "../layergames/PhomMessDef.h"
#include "../mUtils.h"
#include "Common.h"
#include "gameUtils.h"
#include "boost/smart_ptr/make_shared.hpp"
#include "Requests/JoinRoomRequest.h"
#include "Requests/SetUserVariablesRequest.h"
#include "Entities/Variables/UserVariable.h"
#include "Entities/Variables/SFSUserVariable.h"
#include "Entities/Invitation/SFSInvitation.h"
#include "Entities/Invitation/Invitation.h"
#include "Requests/SubscribeRoomGroupRequest.h"
#include "Requests/UnsubscribeRoomGroupRequest.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/Buddylist/SetBuddyVariablesRequest.h"
#include "Entities/Variables/SFSBuddyVariable.h"
#include "layers/LayerPopupInviteGame.h"
#include "layers/LayerPopupHetTien.h"
#include "LayerCreateRoomChess.h"
#include "layergames/ChanUtils.h"
#include "layers/LayerPopupProfile.h"
#include "LayerPopupAlert.h"
#include "Requests/LeaveRoomRequest.h"
#include "layers/LayerChuyenTien.h"
#include "layers/LayerPopupUngTien.h"

USING_NS_CC;
#define TIME_AUTO_RELOAD 60
#define  CHAT_ROOM_ID 118
// on "init" you need to initialize your instance
bool LayerPickRoom::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//    auto rootNode = CSLoader::createNode("LayerChonGame.csb");
	//    this->addChild(rootNode);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//update 29/7
	string pathCSB = "LayerChessPickRoom.csb";
	if (SceneManager::getSingleton().getIsFlagStore() == true)
		pathCSB = "New_LayerChessPickRoom.csb";
	auto rootNode = CSLoader::getInstance()->createNode(pathCSB);

	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	//update 19/8
    this->spriteBg = Sprite::create("background-gold.jpg");
	if (this->spriteBg)
	{
		this->spriteBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->spriteBg->setScaleY(visibleSize.height / spriteBg->getContentSize().height);
	}
	//update 11/7
	Size sizeAdd = ChanUtils::getSizePos();
	Node* nodeMid = (Node*)rootNode->getChildByName("nodeMid");
	if (nodeMid)
	{
		this->pnlList = (Layout *)nodeMid->getChildByName("pnlList");
		this->btnMoneySilver = (Button*)nodeMid->getChildByName("btnMoneyVirtual");
		this->btnMoneyGold = (Button*)nodeMid->getChildByName("btnMoney");
		this->cb1 = (CheckBox*)nodeMid->getChildByName("cb1");
		this->cb2 = (CheckBox*)nodeMid->getChildByName("cb2");
		this->cb3 = (CheckBox*)nodeMid->getChildByName("cb3");
		this->lblGold = (Text*)this->btnMoneyGold->getChildByName("lblGameName_0_0");
		this->lblSilver = (Text*)this->btnMoneySilver->getChildByName("lblGameName_0_0_0");

		this->lblGold->setString("Bàn vip");
		this->lblGold->setFontSize(30);
		this->lblSilver->setString("Bàn thường");
		this->lblSilver->setFontSize(30);

        auto Text_1_0 = (Text*)nodeMid->getChildByName("Text_1");
        Text_1_0->setVisible(false);
        
		ImageView* Image_24 = (ImageView*)nodeMid->getChildByName("Image_24");
		this->lblMoney1 = (Text*)Image_24->getChildByName("lblMoney1");
		this->lblMoney2 = (Text*)Image_24->getChildByName("lblMoney2");
		this->lblMoney3 = (Text*)Image_24->getChildByName("lblMoney3");
		this->lblMoney4 = (Text*)Image_24->getChildByName("lblMoney4");
		this->bt_cuoc2_5 = (Sprite*)Image_24->getChildByName("bt_cuoc2_5");
		//update 11/7
		this->btnPlayNow = (Button *)nodeMid->getChildByName("btnPlayNow");
		this->btnPlayNow->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonPlayNow, this));
		Layout* pnlChat = (Layout*)nodeMid->getChildByName("pnlChat");
		Button* btnSend = (Button*)pnlChat->getChildByName("btnSend");
		this->txtChat = (TextField*)pnlChat->getChildByName("TextField_1");
		Layout* pnlContent = (Layout*)pnlChat->getChildByName("pnlContent");

		this->txtChat->setSwallowTouches(true);

		this->tblChat = TableView::create(this, pnlContent->getContentSize());
		this->tblChat->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
		this->tblChat->setPosition(Vec2(0, 0));
		this->tblChat->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		this->tblChat->setDelegate(this);
		pnlChat->addChild(this->tblChat);


		btnSend->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonChat, this));

		this->pnlList->setSwallowTouches(false);
		this->pnlList->setTouchEnabled(true);
		this->lblMoney1->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onLabelBetValue, this));
		this->lblMoney2->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onLabelBetValue, this));
		this->lblMoney3->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onLabelBetValue, this));
		this->lblMoney4->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onLabelBetValue, this));
		this->btnMoneyGold->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonTypeRoom, this));
		this->btnMoneySilver->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonTypeRoom, this));
		this->cb1->addEventListener(CC_CALLBACK_2(LayerPickRoom::onCheckbox, this));
		this->cb2->addEventListener(CC_CALLBACK_2(LayerPickRoom::onCheckbox, this));
		this->cb3->addEventListener(CC_CALLBACK_2(LayerPickRoom::onCheckbox, this));
        this->lblMoney1->setVisible(false);
        this->lblMoney2->setVisible(false);
        this->lblMoney3->setVisible(false);
        this->lblMoney4->setVisible(false);


		//nodeMid->setPositionY(nodeMid->getPositionY() - sizeAdd.height / 6);
		//update 19/8
		this->pnlNotify = (Layout*)nodeMid->getChildByName("pnlNotify");
		this->createLabelThongBao("   ");
	}

	Node* nodeBot = (Node*)rootNode->getChildByName("nodeBot");
	if (nodeBot)
	{
		nodeBot->setPositionY(0);
		Node* nodeAvatar = (Node*)nodeBot->getChildByName("nodeAvatar");
		this->lblName = (Text*)nodeBot->getChildByName("lblName");
		this->lblLevel = (Text*)nodeBot->getChildByName("lblLevel");
		this->lblMoneySilver = (Text*)nodeBot->getChildByName("lblMoneySilver");
        this->lblMoneySilver->setVisible(false);
		this->lblMoneyGold = (Text*)nodeBot->getChildByName("lblMoneyGold");
        this->lblMoneyGold->setVisible(false);

		this->prgProgress = (LoadingBar*)nodeBot->getChildByName("prgProgress");
		Button* btnFind = (Button*)nodeBot->getChildByName("btnFind");
		Button* btnRecharge = (Button*)nodeBot->getChildByName("btnRecharge");
       // this->btnPlayNow->setPosition(Vec2(btnRecharge->getPositionX(),-820));
        this->btnPlayNow->setVisible(false);
        this->btnPlayNow->setTitleText(dataManager.GetSysString(864));
        auto btnChoiNgay = Button::create("bt_1.png","bt_1.png","bt_1.png");
        btnChoiNgay->setTitleText(dataManager.GetSysString(864));
        btnChoiNgay->setTitleColor(Color3B::BLACK);
        btnChoiNgay->setTitleFontSize(50);
        btnChoiNgay->setPosition(btnRecharge->getPosition());
        btnChoiNgay->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonPlayNow, this));
        nodeBot->addChild(btnChoiNgay);
		Layout* Panel_1 = (Layout*)nodeBot->getChildByName("Panel_1");
		Panel_1->setSwallowTouches(true);
		Panel_1->setTouchEnabled(true);
		//update 11/7
		Button* btnUngTien = (Button*)nodeBot->getChildByName("btnUngTien");
		Button* btnChuyenTien = (Button*)nodeBot->getChildByName("btnChuyenTien");

		btnUngTien->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonUngTien, this));
		btnChuyenTien->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonChuyenTien, this));

		this->circleAvatar = CircleAvatar::create();
		this->circleAvatar->setAnchorPoint(nodeAvatar->getAnchorPoint());
		this->circleAvatar->setPosition(nodeAvatar->getPosition());
		this->circleAvatar->setClickCallBack(CC_CALLBACK_0(LayerPickRoom::onButtonAvatar, this));
		nodeBot->addChild(this->circleAvatar);

		//btnRecharge->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonCharge, this));
        btnRecharge->setVisible(false);
		btnFind->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonFindRoom, this));
	}

	Node* nodeTop = (Node*)rootNode->getChildByName("nodeTop");
	if (nodeTop)
	{
		nodeTop->setPositionY(visibleSize.height);
		this->btnTab1 = (Button*)nodeTop->getChildByName("btnTab1");
		this->btnTab2 = (Button*)nodeTop->getChildByName("btnTab2");
		this->btnTab3 = (Button*)nodeTop->getChildByName("btnTab3");
        
        this->btnTab1->setVisible(false);
        this->btnTab2->setVisible(false);
        this->btnTab3->setVisible(false);

		this->btnCreateRoom = (Button *)nodeTop->getChildByName("btnCreate");
        this->btnCreateRoom->setVisible(false);
		Text* lblGameName = (Text*)nodeTop->getChildByName("lblGameName");
		Button* btnBack = (Button*)nodeTop->getChildByName("btnBack");

		lblGameName->setString(mUtils::getGameNameByID(this->currGameID)->c_str());
        lblGameName->setVisible(false);
		this->btnCreateRoom->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonCreateRoom, this));
		this->btnTab1->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonFilterRoom, this));
		this->btnTab2->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonFilterRoom, this));
		this->btnTab3->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonFilterRoom, this));
		btnBack->addClickEventListener(CC_CALLBACK_1(LayerPickRoom::onButtonBack, this));
	}

	tableView = TableView::create(this, Size(pnlList->getContentSize().width, pnlList->getContentSize().height + sizeAdd.height));
	tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	tableView->setPosition(Vec2(0, -sizeAdd.height));
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tableView->setTag(37);//table pickroom
	pnlList->addChild(tableView);
	tableView->reloadData();
	tableView->setDelegate(this);

	//this->sendListBetRequest();
	//if (dataManager.getAppConfig().hidemoney == 0)
//        this->onButtonTypeRoom(this->btnMoneySilver);
//    else{
		this->onButtonTypeRoom(this->btnMoneyGold);

//	}
	this->registerInviteRequest();
	this->loadUserInfo();
	//auto invite handle
	bool isAutoInvite = UserDefault::getInstance()->getBoolForKey("isAutoInvite", true);
	if (isAutoInvite)
	{
		int inviteTime = dataManager.getAppConfig().invitetime;
		this->schedule(schedule_selector(LayerPickRoom::autoInvitePlay), inviteTime);
	}


	//update 11/7
	this->schedule(schedule_selector(LayerPickRoom::onScheduleUpdateRoomList), TIME_AUTO_RELOAD);
	//update 19/7
	this->schedule(schedule_selector(LayerPickRoom::onScheduleThongBao));

	//update 25/7
	RejoinInfoStruct rejoinInfo = SceneManager::getSingleton().getRejoinInfo();
	mCurrentRoomIDJoin = atoi(rejoinInfo.gameID.c_str());
	//if can rejoin, send to server - HoangDD
	if (rejoinInfo.isNeedRejoin){
		SceneManager::getSingleton().showLoading();
		this->scheduleOnce(schedule_selector(LayerPickRoom::onScheduleRejoinRoom), 0.5);
		return true;
	}

	return true;
}

LayerPickRoom::LayerPickRoom()
{

	this->currGameID = SceneManager::getSingleton().getGameID();
	GameServer::getSingleton().addListeners(this);
	//update 11/7
	this->joinRoomChat();
}

LayerPickRoom::~LayerPickRoom()
{
	GameServer::getSingleton().removeListeners(this);
	this->unschedule(schedule_selector(LayerPickRoom::autoInvitePlay));
	this->unschedule(schedule_selector(LayerPickRoom::onScheduleRejoinRoom));
	this->unschedule(schedule_selector(LayerPickRoom::onScheduleThongBao));
}

void LayerPickRoom::onButtonCreateRoom(Ref *pSender){
	LayerCreateRoomChess* layerNew = LayerCreateRoomChess::create();
	layerNew->setPopupInGame(false);
	layerNew->setCanSetting(true);
	this->addChild(layerNew);
}

void LayerPickRoom::onButtonPlayNow(Ref *pSender){
	if (this->listBetFilter.size() == 0)
		return;
	// 	int groupIdList = 1;
	// 	if (this->currTab == 1)
	// 	{
	// 		groupIdList = 5;
	// 	}
	// 	else if (this->currTab == 2)
	// 	{
	// 		groupIdList = 10;
	// 	}
	// 
	// 	int _gameID = SceneManager::getSingleton().getGameID();
	// 	boost::shared_ptr<ISFSObject> param(new SFSObject());
	// 	param->PutInt(AMOUNT_BET, -1);
	// 	//param->PutUtfString(EXT_FIELD_GID_PICKROOM, StringUtils::format("%d%d", _gameID, groupIdList));
	// 	param->PutInt(EXT_FIELD_ROOM_TYPE, 0);
	// 	param->PutUtfString(EXT_FIELD_GID_PICKROOM, StringUtils::format("%d", _gameID));
	// 	boost::shared_ptr<IRequest> requestAuto(new ExtensionRequest(REQUEST_AUTO_JOIN_ROOM, param));
	// 	GameServer::getSingleton().Send(requestAuto);
	//  	SceneManager::getSingleton().showLoading(10);
	// 	return;

	vector<long> lstRoomsID;
	boost::shared_ptr<vector<boost::shared_ptr<Room> > > rooms =
		GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(this->getCurrGroupList());
	//
	double Amf = 0;
	if (this->roomType == 0)
		Amf = GameUtils::getAmfGoldUser();
	else
		Amf = GameUtils::getAmfSilverUser(GameUtils::getMyName());
	//
	log("rooms size %d", rooms->size());
	int i = 0;
	for (i = 0; i < rooms->size(); i++)
	{
		if (rooms->at(i)->IsPasswordProtected()) continue;
		boost::shared_ptr<Room> r = rooms->at(i);
		boost::shared_ptr<RoomVariable> rv = r->GetVariable("params");
		string s = *rv->GetStringValue();
		int typeRoom = 1;
		boost::shared_ptr<RoomVariable> typeRoomVar = r->GetVariable(EXT_FIELD_ROOM_TYPE);
		if (typeRoomVar != NULL)
		{
			typeRoom = *(typeRoomVar->GetIntValue());
		}
		vector<string> lstV = mUtils::splitString(s, '@');
		double roomMoney = atof(lstV[0].c_str());
		int max_user = r->MaxUsers();
		int room_user = atoi(lstV.at(2).c_str());
		if (max_user == room_user)
			continue;
		if (Amf > roomMoney && typeRoom == this->roomType)
			lstRoomsID.push_back(rooms->at(i)->Id());
	}

	if (lstRoomsID.size() == 0)
	{
		int minMoney = this->listBetFilter.at(this->currTab);
		if (Amf > minMoney)
		{
			//create room 
			int currTab = SceneManager::getSingleton().getBetFilter();
			int groupIdList = 1;
			if (currTab == 1)
				groupIdList = 5;
			else if (currTab == 2)
				groupIdList = 10;
			int roomType = SceneManager::getSingleton().getCurrRoomType();
			string _configStr = StringUtils::format("%d@0@1@%s@900@90@0-0@0@0-0@0@0@1@1", minMoney, GameUtils::getMyName().c_str());
			SceneManager::getSingleton().showLoading();
			boost::shared_ptr<ISFSObject> roomConfig(new SFSObject());
			roomConfig->PutUtfString("gid", StringUtils::format("%d%d", SceneManager::getSingleton().getGameID(), groupIdList));
			roomConfig->PutUtfString("params", _configStr);
			roomConfig->PutUtfString("rpass", "");
			roomConfig->PutInt(EXT_FIELD_ROOM_TYPE, roomType);
			boost::shared_ptr<IRequest> request(new ExtensionRequest("rcroom", roomConfig));
			GameServer::getSingleton().Send(request);
		}
		else{
			//SceneManager::getSingleton().showPopupMoneyCharge();
			//update 16/8
//            LayerPopupHetTienGB* popup = LayerPopupHetTienGB::create();
//            popup->setInPickRoom(true);
//            popup->setCallBackUngTien(CC_CALLBACK_0(LayerPickRoom::callBackFromPopupHetTien, this));
//            this->addChild(popup, 100000);
		}

		return;
	}
	SceneManager::getSingleton().showLoading();
	srand(time(NULL));
	int vt = rand() % lstRoomsID.size();
	log("vao phong id = %ld", lstRoomsID.at(vt));
	boost::shared_ptr<IRequest> request(new JoinRoomRequest(lstRoomsID.at(vt), ""));
	GameServer::getSingleton().Send(request);
}

void LayerPickRoom::sendListBetRequest()
{
	SceneManager::getSingleton().showLoading();
	int gameID = SceneManager::getSingleton().getGameID();
	boost::shared_ptr<SFSObject> param(new SFSObject());
	//param->PutUtfString(EXT_FIELD_GAME_ID, boost::to_string(gameID));
	param->PutUtfString(EXT_FIELD_GAME_ID, StringUtils::format("%d", gameID));
	param->PutInt(EXT_FIELD_ROOM_TYPE, this->roomType);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_GET_LIST_BET, param));
	GameServer::getSingleton().Send(request);

	//ban tin de cho biet la minh vao sanh, de thang trong phong co the moi duoc
	boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
	boost::shared_ptr<int> _a(new int(1));
	boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("pai", _a, VARIABLETYPE_INT));
	collectionUserVariable->push_back(variable);	//
	boost::shared_ptr<IRequest> collectRequest(new SetUserVariablesRequest(collectionUserVariable));
	GameServer::getSingleton().Send(collectRequest);
}

void LayerPickRoom::OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//update 25/7
	RejoinInfoStruct rejoinInfo;
	SceneManager::getSingleton().setRejoinInfo(rejoinInfo);
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueErrorMessage = (*ptrEventParams)["errorMessage"];
	boost::shared_ptr<string> ptrErrorMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueErrorMessage);
	//
	boost::shared_ptr<void> ptrEventParamValueErrorCode = (*ptrEventParams)["errorCode"];
	boost::shared_ptr<short> ptrErrorCode = ((boost::static_pointer_cast<short>))(ptrEventParamValueErrorCode);
	//
    SceneManager::getSingleton().hideLoading();
	log("OnSmartFoxRoomJoinError - errorCode: %d", *ptrErrorCode);
	boost::shared_ptr<string> message(new string("Join Room Failure: " + *ptrErrorMessage));
	//Error code = 20 is Room is fulls

	string mess = mUtils::getErrorSmartfoxExplain(*ptrErrorCode);

	log("Join Room Failure: %s", mess.c_str());
	if (*ptrErrorCode != 19)
		SceneManager::getSingleton().showToast(mess.c_str());
	if (*ptrErrorCode == 22)
	{
		//this->loadAllData();
	}

	isRejoinRoom = false;
}

void LayerPickRoom::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{

}

void LayerPickRoom::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf == NULL)
		return;
	if (ptrNotifiedCmd == NULL)
		return;
	if (0 == ptrNotifiedCmd->compare(AUTO_JOIN_TBL_RESP))
	{
		boost::shared_ptr<long> resptr = param->GetInt(EXT_FIELD_RESULT_CODE);
		if (resptr != NULL)
		{
			int rescode = *resptr;
			log("RESCODE = %d", rescode);
			SceneManager::getSingleton().hideLoading();
			switch (rescode)
			{
			case 0:
				//MINHDV - leave room chat
			{
					  log("join thanh cong");
			}
				break;
			case 1:
				SceneManager::getSingleton().showToast(dataManager.GetSysString(115));
				break;
			case 2:
			{
					  LayerPopupHetTien* popup = LayerPopupHetTien::create();
					  this->addChild(popup, ZORDER_LIST::ZORDER_POPUP_CHONTIEN);
			}
				break;
			default:
				break;
			}
		}
	}
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_RES_GET_LIST_BET))
	{
		log("EXT_EVENT_RES_GET_LIST_BET");
		//Khởi nghiệp:5,50,100,200|Triệu phú:500,1000,2000,5000|Đại gia:10000,20000,50000,100000
		boost::shared_ptr<string> listPtr = param->GetUtfString(EXT_FIELD_LIST_BET);
		boost::shared_ptr<long> dfz_ptr = param->GetInt(EXT_FIELD_DEFAULT_ZONE);
		if (listPtr)
		{
			string listBet = listPtr->c_str();
			//update 19/7
			int dfz = this->currTab;
			if (this->isFirstJoin)
			{
				dfz = *dfz_ptr;
				this->isFirstJoin = false;
			}

			// 			if (SceneManager::getSingleton().getListBet().size() == 0)
			// 			{
			SceneManager::getSingleton().clearListBet();
			SceneManager::getSingleton().setListBet(listBet);
			//}
			this->parseListBet(listBet);
			SceneManager::getSingleton().setBetZone(dfz);
			switch (dfz)
			{
			case 0:
				this->onButtonFilterRoom(btnTab1);
				break;
			case 1:
				this->onButtonFilterRoom(btnTab2);
				break;
			case 2:
				this->onButtonFilterRoom(btnTab3);
				break;
			default:
				break;
			}
		}
	}
	else if (0 == ptrNotifiedCmd->compare("rsilf")){
	}
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_RES_GET_LIST_ROOM))
	{
	}
	//update 11/7
	if (0 == ptrNotifiedCmd->compare(EXT_EVENT_CHAT_HISTORY_NTF))
	{
		boost::shared_ptr<string> listHistoryPtr = param->GetUtfString(EXT_FIELD_CHAT_HISTORY);
		if (listHistoryPtr)
		{
			this->parseChat(listHistoryPtr->c_str());
		}
	}
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_CHAT_NTF))
    {
        boost::shared_ptr<string> messChatPtr = param->GetUtfString(EXT_FIELD_CHAT_MESSAGE);
        boost::shared_ptr<string> userNamePtr = param->GetUtfString(EXT_FIELD_USERNAME);
        boost::shared_ptr<string> isadm = param->GetUtfString("isadm");
        if (messChatPtr && userNamePtr && isadm)
        {
            if (lstChat.size() > 10)
                lstChat.erase(lstChat.begin());
            bool isadmin = *isadm == "1";
            lstChat.push_back(structChat(userNamePtr->c_str(), messChatPtr->c_str(),isadmin));
            tblChat->reloadData();
            tblChat->setContentOffset(Vec2(0, 0));
        }
	}
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_GET_CHAT_HIS_RES))
	{
		boost::shared_ptr<string> listHistoryPtr = param->GetUtfString(EXT_FIELD_CHAT_HISTORY);
		if (listHistoryPtr)
		{
			//vector<string> listHistory = mUtils::splitString(*listHistoryPtr, '|');
			this->parseChat(listHistoryPtr->c_str());
		}
	}

    else if (0 == ptrNotifiedCmd->compare("chres")){
        auto eexp = param->GetUtfString("eexp");
        auto errc = param->GetInt("errc");
        if (eexp != NULL && errc != NULL){
            if (errc != 0){
                SceneManager::getSingleton().showToast(*eexp);
            }
        }
    }

}

void LayerPickRoom::sendListRoomRequest()
{
	SceneManager::getSingleton().showLoading();
	int gameID = SceneManager::getSingleton().getGameID();
	boost::shared_ptr<SFSObject> param(new SFSObject());
	param->PutUtfString(EXT_FIELD_GAME_ID, StringUtils::format("%d", gameID));
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_GET_LIST_ROOM, param));
	GameServer::getSingleton().Send(request);
}

void LayerPickRoom::onScheduleRejoinRoom(float dt)
{
	RejoinInfoStruct sRejoinInfo = SceneManager::getSingleton().getRejoinInfo(); int a = -1;
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()){
		a = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->Id();
	}
	//if (a == -1) return;
	boost::shared_ptr<long int> id2Left(new long int(a));
	if (sRejoinInfo.roomID != -1) {
		boost::shared_ptr<IRequest> request(new JoinRoomRequest(sRejoinInfo.roomID, sRejoinInfo.pass));
		GameServer::getSingleton().Send(request);
		return;
	}
	if (sRejoinInfo.roomName.length() > 0) {
		SceneManager::getSingleton().showLoading();
		boost::shared_ptr<IRequest> request(new JoinRoomRequest(sRejoinInfo.roomName, sRejoinInfo.pass));
		GameServer::getSingleton().Send(request);
		return;
	}
}

void LayerPickRoom::onButtonFilterRoom(Ref* pSender)
{
	if (listBetFilter.size() == 0)
		return;
	SceneManager::getSingleton().showLoading(20);
	int tagButton = ((Button*)pSender)->getTag();
	int groupList = 1;
	Color3B colorPick = Color3B(140, 74, 26);
	Color3B colorNormal = Color3B::WHITE;
	switch (tagButton)
	{
	case 36://tab1
		this->currTab = 0;
		btnTab1->setEnabled(false);
		btnTab2->setEnabled(true);
		btnTab3->setEnabled(true);
		btnTab1->setTitleColor(colorPick);
		btnTab2->setTitleColor(colorNormal);
		btnTab3->setTitleColor(colorNormal);
		break;
	case 37://tab2
		this->currTab = 1;
		btnTab1->setEnabled(true);
		btnTab2->setEnabled(false);
		btnTab3->setEnabled(true);
		groupList = 5;
		btnTab1->setTitleColor(colorNormal);
		btnTab2->setTitleColor(colorPick);
		btnTab3->setTitleColor(colorNormal);
		break;
	case 38://tab3
		this->currTab = 2;
		btnTab1->setEnabled(true);
		btnTab2->setEnabled(true);
		btnTab3->setEnabled(false);
		groupList = 10;
		btnTab1->setTitleColor(colorNormal);
		btnTab2->setTitleColor(colorNormal);
		btnTab3->setTitleColor(colorPick);
		break;
	default:
		break;
	}
	float positionX = this->lblMoney4->getPositionX();
	this->bt_cuoc2_5->setPositionX(positionX);
	Color4B colorNormalLabel = Color4B(140, 74, 26, 255);
	Color4B colorPickLabel = Color4B::WHITE;
	this->lblMoney4->setTextColor(colorPickLabel);
	this->filterBetValue = -1;
	this->lblMoney2->setTextColor(colorNormalLabel);
	this->lblMoney3->setTextColor(colorNormalLabel);
	this->lblMoney1->setTextColor(colorNormalLabel);

	SceneManager::getSingleton().setBetFilter(currTab);
	this->unSubscribeGame();
	this->setCurrGroupList(StringUtils::format("%d%d", this->currGameID, groupList));
	this->subscribeGame(this->getCurrGroupList());
	this->loadLabelByZone(this->currTab);
}

void LayerPickRoom::parseListBet(string listBet)
{
	//Khởi nghiệp:5,50,100,200|Triệu phú:500,1000,2000,5000|Đại gia:10000,20000,50000,100000

	listBetFilter.clear();
	vector<string> arrStr = mUtils::splitString(listBet, '|');
	for (int i = 0; i < arrStr.size(); i++)
	{
		vector<string> arr = mUtils::splitString(arrStr.at(i), ':');
		if (arr.size() > 1)
		{
			vector<string> arrStrBet = mUtils::splitString(arr.at(1), ',');
			for (int j = 0; j < arrStrBet.size(); j++)
			{
				listBetFilter.push_back(atoi(arrStrBet.at(j).c_str()));
			}
		}
	}
}

void LayerPickRoom::autoInvitePlay(float dt)
{
	this->unschedule(schedule_selector(LayerPickRoom::autoInvitePlay));
	bool isAutoInvite = UserDefault::getInstance()->getBoolForKey("denyAllInvite", false);
	if (!isAutoInvite)
	{
		return;
	}
	int gameID = SceneManager::getSingleton().getGameID();

	double amf = 0;
	if (this->roomType == 0)
		amf = GameUtils::getAmfGoldUser();
	else if (this->roomType == 1)
		amf = GameUtils::getAmfSilverUser(GameUtils::getMyName());
	double betUser = this->getMinBetByGame(gameID, amf);
	if (betUser < 5) //khong du tien
	{
		return;
	}
	LayerPopupInviteGame* _layer = LayerPopupInviteGame::create();
	_layer->showFakeInfo(gameID);
	_layer->setClickCallBack(CC_CALLBACK_0(LayerPickRoom::onPopupInviteCallBack, this));
	this->addChild(_layer);
}

double LayerPickRoom::getMinBetByGame(int gameID, double amf)
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
	betUser = amf / bet_ratio;
	return betUser;
}

void LayerPickRoom::registerInviteRequest()
{
	int gID = atoi(this->getCurrGroupList().c_str());
	boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
	boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("cvg", boost::make_shared<int>(gID), VARIABLETYPE_INT));
	collectionUserVariable->push_back(variable);	//
	boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
	GameServer::getSingleton().Send(request);
}

void LayerPickRoom::subscribeGame(string type)
{
	vector<string> lstGroups = mUtils::splitString(type, ';');
	for (ssize_t i = 0; i < lstGroups.size(); i++){
		if (lstGroups.at(i).compare("") == 0) continue;
		boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(lstGroups.at(i)));
		GameServer::getSingleton().Send(request);
	}
}

void LayerPickRoom::unSubscribeGame()
{
	//unsubscribe
	if (GameServer::getSingleton().getSmartFox()->RoomManager() == NULL) return;
	boost::shared_ptr<vector<string> > pRoomGroups = GameServer::getSingleton().getSmartFox()->RoomManager()->GetRoomGroups();
	if (pRoomGroups == NULL) return;
	string lstGroups = "";
	for (int i = 0; i < pRoomGroups->size(); i++){
		if (pRoomGroups->at(i).compare("200") == 0 || pRoomGroups->at(i).compare("Default") == 0)
			continue;
		boost::shared_ptr<IRequest> request(new UnsubscribeRoomGroupRequest(pRoomGroups->at(i)));
		GameServer::getSingleton().Send(request);
	}
}

void LayerPickRoom::OnSmartFoxRoomCreationError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	SceneManager::getSingleton().hideLoading();
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueErrorMessage = (*ptrEventParams)["errorMessage"];
	boost::shared_ptr<string> ptrErrorMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueErrorMessage);
	boost::shared_ptr<string> message(new string("Room Create Failure: " + *ptrErrorMessage));
	//
	SceneManager::getSingleton().showToast(message->c_str());
}

void LayerPickRoom::OnSmartFoxRoomAdd(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//	this->loadAllData();
}

void LayerPickRoom::OnSmartFoxRoomDestroy(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//this->loadAllData();
}

void LayerPickRoom::OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//update 11/7
	SceneManager::getSingleton().showLoading();
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["groupId"];
	boost::shared_ptr<string> roomID = ((boost::static_pointer_cast<string>(ptrEventParamValueRoom)));
	if (roomID->compare(SERVICE_CHAT_ROOM) == 0)
	{
		this->getHisChatReq();
	}
	else{
		this->loadAllData();
		if (SceneManager::getSingleton().getRejoinInfo().roomID != -1 || SceneManager::getSingleton().getRejoinInfo().roomName.length() > 0)
			this->scheduleOnce(schedule_selector(LayerPickRoom::onScheduleRejoinRoom), 0.2);
	}
}

void LayerPickRoom::OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	SceneManager::getSingleton().hideLoading();
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueErrorCode = (*ptrEventParams)["errorCode"];
	boost::shared_ptr<short> ptrErrorCode = ((boost::static_pointer_cast<short>))(ptrEventParamValueErrorCode);
	//
	if (*ptrErrorCode == 19 || *ptrErrorCode == 24){
		//update 11/7
		this->getHisChatReq();
		SceneManager::getSingleton().showLoading();
		this->loadAllData();
		return;
	}

	//
	SceneManager::getSingleton().showToast(dataManager.GetSysString(117));
}

void LayerPickRoom::loadAllData()
{
	//this->unschedule(schedule_selector(LayerPickRoom::onScheduleUpdateRoomList));
	//
	vector<boost::shared_ptr<Room> > lstRooms = getListRooms();

	int i = 0;
	vector<RoomInfo> _lstRoomsSort;
	for (i = 0; i < lstRooms.size(); i++){
		boost::shared_ptr<RoomVariable> rv = lstRooms.at(i)->GetVariable("params");
		if (rv == NULL)
			return;
		vector<string> lstBet = mUtils::splitString(*rv->GetStringValue(), '@');
		int room_id = lstRooms.at(i)->Id();
		int room_max_user = lstRooms.at(i)->MaxUsers();
		bool room_password = lstRooms.at(i)->IsPasswordProtected();
		int room_money = 0;
		int room_numbers = atoi(lstBet.at(2).c_str());
		int spec_count = lstRooms.at(i)->SpectatorCount();
		int typeRoom = 1;
		int groupID = 0;//0 la binh dan, 1 la trieu phu, 2 la dai gia
		boost::shared_ptr<RoomVariable> typeRoomVar = lstRooms.at(i)->GetVariable(EXT_FIELD_ROOM_TYPE);
		if (typeRoomVar != NULL)
		{
			typeRoom = *(typeRoomVar->GetIntValue());
		}
		boost::shared_ptr<RoomVariable> roomVar = lstRooms.at(i)->GetVariable("params");
		string par = roomVar->GetStringValue()->c_str();
		vector<string> listVar = mUtils::splitString(par, '@');
		bool allowView = true;
		room_money = atoi(listVar.at(0).c_str());
		if (listVar.size() >= 13)
		{
			int view_var = atoi(listVar.at(12).c_str());

			allowView = view_var == 1 ? true : false;
		}
		if (room_money < this->listBetFilter.at(currTab * 3))
			continue;

		if (this->isFriendFilter && (!GameServer::getSingleton().getSmartFox()->BuddyManager()->Inited() ||
			GameServer::getSingleton().getSmartFox()->BuddyManager()->BuddyList()->size() == 0))
			continue;
		if (this->isFriendFilter && (GameServer::getSingleton().getSmartFox()->BuddyManager()->Inited() ||
			GameServer::getSingleton().getSmartFox()->BuddyManager()->BuddyList()->size() > 0))
		{
			bool isHasFriend = false;
			boost::shared_ptr<vector<boost::shared_ptr<Buddy> > > lstBuddys = GameServer::getSingleton().getSmartFox()->BuddyManager()->BuddyList();
			for (ssize_t j = 0; j < lstBuddys->size(); j++)
			{
				boost::shared_ptr<Buddy> _buddy = lstBuddys->at(j);
				boost::shared_ptr<BuddyVariable> _gs = _buddy->GetVariable("gs");
				if (_gs != NULL)
				{
					string gsStr = _gs->GetStringValue()->c_str();
					vector<string> _arr = mUtils::splitString(gsStr, ',');
					if (_arr.size() == 2)
					{
						int _gameID = atoi(_arr.at(0).substr(0, 3).c_str());
						int _roomID = atoi(_arr.at(1).c_str());
						if (_gameID == this->currGameID && room_id == _roomID)
						{
							isHasFriend = true;
							break;
						}
					}
				}
			}
			if (!isHasFriend)
				continue;
		}
		//update 19/7
// 		else if (this->isFullRoomFilter && room_max_user != room_numbers){
// 			continue;
// 		}
// 		else if (this->isEmptyRoomFilter && room_numbers < room_max_user)
// 		{
// 			continue;
// 		}
		if (this->filterBetValue != -1)
		{
			if (room_money != this->filterBetValue)
				continue;
		}

		RoomInfo _currRoom;
		_currRoom.room_max_user = room_max_user;
		_currRoom.room_id = room_id;
		_currRoom.room_money = room_money;
		_currRoom.isPasswordProtected = room_password;
		_currRoom.room_numbers = room_numbers;
		_currRoom.allow_view = allowView;
		_currRoom.spectator_count = spec_count;
		if (this->roomType == typeRoom)
			_lstRoomsSort.push_back(_currRoom);
	}
	//sort
	if (_lstRoomsSort.size() > 0){
		for (i = 0; i < _lstRoomsSort.size() - 1; i++) {
			for (int j = i + 1; j < _lstRoomsSort.size(); j++) {
				if (_lstRoomsSort[i].room_money < _lstRoomsSort[j].room_money) {
					RoomInfo _tmp = _lstRoomsSort[i];
					_lstRoomsSort[i] = _lstRoomsSort[j];
					_lstRoomsSort[j] = _tmp;
					continue;
				}
				if (_lstRoomsSort[i].room_money > _lstRoomsSort[j].room_money) {
					RoomInfo _tmp = _lstRoomsSort[i];
					_lstRoomsSort[i] = _lstRoomsSort[j];
					_lstRoomsSort[j] = _tmp;
					continue;
				}
			}
		}
	}
	//
	lstTables.clear();
	if (_lstRoomsSort.size() > 0){
		for (i = 0; i < _lstRoomsSort.size(); i++){
			lstTables.push_back(_lstRoomsSort.at(i));
		}
	}
	//increse empty table
	//Khởi nghiệp:5,50,100,200|Triệu phú:500,1000,2000,5000|Đại gia:10000,20000,50000,100000
	// 	int moneyFake = 20;
	// 	if (this->listBetFilter.size() >= 9)
	// 		moneyFake = this->listBetFilter.at(this->currTab * 4);

	
	if (this->lstTables.size() == 0)
		i = 0;
	for (int j = i; j < 20; j++){
		int idx = j / 8;
		int moneyFake = this->listBetFilter.at(this->currTab * 3 + idx);
		if (this->filterBetValue != -1)
		{
			if (moneyFake != this->filterBetValue)
				continue;
		}
		RoomInfo _currRoom;
		_currRoom.room_money = moneyFake;
		_currRoom.isPasswordProtected = false;
		_currRoom.room_id = 0;
		_currRoom.room_max_user = 4;
		_currRoom.room_numbers = 0;
		lstTables.push_back(_currRoom);
	}
	//update 19/7
	if (this->isEmptyRoomFilter || this->isFullRoomFilter)
	{
		std::sort(lstTables.begin(), lstTables.end(), sort_size_table());
	}
	else{
		std::sort(lstTables.begin(), lstTables.end(), less_than_key());
		
	}
	tableView->reloadData();
	//
	SceneManager::getSingleton().hideLoading();
}

void LayerPickRoom::onScheduleUpdateRoomList(float dt)
{
	this->loadAllData();
}

vector<boost::shared_ptr<Room> > LayerPickRoom::getListRooms()
{
	vector<boost::shared_ptr<Room> > lstRooms;
	std::string lst = this->getCurrGroupList();
	vector<string> lstRoomGroupsName = mUtils::splitString(lst, ';');
	for (ssize_t i = 0; i < lstRoomGroupsName.size(); i++){
		if (lstRoomGroupsName.at(i).compare("") == 0) continue;
		boost::shared_ptr<vector<boost::shared_ptr<Room > > > rooms
			= GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(lstRoomGroupsName.at(i));
		for (ssize_t j = 0; j < rooms->size(); j++)
		{
			//update 21/7 - fix khong hien thi ban treo
			boost::shared_ptr<Room> ro = rooms->at(j);
			if (ro->UserCount() == 0)
				continue;
			lstRooms.push_back(rooms->at(j));
		}
	}
	return lstRooms;
}

void LayerPickRoom::onButtonBack(Ref* pSender)
{
	this->unSubscribeGame();
	SceneManager::getSingleton().setGameID(-1);
	SceneManager::getSingleton().gotoLogin();
}

void LayerPickRoom::onCheckbox(Ref* pSender, CheckBox::EventType type)
{
	int tagCb = ((CheckBox*)pSender)->getTag();
	switch (type)
	{
	case CheckBox::EventType::UNSELECTED:
	{
											switch (tagCb)
											{
											case 3229:
												this->isFullRoomFilter = true;
												break;
											case 3230:
												this->isEmptyRoomFilter = true;
												break;
											case 3231:
												this->isFriendFilter = true;
												break;
											default:
												break;
											}
	}
		break;
	case CheckBox::EventType::SELECTED:{
										   switch (tagCb)
										   {
										   case 3229:
											   this->isFullRoomFilter = false;
											   break;
										   case 3230:
											   this->isEmptyRoomFilter = false;
											   break;
										   case 3231:
											   this->isFriendFilter = false;
											   break;
										   default:
											   break;
										   }
	}
		break;
	default:
		break;
	}

	this->loadAllData();
}

void LayerPickRoom::onLabelBetValue(Ref* pSender)
{
	//update 11/7
	if (this->listBetFilter.size() == 0)
		return;

	float positionX = ((Text*)pSender)->getPositionX();
	int tagLabel = ((Text*)pSender)->getTag();
	this->bt_cuoc2_5->setPositionX(positionX);
	Color4B colorNormal = Color4B(140, 74, 26, 255);
	Color4B colorPick = Color4B::WHITE;
	int jumpInit = this->currTab * 2;
	if (tagLabel != 1003)
		this->filterBetValue = this->listBetFilter.at(this->currTab + tagLabel - 1000 + jumpInit);
	switch (tagLabel)
	{
	case 1000:
		this->lblMoney1->setTextColor(colorPick);
		this->lblMoney2->setTextColor(colorNormal);
		this->lblMoney3->setTextColor(colorNormal);
		this->lblMoney4->setTextColor(colorNormal);
		break;
	case 1001:
		this->lblMoney2->setTextColor(colorPick);
		this->lblMoney1->setTextColor(colorNormal);
		this->lblMoney3->setTextColor(colorNormal);
		this->lblMoney4->setTextColor(colorNormal);
		break;
	case 1002:
		this->lblMoney3->setTextColor(colorPick);
		this->lblMoney2->setTextColor(colorNormal);
		this->lblMoney1->setTextColor(colorNormal);
		this->lblMoney4->setTextColor(colorNormal);
		break;
	case 1003:
		this->lblMoney4->setTextColor(colorPick);
		this->filterBetValue = -1;
		this->lblMoney2->setTextColor(colorNormal);
		this->lblMoney3->setTextColor(colorNormal);
		this->lblMoney1->setTextColor(colorNormal);
		break;

	default:
		break;
	}
	this->loadAllData();

}

void LayerPickRoom::onButtonTypeRoom(Ref* pSender)
{
	int _tagButton = ((Button*)pSender)->getTag();
	if (_tagButton == 345)
	{
		//if (dataManager.getAppConfig().hidemoney == 0){
//            SceneManager::getSingleton().showToast("Hệ thống chưa mở");
//            return;
//
//        }
		//update 19/8
		this->spriteBg->setTexture("bg_vip.png");

		this->btnMoneyGold->setEnabled(false);
		this->btnMoneySilver->setEnabled(true);
		this->roomType = 0;

	}
	else if (_tagButton == 340)
	{
//        if (dataManager.getAppConfig().hidemoney == 1){
//            SceneManager::getSingleton().showToast("Hệ thống chưa mở");
//            return;
//
//        }
		//update 19/8
		this->spriteBg->setTexture("design/LayerCoTuong/sanhgame/bg.jpg");

		this->btnMoneyGold->setEnabled(true);
		this->btnMoneySilver->setEnabled(false);
		this->roomType = 1;

	}
	SceneManager::getSingleton().setCurrRoomType(this->roomType);
	this->sendListBetRequest();
}

void LayerPickRoom::loadLabelByZone(int zoneIdx)
{
	int jumpUnit = zoneIdx * 2;
	int val1 = this->listBetFilter.at(zoneIdx + jumpUnit);
	int val2 = this->listBetFilter.at(zoneIdx + 1 + jumpUnit);
	int val3 = this->listBetFilter.at(zoneIdx + 2 + jumpUnit);
	this->lblMoney1->setString(formatMoneySymbol(val1));
	this->lblMoney2->setString(formatMoneySymbol(val2));
	this->lblMoney3->setString(formatMoneySymbol(val3));
}

void LayerPickRoom::onButtonCharge(Ref *pSender)
{
	SceneManager::getSingleton().showPopupMoneyCharge();
}

void LayerPickRoom::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	if (ptrEventParams == NULL)
		return;
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	if (ptrEventParamValueUser == NULL) return;
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	if (ptrNotifiedUser == NULL) return;
	this->loadUserInfo();
}

void LayerPickRoom::loadUserInfo()
{
	string myName = GameUtils::getMyName();
	int levelUser = GameUtils::getLevelUser();
	double amfSilver = GameUtils::getAmfSilverUser(myName);
	double amfGold = GameUtils::getAmfGoldUser(myName);
	double amUser = GameUtils::getAmUser();
	int expUser = GameUtils::getExpUser();
	int _progress = dataManager.getLevelProgress(levelUser, expUser);
	string urlAvatar = GameUtils::getAvatarUser();

	this->lblName->setString(GameUtils::getNamePlayer(myName));
	this->lblMoneySilver->setString(mUtils::convertMoneyEx(amfGold));
	this->lblMoneyGold->setString(mUtils::convertMoneyEx(amfGold));
	this->prgProgress->setPercent(_progress);
	this->lblLevel->setString(StringUtils::format("EXP: %d", expUser));

	int vipType = GameUtils::getTypeVipUser();
	circleAvatar->setAvatarImage(urlAvatar);
	circleAvatar->setTypeVip(vipType);
}

void LayerPickRoom::onButtonAvatar()
{
	//update 29/7
	if (SceneManager::getSingleton().getIsFlagStore())
	{
		return;
	}

	LayerPopupProfile* profile = LayerPopupProfile::create();
	profile->setLocalZOrder(ZORDER_LIST::ZORDER_POPUP_NOTIFICATION);
	this->addChild(profile);
}

void LayerPickRoom::onEnter()
{
	Layer::onEnter();
	if (SceneManager::getSingleton().getIsFlagStore() == false)
		getNotificationFromUrl();
}

void LayerPickRoom::onExit()
{
	Layer::onExit();
	//update 11/7
	this->leaveRoomChat();
}

void LayerPickRoom::onButtonFindRoom(Ref *pSender)
{
	LayerPopupRoomFind* popup = LayerPopupRoomFind::create();
	this->addChild(popup);
}

void LayerPickRoom::onPopupInviteCallBack()
{
	this->onButtonPlayNow(NULL);
}

void LayerPickRoom::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
}

cocos2d::Size LayerPickRoom::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	int tag = table->getTag();
	if (tag == 37)
	{
		return Size(1100, 250);
	}
	else
	{
		string img_path = this->lstChat.at(idx).Content + ".png";
		bool isExist = isImageExist(img_path);
		int emo_height = 115;
		if (isExist)
		{
			return Size(600, emo_height);
		}
		else{
			string mess = this->lstChat.at(idx).Name + " : " + this->lstChat.at(idx).Content;
			return Size(600, this->getHeightForCell(mess));
		}
	}

}

cocos2d::extension::TableViewCell* LayerPickRoom::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	int tag = table->getTag();
	if (tag == 37)
	{
		return this->createCellForRoomList(table, idx);
	}
	else
	{
		return this->createCellForChat(table, idx);
	}
}

ssize_t LayerPickRoom::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	int tag = table->getTag();
	if (tag == 37)
	{
		if (lstTables.size() == 0)
			return 0;
		if (lstTables.size() < 3)
			return 1;
		int rows = lstTables.size() / 3;
		if (lstTables.size() % 3 != 0)
			rows += (lstTables.size() % 3);
		return rows;
	}
	else
	{
		return lstChat.size();
	}

}

TableViewCell* LayerPickRoom::createCellForChat(TableView* table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
	string mess = this->lstChat.at(idx).Name + " : " + this->lstChat.at(idx).Content;
	if (!cell)
		cell = new TableViewCell();
	cell->removeAllChildren();
	// 	///content
	string img_path = this->lstChat.at(idx).Content + ".png";
	bool isExist = isImageExist(img_path);
	int imgHeight = 115;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
		imgHeight = 160;
	int startX = 10;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
		startX = 40;
	if (isExist)
	{
		RichText *richText = RichText::create();
		richText->setAnchorPoint(Vec2::ZERO);
		richText->setPosition(Vec2(startX, 0));
		richText->ignoreContentAdaptWithSize(false);
		richText->setContentSize(Size(600, imgHeight));

		RichElementText *re1 = RichElementText::create(1, Color3B::YELLOW, 255, this->lstChat.at(idx).Name + " : ", kBoldFont, FONT_SIZE_SMALL);
		richText->pushBackElement(re1);

		RichElementImage *reimg = RichElementImage::create(2, Color3B::WHITE, 255, img_path);
		richText->insertElement(reimg, 1);

		richText->formatText();
		richText->setTag(123);
		cell->addChild(richText);

	}
	else{
		//richText->setContentSize(Size(panelTable->getContentSize().width - 20, getHeightForCell(mess)));
		string content_msg = this->lstChat.at(idx).Name + " : " + this->lstChat.at(idx).Content;
		Label *labelContent = Label::createWithBMFont("robotofont.fnt", content_msg, TextHAlignment::LEFT);
		labelContent->setClipMarginEnabled(true);
		//labelContent->setDimensions(panelTable->getContentSize().width - 100, getHeightForCell(mess));
		labelContent->setAnchorPoint(Vec2::ZERO);
		labelContent->setPosition(Vec2(startX, 0));
		labelContent->setColor(Color3B::WHITE);
		labelContent->setLineBreakWithoutSpace(false);
		labelContent->setScale(1.25);
		labelContent->setMaxLineWidth(600);
		cell->addChild(labelContent);

		for (int i = 0; i < (this->lstChat.at(idx).Name.length() + 1); i++)
		{
			Sprite *spriteText = labelContent->getLetter(i);
			if (spriteText)
			{
				spriteText->setColor(Color3B::YELLOW);
			}
		}
	}

	return cell;
}

TableViewCell* LayerPickRoom::createCellForRoomList(TableView* table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
	int _cTag = 123;
	Size sizeItem = Size(350, 220);
	int cols = 3;
	if (!cell) {
		cell = new TableViewCell();
		cell->setAnchorPoint(Vec2(0, 0));
		cell->autorelease();
		cell->setTag(_cTag);
		//


		Size visibleSize = Director::getInstance()->getVisibleSize();
		int width = visibleSize.width / cols;
		int heightItem = sizeItem.height;

		int startX = (visibleSize.width - sizeItem.width*cols) / 8;

		for (int i = 0; i < cols; i++){
			LayerItemRoom* _child = LayerItemRoom::create();
			float posX = (sizeItem.width)*i + 80;
			_child->setPosition(Vec2(posX, 0));
			_child->setVisible(false);
			cell->addChild(_child, 0, _cTag + i);
		}
	}
	else
	{
	}
	//Change datas
	int i = 0;
	for (i = 0; i < cols; i++){
		int index = idx * cols + i;
		if (index >= lstTables.size())
			break;
		LayerItemRoom* _child = (LayerItemRoom*)cell->getChildByTag(_cTag + i);
		if (_child){
			RoomInfo _info = lstTables.at(index);
			_child->setDatas(_info.room_id, _info.room_money, _info.room_numbers, _info.allow_view, _info.spectator_count);
			_child->setVisible(true);
		}
	}
	for (int j = i; j < cols; j++){
		LayerItemRoom* _child = (LayerItemRoom*)cell->getChildByTag(_cTag + j);
		if (_child){
			_child->setVisible(false);
		}
	}

	return cell;
}

bool LayerPickRoom::isImageExist(string url)
{
	int imgNum = 27;
	for (int i = 0; i < imgNum; i++)
	{
		string imgPath = StringUtils::format("emo_%d.png", (i + 1));
		if (0 == imgPath.compare(url))
			return true;
	}
	return false;
}

float LayerPickRoom::getHeightForCell(string _strMsg)
{
	Label* lblMsg = Label::createWithTTF(_strMsg, kNormalFont, FONT_SIZE_NORMAL + 10);
	float h = lblMsg->getContentSize().width / 600;
	return h*lblMsg->getContentSize().height;
}

void LayerPickRoom::joinRoomChat()
{
	boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(boost::to_string(CHAT_ROOM_ID)));
	GameServer::getSingleton().Send(request);
}

void LayerPickRoom::leaveRoomChat()
{
	boost::shared_ptr<IRequest> request(new UnsubscribeRoomGroupRequest(boost::to_string(CHAT_ROOM_ID)));
	GameServer::getSingleton().Send(request);
	if (roomChat)
	{
		boost::shared_ptr<IRequest> request(new LeaveRoomRequest(roomChat));
		GameServer::getSingleton().Send(request);
	}
	else{
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom)
		{
			if (0 == lastRoom->GroupId()->compare(boost::to_string(CHAT_ROOM_ID)))
			{
				boost::shared_ptr<IRequest> request(new LeaveRoomRequest(lastRoom));
				GameServer::getSingleton().Send(request);
			}
		}
	}
}

void LayerPickRoom::onButtonChat(Ref* pSender)
{
	this->doChat(this->txtChat->getString());
}

void LayerPickRoom::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> Players = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);
	boost::shared_ptr<string> _UserName = Players->Name();
	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> p_Messages = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);

	string _name = *_UserName;
	string message = *p_Messages;

//	if (this->lstChat.size() > 10)
//		this->lstChat.erase(lstChat.begin());
//	this->lstChat.push_back(structChat(_name, message));
//	this->tblChat->reloadData();
//	this->tblChat->setContentOffset(Vec2(0, 0));
}

void LayerPickRoom::parseChat(string json)
{
	rapidjson::Document document;
	if (document.Parse<0>(json.c_str()).HasParseError() == false)
	{
		for (rapidjson::SizeType i = 0; i < document.Size(); i++)
		{
			const rapidjson::Value& c = document[i];
			string username = c["name"].GetString();
			string mess = c["message"].GetString();
            bool isadm = c["admin"].GetInt() == 1;

			lstChat.push_back(structChat(username, mess,isadm));
		}
		this->tblChat->reloadData();
		this->tblChat->setContentOffset(Vec2(0, 0));
	}
	else{
		log("%d", document.GetParseError());
	}
}

void LayerPickRoom::doChat(string txt)
{
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	int acctype = GameUtils::getTypeVipUser();
	Scene *scene = CCDirector::sharedDirector()->getRunningScene();
	if (acctype < dataManager.getAppConfig().chatlevel)
	{
		SceneManager::getSingleton().showToast(StringUtils::format("Chức năng chat chỉ dành cho Vip %d trở lên ", dataManager.getAppConfig().chatlevel));
		return;
	}
	if (SceneManager::getSingleton().chatcounttime == 0) {
		SceneManager::getSingleton().chatcounttime++;
		SceneManager::getSingleton().timerchat++;
		this->startTimeOutChat();

		if (this->checkSpamChat(txt)) {
			SceneManager::getSingleton().showToast("Vui lòng ko spam tin nhắn !");
			return;
		}

	}

	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom)
	{
		string myName = SceneManager::getSingleton().getMyName();
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString(EXT_FIELD_USERNAME, myName);
		params->PutUtfString(EXT_FIELD_CHAT_MESSAGE, mUtils::filterForbidenChat(txt));
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_CHAT_REQ, params, lastRoom));
		GameServer::getSingleton().Send(request);
		this->txtChat->setText("");
	}
}

bool LayerPickRoom::checkSpamChat(string txt)
{
	if (txt.length() >= 10){
		std::transform(txt.begin(), txt.end(), txt.begin(), ::tolower);
		for (int i = 0; i < SceneManager::getSingleton().arrCurrentChat.size(); i++)
		{
			while (strstr(txt.c_str(), SceneManager::getSingleton().arrCurrentChat[i].txt.c_str())){

				if (SceneManager::getSingleton().arrCurrentChat[i].count < dataManager.getAppConfig().chattimemax){
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return false;
				}
				else{
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return true;
				}
			}
			while (strstr(SceneManager::getSingleton().arrCurrentChat[i].txt.c_str(), txt.c_str())){

				if (SceneManager::getSingleton().arrCurrentChat[i].count < dataManager.getAppConfig().chattimemax){
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return false;
				}
				else{
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return true;
				}
			}
		}
		SceneManager::chatspam a;
		a.count = 1;
		a.txt = txt;
		SceneManager::getSingleton().arrCurrentChat.push_back(a);
		return false;
	}
	return false;
}

void LayerPickRoom::timeoutChatMaxtime(float dt)
{
	SceneManager::getSingleton().timerchat++;
	log("timer chat couting!");
	if (SceneManager::getSingleton().timerchat == dataManager.getAppConfig().chattimeout*SceneManager::getSingleton().sec) {
		SceneManager::getSingleton().timerchat = 0;
		SceneManager::getSingleton().chatcounttime = 0;
		SceneManager::getSingleton().arrCurrentChat.clear();
		this->unschedule(schedule_selector(LayerPickRoom::timeoutChatMaxtime));
	}
}

void LayerPickRoom::startTimeOutChat()
{
	if (SceneManager::getSingleton().timerchat > 0){
		this->schedule(schedule_selector(LayerPickRoom::timeoutChatMaxtime), 1);
	}
}

void LayerPickRoom::getHisChatReq()
{
	boost::shared_ptr<IRequest> requestGet(new ExtensionRequest(EXT_EVENT_GET_CHAT_HIS_REQ, NULL));
	GameServer::getSingleton().Send(requestGet);

	boost::shared_ptr<vector<boost::shared_ptr<Room> > > rooms
		= GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(CHAT_ROOM_ID));
	if (rooms == NULL)
		return;
	if (rooms->size() == 0)
		return;
	boost::shared_ptr<Room> r = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<long int> leftid(new long int(r ? r->Id() : -1));
	if (leftid == NULL)
		return;
	int joidid = (rooms->at(0)->Id());
	boost::shared_ptr<IRequest> request(new JoinRoomRequest(joidid, "", leftid));
	GameServer::getSingleton().Send(request);
}

void LayerPickRoom::onButtonChuyenTien(Ref* pSender)
{
	LayerChuyenTien* layerChuyenTien = LayerChuyenTien::create();
	this->addChild(layerChuyenTien);
	UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
	UserDefault::getInstance()->flush();
}

void LayerPickRoom::onButtonUngTien(Ref* pSender)
{
	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return;

	LayerPopupUngTien* popup = LayerPopupUngTien::create();
	this->addChild(popup);
}

void LayerPickRoom::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	boost::shared_ptr<Room> roomJoin = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	if (roomJoin == NULL) return;
	if (0 != roomJoin->GroupId()->compare(boost::to_string(CHAT_ROOM_ID))) return;
	log("OnSmartFoxRoomJoin Chat");
	roomChat = roomJoin;
}

//update 19/8
void LayerPickRoom::onScheduleThongBao(float dt)
{
	if (gListNotification.size() == 0)
	{
		gListNotification = SceneManager::getSingleton().gListNotificationPickRoom;
	}
	else{
		linkPopUp = gListNotification.at(0).linkShow;
	}

	int x = lblThongBao->getPositionX();
	if (x < -1 * lblThongBao->getContentSize().width || lblThongBao->getString().compare("   ") == 0) {
		x = pnlNotify->getContentSize().width;
		//
		currIndexNotification++;
		if (currIndexNotification >= gListNotification.size())
			currIndexNotification = 0;
		string _strMsg = "   ";
		if (gListNotification.size() > 0)
			_strMsg = gListNotification[currIndexNotification].textShow;
		//
		//_strMsg = "Chào mừng bạn tới baifvip. Chúc bạn chơi game vui vẻ!";
		//txtThongBao = _strMsg;
		this->createLabelThongBao(_strMsg);
	}
	else
		x = lblThongBao->getPositionX() - 3;
	lblThongBao->setPositionX(x);
}

void LayerPickRoom::getNotificationFromUrl()
{
	if (dataManager.getAppConfig().url_notification == "")
		return;
	int gameid = SceneManager::getSingleton().getGameID();
	string url_notif = dataManager.getAppConfig().url_notification;
	string urlNotifcation = StringUtils::format("%s?gameid=%d", url_notif.c_str(), gameid);
	if (url_notif.find('new') != std::string::npos) //contain string '-'
		urlNotifcation = StringUtils::format("%sgameid=%d", "https://gamebai.net/notifyServices?", gameid);

	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(urlNotifcation.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("notification");
	request->setResponseCallback(CC_CALLBACK_2(LayerPickRoom::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
}

void LayerPickRoom::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	log("SceneManager::onHttpRequestCompleted");
	if (!response)
	{
		return;
	}

	long statusCode = response->getResponseCode();
	char statusString[64] = {};

	std::vector<char> *buffer = response->getResponseData();
	log("datas receive from [%s]: %s", response->getHttpRequest()->getTag(), buffer[0].data());
	if (strcmp(response->getHttpRequest()->getTag(), "notification") == 0){
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
		//log(datas.c_str());
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
			gListNotification.clear();
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
			linkPopUp = linkShow;
			notifiInfo.isShow = isShow;
			gListNotification.push_back(notifiInfo);
		}

	}
}

void LayerPickRoom::createLabelThongBao(string _str)
{
	Text* _lblThongBao = dynamic_cast<Text*>(pnlNotify->getChildByName("lblThongBao"));
	lblThongBao = Label::create(_str, kBoldFont, 40);
	lblThongBao->setAnchorPoint(_lblThongBao->getAnchorPoint());
	Vec2 pos = _lblThongBao->getPosition();
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
		pos = Vec2(_lblThongBao->getPositionX(), _lblThongBao->getPositionY() - 10);
	lblThongBao->setPosition(pos);
	lblThongBao->setVisible(!dataManager.getAppConfig().isHideBilling);
	pnlNotify->addChild(lblThongBao);
}

void LayerPickRoom::callBackFromPopupHetTien()
{
	int currType = SceneManager::getSingleton().getCurrRoomType();
	if (currType == 0)
	{
		this->onButtonTypeRoom(this->btnMoneySilver);
	}
	else if (currType == 1)
	{
		this->onButtonTypeRoom(this->btnMoneyGold);
	}
}




