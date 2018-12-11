#include "LayerCreateRoomChess.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../SceneManager.h"
#include "ServerMessageDef.h"
#include "Requests/ExtensionRequest.h"
#include "gameUtils.h"
#include "Entities/Variables/SFSUserVariable.h"
#include "Requests/SetUserVariablesRequest.h"
#include "LayerPopupAlert.h"
#include "MyAnimation.h"
#include "Requests/SetRoomVariablesRequest.h"
#include "Entities/Variables/SFSRoomVariable.h"
#include "Requests/Buddylist/SetBuddyVariablesRequest.h"
#include "Entities/Variables/SFSBuddyVariable.h"
#include "boost/smart_ptr/make_shared.hpp"
#include "LayerPickRoom.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool LayerCreateRoomChess::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	return true;
}


LayerCreateRoomChess::LayerCreateRoomChess()
{
	this->layerChapQuan = NULL;
	GameServer::getSingleton().addListeners(this);
}

LayerCreateRoomChess::~LayerCreateRoomChess()
{
	//update 17/7
	if (this->layerChapQuan != NULL)
		this->layerChapQuan->removeFromParent();
	GameServer::getSingleton().removeListeners(this);
}

void LayerCreateRoomChess::onButtonAddTime(Ref *pSender){

}

void LayerCreateRoomChess::onButtonChapQuan(Ref *pSender){
	//update 10/7
	if (this->layerChapQuan == NULL)
	{
		this->layerChapQuan = LayerPopupChapQuan::create();
		this->layerChapQuan->setVisible(false);
		this->layerChapQuan->setCallBackConfirm(CC_CALLBACK_3(LayerCreateRoomChess::callBackFromPopup, this));
		Director::getInstance()->getRunningScene()->addChild(this->layerChapQuan, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_CHAP_QUAN);
	}
	else{
		this->layerChapQuan->setVisible(true);
	}
	//update 12/7
	layerChapQuan->loadFromConfig(this->configChapQuan);
	MyAnimation::createActionShowPopup(this->layerChapQuan, 0.2f);
}
void LayerCreateRoomChess::onButtonMinusTime(Ref *pSender){

}
void LayerCreateRoomChess::onButtonCreateRoom(Ref *pSender){
	//neu la ingame thi se gui ban tin thay doi room variable
	//update 25/7
	if (this->getCanSetting() == false)
	{
		SceneManager::getSingleton().showPopupNotify("Bạn không thể thay đổi cài đặt trận đấu!");
		return;
	}

	if (betValue <= 0)
		return;
	//update 25/7
	if (this->betValue > this->listBetConfig.at(this->listBetConfig.size() - 1))
	{
		string strMax = mUtils::convertMoneyEx(this->listBetConfig.at(this->listBetConfig.size() - 1));
		SceneManager::getSingleton().showToast(StringUtils::format("Số tiền cược quá mức tối đa cho phép là %s", strMax.c_str()));
		return;
	}
	else if (this->betValue < this->listBetConfig.at(0))
	{
		string strMax = mUtils::convertMoneyEx(this->listBetConfig.at(0));
		SceneManager::getSingleton().showToast(StringUtils::format("Số tiền cược dưới mức tối thiểu cho phép là %s", strMax.c_str()));
		return;
	}

	double amfUser = 0;
	int typeRoom = SceneManager::getSingleton().getCurrRoomType();
	if (typeRoom == 0)
		amfUser = GameUtils::getAmfGoldUser(GameUtils::getMyName());
	else if (typeRoom == 1)
		amfUser = GameUtils::getAmfSilverUser(GameUtils::getMyName());
	if (amfUser <= 0 || betValue > amfUser)
	{
		//update 17/7
		SceneManager::getSingleton().showToast("Không đủ tiền tạo bàn");
		SceneManager::getSingleton().showPopupMoneyCharge();
		return;
	}
	int _gameID = SceneManager::getSingleton().getGameID();
	double betUser = this->getMinBetByGame(_gameID, amfUser);
	if (betUser <= betValue)
	{
		//update 17/7
		SceneManager::getSingleton().showToast("Không đủ tiền tạo bàn");
		SceneManager::getSingleton().showPopupMoneyCharge();
		return;
	}

	int timeConfig = layerStepTime->getValueConfig();
	int chapTienConfig = layerStepChapTien->getValueConfig();
	int chapHoa = chbChaphoa->isSelected() ? 1 : 0;
	int allowSetting = chbAllow->isSelected() ? 1 : 0;
	int allowViews = chbView->isSelected() ? 1 : 0;

	string _configStr = generateConfigString(betValue, timeConfig, 90, configChapQuan, chapTienConfig, "0-0", 0, chapHoa, allowSetting, allowViews);
	if (isInGame)
	{
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom != NULL){
			params->PutUtfString("params", _configStr);
			params->PutUtfString("uid", GameUtils::getMyName());
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_CHANGE_SETTING_REQUEST, params, lastRoom));
			GameServer::getSingleton().Send(request);
			this->removeFromParentAndCleanup(true);
		}
	}
	else{

		if (betValue == 0)
			return;
		//create room 
		int currTab = SceneManager::getSingleton().getBetFilter();
		int currRoomType = SceneManager::getSingleton().getCurrRoomType();
		int groupIdList = 1;
		if (currTab == 1)
			groupIdList = 5;
		else if (currTab == 2)
			groupIdList = 10;

        log("tao room %d", SceneManager::getSingleton().getGameID());
		SceneManager::getSingleton().showLoading();
		boost::shared_ptr<ISFSObject> roomConfig(new SFSObject());
		roomConfig->PutUtfString("gid", StringUtils::format("%d%d", SceneManager::getSingleton().getGameID(), groupIdList));
		roomConfig->PutUtfString("params", _configStr);
		roomConfig->PutUtfString("rpass", "");
		roomConfig->PutInt("roomType", currRoomType);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rcroom", roomConfig));
		GameServer::getSingleton().Send(request);

	}

}
void LayerCreateRoomChess::onButtonInvitePlay(Ref *pSender){
}

void LayerCreateRoomChess::onButtonQuickPlay(Ref* pSender)
{
	LayerPickRoom* parent = dynamic_cast<LayerPickRoom*>(this->getParent());
	if (parent)
	{
		parent->onButtonPlayNow(NULL);
	}
}

void LayerCreateRoomChess::sliderEvent(Ref *pSender, Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		// 		int percent = sldMoney->getPercent();
		// 		this->setValueBetFromSlider(percent);
	}
}

void LayerCreateRoomChess::setValueBetFromSlider(int _percent)
{
	int startX = 158;
	float currBet = 0;
	if (_percent == 0)
	{
		currBet = minValue;
	}
	else if (_percent == 100)
	{
		currBet = maxValue;
	}
	else{
		currBet = ((maxValue - minValue) / 100)*_percent;
	}

	betValue = formatValue(currBet);
	// 	spMoney->setPositionX(startX + (_percent * 7.5f));
	// 	lblTienCuoc->setString(StringUtils::format("%.0f", betValue));
}

// void LayerCreateRoom::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
// {
// 	///loi nay hay bi nen bi unschedule luon
// 	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
// 	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
// 	boost::shared_ptr<Room> roomJoin = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
// 	// 	if (!roomJoin->IsPasswordProtected())
// 	// 		SceneManager::getSingleton().setAutoCreateRoom(true);
// 	if (roomJoin->GroupId()->compare(SERVICE_CHAT_ROOM) == 0)
// 		return;
// 	int gameID = SceneManager::getSingleton().getGameID();
// 	log("Join Room In Chon Ban Choi ID = %d ------------------------------", gameID);
// 
// 	string strGameID = StringUtils::format("%d", gameID);
// 	if (strGameID.length() > 3)
// 		strGameID = strGameID.substr(0, 3);
// 
// 	//Update, khong chap nhan loi moi nua
// 	boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
// 	boost::shared_ptr<int> _a(new int(0));
// 	boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("pai", _a, VARIABLETYPE_INT));
// 	collectionUserVariable->push_back(variable);	//
// 	boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
// 	GameServer::getSingleton().Send(request);
// 	//set buddy roomid join
// 	boost::shared_ptr<list<boost::shared_ptr<BuddyVariable> > > myVars(new list<boost::shared_ptr<BuddyVariable> >());
// 	boost::shared_ptr<SFSBuddyVariable> roomIDVar(new SFSBuddyVariable("roomID", boost::make_shared<string>(StringUtils::format("%d", roomJoin->Id())), VARIABLETYPE_STRING));
// 	boost::shared_ptr<SFSBuddyVariable> gameIDVar(new SFSBuddyVariable("gameID", boost::make_shared<string>(StringUtils::format("%d", gameID)), VARIABLETYPE_STRING));
// 	myVars->push_back(roomIDVar);
// 	myVars->push_back(gameIDVar);
// 	boost::shared_ptr<IRequest> requestBuddy(new Buddylist::SetBuddyVariablesRequest(myVars));
// 	GameServer::getSingleton().Send(requestBuddy);
// 
// 	SceneManager::getSingleton().gotoGame(gameID);
// }

float LayerCreateRoomChess::formatValue(float value)
{
	if (value >= 1 && value < 1000)
		return floorf(value / 100) * 100;
	else if (value >= 1000 && value < 10000)
		return floorf(value / 1000) * 1000;
	else if (value >= 10000 && value < 100000)
		return floorf(value / 1000) * 1000;
	else if (value >= 100000 && value < 1000000)
		return floorf(value / 10000) * 10000;
	else if (value >= 1000000 && value < 10000000)
		return floorf(value / 100000) * 100000;
	else if (value >= 10000000 && value < 100000000)
		return floorf(value / 1000000) * 1000000;
	else
		return 0;
}

void LayerCreateRoomChess::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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
				SceneManager::getSingleton().showPopupMoneyCharge();
				break;
			default:
				break;
			}
		}
	}
}

void LayerCreateRoomChess::onButtonHelpChapTien(Ref* pSender)
{
	this->showPopupInfo(dataManager.GetSysString(261), dataManager.GetSysString(263));
}

void LayerCreateRoomChess::onButtonHelpChapHoa(Ref* pSender)
{
	this->showPopupInfo(dataManager.GetSysString(262), dataManager.GetSysString(264));
}

void LayerCreateRoomChess::showPopupInfo(string title, string message)
{
	Scene* currScene = Director::getInstance()->getRunningScene();
	if (currScene != NULL)
	{
		LayerPopupAlert* layerPopup = dynamic_cast<LayerPopupAlert*>(currScene->getChildByTag(LAYER_POPUP_ID::POPUP_ALERT));
		if (layerPopup == NULL)
		{
			layerPopup = LayerPopupAlert::create();
			currScene->addChild(layerPopup, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_ALERT);
		}
		layerPopup->showOnlyButtonAccept();
		layerPopup->setContentMessage(title, message);
	}
}

void LayerCreateRoomChess::callBackFromPopup(LayerPopupChapQuan* sender, string _config, string _display)
{
	log("callBackFromPopup config = %s", _config.c_str());
	configChapQuan = _config;
	if (0 == _config.compare("0-0"))
	{
		lblQuanChap->setString("");
	}
	else{

		lblQuanChap->setString(StringUtils::format("%s", GameUtils::convertConfigToString(_display).c_str()));
		//this->convertConfigString(_config);
	}
}

void LayerCreateRoomChess::setPopupInGame(bool _isInGame)
{
	isInGame = _isInGame;
	string csName = "LayerCreateRoom.csb";
	if (_isInGame)
	{
		csName = "LayerCreateRoomInGame.csb";
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode(csName);
	rootNode->setContentSize(Size(visibleSize.width, rootNode->getContentSize().height));
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	Layout* Panel_1 = (Layout*)rootNode->getChildByName("Panel_1");
	if (Panel_1)
	{
		Panel_1->setTouchEnabled(true);
		Panel_1->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonQuit, this));
		Panel_1->setScale(visibleSize.width / Panel_1->getContentSize().width, visibleSize.height / Panel_1->getContentSize().height);
	}

	pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
		Button* btnChapQuan = (Button *)pnlMain->getChildByName("btnChapQuan");

		btnCreateRoom = (Button *)pnlMain->getChildByName("btnCreateRom");
		btnPlay = (Button*)pnlMain->getChildByName("btnPlay");
		btnCreate = (Button*)pnlMain->getChildByName("btnCreate");


		Button*  btnHelpTien = (Button*)pnlMain->getChildByName("btnHelpTien");
		Button* btnHelpChap = (Button*)pnlMain->getChildByName("btnHelpChap");

		Node* nodeStep1 = (Node*)pnlMain->getChildByName("nodeStep1");
		Node* nodeStep2 = (Node*)pnlMain->getChildByName("nodeStep2");

		layerStepTime = LayerStep::create();
		layerStepTime->setPosition(nodeStep1->getPosition());
		pnlMain->addChild(layerStepTime);

		layerStepChapTien = LayerStep::create();
		layerStepChapTien->setPosition(nodeStep2->getPosition());
		pnlMain->addChild(layerStepChapTien);

		//chon muc cuoc kieu moi

		//spMoney = (Sprite*)pnlMain->getChildByName("spMoney");
		//sldMoney = (Slider *)pnlMain->getChildByName("Slider_1");
		//lblTienCuoc = (TextBMFont*)spMoney->getChildByName("lblTienCuoc");

		chbAllow = (CheckBox *)pnlMain->getChildByName("chbAllow");
		chbChaphoa = (CheckBox *)pnlMain->getChildByName("chbChaphoa");
		lblTime = (Text *)pnlMain->getChildByName("lblTime");
		lblQuanChap = (Text *)pnlMain->getChildByName("lblQuanChap");
		chbView = (CheckBox*)pnlMain->getChildByName("chbView");
		Button* btnClose = (Button*)pnlMain->getChildByName("btnClose");
		Sprite* Sprite_2_0 = (Sprite*)pnlMain->getChildByName("Sprite_2_0");
		Sprite* Sprite_2_0_0 = (Sprite*)pnlMain->getChildByName("Sprite_2_0_0");
		Sprite* Sprite_2_0_1 = (Sprite*)pnlMain->getChildByName("Sprite_2_0_1");
		int roomType = SceneManager::getSingleton().getCurrRoomType();
		string pathImg = "bt_tien3.png";
		if (roomType == 0)//gold
		{
			pathImg = "bt_tien4.png";
		}
		Sprite_2_0->setTexture(TextureCache::getInstance()->addImage(pathImg));
		Sprite_2_0_0->setTexture(TextureCache::getInstance()->addImage(pathImg));
		Sprite_2_0_1->setTexture(TextureCache::getInstance()->addImage(pathImg));

		if (btnCreate)
			btnCreate->setEnabled(false);
		int gameID = SceneManager::getSingleton().getGameID();
		if (gameID == kGameCoUp)
			btnChapQuan->setEnabled(false);

		chbChaphoa->setSelectedState(false);

		btnChapQuan->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonChapQuan, this));
		btnHelpTien->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonHelpChapTien, this));
		btnHelpChap->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonHelpChapHoa, this));
		//pnlMain->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonQuit, this));
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonQuit, this));

		if (btnPlay)
			btnPlay->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonQuickPlay, this));
		btnCreateRoom->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonCreateRoom, this));

		this->initUICuoc();
		this->initFirst();
		this->loadRoomVariables();
	}
}

void LayerCreateRoomChess::initFirst()
{
	//sldMoney->setPercent(0);
	minValue = 5;
	maxValue = (int)(GameUtils::getAmfGoldUser() / BET_RATE);
	//lblTienCuoc->setString("0");
	configChapQuan = "0-0";

	static const int arr[] = { 5, 10, 15, 20, 30, 45 };
	vector<int> timeArr(arr, arr + sizeof(arr) / sizeof(arr[0]));
	//update 25/7
	static const int arr2[] = { 0, 1, 2, 3, 4 };
	vector<int> chapArr(arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]));

	layerStepTime->setData(timeArr);
	layerStepTime->setTypeStep(LayerStep::TIME);
	layerStepTime->loadData();
	//update 11/7
	this->layerStepTime->setSelectedIndex(2);

	layerStepChapTien->setData(chapArr);
	layerStepChapTien->setTypeStep(LayerStep::CHAP_TIEN);
	layerStepChapTien->loadData();
	//update 25/7
	this->layerStepChapTien->setSelectedIndex(0);

	int _betValue = (int)(GameUtils::getRoomBet());
	if (_betValue > 0)
	{
		betValue = _betValue;
		float percent = (float)((_betValue / (double)maxValue) * 100);
		// 		sldMoney->setPercent(percent);
		// 		lblTienCuoc->setString(StringUtils::format("%d", _betValue));
	}
	//update 25/7
	this->chbAllow->setSelectedState(false);
}

std::string LayerCreateRoomChess::generateConfigString(int betValue, int totalTime, int turnTime, string chapquan, int chaptien, string cuocquan, int chapthoigian, int chaphoa, int allowsetting, int allowview)
{
	//money-isPlaying-numOfPlayer-uid-tong_thoigian-thoi gian 1 luot - chap quan- chap tien - cuoc quan - chap thoi gian 
	//- chap hoa cho duoc - cho phep nguoi khac thay doi thiet lap phong- cho phep vao xem - grouplist id
	string configStr = StringUtils::format("%d@%d@%d@%s@%d@%d@%s@%d@%s@%d@%d@%d@%d",
		betValue, 0, 1, GameUtils::getMyName().c_str(), totalTime, turnTime, chapquan.c_str(), chaptien, cuocquan.c_str(), chapthoigian, chaphoa, allowsetting, allowview);
	return configStr;
}

void LayerCreateRoomChess::loadRoomVariables()
{
	boost::shared_ptr<Room> lastJoinRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastJoinRoom != NULL)
	{
		boost::shared_ptr<RoomVariable> rv = lastJoinRoom->GetVariable("params");
		if (rv != NULL){
			string s = *rv->GetStringValue();
			log("Room %s update RoomVariables: %s", lastJoinRoom->Name()->c_str(), s.c_str());
		}
		boost::shared_ptr<RoomVariable> roomUidPtr = lastJoinRoom->GetVariable("uid");
		if (roomUidPtr != NULL)
		{
			string s = *roomUidPtr->GetStringValue();
			log("uid = %s", s.c_str());
		}
	}

}

void LayerCreateRoomChess::onButtonQuit(Ref* pSender)
{
	//update 25/7
	if (isInGame)
	{
		if (this->numUser > 1)
		{
			if (this->getCanSetting())
				SceneManager::getSingleton().showToast("Vui lòng bấm thiết lập để cài đặt phòng!");
			else
				this->removeFromParentAndCleanup(true);
		}
		else{
			this->removeFromParentAndCleanup(true);
		}
	}	
	else{
		this->removeFromParentAndCleanup(true);
	}

}

void LayerCreateRoomChess::setListUser(string listUser)
{
	bool isBossRoom = GameUtils::isBossRoom(listUser);
	if (isBossRoom)
	{
		chbAllow->setEnabled(true);
	}
	else{
		chbAllow->setEnabled(false);
	}

	//update 17/7
	this->enemyUser = "";
	vector<string> userArr = mUtils::splitString(listUser, ';');
	for (unsigned int i = 0; i < userArr.size(); i++)
	{
		vector<string> listInfo = mUtils::splitString(userArr.at(i), '|');
		if (listInfo.size() >= 3)
		{
			string _name = listInfo.at(0);
			if (_name.compare(GameUtils::getMyName()) != 0)
			{
				this->enemyUser = _name;
				break;
			}
		}

	}
	//update 17/7
	this->numUser = userArr.size();
}

void LayerCreateRoomChess::initUICuoc()
{
	lblBetValue = (Text*)pnlMain->getChildByName("lblBetValue");
	lblValueTab1 = (Text*)pnlMain->getChildByName("lblValueTab1");
	lblValueTab2 = (Text*)pnlMain->getChildByName("lblValueTab2");
	lblValueTab3 = (Text*)pnlMain->getChildByName("lblValueTab3");

	btnTab1 = (Button*)pnlMain->getChildByName("btnTab1");
	btnTab2 = (Button*)pnlMain->getChildByName("btnTab2");
	btnTab3 = (Button*)pnlMain->getChildByName("btnTab3");
	btnClear = (Button*)pnlMain->getChildByName("btnClear");

	btnTab1->setTag(1000);
	btnTab2->setTag(1001);
	btnTab3->setTag(1002);

	int betzone = SceneManager::getSingleton().getBetZone();
	string listBet = SceneManager::getSingleton().getListBet();
	vector<string> listConfig = mUtils::splitString(listBet, '|');
	// 	for (size_t i = 0; i < listConfig.size(); i++)
	// 	{
	if (listConfig.size() == 0)
		return;
	int currTab = SceneManager::getSingleton().getBetFilter();
	vector<string> mlist = mUtils::splitString(listConfig.at(currTab), ':');
	if (mlist.size() == 2)
	{
		vector<string> arrBet = mUtils::splitString(mlist.at(1), ','); //tinh tu 1 den 3
		if (arrBet.size() >= 3)
		{
			for (size_t j = 0; j < arrBet.size(); j++)
			{
				listBetConfig.push_back(atoi(arrBet.at(j).c_str()));
			}
		}
		//}
		//set default betvalue
		betValue = listBetConfig.at(0);
		lblBetValue->setString(StringUtils::format("%d", betValue));
		lblValueTab1->setString(StringUtils::format("%d", listBetConfig.at(0)));
		lblValueTab2->setString(StringUtils::format("%d", listBetConfig.at(1)));
		lblValueTab3->setString(StringUtils::format("%d", listBetConfig.at(2)));

		btnTab1->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonTabBet, this));
		btnTab2->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonTabBet, this));
		btnTab3->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonTabBet, this));
		btnClear->addClickEventListener(CC_CALLBACK_1(LayerCreateRoomChess::onButtonClearBet, this));
	}

}

void LayerCreateRoomChess::onButtonTabBet(Ref* pSender)
{
	int tagButton = ((Button*)pSender)->getTag() - 1000;
	//update 17/7
	int currRoomType = SceneManager::getSingleton().getCurrRoomType();
	int enemyAMF = GameUtils::getAmfGoldUser(this->enemyUser);
	if (currRoomType == 1)
		enemyAMF = GameUtils::getAmfSilverUser(this->enemyUser);

	int betValBuff = betValue + listBetConfig.at(tagButton);
	if (betValBuff > enemyAMF && this->enemyUser.compare("") != 0)
	{
		SceneManager::getSingleton().showToast("Số tiền cược không hợp lệ");
		return;
	}

	betValue = betValBuff;
	lblBetValue->setString(StringUtils::format("%d", betValue));
}

void LayerCreateRoomChess::onButtonClearBet(Ref* pSender)
{
	betValue = listBetConfig.at(0);
	lblBetValue->setString(StringUtils::format("%d", betValue));
}

double LayerCreateRoomChess::getMinBetByGame(int gameID, double amf)
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

void LayerCreateRoomChess::setConfigFromString(string configStr)
{
	/*
	string configStr = StringUtils::format("%d@%d@%d@%s@%d@%d@%s@%d@%s@%d@%d@%d@%d@%d",
	betValue, 0, 1, GameUtils::getMyName().c_str(), totalTime, turnTime, chapquan.c_str(),
	chaptien, cuocquan.c_str(), chapthoigian, chaphoa, allowsetting, allowview);
	//chap quan 	//Tốt	Sĩ	Tượng	Mã	Pháo	Xe
	//	0	1	2		3	4		5
	id-vitri, - chi quan tam den so luong, nen se dem thong so dau tien
	*/
	vector<string> listInfo = mUtils::splitString(configStr, '@');
	if (listInfo.size() == 13)
	{
		int betvl = atoi(listInfo[0].c_str());
		int totalTime = atoi(listInfo[4].c_str()) / 60;
		int turnTime = atoi(listInfo[5].c_str());
		string chapQuan = listInfo[6].c_str();
		int chapTien = atoi(listInfo[7].c_str());
		string cuocQuan = listInfo[8];
		int chap_tg = atoi(listInfo[9].c_str());
		int chaphoa = atoi(listInfo[10].c_str());
		int allow_setting = atoi(listInfo[11].c_str());
		int allow_view = atoi(listInfo[12].c_str());

		string chapquan_str = "";
		int demtot = 0;
		int demSi = 0;
		int demTuong = 0;
		int demMa = 0;
		int demPhao = 0;
		int demXe = 0;
		vector<string> listchapQuan = mUtils::splitString(chapQuan, ',');
		for (int i = 0; i < listchapQuan.size(); i++)
		{
			vector<string> infos = mUtils::splitString(listchapQuan[i], '-');
			if (infos.size() == 2)
			{
				int idQuan = atoi(infos[0].c_str());
				//update 12/7
				int vitri = atoi(infos[1].c_str());
				if (vitri == 0)
					continue;
				switch (idQuan)
				{
				case 0:
					demtot++;
					break;
				case 1:
					demSi++;
					break;
				case 2:
					demTuong++;
					break;
				case 3:
					demMa++;
					break;
				case 4:
					demPhao++;
					break;
				case 5:
					demXe++;
					break;
				default:
					break;
				}
			}
		}
		if (demtot > 0)
			chapquan_str.append(StringUtils::format("%d Tốt,", demtot));
		if (demSi > 0)
			chapquan_str.append(StringUtils::format("%d Sĩ,", demSi));
		if (demTuong > 0)
			chapquan_str.append(StringUtils::format("%d Tượng,", demTuong));
		if (demMa > 0)
			chapquan_str.append(StringUtils::format("%d Mã,", demMa));
		if (demPhao > 0)
			chapquan_str.append(StringUtils::format("%d Pháo,", demPhao));
		if (demXe > 0)
			chapquan_str.append(StringUtils::format("%d Xe,", demXe));

		this->betValue = betvl;
		this->lblBetValue->setString(StringUtils::format("%d", this->betValue));
		this->layerStepTime->loadIndexFromValue(totalTime);
		this->layerStepChapTien->loadIndexFromValue(chapTien);
		this->chbChaphoa->setSelected(chaphoa == 1 ? true : false);
		this->chbAllow->setSelected(allow_setting == 1 ? true : false);
		this->chbView->setSelected(allow_view == 1 ? true : false);
		this->lblQuanChap->setString(chapquan_str);
		this->configChapQuan = chapQuan;
	}
}

void LayerCreateRoomChess::disableSetting()
{
	this->btnCreateRoom->setEnabled(false);
	this->setCanSetting(false);
}
