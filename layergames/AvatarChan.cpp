#include "AvatarChan.h"
#include "mUtils.h"
#include "AllData.h"
#include "GameServer.h"
#include "SceneManager.h"
#include "LayerMoiChoi.h"
#include "LayerPlayerInfo.h"
#include "../Common.h"

AvatarChan::AvatarChan(const bool& isMe) 
	:Timer(NULL), 
	LayerWidget(NULL),
	layerInvite(NULL),
	BG(NULL),
	DisplayName(NULL),
	DisplayMoney(NULL),
	nodeIcon(NULL),
	lblNTF(NULL),
	aI(""),
	nameString(""),
	urlAvatar(""),
	IsMe(isMe),
	IsCanTouch(true),
	IsBoss(false),
	flagIsMe(NULL)
{

}

AvatarChan::~AvatarChan() 
{
	CC_SAFE_DELETE(Downloader);
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);
	log("Destructor _Avatar_ class");
}

AvatarChan* AvatarChan::create(const bool& isSmall)
{
	AvatarChan* avatar = new AvatarChan(isSmall);
	if(avatar && avatar->init())
	{
		avatar->autorelease();
		return avatar;
	}
	else
	{
		delete avatar;
		avatar = NULL;
		return NULL;
	}
}

bool AvatarChan::init()
{
	if(!Layer::init())
		return false;

	Downloader = new ImageDownloader();

	this->setAnchorPoint(Point(0, 0));

	if (IsMe) 
	{
		sizeThis.setSize(196, 56);

		Sprite *time = Sprite::create("frame_time_2.png");
		Timer = ProgressTimer::create(time);
		Timer->setType(ProgressTimer::Type::RADIAL);
		Timer->setPosition(Point(GetSizeThis().width / 2, GetSizeThis().height / 2 + 1.68));
		this->addChild(Timer);

		// widget
		LayerWidget = Layer::create();
		LayerWidget->setPosition(Timer->getPosition());
		this->addChild(LayerWidget);

		// background
		BG = ImageView::create();
		BG->loadTexture("frame_me_info.png");
		BG->setTouchEnabled(true);
		BG->addTouchEventListener(this, toucheventselector(AvatarChan::OnAvatarTouches));

		// name
		DisplayName = Label::createWithTTF("", "", 12, Size(GetSizeThis().width / 2, GetSizeThis().height / 2.5), TextHAlignment::LEFT);
		DisplayName->setPosition(Point(20, 10));

		// money
		DisplayMoney = Label::createWithTTF("", "", 12, Size(GetSizeThis().width / 2, GetSizeThis().height / 2.5), TextHAlignment::LEFT);
		DisplayMoney->setPosition(Point(20, -14));
		DisplayMoney->setColor(Color3B::YELLOW);

		LayerWidget->addChild(BG);
		LayerWidget->addChild(DisplayName);
		LayerWidget->addChild(DisplayMoney);
	}

	// Avatar hiển thị cho những người chơi.
	else 
	{
		sizeThis.setSize(70, 100);
		sizeIcon.setSize(68, 68);
		float FontSize = 12;

		Sprite *frame_time = Sprite::create("timer_avatar_chan.png");
		Timer = ProgressTimer::create(frame_time);
		Timer->setType(ProgressTimer::Type::RADIAL);
		Timer->setPosition(Point(GetSizeThis().width / 2, GetSizeThis().height / 2));
		this->addChild(Timer);

		LayerWidget = Layer::create();
		LayerWidget->setAnchorPoint(Point(0.5, 0.5));
		LayerWidget->setPosition(Point(GetSizeThis().width / 2, GetSizeThis().height / 2));

		BG = ImageView::create();
		BG->loadTexture("frame_avatar_chan.png");
		BG->setTouchEnabled(true);
		BG->addTouchEventListener(this, toucheventselector(AvatarChan::OnAvatarTouches));

        DisplayName = Label::createWithTTF("Dậu A", ChanDefaultFont, FontSize, Size(GetSizeThis().width - 2, 15)
			, TextHAlignment::CENTER, TextVAlignment::CENTER);
		DisplayName->setAnchorPoint(Point(0.5, 1));
		DisplayName->setPosition(Point(sizeThis.width / 2, sizeThis.height));

        DisplayMoney = Label::createWithTTF("12345", ChanDefaultFont, FontSize, Size(GetSizeThis().width - 14, 15), TextHAlignment::CENTER);
		DisplayMoney->setAnchorPoint(Point(0.5, 0));
		DisplayMoney->setPosition(Point(sizeThis.width / 2, 0));
		DisplayMoney->setColor(Color3B::YELLOW);

		LayerWidget->addChild(BG);

		this->addChild(DisplayName);
		this->addChild(DisplayMoney);

		// invite
		layerInvite = Layer::create();
		layerInvite->setAnchorPoint(Point(0.5, 0.5));
		layerInvite->setPosition(Point(GetSizeThis().width / 2, GetSizeThis().height / 2));
// 
		ImageView *invite = ImageView::create();
		invite->loadTexture("frame_avatar_chan.png");
		layerInvite->addChild(invite);
// 
 		LayerWidget->setVisible(false);
		this->addChild(LayerWidget);
		this->addChild(layerInvite);

        lblNTF = Label::createWithTTF(dataManager.GetSysString(491),kBoldFont, 25);
		lblNTF->setScale(0.8);
		lblNTF->setPosition(Point(GetSizeThis().width / 2, GetSizeThis().height / 2));
		lblNTF->setColor(Color3B::GREEN);
		lblNTF->setVisible(false);
		this->addChild(lblNTF);

	}

	nodeIcon = Node::create();
	nodeIcon->setContentSize(sizeIcon);
	nodeIcon->setAnchorPoint(Point(0.5, 0.5));

	if (IsMe)
	{
		nodeIcon->setPositionX(-(GetSizeThis().width / 2 - sizeIcon.width / 2 - 3));
		nodeIcon->setPositionY(2);
	}

	LayerWidget->addChild(nodeIcon);

	return true;
}

Size AvatarChan::GetSizeThis() 
{
	return sizeThis;
}

void AvatarChan::OnAvatarTouches(Ref *pObject, TouchEventType pType)
{
	if (pType == TOUCH_EVENT_ENDED) 
	{
		if (IsMe)
			return;

		int length = this->nameString.length();
		if (length == 0)
		{
			if (isStartedGame())
				return;

			if(SceneManager::getSingletonPtr() == NULL)
				return;

// 			NodeLoaderLibrary* NodeLoaderLibrary = SceneManager::getSingleton().getNodeLoaderLibrary();
// 			NodeLoaderLibrary->unregisterNodeLoader("LayerSMS");
// 			NodeLoaderLibrary->registerNodeLoader("LayerSMS", LayerMoiChoiLoader::loader());
// 			CCBReader* ccbReader = new cocosbuilder::CCBReader(NodeLoaderLibrary);
// 			if (ccbReader)
// 			{
// 				LayerMoiChoi* layerMoiChoi;
// 				layerMoiChoi = (LayerMoiChoi *)ccbReader->readNodeGraphFromFile("LayerSMS.ccbi");
// 				layerMoiChoi->loadAllDatas();
// 				this->getParent()->getParent()->addChild(layerMoiChoi, 10000, HEIGHT_DESIGN);
// 				ccbReader->release();
// 			}
		}

		else
		{
// 			NodeLoaderLibrary* NodeLoaderLibrary = SceneManager::getSingleton().getNodeLoaderLibrary();
// 			CCBReader* ccbReader = new cocosbuilder::CCBReader(NodeLoaderLibrary);
// 			if (ccbReader)
// 			{
// 				LayerPlayerInfo* layerPlayerInfo;
// 				layerPlayerInfo = (LayerPlayerInfo *)ccbReader->readNodeGraphFromFile("LayerPlayerInfo.ccbi");
// 				ccbReader->release();
// 				layerPlayerInfo->setFriendId(this->aI);
// 				layerPlayerInfo->setAvatarUrl(urlAvatar);
// 				layerPlayerInfo->setIsBossRoom(IsBoss);
// 				layerPlayerInfo->reloadAllDatas();
// 				if (this->IsCanTouch)
// 					this->getParent()->getParent()->addChild(layerPlayerInfo, 10000, WIDTH_DESIGN);
// 			}
		}
	}
}

void AvatarChan::VisibleLayerInvite(const bool& isShow)
{
	if (isShow)
		ShowLayerInvite();
	else 
		HideLayerInvite();
}

void AvatarChan::HideLayerInvite() 
{
	if (!IsMe) 
		layerInvite->setVisible(false);
	LayerWidget->setVisible(true);
}

void AvatarChan::ShowLayerInvite() 
{
	if (!IsMe) 
		layerInvite->setVisible(true);
	LayerWidget->setVisible(false);
}

void AvatarChan::setIcon(string url)
{
//	Downloader->setPointerNodeImage(nodeIcon);
//	Downloader->downLoadImage(url);
//	urlAvatar = url;
}

void AvatarChan::setName(string name)
{
	this->nameString = name;
	if (name.c_str() != NULL) 
		this->DisplayName->setString(name.c_str());
}

void AvatarChan::setMoney(string pMoney)
{
	if (this->DisplayMoney != NULL)
		this->DisplayMoney->setString(pMoney.c_str());
}

void AvatarChan::setMoney(const long long& money)
{
	setMoney(convertMoney(money) + SceneManager::getSingleton().getCoinUnit());
}

void AvatarChan::setMoney(int money) 
{
	setMoney(convertMoney(money) + SceneManager::getSingleton().getCoinUnit());
}

void AvatarChan::setMoney(double money)
{
	setMoney(convertMoneyFromDouble_Detail(money));
}

void AvatarChan::setReady(bool isReady)
{
	if (this->BG != NULL) 
	{
		if (isReady)
		{
			if (this->IsMe)
				this->BG->setColor(Color3B::RED);
			else 
				this->BG->setColor(Color3B::RED);
		}
		else
		{
			if (this->IsMe)
				this->BG->setColor(Color3B(255, 255, 255));
			else 
				this->BG->setColor(Color3B(255, 255, 255));
		}
	}
}

void AvatarChan::setAI(string ai)
{
	this->aI = ai;
}

void AvatarChan::startTimer(const float& duration_)
{
	stopTimer();
	ProgressTo *actionTimer = ProgressTo::create(duration_, 100);
	//Timer->runAction(Sequence::create(actionTimer,
	//	CallFunc::create(CC_CALLBACK_1(AvatarChan::callbackTimer, this)), nullptr));
}

void AvatarChan::callbackTimer(Node *pSender)
{
	stopTimer();
}

void AvatarChan::stopTimer()
{
	if (Timer == NULL) 
		return;
	if (Timer->getNumberOfRunningActions() > 0) 
		Timer->stopAllActions();
	Timer->setPercentage(0);
}

string AvatarChan::convertMoney(int money)
{
	ostringstream oss;
	if (money < 1000) 
	{
		oss.clear();
		oss << money;
		return (oss.str() + "");
	}
	else if (money >= 1000 && money <= 999999)
	{
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

		if (hTram < 10) 
			hangTram = "00";

		else if (hTram >= 10 && hTram < 100) 
			hangTram = "0";

		hangTram += oss2.str();

		return (hangNghin + "," + hangTram + "");
	}
	else if (money >= 1000000) 
	{
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

		if (hNghin < 10)
			hangNghin = "00";

		else if (hNghin >= 10 && hNghin < 100)
			hangNghin = "0";

		hangNghin += oss2.str();

		// Tram
		int hTram = (money % 1000);
		ostringstream oss3;
		oss3.clear();
		oss3 << hTram;

		if (hTram < 10)
			hangTram = "00";

		else if (hTram >= 10 && hTram < 100)
			hangTram = "0";

		hangTram += oss3.str();

		return (hangTrieu + "," + hangNghin + "," + hangTram + "");
	}
	return "";
}

string AvatarChan::convertMoneyFromDouble(double money)
{
	string mString;
	if (money >= 1000000000) 
	{
		int ti = money / 1000000000;
		mString = convertMoney(ti);
		return (mString + " tỷ");
	}
	else 
		return (convertMoney((int)money));
	return "!";
}

string AvatarChan::convertMoneyFromDouble_Detail(double money) 
{
	if (money < 1000000000) 
		return convertMoney(money) + " xu";
	else 
	{
		money = money / 1000;
		if (money > 1000000000) 
			return convertMoneyFromDouble(money * 1000);
		else 
			return (convertMoney((int)money) + "k");
	}
}

void AvatarChan::setMeIsBoss(bool isBoss)
{
	IsBoss = isBoss;
}

void AvatarChan::setLblNTFChan(const char* text)
{
	lblNTF->setString(text);
}

Label* AvatarChan::getLblNTFChan()
{
	return this->lblNTF;
}

void AvatarChan::setBlinkAvatar()
{
	Sprite* blinkAvatar = Sprite::create("frame_time.png");
	blinkAvatar->setPosition(Point(GetSizeThis().width / 2, GetSizeThis().height / 2));
	blinkAvatar->setTag(20);
	this->addChild(blinkAvatar);

	ActionInterval *blink = CCBlink::create(1, 2);
	ActionInterval *action_back = blink->reverse();
	ActionInterval* seq = CCSequence::create(blink, action_back, NULL);

	blinkAvatar->runAction(CCRepeatForever::create(seq));
}

bool AvatarChan::isStartedGame()
{
	boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
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

void AvatarChan::setCanTouch(bool _cantouch)
{
	if (!_cantouch)
		log("Tag this %d", this->getTag());
	this->IsCanTouch = _cantouch;
}

void AvatarChan::setLbMoneyBet(string text)
{
	moneBet->setString(text.c_str());
}

Label* AvatarChan::getLbMoneyBet()
{
	return moneBet;
}

void AvatarChan::showFlagIsMe()
{
	flagIsMe->setVisible(true);
}

void AvatarChan::hideFlagIsMe()
{
	flagIsMe->setVisible(false);
}
