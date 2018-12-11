//
//  Avatar52LaLayer.cpp
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//
//

#include "mUtils.h"
#include "Avatar52La.h"
#include "SceneManager.h"
#include "LayerMoiChoi.h"
#include "NativeHelper.h"
#include "PhomMessDef.h"
#include "../layers/LayerPopupInfo.h"
#include "../layers/LayerMoiChoiInGame.h"
#include "_Number_.h"
#include "ChanUtils.h"
#include "BaiMauBinh.h"

#define TAG_LBL_RESULT  241
#define TAG_FRAME_RESULT_LIENG  242
#define TAG_FRAME_BET  243
#define tag_ketqua_3cay 245
#define tag_trung_ip 244
#define  tag_icon_result 246
#define tag_icon_lostconnect 249
#define tag_layer_number 251
#define  tag_layer_chat 250
#define tag_emotion 252
#define tag_exp 253
Avatar52La* Avatar52La::create(bool isMe, int typeAva) {
	Avatar52La* avatar52la = new Avatar52La(isMe, typeAva);
	if (avatar52la && avatar52la->init()) {
		avatar52la->autorelease();
		return avatar52la;
	}
	else {
		delete avatar52la;
		avatar52la = NULL;
		return NULL;
	}
}

bool Avatar52La::init() {
	if (!Layer::init())
		return false;
	this->mCurrentExp = -1;
	int gameVersion = SceneManager::getSingleton().getGameVersion();

	setUserIP("");

	numScale = 0.6;
	this->isRunning = false;
	sizeAd = ChanUtils::getSizePos();
	this->isGuess = false;

	this->sizeThis = Size(159, 159);
	this->sizeIcon = Size(159, 159);

	this->layerWidget = Layer::create();
	this->layerWidget->setAnchorPoint(Vec2(0.5, 0.5));

	this->bg = ImageView::create();
	this->bg->loadTexture("icon_transparent_default.png");
	this->bg->setTouchEnabled(true);
	this->bg->addTouchEventListener(CC_CALLBACK_2(Avatar52La::OnAvatarTouches, this));

	Sprite* frame_timerLoader = Sprite::create("frame_time.png");
	this->timer = ProgressTimer::create(frame_timerLoader);
	this->timer->setType(ProgressTimer::Type::RADIAL);
	this->timer->setAnchorPoint(Vec2(0.5, 0.5));
	this->timer->setScale(0.98);
	this->addChild(this->timer);

	this->flag = ImageView::create();
	this->flag->loadTexture("icon-out.png");
	this->flag->setPosition(Vec2(this->sizeThis.width / 2 - this->flag->getContentSize().width / 2,
		this->sizeThis.height / 2 - this->flag->getContentSize().height / 2));
	this->flag->setVisible(false);

	/*moc3- change*/
	this->moneyMask = Sprite::create("moc3-money-mask.png");
	moneyMask->setAnchorPoint(Vec2(0, 0.5));

	this->name = Label::createWithSystemFont("", "",
		FONT_SIZE_NORMAL - 10, Size(FONT_SIZE_NORMAL * 15, FONT_SIZE_NORMAL - 10),
		TextHAlignment::CENTER, TextVAlignment::CENTER);
	this->name->setColor(Color3B::WHITE);
	this->name->setAnchorPoint(Vec2(0.5, 0.5));

	this->money = Label::createWithSystemFont("", "",
		FONT_SIZE_SMALL - 4, Size(FONT_SIZE_SMALL * 8, FONT_SIZE_SMALL),
		TextHAlignment::LEFT, TextVAlignment::CENTER);
	this->money->setColor(Color3B(255, 241, 17));
	this->money->setAnchorPoint(Vec2(0.5, 0.5));

	this->btnRechard = Button::create("daucong.png");
	this->btnRechard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	moneyMask->addChild(this->name);
	moneyMask->addChild(money);
	moneyMask->addChild(btnRechard);
	this->name->setPosition(Vec2(moneyMask->getContentSize().width / 2, 50));
	/*moc3-end change*/
	this->money->setPosition(Vec2(moneyMask->getContentSize().width - btnRechard->getContentSize().width / 2 - 5, 20));
	this->btnRechard->setPosition(Vec2(moneyMask->getContentSize().width - btnRechard->getContentSize().width / 2 - 10.0,
		this->money->getPositionY()));

	this->btnRechard->addTouchEventListener(CC_CALLBACK_2(Avatar52La::processButtonRechardTouched, this));

	moneyMask->setPosition(Vec2(-sizeThis.width / 2 - 10, -sizeThis.height / 2 - moneyMask->getContentSize().height / 2+15));
	if (SceneManager::getSingleton().getGameID() == kGameBaCayChuong ||
		SceneManager::getSingleton().getGameID() == kGameLieng){
		moneyMask->setPosition(Vec2(-sizeThis.width / 2 - 10, -sizeThis.height / 2 - moneyMask->getContentSize().height / 2));
	}
	else{
		if (this->typeAva == 1)
		{
			log("typeAva=1");
			moneyMask->setPosition(Vec2(-W_AVATAR / 2 - 17,
				-sizeThis.height / 2 - FONT_SIZE_NORMAL * 2 + 10+30));
		}
		else if (this->typeAva == 0)
		{
			log("typeAva=0");
			moneyMask->setPosition(Vec2(W_AVATAR / 2 + 15,
				sizeThis.height / 8 - FONT_SIZE_NORMAL+30));
		}
		else if (this->typeAva == 3)
		{
			log("typeAva=3");
			//this->name->setPosition(Vec2(0, sizeThis.height / 2 + FONT_SIZE_NORMAL));
			//bgName->setPosition(Vec2(0, sizeThis.height / 2 + FONT_SIZE_NORMAL));
		}
	}
	this->layerWidget->addChild(moneyMask);

	/*Avatar tron. */
	this->circleAvatar = CircleAvatar::create();
	this->circleAvatar->setClickCallBack(CC_CALLBACK_0(Avatar52La::onAvatarCallback, this));
	//this->circleAvatar->hideIconDefault(false);

	this->labelGuess = Label::createWithSystemFont(dataManager.GetSysString(569), "", FONT_SIZE_NORMAL);
	this->labelGuess->setColor(Color3B(241, 205, 99));
	this->labelGuess->setAnchorPoint(Vec2(0.5, 0.5));
	this->labelGuess->setPosition(Vec2(0, W_AVATAR / 2 + 20));
	this->labelGuess->setVisible(false);

	this->layerWidget->addChild(this->bg);

	this->layerWidget->addChild(this->circleAvatar);
	this->layerWidget->addChild(this->flag);
	this->layerWidget->addChild(this->labelGuess);

	this->layerInvite = Layer::create();
	this->layerInvite->setAnchorPoint(Vec2(0.5, 0.5));

	Button* invite = Button::create();
	invite->loadTextures("moc3-user_invite.png", "", "");
	invite->setPressedActionEnabled(true);
	invite->setTouchEnabled(true);
	invite->addTouchEventListener(CC_CALLBACK_2(Avatar52La::OnAvatarTouches, this));
	this->layerInvite->addChild(invite);

	this->layerWidget->setVisible(false);
	this->addChild(this->layerWidget);
	this->addChild(this->layerInvite);
	///event for level

	GameServer::getSingleton().addListeners(this);


	return true;
}

Avatar52La::Avatar52La(bool isMe, int typeAva)
:timer(NULL),
layerWidget(NULL),
layerInvite(NULL),
bg(NULL),
icon(NULL),
flag(NULL),
name(NULL),
money(NULL),
nodeIcon(NULL),
aI(""),
nameString(""),
urlAvatar52La(""),
isMe(isMe),
isCanTouch(true),
meIsBoss(false),
flagIsMe(NULL),
typeAva(typeAva)
{
}

Avatar52La::~Avatar52La() {
	stopTimer();
	GameServer::getSingleton().removeListeners(this);
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);
	log("Destructor _Avatar52La_ class");
}

Size Avatar52La::getSizeThis() {
	return sizeThis;
}

void Avatar52La::OnAvatarTouches(Ref *pObject, Widget::TouchEventType pType) {
	if (pType == Widget::TouchEventType::ENDED) {

		//port sau (dautv)
		//return;

		// 		if (isMe)
		// 			return;

		int length = this->nameString.length();
		if (length == 0) {
			// 			if (isStartedGame())
			// 				return;
			// 			log("Invite User!!!!!!!!!!!!!");
			// 			auto layerMoiChoi = LayerMoiChoiInGame::create();
			// 			this->getParent()->getParent()->addChild(layerMoiChoi, 10000, WIDTH_DESIGN);
		}

		else {
			log("info of name= %s", this->nameString.c_str());
			auto layerInfo = LayerPopupInfo::create();
			layerInfo->setFriendID(aI);
			layerInfo->setUserIP(_ip);
			layerInfo->setAvatarUrl(urlAvatar52La);
			layerInfo->setIsBossRoom(false);//bo nut chu phong
			layerInfo->reloadAllDatas();
			layerInfo->setIsPlaying(isStartedGame());
			this->getParent()->getParent()->addChild(layerInfo, 10000, WIDTH_DESIGN);

		}

		//this->circleAvatar->setTypeVip(4);
	}
}

void Avatar52La::setVisibleLayerInvite(bool isShow) {
	if (isShow)
		showLayerInvite();
	else
		hideLayerInvite();
}

void Avatar52La::hideLayerInvite() {
	if (!isMe)
		layerInvite->setVisible(false);
	layerWidget->setVisible(true);
}

void Avatar52La::showLayerInvite() {
	if (!isMe)
		layerInvite->setVisible(true);
	layerWidget->setVisible(false);
}

void Avatar52La::setIcon(string url) {
	// 	SceneManager::getSingleton().getImageDownloadProcess()->downLoadImage(nodeIcon, url);
	// 	urlAvatar52La = url;
	this->urlAvatar52La = url;
	//log("avatar link is: %s", url.c_str());
	this->circleAvatar->setAvatarImage(url);
}

void Avatar52La::setFlag(bool isShow) {
	if (flag != NULL) {
		flag->setVisible(isShow);
	}
}

void Avatar52La::setName(string name) {
	//     if (name == "" && this->icc != NULL) {
	// 
	//          this->icc->setVisible(false);
	//     }
	this->nameString = name;
	if (this->name != NULL) {
		int maxLength = 15;
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2 ||
			SceneManager::getSingleton().getGameID() == kGameLieng)
			maxLength = 10;
		this->name->setString(formatUserName(name, maxLength));
	}
}

void Avatar52La::setMoney(string pMoney) {
	this->money->setString(pMoney);
}

void Avatar52La::setMoney(const long long& money) {
	setMoney(convertMoney(money));
}

void Avatar52La::setMoney(int money) {
	setMoney(convertMoney(money));
}

void Avatar52La::setMoney(double money) {
	if (money == -1)//lost connect
	{
		this->showLostConnect(true);
	}
	else{
		this->showLostConnect(false);
		this->money->setString(formatMoneySymbol(money));
	}

}

void Avatar52La::setReady(bool isReady) {
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

void Avatar52La::setAI(string ai) {
	this->aI = ai;
	if (0 == ai.compare(""))
		this->setUserIP("");
	boost::shared_ptr<User> userInfo = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(ai);
	if (userInfo != NULL)
	{
		if (userInfo->GetVariable("accVipType") != NULL){
			int accVipType = *userInfo->GetVariable("accVipType")->GetIntValue();
			this->circleAvatar->setTypeVip(accVipType);
		}
		else{
			this->circleAvatar->setTypeVip(0);
		}
		boost::shared_ptr<UserVariable> exp = userInfo->GetVariable("exp");
		if (exp != NULL) {
			this->mCurrentExp = *exp->GetIntValue();
		}
		boost::shared_ptr<UserVariable> aip_ptr = userInfo->GetVariable("aIp");
		if (aip_ptr)
		{
			boost::shared_ptr<string> aip = aip_ptr->GetStringValue();
			if (aip)
			{
				log("UID= %s / USERIP = %s", ai.c_str(), aip->c_str());
				setUserIP(aip->c_str());
			}
		}
	}

	string myName = GameUtils::getMyName();
	if (0 == myName.compare(ai))
	{
		this->displayButtonRechard(true);
	}
	else{
		this->displayButtonRechard(false);
	}
}

void Avatar52La::startTimer(const float& duration_) {
	// hideLayerInvite();
	stopTimer();

	mCurrentDuration = duration_;
	mCurrentTimer = 0;

	ProgressTo *actionTimer = ProgressTo::create(duration_, DELAY_TIME);
	timer->runAction(Sequence::create(actionTimer,
		CallFuncN::create(CC_CALLBACK_1(Avatar52La::callbackTimer, this)),
		NULL));
	//
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself != NULL) {
		if (0 == this->aI.compare(*myself->Name()))
			Director::getInstance()->getScheduler()->schedule(schedule_selector(Avatar52La::runAnimationWarning),this,duration_ *2/3, false);
	}
}

void Avatar52La::callbackTimer(Node *pSender) {
	stopTimer();
}

void Avatar52La::stopTimer() {
	this->stopAllActions();
	this->setPosition(mMyRawPoint);
	//
	if (timer == NULL) {
		return;
	}
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(Avatar52La::runAnimationWarning),this);

	if (timer->getNumberOfRunningActions() > 0) {
		timer->stopAllActions();
	}
	timer->setPercentage(0);
}

string Avatar52La::convertMoney(int money) {
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

string Avatar52La::convertMoneyFromDouble(double money) {
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

string Avatar52La::convertMoneyFromDouble_Detail(double money) {

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

void Avatar52La::setMeIsBoss(bool isBoss) {
	meIsBoss = isBoss;
}

void Avatar52La::setBlinkAvatar52La() {
	Sprite* blinkAvatar52La = Sprite::create("frame_time.png");
	blinkAvatar52La->setTag(20);
	this->addChild(blinkAvatar52La);

	ActionInterval *blink = Blink::create(1, 2);
	ActionInterval *action_back = blink->reverse();
	ActionInterval* seq = Sequence::create(blink, action_back, NULL);

	blinkAvatar52La->runAction(RepeatForever::create(seq));
}

ImageView* Avatar52La::getFlag() {
	return this->flag;
}

bool Avatar52La::isStartedGame() {
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return false;
	boost::shared_ptr<RoomVariable> rv = lastRoom->GetVariable("params");
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

void Avatar52La::setCanTouch(bool _cantouch) {
	if (!_cantouch)
		log("Tag this %d", this->getTag());
	this->isCanTouch = _cantouch;
}

void Avatar52La::showFlagIsMe() {
	flagIsMe->setVisible(true);
}

void Avatar52La::hideFlagIsMe() {
	flagIsMe->setVisible(false);
}

void Avatar52La::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	if (0 == this->aI.compare("") || !this->isVisible()) return;
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	if (ptrEventParamValueUser == NULL) return;
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	if (ptrNotifiedUser == NULL) return;

	// 	if (ptrNotifiedUser->GetVariable("amf") == NULL ||
	// 		ptrNotifiedUser->GetVariable("lvl") == NULL ||
	// 		ptrNotifiedUser->GetVariable("exp") == NULL) return;
	boost::shared_ptr<string> name_ptr = ptrNotifiedUser->Name();
	if (name_ptr == NULL) return;
	std::string name = name_ptr->c_str();

	if (0 == this->aI.compare(name)) {
		//update new version baivip for game lieng
		if (SceneManager::getSingleton().getGameID() == kGameLieng)
		{
			boost::shared_ptr<UserVariable> gameChip_ptr = ptrNotifiedUser->GetVariable(EXT_FIELD_GAME_CHIP);
			if (gameChip_ptr != NULL){
				boost::shared_ptr<double> money = gameChip_ptr->GetDoubleValue();
				if (money != NULL)
					this->setMoney(*money);
			}
		}
		else{

			if (SceneManager::getSingleton().getCurrRoomType() == 0){
				boost::shared_ptr<UserVariable> money_ptr = ptrNotifiedUser->GetVariable("amf");
				if (money_ptr != NULL){
					boost::shared_ptr<double> money = money_ptr->GetDoubleValue();
					if (money != NULL)
						this->setMoney(*money);
				}
			}
			else{
				boost::shared_ptr<UserVariable> money_ptr = ptrNotifiedUser->GetVariable("amfs");
				if (money_ptr != NULL){
					boost::shared_ptr<double> money = money_ptr->GetDoubleValue();
					if (money != NULL)
						this->setMoney(*money);
				}
			}



		}

		boost::shared_ptr<UserVariable> accVipType_ptr = ptrNotifiedUser->GetVariable("accVipType");
		if (accVipType_ptr == NULL) return;
		int accVipType = (int)*accVipType_ptr->GetIntValue();
		this->setTypeVip(accVipType);

		//MINHDV - changes - exp animation

		int nVar = -1;
		boost::shared_ptr<UserVariable> lvlptr = ptrNotifiedUser->GetVariable("lvl");
		if (lvlptr)
		{
			nVar = *lvlptr->GetIntValue();
			this->SetLevel(boost::to_string(nVar));
			boost::shared_ptr<UserVariable> expptr = ptrNotifiedUser->GetVariable("exp");
			if (expptr)
			{
				nVar = *expptr->GetIntValue();
				if (-1 == mCurrentExp)
				{

				}
				else{
					if (mCurrentExp != nVar)
					{
						int delta = nVar - mCurrentExp;
						if (delta > 0 && delta < 1000)
							StartAnimExp(boost::to_string(delta));
					}
				}
				mCurrentExp = nVar;
			}
		}

	}
}

void Avatar52La::runAnimationWarning(float dt)
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(Avatar52La::runAnimationWarning),this);
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

void Avatar52La::setPosition(float x, float y)
{
	Layer::setPosition(x, y);
	mMyRawPoint = Vec2(x, y);
}

void Avatar52La::setPosition(Vec2 vPos)
{
	Layer::setPosition(vPos);
	mMyRawPoint = vPos;
}

std::string Avatar52La::getUserName()
{
	return nameString;
}

void Avatar52La::setResult(int rescode, int type, bool isOnGame, double money)
{
	string pathImg;
	if (type == kGamePhom)
	{
		switch (rescode)
		{
		case 1:
			if (isOnGame == false)
				pathImg = "alert-utron.png";//hien tai u tron chua co
			break;
		case 0://u 0 diem
		case 2:
			if (isOnGame == false)
				pathImg = "alert-game-u.png";
			break;
		case 12:
		case 3:
			if (isOnGame == true)
				pathImg = "alert-anlao.png";
			else
				pathImg = "alert-game-ukhan.png";
			break;
		case 4:
			if (isOnGame == true)
				pathImg = "alert-denlang.png";
			else
				pathImg = "alert-nhat.png";
			break;
		case 5:
			if (isOnGame == false)
				pathImg = "alert-nhi.png";
			break;
		case 6:
			if (isOnGame == false)
				pathImg = "alert-ba.png";
			break;
		case 7:
			if (isOnGame == false)
				pathImg = "alert-bet.png";
			break;
		case 8:
			if (isOnGame == true)
				pathImg = "alert-game-anchot.png";
			else
				pathImg = "alert-mom.png";
			break;
		case 9:
			if (isOnGame == false)
				pathImg = "alert-game-uden.png";
			break;
			// 		case 10:
			// 			if (isOnGame == false)
			// 				pathImg = "alert-bocuoc.png";
			// 			break;
		case 11:
			if (isOnGame == false)
				pathImg = "alert-anlao.png";
			break;
		case 13:
			if (isOnGame == false)
				pathImg = "alert-game-uden.png";
			break;
		}
	}
	else if (type == kGameSam)
	{
		switch (rescode)
		{
		case 0:
			if (isOnGame == false) {
				if (money < 0)
					pathImg = "alert-thua.png";
				else
					pathImg = "alert-thang.png";
			}
			break;
		case 1:
			if (money < 0)
				pathImg = "alert-thoi2.png";
			break;
		case 6:
			if (money < 0)
				pathImg = "alert-denlang.png";
			//else
			//    pathImg = s_chat_xam;
			break;
		case 7:
			pathImg = "alert-denbao.png";
			break;
			//case 10:
			//    if (money > 0)
			//        pathImg = s_toi_trang;
			//    else
			//        pathImg = s_bi_cong;
			//    break;
		case 11:
			pathImg = "alert-cong.png";
			break;
		case 12:
			if (money > 0)
				pathImg = "alert-dutmu.png";
			break;
		case 13:
			if (money > 0)
				pathImg = "alert-ancong.png";
			break;
		case 14:
			if (money < 0)
				pathImg = "alert-bocuoc_2.png";
			break;
		case 15:
			if (money < 0)
				pathImg = "alert-thoituquy.png";
			break;
		case 16:
			if (money < 0)
				pathImg = "alert-thoi3doithong.png";
			break;
		case 17:
			if (money < 0)
				pathImg = "alert-thoi4doithong.png";
			break;
		case GAME_SAM_TOI_TRANG_NORMAL:
			pathImg = "toi-trang.png";
			break;
		case 100://bao sam
			pathImg = "alert_baosam.png";
			break;
		case 101://bao con 1 quan
			pathImg = "alert-bao1.png";
			break;
		}
	}
	else if (type == 24)//toi trang cua sam
	{
		switch (rescode)
		{

		case GAME_SAM_TOI_TRANG_TYPE_5_DOI:
			pathImg = "alert-5doi.png";
			break;
		case GAME_SAM_TOI_TRANG_TYPE_3_SAM:
			pathImg = "alert-3sam.png";
			break;
		case GAME_SAM_TOI_TRANG_TYPE_DONG_CHAT:
			pathImg = "alert-donghoa.png";
			break;
		case GAME_SAM_TOI_TRANG_TYPE_4_HEO:
			pathImg = "alert-tuquy2.png";
			break;
		case GAME_SAM_TOI_TRANG_TYPE_SANH_10:
			pathImg = "alert-sanhrong.png";
			break;
		}
	}
	else if (type == 25)//toi trang cua tien len dem la
	{
		switch (rescode)
		{
//        case GAME_TIENLEN_TOI_TRANG_TYPE_TU_3:
//            pathImg = "alert-tuquy3.png";
//            break;
		case GAME_TIENLEN_CAM_TOI_TRANG_TYPE_TU_2:
			pathImg = "alert-tuquy2.png";
			break;
		case GAME_TIENLEN_CAM_TOI_TRANG_TYPE_4_SAM:
			pathImg = "alert-4sam.png";
			break;
		case GAME_TIENLEN_CAM_TOI_TRANG_TYPE_6_DOI:
			pathImg = "alert-6doi.png";
			break;
		case GAME_TIENLEN_CAM_TOI_TRANG_TYPE_5_THONG:
			pathImg = "alert-game-5doithong.png";
			break;
		case GAME_TIENLEN_CAM_TOI_TRANG_TYPE_6_THONG:
			pathImg = "6-doi-thong.png";
			break;
		case GAME_TIENLEN_CAM_TOI_TRANG_TYPE_SANH_RONG:
			pathImg = "alert-sanhrong.png";
			break;
		}
	}
	else if (type == kGameBaCay){
		if (rescode == 0){
			//pathImg = "alert-nhat.png";
		}
		else if (rescode == 100) {

			pathImg = "";
		}
		else if (rescode == 101) {
			pathImg = "alert-10cu.png";
		}
		else if (rescode == 102) {
			pathImg = "alert-10-9cuu.png";
		}
		else if (rescode == 103) {
			pathImg = "alert-xam.png";
		}
		else if (rescode == 14){
			// if (money < 0)
			pathImg = "alert-bocuoc_2.png";
		}
		else if (rescode == 152){
			pathImg = "moc3-img-win.png";
		}
		else
		{
			//pathImg = "alert-bet.png";
		}
	}
    if (type == kGameMauBinh){
        switch (rescode){
            case 0:
                if (isOnGame == false) {
                    if (money < 0)
                        pathImg = "alert-thua.png";
                    else if (money > 0)
                        pathImg = "alert-thang.png";
                }
                break;
            case 100:
                pathImg = "mb_bat_sap_ho.png";
                break;
            case 101:
                pathImg = "mb_sap_ho.png";
                
                break;
            case 102:
                pathImg = "mb_bat_sap_lang.png";
                
                break;
            case 103:
                pathImg = "mb_sap_lang.png";
                
                break;
            case SPECIAL_CARD_TYPE_SANH_RONG_CUON:
                pathImg = "mb_rong_cuon.png";
                
                break;
            case SPECIAL_CARD_TYPE_SANH_RONG:
                pathImg = "mb_sanh_rong.png";
                
                break;
            case SPECIAL_CARD_TYPE_LUC_PHE_BON:
                pathImg = "mb_luc_phe_bon.png";
                
                break;
            case SPECIAL_5DOI_1SAM:
                pathImg = "mb_5_doi_1_sam.png";
                
                break;
            case SPECIAL_CARD_TYPE_3_CAI_SANH:
                pathImg = "mb_3_sanh.png";
                
                break;
            case SPECIAL_CARD_TYPE_3_CAI_THUNG:
                pathImg = "mb_3_thung.png";
                
                break;
            case SPECIAL_CARD_TYPE_BINH_LUNG:
                pathImg = "mb_binh_lung.png";
                
                break;
        }
    }
    if (type == kGameCatte){
        switch (rescode){
            case GAME_CATTE_CHET_TUNG:
                pathImg = "chet_tung.png";
                break;
            case GAME_CATTE_TOI_TRANG_TYPE_TU_QUY:
                pathImg = "alert-game-tuquy.png";
                break;
            case GAME_CATTE_TOI_TRANG_DONGCHAT:
                pathImg = "6lacungchat.png";
                break;
            case GAME_CATTE_TOI_TRANG_TYPE_BE_6:
                pathImg = "6lanhohon6.png";
                break;
            case GAME_CATTE_VICTORY_TYPE_THANG_TUNG:
                pathImg = "thang-tung.png";
                break;
            case 7:
                pathImg = "alert-thang.png";
                break;
                
        }
    }
    if (type == kGame3Doi){
        switch (rescode){
            case HAS_3_DOI:
            case HAS_3_DOI_ON_HAND:
                pathImg = "alert-3doi.png";
                break;
            case HAS_3_DOI_THONG:
            case HAS_3_DOI_THONG_ON_HAND:
                pathImg = "alert-game-3doithong.png";
                break;
            case HAS_TU_QUY_ON_HAND:
            case HAS_TU_QUY:
                pathImg = "alert-game-tuquy.png";
                break;
            case HAS_CU_LU:
                pathImg = "mb_cu_lu.png";
                break;
        }
    }
	if (pathImg.size() == 0) return;
	//layerWidget->removeChildByTag(24, true);
	layerWidget->removeChildByTag(tag_icon_result, true);
    string fullPath = StringUtils::format("icon_result/%s%s",SceneManager::getSingleton().getFolderLang().c_str(),pathImg.c_str());
	Sprite* spriteResult = Sprite::create(fullPath);
	spriteResult->setAnchorPoint(Vec2(0.5, 0.5));
	if (type == kGameBaCay || type == kGameBaCayChuong){
		spriteResult->setAnchorPoint(Vec2(0.5, 0.5));
		spriteResult->setPosition(Vec2(0, 130));
		if (rescode == 14 || rescode == 100 || rescode == 101
			|| rescode == 102 || rescode == 103)
			spriteResult->setPosition(Vec2(0, 0));
		else if (rescode == 152){
			spriteResult->setPosition(Vec2(0, -W_AVATAR / 2 + 15.0));
		}
	}
	//spriteResult->setTag(tag_icon_result);
	//layerWidget->addChild(spriteResult);
	layerWidget->addChild(spriteResult, tag_icon_result, tag_icon_result);
	if (isOnGame)
		spriteResult->runAction(Sequence::create(DelayTime::create(3), RemoveSelf::create(true), nullptr));
}
void Avatar52La::showNumberSoChi(string numberString)
{
    long num = atol(numberString.c_str());
    int len = numberString.size();
    if (len > 7)
        numScale = 0.5;
    else if (len > 9)
        numScale = 0.4;
    bool isA = num < 0;
    num = num < 0 ? -1 * num : num;
    numberString = mUtils::convertMoneyEx(num);
    if (isA)
        numberString = "-" + numberString;
    else
        numberString = "+" + numberString;
    std::replace(numberString.begin(), numberString.end(), ',', '.');
    arrNumberString.push_back(numberString);
    if (!isRunning){
        isRunning = true;
        this->runActionShowNumberSoChiMB(0);
    }
}
void Avatar52La::runActionShowNumberSoChiMB(float dt){
    int dem = 0;
    //this->layerWidget->setVisible(true);
    auto layerNumber = layerWidget->getChildByTag(tag_layer_number);
    if (layerNumber)
    {
        //neu co thi set them 1 khoang x lan nua len
        layerNumber->runAction(MoveBy::create(1, Vec2(0, layerNumber->getContentSize().height / 2 + 10)));
        //layerNumber->removeFromParentAndCleanup(true);
    }
    for (int i = 0; i < arrNumberString.size(); ++i)
    {
        string numberString = arrNumberString.at(i);
        string font_name = "font_win-font.fnt";
        if (numberString.find('-') != std::string::npos) //contain string '-'
            font_name = "font_lose-font.fnt";
        Label* labelResult = Label::createWithBMFont(font_name, numberString, TextHAlignment::CENTER);
        if (numberString.size() >= 10)
            labelResult->setScale(0.8);
        labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        layerWidget->addChild(labelResult, tag_layer_number, tag_layer_number);
        labelResult->setPosition(Vec2(0, W_AVATAR / 4 + dem*labelResult->getContentSize().height / 2 + 10));
        labelResult->runAction(Sequence::create(
                                                MoveBy::create(0.3, Vec2(0, labelResult->getContentSize().height + 10)),
                                                DelayTime::create(0.8),
                                                FadeOut::create(0.1), RemoveSelf::create(true), nullptr));
        dem++;
    }
    arrNumberString.clear();
    isRunning = false;
    
}
void Avatar52La::removeResult()
{
	//layerWidget->removeChildByTag(24, true);
	layerWidget->removeChildByTag(tag_icon_result, true);
}

void Avatar52La::setTypeVip(int _type)
{
	this->circleAvatar->setTypeVip(_type);
}

void Avatar52La::hideTypeVip()
{
	this->circleAvatar->displayTypeVip(false);
}

void Avatar52La::showChat(string mess)
{
	if (this->getTag() == -1) return;
	//check for emotion
	string path = mess + ".png";
	Vec2 point = Vec2(0, W_AVATAR / 2);
	bool isExist = isImageExist(path);

	if (isExist)
	{
		Sprite* spriteEmo = Sprite::create(path);
		layerWidget->removeChildByTag(tag_emotion);
		spriteEmo->setAnchorPoint(Vec2(0.5, 0.5));
		spriteEmo->setPosition(point);
		Spawn *acpawn = Spawn::create(ScaleTo::create(1, 2, 2), JumpBy::create(0.3, Vec2(0, 0), 30, 1), nullptr);
		Sequence* actionse = Sequence::create(Repeat::create(acpawn, 3), RemoveSelf::create(true), nullptr);
		layerWidget->addChild(spriteEmo, tag_emotion, tag_emotion);
		spriteEmo->runAction(actionse);
		return;
	}
	else{
		///Change for game p2
		layerWidget->removeChildByTag(tag_layer_chat);

		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
		{
			Chat* message = Chat::create(mess, 0);
			message->setAnchorPoint(Vec2(0.5, 0.5));
			message->setPosition(point);
			//message->setTag(tag_layer_chat);
			layerWidget->addChild(message, tag_layer_chat, tag_layer_chat);
		}
		else{
			Chat* message = Chat::create(mess, "khungchat.png");
			message->setAnchorPoint(Vec2(0.5, 0.5));
			message->setPosition(point);
			//message->setTag(tag_layer_chat);
			layerWidget->addChild(message, tag_layer_chat, tag_layer_chat);
		}
	}
}

void Avatar52La::showNumber(string numberString)
{
    if (numberString == "") return;
	long num = atol(numberString.c_str());
	int len = numberString.size();
	if (len > 7)
		numScale = 0.5;
	else if (len > 9)
		numScale = 0.4;
	bool isA = num < 0;
	num = num < 0 ? -1 * num : num;
	numberString = mUtils::convertMoneyEx(num);
	if (isA)
		numberString = "-" + numberString;
	else
		numberString = "+" + numberString;
	std::replace(numberString.begin(), numberString.end(), ',', '.');
	arrNumberString.push_back(numberString);

	if (!isRunning) {
		isRunning = true;
		//this->scheduleOnce(schedule_selector(Avatar52La::runActionShowNumber), 0.5);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(Avatar52La::runActionShowNumber), this, 0.5, 1, 0.5, false);
	}
}

void Avatar52La::showNumber(double number)
{
	arrNumberDouble.push_back(number);

	if (!isRunning) {
		isRunning = true;
		//this->scheduleOnce(schedule_selector(Avatar52La::runActionShowNumber), 0.5);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(Avatar52La::runActionShowNumber), this, 0.5, 1, 0.5, false);
	}
}

void Avatar52La::runActionShowNumber(float dt)
{
	int dem = 0;
	auto layerNumber = layerWidget->getChildByTag(tag_layer_number);
	if (layerNumber)
	{
		//neu co thi set them 1 khoang x lan nua len
		layerNumber->runAction(MoveBy::create(1, Vec2(0, layerNumber->getContentSize().height / 2 + 15.0)));
		//layerNumber->removeFromParentAndCleanup(true);
	}
	for (int i = 0; i < arrNumberString.size(); ++i)
	{
		string numberString = arrNumberString.at(i);
		string font_name = "font_win-font.fnt";
		if (numberString.find('-') != std::string::npos) //contain string '-'
			font_name = "font_lose-font.fnt";
		Label* labelResult = Label::createWithBMFont(font_name, numberString, TextHAlignment::CENTER);
		if (numberString.size() >= 10)
			labelResult->setScale(0.5);
		labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		layerWidget->addChild(labelResult, tag_layer_number, tag_layer_number);
		labelResult->setPosition(Vec2(0, W_AVATAR / 4 + dem*labelResult->getContentSize().height / 2 + 50 * i));
		labelResult->runAction(Sequence::create(
			MoveBy::create(1, Vec2(0, labelResult->getContentSize().height)),
			DelayTime::create(2),
			FadeOut::create(0.2), RemoveSelf::create(true), nullptr));
		dem++;
	}
	arrNumberString.clear();
	isRunning = false;
}

string Avatar52La::getAI()
{
	return this->aI;
}

void Avatar52La::setVisbleLabelGuess(bool isShow)
{
	int opacityValue;
	if (isShow)
	{
		opacityValue = GUESS_OPACITY;
		this->labelGuess->setVisible(true);
		this->circleAvatar->setPlayerRegist();
	}
	else{
		opacityValue = 255;
		this->labelGuess->setVisible(false);
		this->circleAvatar->resetPlayerRegist();
	}
	this->money->setOpacity(opacityValue);
	this->name->setOpacity(opacityValue);
	this->isGuess = isShow;
}

bool Avatar52La::isChauRia()
{
	return this->isGuess;
}

void Avatar52La::showLostConnect(bool isLost)
{
	this->layerWidget->removeChildByTag(tag_icon_lostconnect);
	if (isLost)
	{
		Sprite *iconLost = Sprite::create("lost_connection.png");
		iconLost->setAnchorPoint(Vec2(0.5, 0.5));
		iconLost->setPosition(Vec2(0, 0));
		iconLost->setTag(tag_icon_lostconnect);
		this->layerWidget->addChild(iconLost, tag_icon_lostconnect);
	}
}

void Avatar52La::showSameIP(bool isShow)
{
	if (isShow)
	{
		// 		showChat("TRUNG IP");
		// 		log("TRUNG IP");
		Sprite* sameIP = Sprite::create("trung-ip.png");
		sameIP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
			sameIP->setPosition(Vec2(-W_AVATAR / 4, W_AVATAR / 3));
		else
			sameIP->setPosition(Vec2(-W_AVATAR / 4, W_AVATAR / 2));
		//layerWidget->addChild(sameIP, tag_trung_ip, tag_trung_ip);
		//sameIP->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1, Vec2(0, 5)), MoveBy::create(1, Vec2(0, -5)), nullptr)));
	}
	else{
		layerWidget->removeChildByTag(tag_trung_ip);
	}
}

void Avatar52La::SetLevel(const int& level) {
	// 	if (this->p_Level == NULL)
	// 		return;
	this->n_Level = level;
	// 	std::string t = boost::to_string(level);
	// 	this->p_Level->setString(t);
}

void Avatar52La::SetLevel(const std::string& level) {
	// 	if (this->p_Level == NULL)
	// 		return;
	int l = atoi(level.c_str());
	this->n_Level = l;
	//this->p_Level->setString(level);
}

int& Avatar52La::GetLevel() {
	return this->n_Level;
}

Label* Avatar52La::GetLabelLevel() {
	//return this->p_Level;
	return NULL;
}

void Avatar52La::StartAnimExp(const std::string& exp) {
	if (0 == exp.compare("") || 0 == exp.compare("0"))
		return;
	layerWidget->removeChildByTag(tag_exp);
	std::string s = StringUtils::format("+%sexp", exp.c_str());
	//s.append(exp);
	//s.append(" exp");

	Layer* layerResult = Layer::create();
	layerResult->setAnchorPoint(Vec2::ZERO);
	layerResult->setPosition(Vec2::ZERO);
	layerResult->setVisible(false);
	layerWidget->addChild(layerResult, tag_exp, tag_exp);

	LabelBMFont* e = LabelBMFont::create(s, "exp-font-export.fnt");
	e->setColor(Color3B::WHITE);
	e->setAnchorPoint(Vec2(0.5, 0));
	e->setPosition(Vec2(0, 0));
	//e->setVisible(false);
	layerResult->addChild(e);
	// 	Label *e = Label::createWithTTF(s, "SFUStaffordBold.TTF", 60);
	// 	e->setAnchorPoint(Vec2(0.5, 0));
	// 	e->setPosition(Vec2(0, 0));
	// 	layerWidget->addChild(e, tag_trung_ip);

	// 	Sprite *expIcon = Sprite::create("exp.png");
	// 	expIcon->setAnchorPoint(Vec2(0.5, 0));
	// 	expIcon->setPosition(Vec2(e->getContentSize().width + 20, 0));
	// 	//expIcon->setVisible(false);
	// 	layerResult->addChild(expIcon);

	MoveBy* move = MoveBy::create(1.7, Vec2(0, W_AVATAR / 2));
	RemoveSelf* removeSef = RemoveSelf::create(true);
	layerResult->runAction(Sequence::create(DelayTime::create(2), Show::create(), move, removeSef, nullptr));
	//expIcon->runAction(Sequence::create(DelayTime::create(1), Show::create(), move->clone(), removeSef->clone(), nullptr));
}

void Avatar52La::StartAnimExp(const int& exp) {
	std::string t = boost::to_string(exp);
	this->StartAnimExp(t);
}

void Avatar52La::SetResult4AllUser(string& resuilt, string& score)
{
	auto BG = Sprite::create("moc3-avatar-result.png");

	string txt1 = "";
	string txt2 = "";
	string txt3 = "";
	if (1 == atoi(resuilt.c_str()))
	{
		log("score: %s", score.c_str());
		vector<string> arr = mUtils::splitString(score, '|');
		if (arr.size() < 3) return;
		txt1 = arr[0];
		txt2 = arr[1] + " " + GetTricks(arr[2]);
		txt3 = arr[3];
		if (1 == atoi(arr[1].c_str())){
			txt1 = arr[0];
			txt2 = "Át " + GetTricks(arr[2]);
		}
		if (0 == txt3.compare("3")){
			txt2 = "Sám cô " + arr[1];
		}
		else if (0 == txt3.compare("1"))
		{
			txt2 = arr[1] + " Cửu";
		}
	}
	log("text2: %s", txt2.c_str());
	auto KetQua1 = Label::createWithSystemFont(txt1, "", 45);
	KetQua1->setAnchorPoint(Vec2(0.5, 0.5));
	KetQua1->setPosition(Vec2(80, 100));
    KetQua1->setColor(Color3B::RED);

	auto KetQua2 = Label::createWithSystemFont(txt2, "", 35);
	KetQua2->setAnchorPoint(Vec2(0.5, 0.5));
	KetQua2->setPosition(Vec2(72, 60));
    KetQua2->setColor(Color3B::RED);

	BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	BG->setPosition(Vec2(0, 0));
	layerWidget->addChild(BG, tag_ketqua_3cay, tag_ketqua_3cay);

	BG->addChild(KetQua1);
	BG->addChild(KetQua2);
}

std::string Avatar52La::GetTricks(string& trick)
{
	int itr = atoi(trick.c_str());
	switch (itr){
	case 0:
		return dataManager.GetSysString(487);
	case 1:
		return dataManager.GetSysString(488);
	case 2:
		return dataManager.GetSysString(489);
	case 3:
		return dataManager.GetSysString(490);
	default:
		return "";
	}
	return "";
}

void Avatar52La::resetAll()
{
	this->setFlag(false);
	this->setName("");
	this->setMoney("");
	this->setVisibleLayerInvite(true);
	this->hideTypeVip();
	this->setVisbleLabelGuess(false);
}

void Avatar52La::removeResultBaCay()
{
	layerWidget->removeChildByTag(tag_ketqua_3cay);
}

bool Avatar52La::isImageExist(string url)
{
	int imgNum = 45;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
		imgNum = 20;
	}
	for (int i = 0; i < imgNum; i++)
	{
		string imgPath = StringUtils::format("emo_%d.png", (i + 1));
		if (0 == imgPath.compare(url))
			return true;
	}
	return false;
}

void Avatar52La::setResultLieng(string pointcard, string card_type, string maxtype, string maxsubtance)
{
	int typeRes = atoi(card_type.c_str());
	layerWidget->removeChildByTag(tag_ketqua_3cay);
	string StringRes;
	if (typeRes == 1)
	{
		StringRes = this->formatResLieng(pointcard, maxtype, maxsubtance);
		ImageView * bgRes = ImageView::create("moc3-avatar-result.png");
		bgRes->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bgRes->setPosition(Vec2::ZERO);
		Label *KetQua1 = Label::createWithSystemFont(StringRes, "", 40);
		KetQua1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		KetQua1->setPosition(Vec2(80, 80));
        KetQua1->setColor(Color3B::RED);
		KetQua1->setHorizontalAlignment(TextHAlignment::CENTER);
		bgRes->addChild(KetQua1);

		layerWidget->addChild(bgRes, tag_ketqua_3cay, tag_ketqua_3cay);
		frameResult->setVisible(false);
	}
	else{
		string pathImg;
		switch (typeRes)
		{
		case 2:
			pathImg = "res_anh.png";
			break;
		case 3:
			pathImg = "res_sap.png";
			break;
		case 4:
			pathImg = "res_lieng.png";
			break;
		case 5:
			StringRes = dataManager.GetSysString(230);
			break;
		case 6:
			StringRes = dataManager.GetSysString(660);
			break;
		case 106:
			StringRes = dataManager.GetSysString(31);
			break;
		case 107:
			StringRes = dataManager.GetSysString(32);
			break;
		default:
			return;
		}
		if (!StringRes.empty())
		{
			ImageView * bgRes = ImageView::create("moc3-avatar-result.png");
			bgRes->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			bgRes->setPosition(Vec2::ZERO);
			Label *KetQua1 = Label::createWithSystemFont(StringRes, "", 40);
			KetQua1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			KetQua1->setPosition(Vec2(73, 73));
			KetQua1->setColor(Color3B(241, 205, 99));
			KetQua1->setHorizontalAlignment(TextHAlignment::CENTER);
			bgRes->addChild(KetQua1);

			layerWidget->addChild(bgRes, tag_ketqua_3cay, tag_ketqua_3cay);
			frameResult->setVisible(false);
		}
		else{
			this->hideFrameBet(true);
			frameResult->setVisible(true);
			frameResult->loadTexture(pathImg);
		}
	}
}

std::string Avatar52La::formatResLieng(string pointcard, string maxtype, string maxsubtance)
{
	string res = StringUtils::format("%s\n%s", pointcard.c_str(),dataManager.GetSysString(556).c_str());
	int point = atoi(pointcard.c_str());
	if (point == -1)
		res = dataManager.GetSysString(908);
	return res;
}

void Avatar52La::createFrameBetLieng()
{
	this->layerWidget->setVisible(true);
	int myTag = this->getTag();
	//Vec2 posFrame = Vec2(W_AVATAR / 2 + 30, W_AVATAR / 2 - 30);
	Vec2 posFrame = Vec2(W_AVATAR / 2 - 15.0, H_MONEY_BG + 5.0);
	Vec2 anchorFrame = Vec2(0, 0);
	destPos = Vec2(posFrame.x + 150, posFrame.y + 20);
	//	h_card_me = 110;
	//h_card_notme = 75;

	int addValue = 70;
	switch (myTag)
	{
	case kuser1:
	case kuser2:
	case kuser3:
		anchorFrame = Vec2(0, 1);
		posFrame = Vec2(100, -50 - H_MONEY_BG / 2);
		destPos = Vec2(posFrame.x + 150, posFrame.y - 20);
		addValue = 190;
		break;
	case kuser5:
	case kuser4:
	case kuser6:
		anchorFrame = Vec2(1, 1);
		posFrame = Vec2(-W_AVATAR + 50, -50 - H_MONEY_BG / 2);
		destPos = Vec2(posFrame.x - 150, posFrame.y - 20);
		addValue = 190;
		break;
	}
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		ImageView *frameBet = ImageView::create("frame_bet.png");
		frameBet->setAnchorPoint(anchorFrame);
		frameBet->setPosition(posFrame);
		this->layerWidget->addChild(frameBet, TAG_FRAME_BET, TAG_FRAME_BET);

		lblBet = Label::createWithSystemFont("", "", 25, Size(200, 52), TextHAlignment::LEFT, TextVAlignment::CENTER);
		lblBet->setColor(Color3B::YELLOW);
		lblBet->setAnchorPoint(Vec2(0, 0.5));
		lblBet->setPosition(Vec2(75, frameBet->getContentSize().height / 2 - lblBet->getContentSize().height / 2));
		frameBet->addChild(lblBet);

		//	w_card_notme = 60;
		//h_card_notme = 75;
		frameResult = ImageView::create("res_anh.png");
		frameResult->setAnchorPoint(anchorFrame);
		//frameResult->setPosition(Vec2(posFrame.x, posFrame.y + addValue));
		frameResult->setPosition(posFrame);
		frameResult->setVisible(false);
		layerWidget->addChild(frameResult, TAG_FRAME_RESULT_LIENG, TAG_FRAME_RESULT_LIENG);

		//create array coins in center avatar
		startPos = this->bg->getPosition();

		string pathImg = "moc3-icon-hao.png";
		if (SceneManager::getSingleton().getCurrRoomType() == 0)//gold
		{
			pathImg = "moc3-icon-moc.png";
		}
		for (int i = 0; i < 10; i++)
		{
			Sprite* coins = Sprite::create(pathImg);
			coins->setAnchorPoint(anchorFrame);
			coins->setPosition(startPos);
			coins->setVisible(false);
			coins->setLocalZOrder(tag_trung_ip);
			this->layerWidget->addChild(coins);
			arrCoins.push_back(coins);
		}
	}
}

void Avatar52La::hideFrameBet(bool isHide)
{
	ImageView *frameBet = (ImageView*)layerWidget->getChildByTag(TAG_FRAME_BET);
	if (frameBet != NULL)
		frameBet->setVisible(!isHide);
}

void Avatar52La::setBetValue(double betvl)
{
	this->hideFrameBet(false);
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
		lblBet->setString(mUtils::convertMoneyEx(betvl));
	else
		lblBet->setString(StringUtils::format("%s %s",dataManager.GetSysString(572).c_str(), mUtils::convertMoneyEx(betvl).c_str()));
}

void Avatar52La::hideFrameResult()
{
	frameResult->setVisible(false);
}

void Avatar52La::runActionBet(double betvl)
{
	if (betvl == 0)
		return;
	this->hideFrameBet(false);
	CallFuncN* callBack = CallFuncN::create([&](Node* pSender){
		pSender->runAction(Hide::create());
		pSender->setPosition(this->startPos);
		pSender->setScale(1);
	});

	for (Sprite* coin : arrCoins)
	{
		coin->setVisible(true);
	}
	for (unsigned int i = 0; i < arrCoins.size(); i++)
	{
		float _delay = 0.01 + 0.02*i;
		MoveTo * moveTo = MoveTo::create(0.2 + _delay, destPos);
		ScaleTo* scaleTo = ScaleTo::create(0.2 + _delay, 0.5);
		DelayTime* delayAction = DelayTime::create(_delay);
		Spawn* spawnAction = Spawn::create(moveTo, scaleTo, NULL);
		arrCoins.at(i)->runAction(Sequence::create(delayAction, spawnAction, callBack, NULL));
	}
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
		lblBet->setString(mUtils::convertMoneyEx(betvl));
	else
		lblBet->setString(StringUtils::format("%s %s",dataManager.GetSysString(572).c_str(), mUtils::convertMoneyEx(betvl).c_str()));
}

void Avatar52La::runActionTotalBet(Vec2 dest)
{
	auto parent = this->getParent();
	if (parent != NULL)
	{
		vector<Sprite*> coinsArr;
		string pathImg = "moc3-icon-hao.png";
		if (SceneManager::getSingleton().getCurrRoomType() == 0)//gold
		{
			pathImg = "moc3-icon-moc.png";
		}
		for (int i = 0; i < 10; i++)
		{
			Sprite* coins = Sprite::create(pathImg);
			coins->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			coins->setPosition(this->getPosition());
			coins->setLocalZOrder(tag_trung_ip);
			parent->addChild(coins);
			coinsArr.push_back(coins);
		}
		CallFuncN* callBack = CallFuncN::create([&](Node* pSender){
			pSender->removeFromParentAndCleanup(true);
		});
		for (unsigned int i = 0; i < coinsArr.size(); i++)
		{
			float _delay = 0.01f + 0.03f*i;
			MoveTo * moveTo = MoveTo::create(0.2 + _delay, dest);
			ScaleTo* scaleTo = ScaleTo::create(0.2 + _delay, 0.5);
			DelayTime* delayAction = DelayTime::create(_delay);
			Spawn* spawnAction = Spawn::create(moveTo, scaleTo, NULL);
			coinsArr.at(i)->runAction(Sequence::create(delayAction, spawnAction, callBack, NULL));
		}
	}
}

void Avatar52La::onAvatarCallback()
{
	this->OnAvatarTouches(NULL, Widget::TouchEventType::ENDED);
}

void Avatar52La::processButtonRechardTouched(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		LayerPopupCuaHang *popupCuaHang = LayerPopupCuaHang::create();
		popupCuaHang->onBtnNapThe(NULL);
		auto _scene = Director::getInstance()->getRunningScene();
		_scene->addChild(popupCuaHang);
	}
}

void Avatar52La::displayButtonRechard(bool _display)
{
	//this->btnRechard->setVisible(_display);
	string pathImg = "moc3-icon-moc.png";
	if (_display)
		pathImg = "daucong.png";
	else {
		int typeRoom = SceneManager::getSingleton().getCurrRoomType();
		if (typeRoom == 1)
			pathImg = "moc3-icon-hao.png";
	}
	this->btnRechard->loadTextures(pathImg, "", "");
}

void Avatar52La::showAnimationWin(bool isShow)
{
	this->circleAvatar->showAnimationWin(isShow);
}




