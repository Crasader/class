#include "LayerItemRoom.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../mUtils.h"
#include "../GameServer.h"
#include "../SceneManager.h"
#include "ServerMessageDef.h"
#include "Requests/ExtensionRequest.h"
#include "boost/smart_ptr/make_shared.hpp"
#include <string>
#include "Requests/JoinRoomRequest.h"
#include "gameUtils.h"
USING_NS_CC;

// on "init" you need to initialize your instance

bool LayerItemRoom::init()
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

	auto rootNode = CSLoader::getInstance()->createNode("LayerItemRoomChess.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
		lblMoney = (Text *)pnlMain->getChildByName("lblMoney");
        lblMoney->setVisible(false);
		button_xu_5 = (Sprite*)pnlMain->getChildByName("button_xu_5");
		imgUserLeft = (ImageView *)pnlMain->getChildByName("imgUserLeft");
		imgUserRight = (ImageView *)pnlMain->getChildByName("imgUserRight");
		table1_1 = (Sprite*)pnlMain->getChildByName("table1_1");
		lblId = (Text*)pnlMain->getChildByName("lblId");
        lblId->setVisible(false);
		this->pnlMain->setSwallowTouches(false);
		this->pnlMain->addClickEventListener(CC_CALLBACK_1(LayerItemRoom::onButtonJoinRoom, this));
	}
	return true;
}

LayerItemRoom::LayerItemRoom()
{
}

LayerItemRoom::~LayerItemRoom()
{

}

void LayerItemRoom::setDataCell(int betvl, int userCount)
{
	int roomType = SceneManager::getSingleton().getCurrRoomType();
	int currTab = SceneManager::getSingleton().getBetFilter();
	string pathImg = "bt_tien3.png";
	if (roomType == 0)//gold
	{
		pathImg = "bt_tien4.png";
	}
	button_xu_5->setTexture(pathImg);
	string pathEmpty = "table3.png";
	string pathPeople = "table2.png";
	string pathTable = "design/LayerCoTuong/sanhgame/table1.png";
	if (currTab == 2)
	{
		pathTable = "table_dg.png";
		pathEmpty = "empty_dg.png";
		pathPeople = "pepople_dg.png";
	}

	this->betValue = betvl;
	this->userCount = userCount;
	lblMoney->setString(mUtils::formatMoneySymbol(betvl));
	table1_1->setTexture(pathTable);
	if (userCount >= 2)
	{
		imgUserLeft->loadTexture(pathPeople);
		imgUserRight->loadTexture(pathPeople);
	}
	else  if (userCount == 1)
	{
		imgUserLeft->loadTexture(pathPeople);
		imgUserRight->loadTexture(pathEmpty);
	}
	else{
		imgUserLeft->loadTexture(pathEmpty);
		imgUserRight->loadTexture(pathEmpty);
	}


}

void LayerItemRoom::onButtonJoinRoom(Ref* pSender)
{
	//update 11/7
	int roomType = SceneManager::getSingleton().getCurrRoomType();
	double amfUser = 0;
	if (roomType == 0)
	{
		amfUser = GameUtils::getAmfGoldUser();
	}
	else if (roomType == 1)
	{
		amfUser = GameUtils::getAmfSilverUser(GameUtils::getMyName());
	}
	int currTab = SceneManager::getSingleton().getBetFilter();
	int groupIdList = 1;
	if (currTab == 1)
		groupIdList = 5;
	else if (currTab == 2)
		groupIdList = 10;

	if (this->idTable == 0)
	{
		//update 11/7
		double betUser = this->getMinBetByGame(SceneManager::getSingleton().getGameID(), amfUser);

		if (betUser < betValue)
		{
			SceneManager::getSingleton().showToast("Không đủ tiền tạo bàn");
			SceneManager::getSingleton().showPopupMoneyCharge();
			return;
		}
		string _configStr = StringUtils::format("%d@0@1@%s@900@90@0-0@0@0-0@0@0@0@1", this->betValue, GameUtils::getMyName().c_str());
		SceneManager::getSingleton().showLoading(20);
		boost::shared_ptr<ISFSObject> roomConfig(new SFSObject());
		roomConfig->PutUtfString("gid", StringUtils::format("%d%d", SceneManager::getSingleton().getGameID(), groupIdList));
		roomConfig->PutUtfString("params", _configStr);
		roomConfig->PutUtfString("rpass", "");
		roomConfig->PutInt(EXT_FIELD_ROOM_TYPE, roomType);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rcroom", roomConfig));
		GameServer::getSingleton().Send(request);

		return;
	}
	boost::shared_ptr<Room> ro = GameServer::getSingleton().getSmartFox()->GetRoomById(this->idTable);
	if (ro == NULL)
	{
		string _configStr = StringUtils::format("%d@0@1@%s@900@90@0-0@0@0-0@0@0@0@1", this->betValue, GameUtils::getMyName().c_str());
		SceneManager::getSingleton().showLoading(20);
		boost::shared_ptr<ISFSObject> roomConfig(new SFSObject());
		roomConfig->PutUtfString("gid", StringUtils::format("%d%d", SceneManager::getSingleton().getGameID(), groupIdList));
		roomConfig->PutUtfString("params", _configStr);
		roomConfig->PutUtfString("rpass", "");
		roomConfig->PutInt(EXT_FIELD_ROOM_TYPE, roomType);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rcroom", roomConfig));
		GameServer::getSingleton().Send(request);
		return;
	}

	boost::shared_ptr<User> MySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (MySelf == NULL || ro->GetVariable("params") == NULL || MySelf->GetVariable("amf") == NULL)
		return;
	vector<string> rParams = mUtils::splitString(*ro->GetVariable("params")->GetStringValue(), '@');
	//int _ri = *ro->GetVariable("ri")->GetIntValue();

	//check tien khi vao xem tran dau
	// 	if (atof(rParams.at(0).c_str()) > *MySelf->GetVariable("amf")->GetDoubleValue())
	// 	{
	// 		SceneManager::getSingleton().showToast(dataManager.GetSysString(118));
	// 		return;
	// 	}

	//HoangDD Update 31/07/2014
	//mCurrentRoomIDJoin = ro->Id();
	boost::shared_ptr<RoomVariable> rv = ro->GetVariable("params");
	vector<string> lstParams = mUtils::splitString(*rv->GetStringValue(), '@');
	// counting players
	int currPlayers = atoi(lstParams.at(2).c_str());
	bool isPlaying = (lstParams.at(1).compare("1") == 0);
	int numOfPlayers = atoi(boost::to_string(ro->MaxUsers()).c_str());
	// get idroom to left
	int a = -1;
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()){
		a = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->Id();
	}
	boost::shared_ptr<long int> id2Left = boost::make_shared<long int>(a);
	//
	if (ro->IsPasswordProtected()){
		return;
	}
	//check
	bool isSpectator = (currPlayers == numOfPlayers);
	if (!isSpectator)
		isSpectator = isPlaying;
	//updated 16/5
	if (!this->allowView && isSpectator)
	{
		SceneManager::getSingleton().showToast("Bàn đã tắt chế độ vào xem");
		return;
	}
	//update 11//7 - check tiep tien nguoi choi neu join vao ban da co 1 nguoi
	double betUser = this->getMinBetByGame(SceneManager::getSingleton().getGameID(), amfUser);

	if (betUser < betValue && currPlayers == 1)
	{
		SceneManager::getSingleton().showToast("Không đủ tiền vào bàn");
		SceneManager::getSingleton().showPopupMoneyCharge();
		return;
	}

	boost::shared_ptr<IRequest> request(new JoinRoomRequest(ro->Id(), "", id2Left, isSpectator));
	GameServer::getSingleton().Send(request);
}

double LayerItemRoom::getMinBetByGame(int gameID, double amf)
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

void LayerItemRoom::setDatas(int _idtable, int betvl, int userCount, bool _allowView, int spec_count)
{
	this->idTable = _idtable;
	if (this->idTable > 0)
	{
		string roomName = GameUtils::getRoomNameByID(this->idTable);
		this->lblId->setString(roomName);
	}
	else{
		this->lblId->setString("---");
	}
	//update 19/7

	this->allowView = _allowView;
	// 	if (!this->allowView)
	// 	{
	// 		imgView->loadTexture("cantview.png");
	// 		lblCount->setVisible(false);
	// 		imgView->setPositionX(142);
	// 	}
	// 	else{
	// 		if (spec_count > 0)
	// 		{
	// 			imgView->setPositionX(129);
	// 			lblCount->setVisible(true);
	// 			lblCount->setString(StringUtils::format("%d", spec_count));
	// 		}
	// 		else{
	// 			imgView->loadTexture("views.png");
	// 			lblCount->setVisible(false);
	// 			imgView->setPositionX(142);
	// 		}
	// 	}
	this->setDataCell(betvl, userCount);
}

