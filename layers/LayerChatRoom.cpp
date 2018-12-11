#include "LayerChatRoom.h"
#include "../scenes/SceneMain.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/PublicMessageRequest.h"
#include "Requests/SubscribeRoomGroupRequest.h"
#include "Requests/UnsubscribeRoomGroupRequest.h"
#include "Requests/JoinRoomRequest.h"
#include "Requests/LeaveRoomRequest.h"
#include "../layergames/_Chat_.h"
#include "../mUtils.h"
#include "../layergames/ChanUtils.h"
LayerChatRoom::LayerChatRoom()
{
	GameServer::getSingleton().addListeners(this);
	boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(SERVICE_CHAT_ROOM));
	GameServer::getSingleton().Send(request);
}

LayerChatRoom::~LayerChatRoom()
{
	GameServer::getSingleton().removeListeners(this);
	boost::shared_ptr<IRequest> request(new UnsubscribeRoomGroupRequest(SERVICE_CHAT_ROOM));
	GameServer::getSingleton().Send(request);
}

bool LayerChatRoom::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(true);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerChatOutGame.csb");

	ui::Helper::doLayout(rootNode);
	rootNode->setPosition(Point());
	this->addChild(rootNode);

	this->loadDatas();

	auto layerTopMenuOnlyTop = dynamic_cast <Node*>(rootNode->getChildByName("LayerTopMenuOnlyBack"));
	auto titleChatRoom = static_cast<Text*>(rootNode->getChildByName("Text_1"));
	layerTopMenuOnlyTop->setAnchorPoint(Point(0, 1));
	layerTopMenuOnlyTop->setPosition(Point(0, visibleSize.height));
	titleChatRoom->setPosition(Point(visibleSize.width / 2, visibleSize.height - layerTopMenuOnlyTop->getContentSize().height / 2));
	// leftChatRoom
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
	auto headerLeftChatRoom = static_cast<ImageView*>(rootNode->getChildByName("Image_3"));
	auto titleHeaderLeftChatRoom = static_cast<Text*>(rootNode->getChildByName("Text_6"));
	headerLeftChatRoom->setAnchorPoint(Point(0, 1));
	headerLeftChatRoom->setPosition(Point(layerTopMenuOnlyTop->getPositionX() + BORDER_TOP_MENU * ratio,
		visibleSize.height - layerTopMenuOnlyTop->getContentSize().height - BORDER_TOP_MENU *ratio));
	titleHeaderLeftChatRoom->setAnchorPoint(Point(0.5, 0.5));
	titleHeaderLeftChatRoom->setPosition(Point(headerLeftChatRoom->getPositionX() + headerLeftChatRoom->getContentSize().width / 2, 
		headerLeftChatRoom->getPositionY() - headerLeftChatRoom->getContentSize().height / 2));
	//RightChatRoom
	auto headerRightChatRoom = static_cast<ImageView*>(rootNode->getChildByName("Image_4"));
	auto titleHeaderRightChatRoom = static_cast<Text*>(rootNode->getChildByName("lblRoomName"));
	headerRightChatRoom->setAnchorPoint(Point(0, 1));
	headerRightChatRoom->setPosition(Point(headerLeftChatRoom->getPositionX() + BORDER_TOP_MENU*ratio + headerLeftChatRoom->getContentSize().width,
		headerLeftChatRoom->getPositionY()));
	titleHeaderRightChatRoom->setAnchorPoint(Point(0.5, 0.5));
	titleHeaderRightChatRoom->setPosition(Point(headerRightChatRoom->getPositionX() + headerRightChatRoom->getContentSize().width / 2,
		headerRightChatRoom->getPositionY() - headerRightChatRoom->getContentSize().height / 2 + BORDER_TOP_MENU));

	if (layerTopMenuOnlyTop != NULL) {
		auto btnClose = dynamic_cast<Button*>(layerTopMenuOnlyTop->getChildByName("btnBack"));
		auto btnOut = dynamic_cast<Button*>(layerTopMenuOnlyTop->getChildByName("btnOut"));
		if (btnOut != NULL) {
			btnOut->addClickEventListener(CC_CALLBACK_1(LayerChatRoom::onBtnBack, this));
			btnOut->setPressedActionEnabled(true);
		}
	}

	panelLeft = static_cast<Layout*>(rootNode->getChildByName("panelLeft"));
	panelRight = static_cast <Layout*>(rootNode->getChildByName("panelRight"));
    
    panelLeft->setContentSize(Size(panelLeft->getContentSize().width, visibleSize.height - TOP_HEIGHT * 2 + BORDER_TOP_MENU * ratio));
    panelLeft->setPositionX(headerLeftChatRoom->getPositionX());
    panelRight->setContentSize(Size(panelRight->getContentSize().width, visibleSize.height - TOP_HEIGHT *3 + BORDER_TOP_MENU * ratio));
    
	panelLeft->setVisible(false);
	panelRight->setVisible(false);
    

	tblLeft = TableView::create(this, panelLeft->getContentSize());
	tblLeft->setAnchorPoint(panelLeft->getAnchorPoint());
	tblLeft->setContentSize(panelLeft->getContentSize());
	tblLeft->setDirection(TableView::Direction::VERTICAL);
	tblLeft->setPosition(panelLeft->getPosition());
	tblLeft->setDelegate(this);
	tblLeft->setTag(tagTableLeft);
	tblLeft->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	rootNode->addChild(tblLeft);

	tblRight = TableView::create(this, panelRight->getContentSize());
	tblRight->setAnchorPoint(panelRight->getAnchorPoint());
	tblRight->setContentSize(panelRight->getContentSize());
	tblRight->setDirection(TableView::Direction::VERTICAL);
	tblRight->setPosition(Vec2(panelRight->getPositionX(),panelRight->getPositionY()-panelRight->getContentSize().height));
	tblRight->setDelegate(this);
	tblRight->setTag(tagTableRight);
	tblRight->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	rootNode->addChild(tblRight);

	auto btn_sendChat = static_cast<Button*>(rootNode->getChildByName("btnChatRoom"));
	if (btn_sendChat)
	{
		btn_sendChat->addClickEventListener(CC_CALLBACK_1(LayerChatRoom::onBtnSendChat, this));
	}
	
	txtNoiDung = static_cast<TextField*>(rootNode->getChildByName("txtChatRoom"));
	txtNoiDung->setMaxLength(100.0f);
	lblRoomName = static_cast<Text*>(rootNode->getChildByName("lblRoomName"));
	return true;
}

void LayerChatRoom::loadDatas()
{
	lstChats.clear();
}

void LayerChatRoom::loadFirst()
{
	boost::shared_ptr<vector<boost::shared_ptr<Room> > > rooms
		= GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(SERVICE_CHAT_ROOM);
	if (rooms->size() == 0)
		return;
	boost::shared_ptr<Room> r = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<long int> leftid(new long int(r ? r->Id() : -1));
	int joidid = (rooms->at(0)->Id());
	boost::shared_ptr<IRequest> request(new JoinRoomRequest(joidid, "", leftid));
	GameServer::getSingleton().Send(request);

	auto c = this->tblLeft->cellAtIndex(0);
	ListRoomCell* friendCell = (ListRoomCell*)c->getChildByTag(123);
	friendCell->bgPressed->setVisible(true);
}

TableViewCell* LayerChatRoom::createCell4Left(TableView *table, int idx)
{
	boost::shared_ptr<vector<boost::shared_ptr<Room> > > rooms
		= GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(SERVICE_CHAT_ROOM);
	auto cell = table->dequeueCell();
	if (!cell)
	{
		cell = new TableViewCell();
		auto itemLeft = ListRoomCell::create();
		itemLeft->setPosition(Vec2(0, 0));
		itemLeft->setTag(123);
		itemLeft->setDatas(rooms->at(idx)->Name()->c_str());
		cell->addChild(itemLeft);
	}
	else{
		auto itemLeft = (ListRoomCell*)cell->getChildByTag(123);
		if (itemLeft)
		{
			itemLeft->setDatas(rooms->at(idx)->Name()->c_str());
		}
	}
	return cell;
}

TableViewCell* LayerChatRoom::createCell4Right(TableView *table, int idx)
{
	std::string soPhong = StringUtils::format("Phòng %d", idx + 1);
	ChatItem info = lstChats.at(idx);

	auto cell = table->dequeueCell();
	if (!cell)
	{
		cell = new TableViewCell();
		auto itemRight = ListContentCell::create();
		itemRight->setPosition(Vec2::ZERO);
		itemRight->setTag(123);
        itemRight->setDatas(info.name, info.message,info.accviptype,info.urlavt, info.name.compare(*GameServer::getSingleton().getSmartFox()->MySelf()->Name()) == 0 ? true : false);
		cell->addChild(itemRight);
	}
	else
	{
		auto itemRight = (ListContentCell*)cell->getChildByTag(123);
		if (itemRight)
		{
			 itemRight->setDatas(info.name, info.message,info.accviptype,info.urlavt, info.name.compare(*GameServer::getSingleton().getSmartFox()->MySelf()->Name()) == 0 ? true : false);
		}
	}
	return cell;
}

void LayerChatRoom::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{

}

void LayerChatRoom::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{

}

void LayerChatRoom::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
	if (table->getTag() == tagTableLeft)
	{
		boost::shared_ptr<vector<boost::shared_ptr<Room> > > rooms
			= GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(SERVICE_CHAT_ROOM);
		for (auto i = 0; i < rooms->size(); i++) {
			auto cc = this->tblLeft->cellAtIndex(i);
			if (cc == NULL)
				continue;
			ListRoomCell* friendCell = (ListRoomCell*)cc->getChildByTag(123);
			friendCell->bgPressed->setVisible(false);
		}
		//check
		auto c = this->tblLeft->cellAtIndex(cell->getIdx());
		ListRoomCell* friendCell = (ListRoomCell*)c->getChildByTag(123);
		friendCell->bgPressed->setVisible(true);

		boost::shared_ptr<Room> r = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		boost::shared_ptr<long int> leftid(new long int(r ? r->Id() : -1));
		int joidid = (rooms->at(cell->getIdx())->Id());
		boost::shared_ptr<IRequest> request(new JoinRoomRequest(joidid, "", leftid));
		GameServer::getSingleton().Send(request);
	}
}

cocos2d::Size LayerChatRoom::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	if (table->getTag() == tagTableLeft) {
		return Size(panelLeft->getContentSize().width, 136);
	}
	return Size(panelRight->getContentSize().width, 180);
}

cocos2d::extension::TableViewCell* LayerChatRoom::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	if (table->getTag() == tagTableLeft)
	{
		return createCell4Left(table, idx);
	}
	return createCell4Right(table, idx);
}

ssize_t LayerChatRoom::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	if (table->getTag() == tagTableLeft)
	{
		boost::shared_ptr<vector<boost::shared_ptr<Room> > > rooms
			= GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(SERVICE_CHAT_ROOM);
		return rooms->size();
	}
	return lstChats.size();
}
void LayerChatRoom::onBtnBack(Ref* pSender)
{
	if (this->getParent() == NULL)
		return;

	SceneMain* _parent = (SceneMain*)this->getParent();
	boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
	GameServer::getSingleton().Send(request);
	if (_parent != NULL){
		_parent->gotoPickGame();
	}
	
}

void LayerChatRoom::onBtnSendChat(Ref* pSender)
{
	if (txtNoiDung->getString() == "")
		return;
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> ro = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (ro != NULL)
	{
        boost::shared_ptr<IRequest> request(new PublicMessageRequest(mUtils::filterForbidenChat(this->txtNoiDung->getString()), params, ro));
		GameServer::getSingleton().Send(request);
	}
	this->txtNoiDung->setString("");
}

void LayerChatRoom::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> Players = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);
	boost::shared_ptr<string> _UserName = Players->Name();
	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> p_Messages = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);
    if (dataManager.getAppConfig().is24h)
    {
        return;
    }
	ChatItem item;
    item.name = ChanUtils::GetNamePlayer(*_UserName);
	item.message = *p_Messages;
    item.accviptype = *Players->GetVariable("accVipType")->GetIntValue();
    item.urlavt = *Players->GetVariable("aal")->GetStringValue();
	lstChats.push_back(item);
	tblRight->reloadData();
	tblRight->setContentOffset(Vec2(0, 0));
}

void LayerChatRoom::OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueErrorCode = (*ptrEventParams)["errorCode"];
	boost::shared_ptr<short> ptrErrorCode = ((boost::static_pointer_cast<short>))(ptrEventParamValueErrorCode);
	//
	if (*ptrErrorCode == 24){
		tblLeft->reloadData();
		return;
	}
	//
	Chat *toast = Chat::create(dataManager.GetSysString(435), -1);
	this->addChild(toast);
}

void LayerChatRoom::OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	log("subcribe ok");
	tblLeft->reloadData();

	loadFirst();
	lstChats.clear();
	tblRight->reloadData();
}

void LayerChatRoom::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
	boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
	//
	boost::shared_ptr<vector<boost::shared_ptr<Room> > > rooms
		= GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(SERVICE_CHAT_ROOM);
	int currRoomID = ptrNotifiedRoom->Id();
	//
	
	lstChats.clear();
	for (int i = 0; i < rooms->size(); i++){
		if (rooms->at(i)->Id() == ptrNotifiedRoom->Id()){
			string str = StringUtils::format("Phong %s", rooms->at(i)->Name()->c_str());
			log("phong %s", str.c_str());
			lblRoomName->setString(str.c_str());
			tblRight->reloadData();
			lstChats.clear();
			break;
		}
	}
	tblRight->reloadData();
}

void LayerChatRoom::OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	tblLeft->reloadData();
}


bool ListRoomCell::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::getInstance()->createNode("LayerItemChatRoomLeft.csb");


	ui::Helper::doLayout(rootNode);
	rootNode->setPosition(Point());
	this->addChild(rootNode);

	labelNameRoom = static_cast<Text*>(rootNode->getChildByName("lblRoomName"));
	bgPressed = static_cast<ImageView*>(rootNode->getChildByName("imgBg"));
    
	return true;
}

void ListRoomCell::setDatas(string roomName)
{
	labelNameRoom->setString(roomName);
}

bool ListContentCell::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::getInstance()->createNode("LayerItemChatRoomRight.csb");


	ui::Helper::doLayout(rootNode);
	rootNode->setPosition(Point());
	this->addChild(rootNode);

	lblContent = static_cast<Text*>(rootNode->getChildByName("lblContent"));
	lblName = static_cast<Text*>(rootNode->getChildByName("lblName"));
    avtVip = static_cast<ImageView*>(rootNode->getChildByName("imgAvatarVip"));
    avtIMG = static_cast<ImageView*>(rootNode->getChildByName("imgAvatar"));
    
    circleAvatar = CircleAvatar::create();
    circleAvatar->hideIconDefault(false);
    circleAvatar->displayTypeVip(false);
    Size _sizeImg = this->avtIMG->getContentSize();
    Size _sizeAva = this->circleAvatar->GetSizeAvatar();
    
    float scX = _sizeImg.width / _sizeAva.width;
    float scY = _sizeImg.height / _sizeAva.height;
    
    this->circleAvatar->setAnchorPoint(Vec2::ZERO);
    this->circleAvatar->setScale(scX, scY);

    circleAvatar->setPosition(avtIMG->getContentSize().width / 2, avtIMG->getContentSize().height / 2);
    avtIMG->addChild(circleAvatar);
	return true;
}

void ListContentCell::setDatas(string name, string content,int accviptype,string avturl,bool isMe)
{
	lblContent->setString(content);
	lblName->setString(name);
    avtVip->loadTexture(mUtils::getSrcVip(accviptype));
    this->circleAvatar->setAvatarImage(avturl);
    
    this->lblName->setColor(Color3B(255,227,160));
    this->lblContent->setColor(Color3B::WHITE);
    
    if (isMe){
        this->lblName->setColor(Color3B(87,255,195));
        this->lblContent->setColor(Color3B::WHITE);
    }
}

void LayerChatRoom::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		this->onBtnBack(this);
	}
}
