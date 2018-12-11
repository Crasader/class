#include "LayerConfigRoomChan.h"
#include "AllData.h"
#include "GameServer.h"
#include "ChanClientMessageDef.h"
#include "Requests/ExtensionRequest.h"
#include "SceneManager.h"
#include "xmllite/xmlParser.h"
#include "ChanUtils.h"

#include "tinyxml2/tinyxml2.h"

//! Test xml smart fox
#include "Util/XMLParser/XMLParser.h"
#include "Util/XMLParser/XMLNode.h"
#include "PhomMessDef.h"
#define kTAG_BTN_SETTING 10
#define kTAG_BTN_CANCEL 11

static std::string getValueByKey(const char* pKey, const string& xml)
{
	tinyxml2::XMLElement* curNode = nullptr;
	if (!pKey) return "";

	do
	{
		tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();

		std::string xmlBuffer = xml;

		if (!xmlBuffer.empty()) {

			xmlDoc->Parse(xmlBuffer.c_str(), xmlBuffer.size());

			// get root node
			tinyxml2::XMLElement* rootNode = xmlDoc->RootElement();
			if (nullptr != rootNode) {
				// find the node
				curNode = rootNode->FirstChildElement();
				while (nullptr != curNode)
				{
					const char* nodeName = curNode->Value();
					if (!strcmp(nodeName, pKey))
						break;
					curNode = curNode->NextSiblingElement();
				}
			}

			//if(rootNode) delete rootNode;
		}
		if (xmlDoc) delete xmlDoc;
	} while (0);

	string value = "";
	if (curNode && curNode->FirstChild()){
		value = boost::to_string((curNode->FirstChild()->Value()));
	}

	return value;
}

LayerConfigRoomChan::LayerConfigRoomChan(const bool& isBoss)
:txtTimeoutStep(NULL),
cbxNuoiGa(NULL),
cbxChoiUXuong(NULL),
cbxChoiDuoi4Diem(NULL),
_bgParent(NULL),
IsBossRoom(isBoss)
{
	ArrOption.clear();
	log("Constructor LayerConfigRoomChan");
}

LayerConfigRoomChan* LayerConfigRoomChan::create(const bool& isBoss)
{
	LayerConfigRoomChan* ConfigLayer = new LayerConfigRoomChan(isBoss);
	if (ConfigLayer && ConfigLayer->init())
	{
		ConfigLayer->autorelease();
		return ConfigLayer;
	}
	else
	{
		delete ConfigLayer;
		ConfigLayer = NULL;
		return NULL;
	}
}

LayerConfigRoomChan::~LayerConfigRoomChan(void)
{
	ArrOption.clear();
	log("Destructor LayerConfigRoomChan");
}

bool LayerConfigRoomChan::init()
{
	if (!Layer::init())
		return false;

	this->setAnchorPoint(Vec2(0, 0));

	Size size = Director::getInstance()->getVisibleSize();
	float scaleX = size.width / WIDTH_DESIGN;
	float scaleY = size.height / HEIGHT_DESIGN;
	float scaleMin = (scaleX < scaleY) ? scaleX : scaleY;
	float scaleMax = (scaleX > scaleY) ? scaleX : scaleY;
	float PosX = 0;
	float PosY = 0;
	if (scaleMax == scaleX)
	{
		PosX = ((size.width - WIDTH_DESIGN * scaleMin) / 2) / scaleMin;
		PosY = 0;
	}
	if (scaleMax == scaleY)
	{
		PosX = 0;
		PosY = ((size.height - HEIGHT_DESIGN * scaleMin) / 2) / scaleMin;
	}

	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 200));
	color->setAnchorPoint(Vec2(0, 0));
	color->setPosition(Vec2(-PosX, -PosY));
	color->setScaleX(size.width / WIDTH_DESIGN);
	color->setScaleY(size.height / HEIGHT_DESIGN);
	color->setOpacity(175);
	this->addChild(color);

	Sprite* sprite = Sprite::create("background_config_chan.png");
	sprite->setAnchorPoint(Vec2(0.5, 0.5));
	sprite->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
	this->addChild(sprite);

	Sprite* sprite1 = Sprite::create("moc3-bg-setup.png");
	sprite1->setAnchorPoint(Vec2(0.5, 0.5));
	sprite1->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
	this->addChild(sprite1);

	Sprite* sprite2 = Sprite::create("title-caidat.png");
	sprite2->setAnchorPoint(Vec2(0.5, 0.5));
	sprite2->setPosition(Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height - 100));
	sprite->addChild(sprite2);

	//
	PosX = 650;
	PosY = HEIGHT_DESIGN / 2 + 50;
	float fontSize = SIZE_FONT_CHAN;
	float scaleSize = 1.3;
	float SpaceY = 70;
	float spaceX = 100;
	Color3B cl(68, 63, 47);

	/*init giao dien
	1- option chon background ban choi
	2- choi nuoi ga, ko choi u xuong, ko choi u duoi 4 diem - left
	3- config thoi gian, muc cuoc, setup am thanh - right
	*/
	// Chọn hình nền của bàn
	//CheckBox* cbxOption1 = CheckBox::create();
	//cbxOption1->loadTextures("option_chan.png", "", "option_chan_selected.png", "", "");
	//cbxOption1->addClickEventListener(CC_CALLBACK_1(LayerConfigRoomChan::OnOptionClick, this));
	//cbxOption1->setAnchorPoint(Vec2(0, 0));
	//cbxOption1->setPosition(Vec2(size.width / 3 + 45, sprite1->getPositionY() + sprite1->getContentSize().height / 2 + 20));
	//cbxOption1->setTouchEnabled(true);
	//cbxOption1->setTag(7);
	//this->addChild(cbxOption1);
	//this->ArrOption.push_back(cbxOption1);

	//Label* lblKieu1 = Label::createWithBMFont(LucidaGrandeFnt, "Kiểu 1");
	//lblKieu1->setScale(scaleSize);
	//lblKieu1->setColor(cl);
	//lblKieu1->setAnchorPoint(Vec2(0, 0));
	//lblKieu1->setPosition(Vec2(cbxOption1->getPositionX() + 60, cbxOption1->getPositionY() + 2));
	//this->addChild(lblKieu1);

	//CheckBox* cbxOption2 = CheckBox::create();
	//cbxOption2->loadTextures("option_chan.png", "", "option_chan_selected.png", "", "");
	//cbxOption2->addClickEventListener(CC_CALLBACK_1(LayerConfigRoomChan::OnOptionClick, this));
	//cbxOption2->setAnchorPoint(Vec2(0, 0));
	//cbxOption2->setPosition(Vec2(cbxOption1->getPositionX() + 200, cbxOption1->getPositionY()));
	//cbxOption2->setTouchEnabled(true);
	//cbxOption2->setTag(8);
	//this->addChild(cbxOption2);
	//this->ArrOption.push_back(cbxOption2);

	//Label* lblKieu2 = Label::createWithBMFont(LucidaGrandeFnt, "Kiểu 2");
	//lblKieu2->setScale(scaleSize);
	//lblKieu2->setAnchorPoint(Vec2(0, 0));
	//lblKieu2->setColor(cl);
	//lblKieu2->setPosition(Vec2(cbxOption2->getPositionX() + 60, cbxOption2->getPositionY() + 2));
	//this->addChild(lblKieu2);

	//CheckBox* cbxOption3 = CheckBox::create();
	//cbxOption3->loadTextures("option_chan.png", "", "option_chan_selected.png", "", "");
	//cbxOption3->addClickEventListener(CC_CALLBACK_1(LayerConfigRoomChan::OnOptionClick, this));
	//cbxOption3->setAnchorPoint(Vec2(0, 0));
	//cbxOption3->setPosition(Vec2(cbxOption2->getPositionX() + 200, cbxOption2->getPositionY()));
	//cbxOption3->setTouchEnabled(true);
	//cbxOption3->setTag(9);
	//this->addChild(cbxOption3);
	//this->ArrOption.push_back(cbxOption3);

	//Label* lblKieu3 = Label::createWithBMFont(LucidaGrandeFnt, "Kiểu 3");
	//lblKieu3->setScale(scaleSize);
	//lblKieu3->setColor(cl);
	//lblKieu3->setAnchorPoint(Vec2(0, 0));
	//lblKieu3->setPosition(Vec2(cbxOption3->getPositionX() + 60, cbxOption3->getPositionY() + 2));
	//this->addChild(lblKieu3);

	//3- muc cuoc, thoi gian danh, am thanh
	Label* Cuoc = Label::createWithTTF(dataManager.GetSysString(551),kBoldFont,25);
	Cuoc->setScale(scaleSize);
	Cuoc->setAnchorPoint(Vec2(0, 0));
	Cuoc->setPosition(Vec2(sprite1->getPositionX() + spaceX / 2, sprite1->getPositionY() + sprite1->getContentSize().height / 2 - SpaceY - 20));
	Cuoc->setColor(cl);
	this->addChild(Cuoc);

	Label* LblSoTien = Label::createWithTTF("",kBoldFont,25);
	LblSoTien->setScale(scaleSize);
	LblSoTien->setAnchorPoint(Vec2(0, 0));
	LblSoTien->setPosition(Vec2(Cuoc->getPositionX() + 100, Cuoc->getPositionY()));
	LblSoTien->setColor(cl);
	this->addChild(LblSoTien);

	// Hiển thị thời gian đánh
	Label* Time = Label::createWithTTF(dataManager.GetSysString(668),kBoldFont,25);
	Time->setScale(scaleSize);
	Time->setAnchorPoint(Vec2(0, 0));
	Time->setPosition(Vec2(Cuoc->getPositionX(), Cuoc->getPositionY() - SpaceY));
	Time->setColor(cl);
	this->addChild(Time);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	this->txtTimeoutStep = TextFieldTTFDefaultTest::create();
	this->txtTimeoutStep->setHolder("<30>");
	this->txtTimeoutStep->setPosition(Vec2((WIDTH_DESIGN / 2) - 25, Time->getPositionY() - 3));
	this->txtTimeoutStep->setString("30");
#else
	this->txtTimeoutStep = ui::EditBox::create(Size(70, 45), ui::Scale9Sprite::create("textbox_timeout_step.png"));
	this->txtTimeoutStep->setAnchorPoint(Vec2(0, 0));
	this->txtTimeoutStep->setPosition(Vec2(Time->getPositionX() + Time->getContentSize().width + SpaceY, Time->getPositionY() - 3));
	this->txtTimeoutStep->setFontColor(cl);
	this->txtTimeoutStep->setMaxLength(2);
	this->txtTimeoutStep->setText("30");
	this->txtTimeoutStep->setFont(ChanDefaultFont, fontSize);
	this->txtTimeoutStep->setPlaceholderFont(ChanDefaultFont, fontSize);
	//txtTimeoutStep->setTouchPriority(-129);
	this->txtTimeoutStep->setInputMode(ui::EditBox::InputMode::ANY);
	this->txtTimeoutStep->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
	this->txtTimeoutStep->setTouchEnabled(true);
#endif
	this->addChild(txtTimeoutStep);

	Label* Giay = Label::createWithBMFont(LucidaGrandeFnt, "(s)");
	Giay->setScale(scaleSize);
	Giay->setAnchorPoint(Vec2(0, 0));
	Giay->setPosition(Vec2(this->txtTimeoutStep->getPositionX() + this->txtTimeoutStep->getContentSize().width + 10, Time->getPositionY()));
	Giay->setColor(cl);
	this->addChild(Giay);
	// Âm thanh
	UserDefault *def = UserDefault::getInstance();
	CheckBox* cbxSound = CheckBox::create();
	cbxSound->loadTextures("chan_chek_box.png", "", "chan_chek_box_selected.png", "", "");
	cbxSound->setSelectedState(def->getBoolForKey("sound", true));
	//chbVibrate->addEventListener(CC_CALLBACK_2(LayerConfigInGame::selectedEvent, this));
	cbxSound->addEventListener(CC_CALLBACK_2(LayerConfigRoomChan::OnCheckBoxClick, this));
	cbxSound->setAnchorPoint(Vec2(0, 0));
	cbxSound->setPosition(Vec2(Cuoc->getPositionX(), Time->getPositionY() - SpaceY));
	cbxSound->setTouchEnabled(true);
	cbxSound->setTag(871);
	this->addChild(cbxSound);

	Label* lblSound = Label::createWithTTF(dataManager.GetSysString(486),kBoldFont, 25);
	lblSound->setScale(scaleSize);
	lblSound->setAnchorPoint(Vec2(0, 0));
	lblSound->setColor(cl);
	lblSound->setPosition(Vec2(cbxSound->getPositionX() + 60, cbxSound->getPositionY() + 2));
	this->addChild(lblSound);


	// 2- Hiển thị Check box nuôi gà
	this->cbxNuoiGa = CheckBox::create();
	this->cbxNuoiGa->loadTextures("chan_chek_box.png", "", "chan_chek_box_selected.png", "", "");
	this->cbxNuoiGa->setAnchorPoint(Vec2(0, 0));
	this->cbxNuoiGa->setPosition(Vec2(sprite1->getPositionX() - sprite1->getContentSize().width / 2 + spaceX / 2, sprite1->getPositionY() + sprite1->getContentSize().height / 2 - SpaceY - 20));
	this->cbxNuoiGa->setTouchEnabled(true);
	this->cbxNuoiGa->addEventListener(CC_CALLBACK_2(LayerConfigRoomChan::OnCheckBoxClick, this));
	this->addChild(this->cbxNuoiGa);

	Label* lblChoiNuoiGa = Label::createWithTTF(dataManager.GetSysString(552),kBoldFont,25);
	lblChoiNuoiGa->setScale(scaleSize);
	lblChoiNuoiGa->setAnchorPoint(Vec2(0, 0));
	lblChoiNuoiGa->setColor(cl);
	lblChoiNuoiGa->setPosition(Vec2(this->cbxNuoiGa->getPositionX() + 60, this->cbxNuoiGa->getPositionY() + 2));
	this->addChild(lblChoiNuoiGa);

	// Check Box Ù Xuông
	this->cbxChoiUXuong = CheckBox::create();
	this->cbxChoiUXuong->loadTextures("chan_chek_box.png", "", "chan_chek_box_selected.png", "", "");
	this->cbxChoiUXuong->addEventListener(CC_CALLBACK_2(LayerConfigRoomChan::OnCheckBoxClick, this));
	this->cbxChoiUXuong->setAnchorPoint(Vec2(0, 0));
	this->cbxChoiUXuong->setPosition(Vec2(this->cbxNuoiGa->getPositionX(), this->cbxNuoiGa->getPositionY() - SpaceY));
	this->cbxChoiUXuong->setTouchEnabled(true);
	this->addChild(this->cbxChoiUXuong);

	Label* lblChoiUXuong = Label::createWithTTF(dataManager.GetSysString(909),kBoldFont,25);
	lblChoiUXuong->setScale(scaleSize);
	lblChoiUXuong->setAnchorPoint(Vec2(0, 0));
	lblChoiUXuong->setColor(cl);
	lblChoiUXuong->setPosition(Vec2(this->cbxChoiUXuong->getPositionX() + 60, this->cbxChoiUXuong->getPositionY() + 2));
	this->addChild(lblChoiUXuong);


	// Check Box Ù dưới 4 điểm
	this->cbxChoiDuoi4Diem = CheckBox::create();
	this->cbxChoiDuoi4Diem->loadTextures("chan_chek_box.png", "", "chan_chek_box_selected.png", "", "");
	this->cbxChoiDuoi4Diem->setAnchorPoint(Vec2(0, 0));
	this->cbxChoiDuoi4Diem->setPosition(Vec2(this->cbxChoiUXuong->getPositionX(), this->cbxChoiUXuong->getPositionY() - SpaceY));
	this->cbxChoiDuoi4Diem->setTouchEnabled(true);
	this->cbxChoiDuoi4Diem->addEventListener(CC_CALLBACK_2(LayerConfigRoomChan::OnCheckBoxClick, this));
	this->addChild(this->cbxChoiDuoi4Diem);

	Label* lblUDuoi4Diem = Label::createWithTTF(dataManager.GetSysString(84),kBoldFont,25);
	lblUDuoi4Diem->setScale(scaleSize);
	lblUDuoi4Diem->setColor(cl);
	lblUDuoi4Diem->setAnchorPoint(Vec2(0, 0));
	lblUDuoi4Diem->setPosition(Vec2(this->cbxChoiDuoi4Diem->getPositionX() + 60, this->cbxChoiDuoi4Diem->getPositionY() + 2));
	this->addChild(lblUDuoi4Diem);

	//// Line ngăn cách
	//Label* lblLine = Label::createWithBMFont(LucidaGrandeFnt, "----------------------");
	//lblLine->setScale(scaleSize);
	//lblLine->setAnchorPoint(Vec2(0, 0));
	//lblLine->setColor(cl);
	//lblLine->setPosition(Vec2(this->cbxChoiDuoi4Diem->getPositionX(), this->cbxChoiDuoi4Diem->getPositionY() - 30));
	//this->addChild(lblLine);

	// Hiển thị 2 button cài đặt
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		MenuItemImage *BtnSetting = CreateButton("", kTAG_BTN_SETTING, IsBossRoom);
		MenuItemImage *BtnCancel = CreateButton("", kTAG_BTN_CANCEL);

		BtnSetting->setPosition(Vec2((WIDTH_DESIGN >> 1) - BtnSetting->getContentSize().width - 50, (HEIGHT_DESIGN >> 1) - (sprite->getContentSize().height / 2) - 38));
		BtnCancel->setPosition(Vec2((WIDTH_DESIGN >> 1) + 50, (HEIGHT_DESIGN >> 1) - (sprite->getContentSize().height / 2) - 38));
		if (!IsBossRoom)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#else
			txtTimeoutStep->setEnabled(false);
#endif
			cbxNuoiGa->setTouchEnabled(false);
			cbxChoiUXuong->setTouchEnabled(false);
			cbxChoiDuoi4Diem->setTouchEnabled(false);
			BtnSetting->setEnabled(false);
		}

		Menu* menu = Menu::create(BtnSetting, BtnCancel, NULL);
		menu->setAnchorPoint(Vec2(0, 0));
		menu->setPosition(Vec2(0, 0));
		this->addChild(menu);

	}
	else{
		MenuItemImage *BtnSetting = CreateButton(dataManager.GetSysString(553).c_str(), kTAG_BTN_SETTING, IsBossRoom);
		MenuItemImage *BtnCancel = CreateButton(dataManager.GetSysString(567).c_str(), kTAG_BTN_CANCEL);

		BtnSetting->setPosition(Vec2((WIDTH_DESIGN >> 1) - BtnSetting->getContentSize().width - 50, (HEIGHT_DESIGN >> 1) - (sprite->getContentSize().height / 2) - 8));
		BtnCancel->setPosition(Vec2((WIDTH_DESIGN >> 1) + 50, (HEIGHT_DESIGN >> 1) - (sprite->getContentSize().height / 2) - 8));
		if (!IsBossRoom)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#else
			txtTimeoutStep->setEnabled(false);
#endif
			cbxNuoiGa->setTouchEnabled(false);
			cbxChoiUXuong->setTouchEnabled(false);
			cbxChoiDuoi4Diem->setTouchEnabled(false);
			BtnSetting->setEnabled(false);
		}

		Menu* menu = Menu::create(BtnSetting, BtnCancel, NULL);
		menu->setAnchorPoint(Vec2(0, 0));
		menu->setPosition(Vec2(0, 0));
		this->addChild(menu);

	}

	boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (room == NULL)
		return false;
	boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
	if (rv != NULL)
	{
		string StrParam = *rv->GetStringValue();
		vector<string> ArrParams = mUtils::splitString(StrParam, '@');

		LblSoTien->setString((ArrParams.at(0)).c_str());

		if (ArrParams.size() > 3)
		{
			string xml = "<?xml version=\"1.0\"?>" + ArrParams.back();

			XMLResults xe;
			XMLNodeEx xMainNode = XMLNodeEx::parseString(xml.c_str(), NULL, &xe);

			log("The following XML: %s\nWith the following info: '%s'\n\n", xml.c_str(), XMLNodeEx::getError(xe.error));
			if (xe.error == XMLError::eXMLErrorNone)
			{
				log("Get text of node %s",
					xMainNode.getChildNodeByPath("tableconfig.playing_rule.choiNuoiGa", true, '.').getText());

				const char* StrVar = "";
				StrVar = xMainNode.getChildNodeByPath("tableconfig.timeout.stepTimeout", true, '.').getText();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
				txtTimeoutStep->setString(string(StrVar));
#else
				txtTimeoutStep->setText(StrVar);
#endif
				StrVar = xMainNode.getChildNodeByPath("tableconfig.playing_rule.choiNuoiGa", true, '.').getText();
				cbxNuoiGa->setSelected(strcmp(StrVar, "1") == 0);
				//if(cbxNuoiGa->isSelected())
				//    cbxNuoiGa->setTouchEnabled(false);

				StrVar = xMainNode.getChildNodeByPath("tableconfig.playing_rule.choiUXuong", true, '.').getText();
				cbxChoiUXuong->setSelected(strcmp(StrVar, "0") == 0);

				StrVar = xMainNode.getChildNodeByPath("tableconfig.playing_rule.choiDiemToiThieu", true, '.').getText();
				cbxChoiDuoi4Diem->setSelected(strcmp(StrVar, "4") == 0);
			}
		}
	}
	//HoangDD
	int KieuBan = SceneManager::getSingleton().GetKieuBanChan();
	log("Kieu Ban: %d", KieuBan);
	for (int i = 0; i < ArrOption.size(); ++i)
	{
		try{
			if (ArrOption.at(i)->getTag() - 6 == KieuBan)
			{
				ArrOption.at(i)->setSelected(true);
			}
			else
			{
				ArrOption.at(i)->setSelected(false);
			}
		}
		catch (std::out_of_range& e){

		}
		catch (std::exception& e){

		}
		catch (...){

		}
	}
	return true;
}

void LayerConfigRoomChan::onEnter()
{
	Layer::onEnter();
	this->runAction(mUtils::getActionOpenPopup());
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerConfigRoomChan::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

bool LayerConfigRoomChan::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	log("Touches");
	return true;
}

void LayerConfigRoomChan::menuItemCallback(Ref* pSender)
{
	MenuItemSprite* menu = (MenuItemSprite*)pSender;
	if (menu)
	{
		if (menu->getTag() == 10)
		{
			log("Button Setting Click");
			SendRequestConfig();
			this->removeFromParentAndCleanup(true);
		}
		else if (menu->getTag() == 11)
		{
			log("Button Cancel Click");
			this->removeFromParentAndCleanup(true);
		}
	}
}

MenuItemImage* LayerConfigRoomChan::CreateButton(const char* mName, const int& mTag, const bool& isBoss)
{
		MenuItemImage* Btn;
		if (mTag == kTAG_BTN_SETTING && !isBoss)
		{
			Btn = MenuItemImage::create(
				"btn_chan_setting_disable.png",
				"btn_chan_setting_disable.png",
				CC_CALLBACK_1(LayerConfigRoomChan::menuItemCallback, this));
		}
		else
		{
			Btn = MenuItemImage::create(
				"btn_chan_setting.png",
				"btn_chan_setting_press.png",
				CC_CALLBACK_1(LayerConfigRoomChan::menuItemCallback, this));
		}

		Btn->setAnchorPoint(Vec2(0, 0));
		Btn->setTag(mTag);

		Label* LblName = Label::createWithTTF(mName,kBoldFont,30);
		LblName->setPosition(Vec2(Btn->getContentSize().width / 2, Btn->getContentSize().height / 2 + 5));
		LblName->setScale(1.4);
		LblName->setColor(Color3B::WHITE);
		Btn->addChild(LblName);

		return Btn;
}

std::string LayerConfigRoomChan::CreateStringConfig(const ConfigRoomChan& config)
{
	string strVar = "";
	string StrConfig = "";
	StrConfig.append("<tableconfig>");
	StrConfig.append("<playing_rule>");

	// Kiem tra an bao khi u
	StrConfig.append("<checkAnBaoWhenU>");
	strVar = (config.checkAnBaoWhenU ? "1" : "0");
	StrConfig.append(strVar);
	StrConfig.append("</checkAnBaoWhenU>");

	// Check U Lao hay ko
	StrConfig.append("<checkULao>");
	strVar = (config.checkULao ? "1" : "0");
	StrConfig.append(strVar);
	StrConfig.append("</checkULao>");

	// Choi nuoi ga 1: Choi
	StrConfig.append("<choiNuoiGa>");
	strVar = (config.choiNuoiGa ? "1" : "0");
	StrConfig.append(strVar);
	StrConfig.append("</choiNuoiGa>");

	// Choi U Xuong 1: choi 0: ko choi
	StrConfig.append("<choiUXuong>");
	strVar = (config.choiUXuong ? "1" : "0");
	StrConfig.append(strVar);
	StrConfig.append("</choiUXuong>");

	// Choi diem toi thieu
	StrConfig.append("<choiDiemToiThieu>");
	strVar = boost::to_string(config.choiDiemToiThieu);
	StrConfig.append(strVar);
	StrConfig.append("</choiDiemToiThieu>");

	// Khong choi cuoc sac
	StrConfig.append("<koChoiCacCuocSac>");
	StrConfig.append(config.koChoiCacCuocSac);
	StrConfig.append("</koChoiCacCuocSac>");

	StrConfig.append("</playing_rule>");
	StrConfig.append("<timeout>");

	// Thoi gian cho ready
	StrConfig.append("<playerReadyTimeout>");
	strVar = boost::to_string(config.playerReadyTimeout);
	StrConfig.append(strVar);
	StrConfig.append("</playerReadyTimeout>");

	// Thoi gian danh bai
	StrConfig.append("<discardTimeout>");
	strVar = boost::to_string(config.discardTimeout);
	StrConfig.append(strVar);
	StrConfig.append("</discardTimeout>");

	// Tong thoi gian 1 luot
	StrConfig.append("<turnTimeout>");
	strVar = boost::to_string(config.turnTimeout);
	StrConfig.append(strVar);
	StrConfig.append("</turnTimeout>");

	// Thoi gian Cho bao U
	StrConfig.append("<expectingUTimeout>");
	strVar = boost::to_string(config.expectingUTimeout);
	StrConfig.append(strVar);
	StrConfig.append("</expectingUTimeout>");

	// Thoi gian Xuong U
	StrConfig.append("<xuongCuocSacTimeout>");
	strVar = boost::to_string(config.xuongCuocSacTimeout);
	StrConfig.append(strVar);
	StrConfig.append("</xuongCuocSacTimeout>");

	// Timeout danh bai
	StrConfig.append("<stepTimeout>");
	strVar = boost::to_string(config.stepTimeout);
	StrConfig.append(strVar);
	StrConfig.append("</stepTimeout>");

	// Thoi gian cho chiu
	StrConfig.append("<waitForChiuTimeout>");
	strVar = boost::to_string(config.waitForChiuTimeout);
	StrConfig.append(strVar);
	StrConfig.append("</waitForChiuTimeout>");

	StrConfig.append("<waitUDeTimeout>");
	int ude = config.stepTimeout;
	strVar = boost::to_string(ude);
	strVar = "10";
	StrConfig.append(strVar);
	StrConfig.append("</waitUDeTimeout>");

	StrConfig.append("</timeout>");
	StrConfig.append("</tableconfig>");

	return StrConfig;
}

void LayerConfigRoomChan::SendRequestConfig()
{
	int time = 0;
	const char* text = "0";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	text = txtTimeoutStep->getString().c_str();
#else
	text = txtTimeoutStep->getText();
#endif

	if (0 == strcmp(text, ""))
		time = 8;
	time = atoi(text);
	if (time < 8)
		time = 8;

	ConfigRoomChan config;
	config.choiNuoiGa = cbxNuoiGa->isSelected();
	config.choiUXuong = !cbxChoiUXuong->isSelected();
	config.stepTimeout = time;

	if (cbxChoiDuoi4Diem->isSelected())
		config.choiDiemToiThieu = 4;
	else
		config.choiDiemToiThieu = 0;

	string xml = CreateStringConfig(config);
	log("Config = %s", xml.c_str());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString(EXT_FIELD_GAME_CFG, xml);
	boost::shared_ptr<IRequest> config_req(new ExtensionRequest(EXT_EVENT_REQ_UPDATE_GAME_CONFIG, params, lastRoom));
	GameServer::getSingleton().Send(config_req);
	log("Send Request Update Room");
}

void LayerConfigRoomChan::setParentBG(ImageView *bg)
{
	this->_bgParent = bg;
}

void LayerConfigRoomChan::OnCheckBoxClick(Ref* pSend, CheckBox::EventType type)
{
	CheckBox* cb = (CheckBox*)pSend;
	if (cb->getTag() == 871){
		UserDefault *def = UserDefault::getInstance();
		def->setBoolForKey("sound", type == cocos2d::ui::CheckBox::EventType::SELECTED);
		def->flush();
	}
}

void LayerConfigRoomChan::OnOptionClick(Ref* pSend)
{
	CheckBox* cbx = (CheckBox*)pSend;
	for (vector<CheckBox*>::iterator it = ArrOption.begin(); it != ArrOption.end(); ++it)
	{
		CheckBox* childCbx = *it;
		if (childCbx->getTag() == cbx->getTag())
		{
			childCbx->setSelected(true);
			int t = childCbx->getTag() - 6;
			string ss = "chan_bg_1.jpg";
			if (t == 2)
				ss = "chan_bg_2.jpg";
			else if (t == 3)
				ss = "chan_bg_3.jpg";

			if (this->_bgParent != NULL)
				this->_bgParent->loadTexture(ss);

			SceneManager::getSingleton().SetKieuBanChan(t);
		}
		else
		{
			childCbx->setSelected(false);
		}
	}
}

void LayerConfigRoomChan::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

