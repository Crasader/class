#include "LayerTCC_PopupRank.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "TCCMsgDef.h"
#include "Requests/ExtensionRequest.h"
#include "SceneManager.h"
#include "mUtils.h"

bool LayerTCC_ItemRank::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_ItemRank.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());

	Panel_1 = (Layout*)root->getChildByName("Panel_1");
	if (Panel_1)
	{
		Panel_1->setTouchEnabled(true);
		Panel_1->setSwallowTouches(false);
		lblNumber = (Text*)Panel_1->getChildByName("lblNumber");
		lblName = (Text*)Panel_1->getChildByName("lblName");
		lblBet = (Text*)Panel_1->getChildByName("lblBet");
	}
	return true;
}

void LayerTCC_ItemRank::setDatas(ItemTCCRank item)
{
	lblNumber->setString(StringUtils::format("%d", item.number));
	lblName->setString(item.name);
	lblBet->setString(mUtils::convertMoneyEx(item.bet));
}

void LayerTCC_ItemRank::setBackGround(int _type)
{
//	string img = "bg-2-rank.png";
//	if (_type % 2 != 0)
//		img = "bg-3-rank.png";
//	Panel_1->setBackGroundImage(img);
}

LayerTCC_PopupRank::LayerTCC_PopupRank()
{
	GameServer::getSingleton().addListeners(this);
	SceneManager::getSingleton().showLoading();
	boost::shared_ptr<ISFSObject> param(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_RANK_REQ, param));
	GameServer::getSingleton().Send(request);
}

LayerTCC_PopupRank::~LayerTCC_PopupRank()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerTCC_PopupRank::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_PopupRank.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());

// 	auto visibleSize = Director::getInstance()->getVisibleSize();
// 	Vec2 origin = Director::getInstance()->getVisibleOrigin();
// 	root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
// 	root->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	
	Layout* pnlMain = (Layout*)root->getChildByName("pnlMain");
	if (pnlMain)
	{
		Button* btnClose = (Button*)pnlMain->getChildByName("btnClose");
		if (btnClose)
		{
			btnClose->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupRank::onBtnClose, this));
		}

		Layout* pnlTable = (Layout*)pnlMain->getChildByName("pnlTable");
		if (pnlTable){
			tblRank = TableView::create(this, pnlTable->getContentSize());
			tblRank->setAnchorPoint(Vec2::ZERO);
			tblRank->setDirection(TableView::Direction::VERTICAL);
			tblRank->setPosition(Vec2::ZERO);
			tblRank->setDelegate(this);
			tblRank->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
			tblRank->setBounceable(true);
			pnlTable->addChild(tblRank);
		}
	}

	return true;
}

void LayerTCC_PopupRank::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{

}

void LayerTCC_PopupRank::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{

}

void LayerTCC_PopupRank::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

cocos2d::Size LayerTCC_PopupRank::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return Size(1142, 81);
}

cocos2d::extension::TableViewCell* LayerTCC_PopupRank::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	auto cell = table->dequeueCell();
	if (!cell)
	{
		cell = new TableViewCell();
		LayerTCC_ItemRank * hisCell = LayerTCC_ItemRank::create();
		hisCell->setPosition(Vec2::ZERO);
		hisCell->setTag(123);
		hisCell->setBackGround(idx);
		hisCell->setDatas(lstRank.at(idx));
		cell->addChild(hisCell);
	}
	else
	{
		LayerTCC_ItemRank* hisCell = (LayerTCC_ItemRank*)cell->getChildByTag(123);
		if (hisCell)
		{
			hisCell->setBackGround(idx);
			hisCell->setDatas(lstRank.at(idx));
		}
	}
	return cell;
}

ssize_t LayerTCC_PopupRank::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return lstRank.size();
}

void LayerTCC_PopupRank::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (0 == cmd->compare(EXT_EVENT_TCC_RANK_RESP)){
		this->event_EXT_EVENT_TCC_RANK_RESP(param);
	}
}

void LayerTCC_PopupRank::event_EXT_EVENT_TCC_RANK_RESP(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> ghtp_ptr = param->GetUtfString(EXT_FIELD_GAME_HISTORY_TOP_PLAYER);
	if (ghtp_ptr)
	{
		SceneManager::getSingleton().hideLoading();
		vector<string> listUser = mUtils::splitString(*ghtp_ptr, '|');
		lstRank.clear();
		for (int i = 0; i < listUser.size(); i++)
		{
			vector<string> userinfos = mUtils::splitString(listUser.at(i), ':');
			int number;
			string username;
			int betvl;
			for (int k = 0; k < userinfos.size(); k++)
			{
				if (userinfos.size() == 3)
				{
					number = i + 1;
					username = userinfos.at(0);
					betvl = atoi(userinfos.at(2).c_str());
				}
			}
			lstRank.push_back(ItemTCCRank(number, username, betvl));
		}
		tblRank->reloadData();
	}
}

void LayerTCC_PopupRank::onBtnClose(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
