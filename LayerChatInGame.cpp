//
//  LayerChatInGame2.cpp
//  iCasino_v4
//
//  Created by Tuan Anh on 1/19/16.
//
//

#include "LayerChatInGame.h"
#include "Requests/PublicMessageRequest.h"
#include "Common.h"
#include "layergames/_Button_inGame_.h"
#include "SceneManager.h"
#include "AllData.h"
#include "layergames/ChanUtils.h"
#include "layergames/_Chat_.h"
#include "mUtils.h"
#include "Requests/PingPongRequest.h"
#include "layergames/PhomMessDef.h"
#include "base/CCConfiguration.h"
#include "scenes/ScenePickRoom.h"
#include "data_manager.h"
LayerChatInGame::LayerChatInGame()
{
	pvContent = NULL;
	pvEmo = NULL;
	this->createListChat();
	isShouldHide = false;
	GameServer::getSingleton().addListeners(this);
}

LayerChatInGame::~LayerChatInGame()
{
	GameServer::getSingleton().removeListeners(this);
}
bool LayerChatInGame::init()
{
	if (!Layer::init())
	{
		return false;
	}
	setIsPickRoom(false);
	Size sizeAdd = ChanUtils::getSizePos();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupChat.csb");
	ui::Helper::doLayout(rootNode);
	rootNode->setPositionY(rootNode->getPositionY() + sizeAdd.height);
	this->addChild(rootNode);

	/************************************************************************/
	/* MINHDV -changes new chat for bai vip                                                                     */
	/************************************************************************/
	Layout* Panel_1 = (Layout*)rootNode->getChildByName("Panel_1");
	if (Panel_1)
	{
		Panel_1->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	}

	Layout* panelMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (panelMain)
	{
		this->pvContent = (PageView*)panelMain->getChildByName("pvContent");
		this->pvEmo = (PageView*)panelMain->getChildByName("pvEmo");
		this->txtChat = (TextField*)panelMain->getChildByName("txtChat");
		this->txtChat->setPlaceHolder(dataManager.GetSysString(27));
        if(SceneManager::getSingleton().isNagaWin){
            txtChat->setColor(Color3B::WHITE);
        }
		Button* btnSend = (Button*)panelMain->getChildByName("btnSendChat");
		if (btnSend)
		{
			btnSend->setTitleText("");
			btnSend->addTouchEventListener(CC_CALLBACK_2(LayerChatInGame::onBtnSend, this));

		}
		Button* btnClose = (Button*)panelMain->getChildByName("btnClose");
		if (btnClose)
		{
			btnClose->addTouchEventListener(CC_CALLBACK_2(LayerChatInGame::onButtonClose, this));
		}
	}

	this->createPageView();
	this->createPageViewEmo();
	return true;
}
void LayerChatInGame::onButtonClicked(Ref* pSender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto button = (ui::Button*)pSender;
		string btn_name = button->getName();
		if (0 == btn_name.compare(""))
		{
			doChat(button->getTitleText(), false);
		}
		else{
			doChat(btn_name, true);
		}

		hide();
	}
}
bool LayerChatInGame::checkSpamChat(string txt){
	if (txt.length() >= 10){
		std::transform(txt.begin(), txt.end(), txt.begin(), ::tolower);
		for (int i = 0; i < SceneManager::getSingleton().arrCurrentChat.size(); i++)
		{
			while (strstr(txt.c_str(), SceneManager::getSingleton().arrCurrentChat[i].txt.c_str())){

				if (SceneManager::getSingleton().arrCurrentChat[i].count < dataManager.getAppConfig().chattimemax){
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return false;
				}
				else{
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return true;
				}
			}
			while (strstr(SceneManager::getSingleton().arrCurrentChat[i].txt.c_str(), txt.c_str())){

				if (SceneManager::getSingleton().arrCurrentChat[i].count < dataManager.getAppConfig().chattimemax){
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return false;
				}
				else{
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return true;
				}
			}
		}
		SceneManager::chatspam a;
		a.count = 1;
		a.txt = txt;
		SceneManager::getSingleton().arrCurrentChat.push_back(a);
		return false;
	}
	return false;
}
void LayerChatInGame::doChat(string txt, bool isEmo)
{
	//check vip for game bai p2 - vip 0 khong duoc su dung chat, vip 1 thi duoc su dung emo, > vip2 duoc dung het

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		int acctype = 0;
		if (mySelf != NULL)
		{
			boost::shared_ptr<UserVariable> acctype_ptr = mySelf->GetVariable("accVipType");
			if (acctype_ptr != NULL)
				acctype = *acctype_ptr->GetIntValue();
		}
		Scene *scene = Director::getInstance()->getRunningScene();
		ScenePickRoom* gameScene = dynamic_cast<ScenePickRoom*>(scene->getChildByTag(8866));
		if (acctype < dataManager.getAppConfig().chatlevel && gameScene != NULL)
		{
			this->showToast(StringUtils::format("%s %d %s ",dataManager.GetSysString(22).c_str(), dataManager.getAppConfig().chatlevel,dataManager.GetSysString(23).c_str()));
			return;
		}
		if (gameScene != NULL){
			if (SceneManager::getSingleton().chatcounttime == 0) {
				SceneManager::getSingleton().chatcounttime++;
				SceneManager::getSingleton().timerchat++;
				gameScene->startTimeOutChat();
			}

			if (checkSpamChat(txt)) {
				this->showToast(dataManager.GetSysString(24));
				return;
			}

		}
	}

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
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
			if (isEmo)
				this->showToast(dataManager.GetSysString(25));
			else
				this->showToast(dataManager.GetSysString(26));
			return;
		}
		else if (acctype == dataManager.getAppConfig().chatlevel && !isEmo)
		{
			this->showToast(dataManager.GetSysString(26));
			return;
		}
	}


	if (_ispick)
	{
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom)
		{
			string myName = SceneManager::getSingleton().getMyName();
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutUtfString(EXT_FIELD_USERNAME, myName);
			params->PutUtfString(EXT_FIELD_CHAT_MESSAGE, mUtils::filterForbidenChat(txt));
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_CHAT_REQ, params, lastRoom));
			GameServer::getSingleton().Send(request);
			txtChat->setString("");
			this->hide();
		}
	}
	else{
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> ro = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (ro != NULL)
		{
			txtChat->setString("");
			this->hide();
			boost::shared_ptr<IRequest> request(new PublicMessageRequest(mUtils::filterForbidenChat(txt), params, ro));
			GameServer::getSingleton().Send(request);
		}
	}
}

void LayerChatInGame::show()
{
	if (this->isVisible() == false) {
		this->setVisible(true);
		Size s = mUtils::getSizePos();
		if (SceneManager::getSingleton().getGameID() == kGameSam || SceneManager::getSingleton().getGameID() == kGameTLMNDemLa || SceneManager::getSingleton().getGameID() == kGamePhom)
			s = Size(0, 0);
		this->runAction(FadeIn::create(0.5));
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(LayerChatInGame::onTouchBegan, this);
		dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
}

void LayerChatInGame::hide()
{
	Size s = mUtils::getSizePos();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListenersForTarget(this);
	this->runAction(FadeOut::create(0.5));
	this->setVisible(false);
}
// void LayerChatInGame::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
// {
// 	auto target = pEvent->getCurrentTarget();
// 	auto locationInNode = target->convertToNodeSpace(pTouch->getLocation());
// 	auto s = target->getContentSize();
// 	auto rect = Rect(0, 0, bgImage->getContentSize().width, bgImage->getContentSize().height);
// 	if (rect.containsPoint(locationInNode)) {
// 
// 	}
// 	else{
// 		if (isShouldHide)
// 			hide();
// 	}
// }
// bool LayerChatInGame::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
// {
// 	isShouldHide = true;
// 	return true;
// }
void LayerChatInGame::onBtnSend(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
#if(CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
		if (string(txtChat->getString()).length() == 0)
			return;
		doChat(string(txtChat->getString()), false);
#endif
	}
}

void LayerChatInGame::reportResultGame(string& result)
{

	try{
		boost::algorithm::trim(result);
	}
	catch (...){

	}

	result = mUtils::replaceAll(result, "\n", " ");

	ChatItem item;
	item.username = "Hệ thống";
	item.message = result;
}

void LayerChatInGame::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->hide();
	}
}

void LayerChatInGame::createPageView()
{
	if (!pvContent || listChat.size() == 0)
		return;
	Color3B colorText = Color3B::WHITE;
	int addWidth = 0;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
		colorText = Color3B::WHITE;
		addWidth = 30;
	}

	//pvContent->setClippingEnabled(true);

	Layout* layout = Layout::create();
	layout->setContentSize(pvContent->getContentSize());
	layout->setSize(pvContent->getContentSize());

	for (int i = 0; i < listChat.size() - 6; i++)
	{
		Button *btnChat = Button::create("text-button.png", "", "");
		btnChat->setTitleFontSize(35);
		//btnChat->setTitleFontName(kNormalFont);
		btnChat->setTitleText(listChat.at(i));
		btnChat->setAnchorPoint(Vec2(0, 0));
		btnChat->setTitleColor(colorText);
		float posY;
		float dis = 40;
		int pos = i / 2;
		if (i % 2 == 0)
		{
			posY = pvContent->getContentSize().height - btnChat->getContentSize().height - addWidth;
		}
		else{
			posY = btnChat->getContentSize().height / 2;
		}
		btnChat->setPosition(Vec2((dis + btnChat->getContentSize().width) * pos, posY));
		btnChat->addTouchEventListener(CC_CALLBACK_2(LayerChatInGame::onButtonClicked, this));
		layout->addChild(btnChat);
	}

	Layout* layout2 = Layout::create();
	layout2->setContentSize(pvContent->getContentSize());
	layout2->setSize(pvContent->getContentSize());
	for (int i = 6; i < listChat.size(); i++)
	{
		Button *btnChat = Button::create("text-button.png", "", "");
		btnChat->setTitleFontSize(35);
		//btnChat->setTitleFontName(kNormalFont);
		btnChat->setTitleText(listChat.at(i));
		btnChat->setAnchorPoint(Vec2(0, 0));
		btnChat->setTitleColor(colorText);
		float posY;
		float dis = 40;
		int pos = (i - 6) / 2;
		if (i % 2 == 0)
		{
			posY = pvContent->getContentSize().height - btnChat->getContentSize().height - addWidth;
		}
		else{
			posY = btnChat->getContentSize().height / 2;
		}
		btnChat->setPosition(Vec2((dis + btnChat->getContentSize().width) * pos, posY));
		btnChat->addTouchEventListener(CC_CALLBACK_2(LayerChatInGame::onButtonClicked, this));
		layout2->addChild(btnChat);
	}

	pvContent->insertPage(layout, 0);
	pvContent->insertPage(layout2, 1);
	pvContent->setUsingCustomScrollThreshold(true);
	pvContent->setCustomScrollThreshold(pvContent->getContentSize().width / 3);
	pvContent->scrollToPage(0);
}

void LayerChatInGame::createListChat()
{
	int gameID = SceneManager::getSingleton().getGameID();
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		if (gameID == kGameSam || gameID == kGameSamSolo)
			gameID = kGameSam;
		else if (gameID == kGameTLMNDemLa || gameID == kGameDemLaSolo)
			gameID = kGameTLMNDemLa;
		else if (gameID != kGamePhom || gameID != kGameChan || gameID != kGameBaCay)
			gameID = 1000;
		getListChatByID(gameID);
	}
	else{
		getListChatByID(1001);
	}

	// 	if (gameID == kGameBaCay)
	// 	{
	// 		listChat.push_back("Bài xấu lại gặp đầu gấu");
	// 		listChat.push_back("Đen vãi");
	// 		listChat.push_back("Mạng lag quá");
	// 		listChat.push_back("Rất vui được gặp các bạn");
	// 		listChat.push_back("Đẹp trai có gì sai");
	// 		listChat.push_back("Bye Bye");
	// 		listChat.push_back("Làm giàu không khó");
	// 		listChat.push_back("Đời quá đen");
	// 		listChat.push_back("Mày hả bưởi");
	// 		listChat.push_back("Ăn gì đỏ thế");
	// 		listChat.push_back("Thế  mà cũng ăn");
	// 		listChat.push_back("Coi như không có em");
	// 	}
	// 	else if (gameID == kGameSam || gameID == kGameSamSolo)
	// 	{
	// 		listChat.push_back("Định chạy hả cưng");
	// 		listChat.push_back("Mạng lag quá");
	// 		listChat.push_back("Cho em chơi với");
	// 		listChat.push_back("Rất vui được gặp các bạn");
	// 		listChat.push_back("Hoy đi nha");
	// 		listChat.push_back("Bye Bye");
	// 		listChat.push_back("Hôm nay xui vãi =.=");
	// 		listChat.push_back("Thua đi cưng");
	// 		listChat.push_back("Ăn gì đỏ thế");
	// 		listChat.push_back("Bài xấu lại gặp đầu gấu");
	// 		listChat.push_back("Đừng hỏi vì sao");
	// 		listChat.push_back("Liều ăn nhiều");
	// 	}
	// 	else if (gameID == kGameTLMNDemLa || gameID == kGameDemLaSolo)
	// 	{
	// 		listChat.push_back("Solo đi thím");
	// 		listChat.push_back("Nhanh lên thím");
	// 		listChat.push_back("Mạng lag quá");
	// 		listChat.push_back("Rất vui được gặp các bạn");
	// 		listChat.push_back("Bye Bye");
	// 		listChat.push_back("Mạnh mẽ lên em");
	// 		listChat.push_back("Các bác cho em xin");
	// 		listChat.push_back("Thua đi cưng");
	// 		listChat.push_back("Ăn gì đỏ thế");
	// 		listChat.push_back("Bài xấu lại gặp đầu gấu");
	// 		listChat.push_back("Đừng hỏi vì sao");
	// 		listChat.push_back("Liều ăn nhiều");
	// 	}
	// 	else if (gameID == kGamePhom)
	// 	{
	// 		listChat.push_back("Xin đi rồi anh tha");
	// 		listChat.push_back("Đánh hài thế");
	// 		listChat.push_back("Mạng lag quá");
	// 		listChat.push_back("Rất vui được gặp các bạn");
	// 		listChat.push_back("Đuối như trái chuối");
	// 		listChat.push_back("Bye Bye");
	// 		listChat.push_back("Ha ha ha");
	// 		listChat.push_back("Thua đi cưng");
	// 		listChat.push_back("Ăn gì đỏ thế");
	// 		listChat.push_back("Bài xấu lại gặp đầu gấu");
	// 		listChat.push_back("Thế  mà cũng ăn");
	// 		listChat.push_back("Em xin =))");
	// 	}
	// 	else if (gameID == kGameChan)
	// 	{
	// 		listChat.push_back("Nhanh lên ông nội");
	// 		listChat.push_back("Thua đi cưng");
	// 		listChat.push_back("Mạng lag quá");
	// 		listChat.push_back("Rất vui được gặp các bạn");
	// 		listChat.push_back("Mình xin");
	// 		listChat.push_back("Bye Bye");
	// 		listChat.push_back("Mày hả bưởi");
	// 		listChat.push_back("Đánh hay đấy");
	// 		listChat.push_back("Ăn gì đỏ thế");
	// 		listChat.push_back("Nốt ván này rồi nghỉ");
	// 		listChat.push_back("Đừng hỏi vì sao");
	// 		listChat.push_back("Đánh Gà thế");
	// 	}
	// 	else{
	// 		listChat.push_back("Nhanh lên ông nội");
	// 		listChat.push_back("Thua đi cưng");
	// 		listChat.push_back("Mạng lag quá");
	// 		listChat.push_back("Rất vui được gặp các bạn");
	// 		listChat.push_back("Mình xin");
	// 		listChat.push_back("Bye Bye");
	// 		listChat.push_back("Mày hả bưởi");
	// 		listChat.push_back("Đánh hay đấy");
	// 		listChat.push_back("Đen vãi lúa");
	// 		listChat.push_back("Đã máu thì đừng hỏi bố cháu là ai");
	// 		listChat.push_back("Ăn gì đỏ thế");
	// 		listChat.push_back("Liều ăn nhiều");
	// 	}
}

bool LayerChatInGame::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerChatInGame::createPageViewEmo()
{
	if (!pvEmo)
		return;
	vector<string> lstPath;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		for (int i = 0; i < 45; i++)
		{
			lstPath.push_back(StringUtils::format("emo_%d.png", i + 1));
		}
		Layout* layout = Layout::create();
		layout->setContentSize(pvEmo->getContentSize());
		for (int i = 0; i < 20; i++)
		{
			Button *btnChat = Button::create(lstPath.at(i), "", "");
			btnChat->setName(StringUtils::format("emo_%d", i + 1));
			btnChat->setAnchorPoint(Vec2(0, 0));
			float posY;
			float dis = 25;
			int pos = i / 2;
			if (i % 2 == 0)
			{
				posY = btnChat->getContentSize().height;
			}
			else{
				posY = 0;
			}
			btnChat->setPosition(Vec2((dis + btnChat->getContentSize().width) * pos, posY));
			btnChat->addTouchEventListener(CC_CALLBACK_2(LayerChatInGame::onButtonClicked, this));
			layout->addChild(btnChat);
		}

		Layout* layout1 = Layout::create();
		layout1->setContentSize(pvEmo->getContentSize());
		for (int i = 20; i < 40; i++)
		{
			Button *btnChat = Button::create(lstPath.at(i), "", "");
			btnChat->setName(StringUtils::format("emo_%d", i + 1));
			btnChat->setAnchorPoint(Vec2(0, 0));
			float posY;
			float dis = 30;
			int pos = i / 2 - 10;
			if (i % 2 == 0)
			{
				posY = btnChat->getContentSize().height;
			}
			else{
				posY = 0;
			}
			btnChat->setPosition(Vec2((dis + btnChat->getContentSize().width) * pos, posY));
			btnChat->addTouchEventListener(CC_CALLBACK_2(LayerChatInGame::onButtonClicked, this));
			layout1->addChild(btnChat);
		}
		Layout* layout2 = Layout::create();
		layout2->setContentSize(pvEmo->getContentSize());

		for (int i = 40; i < lstPath.size(); i++)
		{
			Button *btnChat = Button::create(lstPath.at(i), "", "");
			btnChat->setName(StringUtils::format("emo_%d", i + 1));
			btnChat->setAnchorPoint(Vec2(0, 0));
			float posY;
			float dis = 30;
			int pos = i / 2 - 10;
			if (i % 2 == 0)
			{
				posY = btnChat->getContentSize().height;
			}
			else{
				posY = 0;
			}
			btnChat->setPosition(Vec2((dis + btnChat->getContentSize().width) * pos, posY));
			btnChat->addTouchEventListener(CC_CALLBACK_2(LayerChatInGame::onButtonClicked, this));
			layout2->addChild(btnChat);
		}
		pvEmo->insertPage(layout, 0);
		pvEmo->insertPage(layout1, 1);
		pvEmo->insertPage(layout2, 2);
		pvEmo->setUsingCustomScrollThreshold(true);
		pvEmo->setCustomScrollThreshold(pvEmo->getContentSize().width / 4);
		pvEmo->scrollToPage(0);
	}
	else{
		for (int i = 0; i < 20; i++)
		{
			lstPath.push_back(StringUtils::format("emo_%d.png", i + 1));
		}
		Layout* layout = Layout::create();
		layout->setContentSize(pvEmo->getContentSize());
		for (int i = 0; i < 14; i++)
		{
			Button *btnChat = Button::create(lstPath.at(i), "", "");
			btnChat->setName(StringUtils::format("emo_%d", i + 1));
			btnChat->setAnchorPoint(Vec2(0, 0));
			float posY;
			float dis = 25;
			int pos = i / 2;
			if (i % 2 == 0)
			{
				posY = btnChat->getContentSize().height;
			}
			else{
				posY = 0;
			}
			float width_ = btnChat->getContentSize().width;
			btnChat->setPosition(Vec2((dis + width_) * pos, posY));
			btnChat->addTouchEventListener(CC_CALLBACK_2(LayerChatInGame::onButtonClicked, this));
			layout->addChild(btnChat);
		}

		Layout* layout1 = Layout::create();
		layout1->setContentSize(pvEmo->getContentSize());
		for (int i = 14; i < lstPath.size(); i++)
		{
			Button *btnChat = Button::create(lstPath.at(i), "", "");
			btnChat->setName(StringUtils::format("emo_%d", i + 1));
			btnChat->setAnchorPoint(Vec2(0, 0));
			float posY;
			float dis = 20;
			int pos = i / 2 - 7;
			if (i % 2 == 0)
			{
				posY = btnChat->getContentSize().height;
			}
			else{
				posY = 0;
			}
			float width_ = btnChat->getContentSize().width;
			btnChat->setPosition(Vec2((dis + width_) * pos, posY));
			btnChat->addTouchEventListener(CC_CALLBACK_2(LayerChatInGame::onButtonClicked, this));
			layout1->addChild(btnChat);
		}
		pvEmo->insertPage(layout, 0);
		pvEmo->insertPage(layout1, 1);
		pvEmo->setUsingCustomScrollThreshold(true);
		pvEmo->setCustomScrollThreshold(pvEmo->getContentSize().width / 4);
		pvEmo->scrollToPage(0);
	}
}

void LayerChatInGame::showToast(string msg)
{
	auto currScene = Director::getInstance()->getRunningScene();
	if (currScene != NULL)
	{
		Chat* chatLayer = Chat::create(msg, -1);
		currScene->addChild(chatLayer);
	}
}

void LayerChatInGame::getListChatByID(int gameid)
{
	ValueMap data = FileUtils::getInstance()->getValueMapFromFile(StringUtils::format("chat/%schatconfig.plist",SceneManager::getSingleton().getFolderLang().c_str()));
	string key_value = StringUtils::format("%d", gameid);
	auto listvalue = data.at(key_value).asString();
	vector<string> arrStr = mUtils::splitString(listvalue, ';');
	for (int i = 0; i < arrStr.size(); i++)
		listChat.push_back(arrStr.at(i));
}

LayerMocChatInGame::LayerMocChatInGame()
{
	pvContent = NULL;
	pvEmo = NULL;
	this->createListChat();
	GameServer::getSingleton().addListeners(this);
}

LayerMocChatInGame::~LayerMocChatInGame()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerMocChatInGame::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size sizeAdd = ChanUtils::getSizePos();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerMocChatInGame.csb");
	ui::Helper::doLayout(rootNode);
	rootNode->setPositionY(rootNode->getPositionY() + sizeAdd.height);
	this->addChild(rootNode);

	this->pvContent = static_cast<PageView*>(rootNode->getChildByName("pvContent"));
	this->pvEmo = static_cast<PageView*>(rootNode->getChildByName("pvEmo"));
	this->txtChat = static_cast<TextField*>(rootNode->getChildByName("txtChat"));
    this->txtChat->setPlaceHolder(dataManager.GetSysString(27));
    if(SceneManager::getSingleton().isNagaWin){
        txtChat->setColor(Color3B::WHITE);
    }
	auto btnSend = static_cast<Button*>(rootNode->getChildByName("btnSendChat"));
    btnSend->setTitleText(dataManager.GetSysString(217));
    btnSend->setSwallowTouches(true);
    btnSend->setLocalZOrder(10);
	this->btnEmo = static_cast<Button*>(rootNode->getChildByName("btnIcon"));
	this->btnText = static_cast<Button*>(rootNode->getChildByName("btnText"));

	this->btnEmo->addTouchEventListener(CC_CALLBACK_2(LayerMocChatInGame::processButtonEmoTouched, this));
	this->btnText->addTouchEventListener(CC_CALLBACK_2(LayerMocChatInGame::processButtonTextTouched, this));
	btnSend->addTouchEventListener(CC_CALLBACK_2(LayerMocChatInGame::onBtnSend, this));

	auto btnTouchedOutChat = static_cast<Button*>(rootNode->getChildByName("btnOutChat"));
	btnTouchedOutChat->addTouchEventListener(CC_CALLBACK_2(LayerMocChatInGame::onButtonClose, this));

	this->createPageView();
	this->createPageViewEmo();
	return true;
}

void LayerMocChatInGame::hide()
{
	/*Size s = mUtils::getSizePos();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListenersForTarget(this);
	this->runAction(FadeOut::create(0.5));*/
	this->setVisible(false);
}

void LayerMocChatInGame::reportResultGame(string& result)
{
	try{
		boost::algorithm::trim(result);
	}
	catch (...){

	}
	result = mUtils::replaceAll(result, "\n", " ");

	ChatItem item;
	item.username = "Hệ thống";
	item.message = result;
}

void LayerMocChatInGame::onEnter()
{
	Layer::onEnter();
}

void LayerMocChatInGame::onExit()
{
	Layer::onExit();
}

void LayerMocChatInGame::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		this->setVisible(false);
	}
}

void LayerMocChatInGame::createPageView()
{
	if (!pvContent || listChat.size() == 0)
		return;
	Color3B colorText = Color3B::WHITE;
	int addWidth = 0;
	/*if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
	colorText = Color3B::WHITE;
	addWidth = 30;
	}*/

	//pvContent->setClippingEnabled(true);

	Layout* layout = Layout::create();
	layout->setContentSize(pvContent->getContentSize());
	layout->setSize(pvContent->getContentSize());
	int _sizeW = 0;
	int posX = 0;
	for (int i = 0; i < listChat.size(); i++)
	{
		Button *btnChat = Button::create();
		int _hasCharactor = this->countSpace(listChat.at(i));
		log("Size: %d - %s", _hasCharactor, listChat.at(i).c_str());
		int _sizeWOld = _sizeW;

		if (_hasCharactor <= 1){
			btnChat->loadTextures("text-button.png", "", "");
			_sizeW = 220;
		}
		else if (_hasCharactor == 2){
			btnChat->loadTextures("text-button-size-2.png", "", "");
			_sizeW = 300;
		}
		else {
			btnChat->loadTextures("text-button-size-3.png", "", "");
			_sizeW = 350;
		}
		btnChat->setTitleFontSize(40);
		//btnChat->setTitleFontName(kNormalFont);
		btnChat->setTitleText(listChat.at(i));
		btnChat->setAnchorPoint(Vec2(0, 0));
		btnChat->setTitleColor(colorText);
		float posY;
		float dis = 25;
		int pos = i / 3;
		if (pos != 0)
		{
			posY = layout->getContentSize().height - ((btnChat->getContentSize().height + dis) * (pos + 1));
		}
		else if (pos == 0){
			posY = layout->getContentSize().height - (btnChat->getContentSize().height + dis);
		}
		log("_sizeW: %d - posX: %d", _sizeWOld, posX);
		if (i % 3 == 0){
			posX = 0;
		}
		else{
			posX += _sizeWOld + dis;
		}
		btnChat->setPosition(Vec2(posX, posY));
		posX = btnChat->getPositionX();
		btnChat->addTouchEventListener(CC_CALLBACK_2(LayerMocChatInGame::processButtonClicked, this));
		layout->addChild(btnChat);
	}

	/*Layout* layout2 = Layout::create();
	layout2->setContentSize(pvContent->getContentSize());
	layout2->setSize(pvContent->getContentSize());
	for (int i = 6; i < listChat.size(); i++)
	{
	Button *btnChat = Button::create("text-button.png", "", "");
	btnChat->setTitleFontSize(35);
	btnChat->setTitleFontName(kNormalFont);
	btnChat->setTitleText(listChat.at(i));
	btnChat->setAnchorPoint(Vec2(0, 0));
	btnChat->setTitleColor(colorText);
	float posY;
	float dis = 40;
	int pos = (i - 6) / 2;
	if (i % 2 == 0)
	{
	posY = pvContent->getContentSize().height - btnChat->getContentSize().height - addWidth;
	}
	else{
	posY = btnChat->getContentSize().height / 2;
	}
	btnChat->setPosition(Vec2((dis + btnChat->getContentSize().width) * pos, posY));
	btnChat->addTouchEventListener(CC_CALLBACK_2(LayerMocChatInGame::processButtonClicked, this));
	layout2->addChild(btnChat);
	}
	*/
	pvContent->insertPage(layout, 0);
	//pvContent->insertPage(layout2, 1);
	pvContent->setUsingCustomScrollThreshold(true);
	pvContent->setCustomScrollThreshold(pvContent->getContentSize().width / 3);
	pvContent->scrollToPage(0);
}

void LayerMocChatInGame::createPageViewEmo()
{
	if (!pvEmo)
		return;
	vector<string> lstPath;
	for (int i = 0; i < 20; i++)
	{
		lstPath.push_back(StringUtils::format("emo_%d.png", i + 1));
	}
	Layout* layout = Layout::create();
	layout->setContentSize(pvEmo->getContentSize());
	for (int i = 0; i < 20; i++)
	{
		Button *btnChat = Button::create(lstPath.at(i), "", "");
		btnChat->setName(StringUtils::format("emo_%d", i + 1));
		btnChat->setAnchorPoint(Vec2(0, 0));
		float posY;
		float dis = 25;
		int pos = i / 5;
		int posX = i % 5;
		if (pos != 0)
		{
			posY = btnChat->getContentSize().height * pos;
		}
		else{
			posY = 0;
		}
		btnChat->setPosition(Vec2((dis + btnChat->getContentSize().width) * posX, posY));
		btnChat->addTouchEventListener(CC_CALLBACK_2(LayerMocChatInGame::processButtonClicked, this));
		layout->addChild(btnChat);
	}
    
    pvEmo->setLocalZOrder(9);


	/*Layout* layout1 = Layout::create();
	layout1->setContentSize(pvEmo->getContentSize());*/
	//for (int i = 20; i < 40; i++)
	//{
	//	Button *btnChat = Button::create(lstPath.at(i), "", "");
	//	btnChat->setName(StringUtils::format("emo_%d", i + 1));
	//	btnChat->setAnchorPoint(Vec2(0, 0));
	//	float posY;
	//	float dis = 30;
	//	int pos = i / 2 - 10;
	//	if (i % 2 == 0)
	//	{
	//		posY = btnChat->getContentSize().height;
	//	}
	//	else{
	//		posY = 0;
	//	}
	//	btnChat->setPosition(Vec2((dis + btnChat->getContentSize().width) * pos, posY));
	//	btnChat->addTouchEventListener(CC_CALLBACK_2(LayerMocChatInGame::processButtonClicked, this));
	//	layout1->addChild(btnChat);
	//}
	//Layout* layout2 = Layout::create();
	//layout2->setContentSize(pvEmo->getContentSize());

	//for (int i = 40; i < lstPath.size(); i++)
	//{
	//	Button *btnChat = Button::create(lstPath.at(i), "", "");
	//	btnChat->setName(StringUtils::format("emo_%d", i + 1));
	//	btnChat->setAnchorPoint(Vec2(0, 0));
	//	float posY;
	//	float dis = 30;
	//	int pos = i / 2 - 10;
	//	if (i % 2 == 0)
	//	{
	//		posY = btnChat->getContentSize().height;
	//	}
	//	else{
	//		posY = 0;
	//	}
	//	btnChat->setPosition(Vec2((dis + btnChat->getContentSize().width) * pos, posY));
	//	btnChat->addTouchEventListener(CC_CALLBACK_2(LayerMocChatInGame::processButtonClicked, this));
	//	layout2->addChild(btnChat);
	//}
	pvEmo->insertPage(layout, 0);
	//pvEmo->insertPage(layout1, 1);
	//pvEmo->insertPage(layout2, 2);
	pvEmo->setUsingCustomScrollThreshold(true);
	pvEmo->setCustomScrollThreshold(pvEmo->getContentSize().width / 4);
	pvEmo->scrollToPage(0);
}

void LayerMocChatInGame::createListChat()
{
	int gameID = SceneManager::getSingleton().getGameID();
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		if (gameID == kGameSam || gameID == kGameSamSolo)
			gameID = kGameSam;
		else if (gameID == kGameTLMNDemLa || gameID == kGameDemLaSolo)
			gameID = kGameTLMNDemLa;
		else if (gameID != kGamePhom || gameID != kGameChan || gameID != kGameBaCay)
			gameID = 1000;
		getListChatByID(gameID);
	}
	else{
		getListChatByID(1001);
	}
}

void LayerMocChatInGame::getListChatByID(int gameid)
{
	ValueMap data = FileUtils::getInstance()->getValueMapFromFile(StringUtils::format("chat/%schatconfig.plist",SceneManager::getSingleton().getFolderLang().c_str()));
	string key_value = StringUtils::format("%d", gameid);
	auto listvalue = data.at(key_value).asString();
	log("list chat: %s", listvalue.c_str());
	vector<string> arrStr = mUtils::splitString(listvalue, ';');
	for (int i = 0; i < arrStr.size(); i++)
		listChat.push_back(arrStr.at(i));
}

void LayerMocChatInGame::showToast(string msg)
{
	auto currScene = Director::getInstance()->getRunningScene();
	if (currScene != NULL)
	{
		Chat* chatLayer = Chat::create(msg, -1);
		currScene->addChild(chatLayer);
	}
}

void LayerMocChatInGame::doChat(string txt, bool isEmo)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		int acctype = 0;
		if (mySelf != NULL)
		{
			boost::shared_ptr<UserVariable> acctype_ptr = mySelf->GetVariable("accVipType");
			if (acctype_ptr != NULL)
				acctype = *acctype_ptr->GetIntValue();
		}
		Scene *scene = Director::getInstance()->getRunningScene();
		ScenePickRoom* gameScene = dynamic_cast<ScenePickRoom*>(scene->getChildByTag(8866));
		if (acctype < dataManager.getAppConfig().chatlevel && gameScene != NULL)
		{
			this->showToast(StringUtils::format("%s %d %s ",dataManager.GetSysString(22).c_str(), dataManager.getAppConfig().chatlevel,dataManager.GetSysString(23).c_str()));
			return;
		}
		if (gameScene != NULL){
			if (SceneManager::getSingleton().chatcounttime == 0) {
				SceneManager::getSingleton().chatcounttime++;
				SceneManager::getSingleton().timerchat++;
				gameScene->startTimeOutChat();
			}

			if (checkSpamChat(txt)) {
				this->showToast(dataManager.GetSysString(24));
				return;
			}

		}
	}

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
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
			if (isEmo)
				this->showToast(dataManager.GetSysString(25));
			else
				this->showToast(dataManager.GetSysString(26));
			return;
		}
		else if (acctype == dataManager.getAppConfig().chatlevel && !isEmo)
		{
			this->showToast(dataManager.GetSysString(26));
			return;
		}
	}


	if (_ispick)
	{
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom)
		{
			string myName = SceneManager::getSingleton().getMyName();
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutUtfString(EXT_FIELD_USERNAME, myName);
			params->PutUtfString(EXT_FIELD_CHAT_MESSAGE, mUtils::filterForbidenChat(txt));
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_CHAT_REQ, params, lastRoom));
			GameServer::getSingleton().Send(request);
			txtChat->setString("");
			this->hide();
		}
	}
	else{
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> ro = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (ro != NULL)
		{
            if (dataManager.getAppConfig().is24h)
            {
                return;
            }
			txtChat->setString("");
			this->hide();
			boost::shared_ptr<IRequest> request(new PublicMessageRequest(mUtils::filterForbidenChat(txt), params, ro));
			GameServer::getSingleton().Send(request);
		}
	}
}

void LayerMocChatInGame::onBtnSend(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
#if(CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
		if (string(txtChat->getString()).length() == 0)
			return;
		doChat(string(txtChat->getString()), false);
#endif
	}
}

void LayerMocChatInGame::processButtonClicked(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto button = (ui::Button*)pSender;
		string btn_name = button->getName();
		if (0 == btn_name.compare(""))
		{
			doChat(button->getTitleText(), false);
		}
		else{
			doChat(btn_name, true);
		}

		hide();
	}
}

bool LayerMocChatInGame::checkSpamChat(string txt)
{
	if (txt.length() >= 10){
		std::transform(txt.begin(), txt.end(), txt.begin(), ::tolower);
		for (int i = 0; i < SceneManager::getSingleton().arrCurrentChat.size(); i++)
		{
			while (strstr(txt.c_str(), SceneManager::getSingleton().arrCurrentChat[i].txt.c_str())){

				if (SceneManager::getSingleton().arrCurrentChat[i].count < dataManager.getAppConfig().chattimemax){
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return false;
				}
				else{
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return true;
				}
			}
			while (strstr(SceneManager::getSingleton().arrCurrentChat[i].txt.c_str(), txt.c_str())){

				if (SceneManager::getSingleton().arrCurrentChat[i].count < dataManager.getAppConfig().chattimemax){
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return false;
				}
				else{
					SceneManager::getSingleton().arrCurrentChat[i].count++;
					return true;
				}
			}
		}
		SceneManager::chatspam a;
		a.count = 1;
		a.txt = txt;
		SceneManager::getSingleton().arrCurrentChat.push_back(a);
		return false;
	}
	return false;
}

void LayerMocChatInGame::processButtonEmoTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
		this->btnEmo->setEnabled(false);
		this->btnEmo->setBright(false);
		this->btnText->setEnabled(true);
		this->btnText->setBright(true);
		this->pvContent->setVisible(false);
		this->pvEmo->setVisible(true);
	}
}

void LayerMocChatInGame::processButtonTextTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
		this->btnEmo->setEnabled(true);
		this->btnEmo->setBright(true);
		this->btnText->setEnabled(false);
		this->btnText->setBright(false);
		this->pvEmo->setVisible(false);
		this->pvContent->setVisible(true);
	}
}

int LayerMocChatInGame::countSpace(std::string _string)
{
	int count = 0;
	for (int i = 0; i < _string.size(); ++i){
		\
		if (_string[i] == ' ')
			count++;
	}
	return count;
}
