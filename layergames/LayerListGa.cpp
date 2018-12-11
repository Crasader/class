//
//  LayerListGa.cpp
//  iCasinov2
//
//  Created by AlexDao on 6/3/14.
//
//

#include "LayerListGa.h"
#include "mUtils.h"
#include "Requests/ExtensionRequest.h"
#include "AllData.h"
#include "PhomMessDef.h"
#include "GameServer.h"
#include "cotuong/gameUtils.h"
using namespace cocos2d;
//using namespace CocosDenshion;

LayerListGa::LayerListGa(Size _parentSize)
{
	sizeParent = _parentSize;
}

LayerListGa::~LayerListGa()
{
}

void LayerListGa::onEnter()
{
	Layer::onEnter();
	//
	this->runAction(mUtils::getActionOpenPopup());
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerListGa::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
	//
	this->runAction(mUtils::getActionOpenPopup());
}

void LayerListGa::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerListGa::setListUser(vector<string> _listGa)
{
	listView->removeAllItems();
	if (_listGa.size() == 3)
		listView->setPositionY(400 - 370);
	else if (_listGa.size() > 3)
		listView->setPositionY(0);
	else
		listView->setPositionY(400 - 320);
	listView->setContentSize(Size(353, (_listGa.size() + 1) * 45));
	for (int i = 0; i < _listGa.size(); ++i)
	{
		boost::shared_ptr<User> userInfo = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_listGa.at(i));
		string user_name = "- ";
		string _name = GameUtils::getNamePlayer(_listGa.at(i));
		user_name.append(_name);
// 		if (userInfo != NULL)
// 		{
// 			boost::shared_ptr<UserVariable> aN = userInfo->GetVariable("aN");
// 			if (aN != NULL){
// 				if (strcmp(aN->GetStringValue()->c_str(), "") == 0)
// 					user_name += _listGa.at(i);
// 				else
// 					user_name += aN->GetStringValue()->c_str();
// 			}
// 		}
// 		else{
// 			user_name += _listGa.at(i);
// 		}
		defaultLabel->setString(user_name);
		listView->pushBackDefaultItem();
	}
}

bool LayerListGa::init()
{
	if (!Layer::init())
		return false;
	auto spriteBackground = Sprite::create("ga-pop-7.png");
	int disX = 0;
	int disY = 0;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
		disX = 150;
		disY = 80;
	}

	spriteBackground->setPosition(Vec2(-sizeParent.width / 4 + disX,
		spriteBackground->getContentSize().height / 4 + disY));
	spriteBackground->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(spriteBackground);

	listView = ListView::create();
	listView->setDirection(cocos2d::ui::SCROLLVIEW_DIR_VERTICAL);
	listView->setBounceEnabled(true);
	listView->setBackGroundImageScale9Enabled(false);
	listView->setContentSize(Size(spriteBackground->getContentSize().width,
		700));
	listView->setAnchorPoint(Vec2(0.5, 0));
	listView->setPosition(Vec2(spriteBackground->getContentSize().width / 2,
		spriteBackground->getContentSize().height));
	spriteBackground->addChild(listView);

	lblLevel = Label::createWithSystemFont("", "", FONT_SIZE_NORMAL,
		Size(FONT_SIZE_NORMAL * 10, FONT_SIZE_NORMAL), TextHAlignment::CENTER,
		TextVAlignment::CENTER);
	lblLevel->setAnchorPoint(Vec2(0.5, 0.5));
	lblLevel->setColor(Color3B::BLACK);
	lblLevel->setPosition(Vec2(spriteBackground->getContentSize().width / 2,
		spriteBackground->getContentSize().height - 30));
	spriteBackground->addChild(lblLevel);

	// 	Label *headerLabel = Label::createWithTTF("Đang nuôi", kNormalFont, FONT_SIZE_NORMAL);
	// 	headerLabel->setAnchorPoint(Vec2(0, 0.5));
	// 	headerLabel->setPosition(Vec2(10, spriteBackground->getContentSize().height - 90));
	// 	headerLabel->setColor(Color3B(255, 227, 160));
	// 	spriteBackground->addChild(headerLabel);

	defaultLabel = Text::create();
	defaultLabel->setString("");
	defaultLabel->setFontSize(FONT_SIZE_SMALL);
	defaultLabel->setFontName(kNormalFont);
	defaultLabel->setAnchorPoint(Vec2(0, 0));
	defaultLabel->setPosition(Vec2(5, 0));
	defaultLabel->setColor(Color3B(255, 227, 160));

	Layout* default_item = Layout::create();
	default_item->setContentSize(Size(332, 45));
	default_item->addChild(defaultLabel);
	default_item->setAnchorPoint(Vec2(0, 0));
	default_item->setPosition(Vec2(0, 0));
	listView->setItemModel(default_item);
	listView->setItemsMargin(0);

	return true;
}

void LayerListGa::resetAll()
{
	listView->removeAllItems();
}

void LayerListGa::setLevel(int _lv)
{
    lblLevel->setString(StringUtils::format("%s: %d",dataManager.GetSysString(603).c_str(), _lv));
}

LayerListGa* LayerListGa::create(Size _parentSize)
{
	LayerListGa* avatar52la = new LayerListGa(_parentSize);
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

bool LayerListGa::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return false;
}

