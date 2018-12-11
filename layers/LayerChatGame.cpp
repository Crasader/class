#include "LayerChatGame.h"
#include "Requests/PublicMessageRequest.h"
#include "Requests/SubscribeRoomGroupRequest.h"
#include "Requests/JoinRoomRequest.h"
#include "layergames/PhomMessDef.h"
#include "Requests/LeaveRoomRequest.h"
#include "Requests/UnsubscribeRoomGroupRequest.h"
#include "scenes/ScenePickRoom.h"
#include "../cotuong/gameUtils.h"
#include "../scenes/SceneMain.h"
#include "../Common.h"
#include "../SceneManager.h"
using namespace cocos2d::extension;
#define  CHAT_ROOM_ID 118
LayerChatGame::LayerChatGame()
{
    GameServer::getSingleton().addListeners(this);

}

LayerChatGame::~LayerChatGame()
{
	GameServer::getSingleton().removeListeners(this);
}
bool LayerChatGame::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto target = pEvent->getCurrentTarget();
    
    auto locationInNode = pTouch->getLocation();
    //var s = target.getContentSize();
    auto rect = this->pnlChat->getBoundingBox();
    
    //Check the click area
    if (rect.containsPoint(locationInNode)) {
        //return true;
    }
    else {
        this->pnlChat->setVisible(false);
    }
    return false;
}
void LayerChatGame::onEnter()
{
	Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(LayerChatGame::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void LayerChatGame::onExit()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

bool LayerChatGame::init()
{
	if (!Layer::init())
		return false;
    string filename = "LayerChatGame.csb";
//    if (SceneManager::getSingleton().getGameID() == kGameChan)
//        filename = "LayerPickRoomRightContentChan.csb";
	auto rootNode = CSLoader::getInstance()->createNode(filename);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());
    this->pnlChat = static_cast<Layout*>(rootNode->getChildByName("pnlChat"));
	panelTable = static_cast<Layout*>(this->pnlChat->getChildByName("pnlContent"));
	tblChat = TableView::create(this, Size(panelTable->getContentSize().width, panelTable->getContentSize().height));
	tblChat->setAnchorPoint(panelTable->getAnchorPoint());
	tblChat->setContentSize(panelTable->getContentSize());
	tblChat->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	tblChat->setPosition(Vec2(panelTable->getPosition().x, panelTable->getPosition().y));
	tblChat->setDelegate(this);
	tblChat->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->pnlChat->addChild(tblChat);
	//panelTable->setVisible(false);

	btnSendChat = static_cast<Button*>(rootNode->getChildByName("btnSend"));
	if (btnSendChat)
	{
		btnSendChat->setPressedActionEnabled(true);
		btnSendChat->addClickEventListener(CC_CALLBACK_1
                                           (LayerChatGame::onBtnSendChat, this));
        btnSendChat->setTitleText(dataManager.GetSysString(217));

	}
    
    auto btnOpen = static_cast<Button*>(rootNode->getChildByName("btnOpen"));
    if (btnOpen)
    {
        btnOpen->setPressedActionEnabled(true);
        btnOpen->addTouchEventListener(CC_CALLBACK_2(LayerChatGame::onBtnOpen, this));
    }
    
    this->imgNoti = static_cast<ImageView*>(rootNode->getChildByName("imgNoti"));
    this->imgNoti->setVisible(false);
    this->txtCount = static_cast<Text*>(this->imgNoti->getChildByName("txtCount"));
    
	txtNoiDung = static_cast<TextField*>(rootNode->getChildByName("txtChat"));
	if (txtNoiDung)
		txtNoiDung->setMaxLength(50);
    txtNoiDung->setPlaceHolder(dataManager.GetSysString(846));
    txtNoiDung->setDelegate(this);
    if(SceneManager::getSingleton().isNagaWin){
        txtNoiDung->setColor(Color3B::WHITE);
    }
    this->loadAllDatas();

	return true;
}
void LayerChatGame::editBoxReturn (EditBox *editBox){
    
}
void LayerChatGame::editBoxEditingDidBegin(EditBox *editBox){
    SceneMain::getSingleton().chatting = true;
}
void LayerChatGame::editBoxEditingDidEnd(EditBox *editBox){
    SceneMain::getSingleton().setChatting();
}
void LayerChatGame::loadAllDatas()
{
    this->pnlChat->setVisible(false);
	lstChat.clear();
    this->showNoti(0);
    this->loadFirstCell();
}

void LayerChatGame::showNoti(int count){
    if (count == 0){
        this->imgNoti->setVisible(false);
        this->countMessage = 0;
    }else{
        this->imgNoti->setVisible(true);
        this->imgNoti->runAction(Sequence::create(ScaleTo::create(0.2, 1.7),ScaleTo::create(0.2, 1.6),NULL));
        this->txtCount->setString(StringUtils::format("%d",count));
    }
    
}

void LayerChatGame::onBtnOpen(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if (this->pnlChat->isVisible()){
            this->pnlChat->setVisible(false);
            this->showNoti(0);

            
        }else{
            this->pnlChat->setVisible(true);
            this->showNoti(0);
        }
    }
}

void LayerChatGame::onBtnSendChat(Ref* pSender)
{
	//if (type == ui::Widget::TouchEventType::ENDED)
	//{
        this->sendChat();
    //SceneMain::getSingleton().setChatting();
	//}
	//this->sendChat();
}

void LayerChatGame::sendChat()
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

}

void LayerChatGame::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

cocos2d::Size LayerChatGame::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
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

cocos2d::extension::TableViewCell* LayerChatGame::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return this->createCell4Chat(table, idx);
}

ssize_t LayerChatGame::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return lstChat.size();
}

void LayerChatGame::loadFirstCell()
{
	boost::shared_ptr<IRequest> requestGet(new ExtensionRequest(EXT_EVENT_GET_CHAT_HIS_REQ, NULL));
	GameServer::getSingleton().Send(requestGet);

}

float LayerChatGame::getHeightForCell(string _strMsg)
{
	Label* lblMsg = Label::createWithSystemFont(_strMsg, "", 45);
	float h = lblMsg->getContentSize().width / panelTable->getContentSize().width;
	return h*lblMsg->getContentSize().height+50;
}

TableViewCell* LayerChatGame::createCell4Chat(TableView* table, ssize_t idx)
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

void LayerChatGame::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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

void LayerChatGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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
         this->showNoti(0);
        if (dataManager.getAppConfig().is24h)
        {
            return;
        }
		boost::shared_ptr<string> listHistoryPtr = param->GetUtfString(EXT_FIELD_CHAT_HISTORY);
		if (listHistoryPtr)
		{
			this->parseChat(listHistoryPtr->c_str());
		}
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
            if (userNamePtr->compare(GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str()) != 0){
                this->countMessage++;
                if (this->pnlChat->isVisible()){
                    this->countMessage = 0;
                    this->showNoti(this->countMessage);
                }else{
                    this->showNoti(this->countMessage);
                }
            }
       
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
            this->countMessage++;
            if (this->pnlChat->isVisible()){
                this->countMessage = 0;
                this->showNoti(this->countMessage);
            }else{
                this->showNoti(this->countMessage);
            }
            auto lst = mUtils::splitString(*ntfmsg,'/');
            if (lst.size()<2) return;
            string content = "";
            if (SceneManager::getSingleton().currLang== 0){
                content = lst[0];
            }else if (SceneManager::getSingleton().currLang== 2){
                content = lst[1];
            }
            if (lst.size()>=3 && SceneManager::getSingleton().currLang== 1){
                content = lst[2];
            }
            if (lst.size()>=4 && SceneManager::getSingleton().currLang== 4){
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


void LayerChatGame::parseChat(string json)
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

bool LayerChatGame::isImageExist(string url)
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

