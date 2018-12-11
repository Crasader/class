//
//  LayerPopupDoiDiem.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/6/16.
//
//

#include "LayerPopupDoiDiem.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupThongBao.h"

LayerPopupDoiDiem::LayerPopupDoiDiem()
{
	GameServer::getSingleton().addListeners(this);
}


LayerPopupDoiDiem::~LayerPopupDoiDiem()
{
	GameServer::getSingleton().removeListeners(this);

}

bool LayerPopupDoiDiem::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupDoiDiem.csb");
	auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	Size sizeAdd;
	if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
		sizeAdd = ChanUtils::getSizePos();

	}
	else{
		sizeAdd = Size(0, 0);
	}
	Size sizeAdd2;
	if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
		sizeAdd2 = ChanUtils::getSizePos();

	}
	else{
		sizeAdd2 = Size(0, 0);
	}
	rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);

	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
	//pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
	Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
	if (btnClose){
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiDiem::onBtnClose, this));
	}

	auto pnltbl = static_cast<Layout*>(pnlBg->getChildByName("pnlLstDoiThuong"));

	this->txtDiemHienTai = static_cast<Text*>(pnlBg->getChildByName("txtDiemHienTai"));
	this->txtDiemHienTai->setString("---");
	//
	pnltbl->setVisible(false);
	pnltbl->setSwallowTouches(false);
	this->tblListReward = TableView::create(this, Size(pnltbl->getContentSize().width, pnltbl->getContentSize().height - 10));
	this->tblListReward->setAnchorPoint(pnltbl->getAnchorPoint());
	this->tblListReward->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	this->tblListReward->setPosition(Vec2(pnltbl->getPosition().x + sizeAdd.width, pnltbl->getPosition().y + 10));
	this->tblListReward->setDelegate(this);
	this->tblListReward->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	pnlBg->addChild(this->tblListReward);

	//    boost::shared_ptr<ISFSObject> params(new SFSObject());
	//    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PK_USER_HIS_REQ,params));
	//    GameServer::getSingleton().Send(request);
	//
	//
	//    SceneManager::getSingleton().showLoading();

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("eeglrprq", params));
	GameServer::getSingleton().Send(request);



	boost::shared_ptr<ISFSObject> params1(new SFSObject());
	boost::shared_ptr<IRequest> request1(new ExtensionRequest("eegprq", params1));
	GameServer::getSingleton().Send(request1);

	SceneManager::getSingleton().showLoading();

	return true;
}

void LayerPopupDoiDiem::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerPopupDoiDiem::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(1390, 300);
}

TableViewCell* LayerPopupDoiDiem::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto cell = table->dequeueCell();
	auto _cTag = 123;
	if (!cell) {
		cell = TableViewCell::create();

	}
	else {
		cell->removeAllChildren();
	}

	if (idx < 0)
		return cell;


	auto cols = 4;
	auto sizeTable = Size(1390, 480);
	auto width = 289;
	int space = 60;
	auto startX = (sizeTable.width - (width)*cols - space * (cols - 1)) / 2;

	//comment
	for (int i = 0; i < cols; i++) {
		int index = idx *cols + i;
		if (index < this->lstrw.size())
		{
			auto _child = LayerItemDoiDiem::create();
			_child->setAnchorPoint(Vec2(0, 0));
			_child->setPosition(startX + (i * width) + space * i, 5);
			_child->setDatas(this->lstrw[index].diem, this->lstrw[index].desc, this->lstrw[index].id, this->lstrw[index].type, this->lstrw[index].value);
			cell->addChild(_child, 0, _cTag + i);
			_child->setVisible(true);
			_child->setTag(index);
		}
	}
	return cell;

}
int LayerPopupDoiDiem::getCell(){
	int i = this->lstrw.size();
	if (i == 0)
		return 0;
	if (i < 4)
		return 1;
	int rows = i / 4;
	if (i % 4 != 0)
		rows += 1;
	return rows;
}
ssize_t LayerPopupDoiDiem::numberOfCellsInTableView(TableView *table)
{

	return getCell();//lstRank.size();
}
void LayerPopupDoiDiem::onBtnClose(Ref* pSender){
	this->removeFromParentAndCleanup(true);
}
void LayerPopupDoiDiem::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	if (strcmp("eeglrprsp", cmd->c_str()) == 0){
		// id item | Description | so diem hoat dong de doi | type | _value
		//type = 1: Thưởng silver, type = 2,3: x2 kinh nghiệm, type = 5: Khuyến mãi % nạp thẻ., type = 4: khung vien avatar
		SceneManager::getSingleton().hideLoading();
		if (param->GetUtfString("lrp") == NULL) return;
		string str = *param->GetUtfString("lrp");

		auto lst = mUtils::splitString(str, ';');
		this->lstrw.clear();
		for (int i = 0; i < lst.size(); i++){

			auto lstit = mUtils::splitString(lst[i], '|');
			if (lstit.size() < 2)
				continue;
			itemrw it;
			it.id = atoi(lstit[0].c_str());
			it.desc = lstit[1].c_str();
			it.diem = atoi(lstit[2].c_str());
			it.type = atoi(lstit[3].c_str());
			it.value = atoi(lstit[4].c_str());
			if (it.diem == 0)
				continue;
			this->lstrw.push_back(it);


		}
		this->tblListReward->reloadData();

	}
	else if (strcmp("eeepfrrsp", cmd->c_str()) == 0){

		SceneManager::getSingleton().hideLoading();

		if (param->GetInt("rc") == NULL) return;
		int rc = *param->GetInt("rc");

		if (rc == 0){
			if (param->GetInt("rv") == NULL) return;
			int rv = *param->GetInt("rv");

			auto _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setMessage(dataManager.GetSysString(141));
			layerPopup->setContentMess(StringUtils::format("Bạn đã đổi thành công !"));
			layerPopup->setTag(24);
			layerPopup->showAButton();
			_currScene->addChild(layerPopup, 1000000,9060);

		}
		else{
			if (param->GetUtfString("rd") == NULL) return;
			string rd = *param->GetUtfString("rd");
			auto _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setMessage(dataManager.GetSysString(141));
			layerPopup->setContentMess(rd);
			layerPopup->setTag(24);
			layerPopup->showAButton();
			_currScene->addChild(layerPopup, 1000000,9061);
		}

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> request(new ExtensionRequest("eegprq", params));
		GameServer::getSingleton().Send(request);

		SceneManager::getSingleton().showLoading();
	}
	else if (strcmp("eegprsp", cmd->c_str()) == 0){

		SceneManager::getSingleton().hideLoading();

		if (param->GetInt("pbw") == NULL) return;
		int pbw = *param->GetInt("pbw");

		this->txtDiemHienTai->setString(mUtils::convertMoneyEx(pbw));

	}

}

bool LayerPopupDoiDiem::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupDoiDiem::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupDoiDiem::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
	//
	//    GameServer::getSingleton().addListeners(this);
}

void LayerPopupDoiDiem::onExit()
{
	Layer::onExit();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	//    GameServer::getSingleton().removeListeners(this);
	//    Layer::onExit();
}
bool LayerItemDoiDiem::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto root = CSLoader::getInstance()->createNode("LayerItemDoiDiem.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);

	this->setContentSize(root->getContentSize());

	this->btnDoiDiem = static_cast<Button*>(root->getChildByName("btnDoiDiem"));
	this->txtDescription = static_cast<Text*>(root->getChildByName("txtBac"));
	this->txtDiem = static_cast<Text*>(root->getChildByName("txtDiem"));
	this->imgCoin = (ImageView*)root->getChildByName("imgCoin");
	this->imgItem = (ImageView*)root->getChildByName("imgItem");
	this->lbmPercent = (TextBMFont*)root->getChildByName("lbmPercent");

	this->idItem = 0;
	if (this->btnDoiDiem)
		this->btnDoiDiem->addClickEventListener(CC_CALLBACK_1(LayerItemDoiDiem::onBtnDoiDiem, this));

	return true;
}

void LayerItemDoiDiem::setDatas(int diem, string _desc, int id, int type, int value)
{
	this->idItem = id;
	//type = 1: Thưởng silver, type = 2,3: x2 kinh nghiệm, type = 4: Khuyến mãi % nạp thẻ.

	this->btnDoiDiem->setTitleText(StringUtils::format("%s điểm", mUtils::convertMoneyEx(diem).c_str()));
	if (type == 1)
	{
		this->imgCoin->setVisible(true);
		this->imgItem->setVisible(true);
		string pathImage = "moc3-icon-hao.png";
		/*if (value > 5000 && value < 10000)
		{
			pathImage = "moc3-icon-hao.png";
		}
		else if (value >= 10000 && value < 50000)
		{
			pathImage = "moc3-icon-hao.png";
		}
		else if (value >= 50000)
		{
			pathImage = "moc3-icon-hao.png";
		}
		else if (value <= 5000)
			pathImage = "moc3-icon-hao.png";
*/
		this->imgItem->loadTexture(pathImage);
		this->lbmPercent->setVisible(false);

		this->txtDescription->setFontSize(30);
		this->txtDescription->setPosition(Vec2(168, 248));
		this->txtDescription->setAnchorPoint(Vec2(1, 0.5));
		this->txtDescription->setTextHorizontalAlignment(TextHAlignment::RIGHT);
		this->txtDescription->setContentSize(Size(200, 40));
		this->txtDescription->setString(StringUtils::format("%s", mUtils::convertMoneyEx(value).c_str()));
	}
	else if (type == 2 || type == 3)
	{
		this->imgItem->setVisible(true);
		this->imgCoin->setVisible(false);
		this->imgItem->loadTexture("bt_42.png");
		this->lbmPercent->setVisible(false);

		this->txtDescription->setFontSize(25);
		this->txtDescription->setPosition(Vec2(142, 246));
		this->txtDescription->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->txtDescription->setTextHorizontalAlignment(TextHAlignment::CENTER);
		this->txtDescription->setContentSize(Size(250, 100));
		this->txtDescription->setString(_desc);
	}
	else if (type == 5)
	{
		int km_value = atoi(_desc.c_str());
		this->imgCoin->setVisible(false);
		this->imgItem->setVisible(false);
		this->lbmPercent->setVisible(true);
		this->lbmPercent->setString(StringUtils::format("%d%%", value));

		this->txtDescription->setFontSize(25);
		this->txtDescription->setPosition(Vec2(142, 246));
		this->txtDescription->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->txtDescription->setTextHorizontalAlignment(TextHAlignment::CENTER);
		this->txtDescription->setContentSize(Size(250, 100));
		this->txtDescription->setString(_desc);
	}
	else
	{
		this->imgItem->setVisible(false);
		this->lbmPercent->setVisible(false);
		this->imgCoin->setVisible(false);
		this->txtDescription->setFontSize(30);
		this->txtDescription->setPosition(Vec2(142, 200));
		this->txtDescription->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->txtDescription->setTextHorizontalAlignment(TextHAlignment::CENTER);
		this->txtDescription->setContentSize(Size(200, 100));
		this->txtDescription->setString(_desc);
	}
}

void LayerItemDoiDiem::onBtnDoiDiem(Ref* pSender){
	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return;
	auto node = (Node*)(pSender);
	int tag = node->getTag();
	LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
	layerPopup->setCallBackFromButton(this, callfuncO_selector(LayerItemDoiDiem::popUpCallBack), tag);
	layerPopup->setMessage(dataManager.GetSysString(141));
	layerPopup->setContentMess("Bạn có chắc chắn muốn đổi không?");
	layerPopup->setTag(24);
	_currScene->addChild(layerPopup, 1000000,9063);
}
void LayerItemDoiDiem::popUpCallBack(Ref* pSender){
	auto node = (Node*)(pSender);
	auto _currScene = Director::getInstance()->getRunningScene();
	if (_currScene)
		_currScene->removeChildByTag(24);
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt("idr", this->idItem);
	boost::shared_ptr<IRequest> request(new ExtensionRequest("eeepfrrq", params));
	GameServer::getSingleton().Send(request);

	SceneManager::getSingleton().showLoading();
}
