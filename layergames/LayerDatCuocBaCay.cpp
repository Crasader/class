//
//  LayerDatCuocBaCay.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 3/11/16.
//
//

#include "LayerDatCuocBaCay.h"
#include "../Common.h"
#include "../AllData.h"
#include "GameServer.h"
#include "ClientMsgDefs.h"
#include "Requests/ExtensionRequest.h"
#include <math.h>
#include "mUtils.h"
#include "_Chat_.h"
#include "PhomMessDef.h"
#include "ChanUtils.h"
LayerDatCuocBaCay::LayerDatCuocBaCay()
{

}

LayerDatCuocBaCay::~LayerDatCuocBaCay()
{

}

bool LayerDatCuocBaCay::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->gameId = SceneManager::getSingleton().getGameID();

	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() != NULL)
	{
		boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		std::string paramString = *room->GetVariable("params")->GetStringValue();
		std::vector<std::string> arrInfo = mUtils::splitString(paramString, '@');
		std::string money = arrInfo.at(0);
		//this->minValue = *GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetDoubleValue();
		this->minValue = atoi(money.c_str());
		log("%d", this->minValue);
	}
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf != NULL)
	{
		boost::shared_ptr<UserVariable> amf_ptr = mySelf->GetVariable(EXT_FIELD_GAME_CHIP);
		if (amf_ptr != NULL)
		{
			this->userAmf = *amf_ptr->GetDoubleValue();
			this->maxValue = this->userAmf;
		}
	}
	Size sizeAdd = ChanUtils::getSizePos();
	auto root = CSLoader::getInstance()->createNode("LayerSlider.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(Size(root->getContentSize().height, root->getContentSize().width));
	root->setAnchorPoint(Vec2(0, 0));
	//root->setPosition(Vec2(visibleSize.width - root->getContentSize().width, 0 + sizeAdd.height));

	slider = static_cast<Slider*>(root->getChildByName("slider"));
	slider->setTouchEnabled(true);
	slider->setSwallowTouches(true);
	slider->addEventListener(CC_CALLBACK_2(LayerDatCuocBaCay::sliderEvent, this));
	slider->setPercent(0);

	this->imgMoney = (Sprite*)root->getChildByName("imgMoney");
	ImageView *imgCoin = static_cast<ImageView*>(this->imgMoney->getChildByName("imgCoin"));
	int roomType = SceneManager::getSingleton().getCurrRoomType();

	if (roomType == 1){ //1- hao 0- moc
		imgCoin->loadTexture("moc3-icon-hao.png");
	}
	else{
		imgCoin->loadTexture("moc3-icon-moc.png");
	}
	if (this->imgMoney != NULL)
	{
		this->labelCurrBet = (Text*)this->imgMoney->getChildByName("labelCurrBet");
		this->labelCurrBet->setString(mUtils::convertMoneyEx((long long)(this->minValue)));
	}

	this->btnConfirm = (Button*)root->getChildByName("btnConfirm");
    this->btnConfirm->setTitleText(dataManager.GetSysString(907));
	if (this->gameId == kGameBaCayChuong){
		this->maxValue = this->minValue * 2;
	}

	if (this->gameId == kGameBaCay || this->gameId == kGameLieng)
	{
		this->btnConfirm->addClickEventListener(CC_CALLBACK_1(LayerDatCuocBaCay::onButtonClick, this));
	}

	/*
	this->labelMinValue = (ui::Text*)root->getChildByName("labelMinValue");
	this->valueBet = this->minValue;
	Button* btnConfirm = (Button*)root->getChildByName("btnConfirm");
	if (btnConfirm)
	{
	btnConfirm->addClickEventListener(CC_CALLBACK_1(LayerDatCuocBaCay::onButtonClick, this));
	}

	Sprite* imgPoint75 = (Sprite*)root->getChildByName("Point_2");
	if (imgPoint75 != NULL) {
	this->lblPoint75 = (Text*)imgPoint75->getChildByName("lblPoint2");
	}
	Sprite* imgPoint50 = (Sprite*)root->getChildByName("Point_3");
	if (imgPoint50 != NULL) {
	this->lblPoint50 = (Text*)imgPoint50->getChildByName("lblPoint3");
	}
	Sprite* imgPoint25 = (Sprite*)root->getChildByName("Point_4");
	if (imgPoint25 != NULL) {
	this->lblPoint25 = (Text*)imgPoint25->getChildByName("lblPoint4");
	}
	ImageView* Image_1 = (ImageView*)root->getChildByName("Image_1");
	if (Image_1 != NULL)
	{
	Image_1->setScale9Enabled(true);
	Image_1->setScaleY(visibleSize.height / Image_1->getContentSize().height);
	}*/

	return true;
}
void LayerDatCuocBaCay::sliderEvent(Ref *pSender, Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(pSender);
		auto orgY = this->labelCurrBet->getPositionY();
		int percent = slider->getPercent();
		float distance = this->maxValue - this->minValue;

		//this->valueBet = this->money_round((distance / 100)*percent);
		//if (percent == 100){
		//	this->valueBet = this->maxValue;
		//}
		//else if (this->valueBet < this->minValue)
		//{
		//	this->valueBet += this->minValue;
		//}
		//else if (percent == 0 || this->valueBet < this->minValue){
		//	this->valueBet = this->minValue;
		//	//this->labelMinValue->setVisible(false);
		//}
		if (percent == 100){
			this->valueBet = this->maxValue;
		}
		else if (percent == 0){
			this->valueBet = this->minValue;
		}
		else{
			this->valueBet = this->minValue + distance * ((float)percent / 100);
		}

		this->labelCurrBet->setString(mUtils::convertMoneyEx((long long)(this->valueBet)));

		auto pos = (slider->getContentSize().width / 100)*percent + 320;
		imgMoney->setPositionY(pos);
	}
}

float LayerDatCuocBaCay::formatValue(float value){
	if (value >= 1 && value < 100)
		return floorf(value);
	if (value >= 100 && value < 1000)
		return floorf(value / 100) * 100;
	else if (value >= 1000 && value < 10000)
		return floorf(value / 1000) * 1000;
	else if (value >= 10000 && value < 100000)
		return floorf(value / 1000) * 1000;
	else if (value >= 100000 && value < 1000000)
		return floorf(value / 10000) * 10000;
	else if (value >= 1000000 && value < 10000000)
		return floorf(value / 100000) * 100000;
	else if (value >= 10000000 && value < 100000000)
		return floorf(value / 1000000) * 1000000;

}

void LayerDatCuocBaCay::onEnter()
{
	Layer::onEnter();
	auto _listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);
	_listener->onTouchBegan = CC_CALLBACK_2(LayerDatCuocBaCay::onTouchBegan, this);
	_listener->onTouchEnded = CC_CALLBACK_2(LayerDatCuocBaCay::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void LayerDatCuocBaCay::onExit()
{
	Layer::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool LayerDatCuocBaCay::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void LayerDatCuocBaCay::onTouchEnded(Touch *touch, Event *unused_event)
{
	/*ayerDatCuocBaCay* currentTarget = (LayerDatCuocBaCay*)unused_event->getCurrentTarget();
	Size s = currentTarget->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	Vec2 locationInNode = currentTarget->convertToNodeSpace(touch->getLocation());
	if (!rect.containsPoint(locationInNode))
	{
	currentTarget->removeFromParentAndCleanup(true);
	}*/
}

void LayerDatCuocBaCay::onButtonClick(Ref* pSender)
{
	log("Button Clicked");
	//Send Request Bet Game
	if (this->gameId == kGameBaCay || this->gameId == kGameLieng){
		if (valueBet > userAmf) {
			this->showToast(dataManager.GetSysString(33));
			return;
		}
		else if (valueBet < minValue) {
			this->showToast(dataManager.GetSysString(117));
			return;
		}
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutInt(EXT_FIELD_BET_TYPE, GAME_LIENG_BET);
		params->PutInt(EXT_FIELD_GAME_BET_VALUE, valueBet);
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
		GameServer::getSingleton().Send(request);
		this->runAction(Sequence::create(CallFunc::create(this->listener),
			DelayTime::create(0.2), RemoveSelf::create(), NULL));
	}
	/*else if (this->gameId == kGameBaCayChuong){
		this->setVisible(false);
		}*/
}

void LayerDatCuocBaCay::showToast(string msg)
{
	Scene* currScene = Director::getInstance()->getRunningScene();
	if (currScene)
	{
		Chat* layerChat = Chat::create(msg, -1);
		currScene->addChild(layerChat, ZORDER_LIST::ZORDER_TOAST,9008);
	}
}

void LayerDatCuocBaCay::setPercentPoint(float dis)
{
	if (dis <= 0)
		return;
	/*float per_arr[3] = { 0.25, 0.5, 0.75 };
	float per25 = money_round(dis * per_arr[0]);
	float per50 = money_round(dis* per_arr[1]);
	float per75 = money_round(dis*per_arr[2]);
	if (per25 > this->minValue)
	lblPoint25->setString(mUtils::convertMoneyEx(per25));
	else
	lblPoint25->setString(mUtils::convertMoneyEx(per25 + this->minValue));
	if (per50 > this->minValue)
	lblPoint50->setString(mUtils::convertMoneyEx(per50));
	else
	lblPoint50->setString(mUtils::convertMoneyEx(per50 + this->minValue));
	if (per75 > this->minValue)
	lblPoint75->setString(mUtils::convertMoneyEx(per75));
	else
	lblPoint75->setString(mUtils::convertMoneyEx(per75 + this->minValue));*/
}

float LayerDatCuocBaCay::money_round(float num)
{
	float x = num;
	if (x >= 1000)
	{
		x /= 1000;
		return floor(x + 0.5) * 1000;
	}
	else{
		x /= 100;
		return floor(x + 0.5) * 100;
	}
}

void LayerDatCuocBaCay::setInfoBet(double minbet)
{
	if (minbet <= 0)
	{
		boost::shared_ptr<Room> lastJoinRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastJoinRoom != NULL)
		{
			boost::shared_ptr<RoomVariable> paramsptr = lastJoinRoom->GetVariable("params");
			if (paramsptr)
			{
				vector<string> arrInfo = mUtils::splitString(*paramsptr->GetStringValue(), '@');
				minValue = atol(arrInfo.at(0).c_str());
			}
		}
	}
	else{
		minValue = minbet;
	}
	this->valueBet = minValue;
	labelCurrBet->setString(mUtils::convertMoneyEx(minValue));
	//labelMinValue->setString(mUtils::convertMoneyEx(minValue));
	float distance = maxValue - minValue;
	setPercentPoint(distance);

}

void LayerDatCuocBaCay::setCallBack(const std::function<void()> &_listener)
{
	this->listener = _listener;
}

void LayerDatCuocBaCay::setInfoTest(double minbet, double maxbet)
{
	maxValue = maxbet;
	minValue = minbet;
	labelCurrBet->setString(mUtils::convertMoneyEx(minValue));
	labelMinValue->setString(mUtils::convertMoneyEx(minValue));
	float distance = maxValue - minValue;
	setPercentPoint(distance);
}

double LayerDatCuocBaCay::getValueBet()
{
	return this->valueBet;
}

Button* LayerDatCuocBaCay::getButtonConfirm()
{
	return this->btnConfirm;
}

void LayerDatCuocBaCay::resetSlider()
{
	this->slider->setPercent(0);
	this->labelCurrBet->setString(mUtils::convertMoneyEx(minValue));
	this->imgMoney->setPositionY(this->slider->getPositionY() - this->slider->getContentSize().width / 2);
	this->valueBet = minValue;
}
