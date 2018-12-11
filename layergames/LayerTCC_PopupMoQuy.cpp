#include "LayerTCC_PopupMoQuy.h"
#include "LayerTCC_PopupMain.h"
#include "mUtils.h"
#include "LayerTCC_PopupXHMoQuy.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "TCCMsgDef.h"
#include "layers/LayerPopupThongBao.h"
#include "SceneManager.h"
#include <algorithm>

LayerTCC_PopupMoQuy::LayerTCC_PopupMoQuy()
{
	listHisTop = "";
	listHisUser = "";
	timeToNextWeek = 0;
	timeMoQuy = 0;
}

LayerTCC_PopupMoQuy::~LayerTCC_PopupMoQuy()
{

	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_PopupMoQuy::scheduleUpdateTime),this);
	GameServer::getSingleton().removeListeners(this);
}
bool LayerTCC_PopupMoQuy::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_PopupMoQuy.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());

	// 	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// 	int gameId = SceneManager::getSingleton().getGameID();
	// 	if (gameId == -1)
	// 	{
	// 		root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	// 		root->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	// 	}

	pnlMain = (Layout *)root->getChildByName("pnlMain");
	if (pnlMain)
	{
		createUI();
		createSlotMachines();
		createButton();
	}
    
	GameServer::getSingleton().addListeners(this);

	return true;
}
void LayerTCC_PopupMoQuy::setCurrentType(int type){
    if (type == 0){
        for (int i = 0; i < this->lstCoin.size();i++){
            this->lstCoin[i]->setTexture("profile-coin.png");
        }
    }
    else{
        for (int i = 0; i < this->lstCoin.size();i++){
            this->lstCoin[i]->setTexture("button_xu.png");
        }
    }
}
void LayerTCC_PopupMoQuy::onEnter()
{
	Layer::onEnter();
}

void LayerTCC_PopupMoQuy::onExit()
{
	Layer::onExit();
	LayerTCC_PopupMain *parent = dynamic_cast<LayerTCC_PopupMain*>(this->getParent());
	if (parent)
	{
		parent->setTimeMoQuy(timeMoQuy);
		parent->startScheduleTimeMoQuy();
	}
}

void LayerTCC_PopupMoQuy::setTimeMoQuy(int _time)
{
	timeMoQuy = _time;
}

void LayerTCC_PopupMoQuy::setListPercent(string _list)
{
	vector<string> arrStr = mUtils::splitString(_list, '_');
	if (arrStr.size() == arrLabelPercent.size())
	{
		for (int i = 0; i < arrStr.size(); i++)
		{
			string percent = arrStr.at(i) + "%";
			arrLabelPercent.at(i)->setString(percent);
		}
	}
}

void LayerTCC_PopupMoQuy::startMoQuy()
{
	if (timeMoQuy > -10)//neu > gia tri default thi moi chay
		Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerTCC_PopupMoQuy::scheduleUpdateTime),this,1, false);
}

void LayerTCC_PopupMoQuy::createUI()
{
	imgHeader = (Sprite*)pnlMain->getChildByName("imgHeader");
	pnlTimer = (Layout*)pnlMain->getChildByName("pnlTimer");
    
    Sprite* sprite = static_cast<Sprite *>(pnlMain->getChildByName("coin1"));
    for (int i = 1; i <= 6; i++) {
        auto sprite = (Sprite *)pnlMain->getChildByName(StringUtils::format("coin%d",i));
        this->lstCoin.push_back(sprite);
    }
    
	lblGio = (Text*)pnlTimer->getChildByName("lblGio");
	lblPhut = (Text*)pnlTimer->getChildByName("lblPhut");
	lblNgay = (Text*)pnlTimer->getChildByName("lblNgay");
	Sprite_17 = (Sprite*)pnlTimer->getChildByName("Sprite_17");

	pnlQuayThuong = (Layout*)pnlMain->getChildByName("pnlQuayThuong");
	spriteQuayThuong = (Sprite*)pnlQuayThuong->getChildByName("spriteQuayThuong");
	lblTime = (Text*)pnlQuayThuong->getChildByName("lblTime");

	for (int i = 0; i < 6; i++){
		TextBMFont* labelPercent = (TextBMFont*)pnlMain->getChildByName(StringUtils::format("lblPer%d", i + 1));
		Text* labelUser = (Text*)pnlMain->getChildByName(StringUtils::format("lblName%d", i + 1));
		//new version
		Text* labelMoney = (Text*)pnlMain->getChildByName(StringUtils::format("lblMoney%d", i + 1));
		if (labelPercent)
			arrLabelPercent.push_back(labelPercent);
		if (labelUser)
			arrLabelUser.push_back(labelUser);
		if (labelMoney)
			arrLabelMoney.push_back(labelMoney);
	}


	//cheat ui
	// 	txtCheat = (TextField*)pnlMain->getChildByName("txtCheat");
	// 	if (txtCheat)
	// 		txtCheat->setVisibleEditBox(false);
	// 	Button* btnCheat = (Button*)pnlMain->getChildByName("btnCheat");
	// 	if (btnCheat)
	// 	{
	// 		btnCheat->setVisible(false);
	// 		//btnCheat->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMoQuy::onButtonCheat, this));
	// 	}
}

void LayerTCC_PopupMoQuy::createSlotMachines()
{
	Layout* pnlSlot = (Layout*)pnlMain->getChildByName("pnlSlot");
	if (pnlSlot)
	{
		string ress[6] = { "ca_x2.png", "ga_x2.png", "cua_x2.png", "ho_x2.png", "bau_x2.png", "tom_x2.png" };
		vector<string> listRes;
		for (int i = 0; i < 6; i++)
			listRes.push_back(ress[i]);
		for (int i = 0; i < 6; i++)
		{
			LayerTCC_SLotMachine * layerSlot = LayerTCC_SLotMachine::create();
			layerSlot->setSlotSize(Size(200, 200));
			layerSlot->setShowSize(Size(300, 300));
			layerSlot->insertSlot(listRes, 50);
			layerSlot->setAnchorPoint(Vec2::ZERO);
			layerSlot->setPosition(Vec2(190 * i+ 35, 0));
			layerSlot->setFirstSlot(i);
			layerSlot->setTag(1000 + i);
			layerSlot->setCallBack(this, callfuncND_selector(LayerTCC_PopupMoQuy::slotMachineCallBack));
			pnlSlot->addChild(layerSlot);
			arrLayerSlot.push_back(layerSlot);
		}
	}
}

void LayerTCC_PopupMoQuy::createButton()
{
	Button* btnClose = (Button*)pnlMain->getChildByName("btnClose");
	if (btnClose)
		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMoQuy::onButtonClose, this));

	Button* btnRank = (Button*)pnlMain->getChildByName("btnRank");
	if (btnRank)
		btnRank->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMoQuy::onButtonRank, this));
	Button* btnRule = (Button*)pnlMain->getChildByName("btnRule");
	if (btnRule)
		btnRule->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMoQuy::onButtonRule, this));
}

void LayerTCC_PopupMoQuy::scheduleUpdateTime(float dt)
{
	TimeMQ date_str = getDateFromInt(timeMoQuy);
	int _day = date_str.dayMQ;
	int _hour = date_str.hourMQ;
	int _min = date_str.minuteMQ;
	int _se = date_str.secondMQ;
	log("%d::%d::%d::%d", _day, _hour, _min, _se);
	if (timeMoQuy > 1){
		// 		if (timeToNextWeek != 0 && timeMoQuy > timeToNextWeek)
		// 		{
		// 			pnlTimer->setVisible(true);
		// 			pnlQuayThuong->setVisible(false);
		// 			lblNgay->setString(StringUtils::format("%d", _day));
		// 			lblGio->setString(StringUtils::format("%d", _hour));
		// 			lblPhut->setString(StringUtils::format("%d", _min));
		// 			Sprite_17->setVisible(false);
		// 		}
		// 		else{
		if (timeMoQuy <= 10)
		{
			pnlTimer->setVisible(false);
			pnlQuayThuong->setVisible(true);
			spriteQuayThuong->setTexture("text-4.png");
			lblTime->setString(StringUtils::format("%d", _se));
		}
		else{
			pnlTimer->setVisible(true);
			pnlQuayThuong->setVisible(false);
			lblNgay->setString(StringUtils::format("%d", _day));
			lblGio->setString(StringUtils::format("%d", _hour));
			lblPhut->setString(StringUtils::format("%d", _min));
			//Sprite_17->setVisible(true);
		}
		//}
	}
	else{
		//bat dau quay thuong
		startQuayThuong();
		return;
	}
	timeMoQuy--;
}

// void LayerTCC_PopupMoQuy::slotMachineCallBack(int tag)
// {
// 	log("slotMachineCallBack %d", tag);
// }

void LayerTCC_PopupMoQuy::setHisQuyThuong(string _his)
{
	//"minhdv|CUA|40|100000;hoangdd|GA|20|10000;killer87|GA|20|10000;superman3|GA|5|5000;mrtuan2|GA|5|5000;kimnamcham|GA|5|5000"
	vector<string> arrStr = mUtils::splitString(_his, ';');
	if (arrStr.size() == arrLayerSlot.size() && arrStr.size() == arrLabelUser.size())
	{
		for (int i = 0; i < arrStr.size(); i++)
		{
			vector<string> infos = mUtils::splitString(arrStr.at(i), '|');
			if (infos.size() == 4)
			{
				string uid = infos.at(0);
				string bettype = infos.at(1);
				double money = atol(infos.at(3).c_str());
				arrLabelUser.at(i)->setString(formatUserName(uid, 18));
				arrLayerSlot.at(i)->setFirstSlot(getIdByBetType(bettype));
				arrLabelMoney.at(i)->setString(mUtils::convertMoneyEx(money));
				//new version
				arrLabelUser.at(i)->setName(uid);
				arrLabelUser.at(i)->setTouchEnabled(true);
				arrLabelUser.at(i)->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupMoQuy::onLabelUserTouch, this));
			}
		}
	}
}

void LayerTCC_PopupMoQuy::startQuayThuong()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerTCC_PopupMoQuy::scheduleUpdateTime),this);
	pnlTimer->setVisible(false);
	pnlQuayThuong->setVisible(true);
	spriteQuayThuong->setTexture("text-3.png");
	auto actionScale = ScaleBy::create(0.6, 1.15, 1.15);
	auto resultaction = Sequence::create(actionScale, actionScale->reverse(), NULL);
	spriteQuayThuong->runAction(RepeatForever::create(resultaction));
	lblTime->setVisible(false);
	for (int i = 0; i < arrLabelUser.size(); i++)
	{
		arrLabelUser.at(i)->setVisible(false);
		arrLabelMoney.at(i)->setVisible(false);
	}
	showAllSlot(false);
}

void LayerTCC_PopupMoQuy::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (0 == ptrNotifiedCmd->compare(EXT_EVENT_TCC_OPEN_QUY_THUONG_NTF))
	{
		event_EXT_EVENT_TCC_OPEN_QUY_THUONG_NTF(param);
	}
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_TCC_GAME_STATE_NTF))
	{
		event_EXT_EVENT_TCC_GAME_STATE_NTF(param);
	}
}

void LayerTCC_PopupMoQuy::event_EXT_EVENT_TCC_OPEN_QUY_THUONG_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> ghqt_ptr = param->GetUtfString(EXT_FIELD_GAME_HIS_QUY_THUONG);
	if (ghqt_ptr)
	{
		/// - ở bản tin EXT_EVENT_TCC_OPEN_QUY_THUONG_NTF = "tccoqt_ntf", lúc nào client nhận đc thì thông báo
		//-field: EXT_FIELD_GAME_HIS_QUY_THUONG = "ghqt";
		//-"minhdv|CUA|40|100000;hoangdd|GA|20|10000;killer87|GA|20|10000;superman3|GA|5|5000;mrtuan2|GA|5|5000;kimnamcham|GA|5|5000"
		startQuayThuong();
		vector<string> arrStr = mUtils::splitString(*ghqt_ptr, ';');
		for (int i = 0; i < arrStr.size(); i++)
		{
			vector<string> infos = mUtils::splitString(arrStr.at(i), '|');
			if (infos.size() >= 4)
			{
				string uid = infos.at(0);
				string bettype = infos.at(1);
				string money = infos.at(3);
				arrResMQ.push_back(ResMQ(uid, bettype, money));
			}
		}
		if (arrResMQ.size() > 1)
		{
			imgHeader->setTexture("giai-1.png");
			startSlotWithIdx(0, arrResMQ.at(0).bettype, 10);
		}

	}
}

void LayerTCC_PopupMoQuy::setListHistTichLuy(string _listHisTop, string _listHisUser)
{
	listHisTop = _listHisTop;
	listHisUser = _listHisUser;
}

void LayerTCC_PopupMoQuy::showAllSlot(bool isShow)
{
	for (int i = 0; i < arrLayerSlot.size(); i++)
	{
		arrLayerSlot.at(i)->setVisible(isShow);
	}
}


void LayerTCC_PopupMoQuy::startSlotWithIdx(int idx, string bettype, int time)
{
	//start from slot 1
	arrLayerSlot.at(idx)->runAction(Spawn::create(Show::create(), FadeIn::create(0.5), NULL));
	int idbet = getIdByBetType(bettype);
	arrLayerSlot.at(idx)->setRewardItem(idbet);
	arrLayerSlot.at(idx)->startTurn(time);
}


void LayerTCC_PopupMoQuy::slotMachineCallBack(Node* sender, void* data)
{
	int tag;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	tag = (int)data - 1000;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	tag = *((int*)data) - 1000;
#endif
	log("slotMachineCallBack %d", tag);
	if (arrResMQ.size() < 6)
	{
		log("can not run slot because res null");
		return;
	}

	switch (tag)
	{
	case 0://slot 0
	{
			   imgHeader->setTexture("giai-2.png");
			   startSlotWithIdx(1, arrResMQ.at(1).bettype, 10);
	}
		break;
	case 1:{
			   imgHeader->setTexture("giai-3.png");
			   startSlotWithIdx(2, arrResMQ.at(2).bettype, 10);
			   break;
	}
		break;
	case 2:{
			   imgHeader->setTexture("text-5.png");
			   startSlotWithIdx(3, arrResMQ.at(3).bettype, 1);
			   startSlotWithIdx(4, arrResMQ.at(4).bettype, 1);
			   startSlotWithIdx(5, arrResMQ.at(5).bettype, 1);
			   break;
	}
	default:
		break;
	}
	showResultGame(tag);
}

void LayerTCC_PopupMoQuy::showResultGame(int tag)
{
	arrLabelUser.at(tag)->setName(arrResMQ.at(tag).username);
	arrLabelUser.at(tag)->setTouchEnabled(true);
	arrLabelUser.at(tag)->setString(formatUserName(arrResMQ.at(tag).username, 18));
	arrLabelUser.at(tag)->runAction(Sequence::create(DelayTime::create(0.3), Show::create(), NULL));

	arrLabelMoney.at(tag)->setString(mUtils::convertMoneyEx(atoi(arrResMQ.at(tag).money.c_str())));
	arrLabelMoney.at(tag)->runAction(Sequence::create(DelayTime::create(0.3), Show::create(), NULL));
	if (tag == 5)
	{
		//show ket qua cuoi cung
		imgHeader->setTexture("text-6.png");
		spriteQuayThuong->stopAllActions();
		pnlQuayThuong->setVisible(false);
		pnlTimer->setVisible(false);
		gameStateRequest();
		listHisTop = "";
		listHisUser = "";
		// 		timeMoQuy = dataManager.getAppConfig().timeToQT; //tg bang 1 tuan
		// 		timeToNextWeek = timeMoQuy - dataManager.getAppConfig().timetoNextWeek;
		// 		this->startMoQuy();
	}
}


LayerTCC_PopupMoQuy::TimeMQ LayerTCC_PopupMoQuy::getDateFromInt(int _time)
{
	int _day = _time / (60 * 60 * 24);
	int _hour = (_time / (60 * 60)) % 24;
	int _min = (_time / (60)) % 60;
	int _se = _time % 60;
	return TimeMQ(_day, _hour, _min, _se);
}

void LayerTCC_PopupMoQuy::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void LayerTCC_PopupMoQuy::onButtonRank(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		LayerTCC_PopupXHMoQuy *layerXH = LayerTCC_PopupXHMoQuy::create();
		layerXH->setListRank(listHisTop, listHisUser);
		this->addChild(layerXH);
		//		startQuayThuong();
		// 		boost::shared_ptr<ISFSObject> params(new SFSObject());
		// 		params->PutUtfString(EXT_FIELD_GAME_HIS_QUY_THUONG,
		// 			"minhdv|CUA|40|100000;hoangdd|GA|20|10000;killer87|GA|20|10000;superman3|GA|5|5000;mrtuan2|GA|5|5000;kimnamcham|GA|5|5000");
		// 		this->event_EXT_EVENT_TCC_OPEN_QUY_THUONG_NTF(params);
	}
}
void LayerTCC_PopupMoQuy::onButtonRule(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
// 		boost::shared_ptr<ISFSObject> param(new SFSObject());
// 		param->PutUtfString(EXT_FIELD_GAME_HIS_QUY_THUONG, "");
// 		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_OPEN_QUY_THUONG_REQ, param));
// 		GameServer::getSingleton().Send(request);
// 		return;
		auto currScene = Director::getInstance()->getRunningScene();
		if (currScene)
		{
			LayerPopupWebView* layerTut = LayerPopupWebView::create();
			layerTut->setWebView(LINK_EVENT_TCC);
			currScene->addChild(layerTut, ZORDER_LIST::ZORDER_POPUP_RANK);
		}
	}
}

void LayerTCC_PopupMoQuy::onButtonCheat(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		string strCheat = txtCheat->getString();
		if (0 != strCheat.compare(""))
		{
			//sample : hoangdd|CUA;minhdv|GA
			vector<string> arrStr = mUtils::splitString(strCheat, ';');
			if (arrStr.size() == 0)
			{
				SceneManager::getSingleton().showChatWithCurrentScene("Sai định dạng", -1);
				return;
			}
			string fakevl;
			int arrPer[6] = { 40, 20, 20, 5, 5, 5 };
			int arrPoint[6] = { 1000, 100, 100, 50, 50, 50 };
			for (int i = 0; i < arrStr.size(); i++)
			{
				vector<string> infos = mUtils::splitString(arrStr.at(i), '|');
				if (infos.size() == 2)
				{
					string username = infos.at(0);
					string bettype = infos.at(1);
					//std::transform(bettype.begin(), bettype.end(), bettype.begin(), toupper);
					fakevl.append(StringUtils::format("%s|%s|%d|%d;", username.c_str(), bettype.c_str(), arrPer[i], arrPoint[i]));
				}
			}
			if (0 == fakevl.compare(""))
			{
				SceneManager::getSingleton().showChatWithCurrentScene("Sai định dạng", -1);
				return;
			}
			boost::shared_ptr<ISFSObject> param(new SFSObject());
			//string fakevl = "minhdv|CUA|40|100000;hoangdd|GA|20|10000;killer87|TOM|20|10000;superman3|CA|5|5000;mrtuan2|BAU|5|5000;kimnamcham|HUOU|5|5000";
			param->PutUtfString(EXT_FIELD_GAME_HIS_QUY_THUONG, fakevl);
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_OPEN_QUY_THUONG_REQ, param));
			GameServer::getSingleton().Send(request);
		}

	}
}

void LayerTCC_PopupMoQuy::gameStateRequest()
{
	boost::shared_ptr<ISFSObject> param(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_GET_GAME_INFO_REQ, param));
	GameServer::getSingleton().Send(request);
}

void LayerTCC_PopupMoQuy::event_EXT_EVENT_TCC_GAME_STATE_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<long> toet_ptr = param->GetInt(EXT_FIELD_GAME_TIME_OPEN_EVENT);
	if (toet_ptr)
	{
		timeMoQuy = *toet_ptr;
		this->startMoQuy();
	}
}

void LayerTCC_PopupMoQuy::onLabelUserTouch(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Text* labelUser = (Text*)pSender;
		string uidFull = labelUser->getName();
		if (0 != uidFull.compare(""))
		{
			SceneManager::getSingleton().showChatWithCurrentScene(uidFull, -1);
		}
	}
}

