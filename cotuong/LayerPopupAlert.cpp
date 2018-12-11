#include "LayerPopupAlert.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "GameServer.h"
#include "SceneManager.h"
#include "boost/smart_ptr/make_shared_object.hpp"
#include "Requests/JoinRoomRequest.h"
#include "mUtils.h"
#include "ServerMessageDef.h"
#include "gameUtils.h"
USING_NS_CC;

// on "init" you need to initialize your instance
bool LayerPopupAlert::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//    auto rootNode = CSLoader::createNode("LayerChonGame.csb");
	//    this->addChild(rootNode);
	this->setTagCallBack(0);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupAlert.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	Layout* Panel_1 = (Layout*)rootNode->getChildByName("pnlBg");
	if (Panel_1)
	{
		Panel_1->setScale(visibleSize.width / Panel_1->getContentSize().width, visibleSize.height / Panel_1->getContentSize().height);
	}
	Layout* pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
		btnConfirm = (Button*)pnlMain->getChildByName("btnConfirm");
		btnClose = (Button*)pnlMain->getChildByName("btnClose");
		lblContent = (Text*)pnlMain->getChildByName("lblContent");
		lblTitle = (Text*)pnlMain->getChildByName("lblTitle");
		lblTimer = (Text*)pnlMain->getChildByName("lblTimer");

		lblTimer->setString(StringUtils::format("%d", timeBuff));
		lblTimer->setVisible(false);
		btnConfirm->addClickEventListener(CC_CALLBACK_1(LayerPopupAlert::onButtonConfirm, this));
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupAlert::onButtonClose, this));
	}
	return true;
}

LayerPopupAlert::LayerPopupAlert()
{
	_callback = nullptr;
	timeBuff = DEFAULT_TIME_RESPONSE;
}

LayerPopupAlert::~LayerPopupAlert()
{
	this->unschedule(schedule_selector(LayerPopupAlert::startTimer));
}

void LayerPopupAlert::onButtonConfirm(Ref* pSender)
{
	// 	if (isOnlyButton && !lbmTimer->isVisible())
	// 	{
	// 		this->removeFromParentAndCleanup(true);
	// 		return;
	// 	}

	if (_callback != nullptr)
	{
		this->runAction(Sequence::create(CallFunc::create([=]{
			_callback(this, this->getTagCallBack());
		}), RemoveSelf::create(), NULL));

	}
	else{
		this->removeFromParentAndCleanup(true);
	}
	// 	if (listener != NULL)
	// 	{
	// 		//this->runAction(Sequence::create(CallFunc::create(listener(this->getTag())), RemoveSelf::create(true)));
	// 	}
}

void LayerPopupAlert::onButtonClose(Ref* pSender)
{
	//update 14/7
	if (_callbackClose != nullptr)
	{
		this->runAction(Sequence::create(CallFunc::create([=]{
			_callbackClose(this, this->getTagCallBack());
		}), RemoveSelf::create(), NULL));

	}
	else{
		this->removeFromParentAndCleanup(true);
	}
}

static std::string ReplaceString(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

void LayerPopupAlert::setContentMessage(std::string _title, std::string _mess)
{
	string _newMsg = ReplaceString(_mess, "@", "\n");

	lblTitle->setString(_title);
	lblContent->setString(_newMsg);
}

// void LayerPopupAlert::setCallBackConfirm(std::function<void(int)> _listener)
// {
// 	this->listener = _listener;
// }

void LayerPopupAlert::setCallBackConfirm(const popupAlertCallback& callback)
{
	_callback = callback;
}

void LayerPopupAlert::startTimer(float dt)
{
	if (this->timeBuff <= 0)
	{
		this->unschedule(schedule_selector(LayerPopupAlert::startTimer));
		this->removeFromParentAndCleanup(true);
		return;
	}
	this->timeBuff--;
	this->lblTimer->setString(StringUtils::format("%d", timeBuff));
}

void LayerPopupAlert::runTimer(const float& timeOut)
{
	this->timeBuff = timeOut;
	this->lblTimer->setVisible(true);
	this->schedule(schedule_selector(LayerPopupAlert::startTimer), 1);
}

void LayerPopupAlert::onEnter()
{
	Layer::onEnter();
}

void LayerPopupAlert::onExit()
{
	Layer::onExit();
}

void LayerPopupAlert::showOnlyButtonAccept()
{
	btnClose->setVisible(false);
	this->lblTimer->setVisible(false);

	btnConfirm->setPositionX(950);
	isOnlyButton = true;
}

void LayerPopupAlert::setTitleCloseButton(string title)
{
	this->btnClose->setTitleText(title);
}

void LayerPopupAlert::setTitleConfirmButton(string title)
{
	this->btnConfirm->setTitleText(title);
}

void LayerPopupAlert::setCallBackClose(const popupAlertCallback& callback)
{
	this->_callbackClose = callback;
}




LayerPopupRoomFind::LayerPopupRoomFind()
{

}

LayerPopupRoomFind::~LayerPopupRoomFind()
{

}

bool LayerPopupRoomFind::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupRoomFind.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	Layout* Panel_1 = (Layout*)rootNode->getChildByName("pnlBg");
	if (Panel_1)
	{
		Panel_1->setScale(visibleSize.width / Panel_1->getContentSize().width, visibleSize.height / Panel_1->getContentSize().height);
	}
	Layout* pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
		TextField_1 = (TextField*)pnlMain->getChildByName("TextField_1");
		Button* btnConfirm = (Button*)pnlMain->getChildByName("btnConfirm");
		Button* btnClose = (Button*)pnlMain->getChildByName("btnClose");

		btnConfirm->addClickEventListener(CC_CALLBACK_1(LayerPopupRoomFind::onButtonConfirm, this));
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupRoomFind::onButtonClose, this));
	}
	return true;
}

void LayerPopupRoomFind::onButtonConfirm(Ref* pSender)
{
	if (this->TextField_1->getString().length() == 0)
		return;
	int idRoom = atoi(this->TextField_1->getString().c_str());
	int gameID = SceneManager::getSingleton().getGameID();
	string strGameID = StringUtils::format("%d", gameID).substr(0, 3);
	string sRoomName = StringUtils::format("P%s_%d", strGameID.c_str(), idRoom);
	boost::shared_ptr<Room>  roomPtr = GameServer::getSingleton().getSmartFox()->GetRoomByName(sRoomName);
	if (roomPtr)
	{
		if (roomPtr->IsPasswordProtected())
		{
			SceneManager::getSingleton().showToast("Phòng yêu cầu mật khẩu");
		}
		else{
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
			if (currPlayers == 1)
			{
				int roomBetVal = 0;
				bool allowView = false;
				if (lstParams.size() >= 13)
				{
					roomBetVal = atoi(lstParams.at(0).c_str());
				}

				int roomType = 1;
				boost::shared_ptr<RoomVariable> typeRoomVar = roomPtr->GetVariable(EXT_FIELD_ROOM_TYPE);
				if (typeRoomVar != NULL)
				{
					roomType = *(typeRoomVar->GetIntValue());
				}
				double amfUser = 0;
				if (roomType == 0)
				{
					amfUser = GameUtils::getAmfGoldUser();
				}
				else if (roomType == 1)
				{
					amfUser = GameUtils::getAmfSilverUser(GameUtils::getMyName());
				}
				// get idroom to left

				double betUser = GameUtils::getMinBetByGame(SceneManager::getSingleton().getGameID(), amfUser);

				if (betUser < roomBetVal)
				{
					SceneManager::getSingleton().showToast("Không đủ tiền vào bàn");
					SceneManager::getSingleton().showPopupMoneyCharge();
					return;
				}
			}

			boost::shared_ptr<IRequest> request(new JoinRoomRequest(realRoomID, "", id2Left, isSpectator));
			GameServer::getSingleton().Send(request);
		}
	}
	else{
		SceneManager::getSingleton().showToast("Không tìm thấy phòng");
	}
}

void LayerPopupRoomFind::onButtonClose(Ref* pSender)
{
	this->removeFromParent();
}
