#include "LayerBottomInGame.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "CommonChess.h"
#include "../SceneManager.h"
#include "Requests/ExtensionRequest.h"
#include "ServerMessageDef.h"
#include "Requests/LeaveRoomRequest.h"
#include "gameUtils.h"
USING_NS_CC;

// on "init" you need to initialize your instance
bool LayerBottomInGame::init()
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

	auto rootNode = CSLoader::getInstance()->createNode("LayerBottomInGame.csb");
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	Layout* pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
		btnBack = (Button*)pnlMain->getChildByName("btnBack");
		btnSound = (Button*)pnlMain->getChildByName("btnSound");
		btnPeace = (Button*)pnlMain->getChildByName("btnPeace");
		btnSetting = (Button*)pnlMain->getChildByName("btnSetting");
		btnLose = (Button*)pnlMain->getChildByName("btnLose");
		spVisitor = (Sprite*)pnlMain->getChildByName("spVisitor");
// 		spLine2 = (Sprite*)pnlMain->getChildByName("spLine2");
// 		spLine3 = (Sprite*)pnlMain->getChildByName("spLine3");

		btnBack->addClickEventListener(CC_CALLBACK_1(LayerBottomInGame::onButtonBack, this));
		btnSetting->addClickEventListener(CC_CALLBACK_1(LayerBottomInGame::onButtonSetting, this));
		btnPeace->addClickEventListener(CC_CALLBACK_1(LayerBottomInGame::onButtonPeace, this));
		btnLose->addClickEventListener(CC_CALLBACK_1(LayerBottomInGame::onButtonLose, this));
		btnSound->addClickEventListener(CC_CALLBACK_1(LayerBottomInGame::onButtonSound, this));

		string pathImg = "icon-batam.png";
		if (mUtils::isSoundOn())
		{
			pathImg = "icon-tatam.png";
		}
		btnSound->loadTextures(pathImg, "", "");
	}

	return true;
}

LayerBottomInGame::LayerBottomInGame()
{
	GameServer::getSingleton().addListeners(this);
}

LayerBottomInGame::~LayerBottomInGame()
{
	GameServer::getSingleton().removeListeners(this);
}

void LayerBottomInGame::onButtonBack(Ref* pSender)
{
	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL){
		//SceneManager::getSingleton().gotoPickGame(gameID);
		return;
	}
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_LEAVE_REQ, parameter, lastRoom));
	GameServer::getSingleton().Send(request);
	// 	boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
	// 	GameServer::getSingleton().Send(request);
}

void LayerBottomInGame::onButtonSetting(Ref* pSender)
{
	bool isStartGame = GameUtils::isStartedGame();
	if (isStartGame)
		return;
	if (isShowSetting)
	{
		this->btnSetting->setEnabled(false);
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom != NULL){
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PRE_CHANGE_SETTING_REQUEST, params, lastRoom));
			GameServer::getSingleton().Send(request);
		}
	}
	else{
		//SceneManager::getSingleton().showPopupNotify(dataManager.GetSysString(29));
	}
	//SceneManager::getSingleton().gotoPopup(LAYER_POPUP_ID::POPUP_CHAP_QUAN);
}

void LayerBottomInGame::onButtonPeace(Ref* pSender)
{
	if (GameUtils::isStartedGame())
		createLayerNotification(dataManager.GetSysString(22), dataManager.GetSysString(22), TAG_ALERT::TAG_PEACE_REQUEST);
}

void LayerBottomInGame::onButtonLose(Ref* pSender)
{
	if (GameUtils::isStartedGame())
		createLayerNotification(dataManager.GetSysString(21), dataManager.GetSysString(20), TAG_ALERT::TAG_LOOSE);
}

void LayerBottomInGame::onButtonSound(Ref* pSender)
{
	string pathImg = "icon-tatam.png";
	bool isOn = true;
	if (mUtils::isSoundOn())
	{
		pathImg = "icon-batam.png";
		isOn = false;
	}
	btnSound->loadTextures(pathImg, "", "");
	mUtils::setSoundConfig(isOn);
}

void LayerBottomInGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (0 == cmd->compare(EXT_EVENT_GAME_LEAVE_RES))
	{
		this->event_EXT_EVENT_GAME_LEAVE_RES(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_FOR_PEACE_NTF))
	{
		this->event_EXT_EVENT_FOR_PEACE_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_LIST_USER_UPDATE))
	{
		boost::shared_ptr<string>  listPtr = param->GetUtfString(EXT_FIELD_LIST_USER);
		if (listPtr != NULL)
		{
			listUser = listPtr->c_str();
			this->checkShowSetting();
			bool isSpec = this->checkSpectator(listPtr->c_str(), GameUtils::getMyName());
			this->showSpectatorMode(isSpec);
		}
	}
	else if (0 == cmd->compare(EXT_EVENT_PRE_CHANGE_SETTING_RESPONSE))
	{
		this->event_EXT_EVENT_PRE_CHANGE_SETTING_RESPONSE(param);
	}
}

void LayerBottomInGame::event_EXT_EVENT_GAME_LEAVE_RES(boost::shared_ptr<ISFSObject> param)
{
	int gameID = SceneManager::getSingleton().getGameID();
	boost::shared_ptr<long> rscode = param->GetInt("errc");
	if (rscode == NULL) return;
	int int_res = (int)*rscode;
	switch (int_res)
	{
	case 31:
		createLayerNotification(dataManager.GetSysString(19), dataManager.GetSysString(20), TAG_ALERT::TAG_LEAVE_GAME);
		break;
	}
}

void LayerBottomInGame::createLayerNotification(string title, string content_mess, int tag)
{
	LayerPopupAlert* layerPopup = LayerPopupAlert::create();
	Scene* currScene = Director::getInstance()->getRunningScene();
	if (currScene)
	{
		currScene->removeChildByTag(LAYER_POPUP_ID::POPUP_ALERT);
		currScene->addChild(layerPopup, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_ALERT);
		layerPopup->setContentMessage(title, content_mess);
		layerPopup->setTagCallBack(tag);
		if (tag == TAG_ALERT::TAG_PEACE_RESPONSE)
			layerPopup->runTimer();
		layerPopup->setCallBackConfirm(CC_CALLBACK_2(LayerBottomInGame::callBackFromAlert, this));
	}
}

void LayerBottomInGame::callBackFromAlert(LayerPopupAlert* sender, int tag)
{
	log("tag callback %d", tag);
	switch ((TAG_ALERT)tag)
	{
	case TAG_LEAVE_GAME:
	{
						   SceneManager::getSingleton().showLoading();
						   boost::shared_ptr<ISFSObject> parameter(new SFSObject());
						   boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
						   if (lastRoom == NULL)
							   return;
						   parameter->PutInt("cf", 1);
						   boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_LEAVE_REQ, parameter, lastRoom));
						   GameServer::getSingleton().Send(request);
	}
		break;
	case TAG_PEACE_RESPONSE:
	{
							   // gửi lại sv là mình đồng ý (rep=1)
							   boost::shared_ptr<ISFSObject> parameter(new SFSObject());
							   parameter->PutInt("rep", 1);
							   boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
							   if (lastRoom == NULL)
								   return;
							   boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REPLY_FOR_PEACE, parameter, lastRoom));
							   GameServer::getSingleton().Send(request);
	}
		break;
	case TAG_PEACE_REQUEST:
	{
							  boost::shared_ptr<ISFSObject> parameter(new SFSObject());
							  boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
							  if (lastRoom == NULL)
								  return;
							  boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_FOR_PEACE, parameter, lastRoom));
							  GameServer::getSingleton().Send(request);
	}
		break;
	case TAG_LOOSE:
	{
					  boost::shared_ptr<ISFSObject> parameter(new SFSObject());
					  boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
					  if (lastRoom == NULL)
						  return;
					  boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_IM_LOSE, parameter, lastRoom));
					  GameServer::getSingleton().Send(request);
	}
		break;
	default:
		break;
	}
}

void LayerBottomInGame::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	// Get the user parameter of the event
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	// Get the room parameter of the event
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
	boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
	//
	if (ptrNotifiedUser == NULL || ptrNotifiedRoom == NULL) return;

	if (ptrNotifiedRoom->GroupId()->compare(SERVICE_CHAT_ROOM) == 0)
		return;
	log("OnSmartFoxUserExitRoom name user = %s", ptrNotifiedUser->Name()->c_str());
	//only for taixiu
	if (ptrNotifiedUser->IsItMe()){
		int gameID = SceneManager::getSingleton().getGameID();
		SceneManager::getSingleton().gotoPickGame(gameID);
	}
	else{
		this->checkShowSetting();
	}
}

void LayerBottomInGame::event_EXT_EVENT_FOR_PEACE_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> uidptr = param->GetUtfString(EXT_FIELD_USERID);
	if (uidptr != NULL)
	{
		string strAlert = StringUtils::format("Đối thủ %s xin hòa", uidptr->c_str());
		createLayerNotification(dataManager.GetSysString(22), strAlert, TAG_ALERT::TAG_PEACE_RESPONSE);
	}

}

void LayerBottomInGame::setShowSetting(bool isShow)
{
	isShowSetting = isShow;
}

void LayerBottomInGame::checkShowSetting()
{
	bool isChuPhong = GameUtils::isBossRoom(listUser);
	if (isChuPhong)
		isShowSetting = true;
	else
		isShowSetting = false;
}

void LayerBottomInGame::event_EXT_EVENT_PRE_CHANGE_SETTING_RESPONSE(boost::shared_ptr<ISFSObject> param)
{
//    bool isFirst = SceneManager::getSingleton().getFirstConfig();
//    if (isFirst)
//    {
//        bool isBossRoom = GameUtils::isBossRoom(listUser);
//        if (isBossRoom)
//        {
//            string paramsStr = SceneManager::getSingleton().getGameConfig();
//            boost::shared_ptr<ISFSObject> params(new SFSObject());
//            boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
//            if (lastRoom != NULL){
//                log("EXT_EVENT_CHANGE_SETTING_REQUEST param = %s ", paramsStr.c_str());
//                params->PutUtfString("params", paramsStr);
//                params->PutUtfString("uid", GameUtils::getMyName());
//                boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_CHANGE_SETTING_REQUEST, params, lastRoom));
//                GameServer::getSingleton().Send(request);
//            }
//        }
//    }
//    else{
//        this->btnSetting->setEnabled(true);
//        //SceneManager::getSingleton().showPopupSetting(listUser);
//    }

}

void LayerBottomInGame::showSpectatorMode(bool isSpec)
{
	btnPeace->setVisible(!isSpec);
	btnSetting->setVisible(!isSpec);
	btnLose->setVisible(!isSpec);
	spVisitor->setVisible(isSpec);
}

bool LayerBottomInGame::checkSpectator(string listUser, string myName)
{
	vector<string> arr = mUtils::splitString(listUser, ';');
	int size = arr.size();
	for (int i = 0; i < size; i++){
		vector<string> arrInfo = mUtils::splitString(arr.at(i), '|');
		if (arrInfo.size() < 2) continue;
		string name = arrInfo.at(0);
		if (0 == name.compare(myName)) return false;
	}

	return true;
}



