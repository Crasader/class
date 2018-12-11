#include "LayerPopupRankGame.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Requests/PublicMessageRequest.h"
#include "Entities/Data/SFSObject.h"
#include "Requests/ExtensionRequest.h"
#include "LayerRankContent.h"
#include "mUtils.h"
#include "layergames/PhomMessDef.h"
#define TAG_LEFT_CELL 246
#define TAG_RIGHT_CELL 247
LayerPopupRankGame::LayerPopupRankGame()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupRankGame::~LayerPopupRankGame()
{
	GameServer::getSingleton().removeListeners(this);
}

void LayerPopupRankGame::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupRankGame::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(LayerPopupRankGame::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LayerPopupRankGame::onExit()
{
	Layer::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool LayerPopupRankGame::init()
{
	if (!Layer::init())
		return false;
	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupRankGame.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	panelTable = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	this->setContentSize(panelTable->getContentSize());

	tblRankGame = TableView::create(this, Size(panelTable->getContentSize().width, panelTable->getContentSize().height - 50));
	tblRankGame->setAnchorPoint(panelTable->getAnchorPoint());
	tblRankGame->setContentSize(panelTable->getContentSize());
	tblRankGame->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	tblRankGame->setPosition(Vec2(panelTable->getPositionX(), panelTable->getPositionY() + 50));
	tblRankGame->setDelegate(this);
	tblRankGame->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tblRankGame);

	int gameID = SceneManager::getSingleton().getGameID();
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt("gid", gameID);
	boost::shared_ptr<IRequest> request(new ExtensionRequest("rahbg", params));
	GameServer::getSingleton().Send(request);

	return true;
}
void LayerPopupRankGame::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

cocos2d::Size LayerPopupRankGame::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return Size(520, 120);
}

cocos2d::extension::TableViewCell* LayerPopupRankGame::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return createCell(table, idx);
}

ssize_t LayerPopupRankGame::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return lstTops.size();
}

bool LayerPopupRankGame::onTouchBegan(Touch *touch, Event *event)
{
	return true;
}

void LayerPopupRankGame::onTouchEnded(Touch *touch, Event *event)
{
	auto locationInNode = this->convertToNodeSpace(touch->getLocation());
	auto s = this->getContentSize();
	auto rect = Rect(0, 0, s.width, s.height);
	if (!rect.containsPoint(locationInNode))
	{
		this->removeFromParentAndCleanup(true);
	}
}

void LayerPopupRankGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	log("OnExtensionResponse cmd = %s", cmd->c_str());
	if (strcmp("rahbg", cmd->c_str()) == 0){
		SceneManager::getSingleton().hideLoading();
		lstTops.clear();
		boost::shared_ptr<string> rahbg_ptr = param->GetUtfString("rahbg");
		if (rahbg_ptr == NULL) return;
		vector<string> rankMoneys = mUtils::splitString(*rahbg_ptr, '|');
		for (int i = 0; i < rankMoneys.size(); i++)
		{
			infoUser _i;
			vector<string> infos = mUtils::splitString(rankMoneys.at(i), ',');
			for (int j = 0; j < infos.size(); j++)
			{
				_i.userName = infos.at(0);
				_i.userLv = infos.at(1);
				
				double a = atol(infos.at(2).c_str());
				double b = atol(infos.at(3).c_str());
				double c = a - b > 0 ? a - b : 0;
				_i.userMoney = formatMoneySymbol(c);
				//_i.userRateWin = "";
			}
			lstTops.push_back(_i);
		}
		tblRankGame->reloadData();
		return;
	}
}

TableViewCell* LayerPopupRankGame::createCell(TableView *table, int idx)
{
	TableViewCell* cell = table->dequeueCell();
	try{
		if (!cell)
		{
			cell = new TableViewCell();
			LayerItemPopupRank* rankCell = LayerItemPopupRank::create();
			rankCell->setPosition(Vec2::ZERO);
			rankCell->setTag(TAG_RIGHT_CELL);
			rankCell->setDatas(idx, this->lstTops.at(idx));
			cell->addChild(rankCell);
		}
		else
		{
			LayerItemPopupRank* rankCell = (LayerItemPopupRank*)cell->getChildByTag(TAG_RIGHT_CELL);
			rankCell->setDatas(idx, this->lstTops.at(idx));
		}
	}
	catch (std::out_of_range& e) {
		log("%s", e.what());
	}
	catch (...) {

	}
	return cell;
}
LayerItemPopupRank::LayerItemPopupRank() : circleAvatarItem(NULL) {

}

LayerItemPopupRank::~LayerItemPopupRank(){
	log("deconstructor");
}

bool LayerItemPopupRank::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto rootNode = CSLoader::getInstance()->createNode("LayerItemPopupRank.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	this->txtName = static_cast<Text*>(rootNode->getChildByName("lblName"));
	this->txtMoney = static_cast<Text*>(rootNode->getChildByName("lblTop"));
	this->txtLevel = static_cast<Text*>(rootNode->getChildByName("lblLevel"));
	this->lblTop = dynamic_cast<Text*>(rootNode->getChildByName("lblPos"));
	this->spriteAvatar = dynamic_cast<Sprite*>(rootNode->getChildByName("spriteAvatar"));
	this->avtVip = static_cast<ImageView*>(rootNode->getChildByName("imgAvatarVip"));
	this->avtVip->setVisible(false);

	ImageView* img = static_cast<ImageView*>(rootNode->getChildByName("imgAvatar"));
	if (img != NULL){
		this->circleAvatarItem = CircleAvatar::create();
		this->circleAvatarItem->hideIconDefault(false);
		float scX = img->getScaleX();
		float scY = img->getScaleY();
		circleAvatarItem->displayTypeVip(false);
		Size _sizeImg = img->getContentSize();
		Size _sizeAva = this->circleAvatarItem->GetSizeAvatar();

		scX = _sizeImg.width / _sizeAva.width * scX;
		scY = _sizeImg.height / _sizeAva.height * scY;

		this->circleAvatarItem->setAnchorPoint(Vec2::ZERO);
		this->circleAvatarItem->setScale(scX, scY);
		this->circleAvatarItem->setPosition(img->getPosition());
		rootNode->addChild(this->circleAvatarItem, 3);
		this->avtVip->setLocalZOrder(4);
	}

	return true;
}

void LayerItemPopupRank::setDatas(const int& index, const infoUser& data)
{
	lblTop->setString(StringUtils::format("%d", index + 1));
	this->txtName->setString(data.userName);
	this->txtMoney->setString(data.userMoney);
	this->txtLevel->setString("Lv. " + data.userLv);

	if (this->circleAvatarItem != NULL) {
		this->circleAvatarItem->setAvatarImage(data.userAvatar);
	}
}
