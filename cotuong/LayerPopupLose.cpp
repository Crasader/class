#include "LayerPopupLose.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../GameServer.h"
#include "Requests/ExtensionRequest.h"
#include "ServerMessageDef.h"
#include "Requests/LeaveRoomRequest.h"
#include "gameUtils.h"
#include "MyAnimation.h"


USING_NS_CC;

// on "init" you need to initialize your instance
bool LayerPopupLose::init()
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

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupLose.csb");
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	Layout* pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	MyAnimation::createActionRemovePopup(this, TIME_AUTO_REMOVE);

	return true;
}

LayerPopupLose::LayerPopupLose()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupLose::~LayerPopupLose()
{
	GameServer::getSingleton().removeListeners(this);
}

void LayerPopupLose::onButtonBattle(Ref* pSender)
{

	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	log("onButtonBattle");
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
	GameServer::getSingleton().Send(request);
	this->removeFromParentAndCleanup(true);
}

void LayerPopupLose::onButtonCancel(Ref* pSender)
{
	//this->removeFromParentAndCleanup(true);
	// 	boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
	// 	GameServer::getSingleton().Send(request);
	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL){
		//SceneManager::getSingleton().gotoPickGame(gameID);
		return;
	}
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_LEAVE_REQ, parameter, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerPopupLose::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (0 == cmd->compare(EXT_EVENT_READY_NTF))
	{
		boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
		if (name == NULL) return;
		log("event_EXT_EVENT_READY_NTF:: name=%s", name->c_str());

		if (0 == name->compare(GameUtils::getMyName()))
		{
			this->removeFromParentAndCleanup(true);
		}
	}
}



