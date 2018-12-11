
#include "mUtils.h"

#include "_Avatar_.h"
#include "SceneManager.h"
#include "LayerMoiChoi.h"
#include "LayerPlayerInfo.h"
#include "NativeHelper.h"
#include "../Common.h"
#include "../layers/LayerPopupInfo.h"
#include "cocos2d.h"
#include "../layers/LayerMoiChoiInGame.h"
#include "PhomMessDef.h"
USING_NS_CC;

mUtils mu;

#define font_size_avatar 35
#define TAG_IMG_AVATAR_CROP 193

Avatar::Avatar(bool isMe)
:timer(NULL),
layerWidget(NULL),
layerInvite(NULL),
bg(NULL),
icon(NULL),
flag(NULL),
name(NULL),
money(NULL),
aI(""),
urlAvatar(""),
isMe(isMe),
isCanTouch(true),
meIsBoss(false),
flagIsMe(NULL),
n_Level(1),
p_Level(0),
mCallBack(NULL),
mCallBackListener(NULL),
icc(NULL),
iconLeaveGame(NULL),
_scaleTextName(1),
taolabot(false),
forGuess(false),
circleAvatar(NULL),
isPlayerMe(false),
_vipTypePlayer(0),
lblWaiting(NULL),
sIp("")
{

}

Avatar::~Avatar() {
	this->stopTimer();
	GameServer::getSingleton().removeListeners(this);
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);
}

Avatar* Avatar::create(bool isMe) {
	Avatar* avatar = new Avatar(isMe);
	if (avatar && avatar->init()) {
		avatar->autorelease();
		return avatar;
	}
	else {
		delete avatar;
		avatar = NULL;
		return NULL;
	}
}

bool Avatar::init() {
	if (!Layer::init())
		return false;

	this->mCurrentExp = -1;

	this->setAnchorPoint(Vec2(0, 0));
	this->sizeThis = Size(143, 143);
	this->sizeIcon = Size(143, 143);

	this->layerWidget = Layer::create();
	this->layerWidget->setAnchorPoint(Vec2(0.5, 0.5));

	this->bg = ImageView::create();
	this->bg->loadTexture("icon_transparent_default.png");
	this->bg->setTouchEnabled(true);
	this->bg->addTouchEventListener(CC_CALLBACK_2(Avatar::OnAvatarTouches, this));

	this->sizeThis = this->bg->getContentSize();
	this->sizeIcon = this->bg->getContentSize();

	Sprite* frame_timerLoader = Sprite::create("frame_time.png");
	this->timer = ProgressTimer::create(frame_timerLoader);
	this->timer->setType(ProgressTimer::Type::RADIAL);
	this->timer->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(this->timer);

	this->flag = ImageView::create();
	this->flag->loadTexture("flag.png");
	this->flag->setPosition(Vec2(this->sizeThis.width / 2, this->sizeThis.height / 2));
	this->flag->setVisible(false);

	this->name = Label::createWithSystemFont(dataManager.GetSysString(569),"", 25);//Label::createWithBMFont(RobotoFnt, dataManager.GetSysString(569));
	this->name->setColor(_colorTextNormal);
	this->name->setScale(this->_scaleTextName);
	this->name->setAnchorPoint(Vec2(0.5, 0));
	this->name->setPositionY(-this->sizeThis.height + 35);

	this->lblWaiting = Label::createWithSystemFont(dataManager.GetSysString(569),"", 25);
	this->lblWaiting->setColor(_colorTextNormal);
	this->lblWaiting->setScale(this->_scaleTextName);
	this->lblWaiting->setAnchorPoint(Vec2(0.5, 0));
	this->lblWaiting->setPositionY(this->sizeThis.height / 2 + 3);
	this->lblWaiting->setVisible(false);

	this->money = Label::createWithSystemFont("","", 25);
	this->money->setColor(_colorTextNormal);
	this->money->setScale(this->_scaleTextName);
	this->money->setAnchorPoint(Vec2(0.5, 0));
	this->money->setPosition(Vec2(18, -this->sizeThis.height));

	Size s = this->money->getContentSize();
	this->icc = Button::create("moc3-icon-moc.png", "moc3-icon-moc.png", "moc3-icon-moc.png");
	this->icc->setAnchorPoint(Vec2(0.5, 0));
	this->icc->setPosition(Point(this->money->getPositionX() - s.width * this->_scaleTextName, this->money->getPositionY() - 20));

	this->icc->addTouchEventListener(CC_CALLBACK_2(Avatar::processButtonRechardTouched, this));
	int roomType = SceneManager::getSingleton().getCurrRoomType();
	this->icc->setScale(0.75);
	//this->icc->setEnabled(false);
	//this->icc->setBright(false);
	if (roomType == 0){
		icc->loadTextures("moc3-icon-moc.png", "moc3-icon-moc.png", "moc3-icon-moc.png");
	}
	else{
		icc->loadTextures("moc3-icon-hao.png", "moc3-icon-hao.png", "moc3-icon-hao.png");

	}

	this->iconLeaveGame = Sprite::create("icon-out.png");
	this->iconLeaveGame->setAnchorPoint(Vec2(1, 1));
	this->iconLeaveGame->setPosition(Vec2(this->sizeThis.width / 2 + 23, this->sizeThis.height / 2 + 23));
	this->iconLeaveGame->setLocalZOrder(1000);
	this->iconLeaveGame->setVisible(false);
	this->layerWidget->addChild(this->iconLeaveGame);

	/*Avatar tron. */
	this->circleAvatar = CircleAvatar::create();
	this->circleAvatar->hideIconDefault(false);
	this->circleAvatar->setAvatarImage("");

	this->layerWidget->addChild(this->bg);
	this->layerWidget->addChild(this->flag);
	this->layerWidget->addChild(this->name);
	this->layerWidget->addChild(this->money);
	this->layerWidget->addChild(this->icc);
	this->layerWidget->addChild(this->circleAvatar, LOCAL_ZORDER_CIR_AVATAR);
	this->layerWidget->addChild(this->lblWaiting);

	//Sprite Same Ip
	this->spSameIP = Sprite::create("lost_connection.png");
	this->spSameIP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->spSameIP->setPosition(Vec2(-W_AVATAR / 4, W_AVATAR / 3));
	this->spSameIP->setVisible(false);
	//this->layerWidget->addChild(this->spSameIP, LOCAL_ZORDER_SP_SAME_IP);

	this->layerInvite = Layer::create();
	this->layerInvite->setAnchorPoint(Vec2(0.5, 0.5));

	ImageView* invite = ImageView::create();
	invite->loadTexture("moc3-user_invite.png");
	invite->setTouchEnabled(true);
	invite->addTouchEventListener(CC_CALLBACK_2(Avatar::OnAvatarTouches, this));
	this->layerInvite->addChild(invite);

	this->layerWidget->setVisible(false);
	this->addChild(this->layerWidget);
	this->addChild(this->layerInvite);
	this->circleAvatar->setClickCallBack(CC_CALLBACK_0(Avatar::onAvatarCallback, this));


	GameServer::getSingleton().addListeners(this);

	return true;
}

Size Avatar::getSizeThis() {
	return sizeThis;
}

void Avatar::OnAvatarTouches(Ref *pObject, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {
		if (isMe)
			return;

		int length = this->aI.length();
		if (length == 0) {
			if (isStartedGame())
				return;
			log("Invite User!!!!!!!!!!!!!");
			if (SceneManager::getSingletonPtr() == NULL)
				return;
			log("Here 1");
			// auto layerMoiChoi = LayerMoiChoiInGame::create();
			// this->getParent()->getParent()->addChild(layerMoiChoi, 10000, WIDTH_DESIGN);
		}

		else {
			log("info of name= %s", this->aI.c_str());
			auto layerInfo = PopUpInfoInChan::create();
			layerInfo->setUserIP(getIp());
			layerInfo->setIsBossRoom(this->taolabot);
			layerInfo->setFriendID(this->aI);
			layerInfo->setAvatarUrl(this->urlAvatar);
			layerInfo->setTypeVip(this->_vipTypePlayer);
			layerInfo->reloadAllDatas();
			layerInfo->setIsPlaying(isStartedGame());
			this->getParent()->getParent()->addChild(layerInfo, 10000, WIDTH_DESIGN);
			//
		}
	}
}

void Avatar::setVisibleLayerInvite(bool isShow) {
	if (isShow)
		showLayerInvite();
	else
		hideLayerInvite();
}

void Avatar::hideLayerInvite() {
	if (!isMe)
		layerInvite->setVisible(false);
	layerWidget->setVisible(true);
}

void Avatar::showLayerInvite() {
	if (!isMe)
		layerInvite->setVisible(true);
	layerWidget->setVisible(false);
}

void Avatar::setIcon(string url) {
	this->urlAvatar = url;
	this->circleAvatar->setAvatarImage(url, 0);
}

void Avatar::setFlag(bool isShow) {
	if (flag != NULL) {
		//flag->setVisible(isShow);
	}
}

void Avatar::setName(string name) {
	if (name == "" && this->icc != NULL) {
		//this->icc->setVisible(false);
	}

	if (this->name != NULL) {
		this->name->setString(name);
		//        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		//            this->name->setColor(_colorTextWHITE);
		//        }
	}
}

void Avatar::setMoney(string pMoney) {

	if (pMoney == "" && this->icc != NULL) {
		//this->icc->setVisible(false);
	}

	if (this->money != NULL) {
		this->money->setString(pMoney);
		if (this->icc != NULL){
			Size s = this->money->getContentSize();
			if (s.width == 0) {
				//this->icc->setVisible(false);
			}
			else {
				if (!this->isPlayerMe)
				{
					float t = s.width / 2 * this->_scaleTextName;
					this->icc->setPosition(Point(this->money->getPositionX() - t - 20, this->money->getPositionY() + 3));
					//this->icc->setVisible(true);
				}
			}
		}
	}
}

void Avatar::setMoney(const long long& money) {
	this->setMoney(convertMoney(money));
}

void Avatar::setMoney(int money) {
	this->setMoney(convertMoney(money));
}

void Avatar::setMoney(double money) {
   // log("money %d",money);
	this->setMoney(formatMoneySymbol(money));
}

void Avatar::setReady(bool isReady) {
	if (this->bg != NULL) {
		if (isReady) {
			if (this->isMe)
				this->bg->loadTexture("frame_me_info_ready.png");
			else this->bg->loadTexture("frame_user_ready.png");
		}
		else {
			if (this->isMe)
				this->bg->loadTexture("frame_me_info.png");
			else this->bg->loadTexture("frame_user.png");
		}
	}
}

void Avatar::setAI(string ai, bool forguess) {

	this->aI = ai;
	this->forGuess = forguess;

	boost::shared_ptr<User> user = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(ai);
	if (user == NULL)
		return;

	boost::shared_ptr<UserVariable> lvl = user->GetVariable("lvl");
	if (lvl != NULL) {
		this->SetLevel(*lvl->GetStringValue());
	}

	boost::shared_ptr<UserVariable> exp = user->GetVariable("exp");
	if (exp != NULL) {
		this->mCurrentExp = *exp->GetIntValue();
	}

	boost::shared_ptr<UserVariable> accVipType = user->GetVariable("accVipType");
	if (accVipType != NULL) {
		int __vip = *accVipType->GetIntValue();
		this->circleAvatar->setTypeVip(__vip);
		this->_vipTypePlayer = __vip;
	}

	boost::shared_ptr<UserVariable> aIp = user->GetVariable("aIp");
	if (aIp != NULL){
		this->sIp = *aIp->GetStringValue();
	}


	this->icc->setVisible(true);
	string myName = GameUtils::getMyName();
	string pathImg = "moc3-icon-moc.png";
	if (0 == myName.compare(ai))
	{
		pathImg = "daucong.png";
	}
	else{
		int typeRoom = SceneManager::getSingleton().getCurrRoomType();
		if (typeRoom == 1)
			pathImg = "moc3-icon-hao.png";
	}
	this->icc->loadTextures(pathImg, "", "");
}

const std::string& Avatar::getAI() const{
	return this->aI;
}

const std::string& Avatar::getIp() const{
	return this->sIp;
}

void Avatar::startTimer(const float& duration_) {
	// hideLayerInvite();
	stopTimer();

	ProgressTo *actionTimer = ProgressTo::create(duration_, DELAY_TIME);
	timer->runAction(Sequence::create(actionTimer,
		CallFuncN::create(CC_CALLBACK_1(Avatar::callbackTimer, this)),
		NULL));
	//
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself != NULL && 0 == this->aI.compare(*myself->Name())) {
		Director::getInstance()->getScheduler()->schedule(schedule_selector(Avatar::runAnimationWarning),this,duration_ *2/3, false);
	}
}

void Avatar::callbackTimer(Node *pSender) {
	stopTimer();
}

void Avatar::stopTimer() {
	this->stopAllActions();
	//
	if (timer == NULL) {
		return;
	}
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(Avatar::runAnimationWarning),this);

	if (timer->getNumberOfRunningActions() > 0) {
		timer->stopAllActions();
	}
	timer->setPercentage(0);
	if (mCallBackListener && mCallBack) {
		(mCallBack->*mCallBackListener)();
	}

}

string Avatar::convertMoney(int money) {
	//ostringstream oss;
	//oss<<money;
	//return oss.str();

	ostringstream oss;
	///
	if (money < 1000) {
		oss.clear();
		oss << money;
		return (oss.str() + "");
	}
	else if (money >= 1000 && money <= 999999) {
		string hangTram;
		string hangNghin;

		ostringstream oss1;
		// Nghin
		oss1.clear();
		oss1 << (money / 1000);
		hangNghin = oss1.str();

		ostringstream oss2;
		// tram
		int hTram = (money % 1000);
		oss2.clear();
		oss2 << hTram;
		if (hTram < 10) {
			hangTram = "00";
		}
		else if (hTram >= 10 && hTram < 100) {
			hangTram = "0";
		}
		hangTram += oss2.str();

		return (hangNghin + "," + hangTram + "");
	}
	else if (money >= 1000000) {
		string hangTrieu;
		string hangNghin;
		string hangTram;

		ostringstream oss1;
		// Trieu
		oss1.clear();
		oss1 << (money / 1000000);
		hangTrieu = oss1.str();

		// Nghin
		int hNghin = (money - (money / 1000000) * 1000000) / 1000;
		ostringstream oss2;
		oss2.clear();
		oss2 << hNghin;

		if (hNghin < 10) {
			hangNghin = "00";
		}
		else if (hNghin >= 10 && hNghin < 100) {
			hangNghin = "0";
		}
		hangNghin += oss2.str();

		// Tram
		int hTram = (money % 1000);
		ostringstream oss3;
		oss3.clear();
		oss3 << hTram;

		if (hTram < 10) {
			hangTram = "00";
		}
		else if (hTram >= 10 && hTram < 100) {
			hangTram = "0";
		}
		hangTram += oss3.str();

		return (hangTrieu + "," + hangNghin + "," + hangTram + "");
	}

	return "";
}

string Avatar::convertMoneyFromDouble(double money) {
	// tiền tỷ
	string mString;
	if (money >= 1000000000) {
		int ti = money / 1000000000;
		mString = convertMoney(ti);
		return (mString + " tỷ");
	}
	else {
		return (convertMoney((int)money));
	}
	return "!";
}

string Avatar::convertMoneyFromDouble_Detail(double money) {

	if (money < 1000000000) {
		return convertMoney(money) + " xu";
	}
	else {
		money = money / 1000;

		if (money > 1000000000) {
			return convertMoneyFromDouble(money * 1000);
		}
		else return (convertMoney((int)money) + "k");
	}
}

void Avatar::setMeIsBoss(bool isBoss) {
	this->meIsBoss = isBoss;
}

void Avatar::setBlinkAvatar() {
	Sprite* blinkAvatar = Sprite::create("frame_time.png");
	blinkAvatar->setTag(20);
	this->addChild(blinkAvatar);

	ActionInterval *blink = Blink::create(1, 2);
	ActionInterval *action_back = blink->reverse();
	ActionInterval* seq = Sequence::create(blink, action_back, NULL);

	blinkAvatar->runAction(RepeatForever::create(seq));
}

ImageView* Avatar::getFlag() {
	return this->flag;
}

bool Avatar::isStartedGame() {
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL)
		return false;
	boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
	if (rv == NULL) return false;
	string s = *rv->GetStringValue();

	vector<string> lstBet = mUtils::splitString(s, '@');
	bool isStartedGame = false;
	lstBet.at(1).compare("1") == 0 ? (isStartedGame = true) : (isStartedGame = false);
	if (isStartedGame) {
		log("Ban dang choi!");
	}
	else log("Ban chua choi!");

	return isStartedGame;
}

void Avatar::setCanTouch(bool _cantouch) {
	if (!_cantouch)
		log("Tag this %d", this->getTag());
	this->isCanTouch = _cantouch;
}

void Avatar::setLbMoneyBet(string text) {

}

Label* Avatar::getLbMoneyBet() {
	return 0;
}

void Avatar::showFlagIsMe() {
	flagIsMe->setVisible(true);
}

void Avatar::hideFlagIsMe() {
	flagIsMe->setVisible(false);
}

void Avatar::showSameIP(bool state){
	//this->spSameIP->setVisible(state);
}

void Avatar::showChat(string& mess){
	if (this->getTag() == -1) return;
	//check for emotion
	string path = mess + ".png";
	Vec2 point = Vec2(0, W_AVATAR / 2);
	Sprite* spriteEmo = Sprite::create(path);
	if (spriteEmo)
	{
		layerWidget->removeChildByTag(tag_EMOTION);
		spriteEmo->setAnchorPoint(Vec2(0.5, 0.5));
		spriteEmo->setPosition(point);
		Spawn *acpawn = Spawn::create(ScaleTo::create(1, 2, 2), JumpBy::create(0.3, Vec2(0, 0), 30, 1), nullptr);
		Sequence* actionse = Sequence::create(Repeat::create(acpawn, 3), RemoveSelf::create(true), nullptr);
		layerWidget->addChild(spriteEmo, tag_EMOTION, tag_EMOTION);
		spriteEmo->runAction(actionse);
		return;
	}
	else{
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
		{
			layerWidget->removeChildByTag(tag_STRING_CHAT);
			Chat* message = Chat::create(mess, 0);
			message->setAnchorPoint(Vec2(0.5, 0.5));
			message->setPosition(point);
			message->setTag(tag_STRING_CHAT);
			layerWidget->addChild(message);
		}
		else{
			layerWidget->removeChildByTag(tag_STRING_CHAT);
			Chat* message = Chat::create(mess, "khungchat.png");
			message->setAnchorPoint(Vec2(0.5, 0.5));
			message->setPosition(point);
			message->setTag(tag_STRING_CHAT);
			layerWidget->addChild(message);
		}
	}
}

void Avatar::SetLevel(const int& level) {
	if (this->p_Level == NULL)
		return;
	this->n_Level = level;
	std::string t = boost::to_string(level);
	this->p_Level->setString(t);
}

void Avatar::SetLevel(const std::string& level) {
	if (this->p_Level == NULL)
		return;
	int l = atoi(level.c_str());
	this->n_Level = l;
	this->p_Level->setString(level);
}

int& Avatar::GetLevel() {
	return this->n_Level;
}

Label* Avatar::GetLabelLevel() {
	return this->p_Level;
}

void Avatar::StartAnimExp(const std::string& exp) {
	if (0 == exp.compare("") || 0 == exp.compare("0"))
		return;

	std::string s = "+";
	s.append(exp);
	s.append(" exp");

	Label* e = Label::createWithBMFont(exp_font_export, s);
	e->setAnchorPoint(Vec2(0.5, 0));
	e->setScale(0.7);
	e->setColor(Color3B::YELLOW);
	e->setPosition(Vec2(5, -this->sizeThis.height / 2));
	this->addChild(e);

	MoveTo* move = MoveTo::create(1.7, Vec2(5, this->sizeThis.height - 70));
	RemoveSelf* removeSef = RemoveSelf::create(true);
	e->runAction(Sequence::create(move, removeSef, nullptr));
}

void Avatar::StartAnimExp(const int& exp) {
	std::string t = boost::to_string(exp);
	this->StartAnimExp(t);
}

void Avatar::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	if (0 == this->aI.compare("") || !this->isVisible()) return;
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	if (ptrEventParams == NULL)
		return;
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	if (ptrEventParamValueUser == NULL) return;
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	if (ptrNotifiedUser == NULL) return;

	if (ptrNotifiedUser->GetVariable("amf") == NULL || ptrNotifiedUser->GetVariable("amfs") == NULL ||
		ptrNotifiedUser->GetVariable("lvl") == NULL ||
		ptrNotifiedUser->GetVariable("exp") == NULL ||
		ptrNotifiedUser->GetVariable("accVipType") == NULL) return;

	std::string name = *ptrNotifiedUser->Name();

	if (0 == this->aI.compare(name)) {

        if (SceneManager::getSingleton().getGameID() != kGameXiTo && SceneManager::getSingleton().getGameID() != kGameLieng && SceneManager::getSingleton().getGameID() != kGamePoker ){
		if (SceneManager::getSingleton().getCurrRoomType() == 0){

			double money = *ptrNotifiedUser->GetVariable("amf")->GetDoubleValue();
			this->setMoney(money);
		}
		else{
			double money = *ptrNotifiedUser->GetVariable("amfs")->GetDoubleValue();
			this->setMoney(money);
		}
        }

		int nVar = -1;
		nVar = *ptrNotifiedUser->GetVariable("lvl")->GetIntValue();
		this->SetLevel(boost::to_string(nVar));

		nVar = *ptrNotifiedUser->GetVariable("exp")->GetIntValue();
		if (-1 == this->mCurrentExp) {

		}
		else {
			if (this->mCurrentExp != nVar) {
				int delta = nVar - mCurrentExp;
				if (delta > 0 && delta < 1000)
					this->StartAnimExp(boost::to_string(delta));
			}
		}
		this->mCurrentExp = nVar;

		boost::shared_ptr<UserVariable> accVipType = ptrNotifiedUser->GetVariable("accVipType");
		if (accVipType != NULL) {
			this->circleAvatar->setTypeVip(*accVipType->GetIntValue());
		}
	}
}

void Avatar::setCallBackTimer(Ref *target, SEL_CallFunc listener) {
	mCallBackListener = listener;
	mCallBack = target;
}

void Avatar::runAnimationWarning(float dt)
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(Avatar::runAnimationWarning),this);
	int delta = 2;
	auto MoveTo1 = MoveBy::create(0.10, Point(delta, 0));
	auto MoveTo2 = MoveBy::create(0.10, Point(0, -delta));
	auto MoveTo4 = MoveBy::create(0.10, Point(-delta * 2, 0));
	auto MoveTo3 = MoveBy::create(0.10, Point(0, delta * 2));
	auto MoveTo5 = MoveBy::create(0.10, Point(delta, -delta));
	auto action = (ActionInterval*)Sequence::create(MoveTo1, MoveTo2, MoveTo3, MoveTo4, MoveTo5, NULL);
	this->runAction(RepeatForever::create(action));
	//
	if (mUtils::isVibrateOn()){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
		NativeHelper::callVibrate(3);
#endif
	}
}

void Avatar::ResetAll()
{
	// this->flag = false;
	if (this->getChildByTag(20) != NULL)
		this->removeChildByTag(20);
	this->setName("");
	this->setMoney("");
	this->setVisibleLayerInvite(true);
	this->_vipTypePlayer = 0;
	this->aI = "";
	this->stopTimer();
	this->circleAvatar->displayTypeVip(false);
	this->forGuess = false;

	this->bg->setOpacity(255);
	this->money->setColor(_colorTextNormal);
	this->name->setColor(_colorTextNormal);
	this->money->setOpacity(255);
	this->name->setOpacity(255);
	this->icc->setOpacity(255);
	this->circleAvatar->resetPlayerRegist();
	this->lblWaiting->setVisible(false);
	this->iconLeaveGame->setVisible(false);
	this->sIp = "";
	//	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
	//		this->name->setColor(_colorTextWHITE);
	//	}

}

void Avatar::SetTaoLaBot(bool boss)
{
	this->taolabot = boss;
}

void Avatar::setPlayerRegist() {
	this->money->setColor(_colorTextWHITE);
	this->name->setColor(_colorTextWHITE);
	this->bg->setOpacity(GUESS_OPACITY);
	this->money->setOpacity(GUESS_OPACITY);
	this->name->setOpacity(GUESS_OPACITY);
	this->icc->setOpacity(GUESS_OPACITY);
	this->circleAvatar->setPlayerRegist();
	this->lblWaiting->setVisible(true);
}

void Avatar::DisplayEmotion(Chat *emo)
{
	if (this->layerWidget->getChildByTag(tag_STRING_CHAT) != NULL){
		this->layerWidget->removeChildByTag(tag_STRING_CHAT);
	}

	emo->setPosition(this->circleAvatar->getPosition());
	this->layerWidget->addChild(emo, LOCAL_ZORDER_SP_EMOTION, tag_STRING_CHAT);
	emo->setPosThis(this->circleAvatar->getPosition());
}

void Avatar::setPosition(float x, float y)
{
	Layer::setPosition(x, y);
}

void Avatar::setPosition(Vec2 vPos)
{
	Layer::setPosition(vPos);
}

void Avatar::showIconLeaveGame(bool state){
	if (this->iconLeaveGame != NULL) {
		this->iconLeaveGame->setVisible(state);
	}
}

void Avatar::onAvatarCallback()
{
	this->OnAvatarTouches(NULL, Widget::TouchEventType::ENDED);
}

void Avatar::processButtonRechardTouched(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		LayerPopupCuaHang* popupCuaHang = LayerPopupCuaHang::create();
		popupCuaHang->onBtnNapThe(NULL);
		auto _scene = Director::getInstance()->getRunningScene();
		_scene->addChild(popupCuaHang);
	}
}

void Avatar::loadImgIconRechardMe()
{
	this->icc->loadTextures("daucong.png", "", "");
	this->icc->setEnabled(true);
	this->icc->setBright(true);
}

AvatarChanNormal::AvatarChanNormal() : Avatar(false), _bibao(NULL){

}

AvatarChanNormal::~AvatarChanNormal(){

}

bool AvatarChanNormal::init(){

	if (!Avatar::init()){
		return false;
	}

	this->_bibao = Sprite::create("alert-bibao.png");
	this->addChild(this->_bibao, LOCAL_ZORDER_LB_BI_BAO);

	return true;
}

void AvatarChanNormal::ShowBiBao(){
	this->_bibao->setVisible(true);
}

void AvatarChanNormal::HideBiBao(){
	this->_bibao->setVisible(false);
}

void AvatarChanNormal::ResetAll(){
	Avatar::ResetAll();
	this->_bibao->setVisible(false);
}


AvatarChanIsMe::AvatarChanIsMe() : AvatarChanNormal()
{

}

AvatarChanIsMe::~AvatarChanIsMe()
{

}

bool AvatarChanIsMe::init()
{
	if (!AvatarChanNormal::init()){
		return false;
	}

	this->icc->setColor(Color3B::RED);
	//this->icc->setVisible(false);

	Button* _iconRechard = Button::create("daucong.png", "", "");
	_iconRechard->setAnchorPoint(Vec2(0, 0));
	_iconRechard->setPosition(Vec2(this->sizeThis.width / 2 + 20, -this->sizeThis.height / 2 + _iconRechard->getContentSize().height / 2));
	_iconRechard->setVisible(true);
	this->layerWidget->addChild(_iconRechard);
	this->icc->setScale(0.6);
	_iconRechard->addTouchEventListener(CC_CALLBACK_2(AvatarChanIsMe::processButtonRechardTouched, this));
	/*
		int roomType = SceneManager::getSingleton().getCurrRoomType();

		if (roomType == 0){
		icc->setTexture("moc3-icon-moc.png");
		}else{
		icc->setTexture("moc3-icon-hao.png");

		}*/

	this->money->setAnchorPoint(Vec2(0, 0));
	this->money->setPosition(Vec2(this->sizeThis.width / 2 + 55, -this->sizeThis.height / 2 + 20));

	this->name->setAnchorPoint(Vec2(0, 0));
	this->name->setPosition(Vec2(this->sizeThis.width / 2 + 20,
		-this->sizeThis.height / 2 + 30 * this->name->getScale() + 6 + 25));

	this->isPlayerMe = true;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		this->spSameIP->setPosition(Vec2(-W_AVATAR / 4 + 80, W_AVATAR / 3));
		this->_bibao->setPositionX(50);
	}

	return true;
}

void AvatarChanIsMe::setMoney(string pMoney) {
	if (this->money != NULL) {
		this->money->setString(pMoney);
		//this->icc->setVisible(false);
	}
}

void AvatarChanIsMe::setMoney(const long long& money){
	AvatarChanNormal::setMoney(money);
}

void AvatarChanIsMe::setMoney(int money){
	AvatarChanNormal::setMoney(money);
}

void AvatarChanIsMe::setMoney(double money){
	AvatarChanNormal::setMoney(money);
}

void AvatarChanIsMe::setName(string name){
	this->name->setString(name);
}

