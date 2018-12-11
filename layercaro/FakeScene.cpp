//
//  TicTacToeScene.cpp
//  TicTacToeOSX
//
//  Created by Ricard Pérez del Campo on 24/05/13.
//  Copyright (c) 2013 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "FakeScene.h"
#include "AllData.h"
#include "layers/LayerMainContent.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/UIHelper.h"
#include "Common.h"
#include "layers/LayerPopupThongBao.h"
#include "Requests/LoginRequest.h"
#include "Util/PasswordUtil.h"
#include "SceneManager.h"
#include "layergames/PhomMessDef.h"
#include "Requests/ExtensionRequest.h"

USING_NS_CC;

template<> FakeScene* SingLeton<FakeScene>::mSingleton = 0;

FakeScene* FakeScene::getSingletonPtr(void)
{
	return mSingleton;
}

FakeScene& FakeScene::getSingleton(void)
{
	assert(mSingleton);
	return (*mSingleton);
}
FakeScene::FakeScene()
{
	//GameServer::getSingleton().addListeners(this);
}

FakeScene::~FakeScene()
{
	//GameServer::getSingleton().removeListeners(this);
}
Scene* FakeScene::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	FakeScene *layer = FakeScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool FakeScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	this->setIsShow(false);

	
	this->createGuideLayer();
	/*
	SceneManager::getSingleton().showLoading(30);

	SceneManager::getSingleton().connect2Server(2);*/

	return true;
}

void FakeScene::onButtonClick(Ref* pSender)
{
	int tag = ((Button*)pSender)->getTag();
	string urlFile = "guidehtml/";
	switch (tag)
	{
	case 1:
		urlFile.append("hd_bacaychuong.html");
		break;
	case 2:
		urlFile.append("hd_chan.html");
		break;
	case 3:
		urlFile.append("hd_tienlenmb.html");
		break;
	case 4:
		urlFile.append("hd_phom.html");
		break;
	default:
		break;
	}
	LayerPopupWebView* popup = LayerPopupWebView::create();
	popup->setWebFile(urlFile);
	this->addChild(popup);
}

void FakeScene::loginToBotZone()
{
	boost::shared_ptr<string> _pass = PasswordUtil::MD5Password("654321a");
	_pass = PasswordUtil::MD5Password(*_pass);
	boost::shared_ptr<ISFSObject> parameters(new SFSObject());

	parameters->PutUtfString("LT", "normal");
	parameters->PutUtfString("LSK", "");
	parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
	parameters->PutUtfString("accParId", "");// LOGIN_PARTNER_ID = "accParId";
	parameters->PutInt("srcref", 0);//SRC_REF = "srcref";
	parameters->PutInt("osid", 1);// Operatorsystems
	parameters->PutUtfString("devid", "");// decvice id
	parameters->PutUtfString("aIp", "");//ip
	parameters->PutInt("srcref", 0);

	string _username = "tester16";

	boost::shared_ptr<IRequest> request(new LoginRequest(_username, *_pass, "Bot", parameters));
	GameServer::getSingleton().SendLogin(request);

	// 	boost::shared_ptr<ISFSObject> parameters(new SFSObject());
	// 	int  osid = 2; //ios
	// 	std::string devID = "1111111111sssaa";
	// 	/*bool isConnectting = false;*/
	// #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// 	osid = 1;
	// 	devID = NativeHelper::getDeviceId();
	// 	/*isConnectting = NativeHelper::hasConnection();*/
	// 	//devID = NativeHelper::getDeviceID();
	// 
	// #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// 	osid = 2;
	// 	devID = IOS::getDeviceID();
	// #endif
	// 	parameters->PutUtfString("LT", "sp");
	// 	parameters->PutUtfString("aI", "");
	// 	parameters->PutUtfString("aN", "");
	// 	parameters->PutUtfString("imc", "mobile");
	// 	parameters->PutUtfString("aM", "");
	// 	parameters->PutUtfString("LSK", "");
	// 	parameters->PutUtfString("accZoneId", "1");// LOGIN_PARTNER_ID = "accZoneId";
	// 	parameters->PutUtfString("accParId", "");// LOGIN_PARTNER_ID = "accParId";
	// 	parameters->PutInt("srcref", 0);//SRC_REF = "srcref";
	// 	parameters->PutInt("osid", 1);// Operatorsystems
	// 	parameters->PutUtfString("devid", devID);// decvice id
	// 	parameters->PutUtfString("aIp", "");//ip
	// 	parameters->PutInt("srcref", 0);
	// 	string _ai = mUtils::replaceAll(devID, "-", "");
	// 
	// 	boost::shared_ptr<IRequest> req_login(new LoginRequest(_ai, "", "GameZone1", parameters));
	// 	GameServer::getSingleton().SendLogin(req_login);
}

void FakeScene::OnSmartFoxLogin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//send request join room
	//set variables to hide button in games
	SceneManager::getSingleton().setGameID(kGameBaCay);

	boost::shared_ptr<ISFSObject> param(new SFSObject());
	param->PutInt(AMOUNT_BET, 5);
	param->PutUtfString(GAME_ID, StringUtils::format("%d", kGameBaCay));
	boost::shared_ptr<IRequest> request(new ExtensionRequest(REQUEST_AUTO_JOIN_ROOM, param));
	GameServer::getSingleton().Send(request);


}

void FakeScene::OnSmartFoxLoginError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	log("OnSmartFoxLoginError");
	this->createGuideLayer();
}

void FakeScene::OnSmartFoxConnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSuccess = (*ptrEventParams)["success"];
	boost::shared_ptr<bool> ptrValue = (boost::static_pointer_cast<bool>)(ptrEventParamValueSuccess);
	if (*ptrValue != true)//connected failed
	{
		this->createGuideLayer();
		return;
	}
	this->loginToBotZone();
}

void FakeScene::createGuideLayer()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Node* rootNode = CSLoader::getInstance()->createNode("LayerFake.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());
	// 	ImageView* Image_1 = (ImageView*)rootNode->getChildByName("Image_1");
	// 	Image_1->setPositionY(visibleSize.height - Image_1->getContentSize().height / 2);
	// 	Sprite* Sprite_11 = (Sprite*)rootNode->getChildByName("Sprite_11");
	// 	Sprite_11->setPositionY(visibleSize.height - Sprite_11->getContentSize().height / 2);
	Sprite *Sprite_2 = (Sprite *)rootNode->getChildByName("Sprite_2");
	if (Sprite_2)
	{
		Sprite_2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		Sprite_2->setScaleY(visibleSize.height / Sprite_2->getContentSize().height);
	}
	Button* btnBacay = (Button *)rootNode->getChildByName("btnBacay");
	btnBacay->setTag(1);
	Button* btnChan = (Button *)rootNode->getChildByName("btnChan");
	btnChan->setTag(2);
	Button* btnSam = (Button *)rootNode->getChildByName("btnSam");
	btnSam->setTag(3);
	Button* btnPhom = (Button *)rootNode->getChildByName("btnPhom");
	btnPhom->setTag(4);

	btnBacay->addClickEventListener(CC_CALLBACK_1(FakeScene::onButtonClick, this));
	btnChan->addClickEventListener(CC_CALLBACK_1(FakeScene::onButtonClick, this));
	btnSam->addClickEventListener(CC_CALLBACK_1(FakeScene::onButtonClick, this));
	btnPhom->addClickEventListener(CC_CALLBACK_1(FakeScene::onButtonClick, this));
}

void FakeScene::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{

}

void FakeScene::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	log("OnSmartFoxLogin OK - fly to the ba cay game");
	SceneManager::getSingleton().gotoGame(kGameBaCay);
}

void FakeScene::OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	this->createGuideLayer();
}


