#include "LayerTCC_PopupHistory.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "mUtils.h"
#include "PhomMessDef.h"
#include "TCCMsgDef.h"
#include "Requests/ExtensionRequest.h"
#include "LayerTCC_PopupHistoryAdvance.h"

bool LayerTCC_ItemHistory::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_ItemHistory.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());

	Panel_1 = (Layout*)root->getChildByName("Panel_1");
	if (Panel_1)
	{
		Panel_1->setTouchEnabled(true);
		Panel_1->setSwallowTouches(false);
		lblMatchId = (Text*)Panel_1->getChildByName("lblMatchId");
		lblTime = (Text*)Panel_1->getChildByName("lblTime");
		lblBet = (Text*)Panel_1->getChildByName("lblBet");
		lblBetRoom = (Text*)Panel_1->getChildByName("lblBetRoom");
		lblReceived = (Text*)Panel_1->getChildByName("lblReceived");

		for (int i = 0; i < 3; i++){
			ImageView* imgDice = (ImageView*)Panel_1->getChildByName(StringUtils::format("imgDice%d", i + 1));
			if (imgDice)
				imgDices.push_back(imgDice);
		}
		Button* btnAdvance = (Button*)Panel_1->getChildByName("btnAdvance");
		btnAdvance->addTouchEventListener(CC_CALLBACK_2(LayerTCC_ItemHistory::onButtonAdvance, this));
	}
	return true;
}

void LayerTCC_ItemHistory::setDatas(ItemTCCHis item, ItemTCCHisAdvance listInfo)
{
	lstInfo = listInfo;
	matchID = item.matchid;
	lblMatchId->setString(StringUtils::format("#%d", matchID));
	string betTime = getTimeHistory(item.time);
	lblTime->setString(betTime);
	int betroom = atoi(item.betroom.c_str());
	//lblBetRoom->setString(mUtils::convertMoneyEx(betroom));
	lblBet->setString(formatMoneySymbol(item.bet));
	lblReceived->setString(formatMoneySymbol(item.receive));
	vector<string> arrRes = mUtils::splitString(item.res, '_');
	for (int i = 0; i < arrRes.size(); i++){
		int id = getIdByBetType(arrRes.at(i));
		string url = getImgDiceById(id);
		ImageView* imgDice = (ImageView*)Panel_1->getChildByName(StringUtils::format("imgDice%d", i + 1));
		if (imgDice)
			imgDice->loadTexture(url);
	}
}

void LayerTCC_ItemHistory::setBackGround(int _type)
{
//	string img = "bg-2-his.png";
//	if (_type % 2 != 0)
//		img = "bg-3-his.png";
//	Panel_1->setBackGroundImage(img);
}

void LayerTCC_ItemHistory::onButtonAdvance(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		Scene* currScene = Director::getInstance()->getRunningScene();
		if (currScene)
		{
			LayerTCC_PopupHistoryAdvance *layerHisAdvance = LayerTCC_PopupHistoryAdvance::create();
			string res = lstInfo.res;
			string listinfo = lstInfo.info;
			layerHisAdvance->setDataHis(res, listinfo);
			currScene->addChild(layerHisAdvance, ZORDER_LIST::ZORDER_POPUP_RANK);
		}
	}
}

std::string LayerTCC_ItemHistory::getTimeHistory(string str)
{
	time_t tt = atoi(str.c_str());
	struct tm * ptm = localtime(&tt);
	char buf[30];
	strftime(buf, 30, "%d/%m/%Y\n%H:%M:%S", ptm);
	return string(buf);
}

LayerTCC_PopupHistory::LayerTCC_PopupHistory()
{
	GameServer::getSingleton().addListeners(this);
	SceneManager::getSingleton().showLoading();
	boost::shared_ptr<ISFSObject> param(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_USER_HIS_REQ, param));
	GameServer::getSingleton().Send(request);
}

LayerTCC_PopupHistory::~LayerTCC_PopupHistory()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerTCC_PopupHistory::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_PopupHistory.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());
	Layout* pnlMain = (Layout*)root->getChildByName("pnlMain");

	// 	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// 	int gameId = SceneManager::getSingleton().getGameID();
	// 	if (gameId == -1)
	// 	{
	// 		root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	// 		root->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	// 	}

	if (pnlMain)
	{
		Button* btnClose = (Button*)pnlMain->getChildByName("btnClose");
		if (btnClose)
		{
			btnClose->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupHistory::onBtnClose, this));
		}

		Layout* pnlTable = (Layout*)pnlMain->getChildByName("pnlTable");
		if (pnlTable){
			tblHis = TableView::create(this, pnlTable->getContentSize());
			tblHis->setAnchorPoint(Vec2::ZERO);
			tblHis->setDirection(TableView::Direction::VERTICAL);
			tblHis->setPosition(Vec2::ZERO);
			tblHis->setDelegate(this);
			tblHis->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
			tblHis->setBounceable(true);
			pnlTable->addChild(tblHis);
		}
	}

	return true;
}

void LayerTCC_PopupHistory::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{

}

void LayerTCC_PopupHistory::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{

}

void LayerTCC_PopupHistory::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

cocos2d::Size LayerTCC_PopupHistory::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return Size(1142, 88);
}

cocos2d::extension::TableViewCell* LayerTCC_PopupHistory::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	auto cell = table->dequeueCell();
	int matchid = lstHis.at(idx).matchid;
	ItemTCCHisAdvance listInfo = getInfoByMatchID(matchid);
	if (!cell)
	{
		cell = new TableViewCell();
		LayerTCC_ItemHistory * hisCell = LayerTCC_ItemHistory::create();
		hisCell->setPosition(Vec2::ZERO);
		hisCell->setTag(123);
		hisCell->setBackGround(idx);
		hisCell->setDatas(lstHis.at(idx), listInfo);
		cell->addChild(hisCell);
	}
	else
	{
		LayerTCC_ItemHistory* hisCell = (LayerTCC_ItemHistory*)cell->getChildByTag(123);
		if (hisCell)
		{
			hisCell->setBackGround(idx);
			hisCell->setDatas(lstHis.at(idx), listInfo);
		}
	}
	return cell;
}

ssize_t LayerTCC_PopupHistory::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return lstHis.size();
}

void LayerTCC_PopupHistory::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (0 == cmd->compare(EXT_EVENT_TCC_USER_HIS_RESP)){
		this->event_EXT_EVENT_TCC_USER_HIS_RESP(param);
	}
}

void LayerTCC_PopupHistory::event_EXT_EVENT_TCC_USER_HIS_RESP(boost::shared_ptr<ISFSObject> param)
{
	SceneManager::getSingleton().hideLoading();
	boost::shared_ptr<string> ghs_ptr = param->GetUtfString(EXT_FIELD_GAME_HISTORY_RESULT);
	if (ghs_ptr)
	{
		if (lstHis.size() > 0)
			return;

		vector<string> arrPhien = mUtils::splitString(*ghs_ptr, '#');
		for (int i = 0; i < arrPhien.size(); i++)
		{
			vector<string> arrInfo;
			vector<string> arrInfoBet;

			int matchId;
			string time_phien;
			string res;
			string bettime;
			string bettype;
			string betRoom;
			string betvl;
			string betrecieve;
			string listInfo;
			vector<string> arrHis = mUtils::splitString(arrPhien.at(i), '@');
			int totalbetvl = 0;
			int totalbetreceive = 0;

			for (int k = 0; k < arrHis.size(); k++)
			{
				arrInfo = mUtils::splitString(arrHis.at(k), '|');
				arrInfoBet = mUtils::splitString(arrInfo.at(3), ';');
				if (arrInfo.size() > 3)
				{
					matchId = atoi(arrInfo.at(0).c_str());
					time_phien = arrInfo.at(1);
					res = arrInfo.at(2);
				}
				if (arrInfoBet.size() > 6)
				{
					bettime = atoi(arrInfoBet.at(1).c_str());
					bettype = arrInfoBet.at(2);
					betvl = arrInfoBet.at(3).c_str();
					betrecieve = arrInfoBet.at(5).c_str();
					betRoom = arrInfoBet.at(6);
				}
				listInfo.append(bettype + ";" + betvl + ";" + betrecieve + "|");
				totalbetvl += atoi(betvl.c_str());
				totalbetreceive += atoi(betrecieve.c_str());
			}

			lstDetail.push_back(ItemTCCHisAdvance(matchId, listInfo, res));
			lstHis.push_back(ItemTCCHis(matchId, time_phien, betRoom, totalbetvl, totalbetreceive, res));
			tblHis->reloadData();
		}
	}
}

void LayerTCC_PopupHistory::onBtnClose(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

ItemTCCHisAdvance LayerTCC_PopupHistory::getInfoByMatchID(int matchid)
{
	for (int i = 0; i < lstDetail.size(); i++) {
		if (lstDetail[i].matchid == matchid) {
			return lstDetail[i];
		}
	}
}
