#include "LayerMailInbox.h"
#include "Requests/ExtensionRequest.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../scenes/SceneMain.h"
#include "../scenes/ScenePickRoom.h"
#include "../SceneManager.h"
#include "../layergames/PhomMessDef.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

LayerMailInbox::LayerMailInbox()
{
	GameServer::getSingleton().addListeners(this);
}

LayerMailInbox::~LayerMailInbox()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerMailInbox::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(true);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto rootNode = CSLoader::getInstance()->createNode("LayerMailInboxList.csb");
	ui::Helper::doLayout(rootNode);
	this->setContentSize(rootNode->getContentSize());
	Sprite* sprite;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		sprite = Sprite::create("background-in-game.jpg");
	}
	else{
		sprite = Sprite::create("background-sanh.jpg");
	}
	sprite->setAnchorPoint(Point(0, 0));
	sprite->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	this->addChild(sprite);
	this->addChild(rootNode);

	auto layerMailTop = dynamic_cast<Node*>(rootNode->getChildByName("LayerMailTop"));
	layerMailTop->setAnchorPoint(Point(0, 1));
	layerMailTop->setPosition(Point(0, visibleSize.height));
	auto layerTopMenuOnlyTop = static_cast<Node*>(layerMailTop->getChildByName("layerTopMenuOnlyBack"));
	if (layerTopMenuOnlyTop != NULL){
		auto btnClose = static_cast<ui::Button*>(layerTopMenuOnlyTop->getChildByName("btnBack"));
		if (btnClose != NULL){
			btnClose->addClickEventListener(CC_CALLBACK_1(LayerMailInbox::onBtnClose, this));
			btnClose->setPressedActionEnabled(true);
		}
	}
	auto pnl = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_3"));
	pnl->setAnchorPoint(Vec2(0, 1));
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
		pnl->setPosition(Vec2(0, visibleSize.height - layerMailTop->getContentSize().height + 30));
		pnl->setContentSize(Size(visibleSize.width, visibleSize.height - layerMailTop->getContentSize().height * 2 + 30));
	}
	else{
		pnl->setPosition(Vec2(0, visibleSize.height - layerMailTop->getContentSize().height));
		pnl->setContentSize(Size(visibleSize.width, visibleSize.height - layerMailTop->getContentSize().height));
	}


	tblInbox = TableView::create(this, Size(pnl->getContentSize().width, pnl->getContentSize().height));
	tblInbox->setDirection(TableView::Direction::VERTICAL);
	tblInbox->setAnchorPoint(Point(0, 0));
	tblInbox->setPosition(Point(0, pnl->getPositionY() - (pnl->getContentSize().height) + 20));

	tblInbox->setDelegate(this);
	tblInbox->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tblInbox);

	loadAllDatas();
	return true;
}

void LayerMailInbox::loadAllDatas(){

	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL)
		return;
	//Send request
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString("aI", myself->Name());
	params->PutUtfString("startT", "24/02/2013 08:30:59");
	params->PutUtfString("endT", mUtils::getCurrentDateTime("%d/%m/%Y 23:59:59").c_str());
	params->PutInt("startR", 0);
	params->PutInt("endR", 100);
	boost::shared_ptr<IRequest> request(new ExtensionRequest("glmi", params));
	GameServer::getSingleton().Send(request);

	SceneManager::getSingleton().showLoading();
}

void LayerMailInbox::onExit(){
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerMailInbox::onEnter(){
	Layer::onEnter();
	currentTab = 1;
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerMailInbox::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerMailInbox::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf == NULL)
		return;

	if (strcmp("dmi", cmd->c_str()) == 0){
		// 		Chat *toast = Chat::create("Xóa thư thành công!", -1);
		// 		this->addChild(toast);
		auto layerThongbao = LayerPopupThongBao::create();
		layerThongbao->showAButton();
        layerThongbao->setContentMess(dataManager.GetSysString(710));
		this->addChild(layerThongbao);
		//
		loadAllDatas();
	}

	if (strcmp("glmi", cmd->c_str()) == 0){
		this->lstInboxs.clear();
		this->lstSends.clear();
		boost::shared_ptr<string> glmi = param->GetUtfString("glmi");
		if (glmi == NULL || glmi->compare("") == 0){
			lstInbox.clear();
			tblInbox->reloadData();
			SceneManager::getSingleton().hideLoading();
			return;
		}
		vector<string> lstRegex;
		lstRegex.push_back("$#");
		vector<std::string> lstMsgs = mUtils::splitStringByListRegex(*glmi, lstRegex);
		lstRegex.clear();
		lstRegex.push_back("$%");
		info infos;
		for (int i = 0; i < lstMsgs.size(); ++i){
			auto lstInfos = mUtils::splitStringByListRegex(lstMsgs[i], lstRegex);
			if (lstInfos[1] == mySelf->Name()->c_str()){
                
                    if (lstInfos.size() < 5)
                        return;
				//for (int j = 0; j < lstInfos.size(); ++j){
					infos.id = atoi(lstInfos.at(0).c_str());
					infos.fromUser = lstInfos.at(1);
					infos.toUser = lstInfos.at(2);
					infos.content = lstInfos.at(3);
					infos.datetime = lstInfos.at(4);

					infos.isRead = true;
				//}
				lstSends.push_back(infos);
			}
			else{
					infos.id = atoi(lstInfos.at(0).c_str());
					infos.fromUser = lstInfos.at(1);
					infos.toUser = lstInfos.at(2);
					infos.content = lstInfos.at(3);
					infos.datetime = lstInfos.at(4);
					infos.isRead = (lstInfos.at(5).compare("1") == 0);
					if (lstInfos.size()>=7) {
                        if (lstInfos.at(6) != "")
                            infos.subject = lstInfos.at(6);
                        else
                            infos.subject = lstInfos.at(3);
					}
					else{
						infos.subject = lstInfos.at(3);
					}
				lstInboxs.push_back(infos);
			}
		}
		this->tblInbox->reloadData();

		SceneManager::getSingleton().hideLoading();
	}
	else{

	}

}

void LayerMailInbox::onBtnChildBack(Ref* pSender){

}
void LayerMailInbox::onBtnClose(Ref* pSender){
	if (this->getParent() == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest("cmi", NULL));//count msg inbox
	GameServer::getSingleton().Send(request);
	ScenePickRoom* _pickRoom = dynamic_cast<ScenePickRoom*>(this->getParent());
	if (_pickRoom != NULL){
		_pickRoom->gotoPickRoom();
	}
	else{
		SceneMain* _parent = (SceneMain*)this->getParent();
		if (_parent != NULL){
			_parent->gotoPickGame();
		}
	}
}

void LayerInboxCell::onButtonDelete(Ref* pSender){
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself != NULL) {
		SceneManager::getSingleton().showLoading();
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString("aI", myself->Name());
		params->PutInt("miid", getIDMail());
		boost::shared_ptr<IRequest> request(new ExtensionRequest("dmi", params));
		GameServer::getSingleton().Send(request);
	}
}

void LayerMailInbox::tableCellTouched(cocos2d::extension::TableView *table, cocos2d::extension::TableViewCell *cell){
	if (this->currentTab == 1){
		int idx = cell->getIdx();
		// mark as read
		boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
		if (myself == NULL)
			return;
		if (!lstInboxs.at(idx).isRead){
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutUtfString("aI", myself->Name()->c_str());
			params->PutInt("miid", lstInboxs.at(idx).id);
			boost::shared_ptr<IRequest> request(new ExtensionRequest("rmi", params));
			GameServer::getSingleton().Send(request);
			lstInboxs.at(idx).isRead = true;

			tblInbox->reloadData();
		}

		auto _layer = LayerPopupMailDetails::create();
		_layer->setDatas(lstInboxs.at(idx).id, lstInboxs.at(idx).fromUser, StringUtils::format("%s: %s", dataManager.GetSysString(635).c_str(),lstInboxs.at(idx).datetime.c_str()), lstInboxs.at(idx).content.c_str());
		auto parent = this->getParent();
		if (parent)
		{
			parent->addChild(_layer, 1000);
		}
	}
	// 	else{
	// 		int idx = cell->getIdx();
	// 		auto _layer = LayerPopupMailDetails::create();
	// 		this->addChild(_layer);
	// 		_layer->setDatas(lstSends.at(idx).id, lstSends.at(idx).fromUser, StringUtils::format("Ngày gửi: %s ", lstSends.at(idx).datetime.c_str()), lstSends.at(idx).content.c_str());
	// 	}

}

Size LayerMailInbox::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx){
	//return Size(layerMailInbox->getContentSize().width - 20, 40);
	return Size(800, 136);
}

// Hàm này tạo 1 tableview Row để add vào table view
TableViewCell* LayerMailInbox::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx){

	if (this->currentTab == 1){
		TableViewCell *cell = table->dequeueCell();
		if (!cell){
			cell = new TableViewCell();
			cell->autorelease();
			//auto rankCell1 = LayerInboxCell::create();
			LayerInboxCell* rankCell = LayerInboxCell::create();
			rankCell->setPosition(0, 0);
			rankCell->setTag(123);
			rankCell->setDatas(lstInboxs.at(idx), currentTab);
			cell->addChild(rankCell);
		}
		else{
			auto rankCell = (LayerInboxCell*)cell->getChildByTag(123);
			if (rankCell){
				rankCell->setDatas(lstInboxs.at(idx), currentTab);
			}
		}
		cell->setIdx(idx);
		return cell;
	}
	// 	else{
	// 		TableViewCell *cell = table->dequeueCell();
	// 		if (!cell){
	// 			cell = new TableViewCell();
	// 			cell->autorelease();
	// 			//auto rankCell1 =LayerInboxCell::create();
	// 			LayerInboxCell* rankCell = LayerInboxCell::create();
	// 			rankCell->setPosition(0,0);
	// 			rankCell->setTag(123);
	//             rankCell->setDatas(lstSends.at(idx),currentTab);
	// 			cell->addChild(rankCell);
	// 		}
	// 		else{
	// 			auto rankCell =(LayerInboxCell*) cell->getChildByTag(123);
	// 			if (rankCell){
	//                 rankCell->setDatas(lstSends.at(idx),currentTab);
	// 			}
	// 		}
	// 		cell->setIdx(idx);
	// 		return cell;
	/*	}*/

}

// Hàm gán giá trị số hàng của table view
ssize_t LayerMailInbox::numberOfCellsInTableView(cocos2d::extension::TableView *table){
	if (this->currentTab == 1){
		return this->lstInboxs.size();
	}
	// 	else
	// 	{
	// 		return this->lstSends.size();
	// 	}

}

bool LayerMailInbox::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
	return true;
}

LayerInboxCell::LayerInboxCell(){
	this->lblName = NULL;
	this->lblDate = NULL;
	this->lblMailTitle = NULL;
	this->circleAvatar = NULL;
	this->MyAI = "";
	this->MyAal = "";
	//GameServer::getSingleton().addListeners();
}

LayerInboxCell::~LayerInboxCell(){

}

bool LayerInboxCell::init(){
	if (!Layer::init())	{
		return false;
	}
	try
	{
		boost::shared_ptr<User> user = GameServer::getSingleton().getSmartFox()->MySelf();
		if (user != NULL){
			this->MyAI = *user->Name();
			boost::shared_ptr<UserVariable> var = user->GetVariable("aal");
			if (var != NULL){
				if (var != NULL)
					this->MyAal = *var->GetStringValue();
			}
		}
	}
	catch (...)
	{

	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto rootNode = CSLoader::getInstance()->createNode("LayerItemMailInbox.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());

	this->lblName = static_cast<Text*>(rootNode->getChildByName("lblName"));
	this->lblDate = static_cast<Text*>(rootNode->getChildByName("lblDate"));
	this->lblMailTitle = static_cast<Text*>(rootNode->getChildByName("lblMailTitle"));
	this->imgUnread = static_cast<ImageView*>(rootNode->getChildByName("imgUnread"));
	btnDelete = dynamic_cast<Button*>(rootNode->getChildByName("btnDel"));
	if (btnDelete != NULL){
		btnDelete->addClickEventListener(CC_CALLBACK_1(LayerInboxCell::onButtonDelete, this));
		btnDelete->setPressedActionEnabled(true);
	}
	btnDelete->setVisible(false);

	return true;
}

void LayerInboxCell::setDatas(const LayerMailInbox::info& data, int currTab){
	//     if(currTab == 1)
	//         this->lblName->setString(data.fromUser);
	//     else
	//         this->lblName->setString(data.toUser);
	this->lblDate->setString(data.datetime);
	std::string smallContent = formatUserName(data.subject, 80);
// 	if (data.content.length() > 80){
// 		smallContent = data.subject.substr(0, 30) + "...";
// 	}
// 	else{
// 		smallContent = data.subject;
// 	}
	this->lblMailTitle->setString(smallContent);
	//this->lblMailTitle->setString();
	//    if(data.fromUser == this->MyAI){
	//        if(this->circleAvatar != NULL){
	//            this->circleAvatar->setAvatarImage(this->MyAal);
	//        }
	//    }
	if (this->imgUnread)
		this->imgUnread->setVisible(!data.isRead);
}

void LayerMailInbox::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		this->onBtnClose(this);
	}
}

void LayerInboxCell::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf == NULL)
		return;

	if (strcmp("dmi", cmd->c_str()) == 0){
		// 		Chat *toast = Chat::create("Xóa thư thành công!", -1);
		// 		this->addChild(toast);
		auto layerThongbao = LayerPopupThongBao::create();
		layerThongbao->showAButton();
		layerThongbao->setContentMess(dataManager.GetSysString(710));
		this->addChild(layerThongbao);
		//
		auto _parent = static_cast<LayerMailInbox*>(this->getParent());
		if (_parent)
			_parent->loadAllDatas();
		//
	}
}
