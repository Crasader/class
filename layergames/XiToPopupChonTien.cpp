//
//  XiToPopupChonTien.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/27/16.
//
//

#include "XiToPopupChonTien.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"
#include "../data_manager.h"
#include "../layergames/_Chat_.h"
#include "cotuong/ServerMessageDef.h"
#include "../scenes/ScenePickRoom.h"
#include "../scenes/SceneMain.h"
XiToPopupChonTien::XiToPopupChonTien()
{
	GameServer::getSingleton().addListeners(this);
}


XiToPopupChonTien::~XiToPopupChonTien()
{
	GameServer::getSingleton().removeListeners(this);

}

bool XiToPopupChonTien::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupChonTienXiTo.csb");
	auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    
    //Text_2
    
	pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	Size sizeAdd;
//	if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
		sizeAdd = ChanUtils::getSizePos();

//    }
//    else{
//        sizeAdd = Size(0, 0);
//    }
	Size sizeAdd2 = Size(0,0);
	rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);

	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
	//pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));

    auto Text_2 = static_cast<Text*>(pnlBg->getChildByName("Text_2"));
    Text_2->setString(dataManager.GetSysString(903));
    
	Sprite *iconMoney = static_cast<Sprite*>(pnlBg->getChildByName("Sprite_6"));
	int roomType = SceneManager::getSingleton().getCurrRoomType();
	if (roomType == 1){ //1- hao 0- moc
		iconMoney->setTexture("moc3-icon-hao.png");
	}
	else{
		iconMoney->setTexture("moc3-icon-moc.png");

	}

	Button* btnDongY = static_cast<Button*>(pnlBg->getChildByName("btnDongY"));
	if (btnDongY)
	{
		btnDongY->addClickEventListener(CC_CALLBACK_1(XiToPopupChonTien::onBtnDongY, this));
        btnDongY->setTitleText(dataManager.GetSysString(754));
	}
	Button* btnHuyBo = static_cast<Button*>(pnlBg->getChildByName("btnHuyBo"));
	if (btnHuyBo)
	{
		btnHuyBo->addClickEventListener(CC_CALLBACK_1(XiToPopupChonTien::btnHuyBo, this));
        btnHuyBo->setTitleText(dataManager.GetSysString(755));
	}

	auto btnAdd = static_cast<ImageView*>(pnlBg->getChildByName("Image_4_0"));
	if (btnAdd){
		btnAdd->setTouchEnabled(true);
		btnAdd->addClickEventListener(CC_CALLBACK_1(XiToPopupChonTien::onBtnAddPercent, this));
	}

	auto btnSub = static_cast<ImageView*>(pnlBg->getChildByName("Image_4"));
	if (btnSub){
		btnSub->setTouchEnabled(true);
		btnSub->addClickEventListener(CC_CALLBACK_1(XiToPopupChonTien::onBtnSubPercent, this));
	}


	Button* btnMangVaoKhiHetBac = static_cast<Button*>(pnlBg->getChildByName("btnMangVaoKhiHetBac"));
	if (btnMangVaoKhiHetBac)
	{
		btnMangVaoKhiHetBac->addClickEventListener(CC_CALLBACK_1(XiToPopupChonTien::onBtnMangVaoKhiHetBac, this));
	}

	Button* btnBoSungBac = static_cast<Button*>(pnlBg->getChildByName("btnBoSungBac"));
	if (btnBoSungBac)
	{
		btnBoSungBac->addClickEventListener(CC_CALLBACK_1(XiToPopupChonTien::onBtnBoSungBac, this));
	}

	imgAutoBoSungKhiHetBac = static_cast<ImageView*>(pnlBg->getChildByName("imgChecked"));
	imgAutoBoSungSauMoiVan = static_cast<ImageView*>(pnlBg->getChildByName("imgChecked1"));


	this->txtBacMangVao = static_cast<Text*>(pnlBg->getChildByName("txtMoney"));

	slider = static_cast<Slider*>(pnlBg->getChildByName("sliderMoney"));
	slider->addEventListener(CC_CALLBACK_2(XiToPopupChonTien::sliderEvent, this));
	slider->setPercent(100);

	txtMinMoney = static_cast<Text*>(pnlBg->getChildByName("txtMinMoney"));
	txtMaxMoney = static_cast<Text*>(pnlBg->getChildByName("txtMaxMoney"));

	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();

	Text* Text_9 = (Text*)pnlBg->getChildByName("Text_9");
	Text* Text_9_0 = (Text*)pnlBg->getChildByName("Text_9_0");

	Text_9->setString(dataManager.GetSysString(682));
	Text_9_0->setString(dataManager.GetSysString(683));

	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		if (mySelf != NULL){
			if (mySelf->GetVariable("amf") != NULL) {
				auto myMoney = *mySelf->GetVariable("amf")->GetDoubleValue();
				this->myMoney = myMoney;
			}
		}
	}
	else{
		if (mySelf != NULL){
			if (mySelf->GetVariable("amfs") != NULL) {
				auto myMoney = *mySelf->GetVariable("amfs")->GetDoubleValue();
				this->myMoney = myMoney;
			}
		}
	}

	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return true;
	ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
	if (_scene)
	{
		_scene->hideWebView();
	}
	return true;
}
void XiToPopupChonTien::onBtnSubPercent(Ref* pSender){
	if (slider->getPercent() != 0){
		slider->setPercent(slider->getPercent() - 5);
		displayMoney(slider->getPercent());
	}
}
void XiToPopupChonTien::displayMoney(int percent){
	this->currBacMangVao = ((this->maxValue - this->minValue) / 100)*percent;

	this->bacMangVao = this->minValue + this->formatValue(this->currBacMangVao);

	if (percent == 100){
		this->bacMangVao = this->maxValue;
	}
	else if (percent == 0 || this->bacMangVao < this->minValue){
		this->bacMangVao = this->minValue;
	}

	this->txtBacMangVao->setString(mUtils::convertMoneyEx((long long)(this->bacMangVao)));
}
void XiToPopupChonTien::onBtnAddPercent(Ref* pSender){
	if (slider->getPercent() < 100){
		slider->setPercent(slider->getPercent() + 5);
		displayMoney(slider->getPercent());
	}
}
void XiToPopupChonTien::setJoinType(int type){
	this->currentJoinType = type;
}
void XiToPopupChonTien::onBtnDongY(Ref* pSender){
	if (this->currentJoinType == 0){
		int roomType = SceneManager::getSingleton().getCurrRoomType();
		boost::shared_ptr<ISFSObject> param(new SFSObject());
		param->PutInt(EXT_FIELD_ROOM_TYPE, roomType);


		param->PutInt(AMOUNT_BET, this->betValue);
        param->PutUtfString(GAME_ID, StringUtils::format("%d", SceneManager::getSingleton().getGameID()));
		boost::shared_ptr<IRequest> request(new ExtensionRequest(REQUEST_AUTO_JOIN_ROOM, param));
		GameServer::getSingleton().Send(request);
        SceneMain::getSingleton().prepareToPickGame();

//        this->removeFromParent();
	}
	else if (this->currentJoinType == 2){
        //SceneMain::getSingleton().prepareToPickGame();

		int roomType = SceneManager::getSingleton().getCurrRoomType();
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutInt(EXT_FIELD_ROOM_TYPE, roomType);
		params->PutInt("gmch", this->bacMangVao);
		params->PutInt("bych", this->isAutoBoSungBacKhiHet);
		params->PutInt("brch", this->isAutoBoSungBacSauMoiVan);
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> req(new ExtensionRequest("stchrq", params, lastRoom));
		GameServer::getSingleton().Send(req);
		auto currScene = Director::getInstance()->getRunningScene();
		if (currScene != NULL)
		{
			Chat* toast = Chat::create(dataManager.GetSysString(424), -1);
			currScene->addChild(toast,2000,9012);
		}
		this->removeFromParentAndCleanup(true);
	}

}
void XiToPopupChonTien::onBtnMangVaoKhiHetBac(Ref* pSender){
	if (this->isAutoBoSungBacKhiHet == 1){
		this->isAutoBoSungBacKhiHet = 0;
		this->imgAutoBoSungKhiHetBac->setVisible(false);
	}
	else{
		this->isAutoBoSungBacKhiHet = 1;
		this->imgAutoBoSungKhiHetBac->setVisible(true);
	}
}
void XiToPopupChonTien::onBtnBoSungBac(Ref* pSender){
	if (this->isAutoBoSungBacSauMoiVan == 1){
		this->isAutoBoSungBacSauMoiVan = 0;
		this->imgAutoBoSungSauMoiVan->setVisible(false);
	}
	else{
		this->isAutoBoSungBacSauMoiVan = 1;
		this->imgAutoBoSungSauMoiVan->setVisible(true);

	}
}
void XiToPopupChonTien::btnHuyBo(Ref* pSender){
	this->removeFromParentAndCleanup(true);
}
void XiToPopupChonTien::sliderEvent(Ref *pSender, Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(pSender);
		int percent = slider->getPercent();
		//this->labelMinValue->setVisible(true);
		//this->labelMaxValue->setVisible(true);
		this->currBacMangVao = ((this->maxValue - this->minValue) / 100)*percent;

		this->bacMangVao = this->minValue + this->formatValue(this->currBacMangVao);

		if (percent == 100){
			this->bacMangVao = this->maxValue;
		}
		else if (percent == 0 || this->bacMangVao < this->minValue){
			this->bacMangVao = this->minValue;
		}

		this->txtBacMangVao->setString(mUtils::convertMoneyEx((long long)(this->bacMangVao)));

		//auto pos = (slider->getContentSize().width/100)*percent + (slider->getPositionX()-slider->getContentSize().width/2-25);
		//this->labelCurrBet->setPositionX(pos);
		//this->arrSpr->setPositionX((slider->getContentSize().width/100)*percent + (slider->getPositionX()-slider->getContentSize().width/2));

	}
}
void XiToPopupChonTien::setMinMaxValue(float muccuoc, float muctoithieuvaoban){
	if (this->currentJoinType == 0){
		this->betValue = muccuoc;
		this->minValue = muctoithieuvaoban;
		this->maxValue = muccuoc*dataManager.getAppConfig().maxxito;
		if (this->myMoney < this->maxValue)
			this->maxValue = this->myMoney;
		this->bacMangVao = this->maxValue;
		this->txtBacMangVao->setString(mUtils::convertMoneyEx(maxValue));
		this->txtMinMoney->setString(mUtils::convertMoneyEx(minValue));
		this->txtMaxMoney->setString(mUtils::convertMoneyEx(maxValue));
	}
	else if (this->currentJoinType == 2){
		this->betValue = muccuoc;
		this->minValue = muctoithieuvaoban;
		this->maxValue = muccuoc*dataManager.getAppConfig().maxxito;
		if (this->myMoney < this->maxValue)
			this->maxValue = this->myMoney;
		this->bacMangVao = this->maxValue;
		this->txtBacMangVao->setString(mUtils::convertMoneyEx(maxValue));
		this->txtMinMoney->setString(mUtils::convertMoneyEx(minValue));
		this->txtMaxMoney->setString(mUtils::convertMoneyEx(maxValue));

	}
}
float XiToPopupChonTien::formatValue(float value){
	if (value >= 1 && value < 1000)
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
void XiToPopupChonTien::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	///loi nay hay bi nen bi unschedule luon

	if (SceneManager::getSingleton().getGameID() == kGameXiTo){
		SceneManager::getSingleton().showLoading();
		//Gui thong tin muc cuoc va tien
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutInt("gmch", this->bacMangVao);
		params->PutInt("bych", this->isAutoBoSungBacKhiHet);
		params->PutInt("brch", this->isAutoBoSungBacSauMoiVan);
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_SETUP_CHIP_REQ, params, lastRoom));
		GameServer::getSingleton().Send(req);
		//        boost::shared_ptr<ISFSObject> params(new SFSObject());
		//        boost::shared_ptr<IRequest> request(new ExtensionRequest("rldt", params));
		//        GameServer::getSingleton().Send(request);
		//
		SceneManager::getSingleton().gotoGame(kGameXiTo);
	}
    if (SceneManager::getSingleton().getGameID() == kGamePoker){
        SceneManager::getSingleton().showLoading();
        //Gui thong tin muc cuoc va tien
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt("gmch", this->bacMangVao);
        params->PutInt("bych", this->isAutoBoSungBacKhiHet);
        params->PutInt("brch", this->isAutoBoSungBacSauMoiVan);
        boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
        if (lastRoom == NULL)
            return;
        boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_SETUP_CHIP_REQ, params, lastRoom));
        GameServer::getSingleton().Send(req);
        //        boost::shared_ptr<ISFSObject> params(new SFSObject());
        //        boost::shared_ptr<IRequest> request(new ExtensionRequest("rldt", params));
        //        GameServer::getSingleton().Send(request);
        //
        SceneManager::getSingleton().gotoGame(kGamePoker);
    }
	//
}
void XiToPopupChonTien::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	//    public static final String EXT_EVENT_SETUP_CHIP_REQ					= "stchrq";
	//    public static final String EXT_EVENT_SETUP_CHIP_RES					= "stchrs";

	if (strcmp("stchrs", cmd->c_str()) == 0){
	}
}
bool XiToPopupChonTien::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void XiToPopupChonTien::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(XiToPopupChonTien::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
	//
	//    GameServer::getSingleton().addListeners(this);
}

void XiToPopupChonTien::onExit()
{
	Layer::onExit();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	if (!SceneManager::getSingleton().getIsShowTX() && !SceneManager::getSingleton().getIsShowXD() && !SceneManager::getSingleton().getIsShowPK() && !SceneManager::getSingleton().getIsShowVQ() && !SceneManager::getSingleton().getIsShowCT()){

		auto _currScene = Director::getInstance()->getRunningScene();
		if (!_currScene) return;
		ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
		if (_scene)
		{
			_scene->showWebView();
		}
	}
	//    GameServer::getSingleton().removeListeners(this);
	//    Layer::onExit();
}


