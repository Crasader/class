#include "LayerTCC_PopupHistoryAdvance.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "PhomMessDef.h"
#include "TCCMsgDef.h"
#include "ChanUtils.h"

bool LayerTCC_ItemHistoryAdvance::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_ItemHistoryAdvance.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());

	Panel_1 = (Layout*)root->getChildByName("Panel_1");
	if (Panel_1)
	{
		Panel_1->setTouchEnabled(true);
		Panel_1->setSwallowTouches(false);
		lblPos = (Text*)Panel_1->getChildByName("lblPos");
		lblBet = (Text*)Panel_1->getChildByName("lblBet");
		lblWin = (Text*)Panel_1->getChildByName("lblWin");
	}
	return true;
}

void LayerTCC_ItemHistoryAdvance::setDatas(ItemTCCHis_Ad item)
{
	string bettype = getNameByBetType(item.bettype);
	lblPos->setString(bettype);
	lblBet->setString(mUtils::convertMoneyEx(item.betvl));
	lblWin->setString(mUtils::convertMoneyEx(item.win));
}

void LayerTCC_ItemHistoryAdvance::setBackGround(int _type)
{
//	string img = "bg-2-his_ad.png";
//	if (_type % 2 != 0)
//		img = "bg-3-his_ad.png";
//	Panel_1->setBackGroundImage(img);
}


LayerTCC_PopupHistoryAdvance::LayerTCC_PopupHistoryAdvance()
{

}

LayerTCC_PopupHistoryAdvance::~LayerTCC_PopupHistoryAdvance()
{

}

bool LayerTCC_PopupHistoryAdvance::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_PopupHistoryAdvance.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());

	Size sizeAdd;
	sizeAdd = ChanUtils::getSizePos();
	// 	if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
	// 		//layerTCC->setPositionY(layerTCC->getPositionY() - sizeAdd.height);
	// 	}
	// 	else{
	this->setPositionY(this->getPositionY() + sizeAdd.height);

	//}

	Layout* pnlMain = (Layout*)root->getChildByName("pnlMain");
	if (pnlMain)
	{
		Button* btnClose = (Button*)pnlMain->getChildByName("btnClose");
		if (btnClose)
			btnClose->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupHistoryAdvance::onBtnClose, this));
		Button* btnBack = (Button*)pnlMain->getChildByName("btnBack");
		if (btnBack)
			btnBack->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupHistoryAdvance::onBtnClose, this));

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
		for (int i = 0; i < 3; i++){
			ImageView* img = (ImageView*)pnlMain->getChildByName(StringUtils::format("imgDice%d", i + 1));
			if (img)
				lstImg.push_back(img);
		}
	}

	return true;
}

void LayerTCC_PopupHistoryAdvance::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{

}

void LayerTCC_PopupHistoryAdvance::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{

}

void LayerTCC_PopupHistoryAdvance::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

cocos2d::Size LayerTCC_PopupHistoryAdvance::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return Size(1142, 81);
}

cocos2d::extension::TableViewCell* LayerTCC_PopupHistoryAdvance::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	auto cell = table->dequeueCell();
	if (!cell)
	{
		cell = new TableViewCell();
		LayerTCC_ItemHistoryAdvance * hisCell = LayerTCC_ItemHistoryAdvance::create();
		hisCell->setPosition(Vec2::ZERO);
		hisCell->setTag(123);
		hisCell->setBackGround(idx);
		hisCell->setDatas(lstHis.at(idx));
		cell->addChild(hisCell);
	}
	else
	{
		LayerTCC_ItemHistoryAdvance* hisCell = (LayerTCC_ItemHistoryAdvance*)cell->getChildByTag(123);
		if (hisCell)
		{
			hisCell->setBackGround(idx);
			hisCell->setDatas(lstHis.at(idx));
		}
	}
	return cell;
}

ssize_t LayerTCC_PopupHistoryAdvance::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return lstHis.size();
}

void LayerTCC_PopupHistoryAdvance::onBtnClose(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void LayerTCC_PopupHistoryAdvance::setDataHis(string resStr, string listInfo)
{
	vector<string> arrRes = mUtils::splitString(resStr, '_');
	for (int i = 0; i < arrRes.size(); i++){
		int id = getIdByBetType(arrRes.at(i));
		string url = getImgDiceById(id);
		lstImg.at(i)->loadTexture(url);
	}

	vector<string> arrListInfo = mUtils::splitString(listInfo, '|');
	for (int i = 0; i < arrListInfo.size(); i++){
		vector<string> arrInfo = mUtils::splitString(arrListInfo.at(i), ';');
		if (arrInfo.size() >= 3)
		{
			string bettype = arrInfo.at(0);
			int betvl = atoi(arrInfo.at(1).c_str());
			int betrev = atoi(arrInfo.at(2).c_str());
			lstHis.push_back(ItemTCCHis_Ad(bettype, betvl, betrev));
		}
	}
	tblHis->reloadData();
}
