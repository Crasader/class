#include "LayerPopupInfo.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/Buddylist/AddBuddyRequest.h"
#include "Requests/Buddylist/RemoveBuddyRequest.h"
#include "AllData.h"
#include "mUtils.h"
#include "layergames/ChanUtils.h"
#include "layergames/PhomMessDef.h"
#include "../SceneManager.h"

LayerPopupInfo::LayerPopupInfo() : circleAvatar(NULL)
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupInfo::~LayerPopupInfo()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupInfo::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->gameID = SceneManager::getSingleton().getGameID();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	string pathCSB = "LayerPopupInfo.csb";
	if (this->gameID == kGameCoTuong || this->gameID == kGameCoUp)
		pathCSB = "LayerPopupInfoChess.csb";

	auto rootNode = CSLoader::getInstance()->createNode(pathCSB);
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	if (gameID == kGameChan || gameID == kGameXiTo){
		rootNode->setAnchorPoint(Point::ZERO);
		rootNode->setPosition(Point::ZERO);
	}
	else
	{
		rootNode->setAnchorPoint(Point(0.5, 0.5));
		rootNode->setPosition(Point(visibleSize / 2));
	}


	//}
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());

	auto btnClose = dynamic_cast<Button*>(rootNode->getChildByName("btnClose"));
	if (btnClose)
	{
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupInfo::onBtnClose, this));
	}

	btnKick = dynamic_cast<Button*>(rootNode->getChildByName("btnKickOut"));
	if (btnKick)
	{
		btnKick->addClickEventListener(CC_CALLBACK_1(LayerPopupInfo::onButtonKick, this));
	}

	btnAddFriend = dynamic_cast<Button*>(rootNode->getChildByName("btnAddFriend"));
	if (btnAddFriend)
	{
		btnAddFriend->addClickEventListener(CC_CALLBACK_1(LayerPopupInfo::onButtonInviteFriend, this));
	}
	btnUnfriend = dynamic_cast<Button*>(rootNode->getChildByName("btnUnFriend"));
	if (btnUnfriend)
	{
		btnUnfriend->addClickEventListener(CC_CALLBACK_1(LayerPopupInfo::onButtonUnfriend, this));
	}

	this->btnNapMoc = dynamic_cast<Button*>(rootNode->getChildByName("btnNapMoc"));
	if (this->btnNapMoc){
		this->btnNapMoc->addTouchEventListener(CC_CALLBACK_2(LayerPopupInfo::processButtonNapMocTouched, this));
	}

	this->btnNapHao = dynamic_cast<Button*>(rootNode->getChildByName("btnNapHao"));
	if (this->btnNapHao){
		this->btnNapHao->addTouchEventListener(CC_CALLBACK_2(LayerPopupInfo::processButtonNapHaoTOuched, this));
	}

	btnUnfriend->setVisible(false);
	btnAddFriend->setVisible(false);
	btnKick->setVisible(false);

	labelName = static_cast<Text*>(rootNode->getChildByName("lblName"));
	labelGender = static_cast<Text*>(rootNode->getChildByName("lblGioiTinh"));
	labelIP = static_cast<Text*>(rootNode->getChildByName("lblIP"));
	labelGender->setVisible(false);
	labelMoney = static_cast<Text*>(rootNode->getChildByName("lblMoney"));
	labelSilver = static_cast<Text*>(rootNode->getChildByName("lblSilver"));
	labelLevel = static_cast<Text*>(rootNode->getChildByName("lblLevel"));
	spriteNodeAvatar = static_cast<Sprite*>(rootNode->getChildByName("imgAvatar"));
	progressLevel = static_cast<LoadingBar*>(rootNode->getChildByName("ldbLevel"));
	imgVip = static_cast<ImageView*>(rootNode->getChildByName("imgAvatarVip"));
	imgVip->setVisible(false);

	if (this->spriteNodeAvatar != NULL){
		this->circleAvatar = CircleAvatar::create();
		this->circleAvatar->hideIconDefault(false);
		circleAvatar->displayTypeVip(false);
		float scX = this->spriteNodeAvatar->getScaleX();
		float scY = this->spriteNodeAvatar->getScaleY();

		Size _sizeImg = this->spriteNodeAvatar->getContentSize();
		Size _sizeAva = this->circleAvatar->GetSizeAvatar();

		scX = _sizeImg.width / _sizeAva.width * scX;
		scY = _sizeImg.height / _sizeAva.height * scY;

		this->circleAvatar->setAnchorPoint(Vec2::ZERO);
		this->circleAvatar->setScale(scX, scY);

		this->circleAvatar->setPosition(this->spriteNodeAvatar->getPosition());
		rootNode->addChild(this->circleAvatar, 5);
	}

    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

	return true;
}

void LayerPopupInfo::onBtnClose(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}
void LayerPopupInfo::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupInfo::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void LayerPopupInfo::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(listener);
	Layer::onExit();
}

void LayerPopupInfo::setFriendID(string friendID)
{
	mFriendID = friendID;
	_name = ChanUtils::GetNamePlayer(mFriendID);
	std::string _strNameSmall = "";
	(_name.length() > 22) ? _strNameSmall = _name.substr(0, 22) + "..." : _strNameSmall = _name;
	labelName->setString(_strNameSmall);

	boost::shared_ptr<User> user = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(mFriendID);
	if (user == NULL)
		return;

	boost::shared_ptr<UserVariable> money = user->GetVariable("amf");
	if (money != NULL) {
		labelMoney->setString(mUtils::convertMoneyEx(*money->GetDoubleValue()));
	}
	boost::shared_ptr<UserVariable> amfs = user->GetVariable("amfs");
	if (amfs != NULL) {
		labelSilver->setString(mUtils::convertMoneyEx(*amfs->GetDoubleValue()));
	}
    
    if (amfs != NULL) {
        labelSilver->setString(mUtils::convertMoneyEx(*amfs->GetDoubleValue()));
    }

	string myName = GameUtils::getMyName();
	if (0 != myName.compare(friendID))
	{
		this->btnNapMoc->setVisible(false);
		this->btnNapHao->setVisible(false);
	}
    reloadAllDatas();
    boost::shared_ptr<UserVariable> level_ptr = user->GetVariable("lvl");
    boost::shared_ptr<UserVariable> exp_ptr = user->GetVariable("exp");
    if (level_ptr)
        labelLevel->setString(StringUtils::format("Level %d", *level_ptr->GetIntValue()));
    if (exp_ptr)
        progressLevel->setPercent(dataManager.getLevelProgress(*level_ptr->GetIntValue(), *exp_ptr->GetIntValue()));

    boost::shared_ptr<UserVariable> accVipType = user->GetVariable("accVipType");
    if (accVipType != NULL) {
        int __vip = *accVipType->GetIntValue();
        this->circleAvatar->setTypeVip(__vip);
        
    }
}

void LayerPopupInfo::setMoney(double money)
{
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf)
	{
		string friendID = SceneManager::getSingleton().getMyName();
		mFriendID = friendID;
		_name = ChanUtils::GetNamePlayer(mFriendID);
		labelName->setString(_name);
		boost::shared_ptr<UserVariable> aip_ptr = mySelf->GetVariable("aIp");
		if (aip_ptr)
		{
			boost::shared_ptr<string> aip = aip_ptr->GetStringValue();
			if (aip)
			{
				string strIP = "IP: " + formatIP(aip->c_str());
				labelIP->setString(strIP);
			}
		}
		labelMoney->setString(mUtils::convertMoneyEx(money));
	}
}

void LayerPopupInfo::setAvatarImage(Sprite* sprite)
{
	if (sprite == NULL) return;
	spriteNodeAvatar->addChild(sprite);
}

void LayerPopupInfo::setAvatarUrl(string url)
{
	log("url = %s", url.c_str());
	if (this->circleAvatar != NULL){
		this->circleAvatar->setAvatarImage(url);
	}
}

void LayerPopupInfo::reloadAllDatas()
{
	if (mFriendID == "")
		return;
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString("aI", mFriendID.c_str());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("rgif", params));
	GameServer::getSingleton().Send(request);

}

void LayerPopupInfo::setIsBossRoom(bool isBoss)
{
	btnKick->setVisible(isBoss);
}

void LayerPopupInfo::onButtonInviteFriend(Ref* pSender)
{
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString("aI", mFriendID);
	boost::shared_ptr<IRequest> request(new ExtensionRequest("raaf", params));
	GameServer::getSingleton().Send(request);
	this->removeFromParentAndCleanup(true);
}

void LayerPopupInfo::onButtonUnfriend(Ref* pSender)
{
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString("aI", mFriendID.c_str());
	boost::shared_ptr<IRequest> request(new Buddylist::RemoveBuddyRequest(mFriendID));
	GameServer::getSingleton().Send(request);
	this->removeFromParentAndCleanup(true);
}

void LayerPopupInfo::onButtonKick(Ref* pSender)
{
	boost::shared_ptr<User> user = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(mFriendID);
	if (user == NULL)
		return;
	int roomID = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->c_str());
	//
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	if (roomID == atoi(SERVICE_TLMB.c_str()) || roomID == atoi(SERVICE_TLMN.c_str()) || roomID == atoi(SERVICE_XI_TO.c_str())){
		params->PutInt("id", user->Id());
		params->PutInt("uid", user->Id());
	}
	else{
		params->PutUtfString("uid", mFriendID.c_str());
		params->PutUtfString("id", mFriendID.c_str());
	}

	boost::shared_ptr<IRequest> request(new ExtensionRequest("kckrq", params, GameServer::getSingleton().getSmartFox()->LastJoinedRoom()));
	GameServer::getSingleton().Send(request);
	//
	this->removeFromParentAndCleanup(true);
}

void LayerPopupInfo::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (strcmp("hbc", cmd->c_str()) == 0) return;
	if (strcmp("rgif", cmd->c_str()) == 0){

		//   labelGender->setString(param->GetInt("aS") == 0 ? "Giới tính: Nam" : "Giới tính: Nữ");
		//	labelMoney->setString(mUtils::convertMoneyEx(*param->GetLong("amf")).c_str());
		//
		boost::shared_ptr<long> level_ptr = param->GetInt("lvl");
		boost::shared_ptr<long> exp_ptr = param->GetInt("exp");
		boost::shared_ptr<string> ai_ptr = param->GetUtfString("aI");
		if (level_ptr == NULL || exp_ptr == NULL || ai_ptr == NULL) return;

		labelLevel->setString(StringUtils::format("Level %d", *level_ptr));
		progressLevel->setPercent(dataManager.getLevelProgress(*param->GetInt("lvl"), *exp_ptr));
		boost::shared_ptr<User> user = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(*param->GetUtfString("aI"));
		if (user == NULL)
			return;

		boost::shared_ptr<UserVariable> accVipType = user->GetVariable("accVipType");
		if (accVipType != NULL) {
			int __vip = *accVipType->GetIntValue();
			this->circleAvatar->setTypeVip(__vip);

		}

		if (strcmp(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str(), ai_ptr->c_str()) == 0)
		{
			btnAddFriend->setVisible(false);
			btnUnfriend->setVisible(false);
			return;
		}
		//Check is friend
		// 		btnUnfriend->setVisible(GameServer::getSingleton().getSmartFox()->BuddyManager()->ContainsBuddy(mFriendID));
		// 		btnAddFriend->setVisible(!GameServer::getSingleton().getSmartFox()->BuddyManager()->ContainsBuddy(mFriendID));

	}
}

void LayerPopupInfo::setIsPlaying(bool isPlaying)
{
	isPlaying = isPlaying;
}

bool LayerPopupInfo::getIsPlaying()
{
	return isPlaying;
}

void LayerPopupInfo::hideBtnKick()
{
	btnKick->setVisible(false);
}

std::string LayerPopupInfo::formatIP(string _ip)
{
	vector<string> lstStr = mUtils::splitString(_ip, '.');
	string strResult;
	if (lstStr.size() > 3)
	{
		for (int i = 0; i < lstStr.size(); i++)
		{
			if (i < 2)
			{
				strResult += "xxx.";
			}
			else if (i == lstStr.size() - 1)
			{
				strResult += lstStr.at(i);
			}
			else{
				strResult += lstStr.at(i) + ".";
			}
		}
	}
	return strResult;
}

bool LayerPopupInfo::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void LayerPopupInfo::setUserIP(string ip)
{
	if (0 == ip.compare("") || 0 == ip.compare("undefined"))
		return;
	string strIP = "IP: " + formatIP(ip);
	labelIP->setString(strIP);
}

void LayerPopupInfo::processButtonNapMocTouched(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		LayerPopupCuaHang *popupNapMoc = LayerPopupCuaHang::create();
		popupNapMoc->onBtnNapThe(NULL);
		this->getParent()->addChild(popupNapMoc, 10000);
		this->removeFromParent();
	}
}

void LayerPopupInfo::processButtonNapHaoTOuched(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
		popup->onBtnNapHao(NULL);
		this->getParent()->addChild(popup, 10000);
		this->removeFromParent();
	}
}

PopUpInfoInChan::PopUpInfoInChan() : LayerPopupInfo(), _meIsBoss(false), _vip(0){

}

PopUpInfoInChan::~PopUpInfoInChan(){

}

void PopUpInfoInChan::setIsBossRoom(bool isBoss)
{
	this->_meIsBoss = isBoss;
}

void PopUpInfoInChan::setTypeVip(const int& vip)
{
	this->_vip = vip;
	this->circleAvatar->setTypeVip(this->_vip);
}

bool PopUpInfoInChan::init(){
	if (!LayerPopupInfo::init())
		return false;

	return true;
}

void PopUpInfoInChan::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (strcmp("hbc", cmd->c_str()) == 0)
		return;

	if (0 == strcmp("rgif", cmd->c_str())){
		/*moc3-change aN=>adn*/
		std::string _name = *param->GetUtfString("aN");
		boost::algorithm::trim(_name);
		if (0 == _name.compare("")) {
			_name = *param->GetUtfString("aI");
		}
		this->labelName->setString(GameUtils::getNamePlayer(_name));

		int aS = *param->GetInt("aS");
		this->labelGender->setString(0 == aS ? "Giới tính: Nam" : "Giới tính: Nữ");
		this->labelMoney->setString(mUtils::convertMoneyEx(*param->GetLong("amf")).c_str());
		this->labelLevel->setString(StringUtils::format("Level %ld", *param->GetInt("lvl")));

		if (strcmp(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str(), param->GetUtfString("aI")->c_str()) == 0)
		{
			this->btnAddFriend->setVisible(false);
			this->btnUnfriend->setVisible(false);
			this->btnKick->setVisible(false);
			return;
		}

		//Check is friend
		bool isFriend = GameServer::getSingleton().getSmartFox()->BuddyManager()->ContainsBuddy(mFriendID);
		if (this->_meIsBoss) {
			// 			this->btnKick->setVisible(true);
			// 			this->btnUnfriend->setVisible(isFriend);
			// 			this->btnAddFriend->setVisible(!isFriend);
		}
		else {

			this->btnKick->setVisible(false);

			// 			this->btnUnfriend->setPositionX(WIDTH_DESIGN >> 1);
			// 			this->btnAddFriend->setPositionX(WIDTH_DESIGN >> 1);
			// 
			// 			this->btnUnfriend->setVisible(isFriend);
			// 			this->btnAddFriend->setVisible(!isFriend);
		}
	}
}



