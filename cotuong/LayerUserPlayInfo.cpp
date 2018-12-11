#include "LayerUserPlayInfo.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "CommonChess.h"
#include "gameUtils.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/Buddylist/AddBuddyRequest.h"
#include "Requests/Buddylist/RemoveBuddyRequest.h"
#include "Common.h"
#include "layers/LayerPopupInfo.h"

USING_NS_CC;
#define tag_layer_number 251
// on "init" you need to initialize your instance
bool LayerUserPlayInfo::init()
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

	auto rootNode = CSLoader::getInstance()->createNode("LayerUserPlayInfo.csb");
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	// 	imgAvatar = (ImageView*)rootNode->getChildByName("imgAvatar");
	// 	imgKhungAvatar = (ImageView*)rootNode->getChildByName("imgKhungAvatar");
	// 	spVip = (Sprite*)rootNode->getChildByName("spVip");
	Node* nodeAvatar = (Node*)rootNode->getChildByName("nodeAvatar");
	btnGold = (Button*)rootNode->getChildByName("btnGold");
	this->spBoss = (Sprite*)rootNode->getChildByName("spBoss");
	this->lblUserName = (Text*)rootNode->getChildByName("lblUserName");
	this->lblUserMoney = (Text*)rootNode->getChildByName("lblUserMoney");
    this->lblUserMoney->setVisible(false);
	this->circleAvatar = CircleAvatar::create();
	this->circleAvatar->setAnchorPoint(nodeAvatar->getAnchorPoint());
	this->circleAvatar->setPosition(nodeAvatar->getPosition());
	this->circleAvatar->setClickCallBack(CC_CALLBACK_0(LayerUserPlayInfo::onButtonAvatar, this));
	this->circleAvatar->stopTimer();
	this->circleAvatar->setScaleClipping(0.9);

	this->spTime = (Sprite*)rootNode->getChildByName("spTime");
	this->spTimeTurn = (Sprite*)rootNode->getChildByName("spTimeTurn");
	
	rootNode->addChild(this->circleAvatar);
	this->spBoss->setLocalZOrder(10);

	int roomType = SceneManager::getSingleton().getCurrRoomType();
	string pathImg = "bt_tien3.png";
	if (roomType == 0)//gold
	{
		pathImg = "bt_tien4.png";
	}
	btnGold->loadTextureNormal(pathImg);

	btnGold->addClickEventListener(CC_CALLBACK_1(LayerUserPlayInfo::onButtonGold, this));

	this->resetInfo();

	return true;
}

LayerUserPlayInfo::LayerUserPlayInfo()
{
	GameServer::getSingleton().addListeners(this);
}

LayerUserPlayInfo::~LayerUserPlayInfo()
{
	GameServer::getSingleton().removeListeners(this);
}

void LayerUserPlayInfo::setInfoUser(string _name, string _money, bool isBoss, int vipLv, int khungAva, string urlAvatar)
{
	this->lblUserName->setString(_name);
	this->lblUserMoney->setString(_money);
	if (isBoss)
	{
		this->spBoss->setVisible(true);
	}
	else{
		this->spBoss->setVisible(false);
	}
	this->circleAvatar->setAvatarImage(urlAvatar);
	this->circleAvatar->setTypeVip(vipLv);
	// 	if (vipLv <= 0)
	// 	{
	// 		spVip->setVisible(false);
	// 	}
	// 	else{
	// 		spVip->setVisible(true);
	// 	}

	//load avatar and frame avatar here
}

void LayerUserPlayInfo::setInfoUser(string _name)
{
	this->myName = _name;
	string namePlayer = GameUtils::getNamePlayer(_name);
	double amfUser = 0;
	int typeRoom = SceneManager::getSingleton().getCurrRoomType();
	if (typeRoom == 0)
		amfUser = GameUtils::getAmfGoldUser(this->myName);
	else if (typeRoom == 1)
		amfUser = GameUtils::getAmfSilverUser(this->myName);
	int vipID = GameUtils::getTypeVipUser(_name);
	string urlAvatar = GameUtils::getAvatarUser(this->myName);

	this->lblUserName->setString(namePlayer);
	this->lblUserMoney->setString(mUtils::formatMoneySymbol(amfUser));
	this->circleAvatar->setTypeVip(vipID);
	this->circleAvatar->setAvatarImage(urlAvatar);
	log("name %s/ avatar = %s", namePlayer.c_str(), urlAvatar.c_str());
}

void LayerUserPlayInfo::updateInfo(string _money, bool isBoss)
{
	this->lblUserMoney->setString(_money);
	this->setBossRoom(isBoss);
}

void LayerUserPlayInfo::resetInfo()
{
	this->spBoss->setVisible(false);
	this->lblUserName->setString("---");
	this->lblUserMoney->setString("---");
	this->circleAvatar->setSizeIcon(Size(180, 180));
	this->circleAvatar->avatarFromLocal(DEFAULT_AVATAR);
	this->circleAvatar->setTypeVip(0);
	this->circleAvatar->stopTimer();
}

void LayerUserPlayInfo::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	if (ptrEventParams == NULL)
		return;
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	boost::shared_ptr<string> name_ptr = ptrNotifiedUser->Name();
	if (name_ptr == NULL) return;

	if (0 == name_ptr->compare(this->myName))
	{
		double amfUser = 0;
		int typeRoom = SceneManager::getSingleton().getCurrRoomType();
		if (typeRoom == 0)
			amfUser = GameUtils::getAmfGoldUser(this->myName);
		else if (typeRoom == 1)
			amfUser = GameUtils::getAmfSilverUser(this->myName);
		this->lblUserMoney->setString(formatMoneySymbol(amfUser));
	}
}

void LayerUserPlayInfo::startTimer(const float& duration_ /*= 15.0*/)
{
	//this->circleAvatar->startTimer(duration_);
	this->spTime->setVisible(false);
	this->spTimeTurn->setVisible(true);
}

void LayerUserPlayInfo::stopTimer()
{
	this->circleAvatar->stopTimer();
	this->spTime->setVisible(true);
	this->spTimeTurn->setVisible(false);
}

void LayerUserPlayInfo::setBossRoom(bool isBoss)
{
	this->spBoss->setVisible(isBoss);
}

void LayerUserPlayInfo::onButtonAvatar()
{
	int length = this->myName.length();
	if (length == 0) {
	}

	else {
		log("info of name= %s", this->myName.c_str());
		string userAvatar = GameUtils::getAvatarUser(this->myName);
		string userIp = GameUtils::getUserIP(this->myName);
		auto layerInfo = LayerPopupInfo::create();
		layerInfo->setFriendID(this->myName);
		layerInfo->setUserIP(userIp);
		layerInfo->setAvatarUrl(userAvatar);
		layerInfo->setIsBossRoom(false);//bo nut chu phong
		layerInfo->reloadAllDatas();
		layerInfo->setIsPlaying(GameUtils::isStartedGame());
		this->getParent()->addChild(layerInfo);

	}
}

void LayerUserPlayInfo::OnSmartFoxBuddyAdd(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	// Buddy was added
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueBuddy = (*ptrEventParams)["buddy"];
	boost::shared_ptr<Buddy> ptrNotifiedBuddy = ((boost::static_pointer_cast<Buddy>)(ptrEventParamValueBuddy));
	log(" buddy added");
}

void LayerUserPlayInfo::OnSmartFoxBuddyError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	log(" OnSmartFoxBuddyError");
}

string LayerUserPlayInfo::getMyName()
{
	return myName;
}

void LayerUserPlayInfo::showNumber(string numberString)
{
	long num = atol(numberString.c_str());
	int len = numberString.size();
	bool isA = num < 0;
	num = num < 0 ? -1 * num : num;
	numberString = mUtils::convertMoneyEx(num);
	if (isA)
		numberString = "-" + numberString;
	else
		numberString = "+" + numberString;
	std::replace(numberString.begin(), numberString.end(), ',', '.');

	int dem = 0;
	string font_name = "font_win-font.fnt";
	if (numberString.find('-') != std::string::npos) //contain string '-'
		font_name = "font_lose-font.fnt";
	Label* labelResult = Label::createWithBMFont(font_name, numberString, TextHAlignment::CENTER);
	if (numberString.size() >= 10)
		labelResult->setScale(0.8);
	labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(labelResult, tag_layer_number, tag_layer_number);
	labelResult->setPosition(Vec2(0, this->getContentSize().width / 4 + dem*labelResult->getContentSize().height / 2 + 10));
	labelResult->runAction(Sequence::create(
		MoveBy::create(1, Vec2(0, labelResult->getContentSize().height + 10)),
		DelayTime::create(2),
		FadeOut::create(0.2), RemoveSelf::create(true), nullptr));
}

void LayerUserPlayInfo::onButtonGold(Ref* pSender)
{
	SceneManager::getSingleton().showPopupMoneyCharge();
}

void LayerUserPlayInfo::reversePos()
{
	this->circleAvatar->reverseAvatar();
	Vec2 posMoney = this->lblUserMoney->getPosition();
	Vec2 posGold = this->btnGold->getPosition();
	this->lblUserMoney->setPosition(Vec2(posGold.x + 60, posGold.y));
	this->lblUserMoney->setAnchorPoint(Vec2(1, 0.5));
	this->lblUserMoney->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	this->btnGold->setPosition(posMoney);
}

void LayerUserPlayInfo::showAlertAnim(bool isShow)
{
	this->circleAvatar->showAlertAnim(isShow);
}
//update 25/7
void LayerUserPlayInfo::resumeTimer(int totalTime, int currTime)
{
	float currPecent = ((float)currTime / (float)totalTime) * 100;
	//this->circleAvatar->resumeTimer(currTime, currPecent);
}



bool LayerPopupUserInfo::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupUserInfo.csb");
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);


	Layout* pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
		Node* nodeAvatar = (Node*)pnlMain->getChildByName("nodeAvatar");
		Button* btnClose = (Button*)pnlMain->getChildByName("btnClose");
		this->btnFriend = (Button *)pnlMain->getChildByName("btnFriend");
		this->lblLevel = (Text*)pnlMain->getChildByName("lblLevel");
		this->lblUserName = (Text*)pnlMain->getChildByName("lblUserName");
		this->lblUserMoney = (Text*)pnlMain->getChildByName("lblUserMoney");
		this->lblElo = (Text*)pnlMain->getChildByName("lblElo");
		this->lblTiLe = (Text*)pnlMain->getChildByName("lblTiLe");

		this->circleAvatar = CircleAvatar::create();
		this->circleAvatar->setAnchorPoint(nodeAvatar->getAnchorPoint());
		this->circleAvatar->setPosition(nodeAvatar->getPosition());
		pnlMain->addChild(this->circleAvatar);
		btnFriend->setEnabled(false);

		this->btnFriend->addClickEventListener(CC_CALLBACK_1(LayerPopupUserInfo::onButtonFriend, this));
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupUserInfo::onButtonClose, this));
	}
	return true;
}

LayerPopupUserInfo::LayerPopupUserInfo()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupUserInfo::~LayerPopupUserInfo()
{
	GameServer::getSingleton().removeListeners(this);
}

void LayerPopupUserInfo::onButtonFriend(Ref* pSender)
{
	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	parameter->PutUtfString(EXT_FIELD_ACCOUNT_ID, this->userName);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(ADD_FRIEND_CMD, parameter));
	GameServer::getSingleton().Send(request);
}

void LayerPopupUserInfo::onButtonClose(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

void LayerPopupUserInfo::loadData(string _userName)
{
	string myName = GameUtils::getMyName();
	this->userName = _userName;

	int levelUser = GameUtils::getLevelUser(_userName);
	double amfUser = 0;
	int typeRoom = SceneManager::getSingleton().getCurrRoomType();
	if (typeRoom == 0)
		amfUser = GameUtils::getAmfGoldUser(_userName);
	else if (typeRoom == 1)
		amfUser = GameUtils::getAmfSilverUser(userName);
	string urlAvatar = GameUtils::getAvatarUser(_userName);

	lblUserName->setString(GameUtils::getNamePlayer(_userName));
	lblUserMoney->setString(mUtils::formatMoneySymbol(amfUser));
	lblLevel->setString(StringUtils::format("%d", levelUser));

	int vipType = GameUtils::getTypeVipUser(_userName);
	this->circleAvatar->setSizeIcon(Size(180, 180));
	circleAvatar->setAvatarImage(urlAvatar);
	circleAvatar->setTypeVip(vipType);
	int eloPoint = GameUtils::getEloUser(_userName);
	lblElo->setString(StringUtils::format("%d", eloPoint));

	if (myName.compare(_userName) == 0)
	{
		btnFriend->setEnabled(false);
	}
	else{
		bool isFriend = false;
		boost::shared_ptr<vector<boost::shared_ptr<Buddy> > > buddyList = GameServer::getSingleton().getSmartFox()->BuddyManager()->BuddyList();
		if (buddyList != NULL)
		{
			for (unsigned int i = 0; i < buddyList->size(); i++)
			{
				boost::shared_ptr<Buddy> buddyInfo = buddyList->at(i);
				if (buddyInfo != NULL);
				{
					string userName = buddyInfo->Name()->c_str();
					if (userName.compare(_userName) == 0)
					{
						isFriend = true;
						break;
					}
				}
			}
		}
		if (!isFriend)
		{
			btnFriend->setEnabled(true);
		}
	}
	this->requestHistory();
}

void LayerPopupUserInfo::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	if (0 == cmd->compare(ADD_FRIEND_CMD)){ //History response
		this->btnFriend->setEnabled(false);
	}
	else if (strcmp("rghba", cmd->c_str()) == 0){ //History response
		if (param->GetUtfString("rghba") == NULL)
			return;
		log("History response: %s", param->GetUtfString("rghba")->c_str());
		vector<string> lstHis;
		lstHis = mUtils::splitString(*param->GetUtfString("rghba"), '|');
		//

		double totalMatch = 0;
		double totalWin = 0;
		for (int j = 0; j < lstHis.size(); j++){
			vector<string> his = mUtils::splitString(lstHis.at(j), ',');
			if (his.size() >= 4)
			{
				int _win = atoi(his.at(2).c_str());
				int _lose = atoi(his.at(3).c_str());
				totalMatch += (_win + _lose);
				totalWin += _win;
			}
		}
		float winRate = 0;
		if (totalMatch > 0)
			winRate = (float)((totalWin / totalMatch) * 100);
		lblTiLe->setString(StringUtils::format("%.2f %%", winRate));
	}
}
void LayerPopupUserInfo::requestHistory()
{
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString("aI", this->userName);

	//Get histories - Send request
	boost::shared_ptr<IRequest> request1(new ExtensionRequest("rghba", params));
	GameServer::getSingleton().Send(request1);
}
