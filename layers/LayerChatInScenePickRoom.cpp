#include "LayerChatInScenePickRoom.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Requests/PublicMessageRequest.h"
#include "Requests/SubscribeRoomGroupRequest.h"
#include "Requests/JoinRoomRequest.h"
#include "layergames/PhomMessDef.h"
#include "Requests/LeaveRoomRequest.h"
#include "Requests/UnsubscribeRoomGroupRequest.h"
#include "scenes/ScenePickRoom.h"
#include "scenes/SceneMain.h"

#include "../layers/LayerPopupEvent.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../layergames/NativeHelper.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
using namespace cocos2d::extension;
#define  CHAT_ROOM_ID 118
LayerChatInScenePickRoom::LayerChatInScenePickRoom()
{
    GameServer::getSingleton().addListeners(this);

	layerChatInGame = NULL;
	roomChat = NULL;
}

LayerChatInScenePickRoom::~LayerChatInScenePickRoom()
{
    this->unschedule(schedule_selector(LayerChatInScenePickRoom::scheduleChat));
    this->unschedule(schedule_selector(LayerChatInScenePickRoom::reload));

	GameServer::getSingleton().removeListeners(this);
}

void LayerChatInScenePickRoom::onEnter()
{
	Layer::onEnter();
	
}

void LayerChatInScenePickRoom::onExit()
{
	Layer::onExit();
	boost::shared_ptr<IRequest> request(new UnsubscribeRoomGroupRequest(boost::to_string(CHAT_ROOM_ID)));
	GameServer::getSingleton().Send(request);
	if (roomChat)
	{
		boost::shared_ptr<IRequest> request(new LeaveRoomRequest(roomChat));
		GameServer::getSingleton().Send(request);
	}
	else{
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom)
		{
			if (0 == lastRoom->GroupId()->compare(boost::to_string(CHAT_ROOM_ID)))
			{
				boost::shared_ptr<IRequest> request(new LeaveRoomRequest(lastRoom));
				GameServer::getSingleton().Send(request);
			}
		}
	}
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerChatInScenePickRoom::reload),this);
    this->unschedule(schedule_selector(LayerChatInScenePickRoom::showNapTien));

}

bool LayerChatInScenePickRoom::init()
{
	if (!Layer::init())
		return false;
    string filename = "LayerChatInScenePickRoom.csb";
//    if (SceneManager::getSingleton().getGameID() == kGameChan)
//        filename = "LayerPickRoomRightContentChan.csb";
	auto rootNode = CSLoader::getInstance()->createNode(filename);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());
	this->loadAllDatas();
	panelTable = static_cast<Layout*>(rootNode->getChildByName("pnlChatPickRoom"));
    auto lblTitle = static_cast<Text*> (rootNode->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(865));
	tblChat = TableView::create(this, Size(panelTable->getContentSize().width, panelTable->getContentSize().height));
	tblChat->setAnchorPoint(panelTable->getAnchorPoint());
	tblChat->setContentSize(panelTable->getContentSize());
	tblChat->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	tblChat->setPosition(Vec2(panelTable->getPosition().x, panelTable->getPosition().y));
	tblChat->setDelegate(this);
    tblChat->setLocalZOrder(100);
	tblChat->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	rootNode->addChild(tblChat);
	//panelTable->setVisible(false);

	btnSendChat = static_cast<Button*>(rootNode->getChildByName("btnSend"));
	if (btnSendChat)
	{
		btnSendChat->setPressedActionEnabled(true);
		btnSendChat->addTouchEventListener(CC_CALLBACK_2(LayerChatInScenePickRoom::onBtnSendChat, this));
	}
    this->btnCoBam = static_cast<Button*>(rootNode->getChildByName("btnCoBam"));
    if (this->btnCoBam)
    {
        this->btnCoBam->setPressedActionEnabled(true);
        this->btnCoBam->addTouchEventListener(CC_CALLBACK_2(LayerChatInScenePickRoom::onBtnCoBam, this));
    }
    this->btnCoBam->setVisible(false);
//	btnPlayNow = static_cast<Button*>(rootNode->getChildByName("btnChoiNgay"));
//	if (btnPlayNow)
//		btnPlayNow->setPressedActionEnabled(true);
	txtNoiDung = static_cast<TextField*>(rootNode->getChildByName("txtChat"));
	if (txtNoiDung)
		txtNoiDung->setMaxLength(50);
    txtNoiDung->setPlaceHolder(dataManager.GetSysString(846));
    this->pnlThongBao = dynamic_cast<Layout*>(rootNode->getChildByName("pnlThongBao"));
    auto _txtThongBao = static_cast<Text*>(this->pnlThongBao->getChildByName("txtDieuKien"));
    _txtThongBao->setVisible(false);
    this->schedule(schedule_selector(LayerChatInScenePickRoom::reload));

    txtDieuKien = Label::createWithSystemFont(StringUtils::format("%s :%d",dataManager.GetSysString(559).c_str(),dataManager.getAppConfig().chatlevel), "", 35);
    txtDieuKien->setAnchorPoint(_txtThongBao->getAnchorPoint());
    txtDieuKien->setPosition(_txtThongBao->getPosition());
    txtDieuKien->setVisible(!dataManager.getAppConfig().isHideBilling);
    this->pnlThongBao->addChild(txtDieuKien);
    txtDieuKien->setTag(9578);
    this->reload(0);

//    if (dataManager.getAppConfig().is24h)
//    {
//        this->txtDieuKien->setVisible(false);
//        txtNoiDung->setVisibleEditBox(false);
//    }
    if(SceneManager::getSingleton().isNagaWin){
        txtNoiDung->setColor(Color3B::WHITE);
    }
	//this->loadFirstCell();
    
    
    
    
    if (SceneManager::getSingleton().isNagaWin){
        this->btnBack = dynamic_cast<Button*>(rootNode->getChildByName("btnBack"));
        if (this->btnBack){
            this->btnBack->addClickEventListener(CC_CALLBACK_1(LayerChatInScenePickRoom::onBtnBack,this));
        }
        this->btnHotLine = dynamic_cast<Button*>(rootNode->getChildByName("btnHotLine"));
        if (this->btnHotLine){
            this->btnHotLine->addClickEventListener(CC_CALLBACK_1(LayerChatInScenePickRoom::onBtnHotLine,this));
            auto txtHotLine = dynamic_cast<Text*>(this->btnHotLine->getChildByName("txtHotLine"));
            if (txtHotLine){
                txtHotLine->setString(dataManager.getAppConfig().callcenter);
                
            }
        }
      
        this->btnSuKien = dynamic_cast<Button*>(rootNode->getChildByName("btnEvent"));
        if (this->btnSuKien){
            this->btnSuKien->addClickEventListener(CC_CALLBACK_1(LayerChatInScenePickRoom::onBtnSuKien,this));
        }
        this->setScene(1);
        auto txtSuKien = dynamic_cast<Text*>(rootNode->getChildByName("txtSuKien"));
        txtSuKien->setString(dataManager.GetSysString(762));
        
        this->sprKhuyenMai = (Sprite*)rootNode->getChildByName("sprKhuyenMai");
        this->txtKhuyenMai = (TextBMFont*)this->sprKhuyenMai->getChildByName("txtKhuyenMai");
        this->sprKhuyenMai->setVisible(false);
        this->sprKhuyenMai->setLocalZOrder(200);
        this->sprKhuyenMai->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.8, 1.2),ScaleTo::create(0.8, 1), NULL)));
        if (atoi(dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR.c_str()) > 0){
            this->sprKhuyenMai->setVisible(true);
            this->txtKhuyenMai->setString(StringUtils::format("+%s",dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR.c_str())+"%");
        }
        auto btnNapTien = dynamic_cast<Button*>(this->sprKhuyenMai->getChildByName("btnNapTien"));
        if (btnNapTien){
            btnNapTien->addClickEventListener(CC_CALLBACK_1(LayerChatInScenePickRoom::onBtnNapTien, this));
        }
        this->schedule(schedule_selector(LayerChatInScenePickRoom::showNapTien));

    }
    this->txtDieuKien->setVisible(false);
    this->btnCoBam->setVisible(false);
    btnSendChat->setVisible(true);
    this->isJoin = false;
	return true;
}
void LayerChatInScenePickRoom::showNapTien(float dt){
    if (!SceneManager::getSingleton().isLogin || dataManager.getAppConfig().isHideBilling){
        this->sprKhuyenMai->setVisible(false);
        return;
    }
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    if (myself->GetVariable("am") == NULL) return;
    auto amf = myself->GetVariable("am")->GetDoubleValue();
    if (amf){
        int am = *amf;
        if (am <=0 && atoi(dataManager.getAppConfig().FISRT_CHARGE_PROMOTION_STR.c_str()) > 0){
            this->sprKhuyenMai->setVisible(true);
        }else{
            this->sprKhuyenMai->setVisible(false);
        }
    }
    if (SceneMain::getSingleton().currentLayer == SceneMain::getSingleton().typelayerPickRoom){
        this->sprKhuyenMai->setVisible(false);

    }
}
void LayerChatInScenePickRoom::onBtnNapTien(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
    popup->onBtnNapThe(NULL);
    if (dataManager.getAppConfig().isHideBilling){
        popup->onBtnNapHao(NULL);
    }
    _currScene->addChild(popup,2000,9108);
}
void LayerChatInScenePickRoom::setScene(int type){
    if (type == 1){
        this->btnBack->setVisible(false);
        this->btnHotLine->setVisible(true);

    }else{
        this->btnBack->setVisible(true);
        this->btnHotLine->setVisible(false);
    }
}
void LayerChatInScenePickRoom::onBtnBack(Ref* pSender){
        SceneManager::getSingleton().setGameID(-1);
        SceneManager::getSingleton().gotoMain();
}
void LayerChatInScenePickRoom::onBtnHotLine(Ref* pSender){
        std::string strHotLine = dataManager.getAppConfig().callcenter + "";
        auto nums = mUtils::splitString(strHotLine, '.');
        strHotLine.erase(std::remove(strHotLine.begin(), strHotLine.end(), '.'), strHotLine.end());
        
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        NativeHelper::callPhone(strHotLine.c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        IOS::tryToCall(strHotLine);
#endif
}
void LayerChatInScenePickRoom::onBtnSuKien(Ref* pSender){
    if (SceneManager::getSingleton().lstEvent.size()<=0){
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return;
        
        Chat* popup = Chat::create(dataManager.GetSysString(436),-1);
        _currScene->addChild(popup,10000,9041);
        return;
    }
    
    if (dataManager.getAppConfig().isHideBilling || dataManager.getAppConfig().is24h || !SceneManager::getSingleton().isLogin)
    {
        return;
    }
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    
    LayerPopupEvent* popup = LayerPopupEvent::create();
    _currScene->addChild(popup,10000,9042);
}
void LayerChatInScenePickRoom::reload(float dt){
    if (!SceneManager::getSingleton().isLogin || dataManager.getAppConfig().isHideBilling){
        return;
    }
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    int acctype = 0;
    if (mySelf != NULL)
    {
        boost::shared_ptr<UserVariable> acctype_ptr = mySelf->GetVariable("accVipType");
        if (acctype_ptr != NULL)
            acctype = *acctype_ptr->GetIntValue();
        
    }
    if (acctype < dataManager.getAppConfig().chatlevel)
    {
        this->txtDieuKien->setString(StringUtils::format("%s :%d",dataManager.GetSysString(559).c_str(),dataManager.getAppConfig().chatlevel));
        this->txtNoiDung->setVisibleEditBox(false);
        this->btnCoBam->setVisible(true);
        this->txtDieuKien->setVisible(true);

       // btnSendChat->setVisible(false);
        
    }else{
        this->txtNoiDung->setVisibleEditBox(true);
        this->txtDieuKien->setVisible(false);
        this->btnCoBam->setVisible(false);
        btnSendChat->setVisible(true);
    }
    int x = txtDieuKien->getPositionX();
    if (x < -1 * txtDieuKien->getContentSize().width){
        x = pnlThongBao->getContentSize().width;
    }
    else
        x = txtDieuKien->getPositionX() - 1;
    txtDieuKien->setPositionX(x);
    if (x < -1 * txtDieuKien->getContentSize().width || txtDieuKien->getString().compare("   ") == 0) {
        
        
        x = pnlThongBao->getContentSize().width;
        //
//        currIndexNotification++;
//        if (currIndexNotification >= SceneManager::getSingleton().gListNotification.size())
//            currIndexNotification = 0;
//        string _strMsg = "   ";
//        if (SceneManager::getSingleton().gListNotification.size() > 0)
//            _strMsg = SceneManager::getSingleton().gListNotification[currIndexNotification].textShow;
        //
        //_strMsg = "Chào mừng bạn tới baifvip. Chúc bạn chơi game vui vẻ!";
        //txtThongBao = _strMsg;
       // lblThongBao->setString(_strMsg);
    }
    else
        x = txtDieuKien->getPositionX() - 1;
    txtDieuKien->setPositionX(x);
}
void LayerChatInScenePickRoom::loadAllDatas()
{
    if (this->isJoin) return;
    this->scheduleChat(1);
	//lstChat.clear();
}
void LayerChatInScenePickRoom::scheduleChat(float dt){
    boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(boost::to_string(CHAT_ROOM_ID)));
    GameServer::getSingleton().Send(request);
}
void LayerChatInScenePickRoom::onBtnCoBam(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getSingleton().showPopupNapTien(StringUtils::format("%s %d %s",dataManager.GetSysString(963).c_str(),dataManager.getAppConfig().chatlevel,dataManager.GetSysString(964).c_str()));
    }
    //this->sendChat();
}

void LayerChatInScenePickRoom::onBtnSendChat(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
        this->sendChat();
	}
	//this->sendChat();
}

void LayerChatInScenePickRoom::sendChat()
{
	if (0 == txtNoiDung->getString().compare(""))
		return;
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom)
    {

        string myName = SceneManager::getSingleton().getMyName();

//        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
//        if (myself != NULL){
//            
//            if (myself->GetVariable("adn") != NULL)
//            {
//                
//                myName = *myself->GetVariable("adn")->GetStringValue();
//                //26/01/2016 HoangDD changes
//            }
//        }

        
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString(EXT_FIELD_USERNAME, myName);
        params->PutUtfString(EXT_FIELD_CHAT_MESSAGE, txtNoiDung->getString());
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_CHAT_REQ, params, lastRoom));
        GameServer::getSingleton().Send(request);
        txtNoiDung->setString("");
    }
	// 	boost::shared_ptr<ISFSObject> params(new SFSObject());
	// 	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	// 	if (lastRoom != NULL)
	// 	{
	// 		boost::shared_ptr<IRequest> request(new PublicMessageRequest(txtNoiDung->getString(), params, lastRoom));
	// 		GameServer::getSingleton().Send(request);
	// 		txtNoiDung->setString("");
	// 	}
    else{
        boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(boost::to_string(CHAT_ROOM_ID)));
        GameServer::getSingleton().Send(request);
    }
}

void LayerChatInScenePickRoom::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

cocos2d::Size LayerChatInScenePickRoom::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	string img_path = lstChat.at(idx).Content + ".png";
	bool isExist = isImageExist(img_path);
	int emo_height = 115;
	if (isExist)
	{
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
			emo_height = 160;
		return Size(panelTable->getContentSize().width, emo_height);
	}
	else{
		string mess = lstChat.at(idx).Name + " : " + lstChat.at(idx).Content;
		return Size(panelTable->getContentSize().width, getHeightForCell(mess));
	}
}

cocos2d::extension::TableViewCell* LayerChatInScenePickRoom::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return this->createCell4Chat(table, idx);
}

ssize_t LayerChatInScenePickRoom::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return lstChat.size();
}

void LayerChatInScenePickRoom::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	boost::shared_ptr<Room> roomJoin = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	if (roomJoin == NULL) return;
	if (0 != roomJoin->GroupId()->compare(boost::to_string(CHAT_ROOM_ID))) return;
	log("OnSmartFoxRoomJoin Chat");
    if (0 != txtNoiDung->getString().compare("")){
        string myName = SceneManager::getSingleton().getMyName();

        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString(EXT_FIELD_USERNAME, myName);
        params->PutUtfString(EXT_FIELD_CHAT_MESSAGE, txtNoiDung->getString());
        boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_CHAT_REQ, params, roomJoin));
        GameServer::getSingleton().Send(request);
        txtNoiDung->setString("");
    }else{
        roomChat = roomJoin;
//        lstChat.clear();
//        tblChat->reloadData();
    }
}

void LayerChatInScenePickRoom::OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	boost::shared_ptr<Room> roomJoin = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	if (roomJoin == NULL) return;
	if (0 != roomJoin->GroupId()->compare(boost::to_string(CHAT_ROOM_ID))) return;
	boost::shared_ptr<IRequest> request(new LeaveRoomRequest(roomJoin));
	GameServer::getSingleton().Send(request);

}

void LayerChatInScenePickRoom::OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	log("OnSmartFoxSubscribeRoomGroup");
	this->loadFirstCell();
	//lstChat.clear();
	//tblChat->reloadData();
}

void LayerChatInScenePickRoom::OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    this->loadFirstCell();
    //lstChat.clear();
    //tblChat->reloadData();
}

Button* LayerChatInScenePickRoom::getButtonPlayNow()
{
	return btnPlayNow;
}

void LayerChatInScenePickRoom::loadFirstCell()
{
    if (this->isJoin) return;
    
	boost::shared_ptr<IRequest> requestGet(new ExtensionRequest(EXT_EVENT_GET_CHAT_HIS_REQ, NULL));
	GameServer::getSingleton().Send(requestGet);

	RejoinInfoStruct rejoinInfo = SceneManager::getSingleton().getRejoinInfo();
	//if can rejoin, return - HoangDD
	if (rejoinInfo.isNeedRejoin){
		return;
	}

	boost::shared_ptr<vector<boost::shared_ptr<Room> > > rooms
		= GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(CHAT_ROOM_ID));
	if (rooms == NULL)
		return;
	if (rooms->size() == 0)
		return;
	boost::shared_ptr<Room> r = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<long int> leftid(new long int(r ? r->Id() : -1));
    int a = -1;
    if (leftid == NULL){
        return;
    }
	int joidid = (rooms->at(0)->Id());
	boost::shared_ptr<IRequest> request(new JoinRoomRequest(joidid, "", leftid));
	GameServer::getSingleton().Send(request);
}

float LayerChatInScenePickRoom::getHeightForCell(string _strMsg)
{
	 Label* lblMsg = Label::createWithSystemFont(_strMsg, "", 45);
	float h = lblMsg->getContentSize().width / panelTable->getContentSize().width;
	return h*lblMsg->getContentSize().height+50;
}

TableViewCell* LayerChatInScenePickRoom::createCell4Chat(TableView* table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
	string mess = lstChat.at(idx).Name + " : " + lstChat.at(idx).Content;
	if (!cell)
		cell = new TableViewCell();
	cell->removeAllChildren();
	// 	///content
	string img_path = lstChat.at(idx).Content + ".png";
	bool isExist = isImageExist(img_path);
	int imgHeight = 115;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
		imgHeight = 160;
	int startX = 10;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
		startX = 40;
	if (isExist)
	{
        if (lstChat.at(idx).isadm){
            RichText *richText = RichText::create();
            richText->setAnchorPoint(Vec2::ZERO);
            richText->setPosition(Vec2(startX, 0));
            richText->ignoreContentAdaptWithSize(false);
            richText->setContentSize(Size(panelTable->getContentSize().width - 20, imgHeight));
            
            RichElementText *re1 = RichElementText::create(1, Color3B::RED, 255, lstChat.at(idx).Name + " : ", "", FONT_SIZE_SMALL);
            richText->pushBackElement(re1);
            
            RichElementImage *reimg = RichElementImage::create(2, Color3B::WHITE, 255, img_path);
            richText->insertElement(reimg, 1);
            
            richText->formatText();
            richText->setTag(123);
            cell->addChild(richText);

        }
        else{
            RichText *richText = RichText::create();
            richText->setAnchorPoint(Vec2::ZERO);
            richText->setPosition(Vec2(startX, 0));
            richText->ignoreContentAdaptWithSize(false);
            richText->setContentSize(Size(panelTable->getContentSize().width - 20, imgHeight));
            
            RichElementText *re1 = RichElementText::create(1, Color3B::YELLOW, 255, lstChat.at(idx).Name + " : ", "", FONT_SIZE_SMALL);
            richText->pushBackElement(re1);
            
            RichElementImage *reimg = RichElementImage::create(2, Color3B::WHITE, 255, img_path);
            richText->insertElement(reimg, 1);
            
            richText->formatText();
            richText->setTag(123);
            cell->addChild(richText);

        }
		
	}
	else{
		//richText->setContentSize(Size(panelTable->getContentSize().width - 20, getHeightForCell(mess)));
        if (lstChat.at(idx).isadm){
            string content_msg = lstChat.at(idx).Name + " : " + lstChat.at(idx).Content;
            
            RichText *richText = RichText::create();
            richText->setAnchorPoint(Vec2::ZERO);
            richText->setPosition(Vec2(startX, 0));
            richText->ignoreContentAdaptWithSize(false);
            richText->setWrapMode(ui::RichText::WRAP_PER_CHAR);
            richText->setContentSize(Size(panelTable->getContentSize().width - 20, getHeightForCell(content_msg)));
            
            RichElementText *re1 = RichElementText::create(1, Color3B::RED, 255, lstChat.at(idx).Name + " : ", "", 35);
            richText->pushBackElement(re1);
            
            RichElementText *reimg = RichElementText::create(1, Color3B::WHITE, 255, lstChat.at(idx).Content, "", 35);
            richText->insertElement(reimg, 1);
            richText->formatText();
            richText->setTag(123);
            cell->addChild(richText);
        }else{
            
            if (lstChat.at(idx).isnotify){
                
                string str = lstChat.at(idx).Content;
                auto lst = mUtils::splitString(str,'|');
                
                string content_msg = "";
                
                if (lst.size() == 7){
                    content_msg = lst[0] + " " + lst[1] + " "+ lst[2] + " "+ lst[3] + " " +lst[4] + " " +lst[5];
                    RichText *richText = RichText::create();
                    richText->setAnchorPoint(Vec2::ZERO);
                    richText->setPosition(Vec2(0, 0));
                    richText->ignoreContentAdaptWithSize(false);
                    richText->setWrapMode(ui::RichText::WRAP_PER_CHAR);
                    
                    richText->setContentSize(Size(panelTable->getContentSize().width - 20, getHeightForCell(content_msg)));
                    
                    RichElementText *re1 = RichElementText::create(1, Color3B(0,177,82), 255, lst[0] + " ", "", 35);
                    richText->pushBackElement(re1);
                    
                    RichElementText *re2 = RichElementText::create(1, Color3B::WHITE, 255, lst[1] + " ", "", 35);
                    richText->pushBackElement(re2);
                    
                    RichElementText *re3 = RichElementText::create(1, Color3B(0,177,82), 255, lst[2] + " ", "", 35);
                    richText->pushBackElement(re3);
                    
                    RichElementText *re4 = RichElementText::create(1, Color3B::YELLOW, 255, lst[3] + " ", "", 35);
                    richText->pushBackElement(re4);
                    
                    RichElementText *re5 = RichElementText::create(1, Color3B(0,177,82), 255, lst[4] + " ", "", 35);
                    richText->pushBackElement(re5);
                    
                    RichElementText *re6 = RichElementText::create(1, Color3B(225,239,216), 255, lst[5] + " ", "", 35);
                    richText->pushBackElement(re6);
                    
                    RichElementText *re7 = RichElementText::create(1, Color3B(225,239,216), 255, lst[6], "", 35);
                    richText->pushBackElement(re7);
                    
                    richText->formatText();
                    richText->setTag(123);
                    cell->addChild(richText);
                }
                if (lst.size() == 8){
                    content_msg = lst[0] + " " + lst[1] + " "+ lst[2] + " "+ lst[3] + " " +lst[4] + " " +lst[5]+ " " +lst[6];
                    RichText *richText = RichText::create();
                    richText->setAnchorPoint(Vec2::ZERO);
                    richText->setPosition(Vec2(0, 0));
                    richText->ignoreContentAdaptWithSize(false);
                    richText->setWrapMode(ui::RichText::WRAP_PER_CHAR);
                    
                    richText->setContentSize(Size(panelTable->getContentSize().width - 20, getHeightForCell(content_msg)));
                    
                    RichElementText *re1 = RichElementText::create(1, Color3B(0,177,82), 255, lst[0] + " ", "", 35);
                    richText->pushBackElement(re1);
                    
                    RichElementText *re2 = RichElementText::create(1, Color3B::WHITE, 255, lst[1] + " ", "", 35);
                    richText->pushBackElement(re2);
                    
                    RichElementText *re3 = RichElementText::create(1, Color3B(0,177,82), 255, lst[2] + " ", "", 35);
                    richText->pushBackElement(re3);
                    
                    RichElementText *re4 = RichElementText::create(1, Color3B::YELLOW, 255, lst[3] + " ", "", 35);
                    richText->pushBackElement(re4);
                    
                    RichElementText *re5 = RichElementText::create(1, Color3B(0,177,82), 255, lst[4] + " ", "", 35);
                    richText->pushBackElement(re5);
                    
                    RichElementText *re6 = RichElementText::create(1, Color3B(225,239,216), 255, lst[5] + " ", "", 35);
                    richText->pushBackElement(re6);
                    RichElementText *re7 = RichElementText::create(1, Color3B(225,239,216), 255, lst[6]+ " ", "", 35);
                    richText->pushBackElement(re7);
                    RichElementText *re8 = RichElementText::create(1, Color3B::RED,255, lst[7], "", 35);
                    richText->pushBackElement(re8);
                    
                    richText->formatText();
                    richText->setTag(123);
                    cell->addChild(richText);
                }
                
                
            }else{
            
            string content_msg = lstChat.at(idx).Name + " : " + lstChat.at(idx).Content;

            RichText *richText = RichText::create();
            richText->setAnchorPoint(Vec2::ZERO);
            richText->setPosition(Vec2(startX, 0));
            richText->ignoreContentAdaptWithSize(false);
            richText->setWrapMode(ui::RichText::WRAP_PER_CHAR);
            richText->setContentSize(Size(panelTable->getContentSize().width - 20, getHeightForCell(content_msg)));
            
            RichElementText *re1 = RichElementText::create(1, Color3B::YELLOW, 255, lstChat.at(idx).Name + " : ", "", 35);
            richText->pushBackElement(re1);
            
            RichElementText *reimg = RichElementText::create(1, Color3B::WHITE, 255, lstChat.at(idx).Content, "", 35);
            richText->insertElement(reimg, 1);
            richText->formatText();
            richText->setTag(123);
            cell->addChild(richText);
            }
        }
	}

	return cell;
}

void LayerChatInScenePickRoom::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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
//    string _name = *_UserName;
//    string message = *p_Messages;
//    int accviptype = *Players->GetVariable("accVipType")->GetIntValue();
//    string urlavt = *Players->GetVariable("aal")->GetStringValue();

//    if (lstChat.size() > 10)
//        lstChat.erase(lstChat.begin());
//    lstChat.push_back(structChat(_name, message));
//    tblChat->reloadData();
//    tblChat->setContentOffset(Vec2(0, 0));
}

void LayerChatInScenePickRoom::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (ptrNotifiedCmd == NULL)
		return;
	if (0 == ptrNotifiedCmd->compare(EXT_EVENT_CHAT_HISTORY_NTF))
	{
        if (dataManager.getAppConfig().is24h)
        {
            return;
        }
		boost::shared_ptr<string> listHistoryPtr = param->GetUtfString(EXT_FIELD_CHAT_HISTORY);
		if (listHistoryPtr)
		{
			this->parseChat(listHistoryPtr->c_str());
		}
        this->isJoin = true;
	}
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_CHAT_NTF))
	{
        if (dataManager.getAppConfig().is24h)
        {
            return;
        }
        boost::shared_ptr<string> messChatPtr = param->GetUtfString(EXT_FIELD_CHAT_MESSAGE);
        boost::shared_ptr<string> userNamePtr = param->GetUtfString(EXT_FIELD_USERNAME);
        boost::shared_ptr<long> isadm = param->GetInt("isadm");
        if (messChatPtr && userNamePtr)
        {
            if (this->lstChat.size() > 10)
                this->lstChat.erase (this->lstChat.begin(),this->lstChat.begin());
            bool _isadmin = false;
            
            if (isadm){
                _isadmin = *isadm == 1;
            }
            bool _isnotify = false;

            lstChat.push_back(structChat(userNamePtr->c_str(), messChatPtr->c_str(),_isadmin,_isnotify));
            tblChat->reloadData();
            tblChat->setContentOffset(Vec2(0, 0));
        }
	}
    else if (0 == ptrNotifiedCmd->compare(EVENT_NOTIFY_MESSAGE_HANDLE))
    {
        
        boost::shared_ptr<string> ntfmsg = param->GetUtfString(EXT_FIELD_MESSAGE_STRING);
        if (ntfmsg)
        {
            auto lst = mUtils::splitString(*ntfmsg,'/');
            if (lst.size()<2) return;
            string content = "";
            if (SceneManager::getSingleton().currLang== 0){
                content = lst[0];
            }else if (SceneManager::getSingleton().currLang== 2){
                content = lst[1];
            }
            else if (lst.size()>=3 && SceneManager::getSingleton().currLang== 1){
                content = lst[2];
            }
            else if (lst.size()>=4 && SceneManager::getSingleton().currLang== 4){
                content = lst[3];
            }
            bool _isadmin = false;
            bool _isnotify = true;
            this->lstChat.push_back(structChat("", content,_isadmin,_isnotify));
            this->tblChat->reloadData();
            this->tblChat->setContentOffset(Vec2(0, 0));
        }
        
    }
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_GET_CHAT_HIS_RES))
	{
        if (dataManager.getAppConfig().is24h)
        {
            return;
        }
		boost::shared_ptr<string> listHistoryPtr = param->GetUtfString(EXT_FIELD_CHAT_HISTORY);
		if (listHistoryPtr)
		{
			//vector<string> listHistory = mUtils::splitString(*listHistoryPtr, '|');
			this->parseChat(listHistoryPtr->c_str());
		}
	}
    else if (0 == ptrNotifiedCmd->compare("chres")){
        auto eexp = param->GetUtfString("eexp");
        auto errc = param->GetInt("errc");
        if (eexp != NULL && errc != NULL){
            if (errc != 0){
                SceneManager::getSingleton().showToast(*eexp);
            }
        }
    }
	else{
		log("CMD = %s", ptrNotifiedCmd->c_str());
	}
}

void LayerChatInScenePickRoom::createPopupChat()
{
	//add to parent
	auto parent = this->getParent();
	if (parent)
	{
		layerChatInGame = LayerChatInGame::create();
		layerChatInGame->setIsPickRoom(true);
		layerChatInGame->setAnchorPoint(Vec2(-1, 0));
		layerChatInGame->setPosition(-20, 20);
		parent->addChild(layerChatInGame, 100);
		layerChatInGame->hide();
	}
}

void LayerChatInScenePickRoom::parseChat(string json)
{
    rapidjson::Document document;
    if (document.Parse<0>(json.c_str()).HasParseError() == false)
    {
        for (rapidjson::SizeType i = 0; i < document.Size(); i++)
        {
            const rapidjson::Value& c = document[i];
            string username = c["name"].GetString();
            string mess = c["message"].GetString();
            bool isadm = false;
            bool _isnotify= false;
            if (c.HasMember("admin"))
                isadm = c["admin"].GetInt() == 1;
            lstChat.push_back(structChat(username, mess,isadm,_isnotify));
        }
        tblChat->reloadData();
        tblChat->setContentOffset(Vec2(0, 0));
    }
    else{
       // log("%d", document.GetParseError());
    }
}

bool LayerChatInScenePickRoom::isImageExist(string url)
{
	int imgNum = 27;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
		imgNum = 20;
	}
	for (int i = 0; i < imgNum; i++)
	{
		string imgPath = StringUtils::format("emo_%d.png", (i + 1));
		if (0 == imgPath.compare(url))
			return true;
	}
	return false;
}

