#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../scenes/SceneMain.h"
#include "LayerFriendMain.h"
#include "Requests/Buddylist/InitBuddyListRequest.h"
#include "Requests/ExtensionRequest.h"
#include "../mUtils.h"
#include "../AllData.h"
#include "Requests/Buddylist/RemoveBuddyRequest.h"
#include "../layergames/_Chat_.h"
#include "SceneManager.h"
#include "../layers/LayerSendMessage.h"
#include "../layergames/ChanUtils.h"
using namespace ui;
using namespace Sfs2X;

LayerFriendMain::LayerFriendMain() :
lblName(NULL),
lblXu(NULL),
lblLevel(NULL),
lblOnlineStatus(NULL)
{
	GameServer::getSingleton().addListeners(this);
}

LayerFriendMain::~LayerFriendMain()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerFriendMain::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	rootNode = CSLoader::getInstance()->createNode("LayerFriendLists.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
	//define Node in csb
	//left
	auto headerLeft = static_cast<ImageView*>(rootNode->getChildByName("Image_1"));
	auto titleHeaderLeft = static_cast<Text*>(rootNode->getChildByName("Text_1"));
	nodeFriends = static_cast<Sprite*>(rootNode->getChildByName("Sprite_2"));
	auto pnlList = static_cast<Layout*>(rootNode->getChildByName("Panel_5"));

	//right
	auto headerRight = static_cast<ImageView*>(rootNode->getChildByName("Image_3"));
	auto titleHeaderRight = static_cast<Text*>(rootNode->getChildByName("txtName_0"));
	lblName = static_cast<Text*>(rootNode->getChildByName("lblName"));
	auto bgRight = static_cast<Layout*>(rootNode->getChildByName("Panel_4"));

	spriteTT = static_cast<Sprite*>(rootNode->getChildByName("imgStatus"));
	lblOnlineStatus = static_cast<Text*>(rootNode->getChildByName("lblStatus"));
	spriteAvatar = dynamic_cast<Sprite*>(rootNode->getChildByName("imgAvatar"));
	auto sprCoin = static_cast<Sprite*>(rootNode->getChildByName("Sprite_10"));
	lblXu = static_cast<Text*>(rootNode->getChildByName("lblTien"));
	auto sprExp = static_cast<Sprite*>(rootNode->getChildByName("spriteExp"));
	lblLevel = static_cast<Text*>(rootNode->getChildByName("lblLevel"));
	ldbLevel = static_cast<LoadingBar*>(rootNode->getChildByName("ldbLevel"));

	btnMessage = static_cast<ui::Button*>(rootNode->getChildByName("btnMessage"));
	btnUnFriend = static_cast<ui::Button*>(rootNode->getChildByName("btnUnFriend"));
	nodeHistorys = static_cast<Layout*>(rootNode->getChildByName("pnlHistories"));

	/*xu ly multi*/
	//Left
	nodeFriends->setAnchorPoint(Point(0, 1));
	nodeFriends->setPosition(Point(BORDER_TOP_MENU * ratio, visibleSize.height - TOP_HEIGHT * ratio));
	headerLeft->setAnchorPoint(Point(0, 1));
	headerLeft->setPosition(Point(nodeFriends->getPosition()));
	titleHeaderLeft->setAnchorPoint(Point(0.5, 0.5));
	titleHeaderLeft->setPosition(Point(headerLeft->getPositionX() + headerLeft->getContentSize().width / 2,
		headerLeft->getPositionY() - headerLeft->getContentSize().height / 2));
	pnlList->setPosition(Point(headerLeft->getPositionX(), headerLeft->getPositionY() - headerLeft->getContentSize().height));
	//Right
	bgRight->setAnchorPoint(Point(0, 1));
	bgRight->setPosition(Point(nodeFriends->getPositionX() + nodeFriends->getContentSize().width + BORDER_TOP_MENU * 2 * ratio,
		nodeFriends->getPositionY()));
	headerRight->setAnchorPoint(Point(0, 1));
	headerRight->setPosition(Point(bgRight->getPositionX(),
		bgRight->getPositionY()));
	lblName->setAnchorPoint(Point(0.5, 0.5));
	lblName->setPosition(Point(headerRight->getPositionX() + lblName->getContentSize().width / 2,
		headerRight->getPositionY() - headerRight->getContentSize().height / 2));

	titleHeaderRight->setAnchorPoint(Point(0.5, 0.5));
	titleHeaderRight->setPosition(Point(lblName->getPositionX() + lblName->getContentSize().width / 2 + titleHeaderRight->getContentSize().width / 2,
		lblName->getPositionY()));

	spriteTT->setAnchorPoint(Point(0.5, 0.5));
	spriteTT->setPosition(Point(headerRight->getPositionX() + BORDER_TOP_MENU * ratio,
		headerRight->getPositionY() - headerRight->getContentSize().height - BORDER_TOP_MENU * ratio));
	lblOnlineStatus->setAnchorPoint(Point(0, 0.5));
	lblOnlineStatus->setPosition(Point(spriteTT->getPositionX() + spriteTT->getContentSize().width,
		spriteTT->getPositionY()));

	spriteAvatar->setAnchorPoint(Point(0.5, 0.5));
	spriteAvatar->setPosition(Point(lblName->getPositionX(), spriteTT->getPositionY() - (spriteAvatar->getContentSize().height / 2 + BORDER_TOP_MENU * ratio)));

	sprCoin->setAnchorPoint(Point(0.5, 0.5));
	sprCoin->setPosition(Point(spriteAvatar->getPositionX() - spriteAvatar->getContentSize().width / 2,
		spriteAvatar->getPositionY() - spriteAvatar->getContentSize().height));

	lblXu->setAnchorPoint(Point(0, 0.5));
	lblXu->setPosition(Point(sprCoin->getPositionX() + sprCoin->getContentSize().width, sprCoin->getPositionY()));

	sprExp->setAnchorPoint(Point(0.5, 0.5));
	sprExp->setPosition(Point(spriteAvatar->getPositionX(), sprCoin->getPositionY() - sprCoin->getContentSize().height));

	ldbLevel->setAnchorPoint(Point(0, 0.5));
	ldbLevel->setPosition(Point(sprExp->getPositionX() - sprExp->getContentSize().width / 2, sprExp->getPositionY()));

	lblLevel->setAnchorPoint(Point(0, 0.5));
	lblLevel->setPosition(Point(ldbLevel->getPositionX(), ldbLevel->getPositionY() - sprCoin->getContentSize().height));

	btnMessage->setAnchorPoint(Point(0.5, 0.5));
	btnMessage->setPosition(Point(spriteAvatar->getPositionX(), lblLevel->getPositionY() - BORDER_TOP_MENU * ratio));

	btnUnFriend->setAnchorPoint(Point(0.5, 0.5));
	btnUnFriend->setPosition(Point(spriteAvatar->getPositionX(), btnMessage->getPositionY() - btnMessage->getContentSize().height - BORDER_TOP_MENU * ratio));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//nodeFriends = dynamic_cast<Node*>(rootNode->getChildByName("Sprite_2"));
	//nodeHistorys = dynamic_cast<Layout*>(rootNode->getChildByName("pnlHistories"));

	Size _nodeFriendsSize = Size(nodeFriends->getContentSize().width, nodeFriends->getContentSize().height - 95);
	Size _nodeFriendDetailsSize = Size(nodeHistorys->getContentSize().width, nodeHistorys->getContentSize().height - 95);

	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;

	log("pnlList height %f", pnlList->getPositionY());
	tblFriendList = TableView::create(this, _nodeFriendsSize);
	tblFriendList->setTag(tag_Friends);
	tblFriendList->setAnchorPoint(Point(0, 0));
	log("visibleSizeWidth %f visibleSizeHeight %f", visibleSize.width, visibleSize.height);
	log("AnchopointX %f AnchorpointY %f", nodeFriends->getAnchorPoint().x, nodeFriends->getAnchorPoint().y);
	tblFriendList->setContentSize(_nodeFriendsSize);
	tblFriendList->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	//tblFriendList->setPosition(Vec2(40,40));
	log("%fposX %f posY", headerLeft->getPositionX(), headerLeft->getPositionY());
	tblFriendList->setPosition(Point(headerLeft->getPositionX(),(60 + distance) * ratio));
	tblFriendList->setDelegate(this);
	tblFriendList->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tblFriendList);

	tblFriendDetailList = TableView::create(this, _nodeFriendDetailsSize);
	tblFriendDetailList->setAnchorPoint(Point(0, 0));
	tblFriendDetailList->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	tblFriendDetailList->setPosition(Vec2(970, (60 + distance) * ratio));
	tblFriendDetailList->setDelegate(this);
	tblFriendDetailList->setTag(tag_Historys);
	tblFriendDetailList->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tblFriendDetailList);
	tblFriendDetailList->reloadData();


	//btnMessage = dynamic_cast<Button*>(rootNode->getChildByName("btnMessage"));
	if (btnMessage != NULL) {
		btnMessage->addClickEventListener(CC_CALLBACK_1(LayerFriendMain::onButtonMessage, this));
	}
	auto btnTransferMoney = dynamic_cast<Button*>(rootNode->getChildByName("btnTransferMoney"));
	btnTransferMoney->setVisible(false);
	//if (btnTransferMoney != null) {
	//    btnTransferMoney.addTouchEventListener(this.onButtonTransferMoney, this);
	//    btnTransferMoney.setPressedActionEnabled(true);
	//}
	//btnUnFriend = dynamic_cast<Button*>(rootNode->getChildByName("btnUnFriend"));
	//btnUnFriend->setPosition(btnTransferMoney->getPosition());
	if (btnUnFriend != NULL) {
		btnUnFriend->addClickEventListener(CC_CALLBACK_1(LayerFriendMain::onButtonUnFriend, this));
	}

	//lblName = static_cast<Text*>(rootNode->getChildByName("lblName"));
	//lblOnlineStatus = static_cast<Text*>(rootNode->getChildByName("lblStatus"));
	//lblXu = static_cast<Text*>(rootNode->getChildByName("lblTien"));
	//lblLevel = static_cast<Text*>(rootNode->getChildByName("lblLevel"));
	//spriteAvatar = dynamic_cast<Sprite*>(rootNode->getChildByName("imgAvatar"));
	//spriteTT = dynamic_cast<Sprite*>(rootNode->getChildByName("imgStatus"));
	avtVip = static_cast<ImageView*>(rootNode->getChildByName("imgAvatarVip"));
	//ldbLevel = static_cast<LoadingBar*>(rootNode->getChildByName("ldbLevel"));
	avtVip->setVisible(false);
	if (!GameServer::getSingleton().getSmartFox()->BuddyManager()->Inited()){
		boost::shared_ptr<IRequest> request(new Buddylist::InitBuddyListRequest);
		GameServer::getSingleton().Send(request);
	}
	else {
		tblFriendList->reloadData();
		this->loadFirstFriend();
	}

    lstGameID.push_back(kGameChan);
    lstGameID.push_back(kGamePhom);
    lstGameID.push_back(kGameSam);
    lstGameID.push_back(kGameTLMNDemLa);
    lstGameID.push_back(kGameCoUp);
    lstGameID.push_back(kGameCoTuong);
    
	return true;
}

void LayerFriendMain::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", cell->getIdx());
 	if (table->getTag() == tag_Friends){
        SceneManager::getSingleton().showLoading();
 		CustomTableViewCell* cc = (CustomTableViewCell*)cell;
 		int fid = cc->getIdx();
 		string fname = cc->getStringObject();
 		//getFriend
 		boost::shared_ptr<vector<boost::shared_ptr<Buddy> > > lstBuddys = GameServer::getSingleton().getSmartFox()->BuddyManager()->BuddyList();
		if (lstBuddys == NULL || lstBuddys->size() == 0 || lstBuddys->at(cell->getIdx()) == NULL)
			return;
		auto _buddy = lstBuddys->at(cell->getIdx());
		for (int i = 0; i < lstBuddys->size(); i++)
		{
			CustomTableViewCell* cc = (CustomTableViewCell*)tblFriendList->cellAtIndex(i);
			if (cc == NULL)
				continue;
			LayerFriendCell* child = (LayerFriendCell*)cc->getChildByTag(123);
			child->bgPressed->setVisible(false);
		}
		CustomTableViewCell* c = (CustomTableViewCell*)tblFriendList->cellAtIndex(cell->getIdx());
		LayerFriendCell* child = (LayerFriendCell*)c->getChildByTag(123);
		child->bgPressed->setVisible(true);
		idxChoosed = cell->getIdx();
		//
		string _gs;
		if (_buddy->GetVariable("gs"))
			_gs = _buddy->GetVariable("gs")->GetStringValue()->c_str();
		string _status;
		if (_gs.length()>0)
		{
			auto _arr = mUtils::splitString(_gs, *",");
			if (_arr.size() == 2)
			{
				auto id = atoi(_arr.at(0).substr(0, 3).c_str());
				string str1 = "- ";
				auto str2 = _arr.at(1);
				_status = str1 + *mUtils::getGameNameByID(id)->c_str() + str2;
			}
		}
		//
		//auto id = 
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString("aI", _buddy->Name()->c_str());
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rgif", params));
		GameServer::getSingleton().Send(request);
		//Get history
		boost::shared_ptr<IRequest> request1(new ExtensionRequest("rghba", params));
		GameServer::getSingleton().Send(request1);

		currFriendID = fname;

		mCurrentRoomIndex = cell->getIdx();
		auto status = StringUtils::format("%s%s", (_buddy->IsOnline() ? "Online" : "Offline"), _status.c_str());
		lblOnlineStatus->setString(status);

		auto _src = "friend-status-offline.png";
		if (_buddy->IsOnline())
			_src = "friend-status-online.png";
		spriteTT->setTexture(_src);
	}
}

Size LayerFriendMain::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	if (table->getTag() == tag_Friends) {
		return Size(nodeFriends->getContentSize().width, 151);
	}
	return Size(930, 100);
}

TableViewCell* LayerFriendMain::tableCellAtIndex(TableView *table, ssize_t idx)
{
	if (table->getTag() == tag_Friends)
	{
		return createCell4Friends(table, idx);
	}

	return createCell4History(table, idx);

}

ssize_t LayerFriendMain::numberOfCellsInTableView(TableView *table)
{
	if (table->getTag() == tag_Friends){
		boost::shared_ptr<vector<boost::shared_ptr<Buddy> > > lstBuddys = GameServer::getSingleton().getSmartFox()->BuddyManager()->BuddyList();
		if (lstBuddys == NULL) return 0;
		return lstBuddys->size();
	}
	return lstHistorys.size();
}



TableViewCell* LayerFriendMain::createCell4History(TableView *table, int idx)
{
	auto cell = table->dequeueCell();
	if (!cell)
	{
		cell = new TableViewCell();
		auto detailcell = LayerFriendDetailCell::create();
		detailcell->setPosition(Vec2(0, 0));
		detailcell->setTag(123);
		detailcell->setDatas(lstHistorys[idx].ID, StringUtils::format("Thắng %d \ Thua %d", lstHistorys[idx].numOfWin, lstHistorys[idx].numOfLose));
		cell->addChild(detailcell);
	}
	else
	{
		auto detailCell = (LayerFriendDetailCell*)cell->getChildByTag(123);
		if (detailCell)
		{
			detailCell->setDatas(lstHistorys[idx].ID, StringUtils::format("Thắng %d \ Thua %d", lstHistorys[idx].numOfWin, lstHistorys[idx].numOfLose));
		}
	}

	return cell;
}

TableViewCell* LayerFriendMain::createCell4Friends(TableView *table, int idx)
{
	CustomTableViewCell* cell = (CustomTableViewCell*)table->dequeueCell();
	boost::shared_ptr<vector<boost::shared_ptr<Buddy> > > buddys = GameServer::getSingleton().getSmartFox()->BuddyManager()->BuddyList();
	if (!cell) {
		//log("createCell4Friends");
		cell = new CustomTableViewCell(Size(930, 100));
		//Avatar
		LayerFriendCell* friendCell = LayerFriendCell::create();
		friendCell->setPosition(Vec2(0, 0));
		friendCell->setTag(123);
		friendCell->setDatas(ChanUtils::GetNamePlayer(buddys->at(idx)->Name()->c_str()), buddys->at(idx)->IsOnline());
		cell->addChild(friendCell);
        friendCell->bgPressed->setVisible(false);
		cell->setIdx(idx);
	}
	else
	{
		cell->setIdx(idx);
        
		auto cellfriend = (LayerFriendCell*)cell->getChildByTag(123);
		if (cellfriend)
		{
			cellfriend->setDatas(buddys->at(idx)->Name()->c_str(), buddys->at(idx)->IsOnline());
            cellfriend->bgPressed->setVisible(false);
            if (idxChoosed == idx)
                cellfriend->bgPressed->setVisible(true);
		}
	}
	return cell;
}

void LayerFriendMain::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	if (strcmp("rgif", cmd->c_str()) == 0){
		if (!param->GetUtfString("aN") || !param->GetUtfString("amf") || !param->GetLong("aI"))
			return;
		string _name = *param->GetUtfString("aN");
		if (_name.compare("") == 0)
			_name = *param->GetUtfString("aI");
		lblName->setString(_name);
		lblXu->setString(mUtils::convertMoneyEx(*param->GetLong("amf")).c_str());
		ldbLevel->setPercent(dataManager.getLevelProgress(*param->GetInt("lvl"), *param->GetInt("exp")));
		return;
	}
	else if (strcmp(cmd->c_str(), "rghba") == 0)
	{
		vector<string> lstHis = mUtils::splitString(*param->GetUtfString("rghba"), '|');
		lstHistorys.clear();
		for (int i = 0; i < lstGameID.size(); i++){
			auto check = false;
			historys.ID = lstGameID.at(i);
			for (int j = 0; j < lstHis.size(); j++)
			{
				vector<string> his = mUtils::splitString(lstHis.at(j), ',');
                if (atoi(his.at(0).c_str()) == lstGameID.at(i))
				{
					historys.numOfLevel = atoi(his.at(1).c_str());
					historys.numOfWin = atoi(his.at(2).c_str());
					historys.numOfLose = atoi(his.at(3).c_str());
					check = true;
					break;
				}
            }
			if (!check)
			{
				historys.numOfLevel = 0;
				historys.numOfWin = 0;
				historys.numOfLose = 0;
			}
            lstHistorys.push_back(historys);
		}
		log("size2 :%d", mUtils::numOfGame);
		tblFriendDetailList->reloadData();
        SceneManager::getSingleton().hideLoading();
	}
}

void LayerFriendMain::OnSmartFoxBuddyListInit(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	tblFriendList->reloadData();
	this->loadFirstFriend();
}

void LayerFriendMain::loadFirstFriend()
{
    SceneManager::getSingleton().showLoading();
	boost::shared_ptr<vector<boost::shared_ptr<Buddy> > > lstBuddys = GameServer::getSingleton().getSmartFox()->BuddyManager()->BuddyList();
	if (lstBuddys == NULL || lstBuddys->size() == 0){
         SceneManager::getSingleton().hideLoading();
        this->nodeHistorys->setVisible(false); // edit 22_12 default false
        rootNode->setVisible(false);
		auto label = Label::createWithTTF("Không có bạn bè để hiển thị!", "fonts.ttf", 60);
		this->addChild(label);
        tblFriendDetailList->setVisible(false);
		label->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		return;
	}
	boost::shared_ptr<Sfs2X::Entities::Buddy> buddy = lstBuddys->at(0);
	if (buddy == NULL) return;

	lblOnlineStatus->setString(buddy->IsOnline() ? "Online" : "Offline");

	//
	string _gs;
	if (buddy->GetVariable("gs"))
		_gs = buddy->GetVariable("gs")->GetStringValue()->c_str();
	string _status;
	if (_gs.length() > 0)
	{
		auto _arr = mUtils::splitString(_gs, *",");
		if (_arr.size() == 2)
		{
			auto id = atoi(_arr.at(0).substr(0, 3).c_str());
			string str1 = "- ";
			auto str2 = _arr.at(1);
			_status = str1 + *mUtils::getGameNameByID(id)->c_str() + str2;
		}
	}
	//
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString("aI", buddy->Name()->c_str());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("rgif", params));
	GameServer::getSingleton().Send(request);

	boost::shared_ptr<IRequest> request1(new ExtensionRequest("rghba", params));
	GameServer::getSingleton().Send(request1);

	currFriendID = *buddy->Name();

	auto status = StringUtils::format("%s%s", (buddy->IsOnline() ? "Online" : "Offline"), _status.c_str());
	lblOnlineStatus->setString(status);

	auto _src = "friend-status-offline.png";
	if (buddy->IsOnline())
		_src = "friend-status-online.png";
	spriteTT->setTexture(_src);
    tblFriendDetailList->setVisible(true);
	CustomTableViewCell* cc = (CustomTableViewCell*)tblFriendList->cellAtIndex(0);
	LayerFriendCell* child = (LayerFriendCell*)cc->getChildByTag(123);
	child->bgPressed->setVisible(true);
	idxChoosed = 0;
}

void LayerFriendMain::onButtonMessage(Ref* pSender)
{
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    int accVipType = *myself->GetVariable("accVipType")->GetIntValue();
    if (accVipType >= 2) {
        auto _layer = LayerSendMessage::create();
        _layer->setLocalZOrder(1000);
        _layer->sendReceiver(lblName->getString());
	this->addChild(_layer);
    }
    else{
        Chat *toast = Chat::create("Chức năng này chỉ dành cho VIP 2 trở lên!", -1);
        this->addChild(toast);
    }

}

void LayerFriendMain::onButtonUnFriend(Ref* pSender)
{
	if (lblName->getString() == "-")
		return;
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString("aI", lblName->getString().c_str());
	boost::shared_ptr<IRequest> request(new Buddylist::RemoveBuddyRequest(lblName->getString()));
	GameServer::getSingleton().Send(request);
}

void LayerFriendMain::OnSmartFoxBuddyRemove(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	auto toast = Chat::create("Hủy kết bạn thành công!", -1);
	this->addChild(toast);
	//
	this->tblFriendList->reloadData();
	this->loadFirstFriend();
}

bool LayerFriendCell::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::getInstance()->createNode("LayerItemListFriends.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	labelName = static_cast<Text*>(rootNode->getChildByName("lblName"));
	labelOnlineStatus = static_cast<Text*>(rootNode->getChildByName("lblStatus"));
	spriteAvatar = dynamic_cast<ImageView*>(rootNode->getChildByName("imgAvatar"));

	if (this->spriteAvatar != NULL){

		this->circleAvatar = CircleAvatar::create();
		this->circleAvatar->hideIconDefault(false);

		float scX = this->spriteAvatar->getScaleX();
		float scY = this->spriteAvatar->getScaleY();

		Size _sizeImg = this->spriteAvatar->getContentSize();
		Size _sizeAva = this->circleAvatar->GetSizeAvatar();

		scX = _sizeImg.width / _sizeAva.width * scX;
		scY = _sizeImg.height / _sizeAva.height * scY;

		this->circleAvatar->setAnchorPoint(Vec2::ZERO);
		this->circleAvatar->setScale(scX, scY);

		this->circleAvatar->setPosition(this->spriteAvatar->getPosition());
		rootNode->addChild(this->circleAvatar, 5);
	}

	spriteOnliseStatus = dynamic_cast<Sprite*>(rootNode->getChildByName("imgStatus"));
	bgPressed = dynamic_cast<ImageView*>(rootNode->getChildByName("imgBg"));
	bgPressed->setVisible(false);
	avtVip = static_cast<ImageView*>(rootNode->getChildByName("imgAvatarVip"));
	avtVip->setVisible(false);
	return true;
}

void LayerFriendCell::setDatas(string nameFriend, bool isOnline)
{
	labelName->setString(nameFriend);
	labelOnlineStatus->setString(isOnline ? "Online" : "Offline");
	auto _src = "friend-status-offline.png";
	if (isOnline)
		_src = "friend-status-online.png";
	spriteOnliseStatus->setTexture(_src);
}

void LayerFriendCell::setDatas(bool isOnline, const infoUser& data)
{
	this->labelName->setString(data.userName);
	this->labelOnlineStatus->setString(isOnline ? "Online" : "Offline");
	auto _src = "friend-status-offline.png";
	if (isOnline)
	{
		_src = "friend-status-online.png";
	}
	this->spriteOnliseStatus->setTexture(_src);

	if (this->circleAvatar != NULL){
		this->circleAvatar->setAvatarImage(data.userAvatar);
	}
}

bool LayerFriendDetailCell::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::getInstance()->createNode("LayerItemListFriendDetails.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	labelTenGame = static_cast<Text*>(rootNode->getChildByName("lblName"));
	labelSoTranThangThua = static_cast<Text*>(rootNode->getChildByName("lblWinLose"));
	return true;
}

void LayerFriendDetailCell::setDatas(int idGame, string soTranThangThua)
{
	labelTenGame->setString(*mUtils::getGameNameByID(idGame));
	labelSoTranThangThua->setString(soTranThangThua);
}
