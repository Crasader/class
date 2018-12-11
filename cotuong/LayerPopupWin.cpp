#include "LayerPopupWin.h"
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
bool LayerPopupWin::init()
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

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupWin.csb");
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	MyAnimation::createActionRemovePopup(this, TIME_AUTO_REMOVE);
	// 		btnBattle->addClickEventListener(CC_CALLBACK_1(LayerPopupWin::onButtonBattle, this));
	// 		btnCancel->addClickEventListener(CC_CALLBACK_1(LayerPopupWin::onButtonCancel, this));
	return true;
}

LayerPopupWin::LayerPopupWin()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupWin::~LayerPopupWin()
{
	GameServer::getSingleton().removeListeners(this);
}

void LayerPopupWin::onButtonBattle(Ref* pSender)
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

void LayerPopupWin::onButtonCancel(Ref* pSender)
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

void LayerPopupWin::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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


LayerPopupPeace::LayerPopupPeace()
{

}

LayerPopupPeace::~LayerPopupPeace()
{

}

void LayerPopupPeace::onButtonBattle(Ref* pSender)
{

}

void LayerPopupPeace::onButtonCancel(Ref* pSender)
{

}

bool LayerPopupPeace::init()
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

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupPeace.csb");
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	MyAnimation::createActionRemovePopup(this, TIME_AUTO_REMOVE);
	return true;
}
