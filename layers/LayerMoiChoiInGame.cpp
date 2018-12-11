#include "LayerMoiChoiInGame.h"
#include "mUtils.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/Buddylist/InitBuddyListRequest.h"
#include "layergames/ChanUtils.h"
LayerMoiChoiInGame::LayerMoiChoiInGame()
{
	GameServer::getSingleton().addListeners(this);
}

LayerMoiChoiInGame::~LayerMoiChoiInGame()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerMoiChoiInGame::init()
{
	if (!Layer::init()){
		return false;
	}

	this->gameID = SceneManager::getSingleton().getGameID();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupMoiChoiInGame.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);

	if (gameID == kGameChan || gameID == kGameCoTuong || gameID == kGameCoUp){
        rootNode->setAnchorPoint(Vec2::ZERO);
        rootNode->setPosition(Vec2(0,0));
	}
	else{
		rootNode->setAnchorPoint(Point(0.5, 0.5));
		rootNode->setPosition(Point(visibleSize / 2));
	}
	

	ui::Helper::doLayout(rootNode);
	//rootNode->setAnchorPoint(Vec2(0, 0));
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());

	btnPhongCho = dynamic_cast<Button*>(rootNode->getChildByName("btnPhongCho"));
	if (btnPhongCho)
	{
		btnPhongCho->addClickEventListener(CC_CALLBACK_1(LayerMoiChoiInGame::onBtnphongcho, this));
	}
	auto btnClose = dynamic_cast<Button*>(rootNode->getChildByName("btnClose"));
	if (btnClose)
	{
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerMoiChoiInGame::onBtnClose, this));
	}
	btnBanBe = dynamic_cast<Button*>(rootNode->getChildByName("btnBanBe"));
	if (btnBanBe)
	{
		btnBanBe->addClickEventListener(CC_CALLBACK_1(LayerMoiChoiInGame::onBtnBanbe, this));
	}

    
	Size sizeAd = ChanUtils::getSizePos();
	auto pnlPlayer = dynamic_cast<Layout*>(rootNode->getChildByName("pnlPlayer"));
	tblTable = TableView::create(this, pnlPlayer->getContentSize());
	tblTable->setAnchorPoint(Vec2(0,1));
	tblTable->setDirection(TableView::Direction::VERTICAL);

    float yadd = 0;
    if (gameID == kGameChan || gameID == kGameCoTuong || gameID == kGameCoUp){
        yadd = 0;
    }else{
        yadd = sizeAd.height;
    }

    tblTable->setPosition(Vec2(pnlPlayer->getPosition().x, pnlPlayer->getPosition().y / 2 - 100 + yadd));
	tblTable->setDelegate(this);
	tblTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tblTable);
    this->loadAllDatas();
	return true;

}

void LayerMoiChoiInGame::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{

}

void LayerMoiChoiInGame::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{

}

void LayerMoiChoiInGame::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
	int idx = cell->getIdx();
	StructFriendInfo objInfo = lstFriendInfos.at(idx);

	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL
		|| GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") == NULL)
		return;
	int groupId = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->c_str());
	vector<string> str_params = mUtils::splitString(*GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue(), '@');
	//double mb = atof(lstParams.at(0).c_str());
	//Send request


	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString("gid", GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->c_str());
	params->PutUtfString("mb", str_params.at(0).c_str());
	params->PutUtfString("roomid", StringUtils::format("%d", GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->Id()));
	params->PutUtfString("lu", objInfo.aI.c_str());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("rilu", params));
	GameServer::getSingleton().Send(request);
	//remove it from list
	lstFriendInfos.erase(lstFriendInfos.begin() + idx);
	tblTable->reloadData();
	//
	auto _colorPress = Color3B(175, 8, 0);
	auto _colorNormal = Color3B(255, 236, 192);
	btnPhongCho->setEnabled(false);
	btnPhongCho->setBright(false);
	btnPhongCho->setTitleColor(_colorPress);
	btnBanBe->setEnabled(true);
	btnBanBe->setBright(true);
	btnBanBe->setTitleColor(_colorNormal);
}

cocos2d::Size LayerMoiChoiInGame::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return Size(1005, 164);
}

cocos2d::extension::TableViewCell* LayerMoiChoiInGame::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	auto cell = table->dequeueCell();
	auto strValue = lstFriendInfos.at(idx);
	
	if (!cell)
	{
		cell = TableViewCell::create();
		auto friendCell = LayerMoiChoiCell::create();
		friendCell->setPosition(Vec2::ZERO);
		friendCell->setTag(123);
		friendCell->setDatas(strValue.aN, strValue.amf );
		cell->addChild(friendCell);
	}
	else
	{
		auto friendCell = (LayerMoiChoiCell*)cell->getChildByTag(123);
		if (friendCell)
		{
			friendCell->setDatas(strValue.aN, strValue.amf );
		}
	}

	return cell;
}

ssize_t LayerMoiChoiInGame::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return lstFriendInfos.size();
}

void LayerMoiChoiInGame::onBtnClose(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}
void LayerMoiChoiInGame::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerMoiChoiInGame::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerMoiChoiInGame::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerMoiChoiInGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	log("cmd=%s", cmd->c_str());
	if (strcmp(cmd->c_str(), "hbc") == 0) return;
	if (strcmp(cmd->c_str(), "lip") == 0){
		boost::shared_ptr<string> lipv = param->GetUtfString("lipv");
		log("lipv: %s", lipv->c_str());
		if (strlen(lipv->c_str()) == 0){
			//NoBody aka HoangDD
		}
		else{
			log("ListFriendInvite: %s", lipv->c_str());
			mListPlayers = *lipv;
			onBtnphongcho(btnPhongCho);
			
		}
	}
}

void LayerMoiChoiInGame::loadAllDatas()
{
    if (SceneManager::getSingleton().getCurrRoomType() == 0){
        if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL
            || GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") == NULL
            || GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") == NULL)
            return;
        int gid = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0, 3).c_str());
        boost::shared_ptr<string> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue();
        string paramString = param->c_str();
        vector<string> arrInfo = mUtils::splitString(paramString, '@');
        double gameBet = atof(arrInfo.at(0).c_str());
        int amf_value = (int)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetIntValue();
        //
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt("maqp", 10);
        params->PutDouble("mimp", gameBet);
        params->PutInt("amf", amf_value);
        params->PutInt("gid", gid);
        boost::shared_ptr<IRequest> request(new ExtensionRequest("rglip", params));
        GameServer::getSingleton().Send(request);
        
    }else{if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL
              || GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") == NULL
              || GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") == NULL)
        return;
        int gid = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0, 3).c_str());
        boost::shared_ptr<string> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue();
        string paramString = param->c_str();
        vector<string> arrInfo = mUtils::splitString(paramString, '@');
        double gameBet = atof(arrInfo.at(0).c_str());
        int amf_value = (int)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetIntValue();
        //
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt("maqp", 10);
        params->PutDouble("mimp", gameBet);
        params->PutInt("amfs", amf_value);
        params->PutInt("gid", gid);
        boost::shared_ptr<IRequest> request(new ExtensionRequest("rglip", params));
        GameServer::getSingleton().Send(request);
        
    }
    
	
}

void LayerMoiChoiInGame::onBtnphongcho(Ref* pSender)
{
	auto _colorPress = Color3B(175, 8, 0);
	auto _colorNormal = Color3B(255, 236, 192);
	btnPhongCho->setEnabled(false);
	btnPhongCho->setBright(false);
	btnPhongCho->setTitleColor(_colorPress);
	btnBanBe->setEnabled(true);
	btnBanBe->setBright(true);
	btnBanBe->setTitleColor(_colorNormal);

	//
	vector<string> lstFriends = mUtils::splitString(mListPlayers, '|');
	lstFriendInfos.clear();
	for (int i = 0; i < lstFriends.size(); i++){
	if (strcmp(lstFriends.at(i).c_str(), "") == 0)
		continue;
		vector<string> friendInfo = mUtils::splitString(lstFriends.at(i), ',');
		StructFriendInfo info;
		info.uid = atoi(friendInfo.at(0).c_str());
		info.aI = friendInfo.at(1);
		info.aN = friendInfo.at(2);
		info.amf = friendInfo.at(3);

		lstFriendInfos.push_back(info);
	}
	tblTable->reloadData();

}

void LayerMoiChoiInGame::onBtnBanbe(Ref* pSender)
{
	auto _colorPress = Color3B(175, 8, 0);
	auto _colorNormal = Color3B(255, 236, 192);
	btnPhongCho->setEnabled(true);
	btnPhongCho->setBright(true);
	btnPhongCho->setTitleColor(_colorNormal);
	btnBanBe->setEnabled(false);
	btnBanBe->setBright(false);
	btnBanBe->setTitleColor(_colorPress);

	//
	lstFriendInfos.clear();
	boost::shared_ptr<vector<boost::shared_ptr<Buddy> > > lstBuddys = GameServer::getSingleton().getSmartFox()->BuddyManager()->BuddyList();
	for (int i = 0; i < lstBuddys->size();i++)
	{
		boost::shared_ptr<Buddy> friendInfo = lstBuddys->at(i);
		if (!friendInfo->IsOnline())
			continue;

		StructFriendInfo info;
		info.uid = friendInfo->Id();
		info.aI = friendInfo->Name()->c_str();
		info.aN = friendInfo->Name()->c_str();
		info.amf = 10000;

		lstFriendInfos.push_back(info);
	}

	tblTable->reloadData();
}


bool LayerMoiChoiCell::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::getInstance()->createNode("LayerItemMoiChoi.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	lblName = static_cast<Text*>(rootNode->getChildByName("lblName"));
	lblMoney = static_cast<Text*>(rootNode->getChildByName("lblMoney"));
	imgAvatar = static_cast<ImageView*>(rootNode->getChildByName("imgAvatar"));
    imgAvatarVip = static_cast<ImageView*>(rootNode->getChildByName("imgAvatarVip"));
    imgAvatarVip->setVisible(false);
	return true;
}

void LayerMoiChoiCell::setDatas(string nameFriend, string money)
{
	lblName->setString(nameFriend);
	lblMoney->setString(money);
}
