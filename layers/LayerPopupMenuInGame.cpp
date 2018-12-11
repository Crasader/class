#include "Requests/LeaveRoomRequest.h"
#include "Requests/SetRoomVariablesRequest.h"
#include "LayerPopupMenuInGame.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "Requests/PublicMessageRequest.h"
#include "GameServer.h"
#include "LayerGuideInGame.h"
#include "layergames/_Button_inGame_.h"
#include "layergames/PhomMessDef.h"
#include "LayerConfigInGame.h"
#include "layergames/AvatarInGame52La.h"
#include "layergames/ChanUtils.h"
#include "LayerBangXepHang.h"
using namespace Sfs2X;

LayerPopupMenuInGame::LayerPopupMenuInGame()
{
	isLeave = false;
	isBaoSam = false;
	btnRoiBan = NULL;
}

LayerPopupMenuInGame::~LayerPopupMenuInGame()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupMenuInGame::init()
{
	if (!Layer::init())
		return false;
	//LayerPopupMenuInGame.json
	isChauRia = false;
	Node* rootNode = CSLoader::getInstance()->createNode("LayerPopupMenuInGame.csb");
	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());

	int widButton2 = 123;
	int heiButton = 123;
	int heipopup = 440;
	int dis = 20;
	Size winSize = Director::getInstance()->getVisibleSize();
	Size sizeAdd = ChanUtils::getSizePos();
	int gameID = SceneManager::getSingleton().getGameID();
	if (gameID == kGameChan || gameID == kGameXiTo)
	{
		rootNode->setAnchorPoint(Vec2(0, 0));
		this->setPosition(Vec2(dis, winSize.height - heiButton - heipopup - dis * 2 - sizeAdd.height));
	}
	else{
		this->setPosition(Vec2(dis, winSize.height - heiButton - heipopup));
	}


	//btnRoiBan = (Button*)rootNode->getChildByName("btnRoiBan");
	Button* btnRoiBanNhanh = (Button*)rootNode->getChildByName("btnRoiBanNhanh");
	Button* btnHuongDan = (Button*)rootNode->getChildByName("btnHuongDan");
	Button* btnCaiDat = (Button*)rootNode->getChildByName("btnCaiDat");

	/*btnRoiBan->setPressedActionEnabled(true);
	btnRoiBan->setScale9Enabled(false);*/
	btnRoiBanNhanh->setPressedActionEnabled(true);
	btnHuongDan->setPressedActionEnabled(true);
	btnCaiDat->setPressedActionEnabled(true);

	//btnRoiBan->addTouchEventListener(CC_CALLBACK_2(LayerPopupMenuInGame::onButtonRoiBan, this));
	btnRoiBanNhanh->addTouchEventListener(CC_CALLBACK_2(LayerPopupMenuInGame::onButtonRoiBanNhanh, this));
	btnHuongDan->addTouchEventListener(CC_CALLBACK_2(LayerPopupMenuInGame::onButtonHuongDan, this));
	btnCaiDat->addTouchEventListener(CC_CALLBACK_2(LayerPopupMenuInGame::onButtonCaiDat, this));

	GameServer::getSingleton().addListeners(this);
	return true;
}

void LayerPopupMenuInGame::onButtonRoiBan(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//int gameID = SceneManager::getSingleton().getGameID();
		//if ((gameID == kGamePhom || gameID == kGameChan) && isChauRia)//bay luon neu la chau ria
		//{
		//	boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
		//	GameServer::getSingleton().Send(request);
		//	return;
		//}
		//int _status = 1;
		//string _mess;
		//if (isLeave)
		//	_status = 0;
		//else
		//	_status = 1;

		//boost::shared_ptr<ISFSObject> parameter(new SFSObject());
		//parameter->PutInt(EXT_FIELD_LEAVE_STATUS, _status);
		//boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		//if (lastRoom != NULL)
		//{
		//	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_LEAVE_GAME_REQ, parameter, lastRoom));
		//	GameServer::getSingleton().Send(request);
		//	this->hideMenu();
		//}
	}
}

void LayerPopupMenuInGame::onButtonRoiBanNhanh(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		/*LayerButtonInGame * parent = static_cast<LayerButtonInGame*>(this->getParent());
		if (parent)
		{
		if (isBaoSam)
		{
		parent->showToast(dataManager.GetSysString(254));
		return;
		}
		parent->eventTouchBtnBack(NULL, ui::Widget::TouchEventType::ENDED);
		}*/
        auto _currScene = Director::getInstance()->getRunningScene();
        auto layer = LayerBangXepHangTo::create();
        layer->loadTop(SceneManager::getSingleton().getGameID(),SceneManager::getSingleton().getCurrRoomType());
        _currScene->addChild(layer,2000,9048);
	}
}

void LayerPopupMenuInGame::onButtonHuongDan(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
        auto _currScene = Director::getInstance()->getRunningScene();

		auto layer = LayerGuideInGame::create();
		_currScene->addChild(layer,2000,9049);
	}
}

void LayerPopupMenuInGame::onButtonCaiDat(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto layer = LayerConfigInGame::create();
		int gameID = SceneManager::getSingleton().getGameID();
		if (gameID == kGameXiTo)
		{
			layer->setPosition(Vec2(-mUtils::getSizePos().width, -mUtils::getSizePos().height));
		}
		this->getParent()->addChild(layer);
	}
}

void LayerPopupMenuInGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	log("============== CMD = %s", cmd->c_str());
	/*if (cmd->compare("lgrs") == 0)
	{
	this->event_EXT_EVENT_LEAVE_GAME_RES(param);
	}*/
}

void LayerPopupMenuInGame::showMenu()
{
	this->setVisible(true);
	listener->setSwallowTouches(true);
}

void LayerPopupMenuInGame::hideMenu()
{
	this->setVisible(false);
	listener->setSwallowTouches(false);
}

void LayerPopupMenuInGame::event_EXT_EVENT_LEAVE_GAME_RES(boost::shared_ptr<ISFSObject> ptrEvent)
{
	boost::shared_ptr<long> rescode = ptrEvent->GetInt(EXT_FIELD_RESULT);
	if (rescode != NULL)
	{
		if (*rescode != 0) return;
		isLeave = !isLeave;
		LayerButtonInGame *parent = static_cast<LayerButtonInGame*> (this->getParent());
		if (!parent) return;
		int gameID = SceneManager::getSingleton().getGameID();
		if (isLeave)
		{
			parent->showToast(dataManager.GetSysString(135));
			btnRoiBan->loadTextures("ingame-menu-play.png", "ingame-menu-play.png", "");
			btnRoiBan->setContentSize(Size(328, 94));
		}
		else
		{
			parent->showToast(dataManager.GetSysString(136));
			btnRoiBan->loadTextures("ingame-menu-quit.png", "ingame-menu-quit.png", "");
		}
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
}

void LayerPopupMenuInGame::setBaoSam(bool _isbao)
{
	isBaoSam = _isbao;
}

bool LayerPopupMenuInGame::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void LayerPopupMenuInGame::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto locationInNode = this->convertToNodeSpace(touch->getLocation());
	auto s = this->getContentSize();
	auto rect = Rect(0, 0, s.width, s.height);
	if (!rect.containsPoint(locationInNode))
	{
		this->hideMenu();
	}
}

void LayerPopupMenuInGame::onEnter()
{
	Layer::onEnter();
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupMenuInGame::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(LayerPopupMenuInGame::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LayerPopupMenuInGame::onExit()
{
	Layer::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

void LayerPopupMenuInGame::ResetStatus(){
	this->isLeave = false;
	this->btnRoiBan->loadTextures("ingame-menu-quit.png", "ingame-menu-quit.png", "");
}

void LayerPopupMenuInGame::setChauRia(bool _isChauRia)
{
	isChauRia = _isChauRia;
}

bool LayerPopupMenuInGame::getBaoSam()
{
	return this->isBaoSam;
}

bool LayerPopupMenuInGame::getLeave()
{
	return this->isLeave;
}

bool LayerPopupMenuInGame::getChauRia()
{
	return this->isChauRia;
}
