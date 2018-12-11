#include "LayerGa.h"
#include "AllData.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "PhomMessDef.h"
LayerGa::LayerGa(void)
{
	mCallBack = NULL;
	mCallBackListener = NULL;
}

bool LayerGa::init(){
	if (!Layer::init())
		return false;
	Size sizeParent = Size(WIDTH_DESIGN, HEIGHT_DESIGN);
	Size _visibleSize = Director::getInstance()->getVisibleSize();
	int gameVersion = SceneManager::getSingleton().getGameVersion();
	//spriteGa = Sprite::create("chicken_bg.png");
	Size sizeAd = mUtils::getSizePos();
	spriteGa = ImageView::create();
	labelTienGa = Label::createWithSystemFont("0", "", FONT_SIZE_SMALL, Size::ZERO);
	Sprite *moneyMask = Sprite::create("moc3-money-mask.png");
	toastNotifi = Sprite::create("moc3-toastchicken.png");
	/*moc3-change position*/
	if (SceneManager::getSingleton().getGameID() == kGameBaCayChuong){
		spriteGa->loadTexture("moc3-bg-ga.png");
		spriteGa->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		spriteGa->setPosition(Point(_visibleSize.width / 2, _visibleSize.height / 2 + _visibleSize.height / 6));
		auto labelGa = Label::createWithSystemFont(dataManager.GetSysString(281), "", FONT_SIZE_SMALL, Size::ZERO);
		labelGa->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		spriteGa->addChild(labelGa);
		labelGa->setPosition(Point(50, spriteGa->getContentSize().height / 2));
		labelTienGa->setColor(Color3B::YELLOW);
		labelTienGa->setPosition(Point(250, labelGa->getPositionY()));
		spriteGa->addChild(labelTienGa);
	}
	else
	{
		spriteGa->setTouchEnabled(true);
		spriteGa->loadTexture("moc3-chicken_bg2.png");
		spriteGa->setPosition(Point(_visibleSize.width - spriteGa->getContentSize().width / 2 - 15,
			_visibleSize.height - spriteGa->getContentSize().width - 50));
		spriteGa->addTouchEventListener(CC_CALLBACK_2(LayerGa::clickSpriteGa, this));
		moneyMask->setPosition(Point(spriteGa->getContentSize().width / 2 - 20, -50));
		labelTienGa->setPosition(Point(moneyMask->getContentSize().width / 2, moneyMask->getContentSize().height / 2));
		labelTienGa->setColor(Color3B(241, 205, 99));
		moneyMask->addChild(labelTienGa);
		spriteGa->addChild(moneyMask, -10);

		toastNotifi->setAnchorPoint(Vec2(1, 0.5));
		/*moc3-change*/
		//labelLv = Label::createWithTTF("0", kNormalFont, FONT_SIZE_SMALL - 1, Size::ZERO);
		//labelLv->setColor(Color3B::BLACK);
		//labelLv->setPosition(Point(spriteGa->getContentSize().width / 2, spriteGa->getContentSize().height / 2 - 13));
		//spriteGa->addChild(labelLv);
		toastNotifi->setPosition(Point(spriteGa->getPositionX() - spriteGa->getContentSize().width + 40,
			spriteGa->getPositionY()));
		/*moc3-end change*/
		this->addChild(toastNotifi, -10);

		Label* lblNotifi = Label::createWithSystemFont(dataManager.GetSysString(952), "", 30,
			Size(toastNotifi->getContentSize().width - 50, toastNotifi->getContentSize().height),
			TextHAlignment::LEFT, TextVAlignment::CENTER);
		lblNotifi->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		lblNotifi->setPosition(Vec2(toastNotifi->getContentSize().width / 2, toastNotifi->getContentSize().height / 2));
		lblNotifi->setTextColor(Color4B::WHITE);
		toastNotifi->addChild(lblNotifi);
		toastNotifi->setVisible(false);
		//this->showToast(false);

	}
	//spriteGa->setAnchorPoint(Point(0.5, 0.5));
	this->addChild(spriteGa);
	/*moc3-end change*/
	if (SceneManager::getSingleton().getGameID() == kGameBaCay){
		layerListga7u = LayerListGa7User::create(spriteGa->getContentSize());
		layerListga7u->setAnchorPoint(Vec2::ZERO);
		layerListga7u->setPosition(Vec2(-spriteGa->getContentSize().width * 2,
			-spriteGa->getContentSize().height * 1.5));
		layerListga7u->setVisible(false);
		spriteGa->addChild(layerListga7u);
	}
	else if (SceneManager::getSingleton().getGameID() != kGameBaCayChuong){
		layerListga = LayerListGa::create(spriteGa->getContentSize());
		layerListga->setAnchorPoint(Vec2::ZERO);
		layerListga->setPosition(Vec2(-spriteGa->getContentSize().width * 1.5,
			-spriteGa->getContentSize().height * 1.5));
		layerListga->setVisible(false);
		spriteGa->addChild(layerListga);
	}
    if (SceneManager::getSingleton().isNagaWin){
        spriteGa->setPosition(Point(_visibleSize.width - spriteGa->getContentSize().width / 2 - 15,
                                    _visibleSize.height - spriteGa->getContentSize().width - 100));
    }
	return true;
}

LayerGa::~LayerGa(void)
{
}

void LayerGa::runActionAnGa(Point position, float duration)
{
	// 	Vect *endPos;
	// 	switch (posUser)
	// 	{
	// 	case kUserMe:
	// 		endPos = new Vect();
	// 
	// 		break;
	// 	default:
	// 		break;
	// 	}
	if (!isNuoi) return;
	string strPatern = "gold_";
	int currTypeRoom = SceneManager::getSingleton().getCurrRoomType();
	if (currTypeRoom == 1)
		strPatern = "silver_";
	string coinStr = StringUtils::format("%s2.png", strPatern.c_str());
	for (int i = 0; i < 3; ++i){

		Sprite *spriteCoin = Sprite::createWithSpriteFrameName(coinStr);
		if (i % 2 == 0){
			spriteCoin->setPosition(Point(spriteGa->getPositionX() + i / 2 * spriteCoin->getContentSize().width / 2 - spriteCoin->getContentSize().width,
				spriteGa->getPositionY()));
		}
		else{
			spriteCoin->setPosition(Point(spriteGa->getPositionX() + i / 2 * spriteCoin->getContentSize().width / 2 - spriteCoin->getContentSize().height / 4,
				spriteGa->getPositionY() - spriteCoin->getContentSize().height / 4));
		}
		cocos2d::Vector<SpriteFrame*> animFrames;
		//load all sprite frames into array
		for (int j = 1; j < 5; j++) {
			string name = StringUtils::format("%s%d.png", strPatern.c_str(), j);
			SpriteFrame * frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name.c_str());
			animFrames.pushBack(frame);
		}
		Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
		spriteCoin->setLocalZOrder(-i);
		spriteCoin->runAction(RepeatForever::create(CCAnimate::create(animation)));
		spriteCoin->runAction(Sequence::create(MoveTo::create(duration, position),
			CallFuncN::create(CC_CALLBACK_1(LayerGa::actionCallBack, this)), NULL));
		//spriteBatchNode->setTexture(spriteCoin->getTexture());
		//spriteBatchNode->addChild(spriteCoin, i);
		this->addChild(spriteCoin, i);
	}
}

void LayerGa::actionCallBack(Node *pSender)
{
	pSender->removeFromParentAndCleanup(true);
	this->resetAll();

}

void LayerGa::resetAll()
{
	this->stopAllActions();
	this->labelTienGa->setString("0");
	this->_tienGa = 0;
	this->setLevelChicken(0);
	listGa.clear();
	if (SceneManager::getSingleton().getGameID() == kGameBaCay)
		layerListga7u->resetAll();
	else if (SceneManager::getSingleton().getGameID() != kGameBaCayChuong)
		layerListga->resetAll();
	//labelLv->setString("0");
}

void LayerGa::setChickenMoney(double money)
{
	_tienGa = money;
	this->labelTienGa->setString(formatMoneySymbol(money));
}

void LayerGa::setListUser(string listUser, char c1, char c2)
{
	listGa.clear();
	log("LISTUSER = %s", listUser.c_str());
	if (listUser.size() <= 0) return; //khong lay duoc listuser
	vector<string> arrUsers = mUtils::splitString(listUser, c1);

	int size = arrUsers.size();
	for (int i = 0; i < size; i++) {
		vector<string> arr = mUtils::splitString(arrUsers[i], c2);
		if (arr.size() < 3)
		{
			continue;
		}
		string id = arr[0];
		string name = arr[1];
		std::string _strNameSmall = "";
		(name.length() > 12) ? _strNameSmall = name.substr(0, 12) + "..." : _strNameSmall = name;
		listGa.push_back(_strNameSmall);
	}
}

void LayerGa::setChickenLv(int _lv)
{
	//if (_lv <= 0) return;
	//labelLv->setString(StringUtils::format("%d", _lv));
	this->setLevelChicken(_lv);
	if (SceneManager::getSingleton().getGameID() == kGameBaCay)
		layerListga7u->setLevel(_lv);
	else if (SceneManager::getSingleton().getGameID() != kGameBaCayChuong)
		layerListga->setLevel(_lv);
}

void LayerGa::clickSpriteGa(Ref *pSender, Widget::TouchEventType type)
{
	//HoangDD
	//tam thoi khong cho bat tat ga
	if (type == Widget::TouchEventType::ENDED){
		//co 2 trang thai cua ga la tat opacity =150, bat la opacity = 255, chi config duoc khi chua bat dau choi

		if (mCallBack && mCallBackListener){
			(mCallBack->*mCallBackListener)();
		}
	}
}

void LayerGa::callBackFromActionGa(Ref *target, SEL_CallFunc selector)
{
	mCallBack = target;
	mCallBackListener = selector;
}

void LayerGa::setListDirect(vector<string> listUSer)
{
	listGa.clear();
	listGa = listUSer;
}

double LayerGa::getChickenMoney()
{
	return _tienGa;
}

void LayerGa::setConfigChicken(int isOn)
{
	log("CONFIG = %d", isOn);

	if (isOn == CONFIG::ON)//bat ga
	{
		spriteGa->setOpacity(255);
		setNuoiGa(true);
	}
	else//tat ga
	{
		spriteGa->setOpacity(75);
		setNuoiGa(false);
	}
}

bool LayerGa::getConfigChicken()
{
	//return spriteGa->getOpacity() == 255; //neu dang bat thi la true, tat la false;
	return isNuoi;
}

vector<string> LayerGa::getListGa()
{
	return listGa;
}

void LayerGa::showListChicken()
{
	if (listGa.size() <= 1) return;
	if (SceneManager::getSingleton().getGameID() == kGameBaCay){
		if (layerListga7u->isVisible()){
			layerListga7u->setVisible(false);
		}
		else{
			layerListga7u->setListUser(listGa);
			layerListga7u->setVisible(true);
		}
	}

	else {
		if (layerListga->isVisible()){
			layerListga->setVisible(false);
		}
		else{
			layerListga->setListUser(listGa);
			layerListga->setVisible(true);
		}
	}

}

void LayerGa::showToast(bool isshow)
{
	/*if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
		toastNotifi->setVisible(isshow);*/
}

