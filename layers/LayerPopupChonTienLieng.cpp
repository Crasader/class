#include "../layers/LayerPopupChonTienLieng.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "cotuong/ServerMessageDef.h"
#include "../scenes/ScenePickRoom.h"
#include "../scenes/SceneMain.h"
LayerPopupChonTienLieng::LayerPopupChonTienLieng()
{
	GameServer::getSingleton().addListeners(this);
}


LayerPopupChonTienLieng::~LayerPopupChonTienLieng()
{
	GameServer::getSingleton().removeListeners(this);

}

bool LayerPopupChonTienLieng::init()
{
	if (!Layer::init())
	{
		return false;
	}
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupChonTienLieng.csb");
	auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlB->setPosition(Point::ZERO);
	pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	//rootNode->setPosition(visibleSize / 2);
	rootNode->setPosition(Point::ZERO);
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

	Sprite *iconMoney = static_cast<Sprite*>(rootNode->getChildByName("Sprite_2"));
	int roomType = SceneManager::getSingleton().getCurrRoomType();
	if (roomType == 1){ //1- hao 0- moc
		iconMoney->setTexture("moc3-icon-hao.png");
	}
	else{
		iconMoney->setTexture("moc3-icon-moc.png");
	}

    auto Text_2 = static_cast<Text*>(rootNode->getChildByName("Text_2"));
    Text_2->setString(dataManager.GetSysString(903));
    
    auto Text_13_0 = static_cast<Text*>(rootNode->getChildByName("Text_13_0"));
    Text_13_0->setString(dataManager.GetSysString(904));
    
    auto Text_13 = static_cast<Text*>(rootNode->getChildByName("Text_13"));
    Text_13->setString(dataManager.GetSysString(905));
    
    auto auto_charge_txt = static_cast<Text*>(rootNode->getChildByName("auto_charge_txt"));
    auto_charge_txt->setString(dataManager.GetSysString(682));
    
    auto Text_1_0_0_2_0 = static_cast<Text*>(rootNode->getChildByName("Text_1_0_0_2_0"));
    Text_1_0_0_2_0->setString(dataManager.GetSysString(906));
    
    
	Button* btnDongY = static_cast<Button*>(rootNode->getChildByName("btnOk"));
	if (btnDongY)
	{
		btnDongY->setPressedActionEnabled(true);
		btnDongY->addClickEventListener(CC_CALLBACK_1(LayerPopupChonTienLieng::onBtnOK, this));
        btnDongY->setTitleText(dataManager.GetSysString(754));

	}
	Button* btnHuyBo = static_cast<Button*>(rootNode->getChildByName("btnCancel"));
	if (btnHuyBo)
	{
		btnHuyBo->setPressedActionEnabled(true);
		btnHuyBo->addClickEventListener(CC_CALLBACK_1(LayerPopupChonTienLieng::onBtnCancel, this));
        btnHuyBo->setTitleText(dataManager.GetSysString(755));

	}

	Button* btnClose = static_cast<Button*>(rootNode->getChildByName("btnClose"));
	if (btnClose)
	{
		btnClose->setPressedActionEnabled(true);
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupChonTienLieng::onBtnCancel, this));
	}

	auto btnAdd = static_cast<Button*>(rootNode->getChildByName("btn_add"));
	if (btnAdd){
		btnAdd->setPressedActionEnabled(true);
		btnAdd->addClickEventListener(CC_CALLBACK_1(LayerPopupChonTienLieng::onBtnAddPercent, this));
	}

	auto btnSub = static_cast<Button*>(rootNode->getChildByName("btn_subt"));
	if (btnSub){
		btnSub->setPressedActionEnabled(true);
		btnSub->addClickEventListener(CC_CALLBACK_1(LayerPopupChonTienLieng::onBtnSubPercent, this));
	}

	auto btnAutoChargeMoney = static_cast<ImageView*>(rootNode->getChildByName("btnAutoCharge"));
	if (btnAutoChargeMoney){
		btnAutoChargeMoney->addClickEventListener(CC_CALLBACK_1(LayerPopupChonTienLieng::onBtnAutoCheckChargeMoney, this));
	}
	this->imgAutoChargeMoney = static_cast<ImageView*>(rootNode->getChildByName("imgCheckAutoCharge"));

	this->lblBacMangVao = static_cast<Text*>(rootNode->getChildByName("lblTienMangVao"));

	this->slider = static_cast<Slider*>(rootNode->getChildByName("Slider_2"));
	this->slider->setPercent(0.0);
	this->slider->addEventListener(CC_CALLBACK_2(LayerPopupChonTienLieng::sliderEvent, this));

	this->lblMinMoney = static_cast<Text*>(rootNode->getChildByName("lblTienMin"));
	this->lblMaxMoney = static_cast<Text*>(rootNode->getChildByName("lblTienMax"));
	this->lblBacConLai = static_cast<Text*>(rootNode->getChildByName("lblTienConlai"));
	this->lblBetRoom = static_cast<Text*>(rootNode->getChildByName("lblBetRoom"));

	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		if (myself != NULL){
			if (myself->GetVariable("amf") != NULL) {
				auto myMoney = *myself->GetVariable("amf")->GetDoubleValue();
				this->myMoney = myMoney;
			}
		}

	}
	else{
		if (myself != NULL){
			if (myself->GetVariable("amfs") != NULL) {
				auto myMoney = *myself->GetVariable("amfs")->GetDoubleValue();
				this->myMoney = myMoney;
			}
		}

	}
	this->onBtnAutoCheckChargeMoney(NULL);
	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return true;
	ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
	if (_scene)
	{
		_scene->hideWebView();
	}
	return true;
}
void LayerPopupChonTienLieng::setJoinType(int type){
	this->currentJoinType = type;
}

void LayerPopupChonTienLieng::onBtnOK(Ref* pSender){


	if (this->currentJoinType == 0){
		int roomType = SceneManager::getSingleton().getCurrRoomType();
		boost::shared_ptr<ISFSObject> param(new SFSObject());
		param->PutInt(EXT_FIELD_ROOM_TYPE, roomType);

		param->PutInt(AMOUNT_BET, this->betValue);
		//param->PutInt(AMOUNT_BET, this->bacMangVao);
		param->PutUtfString(GAME_ID, StringUtils::format("%d", kGameLieng));
		boost::shared_ptr<IRequest> request(new ExtensionRequest(REQUEST_AUTO_JOIN_ROOM, param));
		GameServer::getSingleton().Send(request);
        SceneMain::getSingleton().prepareToPickGame();

		this->removeFromParent();
	}
	else if (this->currentJoinType == 2){

		int roomType = SceneManager::getSingleton().getCurrRoomType();
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutInt(EXT_FIELD_ROOM_TYPE, roomType);
		params->PutInt("gmch", this->bacMangVao);
		params->PutInt("bych", this->isAutoChargeMoney);
		params->PutInt(EXT_FIELD_BRING_CHIP, 0);
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_SETUP_CHIP_REQ, params, lastRoom));
		GameServer::getSingleton().Send(req);
		/*this->removeFromParentAndCleanup(true);*/
		this->removeFromParent();
	}

}
void LayerPopupChonTienLieng::onBtnAutoCheckChargeMoney(Ref* pSender){

	if (this->isAutoChargeMoney == 0){
		this->imgAutoChargeMoney->setVisible(true);
		this->isAutoChargeMoney = 1;
	}
	else{
		this->imgAutoChargeMoney->setVisible(false);
		this->isAutoChargeMoney = 0;
	}
}

void LayerPopupChonTienLieng::onBtnCancel(Ref* pSender){
	this->removeFromParentAndCleanup(true);
}

void LayerPopupChonTienLieng::displayMoney(int percent){
	this->currBacMangVao = ((this->maxValue - this->minValue) / 100)*percent;

	this->bacMangVao = this->minValue + this->formatValue(this->currBacMangVao);

	if (percent == 100){
		this->bacMangVao = this->maxValue;
	}
	else if (percent == 0 || this->bacMangVao < this->minValue){
		this->bacMangVao = this->minValue;
	}

	this->lblBacMangVao->setString(mUtils::convertMoneyEx((long long)(this->bacMangVao)));
	this->lblBacConLai->setString(mUtils::convertMoneyEx(myMoney - bacMangVao));
}

void LayerPopupChonTienLieng::onBtnSubPercent(Ref* pSender){
	if (slider->getPercent() != 0){
		slider->setPercent(slider->getPercent() - 5);
		displayMoney(slider->getPercent());
	}
}

void LayerPopupChonTienLieng::onBtnAddPercent(Ref* pSender){
	if (slider->getPercent() < 100){
		slider->setPercent(slider->getPercent() + 5);
		displayMoney(slider->getPercent());
	}
}

void LayerPopupChonTienLieng::sliderEvent(Ref *pSender, Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(pSender);
		int percent = slider->getPercent();
		displayMoney(percent);
	}
}
void LayerPopupChonTienLieng::setMinMaxValue(float muccuoc, float muctoithieuvaoban){
	this->betValue = muccuoc;
	this->minValue = muctoithieuvaoban;
	this->maxValue = muccuoc*dataManager.getAppConfig().maxlieng;
	//xet truong hop so tien cua nguoi choi nho hon so tien max
	if (this->myMoney < this->maxValue)
		this->maxValue = this->myMoney;

	this->bacMangVao = this->maxValue;
	this->lblBacMangVao->setString(mUtils::convertMoneyEx(maxValue));
	this->lblMinMoney->setString(mUtils::convertMoneyEx(minValue));
	this->lblMaxMoney->setString(mUtils::convertMoneyEx(maxValue));
	this->slider->setPercent(100);
	this->sliderEvent(this->slider, Slider::EventType::ON_PERCENTAGE_CHANGED);
	this->lblBetRoom->setString(mUtils::convertMoneyEx(muccuoc));

	// 	this->lblBacMangVao->setString(mUtils::convertMoneyEx(minValue));
	// 	this->lblBacConLai->setString(mUtils::convertMoneyEx(myMoney - minValue));
}
float LayerPopupChonTienLieng::formatValue(float value){
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
void LayerPopupChonTienLieng::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	///loi nay hay bi nen bi unschedule luon

	if (SceneManager::getSingleton().getGameID() == kGameLieng){
		/*request server banr tin setup tien vao ban 'stchrq'
		 - 'gmch': so tien mang vao
		 - 'bych': co tu dong nap tien khi het tien hay khong?
		 */
		log("gmch %d", this->bacMangVao);
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutInt("gmch", this->bacMangVao);
		params->PutInt("bych", this->isAutoChargeMoney);
		params->PutInt(EXT_FIELD_BRING_CHIP, 0);
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_SETUP_CHIP_REQ, params, lastRoom));
		GameServer::getSingleton().Send(req);

	}
}
void LayerPopupChonTienLieng::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	//    public static final String EXT_EVENT_SETUP_CHIP_REQ					= "stchrq";
	//    public static final String EXT_EVENT_SETUP_CHIP_RES					= "stchrs";

	/*ban tin bao vao ban thanh cong hay khong?*/
	if (strcmp("stchrs", cmd->c_str()) == 0){
		log("hee");
	}
}
bool LayerPopupChonTienLieng::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupChonTienLieng::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupChonTienLieng::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
	//
	//    GameServer::getSingleton().addListeners(this);
}

void LayerPopupChonTienLieng::onExit()
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


