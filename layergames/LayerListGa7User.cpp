//
//  LayerListGa7User.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 2/24/16.
//
//

#include "LayerListGa7User.h"
#include "mUtils.h"
#include "Requests/ExtensionRequest.h"
#include "AllData.h"
#include "PhomMessDef.h"
#include "GameServer.h"
#include "cotuong/gameUtils.h"

using namespace cocos2d;
//using namespace CocosDenshion;

LayerListGa7User::LayerListGa7User(Size _parentSize)
{
	sizeParent = _parentSize;
}

LayerListGa7User::~LayerListGa7User()
{
}

void LayerListGa7User::onEnter()
{
	Layer::onEnter();
	//
	this->runAction(mUtils::getActionOpenPopup());
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerListGa7User::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
	//
	this->runAction(mUtils::getActionOpenPopup());
}

void LayerListGa7User::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerListGa7User::setListUser(vector<string> _listGa)
{
	// this->spriteBackground->removeAllChildren();
	// this->spriteBackground->addChild(this->lblLevel);
	//this->spriteBackground->addChild(this->headerLabel);

	for (int i = 0; i < _listGa.size(); i++){
		this->spriteBackground->removeChildByTag(i);
	}

	//this.listView.removeAllItems();
	//if (_data.length == 3)
	//    this.listView.setPositionY(400 - 350);
	//else if (_data.length > 3)
	//    this.listView.setPositionY(400 - 370);
	//else
	//    this.listView.setPositionY(400 - 350);
	//this.listView.setContentSize(cc.size(332, (_data.length + 1) * 45));
	//
	int height = 232;
	switch (_listGa.size()){
	case 2:
		this->spriteBackground->setTexture("ga-pop-2.png");
		height = 309;
		this->spriteBackground->setPositionY(0);
		break;
	case 3:
		this->spriteBackground->setTexture("ga-pop-3.png");
		height = 350;
		this->spriteBackground->setPositionY(-43);
		break;
	case 4:
		this->spriteBackground->setTexture("ga-pop-4.png");
		height = 392;
		this->spriteBackground->setPositionY(-86);
		break;
	case 5:
		this->spriteBackground->setTexture("ga-pop-5.png");
		height = 436;
		this->spriteBackground->setPositionY(-129);
		break;
	case 6:
		this->spriteBackground->setTexture("ga-pop-6.png");
		height = 484;
		this->spriteBackground->setPositionY(-172);
		break;
	case 7:
		this->spriteBackground->setTexture("ga-pop-7.png");
		height = 522;
		this->spriteBackground->setPositionY(-215);
		break;
	}
	//this->lblLevel->setPosition(20, height - 10);
	this->headerLabel->setPosition(5, height - 90);
	this->lblLevel->setPosition(this->spriteBackground->getContentSize().width / 2, height - 50);
	this->headerLabel->setPositionX(30);
	for (int i = 0; i < _listGa.size(); ++i) {
		boost::shared_ptr<User> userInfo = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_listGa.at(i));
		if (userInfo != NULL) {
			boost::shared_ptr<UserVariable> aN = userInfo->GetVariable("aN");
			auto labelUser = Text::create();
			labelUser->setString("");
			labelUser->setFontSize(35);
			labelUser->setColor(Color3B(241, 205, 99));
			labelUser->setFontName(kNormalFont);
			labelUser->setPosition(Vec2(20, height - 130 - 45 * i));
			labelUser->setAnchorPoint(Vec2(0, 0.5));
			labelUser->setTag(i);
			this->spriteBackground->addChild(labelUser);
			if (aN != NULL){
				if (strcmp(aN->GetStringValue()->c_str(), "") == 0)
                    labelUser->setString("- " + GameUtils::getNamePlayer(_listGa.at(i)));
            }
				else
					labelUser->setString("- " + GameUtils::getNamePlayer(string(aN->GetStringValue()->c_str())));
		}
		else
		{
			auto labelUser = Text::create();
			labelUser->setString("- " + _listGa[i]);
			labelUser->setFontSize(35);
			labelUser->setFontName(kNormalFont);
			labelUser->setPosition(Vec2(20, height - 130 - 45 * i));
			labelUser->setAnchorPoint(Vec2(0, 0.5));
			labelUser->setTag(i);
			labelUser->setColor(Color3B(241, 205, 99));
			this->spriteBackground->addChild(labelUser);
		}
	}
}

bool LayerListGa7User::init()
{
	if (!Layer::init())
		return false;
	int disX = 0;
	int disY = 0;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
		disX = 150;
		disY = 150;
	}

	spriteBackground = Sprite::create("ga-pop-2.png");
	spriteBackground->setPosition(Vec2(-sizeParent.width / 4 + disX,
		-200 + disY));
	spriteBackground->setAnchorPoint(Vec2(0.5, 0));
	this->addChild(spriteBackground);

	lblLevel = Label::createWithSystemFont("10", "", 33);
	lblLevel->setAnchorPoint(Vec2(0.5, 0));
	lblLevel->setColor(Color3B::BLACK);
	lblLevel->setPosition(Vec2(spriteBackground->getContentSize().width / 2,
		spriteBackground->getContentSize().height -10));
	spriteBackground->addChild(lblLevel);

	headerLabel = Label::createWithSystemFont(dataManager.GetSysString(585), "", 35);
	headerLabel->setAnchorPoint(Vec2(0, 0.5));
	headerLabel->setPosition(Vec2(30, spriteBackground->getContentSize().height - 100));
	headerLabel->setColor(Color3B(255, 227, 160));
	spriteBackground->addChild(headerLabel);
	headerLabel->setVisible(false);

	defaultLabel = Text::create();
	defaultLabel->setString("");
	defaultLabel->setFontSize(FONT_SIZE_NORMAL);
	defaultLabel->setFontName(kNormalFont);
	defaultLabel->setAnchorPoint(Vec2(0, 0));
	defaultLabel->setPosition(Vec2(5, 0));
	defaultLabel->setColor(Color3B(255, 227, 160));

	Layout* default_item = Layout::create();
	default_item->setContentSize(Size(332, 45));
	default_item->addChild(defaultLabel);
	default_item->setAnchorPoint(Vec2(0, 0));
	default_item->setPosition(Vec2(0, 0));

	return true;
}

void LayerListGa7User::resetAll()
{
	//listView->removeAllItems();
}

void LayerListGa7User::setLevel(int _lv)
{
    lblLevel->setString(StringUtils::format("%s: %d",dataManager.GetSysString(603).c_str(), _lv));
}

LayerListGa7User* LayerListGa7User::create(Size _parentSize)
{
	LayerListGa7User* avatar52la = new LayerListGa7User(_parentSize);
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

bool LayerListGa7User::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return false;
}


