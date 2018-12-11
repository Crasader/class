#include "LayerPopupReady.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "CommonChess.h"
#include "Requests/LeaveRoomRequest.h"
#include "ServerMessageDef.h"
#include "Requests/ExtensionRequest.h"
#include "gameUtils.h"
#include "Requests/SpectatorToPlayerRequest.h"
USING_NS_CC;

// on "init" you need to initialize your instance
bool LayerPopupReady::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//    auto rootNode = CSLoader::createNode("LayerChonGame.csb");
	//    this->addChild(rootNode);
	this->setTag(LAYER_POPUP_ID::POPUP_READY);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupReady.csb");
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	myName = GameUtils::getMyName();

	Layout* pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
		pnlMain->setSwallowTouches(false);
		btnReady = (Button*)pnlMain->getChildByName("btnReady");
		btnLeave = (Button*)pnlMain->getChildByName("btnLeave");
		lblTimer = (Text *)pnlMain->getChildByName("lblTimer");
		lblContent = (Text*)pnlMain->getChildByName("lblContent");

		lblTimer->setString(StringUtils::format("00:%d", time_buff));

		btnReady->addClickEventListener(CC_CALLBACK_1(LayerPopupReady::onButtonReady, this));
		btnLeave->addClickEventListener(CC_CALLBACK_1(LayerPopupReady::onButtonLeave, this));

	}
	return true;
}

LayerPopupReady::LayerPopupReady()
{
	GameServer::getSingleton().addListeners(this);
	time_count = DEFAULT_TIME_READY;
	time_buff = DEFAULT_TIME_READY;
	isIamReady = false;
}

LayerPopupReady::~LayerPopupReady()
{
	GameServer::getSingleton().removeListeners(this);
}

void LayerPopupReady::startTimer()
{
	time_buff = time_count;
	isBossRoom = false;
	lblContent->setString(StringUtils::format("Sau %ds nếu bạn không bắt đầu sẽ tự động rời khỏi bàn chơi", time_count));
	this->schedule(schedule_selector(LayerPopupReady::updateTimer), 1);
}

void LayerPopupReady::stopTimer()
{
	this->unschedule(schedule_selector(LayerPopupReady::updateTimer));
	if (!isIamReady)//chua ready thi out ban
	{
		this->onButtonLeave(NULL);
	}
	else{
		this->hideTimer();
	}
}

void LayerPopupReady::updateTimer(float dt)
{
	if (time_buff < 1)
	{
		this->stopTimer();
		return;
	}
	if (time_buff < 10)
		lblTimer->setString(StringUtils::format("00:0%d", time_buff));
	else
		lblTimer->setString(StringUtils::format("00:%d", time_buff));
	time_buff--;
}

void LayerPopupReady::onButtonReady(Ref* pSender)
{
	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerPopupReady::onButtonLeave(Ref* pSender)
{
	boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
	GameServer::getSingleton().Send(request);
}

void LayerPopupReady::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	//log("cmd = %s", cmd->c_str());
	if (0 == cmd->compare(EXT_EVENT_READY_NTF))
	{
		this->event_EXT_EVENT_READY_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_LIST_USER_UPDATE))
	{
		isIamReady = false;
	}
	// 	 	if (0 == cmd->compare(EXT_EVENT_GAME_LEAVE_RES))
	// 	 	{
	// 	 		this->event_EXT_EVENT_GAME_LEAVE_RES(param);
	// 	 	}
}

void LayerPopupReady::event_EXT_EVENT_READY_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> uidPtr = param->GetUtfString(EXT_FIELD_USERID);
	if (uidPtr != NULL)
	{
		if (0 == uidPtr->compare(myName))
		{
			isIamReady = true;
			btnReady->setEnabled(false);
			lblContent->setString(dataManager.GetSysString(269));
		}
		else{
			//neu khong phai minh - chua ready thi show 
			if (!isIamReady)
				lblContent->setString(StringUtils::format("Đối thủ đã sẵn sàng. Sau %ds nếu bạn không bắt đầu sẽ tự động rời khỏi bàn chơi", time_count));
		}
	}
}

void LayerPopupReady::showUIWithCheckBoss(string _listUser)
{
	isBossRoom = GameUtils::isBossRoom(_listUser);
	if (isBossRoom)
	{
		type_show = 0;
		lblContent->setString(dataManager.GetSysString(269));
	}
	else{
		type_show = 1;
		lblContent->setString(StringUtils::format("Sau %ds hệ thống sẽ tự động sẵn sàng", time_count));
	}
	this->showButtonByType();
}

void LayerPopupReady::showButtonByType()
{
	if (type_show == 0)// show only button ready
	{
		btnLeave->setVisible(false);
		btnReady->setPositionX(designResolutionSize.width / 2);
	}
	else{
		btnLeave->setVisible(true);
		btnReady->setPositionX(392.02);
	}
}

void LayerPopupReady::hideTimer()
{
	this->unschedule(schedule_selector(LayerPopupReady::updateTimer));
	this->setVisible(false);
}

void LayerPopupReady::onEnter()
{
	Layer::onEnter();
}

void LayerPopupReady::onExit()
{
	Layer::onExit();
}

void LayerPopupReady::showTimer()
{
	this->startTimer();
	this->setVisible(true);
	btnReady->setEnabled(true);
}


bool LayerPopupVisitor::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupVisitor.csb");
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	Layout* pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
		Button* btnReady = (Button*)pnlMain->getChildByName("btnReady");
		Button* btnLeave = (Button*)pnlMain->getChildByName("btnLeave");

		btnReady->addClickEventListener(CC_CALLBACK_1(LayerPopupVisitor::onButtonPlay, this));
		btnLeave->addClickEventListener(CC_CALLBACK_1(LayerPopupVisitor::onButtonSpec, this));
	}
	return true;
}

LayerPopupVisitor::LayerPopupVisitor()
{

}

LayerPopupVisitor::~LayerPopupVisitor()
{

}

void LayerPopupVisitor::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	//log("cmd = %s", cmd->c_str());
	if (0 == cmd->compare(EXT_EVENT_READY_NTF))
	{
	}
	else if (0 == cmd->compare(EXT_EVENT_LIST_USER_UPDATE))
	{
	}
}

void LayerPopupVisitor::onButtonPlay(Ref* pSender)
{
	boost::shared_ptr<IRequest> request(new SpectatorToPlayerRequest());
	GameServer::getSingleton().Send(request);
	this->setVisible(false);
}

void LayerPopupVisitor::onButtonSpec(Ref* pSender)
{
	this->setVisible(false);
}
