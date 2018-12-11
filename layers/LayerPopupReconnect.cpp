#include "LayerPopupReconnect.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Requests/ExtensionRequest.h"
#include "../SceneManager.h"
#include "../scenes/SceneLogin.h"
#include "layergames/ChanUtils.h"
#include "Requests/LogoutRequest.h"


LayerPopupReconnect::LayerPopupReconnect()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupReconnect::~LayerPopupReconnect()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupReconnect::scheduleTimeReconnect),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupReconnect::scheduleCheckConnectting),this);
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupReconnect::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(false);
	this->setTag(POPUP_THONGBAO);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupReconnect.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));

	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(pnlBg->getContentSize());

	Size sizeAdd;
	sizeAdd = ChanUtils::getSizePos();
	// 	if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
	// 		//layerTCC->setPositionY(layerTCC->getPositionY() - sizeAdd.height);
	// 	}
	// 	else{
	// 		rootNode->setPositionY(rootNode->getPositionY() + sizeAdd.height);
	// 	}
    
	lblHeader = (Text*)rootNode->getChildByName("lblHeader");

	lblContent1 = (Text*)rootNode->getChildByName("lblContent1");
	lblContent2 = (Text*)rootNode->getChildByName("lblContent2");
	lblContent3 = (Text*)rootNode->getChildByName("lblContent3");
    
    lblHeader->setString(dataManager.GetSysString(953));
    lblContent1->setString(dataManager.GetSysString(954));
    lblContent2->setString(dataManager.GetSysString(955));
    lblContent3->setString(dataManager.GetSysString(956));

    
	lblTime = (Text*)rootNode->getChildByName("lblTime");
	lblCount = (Text*)rootNode->getChildByName("lblCount");
	btnOK = (Button*)rootNode->getChildByName("btnOK");
    btnOK->setTitleText(dataManager.GetSysString(889));
	btnClose = (Button*)rootNode->getChildByName("btnClose");

	btnCancel = (Button*)rootNode->getChildByName("btnCancel");
    btnCancel->setTitleText(dataManager.GetSysString(890));

	loadingBar = (LoadingBar*)rootNode->getChildByName("loadingBar");
    
	bgLoading = (Sprite*)rootNode->getChildByName("bgLoading");

	startPos = btnCancel->getPosition();
	firstStr = lblContent1->getString();

	startPosX = ((timeAutoReconnBuff / timeAutoReconn) / 100) * 520;
	lightLoading = Sprite::create("light-loading.png");
	lightLoading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lightLoading->setPosition(Vec2(startPosX, 10));
	loadingBar->addChild(lightLoading);

	

	lblTime->setString(StringUtils::format("%d s", (int)round(timeAutoReconn - timeAutoReconnBuff)));
	btnClose->setVisible(false);
	btnOK->addClickEventListener(CC_CALLBACK_1(LayerPopupReconnect::onButtonOK, this));
	btnCancel->addClickEventListener(CC_CALLBACK_1(LayerPopupReconnect::onButtonCancel, this));

	return true;
}
bool LayerPopupReconnect::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupReconnect::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupReconnect::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerPopupReconnect::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerPopupReconnect::scheduleTimeReconnect(float dt)
{
	if (timeAutoReconnBuff >= timeAutoReconn)
	{
		//time out, start auto reconnect
		this->onButtonOK(NULL);
        this->removeFromParent();
        return;
	}
	lblTime->setString(StringUtils::format("%d s", (int)round(timeAutoReconn - timeAutoReconnBuff)));
	float percent = (timeAutoReconnBuff / timeAutoReconn) * 100;
	log("PERCENT = %.2f", percent);
	timeAutoReconnBuff++;
	loadingBar->setPercent(percent);
	lightLoading->setPositionX((percent / 100) * 520);
}

void LayerPopupReconnect::showReconnect()
{
	timeAutoReconnBuff = 1;
	timePrepare = 0;
	if (countReconn <= maxCountReconn)
	{
		hideAllLabel(false);
		countReconn++;
		lblContent1->runAction(FadeIn::create(0.1));
		lblContent1->setVisible(true);
		lblCount->setString(StringUtils::format("%d/%d", countReconn, maxCountReconn));
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupReconnect::scheduleCheckConnectting),this);
		Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupReconnect::scheduleTimeReconnect),this,1, false);
	}
}

void LayerPopupReconnect::onButtonClose(Ref* pSender)
{
	//khong reconnect, tat app - hoac login ra man hinh cho
	if (GameServer::getSingleton().getSmartFox()->MySelf() != NULL)
	{
		this->removeFromParentAndCleanup(true);
	}
	else{
		UserDefault *def = UserDefault::getInstance();
		def->setBoolForKey("UCK", true);
		def->flush();
		boost::shared_ptr<IRequest> request(new LogoutRequest());
		GameServer::getSingleton().Send(request);
		//
		SceneManager::getSingleton().isLogout = true;
		SceneManager::getSingleton().rcForLogin = false;
		SceneManager::getSingleton().gotoLogin();
	}
}

void LayerPopupReconnect::onButtonCancel(Ref* pSender)
{
	//khong reconnect, tat app - hoac login ra man hinh cho
	UserDefault *def = UserDefault::getInstance();
	def->setBoolForKey("UCK", true);
	def->flush();
	boost::shared_ptr<IRequest> request(new LogoutRequest());
	GameServer::getSingleton().Send(request);
	//
	SceneManager::getSingleton().isLogout = true;
	SceneManager::getSingleton().rcForLogin = false;
	SceneManager::getSingleton().gotoLogin();
}

void LayerPopupReconnect::onButtonOK(Ref* pSender)
{
	log("now try to reconnect");
	timeAutoReconnBuff = timeAutoReconn;
	lblTime->setString(StringUtils::format("%d s", (int)timeAutoReconn));
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupReconnect::scheduleTimeReconnect),this);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupReconnect::scheduleCheckConnectting),this,1, false);
	GameServer::getSingleton().getSmartFox()->Disconnect();
    SceneManager::getSingleton().connect2Server(0);

    SceneManager::getSingleton().isLogout = true;
    SceneManager::getSingleton().rcForLogin = true;
	SceneManager::getSingleton().gotoLogin();
}

void LayerPopupReconnect::hideAllLabel(bool isHide)
{
	lblContent1->stopAllActions();
	lblContent2->setVisible(!isHide);
	lblContent3->setVisible(!isHide);
	lblCount->setVisible(!isHide);
	lblTime->setVisible(!isHide);
	loadingBar->stopAllActions();
	loadingBar->setVisible(!isHide);
	lightLoading->setVisible(!isHide);
	bgLoading->setVisible(!isHide);
	btnOK->setVisible(!isHide);
	if (isHide)
	{
		loadingBar->setPercent(0);
		btnCancel->setPosition(Vec2(lblContent1->getPositionX(), startPos.y));
		lightLoading->setPositionX(startPosX);
	}
	else
	{
		btnCancel->setPosition(startPos);
		lblContent1->setString(firstStr);
	}
}

void LayerPopupReconnect::showConnecting()
{
	hideAllLabel(true);
	lblContent1->runAction(FadeIn::create(0.1));
	lblContent1->setVisible(true);
	lblContent1->setString(StringUtils::format("%s %s %d s", dataManager.GetSysString(571).c_str(),dataManager.GetSysString(748).c_str(), (8 - timePrepare)));
	lblContent1->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.2), FadeIn::create(0.2), NULL)));
}

void LayerPopupReconnect::showCannotConnect()
{
	hideAllLabel(true);
	lblContent1->runAction(FadeIn::create(0.1));
	lblContent1->setVisible(true);
	lblHeader->setString(dataManager.GetSysString(264));
	lblContent1->setString(dataManager.GetSysString(265));
}

void LayerPopupReconnect::scheduleCheckConnectting(float dt)
{
	// 	if (timeAutoReconnBuff < timeAutoReconn)
	// 	{
	// 		//chua can lam gi ca
	// 	}
	// 	else{
	// 		if (GameServer::getSingleton().getSmartFox()->IsConnecting())//dang ket noi den he thong
	// 		{
	// 			if (countReconn == 1)//reconnect cho lan 1
	// 			{
	// 				timePrepare++;
	// 				if (timePrepare >= 8) //qua 8s,show popup lan 2
	// 				{
	// 					this->startShowConnect();
	// 				}
	// 				else{
	// 					//show dang ket noi
	// 					this->showIsConnecting();
	// 				}
	// 			}
	// 			else{
	// 				timePrepare++;
	// 				if (timePrepare >= 8)
	// 				{
	// 					showIsCannotConnect();
	// 				}
	// 				else{
	// 					//show dang ket noi
	// 					this->showIsConnecting();
	// 				}
	// 			}
	// 		}
	// 		else{
	if (GameServer::getSingleton().getSmartFox()->IsConnected())
	{
		if (GameServer::getSingleton().getSmartFox()->MySelf() != NULL)
			this->removeFromParentAndCleanup(true);
		else{
			if (countReconn == 1)//reconnect cho lan 1
			{
				timePrepare++;
				if (timePrepare >= 8) //qua 8s,show popup lan 2
				{
					this->showReconnect();
				}
				else{
					//show dang ket noi
					this->showConnecting();
				}
			}
			else{
				timePrepare++;
				if (timePrepare >= 8)
				{
					showCannotConnect();
				}
				else{
					//show dang ket noi
					this->showConnecting();
				}
			}
		}
	}
	else{
		timePrepare++;
		if (timePrepare >= 8)
		{
			showCannotConnect();
			this->onButtonCancel(NULL);
		}
		else{
			//show dang ket noi
			this->showConnecting();
		}
	}
	//}
	// 		if (GameServer::getSingleton().getSmartFox()->IsConnected())//reconnect thanh cong
	// 		{
	// 			this->removeFromParentAndCleanup(true);
	// 		}
	// 		else{
	// 			SceneManager::getSingleton().connect2Server(0);
	// 		}
	//}
}

void LayerPopupReconnect::OnSmartFoxConnectionResume(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	log("OnSmartFoxConnectionResume OK");
	this->removeFromParentAndCleanup(true);
}

void LayerPopupReconnect::OnSmartFoxHandShake(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	log("OnSmartFoxHandShake OK");
	this->removeFromParentAndCleanup(true);
}



