#include "LayerChatMiniGame.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "PhomMessDef.h"
#include "Requests/ExtensionRequest.h"
#include "ChanUtils.h"

bool LayerChatMiniGame::init()
{
	if (!Layer::init())
		return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto root = CSLoader::getInstance()->createNode("LayerChatMiniGame.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());
	root->setAnchorPoint(Vec2(0, 0.5));
	this->firstPos = Vec2(root->getContentSize().width / 4, visibleSize.height / 2);
	this->hidePos = Vec2(-root->getContentSize().width, visibleSize.height / 2);

	//this->setPosition(this->firstPos);

	//create chat layer
	this->pnlChat = (Layout*)root->getChildByName("pnlChat");
	this->btnSend = (Button*)this->pnlChat->getChildByName("btnSend");
	this->txtChat = (TextField*)this->pnlChat->getChildByName("txtChat");
    this->txtChat->setPlaceHolder(dataManager.GetSysString(846));
    this->txtChat->setMaxLength(50);
    this->txtChat->setReturnType(EditBox::KeyboardReturnType::SEND);
    this->txtChat->setDelegate(this);
    if(SceneManager::getSingleton().isNagaWin){
        txtChat->setColor(Color3B::WHITE);
    }
	this->pnlChat->setClippingEnabled(true);
    
	this->pnlContent = (Layout*)this->pnlChat->getChildByName("pnlContent");

	tblChat = TableView::create(this, pnlContent->getContentSize());
	tblChat->setAnchorPoint(Vec2::ZERO);
	tblChat->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	tblChat->setPosition(pnlContent->getPosition());
	tblChat->setDelegate(this);
	tblChat->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->pnlChat->addChild(tblChat);

	this->btnSend->addClickEventListener(CC_CALLBACK_1(LayerChatMiniGame::onButtonSend, this));
    
 
	return true;
}
void LayerChatMiniGame::editBoxReturn (EditBox *editBox){
    log ("return type");
}

void LayerChatMiniGame::editBoxEditingDidBegin(EditBox *editBox){
    
}
void LayerChatMiniGame::editBoxEditingDidEnd(EditBox *editBox){
    
}
LayerChatMiniGame::LayerChatMiniGame()
{
	GameServer::getSingleton().addListeners(this);
    
}

LayerChatMiniGame::~LayerChatMiniGame()
{
	GameServer::getSingleton().removeListeners(this);
}

void LayerChatMiniGame::onButtonSend(Ref* pSender)
{
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
        SceneManager::getSingleton().showPopupNapTien(StringUtils::format("%s %d %s",dataManager.GetSysString(963).c_str(),dataManager.getAppConfig().chatlevel,dataManager.GetSysString(964).c_str()));        return;
    }
    
	string content = this->txtChat->getString();
	if (content.empty() == true)
	{
		SceneManager::getSingleton().showToast(dataManager.GetSysString(20));
	}
	else {
		this->txtChat->setString("");
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString("chmsg", content);

		if (this->typeGame == 0)
		{
			boost::shared_ptr<IRequest> request(new ExtensionRequest("xdchtrq", params));
			GameServer::getSingleton().getSmartFox()->Send(request);
		}
		else if (this->typeGame == 1)
		{
			boost::shared_ptr<IRequest> request(new ExtensionRequest("txchtrq", params));
			GameServer::getSingleton().getSmartFox()->Send(request);
		}
	}
}

void LayerChatMiniGame::loadResByID(int gameid)
{
	string ext_name = "txehchrq";
	if (gameid == 0)//tom cua ca thi thoi
	{
		ext_name = "xdehchrq";
	}
	else if (gameid == 1)//tai xiu
	{
		this->typeGame = 1;
//		this->btnSend->loadTextureNormal("bt_chat_tx.png");
//		this->pnlChat->setBackGroundImage("khungchat_tx.png");
	}
	else if (gameid == 2)
	{

	}

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(ext_name, params));
	GameServer::getSingleton().getSmartFox()->Send(request);
}

void LayerChatMiniGame::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

cocos2d::Size LayerChatMiniGame::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	string mess = lstChat.at(idx).Name + " : " + lstChat.at(idx).Content;
	return Size(this->pnlContent->getContentSize().width , getHeightForCell(mess));
}

cocos2d::extension::TableViewCell* LayerChatMiniGame::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
	string mess = lstChat.at(idx).Name + " : " + lstChat.at(idx).Content;
	if (!cell)
		cell = new TableViewCell();
	cell->removeAllChildren();
    if (lstChat.at(idx).isadmin){
        RichText *richText = RichText::create();
        richText->setAnchorPoint(Vec2::ZERO);
        richText->setPosition(Vec2(0, 0));
        richText->ignoreContentAdaptWithSize(false);
        richText->setWrapMode(ui::RichText::WRAP_PER_CHAR);
        string content_msg = lstChat.at(idx).Name + " : " + lstChat.at(idx).Content;
        
        richText->setContentSize(Size(this->pnlContent->getContentSize().width - 20, getHeightForCell(content_msg)));
        
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
                
                richText->setContentSize(Size(this->pnlContent->getContentSize().width - 20, getHeightForCell(content_msg)));
                
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
                
                richText->setContentSize(Size(this->pnlContent->getContentSize().width - 20, getHeightForCell(content_msg)));
                
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
            RichText *richText = RichText::create();
            richText->setAnchorPoint(Vec2::ZERO);
            richText->setPosition(Vec2(0, 0));
            richText->ignoreContentAdaptWithSize(false);
            richText->setWrapMode(ui::RichText::WRAP_PER_CHAR);
            string content_msg = lstChat.at(idx).Name + " : " + lstChat.at(idx).Content;
            
            richText->setContentSize(Size(this->pnlContent->getContentSize().width - 20, getHeightForCell(content_msg)));
            
            RichElementText *re1 = RichElementText::create(1, Color3B::YELLOW, 255, lstChat.at(idx).Name + " : ", "", 35);
            richText->pushBackElement(re1);
            
            RichElementText *reimg = RichElementText::create(1, Color3B::WHITE, 255, lstChat.at(idx).Content, "", 35);
            richText->insertElement(reimg, 1);
            richText->formatText();
            richText->setTag(123);
            cell->addChild(richText);
        }

    }


	return cell;
}

ssize_t LayerChatMiniGame::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return lstChat.size();
}

void LayerChatMiniGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (ptrNotifiedCmd == NULL)
		return;
    if (dataManager.getAppConfig().is24h) {
        return;
    }
	if (0 == ptrNotifiedCmd->compare(EXT_EVENT_GET_CHAT_HIS_RES))
	{
		boost::shared_ptr<string> listHistoryPtr = param->GetUtfString(EXT_FIELD_CHAT_HISTORY);
		if (listHistoryPtr)
		{
			this->parseChat(*listHistoryPtr);
		}
	}
	else if (0 == ptrNotifiedCmd->compare("txchtntf"))
	{
		if (this->typeGame == 1)
		{
            if (this->typeGame == 1)
            {
                boost::shared_ptr<string> messagePtr = param->GetUtfString(EXT_FIELD_CHAT_MESSAGE);
                boost::shared_ptr<string> userNamePtr = param->GetUtfString(EXT_FIELD_USERID);
                boost::shared_ptr<string> isadm = param->GetUtfString("isadm");
                
                if (messagePtr && userNamePtr )
                {
                    if (this->lstChat.size() > 10)
                    this->lstChat.erase (this->lstChat.begin(),this->lstChat.begin());
                    bool _isadmin = false;
                    bool _isnotify = false;
                    if (isadm)
                     _isadmin = *isadm == "1";
                    this->lstChat.push_back(structChat(userNamePtr->c_str(), messagePtr->c_str(),_isadmin,_isnotify));
                    this->tblChat->reloadData();
                    this->tblChat->setContentOffset(Vec2(0, 0));
                }
            }
		}

	}
	else if (0 == ptrNotifiedCmd->compare("txehchrs"))
	{
		if (this->typeGame == 1)
		{
			boost::shared_ptr<string> listHistoryPtr = param->GetUtfString("lsch");
			if (listHistoryPtr)
			{
				this->parseHistChat(listHistoryPtr->c_str());
			}
		}
	}
	else if (0 == ptrNotifiedCmd->compare("xdchtntf"))
	{
		if (this->typeGame == 0)
		{
            boost::shared_ptr<string> messagePtr = param->GetUtfString(EXT_FIELD_CHAT_MESSAGE);
            boost::shared_ptr<string> userNamePtr = param->GetUtfString(EXT_FIELD_USERID);
            boost::shared_ptr<string> isadm = param->GetUtfString("isadm");
            
            if (messagePtr && userNamePtr )
            {
                if (this->lstChat.size() > 10)
                this->lstChat.erase (this->lstChat.begin(),this->lstChat.begin());
                bool _isadmin = false;
                bool _isnotify = false;
                if (isadm)
                    _isadmin = *isadm == "1";
                this->lstChat.push_back(structChat(userNamePtr->c_str(), messagePtr->c_str(),_isadmin,_isnotify));
                this->tblChat->reloadData();
                this->tblChat->setContentOffset(Vec2(0, 0));
            }
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

	else if (0 == ptrNotifiedCmd->compare("xdehchrs"))

	{
		if (this->typeGame == 0)
		{
			boost::shared_ptr<string> listHistoryPtr = param->GetUtfString("lsch");
			if (listHistoryPtr)
			{
				this->parseHistChat(listHistoryPtr->c_str());
			}
		}
	}
    else if (0 == ptrNotifiedCmd->compare("txchtresp")){
        auto eexp = param->GetUtfString("eexp");
        auto errc = param->GetInt("errc");
        if (eexp != NULL && errc != NULL){
            if (errc != 0){
                SceneManager::getSingleton().showToast(*eexp);
            }
        }
    }
    else if (0 == ptrNotifiedCmd->compare("xdchtresp")){
        auto eexp = param->GetUtfString("eexp");
        auto errc = param->GetInt("errc");
        if (eexp != NULL && errc != NULL){
            if (errc != 0){
                SceneManager::getSingleton().showToast(*eexp);
            }
        }
    } 


}

float LayerChatMiniGame::getHeightForCell(string _strMsg)
{
	Label* lblMsg = Label::createWithSystemFont(_strMsg, "", 45);
	float h = lblMsg->getContentSize().width / this->pnlChat->getContentSize().width;
	return (h*lblMsg->getContentSize().height+50);
}

void LayerChatMiniGame::parseChat(std::string _message)
{
    rapidjson::Document document;
    if (document.Parse<0>(_message.c_str()).HasParseError() == false)
    {
        for (rapidjson::SizeType i = 0; i < document.Size(); i++)
        {
            const rapidjson::Value& c = document[i];
            string username = c["name"].GetString();
            string mess = c["message"].GetString();
            bool isadm = false;
            bool isntf= false;

            this->lstChat.push_back(structChat(username, mess,isadm,isntf));
        }
        this->tblChat->reloadData();
        this->tblChat->setContentOffset(Vec2(0, 0));
    }
    else{
       // log("%d", document.GetParseError());
    }
}

void LayerChatMiniGame::showChat()
{
	//this->runAction(Sequence::create(Show::create(), MoveTo::create(0.2, this->firstPos), nullptr));
}

void LayerChatMiniGame::hideChat()
{
	//this->runAction(Sequence::create(MoveTo::create(0.2, this->hidePos), Hide::create(), nullptr));
}

void LayerChatMiniGame::parseHistChat(std::string _strMsg)
{
    this->lstChat.clear();
    vector<string> listChat = mUtils::splitString(_strMsg, ';');
    for (int i = 0; i < listChat.size(); i++)
    {
        vector<string> infos = mUtils::splitString(listChat[i], '|');
        if (infos.size() > 2)
        {
            string username = infos[0];
            string mess = infos[1];
            bool isadm = infos[2] == "1";
            bool isnotify = false;
            this->lstChat.push_back(structChat(username, mess,isadm,isnotify));
        }
        if (infos.size() == 2)
        {
            string username = infos[0];
            string mess = infos[1];
            bool isadm = false;
            bool isnotify = false;

            this->lstChat.push_back(structChat(username, mess,isadm,isnotify));
        }
    }
    this->tblChat->reloadData();
    this->tblChat->setContentOffset(Vec2(0, 0));
    //SceneManager::getSingleton().sendEventNotify("Tại game|Mini Tài Xỉu|người chơi|ABCZXC|đã thắng|12,001|Zen|(Tài 15)");

}



