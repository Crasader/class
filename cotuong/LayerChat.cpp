#include "LayerChat.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "CommonChess.h"
#include "mUtils.h"
#include "Requests/PublicMessageRequest.h"
#include "gameUtils.h"
#include "MyAnimation.h"
#include "Requests/ExtensionRequest.h"
#include "../SceneManager.h"
USING_NS_CC;

// on "init" you need to initialize your instance
bool LayerChat::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerChat.csb");
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	Layout* pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
		svChat = (cocos2d::ui::ScrollView*)pnlMain->getChildByName("svChat");
		btnEmotion = (Button*)pnlMain->getChildByName("btnEmotion");
		txtContent = (TextField*)pnlMain->getChildByName("txtContent");
		btnSend = (Button*)pnlMain->getChildByName("btnSend");
		pnlContent = (Layout*)pnlMain->getChildByName("pnlContent");
		Layout* Panel_1 = (Layout*)pnlMain->getChildByName("Panel_1");
		bt_sound2_24 = (Sprite*)btnEmotion->getChildByName("bt_sound2_24");

		txtContent->setMaxLength(50);
		pnlContent->setTouchEnabled(true);
		pnlContent->setSwallowTouches(true);
		Panel_1->setSwallowTouches(true);
		Panel_1->setTouchEnabled(true);
		//pnlContent->addClickEventListener(CC_CALLBACK_1(LayerChat::onButtonClose, this));

		Button* btnClose = (Button*)pnlMain->getChildByName("btnClose");
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerChat::onButtonClose, this));

		this->initChatContent();

		tblChat = TableView::create(this, Size(pnlContent->getContentSize().width,
			pnlContent->getContentSize().height));
		tblChat->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
		tblChat->setAnchorPoint(Vec2(0, 0));
		tblChat->setPosition(0, 0);
		tblChat->setDelegate(this);
		tblChat->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		pnlContent->addChild(tblChat, 10);
		tblChat->reloadData();

		svChat->addClickEventListener(CC_CALLBACK_1(LayerChat::onButtonClose, this));
		btnEmotion->addClickEventListener(CC_CALLBACK_1(LayerChat::onButtonEmotion, this));
		btnSend->addClickEventListener(CC_CALLBACK_1(LayerChat::onButtonChat, this));
		pnlContent->addTouchEventListener(CC_CALLBACK_2(LayerChat::onButtonPanelChat, this));
		btnSend->setSwallowTouches(true);
		svChat->setSwallowTouches(true);
	}

	return true;
}

LayerChat::LayerChat()
{
	GameServer::getSingleton().addListeners(this);
}

LayerChat::~LayerChat()
{
	GameServer::getSingleton().removeListeners(this);
}

void LayerChat::onButtonClose(Ref* pSender)
{
	this->hideChat();
}

void LayerChat::initChatContent()
{
	svChat->setClippingEnabled(true);
	ValueMap data = FileUtils::getInstance()->getValueMapFromFile("chatconfig.plist");
	string key_value = StringUtils::format("%d", 100);
	auto listvalue = data.at(key_value).asString();
	vector<string> arrStr = mUtils::splitString(listvalue, ';');
	for (unsigned int i = 0; i < arrStr.size(); i++)
		listChats.push_back(arrStr.at(i));

	//add list to scrollview
	int jumpunit = 2;
	int pageCount = 3;
	int distanceX = 5;
	int distanceY = 10;
	Size sizeIcon = Size(453, 130);//size button

	int rows = 1;
	int cols = listChats.size() / rows;
	unsigned int startIndex = 0;

	svChat->setBounceEnabled(true);
	for (int i = 0; i < rows; i++)
	{
		float posX = sizeIcon.width * i + 200;
		for (int j = 0; j < cols; j++)
		{

			float posY = sizeIcon.height * j;
			Button* buttonGame = Button::create("bg-text-chat.png");
			if (startIndex < listChats.size())
				buttonGame->setTitleText(listChats.at(startIndex));

			buttonGame->setAnchorPoint(Vec2(0, 0));
			buttonGame->setPosition(Vec2(posX, posY));
			buttonGame->setTitleFontSize(FONT_SIZE_ITEM);
			buttonGame->setTitleFontName("__Roboto-Bold.ttf");
			buttonGame->addClickEventListener(CC_CALLBACK_1(LayerChat::onButtonContentChat, this));
			svChat->addChild(buttonGame);
			startIndex++;
		}
	}
}

void LayerChat::initEmotion()
{

}

void LayerChat::onButtonContentChat(Ref* pSender)
{
	Button* btn = (Button*)pSender;
	log("Title button = %s", btn->getTitleText().c_str());
	string _content = btn->getTitleText();
	doChat(_content, false);
}

void LayerChat::onButtonChat(Ref* pSender)
{
	if (string(txtContent->getString()).length() == 0)
	{
		SceneManager::getSingleton().showToast(dataManager.GetSysString(279));
		return;
	}
	doChat(string(txtContent->getString()), false);
}

void LayerChat::doChat(string txt, bool isEmo)
{
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> ro = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (ro != NULL)
	{
		txtContent->setString("");
		boost::shared_ptr<IRequest> request(new PublicMessageRequest(txt, params, ro));
		GameServer::getSingleton().Send(request);
		this->hideChat();
	}
}

void LayerChat::setListChat(string _username, string _msg)
{
	if (listStore.size() > MAX_CHAT_LENGTH)
		listStore.pop_back();
	listStore.push_back(ChatInfoStruct(_username, _msg));
	tblChat->reloadData();
}

void LayerChat::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
	this->onButtonClose(NULL);
}

cocos2d::Size LayerChat::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return Size(pnlContent->getContentSize().width, FONT_SIZE_ITEM * 2);
}

cocos2d::extension::TableViewCell* LayerChat::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
	Color3B colorName = COLOR_MYNAME_CELL;
	string _userName = listStore.at(idx).userName;
	string _namePlay = GameUtils::getNamePlayer(_userName);
	string content_msg = _namePlay + " : " + listStore.at(idx).userMsg;

	if (0 != listStore.at(idx).userName.compare(GameUtils::getMyName()) ||
		0 != listStore.at(idx).userName.compare("Hệ thống"))
	{
		colorName = COLOR_DEFAULT_CELL;
	}

	if (!cell)
	{
		cell = new(std::nothrow) TableViewCell();
		cell->autorelease();

		//cell->removeAllChildren();
		int startX = 10;
		if (listStore.size() > 0)
		{
			// 	///content
			//richText->setContentSize(Size(panelTable->getContentSize().width - 20, getHeightForCell(mess)));

			Label *labelContent = Label::createWithTTF(content_msg, "__Roboto-Bold.ttf", FONT_SIZE_ITEM);
			labelContent->setClipMarginEnabled(true);
			labelContent->setAnchorPoint(Vec2::ZERO);
			labelContent->setPosition(Vec2(startX, 0));
			labelContent->setColor(Color3B::WHITE);
			labelContent->setLineBreakWithoutSpace(false);
			//labelContent->setScale(1.25);
			labelContent->setMaxLineWidth(pnlContent->getContentSize().width - 110);
			labelContent->setTag(123);
			cell->addChild(labelContent);
			long size = StringUtils::getCharacterCountInUTF8String(listStore.at(idx).userName);
			for (unsigned int i = 0; i < size; i++)
			{
				Sprite *spriteText = labelContent->getLetter(i);
				if (spriteText)
				{
					spriteText->setColor(Color3B::YELLOW);
				}
			}
		}
	}
	else{
		Label* labelContent = (Label*)cell->getChildByTag(123);
		if (labelContent != NULL)
		{
			labelContent->setString(content_msg);
			long size = StringUtils::getCharacterCountInUTF8String(listStore.at(idx).userName);
			for (unsigned int i = 0; i < size; i++)
			{
				Sprite *spriteText = labelContent->getLetter(i);
				if (spriteText)
				{
					spriteText->setColor(Color3B::YELLOW);
				}
			}
		}
	}

	return cell;
}

ssize_t LayerChat::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return listStore.size();
}

void LayerChat::showChat()
{
	MyAnimation::createActionShowPopup(this, 0.2f);
}

void LayerChat::hideChat()
{
	MyAnimation::createActionHiddenPopup(this, 0.2f);
}

void LayerChat::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	//log("cmd = %s", cmd->c_str());
	if (0 == cmd->compare(EXT_EVENT_READY_NTF))
	{
	}
	else if (0 == cmd->compare(EXT_EVENT_LIST_USER_UPDATE))
	{
		boost::shared_ptr<string>  listPtr = param->GetUtfString(EXT_FIELD_LIST_USER);
		if (listPtr != NULL)
		{
			isSpectator = this->checkSpectator(listPtr->c_str(), GameUtils::getMyName());
			isBossRoom = GameUtils::isBossRoom(listPtr->c_str());
		}
	}
}

bool LayerChat::checkSpectator(string listUser, string myName)
{
	vector<string> arr = mUtils::splitString(listUser, ';');
	int size = arr.size();
	for (int i = 0; i < size; i++){
		vector<string> arrInfo = mUtils::splitString(arr.at(i), '|');
		if (arrInfo.size() < 2) continue;
		string name = arrInfo.at(0);
		if (0 == name.compare(myName)) return false;
	}

	return true;
}

void LayerChat::onButtonEmotion(Ref * pSender)
{
	if (isSpectator)
		return;
	bool isFilter = SceneManager::getSingleton().getFilterChat();
	bool filterVal = !isFilter;
	SceneManager::getSingleton().setFilterChat(filterVal);
	string pathImg = "bt_sound1.png";
	string notifyMess = "Bật nội dung chat từ người xem";
	if (filterVal)//tat khong xem 
	{
		pathImg = "bt_sound2.png";
		notifyMess = "Tắt nội dung chat từ người xem";
	}
	bt_sound2_24->setTexture(TextureCache::getInstance()->addImage(pathImg));
	listStore.push_back(ChatInfoStruct("Hệ thống", notifyMess));
	tblChat->reloadData();
}

void LayerChat::onButtonPanelChat(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		this->onButtonClose(NULL);
	}
}



