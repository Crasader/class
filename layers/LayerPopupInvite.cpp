#include "LayerPopupInvite.h"
#include "cocostudio/CocoStudio.h"

#include "Requests/JoinRoomRequest.h"

#include "../GameServer.h"
#include "../scenes/ScenePickRoom.h"
#include "../scenes/SceneMain.h"

#include "../mUtils.h"
#include "layergames/PhomMessDef.h"
#include "../cotuong/ServerMessageDef.h"
#include "cotuong/gameUtils.h"
#include "boost/smart_ptr/make_shared_object.hpp"
#include "Requests/SubscribeRoomGroupRequest.h"
#include "LayerPopupDoiMoc.h"
#include "../scenes/SceneMain.h"
USING_NS_CC;

LayerPopupInvite::LayerPopupInvite()
{
	GameServer::getSingleton().addListeners(this);
}


LayerPopupInvite::~LayerPopupInvite()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupInvite::init()
{
	if (!Layer::init())
	{
		return false;
	}

	setCurrScreen(-1);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	string csbPath = "LayerPopupInvite.csb";
	auto rootNode = CSLoader::getInstance()->createNode(csbPath);
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);

    Text* Text_1 = static_cast<Text*>(rootNode->getChildByName("Text_1"));
    Text_1->setString(dataManager.GetSysString(913));
	this->lblError = static_cast<Text*>(rootNode->getChildByName("lblError"));
	this->tfUserInvite = static_cast<TextField*>(rootNode->getChildByName("tfUserInvite"));
    this->tfUserInvite->setPlaceHolder(dataManager.GetSysString(822));
    this->btnOk = static_cast<Button*>(rootNode->getChildByName("btnOk"));
    this->btnOk->setTitleText(dataManager.GetSysString(754));
	this->btnExit = static_cast<Button*>(rootNode->getChildByName("btnExit"));
    this->btnExit->setTitleText(dataManager.GetSysString(755));
    Text* Text_2 = static_cast<Text*>(rootNode->getChildByName("Text_2"));
    Text_2->setString(dataManager.GetSysString(914));
	//add event
	this->btnOk->addTouchEventListener(CC_CALLBACK_2(LayerPopupInvite::processButtonOkTouched, this));
	this->btnExit->addTouchEventListener(CC_CALLBACK_2(LayerPopupInvite::processButtonExitTouched, this));
	return true;
}

void LayerPopupInvite::processButtonExitTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		this->removeFromParent();
	}
}

void LayerPopupInvite::processButtonOkTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		string myName = GameUtils::getMyName();
		//std::string _adn = ChanUtils::GetNamePlayer(SceneManager::getSingleton().getMyName());
		std::string _adn = GameUtils::getNamePlayer(myName);
		log("_adn: %s - userInvite: %s", _adn.c_str(), this->tfUserInvite->getString().c_str());
		if (this->tfUserInvite->getString().compare(_adn) == 0
			|| 0 == myName.compare(this->tfUserInvite->getString())){
			this->lblError->setVisible(true);
			this->lblError->setString(dataManager.GetSysString(531));
		}
		else if (this->tfUserInvite->getString().length() != 0){
			this->btnOk->setEnabled(false);
			this->lblError->setVisible(false);
			log("userInvite %s", this->tfUserInvite->getString().c_str());
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutUtfString("adn", this->tfUserInvite->getString());
			boost::shared_ptr<IRequest> request(new ExtensionRequest("adn2aireq", params));
			GameServer::getSingleton().Send(request);
		}
		else{
			this->lblError->setVisible(true);
			this->lblError->setString(dataManager.GetSysString(532));
		}
	}
}

void LayerPopupInvite::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();

	log("OnExtensionResponse %s", cmd->c_str());
	if (0 == cmd->compare("adn2airesp")){//OK
		std::string rd = *param->GetUtfString("rd");
		log("rd: %s", rd.c_str());
		this->btnOk->setEnabled(true);
		if (rd == ""){
			this->lblError->setVisible(true);
			this->lblError->setString(dataManager.GetSysString(699));
		}
		else{
			this->lblError->setVisible(false);
			boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
			if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL
				|| GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") == NULL)
				return;
			int groupId = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->c_str());
			vector<string> str_params = mUtils::splitString(*GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue(), '@');
			//double mb = atof(lstParams.at(0).c_str());
			//Send request
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutUtfString("gid", GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->c_str());
			params->PutUtfString("mb", str_params.at(0).c_str());
			params->PutUtfString("roomid", StringUtils::format("%d", GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->Id()));
			params->PutUtfString("lu", rd);
			params->PutInt("roomType", SceneManager::getSingleton().getCurrRoomType());
			boost::shared_ptr<IRequest> request(new ExtensionRequest("rilu", params));
			GameServer::getSingleton().Send(request);
			this->removeFromParent();
		}
	}
}

void LayerPopupInvite::onEnter()
{
	Layer::onEnter();
}

void LayerPopupInvite::onExit()
{
	Layer::onExit();
}

LayerPopupShowInvite::LayerPopupShowInvite()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupShowInvite::~LayerPopupShowInvite()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupShowInvite::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(false);
	this->setTag(POPUP_THONGBAO);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupShowInviteGame.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
    
    Text* Text_1 = static_cast<Text*>(rootNode->getChildByName("Text_1"));
    Text_1->setString(dataManager.GetSysString(934));
    
    Text* Text_2 = static_cast<Text*>(rootNode->getChildByName("Text_2"));
    Text_2->setString(dataManager.GetSysString(938));

	this->lblUserInvite = static_cast<Text*>(rootNode->getChildByName("lblUserInvite"));
    if (SceneManager::getSingleton().currLang == 1){
        this->lblUserInvite->setPositionX(this->lblUserInvite->getPositionX()-100);
    }
	this->lblGameName = static_cast<Text*>(rootNode->getChildByName("lblGameName"));
	this->lblBetRoomInvite = static_cast<Text*>(rootNode->getChildByName("lblBetRoom"));
	this->imgCoin = static_cast<ImageView*>(rootNode->getChildByName("imgCoin"));
	this->btnOk = static_cast<Button*>(rootNode->getChildByName("btnOk"));
	this->btnExit = static_cast<Button*>(rootNode->getChildByName("btnExit"));
    


	this->imgCoin->setVisible(false);
	//add event
	this->btnExit->addTouchEventListener(CC_CALLBACK_2(LayerPopupShowInvite::processButtonExitTouched, this));
	this->btnOk->addTouchEventListener(CC_CALLBACK_2(LayerPopupShowInvite::processButtonOkTouched, this));
    this->btnOk->setTitleText(dataManager.GetSysString(754));
    this->btnExit->setTitleText(dataManager.GetSysString(890));
 

	return true;
}

void LayerPopupShowInvite::processSetDatasToDisplay(std::string _user, std::string _passRoom, double _betRoom, int _typeRoom, int _gameId, int _roomType,string roomName)
{
	std::string _money = "";
	this->roomType = _roomType;
	this->minBet = _betRoom;
	this->roomID = _typeRoom;
    this->roomName = roomName;
    this->gameID = _gameId;
	std::string displayName = GameUtils::getNamePlayer(_user);
	log("ADN: %s - AI: %s", displayName.c_str(), _user.c_str());
	this->lblUserInvite->setString(displayName);
	if (_roomType == 0){
		_money = " Zen";
	}
	else{
		_money = " Gold";
	}
	this->lblBetRoomInvite->setString(mUtils::convertMoneyEx(_betRoom) + _money);
	this->passRoom = _passRoom;
	this->lblGameName->setString(getGameNameById(_gameId));
    string type = StringUtils::format("%d",this->gameID);
    vector<string> lstGroups = mUtils::splitString(type, ';');
    for (int i = 0; i < lstGroups.size(); i++){
        if (lstGroups.at(i).compare("") == 0) continue;
        boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(lstGroups.at(i)));
        GameServer::getSingleton().Send(request);
    }
    //
    boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
    boost::shared_ptr<int> _a(new int(this->gameID));
    boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("cvg", _a, VARIABLETYPE_INT));
    collectionUserVariable->push_back(variable);    //
    boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
    GameServer::getSingleton().Send(request);
}

void LayerPopupShowInvite::processButtonExitTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		this->removeFromParent();
	}
}

void LayerPopupShowInvite::processButtonOkTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		double amf = 0;

		if (this->roomType == 0)
			amf = GameUtils::getAmfGoldUser(GameUtils::getMyName());
		else if (this->roomType == 1)
			amf = GameUtils::getAmfSilverUser(GameUtils::getMyName());

		double betUser = this->getMinBetByGame(this->gameID, amf);
		if (betUser < this->minBet) //khong du tien
		{

			if (this->roomType == 1){
				auto _currScene = Director::getInstance()->getRunningScene();
				if (!_currScene) return;
				LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
				popup->onBtnNapHao(NULL);
				_currScene->addChild(popup,2000,9098);
                this->removeFromParent();

			}
			else{
				auto _currScene = Director::getInstance()->getRunningScene();
				if (!_currScene) return;
				LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
				popup->onBtnNapThe(NULL);
				_currScene->addChild(popup,2000,9099);
                this->removeFromParent();
			}
		}
		else{
			log("joinroom request");
			if (this->roomID == -1)
				return;
            if (this->roomName == "")
                return;
            //int idRoom = ;
            
            
    
            string sRoomName = this->roomName;
            //SamSoloP1_117_17
            boost::shared_ptr<Room>  roomPtr = GameServer::getSingleton().getSmartFox()->GetRoomByName(sRoomName);
            if (roomPtr)
            {
                if (roomPtr->IsPasswordProtected())
                {
                    
                    int a = -1;
                    boost::shared_ptr<RoomVariable> rv = roomPtr->GetVariable("params");
                    vector<string> lstParams = mUtils::splitString(*rv->GetStringValue(), '@');
                    boost::shared_ptr<long int> id2Left = boost::make_shared<long int>(a);
                    int currPlayers = atoi(lstParams.at(2).c_str());
                    bool isPlaying = (lstParams.at(1).compare("1") == 0);
                    int numOfPlayers = atoi(boost::to_string(roomPtr->MaxUsers()).c_str());
                    int realRoomID = roomPtr->Id();
                    
                    bool isSpectator = (currPlayers == numOfPlayers);
                    if (!isSpectator)
                        isSpectator = isPlaying;
                    
                    //update 12/7
                    if (currPlayers >= numOfPlayers)
                    {
                        SceneManager::getSingleton().showToast(dataManager.GetSysString(400));
                        this->removeFromParent();
                        return;
                    }
                    SceneManager::getSingleton().setGameID(this->gameID);

                    boost::shared_ptr<IRequest> request(new JoinRoomRequest(this->roomID, this->passRoom));
                    GameServer::getSingleton().Send(request);
                    SceneMain::getSingleton().prepareToPickGame();
                    //this->removeFromParent();

                }
            }
            else{
                SceneManager::getSingleton().showToast(dataManager.GetSysString(199));
                this->removeFromParent();
            }
			
		}
	}
	//}
}

std::string LayerPopupShowInvite::getGameNameById(int _gameId)
{
	switch (_gameId) {
	case kGameBaCay:
		return dataManager.GetSysString(168);
	case kGameBaCayChuong:
		return dataManager.GetSysString(169);
	case kGamePhom:
		return dataManager.GetSysString(70);
	case kGameXiTo:
		return dataManager.GetSysString(71);
	case kGameTienLenMienBac:
		return "Tiến lên MB";
	case kGameTienLenMienNam:
		return dataManager.GetSysString(173);
	case kGameLieng:
		//return dataManager.GetSysString(74);
		return dataManager.GetSysString(617);
	case kGameCoTuong:
		return dataManager.GetSysString(75);
	case kGameChan:
		return dataManager.GetSysString(176);
	case kGameCoUp:
		return dataManager.GetSysString(76);
	case kGameSam:
		return dataManager.GetSysString(1206);
	case kGameTLMNDemLa:
		return dataManager.GetSysString(1205);
	case kGameTaiXiu:
		return dataManager.GetSysString(1207);
	case kGameDemLaSolo:
		return dataManager.GetSysString(1257);
	case kGameSamSolo:
		return dataManager.GetSysString(1258);
	case kGameMauBinh:
		return dataManager.GetSysString(932);
        case kGamePoker:
            return dataManager.GetSysString(885);
        case kGameCatte:
            return dataManager.GetSysString(915);
        case kGame3Doi:
            return dataManager.GetSysString(916);
	default:
		break;
	}
	return StringUtils::format("Unknown gid: %d", _gameId);
}

double LayerPopupShowInvite::getMinBetByGame(int _gameId, double _amf)
{
	double betUser = 0;
	vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
	int bet_ratio = 1;
	for (int i = 0; i < arrBet.size(); ++i)
	{
		if (arrBet.at(i).gameid == _gameId)
		{
			bet_ratio = arrBet.at(i).ratioBet;
			break;
		}
	}
	betUser = _amf / bet_ratio;
	return betUser;
}
