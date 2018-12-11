#include "QuayThuong.h"
#include "data_manager.h"
#include "mUtils.h"
#include "PhomMessDef.h"
#include "SceneManager.h"
#include "ChanUtils.h"
#include "Avatar52La.h"
#include "../layers/LayerPopupInfo.h"
#include "layers/LayerPopupThongBao.h"
#define TIME_SHOW_RESULT 5

QuayThuong::QuayThuong()
{
	log("QuayThuong");
	luotquay = 0;
	labelCountRotate = NULL;
	pSprite = NULL;
	layerButtons = NULL;
	labelUserName = NULL;
	labelUserMoney = NULL;
	labelLv = NULL;
	urlAvatar52La = "";
	aI = "";
	amfUser = 0;
	firstAmf = 0;
	GameServer::getSingleton().addListeners(this);
}
QuayThuong::~QuayThuong()
{
	GameServer::getSingleton().removeListeners(this);
}

bool QuayThuong::init()
{
	if (!Layer::init())
		return false;
	visibleSize = Director::getInstance()->getVisibleSize();
	createListBet();
	createBackground();
	createAvatar();
	createButtons();

	return true;
}

void QuayThuong::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEvetnParams)["user"];
	boost::shared_ptr<User> user = ((boost::static_pointer_cast<User>(ptrEventParamValueUser)));
	//
	if (user != NULL)
	{
		string myName = SceneManager::getSingleton().getMyName();
		if (0 == myName.compare(*user->Name()))
		{
			boost::shared_ptr<UserVariable> btamptr = user->GetVariable("btam");
			if (btamptr)
			{
				luotquay = *btamptr->GetIntValue();
				if (!isStaredGame)
				{
					if (gameVersion == GameVersion::p1)
					{
						labelCountRotate->setString(boost::to_string(luotquay));
					}
					else{
						labelCountRotate->setString(StringUtils::format("Bạn còn %d lượt quay", luotquay));
					}
				}
				//labelCountRotate->setString(boost::to_string(luotquay));
			}

			if (0 == aI.compare(""))
			{
				aI = ChanUtils::GetNamePlayer(myName);
				labelUserName->setString(formatUserName(aI, 15));
			}

			// url
			boost::shared_ptr<UserVariable> aalptr = user->GetVariable("aal");
			if (aalptr)
			{
				boost::shared_ptr<string> url = aalptr->GetStringValue();
				if (url && 0 == urlAvatar52La.compare(""))
				{
					//chi can set 1 lan
					urlAvatar52La = url->c_str();
					circleAvatar->setAvatarImage(url->c_str());
				}
			}
            boost::shared_ptr<UserVariable> amfptr = user->GetVariable("amf");
            if (amfptr)
            {
                boost::shared_ptr<double> amf = amfptr->GetDoubleValue();
                if (amf)
                {
                    amfUser = *amf;
                    if (!isStaredGame)
                    {
                        firstAmf = *amf;
                        labelUserMoney->setString(formatMoneySymbol(amfUser));
                    }
                }
            }

			
			boost::shared_ptr<UserVariable> lvlptr = user->GetVariable("lvl");
			if (lvlptr && labelLv)
			{
				boost::shared_ptr<long> lvl = lvlptr->GetIntValue();
				if (lvl && labelLv != NULL)
				{
					labelLv->setString(StringUtils::format("Lv %d", *lvl));
				}
			}
		}
	}
}
void QuayThuong::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (ptrNotifiedCmd == NULL)
		return;
	if (0 == ptrNotifiedCmd->compare("grres"))
	{
		boost::shared_ptr<long> result = param->GetInt("gbv");
		if (result)
		{
			isStaredGame = true;
			log("RESULT %d", *result);
			this->startRotating(*result);
		}
	}
	else if (0 == ptrNotifiedCmd->compare("btres"))
	{
		boost::shared_ptr<long> ltbcptr = param->GetInt("ltbc");
		if (ltbcptr)
		{
			luotquay = *ltbcptr;
			log("SO LUOT QUAY = %d", luotquay);
			if (labelCountRotate)
			{
				if (gameVersion == GameVersion::p1)
				{
					labelCountRotate->setString(boost::to_string(luotquay));
				}
				else{
					labelCountRotate->setString(StringUtils::format("Bạn còn %d lượt quay", luotquay));
				}
			}
		}
	}
	else{
		log("CMD = %s", ptrNotifiedCmd->c_str());
	}
}

void QuayThuong::createBackground()
{
	string bg_img = "sam-3cay-bg.jpg";
	if (gameVersion == GameVersion::p2)
		bg_img = "sam-3cay-bg.jpg";
	Sprite* bg = Sprite::create(bg_img);
	float scaleY = visibleSize.width / HEIGHT_DESIGN;
	bg->setScaleY(scaleY);
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);
	if (gameVersion == GameVersion::p2)
	{
		Sprite* shadow = Sprite::create("vongquay-shadow.png");
		shadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		shadow->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(shadow);
	}

	pSprite = Sprite::create("roulette-cycle.png");
	pSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	pSprite->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(pSprite);
	if (gameVersion == GameVersion::p1)
	{
		Sprite* spriteLightOff = Sprite::create("light-off.png");
		spriteLightOff->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		spriteLightOff->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(spriteLightOff);

		Sprite* spriteLightOn = Sprite::create("light-on.png");
		spriteLightOn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		spriteLightOn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(spriteLightOn);

		ActionInterval *actionrepeat = Sequence::create(FadeIn::create(1.3), FadeOut::create(1), nullptr);
		spriteLightOn->runAction(RepeatForever::create(actionrepeat));
	}
	else{
		Sprite *arrowCircle = Sprite::create("muiten.png");
		arrowCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		arrowCircle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + pSprite->getContentSize().height / 2.7 + 30));
		this->addChild(arrowCircle);
	}
}

void QuayThuong::createButtons()
{
	//Button mua luot quay
	string strconfig = "2000_1;9000_5;18000_10";
	if (dataManager.getAppConfig().sListQuayThuong.size() > 0)
		strconfig = dataManager.getAppConfig().sListQuayThuong;
	log("quay thuong %s", strconfig.c_str());
	vector<string> arrStr = mUtils::splitString(strconfig, ';');
	int dis = 250;
	for (int i = 0; i < arrStr.size(); i++)
	{
		vector<string> arrCfg = mUtils::splitString(arrStr.at(i), '_');
		if (arrCfg.size() > 1)
		{
			string _count = arrCfg.at(1);
			string _money = arrCfg.at(0);
			arrInfo.push_back(qtStruct(atoi(_money.c_str()), atoi(_count.c_str())));
			createButtonBuy(_count, _money, Vec2(visibleSize.width / 2 + 700, visibleSize.height / 2 + dis - dis * i));
			if (i == 0)
				minBetBuy = atoi(_money.c_str());
		}
	}

	btnArrow = Button::create("roulette-ap.png", "disable-roulette-ap.png");
	btnArrow->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 20));
	btnArrow->setAnchorPoint(Vec2(0.5, 0.5));
	btnArrow->addTouchEventListener(CC_CALLBACK_2(QuayThuong::onButtonArrow, this));
	this->addChild(btnArrow);
	if (gameVersion == GameVersion::p1)
	{
		labelCountRotate = Label::createWithTTF(boost::to_string(luotquay), kBoldFont, 60);
		labelCountRotate->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelCountRotate->setPosition(Vec2(btnArrow->getContentSize().width / 2, btnArrow->getContentSize().height / 2));
		btnArrow->addChild(labelCountRotate);
	}
	else{
		labelCountRotate = Label::createWithTTF(StringUtils::format("Bạn còn %d lượt quay", luotquay), kBoldFont, 60);
		labelCountRotate->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelCountRotate->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + pSprite->getContentSize().height / 2.7 + 140));
		this->addChild(labelCountRotate);
	}
	layerButtons = LayerButtonInGame::create();
	this->addChild(layerButtons);
}

void QuayThuong::createListBet()
{
	//int sliceResult[10] = { 200, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 100 };
	gameVersion = SceneManager::getSingleton().getGameVersion();
	if (gameVersion == GameVersion::p1)
	{
		listBet.push_back(100000);
		listBet.push_back(500);
		listBet.push_back(50000);
		listBet.push_back(5000);
		listBet.push_back(500);
		listBet.push_back(500000);
		listBet.push_back(50000);
		listBet.push_back(200);
		listBet.push_back(10000);
		listBet.push_back(1000);
		listBet.push_back(1000000);
		listBet.push_back(100);
	}
	else{
		listBet.push_back(100);
		listBet.push_back(200);
		listBet.push_back(500);
		listBet.push_back(1000);
		listBet.push_back(5000);
		listBet.push_back(10000);
		listBet.push_back(50000);
		listBet.push_back(100000);
		listBet.push_back(500000);
		listBet.push_back(1000000);
		listBet.push_back(2000000);
		listBet.push_back(5000000);
	}
}

void QuayThuong::createButtonBuy(string count, string money, Vec2 pos)
{
	double _tag = atol(money.c_str());

	string _norpath = "btn-disable.png";
	string _presspath = "btn-enable.png";
	string _dispath = "";
	if (gameVersion == GameVersion::p2)
	{
		_norpath = "btn-tienmualuot.png";
		_presspath = "btn-tienmualuot-press.png";
		_dispath = "btn-disable.png";
	}
	Button* btn1Time = Button::create(_norpath, _presspath, _dispath);
	btn1Time->setPosition(pos);
	btn1Time->setTag(_tag);
	btn1Time->addTouchEventListener(CC_CALLBACK_2(QuayThuong::onButtonBuyRotate, this));
	this->addChild(btn1Time);

	Color3B textColor = Color3B::WHITE;
	Color3B btnColor = Color3B::WHITE;
	string count_rotate = count + " Lượt";
	int sizeLblLuot = 40;
	int sizeLblMoney = 60;
	Vec2 coinPos = Vec2(20, btn1Time->getContentSize().height / 2);
	Vec2 moneyPos = Vec2(btn1Time->getContentSize().width / 3, btn1Time->getContentSize().height / 2);
	if (gameVersion == GameVersion::p2)
	{
		textColor = Color3B::WHITE;
		btnColor = Color3B::BLACK;
		sizeLblLuot = 40;
		sizeLblMoney = 50;
		moneyPos.set(btn1Time->getContentSize().width / 2 - 30, btn1Time->getContentSize().height / 2 - 10);
		coinPos.set(btn1Time->getContentSize().width / 4, btn1Time->getContentSize().height / 2);
	}

	Label* txtLuot = Label::createWithTTF(count_rotate, kBoldFont, sizeLblLuot);
	txtLuot->setColor(textColor);
	txtLuot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	txtLuot->setPosition(Vec2(btn1Time->getContentSize().width / 2, btn1Time->getContentSize().height + 50));
	btn1Time->addChild(txtLuot);

	Label* txtMoney = Label::createWithTTF(mUtils::convertMoneyEx(_tag), kBoldFont, sizeLblMoney);
	txtMoney->setColor(btnColor);
	txtMoney->setAnchorPoint(Vec2(0, 0.5));
	txtMoney->setPosition(moneyPos);
	btn1Time->addChild(txtMoney);

	ImageView* imgcoin = ImageView::create("profile-coin.png");
    imgcoin->setScale(1.5);
	imgcoin->setAnchorPoint(Vec2(0, 0.5));
	imgcoin->setPosition(coinPos);
	btn1Time->addChild(imgcoin);
}

void QuayThuong::createAvatar()
{
	//bg-tien.png

	Color3B textColor = Color3B(255, 227, 160);
	Size sizeAdd = ChanUtils::getSizePos();
	float dis = 160 + sizeAdd.width;
	float dislabel = 100 + sizeAdd.width;
	float dislabelname = 250 + sizeAdd.width;

	Sprite* bgAvatar = Sprite::create("bg_avatar.png");
	this->addChild(bgAvatar);
	bgAvatar->setScale(1.5);
	bgAvatar->setAnchorPoint(Vec2(0, 0.5));
	bgAvatar->setPosition(Vec2(dis - 50, visibleSize.height / 2 + 100));

	circleAvatar = CircleAvatar::create();
	circleAvatar->setAnchorPoint(Vec2(0, 0.5));
	circleAvatar->setPosition(Vec2(dis + 103, visibleSize.height / 2 + 205));
	this->addChild(circleAvatar);


	circleAvatar->setScale(1.6);
	//imgAvatar->setScale(1.3);

	labelUserName = Label::createWithTTF("", kNormalFont, 45);
	labelUserName->setHorizontalAlignment(TextHAlignment::CENTER);
	labelUserName->setColor(Color3B::WHITE);
	labelUserName->setAnchorPoint(Vec2(0.5, 0.5));
	labelUserName->setPosition(Vec2(dislabelname + 10, visibleSize.height / 2 + 15));
	this->addChild(labelUserName);

	//		Sprite *iconMoney = Sprite::create("ResQuaythuong/coin.png");
	//		iconMoney->setAnchorPoint(Vec2(0, 0.5));
	//		iconMoney->setPosition(Vec2(dislabel + 20, labelUserName->getPositionY() - 80));
	//		this->addChild(iconMoney);

	labelUserMoney = Label::createWithTTF("", kNormalFont, 40);
	labelUserMoney->setHorizontalAlignment(TextHAlignment::LEFT);
	labelUserMoney->setColor(textColor);
	labelUserMoney->setAnchorPoint(Vec2(0, 0.5));
	labelUserMoney->setPosition(Vec2(dislabel + 20 + 100, labelUserName->getPositionY() - 55));
	this->addChild(labelUserMoney);
	this->circleAvatar->setClickCallBack(CC_CALLBACK_0(QuayThuong::onAvatarCallBack, this));
}

void QuayThuong::onButtonArrow(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		// 		KetQuaQuayThuong* layerketqua = KetQuaQuayThuong::create();
		// 		layerketqua->setMoney(listBet.at(8));
		// 		this->addChild(layerketqua);
		// 		return;
		if (luotquay <= 0)
		{
			this->showToast("Bạn đã hết lượt quay \n Vui lòng mua thêm");
			return;
		}
		btnArrow->setEnabled(false);
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> request(new ExtensionRequest("grreq", params, lastRoom));

		GameServer::getSingleton().Send(request);
	}
}

void QuayThuong::onButtonBuyRotate(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int moneyTag = ((Button*)pSender)->getTag();
		if (amfUser < moneyTag)
		{
			double delta = moneyTag - amfUser;
			layerButtons->eventTouchBtnCoin(NULL, ui::Widget::TouchEventType::ENDED);
			this->showToast(StringUtils::format("Bạn không đủ tiền mua lượt quay này \n Vui lòng nạp thêm %s %s!", mUtils::convertMoneyEx(delta).c_str(), SceneManager::getSingleton().getCoin(0).c_str()));
			return;
		}
		//show popup confirm only for p1 version
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
		{
			int numrotate = 0;
			int moneyrotate = 0;
			for (int i = 0; i < arrInfo.size(); i++)
			{
				if (arrInfo.at(i).money == moneyTag)
				{
					numrotate = arrInfo.at(i).numRotate;
					moneyrotate = arrInfo.at(i).money;
				}
			}
			string title = dataManager.GetSysString(141);
			string content = StringUtils::format("Bạn có chắc chắn mua %d lượt quay với giá %s %s không?", numrotate,
				formatMoneySymbol(moneyrotate).c_str(), SceneManager::getSingleton().getCoin(0).c_str());
			this->createLayerNotification(title, content, moneyTag);
		}
		else if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutInt("am", moneyTag);
			boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom == NULL)
				return;
			boost::shared_ptr<IRequest> request(new ExtensionRequest("btreq", params, lastRoom));

			GameServer::getSingleton().Send(request);
		}


	}
}

void QuayThuong::actionRotateDone()
{
	setStateArrButtonBuy(true);
	isStaredGame = false;
	btnArrow->setEnabled(true);
	if (luotquay >= 0)
	{
		if (gameVersion == GameVersion::p1)
		{
			labelCountRotate->setString(boost::to_string(luotquay));
		}
		else{
			labelCountRotate->setString(StringUtils::format("Bạn còn %d lượt quay", luotquay));
		}
	}
	firstAmf = amfUser;
	labelUserMoney->setString(formatMoneySymbol(amfUser));
	KetQuaQuayThuong* layerketqua = KetQuaQuayThuong::create();
	layerketqua->setMoney(listBet.at(prize));
	this->addChild(layerketqua);

	//log("PHAN THUONG CUA BAN LA %d", listBet.at(prize));
}
void QuayThuong::onAvatarTouches(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		auto layerInfo = LayerPopupInfo::create();
        layerInfo->setFriendID(SceneManager::getSingleton().getMyName());
		layerInfo->setAvatarUrl(urlAvatar52La);
		layerInfo->setIsBossRoom(false);
		layerInfo->reloadAllDatas();
		layerInfo->setIsPlaying(true);
		this->addChild(layerInfo, 10000, WIDTH_DESIGN);
	}
}

void QuayThuong::showToast(string mess)
{
	Chat *toast = Chat::create(mess, -1);
	this->addChild(toast, 1000);
}

void QuayThuong::layerPopupThongBaoCallBack(Ref *pSender)
{
	int	tag = ((Node*)pSender)->getTag();
	//tien lay theo tag button
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt("am", tag);
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest("btreq", params, lastRoom));

	GameServer::getSingleton().Send(request);
	this->removeChildByTag(24);
}

void QuayThuong::createLayerNotification(string title, string content_mess, int tag)
{
	LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
	layerPopup->setCallBackFromButton(this, callfuncO_selector(QuayThuong::layerPopupThongBaoCallBack), tag);
	layerPopup->setMessage(title);
	layerPopup->setContentMess(content_mess);
	layerPopup->setTag(24);
	this->addChild(layerPopup);
}

void QuayThuong::setStateArrButtonBuy(bool _isDis)
{
	for (int i = 0; i < arrInfo.size(); i++)
	{
		int tag = arrInfo.at(i).money;
		Button* button = (Button*)getChildByTag(tag);
		if (button)
		{
			button->setEnabled(_isDis);
		}
	}
}

void QuayThuong::startRotating(int resultBet)
{
	int rounds = rand() % 20 + 10;
	int slices = listBet.size();
	for (int i = 0; i < listBet.size(); i++)
	{
		if (listBet.at(i) == resultBet)
		{
			prize = i;
			break;
		}
	}
	int start_rand = 3;
	int rand_space = 30;
	int dis_value = 1;
	float duration = 10;
	float delayTime = 0.3;
	if (gameVersion == GameVersion::p2)
	{
		rand_space = 10;
		dis_value = 0;
		start_rand = -5;
		rounds = rand() % 10 + 5;
		duration = 1.5;
		delayTime = 0.2;
	}
	int rand_angle_bonus = rand() % rand_space + start_rand;
	float degress = (slices - dis_value - prize)*(360 / slices);
	log("DEGRESS %.3f", degress);
	float angle = 360 * rounds + degress + rand_angle_bonus; //goc quay
	ActionInterval *rotate = RotateTo::create(duration, angle);
	//EaseOut *easeout = EaseOut::create(rotate, 6);
	EaseCircleActionOut *easeout = EaseCircleActionOut::create(rotate->clone());
	setStateArrButtonBuy(false);
	Sequence* seqAction = Sequence::create(easeout,
		DelayTime::create(delayTime),
		CallFunc::create(CC_CALLBACK_0(QuayThuong::actionRotateDone, this)), nullptr);
	pSprite->runAction(seqAction);
}

void QuayThuong::onEnter()
{
	Layer::onEnter();
	// 	auto listener = EventListenerTouchOneByOne::create();
	// 	listener->setSwallowTouches(true);
	// 	listener->onTouchBegan = CC_CALLBACK_2(QuayThuong::onTouchBegan, this);
	// 	listener->onTouchEnded = CC_CALLBACK_2(QuayThuong::onTouchEnded, this);
	// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, circleAvatar);
}

void QuayThuong::onExit()
{
	Layer::onExit();
	//_eventDispatcher->removeEventListenersForTarget(circleAvatar);
}

bool QuayThuong::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto target = static_cast<CircleAvatar*> (unused_event->getCurrentTarget());
	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->GetSizeAvatar();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
	{
		return true;
	}
	return false;
}

void QuayThuong::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto layerInfo = LayerPopupInfo::create();
	layerInfo->setMoney(firstAmf);
	layerInfo->setAvatarUrl(urlAvatar52La);
	layerInfo->setIsBossRoom(false);
	layerInfo->reloadAllDatas();
	layerInfo->setIsPlaying(true);
	this->addChild(layerInfo, 10000, WIDTH_DESIGN);
}

void QuayThuong::onAvatarCallBack()
{
	this->onAvatarTouches(NULL, ui::Widget::TouchEventType::ENDED);
}

bool KetQuaQuayThuong::init()
{
	if (!Layer::init())
		return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(0, 0);
	if (SceneManager::getSingleton().getGameVersion() != GameVersion::p2)
	{
		bg = Sprite::create("text-bg.png");
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(bg);

		Sprite *notice = Sprite::create("chuc-mung-text.png");
		notice->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		notice->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2 + 110));
		bg->addChild(notice);
	}
	else{
		bg = Sprite::create("chuc-mung-text.png");
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(bg);
		// 		Sprite *notice = Sprite::create("chuc-mung-text.png");
		// 		notice->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		// 		notice->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		// 		this->addChild(notice);
	}
	return true;
}

bool KetQuaQuayThuong::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void KetQuaQuayThuong::onTouchEnded(Touch *touch, Event *unused_event)
{
	this->removeFromParentAndCleanup(true);
}

void KetQuaQuayThuong::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(KetQuaQuayThuong::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(KetQuaQuayThuong::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	if (SceneManager::getSingleton().getGameVersion() != GameVersion::p2)
		this->createParticle();
	this->runAction(Sequence::create(DelayTime::create(TIME_SHOW_RESULT), RemoveSelf::create(), nullptr));
}

void KetQuaQuayThuong::onExit()
{
	Layer::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

void KetQuaQuayThuong::setMoney(int money)
{
	if (SceneManager::getSingleton().getGameVersion() != GameVersion::p2)
	{
		string moneystr = StringUtils::format("%d", money);
		Label* labelResult = Label::createWithBMFont("sfuStaff-font.fnt", moneystr, TextHAlignment::CENTER);
		labelResult->setScale(0.8);
		labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelResult->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2 - 110));
		bg->addChild(labelResult);

		Sprite* moneyIcon = Sprite::create("profile-coin.png");
        moneyIcon->setScale(1.8);
		moneyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		moneyIcon->setPosition(Vec2(bg->getContentSize().width / 2 + labelResult->getContentSize().width / 2 + 80, bg->getContentSize().height / 2 - 100));
		bg->addChild(moneyIcon);
	}
	else{
		string moneystr = StringUtils::format("%s %s", mUtils::convertMoneyEx(money).c_str(),SceneManager::getSingleton().getCoin(0).c_str());
		Label* labelResult = Label::createWithBMFont("count_number.fnt", moneystr, TextHAlignment::CENTER);
		labelResult->setScale(0.8);
		labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelResult->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2 - 100));
		bg->addChild(labelResult);
	}

}

KetQuaQuayThuong::KetQuaQuayThuong()
{

}

KetQuaQuayThuong::~KetQuaQuayThuong()
{

}

void KetQuaQuayThuong::createParticle()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	ParticleSystemQuad *_emitter = ParticleExplosion::create();
	_emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("particle-01.png"));
	_emitter->setScale(5);
	_emitter->setSpeed(100.0);
	_emitter->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(_emitter);
	_emitter->setAutoRemoveOnFinish(true);
}

void KetQuaQuayThuong::setResult(string result)
{
	if (SceneManager::getSingleton().getGameVersion() != GameVersion::p2)
	{
		Label* labelResult = Label::createWithBMFont("sfuStaff-font.fnt", result, TextHAlignment::CENTER);
		labelResult->setScale(0.8);
		labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelResult->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2 - 110));
		bg->addChild(labelResult);

		Sprite* moneyIcon = Sprite::create("Coin-big.png");
		moneyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		moneyIcon->setPosition(Vec2(bg->getContentSize().width / 2 + labelResult->getContentSize().width / 2 + 80, bg->getContentSize().height / 2 - 100));
		bg->addChild(moneyIcon);
	}
	else{
		Label* labelResult = Label::createWithBMFont("count_number.fnt", result, TextHAlignment::CENTER);
		labelResult->setScale(0.8);
		labelResult->setLineHeight(160);
		labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelResult->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2 - 100));
		bg->addChild(labelResult);
	}
}
