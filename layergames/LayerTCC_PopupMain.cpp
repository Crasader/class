#include "LayerTCC_PopupMain.h"
#include "PhomMessDef.h"
#include "TCCMsgDef.h"
#include "_Chat_.h"
#include "LayerTCC_PlateShakeAnim.h"
#include "LayerTCC_PopupMoQuy.h"
#include "LayerTCC_PopupRank.h"
#include "LayerTCC_PopupHistory.h"
#include "../layers/LayerPopupThongBao.h"
#include "scenes/SceneMain.h"
#include "ChanUtils.h"
LayerTCC_PopupMain::LayerTCC_PopupMain()
{
	pnlMain = NULL;
	layerDiceHis = NULL;
	layer1DiceHis = NULL;
	layerBetDices = NULL;
	lblMatchId = NULL;
	lblChestMoney = NULL;
	pnlDiceHis = NULL;
	pnl1DiceHis = NULL;
	pnlPlate = NULL;
	imgBowl = NULL;
	layerTimer = NULL;
	line_25 = NULL;
	mThreadQueueMsg = NULL;
	isGameStarted = false;
	timeMoQuy = -10; //gia tri default
	GameServer::getSingleton().addListeners(this);
}

LayerTCC_PopupMain::~LayerTCC_PopupMain()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_PopupMain::scheduleRequestQuyThuong),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_PopupMain::scheduleTimeMoQuy),this);
	if (mThreadQueueMsg != NULL) delete mThreadQueueMsg;
	GameServer::getSingleton().removeListeners(this);
}

bool LayerTCC_PopupMain::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_PopupMain.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	//this->setContentSize(root->getContentSize());

	Size visibleSize = Director::getInstance()->getVisibleSize();
	//root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//root->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//Size sizeAdd = ChanUtils::getSizePos();
//	Layout* pnlBG = (Layout*)root->getChildByName("pnlBG");
//	pnlBG->setScale(visibleSize.width / pnlBG->getContentSize().width, visibleSize.height / pnlBG->getContentSize().height);
//	//pnlBG->setPositionY(pnlBG->getPositionY() - sizeAdd.height);
//    pnlBG->setSwallowTouches(false);

	myName = SceneManager::getSingleton().getMyName();
	pnlMain = (Layout *)root->getChildByName("pnlMain");
	this->btnGold = static_cast<Button*>(pnlMain->getChildByName("btnGold"));
	if (this->btnGold){
		this->btnGold->setPressedActionEnabled(true);
		this->btnGold->addClickEventListener(CC_CALLBACK_1(LayerTCC_PopupMain::onBtnGold, this));
	}
	this->btnSilver = static_cast<Button*>(pnlMain->getChildByName("btnSilver"));
	if (this->btnSilver){
        this->btnSilver->setVisible(false);
		this->btnSilver->setPressedActionEnabled(true);
		this->btnSilver->addClickEventListener(CC_CALLBACK_1(LayerTCC_PopupMain::onBtnSilver, this));
	}

	if (pnlMain)
	{
		this->createLayerDiceHis();
		this->createLayerBetDice();
		this->createListResDices();
		this->createPlate();
		this->createUI();
		this->createLayerTimer();

		this->layerChat = LayerChatMiniGame::create();
        this->layerChat->setPosition(Vec2(-450,430));

		this->pnlMain->addChild(this->layerChat);
	}

	Button* btnChat = (Button*)pnlMain->getChildByName("btnChat");
	btnChat->addClickEventListener(CC_CALLBACK_1(LayerTCC_PopupMain::onButtonChat, this));

	createSmartFoxEvent();
	//start schedule
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerTCC_PopupMain::scheduleRequestQuyThuong),this,1, false);
    //this->setScale(0.8);
	return true;
}

void LayerTCC_PopupMain::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	pObjs->PutUtfString("_cmd_", *cmd);
	//only add to queues
	mThreadQueueMsg->addQueue(pObjs);
}
void LayerTCC_PopupMain::onBtnGold(Ref *sender){
	SceneManager::getSingleton().showLoading();

//	this->btnGold->setEnabled(false);
//	this->btnGold->setBright(false);
//
//	this->btnSilver->setEnabled(true);
//	this->btnSilver->setBright(true);

    this->btnSilver->setVisible(true);
    this->btnGold->setVisible(false);

    lblChestMoney->setString("---");

    layerBetDices->loadAmfUser();
    
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> req(new ExtensionRequest("tccl", params));
	GameServer::getSingleton().Send(req);
	this->resetGame();

}
void LayerTCC_PopupMain::onBtnSilver(Ref *sender){
	SceneManager::getSingleton().showLoading();

//	this->btnGold->setEnabled(true);
//	this->btnGold->setBright(true);
//
//	this->btnSilver->setEnabled(false);
//	this->btnSilver->setBright(false);
    this->btnSilver->setVisible(false);
    this->btnGold->setVisible(true);
    lblChestMoney->setString("---");
    layerBetDices->loadAmfUser();

    
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> req(new ExtensionRequest("tccl", params));
	GameServer::getSingleton().Send(req);
	this->resetGame();

}
void LayerTCC_PopupMain::OnExtensionResponseProcess()
{
	this->param = mThreadQueueMsg->deQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
	//boost::shared_ptr<long> rscode = param->GetInt(EXT_FIELD_RESULT);
	if (cmd == NULL) return;
	if (0 == cmd->compare(EXT_HEATBEAT)){
		return;
	}
	else if (0 == cmd->compare(EXT_EVENT_TCC_GAME_STATE_NTF)){
		this->event_EXT_EVENT_TCC_GAME_STATE_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_TCC_START))
	{
		this->event_EXT_EVENT_TCC_START(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_TCC_GAME_INFO_NTF))
	{
		this->event_EVENT_TCC_GAME_INFO_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_TCC_END))
	{
		this->event_EVENT_TCC_END(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_TCC_GAME_HIS_RESP))
	{
		this->event_EXT_EVENT_TCC_GAME_HIS_RESP(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_TCC_GAME_BET_ERROR_NOTIFY))
	{
		this->event_EXT_EVENT_TCC_GAME_BET_ERROR_NOTIFY(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_TCC_GAME_BET_NTF))
	{
		this->event_EXT_EVENT_TCC_GAME_BET_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_TCC_GET_QUY_THUONG_RESP))
	{
		this->event_EXT_EVENT_TCC_GET_QUY_THUONG_RESP(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_TCC_GAME_RESULT))
	{
		this->event_EXT_EVENT_TCC_GAME_RESULT(param);
	}
	else if (0 == cmd->compare("tccrs")){
		if (this->currTypeMoney == 0){
			SceneManager::getSingleton().hideLoading();

			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutInt("tm", 1);
			boost::shared_ptr<IRequest> req(new ExtensionRequest("tccj", params));
			GameServer::getSingleton().Send(req);
			this->currTypeMoney = 1;
            layerBetDices->setTypeGame(this->currTypeMoney);
            layerBetDices->loadAmfUser();

		}
		else{
			SceneManager::getSingleton().hideLoading();

			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutInt("tm", 0);
			boost::shared_ptr<IRequest> req(new ExtensionRequest("tccj", params));
			GameServer::getSingleton().Send(req);

			this->currTypeMoney = 0;
            layerBetDices->setTypeGame(this->currTypeMoney);

            layerBetDices->loadAmfUser();


		}
	}
}

void LayerTCC_PopupMain::updateEvent(float dt)
{
	if (mThreadQueueMsg == NULL) return;
	if (mThreadQueueMsg->isQueueEmpty())
		return;
	mThreadQueueMsg->run();
}

void LayerTCC_PopupMain::event_EXT_EVENT_TCC_GAME_STATE_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> glb_ptr = param->GetUtfString(EXT_FIELD_GAME_LIST_BET);
	boost::shared_ptr<long> mid_ptr = param->GetInt(EXT_FIELD_MATCH_ID);
	boost::shared_ptr<long> pt_ptr = param->GetInt(EXT_FIELD_PLAY_TIME);
	boost::shared_ptr<long> gtime_ptr = param->GetInt(EXT_FIELD_GAME_TIME);
	boost::shared_ptr<long> gstate_ptr = param->GetInt(EXT_FIELD_GAME_PLAY_STATE);//isGameStarted() ? 1 : 0
	boost::shared_ptr<long> et_ptr = param->GetInt(EXT_FIELD_END_TIME);
	//gbt":"TOM:100 : 0 : 0; CUA:100 : 0 : 0; o cuoc - muc cuoc cua phong - tong tien dat - tien cuoc cua minh
	boost::shared_ptr<string> gbt_ptr = param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);//danh sach tien dat cuoc o cac o, server tra ve het, client nhan va hien thi theo             muc cuoc o phong hien tai
	boost::shared_ptr<long> toet_ptr = param->GetInt(EXT_FIELD_GAME_TIME_OPEN_EVENT);
	boost::shared_ptr<string> gper_ptr = param->GetUtfString(EXT_FIELD_GAME_PERCENT);
	boost::shared_ptr<string> ghqt_ptr = param->GetUtfString(EXT_FIELD_GAME_HIS_QUY_THUONG);
	//danh cho diem tich luy
	boost::shared_ptr<string> ghtp_ptr = param->GetUtfString(EXT_FIELD_GAME_HISTORY_TOP_PLAYER);
	boost::shared_ptr<string> gtbuc_ptr = param->GetUtfString(EXT_FIELD_GAME_TOTAL_USER_BET_CELL);
    //Director::getInstance()->getScheduler()->scheduleRequestQuyThuong(0);
    layerBetDices->setTypeGame(this->currTypeMoney);

    layerBetDices->loadAmfUser();

	if (glb_ptr){
		SceneManager::getSingleton().hideLoading();
		createListBetValues(*glb_ptr);
	}
	if (mid_ptr)
	{
		string str = StringUtils::format("#%d", *mid_ptr);
		lblMatchId->setString(str);
		currMatchID = boost::to_string(*mid_ptr);
	}
	if (gstate_ptr)
	{
		int state = *gstate_ptr;
		int gametime = *gtime_ptr;
		int endtime = *et_ptr / 1000;
		int playtime = *pt_ptr / 1000;
		handleGameState(state, gametime, endtime, playtime);
	}
	if (gbt_ptr)
	{
		handleCurrentBet(*gbt_ptr);
	}
	if (toet_ptr && gper_ptr)
	{
		timeMoQuy = *toet_ptr;
		listPercent = *gper_ptr;
		Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerTCC_PopupMain::scheduleTimeMoQuy),this,1, false);
	}
	if (ghqt_ptr)
		listQuyThuong = ghqt_ptr->c_str();
	if (ghtp_ptr && gtbuc_ptr)
	{
		listHisTop = ghtp_ptr->c_str();
		listHisUser = gtbuc_ptr->c_str();
	}
}

void LayerTCC_PopupMain::event_EVENT_TCC_GAME_INFO_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> glb_ptr = param->GetUtfString(EXT_FIELD_GAME_LIST_BET);
	boost::shared_ptr<string> ghs_ptr = param->GetUtfString(EXT_FIELD_GAME_HISTORY_RESULT);
	if (glb_ptr) {
		layer1DiceHis->setDataHis(*glb_ptr);
	}
	if (ghs_ptr) {
		layerDiceHis->setDataHis(*ghs_ptr);
		if (isGameStarted == false)
			getResFromMatchID(*ghs_ptr, currMatchID);
	}
}

void LayerTCC_PopupMain::event_EVENT_TCC_END(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> rg_ptr = param->GetUtfString(EXT_FIELD_RESULT_GAME);
	if (rg_ptr){
		isGameStarted = false;
		showResult(*rg_ptr);
	}
	boost::shared_ptr<string> ghtp_ptr = param->GetUtfString(EXT_FIELD_GAME_HISTORY_TOP_PLAYER);
	boost::shared_ptr<string> gtbuc_ptr = param->GetUtfString(EXT_FIELD_GAME_TOTAL_USER_BET_CELL);
	if (ghtp_ptr)
	{
		listHisTop = ghtp_ptr->c_str();
	}
	if (gtbuc_ptr)
	{
		listHisUser = gtbuc_ptr->c_str();
	}
}

void LayerTCC_PopupMain::event_EXT_EVENT_TCC_START(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<long> pt_ptr = param->GetInt(EXT_FIELD_PLAY_TIME);
	boost::shared_ptr<long> mid_ptr = param->GetInt(EXT_FIELD_MATCH_ID);
	if (mid_ptr)
	{
		string str = StringUtils::format("#%d", *mid_ptr);
		lblMatchId->setString(str);
		currMatchID = boost::to_string(*mid_ptr);
	}
	if (pt_ptr){
		int gametime = *pt_ptr / 1000;
		layerTimer->setTimeBet(gametime);
		startGame();
	}
}

void LayerTCC_PopupMain::event_EXT_EVENT_TCC_GAME_HIS_RESP(boost::shared_ptr<ISFSObject> param)
{

}

void LayerTCC_PopupMain::event_EXT_EVENT_TCC_GAME_BET_ERROR_NOTIFY(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<long> errc = param->GetInt(EXT_FIELD_ERROR_CODE);
	boost::shared_ptr<string> bettype = param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
	boost::shared_ptr<string> betvalue = param->GetUtfString(EXT_FIELD_GAME_BET_VALUE);
	boost::shared_ptr<string> betroom = param->GetUtfString(EXT_FIELD_GAME_BET_ROOM);
	if (errc != NULL && bettype != NULL && betvalue != NULL && betroom != NULL) {
		//log("errc = %d", errc);
		string _bettype = *bettype;
		int _betvalue = atoi(betvalue->c_str());
		int _betroom = atoi(betroom->c_str());
		if (*errc == RESCODE_NOT_ENOUGH_MONEY) {
			Chat *toast = Chat::create(dataManager.GetSysString(409), -1);
			this->addChild(toast);
			layerBetDices->rollBackBet(_betroom, _bettype, _betvalue);
		}

		if (*errc == RESCODE_GAME_IS_ENDED) {
			Chat *toast = Chat::create(dataManager.GetSysString(410), -1);
			this->addChild(toast);
			this->layerBetDices->rollBackBet(_betroom, _bettype, _betvalue);
		}
	}
}

void LayerTCC_PopupMain::event_EXT_EVENT_TCC_GAME_BET_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<long> gbv_ptr = param->GetInt(EXT_FIELD_GAME_BET_VALUE);
	boost::shared_ptr<string> uid_ptr = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> gbt_ptr = param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
	boost::shared_ptr<string> gbtbuc_ptr = param->GetUtfString(EXT_FIELD_GAME_TOTAL_USER_BET_CELL);
	boost::shared_ptr<string> gtbvc_ptr = param->GetUtfString(EXT_FIELD_GAME_TOTAL_BET_VALUE_CELL);
	boost::shared_ptr<string> gbr_ptr = param->GetUtfString(EXT_FIELD_GAME_BET_ROOM);
	if (uid_ptr != NULL && gbtbuc_ptr != NULL && gtbvc_ptr != NULL && gbr_ptr != NULL && gbt_ptr != NULL && gbv_ptr != NULL) {
		string uid = *uid_ptr;
		string totalbetuser = *gbtbuc_ptr;
		string totalbet = *gtbvc_ptr;
		int betroom = atoi(gbr_ptr->c_str());
		string bettype = *gbt_ptr;
		int betvl = *gbv_ptr;
		layerBetDices->setBetInfo(myName, uid, totalbetuser, totalbet, betroom);
	}
}

void LayerTCC_PopupMain::event_EXT_EVENT_TCC_GET_QUY_THUONG_RESP(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<long> betvl_ptr = param->GetInt(EXT_FIELD_GAME_BET_VALUE);
	if (betvl_ptr)
		lblChestMoney->setString(mUtils::convertMoneyEx(*betvl_ptr));
}

void LayerTCC_PopupMain::event_EXT_EVENT_TCC_GAME_RESULT(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<long>gwv_ptr = param->GetInt(EXT_FIELD_GAME_WIN_VALUE);
	//danh cho diem tich luy
	boost::shared_ptr<string> ghtp_ptr = param->GetUtfString(EXT_FIELD_GAME_HISTORY_TOP_PLAYER);
	boost::shared_ptr<string> gtbuc_ptr = param->GetUtfString(EXT_FIELD_GAME_TOTAL_USER_BET_CELL);
	if (gwv_ptr != NULL) {
		int win_value = *gwv_ptr;
		showResultGame(win_value);
	}
	if (ghtp_ptr)
	{
		listHisTop = ghtp_ptr->c_str();
	}
	if (gtbuc_ptr)
	{
		listHisUser = gtbuc_ptr->c_str();
	}
}

void LayerTCC_PopupMain::createSmartFoxEvent()
{

	mThreadQueueMsg = new ThreadQueueMsg();
	mThreadQueueMsg->setFunCallBack(this, callfunc_selector(LayerTCC_PopupMain::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerTCC_PopupMain::updateEvent),this,1/60, false);

	this->joinGameRequest();
	//this->gameStateRequest();
	SceneManager::getSingleton().showLoading();
}

void LayerTCC_PopupMain::joinGameRequest()
{
	boost::shared_ptr<ISFSObject> param(new SFSObject());
	param->PutInt("tm", 0);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_JOIN, param));
	GameServer::getSingleton().Send(request);
}

void LayerTCC_PopupMain::leaveGameRequest()
{
	boost::shared_ptr<ISFSObject> param(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_LEAVE, param));
	GameServer::getSingleton().Send(request);
}

void LayerTCC_PopupMain::infoGameRequest()
{
	boost::shared_ptr<ISFSObject> param(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_GET_GAME_INFO_REQ, param));
	GameServer::getSingleton().Send(request);
}

void LayerTCC_PopupMain::gameStateRequest()
{
	boost::shared_ptr<ISFSObject> param(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_GET_GAME_INFO_REQ, param));
	GameServer::getSingleton().Send(request);
}

void LayerTCC_PopupMain::scheduleRequestQuyThuong(float dt)
{
	boost::shared_ptr<ISFSObject> param(new SFSObject());
	param->PutInt(EXT_FIELD_GAME_BET_VALUE, 0);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_GET_QUY_THUONG_REQ, param));
	GameServer::getSingleton().Send(request);
}

void LayerTCC_PopupMain::handleGameState(int state, int gametime, int endtime, int playtime)
{
	resetGame();

	if (state == 0)//chua bat dau game
	{

		if (gametime == 0)
		{
			gametime = 1;
		}
		layerTimer->setTimeBet(gametime);
		layerTimer->setTimePrepare(gametime);
		isGameStarted = false;
		prepareGame();
	}
	else if (state == 1)
	{
		layerTimer->setTimeBet(gametime);
		layerTimer->setTimePrepare(endtime);
		isGameStarted = true;
		startGame();
	}
}

void LayerTCC_PopupMain::handleCurrentBet(string gbt)
{
	if (listBetRateValues.size() == 0 || listBetCoinValues.size() == 0)
		return;
	layerBetDices->initBetInfo(gbt);
}

void LayerTCC_PopupMain::createListBetValues(string glb)
{
	if (listBetRateValues.size() != 0 || listBetCoinValues.size() != 0)
		return;
	listBetCoinValues.clear();
	listBetRateValues.clear();

	vector<string> arrBet = mUtils::splitString(glb, '_');
	if (arrBet.size() > 1){
		for (int i = 0; i < arrBet.size(); i++){
			//lay tu vi tri thu 1 - thang 0 bi thua
			vector<string> arrValues = mUtils::splitString(arrBet.at(i), ':');
			if (arrValues.size() > 1){
				listBetRateValues.push_back(arrValues.at(0));
				if (i == 1)
				{
					string strVal = arrValues.at(1);
					vector<string> arrCoinVals = mUtils::splitString(strVal, ';');
					for (int k = 0; k < arrCoinVals.size(); k++)
					{
						listBetCoinValues.push_back(arrCoinVals.at(k));
					}
				}

			}
		}
	}
	else{
		listBetRateValues.push_back("100");
		listBetRateValues.push_back("1000");
		listBetRateValues.push_back("10000");

		listBetCoinValues.push_back("100");
		listBetCoinValues.push_back("500");
		listBetCoinValues.push_back("1000");
		listBetCoinValues.push_back("5000");
		listBetCoinValues.push_back("10000");
		listBetCoinValues.push_back("50000");
		listBetCoinValues.push_back("100000");
		listBetCoinValues.push_back("500000");
		listBetCoinValues.push_back("1000000");

	}

	createListBetCoin();
	//createBetRateList();
}

void LayerTCC_PopupMain::createLayerDiceHis()
{
	pnlDiceHis = (Layout*)pnlMain->getChildByName("pnlDiceHis");
	pnl1DiceHis = (Layout*)pnlMain->getChildByName("pnl1DiceHis");
	pnlMain->setSwallowTouches(false);
	if (pnlDiceHis){
		pnl1DiceHis->setSwallowTouches(false);
		layerDiceHis = LayerTCC_DicesHistory::create();
		layerDiceHis->setAnchorPoint(Vec2::ZERO);
		layerDiceHis->setPosition(Vec2::ZERO);
		pnlDiceHis->addChild(layerDiceHis);
	}
	if (pnl1DiceHis){
		layer1DiceHis = LayerTCC_OneDiceHistory::create();
		layer1DiceHis->setAnchorPoint(Vec2::ZERO);
		layer1DiceHis->setPosition(Vec2::ZERO);
		pnl1DiceHis->addChild(layer1DiceHis);
	}
}

void LayerTCC_PopupMain::createLayerBetDice()
{
	Layout* pnlBetCoin = (Layout*)pnlMain->getChildByName("pnlBetCoin");
	if (pnlBetCoin)
	{
		pnlBetCoin->setSwallowTouches(false);
		layerBetDices = LayerBetDice::create();
		pnlBetCoin->addChild(layerBetDices);
	}
}

void LayerTCC_PopupMain::createListBetCoin()
{
	// 	for (int i = 0; i < 5; i++)
	// 	{
	// 		Button* buttonBetCoin = (Button*)pnlMain->getChildByName(StringUtils::format("btnBetCoin%d", (i + 1)));
	// 		if (buttonBetCoin){
	// 			showPressStateButton(buttonBetCoin, false);
	// 			buttonBetCoin->setTag(i);
	// 			buttonBetCoin->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMain::onButtonBetCoin, this));
	// 			listBetCoins.push_back(buttonBetCoin);
	// 		}
	// 	}

	//for new version
	for (int i = 0; i < listBetCoinValues.size(); i++) {
		Button* buttonBetCoin = (Button*)pnlMain->getChildByName(StringUtils::format("btnBetCoin%d", (i + 1)));
		if (buttonBetCoin) {
			showPressStateButton(buttonBetCoin, false);
			buttonBetCoin->setTag(i);
			buttonBetCoin->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMain::onButtonBetCoin, this));
			listBetCoins.push_back(buttonBetCoin);
			//for new version
			int value_bet = atoi(listBetCoinValues.at(i).c_str());
			listBetCoins.at(i)->setTag(value_bet);
			listBetCoins.at(i)->setTitleText(formatMoneySymbolTCC(value_bet));
			showPressStateButton(listBetCoins.at(i), false);
		}
	}
	///for new version
	if (listBetCoins.size() > 0) {
		int firstBet = dataManager.getAppConfig().firstBetTCC;
		onButtonBetCoin(listBetCoins[firstBet], ui::Widget::TouchEventType::ENDED);
	}
}

void LayerTCC_PopupMain::createBetRateList()
{
	for (int i = 0; i < listBetRateValues.size(); i++){
		Button* buttonBetRate = (Button*)pnlMain->getChildByName(StringUtils::format("btnBetRate%d", i + 1));
		if (buttonBetRate)
		{
			string bet_value = formatMoneySymbolTCC(listBetRateValues.at(i));
			showPressStateButton(buttonBetRate, false);
			buttonBetRate->setTag(i);
			buttonBetRate->setTitleText(bet_value);
			buttonBetRate->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMain::onButtonBetRate, this));
			listBetRates.push_back(buttonBetRate);
		}
	}
	if (listBetRates.size() > 0){
		int firstBet = dataManager.getAppConfig().firstBetTCC;//hien thi muc cuoc dau tien
		onButtonBetRate(listBetRates.at(firstBet), ui::Widget::TouchEventType::ENDED);
	}
}

void LayerTCC_PopupMain::createListResDices()
{
	for (int i = 0; i < 3; i++){
		ImageView* imgRes = (ImageView*)pnlMain->getChildByName(StringUtils::format("imgRes%d", i + 1));
		if (imgRes)
		{
			imgRes->loadTexture("dice_empty.png");
			listResDices.push_back(imgRes);
		}
	}
}

void LayerTCC_PopupMain::createPlate()
{
	pnlPlate = (Layout*)pnlMain->getChildByName("pnlPlate");
	if (pnlPlate)
	{
		pnlPlate->setVisible(false);
		for (int i = 0; i < 3; i++)
		{
			ImageView* imgPlateRes = (ImageView*)pnlPlate->getChildByName(StringUtils::format("imgPlateRes%d", i + 1));
			if (imgPlateRes)
				listResImgPlates.push_back(imgPlateRes);
		}
		imgBowl = (ImageView*)pnlPlate->getChildByName("imgBat");
		if (imgBowl)
			startBowlX = imgBowl->getPositionX();
	}
}

void LayerTCC_PopupMain::createUI()
{
	Button* btnClose = (Button*)pnlMain->getChildByName("btnClose");
    pnlMain->setSwallowTouches(false);
	if (btnClose)
		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMain::onButtonClose, this));
	lblMatchId = (Text*)pnlMain->getChildByName("lblMatchId");
	lblChestMoney = (Text*)pnlMain->getChildByName("lblChestMoney");
	line_25 = (Sprite*)pnlMain->getChildByName("line_25");
	Button* btnRank = (Button*)pnlMain->getChildByName("btnRank");
	if (btnRank)
		btnRank->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMain::onButtonRank, this));
	Button* btnHis = (Button*)pnlMain->getChildByName("btnHistory");
	if (btnHis)
		btnHis->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMain::onButtonHis, this));
	Button* btnTut = (Button*)pnlMain->getChildByName("btnTut");
	if (btnTut)
		btnTut->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMain::onButtonTut, this));
	Button* btnMoquy = (Button*)pnlMain->getChildByName("btnMoquy");
	if (btnMoquy)
		btnMoquy->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMain::onButtonMoQuy, this));
}

void LayerTCC_PopupMain::createLayerTimer()
{
	layerTimer = LayerTCC_Timer::create();
	layerTimer->setCallBackFromTimer(CC_CALLBACK_0(LayerTCC_PopupMain::CallBackFromTimer, this));
	pnlMain->addChild(layerTimer, ZORDER_LIST::ZORDER_POPUP_RANK);
}

void LayerTCC_PopupMain::onButtonBetRate(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Button* btn = (Button*)pSender;
		int tag = btn->getTag();
		currBetRate = tag;
		for (int i = 0; i < listBetRates.size(); i++){
			Button* btnBet = listBetRates.at(i);
			showPressStateButton(btnBet, false);
		}
		showPressStateButton(btn, true);
		reloadBetCoins(tag);
	}
}

void LayerTCC_PopupMain::onButtonBetCoin(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Button* btn = (Button*)pSender;
		for (int i = 0; i < listBetCoins.size(); i++){
			Button* btnbet = listBetCoins.at(i);
			showPressStateButton(btnbet, false);
		}
		int value_bet = btn->getTag();
		showPressStateButton(btn, true);
		//string betRoom = listBetRateValues.at(currBetRate);
		string betRoom = "1000";
		layerBetDices->setBetValue(value_bet);
		layerBetDices->setBetRoom(atoi(betRoom.c_str()));
	}
}

void LayerTCC_PopupMain::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		leaveGameRequest();
		removeFromParentAndCleanup(true);
	}
}

void LayerTCC_PopupMain::onButtonRank(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		LayerTCC_PopupRank *popupRank = LayerTCC_PopupRank::create();
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(popupRank, ZORDER_LIST::ZORDER_POPUP_RANK);
	}
}

void LayerTCC_PopupMain::onButtonHis(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		LayerTCC_PopupHistory *popupRank = LayerTCC_PopupHistory::create();
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(popupRank, ZORDER_LIST::ZORDER_POPUP_RANK);
	}
}

void LayerTCC_PopupMain::onButtonTut(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto currScene = Director::getInstance()->getRunningScene();
		if (currScene)
		{
			string url = StringUtils::format("%s%d", GUIDE_LINK, 131);
			LayerPopupWebView* layerTut = LayerPopupWebView::create();
			layerTut->setWebView(url);
			currScene->addChild(layerTut, ZORDER_LIST::ZORDER_POPUP_RANK);
		}
	}
}

void LayerTCC_PopupMain::onButtonMoQuy(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_PopupMain::scheduleTimeMoQuy),this);
		LayerTCC_PopupMoQuy* layerMQ = LayerTCC_PopupMoQuy::create();
		layerMQ->setTimeMoQuy(timeMoQuy);
		layerMQ->setListPercent(listPercent);
		layerMQ->setHisQuyThuong(listQuyThuong);
		layerMQ->setListHistTichLuy(listHisTop, listHisUser);
        layerMQ->setCurrentType(this->currTypeMoney);
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(layerMQ, ZORDER_LIST::ZORDER_POPUP_WEBVIEW);
		layerMQ->startMoQuy();
	}
}

void LayerTCC_PopupMain::reloadBetCoins(int tag)
{
	if (listBetCoins.size() == 5){
		int jump_unit = 5 * currBetRate;
		for (int i = 0; i < listBetCoins.size(); i++){
			int value_bet = atoi(listBetCoinValues.at(i + jump_unit).c_str());
			listBetCoins.at(i)->setTag(value_bet);
			listBetCoins.at(i)->setTitleText(formatMoneySymbolTCC(value_bet));
			showPressStateButton(listBetCoins.at(i), false);
		}

		layerBetDices->setBetValue(0);
		layerBetDices->setBetRoom(atoi(listBetRateValues.at(currBetRate).c_str()));
		layerBetDices->loadBetInfo();
	}
}

void LayerTCC_PopupMain::showPressStateButton(Button* pSender, bool isPress)
{
	pSender->setBright(!isPress);
	pSender->setEnabled(!isPress);
}

void LayerTCC_PopupMain::startAnimShake()
{
	pnl1DiceHis->setVisible(false);
	pnlDiceHis->setVisible(false);
	pnlPlate->setVisible(false);
	line_25->setVisible(false);
	Vec2 pos = Vec2(270, 330);
	LayerTCC_PlateShakeAnim * layerAnim = LayerTCC_PlateShakeAnim::create(pos, 0);
	//layerAnim->setTarget(this);
	layerAnim->setCallBackListener(CC_CALLBACK_0(LayerTCC_PopupMain::CallBackFromAnim, this));
	pnlMain->addChild(layerAnim, 1000, 2468);
	if (imgBowl->getPositionX() != startBowlX)
		imgBowl->setPositionX(startBowlX);
}

void LayerTCC_PopupMain::showResult(string _result)
{
	layerBetDices->endGame();
	layerTimer->stopTimeBet();
	if (imgBowl->getPositionX() != startBowlX){
		imgBowl->setPositionX(startBowlX);
	}
	pnlPlate->setVisible(true);
	imgBowl->setVisible(true);
	vector<string> arr_res = mUtils::splitString(_result, '_');
	if (arr_res.size() == listResDices.size()){
		for (int i = 0; i < listResImgPlates.size(); i++){
			int id = getIdByBetType(arr_res.at(i));
			string url = getImgDiceById(id);
			listResImgPlates.at(i)->loadTexture(url);
		}
	}

	CallFunc* callFuncCallback = CallFunc::create(CC_CALLBACK_0(LayerTCC_PopupMain::actionOpenBowlDone, this, _result));
	Sequence* seq_action = Sequence::create(MoveBy::create(1, Vec2(-200, 0)), Hide::create(), callFuncCallback, NULL);
	imgBowl->runAction(seq_action);
}

void LayerTCC_PopupMain::actionOpenBowlDone(string _result)
{
	vector<string> arr_res = mUtils::splitString(_result, '_');
	if (arr_res.size() == listResDices.size())
	{
		for (int i = 0; i < listResDices.size(); i++)
		{
			int id = getIdByBetType(arr_res.at(i));
			string url = getImgDiceById(id);
			listResDices.at(i)->loadTexture(url);
		}
	}

	layerBetDices->setResult(_result);
	this->runAction(Sequence::create(DelayTime::create(timePrepare),
		CallFunc::create(CC_CALLBACK_0(LayerTCC_PopupMain::delayToShowPrepare, this)), NULL));
}

void LayerTCC_PopupMain::delayToShowPrepare()
{
	if (!isGameStarted)
		prepareGame();
}

void LayerTCC_PopupMain::resetGame()
{
	pnlPlate->setVisible(false);
	pnl1DiceHis->setVisible(true);
	pnlDiceHis->setVisible(true);
	line_25->setVisible(false);
}

void LayerTCC_PopupMain::startGame()
{
	pnlMain->removeChildByTag(2468);
	layerBetDices->resetGame();

	pnl1DiceHis->setVisible(false);
	pnlDiceHis->setVisible(false);
	line_25->setVisible(false);
	pnlPlate->setVisible(true);
	imgBowl->setVisible(true);
	layerTimer->stopTimeBet();
	layerTimer->stopTimePrepare();

	// 	layerTimer->stopTimePrepare();
	// 	layerTimer->stopTimeBet();
	for (int i = 0; i < listResDices.size(); i++){
		string url = "dice_empty.png";
		listResDices.at(i)->loadTexture(url);
		listResImgPlates.at(i)->loadTexture(url);
	}
	int timeBet = layerTimer->getTimeBet();
	if (timeBet < 50)
	{
		if (imgBowl->getPositionX() != startBowlX)
			imgBowl->setPositionX(startBowlX);
		CallBackFromAnim();
	}
	else{
		startAnimShake();
	}
}

void LayerTCC_PopupMain::prepareGame()
{
	//pnlPlate->setVisible(false);
	pnl1DiceHis->setVisible(true);
	pnlDiceHis->setVisible(true);
	line_25->setVisible(false);
	layerTimer->startTimePrepare();
	pnlPlate->runAction(Sequence::create(DelayTime::create(3), Hide::create(), NULL));
}

void LayerTCC_PopupMain::showResultGame(int money)
{
	string numberString;
	if (money > 0)
		numberString.append(StringUtils::format("+%s", mUtils::convertMoneyEx(money).c_str()));
	else
		numberString.append(StringUtils::format("-%s", mUtils::convertMoneyEx(money).c_str()));
	string font_name = "font_win-font.fnt";
	if (numberString.find('-') != std::string::npos) //contain string '-'
		font_name = "font_lose-font.fnt";
	Size parentSize = pnlMain->getContentSize();
	Label* labelResult = Label::createWithBMFont(font_name, numberString, TextHAlignment::CENTER);
	labelResult->setAnchorPoint(Vec2::ZERO);
	labelResult->setPosition(Vec2(parentSize.width / 2, 0));
	pnlMain->addChild(labelResult);
	labelResult->setVisible(false);
	auto moveAction = Spawn::create(Show::create(), MoveBy::create(1, Vec2(0, labelResult->getContentSize().height)), NULL);
	labelResult->runAction(Sequence::create(
		DelayTime::create(1),
		moveAction,
		DelayTime::create(2),
		FadeOut::create(0.2),
		RemoveSelf::create(true), nullptr));
}

void LayerTCC_PopupMain::startScheduleTimeMoQuy()
{
	//Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_PopupMain::scheduleTimeMoQuy));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerTCC_PopupMain::scheduleTimeMoQuy),this,1, false);
}

// TimeMQ getDateFromInt(int _time)
// {
// 	int _day = _time / (60 * 60 * 24);
// 	int _hour = (_time / (60 * 60)) % 24;
// 	int _min = (_time / (60)) % 60;
// 	int _se = _time % 60;
// 	return TimeMQ(_day, _hour, _min, _se);
// }
void LayerTCC_PopupMain::scheduleTimeMoQuy(float dt)
{
	timeMoQuy--;
	// 	TimeMQ date_str = getDateFromInt(timeMoQuy);
	// 	int _day = date_str.dayMQ;
	// 	int _hour = date_str.hourMQ;
	// 	int _min = date_str.minuteMQ;
	// 	int _se = date_str.secondMQ;
	// 	log("%d::%d::%d::%d", _day, _hour, _min, _se);
	if (timeMoQuy <= 0){
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_PopupMain::scheduleTimeMoQuy),this);
		gameStateRequest();
	}
}
void LayerTCC_PopupMain::CallBackFromAnim()
{
	//an bat va mat quan xuc sac
	for (int i = 0; i < listResDices.size(); i++)
	{
		listResDices.at(i)->setVisible(false);
	}
	pnlPlate->setVisible(false);
	imgBowl->setVisible(false);
	//show lich su
	line_25->setVisible(false);
	pnl1DiceHis->setVisible(true);
	pnlDiceHis->setVisible(true);

	layerTimer->startTimeBet();
	layerBetDices->startGame();
}
void LayerTCC_PopupMain::CallBackFromTimer()
{
	log("CallBackFromTimer");
	// 	if (_flag == 0)
	// 	{
	layerBetDices->endGame();
	//an bat va mat quan xuc sac
	//	}
	for (int i = 0; i < listResDices.size(); i++)
	{
		listResDices.at(i)->setVisible(true);
	}
}
bool LayerTCC_PopupMain::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto rect = this->pnlMain->getBoundingBox();
    auto locationInNode = pTouch->getLocation();
    if (rect.containsPoint(locationInNode)){
        return true;
    }
    //var s = target.getContentSize();
    return false;

}
void LayerTCC_PopupMain::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    auto target = pnlMain;
    auto delta = pTouch->getDelta();
    //  if (target.x ==0 || target.x == WIDTH_DESIGN-143 || target.y ==0 || target.y == HEIGHT_DESIGN-149)
    //     return;
    auto x = target->getPositionX() + delta.x;
    auto y = target->getPositionY() + delta.y;
    
    this->pnlMain->setPosition(Vec2(x,y));
    //    if (abs(delta.x) > 10 || abs(delta.y) > 10)
    //        this->isTouchMove =true;
    //    //listener->setSwallowTouches(true);
}
void LayerTCC_PopupMain::onEnter()
{
	Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerTCC_PopupMain::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerTCC_PopupMain::onTouchMoved, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;

}

void LayerTCC_PopupMain::onExit()
{
	Layer::onExit();
   // SceneManager::getSingleton().setIsShowTCC(false);

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(listener);
}

void LayerTCC_PopupMain::getResFromMatchID(string strHis, string mid)
{
	string res = "";
	vector<string> arrStr = mUtils::splitString(strHis, ';');
	for (int i = 0; i < arrStr.size(); i++)
	{
		vector<string> arrInfo = mUtils::splitString(arrStr.at(i), '|');
		if (arrInfo.size() == 2)
		{
			if (0 == arrInfo.at(0).compare(mid))
			{
				res = arrInfo.at(1);
				break;
			}
		}
	}
	if (0 != res.compare(""))
	{
		vector<string> arr_res = mUtils::splitString(res, '_');
		if (arr_res.size() == listResDices.size())
		{
			for (int i = 0; i < listResDices.size(); i++)
			{
				int id = getIdByBetType(arr_res.at(i));
				string url = getImgDiceById(id);
				listResDices.at(i)->setVisible(true);
				listResDices.at(i)->loadTexture(url);
			}
		}
	}
}

void LayerTCC_PopupMain::onButtonChat(Ref* pSender)
{
	bool isVisible = this->layerChat->isVisible();
	if (isVisible)
	{
		this->layerChat->setVisible(false);
	}
	else{
        this->layerChat->setVisible(true);
	}
}

